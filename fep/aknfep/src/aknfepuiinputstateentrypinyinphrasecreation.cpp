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
 *      Provides the TAknFepInputStateEntryPinyinPhraseCreation methods.
 *
*/












#include <PtiEngine.h>                          //CPtiEngine
#include <PtiUserDicEntry.h>

#include <PtiDefs.h>                            //keys
#include <avkon.rsg>
#include <aknfep.rsg>
#include <aknnotewrappers.h>

#include "AknFepPanic.h"
#include "aknfepuiinputstateentrypinyinphrasecreation.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"    //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepManager.h"

const TInt KMaxPhraseCreationCount = 7;

// ---------------------------------------------------------
// C++ construct.
// ---------------------------------------------------------
//
TAknFepInputStateEntryPinyinPhraseCreation::TAknFepInputStateEntryPinyinPhraseCreation(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputStatePinyinPhraseCreationBase( aOwner, aUIContainer)
    {
    iState = EKeySequenceEdit;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane( EFalse );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutPhraseCreation );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->ShowVerticalScrollArrows( ETrue );
    uiContainer->ShowHorizontalScrollArrows( EFalse );
    uiContainer->InputPane()->SetOverrideFontId( 0 );
    uiContainer->PinyinPopupWindow()->SetPhraseCreationEditingState( EFalse );
    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    if ( uiContainer->PinyinPopupWindow()->GetFromEditToSpellingFlag() )
        {
        uiContainer->PinyinPopupWindow()->SetFromEditToSpellingFlag();
        TInt validCount = uiContainer-> 
            PinyinPopupWindow()->InEffectKeystrokeArray()->Count();
        uiContainer->EditPaneWindow()->SetCursorIndexOfKeystroke( validCount );
        ShowInfo();        
        uiContainer->PinyinPopupWindow()->SetNoneHighlighted();
        uiContainer->EditPaneWindow()->ActiveCursor();
        uiContainer->EditPaneWindow()->EnableCursor();
        }
    else
        {
        ImplicityShowInfo( ETrue );    
        }
    // however we also need to clear the deliberate selection, in case we are not
    // coming back to the input pane from the candidate pane
    ChangeCba();
    }

// ---------------------------------------------------------
// HandleKeyL
// ---------------------------------------------------------
// 
TBool TAknFepInputStateEntryPinyinPhraseCreation::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* showKeystrokeArray = popup->ShowKeystrokeArray();
   
    switch( aKey )
        {
        case EKeyBackspace:
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
                HandleKeyBackspace( aLength );
                }
            break;
            }
        case EKeyRightArrow:
            if ( !IsMoveCursor() )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            else
                {
                MoveCursorToRight();
                }
            break;
        case EKeyLeftArrow:
            if ( !IsMoveCursor() )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            else
                {
                MoveCursorToLeft();
                }
            break;
        case EKeyOK:
        case EKeyCBA1:
            if ( !( popup->IsChangeStatet() && aLength == ELongKeyPress ) )
                {
                popup->SetChangeState( EFalse );
                HandleKeyOKAndKeyCBA1L();
                }
           break;
        case EKeyCBA2:
            iOwner->FepMan()->TryCloseUiL();
            break;
        case EKeyDownArrow:
            if ( ( 0 == showKeystrokeArray->Count() ) ||
                ( 0 == inEffectKeystrokeArray->Count() ) )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            else
                {
                popup->SetChangeState( ETrue );
                popup->SetFromEditToSpellingFlag( ETrue );
                iOwner->ChangeState( ESpellingSelection );
                }
            break;
        case EKeyUpArrow:
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            break;
        default:
            if ( EShortKeyPress == aLength )
                {
                HandleOthersKey( aKey );
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
// Init the show info
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::ImplicityShowInfo( 
                                                   TBool aGetScreenCoordinates )
    {
    TRAP_IGNORE( ImplicityShowInfoL( aGetScreenCoordinates ) );
    }

// ---------------------------------------------------------
// Init the show info
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::ImplicityShowInfoL( 
                                                   TBool aGetScreenCoordinates )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt showKeystrokeCount = popup->ShowKeystrokeArray()->Count();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    // get cursor position
    TPoint baseLine = TPoint( 0, 0 );
    TInt height = 0;
    TInt ascent = 0;
    TBool validChange = EFalse;
    TBool isSpellingValid = EFalse;
    if ( aGetScreenCoordinates )
        {
        iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent);
        uiContainer->SetContainerPosition( baseLine, height );
        }
    if ( popup->GetPhraseCreationEditingState() )
        {
        isSpellingValid = GetValidKeystroke();
        }    
    else if ( popup->GetPhraseCreationState() )
        {
        isSpellingValid = GetValidKeystroke();
        if ( 0 == chineseCount )
            {
            editPane->SetCursorIndexOfKeystroke( showKeystrokeCount );
            }
        else
            {
            editPane->SetCursorIndexOfKeystroke( 0 );
            }
        
        }
    
    if ( !isSpellingValid )
        {
        UIContainer()->SetControlInVisible( EFalse );
        }
    
    validChange = popup->GetValidKeystrokeChange();
    
    ShowInfo();
    SetWarningColor();
    editPane->ActiveCursor();
    if ( ( popup->GetPhraseCreationState() ) || 
        ( popup->GetPhraseCreationEditingState() ) )
        {
        if ( validChange || 
            ( 0 != popup->CurrentPageStartIndex() ) )
            {
            popup->SetPhraseCreationEditingState( EFalse );
            popup->SetPhraseCreationState( EFalse );
            RefreshUI();
            }
        else
            {
            if ( 0 != popup->CurrentSelection() )
                {
                RefreshCandidate();
                }
            }
        }
    else
        {
        if ( validChange )
            {
            RefreshUI( popup->CurrentSelection() );
            }
        }
    }

// ---------------------------------------------------------
// EKeyBackspace
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::HandleKeyBackspace( TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();

    TInt validKeystrokeCount = popup->InEffectKeystrokeArray()->Count();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    TInt cursorOfkeystrokeIndex = editPane->GetCursorIndexOfKeystroke();
    
    if ( 0 == cursorOfkeystrokeIndex )
        {
        if ( !TryCloseUI( aLength ) )
            {
            if ( ( 0 == chineseCount ) && ( 0 != validKeystrokeCount ) )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            else
                {
                if ( ChineseRevertKeystroke() )
                    {
                    ClearDeliberateSelection();
                    popup->SetPhraseCreationEditingState( ETrue );
                    ImplicityShowInfo();
                    ChangeCba();
                    }
                else
                    {
                    iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                    }               
                }
            }        
        }
    else
        {
        if ( 1 == cursorOfkeystrokeIndex )
            {
            editPane->Reset();
            editPane->SetCursorIndexOfKeystroke( 1 );
            }
        //delete the current keystroke
        if ( DeleteCurrentKeystroke() )
            {
            if ( !TryCloseUI( aLength ) )
                {
                ClearDeliberateSelection();
                popup->SetPhraseCreationEditingState( ETrue );
                ImplicityShowInfo();
                ChangeCba();
                }
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }            
        }  
    }

// ---------------------------------------------------------
// EKeyOK And EKeyCBA1L.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::HandleKeyOKAndKeyCBA1L()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    TInt validKeystrokeCount = popup->InEffectKeystrokeArray()->Count();
    TInt keystrokeCount = popup->ShowKeystrokeArray()->Count();
    CDesCArrayFlat* chinese = popup->ChooseChineseCharacterArray();
    TBuf<KMaxPhraseCreationCount> phraseCreated;
    
    if ( ( chinese->Count() >= 2 ) && ( 0 == keystrokeCount ) )
        {
        //commit the chinese character.
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        for (TInt i = 0; i < chinese->Count(); ++i )
            {
            phraseCreated.Append( chinese->MdcaPoint( i ) );
            }
        AddPhraseToDB( phraseCreated );
        fepMan->PinyinPhraseCreation( EFalse );
        fepMan->NewTextL( phraseCreated );
        fepMan->CommitInlineEditL();
        iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
        }
    else if ( 0 != validKeystrokeCount )
        {
        popup->SetChangeState( ETrue );
        popup->SetFromEditToSpellingFlag( ETrue );
        iOwner->ChangeState( ESpellingSelection );
        }
    else
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        }
    }

// ---------------------------------------------------------
// others key input.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::HandleOthersKey( TInt aKey )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    iOwner->FepMan()->SetCcpuFlag( CAknFepManager::ECcpuStateIgnoreStarUp );
    if ( iOwner->IsValidChineseInputKey( aKey ) )
        {
        if ( AddKeystroke( aKey ) )
            {
            ClearDeliberateSelection();
            popup->SetPhraseCreationEditingState( ETrue );
            ImplicityShowInfo();
            ChangeCba();
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        }
    else if ( aKey == EPtiKeyStar ) // we increment the tone mark.
        {
        if ( AddToneMark() )
            {
            ClearDeliberateSelection();
            popup->SetPhraseCreationEditingState( ETrue );
            ImplicityShowInfo();
            ChangeCba();
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }            
        }
    else
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        }
    }

// ---------------------------------------------------------
// move cursor to left.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryPinyinPhraseCreation::IsMoveCursor()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    TInt keystrokeCount = popup->ShowKeystrokeArray()->Count();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    if ( ( 0 == keystrokeCount ) || ( KMaxPhraseCreationCount == chineseCount ) )
        {
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------
// move cursor to left.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::MoveCursorToLeft()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt keystrokeCount = popup->ShowKeystrokeArray()->Count();
    TInt indexOfKeystroke = editPane->GetCursorIndexOfKeystroke();

    if ( 0 == indexOfKeystroke )
        {
        editPane->SetCursorIndexOfKeystroke( keystrokeCount );
        // for fix bug when press left, spelling pane will flash
        // ImplicityShowInfo();
        ShowInfo();
        SetWarningColor();
        }
    else if ( 1 == indexOfKeystroke )
        {
        editPane->SetCursorIndexOfKeystroke( 0 );
        // for fix bug when press left, spelling pane will flash
        // ImplicityShowInfo();
        ShowInfo();
        SetWarningColor();
        }
    else
        {
        editPane->SetCursorIndexOfKeystroke( indexOfKeystroke - 1 );
        editPane->MoveCursorLeft();
        }
    }


// ---------------------------------------------------------
// move cursor to left.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::MoveCursorToRight()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt keystrokeCount = popup->ShowKeystrokeArray()->Count();
    TInt indexOfKeystroke = editPane->GetCursorIndexOfKeystroke();

    if ( indexOfKeystroke == keystrokeCount )
        {
        editPane->SetCursorIndexOfKeystroke( 0 );
        // for fix bug when press right, spelling pane will flash
        // ImplicityShowInfo();
        ShowInfo();
        SetWarningColor();
        }
    else
        {
        editPane->SetCursorIndexOfKeystroke( indexOfKeystroke + 1 );
        editPane->MoveCursorRight();
        }
    }

// ---------------------------------------------------------
// Initialize State.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryPinyinPhraseCreation::InitializeState()
    {
    ChangeCba();
    }

// ---------------------------------------------------------
// Add phrase to DB.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryPinyinPhraseCreation::AddPhraseToDB(
    const TDesC& aPhraseAdd )
    {
    TInt retCode;
    TPtiUserDictionaryEntry addUdbEntry(aPhraseAdd);
    //Add the phrase to the DB by PTI Engine
    retCode = iOwner->PtiEngine()->AddUserDictionaryEntry( addUdbEntry );
    if ( KErrNone != retCode )
        {
        return EFalse;
        }

    return ETrue;
    }

// ---------------------------------------------------------
// Try to close UI.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryPinyinPhraseCreation::TryCloseUI( TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    TInt keystrokeCount = popup->ShowKeystrokeArray()->Count();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    
    if ( ( 0 == chineseCount ) && ( 0 == keystrokeCount ) )
        {
        TRAP_IGNORE( iOwner->FepMan()->TryCloseUiL() ); //no more keys, close the UI.
        if ( aLength == ELongKeyPress )
            {
            iOwner->FepMan()->SetLongClearAfterCloseUI( ETrue );
            }
        return ETrue;
        }
    
    return EFalse;
    }

// End of file
