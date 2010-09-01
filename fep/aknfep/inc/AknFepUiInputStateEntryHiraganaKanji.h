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
* Description:            Provides the TAknFepUiInputStateEntryHiraganaKanji definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_HIRAGANA_KANJI_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_HIRAGANA_KANJI_H__

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
 *  TAknFepUiInputStateEntryHiraganaKanji class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateEntryHiraganaKanji
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
    TAknFepUiInputStateEntryHiraganaKanji(
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

    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
     * Handling Command events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 3.2
     * @param aCommandId Command ID value
     */
     void HandleCommandL(TInt aCommandId);

protected: // Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling navigate keypress events. HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
     TBool HandleNaviKeyL(TInt aKey, TKeyPressLength aLength);

    TBool HandleNumericShortKeyL(TInt aKey);
    };
#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_HIRAGANA_KANJI_H__
// End of file
