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
*       Provides the TAknFepUIInputStateEntryQwertyBase methods.
*
*/












#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUiInputStateEntryQwertyBase.h"

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>

const TText KPinyinTone4Valid = 0x02D9;
const TText KPinyinTone4Invalid = 0x0020;

TAknFepInputStateEntryQwertyBase::TAknFepInputStateEntryQwertyBase(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwerty(aOwner,aUIContainer)                           
    {
    iState = EEntry;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->ShowHorizontalScrollArrows(EFalse);
    uiContainer->FocusCandidatePane(ETrue);

    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    candidatePane->SetHighlighted(ETrue);
    candidatePane->SelectFirst();
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    }

TBool TAknFepInputStateEntryQwertyBase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TBool ret = ETrue;
    
    if(aKey == EStdKeyBackspace)
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
            }
        }
    else if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        ptiengine->AppendKeyPress((TPtiKey)aKey);

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
        ret = TAknFepInputStateCandidateQwertyBase::HandleKeyL(aKey, aLength);
        }
    return ret;
    }

void TAknFepInputStateEntryQwertyBase::RefreshUI()
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
    
    TText toneMark;
    TBool toneMarkEntered = ptiengine->ToneMark(toneMark);
    TBuf<1> validToneMarkBuf;
    if(toneMarkEntered)
        {
        // override specific invalid tonemark character only, the others are the same character 
        // when both valid and invalid
        if(toneMark == KPinyinTone4Invalid)
            toneMark = KPinyinTone4Valid;
        validToneMarkBuf.Append(toneMark);
        }
    
    ptiengine->EnableToneMarks(ETrue);
    TPtrC candidates = ptiengine->CandidatePage();
    uiContainer->CandidatePane()->SetCandidateBuffer(candidates);
    uiContainer->Enable(ETrue);
    uiContainer->CandidatePane()->SelectFirst();
    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);
    inputPane->SetToneMark(validToneMarkBuf);  
    }

void TAknFepInputStateEntryQwertyBase::SetInputPaneToneMark()
    {
    }

void TAknFepInputStateEntryQwertyBase::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AVKON_SOFTKEYS_EMPTY);
    }

// End of file
