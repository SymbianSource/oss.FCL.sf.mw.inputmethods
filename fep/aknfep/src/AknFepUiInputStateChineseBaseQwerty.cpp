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
*       Provides the TAknFepInputStateChineseBaseQwerty methods.
*
*/












#include "AknFepUIInputStateChineseBaseQwerty.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepCaseManager.h"

#include <PtiEngine.h>
#include <PtiDefs.h>

TAknFepInputStateChineseBaseQwerty::TAknFepInputStateChineseBaseQwerty(
					MAknFepUIManagerStateInterface* aOwner,
					MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner,aUIContainer)
    {
    }

TBool TAknFepInputStateChineseBaseQwerty::IsQwertyKey(const TInt aKey)
    {
    TBool response = EFalse;
    if ( (aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
        IsNumberKey( aKey ) ||
        aKey == EPtiKeyQwertyPlus ||
        aKey == EPtiKeyQwertyMinus ||
        aKey == EPtiKeyQwertyComma ||
        aKey == EPtiKeyQwertySemicolon ||
        aKey == EPtiKeyQwertyFullstop ||
        aKey == EPtiKeyQwertyHash ||
        aKey == EPtiKeyQwertySlash ||
        aKey == EPtiKeyQwertyApostrophe)
        {
        response = ETrue;
        }
    return response;
    }

TBool TAknFepInputStateChineseBaseQwerty::IsNumberKey(const TInt aKey)
    {
    TBool response = EFalse;
    if (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9)
        {
        response = ETrue;
        }
    return response;
    }

// End of file
