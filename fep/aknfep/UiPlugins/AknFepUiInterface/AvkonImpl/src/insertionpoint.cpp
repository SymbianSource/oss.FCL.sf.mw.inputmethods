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


#include "insertionpoint.h"
#include "aknfepchineseuidataconv.h"

//Cursor position offset
const TInt KCursorPosCorrectionX = 0;
//define the timer
const TInt KTimer = 500000;

// ---------------------------------------------------------
// Cancel and destroy.
// ---------------------------------------------------------
CInsertionPoint::~CInsertionPoint()
    {
    Cancel(); // Cancel any request, if outstanding
    iTimer.Close(); // Destroy the RTimer object
    // Delete instance variables if any
    }

// ---------------------------------------------------------
// NewL function.
// ---------------------------------------------------------
CInsertionPoint* CInsertionPoint::NewL( CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CInsertionPoint* self = CInsertionPoint::NewLC( aLafDataMgr );
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------
// NewLC funciton.
// ---------------------------------------------------------
CInsertionPoint* CInsertionPoint::NewLC( CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CInsertionPoint* self = new ( ELeave ) CInsertionPoint( aLafDataMgr );
    CleanupStack::PushL( self ) ;
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// Set iRefreshObserver member.
// ---------------------------------------------------------
void CInsertionPoint::SetRefreshObserver(MRefreshObserver* aObserver)
    {
    iRefreshObserver = aObserver;
    }

// ---------------------------------------------------------
// Set the position of cursor.
// ---------------------------------------------------------
void CInsertionPoint::SetPosition(TInt aOffSet)
    {
    iOffSet = aOffSet;
    isVisible = ETrue;
    iRefreshObserver->Refresh();
    TPoint point(iTextRect.iTl.iX -1 + iOffSet + KCursorPosCorrectionX,
            iCursorRect.iTl.iY);
    iCursorRect = TRect(point, TSize(iWidth, iHeight) ) ;
    }

// ---------------------------------------------------------
// Set the rect where visual text will be displayed.
// ---------------------------------------------------------
void CInsertionPoint::SetTextRect(TRect aRect)
    {
    iTextRect = aRect;
    TPoint point(iTextRect.iTl.iX + iOffSet + KCursorPosCorrectionX,
            iCursorRect.iTl.iY);
    iCursorRect = TRect(point, TSize(iWidth, iHeight) ) ;
    if ( 0 == iHeight )
        {
        iHeight = 
            CAknFepChineseUIDataConv::AnyToInt( iLafDataMgr->RequestData(EEEPCursorHeight));
        iWidth = 1;
        }
    }

// ---------------------------------------------------------
// Set the height of cursor.
// ---------------------------------------------------------
void CInsertionPoint::SetHeight()
    {
    TPoint point(iTextRect.iTl.iX + iOffSet + KCursorPosCorrectionX,
            iTextRect.iTl.iY - 1 + iTextRect.Height() / 2 - iHeight / 2);
    iCursorRect = TRect(point, TSize(iWidth, iHeight) ) ;
    }

// ---------------------------------------------------------
// Draw the cursor on the screen.
// ---------------------------------------------------------
void CInsertionPoint::Draw(CWindowGc& aGc)
    {
    if (isActive)
        {
        if ( !this->isVisible)
            {
            return;
            }
        else
            {
            aGc.SetBrushStyle(CGraphicsContext::ESolidBrush) ;
            aGc.SetBrushColor(iActiveBrushColor) ;
            aGc.SetPenColor(iActivePenColor) ;
            aGc.SetPenStyle(CGraphicsContext::ESolidPen) ;
            aGc.SetPenSize(TSize( 1, 1) ) ;
            aGc.DrawRect(iCursorRect) ;
            aGc.SetBrushStyle(CGraphicsContext::ENullBrush) ;
            }
        }
    else
        {
        aGc.SetBrushStyle(CGraphicsContext::ESolidBrush) ;
        aGc.SetBrushColor(iInactiveBrushColor) ;
        aGc.SetPenColor(iInactivePenColor) ;
        aGc.SetPenStyle(CGraphicsContext::ESolidPen) ;
        aGc.SetPenSize(TSize( 1, 1) ) ;
        aGc.DrawRect(iCursorRect) ;
        aGc.SetBrushStyle(CGraphicsContext::ENullBrush) ;
        }
    }

// ---------------------------------------------------------
// Active the cursor into active state.
// ---------------------------------------------------------
void CInsertionPoint::Active()
    {
    Cancel();
    iTimer.Cancel();
    iTimer.After(iStatus, KTimer); // Set for 1 sec later
    SetActive(); // Tell scheduler a request is active
    isActive = ETrue;
    this->iRefreshObserver->Refresh();
    }

// ---------------------------------------------------------
// Deactive the cursor into deactive state.
// ---------------------------------------------------------
void CInsertionPoint::Deactive()
    {
    Cancel();
    isActive = EFalse;
    this->iRefreshObserver->Refresh();
    }

// ---------------------------------------------------------
// Insertion Point.
// ---------------------------------------------------------
CInsertionPoint::CInsertionPoint( CAknFepUiLayoutDataMgr* aLafDataMgr ) :
    CActive(EPriorityStandard)
    {
    iState = EUninitialized;
    isVisible = ETrue;
    iHeight = 0;
    iWidth = 0;
    iOffSet = 0;
    isActive = EFalse;
    iRefreshObserver = NULL;
    iLafDataMgr = aLafDataMgr;
    }

// ---------------------------------------------------------
// Two-phrase constructor.
// ---------------------------------------------------------
void CInsertionPoint::ConstructL()
    {
    User::LeaveIfError(iTimer.CreateLocal() ) ; // Initialize timer
    CActiveScheduler::Add( this); // Add to scheduler
    iActiveBrushColor = KRgbBlack;
    iActivePenColor = KRgbBlack;
    iInactiveBrushColor = KRgbGray;
    iInactivePenColor = KRgbGray;
    }

// ---------------------------------------------------------
// The callback function used by CActiveScheduler.
// ---------------------------------------------------------
void CInsertionPoint::RunL()
    {
    if (iState == EUninitialized)
        {
        // Do something the first time RunL() is called
        iState = EInitialized;
        }
    else
        if (iState != EError)
            {
            // Do something
            isVisible = isVisible ? EFalse : ETrue;
            if (iRefreshObserver)
                {
                this->iRefreshObserver->Refresh();
                }
            }
    
    iTimer.Cancel();
    iTimer.After( iStatus, KTimer ) ; // Set for 1 sec later
    SetActive(); // Tell scheduler a request is active
    }

// ---------------------------------------------------------
// How to cancel me.
// ---------------------------------------------------------
void CInsertionPoint::DoCancel()
    {
    iTimer.Cancel();
    }

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
TInt CInsertionPoint::RunError(TInt aError)
    {
    return aError;
    }

// ---------------------------------------------------------
// Start the cursor with glint if actived or with gray color if deactived.
// ---------------------------------------------------------
void CInsertionPoint::StartL( TTimeIntervalMicroSeconds32 aDelay )
    {
    Cancel(); // Cancel any request, just to be sure
    iState = EUninitialized;
    iTimer.Cancel();
    iTimer.After(iStatus, aDelay) ; // Set for later
    SetActive(); // Tell scheduler a request is active
    }

// End of file
