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
* Description:  client layout implementation
*
*/


// User includes
#include "peninputclientlayoutinfo.h"


// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// CPeninputControlInfo::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputControlInfo* CPeninputControlInfo::NewL( 
    TResourceReader& aReader )
    {
    CPeninputControlInfo* self = new ( ELeave ) CPeninputControlInfo();
    CleanupStack::PushL( self );
    self->ConstructL( aReader );
    CleanupStack::Pop( self );
    
    return self;
    }

// ------------------------------------------------------------------------
// CPeninputControlInfo::~CPeninputControlInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputControlInfo::~CPeninputControlInfo()
    {
    }

// ------------------------------------------------------------------------
// CPeninputControlInfo::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputControlInfo::ConstructL( TResourceReader& aReader )
    {
    ConstructFromResourceL( aReader );
    }

// ------------------------------------------------------------------------
// CPeninputControlInfo::ConstructFromResourceL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputControlInfo::ConstructFromResourceL( TResourceReader& aReader )
    {
    iControlID = aReader.ReadInt16();
    iControlType = aReader.ReadInt16();
    iBeginRow = aReader.ReadInt16();
    iBeginColumn = aReader.ReadInt16();
    iEndRow = aReader.ReadInt16();
    iEndColumn = aReader.ReadInt16();
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputClientLayoutInfo* CPeninputClientLayoutInfo::NewL( 
    TResourceReader& aReader )
    {
    CPeninputClientLayoutInfo* self = NewLC( aReader );
    CleanupStack::Pop( self );

    return self;
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::NewLC
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputClientLayoutInfo* CPeninputClientLayoutInfo::NewLC( 
    TResourceReader& aReader )
    {
    CPeninputClientLayoutInfo* self = 
        new ( ELeave ) CPeninputClientLayoutInfo();
    CleanupStack::PushL( self );
    self->ConstructL( aReader );

    return self;
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::~CPeninputClientLayoutInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputClientLayoutInfo::~CPeninputClientLayoutInfo()
    {
    iControlInfoList.ResetAndDestroy();
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::ConstructFromResourceL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C void CPeninputClientLayoutInfo::ConstructFromResourceL( 
    TResourceReader& aReader )
    {
    iLayoutID = aReader.ReadInt16();
    iColumns = aReader.ReadInt16();
    iRows = aReader.ReadInt16();

    const TInt count = aReader.ReadInt16();
    for ( TInt i = 0; i < count; i++ )
        {
        CPeninputControlInfo* controlInfo = 
            CPeninputControlInfo::NewL( aReader );
        AddControlInfo( controlInfo );
        }
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::FindControlInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputControlInfo* CPeninputClientLayoutInfo::FindControlInfo( 
    const TInt aControlID )
    {
    const TInt count = iControlInfoList.Count();
    
    for ( TInt i = 0; i < count; i++ )
        {
        if ( iControlInfoList[i]->ControlID() == aControlID )
            {
            return iControlInfoList[i];
            }
        }

    return NULL;
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::AddControlInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
// 
EXPORT_C void CPeninputClientLayoutInfo::AddControlInfo( 
    CPeninputControlInfo* aControlInfo )
    {
    iControlInfoList.Append( aControlInfo );
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::RemoveControlInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C void CPeninputClientLayoutInfo::RemoveControlInfo( 
    CPeninputControlInfo* aControlInfo )
    {
    CPeninputControlInfo* controlInfo = 
        FindControlInfo( aControlInfo->ControlID() );
    if ( controlInfo )
        {
        RemoveControlInfo( controlInfo->ControlID() );
        }
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::RemoveControlInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C void CPeninputClientLayoutInfo::RemoveControlInfo( 
    const TInt aControlID )
    {
    const TInt count = iControlInfoList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iControlInfoList[i]->ControlID() == aControlID )
            {
            CPeninputControlInfo* controlInfo = iControlInfoList[i];
            delete controlInfo;
            iControlInfoList.Remove( i );
            break;
            }
        }
    }

// ------------------------------------------------------------------------
// CPeninputClientLayoutInfo::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputClientLayoutInfo::ConstructL( TResourceReader& aReader )
    {
    ConstructFromResourceL( aReader );
    }
