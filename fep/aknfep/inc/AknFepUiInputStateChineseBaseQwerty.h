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
*       Provides the TAknFepInputStateChineseBaseQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_CHINESE_BASE_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_CHINESE_BASE_QWERTY_H__

#include "AknFepUIInputStateChineseBase.h"

class TAknFepInputStateChineseBaseQwerty : public TAknFepInputStateChineseBase
    {
public:
    TAknFepInputStateChineseBaseQwerty(MAknFepUIManagerStateInterface* aOwner,
					MAknFepUICtrlContainerChinese* aUIContainer); 

protected:
    TBool IsQwertyKey(const TInt aKey);
    TBool IsNumberKey(const TInt aKey);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_CHINESE_BASE_QWERTY_H__

// End of file
