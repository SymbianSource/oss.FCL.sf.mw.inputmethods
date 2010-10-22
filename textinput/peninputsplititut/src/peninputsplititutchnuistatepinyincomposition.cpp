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


#include <peninputdropdownlist.h>
#include <PtiEngine.h>
#include <peninputsplititutnew.rsg>
#include <peninputsplititutconfig_chinese.rsg>

#include "peninputsplititutchnuistatepinyincomposition.h"
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutconverter.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePinyinComposition::NewL
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStatePinyinComposition* CChnSplitItutUiStatePinyinComposition::NewL(
    CSplitItutUiMgrBase* aOwner)
    {
    CChnSplitItutUiStatePinyinComposition* self = 
        new (ELeave) CChnSplitItutUiStatePinyinComposition(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePinyinComposition::~CChnSplitItutUiStatePinyinComposition
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStatePinyinComposition::~CChnSplitItutUiStatePinyinComposition()
    {
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePinyinComposition::CChnSplitItutUiStatePinyinComposition
// ---------------------------------------------------------------------------
//
CChnSplitItutUiStatePinyinComposition::CChnSplitItutUiStatePinyinComposition(
    CSplitItutUiMgrBase* aOwner):CSplitItutUiStateBase(aOwner)
    {
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePinyinComposition::OnEntryL
// ---------------------------------------------------------------------------
//
void CChnSplitItutUiStatePinyinComposition::OnEntryL()
    {
    // update resource id of keypad, candidate drop list
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
        R_FINGER_INPUT_KEYPAD_PINYIN_COMPOSITION);
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyCandidateListResourceId,
        R_AKN_FEP_PINYIN_CAND_DROP_DOWN_LIST);

    // show spell drop list
    CFepCtrlDropdownList* spelllist = static_cast<CFepCtrlDropdownList*>(
        iOwner->LayoutContext()->Control(ECtrlIdSpellCandsList));
    spelllist->Hide(EFalse);

    // show candidate drop list
    CFepCtrlDropdownList* candlist = static_cast<CFepCtrlDropdownList*>(
        iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    candlist->Hide(EFalse);
    
    //hide navi button
    iOwner->LayoutContext()->ShowArrowBtn(0);  
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePinyinComposition::OnExit
// ---------------------------------------------------------------------------
//
void CChnSplitItutUiStatePinyinComposition::OnExit()
    {
    /*
    // hide dropdownlist & reset its status
    CFepCtrlDropdownList* spelllist = static_cast<CFepCtrlDropdownList*>(
        iOwner->LayoutContext()->Control(ECtrlIdSpellCandsList));
    spelllist->ResetAndClear(CFepCtrlDropdownList::EListExpandableRollWithIcon);
    iSpellCands.ResetAndDestroy();
    iSpellCands.Close();
    spelllist->Hide(ETrue);

    CFepCtrlDropdownList* candlist = static_cast<CFepCtrlDropdownList*>(
        iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    candlist->ResetAndClear(CFepCtrlDropdownList::EListExpandable);
    iOwner->DataMgr()->ClearChnCandidates(EItutCandidates);
    candlist->Hide(ETrue);

    iOwner->DataMgr()->PtiEngine()->ClearCurrentWord();
    
    //show navi button
    iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight
             | EBtnArrowUp| EBtnArrowDown);
    
    //show indicator 
    iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(EFalse);

    iOwner->DataMgr()->PtiEngine()->ClearCurrentWord();
    */
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePinyinComposition::HandleKeyL
// ---------------------------------------------------------------------------
//
TBool CChnSplitItutUiStatePinyinComposition::HandleKeyL(TInt /*aCmd*/, TInt /*aKey*/)
    {
    /*
    if (!(aKey == EKeyBackspace || 
        (aKey >= EPtiKey0 && aKey <= EPtiKey9) || 
        (aKey == EPtiKeyStar)))
        {
        return EFalse;
        }

    if (aKey >= EPtiKey0 && aKey <= EPtiKey9)
        {
        // append keypress 
        iOwner->DataMgr()->PtiEngine()->AppendKeyPress(TPtiKey(aKey));
        }
    else if (aKey == EKeyBackspace)
        {
        if (!iOwner->DataMgr()->PtiEngine()->DeleteKeyPress().Length())
            {
            iOwner->SetCurrentStateL(CSplitItutUiMgrBase::EStateStandby);
            
            iOwner->SetClrConsume(ETrue);

            return ETrue;
            }
        }
    else{
         // input tonemark
        if (!iOwner->DataMgr()->PtiEngine()->IncrementToneMark(ETrue))
            {
            // tonemark did not set correctly, everything unchanged
            return ETrue;
            }
        }

    // get all spelling
    TInt spellcnt = iOwner->DataMgr()->PtiEngine()->PhoneticSpellingCount();

    iSpellCands.ResetAndDestroy();
    iSpellCands.Close();

    // note: ptiengine's phonetic spelling start index is 1, not 0
    for (TInt i = 1; i < (spellcnt + 1); i++)
        {
        HBufC* spellbuf = (iOwner->DataMgr()->PtiEngine()->GetPhoneticSpelling(i)).AllocL();

        CleanupStack::PushL(spellbuf);
        iSpellCands.Append(spellbuf);
        CleanupStack::Pop(spellbuf);
        }

    TInt nearestIdx = 0;

    
    if (iLastSelSpell)
        {
        nearestIdx = GetNearestSpell(iLastSelSpell->Length());

        delete iLastSelSpell;
        iLastSelSpell = NULL;
        }

    iLastSelSpell = (iOwner->DataMgr()->PtiEngine()->GetPhoneticSpelling(nearestIdx + 1)).AllocL();
    

    // set spell, set highlight of spelling candidate list
    CFepCtrlDropdownList* spelldroplist = static_cast<CFepCtrlDropdownList*>(
        iOwner->LayoutContext()->Control(ECtrlIdSpellCandsList));
    spelldroplist->SetCandidatesL(iSpellCands, CFepCtrlDropdownList::EListExpandableRollWithIconWithBubble);
    spelldroplist->SetHighlightCell(nearestIdx);

    // get candidates of nearest spell
    iOwner->DataMgr()->PtiEngine()->SelectPhoneticSpelling(nearestIdx + 1);
    TPtrC cands = iOwner->DataMgr()->PtiEngine()->CandidatePage();

    iOwner->DataMgr()->ClearChnCandidates(EItutCandidates);
    RPointerArray<HBufC>& candslist = 
        TItutDataConverter::AnyToRptrArray(iOwner->DataMgr()->RequestData(EChnCandidates));
    TItutDataConverter::ConvertChnPhraseCandidateL(cands, candslist);

    // set candidate
    CFepCtrlDropdownList* canddroplist = 
        static_cast<CFepCtrlDropdownList*>(iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
    canddroplist->SetCandidatesL(candslist, CFepCtrlDropdownList::EListExpandableWithBubble);
    */
    return ETrue;
    }

/*
TInt CChnItutUiStatePinyinComposition::GetNearestSpell(TInt aValidLastSpellLen)
    {
    if (aValidLastSpellLen <= 0)
        {
        return 0;
        }
    
    for (TInt i = 0; i < iSpellCands.Count(); i++)
        {
        if ((iSpellCands[i]->Left(aValidLastSpellLen)).Compare(iLastSelSpell->Left(aValidLastSpellLen)) == 0)
            {
            return i;
            }
        }
    
    return GetNearestSpell(aValidLastSpellLen-1);
    }
*/    

// ---------------------------------------------------------------------------
// CChnSplitItutUiStatePinyinComposition::HandleCtrlEventL
// ---------------------------------------------------------------------------
//
TBool CChnSplitItutUiStatePinyinComposition::HandleCtrlEventL(
    TInt /*aEventType*/, CFepUiBaseCtrl* /*aCtrl*/, const TDesC& /*aEventData*/)
    {
    /*
    switch (aEventType)
        {
        case EItutCmdCandidateSelected:
            {
            // press cell on spelling list
            if (aCtrl->ControlId() == ECtrlIdSpellCandsList &&
                aEventData.Length() > 0)
                {
                TInt selCandIdx = aEventData[aEventData.Length() - 1];

                // note for ptiengine, spelling index start from 1, not 0
                iOwner->DataMgr()->PtiEngine()->SelectPhoneticSpelling(selCandIdx + 1);
                
                CFepCtrlDropdownList* spelldroplist = static_cast<CFepCtrlDropdownList*>(aCtrl);
                spelldroplist->SetHighlightCell(selCandIdx);

                                 
                delete iLastSelSpell;
                iLastSelSpell = NULL;
                iLastSelSpell = (iOwner->DataMgr()->PtiEngine()->GetPhoneticSpelling(selCandIdx + 1)).AllocL();
                

                TPtrC cands = iOwner->DataMgr()->PtiEngine()->CandidatePage();

                iOwner->DataMgr()->ClearChnCandidates(EItutCandidates);
                RPointerArray<HBufC>& candslist = TItutDataConverter::AnyToRptrArray(
                    iOwner->DataMgr()->RequestData(EChnCandidates));
                TItutDataConverter::ConvertChnPhraseCandidateL(cands, candslist);

                CFepCtrlDropdownList* canddroplist = static_cast<CFepCtrlDropdownList*>(
                    iOwner->LayoutContext()->Control(ECtrlIdStdCandsList));
                canddroplist->SetCandidatesL(candslist, CFepCtrlDropdownList::EListExpandableWithBubble);

                return ETrue;
                }
            else if (aCtrl->ControlId() == ECtrlIdStdCandsList)
                {
                iOwner->SetCurrentStateL(CSplitItutUiMgrBase::EStatePredict);
                return iOwner->CurrentState()->HandleCtrlEventL(aEventType, aCtrl, aEventData);
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
                static_cast<CFepCtrlDropdownList*>(
                    aCtrl)->SetFlagCandidateExist(iOwner->DataMgr()->NextPageCandidateExist());
                return ETrue;
                }
            }
            break;
        // consume star key at pinyin composition state
        case EEventRawKeyDownEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            
            if (key->iScanCode == EStdKeyNkpAsterisk)
                {
                HandleKeyL(ECmdPenInputFingerKeyPress, EPtiKeyStar);
                return ETrue;
                }
            }
            break;
        case EEventRawKeyUpEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            
            if (key->iScanCode == EStdKeyNkpAsterisk)
                {
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
// CChnSplitItutUiStatePinyinComposition::StateType
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase::TUiState CChnSplitItutUiStatePinyinComposition::StateType()
    {
    return CSplitItutUiMgrBase::EStateComposition;
    }

// End Of File