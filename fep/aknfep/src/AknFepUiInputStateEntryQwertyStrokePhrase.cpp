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
* Description:            Implementation of Stroke phrase Qwerty entry state
*
*/












// System includes
#include <PtiEngine.h>

// User includes
#include "AknFepUiInputStateEntryQwertyStrokePhrase.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManagerUIInterface.h"      
#include "AknFepManager.h"                 

// Constant definition
const TInt KMinCnadidateCount = 1;
const TInt KEnginePhoneticIndex = 1;

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryQwertyStrokePhrase::
// TAknFepInputStateEntryQwertyStrokePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStateEntryQwertyStrokePhrase::TAknFepInputStateEntryQwertyStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    UIContainer()->CandidatePane()->ShowCandidateOrdinals(ETrue);
    
    if (EPtiEngineStrokePhraseQwerty != iOwner->PtiEngine()->InputMode())
        {
        iOwner->PtiEngine()->SetInputMode(EPtiEngineStrokePhraseQwerty);
        }
    iOwner->PtiEngine()->SetCase(EPtiCaseLower);
    }
    
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryQwertyStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryQwertyStrokePhrase::HandleKeyL(TInt aKey, 
                                                           TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    TBool ret = ETrue;
    


    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed) ||
             iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        ret = ETrue;
        } 
    else if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        TInt stringBeforeLength(0);
        TInt stringAfterLength(0);
        
        stringBeforeLength = ptiengine->GetPhoneticSpelling(KEnginePhoneticIndex).Length();
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
   else if (aKey == EStdKeyEnter)
        {
        return ret;
        }
    else
        { 
        ret = TAknFepInputStateEntryQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }
        
    return ret;
    }
    
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryQwertyStrokePhrase::RefreshUI
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryQwertyStrokePhrase::RefreshUI()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    // get cursor position
    TPoint baseLine;
    TInt height(0);
    TInt ascent(0);
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);
        }

    // layout candidate and fill candidate buffer for first display page
    TInt candidateCount(0);
    TRAP(ret,
         candidateCount = ptiengine->GetChinesePhraseCandidatesL(
               *(uiContainer->CandidatePane()->CandidateArray())));
    if(candidateCount == 0 || ret != KErrNone)
        {
        return;
        }  
          
    uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
    uiContainer->CandidatePane()->SetCandidateBuffer(); 

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = ptiengine->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > KMinCnadidateCount;
        
    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
    uiContainer->ShowVerticalScrollArrows(multiplePages);

    uiContainer->Enable(ETrue);
    uiContainer->CandidatePane()->SelectFirstPhrase();
    TPtrC ptr = ptiengine->GetPhoneticSpelling(KEnginePhoneticIndex);
    inputPane->SetText(ptr);
    UpdateIndicator();
    }

// End of file
