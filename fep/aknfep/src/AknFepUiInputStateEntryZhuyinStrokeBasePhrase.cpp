/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryZhuyinStrokeBasePhrase methods.
*
*/












#include "AknFepUiInputStateEntryZhuyinStrokeBasePhrase.h"

#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>
#include <AknFep.rsg>

TAknFepInputStateEntryZhuyinStrokeBasePhrase::TAknFepInputStateEntryZhuyinStrokeBasePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    iState = EEntry;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();

    uiContainer->FocusCandidatePane(EFalse);
    
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    candidatePane->SetHighlighted(ETrue);
    candidatePane->ShowCandidateOrdinals(EFalse);
    candidatePane->SelectFirstPhrase();
    }
void TAknFepInputStateEntryZhuyinStrokeBasePhrase::HandleCommandL(TInt /*aCommandId*/)
	{	
    RefreshUI();	
 	}

TBool TAknFepInputStateEntryZhuyinStrokeBasePhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();

    if(aKey == EKeyBackspace)
        {
		if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
            RefreshUI();
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
        ptiengine->AppendKeyPress((TPtiKey)aKey);
        SetInputPaneToneMark();
        inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
        RefreshUI();
        }
    else if(aKey == EKeyDownArrow)
        {
        if (!candidatePane->IsLastPage())
            {
            candidatePane->NextCandidatePage();
            RefreshUI();
            }
        }
    else if(aKey == EKeyUpArrow)
        {
        if(!candidatePane->IsFirstPage())
            {
            candidatePane->PreviousCandidatePage();
            RefreshUI();
            }
        }   
    else if(aKey == EKeyOK || aKey == EKeyRightArrow || aKey == EKeyLeftArrow)
        {
        iOwner->ChangeState(ECandidate);
        return EFalse;
        }
    return ETrue;
    }

void TAknFepInputStateEntryZhuyinStrokeBasePhrase::RefreshUI()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    // get cursor position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }

    // layout candidate and fill candidate buffer for first display page
    TInt candidateCount = 0;
    if(inputPane->IsFlagSet(MAknFepUICtrlInputPane::EPhoneticChanged))
        {
        inputPane->ClearFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
        TRAPD(ignore,candidateCount = ptiengine->GetChinesePhraseCandidatesL(
                           *(uiContainer->CandidatePane()->CandidateArray())));
        if(candidateCount == 0 || ignore != KErrNone)
            {
            return;
            }    
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        }
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
            
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);

    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable(ETrue);
    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);
    UpdateIndicator();
    }

void TAknFepInputStateEntryZhuyinStrokeBasePhrase::SetInputPaneToneMark()
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

void TAknFepInputStateEntryZhuyinStrokeBasePhrase::KeyTimerExpired()
    {
    }

void TAknFepInputStateEntryZhuyinStrokeBasePhrase::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT);
    }
    
void TAknFepInputStateEntryZhuyinStrokeBasePhrase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    TInt candidateCount = candidatePane->CandidateArray()->Count();
        
    if ( candidatePane->IsFirstPage() || candidateCount== 0 )
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows( ETrue );
        }

    if ( candidatePane->IsLastPage() || candidateCount== 0 )
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows( ETrue );
        }
    }
    
// End of file
