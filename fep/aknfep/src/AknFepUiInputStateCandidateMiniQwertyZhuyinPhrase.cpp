/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Implementation of Zhuyin phrase candidate state
*
*/












// System includes
#include <PtiEngine.h>   

// User includes
#include "AknFepUiInputStateCandidateMiniQwertyZhuyinPhrase.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManager.h" 
#include <AknFep.rsg>   

#include "aknfepuictrleditpane.h"           //for zhuyin phrase creation
#include "aknfepinputstateminiqwertyzhuyinkeymap.h"
#include "aknfepuiinputstateminiqwertyzhuyinphrase.h"
// Constant definition
const TInt KMinCandidateCount = 1;
const TInt KInputPaneLength = 32;
//_LIT( KDelimiter, "\x2022");
_LIT( KDelimiter, "'");
_LIT( KSaiStroke,"\x311D" );

struct TToneZhuYinMap
    {
    TUint16 iValue;
    TInt iInputTone;
    };
const TToneZhuYinMap ZhuyinToneMap[] =
    {
        {0x0020, 1},
        {0x02CA, 2},
        {0x02c7, 3},
        {0x02CB, 4},
        {0x02D9, 5}
    };

// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::
// TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputStateCandidateQwertyBaseChinesePhrase( aOwner, aUIContainer)
    {
    iState = ECandidate;
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
// AknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleKeyL(
    TInt aKey, TKeyPressLength aLength )
    {
    if (iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty4x10 || iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty3x11)
        {
        return HandleKeyForMiniQwertyL(aKey, aLength);
        }
    else
        {
        return HandleKeyForHalfAndCustomQwertyL(aKey, aLength);
        }
    }
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::ComitPhraseCreatedToEditor()
// commit the phrase created to the editor
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::ComitPhraseCreatedToEditorL()
    {
    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
    if ( text.Length() )
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        
    
        TBuf<KInputPaneLength> highText;
        UIContainer()->InputPane()->GetText( highText );
        TInt groupNum = GetInputPaneKeyStrokeCount( highText );
        TInt canidateCout = text.Length();
        if ( groupNum > canidateCout )
            {
            FillPhraseAndStrokArrayL( highText, text );
            }
        else
            {
            fepMan->NewTextL( text );
            fepMan->CommitInlineEditL();
            iOwner->PtiEngine()->SetPredictiveChineseChar( text );
            UIContainer()->EditPaneWindow()->ResetAllArray();
            UIContainer()->EditPaneWindow()->SetEffictiveLength( 0 );
            if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
                {
                fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
                iOwner->FepMan()->TryCloseUiL();
                }
            else
                {
                UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
                iOwner->ChangeState( EPredictiveCandidate );
                }
            }
        }
    return ETrue;
    }
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::GetInputPaneKeyStrokeCount()
// get the input pane key stroke count 
// ---------------------------------------------------------------------------
//
TInt TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::GetInputPaneKeyStrokeCount(
    const TDesC& aBuf )
    {
    TInt groupNum = 1;
    for (TInt i = 0; i < aBuf.Length() - 1; i++ )
        {
        TInt num = sizeof(ZhuyinToneMap )/( sizeof(ZhuyinToneMap[0] ) );
        TBool isFindTone = EFalse;
        TInt isFindDel = 0;
        for (TInt m =0; m< num; m++ )
            {
            if ( aBuf[i] == ZhuyinToneMap[m].iValue )
                {
                isFindTone = ETrue;
                break;
                }
            }
        if ( !isFindTone )
            {
            isFindDel = aBuf.Mid(i,1).Compare( KDelimiter );
            }
        if ( 0 == isFindDel || isFindTone )
            {
            groupNum++;
            }
        }
    if ( 0 == aBuf.Right( 1 ).Compare( KDelimiter ) )
        {
        groupNum++;
        }
    return groupNum;
    }
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::GetIndexOfToneMark()
// get the index of the tonemark 
// ---------------------------------------------------------------------------
//
TInt TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::GetIndexOfToneMark( const TDesC& aText)
    {
    TInt num = sizeof(ZhuyinToneMap )/( sizeof(ZhuyinToneMap[0]));
    TInt len = aText.Length();
    for( TInt j = 0; j< len ;j++ )
        {
        for (TInt i = 0; i<num; i++ )
            {
            if ( aText[j] == ZhuyinToneMap[i].iValue )
                {
                return j;
                }
            }
        }
    return -1;
    }
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::FillPhraseAndStrokArray()
// fill the phrase and keystroke array
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::FillPhraseAndStrokArrayL(
    const TDesC& aText, const TDesC& aPhrase )
    {
    CDesCArrayFlat* keyStrokeArray = UIContainer()->EditPaneWindow()->KeystrokeArray();
    CDesCArrayFlat* phraseKeyStrokeArray = UIContainer()->EditPaneWindow()->PhraseShowKeyStrokeArray() ;
    CDesCArrayFlat* phraseArray = UIContainer()->EditPaneWindow()->PhraseArray();
    
    RArray<TInt>* keyCodeArray = UIContainer()->EditPaneWindow()->KeycodeArray();
    RArray<TInt>* keyCodePhraseArray = UIContainer()->EditPaneWindow()->PhraseKeycodeArray();
    phraseArray->AppendL( aPhrase );
    TBuf<KInputPaneLength> keyStoke;
    TInt replaceCount = 0;
    TInt delimiterIndex = aText.Find( KDelimiter );
    if ( KErrNotFound == delimiterIndex )
        {
        delimiterIndex = -1;
        }
    TInt indexOfToneMark = GetIndexOfToneMark( aText );
    if ( -1 == delimiterIndex && indexOfToneMark == -1 )
        {
        replaceCount = aText.Length();
        }
    else if ( delimiterIndex >= 0 && indexOfToneMark == -1 )
        {
        replaceCount = delimiterIndex;
        }
    else if ( delimiterIndex >= 0 && indexOfToneMark >= 0 )
        {
        replaceCount = Min( delimiterIndex, indexOfToneMark );
        if ( delimiterIndex > indexOfToneMark )
            {
            replaceCount++;
            }
        }
    else
        {
        replaceCount = indexOfToneMark + 1;// have tonemark  but has not deli
        }
    for (TInt i = 0; i < replaceCount; i++ )
        {
        TPtrC ptr = keyStrokeArray->MdcaPoint( 0 );
        keyStoke.Append( ptr );
        keyStrokeArray->Delete( 0 );
        // added for the keycode
        TInt keyCodeValue = (*keyCodeArray)[0];
        keyCodePhraseArray->AppendL( keyCodeValue );
        keyCodeArray->Remove( 0 );
        }
    phraseKeyStrokeArray->AppendL( keyStoke );   
    if ( CheckToneMarkBorderUpon()|| 0 ==keyStrokeArray->MdcaPoint( 0 ).Compare( KSaiStroke ) )
        {
        iOwner->ChangeState( EMiniQwertyEdit );
        }
    else
        {
        //change to the spelling selecting state
        iOwner->ChangeState( EZhuyinSpelling );
        }
    }
// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyZhuyinPhraseBase::GetValidKeystroke
// Get the valid keystroke.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::CheckToneMarkBorderUpon()
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
// AknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleKeyForHalfAndCustomQwertyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleKeyForHalfAndCustomQwertyL(TInt aKey,
                                                                    TKeyPressLength aLength)
    {
    TInt selected = UIContainer()->CandidatePane()->SelectedIndex();
    TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
    
    if (iOwner->FepMan()->FnKeyState())
        {
        return ETrue;
        }
    else if(aKey == EStdKeySpace)
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return ETrue;
        }
    else if(aKey == EStdKeyBackspace )
        {
        iOwner->ChangeState(EEntry);
        iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);

        return ETrue;
        }
    else if(aKey == EStdKeyUpArrow)   
        {
        if(UIContainer()->CandidatePane()->IsFirstPage())
            {
            iOwner->ChangeState(EEntry);
            iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
            return ETrue;
            }
        else
            {
            return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
            }   
        }
    else if ( aKey == EStdKeyEnter)
        {
        TBuf<MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark> atext;
        UIContainer()->PinyinPopupWindow()->GetItemText(
                                    UIContainer()->PinyinPopupWindow()->CurrentSelection(),
                                    atext);
        if(atext.Length())
            {            
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->NewTextL(atext);
            fepMan->CommitInlineEditL();
            iOwner->FepMan()->TryCloseUiL();
            }
        return ETrue;
        }
    else if ( aKey == EStdKeyLeftArrow )
       {
       if ( selected == 0 )
          {
          UIContainer()->CandidatePane()->SelectIndex( numbertotal - 1 );
          }
       else
          {
          UIContainer()->CandidatePane()->SelectIndex( selected - 1 );
          }     
       UpdateIndicator();
       return ETrue;    
       }
    else if ( aKey == EStdKeyRightArrow )
       {
        if ( selected == numbertotal - 1 )
            {
            UIContainer()->CandidatePane()->SelectIndex( 0 );
            }
        else
            {
            UIContainer()->CandidatePane()->SelectIndex( selected + 1 );
            } 
        UpdateIndicator();
        return ETrue;       
       }
    else if ( aKey == EStdKeyDevice0 || aKey == EStdKeyDevice3 )
        {             
        TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
        if(text.Length())
            {            
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->NewTextL(text);
            fepMan->CommitInlineEditL();
            iOwner->PtiEngine()->SetPredictiveChineseChar(text);
            if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                {
                fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                iOwner->FepMan()->TryCloseUiL();
                }
            else
                {
                UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
                iOwner->ChangeState(EPredictiveCandidate);
                }
            }
        return ETrue;
        }   
    else
        {
        return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        } 
    }
// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleKeyForMiniQwertyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleKeyForMiniQwertyL(
    TInt aKey, TKeyPressLength aLength )
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TInt selected = UIContainer()->CandidatePane()->SelectedIndex();
    TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
    //  TInt isSelect = UIContainer()->CandidatePane()->SelectIndex( index, EFalse );
    if ( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() || 
         CAknFepFnKeyManager::EFnKeyLock == iOwner->FepMan()->FnKeyState() )
        {
        return ETrue;
        }
    else if ( aKey == EStdKeyDevice1 )
        {
        iOwner->FepMan()->TryCloseUiL();
        return ETrue;
        }
    else if ( aKey == EStdKeySpace )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return ETrue;
        }
    else if ( aKey == EStdKeyBackspace )
        {
        UIContainer()->EditPaneWindow()->SetChangeState( ETrue );
        iOwner->ChangeState( EEntry );
        return ETrue;
        }
    else if ( aKey == EStdKeyUpArrow )
        {
        if ( UIContainer()->CandidatePane()->IsFirstPage() )
            {
            iOwner->ChangeState( EEntry );
            return ETrue;
            }
        else
            {
            return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(
                aKey, aLength );
            }
        }
    else if ( aKey == EStdKeyDownArrow || aKey == EStdKeySpace )
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
            return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(
                            aKey, aLength );
            }
        }
    else if ( aKey == EStdKeyEnter )
        {
        return ETrue;
        }
    else if ( aKey == EStdKeyLeftArrow )
        {
        if ( selected == 0 )
            {
            UIContainer()->CandidatePane()->SelectIndex( numbertotal-1 );
            }
        else
            {
            UIContainer()->CandidatePane()->SelectIndex( selected - 1 );
            }
        return ETrue;
        }
    else if ( aKey == EStdKeyRightArrow )
        {
        if ( selected == numbertotal-1 )
            {
            UIContainer()->CandidatePane()->SelectIndex( 0 );
            }
        else
            {
            UIContainer()->CandidatePane()->SelectIndex( selected + 1 );
            }
        return ETrue;
        }
    else
        {
        TInt index = 0;
        TBool isIndex = MapKeyToIndexSelectCandidate( aKey, index );
        if ( aLength == EShortKeyPress && 
            ( ( isIndex && UIContainer()->CandidatePane()->SelectIndex( index ) ) ||
                aKey == EStdKeyDevice0 || 
                aKey == EStdKeyDevice3 ) )
            {
            editPane->SetNeedClearDeliberateSelection( ETrue );
            ComitPhraseCreatedToEditorL();
            return ETrue;
            }
        else
            {
            return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(
                aKey, aLength );
            }
        }
        return EFalse;
    }
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase::HandleCommandL(
    TInt aCommandId )
    {
    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate( );
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
    //    case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA            
            if ( text.Length( ) )
                {
                MAknFepManagerUIInterface* fepMan = iOwner->FepMan( );
                fepMan->NewTextL( text );
                fepMan->CommitInlineEditL( );
                iOwner->PtiEngine()->SetPredictiveChineseChar( text );
                if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
                    {
                    fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
                    iOwner->FepMan()->TryCloseUiL( );
                    }
                else
                    {
                    iOwner->ChangeState( EPredictiveCandidate );
                    }
                }
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }

// End of file
