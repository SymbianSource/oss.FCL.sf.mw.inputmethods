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
* Description:            Implementation of predictive input qwerty chinese phrase state
*
*/













#include "AknFepUiInputStatePredictiveInputQwertyChinesePhrase.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInputQwertyChinesePhrase::
TAknFepInputStatePredictiveInputQwertyChinesePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;
    
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    // set container window position to follow cursor
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;

    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutCandidate);
    
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    
    // for some reason needs to feed back to ptiengine current Chinese phrase just before get the prediction
    candidatePane->SelectFirstPhrase();
    if(iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) ||
       iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
    	{// SetPredictiveChineseChar() should be called just after call fepMan->CommitInlineEditL() usually at some where
    	TPtrC text = candidatePane->CurrentPhraseCandidate();
    	ptiengine->SetPredictiveChineseChar(text);
    	}
    
    
    TRAPD(ignore,ptiengine->GetChinesePhraseCandidatesL(*(candidatePane->CandidateArray())));
    if (ignore == KErrNone)
        {
        candidatePane->SplitPhraseCandidatesIntoPages(); 
        uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutCandidate);
        candidatePane->SetCandidateBuffer();

        TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                uiContainer->CandidatePane()->IsFirstPage());
        TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
        TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

        uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
        uiContainer->ShowVerticalScrollArrows(multiplePages);
        uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
        UpdateIndicator();
        }
    }

TBool TAknFepInputStatePredictiveInputQwertyChinesePhrase::HandleKeyL(TInt aKey, 
                                                                      TKeyPressLength aLength)
    {
    TBool ret = ETrue;
    
    if(aKey == EStdKeyBackspace)
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else if(aKey == EStdKeyLeftFunc && aLength == EShortKeyPress) // launch sct table
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        if (fepMan->IsAbleToLaunchSCT())
            {
            fepMan->LaunchSpecialCharacterTableL();
            }
        }
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        // it is needed when we use phrase input engine, otherwise it seems not cleared
        ptiengine->ClearCurrentWord(); 
        ptiengine->ResetToneMark();

        iOwner->ChangeState(EEntry);
        ret = EFalse; //passes to entry state to handle the key
        }
    else 
        {
        ret = TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }
    return ret;
    }

// End of file
