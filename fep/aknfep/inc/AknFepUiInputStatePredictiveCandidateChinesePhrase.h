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












#ifndef T_AKNFEPUIINPUTSTATEPREDCANDCHINESEPHRASE_H_
#define T_AKNFEPUIINPUTSTATEPREDCANDCHINESEPHRASE_H_

#include "AknFepUiInputStateCandidateBaseChinesePhrase.h"

class TAknFepInputStatePredictiveCandidateChinesePhrase : 
public TAknFepInputStateCandidateBaseChinesePhrase 
    {
public:
    TAknFepInputStatePredictiveCandidateChinesePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif T_AKNFEPUIINPUTSTATEPREDCANDCHINESEPHRASE_H_

// End of file
