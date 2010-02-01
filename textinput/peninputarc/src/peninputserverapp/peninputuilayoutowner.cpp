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
* Description:  Implementation for peninput server
*
*/


#include "peninputuilayoutowner.h"	
#include "peninputserver.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::CPeninputUiLayoutOwner
// ---------------------------------------------------------------------------
//
CPeninputUiLayoutOwner::CPeninputUiLayoutOwner ( CPeninputServer& aServer )
    : iServer( aServer )
    {
    }


// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::NewL
// ---------------------------------------------------------------------------
//
CPeninputUiLayoutOwner* CPeninputUiLayoutOwner::NewL( CPeninputServer& aServer )
    {
    CPeninputUiLayoutOwner* self = new (ELeave) CPeninputUiLayoutOwner( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::ConstructL
// ---------------------------------------------------------------------------
//
void CPeninputUiLayoutOwner::ConstructL( )
    {
    }

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::~CPeninputUiLayoutOwner
// Destructor
// ---------------------------------------------------------------------------
//
CPeninputUiLayoutOwner::~CPeninputUiLayoutOwner()
    {
    }


// From MLayoutOwner
// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::UpdateArea
// Update sprite rect
// ---------------------------------------------------------------------------
//
void CPeninputUiLayoutOwner::UpdateArea(const TRect& aRect,TBool aFullUpdate,
                                                    TBool aImmedFlag)
    {
    return iServer.UpdateArea( aRect, aFullUpdate,aImmedFlag);
    }

//from MLayuoutOwner

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::BitmapContext
// Get bitmap context
// ---------------------------------------------------------------------------
//
CBitmapContext* CPeninputUiLayoutOwner::BitmapContext()
    {
    return iServer.BitmapContext();
    }

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::BitmapDevice
// Get bitmap device
// ---------------------------------------------------------------------------
//
CFbsBitmapDevice* CPeninputUiLayoutOwner::BitmapDevice()
    {
    return iServer.BitmapDevice();
    }

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::MaskBmpDevice
// Get mask bitmap device
// ---------------------------------------------------------------------------
//
CFbsBitmapDevice* CPeninputUiLayoutOwner::MaskBmpDevice()
    {
    return iServer.MaskBmpDevice();
    }

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::Hide
// Hide/show sprite
// ---------------------------------------------------------------------------
//
void CPeninputUiLayoutOwner::Hide(TBool aHideFlag)
    {
    return iServer.Hide( aHideFlag );
    }

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::LayoutSizeChanged
// Handle layout size change
// ---------------------------------------------------------------------------
//
TInt CPeninputUiLayoutOwner::LayoutSizeChanged(const TSize& aSize, TBool aNeedUpdate)
	{
	return iServer.LayoutSizeChanged( aSize, aNeedUpdate );	    
	}

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::SignalOwner
// Signal owner
// ---------------------------------------------------------------------------
//
void CPeninputUiLayoutOwner::SignalOwner(TInt aEventType, const TDesC& aEventData)
    {    
    return iServer.SignalOwner( aEventType, aEventData );	 
    }
    
// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::LayoutRect
// Get sprite size and position
// ---------------------------------------------------------------------------
//
TRect CPeninputUiLayoutOwner::LayoutRect()
    {
    return iServer.LayoutRect();
    }

// ---------------------------------------------------------------------------
// CPeninputUiLayoutOwner::SetPosition
// called when sprite has been moved
// ---------------------------------------------------------------------------
//

TPoint CPeninputUiLayoutOwner::SetPosition(const TPoint& aNewPos)
    {
    return iServer.SetPosition( aNewPos );
    }

void CPeninputUiLayoutOwner::FlushUi()
    {
    return iServer.FlushWsSession();
    }
    
// End of File

