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
*       Provides the TAknFepInputStateChineseFindQwerty methods.
*
*/












#include "AknFepUiInputStateChineseFindQwerty.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManagerUIInterface.h"

#include <e32keys.h>
#include <PtiEngine.h>
#include <PtiDefs.h>                //keys

TAknFepInputStateChineseFindQwerty::TAknFepInputStateChineseFindQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    iState = EQwerty;
    }

TBool TAknFepInputStateChineseFindQwerty::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;
   
    if(!IsQwertyKey(aKey))
        {
        ret = EFalse;
        }
    else if (aLength == EShortKeyPress && iOwner->IsValidChineseInputKeyQwerty(aKey)) 
        {
        ptiengine->AppendKeyPress((TPtiKey)aKey);
        TPtrC text = ptiengine->GetPhoneticSpelling(1);

        if(text.Length()) 
            {
            fepMan->NewCharacterL(text);
            fepMan->CommitInlineEditL();
            ptiengine->ClearCurrentWord();
            }
        else
            {
            ret = EFalse;
            }
        }
    return ret;
    }

TBool TAknFepInputStateChineseFindQwerty::IsQwertyKey(const TInt aKey)
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

TBool TAknFepInputStateChineseFindQwerty::IsNumberKey(const TInt aKey)
    {
    TBool response = EFalse;
    if (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9)
        {
        response = ETrue;
        }
    return response;
    }

// End of file
