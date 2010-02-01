/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the  TAknFepInputStateEntryMiniQwertyPinyinPhrase methods.
*
*/












#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "aknfepuiinputstateentryhalfqwertyphrase.h"

#include "AknFepManager.h"                  //FepMan flag
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "AknFepUiCtrlContainerChinese.h"
#include <AknFep.rsg>
#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>

const TInt KFnMappedCharacters = 5;
const TInt KInputPaneLength = 32;
const TText KPinyinTone4Valid = 0x02D9;
const TText KPinyinTone4Invalid = 0x0020;
const TInt KMaxPhraseCount = 150;

//For Pinyin
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::TAknFepInputStateEntryHalfQwertyPinyinPhrase
// the construt funtion 
// ---------------------------------------------------------------------------
//
TAknFepInputStateEntryHalfQwertyPinyinPhrase::TAknFepInputStateEntryHalfQwertyPinyinPhrase(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBaseChinesePhrase(aOwner,aUIContainer)                           
    {
    iState = EEntry;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane(EFalse);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    uiContainer->CandidatePane()->SelectFirstPhrase(); 
    uiContainer->ShowHorizontalScrollArrows(EFalse); 
    uiContainer->ShowVerticalScrollArrows(EFalse);
    uiContainer->InputPane()->SetOverrideFontId(0);
    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    ImplicitlyUpdateSelection();
    UpdateIndicator();
    ClearDeliberateSelection();
    iOwner->PtiEngine()->SetInputMode(EPtiEnginePinyinPhraseHalfQwerty);
    iOwner->PtiEngine()->SetCandidatePageLength(KMaxPhraseCount);
    iOwner->PtiEngine()->SetCase(EPtiCaseLower);
    }
                               
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleKeyL
// handle key event
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryHalfQwertyPinyinPhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    TBool state = ETrue;
    TPtrC text;
    TBool ret = ETrue;
    if ( aLength == EShortKeyPress )
    	{
    	switch (aKey)
			{
			case EPtiKeyQwerty1:
				{
				aKey = EPtiKeyQwertyE;
				break;
			}
		case EPtiKeyQwerty2:
			{
			aKey = EPtiKeyQwertyT;
			break;
			}
		case EPtiKeyQwerty3:
			{
			aKey = EPtiKeyQwertyU;
			break;
			}
		case EPtiKeyQwerty4:
			{
			aKey = EPtiKeyQwertyD;
			break;
			}
		case EPtiKeyQwerty5:
			{
			aKey = EPtiKeyQwertyG;
			break;
			}
		case EPtiKeyQwerty6:
			{
			aKey = EPtiKeyQwertyJ;
			break;
			}
		case EPtiKeyQwerty7:
			{
			aKey = EPtiKeyQwertyC;
			break;
			}
		case EPtiKeyQwerty8:
			{
			aKey = EPtiKeyQwertyB;
			break;
			}
		case EPtiKeyQwerty9:
			{
			aKey = EPtiKeyQwertyM;
			break;
			}
			case EPtiKeyQwerty0:
				{
				aKey = EPtiKeyQwertySpace;
				break;
				}
			}
    	}
	
    if ( aKey != EStdKeyLeftFunc && 
        (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed)))
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return ETrue;
        }
    if(( aKey == EStdKeyLeftFunc  || aKey == EStdKeyFullStop) && aLength == EShortKeyPress) //chr and punctuation key to tone mark   
        {
        if(ptiengine->IncrementToneMark(ETrue))
            {
            ImplicitlyUpdateSelection();
            }
        }
    else if( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() ||     //
        CAknFepFnKeyManager::EFnKeyLock == iOwner->FepMan()->FnKeyState() ||
        CAknFepFnKeyManager::EFnKeyDown == iOwner->FepMan()->FnKeyState())
        {
        popup->SetPreStateIsPredictive( EFalse );
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }                                                    
    else if(aKey == EStdKeyBackspace )
        {
        if ( popup->IsChangeStatet() && aLength == ELongKeyPress )
            {
            state = EFalse;
            }
        else
            {
            popup->SetChangeState( EFalse );
            }
        if ( state )
            {
            if (iOwner->PtiEngine()->DeleteKeyPress().Length())
                {
                ImplicitlyUpdateSelection();
                }
            else
                {
                iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                popup->SetPreStateIsPredictive( EFalse );
                if ( aLength == ELongKeyPress )
                    {
                    iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                    }
                }
            }
        } 
    else if(aKey ==EStdKeyDevice1)//sk2
        {
        iOwner->FepMan()->TryCloseUiL();
        popup->SetPreStateIsPredictive( EFalse );
        }
    else if(aKey == EStdKeyRightArrow)
        {
        if(popup->IsEnabled())
            {
            if(popup->SelectNextPhrase())
                {
                DeliberatelyUpdateSelection();
                }
            }
        }
    else if(aKey == EStdKeyLeftArrow)
        {
        if(popup->IsEnabled())
            {
            if(popup->SelectPrevPhrase())
                {
                DeliberatelyUpdateSelection();
                }
            }
        } 
    else if (aKey == EStdKeyDownArrow
            || aKey == EStdKeyDevice3
            || aKey == EStdKeyDevice0
            ||aKey == EStdKeyEnter) //Downkey,sk1,selection,enter key
        {
        if(popup->IsEnabled())
            {
            popup->Enable(EFalse);
            }
        iOwner->ChangeState(ECandidate);
        popup->SetPreStateIsPredictive( EFalse );
        popup->SetChangeState( ETrue );
        }  
    else if ( aKey == EStdKeyUpArrow )
        {
        if ( !candidatePane->IsFirstPage() )
            {
            candidatePane->PreviousCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirst();
            UpdateIndicator();
            }
        }
    else if(aLength == EShortKeyPress) 
        {
        iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);            
        
        if(iOwner->IsValidChineseInputKeyQwerty(aKey) || aKey ==EStdKeySpace ) //seperator
            {
            TInt stringBeforeLength(0);
            TInt stringAfterLength(0);
            stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
            stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
            if (stringAfterLength==0 && aKey == EPtiKeyQwertyU )  //u,i should not as first character in pinyin
               {
               iOwner->FepMan()->TryCloseUiL();
               popup->SetPreStateIsPredictive( EFalse );
               iOwner->FepMan()->UpdateCbaL(NULL);
//               iOwner->ChangeState(EQwerty);
               iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
               return ETrue;     
               }
            if (stringBeforeLength != stringAfterLength)
                {
                if (ptiengine->GetPhoneticSpelling(1).Length() == 1)
                    {
                    iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
                    }
                ImplicitlyUpdateSelection();
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            }
        else
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }
        if ( popup->PhoneticSpellingArray()->Count() > 0 &&
            popup->PhoneticSpellingArray()->MdcaPoint( 0 ).Length() > 1 )
            {
            popup->SetPreStateIsPredictive( EFalse );
            }
        }
    else 
        {
        if ( popup->GetPreStateIsPredictive() && 
            popup->PhoneticSpellingArray()->Count() > 0 &&
        		popup->PhoneticSpellingArray()->MdcaPoint( 0 ).Length() == 1)    //long press fn mode character should inputted
            {
            iOwner->FepMan()->TryCloseUiL();
            popup->SetPreStateIsPredictive( EFalse );
            if (aKey == EStdKeyEnter)
                {
                // PtiEngine does not have mapping for the enter key.
                const TText KAknFEPLineFeedSymbol = 0x21B2;
                TBuf<1> lineFeed;
                lineFeed.Zero();
                lineFeed.Append(KAknFEPLineFeedSymbol);
                text.Set(lineFeed);
                }
            else
                {
                TBuf<KFnMappedCharacters> allFnMappedCharacters;
                ptiengine->MappingDataForKey((TPtiKey)aKey, allFnMappedCharacters, EPtiCaseFnLower);
                text.Set(allFnMappedCharacters);
                }
            if (text.Length() > 0)
                {
                TBuf<1> chr;
                chr.Append(text[0]);
                iOwner->FepMan()->NewCharacterL(chr);
                iOwner->FepMan()->CommitInlineEditL();
                ptiengine->ClearCurrentWord();
                ret = ETrue;
                }
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::DeliberatelyUpdateSelection
// DeliberatelyUpdateSelection
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyPinyinPhrase::DeliberatelyUpdateSelection()
    {
    TPtr deliberateSelection = iOwner->GetLatestDeliberateSelection();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    TInt selected = popup->CurrentSelection();
    ptiengine->EnableToneMarks(EFalse);
    // our index is zero based, engine index is one based
    TPtrC spelling = ptiengine->GetPhoneticSpelling(selected + 1); 
    deliberateSelection = spelling.Left(MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark);
    ptiengine->EnableToneMarks(ETrue);
    UIContainer()->PinyinPopupWindow()->SetFlag(MAknFepUICtrlPinyinPopup::ESpellingNavigation);    
    RefreshUI(popup->CurrentVisibleSelection());
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::ImplicitlyUpdateSelection
// update the selection
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyPinyinPhrase::ImplicitlyUpdateSelection()
    {
    TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
    TInt oldDeliberateSelectionLength = oldDeliberateSelection.Length();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    // previously selected is compared in turn to each of the new candidates
    ptiengine->EnableToneMarks(EFalse);
    TInt pinyinCount = ptiengine->PhoneticSpellingCount();
    TInt newSelection = 0;
    for(TInt i = 0; i < pinyinCount; i++)
        {
        TPtrC spelling = ptiengine->GetPhoneticSpelling(i + 1);  // our index is zero based, engine index is one based
        TInt spellingLength = spelling.Length();
        // figure out how many characters we are comparing, this depends on the 
        // direction of the edit
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

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::ClearDeliberateSelection
// clear the deliberate selection
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyPinyinPhrase::ClearDeliberateSelection()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    TInt spellingCount = popup->PhoneticSpellingArray()->Count();
    // we must have just deleted the last character, 
    // or we are starting a new zhuyin session, so wipe the last deliberate selection
    if ( 0 == spellingCount )
        {
        TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
        oldDeliberateSelection = KNullDesC;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::RefreshUI
// refresh ui 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyPinyinPhrase::RefreshUI(TInt aSelection)
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
    TBuf<1> validToneMarkBuf;
    if(toneMarkEntered)
        {
        // override specific invalid tonemark character only, the others are the same character 
        // when both valid and invalid
        if(toneMark == KPinyinTone4Invalid)
            toneMark = KPinyinTone4Valid;
        validToneMarkBuf.Append(toneMark);
        }
    // to start with, disable the tone marks so we can get the list of all zhuyin matches
    // later we will figure out which ones are invalid
    ptiengine->EnableToneMarks(EFalse);
    TInt pinyinCountWithoutToneMarks = ptiengine->PhoneticSpellingCount();
    if(pinyinCountWithoutToneMarks > 0)
        {
        if(popup->IsFlagSet(MAknFepUICtrlPinyinPopup::ESpellingChanged))
            {
            CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
            TRAPD(ignore, ptiengine->GetPhoneticSpellingsL(*spelling));
            if (ignore != KErrNone)
                {
                return;
                }
            popup->SplitPhraseSpellingIntoPages(); 
            }
        // set display page for delibrate selection
        popup->SetDisplayPage(aSelection);
        popup->SetPhraseItemTexts();
        // fill in input pane although it's invisible
        inputPane->SetText(ptiengine->GetPhoneticSpelling(popup->CurrentSelection() + 1));
        popup->PopupSizeChanged(); // phrase input
        }
    // turn tone marks back on so that we know where we are.
    ptiengine->EnableToneMarks(ETrue);
    // this is where we start to figure out whether the tonemarks are valid,
    // whether the selected tonemark is valid, and what is the index of the 
    // selected candidate in the list of candidates with tonemarks...
    TBool selectionToneMarkValid = EFalse;
    TInt selectionIndexAdjustedForToneMarkValidity = popup->CurrentSelection();
    // we only need to deal with tone marks if there is one
    if(toneMarkEntered)
        {
        if(pinyinCountWithoutToneMarks > 0)
            {
            for(TInt i = 0; i < pinyinCountWithoutToneMarks; i++)
                {
                TBool valid = EFalse;
                
                TBuf<MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark> nextCandidateWithoutToneMark;
                nextCandidateWithoutToneMark.Copy(ptiengine->GetPhoneticSpelling
                    (i + 1).Left(MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark));
                ptiengine->EnableToneMarks(EFalse); 
                ptiengine->SelectPhoneticSpelling(i + 1);
                valid = ptiengine->IsToneMarkValidForSpelling();
                if(valid)
                    {
                    ptiengine->EnableToneMarks(ETrue); 
                    TInt pinyinCountWithToneMarks = ptiengine->PhoneticSpellingCount();
                    for(TInt j = 0; j < pinyinCountWithToneMarks; j++)
                        {
                        // use j here not i as we are looking at the list with tonemarks
                        TPtrC nextCandidateWithToneMark = ptiengine->GetPhoneticSpelling(j + 1);  
                        if(nextCandidateWithToneMark == nextCandidateWithoutToneMark)
                            {
                            if(i == aSelection)
                                {
                                selectionToneMarkValid = ETrue;
                                
                                // fill in input pane even if it can't be seen, for when we have to hide popup window
                                inputPane->SetText(nextCandidateWithoutToneMark);
                                }
                            break;
                            }
                        }
                    }
                }
            }
        }
    ptiengine->EnableToneMarks(EFalse); 
    ptiengine->SelectPhoneticSpelling(popup->CurrentSelection() + 1); 
    // next two lines are for tonemark validity
    ptiengine->EnableToneMarks(selectionToneMarkValid); 
    ptiengine->SelectPhoneticSpelling(selectionIndexAdjustedForToneMarkValidity + 1); // our index is zero based, engine index is one based
    // update the candidate pane.
    if(popup->IsFlagSet(MAknFepUICtrlPinyinPopup::ESpellingChanged) 
        || popup->IsFlagSet(MAknFepUICtrlPinyinPopup::ESpellingNavigation))
        {
        TRAPD(ignore, ptiengine->GetChinesePhraseCandidatesL(*(uiContainer->CandidatePane()->CandidateArray())));
        if (ignore != KErrNone)
            {
            return;
            }
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages(); 
        popup->ClearFlag(MAknFepUICtrlPinyinPopup::ESpellingChanged 
                        | MAknFepUICtrlPinyinPopup::ESpellingNavigation);
        }
    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable(ETrue);
    // need to enable the pinyin popup after the container so that it is raised to the front
    popup->Enable(pinyinCountWithoutToneMarks > 0);
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::InitializeStateL
// updata the cba 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyPinyinPhrase::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::UpdateIndicator
// updata the indicator
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyPinyinPhrase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        

    uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
    uiContainer->CandidatePane()->ShowDownScrollArrows(EFalse);     
    uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
    uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);        
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyPinyinPhrase::IsCharacterKey
// to check whether the key is character or not
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryHalfQwertyPinyinPhrase::IsCharacterKey(
    const TInt aKey )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine( );
    // Max length doesn't matter because we'll need only the first character.    
    TBuf<KMaxName> lowerdata;
    ptiengine->MappingDataForKey( (TPtiKey)aKey, lowerdata, EPtiCaseLower );
    if ( lowerdata.Length( ) > 0 )
        {
        TChar lower = lowerdata[0];
        TUint upper = lower.GetUpperCase( );
        if ( TUint( lower ) != upper )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

//For stroke
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyStrokePhrase::TAknFepInputStateEntryHalfQwertyStrokePhrase
// the construt funtion 
// ---------------------------------------------------------------------------
//
TAknFepInputStateEntryHalfQwertyStrokePhrase::TAknFepInputStateEntryHalfQwertyStrokePhrase(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBaseChinesePhrase(aOwner,aUIContainer)                           
    {
    iState = EEntry;
    iOwner->PtiEngine()->SetInputMode( EPtiEngineStrokePhraseHalfQwerty );
    iOwner->PtiEngine()->SetCase( EPtiCaseLower );
    iOwner->PtiEngine()->SetCandidatePageLength( KMaxPhraseCount );
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    uiContainer->ShowHorizontalScrollArrows(EFalse); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    UpdateIndicator();
    }
                               
//For stroke
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyStrokePhrase::HandleKeyL
// handle the key event of stroke 
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryHalfQwertyStrokePhrase::HandleKeyL(TInt aKey,
        TKeyPressLength aLength )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBuf<KInputPaneLength> currentText;
    TPtrC text;
    TBool state = ETrue;
    TInt spellingLen = 0;
    TBool ret = ETrue;
    
    if ( aKey == EPtiKeyQwerty0 )
    	{
    	aKey = EPtiKeyQwertySpace;
    	}
   
    if ( aKey != EStdKeyLeftFunc && 
        (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed)))
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return ETrue;
        }
    switch (aKey)
        {
        case EStdKeyBackspace:
            {
            if (popup->IsChangeStatet() && aLength == ELongKeyPress)
                {
                state = EFalse;
                }
            else
                {
                popup->SetChangeState(EFalse);
                }
            if (state)
                {
                if (iOwner->PtiEngine()->DeleteKeyPress().Length())
                    {
                    inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
                    RefreshUI();
                    }
                else
                    {
                    iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                    popup->SetPreStateIsPredictive( EFalse );
                    if (aLength == ELongKeyPress)
                        {
                        iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                        }
                    }
                }
            break;
            }
        case EStdKeyDevice1://sk2
            {
            iOwner->FepMan()->TryCloseUiL();
            popup->SetPreStateIsPredictive( EFalse );
            break;
            }
        case EStdKeyUpArrow:
            {
            if ( !candidatePane->IsFirstPage() )
                {
                candidatePane->PreviousCandidatePage();
                candidatePane->SetCandidateBuffer();
                candidatePane->SelectFirst();
                UpdateIndicator();
                }
            break;
            }
        case EStdKeyRightArrow:
            {
            if (candidatePane->VisibleCandidateCount()!= 1)
                {
                iOwner->ChangeState(ECandidate);
                popup->SetPreStateIsPredictive( EFalse );
                candidatePane->SelectNext();
                }
            break;
            }
        case EStdKeyLeftArrow:
            {
            if (candidatePane->VisibleCandidateCount()!= 1)
                {
                iOwner->ChangeState(ECandidate);
                popup->SetPreStateIsPredictive( EFalse );
                TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
                UIContainer()->CandidatePane()->SelectIndex(numbertotal-1);
                }
            break;
            }
        case EStdKeyDownArrow:
            {
            if ( candidatePane->IsLastPage() )
                {
                return ETrue;
                }
            if ( candidatePane->NeedGetNewCandidatePage( ) )
                {
                if ( !iOwner->PtiEngine()->NextCandidatePage( ) )
                    {
                    if ( candidatePane->IsLastPage() )
                        {
                        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                        return ETrue;
                        }
                    }
                else
                    {
                    CDesCArrayFlat* phraseCandidates =
                        candidatePane->CandidateArray( );
                    RPointerArray<HBufC> list;
                    GetNewCandidatesL( iOwner->PtiEngine()->CandidatePage( ), list );
                    for (TInt i = 0; i < list.Count( ); ++i )
                        {
                        phraseCandidates->AppendL( *(list[i]) );
                        }
                    list.ResetAndDestroy();
                    }
                }
            candidatePane->NextCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirstPhrase();
            UpdateIndicator();
            break;
            }
        case EStdKeyDevice3://Select
        case EStdKeyDevice0://sk1
        case EStdKeyEnter:
            {
            uiContainer->PinyinPopupWindow()->Enable(EFalse);
            TPtrC currentPhraseText = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
            if(currentPhraseText.Length())
                {            
                fepMan->NewTextL(currentPhraseText);
                fepMan->CommitInlineEditL();
                iOwner->PtiEngine()->SetPredictiveChineseChar(currentPhraseText);
                if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                    {
                    fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                    iOwner->FepMan()->TryCloseUiL();
                    popup->SetPreStateIsPredictive( EFalse );
                    }
                else
                    {
                    iOwner->ChangeState(EPredictiveCandidate);
                    popup->SetPreStateIsPredictive( EFalse );
                    }
                }
            break;
            }
        default:
            {
            if (aLength == EShortKeyPress)
                {
                iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);
                if (iOwner->IsValidChineseInputKeyQwerty(aKey) ||
                        aKey ==EStdKeySpace ||
                        aKey == EStdKeyFullStop ||
                        aKey == EStdKeyLeftFunc ) //seperator
                    {
                    TInt stringBeforeLength(0);
                    TInt stringAfterLength(0);
                    stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
                    stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
                    if (stringBeforeLength != stringAfterLength)
                        {
                        if (ptiengine->GetPhoneticSpelling(1).Length() == 1)
                            {
                            iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
                            }
                        inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
                        RefreshUI();
                        }
                    else
                        {
                        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                        }
                    }
                else
                    {
                    iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                    }
                inputPane->GetText(currentText);
                spellingLen = currentText.Length();
                if ( spellingLen > 1 )
                    {
                    popup->SetPreStateIsPredictive( EFalse );
                    }
                }
            else if ( popup->GetPreStateIsPredictive() && 0 == spellingLen)  //long press fn mode character should inputted
                {
                iOwner->FepMan()->TryCloseUiL();
                popup->SetPreStateIsPredictive( EFalse );

                TBuf<KFnMappedCharacters> allFnMappedCharacters;
                ptiengine->MappingDataForKey( (TPtiKey)aKey,
                    allFnMappedCharacters, EPtiCaseFnLower );
                text.Set( allFnMappedCharacters );
                if ( text.Length( ) > 0 )
                    {
                    TBuf<1> chr;
                    chr.Append( text[0] );
                    iOwner->FepMan()->NewCharacterL( chr );
                    iOwner->FepMan()->CommitInlineEditL();
                    ptiengine->ClearCurrentWord();
                    ret = ETrue;
                    }
                }
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyStrokePhrase::InitializeStateL
// change the cba 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyStrokePhrase::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyStrokePhrase::IsCharacterKey
// to check whether the key is character or not
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryHalfQwertyStrokePhrase::IsCharacterKey(
    const TInt aKey )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine( );
    // Max length doesn't matter because we'll need only the first character.    
    TBuf<KMaxName> lowerdata;
    ptiengine->MappingDataForKey( (TPtiKey)aKey, lowerdata, EPtiCaseLower );
    if ( lowerdata.Length( ) > 0 )
        {
        TChar lower = lowerdata[0];
        TUint upper = lower.GetUpperCase( );

        if ( TUint( lower ) != upper )
            {
            return ETrue;
            }
        }

    return EFalse;
    }   

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyStrokePhrase::UpdateIndicator
// updata the indicator
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyStrokePhrase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    
    if (candidatePane->IsFirstPage())
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
    if (candidatePane->IsLastPage())
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows(EFalse);     
        }
    else
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows(ETrue);      
        }

    uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
    uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);   
    }

//for zhuyin
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyZhuyinPhrase::TAknFepInputStateEntryHalfQwertyZhuyinPhrase
// c++ construct funtion
// ---------------------------------------------------------------------------
//
TAknFepInputStateEntryHalfQwertyZhuyinPhrase::TAknFepInputStateEntryHalfQwertyZhuyinPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    uiContainer->CandidatePane()->SelectFirstPhrase(); 
    uiContainer->InputPane()->SetOverrideFontId(0);
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    uiContainer->ShowHorizontalScrollArrows(EFalse); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    iOwner->PtiEngine()->SetInputMode( EPtiEngineZhuyinPhraseHalfQwerty );
    iOwner->PtiEngine()->SetCase( EPtiCaseLower );
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyZhuyinPhrase::HandleKeyL
// handle the key of zhuyin 
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryHalfQwertyZhuyinPhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TInt originalKey = aKey;
    TPtrC text;
    TInt spellingLen = 0;
    TBool state = ETrue;
    TBool ret = ETrue;
    TBuf<KInputPaneLength> currentText;
    if ( aKey != EStdKeyLeftFunc && 
        (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed)))
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return ETrue;
        }
    switch (aKey)
        {
        case EPtiKeyQwertyE:
            aKey = EPtiKey1;
            break;
        case EPtiKeyQwertyT:
            aKey = EPtiKey2;
            break;
        case EPtiKeyQwertyU:
            aKey = EPtiKey3;
            break;
        case EPtiKeyQwertyD:
            aKey = EPtiKey4;
            break;
        case EPtiKeyQwertyG:
            aKey = EPtiKey5;
            break;
        case EPtiKeyQwertyJ:
            aKey = EPtiKey6;
            break;
        case EPtiKeyQwertyC:
            aKey = EPtiKey7;
            break;
        case EPtiKeyQwertyB:
            aKey = EPtiKey8;
            break;
        case EPtiKeyQwertyM:
            aKey = EPtiKey9;
            break;
        case EPtiKeyQwertySpace:
            aKey = EPtiKey0;
            break;
        }
    if( (aKey == EStdKeyLeftFunc || aKey == EStdKeyFullStop )&& 
            aLength == EShortKeyPress )
        {
        if( ptiengine->IncrementToneMark( ETrue ) )
            {
            inputPane->SetFlag( MAknFepUICtrlInputPane::EPhoneticChanged );
            RefreshUI();
            return ETrue;
            }
        }
    switch (aKey)
        {
        case EStdKeyBackspace:
            {
            if (popup->IsChangeStatet() && aLength == ELongKeyPress)
                {
                state = EFalse;
                }
            else
                {
                popup->SetChangeState(EFalse);
                }
            if (state)
                {
                if (iOwner->PtiEngine()->DeleteKeyPress().Length())
                    {
                    inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
                    RefreshUI();
                    }
                else
                    {
                    iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                    popup->SetPreStateIsPredictive( EFalse );
                    if (aLength == ELongKeyPress)
                        {
                        iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                        }
                    }
                }
            break;
            }
        case EStdKeyDevice1://sk2
            {
            iOwner->FepMan()->TryCloseUiL();
            popup->SetPreStateIsPredictive( EFalse );
            break;
            }
        case EStdKeyUpArrow:
            {
            if ( !candidatePane->IsFirstPage() )
                {
                candidatePane->PreviousCandidatePage();
                candidatePane->SetCandidateBuffer();
                candidatePane->SelectFirst();
                UpdateIndicator();
                }
            break;
            }
        case EStdKeyRightArrow:
            {
            if (candidatePane->VisibleCandidateCount()!= 1)
                {
                iOwner->ChangeState(ECandidate);
                popup->SetPreStateIsPredictive( EFalse );
                candidatePane->SelectNext();
                }
            break;
            }
        case EStdKeyLeftArrow:
            {
            if (candidatePane->VisibleCandidateCount()!= 1)
                {
                iOwner->ChangeState(ECandidate);
                popup->SetPreStateIsPredictive( EFalse );
                TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
                UIContainer()->CandidatePane()->SelectIndex(numbertotal-1);
                }
            break;
            }
        case EStdKeyDownArrow:
            {
            if ( candidatePane->IsLastPage() )
                {
                return ETrue;
                }
            if ( candidatePane->NeedGetNewCandidatePage( ) )
                {
                if ( !iOwner->PtiEngine()->NextCandidatePage( ) )
                    {
                    if ( candidatePane->IsLastPage() )
                        {
                        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                        return ETrue;
                        }
                    }
                else
                    {
                    CDesCArrayFlat* phraseCandidates =
                        candidatePane->CandidateArray( );
                    RPointerArray<HBufC> list;
                    GetNewCandidatesL( iOwner->PtiEngine()->CandidatePage( ), list );
                    for (TInt i = 0; i < list.Count( ); ++i )
                        {
                        phraseCandidates->AppendL( *(list[i]) );
                        }
                    list.ResetAndDestroy();
                    }
                }
            candidatePane->NextCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirstPhrase();
            UpdateIndicator();
            break;
            }
        case EStdKeyDevice3://Select
        case EStdKeyDevice0://sk1
        case EStdKeyEnter:
            {
            uiContainer->PinyinPopupWindow()->Enable(EFalse);
            TPtrC phraseText = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
            if(phraseText.Length())
                {            
                fepMan->NewTextL(phraseText);
                fepMan->CommitInlineEditL();
                iOwner->PtiEngine()->SetPredictiveChineseChar(phraseText);
                if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                    {
                    fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                    iOwner->FepMan()->TryCloseUiL();
                    popup->SetPreStateIsPredictive( EFalse );
                    }
                else
                    {
                    iOwner->ChangeState(EPredictiveCandidate);
                    popup->SetPreStateIsPredictive( EFalse );
                    }
                }
            break;
            }
        case EPtiKeyQwertyQ:
        case EPtiKeyQwertyA:
        case EPtiKeyQwertyO:
        case EPtiKeyQwertyL:
        case EPtiKeyQwertyZ:
            {
            ret = ETrue;
            break;
            }
        default:
            {
            if (aLength == EShortKeyPress)
                {
                iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);

                if (iOwner->IsValidChineseInputKeyQwerty(aKey) || aKey
                        ==EStdKeySpace) //seperator
                    {
                    ptiengine->AppendKeyPress((TPtiKey)aKey);
                    SetInputPaneToneMark();
                    if (ptiengine->GetPhoneticSpelling(1).Length() == 1)
                        {
                        iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
                        }
                    inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
                    RefreshUI();
                    }
                else
                    {
                    iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                    }
                inputPane->GetText(currentText);
                spellingLen = currentText.Length();
                if ( spellingLen > 1 )
                    {
                    popup->SetPreStateIsPredictive( EFalse );
                    }
                }
            else if ( popup->GetPreStateIsPredictive() && 0 == spellingLen)  //long press fn mode character should inputted
                {
                iOwner->FepMan()->TryCloseUiL();
                popup->SetPreStateIsPredictive( EFalse );

                TBuf<KFnMappedCharacters> allFnMappedCharacters;
                ptiengine->MappingDataForKey( (TPtiKey)originalKey,
                    allFnMappedCharacters, EPtiCaseFnLower );
                text.Set( allFnMappedCharacters );
                if ( text.Length( ) > 0 )
                    {
                    TBuf<1> chr;
                    chr.Append( text[0] );
                    iOwner->FepMan()->NewCharacterL( chr );
                    iOwner->FepMan()->CommitInlineEditL();
                    ptiengine->ClearCurrentWord();
                    ret = ETrue;
                    }
                }
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyZhuyinPhrase::KeyTimerExpired
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyZhuyinPhrase::KeyTimerExpired()
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
    inputPane->SetText(ptr);
    TBuf<KInputPaneLength> currentText;
    inputPane->GetText(currentText);
    if (ptr.Length() < currentText.Length())
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryHalfQwertyZhuyinPhrase::UpdateIndicator
// updata the indicator
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryHalfQwertyZhuyinPhrase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    
    if (candidatePane->IsFirstPage())
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
    if (candidatePane->IsLastPage())
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows(EFalse);     
        }
    else
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows(ETrue);      
        }

    uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
    uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);   
    }

//End Of File
