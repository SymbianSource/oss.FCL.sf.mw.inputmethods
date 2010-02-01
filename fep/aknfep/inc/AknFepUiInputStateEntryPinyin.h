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
*       Provides the TAknFepInputStateEntryPinyin definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_PINYIN_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_PINYIN_H__

#include "AknFepUiInputStateChineseBase.h"

class TAknFepInputStateEntryPinyin : public TAknFepInputStateChineseBase
    {
public:
    TAknFepInputStateEntryPinyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    void InitializeStateL();

    void HandleCommandL(TInt aCommandId);
private:
    void DeliberatelyUpdateSelection();
    void ImplicitlyUpdateSelection();
    void ClearDeliberateSelection();
    void RefreshUI(TInt aSelection);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INPUT_PINYIN_H__

// End of file
