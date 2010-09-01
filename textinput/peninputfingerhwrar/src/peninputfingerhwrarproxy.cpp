/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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
#include "peninputfingerhwrarlayout.h"


/******************************************************

	CFepUiLayoutInterface Implementation

*******************************************************/

// An implementation of the CFepUiLayoutInterface definition
class CPeninputFingerHwrArLayoutImpl : public CFepUiLayoutInterface
    {
public:
    // Standardised safe construction which leaves nothing the cleanup stack.
    static CPeninputFingerHwrArLayoutImpl* NewL( TAny* aInitParams );

    // Implementation of CFepUiLayoutInterface::CreateFepUiLayoutL
    MPenUiLayoutBase* CreateFepUiLayoutL( MLayoutOwner* aLayoutOwner, 
        const TAny*  aInitData );
    };

// Construction and destruction functions
CPeninputFingerHwrArLayoutImpl* 
    CPeninputFingerHwrArLayoutImpl::NewL( TAny* /*aInitParams*/ )
    {
    CPeninputFingerHwrArLayoutImpl* self = new( ELeave ) CPeninputFingerHwrArLayoutImpl();
    return self;
    }

// Implementation of CPeninputFingerHwrArLayout
MPenUiLayoutBase* CPeninputFingerHwrArLayoutImpl::CreateFepUiLayoutL( 
    MLayoutOwner* aLayoutOwner,
    const TAny* aInitData )
    {
    CPeninputFingerHwrArLayout* uiLayout = 
        CPeninputFingerHwrArLayout::NewL( aLayoutOwner, aInitData );
    return uiLayout;
    }

/******************************************************

	ECom Implementation Proxy

*******************************************************/

// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x2001E2C4, CPeninputFingerHwrArLayoutImpl::NewL )
    };

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }

//End of file
