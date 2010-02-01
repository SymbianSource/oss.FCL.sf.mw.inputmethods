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
* Description:            Provides the TAknFepUiInputStateInitialKatakana definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_KATAKANA_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_KATAKANA_H__

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
 *  TAknFepUiInputStateInitialKatakana class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateInitialKatakana
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
    TAknFepUiInputStateInitialKatakana(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

public: // Functions from base classes
    /**
     * From TAknFepUiInputStateInitialJapaneseBase
     * Initialize state
     * 
     * @since 2.6
     */
     void InitializeStateL();

    /**
     * From TAknFepUiInputStateInitialJapaneseBase
     * Handling Command events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aCommandId Command ID value
     */
     void HandleCommandL(TInt aCommandId);

private: // New functions
    /**
     * Setting input mode to PtiEngine
     *
     * @since 3.0
     * @param aWidth Full or Half width
     */
    void SetInputMode(const TWidthChar aWidth);
    };
#endif //__AKN_FEP_UI_INPUT_STATE_INIT_KATAKANA_H__
// End of file
