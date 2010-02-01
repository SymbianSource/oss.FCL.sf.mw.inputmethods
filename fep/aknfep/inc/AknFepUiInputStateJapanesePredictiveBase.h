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
*       Provides the TAknFepUiInputStateJapanesePredictiveBase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_JAPANESE_PREDICTIVE_BASE_H__
#define __AKN_FEP_UI_INPUT_STATE_JAPANESE_PREDICTIVE_BASE_H__

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
 *  TAknFepUiInputStateJapanesePredictiveBase class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateJapanesePredictiveBase
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
    TAknFepUiInputStateJapanesePredictiveBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

public: // Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * 
     * @since 2.6
     */
     void CloseUI();

protected: // Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling numeric long-keypress events.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
     TBool HandleNumericLongKeyL(TInt aKey);

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

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Update inline editing string
     *
     * @since 2.6
     * @return ETrue If inline editing string was updated
     */
     TBool UpdateInlineEditingCharacterL();

protected: // New Fanctions
    /**
     *
     *
     * @since 2.6
     * @return ETrue
     */
    virtual TBool UpdateCandidateListL(TInt aIndex = 0);
    };
#endif //__AKN_FEP_UI_INPUT_STATE_JAPANESE_PREDICTIVE_BASE_H__
// End of file
