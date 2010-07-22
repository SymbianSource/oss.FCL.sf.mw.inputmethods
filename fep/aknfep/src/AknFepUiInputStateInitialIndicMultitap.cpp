/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:            Provides the TAknFepInputStateInitialIndicMultitap methods.
*
*/











#include "AknFepUIInputStateInitialIndicMultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepUiManagerWestern.h"
#include "AknFepUiIndicEnums.h"
#include "AknFepCaseManager.h"
#include "AknFepUiIndicInputManager.h"

#include <uikon.hrh>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <aknSctDialog.h>
#include <avkon.rsg>
#include <aknfep.rsg>
#include <eikenv.h>

#define PTI_CLEAR_CURRENTWORD( A, B ) \
		( A )->ClearCurrentWord(); \
		( B ) = ETrue; \

	
// -----------------------------------------------------------------------------
// TAknFepInputStateInitialIndicMultitap::TAknFepInputStateInitialIndicMultitap

// C++ default constructor can NOT contain any code, that
// might leave or if it is absolutely necessary then MUST be trapped.
// -----------------------------------------------------------------------------

TAknFepInputStateInitialIndicMultitap::
TAknFepInputStateInitialIndicMultitap( MAknFepUIManagerStateInterface* aOwner,
																			TLanguage aLanguage )
																			:TAknFepInputStateInitialMultitapBase( aOwner )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    iIsStarKeyPressed = EFalse;
    iIsKeyTimerExpired = ETrue;
    iIndicLanguage = aLanguage;
    iIsHalantAllowed = EFalse;
    iZWSPresent = EFalse;
    iRephaPresent = EFalse;
    iRakarPresent = EFalse;
    iResponseInsertZWS = EFalse;
    iLigaturePresent = EFalse;
#ifdef RD_MARATHI
	iPreviousCommittedChar 		= 0;
	iEyeLashRaPresent 	        = EFalse;
	iChandraAPresent 	        = EFalse;
#endif // RD_MARATHI
    TRAP_IGNORE( ptiengine->ActivateLanguageL( aLanguage ) )
    ptiengine->SetCase( EPtiCaseLower );
    }

// -----------------------------------------------------------------------------
// TAknFepInputStateInitialIndicMultitap::HandleKeyL

// Handles the logic of Indic multitap input. This function first checks the validity
// of the inputed text and then enters it.
// -----------------------------------------------------------------------------

TBool TAknFepInputStateInitialIndicMultitap::HandleKeyL( TInt aKey, TKeyPressLength aLength )
	{
	TBool                      result       	= ETrue;
	MAknFepManagerUIInterface* fepMan       	= iOwner->FepMan();
	CPtiEngine* 			   ptiengine 		= iOwner->PtiEngine();

#ifdef RD_MARATHI
	if( iIsKeyTimerExpired )
		{
		iPreviousCommittedChar = fepMan->PreviousChar( ETrue );
		
		if( iIndicLanguage == ELangMarathi )
			{
			HandleKeyMarathiL();
			}
		}
#endif // RD_MARATHI

	if(aKey == EPtiKey1)
		{
		iKey1Pressed = ETrue;	
		}
	else 
		{
		iKey1Pressed = EFalse;	
		}	
	if ( iData && ( iData != aKey ) )
    	{
#ifdef RD_MARATHI
        iPreviousCommittedChar = fepMan->PreviousChar( ETrue );

        if( iPreviousCommittedChar == ZERO_WIDTH_SPACE &&
            aKey == EPtiKey2 )
            {
            // This is a scenario where the timer is forcefully
            // expired by quickly pressing a different key.
            // Should remove the ZWS from the editor.
            fepMan->RemovePreviousCharacterL();
            iZWSPresent = EFalse;
            
            // Reset the previous committed character.
            iPreviousCommittedChar = fepMan->PreviousChar( ETrue );
            }

		if( iIndicLanguage == ELangMarathi )
		    {
		    HandleKeyMarathiL();
		    }
#endif // RD_MARATHI

	    fepMan->CommitInlineEditL();
	    PTI_CLEAR_CURRENTWORD( ptiengine, iIsKeyTimerExpired )

	    if( iRephaPresent )
	    	{
	    	fepMan->AlignLogicalAndVisualCursorL( TTmDocPosSpec::ELeading, EFalse );
	    	iRephaPresent = EFalse;
	    	}
	    if( iRakarPresent )
	    	{
	    	iRakarPresent = EFalse;
	    	}

#ifdef RD_MARATHI
        // This check should be done only after the text is committed.
	    if( iEyeLashRaPresent )
            {
            // Calls the overloaded method which used find doc pos.
            // Reason: Unlike Repha, EyeLash Ra can be inserted in
            // an empty editor. GetNextVisualCursorPosition would
            // result in incorrect cursor alignment in this case.
            fepMan->AlignLogicalAndVisualCursorL();
            iEyeLashRaPresent = EFalse;
            }
#endif

   		}

	if ( aLength == EShortKeyPress )
		{
	    if ( aKey == EPtiKeyStar ) // Overriding Key
            {
            iIsStarKeyPressed = ETrue;
            fepMan->RemoveZWSCharacterL( iKey1Pressed, EFalse, EFalse, iLigaturePresent );	
    		}
		else
			{
			TPtiTextCase newCase = ptiengine->Case();
			
			TIndicInputResponse keyResponse = 
				TAknFepUiIndicInputManager::ValidateInput( aKey,
														fepMan->PreviousChar( ETrue ),
														NULL,NULL,
														iIsHalantAllowed,
														iIsStarKeyPressed,
														iIsKeyTimerExpired,
														newCase,
														aLength,
														iIndicLanguage
#ifdef RD_MARATHI
														, EFalse, EFalse, iChandraAPresent

#endif // RD_MARATHI
														 );
				
			ptiengine->SetCase( newCase );
			TPtrC ptiText = ptiengine->AppendKeyPress( ( TPtiKey )aKey );
			
			if ( ptiText.Length() )
				{
				do
					{
					TText prChar;
					// For Repha, use the Previous to Previous character
					// only if multitap timer hasn't expired. This will
					// handle cases like Matra attached to the consonant.
					// Here, repha is not allowed and the character to 
					// to checked is Matra. In cases like a consonant with
					// Virama, all the characters should be skipped including
					// repha. In this this case the timer is set to expired.
					// So the previous character is Virama. And as per the 
					// language rules, repha is not allowed after Virama.
					if( TAknFepUiIndicInputManager::IsCharRepha( 
						ptiText[0], iIndicLanguage ) && !iIsKeyTimerExpired )
						{
						prChar = fepMan->PreviousToPreviousChar( ETrue );
#ifdef RD_MARATHI
                        prChar = iPreviousCommittedChar;
#endif // RD_MARATHI
						}
					else
						{
						prChar = fepMan->PreviousChar( ETrue );
						}

					// When a base consonant is already entered and the
					// user starts multitapping, the second modifier in the
					// key map will not be allowed if the previous character 
					// is considered. The base consonant has to be used
					// instead. This change had to be added after the new
					// function, IsModifierAllowed was added.
					if( TAknFepUiIndicInputManager::IsCharModifier(
						ptiText[0], iIndicLanguage ) && !iIsKeyTimerExpired )
						{
						prChar = fepMan->PreviousToPreviousChar( ETrue );
						}

					if( TAknFepUiIndicInputManager::IsCharLigature( 
						ptiText[0], iIndicLanguage ) && !iIsKeyTimerExpired )
						{
						if( fepMan->IsZWSCharacterPresent( ETrue ) )
							{
							iZWSPresent = ETrue;
							}
						iLigaturePresent = ETrue;
						prChar = fepMan->PreviousToPreviousChar( ETrue );
						}
					else
						{
						iLigaturePresent = EFalse;
						}

#ifdef RD_MARATHI

					// While multitapping after inserting a base consonant,
					// modifiers will get inserted first, followed by the
					// Nukta character. Hence, use Prev to prev char which 
					// will be a base consonant.
					if( TAknFepUiIndicInputManager::IsCharNukta(
						ptiText[0], iIndicLanguage ) && !iIsKeyTimerExpired )
						{
						prChar = fepMan->PreviousToPreviousChar( ETrue );
						}
#endif // RD_MARATHI

					keyResponse = 
						TAknFepUiIndicInputManager::ValidateInput( aKey,
        														prChar,
																ptiText[0],
        														NULL,
        														iIsHalantAllowed,
        														iIsStarKeyPressed,
																iIsKeyTimerExpired,
																newCase,
																aLength,
        														iIndicLanguage,
        														iRakarPresent,
        														iZWSPresent
#ifdef RD_MARATHI
										  						, iChandraAPresent 
#endif // RD_MARATHI
        														 );	

					if( !IsFreeSpaceAvailable( keyResponse ) )
						{
						keyResponse = EIndicInputResponseInvalid;
						}

					if( keyResponse == EIndicInputResponseInvalid )
						{
						ptiText.Set( ptiengine->AppendKeyPress( ( TPtiKey )aKey ) );
						}
						
					if( !ptiText.Length() )
						{
						break;
						}
						
					}while( keyResponse == EIndicInputResponseInvalid );

				if( iIsStarKeyPressed && 
					aKey == EPtiKey0 && 
					TAknFepUiIndicInputManager::IsCharOther( 
						ptiText[0], iIndicLanguage ) )
					{
					// Forcefully keep the star key flag to true. Else when
					// the ligatures are entered, this flag will be false 
					// thus preventing the ligature formation.
					iIsStarKeyPressed = ETrue;
					}
				else
					{
					iIsStarKeyPressed = EFalse;
					}

				result = HandleIndicKeyResponseL( aKey, keyResponse, ptiText );
				}
			}
		}
   	else
    	{
        /* Long press of a Key */
        if ( aKey == EPtiKeyStar )
	        {
            /* Launch the SCT For Indic */
   			if( fepMan->EditorHasFreeSpace() )
				{
				if (fepMan->IsAbleToLaunchSCT() && !fepMan->EditSubmenuInUse())
        			{
	            	fepMan->LaunchSpecialCharacterTableL();	
	            	}
				}
		    }
        else
	        {
	        TUint prevchar = fepMan->PreviousChar(ETrue);
            if(!((aKey == EPtiKey1) && ((0x0031 == prevchar) || (0x0967 == prevchar)) ))
		        {
		        TChar ch( aKey );
		        TBuf<1> buf;
		        buf.Append( ch );
		        fepMan->NewCharacterSequenceL( buf, EIndicInputResponseNumber );
		        fepMan->CommitInlineEditL();			
		        PTI_CLEAR_CURRENTWORD( ptiengine, iIsKeyTimerExpired )				
				}
	        }
    	}
    iData = aKey;
    return( result );
	}

// -----------------------------------------------------------------------------
// TAknFepInputStateInitialIndicMultitap::KeyTimerExpired

// Handles the logic of post keytimerexpired event. This function commits 
// the inline editing text to the editor.
// -----------------------------------------------------------------------------

void TAknFepInputStateInitialIndicMultitap::KeyTimerExpired()
	{
	iIsKeyTimerExpired = ETrue;
	
	if( iRakarPresent )
		{
		iRakarPresent = EFalse;
		}
		
	if( iRephaPresent 
#ifdef RD_MARATHI
	|| ( iEyeLashRaPresent && 
			TAknFepUiIndicInputManager::
				IsCharBaseConsonant( iOwner->FepMan()->NextChar(), 
									iIndicLanguage ) )
#endif // RD_MARATHI
	    )
		{
		TRAP_IGNORE( iOwner->FepMan()->AlignLogicalAndVisualCursorL( TTmDocPosSpec::ELeading, EFalse ) )
		iRephaPresent = EFalse;
		}

#ifdef RD_MARATHI
		iEyeLashRaPresent = EFalse;
#endif // RD_MARATHI

	if( iResponseInsertZWS || iZWSPresent )
		{
		iZWSPresent = EFalse;
		iResponseInsertZWS = EFalse;
		TRAP_IGNORE( iOwner->FepMan()->RemoveZWSCharacterL( iKey1Pressed, EFalse, 
                                                        EFalse, iLigaturePresent ) )
		}
	iLigaturePresent = EFalse;

#ifdef RD_MARATHI
	iChandraAPresent = EFalse;
#endif // RD_MARATHI

	TAknFepInputStateInitialMultitapBase::KeyTimerExpired();
	}

TBool TAknFepInputStateInitialIndicMultitap::
    HandleIndicKeyResponseL( TInt aKey, TIndicInputResponse aKeyResponse, TPtrC aPtiText )
    {
    TBool result = ETrue;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    switch( aKeyResponse )
        {
        case EIndicInputResponseNone:
        	{
        	if ( aPtiText.Length() && ( !iIsKeyTimerExpired || fepMan->EditorHasFreeSpace() ) )
        		{
            	if ( iIsKeyTimerExpired )
            		{
            		if ( iZWSPresent )
            			{
            			fepMan->RemoveZWSCharacterL( iKey1Pressed,EFalse, 
                        TAknFepUiIndicInputManager::IsCharModifier(aPtiText[0], iIndicLanguage));
            			iZWSPresent = EFalse;
            			}
            		
            		fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseNone );
            		
            		if ( fepMan->IsZWSCharacterPresent() )
            			{
            			iZWSPresent = ETrue;
            			}
            		}
            	else
            		{
            		if ( aKey == EPtiKey1 )
            			{
            			if( iRephaPresent )
            				{
            				fepMan->RemoveRephaCharacterL();
            				iRephaPresent = EFalse;
            				}
            			if( iZWSPresent )
            				{						    				
            				fepMan->RemoveZWSCharacterL( iKey1Pressed, ETrue );
            				iZWSPresent = EFalse;
            				}
        				if( iRakarPresent )
            				{
            				// This is to handle the case when only
            				// two spaces are left in the editor.
            				fepMan->RemoveRakarCharacterL();
            				iRakarPresent = EFalse;
            				}
#ifdef RD_MARATHI
            			iEyeLashRaPresent = EFalse;
#endif // RD_MARATHI
            			}
            		else if( aKey == EPtiKey2 )
            			{
#ifdef RD_MARATHI
                        iChandraAPresent = EFalse;
#endif // RD_MARATHI
            			}
            		fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseNone );
        			}
        		}
        	iIsKeyTimerExpired = EFalse;	
        	}
        	break;

        case EIndicInputResponseZWSandCharacter:
        	{
        	if( fepMan->EditorHasFreeSpace( 2 ) )
        		{
        		fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseZWSandCharacter );
              	iZWSPresent = ETrue;
              	iIsKeyTimerExpired = EFalse;
        		}
        	}
        	break;
        case EIndicInputResponseInsertZWS: 
        	{
        	//Already in multitapping so we need to check wheather 
        	//there is additional single space for ZWS
        	if( !iIsKeyTimerExpired || fepMan->EditorHasFreeSpace( 1 ) ) 
        		{
                fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseInsertZWS );
        		
        		// Using a seperate flag here. Cannot use the flag
        		// iZWSPresent. Consider the scenario:- User enters 
        		// a consonant, then multitaps key-1 to insert halant.
        		// Now consider that we set the flag to ETrue.
        		// But before the key timer expires, key-5 is 
        		// multitapped to insert another consonant.
        		// In this case the code flow will go to the case
        		// EIndicInputResponseNone. Here the condition, 
        		// if ( iZWSPresent ) succeeds and ZWS is removed.
        		// This causes direct ligature formation. It should
        		// actually show the ligature formation after the timeout.
        		iResponseInsertZWS = ETrue;
        		}
        	else // Remove already entered Modifier "Visarga" if there is no space
        		{
        		fepMan->RemovePreviousCharacterL();	
        		}
        	iIsKeyTimerExpired = EFalse;		
        	}
        	break;
        case EIndicInputResponseInsertZWSandLigature:
        	{
        	if( fepMan->EditorHasFreeSpace( 3 )  )
        		{
        		if ( iIsKeyTimerExpired )
            		{
        			fepMan->RemoveZWSCharacterL( iKey1Pressed );
        			}
        		fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseInsertZWSandLigature );
               	}
        	iIsKeyTimerExpired = EFalse;
        	iZWSPresent = ETrue;	
        	}
        	break;
        case EIndicInputResponseIgnore:	
        	break;

        case EIndicInputResponseInsertRepha:
        	{
        	if( !iIsKeyTimerExpired )
        		{
        		if( iRakarPresent )
        			{
        			fepMan->RemoveRakarCharacterL();
        			iRakarPresent = EFalse;
        			}
#ifdef RD_MARATHI
        		else if( iEyeLashRaPresent )
        			{
        			// Remove eye lash ra
        			TBuf<3>buf;

        			// Get the EyeLashRa string.
        			TAknFepUiIndicInputManager::
        					GetEyeLashRa( buf, iIndicLanguage );
        									
        			fepMan->RemoveTextFromEditorL( buf.Length(), 0, ETrue );
        			iEyeLashRaPresent = EFalse;
        			}
#endif // RD_MARATHI
        		else if( TAknFepUiIndicInputManager::IsCharMatra( 
                             fepMan->PreviousToPreviousChar( ETrue ), iIndicLanguage ) )
        			{
        			// Consider that a matra is applied to any base 
        			// consonant and two nore spaces are left in the editor.
        			// On multitapping, user will get the first four modifiers.
        			// After that, rakar should be inserted which requires two
        			// spaces. But since the modifier is present, only one
        			// space is available. Here we need to free up the space 
        			// by removing the previous character which is a modifier.
        			// For inserting repha, the existing inline text needs to
        			// be first committed and then removed.

        			fepMan->RemovePreviousCharacterL();
        			}

        		fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseInsertRepha );
        		iRephaPresent = ETrue;
        		}
        	}
        	break;

        case EIndicInputResponseInsertRakar:
        	{
        	if( !iIsKeyTimerExpired )
        		{
        		// RemoveZWS is not invoked here as we are in inline 
        		// editing state and we can as well overwrite the previous
        		// characters. Hence directly invoke NewCharacterSequenceL.
        		// Secondly, the flag iZWSPresent is not reset here since
        		// it will happen anyways after timeout.
        		
        		fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseInsertRakar );
        		iRakarPresent = ETrue;
        		
#ifdef RD_MARATHI
        		// This check assumes that Rakar comes after EyelashRa in 
        		// Key map order.
        		iEyeLashRaPresent = EFalse;
#endif
        		}
        	}
        	break;
        	
        case EIndicInputResponseInsertDirectLigature:
        	{
        	if( !iIsKeyTimerExpired )
        		{
        		TBuf<3> ligature;
        		ligature.Zero();
        		TAknFepUiIndicInputManager::
        			GetLigature( ligature, aPtiText[0], 
        						iIndicLanguage );

        		fepMan->NewCharacterSequenceL( ligature, 
        			EIndicInputResponseInsertDirectLigature );
        		}
        	}
        	break;

        case EIndicInputResponseInsertViramaZWSandDirectLigature:
        	{
        	if( fepMan->EditorHasFreeSpace( 5 )  )
        		{
        		if ( iIsKeyTimerExpired )
            		{
        			fepMan->RemoveZWSCharacterL( iKey1Pressed );
        			}

        		TBuf<3> ligature;
        		ligature.Zero();
        		TAknFepUiIndicInputManager::
        			GetLigature( ligature, aPtiText[0], 
        						iIndicLanguage );
        		fepMan->NewCharacterSequenceL( ligature, 
        			EIndicInputResponseInsertViramaZWSandDirectLigature );
               	}
        	iIsKeyTimerExpired = EFalse;
        	iZWSPresent = ETrue;	
        	}
        	break;

        case EIndicInputResponseZWSandDirectLigature:
        	{
        	if( fepMan->EditorHasFreeSpace( 4 ) )
        		{
        		TBuf<3> ligature;
        		ligature.Zero();
        		TAknFepUiIndicInputManager::
        			GetLigature( ligature, aPtiText[0], 
        						iIndicLanguage );

        		fepMan->NewCharacterSequenceL( ligature, 
        			EIndicInputResponseZWSandDirectLigature );
              	iZWSPresent = ETrue;
              	iIsKeyTimerExpired = EFalse;
        		}
        	}
        	break;

#ifdef RD_MARATHI
        case EIndicInputResponseInsertEyeLashRa:
        	{
        	fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseInsertEyeLashRa );
        	iIsKeyTimerExpired = EFalse;
        	iEyeLashRaPresent = ETrue;
        	}
        	break;

        case EIndicInputResponseInsertChandraA:
        	{
        	fepMan->NewCharacterSequenceL( aPtiText, EIndicInputResponseInsertChandraA );
        	iChandraAPresent = ETrue;
        	}
        	break;
#endif // RD_MARATHI

        case EindicInputResponseLast:
        	break;
        default: 
            {
            }
        	break;
        }
    return result;
    }

TBool TAknFepInputStateInitialIndicMultitap::IsFreeSpaceAvailable( 
                                TIndicInputResponse aKeyResponse )
    {
    TBool result = ETrue;

    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    TBool isUnlimitedSpaceEditor;

	TInt freeSpaceLeft = 
        fepMan->EditorFreeSpace( isUnlimitedSpaceEditor, ETrue );

	// Check if space is available.
	if( ( !isUnlimitedSpaceEditor ) && 
		( ( ( aKeyResponse == EIndicInputResponseInsertRepha ||
			aKeyResponse == EIndicInputResponseInsertRakar ) && ( freeSpaceLeft < 2 ) ) ||
		( ( aKeyResponse == EIndicInputResponseInsertDirectLigature ) && ( freeSpaceLeft < 3 ) ) 
#ifdef RD_MARATHI
		|| ( ( aKeyResponse == EIndicInputResponseInsertEyeLashRa ) && ( freeSpaceLeft < 3 ) ) 
		|| ( ( aKeyResponse == EIndicInputResponseInsertChandraA ) && ( freeSpaceLeft < 2 ) ) 
#endif // RD_MARATHI
		) )
	    {
        result = EFalse;
	    }
	    return result;
    }

#ifdef RD_MARATHI

void TAknFepInputStateInitialIndicMultitap::HandleKeyMarathiL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    if( fepMan )
        {
        if( fepMan->IsChandraAPresentL() )
            {
            TBuf<1> buf;
            TAknFepUiIndicInputManager::GetChandraA( buf, ETrue );
        	iPreviousCommittedChar = buf[0];
            }
        }
    }
#endif // RD_MARATHI
//End of File
