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
*       Provides the CAknFepUIInputStateQwertyZhuyinPhrase methods.
*
*/












#include "AknFepUiInputStateEntryQwertyZhuyinPhrase.h"
#include "AknFepUIManagerStateInterface.h"

#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>
#include <e32keys.h>
#include <PtiDefs.h>                //keys

const TInt KInputPaneLength = 32;
_LIT(KSep, "'");
_LIT(KReplace, "");
 

TAknFepInputStateEntryQwertyZhuyinPhrase::TAknFepInputStateEntryQwertyZhuyinPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    UIContainer()->CandidatePane()->ShowCandidateOrdinals(EFalse);
    iOwner->PtiEngine()->SetInputMode(EPtiEngineZhuyinPhraseQwerty);
    iOwner->PtiEngine()->SetCase(EPtiCaseLower);
    }

TBool TAknFepInputStateEntryQwertyZhuyinPhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    TBool ret = ETrue;

    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        return ret;
        }
    
    if((aKey == EPtiKey3 || 
        aKey == EPtiKey4 || 
        aKey == EPtiKey6 || 
        aKey == EPtiKey7 ||
        aKey == EStdKeySpace) && 
        (aLength == EShortKeyPress))        
        {
        if(ptiengine->SetToneMark(aKey))
            {
            inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
            
            TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                    uiContainer->CandidatePane()->IsFirstPage());
            TInt bufLength = ptiengine->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
            
            UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
            UIContainer()->ShowVerticalScrollArrows(multiplePages);
            SetInputPaneToneMark();
            RefreshUI();
            }
        }
    else if(aKey == EStdKeyBackspace)
        {
		if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                    uiContainer->CandidatePane()->IsFirstPage()));
            inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
            SetInputPaneToneMark();
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
    else if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        ptiengine->AppendKeyPress((TPtiKey)aKey);
        inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
        uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                uiContainer->CandidatePane()->IsFirstPage()));
        SetInputPaneToneMark();
        RefreshUI();
        }
    else if (aKey == EStdKeyLeftArrow )  
        {
        if (candidatePane->SelectedIndex() == 0 && 
            candidatePane->IsFirstPage())
            {
            return ETrue;    
            }
        else
            {
            iOwner->ChangeState(ECandidate);
            return EFalse;
            }
        }
    else if (aKey == EStdKeyRightArrow)
        {
        if (candidatePane->IsLastPage() && 
           (candidatePane->SelectedIndex() == candidatePane->VisibleCandidateCount() - 1))
            {
            return ETrue;    
            }
        else
            {
            iOwner->ChangeState(ECandidate);
            return EFalse;
            }
        }
    else if(aKey == EStdKeyDevice3)
        {
        TInt index = MapKeyToIndex(aKey);

        candidatePane->SelectIndex(index);
        TPtrC text = candidatePane->CurrentPhraseCandidate();
        if(text.Length())
            {            
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->NewTextL(text);
            fepMan->CommitInlineEditL();
            iOwner->PtiEngine()->SetPredictiveChineseChar(text);
            iOwner->ChangeState(EPredictiveCandidate);
            }
        else
            {
            // No candidates available. Back to Entry state.
            iOwner->ChangeState(EEntry);
            }
        }
   else if (aKey == EStdKeyEnter)
        {
        TBuf<KInputPaneLength> currentText;
        inputPane->GetText(currentText);
        
        while(currentText.Find(KSep)> 0)
            {
            currentText.Replace(currentText.Find(KSep), 1,KReplace);
            }
        
        
        if(currentText.Length())
            {            
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->NewTextL(currentText);
            fepMan->CommitInlineEditL();
            if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                {
                fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                iOwner->FepMan()->TryCloseUiL();
                }
            else
                {
                iOwner->FepMan()->TryCloseUiL();                
                }
            }
        }
    else
        {
        ret = TAknFepInputStateEntryQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }
    return ret;
    }
    
void TAknFepInputStateEntryQwertyZhuyinPhrase::KeyTimerExpired()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    // set container's position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }
 
    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    TBuf<KInputPaneLength> currentText;
    inputPane->GetText(currentText);
    if (ptr.Length() < currentText.Length())
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
    inputPane->SetText(ptr);
    }

// End of file
