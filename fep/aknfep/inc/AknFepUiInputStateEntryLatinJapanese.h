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
*       Provides the TAknFepUiInputStateEntryLatinJapanese definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_FULL_LATIN_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_FULL_LATIN_H__

// INCLUDES
#include <e32std.h>

#include "AknFepUiInputStateEntryJapaneseMultitap.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;
enum TKeyPressLength;

/**
 *  TAknFepUiInputStateEntryLatinJapanese class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateEntryLatinJapanese
    :public TAknFepUiInputStateEntryJapaneseMultitap
    {
public: // Constructors and destructor
    /**
     * C++ default Constructor
     * 
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterface
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */ 
    TAknFepUiInputStateEntryLatinJapanese(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

public: // Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Call this function when Multitap timer of PtiEngine was time-out
     *
     * @since 2.6
     */
    void KeyTimerExpired();

protected: // Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling short keypress events of the numeric and the star.
     * HandleKeyEventL() call this method.
     * 
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
     TBool HandleNumericShortKeyL(TInt aKey);
    };
#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_FULL_LATIN_H__
// End of file
