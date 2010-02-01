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
* Description:  Implementation for insertion point
*
*/



#include "peninputlayoutinsertionpoint.h"
#include "peninputlayouteditareabase.h"
#include "peninputlayouttimer.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CInsertionPoint::CInsertionPoint
// C++ default constructor can NOT contain any code, that might leave
// ---------------------------------------------------------------------------
//
CInsertionPoint::CInsertionPoint(CFepUiBaseCtrl* aOwner,CFbsBitmapDevice* aBitmapDevice,
                                 CBitmapContext* aGc,TInt aHeight)
    :iPosition(0, 0),
     iHeight(aHeight),
     iIsOn(EFalse),
     iIsVisible(EFalse),
     iEditor(aOwner),
     iBitmapDevice(aBitmapDevice),
     iGc(aGc),
     iCursorTempDisabled(EFalse)
    {
    }
 
// ---------------------------------------------------------------------------
// CInsertionPoint::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CInsertionPoint::ConstructL()
    {
    iCursorBlinkingTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    iRestoreCursorTimer = CLayoutTimer::NewL(this,CLayoutTimer::EOthers);
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CInsertionPoint* CInsertionPoint::NewL(CFepUiBaseCtrl* aOwner,CFbsBitmapDevice* 
                                       aBitmapDevice,CBitmapContext* aGc,TInt aHeight)
    {
    CInsertionPoint* self = new(ELeave) CInsertionPoint(aOwner,aBitmapDevice,aGc,aHeight);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); // insertionPoint
    return self;
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::~CInsertionPoint
// Destructor
// ---------------------------------------------------------------------------
//
CInsertionPoint::~CInsertionPoint()
    {
    if(iCursorBlinkingTimer)
        {        
        iCursorBlinkingTimer->Cancel();
        delete iCursorBlinkingTimer;
        }    
   
    delete iRestoreCursorTimer;
    }


// ---------------------------------------------------------------------------
// CInsertionPoint::SetPosition
// Sets the position of the sprite if it has been turned on
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CInsertionPoint::SetPosition(const TPoint& aPosition)
    {
    
    TBool isOn = iIsOn;
    if (isOn)
        {
        SetOn(EFalse);
        }
    iPosition=aPosition;
    TPoint correction(KCursorPosCorrectionX,KCursorPosCorrectionY );
    iCursorRect = TRect(iPosition + correction , TSize(KCursorWidth,iHeight));

    if (isOn)
        {
        SetOn(ETrue);
        }
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::Position
// get the cursor position
// ---------------------------------------------------------------------------
//
TPoint CInsertionPoint::Position()
    {
    return iPosition;
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::SetLayoutOwner
// Sets the layout owner
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
/*void CInsertionPoint::SetLayoutOwner(MLayoutOwner* aOwner)
    {
    iLayoutOwner = aOwner;
    }
*/

// ---------------------------------------------------------------------------
// CInsertionPoint::SetOn
// Turns the sprite on and off
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CInsertionPoint::SetOn(TBool aOn)
    {
    iIsOn = aOn;    
    
    if(iCursorTempDisabled)
        return;
    // If cursor will bee set off and timer is active, Cancel timer
    if ( !iIsOn )
        {
        if ( iCursorBlinkingTimer->IsActive())
            {
            iCursorBlinkingTimer->Cancel();    
            }
        }
    else
        {
        // Activate blinking timer, if it is not already activated
        if ( !iCursorBlinkingTimer->IsActive() )
            {
            iCursorBlinkingTimer->Start(0, 
                   KCursorBlinkPerioid,TCallBack(CursorBlinkCallBack , this));
            }      
        }
        
        // Invalidate InsertionPoint's rect, so it will be removed from editarea
    InvalidateInsertionPoint();
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::SetHeight
// Sets caret height
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CInsertionPoint::SetHeight(TInt aHeight)
    {
    if(aHeight > 0 && aHeight != iHeight )
        {
        TBool isOn = iIsOn;
        if (isOn)
            {
            SetOn(EFalse);
            }

        iHeight = aHeight;
        //change the cursor rect
        TPoint correction(KCursorPosCorrectionX,KCursorPosCorrectionY );
        iCursorRect = TRect(iPosition + correction , 
                                    TSize(KCursorWidth,iHeight));
        if (isOn)
            {
            SetOn(ETrue);
            }
        }   
       
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::Draw
// Draws insertion point's Rect and starts blinking timer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CInsertionPoint::Draw(CBitmapContext* aGc, TBool aReset)
    {
    if(!iEditor || !iEditor->AbleToDraw())
        return;
    if (aReset)    
        {
        iIsVisible = EFalse;
        }
        
    if(iIsOn)
        {        
        // draw cursor by inverting colors in the selected text rectancle
        static_cast<CFbsBitGc*>(aGc)->Activate( iBitmapDevice );
        aGc->SetClippingRegion(iEditor->ValidClipRegion());        	        
        
        aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
        aGc->SetBrushColor(KRgbBlack);
        aGc->SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);

        aGc->SetPenColor(KRgbBlack);
        aGc->SetPenStyle(CGraphicsContext::ESolidPen);
        aGc->SetPenSize( TSize(1,1));

        aGc->DrawRect(iCursorRect);
        iIsVisible = !iIsVisible;

        // restore normal draw mode
        aGc->SetDrawMode(CGraphicsContext::EDrawModePEN);
        aGc->SetBrushStyle(CGraphicsContext::ENullBrush);
        
        if (iEditor)
            {
            iEditor->UpdateArea(iCursorRect,EFalse);
            }
        
        // Activate blinking timer, if it is not already activated
        if ( !iCursorBlinkingTimer->IsActive() )
    	    {
            iCursorBlinkingTimer->Start(KCursorBlinkPerioid, KCursorBlinkPerioid, 
                                        TCallBack(CursorBlinkCallBack , this));	
    	    }
        } 
    else
        {
        if(iIsVisible) //only do when already shown
            {                
            //CFbsBitmapDevice* bitmapDevice = iEditor->BitmapDevice();  
            static_cast<CFbsBitGc*>(aGc)->Activate(iBitmapDevice);
            aGc->SetClippingRegion(iEditor->ValidClipRegion());        	    
            aGc->SetPenColor(KRgbBlack);
            aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
            aGc->SetBrushColor(KRgbBlack);
            aGc->SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);
            aGc->SetPenStyle(CGraphicsContext::ESolidPen);
            aGc->SetPenSize( TSize(1,1));
            aGc->DrawRect(iCursorRect);

            // restore normal draw mode
            aGc->SetDrawMode(CGraphicsContext::EDrawModePEN);
            aGc->SetBrushStyle(CGraphicsContext::ENullBrush);
            
            iIsVisible = EFalse;                       

            if (iEditor)
                {
                iEditor->UpdateArea(iCursorRect,EFalse);
                }
            }   
        }
    aGc->CancelClippingRegion();        	        
    }
    
// ---------------------------------------------------------------------------
// CInsertionPoint::CursorBlinkCallBack
// Invalidates cursor rect, when timer gets out. 
// With periodic caused draw/undraw of cursor will cause cursor blinking
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CInsertionPoint::CursorBlinkCallBack(TAny *aPtr)
    {
    CInsertionPoint* self = static_cast<CInsertionPoint*>(aPtr); 
           
    self->InvalidateInsertionPoint();
        
    return(ETrue);        
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::InvalidateCursor
// Invalidates InsertionPoint's rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CInsertionPoint::InvalidateInsertionPoint()
    {    
    if(iEditor)
        {        
        Draw(iGc);
        }
    }
  
// ---------------------------------------------------------------------------
// CInsertionPoint::DelayCursorWhileDraging
// Disable the cursor while pen is draging
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CInsertionPoint::DelayCursorWhileDraging()
    {
    if(!iIsOn && !iCursorTempDisabled) // do nothing is cursor is off
        return;
    
    if(!iCursorTempDisabled)
        {
        SetOn(EFalse);  // disable the cursor
        iCursorTempDisabled = ETrue;
        }
    iRestoreCursorTimer->SetTimer(KCursorBlinkPerioid);
    }

// ---------------------------------------------------------------------------
// CInsertionPoint::HandleTimerOut
// Set the cursor on
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CInsertionPoint::HandleTimerOut(TInt /*aTimeType*/)
    {
    iCursorTempDisabled = EFalse;
    iIsOn = ETrue;
    //To avoid the ugly first shown, we draw the cursor directly here.
    InvalidateInsertionPoint();
    SetOn(ETrue);    
    }
//  End of File  
