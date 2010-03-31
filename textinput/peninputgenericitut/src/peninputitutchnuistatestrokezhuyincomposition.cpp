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


#include <peninputinputcontextfield.h>
#include <PtiEngine.h>
#include <peninputdropdownlist.h>
#include <peninputcmdparam.h>
#include <peninputgeneralitutnew.rsg>
#include <peninputitutconfig_chinese.rsg>
#include <AknFepGlobalEnums.h>

#include "peninputitutchnuistatestrokezhuyincomposition.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutconverter.h"

CChnItutUiStateStrokeZhuyinComposition* CChnItutUiStateStrokeZhuyinComposition::NewL(
    CGenericItutUiMgrBase* aOwner)
    {
    CChnItutUiStateStrokeZhuyinComposition* self = 
        new (ELeave) CChnItutUiStateStrokeZhuyinComposition(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CChnItutUiStateStrokeZhuyinComposition::~CChnItutUiStateStrokeZhuyinComposition()
    {
    }

CChnItutUiStateStrokeZhuyinComposition::CChnItutUiStateStrokeZhuyinComposition(
    CGenericItutUiMgrBase* aOwner):CGenericItutUiStateBase(aOwner)
    {
    }

void CChnItutUiStateStrokeZhuyinComposition::OnEntryL()
    {
    TInt immode = iOwner->DataMgr()->InputMode();

    if (immode == EStroke)
        {
        TInt resID;
        if (iOwner->DataMgr()->InputLanguage() == ELangPrcChinese)
            {
            resID = R_FINGER_INPUT_KEYPAD_STROKE_COMPOSITION_PRC;
            }
        else
            {
            resID = R_FINGER_INPUT_KEYPAD_STROKE_COMPOSITION_TRA;
            }        
        iOwner->DataMgr()->NotifyChangeControlLayout(
            MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
            resID);
        }
    else if (immode == EZhuyin)
        {
        iOwner->DataMgr()->NotifyChangeControlLayout(
            MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
            R_FINGER_INPUT_KEYPAD_ZHUYIN_COMPOSITION);
        }

    // update resource id of keypad, candidate drop list
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyCandidateListResourceId,
        R_AKN_FEP_NORMAL_CAND_DROP_DOWN_LIST);

    // show spell
    CFepInputContextField* spellctrl = 
        static_cast<CFepInputContextField*>(iOwner->LayoutContext()->Control(ECtrlIdSpellICF));
    TFepInputContextFieldData spelldata;
    spelldata.iCmd = EPeninputICFInitial;
    spelldata.iStartPos = 0;
    spelldata.iLength = 0;
    spelldata.iText.Set( KNullDesC );
    spelldata.iCurSel = TCursorSelection(0, 0);
    spelldata.iCursorVisibility = ETrue;
    spelldata.iCursorSelVisible = ETrue;
    spellctrl->SetTextL(spelldata);
    spellctrl->Hide(EFalse);

    // show candidate drop list
    CFepCtrlDropdownList* candlist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    candlist->Hide(EFalse);
    
    //hide navi button
    iOwner->LayoutContext()->ShowArrowBtn(0);
    
    //hide indicator 
    iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(ETrue);
    
    // Show ICF, Backspace after position changed to avoid flick
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( EFalse );
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide( EFalse );
    }

void CChnItutUiStateStrokeZhuyinComposition::OnExit()
    {
    // hide dropdownlist & reset its status
    CFepCtrlDropdownList* candlist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    candlist->ResetAndClear(CFepCtrlDropdownList::EListExpandable);
    iOwner->DataMgr()->ClearChnCandidates(EItutCandidates);
    candlist->Hide(ETrue);

    CFepInputContextField* spellctrl = 
        static_cast<CFepInputContextField*>(iOwner->LayoutContext()->Control(ECtrlIdSpellICF));

    spellctrl->Hide(ETrue);

    //show indicator 
    iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(EFalse);
    // Hide ICF, Backspace, Arrow contrls when exit to avoid flick
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( ETrue );
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide(ETrue);
    iOwner->LayoutContext()->ShowArrowBtn(0);
    iOwner->DataMgr()->PtiEngine()->ClearCurrentWord();
    }

TBool CChnItutUiStateStrokeZhuyinComposition::HandleKeyL(TInt /*aCmd*/, TInt aKey)
    {
    TInt immode = iOwner->DataMgr()->InputMode();
    
    if (immode != EStroke && immode != EZhuyin)
        {
        return EFalse;
        }

    if (!(aKey == EKeyBackspace || 
         (aKey >= EPtiKey0 && aKey <= EPtiKey9) ||
         (aKey == EPtiKeyStar)))
        {
        return EFalse;
        }
    
    if (aKey == EKeyBackspace)
        {
        if (!iOwner->DataMgr()->PtiEngine()->DeleteKeyPress().Length())
            {
            iOwner->SetCurrentStateL(CGenericItutUiMgrBase::EStateStandby);
            
            iOwner->SetClrConsume(ETrue);
            
            return ETrue;
            }
        } 
    else if (aKey == EPtiKeyStar)
        {
        if (immode == EStroke)
            {
            //iOwner->DataMgr()->PtiEngine()->ClearCurrentWord();
            //iOwner->SetCurrentStateL(CGenericItutUiMgrBase::EStateStandby);
            
            return ETrue;
            }
        else
            {
            // input mode is zhuyin
            // input tonemark
            if (!iOwner->DataMgr()->PtiEngine()->IncrementToneMark(ETrue))
                {
                // tonemark did not set correctly, everything unchanged
                return ETrue;
                }
            }
        }
    else
        {
        if (immode == EZhuyin ||
           (immode == EStroke && aKey >= EPtiKey0 && aKey <= EPtiKey6))
            {
            // append keypress
            iOwner->DataMgr()->PtiEngine()->AppendKeyPress(TPtiKey(aKey));
            } 
        else if (immode == EStroke && aKey >= EPtiKey7 && aKey <= EPtiKey9)
            {
            // just ignore	
            return ETrue;
            }
        }

    // display 1st spell, ptiengine's phonetic spelling start index is 1, not 0
    iOwner->DataMgr()->PtiEngine()->SelectPhoneticSpelling(1);

    CFepInputContextField* spellctrl = 
        static_cast<CFepInputContextField*>(iOwner->LayoutContext()->Control(ECtrlIdSpellICF));
    TPtrC spelltext = iOwner->DataMgr()->PtiEngine()->GetPhoneticSpelling(1);

    TFepInputContextFieldData spelldata;

    spelldata.iCmd = EPeninputICFInitial;
    spelldata.iStartPos = 0;
    spelldata.iLength = spelltext.Length();
    spelldata.iMidPos = -1;
    spelldata.iText.Set(spelltext);

    spelldata.iCurSel = TCursorSelection(spelltext.Length(), spelltext.Length());
    spelldata.iCursorVisibility = EFalse;
    spelldata.iCursorSelVisible = EFalse;

    spellctrl->SetTextL(spelldata);

    // get candidates of 1st spell 
    TPtrC cands = iOwner->DataMgr()->PtiEngine()->CandidatePage();

    iOwner->DataMgr()->ClearChnCandidates(EItutCandidates);
    RPointerArray<HBufC>& candslist = 
        TItutDataConverter::AnyToRptrArray(iOwner->DataMgr()->RequestData(EChnCandidates));
    TItutDataConverter::ConvertChnPhraseCandidateL(cands, candslist);

    CFepCtrlDropdownList* canddroplist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    canddroplist->SetCandidatesL(candslist, CFepCtrlDropdownList::EListExpandableWithBubble);

    return ETrue;
    }

CGenericItutUiMgrBase::TUiState CChnItutUiStateStrokeZhuyinComposition::StateType()
    {
    return CGenericItutUiMgrBase::EStateComposition;
    }
    
TBool CChnItutUiStateStrokeZhuyinComposition::HandleCtrlEventL(TInt aEventType, 
                                                               CFepUiBaseCtrl* aCtrl, 
                                                               const TDesC& aEventData)
    {
    switch (aEventType)
        {
        case EItutCmdCandidateSelected:
            {
            if (aCtrl->ControlId() == ECtrlIdStdCandsList)
                {
                iOwner->SetCurrentStateL(CGenericItutUiMgrBase::EStatePredict);
                return iOwner->CurrentState()->HandleCtrlEventL(aEventType,
                                                                aCtrl,
                                                                aEventData);
                }
            }
            break;
        case EItutCmdGetNextCandidatePage:
            {
            if (aCtrl->ControlId() == ECtrlIdStdCandsList)
                {
                RPointerArray<HBufC>& candslist = TItutDataConverter::AnyToRptrArray(
                    iOwner->DataMgr()->RequestData(EChnCandidates));

                // original candidate count before fetch new candidate
                TInt candscnt = candslist.Count();
                if (iOwner->DataMgr()->GetNextPageCandidateL(candslist) == KErrNone)
                    {
                    static_cast<CFepCtrlDropdownList*>(aCtrl)->AddCandidatesL(candslist);    
                    }

                return ETrue;
                }
            }
            break;
        case EItutCmdCandidateExisted:
            {
            if (aCtrl->ControlId() == ECtrlIdStdCandsList)
                {
                static_cast<CFepCtrlDropdownList*>(aCtrl)->SetFlagCandidateExist(
                    iOwner->DataMgr()->NextPageCandidateExist());
                return ETrue;
                }
            }
            break;
        // consume star key at zhuyin composition state
        case EEventRawKeyDownEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            
            if (key->iScanCode == EStdKeyNkpAsterisk && iOwner->DataMgr()->InputMode() == EZhuyin)
                {
                HandleKeyL(ECmdPenInputFingerKeyPress, EPtiKeyStar);
                return ETrue;
                }
            }
            break;
        case EEventRawKeyUpEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            
            if (key->iScanCode == EStdKeyNkpAsterisk && iOwner->DataMgr()->InputMode() == EZhuyin)
                {
                return ETrue;
                }
            }
            break;
        default:
            break;
        }

    return EFalse;    
    }

// End Of File
