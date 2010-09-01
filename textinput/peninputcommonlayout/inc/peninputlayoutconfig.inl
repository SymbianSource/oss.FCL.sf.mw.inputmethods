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
* Description:  common layout config inline functions
*
*/


// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::Language
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputLayoutConfig::Language()
    {
    return iLanguage;
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::LanguageString
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline HBufC* CPeninputLayoutConfig::LanguageString()
    {
    return iLanguageString;
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::CaseSensitive
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TBool CPeninputLayoutConfig::CaseSensitive()
    {
    return iCaseSensitive;
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::GuideLine
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputLayoutConfig::GuideLine()
  {   
    return iGuideLine;
  }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::PositionTopLine
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputLayoutConfig::PositionTopLine()
  {   
    return iPositionTopLine;
  }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::PositionBottomLine
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline TInt CPeninputLayoutConfig::PositionBottomLine()
  {   
    return iPositionBottomLine;
  }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::ClientLayoutInfoList
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CPeninputClientLayoutInfo>& 
    CPeninputLayoutConfig::ClientLayoutInfoList()
    {
    return iClientLayoutInfoList;
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::VkbLayoutInfoList
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CPeninputVkbLayoutInfo>& 
    CPeninputLayoutConfig::VkbLayoutInfoList()
    {
    return iVkbLayoutInfoList;
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::ShiftCapsInfoList
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline RPointerArray<CPeninputShiftCapsInfo>& 
    CPeninputLayoutConfig::ShiftCapsInfoList()
    {
    return iShiftCapsInfoList;  
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutConfig::RangeBarInfo
// (other items were commented in a header)
// -----------------------------------------------------------------------------
//
inline CPeninputRangeBarInfo* 
    CPeninputLayoutConfig::RangeBarInfo()
    {
    return iRangeBarInfo;
    }

inline TInt CPeninputLayoutConfig::RangebarResId()
    {
    return iRangeBarResId;
    }
       