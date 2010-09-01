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
* Description:  image layout inline function 
*
*/


// -----------------------------------------------------------------------------
// CPeninputControlInfo::ControlID
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline const TInt CPeninputControlInfo::ControlID() const 
    {
    return iControlID;
    }

// -----------------------------------------------------------------------------
// CPeninputControlInfo::ControlType
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline const TInt CPeninputControlInfo::ControlType() const 
    {
    return iControlType;
    }

// -----------------------------------------------------------------------------
// CPeninputControlInfo::BeginRow
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline const TInt CPeninputControlInfo::BeginRow() const 
    {
    return iBeginRow;
    }

// -----------------------------------------------------------------------------
// CPeninputControlInfo::EndRow
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline const TInt CPeninputControlInfo::EndRow() const 
    {
    return iEndRow;
    }

// -----------------------------------------------------------------------------
// CPeninputControlInfo::BeginColumn
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline const TInt CPeninputControlInfo::BeginColumn() const 
    {
    return iBeginColumn; 
    }

// -----------------------------------------------------------------------------
// CPeninputControlInfo::EndColumn
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline const TInt CPeninputControlInfo::EndColumn() const 
    {
    return iEndColumn;
    }

// -----------------------------------------------------------------------------
// CPeninputClientLayoutInfo::LayoutID
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputClientLayoutInfo::LayoutID() const 
    {
    return iLayoutID;        
    }

// -----------------------------------------------------------------------------
// CPeninputClientLayoutInfo::Columns
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputClientLayoutInfo::Columns() const 
    {
    return iColumns;        
    }

// -----------------------------------------------------------------------------
// CPeninputClientLayoutInfo::Rows
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputClientLayoutInfo::Rows() const 
    {
    return iRows;        
    }

// -----------------------------------------------------------------------------
// CPeninputClientLayoutInfo::ControlInfoList
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CPeninputControlInfo>& 
    CPeninputClientLayoutInfo::ControlInfoList() 
    {
    return iControlInfoList;   
    }
