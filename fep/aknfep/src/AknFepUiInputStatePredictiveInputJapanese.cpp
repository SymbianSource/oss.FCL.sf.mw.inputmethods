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
* Description:            Provides the
*                TAknFepUiInputStatePredictiveInputJapanese methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStatePredictiveInputJapanese.h"
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
#include <AknFep.rsg>
#include <avkon.rsg>

// CONSTANTS
const TText KAknFEPLineFeedSymbol = 0x21B2;
const TText KAknFEPMirroredLineFeedSymbol = 0x21B3;

/**
 *  TAknFepUiInputStatePredictiveInputJapanese class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::
//                      TAknFepUiInputStatePredictiveInputJapanese
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStatePredictiveInputJapanese::
    TAknFepUiInputStatePredictiveInputJapanese(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateJapanesePredictiveBase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;
    iCbaResourceID = R_AKNFEP_SOFTKEYS_CONVERT_OTHER_SELECT;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::KeyTimerExpired
// Call this function when Multitap timer of PtiEngine was time-out
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStatePredictiveInputJapanese::KeyTimerExpired()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();

    TPtrC text = data->ConvertingString();
    TInt cursorpos = data->CursorPosition();
    if (text.Length() > 0)
        {
        if (text[0] == KAknFEPLineFeedSymbol
         || text[0] == KAknFEPMirroredLineFeedSymbol)
            {
            iOwner->ChangeState(EInitial);
            }
        else
            {
            // Set on cursor visibility in inline editing
            TRAPD(err, fepMan->SetInlineEditingCursorVisibilityL(ETrue));
            if (err == KErrNone)
                {
                TRAP(err, fepMan->UpdateInlineEditL(text, cursorpos));
                }
            if (err != KErrNone)
                {
                TRAP_IGNORE(iOwner->FepMan()->SimulateKeyEventL(EKeyEscape, ETrue));
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStatePredictiveInputJapanese::InitializeStateL()
    {
    iOwner->SetInlineEditUnderlineVisibility(ETrue);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStatePredictiveInputJapanese::
    HandleCommandL(TInt aCommandId)
    {
    switch (aCommandId)
        {
        case EAknFepSoftkeySelectCandidate:
            {
            iOwner->FepMan()->UpdateCbaL(NULL);
            CPtiEngine* ptiEngine = iOwner->PtiEngine();
            const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
            TPtrC textConv = data->ConvertingString();
            TPtrC textComp = data->CompletedString();
            if (textConv.Length() > 0 || textComp.Length() > 0)
                {
                iOwner->ChangeState(EMultiCompletion);
                }
            else
                {
                iOwner->ChangeState(EInitial);
                }
            }
            break;
        case EAknFepSoftkeyConvert:
            {
            // change state to conversion
            ChangeStateToConversionL();
            }
            break;
        case EAknFepSoftkeyOtherModes:
            {
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            CPtiEngine* ptiEngine = iOwner->PtiEngine();
            MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
            MAknFepUICtrlCandidatePaneInterface* predictivePane =
                uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                           EJapanesePredictiveCandidatePane);
            // Closeing candidate pop-up list
            predictivePane->HideWindow();
            // Update of CBA
            iOwner->FepMan()->UpdateCbaL(NULL);
            // Delete Cursor
            fepMan->SetInlineEditingCursorVisibilityL(EFalse);
            // Change the status to Retroactive input
            iOwner->ChangeState(ECandidateRetroActive);
            }
            break;
        case EAknFepSoftkeyPredictiveOn:
            {
            CPtiEngine* ptiEngine = iOwner->PtiEngine();
            // normal operation
            if (UpdateInlineEditingCharacterL())
                {
                CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
                ptiEngine->GetCandidateListL(*candidateArray);
                UpdateCandidateListL();
                }
            }
            break;
        case EAknFepResourceChanged:
            {
            MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
            MAknFepUICtrlCandidatePaneInterface* predictivePane =
                uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                           EJapanesePredictiveCandidatePane);
            TInt index = predictivePane->CurrentItemIndexOfCandidate();
            predictivePane->HideWindow();
            UpdateCandidateListL(index);
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::HandleNumericShortKeyL
// Handling numeric keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStatePredictiveInputJapanese::
    HandleNumericShortKeyL(TInt aKey)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool cursornisibility = EFalse;
    TBool inputcontinue = ETrue;

    if (aKey == EStdKeySpace)
        {
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC text = data->ConvertingString();
        if (text.Length() > 0)
            {
            HandleCommandL(EAknFepSoftkeyConvert);
            inputcontinue = EFalse;
            }
        }

    if (inputcontinue)
        {
        if (aKey == EPtiKeyStar)
            {
            cursornisibility = ETrue;
            }
        // Setting Cursor visibility
        fepMan->SetInlineEditingCursorVisibilityL(cursornisibility);

        // To send a key to PtiEngine
        // Notice: AppendKeyPress() must be called after SetInlineEditingCursorVisibilityL().
        ptiEngine->AppendKeyPress((TPtiKey)aKey);

        // this process run if completion character (line feed or space) exist.
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC text = data->CompletedString();
        if (text.Length() > 0)
            {
            CloseUI();
            return ETrue;
            }
        // normal operation
        if (UpdateInlineEditingCharacterL())
            {
            // for Qwerty input, if including latin characters,
            // close candidate list.
            if (!iOwner->IsQwerty()
             || !IsFullWidthAlphaChar(data->ConvertingString()))
                {
                CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
                ptiEngine->GetCandidateListL(*candidateArray);
                UpdateCandidateListL();
                }
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStatePredictiveInputJapanese::HandleAuxKeyL(TInt aKey,
                                                                TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();

    switch(aKey)
        {
        case EKeyOK:
        case EStdKeyDevice3:        /* 0xA7 */
        case EStdKeyEnter:          /* 0x03 */
        case EStdKeyNkpEnter:       /* 0x88 */
            {
            iOwner->FepMan()->UpdateCbaL(NULL);
            iOwner->ChangeState(EMultiCompletion);
            ret = EFalse;
            }
            break;
        case EKeyBackspace:
        case EStdKeyBackspace:      /* 0x01 */
        case EStdKeyEscape:         /* 0x04 */
            {
            fepMan->SetInlineEditingCursorVisibilityL(ETrue);
            ptiEngine->DeleteKeyPress();

            if (UpdateInlineEditingCharacterL())
                {
                // get composition data.
                const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
                // for Qwerty input, if including latin characters,
                // close candidate list.
                if (!iOwner->IsQwerty()
                 || !IsFullWidthAlphaChar(data->ConvertingString()))
                    {
                    CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
                    ptiEngine->GetCandidateListL(*candidateArray);
                    UpdateCandidateListL();
                    }
                else
                    {
                    // Update of CBA
                    TInt newResource = R_AVKON_SOFTKEYS_EMPTY;
                    fepMan->UpdateCbaL(newResource);
                    }
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

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapanesePredictiveBase::HandleNaviKeyL
// Handling Navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStatePredictiveInputJapanese::
  HandleNaviKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = EFalse;
    switch(aKey)
        {
        case EKeyDownArrow:
        case EStdKeyDownArrow:      /* 0x11 */
        case EKeyUpArrow:
        case EStdKeyUpArrow:        /* 0x10 */
        case EKeyLeftArrow:
        case EStdKeyLeftArrow:      /* 0x0e */
            {
            CPtiEngine* ptiEngine = iOwner->PtiEngine();
            MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
            MAknFepUICtrlCandidatePaneInterface* predictivePane =
                uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                           EJapanesePredictiveCandidatePane);
            if (predictivePane->CurrentItemIndexOfCandidate() == 0
             && ptiEngine->NumberOfCandidates() == 1)
                {
                // change state to entry
                ChangeStateToEntryL();
                // go through the key event.
                return EFalse;
                }
            }
            break;
        default:
            break;
        }
    if (!ret)
        {
        ret = TAknFepUiInputStateJapanesePredictiveBase::HandleNaviKeyL(aKey, aLength);
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::IsFullWidthAlphaChar
// Check full-width alphabet character
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStatePredictiveInputJapanese::IsFullWidthAlphaChar(TPtrC aPtr)
    {
    const TUint KFullAlphaLargeA = 0xFF21;
    const TUint KFullAlphaLargeZ = 0xFF3A;
    const TUint KFullAlphaSmallA = 0xFF41;
    const TUint KFullAlphaSmallZ = 0xFF5A;

    TBool isAlpha = EFalse;
    for (TInt i=aPtr.Length()-1; i >= 0 && !isAlpha; i--)
        {
        TUint ch = aPtr[i];
        isAlpha = (KFullAlphaLargeA <= ch && ch <= KFullAlphaLargeZ)
                 || (KFullAlphaSmallA <= ch && ch <= KFullAlphaSmallZ);
        }
    return isAlpha;
    }

/**
 * TAknFepUiInputStatePredictiveInputJapanese::ChangeStateToConversionL();
 * Change state to conversion
 */
void TAknFepUiInputStatePredictiveInputJapanese::ChangeStateToConversionL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);
    // Closeing candidate pop-up list
    predictivePane->HideWindow();
    // Update of CBA
    iOwner->FepMan()->UpdateCbaL(NULL);
    // Delete Cursor
    fepMan->SetInlineEditingCursorVisibilityL(EFalse);

    // Update of inline editing characters
    ptiEngine->HandleCommandL(EPtiCommandUserActionConvert);
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->CompletedString();
    TPtrC text2 = data->ConvertingString();
    if (text.Length() >0)
        {
        iOwner->ChangeState(EMultiCompletion);
        fepMan->CommitInlineEditL(text, text.Length());
        if (fepMan->EditorHasFreeSpace())
            {
            iOwner->ChangeState(ENextWord);
            }
        else
            {
            CloseUI();
            }
        }
    else if (text2.Length() > 0)
        {
        // Change to ConversionState
        iOwner->ChangeState(EConversion);
        fepMan->UpdateInlineEditL(text2, text2.Length());
        }
    }

/**
 * TAknFepUiInputStatePredictiveInputJapanese::ChangeStateToEntryL();
 * Change state to Entry
 */
void TAknFepUiInputStatePredictiveInputJapanese::ChangeStateToEntryL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);
    // Closeing candidate pop-up list
    predictivePane->HideWindow();
    // Update of CBA
    iOwner->FepMan()->UpdateCbaL(NULL);
    // Delete Cursor
    fepMan->SetInlineEditingCursorVisibilityL(EFalse);

    // Change to EntryState
    iOwner->ChangeState(EEntry);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::CloseUI
// Closing UI
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStatePredictiveInputJapanese::CloseUI()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);

    const MPtiEngineCompositionDataInterface* currentdata = ptiEngine->CompositionData();
    if (currentdata->ConvertingString().Length() >0)
        {
        TInt index = predictivePane->CurrentItemIndexOfCandidate();
        if (index > -1)
            {
            TRAP_IGNORE( ptiEngine->HandleCommandL(EPtiCommandUserActionCompleteCandidate, &index));
            }
        else
            {
            TRAP_IGNORE( ptiEngine->HandleCommandL(EPtiCommandUserActionComplete));
            }
        }
    const MPtiEngineCompositionDataInterface* commitdata = ptiEngine->CompositionData();
    TPtrC text = commitdata->CompletedString();
    if (text.Length() >0)
        {
        iOwner->ChangeState(EMultiCompletion);
        TRAP_IGNORE( fepMan->CommitInlineEditL(text, text.Length()));
        }

    ptiEngine->ClearCurrentWord();
    // Closing candidate pop-up list
    predictivePane->HideWindow();
    iOwner->ChangeState(EInitial);
    }

// End of file
