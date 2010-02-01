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
*       Provides the TAknFepUiInputStateEntryKatakana definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_KATAKANA_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_KATAKANA_H__

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
 *  TAknFepUiInputStateEntryKatakana class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateEntryKatakana
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
    TAknFepUiInputStateEntryKatakana(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

public:// Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Initialize state
     * 
     * @since 2.6
     */
     void InitializeStateL();

protected: // Functions from base classes
    /**
     * From AknFepUiInputStateEntryJapaneseMultitap
     * Handling short keypress events of the numeric and the star.
     * HandleKeyEventL() call this method.
     * 
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
     TBool HandleNumericShortKeyL(TInt aKey);
    };
#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_JAPANESE_MULTITAP_H__
// End of file
