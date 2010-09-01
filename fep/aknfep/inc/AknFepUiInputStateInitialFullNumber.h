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
* Description:            Provides the TAknFepUiInputStateInitialFullNumber definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_FULL_NUMBER_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_FULL_NUMBER_H__

// INCLUDES
#include <e32std.h>

#include "AknFepUiInputStateInitialJapaneseBase.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;

/**
 *  TAknFepUiInputStateInitialFullNumber class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateInitialFullNumber
 :public TAknFepUiInputStateInitialJapaneseBase
    {
public: // onstructors and destructor
    /**
     * C++ default Constructor
     * 
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterface
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */
    TAknFepUiInputStateInitialFullNumber(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);


public:// Functions from base classes
    /**
     * From TAknFepUiInputStateInitialJapaneseBase
     * Initialize state
     * 
     * @since 2.6
     */
     void InitializeStateL();

    /**
     * From TAknFepInputStateBase
     * Hnadling Keypress events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 3.0
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

public: // Functions from MPtiObserver interface

    virtual void KeyTimerExpired();

private: // New function
    /**
     * Handle ITUT-keypad characters (half-width only)
     * 
     * @since 3.0
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
    TBool HandleOwnKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * Handle Qwerty characters
     * 
     * @since 3.0
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
    TBool HandleQwertyKeyL(TInt aKey,
                           TKeyPressLength aLength,
                           TWidthChar aWidth);


    };
#endif //__AKN_FEP_UI_INPUT_STATE_INIT_FULL_NUMBER_H__
// End of file
