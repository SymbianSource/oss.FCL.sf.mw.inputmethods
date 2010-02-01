/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryZhuyinStrokeBase methods.
*
*/












#include "AknFepUiInputStateEntryZhuyinStrokeBase.h"

#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepManager.h"

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>

TAknFepInputStateEntryZhuyinStrokeBase::TAknFepInputStateEntryZhuyinStrokeBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    iState = EEntry;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    TBool multiplePages = ptiengine->MoreCandidatePages();
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > 1;
            
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(EFalse);
    uiContainer->FocusCandidatePane(ETrue);
    candidatePane->SetHighlighted(ETrue);
    candidatePane->ShowCandidateOrdinals(EFalse);//
    candidatePane->SelectFirst();
    iUseforResourcechange = EFalse;
    }
void TAknFepInputStateEntryZhuyinStrokeBase::HandleCommandL(TInt /*aCommandId*/)
	{	
    RefreshUI(iUseforResourcechange);	
 	}

TBool TAknFepInputStateEntryZhuyinStrokeBase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();

    if(aKey == EKeyBackspace)
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
        }
    else if(aKey == EKeyDownArrow)
        {
        ptiengine->NextCandidatePage();
        iUseforResourcechange = EFalse;
        RefreshUI(EFalse);
        }
    else if(aKey == EKeyUpArrow)
        {
        ptiengine->PreviousCandidatePage();
        iUseforResourcechange = EFalse;
        RefreshUI(EFalse);
        }   
    else if(aKey == EKeyOK || aKey == EKeyRightArrow || aKey == EKeyLeftArrow)
        {
        iOwner->ChangeState(ECandidate);
        return EFalse;
        }
    return ETrue;
    }

void TAknFepInputStateEntryZhuyinStrokeBase::RefreshUI(TBool aOrdinals)
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
    }

void TAknFepInputStateEntryZhuyinStrokeBase::SetInputPaneToneMark()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlInputPane* inputPane = UIContainer()->InputPane();

    TText toneMark;
    TBool toneMarkEntered = ptiengine->ToneMark(toneMark);
    TBuf<1> buf;
    if(toneMarkEntered)
        {
        buf.Append(toneMark);
        }
    inputPane->SetToneMark(buf);
    }

void TAknFepInputStateEntryZhuyinStrokeBase::KeyTimerExpired()
    {
    }

void TAknFepInputStateEntryZhuyinStrokeBase::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AVKON_SOFTKEYS_EMPTY);
    }
// End of file
