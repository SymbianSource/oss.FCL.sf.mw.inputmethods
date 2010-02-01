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
* Description:           
*       Provides the TAknFepUiInputStateEntryKatakana methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateEntryKatakana.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepGlobalEnums.h"

#include <PtiDefs.h>                     // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface
#include <avkon.rsg>

// CONSTANTS

/**
 *  TAknFepUiInputStateEntryKatakana class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryKatakana::
//                                  TAknFepUiInputStateEntryKatakana
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateEntryKatakana::TAknFepUiInputStateEntryKatakana(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateEntryJapaneseMultitap(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryKatakana::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateEntryKatakana::InitializeStateL()
    {
    // In case of Katakana and Hiragana input, underline and highlight aren't always shown.
    iOwner->SetInlineEditUnderlineVisibility(EFalse);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateEntryKatakana::HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateEntryKatakana::HandleNumericShortKeyL(TInt aKey)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;
    if (aKey == EPtiKeyStar)
        {
        iOwner->FepMan()->SetInlineEditingCursorVisibilityL(ETrue);
        }
    ptiEngine->AppendKeyPress((TPtiKey)aKey);

	if ( iOwner->IsQwerty() )
		{
		const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
		TPtrC text = data->CompletedString();
		// In case of Katakana and Hiragana transitory input,
		// underline and highlight are shown.
		iOwner->SetInlineEditUnderlineVisibility(
				(text.Length() > 0)? EFalse : ETrue);
		}
	else
		{
		// In case of Katakana and Hiragana input, underline and highlight aren't always shown .
		iOwner->SetInlineEditUnderlineVisibility( EFalse );
		}

    ret = UpdateInlineEditingCharacterL();
    return ret;
    }

// End of file
