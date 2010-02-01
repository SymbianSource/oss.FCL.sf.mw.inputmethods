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
* Description:            Provides the TAknFepInputStateInitialIndicPhoneticMultitap methods.
*
*/











#include "aknfepuiinputstateinitialindicphoneticmultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepUIManagerWestern.h"
#include "AknFepUiIndicEnums.h"
#include "AknFepCaseManager.h"
#include "AknFepUiIndicInputManager.h"

#include <uikon.hrh>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <aknsctdialog.h>
#include <avkon.rsg>
#include <AknFep.rsg>
#include <EikEnv.h>
#include <Aknutils.h>
#define PHONETIC_SEPARATOR 0x2e

const TUint KSpaceChar = 0x0020;
const TUint KQuestionMark = 0x003F;

#define PTI_CLEAR_CURRENTWORD( A, B ) \
		( A )->ClearCurrentWord(); \
		( B ) = ETrue; \

const TText KAknFEPLineFeedSymbol = 0x21B2;
const TText KAknFEPMirroredLineFeedSymbol = 0x21B3;
	
// -----------------------------------------------------------------------------
// TAknFepInputStateInitialIndicPhoneticMultitap::TAknFepInputStateInitialIndicPhoneticMultitap

// C++ default constructor can NOT contain any code, that
// might leave or if it is absolutely necessary then MUST be trapped.
// -----------------------------------------------------------------------------

TAknFepInputStateInitialIndicPhoneticMultitap::
TAknFepInputStateInitialIndicPhoneticMultitap( MAknFepUIManagerStateInterface* aOwner,
																			TInt aLanguage )
																			:TAknFepInputStateInitialMultitapBase( aOwner )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    iIsStarKeyPressed = EFalse;
    iIndicLanguage = TLanguage(aLanguage);
    iIndicPhoneticBuffer.Zero();
    TRAP_IGNORE( ptiengine->ActivateLanguageL(KLangHindiPhonetic,
                 EPtiEngineInputModeIndicPhoneticMultitap))
    ptiengine->SetCase( EPtiCaseLower );
    iPreviousCommittedChar = NULL;
    }

// -----------------------------------------------------------------------------
// TAknFepInputStateInitialIndicPhoneticMultitap::HandleKeyL

// Handles the logic of Indic multitap input. This function first checks the validity
// of the inputed text and then enters it.
// -----------------------------------------------------------------------------

TBool TAknFepInputStateInitialIndicPhoneticMultitap::HandleKeyL( TInt aKey, 
                                                                 TKeyPressLength aLength )
	{
	TBool                      result       	= ETrue;
	MAknFepManagerUIInterface* fepMan       	= iOwner->FepMan();
	CPtiEngine* 			   ptiengine 		= iOwner->PtiEngine();
	
	if(aKey == EKeyBackspace)
		{
		iIndicPhoneticBuffer.Zero();
		iPhoneticLatinChar = 0;	
    	iPreviousCommittedChar = 0;						
		}
	else if(fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches) 
           && aKey != EPtiKey0 && aLength != ELongKeyPress)
		{
		//no matches, so just play tone
        fepMan->PlaySound(EAvkonSIDConfirmationTone);
		}
	else
		{
		if ( iData && ( iData != aKey ) && (!iIsStarKeyPressed ))
	    	{
	    	//iPrevCharacter = 
	    	TransliterateFromLatinL();
		    PTI_CLEAR_CURRENTWORD( ptiengine, iIsKeyTimerExpired )
		    // = ;
	   		}
		iIsStarKeyPressed = EFalse;
		if ( aLength == EShortKeyPress )
			{
		    if ( aKey == EPtiKeyStar ) // Overriding Key
	            {
		    	ptiengine->HandleCommandL(EPtiCommandBreakSyllable, NULL );	
		    	iIsStarKeyPressed = ETrue;
	    		}
			else
				{
				TPtrC ptiText = ptiengine->AppendKeyPress( ( TPtiKey )aKey );
				iPhoneticLatinChar = ptiText[0];
				
				if (IsToTransliterate(iPhoneticLatinChar) 
				&& (!fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches)))
			    	{
			    	TBuf<CAknFepManager::EMaximumFepWordLength> buf;
		            buf.Copy(iIndicPhoneticBuffer);
		            buf.Append(ptiText);
					fepMan->NewCharacterSequenceL(buf,EIndicInputResponsePhoneticMultitapText);
			    	}
				else if(!IsToTransliterate(iPhoneticLatinChar))
					{
					TChar prevcharacter(fepMan->PreviousChar());
					
					if((fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches)) && (KQuestionMark == prevcharacter))
						{
						fepMan->RemovePreviousCharacterL();		
						fepMan->ClearFlag(CAknFepManager::EFlagNoMatches);	
						ptiengine->HandleCommandL(EPtiCommandBreakSyllable, NULL );
						}
					iIndicPhoneticBuffer.Zero();
					if(IsToTransliterate(prevcharacter) && prevcharacter != '0')
						fepMan->CommitInlineEditL();	
					fepMan->NewCharacterSequenceL(ptiText,EIndicInputResponseNone);
					}
				}
			}
	   	else
	    	{
	    	if(fepMan->PreviousChar() == KQuestionMark)
				{
				fepMan->RemovePreviousCharacterL();		
				}
			fepMan->ClearFlag(CAknFepManager::EFlagNoMatches);

	        /* Long press of a Key */
	        if ( aKey == EPtiKeyStar)
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
		        //commit the inline text
		        fepMan->CommitInlineEditL();
		        //Remove one character, to remove the Latin character
		        //appended because of the short key press. This is 
		        //because long key press results  in short key press 
		        //and then long key press.
		        fepMan->RemovePreviousCharacterL();
		        //Reset the phonetic state in FEP. So, that any further 
		        //phonetic conversion will start from fresh.
		        iIndicPhoneticBuffer.SetLength(0);
		        iPhoneticLatinChar = 0;	
    	        iPreviousCommittedChar = 0;
    	        
		        ptiengine->HandleCommandL(EPtiCommandBreakSyllable, NULL );
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
		}
   //hindi phonetic changes
   if(iPhoneticLatinChar.IsUpper())
	{
		fepMan->HandleIndicCaseL();
	}
    return( result );
	}

// -----------------------------------------------------------------------------
// TAknFepInputStateInitialIndicPhoneticMultitap::KeyTimerExpired

// Handles the logic of post keytimerexpired event. This function commits 
// the inline editing text to the editor.
// -----------------------------------------------------------------------------

void TAknFepInputStateInitialIndicPhoneticMultitap::KeyTimerExpired()
	{
	TRAP_IGNORE(HandleKeyTimerExpiryL())
	iData = 0;	
	iOwner->PtiEngine()->ClearCurrentWord();
	if((iPreviousCommittedChar = iOwner->FepMan()->PreviousChar()) == 0x002E)
		iPreviousCommittedChar = iOwner->FepMan()->PreviousToPreviousChar(ETrue);
	iIsKeyTimerExpired = ETrue;
	}
	
void TAknFepInputStateInitialIndicPhoneticMultitap::HandleKeyTimerExpiryL()
	{
	CPtiEngine* ptiengine = iOwner->PtiEngine();
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();	
	if ( !(fepMan->EditorHasFreeSpace() 
       || fepMan->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction)))
	    {
	    return;
	    }
	if(iData && ( IsToTransliterate(iPhoneticLatinChar) &&  iData != EPtiKeyStar) 
     && !fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
		{
		TransliterateFromLatinL();
		}
	else if ( !IsToTransliterate (iPhoneticLatinChar))
        {
		MPtiLanguage* ptilang = ptiengine->CurrentLanguage();
	    TInt languageCode = (ptilang)? ptilang->LanguageCode() : 0;
		// Space clears Explicite halant for North Indian Languages.
		if(IsToRemoveHalantForLanguage(languageCode))
			{
			RemoveHalantL(TLanguage(languageCode));
			}	
		ptiengine->HandleCommandL(EPtiCommandBreakSyllable, NULL );
		fepMan->CommitInlineEditL();
		iIndicPhoneticBuffer.Zero();
        }	
	else
		{
		iOwner->FepMan()->CommitInlineEditL();
    	iIndicPhoneticBuffer.Zero();
		}	
	}

void TAknFepInputStateInitialIndicPhoneticMultitap :: TransliterateFromLatinL() 
	{
	TPhoneticArg arg;
    TBuf<CAknFepManager::EMaximumFepWordLength> destinationbuf;
    destinationbuf.Zero();               
	TInt errorcode = 0; 
	CPtiEngine* ptiengine = iOwner->PtiEngine();
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
	MPtiLanguage* ptilang = iOwner->PtiEngine()->CurrentLanguage();
    TInt languageCode = (ptilang)? ptilang->LanguageCode() : 0;
	    
	arg.iChar = iPhoneticLatinChar;
	arg.iDest = & destinationbuf;
	errorcode = ptiengine->HandleCommandL(EPtiCommandGetPhoneticText,REINTERPRET_CAST( TAny*, &arg));
	AknTextUtils::LanguageSpecificNumberConversion(destinationbuf);
	switch(errorcode)
    	{
    	case KErrNone:
    		{
    		if(arg.iState == EIndicSyllableStateSyllableBroken)
    			{
				if(iIndicPhoneticBuffer.Length())
					{
					fepMan->CommitInlineEditL(iIndicPhoneticBuffer, iIndicPhoneticBuffer.Length());
					iPreviousCommittedChar = fepMan->PreviousChar(ETrue);
					iIndicPhoneticBuffer.Zero();
					}
      			}

			if (destinationbuf.Length())
	            {
	            	if(! ValidatePhoneticInputL(destinationbuf, TLanguage(languageCode)))
	            		{
	            		break;	
	            		}
	            	
	            fepMan->NewCharacterSequenceL(destinationbuf,
	                                          EIndicInputResponsePhoneticMultitapText);

        		if(TAknFepUiIndicInputManager::IsCharOther(destinationbuf[0],
        		     TLanguage(languageCode)) && 0x002E != destinationbuf[0])
        			{
        			fepMan->CommitInlineEditL();
        			iIndicPhoneticBuffer.Zero();
        			}
				else
					{
        			iIndicPhoneticBuffer.Copy(destinationbuf);
					}
	            }
    		}
    	break;
    	case KErrOverflow:
    		{
			fepMan->NewCharacterSequenceL(destinationbuf,EIndicInputResponsePhoneticMultitapText);    		
    		fepMan->CommitInlineEditL();
        	ptiengine->HandleCommandL(EPtiCommandClearPhoneticBuffer,NULL);
    		}
    	break;
    	default:
    	break;
    	}
    iPhoneticLatinChar = 0;	
	}

TBool TAknFepInputStateInitialIndicPhoneticMultitap::IsToRemoveHalantForLanguage(TInt aLanguage)
	{
	TBool ret = EFalse;
	switch(aLanguage)
		{
		case KLangHindiPhonetic:
			ret = ETrue;
		break;
		}
	return ret;	
	}
	
inline TBool TAknFepInputStateInitialIndicPhoneticMultitap::IsToTransliterate(TChar& aChar)
	{
	return !(KAknFEPLineFeedSymbol == aChar || KSpaceChar == aChar);
	}
	
TBool TAknFepInputStateInitialIndicPhoneticMultitap::ValidatePhoneticInputL(const TDes& aBuf, 
                                                                            TLanguage aLanguage)
	{
	TBool ret = ETrue;
    TUint curChar = 0;
    TUint prevChar=0;
    TUint len=aBuf.Length();
    TPtr buffer(const_cast<TText*>( aBuf.Ptr() ),len,len);
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
	TPtiTextCase newCase = iOwner->PtiEngine()->Case();
	if(! iPreviousCommittedChar)
		{
		iPreviousCommittedChar = fepMan->PreviousChar(ETrue);	
		}
       while(len)
                {
                if(buffer[len-1] == 0x94d)
                    {
                    len--;
                    buffer.SetLength(len);
                    continue;
                    }
                else if(!curChar)
                    {
                    curChar = buffer[len-1] ;
                    len--;
                    }
                else if(!prevChar)
                    {
                    prevChar = buffer[len-1] ;
                    len--;
                    }
                else
                    break;
                }
       if(!prevChar)
            prevChar = iPreviousCommittedChar;
       /*
       Multitap does not allow any fall back characters. The phonetic engine does not
       check for the fallback characters and FEP has to do it.
       */
       //fallback character if Nukta is entered after Nukta
       if((TAknFepUiIndicInputManager::IsCharNukta(prevChar,aLanguage) && 
        TAknFepUiIndicInputManager::IsCharNukta(curChar,aLanguage)) ||
        
        //fallback character if Nukta is entered after Modifier or
        //Modifier is entered after Modifier
        (TAknFepUiIndicInputManager::IsCharModifier(prevChar,aLanguage) && 
        (TAknFepUiIndicInputManager::IsCharNukta(curChar,aLanguage) ||
        TAknFepUiIndicInputManager::IsCharModifier(curChar,aLanguage))) ||
        
        //fallback character if Nukta is entered after Vowel or
        //Nukta is entered after matra
       ((TAknFepUiIndicInputManager::IsCharAnVowel(prevChar,aLanguage) ||
       TAknFepUiIndicInputManager::IsCharMatra(prevChar,aLanguage)) && 
        TAknFepUiIndicInputManager::IsCharNukta(curChar,aLanguage))  ||
        //fallback character if Nukta is entered after Nukta Consonant
        (TAknFepUiIndicInputManager::IsCharNuktaConsonant(prevChar,aLanguage) &&
        TAknFepUiIndicInputManager::IsCharNukta(curChar,aLanguage)) ||
        
        //fallback character if Nukta entered in an empty editor
        (prevChar == PHONETIC_SEPARATOR &&
        TAknFepUiIndicInputManager::IsCharNukta(curChar,aLanguage))
        )
           {
            TBuf<1> buffer;
            fepMan->CommitInlineEditL(iIndicPhoneticBuffer, iIndicPhoneticBuffer.Length());
	        buffer.Append(KQuestionMark);
		if(!fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
			{
			fepMan->StartInlineEditL();	
			}
        fepMan->UpdateInlineEditL(buffer,CAknFepManager::ESingleCharacter);
        fepMan->SetFlag(CAknFepManager::EFlagNoMatches);
		ret = EFalse;
		}
	return ret;
	}
	
TBool TAknFepInputStateInitialIndicPhoneticMultitap::IsInvalidPhoneticCharacter( 
                                              TUint aChar, TLanguage aLanguage )
	{
	TBool ret = EFalse;
	if(TAknFepUiIndicInputManager::IsCharNukta( aChar, aLanguage )
	   || TAknFepUiIndicInputManager::IsCharMatra( aChar,  aLanguage )
	   || TAknFepUiIndicInputManager::IsCharModifier( aChar,  aLanguage )
	   || TAknFepUiIndicInputManager::IsCharVirama( aChar,  aLanguage ))
		{	
		ret = ETrue;
		}
	return ret;	
	}

void TAknFepInputStateInitialIndicPhoneticMultitap::RemoveHalantL(TLanguage aLanguage)
	{
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
	TBool isLineFeedEntered = fepMan->IsFlagSet(CAknFepManager::EFlagLineFeedCharacter);
	if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
        {
        fepMan->CommitInlineEditL();
        }

	TInt position = 0;
	
    if(TAknFepUiIndicInputManager::IsCharVirama(fepMan->PreviousChar(ETrue), aLanguage))
    	{
    	position = 1;
    	}
    else if(!isLineFeedEntered && 
    TAknFepUiIndicInputManager::IsCharVirama(fepMan->PreviousToPreviousChar(ETrue), aLanguage))
    	{
    	position = 2;	
    	}
    if(position)	
    	fepMan->RemoveTextFromEditorL( 1, position-1, EFalse );
   	if(fepMan->NextChar() == KSpaceChar)
   		{
   		fepMan->AlignLogicalAndVisualCursorL(TTmDocPosSpec::ELeading, EFalse);	
   		}
	}

void TAknFepInputStateInitialIndicPhoneticMultitap::GetPhoneticLatinChar(TChar& aChar)const
	{
		aChar = iPhoneticLatinChar;
	}
//End of File
