/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStatePredictiveInputPhrase methods.
*
*/












#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiInputStatePredictiveInputPhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInputPhrase::TAknFepInputStatePredictiveInputPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBasePhrase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();

    candidatePane->SelectFirstPhrase(); 

    TBool multiplePages = iOwner->PtiEngine()->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    UIContainer()->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    candidatePane->ShowCandidateOrdinals(ETrue);
    }

TBool TAknFepInputStatePredictiveInputPhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
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
        TAknFepInputStateCandidateBasePhrase::HandleKeyL(aKey, aLength);
        break;
    case EKeyBackspace: //fall through
        iOwner->FepMan()->TryCloseUiL();
        break;
    default:
        response = TAknFepInputStateCandidateBasePhrase::HandleKeyL(aKey, aLength);
        break;
        }
    return response;
    }

// End of file
