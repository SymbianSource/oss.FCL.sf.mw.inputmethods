/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStatePredictiveInput methods.
*
*/












#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiInputStatePredictiveInput.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInput::TAknFepInputStatePredictiveInput(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    uiContainer->CandidatePane()->SelectFirst(); 

    TBool multiplePages = iOwner->PtiEngine()->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    }

TBool TAknFepInputStatePredictiveInput::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool response = ETrue;
     
    switch (aKey)
        {
    case EKeyOK:        //fall through
    case EPtiKey0:
    case EPtiKey1:
    case EPtiKey2:
    case EPtiKey3:
    case EPtiKey4:
    case EPtiKey5:
    case EPtiKey6:
    case EPtiKey7:
    case EPtiKey8:
    case EPtiKey9:
        TAknFepInputStateCandidateBase::HandleKeyL(aKey, aLength);
        break;
    case EKeyBackspace: //fall through
        iOwner->FepMan()->TryCloseUiL();
        break;
    default:
        response = TAknFepInputStateCandidateBase::HandleKeyL(aKey, aLength);
        break;
        }
    return response;
    }

// End of file
