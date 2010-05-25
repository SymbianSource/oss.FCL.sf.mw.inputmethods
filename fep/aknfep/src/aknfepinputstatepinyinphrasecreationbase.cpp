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
#include <avkon.rsg>
#include <aknfep.rsg>

#include "aknfepinputstatepinyinphrasecreationbase.h"
#include "AknFepPanic.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlPinyinPopup.h"    //Pinyin phrase
#include "aknfepuictrleditpane.h" 
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"

const TUint16 KToneMarkLib[] = 
    {
    0x02C9, 0x02CA, 0x02C7, 0x02CB, 0x02D9
    };

const TUint16 KDelimiter =0x0027; 
const TInt KToneMarkMax = 5;
const TInt KMaxKeystrokeCount = 31;
const TInt KMaxPhraseCreationCount = 7;

_LIT( KKey2, "2");
_LIT( KKey3, "3");
_LIT( KKey4, "4");
_LIT( KKey5, "5");
_LIT( KKey6, "6");
_LIT( KKey7, "7");
_LIT( KKey8, "8");
_LIT( KKey9, "9");
//KMaxPhraseNote means the max phrase is seven
_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
TAknFepInputStatePinyinPhraseCreationBase::TAknFepInputStatePinyinPhraseCreationBase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputStateCandidateBase( aOwner, aUIContainer )
    {    
    }

// ---------------------------------------------------------
// Add keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::AddKeystroke( TInt aKey )
    {
    TBool result = ETrue;
    TRAPD( ret, result = AddKeystrokeL( aKey ) );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    return result;
    }

// ---------------------------------------------------------
// Add tone mark.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::AddToneMark()
    {
    TBool result = ETrue;
    TRAPD( ret, result = AddToneMarkL() );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    return result;
    }

// ---------------------------------------------------------
// get the valid keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::GetValidKeystroke()
    {
    TBool result = ETrue;
    TRAPD( ret, result = GetValidKeystrokeL() );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    return result;
    }

// ---------------------------------------------------------
// Delete current keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::DeleteCurrentKeystroke()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = popup->ShowKeystrokeArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    
    if ( 0 == index )
        {
        return EFalse;
        }
    
    showKeystroke->Delete( index - 1 );
    editPane->SetCursorIndexOfKeystroke( index - 1 );
    
    return ETrue;
    }

// ---------------------------------------------------------
// Delete current keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::ChineseRevertKeystroke()
    {
    TBool result = ETrue;
    TRAPD( ret, result = ChineseRevertKeystrokeL() );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    return result;
    }

// ---------------------------------------------------------
// show info to the eep ctrl.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::ShowInfo()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = popup->ShowKeystrokeArray();
    CDesCArrayFlat* chinese = popup->ChooseChineseCharacterArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    
    TBuf<KMaxKeystrokeCount> buf;
    
    for ( TInt i = 0; i < chinese->Count(); ++i )
        {
        buf.Append( chinese->MdcaPoint( i ) );
        }
    
    for ( TInt i = 0; i < showKeystroke->Count(); ++i )
        {
        buf.Append( showKeystroke->MdcaPoint( i ) );
        }
    
    if ( showKeystroke->Count() == 0 )
        {
        editPane->SetCursorIndexOfKeystroke( 0 );
        index = 0;
        }
    
    editPane->SetText( buf, chinese->Count() + index );
    }

// ---------------------------------------------------------
// set the show info warning color.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::SetWarningColor()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt showKeystrokeCount = popup->ShowKeystrokeArray()->Count();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    TInt validKeytrokeCount = popup->KeystrokeArray()->Count();
    if ( 0 == validKeytrokeCount )
        {
        editPane->SetHighlight( chineseCount, chineseCount + showKeystrokeCount );
        }
    }

// ---------------------------------------------------------
// Get the candidate info.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::GetCandidate( TInt aSelection )
    {
    TRAP_IGNORE( GetCandidateL( aSelection ) );
    }

// ---------------------------------------------------------
// Change CBA
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::ChangeCba()
    {
    TRAP_IGNORE( ChangeCbaL() );
    }

// ---------------------------------------------------------
// Refresh UI
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::RefreshCandidate()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    CDesCArrayFlat* validKeystroke = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    CDesCArrayFlat* candidateArray = 
        uiContainer->CandidatePane()->CandidateArray();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    // have valid keystroke.
    if ( validKeystroke->Count() > 0 )
        {
        uiContainer->SetControlInVisible( ETrue );
        // set display page for delibrate selection
        ClearDeliberateSelection();
        GetCandidate( );
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( ETrue );
        }
    else
        {
        candidateArray->Reset();
        if ( KMaxPhraseCreationCount == chineseCount )
            {
            uiContainer->SetControlInVisible( ETrue );
            TRAP_IGNORE( candidateArray->AppendL( KMaxPhraseNote ) );
            }
        else
            {
            uiContainer->SetControlInVisible( EFalse );
            }
        
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( EFalse );
        }    
        
    if (  EKeySequenceEdit == iState )
        {
        //popup->SetNoneHighlighted();
        }
    uiContainer->Enable(ETrue);
    popup->Enable( validKeystroke->Count() > 0 );    
    UpdateIndicator();
    }

// ---------------------------------------------------------
// Refresh UI
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::RefreshUI(TInt aSelection)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    CDesCArrayFlat* validKeystroke = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    CDesCArrayFlat* candidateArray = 
        uiContainer->CandidatePane()->CandidateArray();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    // have valid keystroke.
    if ( validKeystroke->Count() > 0 )
        {
        uiContainer->SetControlInVisible( ETrue );
        popup->SplitPhraseSpellingIntoPages();
        inputPane->SetText( spelling->MdcaPoint( aSelection ) );
        // set display page for delibrate selection
        popup->SetDisplayPage(aSelection);
        popup->SetPhraseItemTexts();
        popup->PopupSizeChanged();
        GetCandidate( aSelection );
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( ETrue );
        }
    else
        {
        candidateArray->Reset();
        if ( KMaxPhraseCreationCount == chineseCount )
            {
            uiContainer->SetControlInVisible( ETrue );
            TRAP_IGNORE( candidateArray->AppendL( KMaxPhraseNote ) );
            }
        else
            {
            uiContainer->SetControlInVisible( EFalse );
            }
        
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( EFalse );
        }    
        
    if (  EKeySequenceEdit == iState )
        {
        popup->SetNoneHighlighted();
        }
    uiContainer->Enable(ETrue);
    popup->Enable( validKeystroke->Count() > 0 );    
    UpdateIndicator();
    }

// ---------------------------------------------------------
// clear selection.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::ClearDeliberateSelection()
    {
    // we must have just deleted the last character, 
    // or we are starting a new pinyin session, so wipe the last deliberate selection
    TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
    oldDeliberateSelection = KNullDesC;
    }

// ---------------------------------------------------------
// clear the keystroke of PtiEngine.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::ClearPtiEngineKeystroke()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    for ( TInt i = 0; i < KMaxKeystrokeCount; ++i )
        {
        ptiengine->DeleteKeyPress();
        }
    }


// ---------------------------------------------------------
// Get the code of Keystroke in ITUT.
// ---------------------------------------------------------
//
TInt TAknFepInputStatePinyinPhraseCreationBase::GetKeystrokeCode(
                                                       TPtiKey& aKey, 
                                                       const TDesC& aKeystroke )
    {    
    if ( aKeystroke.Length() < 1 )
        {
        return EKeyStrokeNull;
        }
    
    // the input key is delimiter the return -1.
    if ( KDelimiter == aKeystroke[0] )
        {
        aKey = EPtiKey1;
        return EKeyDelimiter;
        }
    // the input key is tone mark return tone mark.
    for ( TInt i = 0; i < KToneMarkMax; ++i )
        {
        if ( KToneMarkLib[i] == aKeystroke[0] )
            {
            return i + 1;
            }
        }
    // the keystroke is 2 - 9.
    if ( 0 == aKeystroke.Compare( KKey2 ) )
        {
        aKey = EPtiKey2;
        }
    else if ( 0 == aKeystroke.Compare( KKey3 ) )
        {
        aKey = EPtiKey3;
        }
    else if ( 0 == aKeystroke.Compare( KKey4 ) )
        {
        aKey = EPtiKey4;
        }
    else if ( 0 == aKeystroke.Compare( KKey5 ) )
        {
        aKey = EPtiKey5;
        }
    else if ( 0 == aKeystroke.Compare( KKey6 ) )
        {
        aKey = EPtiKey6;
        }
    else if ( 0 == aKeystroke.Compare( KKey7 ) )
        {
        aKey = EPtiKey7;
        }
    else if ( 0 == aKeystroke.Compare( KKey8 ) )
        {
        aKey = EPtiKey8;
        }
    else if ( 0 == aKeystroke.Compare( KKey9 ) )
        {
        aKey = EPtiKey9;
        }
    return EkeyNumber;
    }


// ---------------------------------------------------------
// Append the key to the ptiengine.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::AppendKeyPressL( 
                                                 TPtiKey& aKey,
                                                 TBool aAddSpellingFlag
                                                 )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* tempSpelling = popup->TempSpelling();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    CDesCArrayFlat* afterCheck = popup->OptimizeSpelling();
    
    tempSpelling->Reset();
    afterCheck->Reset();
    TInt stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
    TInt stringAfterLength = ptiengine->AppendKeyPress(aKey).Length();
    // the pti can't append key.
    if ( stringBeforeLength == stringAfterLength )
        {
        return EFalse;
        }
    // get the spelling by pti.
    ptiengine->GetPhoneticSpellingsL( *tempSpelling );
    // delete the phrase spelling.
    CheckSpellingL( tempSpelling, afterCheck );
    if ( afterCheck->Count() > 0 )
        {
        if ( !aAddSpellingFlag )
            {
            for( TInt i = afterCheck->Count() - 1; i >= 0; --i )
                {
                spelling->InsertL( 0, afterCheck->MdcaPoint( i ) );
                }
            }
        return ETrue;            
        }
    
    return EFalse;
    }

// ---------------------------------------------------------
// Append the tone mark to the ptiengine.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::AppendToneMarkL( TInt& aToneMark )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* tempSpelling = popup->TempSpelling();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();    
    
    for ( TInt i = 0; i < aToneMark; ++i )
        {
        tempSpelling->Reset();
        ptiengine->IncrementToneMark( ETrue );
        // get the spelling by pti.
        ptiengine->GetPhoneticSpellingsL( *tempSpelling );
        if ( CheckToneMark( aToneMark ) )
            {
            break;
            }
        }
    
    if ( tempSpelling->Count() > 0 )
        {
        for ( TInt i = tempSpelling->Count() - 1; i >= 0; --i )
            {
            spelling->InsertL( 0, tempSpelling->MdcaPoint( i ) );
            }
        return ETrue;
        }
    
    return EFalse;
    }

// ---------------------------------------------------------
// check the spelling delete the phrase spelling.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::CheckSpellingL(
                                               CDesCArrayFlat* aTempSpelling,
                                               CDesCArrayFlat* aAfterCheck
                                               )
    {
    TBuf<1> buf;
    buf.Append( KDelimiter );
    for (TInt i = 0; i < aTempSpelling->Count(); ++i )
        {
        if ( KErrNotFound == aTempSpelling->MdcaPoint(i).Find( buf ) )
            {
            aAfterCheck->AppendL( aTempSpelling->MdcaPoint(i) );
            }
        }
    }

// ---------------------------------------------------------
// check the tone mark and delete the phrase spelling.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::CheckToneMark( TInt& aToneMark )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* tempSpelling = popup->TempSpelling();
    TBuf<1> delimiter;
    delimiter.Append( KDelimiter );
    
    for ( TInt i = tempSpelling->Count() - 1; i >= 0; --i )
        {
        if ( KErrNotFound != tempSpelling->MdcaPoint( i ).Find( delimiter ) )
            {
            tempSpelling->Delete( i );
            }
        else
            {
            if ( KToneMarkLib[ aToneMark - 1 ] != 
                tempSpelling->MdcaPoint( i ).Right( 1 )[0] )
                {
                tempSpelling->Delete( i );
                }
            }
        }
    tempSpelling->Compress();
    return tempSpelling->Count();
    }


// ---------------------------------------------------------
// Add keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::AddKeystrokeL( TInt aKey )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = popup->ShowKeystrokeArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    TBuf<1> key;
    
    if ( showKeystroke->Count() + chineseCount >= KMaxKeystrokeCount )
        {
        return EFalse;
        }
    
    if ( KMaxPhraseCreationCount == chineseCount )
        {
        return EFalse;
        }
    
    switch( (TPtiKey)aKey )
        {
        case EPtiKey0:
            key.Append( KDelimiter );
            break;
        case EPtiKey1:
            key.Append( KDelimiter );
            break;
        case EPtiKey2:
            key.Append( EPtiKey2 );
            break;
        case EPtiKey3:
            key.Append( EPtiKey3 );
            break;
        case EPtiKey4:
            key.Append( EPtiKey4 );
            break;
        case EPtiKey5:
            key.Append( EPtiKey5 );
            break;
        case EPtiKey6:
            key.Append( EPtiKey6 );
            break;  
        case EPtiKey7:
            key.Append( EPtiKey7 );
            break;
        case EPtiKey8:
            key.Append( EPtiKey8 );
            break;
        case EPtiKey9:
            key.Append( EPtiKey9 );
            break;
        default:
            return EFalse;
        }
    
    if ( index > showKeystroke->Count() )
        {
        index = showKeystroke->Count();
        }
    
    showKeystroke->InsertL( index, key );
    editPane->SetCursorIndexOfKeystroke( index + 1 );
    return ETrue;
    }

// ---------------------------------------------------------
// Add tone mark.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::AddToneMarkL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = popup->ShowKeystrokeArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    TBuf<1> key;
    TInt toneMark( 0 );
    
    if ( KMaxPhraseCreationCount == chineseCount )
        {
        return EFalse;
        }
    
    if ( showKeystroke->Count() + chineseCount >= KMaxKeystrokeCount )
        {
        if ( 0 == index )
            {
            return EFalse;
            }
        for ( TInt i = 0; i < KToneMarkMax; ++i )
            {
            if ( KToneMarkLib[i] == showKeystroke->MdcaPoint( index - 1 )[0] )
                {
                toneMark = i + 1;
                break;
                }
            }
        if ( 0 == toneMark )
            {
            return EFalse;
            }
        }
    
    if ( 0 == index )
        {
        key.Append( KToneMarkLib[0] );
        showKeystroke->InsertL( 0, key );
        editPane->SetCursorIndexOfKeystroke( 1 );
        return ETrue;
        }
    
    for ( TInt i = 0; i < KToneMarkMax; ++i )
        {
        if ( KToneMarkLib[i] == showKeystroke->MdcaPoint( index - 1 )[0] )
            {
            toneMark = i + 1;
            break;
            }
        }
    
    if ( 0 == toneMark )
        {
        key.Append( KToneMarkLib[0] );
        showKeystroke->InsertL( index, key );
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        }
    else if ( KToneMarkMax == toneMark )
        {
        key.Append( KToneMarkLib[0] );
        showKeystroke->Delete( index - 1 );
        showKeystroke->InsertL( index - 1, key );
        }
    else
        {
        key.Append( KToneMarkLib[toneMark] );
        showKeystroke->Delete( index - 1 );
        showKeystroke->InsertL( index - 1, key );
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
//  check the keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::IsToneMark( const TDesC& aInput )
    {
    for ( TInt i = 0; i < KToneMarkMax; ++i )
        {
        if ( KToneMarkLib[i] == aInput[0] )
            {
            return ETrue;
            }
        }
    return EFalse;
    }
// ---------------------------------------------------------
//  check the keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::CheckKeystroke()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* showKeystroke = popup->ShowKeystrokeArray();
    TBuf<1> buf;
    buf.Append( KDelimiter );
    // if the keystroke is null then the keystroke is invalid.
    if ( 0 == showKeystroke->Count() )
        {
        return EFalse;
        }
    // if the first keystroke is delimiter then keytroke is invalid. 
    if ( KDelimiter == showKeystroke->MdcaPoint( 0 )[0] )
        {
        return EFalse;
        }
    // if the first keystroke is Tone mark then keytroke is invalid.
    for ( TInt i = 0; i < KToneMarkMax; ++i )
        {
        if ( KToneMarkLib[i] == showKeystroke->MdcaPoint( 0 )[0] )
            {
            return EFalse;
            }            
        }
    // the first keystroke is not delimiter and tone mark, if the number of keystroke
    // is not more than two then must the keytroke are all valid.
    if ( showKeystroke->Count() <= 2 )
        {
        return ETrue;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
// get the valid keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::GetValidKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* showKeystroke = popup->ShowKeystrokeArray();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    CDesCArrayFlat* allValidKeystroke = popup->KeystrokeArray();
    CDesCArrayFlat* validKeystroke = popup->InEffectKeystrokeArray();
    TInt allValidKeyLen = allValidKeystroke->Count();
    TPtiKey key;
    TInt retCode(0);
    TBool nextIsToneMark = EFalse;
    TBuf<1> delimiter;
    TBuf<KMaxKeystrokeCount> beforeBuf;
    TBuf<KMaxKeystrokeCount> afterBuf;
    
    delimiter.Append( KDelimiter );
    
    for ( TInt i = 0; i < allValidKeyLen; ++i )
        {
        if ( 0 == allValidKeystroke->MdcaPoint( i ).Compare( delimiter ) )
            {
            break;
            }
        beforeBuf.Append( allValidKeystroke->MdcaPoint( i ) );
        }
    
    spelling->Reset();
    validKeystroke->Reset();
    allValidKeystroke->Reset();
    
    ClearPtiEngineKeystroke();
    if ( !CheckKeystroke() )
        {
        if ( allValidKeyLen > 0 )
            {
            popup->SetValidKeystrokeChange( ETrue );
            }
        else
            {
            popup->SetValidKeystrokeChange( EFalse );
            }
        return EFalse;
        }
    
    // the firts keystroke is key 2 - 9.    
    for ( TInt i = 0; i < showKeystroke->Count(); ++i )
        {
        retCode = GetKeystrokeCode( key, showKeystroke->MdcaPoint( i ) );
        //the keystroke is delimiter.
        if ( -1 == retCode )
            {
            validKeystroke->AppendL( showKeystroke->MdcaPoint( i ) );
            allValidKeystroke->AppendL( showKeystroke->MdcaPoint( i ) );
            break;
            }
        //the keystroke is 2 - 9
        else if ( 0 == retCode )
            {
            //if the next keystroke is tone mark, then current keystroke and 
            //the next keystroke need to whole check. 
            if ( i < showKeystroke->Count() - 1 )
                {
                TPtiKey keyTmp;
                if ( GetKeystrokeCode( keyTmp, 
                    showKeystroke->MdcaPoint( i  + 1) ) > 0 )
                    {
                    nextIsToneMark = ETrue;
                    }
                }
            if ( AppendKeyPressL( key, nextIsToneMark ) )
                {
                //the next keystroke is not tone mark.
                if ( !nextIsToneMark )
                    {
                    validKeystroke->AppendL( showKeystroke->MdcaPoint( i ) );
                    allValidKeystroke->AppendL( showKeystroke->MdcaPoint( i ) );
                    }
                }
            else
                {
                break;
                }
            }
        else
            {
            if ( AppendToneMarkL( retCode ) )
                {
                validKeystroke->AppendL( showKeystroke->MdcaPoint( i - 1 ) );
                validKeystroke->AppendL( showKeystroke->MdcaPoint( i ) );
                allValidKeystroke->AppendL( showKeystroke->MdcaPoint( i - 1 ) );
                allValidKeystroke->AppendL( showKeystroke->MdcaPoint( i ) );
                }
            break;
            }
        }
    for ( TInt i = 0; i < allValidKeystroke->Count(); ++i )
        {
        if ( 0 == allValidKeystroke->MdcaPoint( i ).Compare( delimiter ) )
            {
            break;
            }
        afterBuf.Append( allValidKeystroke->MdcaPoint( i ) );
        }
    
    if ( allValidKeystroke->Count() > 0 )
        {
        if ( ( 0 == allValidKeyLen ) ||  
            ( 0 != afterBuf.Compare( beforeBuf ) ) )
            {
            popup->SetValidKeystrokeChange( ETrue );
            }
        else
            {
            popup->SetValidKeystrokeChange( EFalse );
            }
        return ETrue;
        }
    
    return EFalse;
    }

// ---------------------------------------------------------
// Revert the current chinse to keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStatePinyinPhraseCreationBase::ChineseRevertKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = popup->ShowKeystrokeArray();
    CDesCArrayFlat* chinese = popup->ChooseChineseCharacterArray();
    CDesCArrayFlat* chineseKeystroke = 
        popup->ChooseChineseCharacterArrayKeystroke();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    
    if ( ( 0 != index ) || ( 0 == chinese->Count() ) )
        {
        return EFalse;
        }
    
    if ( chineseKeystroke->MdcaPoint( chineseKeystroke->Count() - 1 ).Length() < 1 )
        {
        return EFalse;
        }
    
    for( TInt i = chineseKeystroke->MdcaPoint( chineseKeystroke->Count() - 1 ).
        Length() - 1; i >= 0 ; --i )
        {
        showKeystroke->InsertL( 0, chineseKeystroke->MdcaPoint( 
            chineseKeystroke->Count() - 1 ).Mid( i, 1 ) );
        }
    
    editPane->SetCursorIndexOfKeystroke( 
        chineseKeystroke->MdcaPoint( chineseKeystroke->Count() - 1 ).Length() );
    chinese->Delete( chinese->Count() - 1 );
    chineseKeystroke->Delete( chineseKeystroke->Count() - 1 );
    
    // if the show inof's length more than KMaxKeystrokeCount, delete more inof.   
    for ( TInt i = chinese->Count() + showKeystroke->Count() - KMaxKeystrokeCount; 
        i > 0; --i )
        {
        showKeystroke->Delete( showKeystroke->Count() - 1 );
        }
        
    showKeystroke->Compress();
    chinese->Compress();
    chineseKeystroke->Compress();
    return ETrue;
    }


// ---------------------------------------------------------
// Get the candidate info.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::GetCandidateL( TInt aSelection )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* phraseCandidates = 
        uiContainer->CandidatePane()->CandidateArray();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    
    phraseCandidates->Reset();
    if ( ( aSelection > spelling->Count() ) || ( 0 == spelling->Count() ) )
        {
        return;
        }
    extern void CleanArray( TAny* aAny );
    RPointerArray<HBufC> list;
    TCleanupItem cleanitem( CleanArray, &list );
    CleanupStack::PushL( cleanitem );
    TPtrC curSpell = spelling->MdcaPoint( aSelection );
    HBufC* retString = ptiengine->GetCandidatesByInputString( 
                                              spelling->MdcaPoint( aSelection ), 
                                              list,
                                              EFalse );
    
    CleanupStack::PushL( retString );
    for (int index = 0; index < ptiengine->PhoneticSpellingCount(); index++)
        {
        TPtrC ptr = ptiengine->GetPhoneticSpelling(index + 1);               
        if (ptr.Length() == curSpell.Length())
            {
            ptiengine->SelectPhoneticSpelling(index + 1);
            ptiengine->GetChinesePhraseCandidatesL(*phraseCandidates);
            break;
            }
        }
    
    CleanupStack::PopAndDestroy( retString );
    CleanupStack::PopAndDestroy(); // cleanitem   
    }

// ---------------------------------------------------------
// Change CBA
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::ChangeCbaL()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    TInt keystrokeCount = popup->ShowKeystrokeArray()->Count();
    TInt chineseCount = popup->ChooseChineseCharacterArray()->Count();
    TInt validKeystrokeCount = popup->InEffectKeystrokeArray()->Count();
    TInt lastRes = popup->GetLastResouce();

    if ( ( chineseCount == KMaxPhraseCreationCount ) ||
        ( ( 0 == keystrokeCount ) && ( chineseCount > 1 ) ) )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE )
            {
            return;
            }
        //User cba done-done-cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        }
    else if ( ( 0 != keystrokeCount ) && ( 0 != validKeystrokeCount ) )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT )
            {
            return;
            }
        //User cba select-select-cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
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
// Update Indicator.
// ---------------------------------------------------------
//
void TAknFepInputStatePinyinPhraseCreationBase::UpdateIndicator()
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

    if ( candidatePane->SelectedIndex() == 0
        && candidatePane->IsFirstPage() )
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows( ETrue );
        }

    if ( candidatePane->IsLastPage() && (candidatePane->SelectedIndex()
        == candidatePane->VisibleCandidateCount() - 1 ) )
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows( ETrue );
        }
    }

// End of file
