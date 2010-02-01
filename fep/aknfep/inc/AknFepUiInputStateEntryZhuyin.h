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
*       Provides the TAknFepInputStateEntryZhuyin definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_ZHUYIN_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_ZHUYIN_H__

#include "AknFepUiInputStateEntryZhuyinStrokeBase.h"

class TAknFepInputStateEntryZhuyin : public TAknFepInputStateEntryZhuyinStrokeBase
    {
public:
    TAknFepInputStateEntryZhuyin(MAknFepUIManagerStateInterface* aOwner,
                                 MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    void HandleCommandL(TInt aCommandId);

public: // Functions from MPtiObserver interface
    void KeyTimerExpired();
protected:
    void RefreshUI(TBool aOrdinals);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INPUT_ZHUYIN_H__

// End of file
