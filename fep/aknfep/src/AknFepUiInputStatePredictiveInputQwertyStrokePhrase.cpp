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
* Description:            Implementation of predictive input qwertystroke phrase state
*
*/












#include "AknFepUiInputStatePredictiveInputQwertyStrokePhrase.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>                      //CPtiEngine

TAknFepInputStatePredictiveInputQwertyStrokePhrase::
TAknFepInputStatePredictiveInputQwertyStrokePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    iState = EPredictiveInput;
    
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    // set container window position to follow cursor
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }

    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutCandidate);

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    // for some reason needs to feed back to ptiengine current Chinese phrase just before get the prediction
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    candidatePane->SelectFirstPhrase(); 
    ptiengine->SetPredictiveChineseChar(text);
    
    TRAPD(ignore,ptiengine->GetChinesePhraseCandidatesL(*(candidatePane->CandidateArray())));
    if (ignore == KErrNone)
        {
        candidatePane->SplitPhraseCandidatesIntoPages(); 
        uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutCandidate);
        candidatePane->SetCandidateBuffer();

        TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                                uiContainer->CandidatePane()->IsFirstPage());
        TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
        TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

        UIContainer()->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
        UIContainer()->ShowVerticalScrollArrows(multiplePages);
        uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
        UpdateIndicator();
        }
    }

TBool TAknFepInputStatePredictiveInputQwertyStrokePhrase::HandleKeyL(TInt aKey, 
                                                                     TKeyPressLength aLength)
    {
    TBool ret = ETrue;

    if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) ||
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if (IsValidEntryKey(aKey))
        {
        iOwner->FepMan()->TryCloseUiL();
        ret = EFalse;
        }
    else if(aKey == EStdKeyBackspace)
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else if(aKey == EStdKeyLeftFunc && aLength == EShortKeyPress) // launch sct table
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        if (fepMan->IsAbleToLaunchSCT())
            {
            fepMan->LaunchSpecialCharacterTableL();
            }
        }
    else if (aKey == EStdKeyEnter)
        {
        iOwner->FepMan()->TryCloseUiL();
        iOwner->ChangeState(EQwerty);
        return EFalse;
        }
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey))
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
        ret = TAknFepInputStateCandidateQwertyBaseChinesePhrase::HandleKeyL(aKey, aLength);
        }
    
    return ret;
    }
    
TBool  TAknFepInputStatePredictiveInputQwertyStrokePhrase::IsValidEntryKey(TInt aKey)
    {
    TBool response = ETrue;

    if ((aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
        (aKey >= EStdKeyLeftArrow && aKey <= EStdKeyDownArrow) ||
        (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9) ||
         aKey == EStdKeyBackspace || aKey == EStdKeyLeftFunc ||
         aKey == EStdKeyDevice1 || aKey == EStdKeyDevice3 ||
         aKey == EStdKeyDevice0)
        {
        response = EFalse;
        }

    return response;
    }    
// ---------------------------------------------------------------------------
// TAknFepInputStatePredictiveInputQwertyStrokePhrase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStatePredictiveInputQwertyStrokePhrase::HandleCommandL(
    TInt aCommandId )
    {
    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate( );
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
     //   case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
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
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }
// End of file
