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
*       Provides the TAknFepInputStateStrokeFindQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_STROKE_FIND_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_STROKE_FIND_QWERTY_H__

#include "AknFepUiInputStateChineseFindQwerty.h"
#include "AknFepUiInputStateInitialChineseGenericHalfQwerty.h"
#include "AknFepUiInputStateInitialChineseGenericQwerty.h"

class TAknFepInputStateStrokeFindQwerty : public TAknFepInputStateChineseFindQwerty
    {
public:
    TAknFepInputStateStrokeFindQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    };
#ifdef RD_INTELLIGENT_TEXT_INPUT
class TAknFepInputStateStrokeFindMiniQwerty :
    public TAknFepInputStateInitialChineseGenericQwerty
    {
public:
    TAknFepInputStateStrokeFindMiniQwerty(
            MAknFepUIManagerStateInterface* aOwner,
            TInt aMode);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };
#ifdef __HALF_QWERTY_KEYPAD
class TAknFepInputStateStrokeFindHalfQwerty :
    public TAknFepInputStateInitialChineseGenericHalfQwerty
    {
public:
    TAknFepInputStateStrokeFindHalfQwerty(
            MAknFepUIManagerStateInterface* aOwner,
            TInt aMode);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif //__HALF_QWERTY_KEYPAD

#endif //RD_INTELLIGENT_TEXT_INPUT

#endif //__AKN_FEP_UI_INPUT_STATE_STROKE_FIND_QWERTY_H__

// End of file
