/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of mutli-page virtual keyboard control.
*
*/


// INCLUDE
#include <e32std.h>

#include "peninputlayoutbubblectrl.h"
#include "peninputlayoutvkb.h"
#include "peninputpluginutils.h"
#include "peninputfingerhwrarmultipagevkb.h"

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CMultiPageVirtualKeyboard* CMultiPageVirtualKeyboard::NewL(
    const TRect& aRect, CFepUiLayout* aUiLayout,TInt aControlId,
    const TFontSpec& aFontSpec, TBool aIrregular )
    {
    CMultiPageVirtualKeyboard* vkb = new(ELeave) CMultiPageVirtualKeyboard(
        aRect, aUiLayout, aControlId, aFontSpec, aIrregular );
    
    CleanupStack::PushL( vkb );
    vkb->ConstructL();
    vkb->SetRect( aRect );
    CleanupStack::Pop( vkb );
    return vkb;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CMultiPageVirtualKeyboard* CMultiPageVirtualKeyboard::NewL(
    const TRect& aRect, CFepUiLayout* aUiLayout,TInt aControlId,
    CFont* aFont, TBool aIrregular )
    {
    TFontSpec fontSpec;
    fontSpec.iHeight = 0;
    CMultiPageVirtualKeyboard* vkb = new(ELeave) CMultiPageVirtualKeyboard(
            aRect, aUiLayout, aControlId, fontSpec, aIrregular );
    
    vkb->SetFont( aFont );                                                        
    CleanupStack::PushL( vkb );
    vkb->ConstructL();
    vkb->SetRect( aRect );
    CleanupStack::Pop( vkb );
    return vkb;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CMultiPageVirtualKeyboard::~CMultiPageVirtualKeyboard()
    {
    }

// ---------------------------------------------------------------------------
// get current page index.
// ---------------------------------------------------------------------------
//
TInt CMultiPageVirtualKeyboard::CurPageIndex()
    {
    return iCurPageIndex;
    }

// ---------------------------------------------------------------------------
// get current page count.
// ---------------------------------------------------------------------------
//
TInt CMultiPageVirtualKeyboard::PageCount()
    {
    return iPageCount;
    }

// ---------------------------------------------------------------------------
// get current page size.
// ---------------------------------------------------------------------------
//
TInt CMultiPageVirtualKeyboard::PageSize()
    {
    return iPageSize;
    }

// ---------------------------------------------------------------------------
// move to next page.
// ---------------------------------------------------------------------------
//
void CMultiPageVirtualKeyboard::NavToNextPage()
    {
    if ( iCurPageIndex < iPageCount - 1 )
        {
        iCurPageIndex++;
        NavToPage( iCurPageIndex );
        }
    }

// ---------------------------------------------------------------------------
// move to previous page.
// ---------------------------------------------------------------------------
//
void CMultiPageVirtualKeyboard::NavToPrevPage()
    {
    if ( iCurPageIndex > 0 )
        {
        iCurPageIndex--;
        NavToPage( iCurPageIndex );
        }    
    }

// ---------------------------------------------------------------------------
// move to first page.
// ---------------------------------------------------------------------------
//
void CMultiPageVirtualKeyboard::NavToFirstPage()
    {
    if ( iPageCount > 0 )
        {
        NavToPage( 0 );
        }
    }

// ---------------------------------------------------------------------------
// move to last page.
// ---------------------------------------------------------------------------
//
void CMultiPageVirtualKeyboard::NavToLastPage()
    {
    if ( iPageCount > 0 )
        {
        NavToPage( iPageCount - 1 );
        }
    }

// ---------------------------------------------------------------------------
// move to specified page.
// ---------------------------------------------------------------------------
//
void CMultiPageVirtualKeyboard::NavToPage( TInt aPageIndex )
    {
    if ( aPageIndex >= iPageCount || aPageIndex <0 )
        {
        _LIT(KErrorPageIndex,"invalid index");
        User::Panic( KErrorPageIndex, 0 );
        return;
        }
    
    iCurPageIndex = aPageIndex;
    
    const RPointerArray<CVirtualKey>& keys = KeyArray();
    TInt count = keys.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        TInt page = i / iPageSize;
        CVirtualKey* vk = keys[i];

        vk->HideKeyCtrl( page != aPageIndex );
        }
    
    }

// ---------------------------------------------------------------------------
// update paging.
// ---------------------------------------------------------------------------
//
void CMultiPageVirtualKeyboard::UpdatePaging( TInt aRows, TInt aCols )
    {
    TInt count = KeyArray().Count();
    
    iPageSize = aRows * aCols;
    iPageCount = ( count + iPageSize - 1) / iPageSize;
    iCurPageIndex = 0;
    NavToPage( iCurPageIndex );
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CMultiPageVirtualKeyboard::CMultiPageVirtualKeyboard(const TRect& aRect,
    CFepUiLayout* aUiLayout, TInt aControlId, const TFontSpec& aFontSpec, 
    TBool aIrregular )
    : CVirtualKeyboard( aRect, aUiLayout, aControlId, aFontSpec, aIrregular )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CMultiPageVirtualKeyboard::ConstructL()
    {
    CVirtualKeyboard::ConstructL();
    }
//  End Of File
