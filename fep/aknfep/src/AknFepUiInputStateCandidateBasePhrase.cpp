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
*       Provides the TAknFepInputStateCandidateBasePhrase methods.
*
*/












#include "AknFepUiInputStateCandidateBasePhrase.h"

#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                         //CPtiEngine

TAknFepInputStateCandidateBasePhrase::TAknFepInputStateCandidateBasePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBase(aOwner, aUIContainer)
    {
    }

TBool TAknFepInputStateCandidateBasePhrase::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
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
            TPtrC text = candidatePane->CurrentPhraseCandidate();
            if(text.Length()) 
                {            
                MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                fepMan->NewTextL(text);
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

TBool TAknFepInputStateCandidateBasePhrase::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EKeyLeftArrow)
        {
        if(!candidatePane->SelectPrev())
            {
            candidatePane->PreviousCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else if (aKey == EKeyRightArrow)
        {
        if(!candidatePane->SelectNext())
            {
            candidatePane->NextCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirstPhrase();
            }
        response = ETrue;
        }
    return response;
    }

TBool TAknFepInputStateCandidateBasePhrase::HandleVerticalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    if (aKey == EKeyDownArrow)
        {
        candidatePane->NextCandidatePage();
        }
    else if (aKey == EKeyUpArrow)
        {
        candidatePane->PreviousCandidatePage();
        }
    else
        {
        return EFalse;
        }
    candidatePane->SetCandidateBuffer();
    candidatePane->SelectFirstPhrase();
    return ETrue;
    }

// End of file
