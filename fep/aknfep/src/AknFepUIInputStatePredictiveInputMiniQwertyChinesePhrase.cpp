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
* Description:            Implementation of predictive input phrase state
*
*/












#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiInputStatePredictiveInputMiniQwertyChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "aknfepuictrleditpane.h"

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase::
TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();

    candidatePane->SelectFirstPhrase(); 

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    UIContainer()->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    candidatePane->ShowCandidateOrdinals(ETrue);
    UpdateIndicator();
    }

TBool TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase::HandleKeyL(TInt aKey, 
                                                                          TKeyPressLength aLength)
    {  
    TInt selected = UIContainer()->CandidatePane()->SelectedIndex();
    TInt numbertotal = UIContainer()->CandidatePane()->VisibleCandidateCount();
    
    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
    	return ETrue;
        }
    else if (iOwner->FepMan()->FnKeyState())
        {
        return ETrue;
        }
    else if(aKey == EStdKeyBackspace )
        {
        iOwner->FepMan()->TryCloseUiL();
        return ETrue;
        }               
    else if (aKey ==EKeyRightFunc)//fn key
    	{
    	iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
    	return ETrue;
    	} 
    else if ( aKey ==EStdKeySpace)
    	{
    	if (ECangJie ==iOwner->FepMan()->InputMode())
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
	                iOwner->ChangeState(EPredictiveCandidate);
	                }
	            }
	        return ETrue;
    		}
    	else 
    		{
            return ETrue; 
    		}
    	}	 	           
    else if ( aKey == EKeyLeftShift || aKey == EKeyRightShift)
        {
//        iOwner->FepMan()->TryCloseUiL();
//        return EFalse;
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return ETrue;
        }      
    else if ( aKey == EStdKeyDevice0 || aKey == EStdKeyDevice3 || aKey == EStdKeyEnter )
        {
        TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
        if(text.Length())
            {            
            MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
            fepMan->NewTextL(text);
            fepMan->CommitInlineEditL();
            iOwner->PtiEngine()->SetPredictiveChineseChar(text);
            
            // For sogou core, the predictive is not endless, so when there
            // is no predictive candidates, we should call TryCloseUiL().
            TBool noCandidates = EFalse;
            if ( iPlugin.IsEnable() || iStrokePlugin.IsEnable())
                {
                // Get the predictive candidates.
                CDesCArrayFlat* phraseCandidates = new(ELeave) CDesCArrayFlat(1);
                CleanupStack::PushL ( phraseCandidates );
                phraseCandidates->Reset();
                iOwner->PtiEngine()->GetChinesePhraseCandidatesL( *phraseCandidates );
                if ( phraseCandidates->Count() == 0 )
                    {
                    noCandidates = ETrue;
                    }
                CleanupStack::PopAndDestroy( phraseCandidates );
                }
            // If no candidates, call TryCloseUiL().
            if ( fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull) || noCandidates )
                {
                fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                iOwner->FepMan()->TryCloseUiL();
                }
            else
                {
                iOwner->ChangeState(EPredictiveCandidate);
                UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
                }
            }
        return ETrue;
        }   
    else if (aKey == EStdKeyLeftArrow)//in the same row
        {
        if (selected == 0)
           {
       	    UIContainer()->CandidatePane()->SelectIndex(numbertotal-1);
           }
        else
           {
       	    UIContainer()->CandidatePane()->SelectIndex(selected - 1);
           } 
        UIContainer()->CandidatePane()->SetHighlighted(ETrue);     
        return ETrue;	
        }
    else if (aKey == EStdKeyRightArrow)//in the same row
        {
    	if (selected == numbertotal-1)
    		{
       	    UIContainer()->CandidatePane()->SelectIndex(0);
    		}
    	else
    		{
       	    UIContainer()->CandidatePane()->SelectIndex(selected + 1);
    		}
    	UIContainer()->CandidatePane()->SetHighlighted(ETrue);     
        return ETrue;		
        }        
    else
        {
        return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        } 
   // return EFalse;       
    } 
  
// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase::HandleCommandL(
    TInt aCommandId )
    {
    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    switch ( aCommandId )
        {
        // Handle the event frome command.
    case EAknSoftkeySelect:
        //    case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA            
        if ( text.Length() )
            {
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
                iOwner->ChangeState( EPredictiveCandidate );
                }
            }
        break;
    default:
        TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
        break;
    }
}

void TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase::SubmitTextL( const TDesC& aText )
	{
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
	if ( aText.Length() )
	   {
	   fepMan->NewTextL( aText );
	   fepMan->CommitInlineEditL();
	   iOwner->PtiEngine()->SetPredictiveChineseChar( aText );
      
       // For sogou core, the predictive is not endless, so when there
       // is no predictive candidates, we should call TryCloseUiL().
       TBool noCandidates = EFalse;
       
       if ( iPlugin.IsEnable() || iStrokePlugin.IsEnable())
           {
           // Get the predictive candidates.
           CDesCArrayFlat* phraseCandidates = new(ELeave) CDesCArrayFlat(1);
           CleanupStack::PushL ( phraseCandidates );
           phraseCandidates->Reset();
           iOwner->PtiEngine()->GetChinesePhraseCandidatesL( *phraseCandidates );
           if ( phraseCandidates->Count() == 0 )
               {
               noCandidates = ETrue;
               }
           CleanupStack::PopAndDestroy( phraseCandidates );
           }
       
       if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull) || noCandidates )
	      {
	      fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
	      iOwner->FepMan()->TryCloseUiL();
	      }
	   else
	      {
	      iOwner->ChangeState( EPredictiveCandidate );
	      }
	    }
	}
	
// End of file
