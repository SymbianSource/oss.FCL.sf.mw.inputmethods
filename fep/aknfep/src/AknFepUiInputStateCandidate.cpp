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
*       Provides the CAknFepUIInputStateCandidate methods.
*
*/












#include "AknFepUiInputStateCandidate.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUICtrlCandidatePane.h"      //MAknFepUICtrlCandidatePane
#include <PtiEngine.h>                      //CPtiEngine


///////////////////
// Candidate state
///////////////////

TAknFepInputStateCandidate::TAknFepInputStateCandidate(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->CandidatePane()->SelectFirst();

    TBool multiplePages = iOwner->PtiEngine()->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    }


TBool TAknFepInputStateCandidate::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    if(aKey==EKeyBackspace)
        {
        iOwner->ChangeState(EEntry);
        return ETrue;
        }
    return TAknFepInputStateCandidateBase::HandleKeyL(aKey, aLength);
    }


// End of file
