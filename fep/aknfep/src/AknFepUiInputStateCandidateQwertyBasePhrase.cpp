/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateCandidateQwertyBasePhrase methods.
*
*/












#include "AknFepUiInputStateCandidateQwertyBasePhrase.h"

#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStateCandidateQwertyBasePhrase::TAknFepInputStateCandidateQwertyBasePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    }

TBool TAknFepInputStateCandidateQwertyBasePhrase::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    if(aKey == EStdKeyDevice1)
        {
        iOwner->FepMan()->TryCloseUiL();
        }

    else if(!( HandleVerticalNavigation(aKey) || HandleHorizontalNavigation(aKey) ))
        {
        // it may be one of the 'valid' numbers..
        TInt index = MapKeyToIndex(aKey);
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

        if(candidatePane->SelectIndex(index) || aKey == EStdKeyDevice3 || aKey == EStdKeyEnter)
            {
            TPtrC text = candidatePane->CurrentPhraseCandidate();
            if(text.Length())
                {            
                MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                fepMan->NewTextL(text);
                fepMan->CommitInlineEditL();
                iOwner->PtiEngine()->SetPredictiveChineseChar(text);
                if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                    {
                    fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                    iOwner->FepMan()->TryCloseUiL();
                    }
                else
                    {
                    iOwner->ChangeState(EPredictiveInput);
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

TBool TAknFepInputStateCandidateQwertyBasePhrase::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EStdKeyLeftArrow)
        {
        if(!candidatePane->SelectPrev())
            {
            candidatePane->PreviousCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else if (aKey == EStdKeyRightArrow)
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

TBool TAknFepInputStateCandidateQwertyBasePhrase::HandleVerticalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    
    if (aKey == EStdKeyDownArrow || aKey == EStdKeySpace)
        {
        candidatePane->NextCandidatePage();
        }
    else if (aKey == EStdKeyUpArrow)
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

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateQwertyBasePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateQwertyBasePhrase::HandleCommandL(
    TInt aCommandId )
    {
    TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
    }
// End of file
