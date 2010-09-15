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
*       Provides the  TAknFepInputStateEntryMiniQwertyCangJie methods.
*
*/











#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepUIInputStateEntryMiniQwertyCangJie.h"


#include "AknFepManager.h"                  //FepMan flag
#include "AknFepUICtrlInputPane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include <aknfep.rsg>
#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.rsg>

//const TInt KInputPaneLength = 32;
//_LIT(KSep, "'");
//_LIT(KReplace, "");

TAknFepInputStateEntryMiniQwertyCangJie::TAknFepInputStateEntryMiniQwertyCangJie(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBaseChinesePhrase(aOwner,aUIContainer)                           
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();    
    if(iOwner->FepMan()->CangJieMode() == EPtiEngineNormalCangjieQwerty)
        {
        ptiengine->SetInputMode(EPtiEngineNormalCangjieQwerty);            
        }
    else if(iOwner->FepMan()->CangJieMode() == EPtiEngineEasyCangjieQwerty)
        {
        ptiengine->SetInputMode(EPtiEngineEasyCangjieQwerty);            
        }
    else
        {
        ptiengine->SetInputMode(EPtiEngineAdvCangjieQwerty);            
        }
    ptiengine->SetCase(EPtiCaseLower);
    UIContainer()->CandidatePane()->ShowCandidateOrdinals(EFalse);
    }

TBool TAknFepInputStateEntryMiniQwertyCangJie::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

    if(aKey == EStdKeyLeftFunc && aLength == EShortKeyPress)//chr key to close UI
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if( CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() ||
             CAknFepFnKeyManager::EFnKeyLock == iOwner->FepMan()->FnKeyState() ||
             CAknFepFnKeyManager::EFnKeyDown == iOwner->FepMan()->FnKeyState())
        {
    	iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
    	ret = ETrue;
        }
    else if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed) ||
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
    	ret = ETrue;
    	}    
    else if(aKey == EStdKeyBackspace)//should have long and short 
        {
        if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                    uiContainer->CandidatePane()->IsFirstPage()));
            SetInputPaneToneMark();
            inputPane->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);
            RefreshUI();
            }
        else
            {
            iOwner->FepMan()->TryCloseUiL();
            if (aLength == ELongKeyPress)
                {
                iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                }
            }
        }
    else if ( aKey == EStdKeyDevice0 || aKey == EStdKeyDevice3 || aKey == EStdKeySpace ) // Add for SK1
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
    else if(aKey == EStdKeyUpArrow)
        {
    	if (candidatePane->IsFirstPage())
            {
            return ETrue;   
            }
        candidatePane->PreviousCandidatePage(); 
        candidatePane->SetCandidateBuffer();
        candidatePane->SelectFirstPhrase();
        UpdateIndicator();
        }
    else if(aKey == EStdKeyDownArrow)
        {
        return TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }
    else if( aKey == EStdKeyRightArrow )
        {
        if ( candidatePane->VisibleCandidateCount() != 1 )
	        {
	    	candidatePane->SelectNext(); 	
	    	UpdateIndicator();
	        }    	
        }
    else if ( aKey == EStdKeyLeftArrow )  
    	{
    	if ( candidatePane->VisibleCandidateCount()!= 1 )
	    	{
	        candidatePane->SelectPrev();	
	        UpdateIndicator();
	    	}
    	}     
    else if(aLength == EShortKeyPress && iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
       
        TInt stringBeforeLength(0);
        TInt stringAfterLength(0);
        
        stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
        stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
        
        if (stringBeforeLength != stringAfterLength)
            {
            if (ptiengine->GetPhoneticSpelling(1).Length() == 1)
                {
               // iOwner->FepMan()->UpdateCbaL(R_AVKON_SOFTKEYS_EMPTY);
                iOwner->FepMan()->UpdateCbaL(R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL);
                }
            
            TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                    uiContainer->CandidatePane()->IsFirstPage());
            TInt bufLength = ptiengine->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;
        
            uiContainer->ShowVerticalScrollArrows(!(uiContainer->CandidatePane()->IsLastPage() &&
                                                    uiContainer->CandidatePane()->IsFirstPage()));
            uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows);         
            SetInputPaneToneMark();
            uiContainer->InputPane()->SetFlag(MAknFepUICtrlInputPane::EPhoneticChanged);

            RefreshUI();
            }
        else
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }
            
        if (ptiengine->GetPhoneticSpelling(1).Length() == 0)
            {
            iOwner->FepMan()->UpdateCbaL(NULL);
            }
        }
    else    
        {
        ret = TAknFepInputStateEntryQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }
        
    return ret;
    }


// ---------------------------------------------------------------------------
// TAknFepInputStateEntryMiniQwertyCangJie::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateEntryMiniQwertyCangJie::HandleCommandL( TInt aCommandId )
    {
    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate( );
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan( );
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
     //   case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
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
void TAknFepInputStateEntryMiniQwertyCangJie::SubmitTextL( const TDesC& aText )
	{
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan( );
	if ( aText.Length( ) )
	   {
	   fepMan->NewTextL( aText );
	   fepMan->CommitInlineEditL( );
	   iOwner->PtiEngine()->SetPredictiveChineseChar( aText );
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

// End of file
