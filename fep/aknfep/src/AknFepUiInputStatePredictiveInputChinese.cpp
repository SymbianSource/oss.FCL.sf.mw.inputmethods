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
* Description:            Implementation of ITUT preditive input state
*
*/












// System includes
#include <PtiEngine.h>                      //CPtiEngine

// User includes
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiInputStatePredictiveInputChinese.h"

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputChinese::
// TAknFepInputStatePredictiveInputChinese
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStatePredictiveInputChinese::TAknFepInputStatePredictiveInputChinese(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateChinese(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->CandidatePane()->SelectFirst(); 

    TBool multiplePages = iOwner->PtiEngine()->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > 1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    }
    
// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputChinese::
// HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStatePredictiveInputChinese::HandleKeyL(TInt aKey, TKeyPressLength aLength)
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
            TAknFepInputStateCandidateChinese::HandleKeyL(aKey, aLength);
            break;
        case EKeyBackspace: //fall through
            iOwner->FepMan()->TryCloseUiL();
        break;
        default:
            response = TAknFepInputStateCandidateChinese::HandleKeyL(aKey, aLength);
        break;
        }
        
    return response;
    }

// End of file
