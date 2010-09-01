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


#include <peninputsplititutnew.rsg>
#include <peninputsplititutconfig_chinese.rsg>
#include <peninputdropdownlist.h>
#include <coemain.h>
#include <AknFepGlobalEnums.h>
#include <PtiEngine.h>

#include "peninputsplititutchnuistatepredict.h"
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutconverter.h"

const TInt KInvalidIndex = -1;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::NewL
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStatePredict* CChnSplitItutUiStatePredict::NewL(CSplitItutUiMgrBase* aOwner)
    {
    CChnSplitItutUiStatePredict* self = new (ELeave) CChnSplitItutUiStatePredict(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::~CChnSplitItutUiStatePredict
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStatePredict::~CChnSplitItutUiStatePredict()
    {
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::CChnSplitItutUiStatePredict
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStatePredict::CChnSplitItutUiStatePredict(CSplitItutUiMgrBase* aOwner)
                                                          :CSplitItutUiStateBase(aOwner),
                                                           iLastOverlapIdx(KInvalidIndex)
    {
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::OnEntryL
// ---------------------------------------------------------------------------
//
void CChnSplitItutUiStatePredict::OnEntryL()
    {
    /*
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
    CFepCtrlDropdownList* punclist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdPuncCandsList));
    punclist->Hide(EFalse);

    iOwner->DataMgr()->ClearChnCandidates(EItutPuncCandidates);
    RPointerArray<HBufC>& punccandslist = 
        TItutDataConverter::AnyToRptrArray(iOwner->DataMgr()->RequestData(EChnPuncCandidates));
    TItutDataConverter::ConvertNonPhraseCandidateL(*puncText, punccandslist);

    punclist->SetCandidatesL(punccandslist, CFepCtrlDropdownList::EListNoExpandableWithBubble);
    delete puncText;

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
    */      
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::OnEntryL
// ---------------------------------------------------------------------------
//
void CChnSplitItutUiStatePredict::OnExit()
    {
    /*
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
    */
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::HandleKeyL
// ---------------------------------------------------------------------------
//
TBool CChnSplitItutUiStatePredict::HandleKeyL(TInt aCmd, TInt aKey)
    {
    TInt immode = iOwner->DataMgr()->InputMode();

    if (IsKeyValid(aKey))
        {
        iOwner->SetNumLongPressValid(ETrue);
        iOwner->SetCurrentStateL(CSplitItutUiMgrBase::EStateComposition);
        iOwner->CurrentState()->HandleKeyL(aCmd, aKey);
        return ETrue;
        }
    else if ( ( immode == EPinyin && 
         ( aKey == EPtiKey0 || aKey == EPtiKey1 || aKey == EPtiKeyStar ) ) ||
         ( immode == EStroke && 
         ( aKey == EPtiKeyStar || aKey == EPtiKey0 || (aKey >= EPtiKey7 && aKey <= EPtiKey9 ))) ||
         ( immode == EZhuyin && aKey == EPtiKeyStar) ||
         ( aKey == EKeyBackspace))
        {
        iOwner->SetCurrentStateL(CSplitItutUiMgrBase::EStateStandby); 
        }
    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::StateType
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase::TUiState CChnSplitItutUiStatePredict::StateType()
    {
    return CSplitItutUiMgrBase::EStatePredict;
    }
    
// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::IsKeyValid
// ---------------------------------------------------------------------------
//
TBool CChnSplitItutUiStatePredict::IsKeyValid(TInt aKey)
    {
    TInt immode = iOwner->DataMgr()->InputMode();
    
    return (immode == EPinyin && aKey >= EPtiKey2 && aKey <= EPtiKey9) ||
           (immode == EStroke && aKey >= EPtiKey1 && aKey <= EPtiKey6) ||
           (immode == EZhuyin && aKey >= EPtiKey0 && aKey <= EPtiKey9);
    }
    
// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::HandleCtrlEventL
// ---------------------------------------------------------------------------
//
TBool CChnSplitItutUiStatePredict::HandleCtrlEventL(TInt /*aEventType*/, 
                                               CFepUiBaseCtrl* /*aCtrl*/, 
                                               const TDesC& /*aEventData*/)
    {
    /*
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
                iOwner->SetCurrentStateL(CSplitItutUiMgrBase::EStateStandby);

                // ui layout may also need to handle this event
                return EFalse;
                }
            }
            break;
        case EEventIcfPointerUpEvent:
        	{
            if (aCtrl->ControlId() == ECtrlIdICF)
                {
                iOwner->SetCurrentStateL(CSplitItutUiMgrBase::EStateStandby);

                // ui layout may also need to handle this event
                return ETrue;
                }
        	}
        	break;            
        default:
            break;
        }
    */
    return EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePredict::RemoveDuplicateCand
// ---------------------------------------------------------------------------
//
TInt CChnSplitItutUiStatePredict::RemoveDuplicateCand(const RPointerArray<HBufC>& aSrc,
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
