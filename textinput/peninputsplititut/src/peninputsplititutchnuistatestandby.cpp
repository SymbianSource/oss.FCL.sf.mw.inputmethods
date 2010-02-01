/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/


#include <coemain.h>
#include <peninputsplititutnew.rsg>
#include <peninputsplititutconfig_chinese.rsg>
#include <AknFepGlobalEnums.h>
#include <peninputlabel.h>

#include "peninputsplititutchnuistatestandby.h"
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutdatamgr.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::NewL
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStateStandby* CChnSplitItutUiStateStandby::NewL(CSplitItutUiMgrBase* aOwner)
    {
    CChnSplitItutUiStateStandby* self = new (ELeave) CChnSplitItutUiStateStandby(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::~CChnSplitItutUiStateStandby
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStateStandby::~CChnSplitItutUiStateStandby()
    {
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::CChnSplitItutUiStateStandby
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStateStandby::CChnSplitItutUiStateStandby(CSplitItutUiMgrBase* aOwner)
                                                          :CSplitItutUiStateBase(aOwner)
    {
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::StateType
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase::TUiState CChnSplitItutUiStateStandby::StateType()
    {
    return CSplitItutUiMgrBase::EStateStandby;
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::OnEntryL
// ---------------------------------------------------------------------------
//
void CChnSplitItutUiStateStandby::OnEntryL()
    {
    // pinyin    	
    switch (iOwner->DataMgr()->InputMode())
        {
        case EPinyin:
            {
            iOwner->DataMgr()->NotifyChangeControlLayout(
                 MItutPropertySubscriber::EItutPropertyKeypadResourceId,
                 R_FINGER_INPUT_KEYPAD_PINYIN_STANDBY_PREDICT);
            }
            break;
        case EStroke:
            {
            TInt resID;
            if (iOwner->DataMgr()->InputLanguage() == ELangPrcChinese)
                {
                resID = R_FINGER_INPUT_KEYPAD_STROKE_STANDBY_PREDICT_PRC;
                }
            else
                {
                resID = R_FINGER_INPUT_KEYPAD_STROKE_STANDBY_PREDICT_TRA;
                }
            iOwner->DataMgr()->NotifyChangeControlLayout(
                MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                resID);
            }
            break;        
        case EZhuyin:
            {
            iOwner->DataMgr()->NotifyChangeControlLayout(
                MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                R_FINGER_INPUT_KEYPAD_ZHUYIN_STANDBY_PREDICT);
            }
            break;        
        default:
            break;
        }
    

    iOwner->LayoutContext()->ShowArrowBtn(
        EBtnArrowLeft | EBtnArrowRight | EBtnArrowUp | EBtnArrowDown);
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::OnExit
// ---------------------------------------------------------------------------
//
void CChnSplitItutUiStateStandby::OnExit()
    {
    // hide input mode indicator

    iOwner->LayoutContext()->ShowArrowBtn(0);
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::HandleKeyL
// ---------------------------------------------------------------------------
//
TBool CChnSplitItutUiStateStandby::HandleKeyL(TInt aCmd, TInt aKey)
    {
    if (aKey == EKeyBackspace)
        {
        return iOwner->ClrConsumed();
        }

    if (IsKeyValid(aKey))
        {
        iOwner->SetNumLongPressValid(ETrue);
        iOwner->SetCurrentStateL(CSplitItutUiMgrBase::EStateComposition);
        iOwner->CurrentState()->HandleKeyL(aCmd, aKey);
        return ETrue;
        }
    
    return EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CChnSplitItutUiStateStandby::IsKeyValid
// ---------------------------------------------------------------------------
//
TBool CChnSplitItutUiStateStandby::IsKeyValid(TInt aKey)
    {
    TInt immode = iOwner->DataMgr()->InputMode();
    
    return (immode == EPinyin && aKey >= EPtiKey2 && aKey <= EPtiKey9) ||
           (immode == EStroke && aKey >= EPtiKey1 && aKey <= EPtiKey6) ||
           (immode == EZhuyin && aKey >= EPtiKey0 && aKey <= EPtiKey9);
    }

// End Of File
