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
* Description:  drop-down list control
*
*/


#include <coemain.h>
#include <peninputgeneralitutnew.rsg>
#include <peninputitutconfig_chinese.rsg>
#include <AknFepGlobalEnums.h>
#include <peninputlabel.h>

#include "peninputitutchnuistatestandby.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutdatamgr.h"

CChnItutUiStateStandby* CChnItutUiStateStandby::NewL(CGenericItutUiMgrBase* aOwner)
    {
    CChnItutUiStateStandby* self = new (ELeave) CChnItutUiStateStandby(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CChnItutUiStateStandby::~CChnItutUiStateStandby()
    {
    }

CChnItutUiStateStandby::CChnItutUiStateStandby(CGenericItutUiMgrBase* aOwner)
                                                          :CGenericItutUiStateBase(aOwner)
    {
    }

CGenericItutUiMgrBase::TUiState CChnItutUiStateStandby::StateType()
    {
    return CGenericItutUiMgrBase::EStateStandby;
    }

void CChnItutUiStateStandby::OnEntryL()
    {
	iOwner->LayoutContext()->Control(ECtrlIdSwitch)->Hide(EFalse);
	iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(EFalse);
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

void CChnItutUiStateStandby::OnExit()
    {
    // hide input mode indicator

    iOwner->LayoutContext()->ShowArrowBtn(0);
    }

TBool CChnItutUiStateStandby::HandleKeyL(TInt aCmd, TInt aKey)
    {
    if (aKey == EKeyBackspace)
        {
        return iOwner->ClrConsumed();
        }

    if (IsKeyValid(aKey))
        {
        iOwner->SetNumLongPressValid(ETrue);
        iOwner->SetCurrentStateL(CGenericItutUiMgrBase::EStateComposition);
        iOwner->CurrentState()->HandleKeyL(aCmd, aKey);
        return ETrue;
        }
    
    return EFalse;    
    }
    
TBool CChnItutUiStateStandby::IsKeyValid(TInt aKey)
    {
    TInt immode = iOwner->DataMgr()->InputMode();
    
    return (immode == EPinyin && aKey >= EPtiKey2 && aKey <= EPtiKey9) ||
           (immode == EStroke && aKey >= EPtiKey1 && aKey <= EPtiKey6) ||
           (immode == EZhuyin && aKey >= EPtiKey0 && aKey <= EPtiKey9);
    }

// End Of File
