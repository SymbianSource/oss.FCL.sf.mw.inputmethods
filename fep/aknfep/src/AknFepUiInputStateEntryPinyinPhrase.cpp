/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIInputStatePinyinPhrase methods.
*
*/












#include "AknFepPanic.h"
#include "AknFepUiInputStateEntryPinyinPhrase.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"		//Pinyin phrase
#include "AknFepManager.h"

#include <PtiEngine.h>                          //CPtiEngine
#include <PtiDefs.h>                            //keys
#include <avkon.rsg>
#include <AknFep.rsg>
#include <aknnotewrappers.h>

const TText KPinyinTone0Valid = 0x02C9;
const TText KPinyinTone1Valid = 0x02CA;
const TText KPinyinTone2Valid = 0x02C7;
const TText KPinyinTone3Valid = 0x02CB;
const TText KPinyinTone4Valid = 0x02D9;
const TText KPinyinTone4Invalid = 0x0020;

const TInt KMaxPhraseCount = 100;
const TInt KMaxKeystrokeCount = 100;
const TInt KMaxShowKeystrokeCount = 31;

const TInt KMaxPinyinLength = 60;

const TInt KMaxStarCount = 5;

const TInt KKey0Code = 48;
const TInt KKey1Code = 49;
const TInt KKey2Code = 50;
const TInt KKey3Code = 51;
const TInt KKey4Code = 52;
const TInt KKey5Code = 53;
const TInt KKey6Code = 54;
const TInt KKey7Code = 55;
const TInt KKey8Code = 56;
const TInt KKey9Code = 57;

_LIT( KPinyinListSeparator, "'" );
_LIT( KPinyinStar, "*" );
_LIT( KPinyinNote,"\x9020\x65B0\x8BCD" );
_LIT( KPinyinManualSeparator, "\x002E");

TAknFepInputStateEntryPinyinPhrase::TAknFepInputStateEntryPinyinPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    iState = EEntry;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane(EFalse);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    uiContainer->CandidatePane()->SelectFirstPhrase(); 
    uiContainer->ShowVerticalScrollArrows(ETrue);
    uiContainer->ShowHorizontalScrollArrows(EFalse);
    uiContainer->InputPane()->SetOverrideFontId(0);

    iOwner->PtiEngine()->SetInputMode(EPtiEnginePinyinByPhrase);
    iOwner->PtiEngine()->SetCandidatePageLength(KMaxPhraseCount);
    uiContainer->PinyinPopupWindow()->SetTonemarkState( ETrue );
    // set the state is in input state,
    // so that it could handle EKeyCBA1
    iOwner->FepMan()->EntryPhrase( ETrue );
    uiContainer->SetFepMan( iOwner->FepMan() );
    
    ClearKeystrokeBuf();
    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    ImplicitlyUpdateSelection();
    // however we also need to clear the deliberate selection, in case we are not
    // coming back to the input pane from the candidate pane
    if ( uiContainer->PinyinPopupWindow()->GetFromCandidateChangeStateFlag() )
        {
        uiContainer->PinyinPopupWindow()->SetFromCandidateChangeStateFlag( EFalse );
        }
    else
        {
        ClearDeliberateSelection();
        }
    
    UpdateIndicator();
    TRAP_IGNORE( ChangeCbaL() );
    }

void TAknFepInputStateEntryPinyinPhrase::HandleCommandL( TInt aCommandId)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine( );
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer( );
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow( );
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray( );
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray( );
    CDesCArrayFlat* showKeystrokeArray = popup->ShowKeystrokeArray( );
    TInt currentSelect = popup->CurrentSelection( );
    TInt inEffectSpellingCount = ptiengine->PhoneticSpellingCount( );
    TBuf<KMaxKeystrokeCount> keystrokeBuf;
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeyCancel:
            //  case (TUint16)EAknSoftkeyCancel: //the Cancle in soft CBA
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            if ( (currentSelect < inEffectSpellingCount)
                && (keystrokeArray->Count( )
                    == inEffectKeystrokeArray->Count( ) )
                && (popup->GetTonemarkState( ) ) )
                {
                if ( popup->IsEnabled( ) )
                    {
                    popup->Enable( EFalse );
                    }
                iOwner->FepMan()->PinyinPhraseCreation( EFalse );
                popup->SetChangeState( ETrue );
                iOwner->ChangeState( ECandidate );
                }
            else
                {
                GetKeystrokeBuf( *keystrokeArray, keystrokeBuf );
                iOwner->FepMan()->PinyinPhraseCreation( ETrue );
                popup->SetChangeState( ETrue );
                popup->SetPhraseCreationState( ETrue );
                if ( GetChangeState( ) )
                    {
                    ClearDeliberateSelection( );
                    popup->Enable( EFalse );
                    iOwner->ChangeState( ESpellingSelection );
                    }
                else
                    {
                    ClearDeliberateSelection( );
                    popup->Enable( EFalse );
                    iOwner->ChangeState( EKeySequenceEdit );
                    }
                }
            break;
        default:
            DeliberatelyUpdateSelection( );
            break;
        }

    }

TBool TAknFepInputStateEntryPinyinPhrase::HandleKeyL( TInt aKey, TKeyPressLength aLength )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* showKeystrokeArray = popup->ShowKeystrokeArray();
    TInt currentSelect = popup->CurrentSelection();
    TInt inEffectSpellingCount = ptiengine->PhoneticSpellingCount();
    TInt offset = 0;
    
    TBuf<1> keyBuf;
    TBuf<1> keyShow;
    TBuf<KMaxKeystrokeCount> keystrokeBuf;
    
    if(aKey == EKeyBackspace)
        {
        TBool state = ETrue;
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
            DeleteCurrentKeystroke();
            showKeystrokeArray->Delete( showKeystrokeArray->Count() - 1 );
            showKeystrokeArray->Compress();
            if ( inEffectKeystrokeArray->Count()> 0 )
                {
                ImplicitlyUpdateSelection();
                }
            else
                {
                ClearDeliberateSelection();
                iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                if (aLength == ELongKeyPress)
                    {
                    iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                    }
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
    else if( ( aKey == EKeyOK || aKey == EKeyDownArrow || aKey == EKeyCBA1)
        && aLength == EShortKeyPress)
        {
        if ( ( currentSelect < inEffectSpellingCount) && 
            ( keystrokeArray->Count() == inEffectKeystrokeArray->Count() ) &&
            ( popup->GetTonemarkState() ) )
            {
            if(popup->IsEnabled())
                {
                popup->Enable(EFalse);
                }
            iOwner->FepMan()->PinyinPhraseCreation( EFalse );
            popup->SetChangeState( ETrue );
            iOwner->ChangeState(ECandidate);              
            }
        else
            {
            GetKeystrokeBuf( *keystrokeArray, keystrokeBuf );
            iOwner->FepMan()->PinyinPhraseCreation( ETrue );
            popup->SetChangeState( ETrue );
            popup->SetPhraseCreationState( ETrue );
            if ( GetChangeState() )
                {
                ClearDeliberateSelection();
                popup->Enable( EFalse );
                iOwner->ChangeState(ESpellingSelection);
                }
            else
                {
                ClearDeliberateSelection();
                popup->Enable( EFalse );
                iOwner->ChangeState(EKeySequenceEdit);
                }
            }
        }
    else if (aLength == EShortKeyPress) // don't want repeats on these keys
        {
    	iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);            
        
        if ( ( iOwner->IsValidChineseInputKey(aKey) ) &&
             ( CheckKeystroke( aKey, *keystrokeArray ) ) )
            {
            TInt stringBeforeLength(0);
            TInt stringAfterLength(0);
            GetKeystrokeNum( aKey, keyBuf );
            keystrokeArray->AppendL( keyBuf );
            showKeystrokeArray->AppendL( keyBuf );
            
            if ( popup->GetTonemarkState() )
                {
            stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
            stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
                if (stringBeforeLength != stringAfterLength)
                    {
                    if (ptiengine->GetPhoneticSpelling(1).Length() == 1)
                        {
                        iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT);
                        }
                    inEffectKeystrokeArray->AppendL(keyBuf);
                    ImplicitlyUpdateSelection();
                    }
                else
                    {
                    ptiengine->EnableToneMarks(ETrue);
                    UIContainer()->PinyinPopupWindow()->SetFlag(MAknFepUICtrlPinyinPopup::ESpellingChanged);
                    RefreshUI(0);
                    } 
                }
            else
                {
                ImplicitlyUpdateSelection();
                }
            }
        else if ( ( aKey == EPtiKeyStar ) &&  
                  ( CheckKeystroke( aKey, *keystrokeArray ) ) ) // we increment the tone mark.
            {
            keyBuf.Append( EPtiKeyStar );
            
            if ( keystrokeArray->Count() == inEffectKeystrokeArray->Count() )
                {
                inEffectKeystrokeArray->AppendL(keyBuf);
                keystrokeArray->AppendL( keyBuf );
                GetPinyinToneMark( *keystrokeArray, keyShow );
                ShowKeystrokeAppendL( *showKeystrokeArray, *keystrokeArray, keyShow );
                CheckTonemarkL();
                }
            else
                {
                keystrokeArray->AppendL( keyBuf );
                GetPinyinToneMark( *keystrokeArray, keyShow );
                ShowKeystrokeAppendL( *showKeystrokeArray, *keystrokeArray, keyShow );
                for ( TInt i = keystrokeArray->Count() - 1; i >= 0; i-- )
                    {
                    if ( 0 == keystrokeArray->MdcaPoint( i ).
                            Compare( KPinyinStar ) )
                        {
                        offset = offset + 1;
                        }
                    else
                        {
                        break;
                        }
                    }
                if ( offset > KMaxStarCount )
                    {
                    for ( TInt j = 0; j < KMaxStarCount; ++j )
                        {
                        keystrokeArray->Delete( keystrokeArray->Count() - 1 );
                        }
                    keystrokeArray->Compress();
                    }
                }
            ImplicitlyUpdateSelection();
            }
        else
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------
// Optimize spelling
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::OptimizeSpellingL( CDesCArrayFlat& aOptimizeBefore, CDesCArrayFlat& aOptimizeAfter )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    TBool checkSeparator = EFalse;
    TBuf<KMaxPinyinLength> spellingBuf;
    TInt bufLength = 0;
    TInt countSeparator = 0;

    TInt countBuf[KMaxPhraseCount];
    memset( countBuf, 0, sizeof( countBuf ) );
    
    TInt position = 0;
    TInt i = 0;
    TInt j = 0;
    
    if ( 0 == inEffectKeystrokeArray->Count() )
        {
        return;
        }
    
    for ( i = 0 ; i < inEffectKeystrokeArray->Count(); ++i )
        {
        if ( 0 == inEffectKeystrokeArray->MdcaPoint( i ).Compare( KPinyinListSeparator ) )
            {
            checkSeparator = ETrue;
            countBuf[countSeparator] = j;
            countSeparator = countSeparator + 1;
            }
        else
            {
            if (  0 == i  )
                {
                j = j + 1;
                }
            else
                {
                if ( ( 0 == inEffectKeystrokeArray->MdcaPoint( i ).Compare( KPinyinStar ) ) &&
                    ( 0 != inEffectKeystrokeArray->MdcaPoint( i - 1 ).Compare( KPinyinStar ) ))
                    {
                    j = j + 1;
                    }
                else if ( 0 != inEffectKeystrokeArray->MdcaPoint( i ).Compare( KPinyinStar ) )
                    {
                    j = j + 1;
                    }
                }
            }
        }
  
    j = 0;
    for ( i = 0; i < aOptimizeBefore.Count(); ++i )
        {
        j = 0;
        bufLength = 0;
        position = 0;
        spellingBuf.Size();
        spellingBuf.Copy( aOptimizeBefore.MdcaPoint( i ) );
        bufLength = spellingBuf.Length();
        while( j < bufLength )
            {
            position = spellingBuf.Find( KPinyinListSeparator );
            if ( position != KErrNotFound )
                {
                spellingBuf.Replace( position, 1, KPinyinManualSeparator );
                j = position + 1;
                }
            else
                {
                break;
                }
            }
        aOptimizeAfter.AppendL( spellingBuf );
        }    
    
    if ( !checkSeparator )
        {
        return;
        }
    
    aOptimizeBefore.Reset();
    aOptimizeBefore.Compress();
    
    for ( i = 0; i < aOptimizeAfter.Count(); ++i )
        {
        aOptimizeBefore.AppendL( aOptimizeAfter.MdcaPoint( i ) );
        }
    
    aOptimizeAfter.Reset();
    aOptimizeAfter.Compress();
    

    for ( TInt k = 0; k < aOptimizeBefore.Count(); ++k )
        {
        j = 0;
        bufLength = 0;
        position = 0;
        spellingBuf.Size();
        spellingBuf.Copy( aOptimizeBefore.MdcaPoint( k ) );
        bufLength = spellingBuf.Length();
        for ( TInt kk = 0; kk < bufLength; ++kk )
            {
            if ( 0 == spellingBuf.Mid( kk, 1 ).Compare( KPinyinManualSeparator ) )
                {
                for ( TInt kkk = 0; kkk < countSeparator; ++kkk)
                    {
                    if ( j == countBuf[kkk] )
                        {
                        spellingBuf.Replace( kk, 1, KPinyinListSeparator );
                        }
                    }
                }
            else
                {
                j = j + 1;
                }
            }
        aOptimizeAfter.AppendL( spellingBuf );
        }
    }
void TAknFepInputStateEntryPinyinPhrase::DeliberatelyUpdateSelection()
    {
    TPtr deliberateSelection = iOwner->GetLatestDeliberateSelection();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    TInt selected = popup->CurrentSelection();
    ptiengine->EnableToneMarks(EFalse);
    TPtrC spelling = ptiengine->GetPhoneticSpelling(selected + 1); // our index is zero based, engine index is one based
    deliberateSelection = spelling.Left(
                        MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark);
    ptiengine->EnableToneMarks(ETrue);
    UIContainer()->PinyinPopupWindow()->SetFlag(MAknFepUICtrlPinyinPopup::ESpellingNavigation);    
    RefreshUI(popup->CurrentVisibleSelection());
    }

void TAknFepInputStateEntryPinyinPhrase::ImplicitlyUpdateSelection()
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

void TAknFepInputStateEntryPinyinPhrase::ClearDeliberateSelection()
    {
    // we must have just deleted the last character, 
    // or we are starting a new pinyin session, so wipe the last deliberate selection
    TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
    oldDeliberateSelection = KNullDesC;
    }

void TAknFepInputStateEntryPinyinPhrase::RefreshUI(TInt aSelection)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* showKeystrokeArray = popup->ShowKeystrokeArray();
    CDesCArrayFlat* optimizeSpelling = popup->PhoneticSpellingArray();
    CDesCArrayFlat* spelling = popup->OptimizeSpelling();
        
    TBuf<KMaxKeystrokeCount> keystrokeBuf;
    
    // get cursor position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;

    TBool checkTonemark = popup->GetTonemarkState();
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

    // to start with, disable the tone marks so we can get the list of all pinyin matches
    // later we will figure out which ones are invalid
    ptiengine->EnableToneMarks(EFalse);
    TInt pinyinCountWithoutToneMarks = ptiengine->PhoneticSpellingCount();
    if(pinyinCountWithoutToneMarks > 0)
        {
        if(popup->IsFlagSet(MAknFepUICtrlPinyinPopup::ESpellingChanged))
            {
            if ( ( inEffectKeystrokeArray->Count() < keystrokeArray->Count() ) ||
                ( !checkTonemark ) )
                {
                spelling->Reset();
                optimizeSpelling->Reset();
                spelling->Compress();
                optimizeSpelling->Compress();
                }
            else
                {
                spelling->Reset();
                optimizeSpelling->Reset();
                spelling->Compress();
                optimizeSpelling->Compress();
            TRAPD(ignore, ptiengine->GetPhoneticSpellingsL(*spelling));
            if (ignore != KErrNone)
                    {
                    return;
                    }
                
                TRAPD( iOptimize, OptimizeSpellingL( *spelling, *optimizeSpelling ) );
                if ( KErrNone != iOptimize )
                    {
                    return;
                    }
                }
            
            GetKeystrokeBuf( *showKeystrokeArray, keystrokeBuf );
            
            TRAPD( retOptimize, optimizeSpelling->AppendL( keystrokeBuf ) );
            if ( KErrNone != retOptimize )
                {
                return;
                }
            popup->SplitPhraseSpellingIntoPages(); 
            }

        // set display page for delibrate selection
        popup->SetDisplayPage(aSelection);
        popup->SetPhraseItemTexts();
 
        // fill in input pane although it's invisible
        if ( inEffectKeystrokeArray->Count() < keystrokeArray->Count() )
            {
            inputPane->SetText( keystrokeBuf );    
            }
        else
            {
            if ( optimizeSpelling->Count() > popup->CurrentSelection() )
                {
                inputPane->SetText( optimizeSpelling->MdcaPoint( popup->CurrentSelection() ) );
                }
            else
                {
                inputPane->SetText( KNullDesC );
                }
            }
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
                nextCandidateWithoutToneMark.Copy(ptiengine->GetPhoneticSpelling(i + 1).Left(
                      MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark));
                
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
        CDesCArrayFlat* phraseCandidates = uiContainer->CandidatePane()->CandidateArray();
        if ( ( pinyinCountWithoutToneMarks > popup->CurrentSelection() ) && 
             ( inEffectKeystrokeArray->Count() == keystrokeArray->Count() ) && 
             checkTonemark )      
            {
            TRAPD(ignore, ptiengine->GetChinesePhraseCandidatesL(*phraseCandidates));
            if (ignore != KErrNone)
                {
                return;
                }            
            }        
        else if ( keystrokeArray->Count() != 0 )
            {
            phraseCandidates->Reset();
            TRAPD( retAddPinyinNote, phraseCandidates->AppendL( KPinyinNote ) );
            if ( KErrNone != retAddPinyinNote )
                {
                return;
                }
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

void TAknFepInputStateEntryPinyinPhrase::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT);
    }
    
void TAknFepInputStateEntryPinyinPhrase::UpdateIndicator()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
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
        
    if (candidatePane->SelectedIndex() == 0 && 
        candidatePane->IsFirstPage() )
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
        }
    else
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
        }
        
    if (candidatePane->IsLastPage() && 
        (candidatePane->SelectedIndex() == candidatePane->VisibleCandidateCount() - 1))
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);        
        }
    else
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
        }
    } 
// ---------------------------------------------------------
// Delete current keystroke.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::DeleteCurrentKeystroke()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TInt i = keystrokeArray->Count() - 1;
    TInt count = keystrokeArray->Count() - 1;
    TInt inECTKeystrokeCount = inEffectKeystrokeArray->Count();
    TInt validCount = 0;
    
    if ( keystrokeArray->Count() > inEffectKeystrokeArray->Count() )
        {
        for ( i = keystrokeArray->Count() - 1; i >= 0; i-- )
            {
            if ( 0 == keystrokeArray->MdcaPoint( i ).Compare( KPinyinStar ) )
                {
                keystrokeArray->Delete( i );
                }
            else
                {
                if ( i == count )
                    {
                    keystrokeArray->Delete( i );
                    break;
                    }
                break;
                }
           }
        }
    else
        {
        for ( TInt j = 0; j < inECTKeystrokeCount; ++j )
            {            
            if ( 0 != inEffectKeystrokeArray->
                MdcaPoint( j ).Compare( KPinyinStar ) )
                 {
                 validCount = validCount + 1;
                 }
            if ( j > 1 )
                {
                if ( ( 0 != inEffectKeystrokeArray->MdcaPoint( j ).
                    Compare( KPinyinStar ) ) && 
                    ( 0 == inEffectKeystrokeArray->MdcaPoint( j - 1).
                        Compare( KPinyinStar ) ) )
                     {
                     validCount = validCount + 1;
                     }
                if  ( ( j == inECTKeystrokeCount - 1 ) && 
                    ( 0 == inEffectKeystrokeArray->MdcaPoint( j ).
                        Compare( KPinyinStar ) ) )
                    {
                    validCount = validCount + 1;
                    }
                }
            }
        
        if ( inEffectKeystrokeArray->Count() > 0 )
            {
            if ( 0 == inEffectKeystrokeArray->MdcaPoint( 
                inEffectKeystrokeArray->Count() - 1 ).Compare( KPinyinStar ) )
                {
                popup->SetTonemarkState( ETrue );
                }
            }
        
        if ( 0 == keystrokeArray->MdcaPoint( keystrokeArray->Count() - 1 ).
            Compare( KPinyinStar ) )
            {
            ptiengine->DeleteKeyPress();
            for ( i = keystrokeArray->Count() - 1; i >= 0; i-- )
                {
                if ( 0 == keystrokeArray->MdcaPoint( i ).Compare( KPinyinStar ) )
                    {
                    keystrokeArray->Delete( i );
                    inEffectKeystrokeArray->Delete(i);
                    }
                else
                    {
                    break;
                    }
                }
            }
        else
            {
            keystrokeArray->Delete( i );
            inEffectKeystrokeArray->Delete(i);
            ptiengine->DeleteKeyPress();
            }
        }
    keystrokeArray->Compress();
    inEffectKeystrokeArray->Compress();
    }

// ---------------------------------------------------------
// Add current keystroke.
// ---------------------------------------------------------
void TAknFepInputStateEntryPinyinPhrase::AddCurrentKeystrokeL( const TDesC& keystroke )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    const TInt offsetSize = 4;
    TInt count = keystrokeArray->Count();
    TInt offset = 0;
    TInt i = count - 1;
    
    if ( 0 != keystroke.Compare( KPinyinStar ) )
        {
        if ( keystrokeArray->Count() > inEffectKeystrokeArray->Count() )
            {
            keystrokeArray->AppendL( keystroke );
            keystrokeArray->Compress();
            return;
            }
        else
            {
            keystrokeArray->AppendL( keystroke );
            keystrokeArray->Compress();
            inEffectKeystrokeArray->AppendL( keystroke );
            inEffectKeystrokeArray->Compress();
            return;
            }
        }
    
    for ( i = count - 1; i >= 0; i-- )
        {
        if ( 0 == keystrokeArray->MdcaPoint( i ).Compare( KPinyinStar ) )
            {
            offset = offset + 1;
            }
        else
            {
            break;
            }
        }     
    if ( ( 0 <= offset ) && ( offset <= offsetSize ) )
        {
        if ( keystrokeArray->Count() > inEffectKeystrokeArray->Count() )
            {
            keystrokeArray->AppendL( keystroke );
            keystrokeArray->Compress();
            }
        else
            {
            keystrokeArray->AppendL( keystroke );
            keystrokeArray->Compress();
            inEffectKeystrokeArray->AppendL( keystroke );
            inEffectKeystrokeArray->Compress();
            }
        }
    else
        {
        if ( keystrokeArray->Count() > inEffectKeystrokeArray->Count() )
            {
            for ( TInt ii = 0; ii < offsetSize; ++ii )
            {
                keystrokeArray->Delete( count - 1 -ii );
            }
            keystrokeArray->Compress();
            }
        else
            {
            for ( TInt ii = 0; ii < offsetSize; ++ii )
            {
                keystrokeArray->Delete( count - 1 -ii );
                inEffectKeystrokeArray->Delete( count - 1 -ii );
            }
            keystrokeArray->Compress();
            inEffectKeystrokeArray->Compress();
            }
        }    
    }


// ---------------------------------------------------------
// Get the keystroke.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::GetKeystrokeNum( TInt aKey, TDes& aKeystroke ) 
    {
    switch(aKey)
        {
        case KKey0Code:
            aKeystroke.Append( KPinyinListSeparator );
            break;
        case KKey1Code:
            aKeystroke.Append( KPinyinListSeparator );
            break;
        case KKey2Code:
            aKeystroke.Append(EPtiKey2);
            break;
        case KKey3Code:
            aKeystroke.Append(EPtiKey3);
            break;
        case KKey4Code:
            aKeystroke.Append(EPtiKey4);
            break;
        case KKey5Code:
            aKeystroke.Append(EPtiKey5);
            break;
        case KKey6Code:
            aKeystroke.Append(EPtiKey6);
            break;
        case KKey7Code:
            aKeystroke.Append(EPtiKey7);
            break;
        case KKey8Code:
            aKeystroke.Append(EPtiKey8);
            break;
        case KKey9Code:
            aKeystroke.Append(EPtiKey9);
            break;
        default:
            aKeystroke.Append(EPtiKeyStar);
            break;
        }
    }

// ---------------------------------------------------------
// Get the keystroke buf.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::GetKeystrokeBuf(const CDesCArrayFlat& aKeystrokeArray, TDes& aKeystrokeBuf )
    {
    for ( TInt i = 0; i < aKeystrokeArray.Count(); ++i )
        {
        aKeystrokeBuf.Append(aKeystrokeArray.MdcaPoint(i));
        }
    }

// ---------------------------------------------------------
// clear keystroke.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::ClearKeystrokeBuf()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* showKeystrokeArray = popup->ShowKeystrokeArray();
    CDesCArrayFlat* chooseChineseArray = popup->ChooseChineseCharacterArray();
    CDesCArrayFlat* chooseChienseKeystroke = popup->ChooseChineseCharacterArrayKeystroke();
    
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TInt pinyinCount = ptiengine->PhoneticSpellingCount();
    chooseChineseArray->Reset();
    chooseChienseKeystroke->Reset();
    
    if ( 0 == pinyinCount )
        {
        keystrokeArray->Reset();
        inEffectKeystrokeArray->Reset();
        showKeystrokeArray->Reset();
        }
    }

// ---------------------------------------------------------
// Get tome mark.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::GetPinyinToneMark( const CDesCArrayFlat& aKeystrokeArray, TDes& aPinyinToneMark )
    {
    TInt starCount = 0;
    TInt pinyinToneMark;
    TBuf<1> buf;
    TInt count = aKeystrokeArray.Count();
    const TInt toneMark0 = 0;
    const TInt toneMark1 = 1;
    const TInt toneMark2 = 2;
    const TInt toneMark3 = 3;
    const TInt toneMark4 = 4;
    
    if ( 0 == count )
        {
        aPinyinToneMark.Append( KPinyinTone0Valid );
        return;
        }
    
    for ( TInt i = count - 1; i >= 0; --i )
        {
        buf = aKeystrokeArray.MdcaPoint( i );
        if ( 0 == buf.Compare( KPinyinStar ) )
            {
            starCount = starCount + 1; 
            }
        else
            {
            break;
            }
        }
    
    if ( 0 == starCount )
        {
        aPinyinToneMark.Append( KPinyinTone0Valid );
        }
    else
        {
        pinyinToneMark = starCount % KMaxStarCount;
        switch ( pinyinToneMark )
            {
            case toneMark0:
                aPinyinToneMark.Append( KPinyinTone4Valid );
                break;
            case toneMark1:
                aPinyinToneMark.Append( KPinyinTone0Valid );
                break;
            case toneMark2:
                aPinyinToneMark.Append( KPinyinTone1Valid );
                break;
            case toneMark3:
                aPinyinToneMark.Append( KPinyinTone2Valid );
                break;
            case toneMark4:
                aPinyinToneMark.Append( KPinyinTone3Valid );
                break;
            default:
               break;
            }
        }
    }

// ---------------------------------------------------------
// Check keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryPinyinPhrase::CheckKeystroke( TInt aKey, const CDesCArrayFlat& aKeystrokeArray )
    {
    CDesCArrayFlat* showKeystrokeArray = UIContainer()->
        PinyinPopupWindow()->ShowKeystrokeArray();
    
    TInt count = showKeystrokeArray->Count();
    TBuf<1> keyBuf;
    
    if ( count >= KMaxShowKeystrokeCount )
        {
        if ( ( 0 != aKeystrokeArray.MdcaPoint( aKeystrokeArray.Count() - 1 ).
            Compare( KPinyinStar ) ) || ( aKey != EPtiKeyStar )  )
            {
            return EFalse;
            }
        }
    
    GetKeystrokeNum( aKey, keyBuf );

    if ( 0 == aKeystrokeArray.Count() )
        {        
        if ( ( 0 == keyBuf.Compare ( KPinyinStar ) ) || ( 0 == keyBuf.Compare ( KPinyinListSeparator ) ) )
            {
            return EFalse;
            }        
        }
    else
        {
        if ( 0 == aKeystrokeArray.MdcaPoint( aKeystrokeArray.Count() - 1 ).Compare( KPinyinStar ) && 
            ( 0 == keyBuf.Compare ( KPinyinListSeparator ) ) )
            {
            return EFalse;
            }
        
        if ( 0 == aKeystrokeArray.MdcaPoint( aKeystrokeArray.Count() - 1 ).Compare( KPinyinListSeparator ) && 
            ( 0 == keyBuf.Compare ( KPinyinStar ) ) )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------
// Check tome mark.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::CheckTonemarkL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* showKeystrokeArray = popup->ShowKeystrokeArray();
    CDesCArrayFlat* spelling = popup->OptimizeSpelling();

    TInt tonemarkCount = 0;
    TInt spellTonemarkCount = 0;
    TInt keystrokeStarCount = 0;

    TInt offset = 0;
    TInt i = 0;
    TBool checkReturn = EFalse;
    
    TBuf<1> buf;
    TBuf<1> lastTonemark;
    TBuf<1> lastSpellTonemark;
    TBuf<1> tonmarkBuf0;
    TBuf<1> tonmarkBuf1;
    TBuf<1> tonmarkBuf2;
    TBuf<1> tonmarkBuf3;
    TBuf<1> tonmarkBuf4;
    TBuf<KMaxPinyinLength> spellBuf;
    
    tonmarkBuf0.Append( KPinyinTone0Valid );
    tonmarkBuf1.Append( KPinyinTone1Valid );
    tonmarkBuf2.Append( KPinyinTone2Valid );
    tonmarkBuf3.Append( KPinyinTone3Valid );
    tonmarkBuf4.Append( KPinyinTone4Valid );
    
    if ( 0 == inEffectKeystrokeArray->Count() )
        {
        popup->SetTonemarkState( EFalse );
        return;
        }
    
    if ( keystrokeArray->Count() > inEffectKeystrokeArray->Count() )
        {
        popup->SetTonemarkState( EFalse );
        return;
        }
    
    for ( i = 0; i < showKeystrokeArray->Count(); ++i )
        {
        buf.Copy( showKeystrokeArray->MdcaPoint( i ) );
        if ( 0 == buf.Compare( tonmarkBuf0 ) )
            {
            tonemarkCount = tonemarkCount + 1;
            lastTonemark.Copy( showKeystrokeArray->MdcaPoint( i ) );
            }
        else if ( 0 == buf.Compare( tonmarkBuf1 ) )
            {
            tonemarkCount = tonemarkCount + 1;
            lastTonemark.Copy( showKeystrokeArray->MdcaPoint( i ) );
            }
        else if ( 0 == buf.Compare( tonmarkBuf2 ) )
            {
            tonemarkCount = tonemarkCount + 1;
            lastTonemark.Copy( showKeystrokeArray->MdcaPoint( i ) );
            }
        else if ( 0 == buf.Compare( tonmarkBuf3 ) )
            {
            tonemarkCount = tonemarkCount + 1;
            lastTonemark.Copy( showKeystrokeArray->MdcaPoint( i ) );
            }
        else if ( 0 == buf.Compare( tonmarkBuf4 ) )
            {
            tonemarkCount = tonemarkCount + 1;
            lastTonemark.Copy( showKeystrokeArray->MdcaPoint( i ) );
            }
        }
    
    if ( 0 == tonemarkCount )
        {
        popup->SetTonemarkState( ETrue );
        return;
        }

    for( i = 0; i < inEffectKeystrokeArray->Count(); ++i )
        {
        if ( 0 == inEffectKeystrokeArray->MdcaPoint( 
            inEffectKeystrokeArray->Count() - 1 - i ).Compare( KPinyinStar ) )
            {
            keystrokeStarCount = keystrokeStarCount + 1;
            }
        else
            {
            break;
            }
        }
    if ( keystrokeStarCount > 1 )
        {
        ptiengine->DeleteKeyPress();
        }
    
    if ( keystrokeStarCount > KMaxStarCount )
        {
        offset = 1;
        for ( i = 0; i < KMaxStarCount; ++i )
            {
            keystrokeArray->Delete( keystrokeArray->Count() - 1 );
            inEffectKeystrokeArray->Delete( inEffectKeystrokeArray->Count() - 1 );
            }
        }
    else
        {
        offset = keystrokeStarCount;
        }
    
    if ( 0 == offset )
        {
        popup->SetTonemarkState( ETrue );
        return;
        }
    else
        {
        for( i = 0; i < offset; ++i )
            {
            ptiengine->IncrementToneMark( ETrue );
            spelling->Reset();
            spelling->Compress();
            ptiengine->GetPhoneticSpellingsL(*spelling);
            for ( TInt j = 0; j < spelling->Count(); ++j )
                {
                spellTonemarkCount = 0;
                spellBuf.Copy( spelling->MdcaPoint( j ) );
                for ( TInt jj = 0; jj < spellBuf.Length(); ++jj )
                    {
                    if ( 0 == spellBuf.Mid( jj, 1 ).Compare( tonmarkBuf0 ) )
                        {
                        spellTonemarkCount = spellTonemarkCount + 1;
                        lastSpellTonemark.Copy( tonmarkBuf0 );
                        }
                    else if ( 0 == spellBuf.Mid( jj, 1 ).Compare( tonmarkBuf1 ) )
                        {
                        spellTonemarkCount = spellTonemarkCount + 1;
                        lastSpellTonemark.Copy( tonmarkBuf1 );
                        }
                    else if ( 0 == spellBuf.Mid( jj, 1 ).Compare( tonmarkBuf2 ) )
                        {
                        spellTonemarkCount = spellTonemarkCount + 1;
                        lastSpellTonemark.Copy( tonmarkBuf2 );
                        }
                    else if ( 0 == spellBuf.Mid( jj, 1 ).Compare( tonmarkBuf3 ) )
                        {
                        spellTonemarkCount = spellTonemarkCount + 1;
                        lastSpellTonemark.Copy( tonmarkBuf3 );
                        }
                    else if ( 0 == spellBuf.Mid( jj, 1 ).Compare( tonmarkBuf4 ) )
                        {
                        spellTonemarkCount = spellTonemarkCount + 1;
                        lastSpellTonemark.Copy( tonmarkBuf4 );
                        }
                    }
                
                if ( ( tonemarkCount != spellTonemarkCount ) ||
                    ( 0 != lastSpellTonemark.Compare( lastTonemark ) ))
                    {
                    checkReturn = EFalse;
                    break;
                    }
                else
                    {
                    checkReturn = ETrue;
                    }
                }
            if ( checkReturn )
                {
                break;
                }
            }
        }
    
    spelling->Reset();
    spelling->Compress();
    
    if ( checkReturn )
        {
        popup->SetTonemarkState( ETrue );
        return;
        }
    else
        {
        popup->SetTonemarkState( EFalse );
        return;
        }
    
    }

// ---------------------------------------------------------
// Get change state.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryPinyinPhrase::GetChangeState()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* KeystrokeArray = popup->KeystrokeArray();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    TInt firstIndex = -1;
    TInt count = inEffectKeystrokeArray->Count() - 1;
    

    
    if ( 2 > inEffectKeystrokeArray->Count() )
        {
        return ETrue;
        }
    
    for ( TInt i = count; i >= 0; i-- )
        {
        if ( 0 != inEffectKeystrokeArray->MdcaPoint( i ).
            Compare( KPinyinStar ) )
            {
            firstIndex = i;
            break;
            }
        }
    
    if ( ( 0 == firstIndex ) && ( !popup->GetTonemarkState() ) )
        {
        return EFalse;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
// Append show keystroke.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::ShowKeystrokeAppendL( 
                                          CDesCArrayFlat& aShowKeystrokeArray, 
                                          const CDesCArrayFlat& aKeystrokeArray, 
                                          const TDesC& aPinyinToneMark )
    {
    TInt showCount = 0;
    TInt count = 0;
    TBuf<1> buf;
    count = aKeystrokeArray.Count();
    showCount = aShowKeystrokeArray.Count();
    
    if ( ( count > 2 ) && ( showCount > 1 ) )
        {
        if ( 0 == aKeystrokeArray.MdcaPoint( count - 2 ).Compare( KPinyinStar ) )
            {
            aShowKeystrokeArray.Delete( showCount - 1 );
            aShowKeystrokeArray.Compress();
            aShowKeystrokeArray.AppendL( aPinyinToneMark );
            }
        else
            {
            aShowKeystrokeArray.AppendL( aPinyinToneMark );
            }
        }
    else
        {
        aShowKeystrokeArray.AppendL( aPinyinToneMark );
        }
    }

// ---------------------------------------------------------
// change  CBA.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhrase::ChangeCbaL()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    TInt lastRes = popup->GetLastResouce();
    if ( lastRes == R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT )
        {
        return;
        }
    iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
    popup->SetLastResouce( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
    }

// End of file
