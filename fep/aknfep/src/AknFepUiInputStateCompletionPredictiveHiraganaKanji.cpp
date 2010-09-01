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
*       Provides the TAknFepUiInputStateCompletionPredictiveHiraganaKanji methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateCompletionPredictiveHiraganaKanji.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePaneInterface.h"
                                        // MAknFepUICtrlCandidatePaneInterface
#include "AknFepUICtrlContainerJapanese.h"
#include "AknFepManager.h"
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                     // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface

/**
 *  TAknFepUiInputStateCompletionPredictiveHiraganaKanji class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionPredictiveHiraganaKanji::
//                        TAknFepUiInputStateCompletionPredictiveHiraganaKanji
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCompletionPredictiveHiraganaKanji::
    TAknFepUiInputStateCompletionPredictiveHiraganaKanji(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateCompletionHiraganaKanji(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionPredictiveHiraganaKanji::HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionPredictiveHiraganaKanji::HandleNumericShortKeyL(TInt aKey)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = CommitInlineEditingCharacterL();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->ConvertingString();
    if (text.Length() > 0)
        {
        iOwner->ChangeState(EInitial);
        iOwner->FepMan()->SimulateKeyEventL(aKey, ETrue);
        ret = ETrue;
        }
    else
        {
        iOwner->ChangeState(EInitial);
        if (iOwner->IsQwerty() && IsQwertyKey(aKey) && aKey != EStdKeySpace)
            {
            ret = EFalse;
            }
        }

    return ret;
     }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionPredictiveHiraganaKanji::HandleNumericLongKeyL
// Handling Numeric long keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionPredictiveHiraganaKanji::HandleNumericLongKeyL(TInt aKey)
    {
    TBool ret = EFalse;
    
    ret = ProcessNumericLongKeyPressL(aKey);
    
    if (ret && iOwner->FepMan()->EditorHasFreeSpace())
        {
        iOwner->ChangeState(ENextWord);
        }
    else
        {
        MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
        MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);
        // Closeing candidate pop-up list
        predictivePane->HideWindow();
        iOwner->ChangeState(EInitial);
        ret = ETrue;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionPredictiveHiraganaKanji::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionPredictiveHiraganaKanji::HandleAuxKeyL(TInt aKey,
                                                                          TKeyPressLength /*aLength*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;

    if (aKey == EKeyOK 
     || aKey == EStdKeyDevice3          /* 0xA7 */
     || aKey == EStdKeyEnter
     || aKey == EStdKeyNkpEnter)
        {
        TInt index = UIContainer()->CandidatePane(MAknFepUICtrlContainerJapanese::
                                                  EJapanesePredictiveCandidatePane)->CurrentItemIndexOfCandidate();
        if (index > -1)
            {
            ptiEngine->HandleCommandL(EPtiCommandUserActionCompleteCandidate, &index);
            }
        else
            {
            ptiEngine->HandleCommandL(EPtiCommandUserActionComplete);
            }

        UpdateInlineEditingCharacterL();
        ret = ETrue;
        }
    return ret;
    }
// End of file
