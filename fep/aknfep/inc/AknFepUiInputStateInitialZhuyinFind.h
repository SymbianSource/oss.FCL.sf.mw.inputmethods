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
*       Provides the TAknFepInputStateInitialZhuyinFind definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ZHUYIN_FIND_H__
#define __AKN_FEP_UI_INPUT_STATE_ZHUYIN_FIND_H__

#include "AknFepUiInputStateInitialChineseMultitapBase.h"

class TAknFepInputStateInitialZhuyinFind : public TAknFepInputStateInitialChineseMultitapBase
    {
public:
    TAknFepInputStateInitialZhuyinFind(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_ZHUYIN_FIND_H__

// End of file
