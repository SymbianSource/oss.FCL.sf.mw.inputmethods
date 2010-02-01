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
*       Boilerplate for FEP's Avkon UI plugin.
*
*/











#include <e32std.h>
#include <ecom/implementationproxy.h>

#include "AknFepUiAvkonImpl.h"
#include <AknFepUiInterfacePlugin.h>

class CFepUiAvkonImplPlugin : public CFepUiInterfacePlugin
	{
public:
	static CFepUiAvkonImplPlugin* NewL();
	TBool UseThisPlugin() const;
	CAknFepUIInterface* NewImplL();
	};


// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10285839, CFepUiAvkonImplPlugin::NewL)    
    };

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

    return ImplementationTable;
    }


CFepUiAvkonImplPlugin* CFepUiAvkonImplPlugin::NewL()
	{
	return new(ELeave) CFepUiAvkonImplPlugin;
	}

TBool CFepUiAvkonImplPlugin::UseThisPlugin() const
	{
	// AknFepUiInterface will try to load this plugin last, so it should
	// always be used if asked.
	return ETrue;
	}

CAknFepUIInterface* CFepUiAvkonImplPlugin::NewImplL()
	{
	return CAknFepUIAvkonImpl::NewL();
	}
