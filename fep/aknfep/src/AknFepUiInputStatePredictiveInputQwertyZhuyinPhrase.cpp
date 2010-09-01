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
*       Provides the TAknFepInputStatePredictiveInputQwertyPhrase methods.
*
*/












#include "AknFepUiInputStatePredictiveInputQwertyZhuyinPhrase.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag
#include "AknFepManagerUIInterface.h"

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInputQwertyZhuyinPhrase::
TAknFepInputStatePredictiveInputQwertyZhuyinPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyChinesePhrase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;
    
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->CandidatePane()->SelectFirstPhrase(); 

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
    
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue); 
    UpdateIndicator();   
    }

TBool TAknFepInputStatePredictiveInputQwertyZhuyinPhrase::HandleKeyL(TInt aKey, 
                                                                     TKeyPressLength aLength)
    {
    TBool ret = ETrue;

    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))
        {
        return ret;
        }
        
    if(aKey == EStdKeyBackspace)
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else if(aKey == EStdKeyLeftFunc && aLength == EShortKeyPress &&
            iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagNoActionDuringChrKeyPress)) 
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        if (fepMan->IsAbleToLaunchSCT())
            {
            fepMan->LaunchSpecialCharacterTableL();
            }
        }
    else if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        iOwner->FepMan()->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);
        return ret;
        }
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey) &&
        !(aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9))
        {
        ret = ret; //passes to entry state to handle the key
        }
    else if (aKey == EStdKeyEnter || aKey == EStdKeySpace)
        {
        return ret;
        }
    else 
        {
        ret = TAknFepInputStateCandidateQwertyChinesePhrase::HandleKeyL(aKey, aLength);
        }
    return ret;
    }

// End of file
