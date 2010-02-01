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
#include <PtiEngine.h>
#include <avkon.rsg>
#include <AknFep.rsg>
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote 

// User includes
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"
#include "aknfepuiinputstatestrokephrasebase.h"

// Constant definition
const TInt KMinCnadidateCount = 1;
const TInt KMaxPhraseCount = 50;
const TInt KEnginePhoneticIndex = 1;
const TInt KMaxPhraseCreationCount = 7;
const TInt KMaxKeystrokeCount = 31;
const TInt KKey0Code = 48;
const TInt KKey1Code = 49;
const TInt KKey2Code = 50;
const TInt KKey3Code = 51;
const TInt KKey4Code = 52;
const TInt KKey5Code = 53;
const TInt KKey6Code = 54;
const TInt KAutoChangeState = 1;
const TInt KManualChangeState = 0; 
const TUint16 KDelimiter = 0x2022;
const TUint16 KStrokeKey1 = 0x4E00;
const TUint16 KStrokeKey2 = 0x4E28;
const TUint16 KStrokeKey3 = 0x4E3F;
const TUint16 KStrokeKey4 = 0x4E36;
const TUint16 KStrokeKey5 = 0x4E5B;
const TUint16 KStrokeKey6 = 0x003F;

const TUint16 KSegment = 0x0020;

_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );

const TKeyMap StrokeMap[] = 
    {
    {KDelimiter, KKey0Code},
    {KStrokeKey1, KKey1Code},
    {KStrokeKey2, KKey2Code},
    {KStrokeKey3, KKey3Code},
    {KStrokeKey4, KKey4Code},
    {KStrokeKey5, KKey5Code},
    {KStrokeKey6, KKey6Code}
    };

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseCreationBase::TAknFepInputStateStrokePhraseCreationBase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateStrokePhraseBase::TAknFepInputStateStrokePhraseBase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    if ( iOwner->PtiEngine()->InputMode() != EPtiEngineStrokeByPhrase )
        {
        iOwner->PtiEngine()->SetInputMode( EPtiEngineStrokeByPhrase );
        }
    iOwner->PtiEngine()->SetCandidatePageLength( KMaxPhraseCount );
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->SetFepMan( iOwner->FepMan() );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateStrokePhraseBase::HandleKeyL( 
                                        TInt aKey, 
                                        TKeyPressLength aLength )
    {    
    switch( aKey )
        {
        case EKeyBackspace:
            {
            if ( CheckKeyNeedRepeat( aLength ) )
                {
                HandleKeyBackspaceL( aLength );
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            break;
            }
        case EKeyUpArrow:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            break;
            }
        case EKeyDownArrow:
            {
            if ( EStrokeSequenceEdit == iState )
                {
                if ( CheckFirstGroupStroke() )
                    {
                    UIContainer()->PinyinPopupWindow()->SetChangeState( ETrue );
                    iOwner->ChangeState( EEntry );
                    }
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            break;
            }
        case EKeyLeftArrow:
            {
            if ( EStrokeSequenceEdit == iState )
                {
                MoveCursorLeft();
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            break;
            }    
        case EKeyRightArrow:
            {
            if ( EStrokeSequenceEdit == iState )
                {
                MoveCursorRight();
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            break;
            }
        case EKeyOK:
        case EKeyCBA1:
            {
            if ( CheckKeyNeedRepeat( aLength ) )
                {
                HandleKeyCBA1OrKeyOKL( aLength );
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        case EKeyCBA2:
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        default:
            {
            if ( ( EShortKeyPress == aLength ) && 
                ( iOwner->IsValidChineseInputKey( aKey ) ) )
                {
                HandleDefaultKeyL( aKey );
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        }
    
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::RefreshUI
// Update input window.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::RefreshUI( TBool aRefresh )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    // get cursor position
    if ( aRefresh )
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
    
    ShowInfoOnEEPPane();
    if ( !CheckAllKeystroke() )
        {
        SetWarningColor();
        }
    ShowInfoOnCandidatePane();
    UpdateIndicator();
    TRAP_IGNORE( ChangeCbaL() );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleHorizontalNavigation
// Handle horizontal navigation.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateStrokePhraseBase::CheckFirstGroupStroke()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    CDesCArrayFlat* keystroke = uiContainer->
        PinyinPopupWindow()->KeystrokeArrayForStroke();
    
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt keyCode = 0;
    TInt pos = 0;
    if ( 0 == keystroke->Count() )
        {
        return EFalse;
        }
    
    ClearPtiEngineKeystroke();
    TBuf<1> delimiter;
    delimiter.Append( KDelimiter );
    if ( 0 == keystroke->Find( delimiter, pos ) )
        {
        if ( 0 == pos )
            {
            return EFalse;
            }
        }
    else
        {
        pos = keystroke->Count();
        }
    
    for ( TInt i = 0; i < pos; ++i )
        {
        GetKeystrokeCode( keyCode, keystroke->MdcaPoint( i ) );
        stringBeforeLength = 
            ptiengine->GetPhoneticSpelling(KEnginePhoneticIndex).Length();
        stringAfterLength = 
            ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
        //the keystroke is invalid.
        if ( stringBeforeLength == stringAfterLength )
            {
            ClearPtiEngineKeystroke();
            return EFalse;
            }
        } 
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleKeyBackspaceL
// Handle KeyBackspace event.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::HandleKeyBackspaceL(
                                                    TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* keystroke = popup->KeystrokeArrayForStroke();
    TInt phraseCount = popup->ChooseChineseCharacterArrayForStroke()->Count();
    TBool firstGroup = CheckFirstGroupStroke();
    
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
    
    if ( popup->ChooseChineseCharacterArrayForStroke()->Count() == 0 )
        {
        iOwner->FepMan()->PhraseCreation( EFalse );
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
    
    //Auto change from EStrokeSequenceEdit state to EEntry state. 
    if ( EStrokeSequenceEdit == iState )
        {
        if ( ( KAutoChangeState == popup->GetState() ) && 
              ( !firstGroup ) && CheckFirstGroupStroke() )
            {
            popup->SetChangeState( EFalse );
            iOwner->ChangeState( EEntry );
            return;
            }
        }
    
    //Auto change from EEntry state to EStrokeSequenceEdit state.
    if ( EEntry == iState )
        {
        if ( ( firstGroup ) && ( !CheckFirstGroupStroke() ) )
            {
            popup->SetChangeState( EFalse );
            popup->SetState( KAutoChangeState );
            iOwner->ChangeState( EStrokeSequenceEdit );
            return;
            }
        }
    
    RefreshUI();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleKeyCBA1OrKeyOKEntryL
// Entry handle CBA1 or OK key press event.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::HandleKeyCBA1OrKeyOKEntry(
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
        iOwner->ChangeState( ECandidate );        
        }    
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleKeyCBA1OrKeyOKEditL
// Edit state handle CBA1 or OK key press event.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::HandleKeyCBA1OrKeyOKEditL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* phrase = popup->ChooseChineseCharacterArrayForStroke();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    TBuf<KMaxPhraseCreationCount> phraseCreated;
    if ( (phrase->Count() >= 2 ) && ( 0 == keystrokeArray->Count() ) )
        {
        //commit the chinese character.
        for (TInt i = 0; i < phrase->Count(); ++i )
            {
            phraseCreated.Append( phrase->MdcaPoint( i ) );
            }
        AddPhraseToDB( phraseCreated );
        iOwner->FepMan()->NewTextL( phraseCreated );
        iOwner->FepMan()->CommitInlineEditL();
        iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
        }
    else if ( CheckFirstGroupStroke() )
        {
        popup->SetState( KManualChangeState );
        popup->SetChangeState( ETrue );
        iOwner->ChangeState( EEntry );
        }
    else
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        }
    
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleKeyCBA1OrKeyOKL
// Handle CBA1 or OK key press event.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::HandleKeyCBA1OrKeyOKL(
                                                    TKeyPressLength aLength )
    {
    if ( EStrokeSequenceEdit == iState )
        {
        HandleKeyCBA1OrKeyOKEditL();
        }
    else if ( EEntry == iState )
        {
        HandleKeyCBA1OrKeyOKEntry( aLength );
        }
    else
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleDefaultKeyL
// Handle default key press event.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::HandleDefaultKeyL( TInt aKey )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    TBool firstGroup = CheckFirstGroupStroke();
    
    if ( !AddKeystrokeL( aKey ) )
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return;
        }
    
    //Auto change from EStrokeSequenceEdit state to EEntry state. 
    if ( EStrokeSequenceEdit == iState )
        {
        if ( ( KAutoChangeState == popup->GetState() ) && 
              ( !firstGroup ) && CheckFirstGroupStroke() )
            {
            popup->SetChangeState( ETrue );
            iOwner->ChangeState( EEntry );
            return;
            }
        }
    
    //Auto change from EEntry state to EStrokeSequenceEdit state.
    if ( EEntry == iState )
        {
        if ( ( firstGroup ) && ( !CheckFirstGroupStroke() ) )
            {
            popup->SetState( KAutoChangeState );
            popup->SetChangeState( ETrue );
            iOwner->ChangeState( EStrokeSequenceEdit );
            return;
            }
        }
    
    RefreshUI();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::ShowInfoOnEEP
// Show the showinfo on the EEP ctrl.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::ShowInfoOnEEPPane()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = popup->KeystrokeArrayForStroke();
    CDesCArrayFlat* phrase = popup->ChooseChineseCharacterArrayForStroke();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TBuf<KMaxKeystrokeCount> showInfo;
    
    for (TInt i = 0; i < phrase->Count(); ++i )
        {
        showInfo.Append( phrase->MdcaPoint( i ) );
        }

    for (TInt ii = 0; ii < keystroke->Count(); ++ii )
        {
        showInfo.Append( keystroke->MdcaPoint( ii ) );
        }
    
    editPane->SetText( showInfo, phrase->Count() + index );
    if ( EStrokeSequenceEdit == iState )
        {
        editPane->ActiveCursor();
        }
    else if ( EEntry == iState )
        {
        editPane->DeactiveCursor();
        }
    else
        {
        editPane->DisableCursor();
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::ShowInfoOnEEP
// Show the showinfo on the EEP ctrl.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::ShowInfoOnCandidatePane()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->SetControlInVisible( ETrue );
    CDesCArrayFlat* phrase = uiContainer->CandidatePane()->CandidateArray();
    if ( KMaxPhraseCreationCount == uiContainer->PinyinPopupWindow()->
            ChooseChineseCharacterArrayForStroke()->Count() )
        {
        phrase->Reset();
        TRAP_IGNORE( phrase->AppendL( KMaxPhraseNote ) );
        uiContainer->SetControlInVisible( ETrue );
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
        uiContainer->SetControlInVisible( ETrue );
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowVerticalScrollArrows( ETrue );
        }
    else
        {
        uiContainer->SetControlInVisible( EFalse );
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( EFalse );
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::GetShowKeystroke
//Handle horizontal navigation.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::GetShowKeystroke(
                                                        TInt aKey, 
                                                        TDes& aKeystroke )
    {
    TInt count = sizeof( StrokeMap )/sizeof( StrokeMap[0] );
    for ( TInt i = 0; i < count ; i++ )
        {
        if (aKey == StrokeMap[i].iKeyCode)
            {
            aKeystroke.Append(StrokeMap[i].iValue);
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::HandleHorizontalNavigation
//Handle horizontal navigation.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateStrokePhraseBase::HandleHorizontalNavigation(
                                                                   TInt aKey )
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EKeyLeftArrow)
        {        
        if(!candidatePane->SelectPrev())
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else if (aKey == EKeyRightArrow)
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
// TAknFepInputStateStrokePhraseBase::UpdateIndicator
// Update Indicator.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    
    if ( candidatePane->IsFirstPage() )
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows( ETrue );
        }

    if ( candidatePane->IsLastPage() )
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows( ETrue );
        }
    }      

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::DeleteKeystroke
// Delete current keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStateStrokePhraseBase::DeleteKeystroke()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    
    if ( ( 0 == index ) || ( keystrokeArray->Count() == 0 ) )
        {
        return EFalse;
        }
    
    if ( 1 == index )
        {
        keystrokeArray->Delete( 0 );
        editPane->SetCursorIndexOfKeystroke( 0 );
        editPane->Reset();
        }
    else if ( index >= keystrokeArray->Count() )
        {
        keystrokeArray->Delete( keystrokeArray->Count() - 1 );
        editPane->SetCursorIndexOfKeystroke( keystrokeArray->Count() );
        }
    else
        {
        keystrokeArray->Delete( index - 1 );
        editPane->SetCursorIndexOfKeystroke( index - 1 );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::AddPhraseToDB
// Add phrase to DB.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::AddPhraseToDB( 
                                                     const TDesC& aPhraseAdd )
    {   
    TPtiUserDictionaryEntry addUdbEntry( aPhraseAdd );
    //Add the phrase to the DB by PTI Engine
    iOwner->PtiEngine()->AddUserDictionaryEntry( addUdbEntry );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::RefreshCandidate
// Refresh candidate
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::RefreshCandidate()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable( ETrue );

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() && 
            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showArrows = multiplePages || bufLength > KMinCnadidateCount;

    uiContainer->ShowHorizontalScrollArrows( showArrows );
    uiContainer->ShowVerticalScrollArrows( multiplePages );

    uiContainer->CandidatePane()->SelectFirstPhrase();
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::CheckKeyNeedRepeat
//  Check key whether need repeat.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateStrokePhraseBase::CheckKeyNeedRepeat( TKeyPressLength aLength )
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    if ( popup->IsChangeStatet() && aLength == ELongKeyPress )
        {
        return EFalse;
        }
    else
        {
        popup->SetChangeState( EFalse );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::ClearPtiEngineKeystroke
// clear the ptiengine keystroke;
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::ClearPtiEngineKeystroke()
    {
    iOwner->PtiEngine()->ClearCurrentWord();
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::AddKeystrokeL
// Add keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStateStrokePhraseBase::AddKeystrokeL( TInt aKey )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    TInt phraseCount = popup->ChooseChineseCharacterArrayForStroke()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    
    if ( phraseCount + keystrokeArray->Count() >= KMaxKeystrokeCount )
        {
        return EFalse;
        }
    
    TBuf<1> keystroke;
    GetShowKeystroke( aKey, keystroke );
    if ( index >= keystrokeArray->Count() )
        {
        keystrokeArray->AppendL( keystroke );
        editPane->SetCursorIndexOfKeystroke( keystrokeArray->Count() );
        }
    else
        {
        keystrokeArray->InsertL( index, keystroke );
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        }
    return ETrue;
    }
// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::GetCandidateL
// Get the candidate info.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::GetCandidateL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* phraseCandidates = 
        uiContainer->CandidatePane()->CandidateArray();
    CDesCArrayFlat* keystroke = popup->KeystrokeArrayForStroke();
    phraseCandidates->Reset();
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<1> delimiter;
    delimiter.Append( KDelimiter );
    if ( !CheckFirstGroupStroke() )
        {
        return;
        }
    else if ( CheckAllKeystroke() )
        {
        for( TInt i = 0; i < keystroke->Count(); ++i )
            {
            buf.Append( keystroke->MdcaPoint( i ) );
            }
        }
    else
        {
        TInt pos = 0;
        if ( 0 == keystroke->Find( delimiter,pos ) )
            {
            for( TInt i = 0; i < pos; ++i )
                {
                buf.Append( keystroke->MdcaPoint( i ) );
                }
            }
        else
            {
            for( TInt i = 0; i < keystroke->Count(); ++i )
                {
                buf.Append( keystroke->MdcaPoint( i ) );
                }
            }
        }
    extern void CleanArray( TAny* aAny );
    RPointerArray<HBufC> list;
    TCleanupItem cleanitem( CleanArray, &list );
    CleanupStack::PushL( cleanitem );
    HBufC* retString = ptiengine->GetCandidatesByInputString( 
                                              buf, 
                                              list,
                                              EFalse );
    
    delete retString;
    retString = NULL;
    
    for ( TInt i = 0; i < list.Count(); ++i )
        {
        phraseCandidates->AppendL( *(list[i]) );
        }
    CleanupStack::PopAndDestroy();   
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::GetKeystrokeCode
// get Code of the key stroke. 
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::GetKeystrokeCode(
                                                     TInt& aKey, 
                                                     const TDesC& aKeystroke )
    {
    TInt count = sizeof( StrokeMap )/sizeof( StrokeMap[0] );
    for ( TInt i = 0; i < count ; i++ )
        {
        TBuf<1> buf;
        buf.Append(StrokeMap[i].iValue);
        if (0 == aKeystroke.Compare(buf))
            {
            aKey = StrokeMap[i].iKeyCode;
            break;
            }
        }
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::CheckAllKeystroke
// Get the candidate info.
// ---------------------------------------------------------
//
TBool TAknFepInputStateStrokePhraseBase::CheckAllKeystroke()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    CDesCArrayFlat* keystrokeArray = uiContainer->
        PinyinPopupWindow()->KeystrokeArrayForStroke();
    
    TInt chineseCount = uiContainer->PinyinPopupWindow()->
        ChooseChineseCharacterArrayForStroke()->Count();
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt keyCode = 0;
    TInt delimiterCount = 0; 
    TBuf<1> delimiter;
    delimiter.Append( KDelimiter );
    if ( chineseCount > 0 )
        {
        return EFalse;
        }
    
    if ( keystrokeArray->Count() > 0 )
        {
        if ( 0 == keystrokeArray->MdcaPoint( 0 ).Compare( delimiter ) )
            {
            return EFalse;
            }
        }
    ClearPtiEngineKeystroke();
    for ( TInt i = 0; i < keystrokeArray->Count(); ++i )
        {
        if ( 0 == keystrokeArray->MdcaPoint( i ).Compare( delimiter ) )
            {
            delimiterCount = delimiterCount + 1;
            }
        GetKeystrokeCode( keyCode, keystrokeArray->MdcaPoint( i ) );
        stringBeforeLength = 
            ptiengine->GetPhoneticSpelling(KEnginePhoneticIndex).Length();
        stringAfterLength = 
            ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();

        if ( stringBeforeLength == stringAfterLength )
            {
            ClearPtiEngineKeystroke();
            return EFalse;
            }
        }
    if ( delimiterCount >= KMaxPhraseCreationCount )
        {
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::RevertPhraseToKeystrokeL
// Revert the phrase to keystroke.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::RevertPhraseToKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    CDesCArrayFlat* phrase = popup->ChooseChineseCharacterArrayForStroke();
    CDesCArrayFlat* phraseStroke = 
        popup->ChooseChineseCharacterArrayKeystrokeForStroke();

    TBuf<KMaxKeystrokeCount> buf;
    if ( ( phrase->Count() == 0 ) || ( phraseStroke->Count() == 0 ) )
        {
        return;
        }
    
    buf.Append( phraseStroke->MdcaPoint( phraseStroke->Count() - 1 ) );
    
    for ( TInt i = buf.Length() - 1; i >= 0; --i )
        {
        keystrokeArray->InsertL( 0, buf.Mid( i, 1 ) );
        }
    phrase->Delete( phrase->Count() - 1 );
    phraseStroke->Delete( phraseStroke->Count() - 1 );
    
    TInt leng = phrase->Count() + keystrokeArray->Count();
    if ( leng > KMaxKeystrokeCount )
        {
        for ( TInt i = 0; i < leng - KMaxKeystrokeCount; ++i )
            {
            keystrokeArray->Delete( keystrokeArray->Count() - 1 );
            }
        }
    editPane->SetCursorIndexOfKeystroke( buf.Length() );
    keystrokeArray->Compress();
    phrase->Compress();
    phraseStroke->Compress();
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::SetWarningColor()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CDesCArrayFlat* keystrokeArray = UIContainer()->
        PinyinPopupWindow()->KeystrokeArrayForStroke();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TInt phraseCount = UIContainer()->PinyinPopupWindow()->
        ChooseChineseCharacterArrayForStroke()->Count();

    TInt indexStart = phraseCount;
    TInt keyCode = 0;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt keystrokeCount = 0;
    TInt validKeystrokeCountOfPti = 0;
    TBool valid = ETrue;
    TInt delimiterCount = 0;
    TInt delimiterADD = 0;
    TBuf<1> delimiter;
    delimiter.Append( KDelimiter );
    ClearPtiEngineKeystroke();

    for ( TInt i = 0; i < keystrokeArray->Count(); ++i )
        {
        keystrokeCount = keystrokeCount + 1;
        if ( phraseCount + delimiterCount >= KMaxPhraseCreationCount )
            {
            break;
            }
        if ( 0 == keystrokeArray->MdcaPoint( i ).Compare( delimiter ) )
            {
            delimiterADD = i;
            delimiterCount = delimiterCount + 1;
            }
        GetKeystrokeCode( keyCode, keystrokeArray->MdcaPoint( i ) );
        stringBeforeLength = ptiengine->GetPhoneticSpelling(
            KEnginePhoneticIndex).Length();
        stringAfterLength = 
            ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();

        if ( stringBeforeLength == stringAfterLength )
            {
            valid = EFalse;
            }
        else if ( valid )
            {
            validKeystrokeCountOfPti = validKeystrokeCountOfPti + 1;
            }

        if ( ( i == keystrokeArray->Count() - 1 ) && ( KKey0Code != keyCode )
            && ( 1 != keystrokeCount ) && ( !valid ) )
            {
            editPane->SetHighlight( indexStart, indexStart + keystrokeCount - 1 );
            validKeystrokeCountOfPti = 0;
            keystrokeCount = 0;
            valid = ETrue;
            }

        if ( KKey0Code == keyCode )
            {
            ClearPtiEngineKeystroke();
            if ( 0 == i )
                {
                editPane->SetHighlight( indexStart, indexStart
                    + keystrokeCount - 1 );
                indexStart = phraseCount + i + 1;
                validKeystrokeCountOfPti = 0;
                keystrokeCount = 0;
                valid = ETrue;
                }
            else
                {
                if ( ( 0 == keystrokeArray->MdcaPoint( i ).
                Compare( delimiter ) ) && ( 0
                    == keystrokeArray->MdcaPoint( i - 1 ).
                    Compare( delimiter ) ) )
                    {
                    editPane->SetHighlight( indexStart, indexStart
                        + keystrokeCount - 1 );
                    indexStart = phraseCount + i + 1;
                    validKeystrokeCountOfPti = 0;
                    keystrokeCount = 0;
                    valid = ETrue;
                    }
                else
                    {
                    if (  ( !valid ) && ( 1 != keystrokeCount ) && 
                        ( validKeystrokeCountOfPti < keystrokeCount - 1 ) )
                        {
                        editPane->SetHighlight( indexStart, indexStart
                            + keystrokeCount - 1 );
                        indexStart = phraseCount + i + 1;
                        validKeystrokeCountOfPti = 0;
                        keystrokeCount = 0;
                        valid = ETrue;
                        }
                    else
                        {
                        indexStart = phraseCount + i + 1;
                        validKeystrokeCountOfPti = 0;
                        keystrokeCount = 0;
                        valid = ETrue;
                        }
                    }
                }
            }
        }
    
    if ( phraseCount + delimiterCount >= KMaxPhraseCreationCount )
        {
        editPane->SetHighlight( 
                phraseCount + delimiterADD, 
                phraseCount + keystrokeArray->Count() );
        }

    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::ChangeCbaL()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    TInt keystrokeCount = popup->KeystrokeArrayForStroke()->Count();
    TInt phraseCount = popup->ChooseChineseCharacterArrayForStroke()->Count();
    TInt lastRes = popup->GetLastResouce();
    
    TBool first = CheckFirstGroupStroke();
    CheckAllKeystroke();
    if ( phraseCount == KMaxPhraseCreationCount )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE )
            {
            return;
            }
        //User cba done-done-cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        }
    else if ( 0 != keystrokeCount && first )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT )
            {
            return;
            }
        //User cba select-select-cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
        }
    else if ( phraseCount > 1 && 0 == keystrokeCount )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE )
            {
            return;
            }
        //User cba done-done-cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        }
    else
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS__PHRASE_CREATION_EMPTY_CANCEL_EMPTY )
            {
            return;
            }
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS__PHRASE_CREATION_EMPTY_CANCEL_EMPTY );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS__PHRASE_CREATION_EMPTY_CANCEL_EMPTY );
        }
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::MoveCursorLeft
// Move the cursor to Left.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::MoveCursorLeft()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TInt keystrokeCount = popup->KeystrokeArrayForStroke()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    if ( 0 == keystrokeCount )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return;
        }
    else
        {
        popup->SetState( KManualChangeState );
        }
    
    if ( index > 1 )
        {
        editPane->SetCursorIndexOfKeystroke( index - 1 );
        editPane->MoveCursorLeft();
        }
    else
        {
        if ( 1 == index )
            {
            editPane->SetCursorIndexOfKeystroke( 0 );
            }
        else
            {
            editPane->SetCursorIndexOfKeystroke( keystrokeCount );
            }
        ShowInfoOnEEPPane();
        if ( !CheckAllKeystroke() )
            {
            SetWarningColor();
            }
        }
    }

// ---------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::MoveCursorRight
// Move the cursor to Right.
// ---------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::MoveCursorRight()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TInt keystrokeCount = popup->KeystrokeArrayForStroke()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    if ( 0 == keystrokeCount )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return;
        }
    else
        {
        popup->SetState( KManualChangeState );
        }
    
    if ( index < keystrokeCount )
        {
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        editPane->MoveCursorRight();
        }
    else
        {
        editPane->SetCursorIndexOfKeystroke( 0 );
        ShowInfoOnEEPPane();
        if ( !CheckAllKeystroke() )
            {
            SetWarningColor();
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseBase::GetNewCandidatesL 
// Get new candidate
// ---------------------------------------------------------------------------
//
void TAknFepInputStateStrokePhraseBase::GetNewCandidatesL(const TDesC& aSource, 
                                                    RPointerArray<HBufC>& aList)
    {
    TInt start = 0;
    TInt length = 0;

    for (TInt ii = 0; ii < aSource.Length(); ii++)
        {
        if (aSource[ii] == KSegment)
            {
            TPtrC segment(aSource.Ptr() + start, length);
            aList.AppendL(segment.AllocL());
            start += (length + 1);
            length = 0;
            }
        else
            {
            length++;
            }
        }

    if (length)
        {
        TPtrC segm(aSource.Ptr() + start, length);
        aList.AppendL(segm.AllocL());
        }
    }

// End of file
