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
* Description:            Implementation of Pinyin phrase predictive Qwerty state
*
*/












// System includes
#include <PtiEngine.h>                      

// User includes
#include "AknFepUiInputStatePredictiveInputQwertyPinyinPhrase.h"
#include "AknFepUiInputStatePredictiveInputQwertyPhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  
#include "AknFepManagerUIInterface.h" 
#include "AknFepManager.h"      
#include "AknFepUICtrlCandidatePane.h"

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputQwertyPinyinPhrase::
// TAknFepInputStatePredictiveInputQwertyPinyinPhrase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepInputStatePredictiveInputQwertyPinyinPhrase::
TAknFepInputStatePredictiveInputQwertyPinyinPhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStatePredictiveInputQwertyChinesePhrase(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputQwertyPinyinPhrase::HandleKeyL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStatePredictiveInputQwertyPinyinPhrase::HandleKeyL(TInt aKey, 
                                                                     TKeyPressLength aLength)
    {
    TBool ret = ETrue;

    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if (aKey == EStdKeyEnter)
        {
        iOwner->FepMan()->TryCloseUiL();
        iOwner->ChangeState(EQwerty);
        return EFalse;
        }
    else if (IsValidEntryKey(aKey))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        if (IsValidPinyinKey(aKey))
            {
            CPtiEngine* ptiengine = iOwner->PtiEngine();
            // it is needed when we use phrase input engine, otherwise it seems not cleared
            ptiengine->ClearCurrentWord(); 
            ptiengine->ResetToneMark();

            iOwner->ChangeState(EEntry);
            ret = EFalse; //passes to entry state to handle the key
            }
         else
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDWarningTone);
            }
        }
    else if (aKey == EStdKeySpace)
        {
        TInt index = MapKeyToIndex(EStdKeyDevice3);
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

        candidatePane->SelectIndex(index);
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
                iOwner->ChangeState(EPredictiveInput);
                }
            }
        else
            {
            // No candidates available. Back to Entry state.
            iOwner->ChangeState(EEntry);
            }
        }
    else 
        {
        ret = TAknFepInputStatePredictiveInputQwertyChinesePhrase::HandleKeyL(aKey, aLength);
        }
        
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputQwertyPinyinPhrase::IsValidPinyinKey
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
TBool  TAknFepInputStatePredictiveInputQwertyPinyinPhrase::IsValidPinyinKey(TInt aKey)
    {
    TBool response = ETrue;

    if (aKey == EPtiKeyQwertyU || aKey == EPtiKeyQwertyV ||
        aKey == EPtiKeyQwertyI )
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        response = EFalse;
        }

    return response;
    }
    
// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputQwertyPinyinPhrase::IsValidEntryKey
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
TBool  TAknFepInputStatePredictiveInputQwertyPinyinPhrase::IsValidEntryKey(TInt aKey)
    {
    TBool response = ETrue;

    if ((aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
        (aKey >= EStdKeyLeftArrow && aKey <= EStdKeyDownArrow) ||
        (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9) ||
         aKey == EStdKeyBackspace || aKey == EStdKeyLeftFunc ||
         aKey == EStdKeyDevice1 || aKey == EStdKeySpace || 
         aKey == EStdKeyDevice0 ||aKey == EStdKeyDevice3)
        {
        response = EFalse;
        }

    return response;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputQwertyPinyinPhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStatePredictiveInputQwertyPinyinPhrase::HandleCommandL(
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
// End of file
