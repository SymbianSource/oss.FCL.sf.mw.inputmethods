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
* Description:            Implementation of Pinyin phrase predictive candidate MiniQwerty state
*
*/












// System includes
#include <PtiEngine.h>                      

// User includes
#include "AknFepUiInputStatePredictiveCandidateMiniQwertyChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManagerUIInterface.h" 
#include "AknFepManager.h"      
#include "AknFepUICtrlCandidatePane.h"
#include "aknfepuictrleditpane.h"

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase::
// TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase::
TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStatePredictiveInputQwertyChinesePhrase(aOwner, aUIContainer)
    {
    iState = EPredictiveCandidate;
    UIContainer()->CandidatePane()->ShowCandidateOrdinals(EFalse);
    }

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase::HandleKeyL(
                                           TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = ETrue;

    if(iOwner->IsValidChineseSymbol(aKey))
    	{
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        ret = ETrue;
        }
    else if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed)||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if ( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() ||
              CAknFepFnKeyManager::EFnKeyLock == iOwner->FepMan()->FnKeyState() ||
              CAknFepFnKeyManager::EFnKeyDown == iOwner->FepMan()->FnKeyState()) // for fn + a character
        {
    	iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
   else if ( aKey ==EKeyLeftCtrl || aKey == EKeyRightCtrl)      
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if (aKey == EStdKeyEnter)
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if (aKey == EStdKeyBackspace) 
        {
    	iOwner->FepMan()->TryCloseUiL();
        }
    else if (aKey ==EKeyRightFunc)//fn key
    	{
    	iOwner->FepMan()->TryCloseUiL();
    	ret = EFalse;
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
             iOwner->FepMan()->TryCloseUiL();
             ret = EFalse; 
    		}
    	}	    
   else if ( aKey == EStdKeyDevice0 || aKey == EStdKeyDevice3 ) // Add for SK1 and selection 
        {
        TBool state = ETrue;
        if ( UIContainer()->EditPaneWindow()->IsChangeState() && aLength == ELongKeyPress )
            {
            state = EFalse;
            }
        else
            {
            UIContainer()->EditPaneWindow()->SetChangeState( EFalse );
            }
        if ( state )
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
            }
        }
    else if(aKey == EStdKeyRightArrow )
        {
    	iOwner->ChangeState(EPredictiveInput);  
    	UIContainer()->CandidatePane()->SelectNext(); 
        }
    else if (aKey == EStdKeyLeftArrow)  
    	{
    	iOwner->ChangeState(EPredictiveInput);  
        UIContainer()->CandidatePane()->SelectLastPhrase();
    	}    
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        if ( aLength == ELongKeyPress )
            {
            return ETrue;
            }
        
        if(EPinyin ==iOwner->FepMan()->InputMode())
    		{
			CPtiEngine* ptiengine = iOwner->PtiEngine();
			// it is needed when we use phrase input engine, otherwise it seems not cleared
    		ptiengine->ClearCurrentWord(); 
			ptiengine->ResetToneMark();

			iOwner->ChangeState(EEntry);
			ret = EFalse; //passes to entry state to handle the key
    		}
    	else if(EZhuyin  ==iOwner->FepMan()->InputMode() || 
    	        ECangJie ==iOwner->FepMan()->InputMode() ||
    	        EStroke  ==iOwner->FepMan()->InputMode() )	
    		{
    	 	CPtiEngine* ptiengine = iOwner->PtiEngine();
			// it is needed when we use phrase input engine, otherwise it seems not cleared
        	ptiengine->ClearCurrentWord(); 
			ptiengine->ResetToneMark();

			iOwner->ChangeState(EEntry);
			ret = EFalse; //passes to entry state to handle the key
    		}

    	}
    else if(    aKey == EPtiKeyQwertyQ ||
    		    aKey == EPtiKeyQwertyW ||
    		    aKey == EPtiKeyQwertyO ||
    		    aKey == EPtiKeyQwertyP ||
    		    aKey == EPtiKeyQwertyA ||
    		    aKey == EPtiKeyQwertyS ||
    		    aKey == EPtiKeyQwertyL ||
    		    aKey == EPtiKeyQwertyZ ||
    		    aKey == EPtiKeyQwertyX ||
    		    aKey == EPtiKeyQwertyC ||
    		    aKey == EPtiKeyQwertyV ||
    		    aKey == EPtiKeyQwertyB ||
    		    aKey == EPtiKeyQwertyN ||
    		    aKey == EPtiKeyQwertyM )
    	{
    	 if(EStroke ==iOwner->FepMan()->InputMode())
    	 	{
    	 	iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
    		//iOwner->FepMan()->TryCloseUiL();
    	 	}	
    	}
    else
    {
    ret = TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
    }        		    
      
    return ret;
 }

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase::HandleCommandL(
    TInt aCommandId )
    {
    TBool state = ETrue;
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            // case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            if ( UIContainer()->EditPaneWindow()->IsChangeState( ) )
                {
                state = EFalse;
                }
            else
                {
                UIContainer()->EditPaneWindow()->SetChangeState( EFalse );
                }
            if ( state )
                {
                TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate( );
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
                }
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }
// End of file
