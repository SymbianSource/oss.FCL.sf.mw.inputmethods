/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepInputStateEntryWesternPredictiveJp definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_WESTERN_PREDICTIVE_JP_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_WESTERN_PREDICTIVE_JP_H__

#include "AknFepUiInputStateEntryWesternPredictive.h"

class TAknFepInputStateEntryWesternPredictiveJp : public TAknFepInputStateEntryWesternPredictive
    {
public:
    TAknFepInputStateEntryWesternPredictiveJp(MAknFepUIManagerStateInterface* aOwner);

protected:
    TBool HandleStarKeyL(TKeyPressLength aLength);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_WESTERN_PREDICTIVE_JP_H__

// End of file