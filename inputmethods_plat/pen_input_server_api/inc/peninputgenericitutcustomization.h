/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Pen input related enums
*
*/














#ifndef T_PENINPUTITUTCUSTOMIZATION_H
#define T_PENINPUTITUTCUSTOMIZATION_H

#include <aknfeppeninputenums.h>
#include <AknLayout2Def.h>

const TInt KInvalidBmp = -1;

// following standard keys, buttons etc,
// can be dimmed, or hide by application
enum TItutConfigurableKeys
    {
    EItutKeyArrowLeft,
    EItutKeyArrowRight,
    EItutKeyArrowUp,
    EItutKeyArrowDown,
    EItutKeyClear,
    EItutKeySwitchLayout,
    EItutKey1,
    EItutKey2,
    EItutKey3,
    EItutKey4,
    EItutKey5,
    EItutKey6,
    EItutKey7,
    EItutKey8,
    EItutKey9,
    EItutKeyStar,
    EItutKey0,
    EItutKeyHash
    };

// app can send following command to itut
enum TItutExternalCommand
    {
    EItutExtCmdCustomerState = ECmdPenInputImeCmdBase + 1,
    EItutExtCmdSetLayout,     // base layout can be latin, or number
    EItutExtCmdDimKey,        // all keys in TItutConfigurableKeys can be dimmed, or hide
    EItutExtCmdHideKey,       // only 1 key can be dimmed or hide at a time, param: key index
    EItutExtCmdAddKey,        // only 1 key can be added at a time, param: scancode, bitmap handle, new key's position index
    //EItutExtCmdSetPromptText,
    EItutExtCmdSizeChanged,
    EItutExtCmdSetIcfData,
    EItutExtCmdLast = ECmdPenInputImeCmdBase +1000
    };

// following positions are mapping with laf rect, which
// can be used by application to put their new key
enum TItutConfigurablePos
    {
    EItutPosArrowLeft,
    EItutPosArrowRight,
    EItutPosArrowUp,
    EItutPosArrowDown,
    EItutPosClear,
    EItutPosSwitchLayout,
    EItutPosSend,
    EItutPosDisconn,
    EItutPosOk,
    EItutPosCancel,
    EItutPosConfigurableLast = 1000
    };
    
enum TItutConfigurableLayout
    {
    EItutLayoutAlphabetic,
    EItutLayoutNumeric
    };

struct TItutBmp
    {
    TInt iBmpHandle;
    TInt iMaskBmpHandle;	

    TItutBmp(TInt aBmpHandle, TInt aMaskBmpHandle)
        :iBmpHandle(aBmpHandle), iMaskBmpHandle(aMaskBmpHandle)
        {
        }

    TItutBmp()
        :iBmpHandle(KInvalidBmp), iMaskBmpHandle(KInvalidBmp)
        {
        }
    };

struct TItutKey
    {
    TInt iScanCode;
    TItutBmp iForgroundBmp;
    TInt iPos; // one value of TItutConfigurablePos
    TInt iInnerPos; // one value of TItutConfigurablePos
    const TDesC& iText;
    TAknTextLineLayout iTextFormat;
    };

#endif  //T_PENINPUTITUTCUSTOMIZATION_H

//End Of File
