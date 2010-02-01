/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
 *       Provides the  TAknFepInputStateEntryMiniQwertyZhuyinPhrase,
 *       TAknFepInputStateEditingMiniQwertyZhuyinPhrase,
 *       TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase methods.
 *
*/












// System includes
#include <PtiEngine.h>  
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote
#include <AknFep.rsg>  

// User includes
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "aknfepuictrleditpane.h"           //phrase creation
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManager.h" 
#include "aknfepuiinputstateminiqwertyzhuyinphrase.h"
#include "aknfepinputstateminiqwertyzhuyinkeymap.h"

const TInt KMaxKeystrokeCount = 31;
const TInt KMaxKeystrokeSize = 130;
const TInt KMaxPhraseLength = 7;
const TInt KMinCandidateCount = 1;
const TInt KKeystrokeMaxLength = 4;
//const TInt KTonemarkCount = 5;

const TUint16 KToneMark1 = 0x0020;
const TUint16 KToneMark2 = 0x02CA;
const TUint16 KToneMark3 = 0x02c7;
const TUint16 KToneMark4 = 0x02CB;
const TUint16 KToneMark5 = 0x02D9;

_LIT(KLeftBracket, "(");
_LIT(KRightBracket, ")");
_LIT(KReplace, "");

_LIT( KDelimiter, "'");
struct TToneZhuYinMap
    {
    TUint16 iValue;
    TInt iInputTone;
    };
const TToneZhuYinMap ZhuyinToneMap[] =
    {
        {KToneMark1, 1},
        {KToneMark2, 2},
        {KToneMark3, 3},
        {KToneMark4, 4},
        {KToneMark5, 5}
    };
// ---------------------------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::TAknFepInputStateEditingMiniQwertyZhuyinPhrase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateEditingMiniQwertyZhuyinPhrase::TAknFepInputStateEditingMiniQwertyZhuyinPhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputMiniQwertyZhuyinPhraseBase( aOwner, aUIContainer)
    {
    iState = EMiniQwertyEdit;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane( EFalse );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutPhraseCreation );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->ShowVerticalScrollArrows( ETrue );
    uiContainer->ShowHorizontalScrollArrows( EFalse );
    uiContainer->InputPane()->SetOverrideFontId( 0 );
    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    ImplicityShowInfo( ETrue );
    // so that it could handle EKeyCBA1
    iOwner->FepMan()->EntryPhrase( ETrue );
    RefreshEditPane();
    uiContainer->PinyinPopupWindow()->SetNoneHighlighted();
    TRAP_IGNORE( ChangeCbaL() );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
// 
TBool TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    TBool state = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    CDesCArrayFlat* phraseArray = editPane->PhraseArray();
    
    if ( iOwner->FepMan()->GetComposChrFlag() && aKey == EStdKeyLeftFunc )
        {
        iOwner->FepMan()->SetComposChrFlag( EFalse );
        return ret;
        }
    
    switch ( aKey )
        {
        case EStdKeyLeftShift:
        case EStdKeyRightShift:
        case EStdKeyLeftCtrl:
        case EStdKeyRightCtrl:
        case EStdKeyRightFunc://fn 
        case EStdKeyLeftFunc: //chr key
        case EStdKeyUpArrow:
        case EKeyRightFunc:
        case EKeyLeftFunc:
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            break;
            //if the canidate is empty ,inactive;else candiniate selecting state
        case EStdKeyDownArrow:
            {
            if ( 0 != candidatePane->VisibleCandidateCount() && phraseArray->Count() < 7 )
                {
                uiContainer->EditPaneWindow()->SetChangeState( ETrue );
                iOwner->ChangeState( EZhuyinSpelling );
                }
            }
            break;
        case EStdKeyRightArrow:
            MoveCursorRight();
            break;
        case EStdKeyLeftArrow:
            MoveCursorLeft();
            break;
        case EStdKeyDevice1: //"cancle" key
            iOwner->FepMan()->TryCloseUiL();
            break;
        case EStdKeyEnter:
            CommitContentL( EFalse );
            break;
        case EStdKeyDevice0:
        case EStdKeyDevice3:
        	
            if ( editPane->IsChangeState() && aLength == ELongKeyPress )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                HandleCommitL();
                }
            break;
        case EStdKeyBackspace:
            {
            if ( editPane->IsChangeState() && aLength == ELongKeyPress )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                HandleKeyBackspaceL( aLength );
                }
            }
            break;
        default:
            {
            if ( aLength == EShortKeyPress && keystrokeArray->Count() < KMaxKeystrokeCount && 
                !iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed ) && 
                phraseArray->Count() < KMaxPhraseLength &&
                ( iOwner->IsValidChineseInputKeyQwerty( aKey ) || aKey == EStdKeySpace ) )
                {
                AddKeystrokeL( aKey );
                RefreshUI();
                ShowEEPInfo();
                ChangeCbaL();
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            }
            break;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::AddKeystrokeL
// Add key stroke.
// ---------------------------------------------------------------------------
// 
TBool TAknFepInputStateEditingMiniQwertyZhuyinPhrase::AddKeystrokeL( TInt aKey )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt showKeystrokeCount = editPane->ShowKeystrokeArray()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();

    if ( phraseCount + showKeystrokeCount >= KMaxKeystrokeCount - 1 )
        {
        return EFalse;
        }
    TBuf<KKeystrokeMaxLength> keystroke;
    
    TAknFepInputStateMiniQwertyZhuyinKeyMap::GetKeyStroke(
            iOwner->PtiEngine(), aKey, keystroke );
    
    if ( index >= keystrokeArray->Count() )
        {
        keycodeArray->Append( aKey );
        keystrokeArray->AppendL( keystroke );
        editPane->SetCursorIndexOfKeystroke( keystrokeArray->Count() );
        }
    else
        {
        keycodeArray->Insert( aKey, index );
        keystrokeArray->InsertL( index, keystroke );
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        }
    editPane->SetNeedClearDeliberateSelection( ETrue );
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::RevertPhraseToKeystrokeL
// Revert the phrase to keystroke.
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::RevertPhraseToKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    RArray<TInt>* phraseKeycode = editPane->PhraseKeycodeArray();
    CDesCArrayFlat* phraseStroke = editPane->PhraseShowKeyStrokeArray();
    TBuf<KMaxKeystrokeCount> buf;

    if ( (phrase->Count() == 0 ) || (phraseStroke->Count() == 0 ) )
        {
        return;
        }
    
    buf.Append( phraseStroke->MdcaPoint( phraseStroke->Count() - 1 ) );
    
    TInt insertIndex = 0;
    while ( buf.Length() != 0 )
        {
        TBuf<KKeystrokeMaxLength> strokeBuf;
        TInt index = buf.Find( KRightBracket );
        if ( index != KErrNotFound )
            {
            strokeBuf.Copy( buf.Left( index + 1 ) );
            keystrokeArray->InsertL( insertIndex, strokeBuf );
            buf.Replace( 0, index + 1, KNullDesC );
            }
        else
            {
            keystrokeArray->InsertL( insertIndex, buf );
            insertIndex = insertIndex + 1;
            break;
            }
        insertIndex = insertIndex + 1;
        }
    phrase->Delete( phrase->Count() - 1 );
    phraseStroke->Delete( phraseStroke->Count() - 1 );
    
    if ( insertIndex > phraseKeycode->Count() )
        {
        insertIndex = keycodeArray->Count();
        }
    
    for ( TInt i = 0; i < insertIndex ; ++i )
        {
        keycodeArray->Insert( (*phraseKeycode)[phraseKeycode->Count() - 1 ],0 );
        phraseKeycode->Remove( phraseKeycode->Count() - 1 );
        }
    
    TInt keystrokeCount = keystrokeArray->Count();
    TInt lenth = phrase->Count() + keystrokeCount;
    TInt deleteCount = lenth - KMaxKeystrokeCount;

    for (TInt i = 0; i < deleteCount; ++i )
        {
        keycodeArray->Remove( keycodeArray->Count() - 1 );
        keystrokeArray->Delete( keystrokeArray->Count() - 1 );
        }
    editPane->SetCursorIndexOfKeystroke( insertIndex );
    keystrokeArray->Compress();
    phrase->Compress();
    phraseStroke->Compress();
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::AddPhraseToDB
// Add phrase to DB.
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::AddPhraseToDB(
    const TDesC& aPhraseAdd )
    {
    TPtiUserDictionaryEntry addUdbEntry(aPhraseAdd);
    //Add the phrase to the DB by PTI Engine
    iOwner->PtiEngine()->AddUserDictionaryEntry( addUdbEntry );
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::MoveCursorLeft
// Move the cursor to Left.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEditingMiniQwertyZhuyinPhrase::MoveCursorLeft()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keyStrokeArray = editPane->KeystrokeArray();
    TInt keystrokeCount = keyStrokeArray->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TInt phraseCount = editPane->PhraseArray()->Count();

    if ( EMiniQwertyEdit != iState )
        {
        return EFalse;
        }
    if ( 0 == keystrokeCount )
        {
        return EFalse;
        }

    if ( 0 == index )
        {
        editPane->SetCursorIndexOfKeystroke( keystrokeCount );
        ShowEEPInfo();
        }
    else if ( phraseCount > 0 && 1 == index )
        {
        editPane->SetCursorIndexOfKeystroke( 0 );
        ShowEEPInfo();
        }
    else
        {
        TInt stride = keyStrokeArray->MdcaPoint( index - 1 ).Length();
        editPane->MoveCursorLeft( stride );
        editPane->SetCursorIndexOfKeystroke( index - 1 );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::MoveCursorRight
// Move the cursor to Right.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEditingMiniQwertyZhuyinPhrase::MoveCursorRight()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TInt keystrokeCount = keystroke->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();

    if ( EMiniQwertyEdit != iState )
        {
        return EFalse;
        }
    if ( 0 == keystrokeCount )
        {
        return EFalse;
        }
    if ( index >= keystrokeCount )
        {
        editPane->SetCursorIndexOfKeystroke( 0 );
        ShowEEPInfo();
        }
    else
        {
        TInt stride = editPane->KeystrokeArray()->MdcaPoint( index ).Length();
        editPane->MoveCursorRight( stride );
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleKeyBackspace
// Handle Key Backspace
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleKeyBackspaceL( 
                                                     TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();

    CDesCArrayFlat* keyStrokeArray = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt keystrokeCount = keyStrokeArray->Count();
    TInt phraseCount = editPane->PhraseArray()->Count();

    TInt cursorOfkeystrokeIndex = editPane->GetCursorIndexOfKeystroke();

    if ( 1 == cursorOfkeystrokeIndex )
        {
        editPane->Reset();
        }
    
    if ( cursorOfkeystrokeIndex > 0 && 
        ( keystrokeCount > 1 || phraseCount > 0 ) )
        {
        keycodeArray->Remove( cursorOfkeystrokeIndex - 1 );
        keyStrokeArray->Delete( cursorOfkeystrokeIndex - 1 );
        editPane->SetCursorIndexOfKeystroke( cursorOfkeystrokeIndex - 1 );
        }
    else if ( phraseCount > 0 && 0 == cursorOfkeystrokeIndex )
        {
        RevertPhraseToKeystrokeL();
        }
    else if ( 1 == cursorOfkeystrokeIndex )
        {
        iOwner->FepMan()->TryCloseUiL();
        if ( aLength == ELongKeyPress )
            {
            iOwner->FepMan()->SetLongClearAfterCloseUI( ETrue );
            }
        return;
        }
    else
        {
        return;
        }
    editPane->SetNeedClearDeliberateSelection( ETrue );
    RefreshUI();
    ShowEEPInfo();
    ChangeCbaL();
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::CommitContentL
// Commit content
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::CommitContentL(
    TBool aAddUDB )
    {
    TBuf<KMaxKeystrokeSize> currentText;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    MAknFepUICtrlEditPane* editpane = UIContainer()->EditPaneWindow();
    CDesCArrayFlat* phraseArray = editpane->PhraseArray();
    CDesCArrayFlat* keystrokeArray = editpane->KeystrokeArray();
    TInt phraseCount = phraseArray->Count();
    TInt keyStrokeCount = keystrokeArray->Count();

    for (TInt i = 0; i < phraseCount; i++ )
        {
        currentText.Append( phraseArray->MdcaPoint( i ) );
        }
    for (TInt i = 0; i < keyStrokeCount; i++ )
        {
        currentText.Append( keystrokeArray->MdcaPoint( i ) );
        }

    if ( currentText.Length() )
        {
        while (currentText.Find( KLeftBracket ) >= 0 )
            {
            currentText.Replace( currentText.Find( KLeftBracket ), 1,
                KReplace );
            }
        while (currentText.Find( KRightBracket ) > 0 )
            {
            currentText.Replace( currentText.Find( KRightBracket ), 1,
                KReplace );
            }
        fepMan->NewTextL( currentText );
        fepMan->CommitInlineEditL();
        }
    UIContainer()->EditPaneWindow()->ResetAllArray();
    UIContainer()->EditPaneWindow()->SetPhraseCreationFlag( EFalse );
    if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
        {
        fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
        }
    if ( aAddUDB )
        {
        AddPhraseToDB( currentText );
        }
    fepMan->TryCloseUiL();
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleCommitL
// Handle commit action
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleCommitL()
    {
    TBuf<KMaxKeystrokeSize> currentText;
    MAknFepUICtrlEditPane* editpane = UIContainer()->EditPaneWindow();
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    CDesCArrayFlat* phraseArray = editpane->PhraseArray();
    CDesCArrayFlat* keystrokeArray = editpane->KeystrokeArray();
    TInt phraseCount = phraseArray->Count();
    TInt keyStrokeCount = keystrokeArray->Count();

    if ( 0 == candidatePane->VisibleCandidateCount() && ( 1 == phraseCount
        || keyStrokeCount > 0 ) )
        {
        return;
        }
    if ( 0 != candidatePane->VisibleCandidateCount() && phraseCount < KMaxPhraseLength )
        {
        UIContainer()->EditPaneWindow()->SetChangeState( ETrue );
        iOwner->ChangeState( EZhuyinSpelling );
        return;
        }
    CommitContentL( ETrue );
    }

// ---------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::RefreshUI
// Refresh UI if necessary
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::RefreshUI()
    {
    MAknFepUICtrlEditPane* editpane = UIContainer()->EditPaneWindow();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* showKeystroke = editpane->ShowKeystrokeArray();
    TBuf<KMaxKeystrokeCount> preValidKeystroke;
    TInt preValidKeystrokeCount = showKeystroke->Count();
    for (TInt i=0; i < preValidKeystrokeCount; i++ )
        {
        preValidKeystroke.Append( showKeystroke->MdcaPoint( i ) );
        }
    GetValidKeystroke();
    TBuf<KMaxKeystrokeCount> validKeystroke;
    TInt validKeystrokeCount = showKeystroke->Count();
    for (TInt i=0; i < validKeystrokeCount; i++ )
        {
        validKeystroke.Append( showKeystroke->MdcaPoint( i ) );
        }
    if ( preValidKeystroke.Compare( validKeystroke ) != 0 )
        {
        RefreshSpellingPane( ETrue );
        RefreshCandidatePane();
        }
    else if ( popup->CurrentSelection() != 0 )
        {
        RefreshCandidatePane();
        }
    UpdateIndicator();
    }

// ---------------------------------------------------------
// Init the show info
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::ImplicityShowInfo(
    TBool aGetScreenCoordinates )
    {
    TRAP_IGNORE( ImplicityShowInfoL( aGetScreenCoordinates ) );
    }

// ---------------------------------------------------------
// Init the show info
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::ImplicityShowInfoL(
    TBool aGetScreenCoordinates )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    // get cursor position
    TPoint baseLine = TPoint( 0, 0 );
    TInt height = 0;
    TInt ascent = 0;
    if ( aGetScreenCoordinates )
        {
        iOwner->FepMan()->GetScreenCoordinatesL( baseLine, height, ascent );
        uiContainer->SetContainerPosition( baseLine, height );
        }
    editPane->ActiveCursor();
    }

// ---------------------------------------------------------
// Init the show info
// ---------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::ShowEEPInfo()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TInt cursorPosInKeystroke = 0;

    TBuf<KMaxKeystrokeSize> buf;

    for (TInt i = 0; i < phrase->Count(); ++i )
        {
        buf.Append( phrase->MdcaPoint( i ) );
        }

    for (TInt i = 0; i < keystroke->Count(); ++i )
        {
        buf.Append( keystroke->MdcaPoint( i ) );
        }

    for (TInt i = 0; i < index; ++i )
        {
        cursorPosInKeystroke = cursorPosInKeystroke + keystroke->MdcaPoint( i ).Length();
        }

    editPane->SetText( buf, phrase->Count() + cursorPosInKeystroke, ETrue );
    if ( editPane->GetEffictiveLength() == 0 )
        {
        editPane->SetHighlight( phrase->Count(), buf.Length() );
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateSpellingMiniQwertyZhuyin::TAknFepInputStateSpellingMiniQwertyZhuyin
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateSpellingMiniQwertyZhuyin::TAknFepInputStateSpellingMiniQwertyZhuyin(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputMiniQwertyZhuyinPhraseBase( aOwner, aUIContainer)
    {
    iState = EZhuyinSpelling;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane( EFalse );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->ShowVerticalScrollArrows( ETrue );
    uiContainer->ShowHorizontalScrollArrows( EFalse );
    uiContainer->InputPane()->SetOverrideFontId( 0 );
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutPhraseCreation );
    uiContainer->EditPaneWindow()->DeactiveCursor();
    RefreshUI();
    // however we also need to clear the deliberate selection, in case we are not
    // coming back to the input pane from the candidate pane
    ClearDeliberateSelection();
    UpdateIndicator();

    iOwner->PtiEngine()->SetInputMode( EPtiEngineZhuyinPhraseQwerty );
    iOwner->PtiEngine()->SetCase( EPtiCaseLower );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateSpellingMiniQwertyZhuyin::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
// 
TBool TAknFepInputStateSpellingMiniQwertyZhuyin::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TBool ret = ETrue;

    if ( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() || CAknFepFnKeyManager::EFnKeyLock
        == iOwner->FepMan()->FnKeyState() || CAknFepFnKeyManager::EFnKeyDown
        == iOwner->FepMan()->FnKeyState() )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return ETrue;
        }
    else if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed )
        || iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed ) )
        {
        return ETrue;
        }

    switch ( aKey )
        {
        case EPtiKeyQwertyE:
        case EPtiKeyQwertyR:
        case EPtiKeyQwertyY:
        case EPtiKeyQwertyU:
        case EStdKeyLeftFunc:
            {
            if ( aLength == EShortKeyPress )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            }
            break;
        case EKeyRightFunc:
        case EKeyLeftShift:
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EStdKeyEnter:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
            break;
        case EStdKeyBackspace:
        case EStdKeyUpArrow:
            {
            TBool state = ETrue;
            if ( editPane->IsChangeState() && aLength == ELongKeyPress )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                editPane->SetChangeState( ETrue );
                iOwner->ChangeState( EMiniQwertyEdit );
                }
            }
            break;
        case EStdKeyDevice1:
            {
            iOwner->FepMan()->TryCloseUiL();
            }
            break;
        case EStdKeyRightArrow:
            {
            if ( popup->IsEnabled() )
                {
                if ( popup->SelectNextPhrase() )
                    {
                    RefreshVaildKeyStroke();
                    RefreshCandidatePane();
                    RefreshSpellingPane();
                    RefreshEditPane();
                    UpdateIndicator();
                    }
                }
            }
            break;
        case EStdKeyLeftArrow:
            {
            if ( popup->IsEnabled() )
                {
                if ( popup->SelectPrevPhrase() )
                    {
                    RefreshVaildKeyStroke();
                    RefreshCandidatePane();
                    RefreshSpellingPane();
                    RefreshEditPane();
                    UpdateIndicator();
                    }
                }
            }
            break;
        case EStdKeyDownArrow:
        case EStdKeyDevice3:
        case EStdKeyDevice0:
            {
            TBool state = ETrue;
            if ( editPane->IsChangeState() && aLength == ELongKeyPress )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                if ( popup->IsEnabled() )
                    {
                    popup->Enable( EFalse );
                    }
                editPane->SetChangeState( ETrue );
                iOwner->ChangeState( EZhuyinCandidate );
                }
            }
            break;
        default:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            return TAknFepInputMiniQwertyZhuyinPhraseBase::HandleKeyL(
                aKey, aLength );
            }
//            break;
        }
    return ret;
    }

// ---------------------------------------------------------
// TAknFepInputStateSpellingMiniQwertyZhuyin::RefreshUI
// Refresh UI if necessary
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingMiniQwertyZhuyin::RefreshUI()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    // get cursor position
    TPoint baseLine = TPoint( 0, 0 );
    TInt height = 0;
    TInt ascent = 0;

    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if ( ret == KErrNone )
        {
        uiContainer->SetContainerPosition( baseLine, height );
        }
    
    GetValidKeystroke( );
    TBool needClearDeliberateSelection = editPane->NeedClearDeliberateSelection();
    if ( needClearDeliberateSelection )
        {
        editPane->SetNeedClearDeliberateSelection( EFalse );
        }
    else
        {
        RefreshVaildKeyStroke();
        }
    RefreshEditPane();
    RefreshCandidatePane( needClearDeliberateSelection );
    RefreshSpellingPane( needClearDeliberateSelection );
    }

// ---------------------------------------------------------
// TAknFepInputStateSpellingMiniQwertyZhuyin::RefreshUI
// Refresh vaild key stroke length
// ---------------------------------------------------------
//
void TAknFepInputStateSpellingMiniQwertyZhuyin::RefreshVaildKeyStroke()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    TInt currentSel = popup->CurrentSelection();
    if ( currentSel >= spelling->Count() )
        {
        currentSel = 0;
        }
    TPtrC ptr = spelling->MdcaPoint( currentSel );
    TInt currentLength = ptr.Length();
    editPane->SetEffictiveLength( currentLength );
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::TAknFepInputStateCanindateSelectingMiniQwertyZhuyin
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::TAknFepInputStateCanindateSelectingMiniQwertyZhuyin(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputMiniQwertyZhuyinPhraseBase( aOwner, aUIContainer)
    {
    iState = EZhuyinCandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() && uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength
        > KMinCandidateCount;

    uiContainer->ShowHorizontalScrollArrows( showHorizontalScrollArrows );
    uiContainer->ShowVerticalScrollArrows( multiplePages );
    uiContainer->FocusCandidatePane( ETrue );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( ETrue );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    UpdateIndicator();
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::HandleKeyL(
    TInt aKey, TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt selected = uiContainer->CandidatePane()->SelectedIndex();
    TInt numbertotal = uiContainer->CandidatePane()->VisibleCandidateCount();
    TBool ret = ETrue;

    if (iOwner->FepMan()->GetComposChrFlag() )
        {
        iOwner->FepMan()->SetComposChrFlag(EFalse);
        return ETrue;
        }
    switch ( aKey )
        {
        case EStdKeySpace:
        case EStdKeyEnter:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDDefaultSound );
            }
            break;
        case EStdKeyLeftArrow:
            {
            if ( selected == 0 )
                {
                UIContainer()->CandidatePane()->SelectIndex( numbertotal-1 );
                }
            else
                {
                UIContainer()->CandidatePane()->SelectIndex( selected - 1 );
                }
            }
            break;
        case EStdKeyRightArrow:
            {
            if ( selected == numbertotal-1 )
                {
                UIContainer()->CandidatePane()->SelectIndex( 0 );
                }
            else
                {
                UIContainer()->CandidatePane()->SelectIndex( selected + 1 );
                }
            break;
            }
        case EStdKeyDevice1: // cancel key
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        case EStdKeyUpArrow:
            {
            if ( uiContainer->CandidatePane()->IsFirstPage() ) // same as the backspace
                {
                editPane->SetChangeState( ETrue );
                iOwner->ChangeState( EZhuyinSpelling );
                }
            else // just move to the last page 
                {
                uiContainer->CandidatePane()->PreviousCandidatePage();
                uiContainer->CandidatePane()->SetCandidateBuffer();
                uiContainer->CandidatePane()->SelectFirstPhrase();
                UpdateIndicator();
                }
            break;
            }
        case EStdKeyDownArrow:
            {
            ret = TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL( aKey, aLength );
            }
            break;
        case EStdKeyBackspace: // 
            {
            TBool state = ETrue;
            if ( editPane->IsChangeState() && aLength == ELongKeyPress )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                editPane->SetChangeState( ETrue );
                iOwner->ChangeState( EZhuyinSpelling );
                }
            }
            break;
        case EStdKeyDevice0: //commit to editor
        case EStdKeyDevice3:
            {
            TBool state = ETrue;
            if ( editPane->IsChangeState() && aLength == ELongKeyPress )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                editPane->SetNeedClearDeliberateSelection( ETrue );
                CommitCandidateL();
                }
            }
            break;
        default:
            if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDDefaultSound);
                break;
                }
            if ( SelectCandidate( aKey ) ) // is 1-6 key 
                {
                editPane->SetNeedClearDeliberateSelection( ETrue );
                CommitCandidateL();
                }
            else
                {
                return TAknFepInputMiniQwertyZhuyinPhraseBase::HandleKeyL(
                    aKey, aLength );
                }
            break;
        }
    return ret;
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::CommitCandidate
// commint the charater to the editor pane 
// ---------------------------------------------------------------------------

TInt TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::GetIndexOfToneMark( const TDesC& aText)
    {
    TInt indexOfToneMark = -1;
    TInt num = sizeof(ZhuyinToneMap )/( sizeof(ZhuyinToneMap[0]));
    TInt len = aText.Length();
    for( TInt j = 0; j< len ;j++ )
        {
        indexOfToneMark = j;
        TBuf<1> keyStorkeBuf = aText.Mid( j ,1 );
        for (TInt i = 0; i<num; i++ )
            {
            if (  keyStorkeBuf[0] == ZhuyinToneMap[i].iValue  )
                {
                return indexOfToneMark;
                }
            }
        }
    return -1;
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::CommitCandidate
// commint the charater to the editor pane 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::CommitCandidateL()
    {
    CDesCArrayFlat* keyStrokeArray = UIContainer()->EditPaneWindow()->KeystrokeArray();
    CDesCArrayFlat* phraseKeyStrokeArray = UIContainer()->EditPaneWindow()->PhraseShowKeyStrokeArray();
    CDesCArrayFlat* phraseArray = UIContainer()->EditPaneWindow()->PhraseArray();
    TPtrC candidate = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
    
    RArray<TInt>* keyCodeArray = UIContainer()->EditPaneWindow()->KeycodeArray();
    RArray<TInt>* keyCodePhraseArray = UIContainer()->EditPaneWindow()->PhraseKeycodeArray();
    TBuf<KMaxKeystrokeCount> spellingText;
    UIContainer()->InputPane()->GetText( spellingText );
    phraseArray->AppendL( candidate );
    TBuf<KMaxKeystrokeCount> keyStoke;
    TInt replaceCount = 0;
    TInt delimiterIndex = spellingText.Find( KDelimiter );
    if ( KErrNotFound ==delimiterIndex )
        {
        delimiterIndex = -1;
        }
    TInt indexOfToneMark = GetIndexOfToneMark( spellingText );
    if ( delimiterIndex != KErrNotFound && indexOfToneMark >= 0 )
        {
        replaceCount = Min( delimiterIndex, indexOfToneMark );
        if ( delimiterIndex > indexOfToneMark )
            {
            replaceCount++;
            }
        }
    else if ( delimiterIndex != KErrNotFound && indexOfToneMark == -1 )
        {
        replaceCount = delimiterIndex;
        }
    else if ( delimiterIndex == KErrNotFound && indexOfToneMark != -1 )
        {
        replaceCount = indexOfToneMark + 1;
        }
    else
        {
        replaceCount = spellingText.Length();
        } 
    for (TInt i = 0; i < replaceCount; i++ )
        {
        TPtrC ptr = keyStrokeArray->MdcaPoint( 0 );
        keyStoke.Append( ptr );
        keyStrokeArray->Delete( 0 );
        // added for the keycode
        TInt keyCodeValue = ( *keyCodeArray )[0];
        keyCodePhraseArray->AppendL( keyCodeValue );
        keyCodeArray->Remove( 0 );
        }
    phraseKeyStrokeArray->AppendL( keyStoke );

    if ( GetValidKeystroke() )
        {
        iOwner->ChangeState( EZhuyinSpelling );
        }
    else
        {
        iOwner->ChangeState( EMiniQwertyEdit );
        }
    UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
    RefreshSpellingPane();
    RefreshCandidatePane();
    ChangeCbaL();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::CommitCandidate
// to check the select number is valid or not 
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::SelectCandidate(
    TInt aKey )
    {
    TInt index = 0;
    MPtiLanguage *lang= NULL;
    RArray <TPtiNumericKeyBinding> keybinding;
    TBool validnumkey = EFalse;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtiKeyboardType keyboardtype = iOwner->FepMan()->KeyboardLayout();
    lang = ptiengine->CurrentLanguage();
    if ( ptiengine && lang  )
        {
        TRAP_IGNORE(ptiengine->GetNumericModeKeysForQwertyL(lang->LanguageCode(), keybinding, keyboardtype));
        TInt numericKeysCount = keybinding.Count();
        while (numericKeysCount-- )
            {
            TPtiNumericKeyBinding numKeyBind = keybinding[numericKeysCount];
            if ( (numKeyBind.iChar >= 0x31 && numKeyBind.iChar <= 0x36)
                &&(aKey == numKeyBind.iKey) )
                {
                index = numKeyBind.iChar - EPtiKey0 -1;
                if ( UIContainer()->CandidatePane()->SelectIndex( index, EFalse ) )
                    {
                    validnumkey = ETrue;
                    }
                break;
                }
            }
        }
    keybinding.Reset();
    return validnumkey;
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyZhuyinPhrase::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane =
        UIContainer()->CandidatePane();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TBool state = ETrue;
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            if ( editPane->IsChangeState() )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                HandleCommitL();
                }
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateSpellingMiniQwertyZhuyin::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateSpellingMiniQwertyZhuyin::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane =
        UIContainer()->CandidatePane();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    TBool state = ETrue;
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            if ( editPane->IsChangeState() )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                if ( popup->IsEnabled() )
                    {
                    popup->Enable( EFalse );
                    }
                editPane->SetChangeState( ETrue );
                iOwner->ChangeState( EZhuyinCandidate );
                }
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCanindateSelectingMiniQwertyZhuyin::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane =
        UIContainer()->CandidatePane();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TBool state = ETrue;
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            if ( editPane->IsChangeState() )
                {
                state = EFalse;
                }
            else
                {
                editPane->SetChangeState( EFalse );
                }
            if ( state )
                {
                editPane->SetNeedClearDeliberateSelection( ETrue );
                CommitCandidateL();
                }

            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }
//End of File
