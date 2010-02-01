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
*       Provides the TAknFepInputStateQwertyBasePhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_BASE_PHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_BASE_PHRASE_H__

#include "AknFepUiInputStateCandidateQwertyPhrase.h"
#include "AknFepUIManagerStateInterface.h"

class TAknFepInputStateEntryQwertyBasePhrase : public TAknFepInputStateCandidateQwertyPhrase
    {
public:
    TAknFepInputStateEntryQwertyBasePhrase(MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    void InitializeStateL();

protected:
    void RefreshUI();
    void SetInputPaneToneMark();
   };

#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_BASE_PHRASE_H__

// End of file
