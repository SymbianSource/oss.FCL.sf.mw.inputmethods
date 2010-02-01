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
* Description:  range bar layout inline function 
*
*/


// -----------------------------------------------------------------------------
// CPeninputRangeInfo::RangeId
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputRangeInfo::RangeId()
    {
    return iRangeId;
    }

// -----------------------------------------------------------------------------
// CPeninputRangeInfo::ResponseStyle
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TPeninputRangeResponseStyle CPeninputRangeInfo::ResponseStyle()
    {
    return iResponseStyle;
    }

// -----------------------------------------------------------------------------
// CPeninputRangeInfo::ClientLayoutId
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputRangeInfo::ClientLayoutId()
    {
    return iClientLayoutId;
    }

// -----------------------------------------------------------------------------
// CPeninputRangeInfo::VkbLayoutId
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputRangeInfo::VkbLayoutId()
    {
    return iVkbLayoutId;
    }

// -----------------------------------------------------------------------------
// CPeninputRangeBarInfo::RangeStyle
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputRangeBarInfo::RangeStyle()
    {
    return iRangeStyle;
    }


// -----------------------------------------------------------------------------
// CPeninputRangeBarInfo::Ranges
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CPeninputRangeInfo>& CPeninputRangeBarInfo::Ranges()
    {
    return iRanges;
    }
