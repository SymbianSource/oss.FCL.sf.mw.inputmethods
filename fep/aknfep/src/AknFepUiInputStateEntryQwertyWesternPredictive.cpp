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
*       Provides the TAknFepInputStateEntryQwertyWesternPredictive methods.
*
*/












#include "AknFepUiInputStateEntryQwertyWesternPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiManagerWestern.h"
#include "AknFepManager.h"
#include "AknFepPanic.h"
#include "AknFepCaseManager.h"
#include "AknFepPluginManager.h"

#include <PtiEngine.h>              //CPtiEngine
#include <PtiDefs.h>                //keys
#include <PtiCompositionDataIF.h>   //MPtiEngineCompositionDataInterface
#include <featmgr.h>                //FeatureManager
#include <e32keys.h>
#include <aknfep.rsg>


static const TInt KKeyMappingsLength = 63;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
TAknFepInputStateEntryQwertyWesternPredictive::
TAknFepInputStateEntryQwertyWesternPredictive(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateBase(aOwner)
    {
    //this class insists on the existence of a case manager
    __ASSERT_DEBUG(iOwner->CaseMan(), AknFepPanic(EAknFepPanicNoCaseManager));

    iState = EEntry;
    iMatchState = EWordMatchNone;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    TPtiTextCase caseMode = ptiengine->Case();

    ptiengine->SetInputMode(EPtiEngineQwertyPredictive );    
    ptiengine->SetCase(caseMode);
    TBool isUnlimitedEditor = EFalse;
    ptiengine->SetMaxLengthForAutoCompletedCandidates(
              fepMan->EditorFreeSpaceForAutoWordCompletion(isUnlimitedEditor ));
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if(fepMan->IsAutoCompleteOn())
    {
	    TRAP_IGNORE(ptiengine->HandleCommandL( EPtiCommandEnableAutoCompletion ));
    }
    else
    {
	    TRAP_IGNORE(ptiengine->HandleCommandL( EPtiCommandDisableAutoCompletion ));
    }
    TPtiErrorCorrectionLevel correctionLevel = 
                    (TPtiErrorCorrectionLevel)fepMan->AdvancedPredictiveTypingCorrectionLevel();
    TRAP_IGNORE(ptiengine->HandleCommandL( EPtiCommandSetCorrectionLevel, 
                                          &correctionLevel ));	
	
    TPtiKeyboardType keyboard = fepMan->KeyboardLayout();
    ptiengine->SetKeyboardType(keyboard);
	
    TBool isNumberCandidateShown = fepMan->IsAdvancedPredictiveNumberCandidateShown();
    TRAP_IGNORE(ptiengine->HandleCommandL( EPtiCommandSetNumberCandidateStatus, 
                                          &isNumberCandidateShown ));
	
    TBool primaryCandidateState = fepMan->AdvancedPredictivePrimaryCandidate();
    TRAP_IGNORE(ptiengine->HandleCommandL( EPtiCommandSetProactiveCorrection, 
                                          &primaryCandidateState ));
#endif //RD_INTELLIGENT_TEXT_INPUT
    }


///////////////////////////////////////////////////////////////////////////////////////////////////
//
TBool TAknFepInputStateEntryQwertyWesternPredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
	if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
		{
		ptiengine->SetCase(EPtiCaseFnLower);
		}

    // Handle the Chr cycling
    if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) )
        {
        if ( iData && (iData != aKey) )
            {
            // Chr key is still pressed down but character key was changed from previous one
            // to new one. The previous character is committed and PtiEngine starts 
            // prosessing new key.
            ptiengine->CancelTimerActivity();
            
            if ( !fepMan->EditorHasFreeSpace() )
                {
                return EFalse;
                }
            }
        fepMan->SetFlag( CAknFepManager::EFlagInsideMultitapInlineEditingTransaction );
        }

    TBool response = EFalse;


    // Handle dead keys with space or backspace. These keys are not mapped in PtiEngine, so they cannot
    // be handled there.
    if ( ptiengine->HandleCommandL(EPtiCommandDeadKeyWaiting) == 1 )
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

    if (!response) // key not yet handled
        {
        switch (aKey)
            {
            case EStdKeySpace: //fall through
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
            // Selection key handle by AknFepManeger.
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
#ifdef RD_INTELLIGENT_TEXT_INPUT                    
                    //fepMan->HideExactWordPopUp();
                    // Select the exact match with the up arrow and lock it.
                    // Another alternative could be to call function DoPreviousMatchL() from here
                    TBool primaryCandidateState = fepMan->AdvancedPredictivePrimaryCandidate(); 
                    TInt wordIndexToSelect = 0;
            		if (EFalse == primaryCandidateState)
            			{
            			ptiengine->HandleCommandL( EPtiCommandUserActionGetIndexOfSecondaryCandidate, 
        							&wordIndexToSelect );
            			}
            			
            			if (fepMan->IsExactWordPopUpShown())
            			{
            			    fepMan->HideExactWordPopUp();			
				        	ptiengine->HandleCommandL( 
	                               EPtiCommandUserActionSetCurrentIndexOfCandidates, 
	                			   &wordIndexToSelect );
	                    	ptiengine->HandleCommandL( EPtiCommandLockCurrentCandidate );
	                    	TPtrC selectedWord = ptiengine->CurrentWord();
	                    	fepMan->UpdateInlineEditL( selectedWord, selectedWord.Length() ); 
	                    	response = ETrue;        							
	            			break;	
            			}
						else
						{
							fepMan->LaunchCandidatePopupListL( wordIndexToSelect );
						    response = ETrue;        							
	            			break;                 
						}

#endif // RD_INTELLIGENT_TEXT_INPUT            			
                    }
                    // fall through
            case EKeyDownArrow:
            case EKeyLeftArrow:
            case EKeyRightArrow:
            case EKeyBackspace:
                response = HandleNaviKeysL(aKey, aLength);
                break;
            
            
            default:
                HandleShiftState(aKey);
                response = HandleQwertyKeysL(aKey);
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
                if ( aLength == ELongKeyPress )
                    {
                    // Expire multitap timer after long press. 
                    ptiengine->CancelTimerActivity();
                    }
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
                break;
            }
        }
    
    iData = aKey; // store the pressed key for future reference
    return response;
    }


//////////////////////////////////////////////////////////////////////////////////
//
void TAknFepInputStateEntryQwertyWesternPredictive::HandleShiftState( TInt aKey )
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
    
    // For addition of ITI features. 
    // When FSQ is opened wiht ITI features enabled,
    // after pressing keys, don't need to re-set current text case by editor's text or FN key state.
    // Because FSQ's case doesn't obey editor's case and there is no fn key on FSQ.
    if ( fepMan->PluginUIManager() 
         && fepMan->PluginUIManager()->PluginInputMode() == EPluginInputModeFSQ
         && fepMan->WesternPredictive() )
        {
        return;
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
                // Thai language should not go to automatic capitalization in text case
                // mode and predictive input. This is because it would lead to the character 
                // mapped to upper case being displayed instead of the intended character.
                // So below is what is essentially a XNOR condition - 
                // 1) no-shiftkey and language with case    => upper case
                // 2) no-shiftkey and language without case => lower case
                // 3) shiftkey and language with case       => lower case
                // 4) shiftkey and language without case    => upper case
                TInt inputLang = ptiengine->CurrentLanguage()->LanguageCode();
                if ( ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ||
                       fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) )  
                      == ( inputLang == ELangThai ||
					  	   inputLang == ELangArabic ||
                           inputLang == ELangFarsi ||
                           inputLang == ELangHebrew ||
                           inputLang == ELangUrdu ||
                           inputLang == ELangHindi ) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }          
                }
            break;
            
            case EAknEditorLowerCase:
                {
                // Pressing shift enters uppercae letters in the "abc" mode
                if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ||
                     fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }                
                }
            break;
            
            case EAknEditorUpperCase:
                {
                // In the "ABC" mode (capslock mode) the letters are uppercased
                // unless the shift is pressed
                if ( !(fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)||
                      fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress)) )
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
        if ( (fnKeyState  == CAknFepFnKeyManager::EFnKeyNone) &&
             ( fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) ||
               fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ) )
            {
            shiftedCase = EPtiCaseUpper;
            }
        else if( (fnKeyState  == CAknFepFnKeyManager::EFnKeyNext) &&
                 (caseMan->CurrentCase() == EAknEditorTextCase ||
                  caseMan->CurrentCase() == EAknEditorUpperCase ||
                  fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) || 
                  fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)))
            {
            shiftedCase = EPtiCaseFnUpper;  
            }
        else if (( fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) || 
        		  fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)) && 
        		  (fnKeyState  != CAknFepFnKeyManager::EFnKeyNone))
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

TBool TAknFepInputStateEntryQwertyWesternPredictive::HandleWordBreakingKeysL(TInt aKey)
    {
    TBool retVal = EFalse; // in most cases we let also the upper levels handle these key events
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
    

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
                        // To fix the Suggested word is not committed when Arrow right key is pressed.
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
            	TBool wasInlineEditing = EFalse;	
            	wasInlineEditing = 
                          fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction);

                if(aKey == EStdKeySpace && tailLength > 0)
                    {
                    fepMan->RemoveSuggestedAdvanceCompletionL();
                    }
                	
            	ptiengine->CommitCurrentWord();
                fepMan->CommitInlineEditL();
                /* Temporary Fix */
                //EFlagInsideInlineEditingTransaction flag check is not required
                //but just to make sure.
                if (aKey == EStdKeySpace && fnKeyState == CAknFepFnKeyManager::EFnKeyDown)
                    fepMan->LaunchSelectModeMenuL();
                else if(aKey == EStdKeySpace && wasInlineEditing)
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
TBool TAknFepInputStateEntryQwertyWesternPredictive::HandleQwertyKeysL(TInt aKey)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    
    CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
    
    // Key is consumed if the PtiEngine maps it to some character
    TBuf<KKeyMappingsLength> keyMappings; 
    
    if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
		{
		
		// Try to know is there any mapping for function upper
		// char. For user point of view this changes has been made.
		if( ptiengine->Case() == EPtiCaseFnUpper )
			{
	    	ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseFnUpper);
			}
		else
			{
		   	ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseFnLower);			
			}
		}
	else
 		{
	    ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseLower);   
 		}
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
 		
 	// For user point of view this changes has been made.
 	// If key does not map to function upper char, then try to insert
 	// functionized char.
	if( (EPtiCaseFnUpper == ptiengine->Case()) && !keyIsMapped )
  		{
        ptiengine->SetCase((TPtiTextCase)EPtiCaseFnLower);
       	ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseFnLower);
        keyIsMapped = ( keyMappings.Length() > 0 );
 		}

    if ( keyIsMapped )
        {
        if ( !(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)) )
            { // 'new' inline edit                        
            ptiengine->ClearCurrentWord();
            if (caseMan->CurrentCase() == EAknEditorTextCase)
                {
                ptiengine->MarkAutoCapitalized();    
                }            
            
            fepMan->StartInlineEditL();
            }
                       
        TPtrC newText = ptiengine->AppendKeyPress((TPtiKey)aKey);
        fepMan->UpdateInlineEditL(newText, newText.Length());
        
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
           
        return ETrue;
        }
        // MT error fixing : If any key does not map any functionized char, 
        // its should not display any char fn mode, other behaviour as usual.
        // If the Fn key stae indicate that fn key pres & we are looking for functionnized
        // char, if its not availble it will do nothing. So we have to block the event
        // go to framework.
        if( aKey!=EStdKeyBackspace && fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
        	return ETrue;
        
        // Framework will handle the key event.
       	return EFalse;
    }
    

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
TBool TAknFepInputStateEntryQwertyWesternPredictive::HandleNaviKeysL(TInt aKey, TKeyPressLength /*aLength*/)
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
        TPtrC newText = ptiengine->DeleteKeyPress();
        TPtrC selectedWord = ptiengine->CurrentWord();
        fepMan->UpdateInlineEditL(selectedWord, selectedWord.Length());

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


/////////////////////////////////////////////////////////////////////////////
//
void TAknFepInputStateEntryQwertyWesternPredictive::LastWordInSelectionList()
    {

    }


//////////////////////////////////////////////////////////////////////////////
//
void TAknFepInputStateEntryQwertyWesternPredictive::FirstWordInSelectionList()
    {   
    }


/////////////////////////////////////////////////////////////////
//
void TAknFepInputStateEntryQwertyWesternPredictive::KeyTimerExpired()
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
   

// End of file
