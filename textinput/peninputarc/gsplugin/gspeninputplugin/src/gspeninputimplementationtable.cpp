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
* Description:  ECOM proxy table for GSPenInputPlugin.
*
*/


#include <e32std.h> 
#include <implementationproxy.h>

#include "gspeninputplugin.h"
#include "gspeninputimple.h"

const TImplementationProxy KGSPenInputPluginImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY(0x1027510C, CGSPenInputPlugin::NewL),
    IMPLEMENTATION_PROXY_ENTRY(0x1028185A, CGSPenInputImple::NewL)
    };

// ---------------------------------------------------------
// Map the interface UIDs to implementation factory functions
// ---------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(KGSPenInputPluginImplementationTable) 
                  / sizeof(TImplementationProxy);

    return KGSPenInputPluginImplementationTable;
    }


// ---------------------------------------------------------
// Exported proxy for instantiation method resolution
// ---------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll( TDllReason /*aReason*/ )
    {
    return(KErrNone);
    }

#endif // EKA2

// End Of File
