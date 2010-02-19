/*
* Copyright (c) 2005,2006 Choe Hwanjin
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#include "OssKeyboard.h"
#include "hangul.h"

EXPORT_C COssKeyboard* COssKeyboard::NewLC()
    {
    COssKeyboard* self = new (ELeave) COssKeyboard();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

EXPORT_C COssKeyboard* COssKeyboard::NewL()
    {
    COssKeyboard* self = COssKeyboard::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

COssKeyboard::COssKeyboard()
    {
    }

EXPORT_C COssKeyboard::~COssKeyboard()
    {
    hangul_keyboard_delete(iHangulKeyboard);
    }

void COssKeyboard::ConstructL()
    {
    iHangulKeyboard=hangul_keyboard_new();
    }

EXPORT_C void COssKeyboard::SetValue(const TInt& aKey,const TChar& aValue)
    {
    hangul_keyboard_set_value(iHangulKeyboard,aKey,aValue);
    }

EXPORT_C void COssKeyboard::SetType(const TInt& aType)
    {
    hangul_keyboard_set_type(iHangulKeyboard,aType);
    }
