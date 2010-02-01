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
*       Provides the TAknFepInputStateInitialQwertyWesternPredictive definition.
*       This state is active when the predictive QWERTY mode is used but no
*       inline editing is yet ongoing. The entry state is activated when some
*       character key press is gained.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INITIAL_QWERTY_WESTERN_PREDICTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_INITIAL_QWERTY_WESTERN_PREDICTIVE_H__

#include "AknFepUiInputStateBase.h"

class TAknFepInputStateInitialQwertyWesternPredictive : public TAknFepInputStateBase
    {
public:
    /**
    * Constructor
    * @param    aOwner  The UI manager owning this state object.
    */
    TAknFepInputStateInitialQwertyWesternPredictive(MAknFepUIManagerStateInterface* aOwner);

    /**
    * Handler for the key presses.
    * @param    aKey    The key code to be handled
    * @param    aLength The lengyh of the key press
    */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INITIAL_QWERTY_WESTERN_PREDICTIVE_H__

// End of file
