/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryThaiPredictive methods.
*
*/












#include "AknFepUiInputStateEntryThaiPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepManager.h"
#include "AknFepPanic.h"
#include "AknFepCaseManager.h"

#include <PtiEngine.h>                             //CPtiEngine
#include <PtiDefs.h>                            //keys
#include <e32keys.h>
#include <aknfep.rsg>

TAknFepInputStateEntryThaiPredictive::
TAknFepInputStateEntryThaiPredictive(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateEntryWesternPredictive(aOwner)
    {
    //this class insists on the existence of a case manager
    __ASSERT_DEBUG(iOwner->CaseMan(), AknFepPanic(EAknFepPanicNoCaseManager));

    iState = EEntry;
    iMatchState = EWordMatchNone;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    ptiengine->SetInputMode(EPtiEnginePredictive);
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool isUnlimitedEditor = EFalse;
    ptiengine->SetMaxLengthForAutoCompletedCandidates(fepMan->EditorFreeSpaceForAutoWordCompletion(isUnlimitedEditor ));
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
	TRAP_IGNORE(ptiengine->HandleCommandL( EPtiCommandSetNumberCandidateStatus, &isNumberCandidateShown ));
#else
    ptiengine->ClearCurrentWord();
#ifdef RD_PRED_AW_COMPLETION
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    // To set the input mode
    if(fepMan->IsAutoCompleteOn())
        {
        ptiengine->SetInputMode(EPtiEngineWordCompletion);
        }
    else
#endif // RD_PRED_AW_COMPLETION
        {
        ptiengine->SetInputMode(EPtiEnginePredictive);
        }
#endif // RD_INTELLIGENT_TEXT_INPUT
    ptiengine->SetCase(EPtiCaseLower);
    }

TBool TAknFepInputStateEntryThaiPredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
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
    case EPtiKey0:
        response = HandleStandardNumericKeysL(aKey, aLength);
        break;
    case EPtiKeyStar:
        if ( aLength == ELongKeyPress )
            {
            response = HandleStarKeyL(aLength);
            }
        else
            {
            response = HandleStandardNumericKeysL(aKey, aLength);
            }
        break;
    case EKeyDownArrow:
        response = HandleDownKeyL();
        break;
    case EKeyUpArrow:
        response = HandleUpKeyL();
        break;
    case EKeyRightArrow: // fall through
    case EKeyLeftArrow: 
    case EKeyBackspace:
        response = HandleNaviKeysL(aKey, aLength);
        break;
    default:
        response = EFalse;
        break;
        }
    return response;
    }

TBool TAknFepInputStateEntryThaiPredictive::HandleDownKeyL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    
    if (!(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)))
        {
        return EFalse;
        }
    else
        {
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
            fepMan->UpdateInlineEditL(newText, 0);
            }
        }
    return ETrue;
    }

TBool TAknFepInputStateEntryThaiPredictive::HandleUpKeyL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if (!(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)))
        {
        return EFalse;
        }    
    
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    TInt lastCandidateIndex = ptiengine->NumberOfCandidates() -1;
    if(lastCandidateIndex >= 0)
        {
        // Get last word on list
        fepMan->LaunchCandidatePopupListL( lastCandidateIndex );        
        }
    else
        {
        TInt wordIndexToSelect = 0;
        // Get next word on list (wrap around if at end)
        ptiengine->HandleCommandL( EPtiCommandUserActionGetCurrentIndexOfCandidates, 
			&wordIndexToSelect );
        wordIndexToSelect < lastCandidateIndex ? wordIndexToSelect++ : wordIndexToSelect = 0;
        fepMan->LaunchCandidatePopupListL( wordIndexToSelect );        
        }                        	                        	    
	   
#else      
    if (!( (iMatchState == EWordMatchFirst) || !(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))))
        {
        if (iMatchState == EWordMatchLast || iMatchState == EWordMatchAfterLast)
            {
            fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_EMPTY_PREVIOUS);
            }
        iMatchState = EWordMatchStd; 
        TPtrC prevText = ptiengine->PreviousCandidate();
        fepMan->UpdateInlineEditL(prevText, 0);
        }
        
#endif //RD_INTELLIGENT_TEXT_INPUT
    return ETrue;
    }

// End of file
