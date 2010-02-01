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


// ---------------------------------------------------------------------------
// Get the control which captures the pointer
// ---------------------------------------------------------------------------
//
inline CFepUiBaseCtrl* CFepUiLayoutRootCtrl::CtrlCapPointer()
    {
    return iPointerCaptureCtrl;
    }

// ---------------------------------------------------------------------------
// Get the region where user can't start the drawing.
// ---------------------------------------------------------------------------
//
const TRegion& CFepUiLayoutRootCtrl::NonHwrStartingPtRegion()
    {
    return iNonHwrStartPtRegion;
    }

//end of file    
