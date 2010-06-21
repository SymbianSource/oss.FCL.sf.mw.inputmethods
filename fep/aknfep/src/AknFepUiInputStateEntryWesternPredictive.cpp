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
*       Provides the TAknFepInputStateEntryWesternPredictive methods.
*
*/












#include "AknFepUiInputStateEntryWesternPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepManager.h"
#include "AknFepPanic.h"
#include "AknFepCaseManager.h"
#include "AknFepPluginManager.h"
#include "AknFepManagerInterface.h" // MAknFepManagerInterface

#include <aknedsts.h> 
#include <PtiEngine.h>                             //CPtiEngine
#include <PtiDefs.h>                            //keys
#include <featmgr.h>                //FeatureManager
#include <e32keys.h>
#include <aknfep.rsg>
#include <PtiUserDicEntry.h>

// The Maximum world size
const TInt KMaxWordSize = 63;
TAknFepInputStateEntryWesternPredictive::
TAknFepInputStateEntryWesternPredictive(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateBase(aOwner)
    {
    //this class insists on the existence of a case manager
    __ASSERT_DEBUG(iOwner->CaseMan(), AknFepPanic(EAknFepPanicNoCaseManager));
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    iState = EEntry;
    iMatchState = EWordMatchNone;
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TPtiTextCase caseMode = ptiengine->Case();
	ptiengine->SetInputMode(EPtiEnginePredictive);  

#ifdef RD_SCALABLE_UI_V2

    if (fepMan->PluginUIManager()->MatchCandidateList().Count() > 0)
        {
        fepMan->PluginUIManager()->ResetCandidateList();
        }
    
        iIsLastWord = EFalse;        
/*
    if (fepMan->PluginUIManager()->GetSpellText())
        {
        TBuf<CAknFepManager::EMaximumFepWordLength> textToUncommit;
        textToUncommit.Zero();
        textToUncommit.Copy(*fepMan->PluginUIManager()->GetSpellText());
        ptiengine->SetCurrentWord(textToUncommit); 
        }
*/        
#ifdef RD_INTELLIGENT_TEXT_INPUT
    	ptiengine->SetInputMode(EPtiEnginePredictive);  
        TBool isUnlimitedEditor = EFalse;
        ptiengine->SetMaxLengthForAutoCompletedCandidates(
                          fepMan->EditorFreeSpaceForAutoWordCompletion(isUnlimitedEditor ));
        // To set the input mode
        if(fepMan->IsAutoCompleteOn())
        	{
        	TRAP_IGNORE(ptiengine->HandleCommandL(EPtiCommandEnableAutoCompletion));
        	}
        else
        	{
        	TRAP_IGNORE(ptiengine->HandleCommandL(EPtiCommandDisableAutoCompletion));
        	}

        TPtiKeyboardType keyboard = fepMan->KeyboardLayout();
    	ptiengine->SetKeyboardType(keyboard);

    	TBool isNumberCandidateShown = fepMan->IsAdvancedPredictiveNumberCandidateShown();
    	TRAP_IGNORE(ptiengine->HandleCommandL( EPtiCommandSetNumberCandidateStatus, 
                 &isNumberCandidateShown ));
#endif

#else

    ptiengine->ClearCurrentWord();
        
#ifndef RD_INTELLIGENT_TEXT_INPUT

    // To set the input mode
    if(fepMan->IsAutoCompleteOn())
    	{
    	   ptiengine->SetInputMode(EPtiEngineWordCompletion);
    	}
#endif // RD_PRED_AW_COMPLETION
#endif //RD_SCALABLE_UI_V2
    ptiengine->SetCase(caseMode);
    }

TBool TAknFepInputStateEntryWesternPredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

#ifdef RD_SCALABLE_UI_V2

    if (fepMan->PluginUIManager()->GetSpellText())
        {
        fepMan->PluginUIManager()->SpellText(NULL);
        }    
#endif //

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
#ifdef RD_SCALABLE_UI_V2
        fepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);    
#endif //RD_SCALABLE_UI_V2
        
        response = HandleStandardNumericKeysL(aKey, aLength);
        break;
    case EPtiKey0:
        if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
            {
            fepMan->TryRemoveNoMatchesIndicatorL();
#ifndef RD_INTELLIGENT_TEXT_INPUT
 	    			// To rip off suggested word completion when user presses SPACE
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
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
            {
            ptiengine->CommitCurrentWord();
			fepMan->CommitInlineEditL();
            fepMan->TryCloseUiL();
            response = EFalse;
            }
        else
        	{
#endif
		fepMan->SetDelayedCommit(); // Delayed 'commit'. 				                                        	
	    fepMan->TryCloseUiL();	                                
        response = EFalse;  //pass to initial state to handle key
#ifdef RD_INTELLIGENT_TEXT_INPUT
        	}
#endif        	
#ifdef RD_SCALABLE_UI_V2
        fepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);    
#endif //RD_SCALABLE_UI_V2
        break;
    case EKeyLeftArrow: // fall through
    case EKeyRightArrow:
    case EKeyBackspace:
        response = HandleNaviKeysL(aKey, aLength);
#ifdef RD_SCALABLE_UI_V2
        fepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);    
#endif //RD_SCALABLE_UI_V2
        break;
    case EPtiKeyStar:
            {
            response = HandleStarKeyL(aLength);    
            }
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
#ifdef RD_SCALABLE_UI_V2
        fepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);            
#endif //RD_SCALABLE_UI_V2
        break;
        }
    return response;
    }

void TAknFepInputStateEntryWesternPredictive::HandleCommandL(TInt aCommandId)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();    
    if (aCommandId == EAknFepSoftkeyPrevious)
        {
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        
        __ASSERT_DEBUG(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction), 
                       AknFepPanic(EAknFepPanicNotInsideInlineEdit));   

        if (iMatchState == EWordMatchLast || iMatchState == EWordMatchAfterLast)
            {
            fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_EMPTY_PREVIOUS);
            }
        iMatchState = EWordMatchStd; 
        TPtrC prevText = ptiengine->PreviousCandidate();
        if(prevText.Length() == 1)
   			{
        	TDigitType digittype = fepMan->LocalDigit();
            TChar chchar(prevText[0]);
            if(!chchar.IsDigit() || (chchar.IsDigit() && !digittype))
            	{ 	
            	fepMan->UpdateInlineEditL(prevText, prevText.Length());    
            	}
            else 
            	{
            	TBuf<1> tempbuf;
	            tempbuf.Append(prevText[0]);
            	ConvertDigitToDigittype(digittype,tempbuf);
                fepMan->UpdateInlineEditL(tempbuf, tempbuf.Length());	
            	}
   			}
   		else 
   			{        
			fepMan->UpdateInlineEditL(prevText, prevText.Length());
   			}
        }
    else if (aCommandId == ESignalSelectMatchSelectionText)
        {
        //fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_FINGER);
#ifdef RD_SCALABLE_UI_V2
        fepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);
#endif //RD_SCALABLE_UI_V2
        iOwner->ChangeState(EInitial);
        }
    else if (aCommandId == ESignalNaviLeftMatchSelectionText)
        {
        //fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_FINGER);
#ifdef RD_SCALABLE_UI_V2
        fepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);
#endif //RD_SCALABLE_UI_V2
        iOwner->ChangeState(EEntry);

        TRawEvent event;
        event.Set(TRawEvent::EKeyDown, EStdKeyLeftArrow);
        CCoeEnv::Static()->WsSession().SimulateRawEvent(event);
        
        event.Set(TRawEvent::EKeyUp, EStdKeyLeftArrow);
        CCoeEnv::Static()->WsSession().SimulateRawEvent(event);
        }
    else if (aCommandId == ESignalNaviRightMatchSelectionText)
        {
        //fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_FINGER);
#ifdef RD_SCALABLE_UI_V2
        fepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);
#endif //RD_SCALABLE_UI_V2
        iOwner->ChangeState(EEntry);

        TRawEvent event;
        event.Set(TRawEvent::EKeyDown, EStdKeyRightArrow);
        CCoeEnv::Static()->WsSession().SimulateRawEvent(event);
        
        event.Set(TRawEvent::EKeyUp, EStdKeyRightArrow);
        CCoeEnv::Static()->WsSession().SimulateRawEvent(event);
        }        
    }

TBool TAknFepInputStateEntryWesternPredictive::HandleStandardNumericKeysL(TInt aKey, 
                                                                          TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TBool response = ETrue;

    if (aLength == ELongKeyPress)
        {
        if (!fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
        	{
        	// No edit transaction going on, we have ended here because of long
        	// key press created editor on UI or some other similar non-standard situation.
        	// We can't handle key press, so just consume it.
        	return ETrue;	
        	}
                        
        // try and insert a digit
        if (!fepMan->TryRemoveNoMatchesIndicatorL())
            { 
            // remove last character
            TPtrC newText = ptiengine->DeleteKeyPress();
#ifndef RD_INTELLIGENT_TEXT_INPUT
            if(ptiengine->InputMode() == EPtiEngineWordCompletion 
           && (newText.Length() > ptiengine->CurrentInputSequence().Length()))
            	{
            	    	fepMan->RemoveSuggestedCompletionL();
            	}
       		else
#endif
	       		{
            fepMan->UpdateInlineEditL(newText, newText.Length());
	            }
            }
        //closing the UI will commit the current word and move to intial state
        fepMan->TryCloseUiL();
        //this flag to ensures that initial state does not send us back 
        //to this state as it would normally do for numeric keys
        fepMan->SetFlag(CAknFepManager::EFlagSuppressInputStateChange);
        response = EFalse;
        }
    else  // short keypress 
        {
        //To restrict up to 32 character..
        
        if(ptiengine->CurrentInputSequence().Length() >= KMaxWordSize)
        	{
            fepMan->SetFlag(CAknFepManager::EFlagNoMatches);
            
            // Enter word reach to max inline edit state
            // if we call upadte inline edit, it will insert '?' mark
            // which would be the 64th/33rd character
           
            // Not need to call appedkeypress/ updateinlineedit
            // ignore the current key press
            
            //fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_EMPTY);
            //TPtrC newText = ptiengine->CurrentWord();
			//fepMan->UpdateInlineEditL(newText, newText.Length());
            }
        if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
            {
            //no matches, so just play tone
            fepMan->PlaySound(EAvkonSIDConfirmationTone);
            }           
        else //insert a letter
            {
            fepMan->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp |
                                CAknFepManager::ECcpuStateNewPredictiveWord);
            if (iMatchState == EWordMatchLast || iMatchState == EWordMatchAfterLast)
                {
                iMatchState = EWordMatchNone;
                }
            if (!(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)))
                { // 'new' inline edit
                ptiengine->ClearCurrentWord();
                fepMan->StartInlineEditL();
                fepMan->TryStartCompoundWord();
                }
#ifndef RD_INTELLIGENT_TEXT_INPUT
  			TInt prevSeqLength = ptiengine->CurrentInputSequence().Length();
#endif 
            TPtrC newText = ptiengine->AppendKeyPress((TPtiKey)aKey);

 			fepMan->ClearFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);   
 			
#ifdef RD_INTELLIGENT_TEXT_INPUT
 			if (ptiengine->NumberOfCandidates() > 0)
#else
           if ((fepMan->IsMoreGoodWordsComing(newText.Length())
#ifndef RD_INTELLIGENT_TEXT_INPUT
				&& (ptiengine->InputMode() != EPtiEngineWordCompletion))
                || fepMan->IsMoreGoodAutoCompleteWordsComing(ptiengine->InputMode(), 
                										  prevSeqLength, ptiengine->CurrentInputSequence().Length()
#endif
		))
#endif
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
           			}
           		else 
           			{
           			fepMan->UpdateInlineEditL(newText, newText.Length());	
           			}
           			
                TBool lastEnteredDigitOne = (ptiengine->LastEnteredKey() == EPtiKey1);
                if (!lastEnteredDigitOne)
                    {
                    iOwner->CaseMan()->UpdateCase(ENullNaviEvent);
                    }
                fepMan->ClearFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);
				if (ptiengine->NumberOfCandidates() <= 1)                    
					{
	                fepMan->SetFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);                					
					}
                }
            else 
                {
                // engine has run out of matches
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
#ifdef RD_INTELLIGENT_TEXT_INPUT
           			// Remove the autocompletion tail if present
                    if (fepMan->IsAutoCompleteOn())
                        {
                        TInt tailLength = 0;
                        ptiengine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, 
                                                  &tailLength );
                        if(tailLength > 0)
                            {
                            // To remove the autocompletion part, the below function is used. 
                            // DeleteKeyPress was used earlier. This is removed to fix the bug :
                            // enter characters to show autocompletion part. Now press a 
                            // character so that the no matches character is shown. Now press
                            // backspace key, last character is not deleted.
                            fepMan->RemoveSuggestedAdvanceCompletionL();
                            fepMan->PlaySound(EAvkonSIDConfirmationTone);
                            return response;
                            }
                        }
#endif
           			fepMan->UpdateInlineEditL(newText, newText.Length());	
           			}
                fepMan->PlaySound(EAvkonSIDConfirmationTone);
                }
            }
        }
    return response;
    }

TBool TAknFepInputStateEntryWesternPredictive::HandleNaviKeysL(TInt aKey, TKeyPressLength /*aLength*/)
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
#ifdef RD_INTELLIGENT_TEXT_INPUT
        ptiengine->ClearCurrentWord(); // Clear the buffer or setting input may fail
#endif
        ptiengine->SetCurrentWord(textToUncommit);
        if( ptiengine->CurrentWord().Length() != textToUncommit.Length())
        	{
        	TPtiUserDictionaryEntry newUdbEntry(textToUncommit);
        	ptiengine->AddUserDictionaryEntry(newUdbEntry);
        	ptiengine->SetCurrentWord(textToUncommit);
        	}

#ifdef   __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__ 
		// First generation ITUT support loops through candidate list on star key press. When we bring
		// a word in inline edit by navigating with arrow keys, we need to fetch the candidate list in order
		// to navigate through the predictive list if any or get spell CBA.
        CDesCArrayFlat* matchesList=new(ELeave)CDesCArrayFlat(1);
        CleanupStack::PushL(matchesList);
        ptiengine->GetCandidateListL(*matchesList);
        CleanupStack::PopAndDestroy();//matchesList
#endif        
        	
		fepMan->ClearFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           
        if (ptiengine->NumberOfCandidates() <= 1)
        	{
			fepMan->SetFlag(CAknFepManager::EFlagRemoveMatchesMenuItem);           	
        	}        
        if (ptiengine->NumberOfCandidates() < 1)
        	{
            fepMan->SetFlag(CAknFepManager::EFlagNoMatches);
            fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_EMPTY);           	
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
#ifdef RD_INTELLIGENT_TEXT_INPUT
            ptiengine->HandleCommandL(EPtiCommandSetFlagToUpdateCandidates);
#endif //RD_INTELLIGENT_TEXT_INPUT
            // Need to return as DeleteKeyPress should not be passed to ptiengine.
            // This is because ptiengine does not maintain the input sequence
            // if the key press does not increase the word length. So when no matches
            // character is (?) is deleted, no need to pass to ptiengine. This is to done
            // to solve the bug :enter characters till the no matches character is shown. Now press
            // backspace, last character is not deleted.
            return ETrue;
            }
        else if (iMatchState != EWordMatchFirst)
            {
            //let FirstWordInSelectionList() reset the CBA
            iMatchState = EWordMatchNone;
            }
        
        TPtrC newText = ptiengine->DeleteKeyPress();
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TPtiEngineInputMode inputMode = ptiengine->InputMode();
        if((inputMode != EPtiEngineWordCompletion && inputMode != EPtiEnginePredictive )
        	&& newText.Length() != ptiengine->CurrentInputSequence().Length())
#else
        if((ptiengine->InputMode() != EPtiEngineWordCompletion )
            	&& newText.Length() != ptiengine->CurrentInputSequence().Length())
#endif
        {
        	TBuf<CAknFepManager::EMaximumFepWordLength> textToUncommit;
        	TBool comsumeKey;
        	fepMan->TryGetTextToUncommitL(textToUncommit, aKey, comsumeKey);
           	textToUncommit.SetLength(textToUncommit.Length() - 1);
           	ptiengine->SetCurrentWord(textToUncommit);
           	newText.Set(textToUncommit);
        }
        

#ifdef RD_INTELLIGENT_TEXT_INPUT
        if (!fepMan->IsAutoCompleteOn())
            {
			// rip-off the autocompletion part if any, do this before updating the editor content
			// this is done to avoid any flickering
    	    TInt tailLength = 0;
        	ptiengine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, 
                                            &tailLength );
	        if(tailLength > 0)
    	        {
        	    fepMan->RemoveSuggestedAdvanceCompletionL();
				// text would have changed after ripping off the autocompletion part
	            newText.Set(ptiengine->CurrentWord());
    	        }
            }
#endif            
            
        fepMan->UpdateInlineEditL(newText, newText.Length());                    
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

TBool TAknFepInputStateEntryWesternPredictive::HandleStarKeyL(TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (aLength == ELongKeyPress)
        { 

// In non-ITI mode, during long star key press, initially it is treated as short key press 
// and the next candidate word is shown. So before displaying the SCT, we revert to
// the previous candidate. In ITI, long star key press does not result in showing the next
// candidate, as short key press only launches the candidate list.
// So reverting to the previous candidate is not needed in ITI mode.        
#ifndef RD_INTELLIGENT_TEXT_INPUT        
        if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
            {
            // replace with previous match
            fepMan->TryRemoveNoMatchesIndicatorL();
            iMatchState = EWordMatchStd;
            TPtrC newText = ptiengine->PreviousCandidate();
            fepMan->UpdateInlineEditL(newText, newText.Length());
            }
		// to rip off suggested completion when SCT is launched
		if(fepMan->IsAutoCompleteOn()) 
			{
			fepMan->RemoveSuggestedCompletionL();	
			}
#endif
            
       	fepMan->LaunchSpecialCharacterTableL();
        }
    else 
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifndef FF_ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT
        // Short key presses get filtered in AknFepKeyCatcher, so that short star
        // key press only gets through when the key is lifted. This means we can
        // launch the candidate popup without worrying about long keypress
        if (fepMan->WesternPredictive() && aLength == EShortKeyPress && !fepMan->IsQwerty()
        	&& fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)
        	&& ptiengine->NumberOfCandidates() > 0 
        	&& fepMan->PluginUIManager()->PluginInputMode() == EPluginInputModeNone )
        	{
    		// With short * key press and candidates remaining, override normal behaviour
        	// and function like arrow down
        	TInt wordIndexToSelect = 0;
        	TInt lastCandidateIndex = ptiengine->NumberOfCandidates()-1;
    		ptiengine->HandleCommandL( EPtiCommandUserActionGetCurrentIndexOfCandidates, 
					&wordIndexToSelect );
    		wordIndexToSelect < lastCandidateIndex ? wordIndexToSelect++ : wordIndexToSelect = 0;
    		fepMan->LaunchCandidatePopupListL( wordIndexToSelect );
    		return ETrue;
        	}
#endif // FF_ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT  
#endif // RD_INTELLIGENT_TEXT_INPUT          
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
                CAknFepManager* fepMgr = static_cast<CAknFepManager*>(fepMan);
                if(( fepMgr->EditorState()->Flags() & EAknEditorFlagEnablePartialScreen ) &&
                   ( fepMgr->PluginUIManager()->PluginInputMode() == EPluginInputModeItut ))
                    {
                    // launch full screen query for split itut
                    fepMgr->PluginUIManager()->HandleServerEventL(ESignalEnterSpellMode);  
                    }
                else
                    {
                    //no more matches
                    fepMan->PlaySound(EAvkonSIDWarningTone);
                    }
                }
            else if (ptiengine->NumberOfCandidates() <= 1) //1
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
	                	tempbuf.Append(newText[0]);
		            	ConvertDigitToDigittype(digittype,tempbuf);
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

void TAknFepInputStateEntryWesternPredictive::LastWordInSelectionList()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    
    iIsLastWord = ETrue;
    
    if (iMatchState != EWordMatchAfterLast)
        {
        TInt newResource = R_AKNFEP_SOFTKEYS_SPELL_PREVIOUS;
        if (ptiengine->LastEnteredKey() == EPtiKey1)
            {
            newResource=R_AKNFEP_SOFTKEYS_SYMBOL_PREVIOUS;
            }
        TRAP_IGNORE(fepMan->UpdateCbaL(newResource));
        iMatchState = EWordMatchLast;
        }
    }

void TAknFepInputStateEntryWesternPredictive::FirstWordInSelectionList()
    {   
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    iIsLastWord = EFalse;

    if (iMatchState == EWordMatchLast)
        {
        //have cycled past the last word back to the beginning
        iMatchState = EWordMatchAfterLast;
        }
    else if (iMatchState == EWordMatchNone || iMatchState == EWordMatchStd) // called on every new keypress so checked to stop constant update
        {
        //have started/restarted a word or
        //have got back to the beginning using previous
        TRAP_IGNORE(fepMan->UpdateCbaL(NULL));
        iMatchState = EWordMatchFirst;
        }
    }

void TAknFepInputStateEntryWesternPredictive::DoPreviousMatchL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    
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
        if (iMatchState == EWordMatchLast || iMatchState == EWordMatchAfterLast)
            {
            fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_EMPTY_PREVIOUS);
            }
        else if (iMatchState == EWordMatchFirst || iMatchState == EWordMatchNone)
            {
            fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_EMPTY_PREVIOUS);
            }
        iMatchState = EWordMatchStd;
        TPtrC newText = ptiengine->PreviousCandidate();
        fepMan->UpdateInlineEditL(newText, newText.Length());
        }
    }

void TAknFepInputStateEntryWesternPredictive::DoNextMatch()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    
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
            TRAP_IGNORE(fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_EMPTY));
            iMatchState = EWordMatchLast;
            }
        }
    else
        { 
        // > 1 match
        if (iMatchState == EWordMatchLast && !(ptiengine->LastEnteredKey() == EPtiKey1))
            {
            TRAP_IGNORE(fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_PREVIOUS));
            }
        else if (iMatchState == EWordMatchFirst || iMatchState == EWordMatchNone)
            {
            TRAP_IGNORE(fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_EMPTY_PREVIOUS));
            iMatchState = EWordMatchStd;
            }
        TPtrC newText = ptiengine->NextCandidate();
        TRAP_IGNORE(fepMan->UpdateInlineEditL(newText, newText.Length()));
        }
    }

void TAknFepInputStateEntryWesternPredictive::ConvertDigitToDigittype(TDigitType& digittype,
                                                                      TDes& aConvertedNumeral)
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

TBool TAknFepInputStateEntryWesternPredictive::IsMoreCandidate()
    {
    TBool ret = ETrue;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();    
    
    if (fepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
        {
        ret = EFalse;
        }

    if (ptiengine->NumberOfCandidates() < 1)
        { 
        ret = EFalse;
        }
    else if (ptiengine->NextCandidate().Length() == 0)
        {
        ret = EFalse;
        }
    else
        {
        ptiengine->PreviousCandidate();    
        }    
    
    return ret;
    }

// End of file
