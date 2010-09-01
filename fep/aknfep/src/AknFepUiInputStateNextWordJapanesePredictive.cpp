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
*       Provides the TAknFepUiInputStateNextWordJapanesePredictive methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateNextWordJapanesePredictive.h"
#include "AknFepUIManagerStateInterface.h"  // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       // MAknFepManagerUIInterface
#include "AknFepUICtrlContainerJapanese.h"  // MAknFepUICtrlContainerJapanese
#include "AknFepUICtrlCandidatePaneInterface.h"
                                       // MAknFepUICtrlCandidatePaneInterface
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"

#include <PtiDefs.h>                    // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>       // MPtiEngineCompositionDataInterface
#include <aknfep.rsg>


// CONSTANTS

/**
 *  TAknFepUiInputStateNextWordJapanesePredictive class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateNextWordJapanesePredictive::
//                              TAknFepUiInputStateNextWordJapanesePredictive
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateNextWordJapanesePredictive::
    TAknFepUiInputStateNextWordJapanesePredictive(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateJapanesePredictiveBase(aOwner, aUIContainer)
    {
    iState = ENextWord;
    iCbaResourceID = R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_SELECT;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateNextWordJapanesePredictive::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateNextWordJapanesePredictive::InitializeStateL()
    {
    CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
    iOwner->PtiEngine()->GetNextWordCandidateListL(*candidateArray);
    UpdateCandidateListL();
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateNextWordJapanesePredictive::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateNextWordJapanesePredictive::HandleCommandL(TInt aCommandId)
    {
    if (aCommandId == EAknFepAllCompletion)
        {
        iOwner->PtiEngine()->ClearCurrentWord();
        iOwner->ChangeState(EInitial);
        }
    else if (aCommandId == EAknFepSoftkeyCloseWindow)
        {
        CloseUI();
        iOwner->ChangeState(EInitial);
        }
    else if(aCommandId == EAknFepSoftkeySelectCandidate)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        fepMan->StartInlineEditL();
        iOwner->ChangeState(EMultiCompletion);
        }
    else if(aCommandId == EAknFepResourceChanged)
        {
        MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
        MAknFepUICtrlCandidatePaneInterface* predictivePane =
            uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                       EJapanesePredictiveCandidatePane);
        TInt index = predictivePane->CurrentItemIndexOfCandidate();
        predictivePane->HideWindow();
        UpdateCandidateListL(index);
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateNextWordJapanesePredictive::HandleNumericKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateNextWordJapanesePredictive::HandleNumericShortKeyL(TInt /*aKey*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);

    ptiEngine->ClearCurrentWord();
    predictivePane->HideWindow();
    iOwner->ChangeState(EInitial);
    // This State doesn't Handle numeric key, always return false.
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateNextWordJapanesePredictive::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateNextWordJapanesePredictive::HandleAuxKeyL(TInt aKey,
                                                                   TKeyPressLength /*aLength*/)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);
    TBool ret = EFalse;

    switch(aKey)
        {
        case EKeyOK:
        case EStdKeyDevice3:        /* 0xA7 */
        case EStdKeyEnter:          /* 0x03 */
        case EStdKeyNkpEnter:       /* 0x88 */
            {
            fepMan->StartInlineEditL();
            iOwner->ChangeState(EMultiCompletion);
            }
            break;
        case EKeyBackspace:
        case EStdKeyBackspace:      /* 0x01 */
        case EStdKeyEscape:         /* 0x04 */
            {
            ptiEngine->ClearCurrentWord();
            predictivePane->HideWindow();
            iOwner->ChangeState(EInitial);
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
// End of file
