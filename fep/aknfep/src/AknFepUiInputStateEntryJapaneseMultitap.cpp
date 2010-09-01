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
*       Provides the TAknFepUiInputStateEntryJapaneseMultitap methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateEntryJapaneseMultitap.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"
#include "AknFepManager.h"
#include "AknFepUICtrlCandidatePaneInterface.h" // MAknFepUICtrlCandidatePaneInterface

#include <PtiDefs.h>                    // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>       // MPtiEngineCompositionDataInterface

// CONSTANTS
const TText KAknFEPLineFeedSymbol = 0x21B2;
const TText KAknFEPMirroredLineFeedSymbol = 0x21B3;

/**
 *  TAknFepUiInputStateEntryJapaneseMultitap class.
 *
 */

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::
//                                  TAknFepUiInputStateEntryJapaneseMultitap
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateEntryJapaneseMultitap::
    TAknFepUiInputStateEntryJapaneseMultitap(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateJapaneseBase(aOwner, aUIContainer)
    {
    iState = EEntry;
    }

/**
 * MPtiObserver interface.
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::KeyTimerExpired
// Call this function when Multitap timer of PtiEngine was time-out
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateEntryJapaneseMultitap::KeyTimerExpired()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->CompletedString();

    if (text.Length() > 0)
        {
        iOwner->ChangeState(EMultiCompletion);
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
        TRAP_IGNORE(fepMan->CommitInlineEditL());
        iOwner->ChangeState(EInitial);
        }
    else
        {
        TPtrC text2 = data->ConvertingString();
        TInt cursorpos = data->CursorPosition();
        if (text2.Length() > 0)
            {
            if (text2[0] == KAknFEPLineFeedSymbol
             || text2[0] == KAknFEPMirroredLineFeedSymbol)
                {
                iOwner->ChangeState(EInitial);
                }
            else
                {
                // Set on cursor visibility in inline editing
                TRAPD(err, fepMan->SetInlineEditingCursorVisibilityL(ETrue));
                if (err == KErrNone)
                    {
                    TRAP(err, fepMan->UpdateInlineEditL(text2, cursorpos));
                    }
                if (err != KErrNone)
                    {
                    TRAP_IGNORE(iOwner->FepMan()->SimulateKeyEventL(EKeyEscape, ETrue));
                    }
                }
            }
        }
    }

/**
 *  TAknFepUiInputStateEntryJapaneseMultitap class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateEntryJapaneseMultitap::InitializeStateL()
    {
    iOwner->SetInlineEditUnderlineVisibility(EFalse);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::HandleNumericShortKeyL
// HHandling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryJapaneseMultitap::HandleNumericShortKeyL(TInt aKey)
    {
    TBool cursorVisibility = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;
    if (aKey == EPtiKeyStar)
        {
        cursorVisibility = ETrue;
        }
    fepMan->SetInlineEditingCursorVisibilityL(cursorVisibility);
    ptiEngine->AppendKeyPress((TPtiKey)aKey);
    ret = UpdateInlineEditingCharacterL();
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::HandleNumericLongKeyL
// Handling Numeric long keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryJapaneseMultitap::HandleNumericLongKeyL(TInt /*aKey*/)
    {
    iOwner->ChangeState(EMultiCompletion);
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::HandleNaviKeyL
// Handling navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryJapaneseMultitap::HandleNaviKeyL(TInt /*aKey*/,
                                                               TKeyPressLength /*aLength*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;

    ptiEngine->HandleCommandL(EPtiCommandUserActionComplete);
    ret = UpdateInlineEditingCharacterL();

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryJapaneseMultitap::HandleAuxKeyL(TInt aKey,
                                                              TKeyPressLength /*aLength*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;

    switch(aKey)
        {
        case EKeyBackspace:
        case EStdKeyBackspace:      /* 0x01 */
            {
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->SetInlineEditingCursorVisibilityL(ETrue);
            ptiEngine->DeleteKeyPress();
            ret = UpdateInlineEditingCharacterL();
            }
            break;
        case EKeyOK:
        case EStdKeyDevice3:        /* 0xA7 */
        case EStdKeyEnter:          /* 0x03 */
        case EStdKeyNkpEnter:       /* 0x88 */
            {
            iOwner->ChangeState(EMultiCompletion);
            }
            break;
        default:
            break;
        }


    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::UpdateInlineEditingCharacterL
// Update inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryJapaneseMultitap::UpdateInlineEditingCharacterL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;

    if (!CommitInlineEditingCharacterL())
        {
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC text = data->ConvertingString();
        TInt cursorpos = data->CursorPosition();
        if (text.Length() > 0)
            {
            if (State() == EConversion
             || State() == ECandidate
             || State() == ECandidateRetroActive
             || State() == EPredictiveInput)
            	{
	            if (data->NumberOfPhrase() > 0)
	                {
	                TPtrC8 clauseChar = data->LengthOfPhrase();
	                cursorpos = clauseChar[0];
	                }
            	}
            fepMan->UpdateInlineEditL(text, cursorpos);
            }
        else
            {
            __ASSERT_DEBUG(text.Length() == 0,
                RDebug::Print(
                   _L("In TAknFepUiInputStateEntryJapaneseMultitap::UpdateInlineEditingCharacterL,\nINVALID DATA: ConvertingString. Roll back to initial State!")));
            ptiEngine->ClearCurrentWord();
            fepMan->CancelInlineEdit();
            iOwner->ChangeState(EInitial);
            }
        ret = ETrue;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryJapaneseMultitap::CommitInlineEditingCharacterL
// Commit inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryJapaneseMultitap::CommitInlineEditingCharacterL()
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->CompletedString();
    if (text.Length() > 0)
        {
        iOwner->ChangeState(EMultiCompletion);
        ret = ETrue;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStatePredictiveInputJapanese::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateEntryJapaneseMultitap::
    HandleCommandL(TInt aCommandId)
    {
    if(aCommandId == EAknFepSoftkeyCommit)
        {
        CPtiEngine* ptiEngine = iOwner->PtiEngine();
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC textConv = data->ConvertingString();
        TPtrC textComp = data->CompletedString();
        MAknFepUICtrlCandidatePaneInterface* candidatePane =
            UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                         EJapaneseCandidatePane);
        TInt curr = candidatePane->CurrentItemIndexOfCandidate();
        if (curr > -1)
            {
            TInt old = 0;
            ptiEngine->HandleCommandL(EPtiCommandUserActionGetCurrentIndexOfCandidates, &old);
            while(curr > old)
                {
                ++old;
                ptiEngine->HandleCommandL(EPtiCommandUserActionConvert);
                }
            while(curr < old)
                {
                --old;
                ptiEngine->HandleCommandL(EPtiCommandUserActionPrevious);
                }
            }
        candidatePane->HideWindow();
        if (textConv.Length() > 0 || textComp.Length() > 0)
            {
            iOwner->ChangeState(EMultiCompletion);
            }
        else
            {
            iOwner->ChangeState(EInitial);
            }
        }
    }

// End of file
