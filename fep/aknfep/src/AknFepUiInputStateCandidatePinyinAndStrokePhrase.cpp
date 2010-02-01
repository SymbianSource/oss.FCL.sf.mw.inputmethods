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
* Description:            Implementation of Pinyin and Stroke phrase candidate state
*
*/












// System includes
#include <PtiEngine.h>   

// User includes
#include "AknFepUiInputStateCandidatePinyinAndStrokePhrase.h"
#include "AknFepUiInputStateCandidateBasePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManager.h"                  
// add for phrase creation
#include "AknFepUICtrlPinyinPopup.h"

// Constant definition
const TInt KMinCnadidateCount = 1;

// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidatePinyinAndStrokePhrase::
// TAknFepUiInputStateCandidatePinyinAndStrokePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidatePinyinAndStrokePhrase::
    TAknFepUiInputStateCandidatePinyinAndStrokePhrase(MAknFepUIManagerStateInterface* aOwner,
                                                      MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBaseChinesePhrase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > KMinCnadidateCount;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    uiContainer->CandidatePane()->SelectFirstPhrase();
    UpdateIndicator();
    }

void TAknFepUiInputStateCandidatePinyinAndStrokePhrase::HandleCommandL(TInt aCommandID)
   {
   TAknFepInputStateCandidateBaseChinesePhrase::HandleCommandL(aCommandID);
   }
// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidatePinyinAndStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidatePinyinAndStrokePhrase::HandleKeyL(TInt aKey,
                                                                    TKeyPressLength aLength)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane(); 
           
    if ( ( aKey == EKeyBackspace ) || ( aKey == EKeyUpArrow &&
        iOwner->FepMan()->InputMode() == EPinyin &&
        candidatePane->IsFirstPage() ) )
        {
        // the follow 2 lines add for phrase creation
        UIContainer()->PinyinPopupWindow()->SetChangeState( ETrue );
        UIContainer()->PinyinPopupWindow()->SetFromCandidateChangeStateFlag( ETrue );
        iOwner->FepMan()->EntryPhrase( EFalse );
        iOwner->ChangeState(EEntry);
        return ETrue;
        }
        
    return TAknFepInputStateCandidateBaseChinesePhrase::HandleKeyL(aKey, aLength);
    }

// End of file
