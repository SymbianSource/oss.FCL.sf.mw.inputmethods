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
* Description:  ECOM proxy table for GSLangPlugin.
*
*/


// User includes
#include "GSLangPlugin.h"

// System includes
#include <e32std.h>
#include <implementationproxy.h>

// Constants
const TImplementationProxy KGSLangPluginImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x10275067,	CGSLangPlugin::NewL )
	};


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
	{
	aTableCount = sizeof( KGSLangPluginImplementationTable ) 
        / sizeof( TImplementationProxy );
	return KGSLangPluginImplementationTable;
	}


// End of File
