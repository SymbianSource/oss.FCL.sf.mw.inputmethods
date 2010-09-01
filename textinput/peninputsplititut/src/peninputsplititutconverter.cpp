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
* Description:
*
*/
#include "peninputsplititutconverter.h"

const TUint16 KSegment = 0x0020;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToInt
// ---------------------------------------------------------------------------
//
TInt TItutDataConverter::AnyToInt(TAny* aOriginal)
    {
    return *(reinterpret_cast<TInt*>(aOriginal));
    }
    
// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToRect
// ---------------------------------------------------------------------------
//
TRect TItutDataConverter::AnyToRect(TAny* aOriginal)
    {
    return *(reinterpret_cast<TRect*>(aOriginal));
    }
    
// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToFont
// ---------------------------------------------------------------------------
//
CFont* TItutDataConverter::AnyToFont(TAny* aOriginal)
    {
    return reinterpret_cast<CFont*>(aOriginal);
    }
    
// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToPoint
// ---------------------------------------------------------------------------
//
TPoint TItutDataConverter::AnyToPoint(TAny* aOriginal)
    {
    return *(reinterpret_cast<TPoint*>(aOriginal));
    }
    
// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToRgb
// ---------------------------------------------------------------------------
//
TRgb TItutDataConverter::AnyToRgb(TAny* aOriginal)
    {
    return *(reinterpret_cast<TRgb*>(aOriginal));
    }
    
// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToSize
// ---------------------------------------------------------------------------
//
TSize TItutDataConverter::AnyToSize(TAny* aOriginal)
    {
    return *(reinterpret_cast<TSize*>(aOriginal));
    }
    
// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToRptrArray
// ---------------------------------------------------------------------------
//
RPointerArray<HBufC>& TItutDataConverter::AnyToRptrArray(TAny* aOriginal)    
    {
    return *(reinterpret_cast<RPointerArray<HBufC>*>(aOriginal));
    }

// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToRectArray
// ---------------------------------------------------------------------------
//
RArray<TRect>& TItutDataConverter::AnyToRectArray(TAny* aOriginal)
    {
    return *(reinterpret_cast<RArray<TRect>*>(aOriginal));
    }

// ---------------------------------------------------------------------------
// TItutDataConverter::AnyToTextLine
// ---------------------------------------------------------------------------
//
TAknTextLineLayout TItutDataConverter::AnyToTextLine(TAny* aOriginal)
    {
    return *(reinterpret_cast<TAknTextLineLayout*>(aOriginal));
    }

// ---------------------------------------------------------------------------
// TItutDataConverter::ConvertChnPhraseCandidateL
// ---------------------------------------------------------------------------
//
void TItutDataConverter::ConvertChnPhraseCandidateL(const TDesC& aSource, 
                                                    RPointerArray<HBufC>& aList)
    {
    TInt start = 0;
    TInt length = 0;

    for (TInt ii = 0; ii < aSource.Length(); ii++)
        {
        if (aSource[ii] == KSegment)
            {
            TPtrC segment(aSource.Ptr() + start, length);
            aList.AppendL(segment.AllocL());
            start += (length + 1);
            length = 0;
            }
        else
            {
            length++;
            }
        }

    if (length)
        {
        TPtrC segm(aSource.Ptr() + start, length);
        aList.AppendL(segm.AllocL());
        }
    }

// ---------------------------------------------------------------------------
// TItutDataConverter::ConvertNonPhraseCandidateL
// ---------------------------------------------------------------------------
//
void TItutDataConverter::ConvertNonPhraseCandidateL(const TDesC& aSource, 
                                                    RPointerArray<HBufC>& aList)
    {
    TBuf<1> str;
    for (TInt ii = 0; ii < aSource.Length(); ii++)
        {
        str.Zero();
        str.Append(aSource[ii]);
        aList.AppendL(str.AllocL());
        }
    }

// End Of File
