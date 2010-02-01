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
 *      Provides the TAknFepInputStateEntryZhuyinPhraseCreation methods.
 *
*/











#include <PtiEngine.h>                          //CPtiEngine
#include <PtiUserDicEntry.h>

#include <PtiDefs.h>                            //keys
#include <avkon.rsg>
#include <AknFep.rsg>
#include <aknnotewrappers.h>

#include "AknFepPanic.h"
#include "aknfepuiinputstateentryzhuyinphrasecreation.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"		//Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepManager.h"
#include "aknfepzhuyinkeyhandlerinterface.h"
#include "AknFepZhuyinAnalyser.h"
#include "aknfepzhuyinconvertion.h"

const TInt KMaxPhraseCount = 50;

const TInt KMaxKeystrokeCount = 100;
const TInt KMaxPhraseCreationCount = 7;
const TInt KStateFromKeyDown = 0;
const TInt KStateAuto = 1;
// it means "the max phrase's length is seven"
_LIT( KDelimiter, "\x002E" );

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
TAknFepInputStateEntryZhuyinPhraseCreation::TAknFepInputStateEntryZhuyinPhraseCreation(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputStateZhuyinPhraseCreationBase( aOwner, aUIContainer)
    {
    iState = EZhuyinSequenceEdit;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane( EFalse );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutKeystroke );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->ShowVerticalScrollArrows( ETrue );
    uiContainer->ShowHorizontalScrollArrows( EFalse );
    uiContainer->InputPane()->SetOverrideFontId( 0 );

    iOwner->PtiEngine()->SetInputMode( EPtiEngineZhuyinByPhrase );
    iOwner->PtiEngine()->SetCandidatePageLength( KMaxPhraseCount );
    uiContainer->EditPaneWindow()->ActiveCursor(); 
    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    UpdateEEPContent();
    RefreshUI();
    // however we also need to clear the deliberate selection, in case we are not
    // coming back to the input pane from the candidate pane
    UpdateIndicator();
    TRAP_IGNORE( ChangeCbaL() );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryZhuyinPhraseCreation::HandleKeyL
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryZhuyinPhraseCreation::HandleKeyL( 
                                                     TInt aKey,
                                                     TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    CDesCArrayFlat* keystrokeArray = popup->ZhuyinSymbols();
    CDesCArrayFlat* ChineseArray = popup->ChineseCharsDisped();

    CDesCArrayFlat* candidateArray = 
        uiContainer->CandidatePane()->CandidateArray();
    TBuf<1> keyBuf;
    TBuf<1> keyShow;
    TBuf<KMaxPhraseCreationCount> phraseCreated;
    
    if ( aKey == EKeyBackspace )
        {
        DeleteZhuyinSymbolOrChinese();
        UpdateEEPContent();
        RefreshUI();
        if ( AutoChangeState() )
            {
            popup->SetChangeState( ETrue );
            iOwner->ZhuyinKeyHandler()->SetState( KStateAuto );
            iOwner->ZhuyinKeyHandler()->SetStatusChgFlag(MZhuyinKeyHandler::EInit);
            ChangeCbaL();
            iOwner->ChangeState( EEntry );
            return ETrue;
            }
        else
            {
            ChangeCbaL();
            }
        
        if ( keystrokeArray->Count() == 0
            && ChineseArray->Count() == 0 )
            {
            iOwner->FepMan()->TryCloseUiL();
            if (aLength == ELongKeyPress)
                {
                iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                }
            }
        
        if ( 0 == ChineseArray->Count() )
            {
            iOwner->FepMan()->PhraseCreation( EFalse );
            }
        }
    else if ( aKey == EKeyRightArrow )
        {
        if ( KMaxPhraseCreationCount == ChineseArray->Count() )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
            }
        else if ( 0 == keystrokeArray->Count() )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
            }
        else
            {
            if( !MoveIndexOfKeystrokeRight() )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
                }
            else
                {
                iOwner->ZhuyinKeyHandler()->SetState( KStateFromKeyDown );
                }
            }
        }
    else if ( aKey == EKeyLeftArrow )
        {
        if ( KMaxPhraseCreationCount == ChineseArray->Count() )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        else if ( 0 == keystrokeArray->Count() )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        else
            {
            if ( !MoveIndexOfKeystrokeLeft() )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            else
                {
                iOwner->ZhuyinKeyHandler()->SetState( KStateFromKeyDown );
                }
            }
        }
    else if ( aKey == EKeyOK || aKey == EKeyCBA1 )
        {
        if ( ( ChineseArray->Count() >= 2 ) && ( 0
            == keystrokeArray->Count() ) )
            {
            //commit the chinese character.
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            candidateArray->Reset();
            for (TInt i = 0; i < ChineseArray->Count(); ++i )
                {
                phraseCreated.Append( ChineseArray->MdcaPoint( i ) );
                }
            candidateArray->AppendL( ChineseArray->MdcaPoint( 
                ChineseArray->Count() - 1 ) );
            uiContainer->CandidatePane()->SelectFirst();
            uiContainer->CandidatePane()->SetCandidateBuffer( ChineseArray->MdcaPoint( 
                ChineseArray->Count() - 1 ) );
            AddPhraseToDB( phraseCreated );
            fepMan->NewTextL( phraseCreated );
            fepMan->CommitInlineEditL();
            iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
            }
        else if ( ( 1 == ChineseArray->Count() ) && 
            ( 0 == keystrokeArray->Count() ) )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        else if ( !CheckHighLightL() )
            {
            popup->SetChangeState( ETrue );
            iOwner->ZhuyinKeyHandler()->SetState( KStateFromKeyDown );
            iOwner->ZhuyinKeyHandler()->Reset();
            iOwner->ChangeState( EEntry );
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        }
    else if ( aKey == EKeyDownArrow )
        {
        if ( !CheckHighLightL() )
            {
            if ( 0 == keystrokeArray->Count() )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            else
                {
                popup->SetChangeState( ETrue );
                iOwner->ZhuyinKeyHandler()->SetState( KStateFromKeyDown );
                iOwner->ZhuyinKeyHandler()->SetStatusChgFlag(
                    MZhuyinKeyHandler::EFromSpellingEditingToEntry);
                iOwner->ZhuyinKeyHandler()->Reset();
                iOwner->ChangeState( EEntry );
                }
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        }
    else if ( aKey == EKeyUpArrow )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        }
    else if ( aLength == EShortKeyPress ) // don't want repeats on these keys
        {
        iOwner->FepMan()->SetCcpuFlag( CAknFepManager::ECcpuStateIgnoreStarUp );
        if ( iOwner->IsValidChineseInputKey( aKey ) )
            {
            if ( AddKeystrokeL( aKey ) )
                {
                UpdateEEPContent();
                if ( AutoChangeState() )
                    {
                    popup->SetChangeState( ETrue );
                    iOwner->ZhuyinKeyHandler()->SetState( KStateAuto );
                    RefreshUI();
                    ChangeCbaL();
                    iOwner->ChangeState( EEntry );
                    }
                else
                    {
                    RefreshUI();
                    ChangeCbaL();                
                    }
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            }
        else if ( aKey == EPtiKeyStar )
            {
            if ( AddToneMarkL() )
                {
                UpdateEEPContent();
                if ( AutoChangeState() )
                    {
                    popup->SetChangeState( ETrue );
                    iOwner->ZhuyinKeyHandler()->SetState( KStateAuto );
                    iOwner->ZhuyinKeyHandler()->Reset();
                    ChangeCbaL();
                    iOwner->ChangeState( EEntry );
                    }
                else
                    {
                    RefreshUI();
                    ChangeCbaL();                
                    }
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            }
        else if ( aKey == EKeyCBA2 )
            {
            iOwner->FepMan()->TryCloseUiL();
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------
// set high light
// ---------------------------------------------------------
//
void TAknFepInputStateEntryZhuyinPhraseCreation::SetHighLight()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt symbolsShowCount = popup->GetEEPContent().Length();
    TInt chineseCount = popup->ChineseCharsDisped()->Count();
    
    editPane->SetHighlight( chineseCount, symbolsShowCount );
    }

// ---------------------------------------------------------
// set high light
// ---------------------------------------------------------
//
void TAknFepInputStateEntryZhuyinPhraseCreation::SetHighLightMoreThanMax()
    {
    TInt begPos = BegPosOfMoreThanSeven();

    if ( begPos )
        {
        HighlightMoreThanSeven( begPos );
        }
    }

// ---------------------------------------------------------
// move index of the keystroke left.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryZhuyinPhraseCreation::MoveIndexOfKeystrokeLeft()
    {
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* zhuyinSymbol = popup->ZhuyinSymbols();
    CDesCArrayFlat* chineseArray = popup->ChineseCharsDisped();
    TInt currentIndex = editPane->GetCursorIndexOfKeystroke();
    TInt cursorPosition = editPane->GetCursor();

    TBool ret = EFalse;
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<KMaxKeystrokeCount> symbolBuf;

    if ( cursorPosition < chineseArray->Count() )
        {
        return EFalse;
        }
    
    for ( TInt i = 0; i < zhuyinSymbol->Count(); ++i )
        {
        symbolBuf.Append( zhuyinSymbol->MdcaPoint( i ) );
        }
    buf.Append( popup->GetEEPContent() );
    
    if ( chineseArray->Count() == cursorPosition )
        {
        TRAP_IGNORE( ret = CheckHighLightL() );
        if ( ret )
            {
            editPane->Reset();
            editPane->SetTextForZhuyin( buf, buf.Length(),
                chineseArray->Count(), symbolBuf, 
                zhuyinSymbol->Count(), ETrue );
            SetHighLight();
            }
        else
            {
            editPane->Reset();
            editPane->SetTextForZhuyin( buf, buf.Length(),
                chineseArray->Count(), symbolBuf, 
                zhuyinSymbol->Count() );
            SetHighLightMoreThanMax();
            }
        editPane->SetCursorIndexOfKeystroke( zhuyinSymbol->Count() );
        return ETrue;
        }

    if ( 1 == cursorPosition - chineseArray->Count() )
        {
        TRAP_IGNORE( ret = CheckHighLightL() );
        if ( ret )
            {
            editPane->Reset();
            editPane->SetTextForZhuyin( buf, chineseArray->Count(), 
                chineseArray->Count(), symbolBuf, zhuyinSymbol->Count(), ETrue );
            SetHighLight();
            }
        else
            {
            editPane->Reset();
            editPane->SetTextForZhuyin( buf, chineseArray->Count(), 
                chineseArray->Count(), symbolBuf, zhuyinSymbol->Count() );
            SetHighLightMoreThanMax();
            }
        editPane->SetCursorIndexOfKeystroke( 0 );
        return ETrue;
        }
    
    
    if ( 0 == buf.Mid( 
        (cursorPosition - 2 ), 1 ).Compare( KDelimiter ) )
        {
        editPane->MoveCursorLeft( 2 );
        editPane->SetCursorIndexOfKeystroke( currentIndex - 1 );
        }
    else
        {
        editPane->MoveCursorLeft();
        editPane->SetCursorIndexOfKeystroke( currentIndex - 1 );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// move index of the keystroke right.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryZhuyinPhraseCreation::MoveIndexOfKeystrokeRight()
    {
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* zhuyinSymbol = popup->ZhuyinSymbols();
    CDesCArrayFlat* chineseArray = popup->ChineseCharsDisped();
    TInt currentIndex = editPane->GetCursorIndexOfKeystroke();
    TInt cursorPosition = editPane->GetCursor();

    TBool ret = EFalse;
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<KMaxKeystrokeCount> symbolBuf;
    
    for ( TInt i = 0; i < zhuyinSymbol->Count(); ++i )
        {
        symbolBuf.Append( zhuyinSymbol->MdcaPoint( i ) );
        }
    buf.Append( popup->GetEEPContent() );
    
    if ( buf.Length() ==  cursorPosition )
        {
        TRAP_IGNORE( ret = CheckHighLightL() );
        if ( ret )
            {
            editPane->Reset();
            editPane->SetTextForZhuyin( buf, chineseArray->Count(), 
                chineseArray->Count(), symbolBuf, 0, ETrue );
            SetHighLight();
            }
        else
            {
            editPane->Reset();
            editPane->SetTextForZhuyin( buf, chineseArray->Count(), 
                chineseArray->Count(), symbolBuf, 0 );
            SetHighLightMoreThanMax();
            }
        editPane->SetCursorIndexOfKeystroke( 0 );
        return ETrue;
        }
    
    if ( chineseArray->Count() == cursorPosition )
        {
        editPane->MoveCursorRight();
        editPane->SetCursorIndexOfKeystroke( 1 );
        return ETrue;
        }
    
    if ( buf.Length() == cursorPosition + 1 )
        {
        editPane->MoveCursorRight();
        editPane->SetCursorIndexOfKeystroke( zhuyinSymbol->Count() );
        return ETrue;
        }
    
    if ( 0 == buf.Mid( ( cursorPosition ), 1 ).Compare( KDelimiter ) )
        {
        editPane->MoveCursorRight( 2 );
        editPane->SetCursorIndexOfKeystroke( currentIndex + 1 );
        }
    else
        {
        editPane->MoveCursorRight();
        editPane->SetCursorIndexOfKeystroke( currentIndex + 1 );
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
// Initialize State.
// ---------------------------------------------------------
//
void TAknFepInputStateEntryZhuyinPhraseCreation::InitializeState()
    {
    TRAP_IGNORE( ChangeCbaL() );
    }

// ---------------------------------------------------------
// check auto change state.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryZhuyinPhraseCreation::AutoChangeState()
    {
    TInt keystrokeCount = 
        UIContainer()->PinyinPopupWindow()->ZhuyinSymbols()->Count();
    TBool ret = EFalse;
    
    TRAP_IGNORE( ret = CheckHighLightL() );

    if ( ( !ret ) &&
        ( KStateFromKeyDown != iOwner->ZhuyinKeyHandler()->GetState() ) &&
        ( 0 != keystrokeCount ) )
        {
        return ETrue;
        }    
    
    return EFalse;
    }

// ---------------------------------------------------------
// Add phrase to DB.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryZhuyinPhraseCreation::AddPhraseToDB( const TDesC& aPhraseAdd )
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
// Check high light.
// ---------------------------------------------------------
//
TBool TAknFepInputStateEntryZhuyinPhraseCreation::CheckHighLightL()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* zhuyinSymbol = popup->ZhuyinSymbols();
    CDesCArrayFlat* candidateArray = 
        UIContainer()->CandidatePane()->CandidateArray();
    TBuf<KMaxKeystrokeCount> inputBuf;
    
    inputBuf.Zero();
    
    if ( 0 == candidateArray->Count() )
        {
        return ETrue;
        }
    
    for ( TInt i = 0; i < zhuyinSymbol->Count(); ++i )
        {
        inputBuf.Append( zhuyinSymbol->MdcaPoint( i ) );
        }
    
    if ( 0 == inputBuf.Length() )
        {
        return EFalse;
        }
    
    if ( !iOwner->ZhuyinAnalyser()->AnalyzeL( inputBuf ) )
        {
        return ETrue;
        }
    
    return EFalse;
    }

// End of file
