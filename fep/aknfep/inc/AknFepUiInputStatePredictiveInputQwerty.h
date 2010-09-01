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












#ifndef __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_H__

#include "AknFepUiInputStateCandidateQwertyChineseBase.h"

class TAknFepInputStatePredictiveInputQwerty : public TAknFepInputStateCandidateQwertyChineseBase
    {
public:
    TAknFepInputStatePredictiveInputQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    };

#endif __AKN_FEP_UI_INPUT_STATE_PRED_INPUT_QWERTY_H__

// End of file
