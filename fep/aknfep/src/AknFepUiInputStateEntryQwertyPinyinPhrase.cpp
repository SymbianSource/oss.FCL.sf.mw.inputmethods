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
*       Provides the CAknFepUIInputStateEntryQwertyPinyinPhrase methods.
*
*/












#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUiInputStateEntryQwertyPinyinPhrase.h"
#include "AknFepManager.h"  

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>

const TInt KInputPaneLength = 32;
_LIT(KSep, "'");
_LIT(KReplace, "");

TAknFepInputStateEntryQwertyPinyinPhrase::TAknFepInputStateEntryQwertyPinyinPhrase(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBaseChinesePhrase(aOwner,aUIContainer)                           
    {
    iOwner->PtiEngine()->SetInputMode(EPtiEnginePinyinPhraseQwerty);
    iOwner->PtiEngine()->SetCase(EPtiCaseLower);
    }

TBool TAknFepInputStateEntryQwertyPinyinPhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();

    if(aKey == EStdKeyLeftFunc && aLength == EShortKeyPress)
        {
        if(ptiengine->IncrementToneMark(ETrue))
            {
            TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                    uiContainer->CandidatePane()->IsFirstPage());
            TInt bufLength = ptiengine->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
            
            uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                    uiContainer->CandidatePane()->IsFirstPage()));
            uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
            uiContainer->InputPane()->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
            SetInputPaneToneMark();
            RefreshUI();
            }
        }
    else if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed) ||
             iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        ret = ETrue;
        }        
    else if(aLength == EShortKeyPress && iOwner->IsValidChineseInputKeyQwerty(aKey) )
        {
        if (!uiContainer->IsEnabled() && 
            (aKey == EPtiKeyQwertyI || aKey == EPtiKeyQwertyU || aKey == EPtiKeyQwertyV))
            {
            iOwner->FepMan()->UpdateCbaL(NULL);
            iOwner->ChangeState(EQwerty);
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            return ret;     
            }

        TInt stringBeforeLength(0);
        TInt stringAfterLength(0);
        
        stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
        stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
        
        if (stringBeforeLength != stringAfterLength)
            {
            if (ptiengine->GetPhoneticSpelling(1).Length() == 1)
                {
                iOwner->FepMan()->UpdateCbaL(R_AVKON_SOFTKEYS_EMPTY);
                }
            
            TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                    uiContainer->CandidatePane()->IsFirstPage());
            TInt bufLength = ptiengine->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
        
            uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                    uiContainer->CandidatePane()->IsFirstPage()));
            uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);         
            SetInputPaneToneMark();
            uiContainer->InputPane()->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);

            RefreshUI();
            }
        else
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }
            
        if (ptiengine->GetPhoneticSpelling(1).Length() == 0)
            {
            iOwner->FepMan()->UpdateCbaL(NULL);
            }
        }
    else if(aKey == EStdKeyBackspace)
        {
        if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
            uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                    uiContainer->CandidatePane()->IsFirstPage()));
            SetInputPaneToneMark();
            inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
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
    else if (aKey == EStdKeySpace)
        {
        TInt index = MapKeyToIndex(EStdKeyDevice3);
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

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
    else 
        {
        ret = TAknFepInputStateEntryQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }
        
    return ret;
    }

// End of file
