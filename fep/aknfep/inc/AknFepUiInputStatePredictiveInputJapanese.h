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
*       Provides the TAknFepUiInputStatePredictiveInputJapanese definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_PREDICTIVE_INPUT_JAPANESE_H__
#define __AKN_FEP_UI_INPUT_STATE_PREDICTIVE_INPUT_JAPANESE_H__

// INCLUDES
#include <e32std.h>

#include "AknFepUiInputStateJapanesePredictiveBase.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;
enum TKeyPressLength;

/**
 *  TAknFepUiInputStatePredictiveInputJapanese class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStatePredictiveInputJapanese
    :public TAknFepUiInputStateJapanesePredictiveBase
    {
public: // Constructors and destructor
    /**
     * C++ default Constructor
     *
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterface
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */
    TAknFepUiInputStatePredictiveInputJapanese(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer);

public: // Functions from base classes
    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
     * Call this function when Multitap timer of PtiEngine was time-out
     *
     * @since 2.6
     */
    void KeyTimerExpired();

    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
     * Initialize state
     *
     * @since 2.6
     */
     void InitializeStateL();

    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
     * Handling Command events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aCommandId Command ID value
     */
     void HandleCommandL(TInt aCommandId);

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Close UI (Predictive popup)
     * @since 2.6
     */
     void CloseUI();

protected: // Functions from base classes
    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
     * Handling short keypress events of the numeric and the star.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
     TBool HandleNumericShortKeyL(TInt aKey);

    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
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
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling navigate keypress events. HandleKeyEventL() call this method.
     *
     * @since 3.2
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
     TBool HandleNaviKeyL(TInt aKey, TKeyPressLength aLength);

private: // New function

    /**
     * TAknFepUiInputStatePredictiveInputJapanese::IsFullWidthAlphaChar
     * Check full-width alphabet character
     */
    TBool IsFullWidthAlphaChar(TPtrC aPtr);

    /**
     * TAknFepUiInputStatePredictiveInputJapanese::ChangeStateToConversionL()
     * Change state to conversion
     */
    void ChangeStateToConversionL();

    /**
     * TAknFepUiInputStatePredictiveInputJapanese::ChangeStateToEntryL()
     * Change state to Entry
     */
    void ChangeStateToEntryL();
    };
#endif //__AKN_FEP_UI_INPUT_STATE_PREDICTIVE_INPUT_JAPANESE_H__
// End of file
