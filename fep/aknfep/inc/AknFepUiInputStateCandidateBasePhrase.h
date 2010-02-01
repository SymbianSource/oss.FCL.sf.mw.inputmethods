/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateCandidateBasePhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_CANDIDATE_BASE_PHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_CANDIDATE_BASE_PHRASE_H__

#include "AknFepUiInputStateCandidateBase.h"
#include <PtiDefs.h>

class TAknFepInputStateCandidateBasePhrase : public TAknFepInputStateCandidateBase
    {
protected:
    TAknFepInputStateCandidateBasePhrase(MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    TBool HandleHorizontalNavigation(TInt aKey);
    TBool HandleVerticalNavigation(TInt aKey);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_CANDIDATE_BASE_PHRASE_H__

// End of file
