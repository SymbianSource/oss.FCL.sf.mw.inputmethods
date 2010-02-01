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
* Description:            Provides the TAknFepUiInputStateInitialHiragana methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateInitialHiragana.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface

#include <PtiDefs.h>                // keys
#include <PtiEngine.h>

// CONSTANTS

/**
 *  TAknFepUiInputStateInitialHiragana class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialHiragana::TAknFepUiInputStateInitialHiragana
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateInitialHiragana::TAknFepUiInputStateInitialHiragana(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateInitialJapaneseBase(aOwner, aUIContainer)
    {
    
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialHiragana::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialHiragana::InitializeStateL()
    {
    TAknFepUiInputStateInitialJapaneseBase::InitializeStateL();

    // Setup PtiEngine
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    ptiEngine->ClearCurrentWord();
    ptiEngine->SetInputMode( (iOwner->IsQwerty())? 
                            EPtiEngineHiraganaQwerty
                            : EPtiEngineHiragana);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialHiragana::HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateInitialHiragana::HandleNumericShortKeyL(TInt aKey)
    {
    TBool ret = EFalse;
    if (aKey == EPtiKeyStar)
        {
        ret = ETrue;
        }
    else
        {
        iOwner->FepMan()->StartInlineEditL();
        iOwner->ChangeState(EEntry);
        }
    return ret;
    }
// End of file
