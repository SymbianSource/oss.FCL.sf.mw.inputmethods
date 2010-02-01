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
*       Provides the TAknFepInputStateInitialMultitapBase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_MULTITAP_BASE_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_MULTITAP_BASE_H__

#include "AknFepUiInputStateBase.h"

class TAknFepInputStateInitialMultitapBase : public TAknFepInputStateBase
    {
public:
    TAknFepInputStateInitialMultitapBase(MAknFepUIManagerStateInterface* aOwner); 

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

	/**
	 * Set the Star key flag
	 *
     * @param aSet TBool.
     * @return None
	 */
	void SetStarKeyFlag( TBool aSet );

public: // Functions from MPtiObserver interface

    virtual void KeyTimerExpired();
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INIT_MULTITAP_BASE_H__

// End of file
