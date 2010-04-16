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
* Description:            Implementation of candidate qwerty base chinese phrase state
*
*/












#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"                  //FepMan flag
#include "AknFepUICtrlPinyinPopup.h"

#include <PtiEngine.h>                      //CPtiEngine

const TUint16 KSegment = 0x0020;

TAknFepInputStateCandidateQwertyBaseChinesePhrase::
TAknFepInputStateCandidateQwertyBaseChinesePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    }

TBool TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    if(aKey == EStdKeyDevice1)
        {
        iOwner->FepMan()->TryCloseUiL();
        }

    else if(!( HandleVerticalNavigation(aKey) || HandleHorizontalNavigation(aKey) ))
        {
        TInt index = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // it may be one of the 'valid' numbers..
        TPtiKeyboardType keyboardtype = iOwner->FepMan()->KeyboardLayout();
        if ( EPtiKeyboardQwerty4x10 == keyboardtype ||
             EPtiKeyboardQwerty3x11 == keyboardtype ||
             EPtiKeyboardHalfQwerty == keyboardtype ||
             EPtiKeyboardCustomQwerty == keyboardtype)
            {      
            MPtiLanguage *lang = NULL;
            RArray < TPtiNumericKeyBinding > keybinding;
            TBool validnumkey = EFalse;
            CPtiEngine* ptiengine = iOwner->PtiEngine();
            
            if( ptiengine != NULL )
                {
                lang = ptiengine->CurrentLanguage();
                }
            if( ptiengine && lang )
                {
                TRAP_IGNORE(ptiengine->GetNumericModeKeysForQwertyL(lang->LanguageCode(), 
                                                                    keybinding, keyboardtype));
                TInt numericKeysCount = keybinding.Count();
                TInt candidateCount = UIContainer()->CandidatePane()->VisibleCandidateCount();
                while ( numericKeysCount-- )
                    {
                    TPtiNumericKeyBinding numKeyBind = keybinding[numericKeysCount];
                    if ( (numKeyBind.iChar >= 0x31 && numKeyBind.iChar <= ( 0x31 + candidateCount - 1 ))
                    
                       &&(aKey == numKeyBind.iKey))
                    	{
                    	index = numKeyBind.iChar - EPtiKey0 -1;
                    	validnumkey = ETrue;
                    	break;
                    	}
                    }
                }
            keybinding.Reset();
        
            if (!validnumkey)
            	{
            	iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                return ETrue;
            	}
                
            }
        else
            {
#endif
            index = MapKeyToIndex(aKey);
#ifdef RD_INTELLIGENT_TEXT_INPUT
            }             
#endif
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

        if(candidatePane->SelectIndex(index) || aKey == EStdKeyDevice3 || aKey == EStdKeyEnter)
            {
            TPtrC text = candidatePane->CurrentPhraseCandidate();
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
#ifdef RD_INTELLIGENT_TEXT_INPUT
                    if ( EPtiKeyboardQwerty4x10 == iOwner->FepMan()->KeyboardLayout() ||
                            EPtiKeyboardHalfQwerty == iOwner->FepMan()->KeyboardLayout() ||
                            EPtiKeyboardQwerty3x11 == iOwner->FepMan()->KeyboardLayout() )
                        {
                        UIContainer()->PinyinPopupWindow()->SetChangeState( ETrue );
                        iOwner->ChangeState(EPredictiveCandidate);
                        }                    
#endif
                    }
                }
            else
                {
                // No candidates available. Back to Entry state.
                iOwner->ChangeState(EEntry);
                }
            }
        }
    return ETrue;
    }

TBool TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EStdKeyLeftArrow)
        {
        if (candidatePane->SelectedIndex() == 0 && 
            candidatePane->IsFirstPage() )
            {
            return ETrue;
            }
        
        if(!candidatePane->SelectPrev())
            {
            candidatePane->PreviousCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else if (aKey == EStdKeyRightArrow)
        {
        if ( candidatePane->IsLastPage() && 
            (candidatePane->SelectedIndex() == candidatePane->VisibleCandidateCount() - 1))
            {
            return ETrue;
            }  
                  
        if(!candidatePane->SelectNext())
            {
            candidatePane->NextCandidatePage();
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

TBool TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleVerticalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    
    if (aKey == EStdKeyDownArrow || aKey == EStdKeySpace)
        {
        if (candidatePane->IsLastPage())
            {
            return ETrue;
            }
        if ( iState != EPredictiveCandidate &&
        		iState != EPredictiveInput )
        	{
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
                    TRAP_IGNORE(
                    GetNewCandidatesL( iOwner->PtiEngine()->CandidatePage( ), list );
                    for (TInt i = 0; i < list.Count( ); ++i )
                        {
                        phraseCandidates->AppendL( *(list[i]) );
                        }
                    list.ResetAndDestroy();)
                    }
                }
        	}
        candidatePane->NextCandidatePage();
        }
    else if (aKey == EStdKeyUpArrow)
        {
        if (candidatePane->IsFirstPage())
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

void TAknFepInputStateCandidateQwertyBaseChinesePhrase::UpdateIndicator()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
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
   
   if (candidatePane->IsLastPage())
       {
       uiContainer->CandidatePane()->ShowDownScrollArrows(EFalse);     
       }
   else
   	   {
   	   uiContainer->CandidatePane()->ShowDownScrollArrows(ETrue);      
   	   }
#ifdef RD_INTELLIGENT_TEXT_INPUT
   if(iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty4x10)
   		{
   	    if(candidatePane->VisibleCandidateCount()==1)
   	    	{
   	    	uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
   	    	uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);
   	    	}
   	    else
   	    	{
   	    	uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
   	    	uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
   	    	}	
   		}
   else
   		{
#endif
    if (candidatePane->SelectedIndex() == 0 && 
        candidatePane->IsFirstPage() )
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
        }
    else
        {
        uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
        }
        
    if (candidatePane->IsLastPage() && 
        (candidatePane->SelectedIndex() == candidatePane->VisibleCandidateCount() - 1))
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);        
        }
    else
        {
        uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
        }
    }    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    }    
#endif

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateQwertyBaseChinesePhrase::MapKeyToIndexSelectCandidate
// Check the select candidate numeric key is valid or not
// ---------------------------------------------------------------------------
//
#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool  TAknFepInputStateCandidateQwertyBaseChinesePhrase::MapKeyToIndexSelectCandidate(
    TInt aKey,TInt& aIndex )
    {
    TBool validnumkey = EFalse;
    MPtiLanguage *lang= NULL;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtiKeyboardType keyboardtype = iOwner->FepMan()->KeyboardLayout();
    if (EPtiKeyboardQwerty4x10 == keyboardtype || EPtiKeyboardQwerty3x11
            == keyboardtype || EPtiKeyboardHalfQwerty == keyboardtype
            || EPtiKeyboardCustomQwerty == keyboardtype)
        {
        RArray <TPtiNumericKeyBinding> keybinding;
        
        if( ptiengine != NULL )
            {
            lang = ptiengine->CurrentLanguage();
            }
        if ( ptiengine && lang )
            {
            TRAP_IGNORE(ptiengine->GetNumericModeKeysForQwertyL(lang->LanguageCode(), keybinding, keyboardtype));
            TInt numericKeysCount = keybinding.Count();
            while ( numericKeysCount-- )
                {
                TPtiNumericKeyBinding numKeyBind =
                        keybinding[numericKeysCount];
                if ( (numKeyBind.iChar >= 0x31 && numKeyBind.iChar <= 0x36)
                        &&(aKey == numKeyBind.iKey))
                    {
                    aIndex = numKeyBind.iChar - EPtiKey0 - 1;
                    validnumkey = ETrue;
                    break;
                    }
                }
            }
        keybinding.Reset();
        if ( !validnumkey ) 
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }       
        }
    else
        {
        aIndex = MapKeyToIndex(aKey);
        }
    return validnumkey ;
    }
#endif
// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleCommandL(
    TInt aCommandId )
    {
    TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
    }

void TAknFepInputStateCandidateQwertyBaseChinesePhrase::GetNewCandidatesL(const TDesC& aSource, 
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
