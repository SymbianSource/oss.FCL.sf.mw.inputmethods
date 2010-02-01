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
* Description:            Implementation of predictive candidate chinese phrase state
*
*/












#include "AknFepUiInputStatePredictiveCandidateChinesePhrase.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveCandidateChinesePhrase::
TAknFepInputStatePredictiveCandidateChinesePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBaseChinesePhrase(aOwner, aUIContainer)
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
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    candidatePane->SelectFirstPhrase(); 
    ptiengine->SetPredictiveChineseChar(text);
    
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
        candidatePane->ShowCandidateOrdinals(EFalse);
        UpdateIndicator();
        }
    }

TBool TAknFepInputStatePredictiveCandidateChinesePhrase::HandleKeyL(TInt aKey, 
                                                                    TKeyPressLength aLength)
    {
    TBool ret = ETrue;

    if(iOwner->IsValidChineseInputKey(aKey) && aLength != ELongKeyPress)
        {
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        // needs to clear current word for current engine, otherwise buffer is not cleared
        ptiengine->ClearCurrentWord(); 
        ptiengine->ResetToneMark();

        iOwner->ChangeState(EEntry);
        ret = EFalse; //passes to entry state to handle the key
        }
    if(!HandleVerticalNavigation(aKey))
        {
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
        // Do navigation...
        if (aKey == EKeyLeftArrow && aLength != ELongKeyPress)
            {
//            if (candidatePane->SelectedIndex() == 0 && 
//                candidatePane->IsFirstPage() )
//                {
//                return ETrue;
//                }
//            else 
                {
                iOwner->ChangeState(EPredictiveInput);
                ret = EFalse; //passes to predictive input state to handle the key
                } 
            }
        else if (aKey == EKeyRightArrow && aLength != ELongKeyPress)
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
        else if ( ( aKey == EKeyOK || aKey == EKeyCBA1 ) && aLength != ELongKeyPress)
            {
            iOwner->ChangeState(EPredictiveInput);
            ret = EFalse; //passes to predictive input state to handle the key
            }
        else if(aKey == EKeyBackspace && aLength != ELongKeyPress)
            {
            iOwner->FepMan()->TryCloseUiL();
            }
        else if (aKey == EPtiKeyStar && aLength != ELongKeyPress)
            {
            ret = TAknFepInputStateCandidateBaseChinesePhrase::HandleKeyL(aKey, aLength);
            }   
        }
    return ret;
    }

// End of file
