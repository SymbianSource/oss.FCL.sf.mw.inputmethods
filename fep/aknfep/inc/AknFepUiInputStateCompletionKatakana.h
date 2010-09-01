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
*        Provides the TAknFepUiInputStateCompletionKatakana definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_COMPLETION_KATAKANA_H__
#define __AKN_FEP_UI_INPUT_STATE_COMPLETION_KATAKANA_H__
// INCLUDES
#include <e32std.h>

#include "AknFepUiInputStateCompletionJapaneseMultitap.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;

/**
 *  TAknFepUiInputStateCompletionLatinJapanese class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateCompletionKatakana
    :public TAknFepUiInputStateCompletionJapaneseMultitap
    {
public: // Constructors and destructor
    /**
     * C++ default Constructor
     * 
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterface
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */
    TAknFepUiInputStateCompletionKatakana(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

protected: // Functions from base classes
    /**
     * From AknFepUiInputStateCompletionJapaneseMultitap
     * Handling short keypress events of the numeric and the star.
     * HandleKeyEventL() call this method.
     * 
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
     TBool HandleNumericShortKeyL(TInt aKey);

    /**
     * From TAknFepUiInputStateJapaneseBase
     * Update inline editing string
     *
     * @since 3.0
     * @return ETrue If inline editing string was updated
     */
    TBool UpdateInlineEditingCharacterL();

    /**
     * From TAknFepUiInputStateJapaneseBase
     * Commit inline editing string
     *
     * @since 3.0
     * @return ETrue If inline editing string was updated
     */
    TBool CommitInlineEditingCharacterL();

    /**
     * Check Full-width hiragana
     *
     * @since 3.0
     * @return ETrue If there are some hiragana characters
     */
    TBool IsFullHiragana(const TChar aChr);
    
    /**
     * From TAknFepUiInputStateJapaneseBase
     * Handling auxiliary keypress events including OK and Backspace.
     * HandleKeyEventL() call this method.
     *
     * @since 3.1
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
     TBool HandleAuxKeyL(TInt aKey, TKeyPressLength aLength);
    };
#endif //__AKN_FEP_UI_INPUT_STATE_COMPLETION_FULL_LATIN_H__
// End of file
