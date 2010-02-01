/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStatePredictiveInputQwertyZhuyin definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_ZHUYIN_H__
#define __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_ZHUYIN_H__

#include "AknFepUiInputStateCandidateBase.h"
#include "AknFepUiInputStateCandidateQwertyChinese.h"

class TAknFepInputStatePredictiveInputQwertyZhuyin : public TAknFepInputStateCandidateQwertyChinese
    {
public:
    TAknFepInputStatePredictiveInputQwertyZhuyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_ZHUYIN_H__

// End of file
