/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateInitialHalfQwertyWesternPredictive definition.
*       This state is active when the predictive Half QWERTY mode is used but no
*       inline editing is yet ongoing. The entry state is activated when some
*       character key press is gained.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INITIAL_HALFQWERTY_WESTERN_PREDICTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_INITIAL_HALFQWERTY_WESTERN_PREDICTIVE_H__

#include "AknFepUiInputStateBase.h"
#include "AknFepUiInputStateInitialMultitapBase.h"

class TAknFepUiInputStateInitialHalfQwertyWesternPredictive : /*public TAknFepInputStateBase, */public  TAknFepInputStateInitialMultitapBase
    {
public:
    /**
    * Constructor
    * @param    aOwner  The UI manager owning this state object.
    */
    TAknFepUiInputStateInitialHalfQwertyWesternPredictive(MAknFepUIManagerStateInterface* aOwner);

    /**
    * Handler for the key presses.
    * @param    aKey    The key code to be handled
    * @param    aLength The lengyh of the key press
    */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    /**
    * Called when the Chr multitap timer expires. The case should be updated at that time.
    */ 
    void KeyTimerExpired();
private:
    void HandleShiftState( TInt aKey );
    
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INITIAL_HALFQWERTY_WESTERN_PREDICTIVE_H__

// End of file
