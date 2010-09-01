/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIInputStatePinyin methods.
*
*/












#include "AknFepPanic.h"
#include "AknFepUiInputStateEntryPinyin.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepManager.h"

#include <PtiEngine.h>                          //CPtiEngine
#include <PtiDefs.h>                            //keys
#include <avkon.rsg>

const TText KPinyinTone4Valid = 0x0020;
const TText KPinyinTone4Invalid = 0x02D9;


TAknFepInputStateEntryPinyin::TAknFepInputStateEntryPinyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    iState = EEntry;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane(EFalse);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    uiContainer->CandidatePane()->SelectFirst(); 
    uiContainer->ShowVerticalScrollArrows(EFalse);
    uiContainer->ShowHorizontalScrollArrows(EFalse);
    uiContainer->InputPane()->SetOverrideFontId(0);

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if (ptiengine->InputMode() != EPtiEnginePinyin)
        {
        ptiengine->SetInputMode(EPtiEnginePinyin);
        }
    ptiengine->SetCandidatePageLength(MAknFepUICtrlContainerChinese::ELayoutInput);
    ptiengine->EnableToneMarks(ETrue); 

    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    ImplicitlyUpdateSelection();

    // however we also need to clear the deliberate selection, in case we are not
    // coming back to the input pane from the candidate pane
    ClearDeliberateSelection();
    }
void TAknFepInputStateEntryPinyin::HandleCommandL(TInt /*aCommandId*/)
	{	
    DeliberatelyUpdateSelection();	
 	}

TBool TAknFepInputStateEntryPinyin::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    if(aKey == EKeyBackspace)
        {
		if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            ImplicitlyUpdateSelection();
            }
        else
            {
            ClearDeliberateSelection();
            iOwner->FepMan()->TryCloseUiL(); // no more keys, close the UI.
            if (aLength == ELongKeyPress)
                {
                iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                }
            }
        }
    else if(aKey == EKeyRightArrow)
        {
        if(popup->IsEnabled())
            {
            if(popup->SelectNextPhrase())
                {
                DeliberatelyUpdateSelection();
                }
            }
        }
    else if(aKey == EKeyLeftArrow)
        {
        if(popup->IsEnabled())
            {
            if(popup->SelectPrevPhrase())
                {
                DeliberatelyUpdateSelection();
                }
            }
        }
    else if(aKey == EKeyOK || aKey == EKeyDownArrow)
        {
        if(popup->IsEnabled())
            {
            popup->Enable(EFalse);
            }
        iOwner->ChangeState(ECandidate);        
        }
    else if (aLength == EShortKeyPress) // don't want repeats on these keys
        {
     	iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);        
        if(iOwner->IsValidChineseInputKey(aKey))
            {
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
                
                ImplicitlyUpdateSelection();
                }
            
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }           
            }
        else if(aKey == EPtiKeyStar) // we increment the tone mark.
            {
            if(ptiengine->IncrementToneMark(ETrue))
                {
                ImplicitlyUpdateSelection();
                }
            }
        }
        
    return ETrue;
    }

void TAknFepInputStateEntryPinyin::DeliberatelyUpdateSelection()
    {
    TPtr deliberateSelection = iOwner->GetLatestDeliberateSelection();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    // the selected is based on whole spelling candidates, not only current display page
    TInt selected = popup->CurrentSelection(); 
    ptiengine->EnableToneMarks(EFalse);
    TPtrC spelling = ptiengine->GetPhoneticSpelling(selected + 1); // our index is zero based, engine index is one based
    deliberateSelection = spelling.Left(
                MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark);
    ptiengine->EnableToneMarks(ETrue);
    
    RefreshUI(selected);
    }

void TAknFepInputStateEntryPinyin::ImplicitlyUpdateSelection()
    {
    TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
    TInt oldDeliberateSelectionLength = oldDeliberateSelection.Length();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    ptiengine->EnableToneMarks(EFalse);
    TInt pinyinCount = ptiengine->PhoneticSpellingCount();
    TInt maxUIPinyinCount = MAknFepUICtrlPinyinPopup::EMaxCandidates;
    TInt visible = pinyinCount < maxUIPinyinCount ? pinyinCount : maxUIPinyinCount;
    TInt newSelection = 0;
    for(TInt i = 0; i < visible; i++)
        {
        TPtrC spelling = ptiengine->GetPhoneticSpelling(i + 1);  // our index is zero based, engine index is one based
        TInt spellingLength = spelling.Length();

        // figure out how many characters we are comparing
        TInt compareLength = oldDeliberateSelectionLength <= spellingLength ? 
            oldDeliberateSelectionLength : spellingLength;

        if(oldDeliberateSelection.Left(compareLength) == spelling.Left(compareLength))
            {
            // as soon as a substring match is found, the 
            // highlight position is set to that candidate in the new list. 
            // note that in the case of the first character entered, the
            // oldBuf will be empty so it will always match... which is 
            // fine as we want to select the top one anyway, so we will
            // quit the loop early.
            newSelection = i;
            break;
            }
        }
    ptiengine->EnableToneMarks(ETrue);
    UIContainer()->PinyinPopupWindow()->SetFlag(MAknFepUICtrlPinyinPopup::ESpellingChanged);
    RefreshUI(newSelection);
    }

void TAknFepInputStateEntryPinyin::ClearDeliberateSelection()
    {
    // we must have just deleted the last character, 
    // or we are starting a new pinyin session, so wipe the last deliberate selection
    TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
    oldDeliberateSelection = KNullDesC;
    }

void TAknFepInputStateEntryPinyin::RefreshUI(TInt aSelection)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();

    // get cursor position
    TPoint baseLine = TPoint(0,0);  
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }

    TText toneMark;
    TBool toneMarkEntered = ptiengine->ToneMark(toneMark);
    TText invalidToneMark = toneMark;
    TBuf<1> validToneMarkBuf;
    TBuf<1> invalidToneMarkBuf;
    if(toneMarkEntered)
        {
        // override specific invalid tonemark character only, the others are the same character 
        // when both valid and invalid
        if(toneMark == KPinyinTone4Valid)
            invalidToneMark = KPinyinTone4Invalid;
        validToneMarkBuf.Append(invalidToneMark);
        invalidToneMarkBuf.Append(invalidToneMark);
        }

    // to start with, disable the tone marks so we can get the list of all pinyin matches
    // later we will figure out which are invalid
    ptiengine->EnableToneMarks(EFalse);
    TInt pinyinCountWithoutToneMarks = ptiengine->PhoneticSpellingCount();
    TInt maxUIPinyinCount = popup->EMaxCandidates;
    TInt visible = pinyinCountWithoutToneMarks < maxUIPinyinCount ? 
            pinyinCountWithoutToneMarks : maxUIPinyinCount;
    TInt visibleCount = 0;
    TInt spellingStartIndex = 0;
    TBool pinyinSpellingChanged = popup->IsFlagSet(MAknFepUICtrlPinyinPopup::ESpellingChanged);
    TBool pinyinDisplayPageChanged = popup->IsFlagSet(MAknFepUICtrlPinyinPopup::EDispPageChanged);

    if(pinyinCountWithoutToneMarks > 0)
        {
        if(pinyinSpellingChanged)
            {
            visibleCount = visible;
            spellingStartIndex = 0;
            popup->SetVisibleCount(visible);
            }
        else if(pinyinDisplayPageChanged) 
            {
            visibleCount = popup->VisibleSelectionCount();
            spellingStartIndex = popup->CurrentPageStartIndex();
            }
        if(pinyinDisplayPageChanged || pinyinSpellingChanged)
            {
            TInt index;
            for(TInt i = 0; i < visibleCount; i++)
                {
                if(pinyinDisplayPageChanged)
                    {
                    index = spellingStartIndex + i;
                    }
                else
                    {
                    index = i;
                    }
                TPtrC spelling = ptiengine->GetPhoneticSpelling(index + 1);  // our index is zero based, engine index is one based
                popup->SetItemText(i, spelling);
                popup->SetToneMark(i, validToneMarkBuf); 
                if(i == aSelection)
                    {
                    // fill in input pane even if it can't be seen, for when we have to hide popup window
                    inputPane->SetText(spelling);
                    inputPane->SetToneMark(validToneMarkBuf); 
                    }
                }
            popup->ClearFlag(MAknFepUICtrlPinyinPopup::EDispPageChanged);
            }
        if(pinyinSpellingChanged)
            {
            popup->SplitSpellingIntoPages(visible);
            }
        popup->SetDisplayPage(aSelection);
        
        if(pinyinSpellingChanged)
            {
            popup->ClearFlag(MAknFepUICtrlPinyinPopup::ESpellingChanged);
            visibleCount = popup->VisibleSelectionCount();
            spellingStartIndex = popup->CurrentPageStartIndex();
            TInt index;
            for(TInt i = 0; i < visibleCount; i++)
                {
                index = spellingStartIndex + i;
                
                TPtrC spelling = ptiengine->GetPhoneticSpelling(index + 1);  // our index is zero based, engine index is one based
                popup->SetItemText(i, spelling);
                popup->SetToneMark(i, validToneMarkBuf); 
                if(i == aSelection)
                    {
                    // fill in input pane even if it can't be seen, for when we have to hide popup window
                    inputPane->SetText(spelling);
                    inputPane->SetToneMark(validToneMarkBuf); 
                    }
                }
            }
        else
            {
            inputPane->SetText(ptiengine->GetPhoneticSpelling(aSelection + 1));
            inputPane->SetToneMark(validToneMarkBuf); 
            }
        popup->PopupSizeChanged();
        }

    // turn tone marks back on so that we know where we are.
    ptiengine->EnableToneMarks(ETrue);

    // this is where we start to figure out whether the tonemarks are valid,
    // whether the selected tonemark is valid, and what is the index of the 
    // selected candidate in the list of candidates with tonemarks...
    TBool selectionToneMarkValid = EFalse;
    TInt selectionIndexAdjustedForToneMarkValidity = aSelection;

    // we only need to deal with tone marks if there is one
    if(toneMarkEntered)
        {
        if(pinyinCountWithoutToneMarks > 0)
            {
            visibleCount = visible;

            for(TInt i = 0; i < visibleCount; i++)
                {
                TBool valid = EFalse;
                
                // compare the list of answers with the ones in the UI controls... 
                // any missing from this list don't support the tone mark
                TBuf<MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark> nextCandidateWithoutToneMark;
                popup->GetItemText(i, nextCandidateWithoutToneMark);

                // disable tone marks temporarily, as we need to switch the spelling, and it might have 
                // no valid tonemarks
                ptiengine->EnableToneMarks(EFalse); 

                // temporarily change the current selection in the engine, so 
                // that we can see what tonemarks would be valid for it
                // we will set the real selection later on

                ptiengine->SelectPhoneticSpelling(spellingStartIndex + i + 1);
                // if it's valid, we need to find it's position in the with tonemark list
                // as the lists may be ordered differently, and different lengths
                valid = ptiengine->IsToneMarkValidForSpelling();
                if(valid)
                    {
                    ptiengine->EnableToneMarks(ETrue); 
                    TInt pinyinCountWithToneMarks = ptiengine->PhoneticSpellingCount();

                    for(TInt j = 0; j < pinyinCountWithToneMarks; j++)
                        {

                        // use j here not i as we are looking at the list with tonemarks
                        TPtrC nextCandidateWithToneMark = ptiengine->GetPhoneticSpelling(j + 1);  // our index is zero based, engine index is one based

                        if(nextCandidateWithToneMark == nextCandidateWithoutToneMark)
                            {
                            // we are setting the tone mark for the i'th entry in the list without tonemarks
                            // but if its tonemark is valid, the selection in the engine will have to be relative
                            // to the list with tonemarks
                            if( i + spellingStartIndex == aSelection)
                                {
                                selectionToneMarkValid = ETrue;
                                selectionIndexAdjustedForToneMarkValidity =  j;

                                // fill in input pane even if it can't be seen, for when we have to hide popup window
                                inputPane->SetText(nextCandidateWithoutToneMark);
                                }
                            break;
                            }
                        }
                    }
                popup->SetItemToneMarkValidity(i, valid); 
                popup->SetToneMark(i, valid ? validToneMarkBuf : invalidToneMarkBuf); 

                // fill in input pane even if it can't be seen, for when we have to hide popup window
                if(i == aSelection)
                    {
                    inputPane->SetToneMarkValidity(valid); 
                    inputPane->SetToneMark(valid ? validToneMarkBuf : invalidToneMarkBuf); 
                    }
                
                }
            }
    }
    // set the spelling for the selection with no tone mark
    ptiengine->EnableToneMarks(EFalse); 
    ptiengine->SelectPhoneticSpelling(popup->CurrentSelection() + 1); // our index is zero based, engine index is one based
    
    // now switch the tonemark on only if the selection has a valid tonemark
    // and then set the spelling to the adjusted tonemark position
    ptiengine->EnableToneMarks(selectionToneMarkValid); 
    ptiengine->SelectPhoneticSpelling(selectionIndexAdjustedForToneMarkValidity + 1); // our index is zero based, engine index is one based

    // update the candidate pane.
    uiContainer->CandidatePane()->SetCandidateBuffer(ptiengine->CandidatePage());
    uiContainer->Enable(ETrue);

    // need to enable the pinyin popup after the container so that it is raised to the front
    popup->Enable(pinyinCountWithoutToneMarks > 0);
    }

void TAknFepInputStateEntryPinyin::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AVKON_SOFTKEYS_EMPTY);
    }
// End of file
