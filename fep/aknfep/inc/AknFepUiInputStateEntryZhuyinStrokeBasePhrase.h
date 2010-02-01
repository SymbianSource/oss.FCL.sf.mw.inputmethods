/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryZhuyinStrokeBasePhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ZHUYIN_STROKE_BASE_PHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_ZHUYIN_STROKE_BASE_PHRASE_H__

#include "AknFepUiInputStateChineseBase.h"

/**
 * Zhuyin and Stroke share a base class as there is commonality
 * over most of their functionality
 */
class TAknFepInputStateEntryZhuyinStrokeBasePhrase : public TAknFepInputStateChineseBase
    {
public:
    enum 
        {
        EPhoneticChanged  = 0x0001
        };

    TAknFepInputStateEntryZhuyinStrokeBasePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);

    virtual TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    void InitializeStateL();

    void HandleCommandL(TInt aCommandId);

public: // Functions from MPtiObserver interface
    virtual void KeyTimerExpired();

protected:
    void RefreshUI();
    void SetInputPaneToneMark();
    void UpdateIndicator();
    };


#endif //__AKN_FEP_UI_INPUT_STATE_ZHUYIN_STROKE_BASE_PHRASE_H__

// End of file
