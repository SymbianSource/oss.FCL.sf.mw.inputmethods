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
*       Provides the TAknFepInputStateZhuyinFindQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ZHUYIN_FIND_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_ZHUYIN_FIND_QWERTY_H__

#include "AknFepUiInputStateChineseFindQwerty.h"
#include "AknFepUiInputStateInitialChineseGenericQwerty.h"
#include "AknFepUiInputStateInitialChineseGenericHalfQwerty.h"

class TAknFepInputStateZhuyinFindQwerty : public TAknFepInputStateChineseFindQwerty
    {
public:
    TAknFepInputStateZhuyinFindQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    };

#ifdef RD_INTELLIGENT_TEXT_INPUT
class TAknFepInputStateZhuyinFindMiniQwerty : public TAknFepInputStateInitialChineseGenericQwerty
    {
public:
    TAknFepInputStateZhuyinFindMiniQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                TInt aMode);
    
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
     // Functions from MPtiObserver interface
    void KeyTimerExpired();
    };

#ifdef __HALF_QWERTY_KEYPAD
class TAknFepInputStateZhuyinFindHalfQwerty : 
public TAknFepInputStateInitialChineseGenericHalfQwerty
    {
public:
    TAknFepInputStateZhuyinFindHalfQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                TInt aMode);
    
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
     // Functions from MPtiObserver interface
    void KeyTimerExpired();
private:
    TBool IsValidZhuyinKey(TInt aKey);
    };

#endif //__HALF_QWERTY_KEYPAD

#endif //RD_INTELLIGENT_TEXT_INPUT

#endif //__AKN_FEP_UI_INPUT_STATE_STROKE_FIND_QWERTY_H__

// End of file
