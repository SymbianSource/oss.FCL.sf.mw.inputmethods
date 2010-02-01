/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepUiInputStateCandidateRetroActive methods.
*
*/












// INCLUDE FILES


#include "AknFepUiInputStateCandidateRetroActive.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepUICtrlContainerJapanese.h"   // MAknFepUICtrlContainerJapanese
#include "AknFepUICtrlCandidatePaneInterface.h"
                                        // MAknFepUICtrlCandidatePaneInterface
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"

#include <PtiDefs.h>                    // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>       // MPtiEngineCompositionDataInterfac
#include <avkon.rsg>
#include <AknFep.rsg>


// CONSTANTS

/**
 *  TAknFepUiInputStateCandidateRetroActive class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateRetroActive::
//                                  TAknFepUiInputStateCandidateRetroActive
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidateRetroActive::TAknFepUiInputStateCandidateRetroActive(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerJapanese* aUIContainer)

    :TAknFepUiInputStateEntryJapaneseMultitap(aOwner, aUIContainer)
    {
    iState = ECandidateRetroActive;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateRetroActive::InitializeStateL()
    {
    iOwner->SetInlineEditUnderlineVisibility(ETrue);

    ShowListL(EAknFepSoftkeyRetroActiveHalf);
    iData = EAknFepSoftkeyRetroActiveHalf;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateRetroActive::CloseUI
// Closing UI
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateRetroActive::CloseUI()
    {
    // commit the current item
    TRAP_IGNORE(DoCommitItemL());
    // Move the status to EInitial.
    iOwner->PtiEngine()->ClearCurrentWord();
    iOwner->ChangeState(EInitial);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateRetroActive::HandleNumericKeyL
// Handling short keypress events of the numeric and the star
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateRetroActive::HandleNumericShortKeyL(TInt /*aKey*/)
    {
    // This state doesn't handle numeric key.
    // commit the current item
    DoCommitItemL();
    // Move the status to EInitial.
    iOwner->PtiEngine()->ClearCurrentWord();
    iOwner->ChangeState(EInitial);
    // Return EFalse, because of going through the event
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateRetroActive::HandleNaviKeyL
// Handling Navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateRetroActive::HandleNaviKeyL(TInt aKey,
                                                                TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;
    MAknFepUICtrlCandidatePaneInterface* candidatePane =
        UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                     EJapaneseCandidatePane);
    TAknFepUICtrlScroll direction = EScrollNone;

    switch(aKey)
        {
        case EKeyDownArrow:
        case EKeyUpArrow:
        case EStdKeyDownArrow:      /* 0x11 */
        case EStdKeyUpArrow:        /* 0x10 */
        case EStdKeySpace:
            { // The highlight of candidate is moved up/down
            direction = EScrollDown;
            if (aKey == EKeyUpArrow || aKey == EStdKeyUpArrow)
                {
                direction = EScrollUp;
                }

            candidatePane->MoveItem(direction);
            ret = ETrue;
            }
            break;
        case EKeyLeftArrow:
        case EKeyRightArrow:
        case EStdKeyLeftArrow:      /* 0x0e */
        case EStdKeyRightArrow:     /* 0x0f */
            ret = ETrue;
            break;
        default:
            break;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateRetroActive::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateRetroActive::HandleAuxKeyL(TInt aKey,
                                                               TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlCandidatePaneInterface* candidatePane =
        UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                     EJapaneseCandidatePane);
    TInt currentIdex = candidatePane->CurrentItemIndexOfCandidate();
    candidatePane->HideWindow();
    switch(aKey)
        {
        case EKeyOK:
        case EStdKeyDevice3:        /* 0xA7 */
        case EStdKeyEnter:          /* 0x03 */
        case EStdKeyNkpEnter:       /* 0x88 */
            {
            DoSelectItemL();
            ret = ETrue;
            }
            break;
        case EKeyBackspace:
        case EStdKeyBackspace:      /* 0x01 */
        case EStdKeyEscape:         /* 0x04 */
            {
            fepMan->SetInlineEditingCursorVisibilityL(ETrue);
            ptiEngine->HandleCommandL(EPtiCommandUserActionCancel);
            iOwner->SetInlineEditUnderlineVisibility(ETrue);

            if(iOwner->IsPredictive())
                {
                iOwner->ChangeState(EPredictiveInput);
                ShowPredictiveListL();
                }
            else
                {
                UpdateInlineEditingCharacterL();
                iOwner->ChangeState(EEntry);
                }
            ret = ETrue;
            }
            break;
      default: // Panic
            {
            AknFepPanic(EAknFepPanicNotSupportKey);
            }
            break;
        }
    return ret;
    }

void TAknFepUiInputStateCandidateRetroActive::HandleCommandL(TInt aCommandId)
    {
    // Close current popup window
    MAknFepUICtrlCandidatePaneInterface*
        candidatePane = UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                                     EJapaneseCandidatePane);
    switch (aCommandId)
        {
        case EAknFepSoftkeyCommit:
            candidatePane->HideWindow();
            DoSelectItemL();
            break;
        case EAknFepSoftkeyRetroActiveHalf:
        case EAknFepSoftkeyRetroActiveFull:
            {
            candidatePane->HideWindow();
            // Change from half to full or from full to half
            ShowListL(aCommandId);
            iData = aCommandId;
            }
            break;
        case EAknFepResourceChanged:
            {
            TInt currentIdex = candidatePane->CurrentItemIndexOfCandidate();
            candidatePane->HideWindow();
            // Change from half to full or from full to half
            ShowListL(iData, currentIdex);
            break;
            }
        default:
            // Go through the event to base class.
            TAknFepUiInputStateEntryJapaneseMultitap::HandleCommandL(aCommandId);
            break;
        }
    }

void TAknFepUiInputStateCandidateRetroActive::ShowListL(TInt aCommandId, TInt aIndex /*=0*/)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
    MAknFepUICtrlCandidatePaneInterface*
        candidatePane = UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                                     EJapaneseCandidatePane);
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    // Check ComposirionData
    __ASSERT_DEBUG( data->CompletedString().Length() < 0,
        RDebug::Print(
           _L("In TAknFepUiInputStateCandidateRetroActive::TAknFepUiInputStateCandidateRetroActive INVALID DATA: ConvertingString!")));

    // Get coordinate of input area
    TPoint  leftSideOfBaseLine;
    TInt    height;
    TInt    ascent;
    TInt    documentOffset = data->CursorPosition();
    fepMan->GetScreenCoordinatesL(leftSideOfBaseLine, height, ascent, documentOffset);

    // Decide either half or full
    TPtiEngineCommand command = EPtiCommandUserActionRetroActiveCandidateHalf;
    TInt resId = R_AKNFEP_SOFTKEYS_RETROACTIVE_FULL_WIDTH;
    if (aCommandId == EAknFepSoftkeyRetroActiveFull)
        {
        command = EPtiCommandUserActionRetroActiveCandidateFull;
        resId = R_AKNFEP_SOFTKEYS_RETROACTIVE_HALF_WIDTH;
        }

    // Get the showing candidates
    ptiEngine->HandleCommandL(command, (TAny*)candidateArray);
    if (candidateArray->Count() > 0)
        {
        // Show the retroactive popup
        candidatePane->SetWindowPosition(leftSideOfBaseLine, height, ascent);
        candidatePane->ShowCandidateListL(candidateArray, aIndex, resId);
        }
    else
        {
        // return to Entry state
        iOwner->ChangeState(EEntry);
        }
    }

void TAknFepUiInputStateCandidateRetroActive::ShowPredictiveListL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    // normal operation
    if (UpdateInlineEditingCharacterL())
        {
        // Get the candidate array object
        CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
        // Set the candidates to array on PtiEngine
        ptiEngine->GetCandidateListL(*candidateArray);

        MAknFepUICtrlCandidatePaneInterface* predictivePane =
            UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                       EJapanesePredictiveCandidatePane);
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        // Get coordinate of input area
        TPoint  leftSideOfBaseLine;
        TInt    height;
        TInt    ascent;
        TInt    documentOffset = data->CursorPosition() > 0 ? data->CursorPosition() : 0;

        fepMan->GetScreenCoordinatesL(leftSideOfBaseLine,
                                      height,
                                      ascent,
                                      documentOffset);
        // show candidate pop-up list
        predictivePane->SetWindowPosition(leftSideOfBaseLine, height, ascent);
        predictivePane->ShowCandidateListL(candidateArray, 0, iCbaResourceID);
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateRetroActive::DoSelectItemL
// Select a current item, and commit it.
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateRetroActive::DoSelectItemL()
    {
    // commit the current item
    DoCommitItemL();
    if (iOwner->FepMan()->EditorHasFreeSpace()
     && iOwner->IsPredictive())
        {
        iOwner->ChangeState(ENextWord);
        }
    else
        {
        iOwner->PtiEngine()->ClearCurrentWord();
        iOwner->ChangeState(EInitial);
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateRetroActive::DoCommitItemL
// Commit a current item
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateRetroActive::DoCommitItemL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlCandidatePaneInterface* candidatePane =
        UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                     EJapaneseCandidatePane);
    TInt currentIdex = candidatePane->CurrentItemIndexOfCandidate();
    candidatePane->HideWindow();

    iOwner->ChangeState(EMultiCompletion);
    CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
    TPtrC ptr((*candidateArray)[currentIdex]);
    ptiEngine->HandleCommandL(EPtiCommandUserActionRetroActiveCommit, &ptr);
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->CompletedString();
    if (text.Length() > 0)
        {
        fepMan->CommitInlineEditL(text, text.Length());
        }
    }

// End of file
