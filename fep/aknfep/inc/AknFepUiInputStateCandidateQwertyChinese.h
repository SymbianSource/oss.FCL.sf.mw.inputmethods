/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            ?Description
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_CANDIDATE_QWERTY_CHINESE_H__
#define __AKN_FEP_UI_INPUT_STATE_CANDIDATE_QWERTY_CHINESE_H__

#include "AknFepUiInputStateCandidateQwertyChineseBase.h"
#include "AknFepUIManagerStateInterface.h"

class TAknFepInputStateCandidateQwertyChinese : public TAknFepInputStateCandidateQwertyChineseBase
    {
public:
    TAknFepInputStateCandidateQwertyChinese(MAknFepUIManagerStateInterface* aOwner,
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

#endif //__AKN_FEP_UI_INPUT_STATE_CANDIDATE_QWERTY_CHINESE_H__

// End of file
