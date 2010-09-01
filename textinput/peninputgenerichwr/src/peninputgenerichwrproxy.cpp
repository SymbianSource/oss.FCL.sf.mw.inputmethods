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
* Description:  Implementation to Proxy
*
*/


// System includes
#include <e32std.h>
#include <implementationproxy.h>

// User includes
#include "peninputgenerichwrlayout.h"
#include "peninputhwrarabiclayout.h"
// ---------------------------------------------------------
// Map the interface UIDs to implementation factory functions
// ---------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x1028235B, 
                                CPeninputGenericHwrLayoutImp::NewL ),
    IMPLEMENTATION_PROXY_ENTRY(0x20019599,
                                CPeninputHwrBxAbLayoutImp::NewL)
    };

// ---------------------------------------------------------
// Exported proxy for instantiation method resolution
// ---------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
    {
    aTableCount = 
        sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }
