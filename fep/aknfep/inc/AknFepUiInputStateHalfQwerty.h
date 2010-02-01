/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateHalfQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_HALF_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_HALF_QWERTY_H__

#include <w32std.h>
#include "AknFepUiInputStateBase.h"

class TAknFepInputStateHalfQwerty : public TAknFepInputStateBase
    {
public:
    TAknFepInputStateHalfQwerty(MAknFepUIManagerStateInterface* aOwner); 

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    void HandleCaseChange(TInt aKey, TBool aIsCharacter);
    TBool IsCharacterKey(const TInt aKey);

  
public: // Functions from MPtiObserver interface
    void KeyTimerExpired();
    
protected:
    TBool TryGetValidCharForKey(const TInt& aKey, TChar& aChar);
    TBool TryGetValidCharForKeyAndCase(const TInt& aKey, TChar& aChar, TPtiTextCase aCase);
    static TInt TimerExpire(TAny* aPtr);
    TInt ChrKeyTimerExpired();
    TBool IsKeymapped(TInt aKey);
    TBool CheckHasValidNumerKeyMappedL(TInt aKey, TBool &aLangChanged);	
    };

#endif //__AKN_FEP_UI_INPUT_STATE_HALF_QWERTY_H__

// End of file
