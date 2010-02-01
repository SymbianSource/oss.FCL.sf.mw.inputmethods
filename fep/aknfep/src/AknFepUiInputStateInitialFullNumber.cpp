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
* Description:            Provides the TAknFepUiInputStateInitialFullNumber methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateInitialFullNumber.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepManager.h"

#include <PtiEngine.h>
#include <featmgr.h>                //FeatureManager
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface
#include <jplangutil.h>

// CONSTANTS
const TInt KNumAllowedIdleNumericChars = 21;

_LIT(KAknFepPlainNumbers, "1234567890");
const TUint16 KAllAllowedNumericChars[] =
    {'*','+','p','w','#','1','2','3','4','5','6','7','8','9','0', ';','.',',','-','E','?'};
const TKeyEvent KEventsForSecondPass[]  =
    {
        {42, EStdKeyNkpAsterisk, 3, 0},
        {43, EStdKeyNkpPlus, 3, 0},
        {112, 80, EModifierAutorepeatable, 0},
        {119, 87, EModifierAutorepeatable, 0},
        {35, EStdKeyHash, EModifierAutorepeatable, 0},
        {EPtiKey1, EPtiKey1, EModifierAutorepeatable, 0},
        {EPtiKey2, EPtiKey2, EModifierAutorepeatable, 0},
        {EPtiKey3, EPtiKey3, EModifierAutorepeatable, 0},
        {EPtiKey4, EPtiKey4, EModifierAutorepeatable, 0},
        {EPtiKey5, EPtiKey5, EModifierAutorepeatable, 0},
        {EPtiKey6, EPtiKey6, EModifierAutorepeatable, 0},
        {EPtiKey7, EPtiKey7, EModifierAutorepeatable, 0},
        {EPtiKey8, EPtiKey8, EModifierAutorepeatable, 0},
        {EPtiKey9, EPtiKey9, EModifierAutorepeatable, 0},
        {EPtiKey0, EPtiKey0, EModifierAutorepeatable, 0},
        {59,       EStdKeySemiColon, EModifierAutorepeatable, 0},
        {46,       122, EModifierAutorepeatable, 0},
        {44,       121, EModifierAutorepeatable, 0},
        {45,       130, EModifierAutorepeatable, 0},
        {101,      69,  EModifierAutorepeatable|EModifierLeftShift, 0},
        {63,       EStdKeyForwardSlash, EModifierAutorepeatable|EModifierLeftShift, 0}
    };

/**
 *  AknFepUiInputStateInitialFullNumber class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialFullNumber::TAknFepUiInputStateInitialFullNumber
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateInitialFullNumber::TAknFepUiInputStateInitialFullNumber(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateInitialJapaneseBase(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialFullNumber::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialFullNumber::InitializeStateL()
    {
    TAknFepUiInputStateInitialJapaneseBase::InitializeStateL();

    // Setup PtiEngine
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const TWidthChar width = iOwner->FepMan()->CharacterWidth();
    TPtiEngineInputMode inputmode = EPtiEngineInputModeNone;
    switch(width)
        {
        case EHalfWidthChar:
            inputmode = (iOwner->IsQwerty())?
                            EPtiEngineQwerty
                            : EPtiEngineNumeric;
            break;

        case EFullWidthChar:
            inputmode = (iOwner->IsQwerty())?
                            EPtiEngineFullWidthNumericQwerty
                            : EPtiEngineFullWidthNumeric;
            break;

        default: // no operation
            break;
        }
    if (inputmode != EPtiEngineInputModeNone)
        {
        ptiEngine->SetInputMode(inputmode);

        if (inputmode == EPtiEngineNumeric)
            {
            TRAP_IGNORE(ptiEngine->SetExternalKeyMapL(EPtiEngineNumeric, EPtiKeyStar, 
                                                     (TDesC&)iOwner->NumberModeKeyMapping(), 
                                                      EPtiCaseLower))
            }
        }

    ptiEngine->ClearCurrentWord();
    }

TBool TAknFepUiInputStateInitialFullNumber::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool rtn = EFalse;

    switch(ptiengine->InputMode())
        {
        case EPtiEngineNumeric:         // non-Qwerty number input
            rtn = HandleOwnKeyL(aKey, aLength);
            break;
        case EPtiEngineQwerty:                  // Qwerty number input
        case EPtiEngineFullWidthNumericQwerty:  // Qwerty full-width number input
            rtn = HandleQwertyKeyL(aKey, aLength, fepMan->CharacterWidth());
            break;
        case EPtiEngineFullWidthNumeric:    // non-Qwerty full-width number input
            rtn = TAknFepUiInputStateInitialJapaneseBase::HandleKeyL(aKey, aLength);
            break;
        default:
            break;
        }
    return rtn;
    }

TBool TAknFepUiInputStateInitialFullNumber::HandleOwnKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    if (aLength == ELongKeyPress)
        {
        switch (aKey)
            {
        case EKeyRightArrow:
        case EKeyLeftArrow:
        case EKeyDownArrow:
        case EKeyUpArrow:
        case EKeyBackspace:
            return EFalse;
        case EPtiKeyStar:
             if (!fepMan->HashKeySelectionInUse()
              && fepMan->EditSubmenuInUse())
                {
                fepMan->LaunchSpecialCharacterTableL();
                }
        default:
            return ETrue;
            }
        }

    if(aKey == EPtiKeyStar)
        {
        TBool append = EFalse;
        if (aLength == EShortKeyPress)
            {
            if (fepMan->EditSubmenuInUse())
                {
                if (fepMan->IsOnlyNumericPermitted())
                    {
                    append = ETrue;
                    }
                else
                    {
                    fepMan->ClearCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);
                    }
                }
            else
                {
                if (fepMan->IsAbleToLaunchSCT())
                    {
                    fepMan->LaunchSpecialCharacterTableL();
                    }
                else
                    {
                    append = ETrue;
                    }
                }
            }
        else
            {
            if (fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
                {
                fepMan->LaunchSpecialCharacterTableL();
                }
            }
        if (append)
            {
            iData = aKey;
            TPtrC text = ptiengine->AppendKeyPress((TPtiKey)aKey);

            // star key is inactive if keymapping is empty.
            if (text.Length() > 0 && text[0] != 0)
                {
                fepMan->NewCharacterL(text);

                // Commit immediately if there is only one character associated to the key.
                TBuf<CAknFepManager::EMaximumFepWordLength> mapdata;
                ptiengine->MappingDataForKey((TPtiKey)aKey, mapdata, ptiengine->Case());
                if (mapdata.Length() <= 1)
                    {
                    fepMan->CommitInlineEditL();
                    ptiengine->ClearCurrentWord();
                    iData = 0;
                    }
                }
            }

        return ETrue;
        }

    if(iData)
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();

        iData = 0;
        }
    return EFalse;
    }

TBool TAknFepUiInputStateInitialFullNumber::HandleQwertyKeyL(TInt aKey, 
                                            TKeyPressLength /*aLength*/, const TWidthChar aWidth)
    {
    TBool rtn = EFalse;

    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepCaseManager* caseMan = iOwner->CaseMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringShiftKeyPress);

    if ( fepMan->IsOnlyNumericPermitted() )
        {
        if (aKey == EStdKeySpace || aKey == EStdKeyEnter)
            {
            // return Consumed
            return ETrue;
            }
        if (!IsQwertyKey(aKey))
            {
            // return Not Consumed
            return EFalse;
            }

        TChar chr(0);
        TKeyEvent numKey;

        ptiengine->AppendKeyPress((TPtiKey)aKey);
        const MPtiEngineCompositionDataInterface* data = ptiengine->CompositionData();
        TPtrC text = data->CompletedString();
        if (text.Length() > 0 && text[0] != 0)
            {
            TChar compchar = text[0];
            if (aWidth == EFullWidthChar)
                {
                TBuf<2> halfstring;
                TInt convlen = JPLangUtil::ConvertFullToHalfWidth(
                                text.Left(1), halfstring);
                if (convlen > 0)
                    {
                    compchar = halfstring[0];
                    }
                }
            for (TInt i = 0; i < KNumAllowedIdleNumericChars; i++)
                {

                if (compchar == KAllAllowedNumericChars[i])
                    {
                    numKey = KEventsForSecondPass[i];
                    chr = KAllAllowedNumericChars[i];
                    break;
                    }
                }
            }

        if ((chr != 0) &&
            ((iOwner->NumberModeKeyMapping().Locate(chr) != KErrNotFound) ||
             (KAknFepPlainNumbers().Locate(chr) != KErrNotFound)) ||
             fepMan->IsAllowedKeymappingForNumberMode(chr))
            {
            if (aWidth == EFullWidthChar)
                {
                // Full-width characters are committed immediately.
                fepMan->NewCharacterL(text);
                fepMan->CommitInlineEditL();
                }
            else
                {
                // If editor is in number mode and incoming key press would produce
                // any of the allowed number mode characters,
                // resend suitable key event let it pass through FEP. That way phone idle
                // receives the event too. This also takes care of remapping
                // different qwerty keymap layouts to edwin events.
                fepMan->SetFlag(CAknFepManager::EFlagPassNextKey);
                CCoeEnv::Static()->SimulateKeyEventL(numKey, EEventKey);
                }
            }
        ptiengine->ClearCurrentWord();

        return ETrue;
        }

    return rtn;
    }



// Functions from MPtiObserver interface
void TAknFepUiInputStateInitialFullNumber::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        TRAP_IGNORE(fepMan->CommitInlineEditL())

        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();

        iData = 0;
        }
    }

// End of file
