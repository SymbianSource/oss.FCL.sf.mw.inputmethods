/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryQwertyWesternPredictive definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_WESTERN_PREDICTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_WESTERN_PREDICTIVE_H__

#include "AknFepUiInputStateBase.h"

class TAknFepInputStateEntryQwertyWesternPredictive : public TAknFepInputStateBase
    {
public:
    /**
    * Constructor
    * @param    aOwner  The UI manager that owns this input state
    */
    TAknFepInputStateEntryQwertyWesternPredictive(MAknFepUIManagerStateInterface* aOwner);
    
    /**
    * The main handler function for the key presses.
    * @param    aKey    The key to be handled. This may be either TKeyCode or TStdScanCode value.
    * @param    aLength Tells if the key press is short or long one.
    */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    

public: // Functions from MPtiObserver interface
    /**
    * Called when last word in the selection list is activated. Not relevant as we use the 
    * candidate popup.
    */
    void LastWordInSelectionList();
    
    /**
    * Called when the first word in the selection list is activated. Not relevant as we use the
    * candidate popup.
    */
    void FirstWordInSelectionList();

    /**
    * Called when the Chr multitap timer expires. The case should be updated at that time.
    */ 
    void KeyTimerExpired();

protected:
    /**
    * Sets the shift state of the PtiEngine according the shift and chr states of the Fep.
    * @param    aKey    The with which the shift state is used. This is needed because shift
    *                   works differently with letter keys than with numbers and punctuation.
    */
    void HandleShiftState(TInt aKey);
    
    /**
    * Handles keys which cause the active word to be committed (like enter, space, tab,...).
    * @param    aKey    The key to be handled.
    */
    TBool HandleWordBreakingKeysL(TInt aKey);
    
    /**
    * Handles the "normal" keys which (may) cause some character to be appended to the current word.
    * @param    aKey    The key to be handled.
    */
    TBool HandleQwertyKeysL(TInt aKey);
    
    /**
    * Handles the keys which cause cursor movements (arrows, backspace).
    * @param    aKey    The key to be handled
    * @param    aLength The key press length.
    */
    TBool HandleNaviKeysL(TInt aKey, TKeyPressLength aLength);
};

#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_WESTERN_PREDICTIVE_H__

// End of file
