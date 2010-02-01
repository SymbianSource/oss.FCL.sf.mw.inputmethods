/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "peninputfullscrlayout.h"


// ---------------------------------------------------------------------------
// Get the region where user can't start the drawing.
// ---------------------------------------------------------------------------
//
const RArray<TRawEvent>& CFepFullScreenLayout::BufferedEvent()
    {
    return iEventBuf;
    }
//end of file    
