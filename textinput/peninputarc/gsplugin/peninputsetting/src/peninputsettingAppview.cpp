/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  implement of class CpeninputsettingAppView
*
*/
#include <coemain.h>
#include <peninputsetting.rsg>

#include "peninputsettingAppView.h"

// Standard construction sequence
CpeninputsettingAppView* CpeninputsettingAppView::NewL(const TRect& aRect)
    {
    CpeninputsettingAppView* self = CpeninputsettingAppView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
    }

CpeninputsettingAppView* CpeninputsettingAppView::NewLC(const TRect& aRect)
    {
    CpeninputsettingAppView* self = new (ELeave) CpeninputsettingAppView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

CpeninputsettingAppView::CpeninputsettingAppView()
    {
	// no implementation required
    }

CpeninputsettingAppView::~CpeninputsettingAppView()
    {
	// no implementation required
    }

void CpeninputsettingAppView::ConstructL(const TRect& aRect)
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// Draw this application's view to the screen
void CpeninputsettingAppView::Draw(const TRect& /*aRect*/) const
    {
    // Get the standard graphics context 
    CWindowGc& gc = SystemGc();
    
    // Gets the control's extent
    TRect rect = Rect();
    
    // Clears the screen
    gc.Clear(rect);
    }


