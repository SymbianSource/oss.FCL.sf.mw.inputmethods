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
* Description:            Provides the TAknFepUiInputStateJapaneseBase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUTSTATE_JAPANESE_BASE_H__
#define __AKN_FEP_UI_INPUTSTATE_JAPANESE_BASE_H__

// INCLUDES
#include <e32std.h>
#include <uikon.hrh>

#include "AknFepUiInputStateBase.h"
#include "AknFepGlobalEnums.h"
#include "AknFepUICtrlContainerJapanese.h"

#include <PtiObserver.h> // MPtiObserver

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;

/**
 *  TAknFepUiInputStateJapaneseBase class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateJapaneseBase : public TAknFepInputStateBase
    {
protected:
        enum TStateEvent
        {
        EStateEventNone,
        EStateEventCompleteCharacter,
        EStateEventDeleteCharacter,
        EStateEventScrollLeft,
        EStateEventScrollRigth,
        EStateEventScrollUp,
        EStateEventScrollDown,
        EStateEventCandidateOff,
        EStateEventCandidateOn
        };

public: // Constructors and destructor
    /**
     * C++ default Constructor
     *
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterface
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */
    TAknFepUiInputStateJapaneseBase(MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerJapanese* aUIContainer);


public: // Functions from base classes
    /**
     * From MPtiObserver
     * Call this function when Multitap timer of PtiEngine was time-out
     *
     * @since 2.6
     */
     void KeyTimerExpired();

    /**
     * From MPtiObserver
     * not-use Japanese state
     *
     * @since 2.6
     */
     void LastWordInSelectionList();

    /**
     * From MPtiObserver
     * not-use Japanese state
     *
     * @since 2.6
     */
     void FirstWordInSelectionList();

    /**
     * From TAknFepInputStateBase
     * Hnadling Keypress events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
     TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepInputStateBase
     * Handling Command events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aCommandId Command ID value
     */
     void HandleCommandL(TInt aCommandId);

    /**
     * From TAknFepInputStateBase
     * A patch to force the vtable to be copied during an assignment.
     *
     * @since 2.6
     */
    void operator=(const TAknFepInputStateBase& aState);

protected: // New Functions
    /**
     * Handling short keypress events of the numeric and the star.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
    virtual TBool HandleNumericShortKeyL(TInt aKey);

    /**
     * Handling numeric long-keypress events.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
    virtual TBool HandleNumericLongKeyL(TInt aKey);

    /**
     * Handling Qwerty keypress events.
     * HandleKeyEventL() call this method.
     *
     * @since 2.8
     * @param aKey The event key code
     * @return ETrue If aKey was handled
     */
    virtual TBool HandleQwertyShortKeyL(TInt aKey);

    /**
     * Handling navigate keypress events. HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
    virtual TBool HandleNaviKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * Handling auxiliary keypress events including OK and Backspace.
     * HandleKeyEventL() call this method.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
    virtual TBool HandleAuxKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * Update inline editing string
     *
     * @since 2.6
     * @return ETrue If inline editing string was updated
     */
    virtual TBool UpdateInlineEditingCharacterL();

    /**
     * Commit inline editing string
     *
     * @since 2.6
     * @return ETrue If inline editing string was updated
     */
    virtual TBool CommitInlineEditingCharacterL();

    /**
     * Getting UI container object
     *
     * @since 2.6
     * @return Japanese UI container object
     */
    MAknFepUICtrlContainerJapanese* UIContainer();

    /**
     * Update character case
     *
     * @since 2.6
     * @param aChar 1-character, Judgment data of character case
     */
    void  UpdateCase(TPtrC& aChar);

    TBool IsNumberKey(const TInt aKey);
    TBool IsQwertyKey(const TInt aKey);

private: // New Functions
    /**
     * Check whether last catacter case and current catacter case is same
     * 
     * @since 2.6
     * @param aChar 1-character, Judgment data of character case
     * @return ETrue If aChar case is diffrent from last character
     */
    TBool IsDiffrentCase(TPtrC& aChar) const;

    /**
     * Check a caracter case is upper
     * 
     * @since 2.6
     * @param aChar 1-character, Judgment data of character case
     * @return ETrue If aChar case is upper case
     */
    TBool IsUpperCase(TPtrC& aChar) const;

    /**
     * Check a caracter case is lower
     *
     * @since 2.6
     * @param aChar 1-character, Judgment data of character case
     * @return ETrue If aChar case is lower case
     */
    TBool IsLowerCase(TPtrC& aChar) const;

protected: // Data
    MAknFepUICtrlContainerJapanese* iUIContainer; // not owner
    };
#endif//__AKN_FEP_UI_INPUT_STATE_JAPANESE_BASE_H__
// End of file
