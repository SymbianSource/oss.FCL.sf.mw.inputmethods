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
*       Provides the CAknFepUIInputStateCandidate methods.
*
*/












#include "AknFepUiInputStateCandidatePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUICtrlCandidatePane.h"      //MAknFepUICtrlCandidatePane
#include <PtiEngine.h>                      //CPtiEngine


///////////////////
// Candidate state
///////////////////

TAknFepInputStateCandidatePhrase::TAknFepInputStateCandidatePhrase(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBasePhrase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    uiContainer->CandidatePane()->SelectFirstPhrase();

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    }

TBool TAknFepInputStateCandidatePhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    if(aKey == EKeyBackspace)
        {
        iOwner->ChangeState(EEntry);
        return ETrue;
        }
    return TAknFepInputStateCandidateBasePhrase::HandleKeyL(aKey, aLength);
    }

// End of file
