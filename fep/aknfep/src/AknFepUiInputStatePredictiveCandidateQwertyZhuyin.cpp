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
*       Provides the TAknFepInputStatePredictiveCandidateQwertyZhuyin methods.
*
*/












#include "AknFepUiInputStatePredictiveCandidateQwertyZhuyin.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveCandidateQwertyZhuyin::TAknFepInputStatePredictiveCandidateQwertyZhuyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyChinese(aOwner, aUIContainer)
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
    uiContainer->CandidatePane()->SelectFirst(); 
    uiContainer->CandidatePane()->SetCandidateBuffer(ptiengine->CandidatePage());

    TBool multiplePages = ptiengine->MoreCandidatePages();
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
    
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
    UpdateIndicator();
    }

TBool TAknFepInputStatePredictiveCandidateQwertyZhuyin::HandleKeyL(TInt aKey, 
                                                                   TKeyPressLength aLength)
    {
    TBool ret = ETrue;
    
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
    else if(!HandleVerticalNavigation(aKey))
        {
        if (aKey == EStdKeyLeftArrow || aKey == EStdKeyRightArrow)
            {
            iOwner->ChangeState(EPredictiveInput);
            ret = EFalse; //passes to predictive input state to handle the key
            }
        else
            {
            ret = TAknFepInputStateCandidateQwertyChinese::HandleKeyL(aKey, aLength);
            }
        }
    return ret;
    }

// End of file
