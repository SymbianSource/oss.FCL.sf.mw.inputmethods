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
*       Provides the TAknFepUiInputStateConversionHiraganaKanji methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateConversionHiraganaKanji.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                     // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface
#include <aknfep.rsg>

// CONSTANTS

/**
 *  TAknFepUiInputStateConversionHiraganaKanji class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateConversionHiraganaKanji::
//                                  TAknFepUiInputStateConversionHiraganaKanji
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateConversionHiraganaKanji::
    TAknFepUiInputStateConversionHiraganaKanji(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateEntryJapaneseMultitap(aOwner, aUIContainer)
    {
    iData = EStateEventCandidateOn;
    iState = EConversion;

    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateConversionHiraganaKanji::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateConversionHiraganaKanji::InitializeStateL()
    {
    iOwner->SetInlineEditUnderlineVisibility(ETrue);

    // Update of CBA
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TInt newResource = R_AKNFEP_SOFTKEYS_EMPTY_EMPTY_COMMIT;
    fepMan->UpdateCbaL(newResource);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateConversionHiraganaKanji::HandleNumericKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateConversionHiraganaKanji::HandleNumericShortKeyL(TInt /*aKey*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    ptiEngine->HandleCommandL(EPtiCommandUserActionAllComplete);
    iOwner->ChangeState(EMultiCompletion);
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::HandleNaviKeyL
// Handling Navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateConversionHiraganaKanji::HandleNaviKeyL(TInt aKey,
                                                                 TKeyPressLength /*aLength*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TPtiEngineCommand ptiCommand = EPtiCommandNone;

    switch(aKey)
        {
        case EKeyLeftArrow:
        case EKeyRightArrow:
        case EStdKeyLeftArrow:      /* 0x0e */
        case EStdKeyRightArrow:     /* 0x0f */
            {
            ptiCommand = EPtiCommandUserActionShorten;
            if (aKey == EKeyRightArrow
             || aKey == EStdKeyRightArrow)
                ptiCommand = EPtiCommandUserActionLengthen;

            ptiEngine->HandleCommandL(ptiCommand);
            UpdateInlineEditingCharacterL();
            iData = EStateEventCandidateOff;
            }
            break;
        case EKeyDownArrow:
        case EKeyUpArrow:
        case EStdKeyDownArrow:      /* 0x11 */
        case EStdKeyUpArrow:        /* 0x10 */
        case EStdKeySpace:
            {
            ptiCommand = EPtiCommandUserActionConvert;
            if (aKey == EKeyUpArrow
             || aKey == EStdKeyUpArrow)
                {
                ptiCommand = EPtiCommandUserActionPrevious;
                }
            ptiEngine->HandleCommandL(ptiCommand);
            UpdateInlineEditingCharacterL();
            if(iData == EStateEventCandidateOn)
                {
                iOwner->ChangeState(ECandidate);
                }
            iData = EStateEventCandidateOn;
            }
            break;
        default:
             break;
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateConversionHiraganaKanji::HandleAuxKeyL(TInt aKey,
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
            iOwner->ChangeState(EMultiCompletion);
            }
            break;
        case EKeyBackspace:
        case EStdKeyBackspace:      /* 0x01 */
        case EStdKeyEscape:         /* 0x04 */
            {
            fepMan->SetInlineEditingCursorVisibilityL(ETrue);
            ptiEngine->HandleCommandL(EPtiCommandUserActionCancel);
            iOwner->SetInlineEditUnderlineVisibility(ETrue);
            UpdateInlineEditingCharacterL();
            iOwner->ChangeState(EEntry);
            ret = ETrue;
            }
            break;
        default:
            break;
        }

    return ret;
    }

// End of file
