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
* Description:  the data converter of peninput
*
*/


// System includes
#include <aknfeppeninputenums.h>

// User includes
#include "peninputdataconverter.h"

// Const
const TUint KNumToBuffer = sizeof( TUint )/sizeof( TUint16);
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputDataConverter::CPeninputDataConverter
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputDataConverter::CPeninputDataConverter()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputDataConverter::~CPeninputDataConverter
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputDataConverter::~CPeninputDataConverter()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputDataConverter::IntToDesc
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputDataConverter::IntToDesc( TUint aNum, 
                                                 TDes16& aBuf )
    {
    aBuf.Zero();
    // Convert integer number to be string
    aBuf.Append( reinterpret_cast<TUint16*>( &aNum ),KNumToBuffer );
    }

// ---------------------------------------------------------------------------
// CPeninputDataConverter::CombinationToDesc
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputDataConverter::CombinationToDesc( 
    TBool aBool, TUint aNum, TDes16& aBuf )
    {
    aBuf.Zero();
    // Convert integer number to be string
    aBuf.Append( reinterpret_cast<TUint16*>( &aBool ), KNumToBuffer );
    aBuf.Append( reinterpret_cast<TUint16*>( &aNum ), KNumToBuffer );
    }
    
// ---------------------------------------------------------------------------
// CPeninputDataConverter::AnyToInt
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputDataConverter::AnyToInt( TAny* aOrginal )
    {
    return *( reinterpret_cast<TInt*>( aOrginal ) );
    }

// ---------------------------------------------------------------------------
// CPeninputDataConverter::ShiftCapslockByCase
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputDataConverter::ShiftCapslockByCase( TInt aCase, 
                                                           TInt& aShiftStatus, 
                                                           TInt& aCapsStatus )
    {
    switch ( aCase )
        {
        case ECaseUpper:
            aShiftStatus = 0;
            aCapsStatus = 1;
            break;
        case ECaseText:
            aShiftStatus = 1;
            aCapsStatus = 0;
            break;
        case ECaseLower:
        case ECaseInverseText:        
        case ECaseInvalide:
        default:
            aShiftStatus = 0;
            aCapsStatus = 0;        
            break;   
        }
    }
    
EXPORT_C TInt CPeninputDataConverter::FepCaseByCaseId( TInt aCaseId )
    {
    switch ( aCaseId )
        {
        case 0:
            return ECaseLower;
        case 1:
            return ECaseText;
        case 2:
            return ECaseUpper;
        case 3:        
            return ECaseInverseText;
        default:
            return ECaseLower;
        }        
    }
