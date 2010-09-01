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
* Description:  vkb layout inline function 
*
*/

    
// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::Unicode
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline HBufC* CPeninputVkbKeyInfo::Unicode() const
    {
    return iUnicode;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::ScanCode
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbKeyInfo::ScanCode() const
    {
    return iScanCode;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::BeginRow
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbKeyInfo::BeginRow() const
    {
    return iBeginRow;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::BeginColumn
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbKeyInfo::BeginColumn() const
    {
    return iBeginColumn;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::EndRow
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbKeyInfo::EndRow() const
    {
    return iEndRow;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::EndColumn
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbKeyInfo::EndColumn() const
    {
    return iEndColumn;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::HighlightBmpIndex
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbKeyInfo::HighlightBmpIndex() const 
    {
    return iHighlightBmpIndex;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::HighlightMaskBmpIndex
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbKeyInfo::HighlightMaskBmpIndex() const 
    {
    return iHighlightMaskBmpIndex;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbKeyInfo::Key
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline CVirtualKey* CPeninputVkbKeyInfo::Key() const 
    {
    return iKey;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::LayoutID
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbLayoutInfo::LayoutID() const
    {
    return iLayoutID;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::Columns
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbLayoutInfo::Columns() const
    {
    return iColumns;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::Rows
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbLayoutInfo::Rows() const
    {
    return iRows;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::UnitWidth
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbLayoutInfo::UnitWidth() const
    {
    return iUnitWidth;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::UnitHeight
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbLayoutInfo::UnitHeight() const
    {
    return iUnitHeight;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::UnitCharWidth
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbLayoutInfo::UnitCharWidth() const
    {
    return iUnitCharWidth;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::UnitCharHeight
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputVkbLayoutInfo::UnitCharHeight() const
    {
    return iUnitCharHeight;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::CharLeftTopPoint
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TPoint CPeninputVkbLayoutInfo::CharLeftTopPoint() const
    {
    return iCharLeftTopPoint;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::CharFont
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline const CFont* CPeninputVkbLayoutInfo::CharFont() const
    {
    return iFont;
    } 
    
// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::BgBmp
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline CFbsBitmap* CPeninputVkbLayoutInfo::BgBmp() const
    {
    return iBgBmp;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::BgMaskBmp
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline CFbsBitmap* CPeninputVkbLayoutInfo::BgMaskBmp() const 
    {
    return iBgMaskBmp;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::DimBmp
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline CFbsBitmap* CPeninputVkbLayoutInfo::DimBmp() const 
    {
    return iDimBmp;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::DimMaskBmp
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline CFbsBitmap* CPeninputVkbLayoutInfo::DimMaskBmp() const 
    {
    return iDimMaskBmp;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::KeyInfoList
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CPeninputVkbKeyInfo> 
    CPeninputVkbLayoutInfo::KeyInfoList() const 
    {
    return iKeyInfoList;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::HighlightBmps
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CFbsBitmap> 
    CPeninputVkbLayoutInfo::HighlightBmps() const 
    {
    return iHighlightBmps;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::HighlightMaskBmps
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CFbsBitmap> 
    CPeninputVkbLayoutInfo::HighlightMaskBmps() const
    {
    return iHighlightMaskBmps;     
    }
