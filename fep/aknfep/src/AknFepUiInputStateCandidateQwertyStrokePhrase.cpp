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
* Description:            Implementation of Stroke phrase Qwerty candidate state
*
*/












// System includes
#include <PtiEngine.h>                      

// User includes
#include "AknFepUiInputStateCandidateQwertyStrokePhrase.h"
#include "AknFepUICtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepUICtrlCandidatePane.h"      
#include "AknFepManager.h"                  

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateQwertyStrokePhrase::
// TAknFepUiInputStateCandidateQwertyStrokePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidateQwertyStrokePhrase::TAknFepUiInputStateCandidateQwertyStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyPhrase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();

    TBool multiplePages = iOwner->PtiEngine()->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    }

// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidatePinyinAndStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateQwertyStrokePhrase::HandleKeyL(TInt aKey, 
                                                                 TKeyPressLength aLength)
    {
    if(aKey == EStdKeyBackspace)
        {
        iOwner->ChangeState(EEntry);
        return EFalse;
        }
    else
        {
        return TAknFepInputStateCandidateQwertyPhrase::HandleKeyL(aKey,aLength);
        }
    }

// End of file
