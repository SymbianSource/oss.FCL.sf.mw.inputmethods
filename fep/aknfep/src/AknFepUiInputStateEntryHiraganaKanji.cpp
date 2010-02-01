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
*       Provides the TAknFepUiInputStateEntryHiraganaKanji methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateEntryHiraganaKanji.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                     // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface
#include <AknFep.rsg>

// CONSTANTS

/**
 *  TAknFepUiInputStateEntryHiraganaKanji class.
 *
 */

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryHiraganaKanji::TAknFepUiInputStateEntryHiraganaKanji
// Constractor
// ---------------------------------------------------------------------------
//

TAknFepUiInputStateEntryHiraganaKanji::TAknFepUiInputStateEntryHiraganaKanji(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateEntryJapaneseMultitap(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryHiraganaKanji::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateEntryHiraganaKanji::InitializeStateL()
    {
    // Set on Underline visibility in inline editing
    iOwner->SetInlineEditUnderlineVisibility(ETrue);

    // Update of CBA
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TInt newResource = R_AKNFEP_SOFTKEYS_PREDICT_OTHER_COMMIT;
    if(!fepMan->IsAbleToChangePrediction())
        {
        newResource = R_AKNFEP_SOFTKEYS_EMPTY_OTHER_COMMIT;
        }

    fepMan->UpdateCbaL(newResource);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryHiraganaKanji::HandleNaviKeyL
// Handling Navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryHiraganaKanji::HandleNaviKeyL(TInt aKey,
                                                            TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TPtiEngineCommand ptiCommand = EPtiCommandNone;

    switch(aKey)
        {
        case EKeyLeftArrow:
        case EStdKeyLeftArrow:      /* 0x0e */
            {
            fepMan->SetInlineEditingCursorVisibilityL(ETrue);
            ptiCommand = EPtiCommandUserActionLeft;
            }
            break;
        case EKeyRightArrow:
        case EStdKeyRightArrow:     /* 0x0f */
            {
            fepMan->SetInlineEditingCursorVisibilityL(ETrue);
            ptiCommand = EPtiCommandUserActionRight;
            }
            break;
        case EKeyDownArrow:
        case EStdKeyDownArrow:      /* 0x11 */
        case EStdKeySpace:
            {
            ptiCommand = EPtiCommandUserActionConvert;
            }
            break;
        case EKeyUpArrow:
        case EStdKeyUpArrow:        /* 0x10 */
            {
            ptiCommand = EPtiCommandUserActionPrevious;
            }
            break;
        default:
            break;
        }
    ptiEngine->HandleCommandL(ptiCommand);
    UpdateInlineEditingCharacterL();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->ConvertingString();
    if (text.Length() > 0)
        {
        switch(aKey)
            {
            case EKeyDownArrow:
            case EStdKeyDownArrow:      /* 0x11 */
            case EKeyUpArrow:
            case EStdKeyUpArrow:        /* 0x10 */
            case EStdKeySpace:
                {
                iOwner->ChangeState(EConversion);
                // Set OFF Cursor unvisibility
                fepMan->SetInlineEditingCursorVisibilityL(EFalse);
                }
                break;
            default:
                break;
            }
        ret = ETrue;
        }

    return ret;
    }


TBool TAknFepUiInputStateEntryHiraganaKanji::HandleNumericShortKeyL(TInt aKey)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->ConvertingString();
    TBool ret = EFalse;

    if (aKey == EStdKeySpace
     && text.Length() > 0)
        {
        ret = HandleNaviKeyL(aKey, EShortKeyPress);
        }
    else
        {
        ret = TAknFepUiInputStateEntryJapaneseMultitap::HandleNumericShortKeyL(aKey);
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryHiraganaKanji::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateEntryHiraganaKanji::
    HandleCommandL(TInt aCommandId)
    {
    switch (aCommandId)
        {
        case EAknFepSoftkeyOtherModes:
            {
            // Change the status to Retroactive input
            iOwner->ChangeState(ECandidateRetroActive);
            }
            break;
        case EAknFepSoftkeyPredictiveOn:
            {
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            CPtiEngine* ptiEngine = iOwner->PtiEngine();

            // Update of inline editing characters
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
                    iOwner->ChangeState(EInitial);
                    }
                }
            else if (text2.Length() > 0)
                {
                // Change to ConversionState
                iOwner->ChangeState(EPredictiveInput);
                fepMan->UpdateInlineEditL(text2, text2.Length());
                }
            }
            break;
        default:
            TAknFepUiInputStateEntryJapaneseMultitap::HandleCommandL(aCommandId);
            break;
        }
    }

// End of file
