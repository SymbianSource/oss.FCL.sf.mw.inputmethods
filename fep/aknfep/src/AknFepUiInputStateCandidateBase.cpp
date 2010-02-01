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
*       Provides the TAknFepInputStateCandidateBase methods.
*
*/












#include "AknFepUiInputStateCandidateBase.h"

#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                         //CPtiEngine

TAknFepInputStateCandidateBase::TAknFepInputStateCandidateBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    }

void TAknFepInputStateCandidateBase::HandleCommandL(TInt /*aCommandID*/)
    {
	CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    // get cursor position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;

    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }

    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable(ETrue);
    }

TBool TAknFepInputStateCandidateBase::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    if (aKey == EPtiKeyStar)
        {
        iOwner->FepMan()->TryCloseUiL();
        return EFalse;
        }   
    if(!( HandleVerticalNavigation(aKey) || HandleHorizontalNavigation(aKey) ))
        {
        // it may be one of the 'valid' numbers..
        TInt index = MapKeyToIndex(aKey);
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

        if(candidatePane->SelectIndex(index) || aKey == EKeyOK)
            {
            TPtrC text = candidatePane->CurrentCandidate();
            if(text.Length()) 
                {            
                MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                fepMan->NewCharacterL(text);
                fepMan->CommitInlineEditL();
                
                if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                    {
                    fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                    iOwner->FepMan()->TryCloseUiL();
                    }
                else
                    {
                    iOwner->ChangeState(EPredictiveCandidate);
                    }
                }
            else
                {
                // No candidates available. Back to Entry state.
                iOwner->ChangeState(EEntry);
                }
            }
        }
    return ETrue;
    }

TBool TAknFepInputStateCandidateBase::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EKeyLeftArrow)
        {
        if(!candidatePane->SelectPrev())
            {
            ptiengine->PreviousCandidatePage();
            candidatePane->SetCandidateBuffer(ptiengine->CandidatePage());
            candidatePane->SelectLast();
            }
        response = ETrue;
        }
    else if (aKey == EKeyRightArrow)
        {
        if(!candidatePane->SelectNext())
            {
            ptiengine->NextCandidatePage();
            candidatePane->SetCandidateBuffer(ptiengine->CandidatePage());
            candidatePane->SelectFirst();
            }
        response = ETrue;
        }
    return response;
    }

TBool TAknFepInputStateCandidateBase::HandleVerticalNavigation(TInt aKey)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (aKey == EKeyDownArrow)
        {
        ptiengine->NextCandidatePage();
        }
    else if (aKey == EKeyUpArrow)
        {
        ptiengine->PreviousCandidatePage();
        }
    else
        {
        return EFalse;
        }

    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    candidatePane->SelectFirst();
    candidatePane->SetCandidateBuffer(ptiengine->CandidatePage());
    return ETrue;
    }

// End of file
