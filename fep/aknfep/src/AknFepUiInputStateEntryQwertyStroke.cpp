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
*       Provides the CAknFepUIInputStateEntryQwertyStroke methods.
*
*/












#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUiInputStateEntryQwertyStroke.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  

const TInt KMaxCandidateCountFromCore = 6;

TAknFepInputStateEntryQwertyStroke::TAknFepInputStateEntryQwertyStroke(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBase(aOwner,aUIContainer)                           
    {
    UIContainer()->CandidatePane()->ShowCandidateOrdinals(ETrue);
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if (ptiengine->InputMode() != EPtiEngineStrokeQwerty)
        {
        ptiengine->SetInputMode(EPtiEngineStrokeQwerty);
        }
    
    ptiengine->SetCase(EPtiCaseLower);
    ptiengine->SetCandidatePageLength(MAknFepUICtrlContainerChinese::ELayoutInput);
    }

TBool TAknFepInputStateEntryQwertyStroke::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TBool ret = ETrue;
    
    if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        TInt stringBeforeLength(0);
        TInt stringAfterLength(0);
        
        stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
        stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
        
        if (stringBeforeLength != stringAfterLength)
            {
            RefreshUI();
            }
        else
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }            
        }
    else if(aKey == EStdKeyBackspace)
        {
		if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            RefreshUI();
            }
        else
            {
            iOwner->FepMan()->TryCloseUiL();
            if (aLength == ELongKeyPress)
                {
                iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                }
            }
        }
    else if(aKey == EStdKeyDevice1)
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
    else if (!iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
        
    return ret;
    }

void TAknFepInputStateEntryQwertyStroke::RefreshUI()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    TBool multiplePages = ptiengine->MoreCandidatePages();
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
        
    UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
    UIContainer()->ShowVerticalScrollArrows(multiplePages);
    // set container's position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }
    
    TPtrC candidates = ptiengine->CandidatePage();
    uiContainer->CandidatePane()->SetCandidateBuffer(candidates);
    
    uiContainer->Enable(ETrue);
    uiContainer->CandidatePane()->SelectFirst();
    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);
    UpdateIndicator();
    }

TBool TAknFepInputStateEntryQwertyStroke::HandleHorizontalNavigation(TInt aKey)
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
        UpdateIndicator();            
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
        UpdateIndicator();    
        response = ETrue;
        }
    return response;
    }

TBool TAknFepInputStateEntryQwertyStroke::HandleVerticalNavigation(TInt aKey)
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
 
void TAknFepInputStateEntryQwertyStroke::UpdateIndicator()
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
// TAknFepInputStateEntryQwertyStroke::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//

void TAknFepInputStateEntryQwertyStroke::HandleCommandL( TInt aCommandId )
    {
    TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
    }
// End of file
