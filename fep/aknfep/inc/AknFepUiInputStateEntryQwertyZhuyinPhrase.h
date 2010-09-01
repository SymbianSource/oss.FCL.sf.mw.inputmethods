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
*       Provides the TAknFepInputStateEntryQwertyZhuyinPhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_QWERTY_ZHUYIN_PHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_QWERTY_ZHUYIN_PHRASE_H__

#include "AknFepUiInputStateEntryQwertyBaseChinesePhrase.h"

class TAknFepInputStateEntryQwertyZhuyinPhrase : 
public TAknFepInputStateEntryQwertyBaseChinesePhrase
    {
public:
    TAknFepInputStateEntryQwertyZhuyinPhrase(MAknFepUIManagerStateInterface* aOwner,
                                 MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
public: // Functions from MPtiObserver interface
    void KeyTimerExpired();
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INPUT_QWERTY_ZHUYIN_PHRASE_H__

// End of file
