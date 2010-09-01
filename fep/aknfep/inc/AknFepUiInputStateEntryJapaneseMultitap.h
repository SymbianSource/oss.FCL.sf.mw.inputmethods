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
*       Provides the TAknFepUiInputStateEntryJapaneseMultitap definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_JAPANESE_MULTITAP_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_JAPANESE_MULTITAP_H__

// INCLUDES
#include <e32std.h>

#include "AknFepUiInputStateJapaneseBase.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;
enum TKeyPressLength;

/**
 *  TAknFepUiInputStateEntryJapaneseMultitap class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateEntryJapaneseMultitap
    :public TAknFepUiInputStateJapaneseBase
    {
public: // Constructors and destructor
    /**
     * C++ default Constructor
     *
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterface
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */
    TAknFepUiInputStateEntryJapaneseMultitap(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

public: // Functions from base classes
    /**
     * From TAknFepUiInputStateJapaneseBase
     * Call this function when Multitap timer of PtiEngine was time-out
     *
     * @since 2.6
     */
     void KeyTimerExpired();

    /**
     * From TAknFepUiInputStateJapaneseBase
     * Initialize state
     *
     * @since 2.6
     */
     void InitializeStateL();

protected: // Functions from base classes
    /**
     * From TAknFepUiInputStateJapaneseBase
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
     * Handling numeric long-keypress events.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
     TBool HandleNumericLongKeyL(TInt aKey);

    /**
     * From TAknFepUiInputStateJapaneseBase
     * Handling navigate keypress events. HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
     TBool HandleNaviKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepUiInputStateJapaneseBase
     * Handling auxiliary keypress events including OK and Backspace.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
     TBool HandleAuxKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepUiInputStateJapaneseBase
     * Update inline editing string
     *
     * @since 2.6
     * @return ETrue If inline editing string was updated
     */
     TBool UpdateInlineEditingCharacterL();

    /**
     * From TAknFepUiInputStateJapaneseBase
     * Commit inline editing string
     *
     * @since 2.6
     * @return ETrue If inline editing string was updated
     */
     TBool CommitInlineEditingCharacterL();

    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
     * Handling Command events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 3.2
     * @param aCommandId Command ID value
     */
     void HandleCommandL(TInt aCommandId);

    };

#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_JAPANESE_MULTITAP_H__
// End of file
