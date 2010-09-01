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
* Description:            Provides the TAknFepUiInputStateInitialJapaneseBase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_JAPANESE_BASE_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_JAPANESE_BASE_H__

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
 *  TAknFepUiInputStateInitialJapaneseBase class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateInitialJapaneseBase :public TAknFepUiInputStateJapaneseBase
    {
public: // onstructors and destructor
    /**
     * C++ default Constructor
     * 
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterface
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */
    TAknFepUiInputStateInitialJapaneseBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

public:// Functions from base classes
    /**
     * From TAknFepUiInputStateJapaneseBase
     * Initialize state
     * 
     * @since 2.6
     */
     void InitializeStateL();

protected:// Functions from base classes
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
     * Handling auxiliary keypress events including OK and Backspace.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
     TBool HandleAuxKeyL(TInt aKey, TKeyPressLength aLength);
    };
#endif //__AKN_FEP_UI_INPUT_STATE_INIT_JAPANESE_BASE_H__
// End of file
