/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
*       Provides the TAknFepUiInputStateEntryHalfQwertyWesternPredictive methods.
*
*/












#include "AknFepUiInputStateEntryHalfQwertyWesternPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiManagerWestern.h"
#include "AknFepManager.h"
#include "AknFepPanic.h"
#include "AknFepCaseManager.h"

#include <PtiEngine.h>              //CPtiEngine
#include <PtiDefs.h>                //keys
#include <PtiCompositionDataIF.h>   //MPtiEngineCompositionDataInterface
#include <featmgr.h>                //FeatureManager
#include <e32keys.h>
#include <aknfep.rsg>
#include <PtiUserDicEntry.h>
// TODO: Remove this include
#include <AvkonInternalCRKeys.h>
#include <AknUtils.h>

static const TInt KKeyMappingsLength = 63;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
TAknFepUiInputStateEntryHalfQwertyWesternPredictive::TAknFepUiInputStateEntryHalfQwertyWesternPredictive(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateBase(aOwner)
    {
    //this class insists on the existence of a case manager
    __ASSERT_DEBUG(iOwner->CaseMan(), AknFepPanic(EAknFepPanicNoCaseManager));

    iState = EEntry;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    
    ptiengine->SetInputMode( EPtiEngineHalfQwertyPredictive );
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    TPtiTextCase caseMode = ptiengine->Case();

    ptiengine->SetCase(caseMode);
    TBool isUnlimitedEditor = EFalse;
    ptiengine->SetMaxLengthForAutoCompletedCandidates(fepMan->EditorFreeSpaceForAutoWordCompletion(isUnlimitedEditor ));
    
	if(fepMan->IsAutoCompleteOn())
	    {
	    TRAP_IGNORE( ptiengine->HandleCommandL( EPtiCommandEnableAutoCompletion ) );
	    }
	else
	    {
	    TRAP_IGNORE( ptiengine->HandleCommandL( EPtiCommandDisableAutoCompletion ) );
	    }
	
//	TPtiErrorCorrectionLevel correctionLevel = (TPtiErrorCorrectionLevel)fepMan->AdvancedPredictiveTypingCorrectionLevel();
//	TRAP_IGNORE( ptiengine->HandleCommandL( EPtiCommandSetCorrectionLevel, &correctionLevel ) );	
	
	TPtiKeyboardType keyboard = fepMan->KeyboardLayout();
	ptiengine->SetKeyboardType(keyboard);
	

//	TBool isNumberCandidateShown = fepMan->IsAdvancedPredictiveNumberCandidateShown();
//	TRAP_IGNORE( ptiengine->HandleCommandL( EPtiCommandSetNumberCandidateStatus, &isNumberCandidateShown ) );
//	
//	TBool primaryCandidateState = fepMan->AdvancedPredictivePrimaryCandidate();
//	TRAP_IGNORE( ptiengine->HandleCommandL( EPtiCommandSetProactiveCorrection, &primaryCandidateState ) );
    }


///////////////////////////////////////////////////////////////////////////////////////////////////
//
TBool TAknFepUiInputStateEntryHalfQwertyWesternPredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
	
	CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
	
	if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
		{
		ptiengine->SetCase(EPtiCaseFnLower);
		}

    // Handle the Chr cycling
    if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) && aKey !=EKeyBackspace)
        {
        // if the key has no accented characters mapped to it, then just block the event
		TBuf<KKeyMappingsLength> keyMappings; 
    	ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseChrLower );   
		if( keyMappings.Length() == 0 )       	
        	{
        	return EFalse;
        	}
       // if there is a no matches indicator remove it 
        if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) && fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
             {
             fepMan->TryRemoveNoMatchesIndicatorL();
             fepMan->UpdateCbaL(NULL);
             }	
        // if there is an autocompletion part rip it off
        fepMan->RemoveSuggestedAdvanceCompletionL();			        
        
        // now if the editor has no free space just return
        if ( !fepMan->EditorHasFreeSpace() )
            {
            return EFalse;
            }
        // if we are in inline editing, break the word
		if( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
			{                        
			TPtrC selectedWord = ptiengine->CurrentWord();
        	if( ptiengine->CurrentWord() != KNullDesC)
        		{
            	fepMan->UpdateInlineEditL( selectedWord, selectedWord.Length() );  //selection.iAnchorPosition
        		ptiengine->CommitCurrentWord();	
            	}
        	fepMan->TryCloseUiL(); 
			}
            
        if ( iData && (iData != aKey) )
            {
            // Chr key is still pressed down but character key was changed from previous one
            // to new one. The previous character is committed and PtiEngine starts 
            // processing new key.
            ptiengine->CancelTimerActivity();            
            }
        fepMan->SetFlag( CAknFepManager::EFlagInsideMultitapInlineEditingTransaction );
        }

    TBool response = EFalse;


    // Handle dead keys with space or backspace. These keys are not mapped in PtiEngine, so they cannot
    // be handled there.
/*    if ( ptiengine->HandleCommandL(EPtiCommandDeadKeyWaiting) == 1 )
        {
        if ( aKey == EStdKeyBackspace )
            {
            // Pending dead key is removed with backspace. No other characters are removed.
            ptiengine->HandleCommandL(EPtiCommandGetAndClearDeadKeyRootChar);
            response = ETrue;
            }
        else if ( aKey == EStdKeySpace )
            {
            // Dead key followed by space enters the dead key root character
            TText deadRoot = ptiengine->HandleCommandL(EPtiCommandGetAndClearDeadKeyRootChar);
            ptiengine->HandleCommandL( EPtiCommandAppendCharacter, &deadRoot );
            
            TPtrC newText = ptiengine->CurrentWord();
            fepMan->UpdateInlineEditL( newText, newText.Length() );
            iOwner->CaseMan()->UpdateCase(ENullNaviEvent);
            
            response = ETrue;
            }
        }		    
*/
    if (!response) // key not yet handled
        {
        switch (aKey)
            {
            case EStdKeySpace: 
            	if( fepMan->FnKeyState() 
            				== CAknFepFnKeyManager::EFnKeyNone )
            		{
            		response = HandleWordBreakingKeysL(aKey);            		
            		}
            	else
            		{
            		response = HandleQwertyKeysL( aKey, aLength);
            		}	
            	break;
            case EStdKeyFullStop:
            	if( aLength == ELongKeyPress )
            		{
            		// first commit the word if in inline editing
					if( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
						{ 
						if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
							{
							fepMan->TryRemoveNoMatchesIndicatorL();
							fepMan->UpdateCbaL(NULL);
							}
					
						TPtrC selectedWord = ptiengine->CurrentWord();
			        	if( ptiengine->CurrentWord() != KNullDesC)
			        		{
			            	fepMan->UpdateInlineEditL( selectedWord, selectedWord.Length() );  //selection.iAnchorPosition			            	
                            ptiengine->CommitCurrentWord();
			            	}
			        	fepMan->TryCloseUiL(); 
						}
            		response = EFalse;            		
            		}
            	else
            		{
            		HandleShiftState(aKey);
            		response = HandleQwertyKeysL( aKey, aLength);
            		}	
            	break;
            case EStdKeyEnter:
            case EStdKeyTab:
            case EStdKeyLeftArrow:
            case EStdKeyRightArrow:
            case EStdKeyUpArrow:
            case EStdKeyDownArrow:
                response = HandleWordBreakingKeysL(aKey);
                break;
                
            // Long press of shift key start copy event.     
            // Its should handle by framework.    
            case EStdKeyF21:
            case EKeyF21:
            // Soft key handle by framework
            // Selection key handle by AknFepManager.
            case EStdKeyDevice0: // iCode:EKeyCBA1
            case EStdKeyDevice1: //	iCode:EKeyCBA2
            case EStdKeyDevice3: // iCode:EKeyOK
            case EKeyCBA1:
            case EKeyCBA2:
            case EKeyOK:
            	response = EFalse;
            	break;

            case EKeyUpArrow: 
                if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
                    {
                    // Select the exact match with the up arrow and lock it.
                    // Another alternative could be to call function DoPreviousMatchL() from here
                    
                    TPtrC exactWord = ptiengine->FirstCandidate();
                    ptiengine->HandleCommandL( EPtiCommandLockCurrentCandidate, NULL );
                    
                    fepMan->UpdateInlineEditL(exactWord, exactWord.Length());
                    
                    response = ETrue;
                    break;
                    }
                    // fall through
            case EKeyDownArrow:
            case EKeyLeftArrow:
            case EKeyRightArrow:
            case EKeyBackspace:
            	{
                response = HandleNaviKeysL(aKey, aLength);
            	}
                break;
            
            case EStdKeyLeftShift:
            case EStdKeyRightShift:
            case EStdKeyLeftFunc:
                {
                if( EPtiCaseFnLower == fepMan->PtiEngine()->Case() )
                    {
                    if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
                        {
                        fepMan->TryRemoveNoMatchesIndicatorL();
                        }
                    
                    fepMan->UpdateCbaL(NULL);
    
                    /*if (fepMan->IsFlagSet(CAknFepManager::EFlagCompoundWord))
                        { 
                        if (fepMan->IsCursorAtEndOfWord())
                            {
                            fepMan->AddCompoundWordToUdbL();                
                            }
                        fepMan->ClearFlag(CAknFepManager::EFlagCompoundWord);
                        }
                        */
    
                    fepMan->RemoveSuggestedAdvanceCompletionL();
                    ptiengine->CommitCurrentWord();
                    fepMan->CommitInlineEditL();
                    fepMan->TryCloseUiL();
                    response = EFalse;
                    }
                }
                break;
                
            default:
                HandleShiftState(aKey);
                response = HandleQwertyKeysL(aKey, aLength);
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
                if ( aLength == ELongKeyPress )
                    {
                    // Expire multitap timer after long press. 
                    ptiengine->CancelTimerActivity();
                    }
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
                break;
            }
        }
        
    if ( aKey == EStdKeyEnter)
        {
        iData = 0;
        return 0;
        }
        
    if (!iOwner->PtiEngine()->IsValidKey((TPtiKey)aKey))
        {     
        iData = 0;
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);        
        fepMan->PtiEngine()->CancelTimerActivity();
                
        return response;
        }
        
        
    iData = aKey; // store the pressed key for future reference
    return response;
    }


//////////////////////////////////////////////////////////////////////////////////
//
void TAknFepUiInputStateEntryHalfQwertyWesternPredictive::HandleShiftState( TInt aKey )
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CAknFepCaseManager* caseMan = iOwner->CaseMan();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
    TPtiTextCase shiftedCase;
    if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
		{
		shiftedCase = EPtiCaseFnLower;
		}
	else
 		{
 		shiftedCase= EPtiCaseLower;	
 		}
    
    // Find out if the key is alphabetic
    TBuf<KKeyMappingsLength> keyMappings; 
    ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseUpper );   
    TBool isAlpha = ( keyMappings.Length() && TChar(keyMappings[0]).IsAlpha() );
    
    if (isAlpha && (fnKeyState  == CAknFepFnKeyManager::EFnKeyNone))
        {
        switch ( caseMan->CurrentCase() )
            {
            case EAknEditorTextCase:
                {
                // Alphabetic characters are always capitalized in "Abc" mode 
                // unless the shift is pressed
                if ( !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }          
                }
            break;
            
            case EAknEditorLowerCase:
                {
                // Pressing shift enters uppercae letters in the "abc" mode
                if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }                
                }
            break;
            
            case EAknEditorUpperCase:
                {
                // In the "ABC" mode (capslock mode) the letters are uppercased
                // unless the shift is pressed
                if ( !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }                
                }
            break;
                
            default:
            break;
            }

        
        }
    else
        {
        // non-alphabetic keys are shifted if and only if shift is pressed, no matter what is the input state
        if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) && (fnKeyState  == CAknFepFnKeyManager::EFnKeyNone))
            {
            shiftedCase = EPtiCaseUpper;
            }
        else if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) && (fnKeyState  != CAknFepFnKeyManager::EFnKeyNone))
	        {
			shiftedCase = EPtiCaseFnUpper;
	        }    
        }

    
    // Handle the Chr key state
    if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) )
        {
        // Chr key is pressed down. Chr mode is used.
        if (shiftedCase == EPtiCaseLower)
            {
            shiftedCase = EPtiCaseChrLower;
            }
        else if (shiftedCase == EPtiCaseUpper)
            {
            shiftedCase = EPtiCaseChrUpper;
            }
        }
    
    ptiengine->SetCase(shiftedCase);
    
    fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringShiftKeyPress);
    }

//////////////////////////////////////////////////////////////////////////////////
//

TBool TAknFepUiInputStateEntryHalfQwertyWesternPredictive::HandleWordBreakingKeysL(TInt aKey)
    {
    TBool retVal = EFalse; // in most cases we let also the upper levels handle these key events
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    

    // Up and down arrows break the inline word input only when shift or chr is held down
    if ( !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) &&
         !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) &&
         ( aKey==EStdKeyUpArrow || aKey==EStdKeyDownArrow ) )
        {
        return retVal;    
        }


    if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
        {
        TBool rightToLeftLang = fepMan->IsRightToLeftLanguage();
        TInt tailLength = 0;
	    ptiengine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
        switch (aKey)
            {
            case EStdKeyLeftArrow:
            case EStdKeyRightArrow:    
            	// In case the inline editing is ongoing and there is a word completion
                // currently shown, the first backwards tap will remove the suggested 
                // completion. 
                if(tailLength > 0)
	                {
	                if ((rightToLeftLang && aKey == EStdKeyLeftArrow) 
	                	|| (! rightToLeftLang && aKey == EStdKeyRightArrow ))
	                    {
					    ptiengine->HandleCommandL( EPtiCommandLockCurrentCandidate);
					    TPtrC selectedWord = ptiengine->CurrentWord();
                        fepMan->UpdateInlineEditL( selectedWord, selectedWord.Length() );  //selection.iAnchorPosition
                        // To fix the bug: STAA-7GYBJC Suggested word is not committed when Arrow right key is pressed.
                        ptiengine->CommitCurrentWord();
                        fepMan->TryCloseUiL();
                        // Asyncronous case update after the editor has handled the key
                		fepMan->SimulateKeyEventL(EKeyF19); 
	                    retVal = ETrue;
	                    }
	                else if((rightToLeftLang && aKey == EStdKeyRightArrow) 
	                	|| (! rightToLeftLang && aKey == EStdKeyLeftArrow )) 
		                {
	                    fepMan->RemoveSuggestedAdvanceCompletionL();
	                    retVal = ETrue;
		                }    	
	                }
                else
	                {
	                ptiengine->CommitCurrentWord();
	                retVal = EFalse;
	                }
            	break;
            case EStdKeyUpArrow:
            case EStdKeyDownArrow:
            case EStdKeyEnter:
            case EStdKeySpace:
            case EStdKeyTab:
            {
            	//retVal = ETrue;
				
				if ( ( aKey == EStdKeySpace || aKey == EStdKeyEnter ||aKey == EStdKeyTab)
					 && fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) 
					 && (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches)) )
		            {
		            fepMan->TryRemoveNoMatchesIndicatorL();
		            fepMan->UpdateCbaL(NULL);
		            }

            	TBool wasInlineEditing = EFalse;	
            	wasInlineEditing = fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction);            	
                ptiengine->CommitCurrentWord();
                fepMan->CommitInlineEditL();
                /* Temporary Fix */
                //EFlagInsideInlineEditingTransaction flag check is not required
                //but just to make sure.
                if(aKey == EStdKeySpace && wasInlineEditing)
                	{
                	_LIT(KSpace," ");                
	                fepMan->StartInlineEditL();
	                fepMan->UpdateInlineEditL(KSpace,0);
	                fepMan->CommitInlineEditL();
                	retVal = ETrue;
                	}                
                /* Temporary Fix */
                fepMan->TryCloseUiL();                
                // Asyncronous case update after the editor has handled the key
                fepMan->SimulateKeyEventL(EKeyF19); 
            	}                
            break;
            default:
                __ASSERT_DEBUG( EFalse, AknFepPanic(EAknFepPanicNotSupportKey) );
            }

        }
    else
        {
        iOwner->ChangeState( EInitial );
        }

    return retVal;
    }


/////////////////////////////////////////////////////////////////////////////////
//
TBool TAknFepUiInputStateEntryHalfQwertyWesternPredictive::HandleQwertyKeysL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
	TBool retVal = EFalse;
    CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
    
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
    
    // Key is consumed if the PtiEngine maps it to some character
    TBuf<KKeyMappingsLength> keyMappings; 
    
    if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
		{
	    ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseFnLower);
		}
	else
 		{
	    ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseLower);   
 		}
    //ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseLower);   
    //ITI : Pti Engine can be asked to give the proper data for the keys under Fn Key.
    TBool keyIsMapped = ( keyMappings.Length() > 0 );
    
    // The dead key handling inside the PtiDefaultCore assumes that no other key mapping queries
    // have been made between handling the dead key and the next one (these queries change the
    // internal state of the key mapping object). For this reason, we must return the key mappings
    // to their previous state. This is quite dirty hack, but so is the dead key handling code which
    // forces us to do this.
    if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
		{
	    ptiengine->MappingDataForKey( (TPtiKey)iData, keyMappings, EPtiCaseFnLower);
		}
	else
 		{
 		ptiengine->MappingDataForKey( (TPtiKey)iData, keyMappings, EPtiCaseLower);
 		}
 		
    if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches) && fnKeyState  == CAknFepFnKeyManager::EFnKeyNone )            
    	{
    	//no matches, so just play tone
        fepMan->PlaySound(EAvkonSIDConfirmationTone);   
        return keyIsMapped;     
        }  
    
    if ( keyIsMapped )
        {
       	if( fnKeyState  != CAknFepFnKeyManager::EFnKeyNone ) 
        	{
    	    //closing the UI will commit the current word and move to intial state
	    
        	// break the word    		
        	if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
        	    {
        	    fepMan->TryRemoveNoMatchesIndicatorL();
        	    }
        	fepMan->RemoveSuggestedAdvanceCompletionL();
			TPtrC selectedWord = ptiengine->CurrentWord();
            if( ptiengine->CurrentWord() != KNullDesC)
            	{
            	fepMan->UpdateInlineEditL( selectedWord, selectedWord.Length() );  //selection.iAnchorPosition
        		ptiengine->CommitCurrentWord();	
            	}
            fepMan->TryCloseUiL(); 
            ptiengine->SetCase(EPtiCaseFnLower);
        	}
        
        if ( !(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)) )
            { // 'new' inline edit                        
            ptiengine->ClearCurrentWord();
            if (caseMan->CurrentCase() == EAknEditorTextCase)
                {
                ptiengine->MarkAutoCapitalized();    
                }            
            
            fepMan->StartInlineEditL();
            }
            
		TPtrC newText;
        
        if( aKey == EStdKeyFullStop )
        	{
        	TInt tailLength = 0;
        	ptiengine->HandleCommandL( EPtiCommandLockCurrentCandidate, &tailLength );
/*        	if( fepMan->IsAutoCompleteOn() )
        		{
                TInt tailLength = 0;
                ptiengine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
                if( tailLength > 0)
                    {
                    // if autoword part is visible update the input sequence
                    newText.Set(ptiengine->CurrentWord() );
                    ptiengine->ClearCurrentWord();
                    ptiengine->SetCurrentWord(newText);
                    }
                }*/
        	}
        	
        newText.Set(ptiengine->AppendKeyPress((TPtiKey)aKey));
        	
        if ( aLength == EShortKeyPress )
        	{
        	SetShortKeyPressText(newText);
        	}
        if (ptiengine->NumberOfCandidates() > 0 )
        	{
                // more good words coming in from predictive text engine so carry on
                if(newText.Length() == 1)
           			{
	            	TDigitType digittype = fepMan->LocalDigit();
	                TChar chchar(newText[0]);
	                if(!chchar.IsDigit() || (chchar.IsDigit() && !digittype))
	                	{ 	
	                	fepMan->UpdateInlineEditL(newText, newText.Length());    
    	            	}
	                else 
	                	{
	                	TBuf<1> tempbuf;
	                	tempbuf.Append(newText[0]);
		            	ConvertDigitToDigittype(digittype,tempbuf);
		                fepMan->UpdateInlineEditL(tempbuf, tempbuf.Length());				                	
	                	}
	                if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)	
	                	{
            			ptiengine->CommitCurrentWord();
        				fepMan->CommitInlineEditL();	                	
	                	}
           			}
           		else 
           			{
           			fepMan->UpdateInlineEditL(newText, newText.Length());	
           			}
           			
        	}
        	
        else 
                {
                // engine has run out of matches 
                // we are not in the functionized state
                fepMan->SetFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);                
                fepMan->SetFlag(CAknFepManager::EFlagNoMatches);
                fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_EMPTY);
                if(newText.Length() == 1)
           			{
	            	TDigitType digittype = fepMan->LocalDigit();
	                TChar chchar(newText[0]);
	                if(!chchar.IsDigit() || (chchar.IsDigit() && !digittype))
	                	{ 	
	                	fepMan->UpdateInlineEditL(newText, newText.Length());    
    	            	}
	                else 
	                	{
	                	TBuf<1> tempbuf;
	                	tempbuf.Append(newText[0]);
		            	ConvertDigitToDigittype(digittype,tempbuf);
		                fepMan->UpdateInlineEditL(tempbuf, tempbuf.Length());					                	
	                	}
           			}
           		else 
           			{
           			// Remove the autocompletion tail if present
                    if (fepMan->IsAutoCompleteOn())
                        {
                        TInt tailLength = 0;
                        ptiengine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
                        if(tailLength > 0)
                            {
							// To remove the autocompletion part, the below function is used. 
							// DeleteKeyPress was used earlier. This is removed to fix the bug :
							// enter characters to show autocompletion part. Now press a 
							// character so that the no matches character is shown. Now press
                            // backspace key, last character is not deleted.
                            fepMan->RemoveSuggestedAdvanceCompletionL();
                            fepMan->PlaySound(EAvkonSIDConfirmationTone);
                            return ETrue;
                            }
                        }
           			fepMan->UpdateInlineEditL(newText, newText.Length());	
           			}
                fepMan->PlaySound(EAvkonSIDConfirmationTone);
                }

        // Normally we update the case after key press. However, if Chr key is held down, the case update
        // must not happen until when the Chr is released or the timer expires (in both of these cases
        // the function KeyTimerExpired() will get callled).         
        if ( !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) )
            {
       	    iOwner->CaseMan()->UpdateCase(ENullNaviEvent);
       	    }
    	else
            {
            fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);
            }
       	
        retVal = ETrue;
        }
    else
        {
        retVal = EFalse;
        }
        
    if(ptiengine->IsValidKey((TPtiKey)aKey))
       	retVal = ETrue;
        
    if( aKey!=EStdKeyBackspace && fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
        {
        retVal = ETrue;        
        }
	if( !fepMan->IsFlagSet( CAknFepManager::EFlagLongShiftKeyPress ) &&
	    fnKeyState  == CAknFepFnKeyManager::EFnKeyNext ) 
    	{
       	fepMan->SetFnKeyState(CAknFepFnKeyManager::EFnKeyNone);
       	}
       	
   	return retVal; 
    }
    

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
TBool TAknFepUiInputStateEntryHalfQwertyWesternPredictive::HandleNaviKeysL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (!(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)))
        {
        TBuf<CAknFepManager::EMaximumFepWordLength> textToUncommit;
        TBool comsumeKey;
        if (!(fepMan->TryGetTextToUncommitL(textToUncommit, aKey, comsumeKey)))
            {
            // If T9 word length > 32, the navi key is consumed without action.
            // Maybe should navigate inside word.
            return comsumeKey;
            }
        ptiengine->ClearCurrentWord();
        ptiengine->SetCurrentWord(textToUncommit); // this set the input sequence of PtiEngine
        fepMan->StartInlineEditL(fepMan->UncommittedText(), textToUncommit, 
        						 textToUncommit.Length(), EFalse);
        }

    if (aKey == EKeyBackspace)
        {
        if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
            {
            fepMan->TryRemoveNoMatchesIndicatorL();
            fepMan->UpdateCbaL(NULL);
            ptiengine->HandleCommandL(EPtiCommandSetFlagToUpdateCandidates);         
            // Need to return as DeleteKeyPress should not be passed to ptiengine.
            // This is because ptiengine does not maintain the input sequence
            // if the key press does not increase the word length. So when no matches
            // character is (?) is deleted, no need to pass to ptiengine. This is to done
            // to solve the bug :enter characters till the no matches character is shown. Now press
            // backspace, last character is not deleted.
            return ETrue;
            }
        
        TPtrC newText = ptiengine->DeleteKeyPress();
        // remove autocompletion part if any
		// this has to be done before updateinlineedit to avoid flickering
        TInt tailLength = 0;
        ptiengine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, 
                                            &tailLength );
        if(tailLength > 0)
            {
            fepMan->RemoveSuggestedAdvanceCompletionL();
            newText.Set(ptiengine->CurrentWord());
            }
        fepMan->UpdateInlineEditL(newText, newText.Length());

		fepMan->ClearFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           
        if (ptiengine->NumberOfCandidates() <= 1)
        	{
			fepMan->SetFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           	
        	}

        iOwner->CaseMan()->UpdateCase(EBackspaceEvent);
        fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);   
        fepMan->CloseUiIfWordDeletedL();
        }
    
    return ETrue;
    }


/////////////////////////////////////////////////////////////////////////////
//
void TAknFepUiInputStateEntryHalfQwertyWesternPredictive::LastWordInSelectionList()
    {

    }


//////////////////////////////////////////////////////////////////////////////
//
void TAknFepUiInputStateEntryHalfQwertyWesternPredictive::FirstWordInSelectionList()
    {   
    }


/////////////////////////////////////////////////////////////////
//
void TAknFepUiInputStateEntryHalfQwertyWesternPredictive::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
        caseMan->UpdateCase(ENullNaviEvent); 
        fepMan->ClearFlag( CAknFepManager::EFlagInsideMultitapInlineEditingTransaction );
        iData = 0;
        }
    }
   
void TAknFepUiInputStateEntryHalfQwertyWesternPredictive::ConvertDigitToDigittype(TDigitType& digittype,TDes& aConvertedNumeral)
	{
	TChar chchar(aConvertedNumeral[0]);
	switch(digittype)
    	{
    	case EDigitTypeDevanagari:
        	{
        	chchar = (aConvertedNumeral[0] + KLatinToDevanagariDigitsDelta);
	    	}
        	break;	
        case EDigitTypeArabicIndic:
            {
        	chchar = (aConvertedNumeral[0] + KLatinToArabicIndicDigitsDelta);
	        }
            break;
    	case EDigitTypeEasternArabicIndic:
        	{
        	chchar = (aConvertedNumeral[0] + KLatinToEasternArabicIndicDigitsDelta);
        	}
    	}
    aConvertedNumeral.Zero();
    aConvertedNumeral.Append(chchar);
	}
// End of file
