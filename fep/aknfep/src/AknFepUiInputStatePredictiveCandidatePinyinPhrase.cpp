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
*       Provides the TAknFepInputStatePredictiveCandidatePinyinPhrase methods.
*
*/












#include "AknFepUiInputStatePredictiveCandidatePinyinPhrase.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveCandidatePinyinPhrase::TAknFepInputStatePredictiveCandidatePinyinPhrase(
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
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
            
    UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    UIContainer()->ShowVerticalScrollArrows(multiplePages);
    UpdateIndicator();
    }

TBool TAknFepInputStatePredictiveCandidatePinyinPhrase::HandleKeyL(TInt aKey, 
                                                                   TKeyPressLength aLength)
    {
    if(((aKey == EPtiKey0) || (aKey == EPtiKey1)) && aLength == EShortKeyPress )
        {
        iOwner->FepMan()->TryCloseUiL();
        return EFalse;
        }
    else
        {
        return TAknFepInputStatePredictiveCandidateChinesePhrase::HandleKeyL(aKey, aLength);
        }
    }
// End of file
