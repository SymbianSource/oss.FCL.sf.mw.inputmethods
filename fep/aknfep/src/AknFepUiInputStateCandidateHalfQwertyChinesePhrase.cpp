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
#include "AknFepUiInputStateCandidateHalfQwertyChinesePhrase.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManager.h" 
#include <AknFep.rsg>                 

// Constant definition
const TInt KMinCandidateCount = 1;
//const TInt KInputPaneLength = 32;
//_LIT(KSep, "'");
//_LIT(KReplace, "");
// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidateHalfQwertyChinesePhrase::
// TAknFepUiInputStateCandidateHalfQwertyChinesePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidateHalfQwertyChinesePhrase::
TAknFepUiInputStateCandidateHalfQwertyChinesePhrase(MAknFepUIManagerStateInterface* aOwner,
                                                      MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > KMinCandidateCount;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    uiContainer->CandidatePane()->SelectFirstPhrase();
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateHalfQwertyChinesePhrase::HandleKeyL(TInt aKey,
                                                                    TKeyPressLength aLength)
    {
    TInt selected = UIContainer()->CandidatePane()->SelectedIndex();
    TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();


    if ( aKey != EStdKeyLeftFunc && 
        (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed)))
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return ETrue;
        }
    else if ( aKey == EStdKeyLeftFunc )
        {
        iOwner->FepMan()->TryCloseUiL();
        iOwner->FepMan()->LaunchSelectModeMenuL();
        }
    else if ( aKey ==EStdKeySpace)
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return ETrue;
        }
    else if(aKey == EStdKeyBackspace) //clear key
        {
        UIContainer()->CandidatePane()->SplitPhraseCandidatesIntoPages();
        UIContainer()->CandidatePane()->SetCandidateBuffer();
        iOwner->ChangeState(EEntry);
        popup->SetChangeState( ETrue );
        iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);

        return ETrue;
        }   
    else if(aKey == EStdKeyUpArrow)   //up key
        {
        if(UIContainer()->CandidatePane()->IsFirstPage())
            {
            if ( EPinyin ==iOwner->FepMan()->InputMode() )
                {
                iOwner->ChangeState(EEntry);
                iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
                }
            return ETrue;
            }
        else
            {
            return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
            }   
        }
    else if ( aKey == EStdKeyDownArrow )
        {
        TBool state = ETrue;
        if ( popup->IsChangeStatet( ) && aLength == ELongKeyPress )
            {
            state = EFalse;
            }
        else
            {
            popup->SetChangeState( EFalse );
            }
        if ( state )
            {
            return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(
                            aKey, aLength );
            }
        }
    else if (aKey == EStdKeyLeftArrow)//in the same row
        {
        if (selected == 0)
           {
            UIContainer()->CandidatePane()->SelectIndex(numbertotal-1);
           }
        else
           {
            UIContainer()->CandidatePane()->SelectIndex(selected - 1);
           }      
        return ETrue;   
        }
    else if (aKey == EStdKeyRightArrow)//in the same row
        {
        if (selected == numbertotal-1)
            {
            UIContainer()->CandidatePane()->SelectIndex(0);
            }
        else
            {
            UIContainer()->CandidatePane()->SelectIndex(selected + 1);
            }     
        return ETrue;       
        } 
    else if ( aKey == EStdKeyDevice0 || 
              aKey == EStdKeyDevice3 ||
              aKey == EStdKeyEnter )//SK1,Select,Enter
        {
        TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
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
                iOwner->ChangeState(EPredictiveCandidate);
                }
            }
        return ETrue;
        }   
    else
        {
        return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }  
        
    return EFalse;     
    }

// End of file
