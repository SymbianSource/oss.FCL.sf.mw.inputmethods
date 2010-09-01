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












#ifndef T_AKNFEPUIINPUTSTATEENTRYQWERTYBASECHINESEPHRASE_H_
#define T_AKNFEPUIINPUTSTATEENTRYQWERTYBASECHINESEPHRASE_H_

#include "AknFepUiInputStateCandidateQwertyChinesePhrase.h"
#include "AknFepUIManagerStateInterface.h"

class TAknFepInputStateEntryQwertyBaseChinesePhrase : 
public TAknFepInputStateCandidateQwertyChinesePhrase
    {
public:
    TAknFepInputStateEntryQwertyBaseChinesePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    void InitializeStateL();

protected:
    void RefreshUI();
    void SetInputPaneToneMark();
   };

#endif //T_AKNFEPUIINPUTSTATEENTRYQWERTYBASECHINESEPHRASE_H_

// End of file
