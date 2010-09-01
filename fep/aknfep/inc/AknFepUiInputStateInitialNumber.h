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
*       Provides the TAknFepInputStateInitialNumber definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INITIAL_NUMBER_H__
#define __AKN_FEP_UI_INPUT_STATE_INITIAL_NUMBER_H__

#include "AknFepUiInputStateInitialMultitapBase.h"

class TAknFepInputStateInitialNumber : public TAknFepInputStateInitialMultitapBase
    {
public:
    TAknFepInputStateInitialNumber(MAknFepUIManagerStateInterface* aOwner); 

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
protected:
    static TInt NumericResourceTimerTimeoutL(TAny* aPtr);
    TInt NumericResourceKeyTimerTimeoutL();    
    
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INITIAL_NUMBER_H__

// End of file
