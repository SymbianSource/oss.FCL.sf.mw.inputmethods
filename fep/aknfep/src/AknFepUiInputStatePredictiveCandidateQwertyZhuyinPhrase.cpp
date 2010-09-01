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
*       Provides the TAknFepInputStatePredictiveCandidateQwertyZhuyinPhrase methods.
*
*/












#include "AknFepUiInputStatePredictiveCandidateQwertyZhuyinPhrase.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveCandidateQwertyZhuyinPhrase::
TAknFepInputStatePredictiveCandidateQwertyZhuyinPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyChinesePhrase(aOwner, aUIContainer)
    {
    iState = EPredictiveCandidate;
    
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    // set container window position to follow cursor
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }
    uiContainer->FocusCandidatePane(ETrue);

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    // for some reason needs to feed back to ptiengine current Chinese phrase just before get the prediction
    candidatePane->SelectFirstPhrase(); 
    
    TRAPD(ignore,ptiengine->GetChinesePhraseCandidatesL(*(candidatePane->CandidateArray())));
    if (ignore == KErrNone)
        {
        candidatePane->SplitPhraseCandidatesIntoPages(); 
        uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutCandidate);
        candidatePane->SetCandidateBuffer();
    
        TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                uiContainer->CandidatePane()->IsFirstPage());
        TInt bufLength = ptiengine->CandidatePage().Length();
        TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
    
        uiContainer->ShowVerticalScrollArrows(multiplePages);
        uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
        candidatePane->ShowCandidateOrdinals(EFalse);
        UpdateIndicator();    
        }
    }

TBool TAknFepInputStatePredictiveCandidateQwertyZhuyinPhrase::HandleKeyL(TInt aKey, 
                                                                         TKeyPressLength aLength)
    {
    TBool ret = ETrue;

    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        iOwner->FepMan()->TryCloseUiL();
        return  EFalse;
        }    
    if(aKey == EStdKeyBackspace)
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else if(aKey == EStdKeyLeftFunc && aLength == EShortKeyPress)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        if (fepMan->IsAbleToLaunchSCT())
            {
            fepMan->LaunchSpecialCharacterTableL();
            }
        }
    else if(iOwner->IsQwertyZhuyinToneMarkKey(aKey))
        {
        iOwner->FepMan()->TryCloseUiL();
        return EFalse;
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
    else if (aKey == EStdKeyEnter || aKey == EStdKeySpace)
        {
        iOwner->FepMan()->TryCloseUiL();
        iOwner->ChangeState(EQwerty);
        return EFalse;
        }
    else if(!HandleVerticalNavigation(aKey))
        {
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
      
        if (aKey == EStdKeyLeftArrow )
            {
            if (candidatePane->SelectedIndex() == 0 && 
                candidatePane->IsFirstPage())
                {
                return ETrue;    
                }
            else
                {
                iOwner->ChangeState(EPredictiveInput);
                return EFalse;
                }
            }				
        else if (aKey == EStdKeyRightArrow)
            {
            if (candidatePane->IsLastPage() && 
               (candidatePane->SelectedIndex() == candidatePane->VisibleCandidateCount() - 1))
                {
                return ETrue;    
                }
            else
                {
                iOwner->ChangeState(EPredictiveInput);
                return EFalse;
                }
            }
        else
            {
            ret = TAknFepInputStateCandidateQwertyChinesePhrase::HandleKeyL(aKey, aLength);
            }
        }
        
    return ret;
    }

// End of file
