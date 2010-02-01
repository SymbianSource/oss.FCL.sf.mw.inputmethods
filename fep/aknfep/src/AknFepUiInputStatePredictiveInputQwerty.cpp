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
*       Provides the TAknFepInputStatePredictiveInputQwerty methods.
*
*/












#include "AknFepUiInputStatePredictiveInputQwerty.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInputQwerty::TAknFepInputStatePredictiveInputQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyChineseBase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;
    
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
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    UpdateIndicator();
    }

TBool TAknFepInputStatePredictiveInputQwerty::HandleKeyL(TInt aKey, TKeyPressLength aLength)
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
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        // it is needed when we use phrase input engine, otherwise it seems not cleared
        ptiengine->ClearCurrentWord(); 
        ptiengine->ResetToneMark();

        iOwner->ChangeState(EEntry);
        ret = EFalse; //passes to entry state to handle the key
        }
    else if(aKey == EStdKeyDevice1)
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else 
        {
        ret = TAknFepInputStateCandidateQwertyChineseBase::HandleKeyL(aKey, aLength);
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputQwerty::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStatePredictiveInputQwerty::HandleCommandL( TInt aCommandId )
    {
    TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
    }
// End of file
