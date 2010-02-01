/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateInitialThaiPredictive definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INITIAL_THAI_PREDICTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_INITIAL_THAI_PREDICTIVE_H__

#include "AknFepUiInputStateInitialMultitapBase.h"

class TAknFepInputStateInitialThaiPredictive : public TAknFepInputStateInitialMultitapBase
    {
public:
    TAknFepInputStateInitialThaiPredictive(MAknFepUIManagerStateInterface* aOwner);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    void HandleCommandL(TInt aCommandId);

    };

#endif //__AKN_FEP_UI_INPUT_STATE_INITIAL_THAI_PREDICTIVE_H__

// End of file
