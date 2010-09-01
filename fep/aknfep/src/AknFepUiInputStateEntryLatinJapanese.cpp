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
*       Provides the TAknFepUiInputStateEntryLatinJapanese methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateEntryLatinJapanese.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepCaseManager.h"               // CAknFepCaseManager
#include "AknFepManager.h"
#include "AknFepGlobalEnums.h"

#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface

// CONSTANTS

/**
 *  TAknFepUiInputStateEntryLatinJapanese class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryLatinJapanese::
//                                  TAknFepUiInputStateEntryLatinJapanese
// Default constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateEntryLatinJapanese::TAknFepUiInputStateEntryLatinJapanese(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateEntryJapaneseMultitap(aOwner, aUIContainer)
    {
    }

/**
 * MPtiObserver interface.
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryLatinJapanese::KeyTimerExpired
// Call this function when Multitap timer of PtiEngine was time-out
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateEntryLatinJapanese::KeyTimerExpired()
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->CompletedString();
    if (text.Length() > 0)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

        if (fepMan->IsSupportsSecretText()
         && fepMan->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
            {
            TRAP_IGNORE(fepMan->CommitInlineEditL());
            // Update case without Qwerty
            if (!iOwner->IsQwerty())
                {
                UpdateCase(text);
                }
            }
        else
            {
            iOwner->ChangeState(EMultiCompletion);
            // Update case without Qwerty
            if (!iOwner->IsQwerty())
                {
                UpdateCase(text);
                }

            // Commit a character
            TRAP_IGNORE(iOwner->FepMan()->CommitInlineEditL(text, text.Length()));

            // Update case with Qwerty
            if (iOwner->IsQwerty())
                {
                CAknFepCaseManager* caseMan = iOwner->CaseMan();
                caseMan->UpdateCase(ENullNaviEvent);
                }
            }
        }
    iData = 0;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    ptiengine->ClearCurrentWord();
    iOwner->ChangeState(EInitial);
   }

/**
*  TAknFepUiInputStateEntryLatinJapanese class.
*
*/
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryLatinJapanese::HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryLatinJapanese::HandleNumericShortKeyL(TInt aKey)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool ret = EFalse;

    if (iData && iData != aKey)
        {
        if (!iOwner->IsQwerty()
         && iOwner->FepMan()->IsSupportsSecretText()
         && fepMan->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
            {
            const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
            TPtrC text = data->ConvertingString();
            if (text.Length() > 0)
                {
                fepMan->CommitInlineEditL();
                if (!iOwner->IsQwerty())
                    {
                    UpdateCase(text);
                    }
                }
            ptiEngine->ClearCurrentWord();
            iOwner->ChangeState(EInitial);
            iData = 0;
            ret = EFalse;
            }
        else
            {
            ptiEngine->HandleCommandL(EPtiCommandUserActionComplete);
            iOwner->ChangeState(EMultiCompletion);
            }
        }
    else
        {
        iData = aKey;

        const MPtiEngineCompositionDataInterface* previousData = ptiEngine->CompositionData();
        TChar previousChar = 0;
        TPtrC previousText(previousData->ConvertingString());
        if (previousText.Length() > 0)
            {
            previousChar = previousText[0];
            }

        // Append a key to PtiEngine
        ptiEngine->AppendKeyPress((TPtiKey)aKey);

        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC text(data->ConvertingString());
        if (text.Length() > 0)
            {
            TChar ch = text[0];
            TBool validNewCandidate = fepMan->CharIsValidInEditor(ch);
            // Check if the character is valid for the editor.
            if ( !validNewCandidate )
                {
                TChar firstNewCandidate = ch;
                TBool goneThroughAllCandidates(EFalse);
                TPtrC8 sequence = ptiEngine->CurrentInputSequence();

                if ( sequence.Length() )
                    {
                    TPtiKey key = (TPtiKey)sequence[0];

                    // Go through candidates and check if valid characters are found.
                    do  {
                        TPtrC text = ptiEngine->AppendKeyPress(key);
                        ch = text[0];
                        goneThroughAllCandidates = (ch == firstNewCandidate);
                        validNewCandidate = fepMan->CharIsValidInEditor(ch);
                        }
                    while (!validNewCandidate && !goneThroughAllCandidates);
                    }

                if (!validNewCandidate)
                    {
                    // No valid characters at all.
                    ptiEngine->DeleteKeyPress();
                    iOwner->ChangeState(EMultiCompletion);
                    ptiEngine->HandleCommandL(EPtiCommandUserActionComplete);
                    iOwner->FepMan()->CommitInlineEditL();
                    ptiEngine->ClearCurrentWord();
                    iOwner->ChangeState(EInitial);
                    return ETrue;
                    }
                else if (ch == previousChar)
                    {
                    // The valid key loop count is 1.
                    // The current characters are committed, and send the same key again.
                    iOwner->ChangeState(EMultiCompletion);
                    ptiEngine->HandleCommandL(EPtiCommandUserActionComplete);
                    iOwner->FepMan()->CommitInlineEditL();
                    ptiEngine->ClearCurrentWord();
                    iOwner->ChangeState(EInitial);
                    iOwner->FepMan()->SimulateKeyEventL(aKey, ETrue);
                    return ETrue;
                    }
                }
            }

        ret = UpdateInlineEditingCharacterL();
        // Qwerty input without Secret input mode is continuing to Comleteion.
        if (ret && iOwner->IsQwerty())
            {
            if (iOwner->FepMan()->IsSupportsSecretText()
             || iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagPassNextKey))
                {
                iOwner->ChangeState(EMultiCompletion);
                ptiEngine->HandleCommandL(EPtiCommandUserActionComplete);
                iOwner->FepMan()->CommitInlineEditL();
                ptiEngine->ClearCurrentWord();
                iOwner->ChangeState(EInitial);
                ret = ETrue;
                }
            else
                {
                // Qwerty need to change status
                ptiEngine->HandleCommandL(EPtiCommandUserActionComplete);
                iOwner->ChangeState(EMultiCompletion);
                ret = EFalse;
                }
            }
        }
    return ret;
    }
// End of file
