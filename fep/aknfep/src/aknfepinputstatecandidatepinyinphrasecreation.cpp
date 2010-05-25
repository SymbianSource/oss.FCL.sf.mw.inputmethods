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
*       Provides the aknfepinputstatecandidatepinyinphrasecreation definition.
*
*/












// System includes
#include <PtiEngine.h>   
#include <avkon.rsg>
#include <aknfep.rsg>
//#include <aknquerydialog.h> //CAknQueryDialog  

// User includes
#include "aknfepinputstatecandidatepinyinphrasecreation.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManager.h"     

// Constant definition
const TInt KMinCnadidateCount = 1;
const TInt KMaxSpellingLength = 10;
const TInt KMaxPhraseCreationCount = 7;

//add  the max phrase is seven
_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );


// ---------------------------------------------------------------------------
// AknFepUiInputStateCandidatePinyinAndStrokePhrase::
// TAknFepUiInputStateCandidatePinyinAndStrokePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStateCandidatePinyinPhraseCreation::
    TAknFepInputStateCandidatePinyinPhraseCreation(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer) :
        TAknFepInputStatePinyinPhraseCreationBase( aOwner, aUIContainer)
    {
    iState = ECandidateSelection;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength > KMinCnadidateCount;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(ETrue);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    uiContainer->CandidatePane()->SelectFirstPhrase();
    uiContainer->SetFepMan( iOwner->FepMan() );
    
    UpdateIndicator();
    ChangeCba();
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidatePinyinPhraseCreation::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateCandidatePinyinPhraseCreation::HandleKeyL(TInt aKey,
                                                                 TKeyPressLength aLength)
    {
    switch( aKey )
        {
        case EKeyBackspace:
            KeyBackspace();
            break;
        case EKeyCBA2:
            iOwner->FepMan()->TryCloseUiL();
            break;
        default:
            HandleKeyUpDownLeftRightArrowOrKeyCBA1OKL( aKey, aLength );
            break;
        }
    return ETrue;
    }

// ---------------------------------------------------------
// Update indicator.
// ---------------------------------------------------------
//
void TAknFepInputStateCandidatePinyinPhraseCreation::UpdateIndicator()
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
    if ( candidatePane->IsLastPage() )
       {
       uiContainer->CandidatePane()->ShowDownScrollArrows(EFalse);
       }
    else
       {
       uiContainer->CandidatePane()->ShowDownScrollArrows(ETrue);      
       }
    }

// ---------------------------------------------------------
// Handle horizontal navigation.
// ---------------------------------------------------------
//
TBool TAknFepInputStateCandidatePinyinPhraseCreation::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EKeyLeftArrow)
        {
        if( !candidatePane->SelectPrev() )
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else if (aKey == EKeyRightArrow)
        {                  
        if( !candidatePane->SelectNext() )
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirstPhrase();
            }
        response = ETrue;
        }
        
    if (response)
        {
        UpdateIndicator();
        }   
             
    return response;
    }

// ---------------------------------------------------------
// Handle vertical navigation.
// ---------------------------------------------------------
//
TBool TAknFepInputStateCandidatePinyinPhraseCreation::HandleVerticalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

    if (aKey == EKeyDownArrow)
        {
        if (candidatePane->IsLastPage())
            {
            return ETrue;
            }                
        candidatePane->NextCandidatePage();
        }
    else if (aKey == EKeyUpArrow)
        {
        if (candidatePane->IsFirstPage() )
            {
            return ETrue;   
            }       
        candidatePane->PreviousCandidatePage();
        }
    else
        {
        return EFalse;
        }
    candidatePane->SetCandidateBuffer();
    candidatePane->SelectFirstPhrase();
    UpdateIndicator(); 

    return ETrue;
    }

// ---------------------------------------------------------
// Commit inline to the EEP ctrl.
// ---------------------------------------------------------
//
TBool TAknFepInputStateCandidatePinyinPhraseCreation::CommitInlineEEPL( const TDesC& aText )
    {//select from phrase candidates
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* keystrokeArray = popup->ShowKeystrokeArray();
    CDesCArrayFlat* validKeystroke = popup->InEffectKeystrokeArray();
    CDesCArrayFlat* chooseChineseCharacterArray = 
        popup->ChooseChineseCharacterArray();
    CDesCArrayFlat* chooseChineseCharacterArrayKeystroke = 
        popup->ChooseChineseCharacterArrayKeystroke();
    TInt keyCount = validKeystroke->Count();
    
    TBuf<KMaxSpellingLength> keystroke;
    
    popup->SetPhraseCreationState( ETrue );
    
    //stroe spelling 
    CDesCArrayFlat* spellingArray = popup->PhoneticSpellingArray();//spelling array
	TInt selection = popup->CurrentSelection();//selection index of spelling array
    CDesCArrayFlat* chooseChineseCharacterArraySpelling = popup->ChooseChineseCharacterArraySpelling();//phrase spelling array
	TInt delCount = chooseChineseCharacterArraySpelling->Count() - chooseChineseCharacterArray->Count();
    if(delCount > 0)//we can sure delCount>=0,impossible <0
    	{//remove unwanted spelling
		chooseChineseCharacterArraySpelling->Delete(chooseChineseCharacterArray->Count(),delCount);
    	}
	chooseChineseCharacterArraySpelling->AppendL(spellingArray->MdcaPoint(selection));
    
    for ( TInt i = 0; i < keyCount; i++ )
        {
        keystroke.Append( keystrokeArray->MdcaPoint( 0 ) );
        keystrokeArray->Delete( 0 );
        }
    chooseChineseCharacterArray->AppendL( aText );
    chooseChineseCharacterArrayKeystroke->AppendL( keystroke );
    validKeystroke->Reset();
    
    if ( chooseChineseCharacterArray->Count() >= KMaxPhraseCreationCount )
        {
        keystrokeArray->Reset();
        CDesCArrayFlat* phraseCandidates = 
            UIContainer()->CandidatePane()->CandidateArray();
        phraseCandidates->Reset();
        phraseCandidates->AppendL( KMaxPhraseNote );
        RefreshUI( 0 );
        return EFalse;
        }  
    return keystrokeArray->Count();
    }

// ---------------------------------------------------------
// KeyBackspace.
// ---------------------------------------------------------
//
void TAknFepInputStateCandidatePinyinPhraseCreation::KeyBackspace()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    // there is  "ChangeCba()" before 
    popup->SetChangeState( ETrue );
    iOwner->ChangeState(ESpellingSelection);
    }

// ---------------------------------------------------------
// Key Up,Down,Left,RightArrow Or Key CBA1,OK.
// ---------------------------------------------------------
//
void TAknFepInputStateCandidatePinyinPhraseCreation::HandleKeyUpDownLeftRightArrowOrKeyCBA1OKL( 
                                     TInt aKey, 
                                     TKeyPressLength aLength )
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TInt index = MapKeyToIndex(aKey);
           
    if ( aKey == EKeyUpArrow &&
        iOwner->FepMan()->InputMode() == EPinyin &&
        candidatePane->IsFirstPage())
        {
        popup->SetChangeState( ETrue );
        iOwner->ChangeState(ESpellingSelection);
        return;
        }
    TBool state = ETrue;
    if ( popup->IsChangeStatet() && aLength == ELongKeyPress )
        {
        state = EFalse;
        }
    else
        {
        popup->SetChangeState( EFalse );
        }
    
    if ( state )
       {
       if ( !(HandleVerticalNavigation( aKey )
           || HandleHorizontalNavigation( aKey ) ) )
         {
              // it may be one of the 'valid' numbers..
           if ( candidatePane->SelectIndex( index, EFalse ) || aKey == EKeyOK
               || aKey == EKeyCBA1 )
              {
               ChangeStateOrNotL(); 
              }
           ChangeCba();
          }
       }
    }
   
// ---------------------------------------------------------
// change the state or not 
// ---------------------------------------------------------
//
void TAknFepInputStateCandidatePinyinPhraseCreation::ChangeStateOrNotL()
	{
	 MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
	 MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    if ( text.Length() )
        {
        CDesCArrayFlat* chooseChineseCharacterArray =
            popup->ChooseChineseCharacterArray();
        //here need Commit text to EEP and change 
        //state to EEP entry state if keystroke is empty
        //else keep spelling state 
        if ( chooseChineseCharacterArray->Count() < KMaxPhraseCreationCount )
            {
            if ( !CommitInlineEEPL( text ) )
                {
                popup->SetChangeState( ETrue );
                iOwner->ChangeState( EKeySequenceEdit );
                TBool visible =
                    ( chooseChineseCharacterArray->Count() == KMaxPhraseCreationCount );
                UIContainer()->SetControlInVisible( visible );
                }
            else
                {
                if ( !popup->IsEnabled() )
                    {
                    popup->Enable( ETrue );
                    }
                if ( !GetValidKeystroke() )
                    {
                    UIContainer()->SetControlInVisible( EFalse );
                    popup->SetChangeState( ETrue );
                    iOwner->ChangeState( EKeySequenceEdit );
                    }
                else
                    {
                    popup->SetChangeState( ETrue );
                    iOwner->ChangeState( ESpellingSelection );
                    }
                }
            }
        else
            {
            popup->SetChangeState( ETrue );
            iOwner->ChangeState( EKeySequenceEdit );
            }
        }
    else
        {
        // No candidates available. Back to Entry state.
        popup->SetChangeState( ETrue );
        iOwner->ChangeState( EKeySequenceEdit );
        }
    }


// End of file
