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
*       Provides the TAknFepInputStateInitialChineseGenericQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_CHIN_GEN_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_CHIN_GEN_QWERTY_H__

#include "AknFepUiInputStateQwerty.h"

class TAknFepInputStateInitialChineseGenericQwerty : public TAknFepInputStateQwerty
    {
public:
    TAknFepInputStateInitialChineseGenericQwerty(MAknFepUIManagerStateInterface* aOwner, 
                                                 TInt aMode);
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    void KeyTimerExpired();
private:
    TBool IsSCTKey(TInt aKey);
    TBool IsCharacter(TInt aKey);
    TBool HandleKeyByShiftOrCharPressed(TInt aKey, TKeyPressLength aLength);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INIT_CHIN_GEN_QWERTY_H__

// End of file
