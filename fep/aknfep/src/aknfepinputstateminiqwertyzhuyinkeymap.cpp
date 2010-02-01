/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            This file contains classes of phrase creation layout data management
*
*/













#include "aknfepinputstateminiqwertyzhuyinkeymap.h"
#include <PtiDefs.h>

const TInt16  KZhuyinSymbolUnicode = 0x2462;

_LIT(KToneMark1, "\x02D9");
_LIT(KToneMark2, "\x02CA");
_LIT(KToneMark3, "\x02C7");
_LIT(KToneMark4, "\x02CB");
_LIT(KToneMark5, "\x0020");
_LIT(KRightBracket, "(");
_LIT(KLeftBracket, ")");

// ---------------------------------------------------------------------------
// TAknFepInputStateMiniQwertyZhuyinKeyMap::GetKeyStroke
// Get the key stroke.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateMiniQwertyZhuyinKeyMap::GetKeyStroke(
    CPtiEngine* aPtiEngine, TInt& aKey, TDes& aKeystroke )
    {
    TBuf<KMaxName> lowerdata;
    aPtiEngine->MappingDataForKey( (TPtiKey)aKey, lowerdata, EPtiCaseLower );

    TBuf<2> zhuyinStoke;
    if ( lowerdata.Length( ) > 0 )
        {
        for (TInt i=0; i<lowerdata.Length()-1; i++ )
            {
            if ( lowerdata[i]==KZhuyinSymbolUnicode )
                {
                zhuyinStoke.Append( lowerdata.Mid( i+1, 1 ) );
                }
            }
        }

    if ( zhuyinStoke.Compare( KToneMark1 ) == 0
        || zhuyinStoke.Compare( KToneMark2 ) == 0
        || zhuyinStoke.Compare( KToneMark3 ) == 0
        || zhuyinStoke.Compare( KToneMark4 ) == 0
        || zhuyinStoke.Compare( KToneMark5 ) == 0 )
        {
        aKeystroke.Copy( zhuyinStoke );
        }
    else
        {
        aKeystroke.Append( KRightBracket );
        aKeystroke.Append( zhuyinStoke );
        aKeystroke.Append( KLeftBracket );
        }
    }

// End of File
