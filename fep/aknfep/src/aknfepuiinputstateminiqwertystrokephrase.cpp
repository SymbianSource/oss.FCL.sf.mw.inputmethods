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
 *       Provides the  TAknFepInputStateEntryMiniQwertyStrokePhrase,
 *       TAknFepInputStateEditingMiniQwertyStrokePhrase,
 *       TAknFepUiInputStateCandidateMiniQwertyStrokePhrase methods.
 *
*/












// System includes
#include <PtiEngine.h>  
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote
#include <aknfep.rsg>  

// User includes
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "aknfepuictrleditpane.h"           //phrase creation
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManager.h" 
#include "aknfepuiinputstateminiqwertystrokephrase.h"
const TInt KManualChangeState = 0; 

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyStrokePhrase::TAknFepInputStateEntryMiniQwertyStrokePhrase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateEntryMiniQwertyStrokePhrase::TAknFepInputStateEntryMiniQwertyStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ):
    TAknFepInputMiniQwertyStrokePhraseBase( aOwner, aUIContainer )
    {
    iState = EEntry;
    UIContainer()->SetLayout( MAknFepUICtrlContainerChinese::ELayoutKeystroke );
    UIContainer()->FocusCandidatePane( ETrue );
    UIContainer()->CandidatePane()->SelectFirstPhrase();
    UIContainer()->CandidatePane()->ShowCandidateOrdinals( EFalse );
    UIContainer()->SetFepMan( iOwner->FepMan() );
    // so that it could handle EKeyCBA1
    iOwner->FepMan()->EntryPhrase( ETrue );
    RefreshUI();
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyStrokePhraseCreation::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
// 
TBool TAknFepInputStateEntryMiniQwertyStrokePhrase::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    switch ( aKey )
        {
        case EStdKeyLeftFunc: //chr key
        case EKeyLeftShift:
        case EKeyRightShift:
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EKeyRightFunc://fn 
        case EKeyLeftFunc: // fn 
            {
            iOwner->FepMan()->TryCloseUiL();
            ret = EFalse;
            break;
            }
        case EStdKeyDevice1: // cancel key
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        case EStdKeyEnter:
        case EStdKeySingleQuote: // The "&" key
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
            break;
            }
        case EStdKeyUpArrow:
            {
            if ( candidatePane->IsFirstPage() )
                {
                UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( KManualChangeState );
                iOwner->ChangeState( EMiniQwertyEdit );
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
                uiContainer->EditPaneWindow()->SetCandidateRefreshFlag( ETrue );
                iOwner->ChangeState( ECandidate );
                candidatePane->SelectNext();
                }
            break;
            }
        case EStdKeyLeftArrow:
            {
            if ( candidatePane->VisibleCandidateCount()!= 1 )
                {
                uiContainer->EditPaneWindow()->SetCandidateRefreshFlag( ETrue );
                iOwner->ChangeState( ECandidate );
                TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
                UIContainer()->CandidatePane()->SelectIndex( numbertotal-1 );
                }
            break;
            }
        case EStdKeyDevice0: //select key 
        case EStdKeyDevice3: // msk 
            {
            TPtrC text = uiContainer->CandidatePane()->CurrentPhraseCandidate();
            if ( text.Length() && aLength != ELongKeyPress )
                {
                if ( !CommitInlineEEPL( text ) )
                    {
                    DoActionAfterCommit();
                    }
                else
                    {
                    if ( CheckFirstGroupStroke() )
                        {
                        iOwner->ChangeState( ECandidate );
                        }
                    else
                        {
                        uiContainer->EditPaneWindow()->SetAutoChangeStateFlag( EAutoChangeStateFromInput );
                        iOwner->ChangeState( EMiniQwertyEdit );
                        }
                    }
                }
            break;
            }
        default:
            // chr + character 
            if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed ) )
                {
                iOwner->FepMan()->TryCloseUiL();
                ret = EFalse;
                }
            else if ( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() || CAknFepFnKeyManager::EFnKeyLock
                == iOwner->FepMan()->FnKeyState() || CAknFepFnKeyManager::EFnKeyDown
                == iOwner->FepMan()->FnKeyState() ) //fn play error tone
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            else
                {
                ret = TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyL(
                    aKey, aLength );
                }
            break;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyStrokePhrase::TAknFepInputStateEditingMiniQwertyStrokePhrase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateEditingMiniQwertyStrokePhrase::TAknFepInputStateEditingMiniQwertyStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ):
    TAknFepInputMiniQwertyStrokePhraseBase( aOwner, aUIContainer )
    {
    iState = EMiniQwertyEdit;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    RefreshUI();
    uiContainer->FocusCandidatePane( EFalse );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyStrokePhrase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
// 
TBool TAknFepInputStateEditingMiniQwertyStrokePhrase::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    
    //handle chr up when composition key with chr end
    //avoid closing UI
    if ( iOwner->FepMan()->GetComposChrFlag() && aKey == EStdKeyLeftFunc )
        {
        iOwner->FepMan()->SetComposChrFlag( EFalse );
        return ETrue;
        }
    switch ( aKey )
        {
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
                }
            else
                {
                iOwner->FepMan()->TryCloseUiL();
                ret = EFalse;
                }
            break;
            }
        case EStdKeyLeftFunc: //chr key
            {
            iOwner->FepMan()->TryCloseUiL();
            ret =EFalse;
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
        case EStdKeyDevice1: //"cancle" key
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        case EStdKeySingleQuote:
        case EStdKeyEnter:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
            break;
            }
        default:
            // chr + character 
            if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed ) )
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
                ret = TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyL(
                    aKey, aLength);
                break;
                }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyStrokePhrase::TAknFepUiInputStateCandidateMiniQwertyStrokePhrase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidateMiniQwertyStrokePhrase::TAknFepUiInputStateCandidateMiniQwertyStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ):
    TAknFepInputMiniQwertyStrokePhraseBase( aOwner, aUIContainer )
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane( ETrue );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( ETrue );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    if ( uiContainer->EditPaneWindow()->GetCandidateRefreshFlag() )
        {
        uiContainer->EditPaneWindow()->DisableCursor();
        uiContainer->EditPaneWindow()->SetCandidateRefreshFlag( EFalse );
        }
    else
        {
        RefreshUI();
        }
    }
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyStrokePhrase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateMiniQwertyStrokePhrase::HandleKeyL(
    TInt aKey, TKeyPressLength aLength )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TInt selected = uiContainer->CandidatePane()->SelectedIndex();
    TInt numbertotal = uiContainer->CandidatePane()->VisibleCandidateCount();
    TBool ret = ETrue;

    if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed )
        || iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed ) )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return ETrue;
        }

    switch ( aKey )
        {
        case EStdKeyBackspace:
            {
            uiContainer->CandidatePane()->SetCandidateBuffer();
            uiContainer->EditPaneWindow()->SetAutoChangeStateFlag( KManualChangeState );
            uiContainer->EditPaneWindow()->SetChangeState( ETrue );
            iOwner->ChangeState( EMiniQwertyEdit );
            iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
            }
            break;
        case EStdKeyLeftArrow:
            {
            if ( selected == 0 )
                {
                uiContainer->CandidatePane()->SelectIndex( numbertotal-1 );
                }
            else
                {
                uiContainer->CandidatePane()->SelectIndex( selected - 1 );
                }
            }
            break;
        case EStdKeyRightArrow:
            {
            if ( selected == numbertotal-1 )
                {
                uiContainer->CandidatePane()->SelectIndex( 0 );
                }
            else
                {
                uiContainer->CandidatePane()->SelectIndex( selected + 1 );
                }
            }
            break;
        case EStdKeyDownArrow:
            {
            ret = TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL( aKey, aLength );
            }
            break;
        case EStdKeyUpArrow:
            {
            if ( uiContainer->CandidatePane()->IsFirstPage() )
                {
                uiContainer->EditPaneWindow()->SetAutoChangeStateFlag( KManualChangeState );
                iOwner->ChangeState( EMiniQwertyEdit );
                }
            else
                {
                uiContainer->CandidatePane()->PreviousCandidatePage();
                uiContainer->CandidatePane()->SetCandidateBuffer();
                uiContainer->CandidatePane()->SelectFirstPhrase();
                UpdateIndicator();
                }
            }
            break;
        case EStdKeyDevice0:
        case EStdKeyDevice3:
            {
            TPtrC text = uiContainer->CandidatePane()->CurrentPhraseCandidate();
            if ( text.Length() && aLength != ELongKeyPress )
                {
                if ( !CommitInlineEEPL( text ) )
                    {
                    DoActionAfterCommit();
                    }
                else
                    {
                    if ( CheckFirstGroupStroke() )
                        {
                        RefreshUI();
                        uiContainer->CandidatePane()->SelectFirstPhrase();
                        }
                    else
                        {
                        uiContainer->EditPaneWindow()->SetAutoChangeStateFlag( EAutoChangeStateFromCandidate );
                        iOwner->ChangeState( EMiniQwertyEdit );
                        }
                    }
                }
            }
            break;
        case EStdKeyEnter:
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EKeyLeftShift:
        case EStdKeySpace:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            }
            break;
//        case EKeyRightShift:   // need to check later
//            {
//            return ret;
//            }
//            break;
        default:
            return TAknFepInputMiniQwertyStrokePhraseBase::HandleKeyL( aKey,
                aLength );
        }
    return ret;
    }
// ---------------------------------------------------------------------------
// TAknFepInputStateEditingMiniQwertyStrokePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEditingMiniQwertyStrokePhrase::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer( );
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:            
            if ( !uiContainer->EditPaneWindow()->GetAutoChangeStateFlag( ) || uiContainer->EditPaneWindow()->PhraseArray()->Count( ) != 0 )
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDStandardKeyClick );
                }
            else
                {
                iOwner->FepMan()->TryCloseUiL( );
                }
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }


// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyStrokePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryMiniQwertyStrokePhrase::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TPtrC text = uiContainer->CandidatePane()->CurrentPhraseCandidate();
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            if ( text.Length() )
                {
                if ( !CommitInlineEEPL( text ) )
                    {
                    DoActionAfterCommit();
                    }
                else
                    {
                    if ( CheckFirstGroupStroke() )
                        {
                        iOwner->ChangeState( ECandidate );
                        }
                    else
                        {
                        uiContainer->EditPaneWindow()->SetAutoChangeStateFlag(
                            EAutoChangeStateFromInput );
                        iOwner->ChangeState( EMiniQwertyEdit );
                        }
                    }
                }
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyStrokePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateMiniQwertyStrokePhrase::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TPtrC text = uiContainer->CandidatePane()->CurrentPhraseCandidate();
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            if ( text.Length() )
                {
                if ( !CommitInlineEEPL( text ) )
                    {
                    DoActionAfterCommit();
                    }
                else
                    {
                    if ( CheckFirstGroupStroke() )
                        {
                        RefreshUI();
                        uiContainer->CandidatePane()->SelectFirstPhrase();
                        }
                    else
                        {
                        uiContainer->EditPaneWindow()->SetAutoChangeStateFlag(
                            EAutoChangeStateFromCandidate );
                        iOwner->ChangeState( EMiniQwertyEdit );
                        }
                    }
                }
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }
