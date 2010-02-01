/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIInputStateCandidateQwerty methods.
*
*/












#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUiInputStateCandidateQwerty.h"

#include "AknFepManager.h"                  //FepMan flag

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  

TAknFepInputStateCandidateQwerty::TAknFepInputStateCandidateQwerty(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyBase(aOwner, aUIContainer)
    {
    }

TBool TAknFepInputStateCandidateQwerty::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    if(aKey == EStdKeyDevice1)
        {
        iOwner->FepMan()->TryCloseUiL();
        }
    else if(!( HandleVerticalNavigation(aKey) || HandleHorizontalNavigation(aKey) ))
        {
        // it may be one of the 'valid' numbers..
        TInt index = MapKeyToIndex(aKey);
        MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

        if(candidatePane->SelectIndex(index) || aKey == EStdKeyDevice3 || aKey == EStdKeyEnter)
            {
            TPtrC text = candidatePane->CurrentCandidate();
            if(text.Length())
                {            
                MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                fepMan->NewCharacterL(text);
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
            else
                {
                // No candidates available. Back to Entry state.
                iOwner->ChangeState(EEntry);
                }
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateQwerty::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateQwerty::HandleCommandL( TInt aCommandId )
    {
    TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
    }
// End of file
