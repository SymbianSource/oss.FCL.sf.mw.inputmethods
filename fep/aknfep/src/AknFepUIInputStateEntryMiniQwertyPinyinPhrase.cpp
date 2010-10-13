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
*       Provides the  TAknFepInputStateEntryMiniQwertyPinyinPhrase methods.
*
*/











#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepUIInputStateEntryMiniQwertyPinyinPhrase.h"
#include "aknfepuictrleditpane.h"           //pinyin phrase creation

#include "AknFepManager.h"                  //FepMan flag
#include "AknFepUICtrlInputPane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include <aknfep.rsg>
#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>

const TInt KManualChangeState = 0; 
//const TUint16 KSegment = 0x0020;
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyPinyinPhrase::TAknFepInputStateEntryMiniQwertyPinyinPhrase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateEntryMiniQwertyPinyinPhrase::TAknFepInputStateEntryMiniQwertyPinyinPhrase(
     MAknFepUIManagerStateInterface* aOwner,MAknFepUICtrlContainerChinese* aUIContainer ):
     TAknFepInputMiniQwertyPinyinPhraseBase( aOwner, aUIContainer)
    {
    iState = EEntry;
    iOwner->PtiEngine()->SetInputMode( EPtiEnginePinyinPhraseQwerty );
    iOwner->PtiEngine()->SetCase( EPtiCaseLower );
    UIContainer()->SetLayout( MAknFepUICtrlContainerChinese::ELayoutKeystroke );
    UIContainer()->FocusCandidatePane( ETrue );
    UIContainer()->CandidatePane()->SelectFirstPhrase();
    UIContainer()->CandidatePane()->ShowCandidateOrdinals( EFalse );
    UIContainer()->EditPaneWindow()->DeactiveCursor();
    UIContainer()->SetFepMan( iOwner->FepMan() );
    // so that it could handle EKeyCBA1
    iOwner->FepMan()->EntryPhrase( ETrue );
    RefreshUI( ETrue );
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyPinyinPhrase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
//     
TBool TAknFepInputStateEntryMiniQwertyPinyinPhrase::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    switch ( aKey )
        {
        case EStdKeyLeftFunc: //chr key
        case EStdKeySpace:
        case EStdKeyBackspace:
        case EStdKeyEnter:
            {
            ret = TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL( aKey,
                aLength );
            break;
            }
        case EKeyLeftShift:
        case EKeyRightShift:
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EKeyRightFunc://fn 
        case EKeyLeftFunc: // fn 
            {
            iOwner->FepMan()->TryCloseUiL();
            ret =EFalse;
            break;
            }
        case EStdKeyUpArrow:
            {
            if ( candidatePane->IsFirstPage() )
                {
                UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( KManualChangeState );
                iOwner->ChangeState( EMiniQwertyEdit );
                ret = ETrue;
                }
            else 
                {
                candidatePane->PreviousCandidatePage();
                candidatePane->SetCandidateBuffer();
                candidatePane->SelectFirst();
                UpdateIndicator();
                }
            break;
            }
        case EStdKeyDownArrow:
            {
            ret = TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL( aKey, aLength );
            break;
            }
        case EStdKeyRightArrow:
            {
            if ( candidatePane->VisibleCandidateCount()!= 1 )
                {
                iOwner->ChangeState( ECandidate );
                candidatePane->SelectNext();
                UpdateIndicator();
                }
            break;
            }
        case EStdKeyLeftArrow:
            {
            if ( candidatePane->VisibleCandidateCount()!= 1 )
                {
                iOwner->ChangeState( ECandidate );
                TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
                UIContainer()->CandidatePane()->SelectIndex( numbertotal - 1 );
                UpdateIndicator();
                }
            break;
            }
        case EStdKeyDevice0: //select key 
        case EStdKeyDevice3: // msk 
            {
            ret = TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL(
               aKey, aLength );
            break;
            }
        case EStdKeyDevice1:
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        case EStdKeySingleQuote:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
            break;
            }
        default:
               // chr + character 
            if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed )
                    || iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed ) )
                {
                iOwner->FepMan()->TryCloseUiL();
                ret = EFalse;
                }
            else if ( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() 
                    || CAknFepFnKeyManager::EFnKeyLock == iOwner->FepMan()->FnKeyState()
                    || CAknFepFnKeyManager::EFnKeyDown == iOwner->FepMan()->FnKeyState()
                    ||!iOwner->IsValidChineseInputKeyQwerty( aKey ) ) //fn play error tone
                    {
                    iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                    }
            else
                 {
                  ret = TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL(
                     aKey, aLength );
                 }
            break;
        }
    return ret;
    };
// ---------------------------------------------------------------------------
// TAknFepInputStateEditinigMiniQwertyPinyinPhrase::TAknFepInputStateEditinigMiniQwertyPinyinPhrase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateEditinigMiniQwertyPinyinPhrase::TAknFepInputStateEditinigMiniQwertyPinyinPhrase( 
    MAknFepUIManagerStateInterface* aOwner,MAknFepUICtrlContainerChinese* aUIContainer ):
    TAknFepInputMiniQwertyPinyinPhraseBase( aOwner,aUIContainer )                           
    {
    iState = EMiniQwertyEdit;
    iOwner->PtiEngine()->SetInputMode( EPtiEnginePinyinPhraseQwerty );
    iOwner->PtiEngine()->SetCase( EPtiCaseLower );
    RefreshUI( ETrue );
    UIContainer()->FocusCandidatePane( EFalse );
    UIContainer()->CandidatePane()->ShowCandidateOrdinals( EFalse );
    UIContainer()->EditPaneWindow()->ActiveCursor();
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateEditinigMiniQwertyPinyinPhrase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
// 
TBool TAknFepInputStateEditinigMiniQwertyPinyinPhrase::HandleKeyL( TInt aKey ,TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    if ( iOwner->FepMan()->GetComposChrFlag() && aKey == EStdKeyLeftFunc )
        {
        iOwner->FepMan()->SetComposChrFlag(EFalse);
        return ETrue;
        }
    switch ( aKey )
        {
        //the following key is handled by the base class 
        case EStdKeyLeftFunc: //chr key
        case EStdKeySpace:
        case EStdKeyBackspace:
        case EStdKeyEnter:
        case EStdKeyRightArrow:
        case EStdKeyLeftArrow:
            {
            ret = TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL(
                aKey, aLength);
            break;
            }
        case EKeyLeftShift:
        case EKeyRightShift:
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EKeyRightFunc://fn 
        case EKeyLeftFunc: // fn 
            {
            if ( !uiContainer->EditPaneWindow()->GetAutoChangeStateFlag()
                 || uiContainer->EditPaneWindow()->PhraseArray()->Count() != 0)
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
                ret = ETrue;
                }
            else
                {
                iOwner->FepMan()->TryCloseUiL();
                ret = EFalse;
                }
            break;
            }
    // inactive 
        case EStdKeyUpArrow:
            {
            break;
            }
            //if the canidate is empty ,inactive;else candiniate selecting state
        case EStdKeyDownArrow:
            {
            if ( 0 != candidatePane->VisibleCandidateCount() )
                {
                uiContainer->EditPaneWindow()->SetChangeState( ETrue );
                iOwner->ChangeState( ECandidate );
                }
            break;
            }
        case EStdKeyDevice0:
        case EStdKeyDevice3:
            {
            ret = TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL(
                aKey, aLength);
            break;
            }
          
        case EStdKeyDevice1:
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        case EStdKeySingleQuote:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
            break;
            }
        default:
            // chr + character 
            if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed )
                || iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed ) )
                {
                iOwner->FepMan()->SetComposChrFlag( ETrue );
                if ( !uiContainer->EditPaneWindow()->GetAutoChangeStateFlag()
                      || uiContainer->EditPaneWindow()->PhraseArray()->Count() != 0)
                    {
                    iOwner->FepMan()->PlaySound(EAvkonSIDStandardKeyClick);
                    return  ETrue;
                    }
                else
                    {
                    iOwner->FepMan()->TryCloseUiL();
                    return EFalse;
                    }
                }
            else
                {
                ret = TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL(
                    aKey, aLength);
                break;
                }
        }
    return ret;
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyPinyinPhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryMiniQwertyPinyinPhrase::HandleCommandL(
    TInt aCommandId )
    {
    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate( );
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan( );
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
    //    case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            if ( text.Length( ) )
                {
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
// ---------------------------------------------------------------------------
// TAknFepInputStateEditinigMiniQwertyPinyinPhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEditinigMiniQwertyPinyinPhrase::HandleCommandL(
    TInt aCommandId )
    {
    TAknFepInputMiniQwertyPinyinPhraseBase::HandleCommandL( aCommandId );
    }
// End Of File

