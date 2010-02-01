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
* Description:            Implementation of candidate qwerty chinese base state
*
*/












#include "AknFepUiInputStateCandidateQwertyChineseBase.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                      //CPtiEngine

const TInt KMaxCandidateCountFromCore = 6;

TAknFepInputStateCandidateQwertyChineseBase::TAknFepInputStateCandidateQwertyChineseBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    }

TBool TAknFepInputStateCandidateQwertyChineseBase::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
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

TBool TAknFepInputStateCandidateQwertyChineseBase::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EStdKeyLeftArrow)
        {
        if (candidatePane->SelectedIndex() == 0 && 
            ptiengine->NumberOfCandidates() == 0)
            {
            return ETrue;
            }
        
        if(!candidatePane->SelectPrev())
            {
            ptiengine->PreviousCandidatePage();
            candidatePane->SetCandidateBuffer(ptiengine->CandidatePage());
            candidatePane->SelectLast();
            }
        response = ETrue;
        }
    else if (aKey == EStdKeyRightArrow)
        {
        TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
        
        if (bufLength != KMaxCandidateCountFromCore &&
            candidatePane->SelectedIndex() == (bufLength -1))
            {
			return ETrue;
            }
        
        if(!candidatePane->SelectNext())
            {
            ptiengine->NextCandidatePage();
            candidatePane->SetCandidateBuffer(ptiengine->CandidatePage());
            candidatePane->SelectFirst();
            }
        response = ETrue;
        }
    if (response) 
        {
        UpdateIndicator();
        }
                
    return response;
    }

TBool TAknFepInputStateCandidateQwertyChineseBase::HandleVerticalNavigation(TInt aKey)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (aKey == EStdKeyDownArrow || aKey == EStdKeySpace)
        {
        if (iOwner->PtiEngine()->CandidatePage().Length() != 
            KMaxCandidateCountFromCore)
            {
            return ETrue;
            }
        
        ptiengine->NextCandidatePage();
        }
    else if (aKey == EStdKeyUpArrow)
        {
        if (ptiengine->NumberOfCandidates() == 0)
            {								 
            return ETrue;
            }
        
        ptiengine->PreviousCandidatePage();
        }
    else
        {
        return EFalse;
        }

    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    candidatePane->SelectFirst();
    candidatePane->SetCandidateBuffer(ptiengine->CandidatePage());
    UpdateIndicator();     
    return ETrue;
    }

void TAknFepInputStateCandidateQwertyChineseBase::UpdateIndicator()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
        
    if (ptiengine->NumberOfCandidates() == 0)
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
    
    uiContainer->CandidatePane()->ShowDownScrollArrows
        (bufLength == KMaxCandidateCountFromCore ? ETrue : EFalse);
        
    if (candidatePane->SelectedIndex() == 0 && 
        ptiengine->NumberOfCandidates() == 0)
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
        }
    else
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
        }
        
    if (bufLength != KMaxCandidateCountFromCore &&
        candidatePane->SelectedIndex() == (bufLength -1))
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);        
        }
    else
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateQwertyChineseBase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateQwertyChineseBase::HandleCommandL(
    TInt aCommandId )
    {
    TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
    }
// End of file
