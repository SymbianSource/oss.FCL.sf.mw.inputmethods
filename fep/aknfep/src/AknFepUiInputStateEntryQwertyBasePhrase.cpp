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
*       Provides the TAknFepUIInputStateEntryQwertyBasePhrase methods.
*
*/












#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUiInputStateEntryQwertyBasePhrase.h"

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>

TAknFepInputStateEntryQwertyBasePhrase::TAknFepInputStateEntryQwertyBasePhrase(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyPhrase(aOwner,aUIContainer)                           
    {
    iState = EEntry;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    candidatePane->SetHighlighted(ETrue);
    candidatePane->SelectFirstPhrase();
    candidatePane->ShowCandidateOrdinals(ETrue);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    }

TBool TAknFepInputStateEntryQwertyBasePhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    TBool ret = ETrue;

    if(aKey == EStdKeyBackspace)
        {
        if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                    uiContainer->CandidatePane()->IsFirstPage()));
            SetInputPaneToneMark();
            inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
            RefreshUI();
            }
        else
            {
            iOwner->FepMan()->TryCloseUiL();
            }
        }
    else if(aLength == EShortKeyPress && iOwner->IsValidChineseInputKeyQwerty(aKey) )
        {
        ptiengine->AppendKeyPress((TPtiKey)aKey);
        uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                uiContainer->CandidatePane()->IsFirstPage()));
        SetInputPaneToneMark();
        inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
        RefreshUI();
        }
    else
        {
        ret = TAknFepInputStateCandidateQwertyBasePhrase::HandleKeyL(aKey,aLength);
        }

    return ret;
    }

void TAknFepInputStateEntryQwertyBasePhrase::RefreshUI()
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
        TRAPD(ignore,
              candidateCount = ptiengine->GetChinesePhraseCandidatesL(
                   *(uiContainer->CandidatePane()->CandidateArray())));
        if(candidateCount == 0 || ignore != KErrNone)
            {
            return;
            }    
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        }
 
    uiContainer->Enable(ETrue);
    uiContainer->CandidatePane()->SetCandidateBuffer();
 
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
        
    UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
    UIContainer()->ShowVerticalScrollArrows(multiplePages);
    uiContainer->CandidatePane()->SelectFirstPhrase();
    
    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);
    }

void TAknFepInputStateEntryQwertyBasePhrase::SetInputPaneToneMark()
    {
    }

void TAknFepInputStateEntryQwertyBasePhrase::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AVKON_SOFTKEYS_EMPTY);
    }

// End of file
