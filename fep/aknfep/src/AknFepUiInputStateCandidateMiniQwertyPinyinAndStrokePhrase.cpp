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
* Description:            Implementation of Pinyin and Stroke phrase candidate state
*
*/












// System includes
#include <PtiEngine.h>  
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote 
#include "AknFepUiManagerBase.h"

const TInt KMinCandidateCount = 1;
// User includes
#include "AknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "aknfepuictrleditpane.h"           //phrase creation
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManager.h" 
#include <aknfep.rsg>                 
#include "AknFepUiManagerChinese.h"

// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::
// TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::
    TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase(MAknFepUIManagerStateInterface* aOwner,
                                                      MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputMiniQwertyPinyinPhraseBase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > KMinCandidateCount;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    uiContainer->CandidatePane()->SelectFirstPhrase();
    UIContainer()->EditPaneWindow()->DisableCursor();
    }

// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::HandleKeyL(TInt aKey,
                                                                    TKeyPressLength aLength)
    {
    TInt selected = UIContainer()->CandidatePane()->SelectedIndex();
    TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();

    if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed )
        || iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed ) )
        {
        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
        return ETrue;
        }
    TBool ret = EFalse;
    switch ( aKey )
        {
        case EStdKeySpace:
            {
            if ( ECangJie ==iOwner->FepMan()->InputMode() )
                {
                TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
                if ( text.Length() )
                    {
                    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                    fepMan->NewTextL( text );
                    fepMan->CommitInlineEditL();
                    iOwner->PtiEngine()->SetPredictiveChineseChar( text );
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
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            ret = ETrue;
            }
            break;
        case EStdKeyBackspace:
            {
            UIContainer()->CandidatePane()->SetCandidateBuffer();
            UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EManualChangeState );
            UIContainer()->EditPaneWindow()->SetChangeState( ETrue );
            iOwner->ChangeState( EMiniQwertyEdit );
            iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
            ret = ETrue;
            }
            break;
        case EStdKeyLeftArrow:
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
            ret = ETrue;
            }
            break;
        case EStdKeyRightArrow:
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
            ret = ETrue;
            }
            break;
        case EStdKeyDownArrow:
            {
            ret = TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
            }
            break;
        case EStdKeyUpArrow:
            {
            if ( UIContainer()->CandidatePane()->IsFirstPage() )
                {
                UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EManualChangeState );
                iOwner->ChangeState( EMiniQwertyEdit );
                return ETrue;   
                }
            else
                {
                UIContainer()->CandidatePane()->PreviousCandidatePage();
                UIContainer()->CandidatePane()->SetCandidateBuffer();
                UIContainer()->CandidatePane()->SelectFirstPhrase();
                UpdateIndicator();
                }
            ret = ETrue;
            }
            break;
        case EStdKeyEnter:
            {
            if ( EStroke == iOwner->FepMan()->InputMode() ||
            ECangJie == iOwner->FepMan()->InputMode() ||
            EPinyin == iOwner->FepMan()->InputMode() ) 
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            ret = ETrue;
            }
            break;
        case EStdKeyDevice0:
        case EStdKeyDevice3:
            {
            TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
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
                        //stroke to be shown
                        if ( iPlugin.IsEnable())
                            {
                            iPlugin.AnalyseL();
                            }
                        RefreshUI( EFalse );
                        UIContainer()->CandidatePane()->SelectFirstPhrase();
                        }
                    else
                        {
                        //stroke to be shown
                        if ( iPlugin.IsEnable())
                            {
                            iPlugin.AnalyseL();
                            }
                        UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EAutoChangeStateFromCandidate );
                        iOwner->ChangeState( EMiniQwertyEdit );
                        }
                    }
                }
            }
            ret = ETrue;
            break;
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EKeyLeftShift:
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            ret = ETrue;
            }
            break;
        case EKeyRightShift:
            {
            ret = ETrue;
            }
            break;
        default:
            return TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL(
                aKey, aLength );
        }
    return ret;
    }
// ---------------------------------------------------------------------------
// TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::HandleCommandL(
    TInt aCommandId )
    {
    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate( );
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
       // case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
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
                        RefreshUI( EFalse );
                        UIContainer()->CandidatePane()->SelectFirstPhrase( );
                        }
                    else
                        {
                        UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EAutoChangeStateFromCandidate );
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
void TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase::SubmitTextL( const TDesC& aText )
	{
	 if ( aText.Length() )
		 {
		 if ( !CommitInlineEEPL( aText ) )
			{
			DoActionAfterCommit();
			}
		else
			{
			if ( CheckFirstGroupStroke() )
				{
                //stroke to be shown
                if ( iPlugin.IsEnable())
                    {
                    iPlugin.AnalyseL();
                    }
				RefreshUI( EFalse );
				UIContainer()->CandidatePane()->SelectFirstPhrase();
				}
			else
				{
                if ( iPlugin.IsEnable())
			        {
			        iPlugin.AnalyseL();
			        }
				UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EAutoChangeStateFromCandidate );
				iOwner->ChangeState( EMiniQwertyEdit );
				}
			}
		}
	}
// End of file
