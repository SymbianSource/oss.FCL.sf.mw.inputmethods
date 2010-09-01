/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#include <e32std.h>
#include <implementationproxy.h>

// User includes
#include "PeninputPluginKr.hrh"
#include "PeninputPluginKr.h"

// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] = 
    {
    //lint !e611    
    IMPLEMENTATION_PROXY_ENTRY( KAknFepImePluginImplementationId, CPeninputPluginKr::NewL ) 
    };

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }

//End Of File
