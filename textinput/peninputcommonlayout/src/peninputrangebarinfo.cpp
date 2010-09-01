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
* Description:  range bar layout implementation
*
*/


// User includes
#include "peninputrangebarinfo.h"

// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// CPeninputRangeInfo::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputRangeInfo* CPeninputRangeInfo::NewL( 
    TResourceReader& aReader )                            
    {
    CPeninputRangeInfo* self = NewLC( aReader );
    CleanupStack::Pop( self );
    
    return self;
    }

// ------------------------------------------------------------------------
// CPeninputRangeInfo::NewLC
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputRangeInfo* CPeninputRangeInfo::NewLC( 
    TResourceReader& aReader )
    {
    CPeninputRangeInfo* self = new ( ELeave ) CPeninputRangeInfo();

    CleanupStack::PushL( self );
    self->ConstructL( aReader );

    return self;
    }

// ------------------------------------------------------------------------
// CPeninputRangeInfo::~CPeninputRangeInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputRangeInfo::~CPeninputRangeInfo()
    {
    }

// ------------------------------------------------------------------------
// CPeninputRangeInfo::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputRangeInfo::ConstructL( TResourceReader& aReader )
    {
    ConstructFromResourceL( aReader );
    }

// ------------------------------------------------------------------------
// CPeninputRangeInfo::ConstructFromResourceL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputRangeInfo::ConstructFromResourceL( TResourceReader& aReader )
    {
    User::LeaveIfError( iRangeId = aReader.ReadInt16() );    
    
    TInt responseStyle = aReader.ReadInt16();
    
    switch ( responseStyle )
        {     
        case 0:
            {
            iResponseStyle = EPeninputRangeResponseNoAction;
            }
            break;
        case 1:
            {
            iResponseStyle = EPeninputRangeResponseSwitchRange;
            }
            break;
        case 2:
            {
            iResponseStyle = EPeninputRangeResponsePopupChoiceItem;
            }
            break;
        default:
            {
            iResponseStyle = EPeninputRangeResponseNoAction;
            }
            break;
        }

    iClientLayoutId = aReader.ReadInt16();
    iVkbLayoutId = aReader.ReadInt16();

    }

// ------------------------------------------------------------------------
// CPeninputRangeBarInfo::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputRangeBarInfo* CPeninputRangeBarInfo::NewL( 
    TResourceReader& aReader )                                    
    {
    CPeninputRangeBarInfo* self = NewLC( aReader );
    CleanupStack::Pop( self );
    
    return self;
    }

// ------------------------------------------------------------------------
// CPeninputRangeBarInfo::NewLC
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputRangeBarInfo* CPeninputRangeBarInfo::NewLC( 
    TResourceReader& aReader )
    {
    CPeninputRangeBarInfo* self = new ( ELeave ) CPeninputRangeBarInfo();

    CleanupStack::PushL( self );
    self->ConstructL( aReader );

    return self;
    }

// ------------------------------------------------------------------------
// CPeninputRangeBarInfo::~CPeninputRangeBarInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputRangeBarInfo::~CPeninputRangeBarInfo()
    {
    iRanges.ResetAndDestroy();
    iRanges.Close();
    }

// ------------------------------------------------------------------------
// CPeninputRangeBarInfo::FindRange
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputRangeInfo* CPeninputRangeBarInfo::FindRange( TInt aRangeId )
    {
    const TInt count = iRanges.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iRanges[i]->RangeId() == aRangeId )
            {
            return iRanges[i];
            }
        }

    return NULL;
    }

// ------------------------------------------------------------------------
// CPeninputRangeBarInfo::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputRangeBarInfo::ConstructL( TResourceReader& aReader )
    {
    ConstructFromResourceL( aReader );
    }

// ------------------------------------------------------------------------
// CPeninputRangeBarInfo::ConstructFromResourceL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputRangeBarInfo::ConstructFromResourceL( 
    TResourceReader& aReader )
    {
    iRangeStyle = aReader.ReadInt16();
    
    TInt count = aReader.ReadInt16();
    for ( TInt i = 0; i < count; i++ )
        {
        CPeninputRangeInfo* rangeInfo = CPeninputRangeInfo::NewL( aReader );
        iRanges.Append( rangeInfo );
        }
    }
