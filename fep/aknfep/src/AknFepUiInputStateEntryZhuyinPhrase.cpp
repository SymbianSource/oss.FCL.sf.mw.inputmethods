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
*       Provides the CAknFepUIInputStateZhuyinPhrase methods.
*
*/












#include "AknFepUiInputStateEntryZhuyinPhrase.h"
#include "AknFepUIManagerStateInterface.h"
#include <PtiEngine.h>

#include <e32keys.h>
#include <PtiDefs.h>                //keys
#include <aknnotewrappers.h>
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"
#include "aknfepuictrleditpane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "aknfepzhuyinconvertion.h"
#include "aknfepzhuyinkeyhandlerinterface.h"
#include "AknFepZhuyinAnalyserDb.h"
#include "AknFepZhuyinAnalyser.h"
#include <PtiUserDicEntry.h>

const TInt KMaxPhraseCount = 50;
const TInt KInputPaneLength = 32;
const TUint16 KSegment = 0x0020;

const TInt KMaxBufLen = 100;
const TInt KInputPhraseMaxLength = 7;
_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );

TAknFepInputStateEntryZhuyinPhrase::TAknFepInputStateEntryZhuyinPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateZhuyinPhraseCreationBase(aOwner, aUIContainer)
    {
    iState = EEntry;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutKeystroke);
    uiContainer->CandidatePane()->SelectFirstPhrase(); 
    uiContainer->InputPane()->SetOverrideFontId(0);

    if (iOwner->PtiEngine()->InputMode() != EPtiEngineZhuyinByPhrase)
        {
        iOwner->PtiEngine()->SetInputMode(EPtiEngineZhuyinByPhrase);
        }
    iOwner->PtiEngine()->SetCandidatePageLength(KMaxPhraseCount);
    iOwner->FepMan()->EntryPhrase(ETrue);
    uiContainer->SetFepMan(iOwner->FepMan());
    uiContainer->EditPaneWindow()->DeactiveCursor();
    UpdateEEPContent();
    RefreshUI();
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryZhuyinPhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryZhuyinPhrase::HandleCommandL( TInt aCommandId )
    {
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeyCancel:
            //  case (TUint16)EAknSoftkeyCancel: //the Cancle in soft CBA
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            HandleCommitL();
            break;
        default:
            TAknFepInputStateZhuyinPhraseCreationBase::HandleCommandL(
                aCommandId );
            break;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryZhuyinPhrase::HandleCommit 
// Handle CBA1,ok and select candidate phrase key press.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryZhuyinPhrase::HandleCommitL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();

    CDesCArrayFlat* zhuyinSymbols =
        uiContainer->PinyinPopupWindow()->ZhuyinSymbols();
    CDesCArrayFlat* chineseDisped =
        uiContainer->PinyinPopupWindow()->ChineseCharsDisped();
    CDesCArrayFlat
        * zhuyinSymbolsForChinese =
            uiContainer->PinyinPopupWindow()->ZhuyinSymbolsForChineseCharsDisped();
    CDesCArrayFlat* candidateList =
        uiContainer->CandidatePane()->CandidateArray();
    MZhuyinKeyHandler* zhuyinKeyHandler = iOwner->ZhuyinKeyHandler();

    TPtrC text = candidatePane->CurrentPhraseCandidate();
    if ( text.Length() == 0 )
        {
        return;
        }
    TBuf<KMaxBufLen> pinyinshown =
        UIContainer()->PinyinPopupWindow()->GetEEPContent();
    TInt count = TZhuyinSymbolsMethods::GetChineseCount( pinyinshown );
    if ( count > text.Length() )
        {
        fepMan->PhraseCreation( ETrue );
        }
    if ( fepMan->IsPhraseCreation() )
        {
        TBuf<KMaxBufLen> buf;
        buf.Copy( text );
        TBool commit = TZhuyinSymbolsMethods::AppendChineseChar( buf,
            fepMan->ZhuyinAnalyser(), zhuyinSymbols, chineseDisped,
            zhuyinSymbolsForChinese );
        TBool changeState = UpdateEEPContent();
        if ( commit )
            {
            TBuf<KMaxBufLen> commitbuf;
            TZhuyinSymbolsMethods::Convert2Des( chineseDisped, commitbuf );
            fepMan->NewTextL( commitbuf );
            fepMan->CommitInlineEditL();
            AddPhraseToDB( commitbuf );
            if ( chineseDisped->Count() < KInputPhraseMaxLength
                || zhuyinSymbols->Count() == 0 )
                {
                iOwner->FepMan()->TryCloseUiL();
                }
            else
                {
                CAknWarningNote* errnote = new (ELeave) CAknWarningNote();
                errnote->SetTone( CAknNoteDialog::EWarningTone );
                errnote->ExecuteLD( KMaxPhraseNote );
                fepMan->TryCloseUiL();
                }
            popup->ResetStrokeArray();
            }
        else
            {
            editPane->SetCursorIndexOfKeystroke( 0 );
            if ( changeState )
                {
                for (TInt i = 0; i < candidateList->Count(); i++ )
                    {
                    if ( TZhuyinSymbolsMethods::IsZhuyinSymbol(
                        candidateList->MdcaPoint( i ) ) )
                        {
                        candidateList->Delete( i, 1 );
                        break;
                        }
                    }
                if ( candidateList->Count() > 0 )
                    {
                    popup->SetChangeState( ETrue );
                    iOwner->ChangeState( EEntry );
                    }
                else
                    {
                    popup->SetChangeState( ETrue );
                    iOwner->ChangeState( EZhuyinSequenceEdit );
                    }
                }
            else
                {
                popup->SetChangeState( ETrue );
                iOwner->ChangeState( EZhuyinSequenceEdit );
                }
            }
        }
    else
        {
        TBuf<KMaxBufLen> textbuf;
        for (TInt i = 0; i < chineseDisped->Count(); i++ )
            {
            textbuf.Append( chineseDisped->MdcaPoint( i ) );
            }
        textbuf.Append( text );
        fepMan->NewTextL( textbuf );
        fepMan->CommitInlineEditL();
        AddPhraseToDB( textbuf );
        popup->ResetStrokeArray();
        if ( text.Length() <= KInputPhraseMaxLength
            && !fepMan->IsPhraseCreation() )
            {
            if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
                {
                fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
                iOwner->FepMan()->TryCloseUiL();
                }
            else
                {
                iOwner->ChangeState( EPredictiveCandidate );
                }
            }
        else
            {
            fepMan->TryCloseUiL();
            }
        }
    }

void TAknFepInputStateEntryZhuyinPhrase::AddPhraseToDB( TDes& aPhrase )
    {
    TPtiUserDictionaryEntry addUdbEntry( aPhrase );
    //Add the phrase to the DB by PTI Engine
    iOwner->PtiEngine()->AddUserDictionaryEntry( addUdbEntry );
    }

TBool TAknFepInputStateEntryZhuyinPhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MZhuyinKeyHandler* zhuyinKeyHandler = iOwner->ZhuyinKeyHandler();
    
    CDesCArrayFlat* zhuyinSymbols = uiContainer->PinyinPopupWindow()->ZhuyinSymbols();
    CDesCArrayFlat* chineseDisped = uiContainer->PinyinPopupWindow()->ChineseCharsDisped();

    iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);            
    
    if((aKey == EPtiKeyStar) && (aLength == EShortKeyPress))
        {
        AddToneMarkL();
        if ( !UpdateEEPContent( ) )
            {
            RefreshUI();
            ChangeCbaL();
            popup->SetChangeState( ETrue );
            iOwner->ZhuyinKeyHandler()->SetState(1);
            iOwner->ChangeState(EZhuyinSequenceEdit);
            return ETrue;
            }
        RefreshUI();
        ChangeCbaL();
        return ETrue;
        }
    else if (aKey == EKeyLeftArrow )
        {
        iOwner->ChangeState(ECandidate);
        return EFalse;
        }
    else if (aKey == EKeyRightArrow)
        {
        iOwner->ChangeState(ECandidate);
        return EFalse;
        }
    else 
        {
        if(aKey == EKeyBackspace)
            {
            if ( aLength == ELongKeyPress
                && zhuyinKeyHandler->GetStatusChgFlag() == MZhuyinKeyHandler::EFromCandidateToEntry )
                {
                return ETrue;
                }
            else
                {
                zhuyinKeyHandler->SetStatusChgFlag(MZhuyinKeyHandler::EInit);
                }
            
            DeleteZhuyinSymbolOrChinese();
            
            if ( !UpdateEEPContent( ) )
                {
                RefreshUI();
                popup->SetChangeState( ETrue );
                iOwner->ZhuyinKeyHandler()->SetState(1);
                ChangeCbaL();
                iOwner->ChangeState(EZhuyinSequenceEdit);
                if ( zhuyinSymbols->Count() == 0
                    && chineseDisped->Count() == 0 )
                    {
                    iOwner->FepMan()->TryCloseUiL();
                    if (aLength == ELongKeyPress)
                        {
                        iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                        }
                    }
                if ( 0 == chineseDisped->Count() )
                    {
                    iOwner->FepMan()->PhraseCreation( EFalse );
                    }
                return ETrue;
                }
            ChangeCbaL();
            RefreshUI();
            if ( zhuyinSymbols->Count() == 0
                && chineseDisped->Count() == 0 )
                {
                iOwner->FepMan()->TryCloseUiL();
                if (aLength == ELongKeyPress)
                    {
                    iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                    }
                }
            if ( 0 == chineseDisped->Count() )
                {
                iOwner->FepMan()->PhraseCreation( EFalse );
                }
            }
        else if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKey(aKey))
            {
            if ( AddKeystrokeL( aKey ) )
                {
                if ( !UpdateEEPContent( ) )
                    {
                    RefreshUI();
                    popup->SetChangeState( ETrue );
                    iOwner->ZhuyinKeyHandler()->SetState(1);
                    iOwner->ChangeState(EZhuyinSequenceEdit);
                    return ETrue;
                    }
                RefreshUI();
                ChangeCbaL();
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            return ETrue;
            }
        else if(aKey == EKeyDownArrow)
            {
            if ( aLength == ELongKeyPress
                && zhuyinKeyHandler->GetStatusChgFlag() == MZhuyinKeyHandler::EFromSpellingEditingToEntry )
                {
                return ETrue;
                }
            else if (!candidatePane->IsLastPage())
               {
               zhuyinKeyHandler->SetStatusChgFlag(MZhuyinKeyHandler::EInit);
               if ( candidatePane->NeedGetNewCandidatePage() )
                   {
                   if ( !iOwner->PtiEngine()->NextCandidatePage( ) )
                       {
                       if ( candidatePane->IsLastPage() )
                           {
                           iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                           return ETrue;
                           }
                       }
                   else
                       {
                       CDesCArrayFlat* phraseCandidates = candidatePane->CandidateArray();
                       RPointerArray<HBufC> list;
                       GetNewCandidatesL( iOwner->PtiEngine()->CandidatePage( ), list );
                       for (TInt i = 0; i < list.Count( ); ++i )
                           {
                           phraseCandidates->AppendL( *(list[i]) );
                           }
                       list.ResetAndDestroy();
                       }
                   }           
               candidatePane->NextCandidatePage();
               RefreshUI();
               }
            }
        else if(aKey == EKeyUpArrow)
            {
            if(!candidatePane->IsFirstPage())
                {
                candidatePane->PreviousCandidatePage();
                RefreshUI();
                }
            else
                {
                iOwner->ZhuyinKeyHandler()->SetState(0);
                popup->SetState( EFalse );
                iOwner->ChangeState(EZhuyinSequenceEdit);
                return EFalse;
                }
            }   
        else if ( (aKey == EKeyOK || aKey == EKeyCBA1 || aKey == EKeyRightArrow || aKey == EKeyLeftArrow)
            && aLength == EShortKeyPress )
            {
            iOwner->ChangeState(ECandidate);
            return EFalse;
            }
        return ETrue;
        }
    }

void TAknFepInputStateEntryZhuyinPhrase::KeyTimerExpired()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    
    // set container's position
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }

    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);
    TBuf<KInputPaneLength> currentText;
    inputPane->GetText(currentText);
    if (ptr.Length() < currentText.Length())
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
    UpdateIndicator();
    }

void TAknFepInputStateEntryZhuyinPhrase::GetNewCandidatesL(const TDesC& aSource, 
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
