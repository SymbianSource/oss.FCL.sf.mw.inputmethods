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
* Description:            Implementation of Stroke phrase predictive candidate state
*
*/












// System includes
#include <PtiEngine.h>                      

// User includes
#include "AknFepUiInputStatePredictiveCandidateStrokePhrase.h"
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManagerUIInterface.h"       
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"

// Constant definition
const TInt KMinCnadidateCount = 1;

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveCandidateStrokePhrase::
// TAknFepInputStatePredictiveCandidateStrokePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStatePredictiveCandidateStrokePhrase::TAknFepInputStatePredictiveCandidateStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStatePredictiveCandidateChinesePhrase(aOwner, aUIContainer)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutCandidate);

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > KMinCnadidateCount;
            
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveCandidateStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStatePredictiveCandidateStrokePhrase::HandleKeyL(TInt aKey, 
                                                                   TKeyPressLength aLength)
    {
    if(aKey == EPtiKey0 || aKey == EPtiKey7 ||
       aKey == EPtiKey8 || aKey == EPtiKey9)
        {
        iOwner->FepMan()->TryCloseUiL();
        return EFalse;
        }

    return TAknFepInputStatePredictiveCandidateChinesePhrase::HandleKeyL(aKey, aLength);
    }
    
// End of file
