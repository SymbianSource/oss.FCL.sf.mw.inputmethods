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
* Description:            Implementation of Stroke phrase entry state
*
*/












// System includes
#include <PtiEngine.h>
#include <aknnotewrappers.h> //CAknWarningNote 

// User includes
#include "AknFepUiInputStateEntryStrokePhrase.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepUICtrlInputPane.h"
#include "aknfepuictrleditpane.h" 
#include "AknFepManager.h"

// const param
const TInt KMinCnadidateCount = 1;
const TInt KMaxKeystrokeCount = 31;
const TInt KMaxPhraseCreationCount = 7;
const TInt KManualChangeState = 0; 
const TInt KChangeStatebyBackSpce = 0x1000;//set popup flag for change state.
//const TUint16 KSegment = 0x0020;
_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );
_LIT( KDelimiter, "\x2022");

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryStrokePhrase::
// TAknFepInputStateEntryStrokePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStateEntryStrokePhrase::TAknFepInputStateEntryStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer) :
    TAknFepInputStateStrokePhraseBase( aOwner, aUIContainer)
    {  
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    
    iState = EEntry;
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutKeystroke );
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    candidatePane->SetHighlighted(ETrue);
    candidatePane->ShowCandidateOrdinals(EFalse);
    candidatePane->SelectFirstPhrase();
    uiContainer->InputPane()->SetOverrideFontId( 0 );
    iOwner->FepMan()->EntryPhrase( ETrue );

    RefreshUI( ETrue );
 	}

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryStrokePhrase::HandleKeyL( 
                                                    TInt aKey,
                                                    TKeyPressLength aLength )
    {
    TBool ret = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    switch ( aKey )
        {
        case EPtiKeyStar:
            {
            break;
            }
        case EKeyUpArrow:
            {
            if ( !candidatePane->IsFirstPage() )
                {
                candidatePane->PreviousCandidatePage();  
                RefreshCandidate();
                }
            else
                {
                popup->SetState( EFalse );
                popup->SetState( KManualChangeState );
                popup->SetChangeState( ETrue );
                //change state to sequence edit
                iOwner->ChangeState( EStrokeSequenceEdit );
                }
            break;
            }
        case EKeyDownArrow:
            {
            if ( CheckKeyNeedRepeat( aLength ) )
                {
                if ( candidatePane->IsLastPage( ) )
                    {
                    return ETrue;
                    }
                if ( candidatePane->NeedGetNewCandidatePage( ) )
                    {
                    if ( !iOwner->PtiEngine()->NextCandidatePage( ) )
                        {
                        if ( candidatePane->IsLastPage( ) )
                            {
                            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                            return ETrue;
                            }
                        }
                    else
                        {
                        CDesCArrayFlat* phraseCandidates =
                            candidatePane->CandidateArray( );
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
                candidatePane->SetCandidateBuffer();
                candidatePane->SelectFirstPhrase();
                RefreshCandidate();
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        case EKeyLeftArrow:
        case EKeyRightArrow:
            {
            popup->SetChangeState( ETrue );
            iOwner->ChangeState( ECandidate );
            ret = EFalse;
            break;
            }
        case EKeyCBA1:
        case EKeyOK:
            {
            if ( CheckKeyNeedRepeat( aLength ) )
                {
                popup->SetChangeState( ETrue );
                iOwner->ChangeState( ECandidate );
                ret = EFalse;
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        default:
            return TAknFepInputStateStrokePhraseBase::HandleKeyL( aKey, aLength );
        }
    
    return ret;
    }


// ---------------------------------------------------------------------------
// TAknFepInputStateEntryStrokePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryStrokePhrase::HandleCommandL( TInt aCommandId )
    {
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            HandleCommitL();
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryStrokePhrase::HandleCommit 
// Handle CBA1,ok and select candidate phrase key press.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryStrokePhrase::HandleCommitL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    CDesCArrayFlat* phrase = popup->ChooseChineseCharacterArrayForStroke();
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    if( text.Length() ) 
        {
        if ( CommitInlineEEPL( text ) )
            {
            if ( phrase->Count() >= KMaxPhraseCreationCount )
                {
                if ( keystrokeArray->Count() != 0 )
                    {
                    CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
                    errnote->SetTone( CAknNoteDialog::EWarningTone );
                    errnote->ExecuteLD( KMaxPhraseNote );
                    }
                else
                    {
                    iOwner->FepMan()->PhraseCreation( EFalse );
                    iOwner->FepMan()->EntryPhrase( EFalse );
                    iOwner->FepMan()->PinyinPhraseCreation( EFalse );
                    iOwner->FepMan()->EnableKeyStar( ETrue );
                    iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                    }
                }
            else
                {
                if ( !iOwner->FepMan()->IsPhraseCreation() )
                    {
                    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagEditorFull))
                        {
                        iOwner->FepMan()->ClearFlag(CAknFepManager::EFlagEditorFull);
                        iOwner->FepMan()->TryCloseUiL();
                        }
                    else
                        {
                        iOwner->ChangeState( EPredictiveCandidate );
                        }
                    }
                else
                    {
                    iOwner->FepMan()->PhraseCreation( EFalse );
                    iOwner->FepMan()->EntryPhrase( EFalse );
                    iOwner->FepMan()->PinyinPhraseCreation( EFalse );
                    iOwner->FepMan()->EnableKeyStar( ETrue );
                    iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                    }
                }
            popup->ResetStrokeArray();
            }
        else
            {
            editPane->SetCursorIndexOfKeystroke( 0 );
            if ( CheckFirstGroupStroke() )
                {
                popup->SetState( ETrue );
                iOwner->ChangeState( EEntry );
                }
            else
                {
                popup->SetState( ETrue );
                //change state to sequence edit
                iOwner->ChangeState( EStrokeSequenceEdit );
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryStrokePhrase::CommitInlineEEPL 
// Commit phrase to the EEP ctrl.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryStrokePhrase::CommitInlineEEPL( 
                                                        const TDesC& aText )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    CDesCArrayFlat* phrase = popup->ChooseChineseCharacterArrayForStroke();
    CDesCArrayFlat* phraseKeystroke = 
        popup->ChooseChineseCharacterArrayKeystrokeForStroke();
    
    TBuf<KMaxPhraseCreationCount> phraseCreated;
    
    TInt textCount = aText.Length();
    TInt allTextCount = 0;
    TInt i = 0;
    TInt j = 0;
    // append character to chinese array
    for ( i = 0; i < textCount; i++ )
        {
        phrase->AppendL( aText.Mid( i, 1 ) );
        }
    
    TInt characterCount = phrase->Count();
    
    // if chinese character is more than 7 or not
    // if more than 7 show a warning not
    if ( characterCount >= KMaxPhraseCreationCount )
        {
        for ( i = 0; i < KMaxPhraseCreationCount; ++i )
            {
            phraseCreated.Append( phrase->MdcaPoint(i) );
            }
        AddPhraseToDB( phraseCreated );
        iOwner->FepMan()->NewTextL( phraseCreated );
        iOwner->FepMan()->CommitInlineEditL();
        }
    
    // calculate the total number of character according to stroke
    for ( i = 0; i < keystrokeArray->Count(); i++ )
        {
        if ( keystrokeArray->MdcaPoint( i ) == KDelimiter )
            {
            allTextCount = allTextCount + 1;
            }
        }
    
    allTextCount = allTextCount + 1;
    if ( keystrokeArray->MdcaPoint( keystrokeArray->Count() - 1 ) ==  
        KDelimiter )
        {
        allTextCount--;
        }
    
    // if total number is less than the character's count which user has selected
    // commit the character to editor and user db   
    if ( allTextCount <= textCount && characterCount < KMaxPhraseCreationCount )
        {
        for ( i = 0; i < characterCount; ++i )
            {
            phraseCreated.Append( phrase->MdcaPoint(i) );
            } 
        iOwner->FepMan()->NewTextL( phraseCreated );
        iOwner->FepMan()->CommitInlineEditL();
        AddPhraseToDB( phraseCreated );
        return ETrue;
        }
    
    // add the stroke to chosen character array
    // delete key stroke which charactor is corresponding
    for ( j = 0; j < textCount; j++ )
        {
        TInt keyCount = 0;
        TInt ret = keystrokeArray->Find( KDelimiter , 
                                         keyCount, 
                                         ECmpFolded );
        
        if ( ret != 0)
            {
            keyCount = keystrokeArray->Count();
            j = textCount;
            }
        else
            {
            keyCount++;
            }

        TBuf<KMaxKeystrokeCount> keystroke;
        for ( i = 0; i < keyCount; i++ )
            {
            keystroke.Append( keystrokeArray->MdcaPoint( 0 ) );
            keystrokeArray->Delete( 0 );
            }
        phraseKeystroke->AppendL( keystroke );
        }    
    if ( keystrokeArray->Count() > 0 )
        {
        iOwner->FepMan()->PhraseCreation( ETrue );
        }
    if ( characterCount >= KMaxPhraseCreationCount )
        {
        return ETrue;
        }

    return EFalse;
    }

// End of file

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
TAknFepInputStateEditStrokePhrase::TAknFepInputStateEditStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputStateStrokePhraseBase( aOwner, aUIContainer)
    {
    iState = EStrokeSequenceEdit;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->FocusCandidatePane( EFalse );
    uiContainer->CandidatePane()->ShowCandidateOrdinals( EFalse );
    uiContainer->SetLayout( MAknFepUICtrlContainerChinese::ELayoutKeystroke );
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->ShowVerticalScrollArrows( ETrue );
    uiContainer->ShowHorizontalScrollArrows( EFalse );
    uiContainer->InputPane()->SetOverrideFontId( 0 );

    RefreshUI();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEditStrokePhrase::HandleKeyL
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEditStrokePhrase::HandleKeyL( 
                                                    TInt aKey,
                                                    TKeyPressLength aLength )
    {
    return TAknFepInputStateStrokePhraseBase::HandleKeyL( 
                                                   aKey, 
                                                   aLength );
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateEntryStrokePhraseCreation::
// TAknFepInputStateEntryStrokePhraseCreation
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStateCandidateStrokePhrase::TAknFepInputStateCandidateStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateStrokePhraseBase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > KMinCnadidateCount;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->EditPaneWindow()->DisableCursor();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateStrokePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCandidateStrokePhrase::HandleKeyL( 
                                        TInt aKey, 
                                        TKeyPressLength /*aLength*/ )
    {
    TBool ret = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    // it may be one of the 'valid' numbers..
    TInt index = MapKeyToIndex(aKey);

    switch( aKey )
        {
        case EKeyBackspace:
            {
            editPane->EnableCursor();
            popup->SetChangeState( ETrue );
            iOwner->ChangeState( EEntry );
            popup->SetFlag( KChangeStatebyBackSpce );
            break;
            }
        case EKeyDownArrow:
            {
            if ( candidatePane->IsLastPage() )
                {
                return ETrue;
                }
            if ( candidatePane->NeedGetNewCandidatePage( ) )
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
                    CDesCArrayFlat* phraseCandidates =
                        candidatePane->CandidateArray( );
                    RPointerArray<HBufC> list;
                    GetNewCandidatesL( iOwner->PtiEngine()->CandidatePage( ), list );
                    for (TInt i = 0; i < list.Count( ); ++i )
                        {
                        phraseCandidates->AppendL( *(list[i]) );
                        }
                    list.ResetAndDestroy();
                    }
                }
            candidatePane->NextCandidatePage( );
            RefreshCandidate( );
            break;
            }
        case EKeyUpArrow:
            {
            if( !candidatePane->IsFirstPage() )
                {
                candidatePane->PreviousCandidatePage();
                RefreshCandidate();
                }
            else
                {
                popup->SetChangeState( ETrue );
                popup->SetState( EFalse );
                iOwner->ChangeState( EStrokeSequenceEdit );
                }
            break;
            }
        case EKeyLeftArrow:
        case EKeyRightArrow:
            {
            HandleHorizontalNavigation( aKey );
            break;
            }
        case EPtiKeyStar:
            {
            break;
            }
        case EKeyCBA2:
            {
            popup->ResetStrokeArray();
            iOwner->FepMan()->EntryPhrase( EFalse );
            iOwner->FepMan()->PhraseCreation( EFalse );
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        default:
            {
            if ( ( aKey == EKeyOK ) || aKey == EKeyCBA1 ||
                        candidatePane->SelectIndex( index,EFalse ) )
                {
                HandleCommitL();
                }
            else
                {
                iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                }
            break;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateStrokePhrase::HandleHorizontalNavigation 
// Handle Horizontal Navigation.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateStrokePhrase::HandleHorizontalNavigation(
                                                                   TInt aKey )
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    // Do navigation...
    if (aKey == EKeyLeftArrow)
        {        
        if(!candidatePane->SelectPrev())
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        }
    else if (aKey == EKeyRightArrow)
        {              
        if(!candidatePane->SelectNext())
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirstPhrase();
            }        
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateStrokePhrase::HandleCommit 
// Handle CBA1,ok and select candidate phrase key press.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateStrokePhrase::HandleCommitL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    CDesCArrayFlat* phrase = popup->ChooseChineseCharacterArrayForStroke();
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    if( text.Length() ) 
        {
        if ( CommitInlineEEPL( text ) )
            {
            if ( phrase->Count() <= KMaxPhraseCreationCount && !iOwner->FepMan()->IsPhraseCreation() )
                {
                if ( !iOwner->FepMan()->IsPhraseCreation( ) )
                    {
                    if ( iOwner->FepMan()->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
                        {
                        iOwner->FepMan()->ClearFlag( CAknFepManager::EFlagEditorFull );
                        iOwner->FepMan()->TryCloseUiL( );
                        }
                    else
                        {
                        iOwner->ChangeState( EPredictiveCandidate );
                        }
                    }
                else
                    {
                    iOwner->FepMan()->PhraseCreation( EFalse );
                    iOwner->FepMan()->EntryPhrase( EFalse );
                    iOwner->FepMan()->PinyinPhraseCreation( EFalse );
                    iOwner->FepMan()->EnableKeyStar( ETrue );
                    iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                    }
                }
            else
                {
                if ( keystrokeArray->Count() != 0 && phrase->Count() >= KMaxPhraseCreationCount )
                    {
                    CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
                    errnote->SetTone( CAknNoteDialog::EWarningTone );
                    errnote->ExecuteLD( KMaxPhraseNote );
                    }
                else
                    {
                    iOwner->FepMan()->PhraseCreation( EFalse );
                    iOwner->FepMan()->EntryPhrase( EFalse );
                    iOwner->FepMan()->PinyinPhraseCreation( EFalse );
                    iOwner->FepMan()->EnableKeyStar( ETrue );
                    iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
                    }
                }
            popup->ResetStrokeArray();
            }
        else
            {
            editPane->SetCursorIndexOfKeystroke( 0 );
            if ( CheckFirstGroupStroke() )
                {
                popup->SetState( ETrue );
                iOwner->ChangeState( EEntry );
                }
            else
                {
                popup->SetState( ETrue );
                //change state to sequence edit
                iOwner->ChangeState( EStrokeSequenceEdit );
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateStrokePhrase::CommitInlineEEPL 
// Commit phrase to the EEP ctrl.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCandidateStrokePhrase::CommitInlineEEPL( 
                                                        const TDesC& aText )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArrayForStroke();
    CDesCArrayFlat* phrase = popup->ChooseChineseCharacterArrayForStroke();
    CDesCArrayFlat* phraseKeystroke = 
        popup->ChooseChineseCharacterArrayKeystrokeForStroke();
    
    TBuf<KMaxPhraseCreationCount> phraseCreated;
    
    TInt textCount = aText.Length();
    TInt allTextCount = 0;
    TInt i = 0;
    TInt j = 0;
    // append character to chinese array
    for ( i = 0; i < textCount; i++ )
        {
        phrase->AppendL( aText.Mid( i, 1 ) );
        }
    
    TInt characterCount = phrase->Count();
    
    // if chinese character is more than 7 or not
    // if more than 7 show a warning not
    if ( characterCount >= KMaxPhraseCreationCount )
        {
        for ( i = 0; i < KMaxPhraseCreationCount; ++i )
            {
            phraseCreated.Append( phrase->MdcaPoint(i) );
            }
        AddPhraseToDB( phraseCreated );
        iOwner->FepMan()->NewTextL( phraseCreated );
        iOwner->FepMan()->CommitInlineEditL();
        }
    
    // calculate the total number of character according to stroke
    for ( i = 0; i < keystrokeArray->Count(); i++ )
        {
        if ( keystrokeArray->MdcaPoint( i ) == KDelimiter )
            {
            allTextCount = allTextCount + 1;
            }
        }
    
    allTextCount = allTextCount + 1;
    if ( keystrokeArray->MdcaPoint( keystrokeArray->Count() - 1 ) ==  
        KDelimiter )
        {
        allTextCount--;
        }
    
    // if total number is less than the character's count which user has selected
    // commit the character to editor and user db   
    if ( allTextCount <= textCount && characterCount < KMaxPhraseCreationCount )
        {
        for ( i = 0; i < characterCount; ++i )
            {
            phraseCreated.Append( phrase->MdcaPoint(i) );
            } 
        iOwner->FepMan()->NewTextL( phraseCreated );
        iOwner->FepMan()->CommitInlineEditL();
        AddPhraseToDB( phraseCreated );
        return ETrue;
        }
    
    // add the stroke to chosen character array
    // delete key stroke which charactor is corresponding
    for ( j = 0; j < textCount; j++ )
        {
        TInt keyCount = 0;
        TInt ret = keystrokeArray->Find( KDelimiter , 
                                         keyCount, 
                                         ECmpFolded );
        
        if ( ret != 0)
            {
            keyCount = keystrokeArray->Count();
            j = textCount;
            }
        else
            {
            keyCount++;
            }

        TBuf<KMaxKeystrokeCount> keystroke;
        for ( i = 0; i < keyCount; i++ )
            {
            keystroke.Append( keystrokeArray->MdcaPoint( 0 ) );
            keystrokeArray->Delete( 0 );
            }
        phraseKeystroke->AppendL( keystroke );
        }    
    if ( keystrokeArray->Count() > 0 )
        {
        iOwner->FepMan()->PhraseCreation( ETrue );
        }    
    
    if ( characterCount >= KMaxPhraseCreationCount )
        {
        return ETrue;
        }

    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateStrokePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateStrokePhrase::HandleCommandL( TInt aCommandId )
    {
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            //    case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            HandleCommitL();
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }

// End of file
