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
*       Provides the TAknFepUiInputStateCompletionLatinJapanese methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateCompletionLatinJapanese.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepCaseManager.h"               // CAknFepCaseManager
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                     // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface

// CONSTANTS

/**
*  TAknFepUiInputStateCompletionLatinJapanese class.
*
*/

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionLatinJapanese::
//                              TAknFepUiInputStateCompletionLatinJapanese
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCompletionLatinJapanese::TAknFepUiInputStateCompletionLatinJapanese(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateCompletionJapaneseMultitap(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionLatinJapanese::HandleNumericShortKeyL(TInt aKey)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool ret = ETrue;
    CommitInlineEditingCharacterL();
    iOwner->ChangeState(EInitial);
    if (!iOwner->IsQwerty())
        {
        fepMan->SimulateKeyEventL(aKey, ETrue);
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionLatinJapanese::CommitInlineEditingCharacterL
// Commit inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionLatinJapanese::CommitInlineEditingCharacterL()
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    if (!iOwner->IsQwerty()
     && iOwner->FepMan()->IsSupportsSecretText()
     && fepMan->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
        {
        fepMan->CommitInlineEditL();
        ptiEngine->ClearCurrentWord();
        iData = 0;
        ret = ETrue;
        }
    else
        {
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC text = data->CompletedString();
        if (text.Length() > 0)
            {
            // Update case without Qwerty
            if (!iOwner->IsQwerty())
            {
            UpdateCase(text);
            }

        // Commit a character
        fepMan->CommitInlineEditL(text, text.Length());

        // Update case with Qwerty
        if (iOwner->IsQwerty())
            {
            CAknFepCaseManager* caseMan = iOwner->CaseMan();
            caseMan->UpdateCase(ENullNaviEvent);
                }

            ptiEngine->ClearCurrentWord();
            iData = 0;
            ret = ETrue;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionLatinJapanese::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionLatinJapanese::HandleAuxKeyL(TInt aKey,
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
        UpdateInlineEditingCharacterL();
        // OK-key or Enter-key shouldn't be consumed in Latin input.
        // Because OK-key or Enter-key might be used in application side.
        ret = EFalse;
        }

    return ret;
    }

// End of file
