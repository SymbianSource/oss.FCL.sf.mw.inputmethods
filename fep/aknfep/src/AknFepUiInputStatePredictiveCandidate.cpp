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
*       Provides the TAknFepInputStatePredictiveCandidate methods.
*
*/












#include "AknFepUiInputStatePredictiveCandidate.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveCandidate::TAknFepInputStatePredictiveCandidate(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBase(aOwner, aUIContainer)
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
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutCandidate);

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    ptiengine->SetCandidatePageLength(MAknFepUICtrlContainerChinese::ELayoutInput);

    // for some reason needs to feed back to ptiengine current Chinese char just before get the prediction
    TPtrC text = uiContainer->CandidatePane()->CurrentCandidate();
    uiContainer->CandidatePane()->SelectFirst(); 
    ptiengine->SetPredictiveChineseChar(text);
    
    uiContainer->CandidatePane()->SetCandidateBuffer(ptiengine->CandidatePage());

    TBool multiplePages = ptiengine->MoreCandidatePages();
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
    
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
//  Do standard candidate pane init...
    }


TBool TAknFepInputStatePredictiveCandidate::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = ETrue;
    if(aLength == ELongKeyPress) 
        {
        return ret;
        }
    if(iOwner->IsValidChineseInputKey(aKey))
        {
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        // it is needed when we use phrase input engine, otherwise it seems not cleared
        ptiengine->ClearCurrentWord(); 
        ptiengine->ResetToneMark();

        iOwner->ChangeState(EEntry);
        ret = EFalse; //passes to entry state to handle the key
        }
    if(!HandleVerticalNavigation(aKey))
        {
        if (aKey == EKeyLeftArrow || aKey == EKeyRightArrow || aKey == EKeyOK)
            {
            iOwner->ChangeState(EPredictiveInput);
            ret = EFalse; //passes to predictive input state to handle the key
            }
        else if(aKey == EKeyBackspace)
            {
            iOwner->FepMan()->TryCloseUiL();
            }
        else if (aKey == EPtiKeyStar)
            {
            ret = TAknFepInputStateCandidateBase::HandleKeyL(aKey, aLength);
            }   
        }
    return ret;
    }

// End of file
