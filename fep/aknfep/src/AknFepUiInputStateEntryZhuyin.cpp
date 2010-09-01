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
*       Provides the CAknFepUIInputStateZhuyin methods.
*
*/












#include "AknFepUiInputStateEntryZhuyin.h"
#include "AknFepUIManagerStateInterface.h"

#include <PtiEngine.h>

#include <e32keys.h>
#include <PtiDefs.h>                //keys

#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"

const TInt KMaxCandidateCountFromCore = 6;

TAknFepInputStateEntryZhuyin::TAknFepInputStateEntryZhuyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryZhuyinStrokeBase(aOwner, aUIContainer)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    uiContainer->CandidatePane()->SelectFirst(); 
    uiContainer->InputPane()->SetOverrideFontId(0);

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if (EPtiEngineZhuyin != ptiengine->InputMode())
        {
        ptiengine->SetInputMode(EPtiEngineZhuyin);
        }
    ptiengine->SetCandidatePageLength(MAknFepUICtrlContainerChinese::ELayoutInput);
    
    if (ptiengine->NumberOfCandidates() == 0)
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
 
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    uiContainer->CandidatePane()->ShowDownScrollArrows
        (bufLength == KMaxCandidateCountFromCore ? ETrue : EFalse);
    uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
    uiContainer->CandidatePane()->ShowRightScrollArrows(bufLength ? ETrue : EFalse);
    
    iUseforResourcechange = EFalse;
    }
void TAknFepInputStateEntryZhuyin::HandleCommandL(TInt /*aCommandId*/)
	{	
    RefreshUI(iUseforResourcechange);	
 	}
TBool TAknFepInputStateEntryZhuyin::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
   	iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);        
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
            
    if((aKey == EPtiKeyStar) && (aLength == EShortKeyPress))
        {
        ptiengine->CancelTimerActivity();

        if(ptiengine->IncrementToneMark(ETrue))
            {
            uiContainer->ShowVerticalScrollArrows(ptiengine->MoreCandidatePages());
            SetInputPaneToneMark();
            iUseforResourcechange = EFalse;
            RefreshUI(EFalse);
            }
        return ETrue;
        }
    else if(aKey == EKeyBackspace)
        {
		if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            TBool multiplePages = ptiengine->MoreCandidatePages();
            TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

            uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
            uiContainer->ShowVerticalScrollArrows(multiplePages);
            iUseforResourcechange = EFalse;
            RefreshUI(EFalse);
            SetInputPaneToneMark();
            }
        else
            {
            iOwner->FepMan()->TryCloseUiL();
            if (aLength == ELongKeyPress)
                {
                iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                }
            }
        return ETrue;    
        }
    else if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKey(aKey))
        {
     	iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);        

        ptiengine->AppendKeyPress((TPtiKey)aKey);
        
        TBool multiplePages = ptiengine->MoreCandidatePages();
        TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
        TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

        uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
        uiContainer->ShowVerticalScrollArrows(multiplePages);
        SetInputPaneToneMark();
        iUseforResourcechange = EFalse;
        RefreshUI(EFalse);
        return ETrue;
        }
    else if(aKey == EKeyDownArrow)
        {
        if (iOwner->PtiEngine()->CandidatePage().Length() == 
            KMaxCandidateCountFromCore)
            {
            ptiengine->CancelTimerActivity();
            ptiengine->NextCandidatePage();
            iUseforResourcechange = ETrue;
            RefreshUI(ETrue);
            }
        return ETrue;
        }
    else if(aKey == EKeyUpArrow)
        {
        if (ptiengine->NumberOfCandidates() != 0)
            {
            ptiengine->CancelTimerActivity();
            ptiengine->PreviousCandidatePage();
            iUseforResourcechange = ETrue;
            RefreshUI(ETrue);
            }
        return ETrue;
        }   
    else if(aKey == EKeyOK || aKey == EKeyRightArrow || aKey == EKeyLeftArrow)
        {
        ptiengine->CancelTimerActivity();        
        iOwner->ChangeState(ECandidate);
        return EFalse;
        }

    return TAknFepInputStateEntryZhuyinStrokeBase::HandleKeyL(aKey, aLength);
    }

void TAknFepInputStateEntryZhuyin::KeyTimerExpired()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtrC candidates = ptiengine->CandidatePage();
    TText toneMark;
    TBool toneMarkEntered = ptiengine->ToneMark(toneMark);
    TBool toneMarkValid = ptiengine->IsToneMarkValidForSpelling();
    if(!candidates.Length() || (toneMarkEntered && !toneMarkValid))
        {
        MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
        if(toneMarkEntered)
            {
            ptiengine->ResetToneMark();
            if( !candidates.Length() )  
            	{
            	ptiengine->DeleteKeyPress();
            	}
            }
        else
            {
            ptiengine->DeleteKeyPress();
            }
        SetInputPaneToneMark();
 
        TBool multiplePages = ptiengine->MoreCandidatePages();
        TInt bufLength = ptiengine->CandidatePage().Length();
        TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
        
        uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
        uiContainer->ShowVerticalScrollArrows(ptiengine->MoreCandidatePages());
        RefreshUI(EFalse);
        }
    }

void TAknFepInputStateEntryZhuyin::RefreshUI(TBool aOrdinals)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();

    // Universal layout 
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
    if (aOrdinals)
        {
        uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
        }
    else
        {
        uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
        }

    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);

    if (ptiengine->NumberOfCandidates() == 0)
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
 
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    uiContainer->CandidatePane()->ShowDownScrollArrows
        (bufLength == KMaxCandidateCountFromCore ? ETrue : EFalse);
    uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
    uiContainer->CandidatePane()->ShowRightScrollArrows(bufLength ? ETrue : EFalse);
    }

// End of file
