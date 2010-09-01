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











#include <PtiEngine.h>                          //CPtiEngine
#include <PtiDefs.h>                            //keys
#include <avkon.rsg>
#include <aknfep.rsg>
#include <aknnotewrappers.h>

#include "AknFepPanic.h"
#include "aknfepinputstatespellingpinyinphrasecreation.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"		//Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepManager.h"

const TUint16 KDelimiter =0x0027;
const TInt KMaxPhraseCreationCount = 7;
const TInt KMaxSpellingLength = 10;

// ---------------------------------------------------------
// C++ construct.
// ---------------------------------------------------------
//
TAknFepInputStateSpellingPinyinPhraseCreation::TAknFepInputStateSpellingPinyinPhraseCreation(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStatePinyinPhraseCreationBase( aOwner, aUIContainer )
    {
    iState = ESpellingSelection;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane(EFalse);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutPhraseCreation);
    uiContainer->CandidatePane()->SelectFirstPhrase(); 
    uiContainer->ShowVerticalScrollArrows(ETrue);
    uiContainer->ShowHorizontalScrollArrows(EFalse);
    uiContainer->InputPane()->SetOverrideFontId(0);
    uiContainer->SetFepMan( iOwner->FepMan() );
    uiContainer->EditPaneWindow()->DeactiveCursor();
    
    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    if ( uiContainer->PinyinPopupWindow()->GetFromEditToSpellingFlag() )
        {
        TInt validCount = uiContainer-> 
            PinyinPopupWindow()->InEffectKeystrokeArray()->Count();
        TInt chineseCount = uiContainer-> 
            PinyinPopupWindow()->ChooseChineseCharacterArray()->Count();
        TInt index = 0;
        if ( !uiContainer->PinyinPopupWindow()->GetPhraseCreationEditingState() )
            {
            index = uiContainer->PinyinPopupWindow()->CurrentSelection();
            }        
        uiContainer->EditPaneWindow()->SetCursorIndexOfKeystroke( validCount );
        ShowInfo();
        uiContainer->EditPaneWindow()->
            SetUnderline( chineseCount, chineseCount + validCount - 1 );
        uiContainer->InputPane()->SetText( uiContainer->
            PinyinPopupWindow()->PhoneticSpellingArray()->MdcaPoint( index ) );
        uiContainer->PinyinPopupWindow()->SetFromEditToSpellingFlag();
        uiContainer->PinyinPopupWindow()->SplitPhraseSpellingIntoPages();
        uiContainer->PinyinPopupWindow()->SetDisplayPage( index );
        uiContainer->PinyinPopupWindow()->SetPhraseItemTexts();
        uiContainer->PinyinPopupWindow()->PopupSizeChanged(); // phrase input
        uiContainer->PinyinPopupWindow()->Enable( ETrue );
        }
    else
        {
        ImplicityShowInfo( ETrue );
        }
    
    ChangeCba();
    }

// ---------------------------------------------------------
// HandleKeyL
// ---------------------------------------------------------
//
TBool TAknFepInputStateSpellingPinyinPhraseCreation::HandleKeyL(
                                 TInt aKey, 
                                 TKeyPressLength aLength )
    {
    switch( aKey )
        {
        case EKeyBackspace:
        case EKeyUpArrow:
            HandleKeyBackspaceOrKeyUpArrow( aLength );
            break;
        case EKeyRightArrow:
            HandleKeyRightArrow();
            break;
        case EKeyLeftArrow:
            HandleKeyLeftArrow();
            break;
        case EKeyOK:
        case EKeyDownArrow:
        case EKeyCBA1:    
             HandleKeyOKOrKeyDownArrowOrKeyCBA1( aLength );
            break;
        case EKeyCBA2:
            iOwner->FepMan()->TryCloseUiL();
            break;
        default:
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            break;
        }
        return ETrue;
    }

// ---------------------------------------------------------
// Initialize state
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::InitializeStateL(void)
    {
    iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT);
    }

// ---------------------------------------------------------
// Implicity the show info
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::ImplicityShowInfo( 
                                                 TBool aGetScreenCoordinates )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* validKeystroke = popup->InEffectKeystrokeArray();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    TInt selection(0);
    // get cursor position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    if ( aGetScreenCoordinates )
        {
        TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
        if (ret == KErrNone)
            {
            uiContainer->SetContainerPosition(baseLine, height);    
            }
        }
    if ( popup->GetPhraseCreationState() )
        {
        GetValidKeystroke();
        editPane->SetCursorIndexOfKeystroke( validKeystroke->Count() );
        }
    else
        {
        editPane->SetCursorIndexOfKeystroke( validKeystroke->Count() );
        }
    
    ShowInfo();
    editPane->SetUnderline( chineseCount, chineseCount + 
        validKeystroke->Count() - 1 );
    if ( popup->GetPhraseCreationState() )
        {
        ClearDeliberateSelection();        
        }
    else
        {
        selection = popup->CurrentSelection();
        }
    RefreshUI( selection );
    if ( popup->GetPhraseCreationState() )
        {
        popup->SetPhraseCreationState( EFalse );
        }
    }

// ---------------------------------------------------------
// Refresh the show info
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::RefreshShowInfo()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    CDesCArrayFlat* validKeystroke = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    CDesCArrayFlat* candidateArray = 
        uiContainer->CandidatePane()->CandidateArray();
    TInt selection = popup->CurrentSelection();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    TPtr deliberateSelection = iOwner->GetLatestDeliberateSelection();
    
    editPane->SetCursorIndexOfKeystroke( validKeystroke->Count() );
    ShowInfo();
    editPane->SetUnderline( chineseCount, chineseCount + 
        validKeystroke->Count() - 1 );
    inputPane->SetText( spelling->MdcaPoint( selection ) );
    deliberateSelection = spelling->MdcaPoint( selection );
    
    popup->SplitPhraseSpellingIntoPages();
    popup->SetDisplayPage(selection);
    popup->SetPhraseItemTexts();
    popup->PopupSizeChanged();
    popup->Enable( validKeystroke->Count() );
    GetCandidate( selection );
    uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable( ETrue );
    uiContainer->ShowHorizontalScrollArrows( EFalse );
    uiContainer->ShowVerticalScrollArrows( ETrue );
    UpdateIndicator();
    }

// ---------------------------------------------------------
// KeyBackspace Or KeyUpArrow
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::HandleKeyBackspaceOrKeyUpArrow( 
    TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    
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
        popup->SetChangeState( ETrue );
        //change state to EKeySequenceEdit
        popup->SetFromEditToSpellingFlag( ETrue );
        iOwner->ChangeState( EKeySequenceEdit );
        }
    }

// ---------------------------------------------------------
// KeyOK Or KeyDownArrow Or KeyCBA1
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::HandleKeyOKOrKeyDownArrowOrKeyCBA1( 
    TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    TBool state = ETrue;
    if ( popup->IsChangeStatet() && aLength == ELongKeyPress )
        {
        state = EFalse;
        }
    else
        {
        popup->SetChangeState( EFalse );
        }

    TBool changeState = ( popup->ChooseChineseCharacterArray()->Count() != 
        KMaxPhraseCreationCount ) && ( popup->KeystrokeArray()->Count() != 0 );
    
    if ( changeState && state )
        {
        if(popup->IsEnabled())
            {
            popup->Enable(EFalse);
            }
        popup->SetChangeState( ETrue );
        iOwner->ChangeState(ECandidateSelection);     
        }
    }

// ---------------------------------------------------------
// KeyRightArrow
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::HandleKeyRightArrow()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    TBuf<KMaxSpellingLength> spelling;
    popup->SetPhraseCreationState( EFalse );
    if( popup->SelectNextPhrase() )
        {
        if ( RefreshValidKeystroke() )
            {
            RefreshShowInfo();
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
    }

// ---------------------------------------------------------
// KeyRightArrow
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::HandleKeyLeftArrow()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    if( popup->SelectPrevPhrase() )
        {
        if ( RefreshValidKeystroke() )
            {
            RefreshShowInfo();
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
    }

// ---------------------------------------------------------
// Refresh Valid Keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStateSpellingPinyinPhraseCreation::RefreshValidKeystroke()
    {
    TRAPD( ret, RefreshValidKeystrokeL() );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------
// Refresh Valid Keystroke.
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingPinyinPhraseCreation::RefreshValidKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* validKeystroke = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* keystroke = popup->ShowKeystrokeArray();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();    
    TInt selection = popup->CurrentSelection();
    validKeystroke->Reset();
    TInt spellLength = spelling->MdcaPoint( selection ).Length();
    
    for ( TInt i = 0; ( i < spellLength ) && ( spellLength <= 
        keystroke->Count() ); ++i )
        {
        validKeystroke->AppendL( keystroke->MdcaPoint( i ) );
        }
    
    if ( spellLength < keystroke->Count() )
        {
        if ( KDelimiter == keystroke->MdcaPoint( spellLength )[0] )
            {
            validKeystroke->AppendL( keystroke->MdcaPoint( spellLength ) );
            }
        }
    
    validKeystroke->Compress();
    }

// End of file
