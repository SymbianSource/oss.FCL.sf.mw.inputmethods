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
 *       Provides the CAknFepUIInputStateMiniQwertyZhuyinPhrase methods.
 *
*/












#include "AknFepUiInputStateEntryMiniQwertyZhuyinPhrase.h"
#include "AknFepUIManagerStateInterface.h"

#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag
#include "AknFepUICtrlPinyinPopup.h"		//Pinyin phrase
#include "aknfepuictrleditpane.h"           //phrase creation
#include "aknfepinputstateminiqwertyzhuyinkeymap.h"  //phrase creation
#include <PtiEngine.h>
#include <e32keys.h>
#include <PtiDefs.h>    
#include <avkon.rsg>            //keys
#include <aknfep.rsg>

const TInt KInputPaneLength = 32;
_LIT(KSep, "'");
_LIT(KReplace, "");
_LIT(KLeftBracket, "(");
_LIT(KRightBracket, ")");
const TText KPinyinTone4Valid = 0x02D9;
const TText KPinyinTone4Invalid = 0x0020;
const TInt KMaxKeystrokeCount = 31;
const TInt KKeystrokeMaxLength =5;
const TInt KKeyBufferMaxLength =150;

const TUint16 KSai = 0x311D;
const TUint16 KToneMark1 = 0x0020;
const TUint16 KToneMark2 = 0x02CA;
const TUint16 KToneMark3 = 0x02c7;
const TUint16 KToneMark4 = 0x02CB;
const TUint16 KToneMark5 = 0x02D9;

struct TToneZhuYinMap
    {
    TUint16 iValue;
    TInt iInputTone;
    };
const TToneZhuYinMap ZhuyinToneMap[] =
    {
        {
        KToneMark1, 1
        },
        {
        KToneMark2, 2
        },
        {
        KToneMark3, 3
        },
        {
        KToneMark4, 4
        },
        {
        KToneMark5, 5
        }
    };

const TInt KMaxPhraseCount = 150;//This should be the same as that in pticpicore. 

_LIT( KZhuyinNote,"\x9020\x65B0\x8BCD" );

TAknFepInputStateEntryMiniQwertyZhuyinPhrase::TAknFepInputStateEntryMiniQwertyZhuyinPhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputStateEntryQwertyBaseChinesePhrase( aOwner, aUIContainer)
    {
    iState = EEntry;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane( EFalse );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutInput );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->ShowVerticalScrollArrows( ETrue );
    uiContainer->ShowHorizontalScrollArrows( EFalse );
    uiContainer->InputPane()->SetOverrideFontId( 0 );
    uiContainer->SetFepMan( iOwner->FepMan() );
    
    //For fix bug that the text in input pane could not be shown totally. 
    if ( iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty4x10 ||
        iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty3x11)
        {
        uiContainer->PinyinPopupWindow()->SetInputLayoutMiniQwertyZhuyin();
        }
    
    iOwner->PtiEngine()->SetCandidatePageLength( KMaxPhraseCount );
    // in the case that we are coming back to the input pane from the candidate pane, 
    // we need to ensure that the current selection is selected correctly
    ImplicitlyUpdateSelection();
    // however we also need to clear the deliberate selection, in case we are not
    // coming back to the input pane from the candidate pane
    ClearDeliberateSelection();
    UpdateIndicator();

    iOwner->PtiEngine()->SetInputMode( EPtiEngineZhuyinPhraseQwerty );
    iOwner->PtiEngine()->SetCase( EPtiCaseLower );
    }

TBool TAknFepInputStateEntryMiniQwertyZhuyinPhrase::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    if ( iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty4x10 ||
        iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty3x11)
        {
        return HandleKeyForMiniQwertyL( aKey, aLength );
        }
    else
        {
        return HandleKeyForHalfAndCustomQwertyL( aKey, aLength );
        }
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::KeyTimerExpired()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();

    // set container's position
    TPoint baseLine = TPoint( 0, 0 );
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent))
    ;
    if ( ret == KErrNone )
        {
        uiContainer->SetContainerPosition( baseLine, height );
        }

    TInt pinyinCountWithoutToneMarks = ptiengine->PhoneticSpellingCount();
    if ( editPane->GetEffictiveLength() < keystroke->Count()
        || pinyinCountWithoutToneMarks == popup->CurrentSelection() )
        {
        TBuf<KKeyBufferMaxLength> keystrokeBuf;
        for (TInt i = 0; i < keystroke->Count(); i++ )
            {
            keystrokeBuf.Append( keystroke->MdcaPoint( i ) );
            }
        inputPane->SetText( keystrokeBuf );
        }
    else
        {
        TPtrC ptr = ptiengine->GetPhoneticSpelling( 1 );
        TBuf<KInputPaneLength> currentText;
        inputPane->GetText( currentText );
        if ( ptr.Length() < currentText.Length() )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        inputPane->SetText( ptr );
        }
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::DeliberatelyUpdateSelection()
    {
    TPtr deliberateSelection = iOwner->GetLatestDeliberateSelection();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    TInt selected = popup->CurrentSelection();
    ptiengine->EnableToneMarks( EFalse );
    TPtrC spelling = ptiengine->GetPhoneticSpelling( selected + 1 ); // our index is zero based, engine index is one based
    deliberateSelection
        = spelling.Left( MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark );
    ptiengine->EnableToneMarks( ETrue );
    UIContainer()->PinyinPopupWindow()->SetFlag( MAknFepUICtrlPinyinPopup::ESpellingNavigation );
    RefreshUI( popup->CurrentVisibleSelection() );
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::ImplicitlyUpdateSelection()
    {
    TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
    TInt oldDeliberateSelectionLength = oldDeliberateSelection.Length();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    // previously selected is compared in turn to each of the new candidates
    ptiengine->EnableToneMarks( EFalse );
    TInt pinyinCount = ptiengine->PhoneticSpellingCount();
    TInt newSelection = 0;
    for (TInt i = 0; i < pinyinCount; i++ )
        {
        TPtrC spelling = ptiengine->GetPhoneticSpelling( i + 1 ); // our index is zero based, engine index is one based
        TInt spellingLength = spelling.Length();

        // figure out how many characters we are comparing, this depends on the 
        // direction of the edit
        TInt compareLength = oldDeliberateSelectionLength <= spellingLength
            ? oldDeliberateSelectionLength : spellingLength;

        if ( oldDeliberateSelection.Left( compareLength )
            == spelling.Left( compareLength ) )
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
    ptiengine->EnableToneMarks( ETrue );
    UIContainer()->PinyinPopupWindow()->SetFlag( MAknFepUICtrlPinyinPopup::ESpellingChanged );
    RefreshUI( newSelection );
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::ClearDeliberateSelection()
    {
    // we must have just deleted the last character, 
    // or we are starting a new zhuyin session, so wipe the last deliberate selection
    CDesCArrayFlat* keystroke = UIContainer()->EditPaneWindow()->KeystrokeArray();
    if ( keystroke->Count() == 0 )
        {
        TPtr oldDeliberateSelection = iOwner->GetLatestDeliberateSelection();
        oldDeliberateSelection = KNullDesC;
        }
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::RefreshUI( TInt aSelection )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    CDesCArrayFlat* spelling = popup->PhoneticSpellingArray();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TBuf<KKeyBufferMaxLength> keystrokeBuf;

    // get cursor position
    TPoint baseLine = TPoint( 0, 0 );
    TInt height = 0;
    TInt ascent = 0;

    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if ( ret == KErrNone )
        {
        uiContainer->SetContainerPosition( baseLine, height );
        }

    TText toneMark;
    TBool toneMarkEntered = ptiengine->ToneMark( toneMark );
    TBuf<1> validToneMarkBuf;
    if ( toneMarkEntered )
        {
        // override specific invalid tonemark character only, the others are the same character 
        // when both valid and invalid
        if ( toneMark == KPinyinTone4Invalid )
            toneMark = KPinyinTone4Valid;
        validToneMarkBuf.Append( toneMark );
        }

    // to start with, disable the tone marks so we can get the list of all zhuyin matches
    // later we will figure out which ones are invalid
    ptiengine->EnableToneMarks( EFalse );
    TInt pinyinCountWithoutToneMarks = ptiengine->PhoneticSpellingCount();

    for (TInt i = 0; i < keystroke->Count(); i++ )
        {
        keystrokeBuf.Append( keystroke->MdcaPoint( i ) );
        }

    if ( pinyinCountWithoutToneMarks > 0 )
        {
        if ( popup->IsFlagSet( MAknFepUICtrlPinyinPopup::ESpellingChanged ) )
            {
            if ( editPane->GetEffictiveLength() < keystroke->Count() )
                {
                spelling->Reset();
                spelling->Compress();
                }
            else
                {
                TRAPD(ignore, ptiengine->GetPhoneticSpellingsL(*spelling));
                if ( ignore != KErrNone )
                    {
                    return;
                    }
                }

            TRAPD( retOptimize, spelling->AppendL( keystrokeBuf ) );
            if ( KErrNone != retOptimize )
                {
                return;
                }

            popup->SplitPhraseSpellingIntoPages();
            }

        // set display page for delibrate selection
        popup->SetDisplayPage( aSelection );
        popup->SetPhraseItemTexts();

        // fill in input pane although it's invisible
        if ( editPane->GetEffictiveLength() < keystroke->Count()
            || pinyinCountWithoutToneMarks == popup->CurrentSelection() )
            {
            inputPane->SetText( keystrokeBuf );
            }
        else
            {
            inputPane->SetText( ptiengine->GetPhoneticSpelling( popup->CurrentSelection()
                + 1 ) );
            }
        popup->PopupSizeChanged(); // phrase input
        }
    // turn tone marks back on so that we know where we are.
    ptiengine->EnableToneMarks( ETrue );

    // this is where we start to figure out whether the tonemarks are valid,
    // whether the selected tonemark is valid, and what is the index of the 
    // selected candidate in the list of candidates with tonemarks...
    TBool selectionToneMarkValid = EFalse;
    TInt selectionIndexAdjustedForToneMarkValidity =
        popup->CurrentSelection();

    // we only need to deal with tone marks if there is one
    if ( toneMarkEntered )
        {
        if ( pinyinCountWithoutToneMarks > 0 )
            {
            for (TInt i = popup->CurrentSelection() + 1; i < pinyinCountWithoutToneMarks; i++ )
                {
                TBool valid = EFalse;

                TBuf<MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark>
                    nextCandidateWithoutToneMark;
                nextCandidateWithoutToneMark.Copy( ptiengine->GetPhoneticSpelling(i + 1).Left( MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark ) );

                ptiengine->EnableToneMarks( EFalse );
                ptiengine->SelectPhoneticSpelling( i + 1 );

                valid = ptiengine->IsToneMarkValidForSpelling();
                if ( valid )
                    {
                    ptiengine->EnableToneMarks( ETrue );
                    TInt pinyinCountWithToneMarks =
                        ptiengine->PhoneticSpellingCount();

                    for (TInt j = popup->CurrentSelection() + 1; j < pinyinCountWithToneMarks; j++ )
                        {
                        // use j here not i as we are looking at the list with tonemarks
                        TPtrC nextCandidateWithToneMark =
                            ptiengine->GetPhoneticSpelling( j + 1 );
                        if ( nextCandidateWithToneMark
                            == nextCandidateWithoutToneMark )
                            {
                            if ( i == aSelection )
                                {
                                selectionToneMarkValid = ETrue;

                                // fill in input pane even if it can't be seen, for when we have to hide popup window
                                inputPane->SetText( nextCandidateWithoutToneMark );
                                }
                            break;
                            }
                        }
                    }
                }
            }
        }
    ptiengine->EnableToneMarks( EFalse );
    ptiengine->SelectPhoneticSpelling( popup->CurrentSelection() + 1 );
    // next two lines are for tonemark validity
    ptiengine->EnableToneMarks( selectionToneMarkValid );
    ptiengine->SelectPhoneticSpelling( selectionIndexAdjustedForToneMarkValidity
        + 1 ); // our index is zero based, engine index is one based

    // update the candidate pane.
    if ( popup->IsFlagSet( MAknFepUICtrlPinyinPopup::ESpellingChanged )
        || popup->IsFlagSet( MAknFepUICtrlPinyinPopup::ESpellingNavigation ) )
        {
        CDesCArrayFlat* phraseCandidates = uiContainer->CandidatePane()->CandidateArray();
        if ( editPane->GetEffictiveLength() == keystroke->Count()
            && pinyinCountWithoutToneMarks > popup->CurrentSelection() )
            {
            TRAPD(ignore, ptiengine->GetChinesePhraseCandidatesL(*phraseCandidates));
            if ( ignore != KErrNone )
                {
                return;
                }
            }
        else if ( keystroke->Count() != 0 )
            {
            phraseCandidates->Reset();
            phraseCandidates->Compress();
            TRAPD( retAddZhuyinNote, phraseCandidates->AppendL( KZhuyinNote ) );
            if ( KErrNone != retAddZhuyinNote )
                {
                return;
                }
            }

        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        popup->ClearFlag( MAknFepUICtrlPinyinPopup::ESpellingChanged
            | MAknFepUICtrlPinyinPopup::ESpellingNavigation );
        }
    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable( ETrue );
    // need to enable the pinyin popup after the container so that it is raised to the front
    popup->Enable( pinyinCountWithoutToneMarks > 0 );
    UpdateIndicator();
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::InitializeStateL( void )
    {
    iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TInt pinyinCountWithoutToneMarks = ptiengine->PhoneticSpellingCount();
        
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
    
    if ( editPane->GetEffictiveLength() < keystroke->Count() ||
        pinyinCountWithoutToneMarks == popup->CurrentSelection() )
        {
        uiContainer->ShowVerticalScrollArrows(EFalse);
        }
    else
        {
        uiContainer->ShowVerticalScrollArrows(ETrue);
        }
    }

TBool TAknFepInputStateEntryMiniQwertyZhuyinPhrase::CheckToneMarkBorderUpon()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TPtrC ptr = keystroke->MdcaPoint( keystroke->Count() - 1 );
    TInt num = sizeof(ZhuyinToneMap )/( sizeof(ZhuyinToneMap[0]));
    for (TInt i = 0; i< num; i++ )
        {
        //if ( ptr.Compare( ZhuyinToneMap[i].iValue) == 0 )
        if ( ptr[0] == ZhuyinToneMap[i].iValue )
            {
            return ETrue;
            }
        }

    return EFalse;
    }

TBool TAknFepInputStateEntryMiniQwertyZhuyinPhrase::HandleKeyForMiniQwertyL( TInt aKey,
    TKeyPressLength aLength )
    {

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    RArray<TInt>* keycodeArray = editPane->KeycodeArray();
    TInt keystrokeLength = keystroke->Count();

    TBool ret = ETrue;

    if ( (iOwner->IsQwertyZhuyinToneMarkKey(aKey)|| aKey == EStdKeySpace)
        && (aLength == EShortKeyPress) )
        {
        if ( keystrokeLength == 0 )
            {
            iOwner->FepMan()->TryCloseUiL();
            CommitToneMarkL( aKey );
            return ETrue;
            }
        
        if ( keystrokeLength < KMaxKeystrokeCount && 
            !CheckToneMarkBorderUpon() )
            {
            if ( editPane->GetEffictiveLength() == keystrokeLength )
            	{
            	TBool isToneValid = ptiengine->SetToneMark(aKey);
				TInt stringAfterLength =
						ptiengine->GetPhoneticSpelling(1).Length();
				TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
				TInt spellSepLen = 0;
				TInt keySepLen = 0;
				for (TInt i = 0; i < stringAfterLength; i++)
					{
					if (ptr.Mid(i, 1).Compare(KSep) == 0)
            		{
            		spellSepLen++;
            		}
            	}
            for ( TInt j = 0; j < keystrokeLength; j++ )
            	{
            	if ( keystroke->MdcaPoint( j ).Compare(KSep) == 0 )
            		{
						keySepLen++;
						}
					}

				// +1 is for we had add tone mark to engine, perhaps it is legal, but 
				// We had not add it to keystroke array
				if ((stringAfterLength - spellSepLen) == (keystrokeLength
						- keySepLen + 1) && isToneValid)
					{
					inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);

					UIContainer()->ShowHorizontalScrollArrows(EFalse);
					UIContainer()->ShowVerticalScrollArrows(ETrue);
					SetInputPaneToneMark();
					editPane->AddEffictiveLength();
					}
				}      

            TBuf<KKeystrokeMaxLength> buf;
            TAknFepInputStateMiniQwertyZhuyinKeyMap::GetKeyStroke(
            		ptiengine, aKey, buf );
            keystroke->AppendL( buf );
            keycodeArray->AppendL( aKey );
            ImplicitlyUpdateSelection();
            }
        else
            {
            UpdateIndicator();
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }

        }
    else if ( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() || CAknFepFnKeyManager::EFnKeyLock
        == iOwner->FepMan()->FnKeyState() || CAknFepFnKeyManager::EFnKeyDown
        == iOwner->FepMan()->FnKeyState() )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        }
    else if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed )
        || iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed )
        || ((aKey == EStdKeyLeftFunc) && aLength == EShortKeyPress) )
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if ( aKey == EKeyLeftShift || aKey ==EKeyLeftCtrl || aKey
        == EKeyRightCtrl || aKey ==EKeyRightFunc )
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if ( aKey == EStdKeyBackspace )
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
            if ( editPane->GetEffictiveLength() == keystrokeLength )
                {
                if ( iOwner->PtiEngine()->DeleteKeyPress().Length() && keystrokeLength != 1 )
                    {
                    editPane->DecEffictiveLength();
                    keystroke->Delete( keystrokeLength - 1 );
                    keycodeArray->Remove( keystrokeLength - 1 );
                    inputPane->SetFlag( MAknFepUICtrlInputPane::EPhoneticChanged );
                    ImplicitlyUpdateSelection();
                    }
                else
                    {
                    ClearDeliberateSelection();
                    iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                    if ( aLength == ELongKeyPress )
                        {
                        iOwner->FepMan()->SetLongClearAfterCloseUI( ETrue );
                        }
                    }
                }
            else
                {
                keystroke->Delete( keystrokeLength - 1 );
                keycodeArray->Remove( keystrokeLength - 1 );
                ImplicitlyUpdateSelection();
                }
            }
        }
    else if ( aKey ==EStdKeyDevice1 )//sk2
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else if ( aKey == EStdKeyRightArrow )
        {
        if ( popup->IsEnabled() )
            {
            if ( popup->SelectNextPhrase() )
                {
                DeliberatelyUpdateSelection();
                UpdateIndicator();
                }
            }
        }
    else if ( aKey == EStdKeyLeftArrow )
        {
        if ( popup->IsEnabled() )
            {
            if ( popup->SelectPrevPhrase() )
                {
                DeliberatelyUpdateSelection();
                UpdateIndicator();
                }
            }
        }
    else if ( aKey == EStdKeyDownArrow || aKey == EStdKeyDevice3 || aKey
        == EStdKeyDevice0 )//selection key
        {
        editPane->SetChangeState( ETrue );
        if ( editPane->GetEffictiveLength() == keystroke->Count()
            && ptiengine->PhoneticSpellingCount()
                > popup->CurrentSelection() )
            {
            if ( popup->IsEnabled() )
                {
                popup->Enable( EFalse );
                }
            iOwner->ChangeState( ECandidate );
            }
        else
            {
            if ( keystrokeLength > 0 && keystroke->MdcaPoint( 0 )[0] == KSai )
                {
                UIContainer()->SetControlInVisible( EFalse );
                iOwner->ChangeState( EMiniQwertyEdit );
                }
            else
                {
                ClearDeliberateSelection();
                editPane->SetNeedClearDeliberateSelection( ETrue );
                iOwner->ChangeState( EZhuyinSpelling );
                }
            }
        }
    else if ( aKey == EStdKeyEnter )//enter the characters in the entry pane into the editor
        {
        TInt currentIndex = uiContainer->PinyinPopupWindow()->CurrentSelection();
        TBuf<KInputPaneLength> currentText;
        if ( currentIndex < ptiengine->PhoneticSpellingCount() )
            {
            currentText.Copy(ptiengine->GetPhoneticSpelling( currentIndex + 1 ));
            }
        else
            {
            inputPane->GetText( currentText );
            }
        

        while (currentText.Find( KSep )> 0 )
            {
            currentText.Replace( currentText.Find( KSep ), 1, KReplace );
            }
        while (currentText.Find( KLeftBracket ) >= 0 )
            {
            currentText.Replace( currentText.Find( KLeftBracket ), 1,
                KReplace );
            }
        while (currentText.Find( KRightBracket ) >= 0 )
            {
            currentText.Replace( currentText.Find( KRightBracket ), 1,
                KReplace );
            }

        if ( currentText.Length() )
            {
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->NewTextL( currentText );
            fepMan->CommitInlineEditL();
            if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
                {
                fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
                }
            }

        iOwner->FepMan()->TryCloseUiL();

        }
    else if ( aLength == EShortKeyPress ) // don't want repeats on these keys
        {
        iOwner->FepMan()->SetCcpuFlag( CAknFepManager::ECcpuStateIgnoreStarUp );

        if ( keystroke->Count() < KMaxKeystrokeCount
            && (iOwner->IsValidChineseInputKeyQwerty( aKey ) || aKey
                ==EStdKeySpace ) ) //seperator
            {
            TInt stringBeforeLength(0);
            TInt stringAfterLength(0);

            stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
            stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();

            if ( stringBeforeLength != stringAfterLength )
                {
                if ( ptiengine->GetPhoneticSpelling(1).Length() == 1 )
                    {
                    iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
                    }
                editPane->AddEffictiveLength();
                }

            TBuf<KKeystrokeMaxLength> buf;
            TAknFepInputStateMiniQwertyZhuyinKeyMap::GetKeyStroke(
            		ptiengine, aKey, buf );
            keystroke->AppendL( buf );
            keycodeArray->AppendL( aKey );

            ImplicitlyUpdateSelection();
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
        }
    return ret;
    }

TBool TAknFepInputStateEntryMiniQwertyZhuyinPhrase::HandleKeyForHalfAndCustomQwertyL( TInt aKey,
    TKeyPressLength aLength )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();

    TBool ret = ETrue;

    if((aKey == EPtiKeyQwertyE || 
        aKey == EPtiKeyQwertyR || 
        aKey == EPtiKeyQwertyY || 
        aKey == EPtiKeyQwertyU ||
        aKey == EStdKeySpace) && 
        (aLength == EShortKeyPress))        
        {
        if(ptiengine->SetToneMark(aKey))
            {
            inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
            
            TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                    uiContainer->CandidatePane()->IsFirstPage());
            TInt bufLength = ptiengine->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
            
            UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows);
            UIContainer()->ShowVerticalScrollArrows(multiplePages);
            SetInputPaneToneMark();
            ImplicitlyUpdateSelection();
            }
        }
    if(( aKey == EStdKeyLeftFunc) && aLength == EShortKeyPress)     //chr key to tone mark   
        {
        if(ptiengine->IncrementToneMark(ETrue))
            {
            ImplicitlyUpdateSelection();
            }
        }
    else if( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() ||
        CAknFepFnKeyManager::EFnKeyLock == iOwner->FepMan()->FnKeyState() ||
        CAknFepFnKeyManager::EFnKeyDown == iOwner->FepMan()->FnKeyState())
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }                                               
    else if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed)||
             iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if (aKey == EKeyLeftShift || aKey ==EKeyLeftCtrl || aKey == EKeyRightCtrl)      
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if (aKey ==EKeyRightFunc)//fn
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }        
    else if(aKey == EStdKeyBackspace )
        {
        if (iOwner->PtiEngine()->DeleteKeyPress().Length())
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
    else if(aKey ==EStdKeyDevice1)//sk2
        {
        iOwner->FepMan()->TryCloseUiL();
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
    else if (aKey == EStdKeyDownArrow || aKey == EStdKeyDevice3 || aKey == EStdKeyDevice0)//selection key
        {
        if(popup->IsEnabled())
            {
            popup->Enable(EFalse);
            }
        iOwner->ChangeState(ECandidate);
        }   
   else if (aKey == EStdKeyEnter)//enter the characters in the entry pane into the editor
        {
        TBuf<KInputPaneLength> currentText;
        inputPane->GetText(currentText);
        
        while(currentText.Find(KSep)> 0)
            {
            currentText.Replace(currentText.Find(KSep), 1,KReplace);
            }
        
        
        if(currentText.Length())
            {            
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->NewTextL(currentText);
            fepMan->CommitInlineEditL();
            if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                {
                fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                }
            }
            
        iOwner->FepMan()->TryCloseUiL();  
          
        }
    else if(aLength == EShortKeyPress) // don't want repeats on these keys
        {
        iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);            
        
        if(iOwner->IsValidChineseInputKeyQwerty(aKey) || aKey ==EStdKeySpace ) //seperator
            {
            TInt stringBeforeLength(0);
            TInt stringAfterLength(0);
        
            stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
            stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
            
            if (stringBeforeLength != stringAfterLength)
                {
                if (ptiengine->GetPhoneticSpelling(1).Length() == 1)
                    {
                    //iOwner->FepMan()->UpdateCbaL(R_AVKON_SOFTKEYS_EMPTY);                           iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
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
        }
    return ret;
    }

void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::CommitToneMarkL( TInt aKey )
    {
    TBuf<1> ToneMarkBuf;
    switch ( aKey )
        {
        case EPtiKeyQwertyE:
            ToneMarkBuf.Append( KToneMark3 );
            break;

        case EPtiKeyQwertyR:
            ToneMarkBuf.Append( KToneMark4 );
            break;

        case EPtiKeyQwertyY:
            ToneMarkBuf.Append( KToneMark2 );
            break;

        case EPtiKeyQwertyU:
            ToneMarkBuf.Append( KToneMark5 );
            break;
        default:
            break;
        }
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    fepMan->NewCharacterL(ToneMarkBuf);
    fepMan->CommitInlineEditL();
    if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
        {
        fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
        iOwner->FepMan()->TryCloseUiL();
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyZhuyinPhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryMiniQwertyZhuyinPhrase::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            //     case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            if ( popup->IsEnabled() )
                {
                popup->Enable( EFalse );
                }
            iOwner->ChangeState( ECandidate );
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }
// End of file   

