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
*       Provides the TAknFepInputStateEntryStroke definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_STROKE_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_STROKE_H__

#include "AknFepUiInputStateEntryZhuyinStrokeBase.h"

class TAknFepInputStateEntryStroke : public TAknFepInputStateEntryZhuyinStrokeBase
    {
public:
    TAknFepInputStateEntryStroke(MAknFepUIManagerStateInterface* aOwner,
                                 MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    

    void HandleCommandL(TInt aCommandId);
    
    void RefreshUI();

    };

#endif //__AKN_FEP_UI_INPUT_STATE_INPUT_STROKE_H__

// End of file
