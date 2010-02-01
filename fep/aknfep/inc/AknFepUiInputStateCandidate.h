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
*       Provides the TAknFepInputStateCandidate definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_CANDIDATE_H__
#define __AKN_FEP_UI_INPUT_STATE_CANDIDATE_H__

#include "AknFepUiInputStateCandidateBase.h"

class TAknFepInputStateCandidate : public TAknFepInputStateCandidateBase 
    {
public:
    TAknFepInputStateCandidate(MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_CANDIDATE_H__

// End of file
