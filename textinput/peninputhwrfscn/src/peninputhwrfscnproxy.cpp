/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr
*
*/


#include <e32std.h>
#include <implementationproxy.h>
#include <peninputlayoutplugininterface.h>
#include "peninputhwrfscnlayout.h"


/******************************************************

	CFepUiLayoutInterface Implementation

*******************************************************/

// An implementation of the CFepUiLayoutInterface definition
class CPeninputHwrfscnLayoutImpl : public CFepUiLayoutInterface
    {
public:
    // Standardised safe construction which leaves nothing the cleanup stack.
    static CPeninputHwrfscnLayoutImpl* NewL( TAny* aInitParams );

    // Implementation of CFepUiLayoutInterface::CreateFepUiLayoutL
    MPenUiLayoutBase* CreateFepUiLayoutL( MLayoutOwner* aLayoutOwner, 
        const TAny*  aInitData );
    };

// Construction and destruction functions
CPeninputHwrfscnLayoutImpl* 
    CPeninputHwrfscnLayoutImpl::NewL( TAny* /*aInitParams*/ )
    {
    CPeninputHwrfscnLayoutImpl* self = new( ELeave ) CPeninputHwrfscnLayoutImpl();
    return self;
    }

// Implementation of CPeninputHwrfscnLayoutImpl
MPenUiLayoutBase* CPeninputHwrfscnLayoutImpl::CreateFepUiLayoutL( 
    MLayoutOwner* aLayoutOwner,
    const TAny* aInitData )
    {
    CPeninputHwrfscnLayout* uiLayout = 
        CPeninputHwrfscnLayout::NewL( aLayoutOwner, aInitData );
    return uiLayout;
    }

/******************************************************

	ECom Implementation Proxy

*******************************************************/

// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] =
    {
    //    {{0x2000B132},    CPeninputHwrfscnLayoutImpl::NewL}
    IMPLEMENTATION_PROXY_ENTRY( 0x2000B132, CPeninputHwrfscnLayoutImpl::NewL )
    };

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }

//End of file
