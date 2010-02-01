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
* Description:            Implementation of Pinyin phrase predictive Qwerty state
*
*/












// System includes
#include <PtiEngine.h>                      //CPtiEngine

// User includes
#include "AknFepUiInputStateCandidateChinese.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUICtrlCandidatePane.h"      //MAknFepUICtrlCandidatePane
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"

const TInt KMaxCandidateCountFromCore = 6;

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateChinese::
// TAknFepInputStateCandidateChinese
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStateCandidateChinese::TAknFepInputStateCandidateChinese(
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
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateChinese::
// HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCandidateChinese::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    if(aKey==EKeyBackspace)
        {
        iOwner->ChangeState(EEntry);
        return ETrue;
        }
        
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
                    UpdateIndicator();
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
    
// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateChinese::
// HandleHorizontalNavigation
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCandidateChinese::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EKeyLeftArrow)
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
    else if (aKey == EKeyRightArrow)
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

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateChinese::
// HandleVerticalNavigation
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCandidateChinese::HandleVerticalNavigation(TInt aKey)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    if (aKey == EKeyDownArrow)
        {
        if (iOwner->PtiEngine()->CandidatePage().Length() != 
            KMaxCandidateCountFromCore)
            {
            return ETrue; 
            }
        ptiengine->NextCandidatePage();
        }
    else if (aKey == EKeyUpArrow)
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
    
// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateChinese::
// UpdateIndicator
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateChinese::UpdateIndicator()
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

// End of file
