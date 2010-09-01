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
*       Provides the TAknFepInputStatePredictiveInput definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_H__
#define __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_H__

#include "AknFepUiInputStateCandidateBase.h"

class TAknFepInputStatePredictiveInput : public TAknFepInputStateCandidateBase 
    {
public:
    TAknFepInputStatePredictiveInput(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_H__

// End of file
