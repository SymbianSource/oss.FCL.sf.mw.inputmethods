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
* Description:            Provides the TAknFepUiInputStateInitialKatakana methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateInitialKatakana.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                // keys
#include <PtiEngine.h>

// CONSTANTS

/**
 *  TAknFepUiInputStateInitialKatakana class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialKatakana::TAknFepUiInputStateInitialKatakana
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateInitialKatakana::TAknFepUiInputStateInitialKatakana(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateInitialJapaneseBase(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialKatakana::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialKatakana::InitializeStateL()
    {
    TAknFepUiInputStateInitialJapaneseBase::InitializeStateL();

    // Setup PtiEngine
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    ptiEngine->ClearCurrentWord();
    // Set Character Mode
    SetInputMode(iOwner->FepMan()->CharacterWidth());
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialKatakana::HandleCommandL
// Handling command
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialKatakana::HandleCommandL(TInt aCommandId)
    {
    if (aCommandId == EJapanFepCmdModeFullWidth
        || aCommandId == EJapanFepCmdModeHalfWidth)
        {
        // Set Character Mode
        SetInputMode(iOwner->FepMan()->CharacterWidth());
        }
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialKatakana::SetInputMode
// Setting input mode to PtiEngine
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialKatakana::SetInputMode(const TWidthChar aWidth)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TPtiEngineInputMode inputmode = EPtiEngineInputModeNone;
    switch(aWidth)
        {
        case EHalfWidthChar:
            inputmode = (iOwner->IsQwerty())? 
                            EPtiEngineKatakanaQwerty
                            : EPtiEngineKatakana;
            break;
        
        case EFullWidthChar:
            inputmode = (iOwner->IsQwerty())? 
                            EPtiEngineFullWidthKatakanaQwerty
                            : EPtiEngineFullWidthKatakana;
            break;
        
        default: // no operation
            break;
        }
    if (inputmode != EPtiEngineInputModeNone)
        {
        ptiEngine->SetInputMode(inputmode);
        }
    }
// End of file
