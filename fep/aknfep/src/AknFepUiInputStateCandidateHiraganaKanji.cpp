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
*       Provides the TAknFepUiInputStateCandidateHiraganaKanji methods.
*
*/












// INCLUDE FILES


#include "AknFepUiInputStateCandidateHiraganaKanji.h"
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
#include <AknFep.rsg>

// CONSTANTS

/**
 *  TAknFepUiInputStateCandidateHiraganaKanji class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateHiraganaKanji::
//                                  TAknFepUiInputStateCandidateHiraganaKanji
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidateHiraganaKanji::TAknFepUiInputStateCandidateHiraganaKanji(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerJapanese* aUIContainer)

    :TAknFepUiInputStateEntryJapaneseMultitap(aOwner, aUIContainer)
    {
    iState = ECandidate;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateHiraganaKanji::InitializeStateL()
    {
    iOwner->SetInlineEditUnderlineVisibility(ETrue);

    // Get index number to show at first
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TInt index;
    CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
    ptiEngine->GetCandidateListL(*candidateArray);
    ptiEngine->HandleCommandL(EPtiCommandUserActionGetCurrentIndexOfCandidates, &index);

    UpdateCandidateListL(index);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateHiraganaKanji::CloseUI
// Closing UI
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateHiraganaKanji::CloseUI()
    {
    UIContainer()->
        CandidatePane(MAknFepUICtrlContainerJapanese::EJapaneseCandidatePane)->
            HideWindow();
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateHiraganaKanji::HandleNumericKeyL
// Handling short keypress events of the numeric and the star
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateHiraganaKanji::HandleNumericShortKeyL(TInt /*aKey*/)
    {
    // This state doesn't handle numeric key.
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    ptiEngine->HandleCommandL(EPtiCommandUserActionAllComplete);
    CloseUI();
    iOwner->ChangeState(EMultiCompletion);
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateHiraganaKanji::HandleNaviKeyL
// Handling Navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateHiraganaKanji::HandleNaviKeyL(TInt aKey,
                                                                TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlCandidatePaneInterface* candidatePane =
        UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                     EJapaneseCandidatePane);
    TAknFepUICtrlScroll direction = EScrollNone;
    TPtiEngineCommand ptiCommand = EPtiCommandNone;

    switch(aKey)
        {
        case EKeyLeftArrow:
        case EKeyRightArrow:
        case EStdKeyLeftArrow:      /* 0x0e */
        case EStdKeyRightArrow:     /* 0x0f */
            { // Change Conversion state
            CloseUI();
            iOwner->ChangeState(EConversion);
            ret = EFalse;
            }
            break;
        case EKeyDownArrow:
        case EKeyUpArrow:
        case EStdKeyDownArrow:      /* 0x11 */
        case EStdKeyUpArrow:        /* 0x10 */
        case EStdKeySpace:
            { // The highlight of candidate is moved up/down
            // aKey == EKeyDownArrow
            ptiCommand = EPtiCommandUserActionConvert;
            direction = EScrollDown;
            // aKey == EKeyUpArrow
            if (aKey == EKeyUpArrow || aKey == EStdKeyUpArrow)
                {
                ptiCommand = EPtiCommandUserActionPrevious;
                direction = EScrollUp;
                }

            ptiEngine->HandleCommandL(ptiCommand);
            UpdateInlineEditingCharacterL();
            candidatePane->MoveItem(direction);
            ret = ETrue;
            }
            break;
        default:
            break;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateHiraganaKanji::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateHiraganaKanji::HandleAuxKeyL(TInt aKey,
                                                               TKeyPressLength /*aLength*/)
    {
    MAknFepUICtrlCandidatePaneInterface* candidatePane =
        UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                     EJapaneseCandidatePane);
    candidatePane->HideWindow();
    switch(aKey)
        {
        case EKeyOK:
        case EStdKeyDevice3:        /* 0xA7 */
        case EStdKeyEnter:          /* 0x03 */
        case EStdKeyNkpEnter:       /* 0x88 */
            {
            iOwner->ChangeState(EMultiCompletion);
            }
            break;
        case EKeyBackspace:
        case EStdKeyBackspace:      /* 0x01 */
        case EStdKeyEscape:         /* 0x04 */
            {
            iOwner->ChangeState(EConversion);
            }
            break;
      default: // Panic
            {
            AknFepPanic(EAknFepPanicNotSupportKey);
            }
            break;
        }
    return EFalse;
    }


// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateHiraganaKanji::UpdateCandidateListL
// Update candidate list
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateHiraganaKanji::UpdateCandidateListL(TInt aIndex /*=0*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
    MAknFepUICtrlCandidatePaneInterface* candidatePane =
        UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                     EJapaneseCandidatePane);
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    // Check ComposirionData
    __ASSERT_DEBUG( data->CompletedString().Length() < 0,
        RDebug::Print(
           _L("In TAknFepUiInputStateCandidateHiraganaKanji::TAknFepUiInputStateCandidateHiraganaKanji INVALID DATA: ConvertingString!")));

    // Get coordinate of input area
    TPoint  leftSideOfBaseLine;
    TInt    height;
    TInt    ascent;
    TInt    documentOffset = data->CursorPosition();
    if (data->NumberOfPhrase() > 0)
        {
        TPtrC8 clauseChar = data->LengthOfPhrase();
        documentOffset = clauseChar[0];
        }
    fepMan->GetScreenCoordinatesL(leftSideOfBaseLine, height, ascent, documentOffset);

    // The showing candidate pop-up list
    candidatePane->SetWindowPosition(leftSideOfBaseLine, height, ascent);
    candidatePane->ShowCandidateListL(candidateArray, aIndex, 
                                      R_AKNFEP_SOFTKEYS_EMPTY_EMPTY_COMMIT);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateHiraganaKanji::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateHiraganaKanji::HandleCommandL(TInt aCommandId)
    {
    if(aCommandId == EAknFepResourceChanged)
        {
        // Get the current index number
        CPtiEngine* ptiEngine = iOwner->PtiEngine();
        CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
        TInt index;
        ptiEngine->GetCandidateListL(*candidateArray);
        ptiEngine->HandleCommandL(EPtiCommandUserActionGetCurrentIndexOfCandidates, &index);

        CloseUI();
        UpdateCandidateListL(index);
        }
    else
        {
        // Go through the event to base class.
        TAknFepUiInputStateEntryJapaneseMultitap::HandleCommandL(aCommandId);
        }
    }
// End of file
