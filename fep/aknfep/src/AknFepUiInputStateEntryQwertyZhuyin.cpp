/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIInputStateEntryQwertyZhuyin methods.
*
*/












#include "AknFepUiInputStateEntryQwertyZhuyin.h"
#include "AknFepUIManagerStateInterface.h"

#include <PtiEngine.h>
#include <e32keys.h>
#include <PtiDefs.h>                //keys

#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag

const TInt KMaxCandidateCountFromCore = 6;

TAknFepInputStateEntryQwertyZhuyin::TAknFepInputStateEntryQwertyZhuyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBase(aOwner, aUIContainer)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
    
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if (EPtiEngineZhuyinQwerty != ptiengine->InputMode())
        {
        ptiengine->SetInputMode(EPtiEngineZhuyinQwerty);
        }
    TBool multiplePages = iOwner->PtiEngine()->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->CandidatePane()->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->CandidatePane()->ShowVerticalScrollArrows(multiplePages);
        
    ptiengine->SetCandidatePageLength(MAknFepUICtrlContainerChinese::ELayoutInput);
    iOwner->PtiEngine()->SetCase(EPtiCaseLower);
    _LIT(KToneNoneZhuyin, "");
    inputPane->SetToneMark(KToneNoneZhuyin);

    UpdateIndicator();
    }

TBool TAknFepInputStateEntryQwertyZhuyin::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    TBool ret = ETrue;
    
    if((aKey == EPtiKey3 || 
        aKey == EPtiKey4 || 
        aKey == EPtiKey6 || 
        aKey == EPtiKey7 ||
        aKey == EStdKeySpace) && 
        (aLength == EShortKeyPress))
        {
        ptiengine->CancelTimerActivity();
        if(ptiengine->SetToneMark(aKey))
            {
            TBool multiplePages = ptiengine->MoreCandidatePages();
            TInt bufLength = ptiengine->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
            
            UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
            UIContainer()->ShowVerticalScrollArrows(ptiengine->MoreCandidatePages());
            SetInputPaneToneMark();
            RefreshUI();
            }
        }
    else if(aKey == EStdKeyBackspace)
        {
		if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            TBool multiplePages = ptiengine->MoreCandidatePages();
            TInt bufLength = ptiengine->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
            
            UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
            uiContainer->ShowVerticalScrollArrows(ptiengine->MoreCandidatePages());
            SetInputPaneToneMark();
            RefreshUI();
            }
        else
            {
            iOwner->FepMan()->TryCloseUiL();
            if (aLength == ELongKeyPress)
                {
                iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                }
            }
        }
    else if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        ptiengine->CancelTimerActivity();
        ptiengine->AppendKeyPress((TPtiKey)aKey);
        
        TBool multiplePages = ptiengine->MoreCandidatePages();
        TInt bufLength = ptiengine->CandidatePage().Length();
        TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
        
        UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
        uiContainer->ShowVerticalScrollArrows(ptiengine->MoreCandidatePages());
        SetInputPaneToneMark();
        RefreshUI();
        }
    else if(aKey == EStdKeyRightArrow || aKey == EStdKeyLeftArrow) 
        {
        ptiengine->CancelTimerActivity();
        iOwner->ChangeState(ECandidate);
        ret = EFalse;
        }
    else if(aKey == EStdKeyDevice1)
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else if(!HandleVerticalNavigation(aKey))
        {
        // it may be one of the 'valid' numbers..
        TInt index = MapKeyToIndex(aKey);
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

        if(candidatePane->SelectIndex(index) || aKey == EStdKeyDevice3 || aKey == EStdKeyEnter)
            {
            TPtrC text = candidatePane->CurrentCandidate();
            if(text.Length())
                {            
                MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                fepMan->NewCharacterL(text);
                fepMan->CommitInlineEditL();
                iOwner->PtiEngine()->SetPredictiveChineseChar(text);
                if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                    {
                    fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                    iOwner->FepMan()->TryCloseUiL();
                    }
                else
                    {
                    iOwner->ChangeState(EPredictiveCandidate);
                    }
                }
            else
                {
                // No candidates available. Back to Entry state.
                iOwner->ChangeState(EEntry);
                }
            }
        }
    return ret;
    }

void TAknFepInputStateEntryQwertyZhuyin::KeyTimerExpired()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtrC candidates = ptiengine->CandidatePage();
    TText toneMark;
    TBool toneMarkEntered = ptiengine->ToneMark(toneMark);
    TBool toneMarkValid = ptiengine->IsToneMarkValidForSpelling();
    if(!candidates.Length() || (toneMarkEntered && !toneMarkValid))
        {
        MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
        ptiengine->DeleteKeyPress();
        SetInputPaneToneMark();

        TBool multiplePages = ptiengine->MoreCandidatePages();
        TInt bufLength = ptiengine->CandidatePage().Length();
        TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
        
        UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
        uiContainer->ShowVerticalScrollArrows(ptiengine->MoreCandidatePages());
        RefreshUI();
        }
    }
    
void TAknFepInputStateEntryQwertyZhuyin::RefreshUI()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    // set container's position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }
 
    TPtrC candidates = ptiengine->CandidatePage();
    uiContainer->CandidatePane()->SetCandidateBuffer(candidates);
    uiContainer->Enable(ETrue);
    uiContainer->CandidatePane()->SelectFirst();
    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);
    UpdateIndicator();
   }

void TAknFepInputStateEntryQwertyZhuyin::UpdateIndicator()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
        
    if (ptiengine->NumberOfCandidates() == 0)
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
    
    uiContainer->CandidatePane()->ShowDownScrollArrows
        (bufLength == KMaxCandidateCountFromCore ? ETrue : EFalse);
        
    if (candidatePane->SelectedIndex() == 0 && 
        ptiengine->NumberOfCandidates() == 0)
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
        }
    else
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
        }
        
    if (bufLength != KMaxCandidateCountFromCore &&
        candidatePane->SelectedIndex() == (bufLength -1))
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);        
        }
    else
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
        }
    }

TBool TAknFepInputStateEntryQwertyZhuyin::HandleVerticalNavigation(TInt aKey)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (aKey == EStdKeyDownArrow || aKey == EStdKeySpace)
        {
        if (iOwner->PtiEngine()->CandidatePage().Length() != 
            KMaxCandidateCountFromCore)
            {
            return ETrue;
            }
        ptiengine->NextCandidatePage();
        }
    else if (aKey == EStdKeyUpArrow)
        {
        if (ptiengine->NumberOfCandidates() == 0)
            {								 
            return ETrue;
            }
        ptiengine->PreviousCandidatePage();
        }
    else
        {
        return EFalse;
        }

    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    candidatePane->SelectFirst();
    candidatePane->SetCandidateBuffer(ptiengine->CandidatePage());
    UpdateIndicator();    
    return ETrue;
    }
        
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryQwertyZhuyin::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryQwertyZhuyin::HandleCommandL( TInt aCommandId )
    {
    TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
    }
// End of file
