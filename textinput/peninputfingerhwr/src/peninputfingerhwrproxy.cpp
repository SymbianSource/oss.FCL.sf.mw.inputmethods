/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ECom entry point for finger hwr
*
*/

#include <e32std.h>
#include <implementationproxy.h>
#include <peninputlayoutplugininterface.h>
#include "peninputfingerhwrlayout.h"


/******************************************************

	CFepUiLayoutInterface Implementation

*******************************************************/

// An implementation of the CFepUiLayoutInterface definition
class CPeninputFingerHwrLayoutImpl : public CFepUiLayoutInterface
    {
public:
    // Standardised safe construction which leaves nothing the cleanup stack.
    static CPeninputFingerHwrLayoutImpl* NewL( TAny* aInitParams );

    // Implementation of CFepUiLayoutInterface::CreateFepUiLayoutL
    MPenUiLayoutBase* CreateFepUiLayoutL( MLayoutOwner* aLayoutOwner, 
        const TAny*  aInitData );
    };

// Construction and destruction functions
CPeninputFingerHwrLayoutImpl* 
    CPeninputFingerHwrLayoutImpl::NewL( TAny* /*aInitParams*/ )
    {
    CPeninputFingerHwrLayoutImpl* self = new( ELeave ) CPeninputFingerHwrLayoutImpl();
    return self;
    }

// Implementation of CPeninputFingerHwrLayout
MPenUiLayoutBase* CPeninputFingerHwrLayoutImpl::CreateFepUiLayoutL( 
    MLayoutOwner* aLayoutOwner,
    const TAny* aInitData )
    {
    CPeninputFingerHwrLayout* uiLayout = 
        CPeninputFingerHwrLayout::NewL( aLayoutOwner, aInitData );
    return uiLayout;
    }

/******************************************************

	ECom Implementation Proxy

*******************************************************/

// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x2001CB83, CPeninputFingerHwrLayoutImpl::NewL )
    };

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }

//End of file
