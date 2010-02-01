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
*       Provides the TAknFepUiInputStateCompletionJapaneseMultitap methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateCompletionJapaneseMultitap.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"
#include "AknFepManager.h"                   // CAknFepCaseManager

#include <PtiDefs.h>                     // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface

// CONSTANTS

/**
 *  TAknFepUiInputStateCompletionJapaneseMultitap class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::
//                              TAknFepUiInputStateCompletionJapaneseMultitap
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCompletionJapaneseMultitap::
    TAknFepUiInputStateCompletionJapaneseMultitap(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateJapaneseBase(aOwner, aUIContainer)
    {
    iState = EMultiCompletion;
    }


// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCompletionJapaneseMultitap::HandleCommandL(TInt aCommandId)
    {
    if (aCommandId == EAknFepAllCompletion)
        {
        iOwner->PtiEngine()->HandleCommandL(EPtiCommandUserActionAllComplete);
        CommitInlineEditingCharacterL();
        iOwner->ChangeState(EInitial);
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionJapaneseMultitap::HandleNumericShortKeyL(TInt /*aKey*/)
    {
    return UpdateInlineEditingCharacterL();
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::HandleNaviKeyL
// Handling Navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionJapaneseMultitap::HandleNaviKeyL(TInt /*aKey*/,
                                                                    TKeyPressLength /*aLength*/)
    {
    return UpdateInlineEditingCharacterL();
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionJapaneseMultitap::HandleAuxKeyL(TInt aKey,
                                                                   TKeyPressLength aLength)
    {
    TBool ret = ETrue;
    CPtiEngine* ptiEngine = iOwner->PtiEngine();

    if (aKey == EKeyOK
     || aKey == EStdKeyDevice3          /* 0xA7 */
     || aKey == EStdKeyEnter
     || aKey == EStdKeyNkpEnter)
        {
        TPtiEngineCommand ptiCommand = EPtiCommandUserActionComplete;
        if (aLength == ELongKeyPress)
            {
            ptiCommand = EPtiCommandUserActionAllComplete;
            }
        ptiEngine->HandleCommandL(ptiCommand);
        ret = UpdateInlineEditingCharacterL();
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::HandleNumericLongKeyL
// Handling Numeric long keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionJapaneseMultitap::HandleNumericLongKeyL(TInt aKey)
    {
    TBool ret = ProcessNumericLongKeyPressL(aKey);
    iOwner->ChangeState(EInitial);
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::UpdateInlineEditingCharacterL
// Update inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionJapaneseMultitap::UpdateInlineEditingCharacterL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;
    
    if(CommitInlineEditingCharacterL())
        {
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC text = data->ConvertingString();
        TInt cursorpos = data->CursorPosition();
        if (text.Length() > 0)
            {
            if (data->NumberOfPhrase() > 0)
                {
                TPtrC8 clauseChar = data->LengthOfPhrase();
                cursorpos = clauseChar[0];
                }
            iOwner->ChangeState(EEntry);
            fepMan->StartInlineEditL();
            fepMan->UpdateInlineEditL(text, cursorpos);
            }
        else
            {
            __ASSERT_DEBUG(text.Length() == 0,
                RDebug::Print(
                   _L("In TAknFepUiInputStateCompletionJapaneseMultitap::UpdateInlineEditingCharacterL,\nINVALID DATA: ConvertingString. Roll back to initial State!")));
            ptiEngine->ClearCurrentWord();
            iOwner->ChangeState(EInitial);
            }

        ret = ETrue;
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::CommitInlineEditingCharacterL
// Commit inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionJapaneseMultitap::CommitInlineEditingCharacterL()
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->CompletedString();
    if (text.Length() > 0)
        {
        fepMan->CommitInlineEditL(text, text.Length());
        ret = ETrue;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::ProcessNumericLongKeyPressL
// 
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionJapaneseMultitap::ProcessNumericLongKeyPressL(TInt aKey)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;

    ptiEngine->DeleteKeyPress();

    if (!iOwner->IsQwerty()
     && fepMan->IsSupportsSecretText()
     && fepMan->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
        {
        iOwner->PtiEngine()->ClearCurrentWord();
        TPtiEngineCommand cmd = (TPtiEngineCommand)(EPtiCommandUserActionCompleteHalf0 + (aKey - '0'));
        ptiEngine->HandleCommandL(cmd);
        const MPtiEngineCompositionDataInterface* srcretdata = ptiEngine->CompositionData();
        TPtrC srcrettext = srcretdata->CompletedString();
        if (srcrettext.Length() > 0)
            {
            TBuf<1> buf;
            buf.Append(srcrettext[0]);

            // if the previous character is the special character (line feed or space),
            // it is necessary to clear the flag.
            TInt specialcharFlags = CAknFepManager::EFlagLineFeedCharacter
                                  | CAknFepManager::EFlagSpaceCharacter
                                  | CAknFepManager::EFlagFullWidthSpaceCharacter;
            if (fepMan->IsFlagSet(specialcharFlags))
                {
                fepMan->ClearFlag(specialcharFlags);
                }

            fepMan->NewCharacterL(buf);
            fepMan->CommitInlineEditL();
            }
        ptiEngine->ClearCurrentWord();
        ret = ETrue;
        }
    else
        {
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->ConvertingString();
    if (text.Length() > 0)
        {
        ptiEngine->HandleCommandL(EPtiCommandUserActionAllComplete);
            ret = CommitInlineEditingCharacterL();
            __ASSERT_DEBUG(ret, AknFepPanic(EAknFepPanicCandidatesEmpty));
            }
        else
            {
            fepMan->CancelInlineEdit();
            iOwner->PtiEngine()->ClearCurrentWord();
            }
    if (fepMan->EditorHasFreeSpace())
        {
        TPtiEngineCommand cmd = (TPtiEngineCommand)(EPtiCommandUserActionCompleteHalf0 + (aKey - '0'));

            ptiEngine->HandleCommandL(cmd);
            fepMan->StartInlineEditL();
            const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
            TPtrC text = data->CompletedString();
            if (text.Length() > 0)
                {
                fepMan->CommitInlineEditL(text, text.Length());
                ret = ETrue;
                }
            else
                {
                ptiEngine->ClearCurrentWord();
                fepMan->CancelInlineEdit();
                iOwner->ChangeState(EInitial);
                }
            }
        }
    return ret;
    }
// End of file
