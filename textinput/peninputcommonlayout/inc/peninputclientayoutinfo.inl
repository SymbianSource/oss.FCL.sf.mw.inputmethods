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
// -----------------------------------------------------------------------------
//
inline const TInt CPeninputControlInfo::ControlID() const 
    {
    return iControlID;
    }

inline const TInt CPeninputControlInfo::ControlType() const 
    {
    return iControlType;
    }
    
inline const TInt CPeninputControlInfo::BeginRow() const 
    {
    return iBeginRow;
    }
     
inline const TInt CPeninputControlInfo::EndRow() const 
    {
    return iEndRow;
    }
    
inline const TInt CPeninputControlInfo::BeginColumn() const 
    {
    return iBeginColumn; 
    }

inline const TInt CPeninputControlInfo::EndColumn() const 
    {
    return iEndColumn;
    }

inline TInt CPeninputClientLayoutInfo::LayoutID() const 
    {
    return iLayoutID;        
    }

inline TInt CPeninputClientLayoutInfo::Columns() const 
    {
    return iColumns;        
    }

inline TInt CPeninputClientLayoutInfo::Rows() const 
    {
    return iRows;        
    }
    
inline RPointerArray<CPeninputControlInfo>& CPeninputClientLayoutInfo::ControlInfoList() 
    {
    return iControlInfoList;   
    }

// End Of File        
