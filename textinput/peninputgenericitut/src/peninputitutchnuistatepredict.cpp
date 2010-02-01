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


#include <peninputgeneralitutnew.rsg>
#include <peninputitutconfig_chinese.rsg>
#include <peninputdropdownlist.h>
#include <coemain.h>
#include <AknFepGlobalEnums.h>
#include <PtiEngine.h>

#include "peninputitutchnuistatepredict.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutconverter.h"
#include "peninputgenericitutwindowmanager.h"

const TInt KInvalidIndex = -1;

CChnItutUiStatePredict* CChnItutUiStatePredict::NewL(CGenericItutUiMgrBase* aOwner)
    {
    CChnItutUiStatePredict* self = new (ELeave) CChnItutUiStatePredict(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CChnItutUiStatePredict::~CChnItutUiStatePredict()
    {
    }

CChnItutUiStatePredict::CChnItutUiStatePredict(CGenericItutUiMgrBase* aOwner)
                                                          :CGenericItutUiStateBase(aOwner),
                                                           iLastOverlapIdx(KInvalidIndex)
    {
    }

void CChnItutUiStatePredict::OnEntryL()
    {
    HBufC* puncText = NULL;
    switch (iOwner->DataMgr()->InputMode())
        {
        case EPinyin:
            {
            iOwner->DataMgr()->NotifyChangeControlLayout(
                MItutPropertySubscriber::EItutPropertyKeypadResourceId,
                R_FINGER_INPUT_KEYPAD_PINYIN_STANDBY_PREDICT);
            puncText = CCoeEnv::Static()->AllocReadResourceL(R_ITUT_PUNC_PINYIN_ZHUYIN);             
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
            puncText = CCoeEnv::Static()->AllocReadResourceL(R_ITUT_PUNC_STROKE);	
            }
            break;        
        case EZhuyin:
            {
            iOwner->DataMgr()->NotifyChangeControlLayout(
                MItutPropertySubscriber::EItutPropertyKeypadResourceId,
                R_FINGER_INPUT_KEYPAD_ZHUYIN_STANDBY_PREDICT);
            puncText = CCoeEnv::Static()->AllocReadResourceL(R_ITUT_PUNC_PINYIN_ZHUYIN);
            }
            break;        
        default:
            {
            // wrong input mode	
            return;
            }
        }

    // show punc drop list, and set candidates to it
    CleanupStack::PushL( puncText );
    CFepCtrlDropdownList* punclist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdPuncCandsList));
    punclist->Hide(EFalse);

    iOwner->DataMgr()->ClearChnCandidates(EItutPuncCandidates);
    RPointerArray<HBufC>& punccandslist = 
        TItutDataConverter::AnyToRptrArray(iOwner->DataMgr()->RequestData(EChnPuncCandidates));
    TItutDataConverter::ConvertNonPhraseCandidateL(*puncText, punccandslist);

    punclist->SetCandidatesL(punccandslist, CFepCtrlDropdownList::EListNoExpandableWithBubble);
    CleanupStack::PopAndDestroy( puncText );

    // show candidate drop list
    CFepCtrlDropdownList* candlist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    candlist->Hide(EFalse);

    // may need to change resource id of candidte list to predictive, change keypad resource id if need
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyCandidateListResourceId,
        R_AKN_FEP_NORMAL_CAND_DROP_DOWN_LIST);
        
        
    //hide navi button
    iOwner->LayoutContext()->ShowArrowBtn(0);
    
    //hide indicator 
    iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(ETrue);        
    }

void CChnItutUiStatePredict::OnExit()
    {
    // hide dropdownlist & reset its status
    CFepCtrlDropdownList* punclist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdPuncCandsList));
    punclist->ResetAndClear(CFepCtrlDropdownList::EListNoExpandable);
    iOwner->DataMgr()->ClearChnCandidates(EItutPuncCandidates);
    punclist->Hide(ETrue);

    CFepCtrlDropdownList* candlist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    candlist->ResetAndClear(CFepCtrlDropdownList::EListExpandable);
    iOwner->DataMgr()->ClearChnCandidates(EItutCandidates);
    candlist->Hide(ETrue);
    
    //show navi button
    iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight
             | EBtnArrowUp| EBtnArrowDown);
    
    //show indicator 
    iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(EFalse);
    
    }

TBool CChnItutUiStatePredict::HandleKeyL(TInt aCmd, TInt aKey)
    {
    TInt immode = iOwner->DataMgr()->InputMode();

    if (IsKeyValid(aKey))
        {
        iOwner->SetNumLongPressValid(ETrue);
        iOwner->SetCurrentStateL(CGenericItutUiMgrBase::EStateComposition);
        iOwner->CurrentState()->HandleKeyL(aCmd, aKey);
        return ETrue;
        }
    return EFalse;
    }

CGenericItutUiMgrBase::TUiState CChnItutUiStatePredict::StateType()
    {
    return CGenericItutUiMgrBase::EStatePredict;
    }
    
TBool CChnItutUiStatePredict::IsKeyValid(TInt aKey)
    {
    TInt immode = iOwner->DataMgr()->InputMode();
    
    return (immode == EPinyin && aKey >= EPtiKey2 && aKey <= EPtiKey9) ||
           (immode == EStroke && aKey >= EPtiKey1 && aKey <= EPtiKey6) ||
           (immode == EZhuyin && aKey >= EPtiKey0 && aKey <= EPtiKey9);
    }
    
TBool CChnItutUiStatePredict::HandleCtrlEventL(TInt aEventType, 
                                               CFepUiBaseCtrl* aCtrl, 
                                               const TDesC& aEventData)
    {
    switch (aEventType)
        {
        case EItutCmdCandidateSelected:
            {
            TPtrC buf = aEventData.Left(aEventData.Length() - 1);

            if (aCtrl->ControlId() == ECtrlIdStdCandsList)
                {
                // if press cell on candidate list, set predictive candidates of it
                iOwner->DataMgr()->PtiEngine()->SetPredictiveChineseChar(buf);
                TPtrC cands = iOwner->DataMgr()->PtiEngine()->CandidatePage();

                iOwner->DataMgr()->ClearChnCandidates(EItutCandidates);

                RPointerArray<HBufC>& candslist = TItutDataConverter::AnyToRptrArray(
                    iOwner->DataMgr()->RequestData(EChnCandidates));
                TItutDataConverter::ConvertChnPhraseCandidateL(cands, candslist);

                // remove duplicate punctuation from predict candidate
                RPointerArray<HBufC>& punccandslist = TItutDataConverter::AnyToRptrArray(
                    iOwner->DataMgr()->RequestData(EChnPuncCandidates));
                iLastOverlapIdx = RemoveDuplicateCand(punccandslist,
                                                      candslist,
                                                      0,
                                                      0);

                CFepCtrlDropdownList* canddroplist = 
                    reinterpret_cast<CFepCtrlDropdownList*>(aCtrl);
                canddroplist->SetCandidatesL(candslist, CFepCtrlDropdownList::EListExpandableWithBubble);

                // input pressed candidate
                iOwner->LayoutContext()->SubmitText(buf);

                return ETrue;
                }
            else if (aCtrl->ControlId() == ECtrlIdPuncCandsList)
                {
                // if press cell on punc drop list, input it directly
                iOwner->LayoutContext()->SubmitText(buf);
                
                return ETrue;
                }
            }
            break;
        case EItutCmdGetNextCandidatePage:
            {
            if (aCtrl->ControlId() == ECtrlIdStdCandsList)
                {
                RPointerArray<HBufC>& candslist = TItutDataConverter::AnyToRptrArray(
                    iOwner->DataMgr()->RequestData(EChnCandidates));

                if (candslist.Count() >= KMaxPredictCandCnt)
                    {
                    return ETrue;
                    }

                // original candidate count before fetch new candidate
                TInt candscnt = candslist.Count();
                if (iOwner->DataMgr()->GetNextPageCandidateL(candslist) == KErrNone)
                    {
                    // remove duplicate punctuation from predict candidate
                    RPointerArray<HBufC>& punccandslist = TItutDataConverter::AnyToRptrArray(
                        iOwner->DataMgr()->RequestData(EChnPuncCandidates));
                    iLastOverlapIdx = RemoveDuplicateCand(punccandslist,
                                                          candslist,
                                                          iLastOverlapIdx + 1,
                                                          candscnt);

                    if (candslist.Count() >= KMaxPredictCandCnt)
                        {
                        candscnt = candslist.Count();
                        for (TInt i = KMaxPredictCandCnt; i < candscnt; i++)
                            {
                            delete candslist[KMaxPredictCandCnt];
                            candslist.Remove(KMaxPredictCandCnt);
                            }
                        }

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
                RPointerArray<HBufC>& candslist = TItutDataConverter::AnyToRptrArray(
                    iOwner->DataMgr()->RequestData(EChnCandidates));

                if (candslist.Count() >= KMaxPredictCandCnt)
                    {
                    static_cast<CFepCtrlDropdownList*>(aCtrl)->SetFlagCandidateExist(EFalse);
                    }
                else
                    {
                    static_cast<CFepCtrlDropdownList*>(aCtrl)->SetFlagCandidateExist(
                        iOwner->DataMgr()->NextPageCandidateExist());
                    }
                
                return ETrue;
                }
            }
            break;
        case EEventControlFocusGained:
            {
            if (aCtrl->ControlId() == ECtrlIdICF)
                {
                iOwner->SetCurrentStateL(CGenericItutUiMgrBase::EStateStandby);

                // ui layout may also need to handle this event
                return EFalse;
                }
            }
            break;
        case EEventIcfPointerUpEvent:
        	{
            if (aCtrl->ControlId() == ECtrlIdICF)
                {
                iOwner->SetCurrentStateL(CGenericItutUiMgrBase::EStateStandby);

                // ui layout may also need to handle this event
                return ETrue;
                }
        	}
        	break;
        case EEventRawKeyDownEvent:
        	{
        	TInt immode = iOwner->DataMgr()->InputMode();
 			const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            if ( ( immode == EPinyin && 
                 ( key->iScanCode == EPtiKey0 || key->iScanCode == EPtiKey1 || key->iScanCode == EPtiKeyStar ) ) ||
                 ( immode == EStroke && 
                 ( key->iScanCode == EPtiKeyStar || key->iScanCode == EPtiKey0 || (key->iScanCode >= EPtiKey7 && key->iScanCode <= EPtiKey9 ))) ||
                 ( immode == EZhuyin && key->iScanCode == EPtiKeyStar) ||
                 ( key->iScanCode == EStdKeyBackspace))
                {                
				// For those special keys, simulate down key immedidately.
                // to ensure to simulate down key event before up key event
                iOwner->UiManager()->SimulateImmeRawEvent( key->iScanCode, TRawEvent::EKeyDown );
                iOwner->UiManager()->SetLastRawKeyDown( key->iScanCode, ETrue, aCtrl );               			
				return ETrue;                               
                }        	
        	}
        	break;
        case EEventRawKeyUpEvent:
        	{
        	TInt immode = iOwner->DataMgr()->InputMode();
 			const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            if ( ( immode == EPinyin && 
                 ( key->iScanCode == EPtiKey0 || key->iScanCode == EPtiKey1 || key->iScanCode == EPtiKeyStar ) ) ||
                 ( immode == EStroke && 
                 ( key->iScanCode == EPtiKeyStar || key->iScanCode == EPtiKey0 || (key->iScanCode >= EPtiKey7 && key->iScanCode <= EPtiKey9 ))) ||
                 ( immode == EZhuyin && key->iScanCode == EPtiKeyStar) ||
                 ( key->iScanCode == EStdKeyBackspace))
                {
				// For those special keys, simulate up key events immediately before changing state.
                // The action can avoid up key event being delayed by running changing state,
				// otherwise, short press will be thought as long press by window server.
				iOwner->UiManager()->SimulateImmeRawEvent( key->iScanCode, TRawEvent::EKeyUp );
				iOwner->UiManager()->SetLastRawKeyDown( key->iScanCode, EFalse, aCtrl );                
				iOwner->SetCurrentStateL( CGenericItutUiMgrBase::EStateStandby );
				return ETrue;                          
                }
        	}
        	break;
        default:
            break;
        }
    
    return EFalse;    
    }
    
TInt CChnItutUiStatePredict::RemoveDuplicateCand(const RPointerArray<HBufC>& aSrc,
                                                 RPointerArray<HBufC>& aTgt,
                                                 TInt aSrcStartIdx,
                                                 TInt aTgtStartIdx)
    {
    TInt lastOverlapIdx = KInvalidIndex;

    // on the assumption that if candidate overlap,
    // it is overlap only once
    TInt srccandcnt = aSrc.Count();

    for (TInt i = aSrcStartIdx; i < srccandcnt; i++)
        {
        for (TInt j = aTgtStartIdx; j < aTgt.Count(); j++)
            {
            if (aTgt[j]->Compare(*(aSrc[i])) == 0)
                {
                lastOverlapIdx = i;
                delete aTgt[j];
                aTgt.Remove(j);
                j--;
                break;
                }
            }
        }

    return lastOverlapIdx;
    }

// End Of File
