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
*       Provides the TAknFepInputStatePredictiveCandidateStroke methods.
*
*/












#include "AknFepUiInputStatePredictiveCandidateStroke.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"      //MAknFepUICtrlCandidatePane
#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveCandidateStroke::TAknFepInputStatePredictiveCandidateStroke(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateChinese(aOwner, aUIContainer)
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
    }

TBool TAknFepInputStatePredictiveCandidateStroke::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    if (aKey == EPtiKey0 || aKey == EPtiKey7 ||
        aKey == EPtiKey8 || aKey == EPtiKey9)
        {
        iOwner->FepMan()->TryCloseUiL();
        return EFalse;
        }
        
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
            ret = TAknFepInputStateCandidateChinese::HandleKeyL(aKey, aLength);
            }   
        }
    return ret;
    }
