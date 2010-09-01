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
* Description:  shift&caps info implementation
*
*/


// User includes
#include "peninputshiftcapsInfo.h"


// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// CPeninputShiftCapsInfo::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputShiftCapsInfo* CPeninputShiftCapsInfo::NewL( 
    TResourceReader& aReader )
    {
    CPeninputShiftCapsInfo* self = NewLC( aReader );
    CleanupStack::Pop( self );
    
    return self;
    }

// ------------------------------------------------------------------------
// CPeninputShiftCapsInfo::NewLC
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputShiftCapsInfo* CPeninputShiftCapsInfo::NewLC( 
    TResourceReader& aReader )
    {
    CPeninputShiftCapsInfo* self = new ( ELeave ) CPeninputShiftCapsInfo();

    CleanupStack::PushL( self );
    self->ConstructL( aReader );

    return self;
    }

// ------------------------------------------------------------------------
// CPeninputShiftCapsInfo::~CPeninputShiftCapsInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputShiftCapsInfo::~CPeninputShiftCapsInfo()
    {
    iShiftCapsSingleList.ResetAndDestroy();
    iShiftCapsSingleList.Close();
    }

// ------------------------------------------------------------------------
// CPeninputShiftCapsInfo::FindShiftCapsSingleById
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputShiftCapsInfo::TPeninputShiftCapsSingle* 
    CPeninputShiftCapsInfo::FindShiftCapsSingleById( TInt aId )
    {
    const TInt count = iShiftCapsSingleList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iShiftCapsSingleList[i]->iId == aId )
            {
            return iShiftCapsSingleList[i];
            }
        }

    return NULL;
    }

// ------------------------------------------------------------------------
// CPeninputShiftCapsInfo::FindShiftCapsSingleByCase
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputShiftCapsInfo::TPeninputShiftCapsSingle* 
    CPeninputShiftCapsInfo::FindShiftCapsSingleByCase( TInt aCase )
    {
    const TInt count = iShiftCapsSingleList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iShiftCapsSingleList[i]->iCaseUsed == aCase )
            {
            return iShiftCapsSingleList[i];
            }
        }

    return NULL;
    }
    
// ------------------------------------------------------------------------
// CPeninputShiftCapsInfo::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputShiftCapsInfo::ConstructL( TResourceReader& aReader )
    {
    ConstructFromResourceL( aReader );
    }

// ------------------------------------------------------------------------
// CPeninputShiftCapsInfo::ConstructFromResourceL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputShiftCapsInfo::ConstructFromResourceL( 
    TResourceReader& aReader )
    {
    iRangeId = aReader.ReadInt16();

    const TInt count = aReader.ReadInt16();
    for ( TInt i = 0; i < count; i++ )
        {
        TPeninputShiftCapsSingle* shiftCapsSingle = 
            new ( ELeave ) TPeninputShiftCapsSingle();
        
        shiftCapsSingle->iId = aReader.ReadInt16();
        shiftCapsSingle->iVkbLayoutId = aReader.ReadInt16();
        shiftCapsSingle->iCaseUsed = aReader.ReadInt16();

        iShiftCapsSingleList.Append( shiftCapsSingle );
        }
    }
