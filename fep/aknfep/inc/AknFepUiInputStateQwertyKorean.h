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
*       CAknFepInputStateQwertyKorean declaration
*
*/

#ifndef AKNFEPINPUTSTATEQWERTYKOREAN_H
#define AKNFEPINPUTSTATEQWERTYKOREAN_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "AknFepUiInputStateQwerty.h"

class CPtiEngine;

class TAknFepInputStateQwertyKorean : public TAknFepInputStateQwerty
    {
public:
    
    TAknFepInputStateQwertyKorean(MAknFepUIManagerStateInterface* aOwner);
    
    virtual ~TAknFepInputStateQwertyKorean();

private:

    virtual TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    void KeyTimerExpired();
    
private:
    
    };

#endif // AKNFEPINPUTSTATEQWERTYKOREAN_H
