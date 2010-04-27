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
* Description:           
*       Provides the TAknFepUiInputStateEntryVietnamesePredictive methods.
*
*/












#include "AknFepUiInputStateEntryVietnamesePredictive.h"
#include "AknFepUIInputStateInitialVietnameseMultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepCaseManager.h"
#include "AknFepVietnameseToneManager.h"
#include "AknFepPluginManager.h"

#include <uikon.hrh>
#include <PtiEngine.h>                          //CPtiEngine
#include <PtiDefs.h>                            //keys
#include <aknSctDialog.h>
#include <avkon.rsg>
#include <AknFep.rsg>

#include "AknFepUiManagerWestern.h"
TAknFepInputStateEntryVietnamesePredictive::
TAknFepInputStateEntryVietnamesePredictive(MAknFepUIManagerStateInterface* aOwner)
: TAknFepInputStateEntryWesternPredictive(aOwner)   
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if (ptiengine->CurrentLanguage()->LanguageCode() != ELangVietnamese)
        {
        TRAP_IGNORE(ptiengine->ActivateLanguageL(ELangVietnamese));
        }
#ifndef RD_INTELLIGENT_TEXT_INPUT
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        if(fepMan->IsAutoCompleteOn())
    	{
    	   ptiengine->SetInputMode(EPtiEngineWordCompletion);
    	}
    	else
#endif
        {
        ptiengine->SetInputMode(EPtiEnginePredictive);
        }
    }

TBool TAknFepInputStateEntryVietnamesePredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TBool response = ETrue;

    switch (aKey)
        {
    case EPtiKey1: //fall through
    case EPtiKey2:
    case EPtiKey3:
    case EPtiKey4:
    case EPtiKey5:
    case EPtiKey6:
    case EPtiKey7:
    case EPtiKey8:
    case EPtiKey9:
        response = HandleStandardNumericKeysL(aKey, aLength);
        break;
    case EPtiKey0:
        if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
            {
            fepMan->TryRemoveNoMatchesIndicatorL();
#ifndef RD_INTELLIGENT_TEXT_INPUT
            if(fepMan->IsAutoCompleteOn())
					{
					fepMan->RemoveSuggestedCompletionL();
					}
#endif
            }
        fepMan->UpdateCbaL(NULL);
        if (fepMan->IsFlagSet(CAknFepManager::EFlagCompoundWord))
            { 
            if (fepMan->IsCursorAtEndOfWord())
                {            
                fepMan->AddCompoundWordToUdbL();
                }
            fepMan->ClearFlag(CAknFepManager::EFlagCompoundWord);
            }
        ptiengine->CommitCurrentWord();  // commit word after calling TryCloseUiL().
        fepMan->TryCloseUiL();
        response = EFalse;  //pass to initial state to handle key
        break;
    case EKeyLeftArrow: // fall through
    case EKeyRightArrow:
        response = HandleNaviKeysL(aKey, aLength);
        break;    
    case EKeyBackspace:
        response = HandleDeleteKeysL(aKey, aLength);
        break;
    case EPtiKeyStar:
        response = HandleStarKeyL(aLength);
        break;
    case EKeyDownArrow: // Japanese input required
        if (fepMan->IsAbleScrollLatinPredictive())
            {
            if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                DoNextMatch();
                }
            else
                {
                response = EFalse;
                }
            }
        else
            {
            response = EFalse;
            }
        break;
    case EKeyUpArrow: // Japanese input required
        if (fepMan->IsAbleScrollLatinPredictive())
            {
            if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                DoPreviousMatchL();
                }
            else
                {
                response = EFalse;
                }
            }
        else
            {
            response = EFalse;
            }
        break;
    default:
        response = EFalse;
        break;
        }
    return response;
    }

TBool TAknFepInputStateEntryVietnamesePredictive::HandleStarKeyL(TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (aLength == ELongKeyPress)
        { 
        if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
            {
            // replace with previous match
            fepMan->TryRemoveNoMatchesIndicatorL();
            iMatchState = EWordMatchStd;
            TPtrC newText = ptiengine->PreviousCandidate();
            fepMan->UpdateInlineEditL(newText, newText.Length());
            }
            
#ifndef RD_INTELLIGENT_TEXT_INPUT
        	// to rip off suggested completion when SCT is launched
		if(fepMan->IsAutoCompleteOn()) 
			{
			fepMan->RemoveSuggestedCompletionL();	
			}
#endif
       	if (!fepMan->EditSubmenuInUse())
        	{
        	fepMan->LaunchSpecialCharacterTableL();
        	}            
        }
    else 
        {
        if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
            {
#ifdef RD_SCALABLE_UI_V2
            if ( (iIsLastWord || ptiengine->NumberOfCandidates() == 1) 
                 && ( fepMan->PluginUIManager()->PluginInputMode() == EPluginInputModeItut ) )
                {
                fepMan->PluginUIManager()->SetFingerMatchSelectionState(ETrue);
                fepMan->PluginUIManager()->ShowAllCandidates();
                iMatchState = EWordMatchNone;
                iIsLastWord = EFalse;
                return ETrue;
                }
#endif //RD_SCALABLE_UI_V2
            if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
                {
                //no more matches
                fepMan->PlaySound(EAvkonSIDWarningTone);
                }
            else if (ptiengine->NumberOfCandidates()<=1)
                { 
                // only one match
                if (iMatchState == EWordMatchFirst || iMatchState == EWordMatchNone)
                    {
                    fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_EMPTY);
                    iMatchState = EWordMatchLast;
                    }
                }
            else
                { 
                // > 1 match
                if (iMatchState == EWordMatchLast && !(ptiengine->LastEnteredKey() == EPtiKey1))
                    {
                    fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_PREVIOUS);
                    }
                else if (iMatchState == EWordMatchFirst || iMatchState == EWordMatchNone)
                    {
                    fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_EMPTY_PREVIOUS);
                    iMatchState = EWordMatchStd;
                    }
                TPtrC newText = ptiengine->NextCandidate();
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
	                	switch(digittype)
		                	{
		                	case EDigitTypeDevanagari:
			                	{
			                	chchar = (newText[0] + KLatinToDevanagariDigitsDelta);
	            		    	}
			                	break;	
			                case EDigitTypeArabicIndic:
				                {
			                	chchar = (newText[0] + KLatinToArabicIndicDigitsDelta);
	            		        }
				                break;
		                	case EDigitTypeEasternArabicIndic:
			                	{
			                	chchar = (newText[0] + KLatinToEasternArabicIndicDigitsDelta);
			                	}
		                	}
	            		tempbuf.Append(chchar);				                	
		                fepMan->UpdateInlineEditL(tempbuf, tempbuf.Length());	
	                	}
           			}
           		else 
           			{
           			fepMan->UpdateInlineEditL(newText, newText.Length());	
           			}
                }
            }
        else
            {
  			if (!fepMan->EditSubmenuInUse())
        		{
        		fepMan->LaunchSpecialCharacterTableL();
        		}
            }
        }
    return ETrue;
    }

TBool TAknFepInputStateEntryVietnamesePredictive::HandleDeleteKeysL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (!(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)))
        {
        if(fepMan->PreviousChar() == TText(0x0020))      // 0x0020 is space 
        {
        	return EFalse;
        }
        TBuf<CAknFepManager::EMaximumFepWordLength> textToUncommit;
        TBool comsumeKey;
        if (!(fepMan->TryGetTextToUncommitL(textToUncommit, aKey, comsumeKey)))
            {
            // If T9 word length > 32, the navi key is consumed without action.
            // Maybe should navigate inside word.
            return comsumeKey;
            }
        ptiengine->SetCurrentWord(textToUncommit);
		fepMan->ClearFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           
        if (ptiengine->NumberOfCandidates() <= 1)
        	{
			fepMan->SetFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           	
        	}        
        fepMan->StartInlineEditL(fepMan->UncommittedText(), textToUncommit, 
                                 textToUncommit.Length(), EFalse);
        }
  
    if (aKey == EKeyBackspace)
        {
        if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
            {
            fepMan->TryRemoveNoMatchesIndicatorL();
            fepMan->UpdateCbaL(NULL);
            return ETrue;
            }
        else if (iMatchState != EWordMatchFirst)
            {
            //let FirstWordInSelectionList() reset the CBA
            iMatchState = EWordMatchNone;
            }
        
        TBuf<2> newText;
        // Get the last character from the current editor
        TText prevChar = fepMan->PreviousChar();
        TBool vowelList = EFalse;
        for (TInt index = 0; index < KNumberOfToneMarks; index ++)
            {
            if (VietToneMatrix[0][index] == prevChar)
                {
                vowelList = ETrue;
                break;
                }
            }
        
        if (vowelList)
            {
                ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();
            }
        else
            {
            if (prevChar <= 0)
                {
                return EFalse;
                }
            
            // Judge if the last character needs to be converted
            if ( TAknFepInputStateInitialVietnameseMultitap::NeedsVietnameseBkDelSupport(prevChar,
                                                                                         newText) )
                {
                ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();
                }
            else
                {
                TPtrC  updatedText = ptiengine->DeleteKeyPress();
                if(ptiengine->InputMode() != EPtiEngineWordCompletion && newText.Length())
                	{
                	fepMan->UpdateInlineEditL(updatedText, newText.Length());
                	}
                else
                	{
                	fepMan->UpdateInlineEditL(updatedText, updatedText.Length());	
                	}
                }
            }

		fepMan->ClearFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           
        if (ptiengine->NumberOfCandidates() <= 1)
        	{
			fepMan->SetFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           	
        	}

        iOwner->CaseMan()->UpdateCase(EBackspaceEvent);
        fepMan->CloseUiIfWordDeletedL();
        }
        
    return ETrue;
    }

// End of file    
