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
 *      Provides the TAknFepInputMiniQwertyZhuyinPhraseBase definition.
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
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"
#include "aknfepuiinputminiqwertyzhuyinphrasebase.h"
#include "aknfepinputstateminiqwertyzhuyinkeymap.h"

const TInt KMaxPhraseCount = 100;
const TInt KMaxBufLength = 150;
const TInt KMaxPhraseCreationCount = 7;
//KMaxPhraseNote means the max phrase is seven
_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::TAknFepInputMiniQwertyZhuyinPhraseBase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputMiniQwertyZhuyinPhraseBase::TAknFepInputMiniQwertyZhuyinPhraseBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer) :
    TAknFepInputStateCandidateQwertyBaseChinesePhrase( aOwner, aUIContainer )
    {
    if (iOwner->PtiEngine()->InputMode() != EPtiEngineZhuyinPhraseQwerty)
        {
        iOwner->PtiEngine()->SetInputMode( EPtiEngineZhuyinPhraseQwerty );
        }
    iOwner->PtiEngine()->SetCandidatePageLength( KMaxPhraseCount );
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutPhraseCreation );
    iOwner->PtiEngine()->SetCase(EPtiCaseLower);
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyZhuyinPhraseBase::HandleKeyL(
	                                             TInt /*aKey*/ ,
                                                 TKeyPressLength /*aLength*/ )
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::GetValidKeystroke
// Get the valid keystroke.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyZhuyinPhraseBase::GetValidKeystroke()
    {
    TBool result = ETrue;
    TRAPD( ret, result = GetValidKeystrokeL() );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    return result;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::RefreshEditPane
// Refresh editing pane.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::RefreshEditPane()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    
    TInt validCount = editPane->GetEffictiveLength();
    TInt underLineLen = 0;
    TBuf<KMaxBufLength> buf;
    
    for ( TInt i = 0; i < phrase->Count(); ++i )
        {
        buf.Append( phrase->MdcaPoint( i ) );
        }
    
    for ( TInt i = 0; i < keystroke->Count(); ++i )
        {
        buf.Append( keystroke->MdcaPoint( i ) );
        if ( i < validCount )
            {
            underLineLen = underLineLen + keystroke->MdcaPoint( i ).Length();
            }
        }
    editPane->SetCursorIndexOfKeystroke( validCount );
    editPane->SetText( buf, phrase->Count() + underLineLen, ETrue );
    if ( 0 == validCount )
        {
        editPane->SetHighlight( phrase->Count(), buf.Length() );
        }
    
    if ( EMiniQwertyEdit == iState )
        {
        editPane->ActiveCursor();
        editPane->EnableCursor();
        }
    else if ( EZhuyinSpelling == iState )
        {
        editPane->DeactiveCursor();
        editPane->EnableCursor();
        editPane->SetUnderline( phrase->Count(), phrase->Count() + underLineLen - 1 );
        }
    else if ( EZhuyinCandidate == iState )
        {
        editPane->DisableCursor();
        editPane->SetUnderline( phrase->Count(), phrase->Count() + underLineLen - 1 );
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::RefreshSpellingPane
// Refresh spelling pane.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::RefreshSpellingPane( TBool aClearSelection )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    TInt selection = 0;
    if ( !aClearSelection )
        {
        selection = popup->CurrentSelection();
        if ( selection >= spelling->Count() )
            {
            selection = 0;
            }
        }
    popup->SplitPhraseSpellingIntoPages( );
    if ( spelling->Count( ) == 0 )
        {
        uiContainer->SetControlInVisible( EFalse );
        }
    else
        {
        // set display page for delibrate selection
        inputPane->SetText( spelling->MdcaPoint( selection ) );
        popup->SetDisplayPage( selection );
        popup->SetPhraseItemTexts( );
        popup->PopupSizeChanged( );
        uiContainer->Enable( ETrue );  
        }

    if ( EMiniQwertyEdit == iState )
        {
        //popup need to add none high lighted interface.
        popup->SetNoneHighlighted();
        }
    popup->Enable( spelling->Count( ) > 0 );
    uiContainer->Enable( ETrue );    
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::RefreshSpellingPane
// Refresh spelling pane.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::RefreshCandidatePane( TBool aClearSelection )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt validCount = editPane->ShowKeystrokeArray()->Count();
    CDesCArrayFlat* candidateArray = 
        uiContainer->CandidatePane()->CandidateArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    // have valid keystroke.
    if ( validCount > 0 )
        {
        // set display page for delibrate selection
        ClearDeliberateSelection();
        GetCandidate( aClearSelection );
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->ShowVerticalScrollArrows( ETrue );
        uiContainer->Enable( ETrue );
        }
    else
        {
        candidateArray->Reset();
        if ( KMaxPhraseCreationCount == phraseCount )
            {
            TRAP_IGNORE( candidateArray->AppendL( KMaxPhraseNote ) );
            }
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( EFalse );
        }    
    uiContainer->Enable(ETrue);
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::UpdateIndicator
// Update Indicator.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::UpdateIndicator()
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

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::ClearDeliberateSelection
// clear selection.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::ClearDeliberateSelection()
    {
    // we must have just deleted the last character, 
    // or we are starting a new pinyin session, 
    // so wipe the last deliberate selection
    TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
    oldDeliberateSelection = KNullDesC;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::ClearPtiEngineKeystroke
// clear the ptiengine keystroke.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::ClearPtiEngineKeystroke()
    {
    iOwner->PtiEngine()->ClearCurrentWord();
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::GetCandidateL
// Get the candidate info.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::GetCandidate( TBool aClearSelection )
    {
    TRAP_IGNORE( GetCandidateL( aClearSelection ) );
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::ChangeCbaL()
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

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::CheckPhraseSpellingL
// Check the spelling whether is phrase spelling.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyZhuyinPhraseBase::CheckPhraseSpellingL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* spelling = 
        uiContainer->PinyinPopupWindow()->PhoneticSpellingArray();
    CDesCArrayFlat* tmpArray = editPane->TempArray();
    const TUint16 specZhuyin = 0x311D;
    _LIT( delimiter, "\x0027" );
    TInt validSpellingCount = 0;
    tmpArray->Reset();
    
    if ( 0 == ptiengine->GetPhoneticSpellingsL( *tmpArray ) )
        {
        return EFalse;
        }
    
    for ( TInt i = 0; i < tmpArray->Count(); ++i )
        {
        if ( ( KErrNotFound == tmpArray->MdcaPoint( i ).Find( delimiter ) ) &&
            ( specZhuyin != tmpArray->MdcaPoint( i )[0] ) )
            {
            validSpellingCount = validSpellingCount + 1;
            spelling->InsertL( 0, tmpArray->MdcaPoint( i ) );
            }
        }
    
    if ( 0 == validSpellingCount )
        {
        return EFalse;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::GetValidKeystroke
// Get the valid keystroke.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyZhuyinPhraseBase::GetValidKeystrokeL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    CDesCArrayFlat* spelling = 
        uiContainer->PinyinPopupWindow()->PhoneticSpellingArray();
    TInt keyCode = 0;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    
    spelling->Reset();
    spelling->Compress();
    showKeystroke->Reset();
    showKeystroke->Compress();
    
    editPane->SetEffictiveLength( 0 );
    if ( KMaxPhraseCreationCount == phraseCount )
        {
        if ( keystroke->Count() > 0 )
            {
            keystroke->Reset();
            }
        return EFalse;
        }
    
    if ( 0 == keystroke->Count() )
        {
        return EFalse;
        }
    
    if ( 1 == keystroke->MdcaPoint( 0 ).Length() )
        {
        return EFalse;
        }
   
    if ( CheckToneMarkBorderUpon() )
        {
        return EFalse;
        }
    
    ClearPtiEngineKeystroke();
    for ( TInt i = 0; ( i < keystroke->Count() ) && 
        ( i < keycodeArray->Count() ); ++i )
        {
        keyCode = (*keycodeArray)[i];
        if ( keyCode == EPtiKeyQwertyE || 
            keyCode == EPtiKeyQwertyR || 
            keyCode == EPtiKeyQwertyY || 
            keyCode == EPtiKeyQwertyU ||
            keyCode == EStdKeySpace )
            {
            if( ptiengine->SetToneMark( keyCode ) )
                {
                if ( !CheckPhraseSpellingL() )
                    {
                    break;
                    }
                else
                    {
                    showKeystroke->AppendL( keystroke->MdcaPoint( i - 1 ) );
                    showKeystroke->AppendL( keystroke->MdcaPoint( i ) );
                    }
                }
            break;
            }
        else
            {
            stringBeforeLength = 
                ptiengine->GetPhoneticSpelling(1).Length();
            stringAfterLength = 
                ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
            if ( stringBeforeLength != stringAfterLength )
                {
                if ( i + 1 < keystroke->Count() )
                    {
                    if ( 1 != keystroke->MdcaPoint( i + 1 ).Length() )
                        {
                        if ( !CheckPhraseSpellingL() )
                            {
                            break;
                            }
                        showKeystroke->AppendL( keystroke->MdcaPoint( i ) );
                        }
                    }
                else
                    {
                    if ( CheckPhraseSpellingL() )
                        {
                        showKeystroke->AppendL( keystroke->MdcaPoint( i ) );
                        }
                    }
                }
            else
                {
                break;
                }
            }
        }
    
    editPane->SetEffictiveLength( showKeystroke->Count() );
    if ( 0 == showKeystroke->Count() )
        {
        return EFalse;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::GetValidKeystroke
// Get the valid keystroke.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyZhuyinPhraseBase::CheckToneMarkBorderUpon()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    
    if ( keystroke->Count() > 1 )
        {
        for ( TInt i = 1; i < keystroke->Count(); ++i )
            {
            if ( ( 1 == keystroke->MdcaPoint( i - 1 ).Length() ) &&
                ( 1 == keystroke->MdcaPoint( i ).Length() ) )
                {
                return ETrue;
                }
            }
        }
    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::GetCandidateL
// Get candidate info.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyZhuyinPhraseBase::GetCandidateL( TBool aClearSelection )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    CDesCArrayFlat* candidateArray = candidatePane->CandidateArray();
    candidateArray->Reset();
    candidateArray->Compress();
    
    if ( 0 == spelling->Count() )
        {
        return;
        }
    
    TInt selection = 0;
    if ( !aClearSelection )
        {
        selection = popup->CurrentSelection();
        if ( selection >= spelling->Count() )
            {
            selection = 0;
            }
        }
    RPointerArray<HBufC> list;
    extern void CleanArray( TAny* aAny );
    TCleanupItem cleanitem( CleanArray, &list );
    CleanupStack::PushL( cleanitem );
    HBufC* retString = ptiengine->GetCandidatesByInputString( 
                                              spelling->MdcaPoint( selection ), 
                                              list,
                                              EFalse );
    
    delete retString;
    retString = NULL;
    
    for ( TInt i = 0; i < list.Count(); ++i )
        {
        if ( 1 == spelling->MdcaPoint( selection ).Length() )
            {
            if ( 0 != spelling->MdcaPoint( selection ).Compare( *(list[i]) ) )
                {
                candidateArray->AppendL( *(list[i]) );
                }
            }
        else
            {
            candidateArray->AppendL( *(list[i]) );
            }
        }
    
    CleanupStack::PopAndDestroy(); // cleanitem  
    }

// End of file
