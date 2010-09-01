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












#include "AknFepUiInputStatePredictiveInputQwertyZhuyin.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag
#include "AknFepManagerUIInterface.h"

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInputQwertyZhuyin::TAknFepInputStatePredictiveInputQwertyZhuyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyChinese(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;
    
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    uiContainer->CandidatePane()->SelectFirst(); 

    TBool multiplePages = ptiengine->MoreCandidatePages();
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
    
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);    
    UpdateIndicator();
    }

TBool TAknFepInputStatePredictiveInputQwertyZhuyin::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = ETrue;

    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if(aKey == EStdKeyBackspace)
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
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey) && 
            !(aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9))
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
        ret = TAknFepInputStateCandidateQwertyChinese::HandleKeyL(aKey, aLength);
        }
    return ret;
    }

// End of file
