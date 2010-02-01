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
* Description:            Provides the TAknFepUiInputStateJapaneseBase methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateJapaneseBase.h"
#include "AknFepUIManagerStateInterface.h"   //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        //MAknFepManagerUIInterface
#include "AknFepCaseManager.h"               // CAknFepCaseManager
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"

#include <PtiDefs.h>                // keys
#include <avkon.rsg>

// CONSTANTS
#define EFirstHalfUpperChar  0x0041
#define ELastHalfUpperChar   0x005A
#define EFirstFullUpperChar  0xff21
#define ELastFullUpperChar   0xff3A
#define EFirstHalfLowerChar  0x0061
#define ELastHalfLowerChar   0x007A
#define EFirstFullLowerChar  0xff41
#define ELastFullLowerChar   0xff5A

/**
 *  TAknFepUiInputStateJapaneseBase class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::TAknFepUiInputStateJapaneseBase
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateJapaneseBase::
TAknFepUiInputStateJapaneseBase(MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepInputStateBase(aOwner)
    {
    iUIContainer = aUIContainer;
    if (aOwner)
        {
        aOwner->FepMan()->SendUIActivatedNotification();
        }
    }

/**
 * MPtiObserver interface.
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::KeyTimerExpired
// Call this function when Multitap timer of PtiEngine was time-out
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateJapaneseBase::KeyTimerExpired()
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::LastWordInSelectionList
// not-use Japanese state
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateJapaneseBase::LastWordInSelectionList()
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::FirstWordInSelectionList
// not-use Japanese state
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateJapaneseBase::FirstWordInSelectionList()
    {
    }

/**
 *  TAknFepUiInputStateJapaneseBase class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::HandleKeyL
// Hnadling Keypress events. This Method is called by CAknFepUIManagerJapanese
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::
    HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = ETrue;
    if(iState == EInitial)
        {
        ret = EFalse;
        }

    if (iOwner->IsQwerty())
        {
        switch(aKey)
            {
            case EStdKeyLeftArrow:      /* 0x0e */
            case EStdKeyRightArrow:     /* 0x0f */
            case EStdKeyUpArrow:        /* 0x10 */
            case EStdKeyDownArrow:      /* 0x11 */
                {
                // Long and short keypress is same Handling
                ret = HandleNaviKeyL(aKey, aLength);
                }
                break;
            case EKeyOK:
            case EStdKeyEnter:          /* 0x03 */
            case EStdKeyNkpEnter:       /* 0x88 */
            case EStdKeyDevice3:        /* 0xA7 */
                {
                ret = HandleAuxKeyL(aKey, aLength);
                }
                break;
            case EStdKeyBackspace:      /* 0x01 */
            case EStdKeyEscape:         /* 0x04 */
                {
                // Long and short keypress is same Handling
                ret = HandleAuxKeyL(aKey, aLength);
                }
                break;
            case EStdKeySpace:
                if( iState == EConversion
                 || iState == ECandidate
                 || iState == ECandidateRetroActive )
                    {
                    ret = HandleNaviKeyL(aKey, aLength);
                    }
                else
                    {
                    ret = HandleNumericShortKeyL(aKey);
                    }
                break;
            default:
                // for Qwerty
                if (aLength == EShortKeyPress
                 && IsQwertyKey(aKey))
                    {
                    ret = HandleNumericShortKeyL(aKey);
                    }
                break;
            }
        }
    else
        {
        switch(aKey)
            {
            case EPtiKey0:
            case EPtiKey1:
            case EPtiKey2:
            case EPtiKey3:
            case EPtiKey4:
            case EPtiKey5:
            case EPtiKey6:
            case EPtiKey7:
            case EPtiKey8:
            case EPtiKey9:
            case EPtiKeyStar:
                {
                if(aLength == ELongKeyPress)
                    {
                    if (aKey != EPtiKeyStar)
                        {
                        ret = HandleNumericLongKeyL(aKey);
                        }
                    else
                        {
                        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                        if (fepMan->HashKeySelectionInUse())
                            {
                            ret = ETrue;
                            }
                        }
                    }
                else if (aLength == EShortKeyPress)
                    {
                    ret = HandleNumericShortKeyL(aKey);
                    }
                }
                break;
            case EKeyLeftArrow:
            case EKeyRightArrow:
            case EKeyDownArrow:
            case EKeyUpArrow:
                {
                // Long and short keypress is same Handling
                ret = HandleNaviKeyL(aKey, aLength);
                }
                break;
            case EKeyOK:
                {
                ret = HandleAuxKeyL(aKey, aLength);
                }
                break;
            case EKeyBackspace:
                {
                // Long and short keypress is same Handling
                ret = HandleAuxKeyL(aKey, aLength);
                }
                break;
            default:
                {
                AknFepPanic(EAknFepPanicNotSupportKey);
                }
                break;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::HandleCommandL
// Handling Command events. This Method is called by CAknFepUIManagerJapanese.
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateJapaneseBase::HandleCommandL(TInt /*aCommandId*/)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::operator=
// A patch to force the vtable to be copied during an assignment.
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateJapaneseBase::operator=(const TAknFepInputStateBase& aState)
    {
    Mem::Copy(this, &aState, sizeof(TAknFepUiInputStateJapaneseBase));
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::HandleNumericKeyL
// Handling short keypress events of the numeric and the star.
// HandleKeyEventL() call this method.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::HandleNumericShortKeyL(TInt /*aKey*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::HandleNumericLongKeyL
// Handling Numeric long keypress event. HandleKeyEventL() call this method.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::HandleNumericLongKeyL(TInt /*aKey*/)
    {
    iOwner->ChangeState(EMultiCompletion);
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::HandleNaviKeyL
// Handling navigate keypress event. HandleKeyEventL() call this method.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::HandleNaviKeyL(TInt /*aKey*/,
                                                      TKeyPressLength /*aLength*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace.
// HandleKeyEventL() call this method.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::HandleAuxKeyL(TInt /*aKey*/,
                                                     TKeyPressLength /*aLength*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::UpdateInlineEditingCharacterL
// Update inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::UpdateInlineEditingCharacterL()
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::CommitInlineEditingCharacterL
// Commit inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::CommitInlineEditingCharacterL()
    {
    return EFalse;
    }


// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::UIContainer()
// Getting UI container object
// ---------------------------------------------------------------------------
//

MAknFepUICtrlContainerJapanese* TAknFepUiInputStateJapaneseBase::UIContainer()
    {
    return static_cast<MAknFepUICtrlContainerJapanese*>(iUIContainer);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::UpdateCase
// Update character case
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateJapaneseBase::UpdateCase(TPtrC& aChar)
    {
    CAknFepCaseManager* caseMan = iOwner->CaseMan();
    if (IsDiffrentCase(aChar))
        {
        caseMan->ManualCaseUpdate();
        }
    else
        {
        caseMan->UpdateCase(ENullNaviEvent);
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::IsDiffrentCase
// Check whether last catacter case and current catacter case is same
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::IsDiffrentCase(TPtrC& aChar) const
    {
    TBool ret = EFalse;
    CAknFepCaseManager* caseMan = iOwner->CaseMan();
    TInt currentCase = caseMan->CurrentCase();
    switch(currentCase)
        {
        case EAknEditorUpperCase:
            {
            if (IsLowerCase(aChar))
                {
                ret = ETrue;
                }
            }
            break;
        case EAknEditorLowerCase:
            {
            if (IsUpperCase(aChar))
                {
                ret = ETrue;
                }
            }
        case EAknEditorTextCase:
            break;
        default: // Panic
            {
            AknFepPanic(EAknFepPanicBadCase);
            }
            break;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::IsUpperCase
// Check a caracter case is upper
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::IsUpperCase(TPtrC& aChar) const
    {
    if ((EFirstHalfUpperChar <= aChar[0] && aChar[0] <= ELastHalfUpperChar)
     || (EFirstFullUpperChar <= aChar[0] && aChar[0] <= ELastFullUpperChar))
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::IsLowerCase
// Check a caracter case is lower
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapaneseBase::IsLowerCase(TPtrC& aChar) const
    {
    if ((EFirstHalfLowerChar <= aChar[0] && aChar[0] <= ELastHalfLowerChar)
     || (EFirstFullLowerChar <= aChar[0] && aChar[0] <= ELastFullLowerChar))
        {
        return ETrue;
        }
    return EFalse;
    }

TBool TAknFepUiInputStateJapaneseBase::HandleQwertyShortKeyL(TInt /*aKey*/)
    {
    return EFalse;
    }

TBool TAknFepUiInputStateJapaneseBase::IsNumberKey(const TInt aKey)
    {
    TBool response = EFalse;
    if (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9)
        {
        response = ETrue;
        }
    return response;
    }

TBool TAknFepUiInputStateJapaneseBase::IsQwertyKey(const TInt aKey)
    {
    TBool response = EFalse;
    if ( (aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
        IsNumberKey( aKey ) ||
        aKey == EStdKeySpace ||
        aKey == EPtiKeyQwertyPlus ||
        aKey == EPtiKeyQwertyMinus ||
        aKey == EPtiKeyQwertyComma ||
        aKey == EPtiKeyQwertySemicolon ||
        aKey == EPtiKeyQwertyFullstop ||
        aKey == EPtiKeyQwertyHash ||
        aKey == EPtiKeyQwertySlash ||
        aKey == EPtiKeyQwertyApostrophe)
        {
        response = ETrue;
        }
    return response;
    }

// End of file
