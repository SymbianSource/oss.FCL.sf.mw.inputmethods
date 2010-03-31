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
*      Provides the TAknFepInputStateStrokePhraseCreationBase definition.
*
*/













// System includes
#include <e32cmn.h>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <avkon.rsg>
#include <AknFep.rsg>
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote 

// User includes
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlInputPane.h"
#include "aknfepuictrleditpane.h"           //Stroke phrase creation
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"
#include "aknfepuiinputminiqwertystrokephrasebase.h"

// CONSTANTS

const TInt KMaxPhraseCount = 50;
const TInt KMaxPhraseCreationCount = 7;
const TInt KMaxKeystrokeCount = 31;
const TInt16 KStrokeDelimiter = 0x2022;
const TInt16  KStrokeUnicode = 0x2461;
const TInt16 KKey1 = 0x31;
const TInt16 KKey6 = 0x36;

_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseCreationBase::TAknFepInputStateStrokePhraseCreationBase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputMiniQwertyStrokePhraseBase::TAknFepInputMiniQwertyStrokePhraseBase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    iOwner->PtiEngine()->SetInputMode(EPtiEngineStrokePhraseQwerty);
    iOwner->PtiEngine()->SetCase(EPtiCaseLower);
    iOwner->PtiEngine()->SetCandidatePageLength( KMaxPhraseCount );
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    TBool multiplePages = !(candidatePane->IsLastPage() &&
                            candidatePane->IsFirstPage());
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    }
    
// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyL( 
                                        TInt aKey, 
                                        TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    const TPtiKeyboardType keyboardType = iOwner->PtiEngine()->KeyboardType();
    
    switch( aKey )
        {
        case EKeyLeftShift:
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EStdKeyLeftFunc:    
            {
            iOwner->FepMan()->TryCloseUiL();
            ret = EFalse;
            break;
            }
        case EStdKeyDevice1:// CBA2
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        case EStdKeyUpArrow:
        case EStdKeyDownArrow:    
        case EKeyRightFunc:
        case EStdKeyEnter:     
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            break;
            }
        case EStdKeyBackspace:
            {
            HandleKeyBackspaceL( aLength );
            break;
            }
        case EStdKeyDevice0:// CBA1
        case EStdKeyDevice3:// CBASelect
            {
            HandleCommitL( aLength );
            break;
            }
        case EStdKeyLeftArrow:
            {
            if ( !MoveCursorLeft() )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            break;
            }    
        case EStdKeyRightArrow:
            {            
            if ( !MoveCursorRight() )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }            
            break;
            }
        default:
            {
            ret = HandleDefaultL( aKey, aLength );
            break;
            }
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::RefreshUI
// Update input window.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::RefreshUI()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    // get cursor position
    if ( ( uiContainer->EditPaneWindow()->KeystrokeArray()->Count() == 0 ) && 
        ( uiContainer->EditPaneWindow()->PhraseArray()->Count() == 0 ) )
        {
        TPoint baseLine;
        TInt height(0);
        TInt ascent(0);
        TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
        if (ret == KErrNone)
            {
            uiContainer->SetContainerPosition(baseLine, height);    
            }
        }
    if ( ( uiContainer->EditPaneWindow()->KeystrokeArray()->Count() > 0 ) ||
        ( uiContainer->EditPaneWindow()->PhraseArray()->Count() > 0 ) )
        {
        ShowInfoOnEEPPane();
        SetWarningColor();
        ShowInfoOnCandidatePane();
        UpdateIndicator();
        TRAP_IGNORE( ChangeCbaL() );
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::CheckFirstGroupStroke
// check the first group stroke of keystroke.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::CheckFirstGroupStroke()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt keyCode = 0;
    TInt pos = 0;
    TBuf<1> delimiter;
    
    if ( 0 == keystroke->Count() )
        {
        return EFalse;
        }
    
    delimiter.Append( KStrokeDelimiter );
    
    //the keystroke is not delimiter the all keystroke is first group keystroke.
    if ( 0 != keystroke->Find( delimiter, pos ) )
        {
        pos = keystroke->Count();
        }
    //first keystroke is delimiter then first group keystroke is invalid.
    if ( 0 == pos )
        {
        return EFalse;
        }
    
    ClearPtiEngineKeystroke();
    
    for ( TInt i = 0; i < pos; ++i )
        {
        //GetKeystrokeCode( keyCode, keystroke->MdcaPoint( i ) );
        keyCode = (*keycodeArray)[i];
        stringBeforeLength = 
            ptiengine->GetPhoneticSpelling(1).Length();
        stringAfterLength = 
            ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
        //the keystroke is invalid.
        if ( stringBeforeLength == stringAfterLength )
            {
            return EFalse;
            }
        } 
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleCommitL
// Handle CBA1 or select event.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::HandleCommitL( 
                                                     TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt keyCount = editPane->KeystrokeArray()->Count();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    TInt cdtCount = uiContainer->CandidatePane()->CandidateArray()->Count(); 
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    
    if ( !CheckKeyNeedRepeat( aLength ) )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return;
        }
    
    if ( EMiniQwertyEdit == iState )
        {
        if ( cdtCount > 0 )
            {
            UIContainer()->EditPaneWindow()->SetChangeState( ETrue );
            iOwner->ChangeState( ECandidate );            
            return;
            }
        
        if ( keyCount > 0 )
            {
            fepMan->PlaySound( EAvkonSIDErrorTone );
            return;
            }
        }
    
    if ( phrase->Count( ) <= 1 )
        {
        fepMan->PlaySound( EAvkonSIDErrorTone );
        return;
        }

    TBuf<KMaxPhraseCreationCount> phraseCreated;
    for (TInt i = 0; i < phrase->Count( ); ++i )
        {
        phraseCreated.Append( phrase->MdcaPoint( i ) );
        }
    fepMan->NewTextL( phraseCreated );
    fepMan->CommitInlineEditL( );
    AddPhraseToDB( phraseCreated );
    fepMan->TryCloseUiL( );
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleAutoChangeState
// Auto change state.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::HandleAutoChangeState()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TBool FirstStrokeValid = CheckFirstGroupStroke();
    //Auto change from EMiniQwertyEdit state to EEntry state. 
    if ( EMiniQwertyEdit == iState )
        {
        if ( FirstStrokeValid )
            {
            if ( EAutoChangeStateFromInput == editPane->GetAutoChangeStateFlag() )
                {
                editPane->SetChangeState( EFalse );
                iOwner->ChangeState( EEntry );
                return ETrue;
                }
            else if ( EAutoChangeStateFromCandidate == editPane->GetAutoChangeStateFlag() )
                {
                editPane->SetChangeState( ETrue );
                iOwner->ChangeState( ECandidate );
                return ETrue;
                }
            }
        }
    
    //Auto change from EEntry state to EMiniQwertyEdit state.
    if ( EEntry == iState )
        {
        if ( ( 0 <= index ) && ( !FirstStrokeValid ) )
            {
            editPane->SetChangeState( EFalse );
            editPane->SetAutoChangeStateFlag( EAutoChangeStateFromInput );
            iOwner->ChangeState( EMiniQwertyEdit );
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyBackspaceL
// Handle KeyBackspace event.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyBackspaceL(
                                                    TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();

    if ( !CheckKeyNeedRepeat( aLength ) )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return;
        }
    
    if ( ( 0 == index ) && ( phraseCount != 0 ) )
        {
        RevertPhraseToKeystrokeL();
        }
    else if ( ( 0 == index ) && ( keystroke->Count() > 0 ) )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return;
        }
    else
        {
        if ( !DeleteKeystroke() )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            return;
            }
        }
    
    //if the keystroke is null and phrase is null then close input window.
    if ( ( 0 == phraseCount ) && ( keystroke->Count() == 0 ) )
        {
        iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
        if ( aLength == ELongKeyPress )
            {
            iOwner->FepMan()->SetLongClearAfterCloseUI( ETrue );
            }
        return;
        }
    
    if ( !HandleAutoChangeState() )
        {
        RefreshUI();
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleDefaultL
// Handle default event.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::HandleDefaultL( 
                                                     TInt aKey,
                                                     TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    if( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() ||
             CAknFepFnKeyManager::EFnKeyLock == iOwner->FepMan()->FnKeyState() ||
             ( CAknFepFnKeyManager::EFnKeyDown == iOwner->FepMan()->FnKeyState() && iState != ECandidate ))
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
    else if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed) ||
             iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if ( ( EShortKeyPress == aLength ) && 
        ( iOwner->IsValidChineseInputKeyQwerty( aKey ) ) )
        {
        if ( iState == ECandidate )
            {
            HandleKeyFromCandidateL( aKey, aLength );
            }
        else
            {
            HandleDefaultKeyL( aKey );
            }
        }
    else
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleDefaultKeyL
// Handle default key press event.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::HandleDefaultKeyL( TInt aKey )
    {
    if ( !AddKeystrokeL( aKey ) )
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return;
        }
    
    if ( !HandleAutoChangeState() )
        {
        RefreshUI();
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::ShowInfoOnEEP
// Show the showinfo on the EEP ctrl.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::ShowInfoOnEEPPane()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TBuf<KMaxKeystrokeCount> showInfo;
    
    for ( TInt i = 0; i < phrase->Count(); ++i )
        {
        showInfo.Append( phrase->MdcaPoint( i ) );
        }

    for ( TInt ii = 0; ii < keystroke->Count(); ++ii )
        {
        showInfo.Append( keystroke->MdcaPoint( ii ) );
        }
    
    editPane->SetText( showInfo, index + phrase->Count() );
    
    switch ( iState )
        {
        case ECandidate:
            editPane->DisableCursor();
            break;
        case EEntry:
            editPane->EnableCursor();
            editPane->DeactiveCursor();
            break;
        case EMiniQwertyEdit:
            editPane->EnableCursor();
            editPane->ActiveCursor();
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::ShowInfoOnEEP
// Show the showinfo on the EEP ctrl.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::ShowInfoOnCandidatePane()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    CDesCArrayFlat* phrase = uiContainer->CandidatePane()->CandidateArray();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    if ( KMaxPhraseCreationCount == editPane->PhraseArray()->Count() )
        {
        phrase->Reset();
        TRAP_IGNORE( phrase->AppendL( KMaxPhraseNote ) );
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( EFalse );
        }
    
    TRAP_IGNORE( GetCandidateL() );
    if ( phrase->Count() > 0 )
        {
        // layout candidate and fill candidate buffer for first display page
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                uiContainer->CandidatePane()->IsFirstPage());
        uiContainer->ShowVerticalScrollArrows( multiplePages );
        if ( iState == EMiniQwertyEdit )
            {
            UIContainer()->FocusCandidatePane( EFalse );
            }
        else
            {
            UIContainer()->FocusCandidatePane( ETrue );
            UIContainer()->CandidatePane()->SelectFirstPhrase();
            }
        }
    else
        {
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( EFalse );
        uiContainer->SetControlInVisible( EFalse );
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleHorizontalNavigation
//Handle horizontal navigation.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::HandleHorizontalNavigation(
                                                                   TInt aKey )
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if ( aKey == EStdKeyLeftArrow )
        {        
        if(!candidatePane->SelectPrev())
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else if ( aKey == EStdKeyRightArrow )
        {              
        if(!candidatePane->SelectNext())
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirstPhrase();
            }
        response = ETrue;
        }
        
    if (response)
        {
        UpdateIndicator();
        }   
             
    return response;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::UpdateIndicator
// Update Indicator.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::UpdateIndicator()
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
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if (iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty4x10)
        {
        if (candidatePane->VisibleCandidateCount()==1)
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
            uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);
            }
        else
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
            uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
            }
        }
    else
        {
#endif
        if (candidatePane->SelectedIndex() == 0
                && candidatePane->IsFirstPage() )
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
            }
        else
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
            }

        if (candidatePane->IsLastPage() && (candidatePane->SelectedIndex()
                == candidatePane->VisibleCandidateCount() - 1))
            {
            uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);
            }
        else
            {
            uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    }
#endif    

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::DeleteKeystroke
// Delete current keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::DeleteKeystroke()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    
    if ( ( 0 == index ) || ( keystroke->Count() == 0 ) )
        {
        return EFalse;
        }

    if ( 1 == index )
        {
        editPane->Reset();
        }
    
    if ( index >= keystroke->Count() )
        {
        keystroke->Delete( keystroke->Count() - 1 );
        keycodeArray->Remove( keycodeArray->Count() - 1 );
        editPane->SetCursorIndexOfKeystroke( keystroke->Count() );
        }
    else
        {
        keystroke->Delete( index - 1 );
        keycodeArray->Remove( index - 1 );
        editPane->SetCursorIndexOfKeystroke( index - 1 );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::AddPhraseToDB
// Add phrase to DB.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::AddPhraseToDB( 
                                                     const TDesC& aPhraseAdd )
    {   
    TPtiUserDictionaryEntry addUdbEntry( aPhraseAdd );
    //Add the phrase to the DB by PTI Engine
    iOwner->PtiEngine()->AddUserDictionaryEntry( addUdbEntry );
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::RefreshCandidate
// Refresh candidate
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::RefreshCandidate()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable( ETrue );

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() && 
            uiContainer->CandidatePane()->IsFirstPage());
    uiContainer->ShowVerticalScrollArrows( multiplePages );

    uiContainer->CandidatePane()->SelectFirstPhrase();
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::CheckKeyNeedRepeat
//  Check key whether need repeat.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::CheckKeyNeedRepeat( TKeyPressLength aLength )
    {
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    if ( editPane->IsChangeState() && aLength == ELongKeyPress )
        {
        return EFalse;
        }
    else
        {
        editPane->SetChangeState( EFalse );
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::CommitInlineEEPL
// Commint text to EEP or editor
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::CommitInlineEEPL( const TDesC& aDes )
    {
    TInt charCount = aDes.Length();
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
	MAknFepUICtrlEditPane* editpane = UIContainer()->EditPaneWindow();
	CDesCArrayFlat* keyStoke = editpane->KeystrokeArray();
	RArray<TInt>* keycode = editpane->KeycodeArray();
	CDesCArrayFlat* phraseArray = editpane->PhraseArray();
	CDesCArrayFlat* phraseShowKeyStroke = editpane->PhraseShowKeyStrokeArray();
	RArray<TInt>* phraseKeycode = editpane->PhraseKeycodeArray();
	TBuf<KMaxKeystrokeCount> buf;
	TBuf<1> strokeDLT;
	strokeDLT.Append( KStrokeDelimiter );

	phraseArray->AppendL( aDes );

	TInt phraseCount = phraseArray->Count();
	TInt index = 0;
	TInt isFound = keyStoke->Find( strokeDLT, index );

	//phrase creation state
	if ( phraseCount > 1 || ( 0 == isFound && charCount <= 1 ))
		{
		editpane->SetPhraseCreationFlag( ETrue );
		if ( 0 == isFound && phraseCount < 7 && index < keyStoke->Count() - 1 )
			{
			for ( TInt i = 0; i <= index; i++ )
				{
				TPtrC ptr = keyStoke->MdcaPoint( 0 );
				buf.Append( ptr );
				keyStoke->Delete( 0);
				phraseKeycode->AppendL( (*keycode)[0]);
				keycode->Remove( 0 );
				}
			phraseShowKeyStroke->AppendL( buf );
			editpane->SetCursorIndexOfKeystroke( 0 );
			editpane->DisableCursor();			
			return ETrue;
			}
		else
			{
			TBuf<KMaxPhraseCreationCount> phraseCreated;
			for ( TInt ii = 0; ii < phraseCount; ++ii )
				{
				phraseCreated.Append( phraseArray->MdcaPoint(ii) );
				}
			if ( 0 != isFound || ( 0 == isFound && index == keyStoke->Count() - 1 ) )
				{
				editpane->ResetAllArray();
				}
			fepMan->NewTextL( phraseCreated );
			fepMan->CommitInlineEditL();
			AddPhraseToDB( phraseCreated );
			return EFalse;
			}
		}
	else
		{
		TBuf<KMaxPhraseCreationCount> phraseCreated;
		for ( TInt ii = 0; ii < phraseCount; ++ii )
			{
			phraseCreated.Append( phraseArray->MdcaPoint( ii ) );
			}
		editpane->ResetAllArray();
		editpane->SetPhraseCreationFlag( EFalse );
		fepMan->NewTextL( phraseCreated );
		fepMan->CommitInlineEditL();
		iOwner->PtiEngine()->SetPredictiveChineseChar( phraseCreated );
		return EFalse;
		}
	}

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::ClearPtiEngineKeystroke
// clear the ptiengine keystroke;
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::ClearPtiEngineKeystroke()
    {
    iOwner->PtiEngine()->ClearCurrentWord();
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::AddKeystrokeL
// Add keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::AddKeystrokeL( TInt aKey )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt keystrokeCount = keystrokeArray->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    
    if ( phraseCount + keystrokeCount >= KMaxKeystrokeCount )
        {
        return EFalse;
        }
    
    
    TBuf<1> keystroke;
    GetShowKeystroke( aKey, keystroke );
    if ( index >= keystrokeCount )
        {
        keystrokeArray->AppendL( keystroke );
        keycodeArray->AppendL( aKey );
        editPane->SetCursorIndexOfKeystroke( keystrokeCount + 1 );
        }
    else
        {
        keystrokeArray->InsertL( index, keystroke );
        keycodeArray->InsertL( aKey, index );
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::GetCandidateL
// Get the candidate info.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::GetCandidateL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* phraseCandidates = 
        uiContainer->CandidatePane()->CandidateArray();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    phraseCandidates->Reset();

    if ( keystrokeArray->Count() == 0 )
        {
        return;
        }
    
    if ( ( 0 == phraseCount ) && editPane->GetAllValidFlag() )
        {
        ptiengine->GetChinesePhraseCandidatesL( *phraseCandidates ); 
        return;
        }
    
    if ( !CheckFirstGroupStroke() )
        {
        return;
        }
    
    ptiengine->GetChinesePhraseCandidatesL( *phraseCandidates );    
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::GetShowKeystroke
// get the show keystroke.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::GetShowKeystroke(
                                                      TInt aKey,
                                                      TDes& aKeystroke
                                                          )
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBuf<KMaxName> lowerdata;   
    ptiEngine->MappingDataForKey((TPtiKey)aKey, lowerdata, EPtiCaseLower);  
    TInt StrokeUnicodePosition =0;
   
    if(lowerdata.Length()>0)
        {
        for(TInt i=0;i<lowerdata.Length()-1;i++)
            {
            if(lowerdata[i]==KStrokeUnicode)
                {
                StrokeUnicodePosition = i+1;
                break;
                }
            }
        if ( StrokeUnicodePosition < lowerdata.Length() )
            {
            aKeystroke.Copy( lowerdata.Mid( StrokeUnicodePosition, 1 ) );
            }
        }
    
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::RevertPhraseToKeystrokeL
// Revert the phrase to keystroke.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::RevertPhraseToKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    RArray<TInt>* phraseKeycodeArray = editPane->PhraseKeycodeArray();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    CDesCArrayFlat* phraseStroke = editPane->PhraseShowKeyStrokeArray();
    
    TBuf<KMaxKeystrokeCount> buf;
    
    if ( ( phrase->Count() == 0 ) || ( phraseStroke->Count() == 0 ) )
        {
        return;
        }
    
    buf.Append( phraseStroke->MdcaPoint( phraseStroke->Count() - 1 ) );
    
    for ( TInt i = buf.Length() - 1; i >= 0; --i )
        {
        keystrokeArray->InsertL( 0, buf.Mid( i, 1 ) );
        keycodeArray->Insert( (*phraseKeycodeArray)[phraseKeycodeArray->Count()-1], 0 );
        phraseKeycodeArray->Remove( phraseKeycodeArray->Count()-1 );
        }
    phrase->Delete( phrase->Count() - 1 );
    phraseStroke->Delete( phraseStroke->Count() - 1 );
    
    TInt leng = phrase->Count() + keystrokeArray->Count();
    if ( leng > KMaxKeystrokeCount )
        {
        for ( TInt i = 0; i < leng - KMaxKeystrokeCount; ++i )
            {
            if (  keystrokeArray->Count() == 0 )
                {
                break;
                }
            keystrokeArray->Delete( keystrokeArray->Count() - 1 );
            keycodeArray->Remove( keycodeArray->Count() - 1 );
            }
        }
    
    editPane->SetCursorIndexOfKeystroke( buf.Length() );
    keystrokeArray->Compress();
    phrase->Compress();
    phraseStroke->Compress();
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::SetWarningColor()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt keystrokeCount = keystrokeArray->Count();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TBool valid = ETrue;
    TInt index = 0;
    TInt count = 0;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt keyCode = 0;
    TInt invalidPos = 0;
    TBuf<1> delimiter;
    delimiter.Append( KStrokeDelimiter );
    
    //set warning color stroke after 7th group when it's more than 7 groups.
    if ( 0 == keystrokeCount )
        {
        return;
        }
    
    if ( CheckAllGroupStroke() )
        {
        return;
        }
    
    TInt delimiterCount = phraseCount + editPane->GetDelimiterCount();
    invalidPos = editPane->GetInvalidIndex();
    index = invalidPos;
    ClearPtiEngineKeystroke();
    for ( TInt i = invalidPos; i < keystrokeCount; ++i )
        {
        count = count + 1;
        //GetKeystrokeCode( keyCode, keystrokeArray->MdcaPoint( i ) );
        keyCode = (*keycodeArray)[i];
        if ( EPtiKeyQwertySpace == keyCode )
            {
            delimiterCount = delimiterCount + 1;
            if ( delimiterCount == KMaxPhraseCreationCount )
                {
                if ( !valid )
                    {
                    editPane->SetHighlight( 
                            phraseCount + index, 
                            phraseCount + keystrokeCount - 1 );
                    }
                else
                    {
                    editPane->SetHighlight( 
                            phraseCount + i, 
                            phraseCount + keystrokeCount - 1 );
                    }
                break;
                }
            if ( ( !valid ) || ( 0 == i ) )
                {
                editPane->SetHighlight( 
                        phraseCount + index, 
                        phraseCount + index + count - 1 );
                }
            else if ( i > 0 )
                {
                if ( 0 == keystrokeArray->MdcaPoint( i - 1 ).Compare( delimiter ) )
                    {
                    editPane->SetHighlight( 
                            phraseCount + index, 
                            phraseCount + index + count - 1 );
                    }
                }
            valid = ETrue;
            index = i + 1;
            count = 0;
            ClearPtiEngineKeystroke();
            }
        else if ( valid )
            {
            stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
            stringAfterLength = 
                ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
            if ( stringBeforeLength == stringAfterLength )
                {
                valid = EFalse;
                }  
            }
        
        if ( ( !valid ) && 
            ( keystrokeCount == i + 1 ) && 
            ( EPtiKeyQwertySpace != keyCode ) )
            {
            editPane->SetHighlight( 
                    phraseCount + index, 
                    phraseCount + index + count - 1 );
            }
        }
    
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::ChangeCbaL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TInt candidateCount = UIContainer()->CandidatePane()->CandidateArray()->Count();
    TInt keyStrokeCount = editPane->KeystrokeArray()->Count();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt currentCBAResId = editPane->GetCurrentCBAResID();
    if ( ( candidateCount == 0 && keyStrokeCount != 0 ) || 
        ( phraseCount == 1 && keyStrokeCount == 0 ) )
        {
        if (R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_EMPTY != currentCBAResId )
            {
            fepMan->UpdateCbaL( R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_EMPTY );
            editPane->SetCurrentCBAResID( R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_EMPTY );
            }

        }
    else if ( keyStrokeCount == 0 )
        {
        if ( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE != currentCBAResId )
            {
            fepMan->UpdateCbaL( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
            editPane->SetCurrentCBAResID( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
            }
        }
    else if ( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL != currentCBAResId )
        {
        fepMan->UpdateCbaL( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
        editPane->SetCurrentCBAResID( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::MoveCursorLeft
// Move the cursor to Left.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::MoveCursorLeft()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt keystrokeCount = editPane->KeystrokeArray()->Count();
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
        ShowInfoOnEEPPane();
        SetWarningColor();        
        }
    else if ( ( phraseCount > 0 ) && ( 1 == index ) )
        {
        editPane->SetCursorIndexOfKeystroke( 0 );
        ShowInfoOnEEPPane();
        SetWarningColor();    
        }
    else
        {
        editPane->MoveCursorLeft();
        editPane->SetCursorIndexOfKeystroke( index - 1 );
        }
    
    editPane->SetAutoChangeStateFlag( EManualChangeState );
    
    return ETrue;    
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::MoveCursorRight
// Move the cursor to Right.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::MoveCursorRight()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt keystrokeCount = editPane->KeystrokeArray()->Count();
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
        ShowInfoOnEEPPane();
        SetWarningColor(); 
        }
    else
        {
        editPane->MoveCursorRight();
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        }
    
    editPane->SetAutoChangeStateFlag( EManualChangeState );
    
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyFromCandidateL
// Handle key from candidate.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyFromCandidateL(
    TInt aKey, TKeyPressLength /*aLength*/ )
    {
    TInt index = 0;
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // it may be one of the 'valid' numbers..
    TPtiKeyboardType keyboardtype = iOwner->FepMan()->KeyboardLayout();
    if ( EPtiKeyboardQwerty4x10 == keyboardtype || EPtiKeyboardQwerty3x11
        == keyboardtype || EPtiKeyboardHalfQwerty == keyboardtype
        || EPtiKeyboardCustomQwerty == keyboardtype )
        {
        MPtiLanguage *lang= NULL;
        RArray < TPtiNumericKeyBinding > keybinding;
        TBool validnumkey = EFalse;
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        
        if( ptiengine != NULL )
            {
            lang = ptiengine->CurrentLanguage();
            }
        if ( ptiengine && lang )
            {
            TRAP_IGNORE(ptiengine->GetNumericModeKeysForQwertyL(lang->LanguageCode(), keybinding, keyboardtype));
            TInt numericKeysCount = keybinding.Count();
            while (numericKeysCount-- )
                {
                TPtiNumericKeyBinding numKeyBind =
                    keybinding[numericKeysCount];
                if ( (numKeyBind.iChar >= KKey1 && numKeyBind.iChar <= KKey6)
                    &&(aKey == numKeyBind.iKey) )
                    {
                    index = numKeyBind.iChar - EPtiKey0 -1;
                    if ( candidatePane->SelectIndex( index ) )
                        {
                        validnumkey = ETrue;
                        }
                    break;
                    }
                }
            }
        keybinding.Reset();
        keybinding.Close();
        if ( !validnumkey )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            return;
            }
        }
    else
        {
#endif
        index = MapKeyToIndex( aKey );
#ifdef RD_INTELLIGENT_TEXT_INPUT
        }
#endif
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    if ( text.Length( ) )
        {
        if ( !CommitInlineEEPL( text ) )
            {
            DoActionAfterCommit( );
            }
        else
            {
            if ( CheckFirstGroupStroke( ) )
                {
                RefreshUI( );
                UIContainer()->CandidatePane()->SelectFirstPhrase( );
                }
            else
                {
                UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EAutoChangeStateFromCandidate );
                iOwner->ChangeState( EMiniQwertyEdit );
                }
            }
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::CheckAllGroupStroke
// check the all group stroke of keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::CheckAllGroupStroke()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt keystrokeCount = keystroke->Count();
    TInt keyCode = 0;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt lastDLTPos = 0;
    TBool check = ETrue;
    TInt delimiterCount = 0;
    //If has phrase on the entry pane then the all keystroke is invalid.
    if ( ( phraseCount > 0 ) || ( 0 == keystrokeCount ) )
        {
        editPane->SetAllValidFlag( EFalse );
        editPane->SetDelimiterCount( 0 );
        editPane->SetInvalidIndex( 0 );
        return EFalse;
        }
    
    ClearPtiEngineKeystroke();
    for ( TInt i = 0; i < keystrokeCount; ++i )
        {
        //GetKeystrokeCode( keyCode, keystroke->MdcaPoint( i ) );
        keyCode = (*keycodeArray)[i];
        if ( EPtiKeyQwertySpace == keyCode )
            {
            //If first keystroke is delimiter then all keystroke is not valid.
            if ( i == 0 )
                {
                editPane->SetAllValidFlag( EFalse );
                editPane->SetDelimiterCount( 0 );
                editPane->SetInvalidIndex( 0 );
                return EFalse;
                }
            //If two delimiter is border upon then the check evaluate EFalse.
            else if ( lastDLTPos == ( i - 1 ) )
                {
                check = EFalse;
                }
            //lastDLTPos evaluate last delimiter that has not border upon deliminter. 
            else if ( check )
                {
                delimiterCount = delimiterCount + 1;
                lastDLTPos = i;
                }
            }
        stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
        stringAfterLength = 
            ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
        if ( stringBeforeLength == stringAfterLength )
            {
            editPane->SetAllValidFlag( EFalse );
            if ( 0 == lastDLTPos )
                {
                editPane->SetDelimiterCount( 0 );
                editPane->SetInvalidIndex( 0 );
                }
            else
                {
                editPane->SetDelimiterCount( delimiterCount );
                editPane->SetInvalidIndex( lastDLTPos + 1 );
                }            
            return EFalse;
            }          
        }
    
    editPane->SetAllValidFlag( ETrue );
    editPane->SetDelimiterCount( 0 );
    editPane->SetInvalidIndex( 0 );
    return ETrue;
    }


// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::DoActionAfterCommit
// Do action afer commit, change state to predictive or not.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::DoActionAfterCommit()
    {
    TRAP_IGNORE( DoActionAfterCommitL() ); 
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::DoActionAfterCommitL
// Do action afer commit, change state to predictive or not.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::DoActionAfterCommitL( )
    {
    if ( UIContainer()->EditPaneWindow()->KeystrokeArray()->Count( ) != 0 )
        {
        UIContainer()->EditPaneWindow()->ResetAllArray( );
        CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
        errnote->SetTone( CAknNoteDialog::EWarningTone );
        errnote->ExecuteLD( KMaxPhraseNote );
        }
    else if ( !UIContainer()->EditPaneWindow()->GetPhraseCreationFlag( ) )
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TBool isEditorFull = iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagEditorFull);
        if ( !isEditorFull )
            {
            iOwner->ChangeState( EPredictiveCandidate );
            UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
            }
        else
            {
            iOwner->FepMan()->ClearFlag(CAknFepManager::EFlagEditorFull);
            iOwner->FepMan()->TryCloseUiL();
            }
#endif
        }
    else
        {
        iOwner->FepMan()->TryCloseUiL( );
        }
    }
// ---------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::CheckSpellingDLT
// Check the delimiter of spelling.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyStrokePhraseBase::CheckSpellingDLT( const TDesC& aKeystroke )
    {
    if ( aKeystroke.Length() == 0 )
        {
        return EFalse;
        }
    if (  KStrokeDelimiter == aKeystroke[0]  )
        {
        return ETrue;
        }    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyStrokePhraseBase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyStrokePhraseBase::HandleCommandL( TInt aCommandId )
    {
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            HandleCommitL( EShortKeyPress );
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }
// End of file
