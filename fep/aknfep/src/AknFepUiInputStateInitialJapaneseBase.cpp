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
* Description:            Provides the TAknFepUiInputStateInitialJapaneseBase methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateInitialJapaneseBase.h"
#include "AknFepUIManagerStateInterface.h"  // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                // keys
#include <PtiEngine.h>


// CONSTANTS

/**
 *  AknFepUIInputStateInitialJapanesBase class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::
//                                    TAknFepUiInputStateInitialJapaneseBase
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateInitialJapaneseBase::TAknFepUiInputStateInitialJapaneseBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateJapaneseBase(aOwner, aUIContainer)
    {
    iState = EInitial;

    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialJapaneseBase::InitializeStateL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    fepMan->SendUIDeActivatedNotification();
    TRAP_IGNORE(fepMan->UpdateCbaL(NULL));
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::HandleNumericShortKeyL
// Handling numeric short keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateInitialJapaneseBase::
    HandleNumericShortKeyL(TInt aKey)
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    if(aKey == EPtiKeyStar)
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
        fepMan->StartInlineEditL();
        iOwner->ChangeState(EEntry);
        }
    // All number keypress event must be handled by InputStateEntry.
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::HandleAuxKeyL
// Handling OK and Backspace keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateInitialJapaneseBase::
    HandleAuxKeyL(TInt /*aKey*/,TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;
    return ret;
    }
// End of file
