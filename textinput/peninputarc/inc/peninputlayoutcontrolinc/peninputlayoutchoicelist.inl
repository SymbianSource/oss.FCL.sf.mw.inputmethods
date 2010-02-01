/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for choice list controls
*
*/


//
//Set choice list item text display position
//
inline void CFepLayoutChoiceList::SetMarginAndBaseline(TInt16 aMargin, TInt16 aBaseLine)
    {
    iMargin = aMargin;
    iBaseline = aBaseLine;
    }

//
//Set choice list item text display font.
//
inline void CFepLayoutChoiceList::SetFont(const CFont* aFont)
    {
    iFont = aFont;
    }

//
//Set choice list item text display color.
//
inline void CFepLayoutChoiceList::SetFontColor(const TRgb& aCol)
    {
    iFontColor = aCol;
    }

//
//Get bitmap for focus.
//
inline const CFbsBitmap* CFepLayoutChoiceList::FocusBmpMask()
    {
    return iItemFocusBmp;
    }

//
//Get mask bitmap for focus.
//
inline const CFbsBitmap* CFepLayoutChoiceList::FocusBmp()
    {
    return iItemFocusBmpMask;
    }

inline const RPointerArray<CFepLayoutChoiceList::SItem>& CFepLayoutChoiceList::ItemArray()
    {
    return iItemList;
    }

//End Of File
