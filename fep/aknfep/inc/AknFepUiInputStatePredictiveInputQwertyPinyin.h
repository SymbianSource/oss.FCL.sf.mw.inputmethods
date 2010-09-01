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
*       Provides the TAknFepInputStatePredictiveInputQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_PINYIN__H__
#define __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_PINYIN__H__

#include "AknFepUiInputStateCandidateQwertyChineseBase.h"

class TAknFepInputStatePredictiveInputQwertyPinyin : 
public TAknFepInputStateCandidateQwertyChineseBase
    {
public:
    TAknFepInputStatePredictiveInputQwertyPinyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
private:

    TBool IsValidPinyinKey(TInt aKey);
    TBool IsValidEntryKey(TInt aKey);
    };

#endif __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_PINYIN_H__

// End of file
