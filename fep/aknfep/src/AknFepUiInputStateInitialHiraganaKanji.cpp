/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepUiInputStateInitialHiraganaKanji methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateInitialHiraganaKanji.h"
#include "AknFepUIManagerStateInterface.h"  // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                // keys
#include <PtiEngine.h>

// CONSTANTS

/**
 *  TAknFepUiInputStateInitialHiraganaKanji class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// -----------------------------------------------------------------------------
// TAknFepUiInputStateInitialHiraganaKanji::TAknFepUiInputStateInitialHiraganaKanji
// C++ default Constructor
// -----------------------------------------------------------------------------
//
TAknFepUiInputStateInitialHiraganaKanji::TAknFepUiInputStateInitialHiraganaKanji(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateInitialJapaneseBase(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialHiraganaKanji::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialHiraganaKanji::InitializeStateL()
    {
    TAknFepUiInputStateInitialJapaneseBase::InitializeStateL();

    // Setup of PtiEngine
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TPtiEngineInputMode inputmode;
    if(iOwner->IsPredictive())
        {
        inputmode = (iOwner->IsQwerty())? 
                        EPtiEngineHiraganaKanjiPredictiveQwerty
                        : EPtiEngineHiraganaKanjiPredictive;
        }
    else
        {
        inputmode = (iOwner->IsQwerty())? 
                        EPtiEngineHiraganaKanjiQwerty
                        : EPtiEngineHiraganaKanji;
        }
    ptiEngine->SetInputMode(inputmode);
    ptiEngine->ClearCurrentWord();
    }

// -----------------------------------------------------------------------------
// TAknFepUiInputStateInitialHiraganaKanji::HandleNumericShortKeyL
// Handling numeric short keypress event
// -----------------------------------------------------------------------------
//
TBool TAknFepUiInputStateInitialHiraganaKanji::
    HandleNumericShortKeyL(TInt aKey)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool ret = EFalse;
    if (aKey == EPtiKeyStar)
        { // Handling of launch special character table
        if (!fepMan->HashKeySelectionInUse())
            {
            if (fepMan->EditSubmenuInUse())
                {
                fepMan->LaunchSelectModeMenuL();
                }
            else if (fepMan->IsAbleToLaunchSCT())
                {
                fepMan->LaunchSpecialCharacterTableL();
                }
            }
        ret = ETrue;
        }
    else
        {
        iOwner->FepMan()->StartInlineEditL();

        // Clear RetroActive buffer on PtiEngine
        CPtiEngine* ptiEngine = iOwner->PtiEngine();
        ptiEngine->HandleCommandL(EPtiCommandUserActionRetroActiveClear);

        // The numeric key will be handled by HandleNumericKeyL of
        // EntryState or PredictiveInputState
        if(iOwner->IsPredictive())
            {
            iOwner->ChangeState(EPredictiveInput);
            }
        else
            {
            iOwner->ChangeState(EEntry);
            }
        }

    return ret;
    }
// End of file
