/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for CFepUiLayout
*
*/

// ---------------------------------------------------------------------------
// get layout root control
// ---------------------------------------------------------------------------
//
#include "peninputlayoutrootctrl.h"
inline CFepUiLayoutRootCtrl* CFepUiLayout::RootControl()
    {
    return iRootCtrl;
    }

// ---------------------------------------------------------------------------
// get layout control list
// ---------------------------------------------------------------------------
//    
inline const RPointerArray<CFepUiBaseCtrl>& CFepUiLayout::ControlList()
    {
    return iRootCtrl->ControlList();
    }

// ---------------------------------------------------------------------------
// get layout owner
// ---------------------------------------------------------------------------
//
inline MLayoutOwner* CFepUiLayout::LayoutOwner()
    {
    return iLayoutOwner;
    }
    
// ---------------------------------------------------------------------------
// Set layout window
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Set display screen size
// ---------------------------------------------------------------------------
//
inline void CFepUiLayout::SetScreenSize(const TSize& aSize)
    {
    iScreenSize = aSize;
    }
    
// ---------------------------------------------------------------------------
// Get screen size
// ---------------------------------------------------------------------------
//
inline const TSize& CFepUiLayout::ScreenSize()
    {
    return iScreenSize;
    }

// ---------------------------------------------------------------------------
// Get layout status
// ---------------------------------------------------------------------------
//
inline TBool CFepUiLayout::LayoutReady()
    {
    return iLayoutReady;
    }
  
// ---------------------------------------------------------------------------
// Get the locked area
// ---------------------------------------------------------------------------
//    
inline const TRect& CFepUiLayout::LockedArea()
    {
    return iLockedArea;
    }
    
// ---------------------------------------------------------------------------
// Get the locked area owner
// ---------------------------------------------------------------------------
//        
const CFepUiBaseCtrl* CFepUiLayout::LockareaOwner() const
    {
    return iLockedAreaOwner;
    }
//end of file    
