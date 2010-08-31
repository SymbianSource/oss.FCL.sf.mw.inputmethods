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
* Description:  Implementation for hwr window base and transparent window
*
*/



#include <coemain.h>
#include "peninputlayouthwrwnd.h"
#include "peninputlayoutrootctrl.h"
#include "peninputlayouttimer.h"
#include "peninputlayout.h"

#include <settingsinternalcrkeys.h>

#include "peninputpentracedecorator.h"

const TUint8 KDefaultTransparency = 128;
const TInt KDefaultPenSize = 3;
const TInt KFadeAreaCount = 30;

/******************** start of CHwrWndBase **********************************/
// Implementation of Class CHwrWndBase
//

// ---------------------------------------------------------------------------
// CHwrWndBase::CHwrWndBase
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CHwrWndBase::CHwrWndBase(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aControlId):
                    CFepUiBaseCtrl(aRect,aUiLayout,aControlId), 
                    iHasNewTrace(EFalse),
                    iSubmitted(ETrue)
    {
    const TTimeIntervalMicroSeconds32 KDefaultCharDelay = 1000*500; //1/2 seconds    
    const TTimeIntervalMicroSeconds32 KDefaultStrokeDelay = 1000*300; //1/10 seconds
    
    
    SetControlType(ECtrlHwrWnd);
    SetPenColor(KRgbBlack);
    SetPenSize(TSize(KDefaultPenSize,KDefaultPenSize));
    iStrokeEndMark=TPoint(0xffff,0);
    iStrokeDelay = KDefaultStrokeDelay;    
    iCharDelay = KDefaultCharDelay;    
    iDirtyRect.SetRect(0,0,0,0);
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::~CHwrWndBase
// Destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CHwrWndBase::~CHwrWndBase()
    {

    iListOfPoints.Close();
    delete iCharacterTimer;
    iCharacterTimer = NULL;
    delete iStrokeTimer;
    iStrokeTimer = NULL;     
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::ClearStroke
// Clear the stroke
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C void CHwrWndBase::ClearStroke()
    {
    iDirtyRect.Intersection(iRect);
    
    ClearRect( iRect );       

    iListOfPoints.Reset();            
    
    iHasNewTrace=EFalse;
    iSubmitted = ETrue;
    
    if(AbleToDraw())
        {
        RootControl()->DrawRect(iRect);
        UpdateArea(iRect, EFalse);
        }

    iDirtyRect.SetRect(0,0,0,0);        
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::AddGuidingLine
// Add a guilding line.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrWndBase::AddGuidingLine(const TPoint& /*aStart*/, const TPoint& /*aEnd*/)
    {
    //not implemented
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::EndStrokeL
// Do recognition after a stroke is ended.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
void CHwrWndBase::EndStrokeL(TBool aCharacterTimerFlag)
    {
    if(iHasNewTrace)    //only do this when we have new trace
        {               
        if(aCharacterTimerFlag)
            {
            iCharacterTimer->SetTimer(iCharDelay);  //set character timer            
            //only send finish event when character timer starter, 
            //otherwise, send charater timer out event.
            ReportEvent(EEventHwrStrokeFinished); 
            iSubmitted = EFalse;
            }            
        else
            {
            ReportEvent(EEventHwrCharacterTimerOut);
            iSubmitted = ETrue;
            }
        
        iHasNewTrace=EFalse; //Current stroke is ended,reset the flag.
        
        }
    else
        {
        //no new trace, no need to update the candidate content, just show it.
        if(!aCharacterTimerFlag)
            {
            ReportEvent(EEventHwrCharacterTimerOut);
            iSubmitted = ETrue;
            }
        }
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::CancelCharWriting
// Cancel current drawing. Clears the stroke and stops the timer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CHwrWndBase::CancelCharWriting()
    {
    SetPointerDown(EFalse);
    
    //cancel timer
    iCharacterTimer->Cancel();
    iStrokeTimer->Cancel();            
    
    //reset the stroke
    ClearStroke();
    ReportEvent(EEventHwrStrokeCanceled);
    }
// ---------------------------------------------------------------------------
// CHwrWndBase::Draw
// Draw the window and stroke
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CHwrWndBase::Draw()
    {
	if(!AbleToDraw())
        return;
    
    //draw back ground
    DrawBackground();
    DrawStroke(ETrue);
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::DrawLine
// Draw a line
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
TRect CHwrWndBase::DrawLine(const TPoint& aPt1,const TPoint& aPt2,TBool /*aDrawFlag*/)
    {
    //do nothing
    return TRect(aPt1,aPt2);
    }
    
// ---------------------------------------------------------------------------
// CHwrWndBase::HandlePointerDownEventL
// handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CHwrWndBase::HandlePointerDownEventL( const TPoint& aPt)
    {
    CFepUiBaseCtrl::HandlePointerDownEventL(aPt);
        
    iPtPrev = aPt;

    iListOfPoints.AppendL(aPt- Rect().iTl);
    iStrokeTimer->Cancel();
    iCharacterTimer->Cancel();

    iHasNewTrace=ETrue;
    iSubmitted = EFalse;
    
    TRect rect;    
    rect = DrawPoint(aPt,ETrue);
    if(iDirtyRect.IsEmpty())
        iDirtyRect = rect;
    else
        {
        iDirtyRect.BoundingRect(rect);
        }

    UpdateArea(rect,EFalse);         
   
    ReportEvent(EEventHwrStrokeStarted);
    return this; 
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::HanldePointerUpEventL
// handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CHwrWndBase::HandlePointerUpEventL( const TPoint& aPoint)
    {
    iListOfPoints.AppendL(iStrokeEndMark);
    iStrokeTimer->SetTimer(iStrokeDelay);  
    CFepUiBaseCtrl::HandlePointerUpEventL(aPoint); 
    // For EKZG-7JCBGW
    // iCtrlWithPointerDown of its CControlGroup must be set to NULL
    // when its up event occurs.
    CFepUiBaseCtrl* parent = ParentCtrl();
    while ( parent )
        {
        parent->CancelPointerDownL();
        parent = parent->ParentCtrl();
        }    
    return this;  
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::HandleButtonMoveEventL
// Handle button move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CHwrWndBase::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    if(!PointerDown())
       return this;   
    
    iListOfPoints.AppendL(aPoint - Rect().iTl);     
    
    TRect rect=DrawLine(iPtPrev,aPoint,ETrue);
    
    /*if(!rect.IsEmpty())
        {
        TRect& drawrect = DirtyRect();
        drawrect.BoundingRect(rect); //find the bounding rect and set the rect
        }    */
    UpdateArea(rect,EFalse);         
    iPtPrev=aPoint;    
    return this;
    }


// ---------------------------------------------------------------------------
// CHwrWndBase::CancelPointerDownL
// Cancel the pointer down event in HWR window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CHwrWndBase::CancelPointerDownL()
    {
    if( !PointerDown() )
        {
        return;
        }
	//allow pen up in other control. Do us normal pen up
	if( iListOfPoints.Count() > 0 )
        {
        iListOfPoints.AppendL(iStrokeEndMark);
        iStrokeTimer->SetTimer(iStrokeDelay);   
        }

	SetPointerDown(EFalse);
    CFepUiBaseCtrl::CancelPointerDownL();
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::UpdateRegion
// Updates the area where other control made dirty
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
void CHwrWndBase::UpdateRegion(CFepUiBaseCtrl* aCtrl)
    {
    if(aCtrl)
        {
        TRect rect;
        if(iDirtyRect.Intersects(aCtrl->Rect()))
            {
            DrawStroke(ETrue);
            rect.Intersection(iDirtyRect);
            UpdateArea(rect,EFalse);
            } 
        }    
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::HandleTimerOut
// Handle timer out event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CHwrWndBase::HandleTimerOut(TInt aTimeType)
    {
    switch(aTimeType)
        {
        case CLayoutTimer::ECharacterTimer:
            {
            TRAP_IGNORE(EndStrokeAndClearL());
            }
            break;
        case CLayoutTimer::EStrokeTimer:
            {
            TRAP_IGNORE(EndStrokeL(ETrue));
            }
            break;
        default:
            //Do nothing
            break;
        }
    }

// ---------------------------------------------------------------------------
// CHwrWndBase::ConstructL
// Symbian second phrase constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CHwrWndBase::ConstructL()
    {    
    BaseConstructL();
    iCharacterTimer = CLayoutTimer::NewL(this,CLayoutTimer::ECharacterTimer);
    iStrokeTimer = CLayoutTimer::NewL(this);    
    }


// ---------------------------------------------------------------------------
// CHwrWndBase::DrawStroke
// Draw stroke
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TRect CHwrWndBase::DrawStroke(TBool /*aDrawFlag*/)
    {
    //not implemented
    return TRect();
    }


// ---------------------------------------------------------------------------
// CHwrWndBase::EndStrokeAndClearL
// Called when character timer out. Not implemented, it's for derivated class.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
void CHwrWndBase::EndStrokeAndClearL()
    {
    EndStrokeL(EFalse);
    ClearStroke();
    } 

// ---------------------------------------------------------------------------
// CHwrWndBase::OnDeActivate
// Response to layout de activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CHwrWndBase::OnDeActivate()
    {
    CFepUiBaseCtrl::OnDeActivate();
    iStrokeTimer->Cancel();
    iCharacterTimer->Cancel();
    }
   
// ---------------------------------------------------------------------------
// CHwrWndBase::OnLayoutDraggingStart
// Response to layout dragging start event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CHwrWndBase::OnLayoutDraggingStart()
    {
    if ( iHasNewTrace )
        {
        ReportEvent(EEventHwrStrokeFinished);
        iHasNewTrace=EFalse;
        }
    iStrokeTimer->Cancel();
    
    if ( !iSubmitted )
        {
        ReportEvent(EEventHwrCharacterTimerOut);
        iSubmitted = ETrue;
        }
    iCharacterTimer->Cancel();
    iListOfPoints.Reset();                
    }
    
// ---------------------------------------------------------------------------
// CHwrWndBase::OnLayoutDraggingEnd
// Response to layout dragging end event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CHwrWndBase::OnLayoutDraggingEnd()
    {
    }    
   
/******************** end of CHwrWndBase **********************************/




//class CTransparentHwrWnd
// ---------------------------------------------------------------------------
// CTransparentHwrWnd::CTransparentHwrWnd
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CTransparentHwrWnd::CTransparentHwrWnd(const TRect& aRect,
                             CFepUiLayout* aUiLayout,TInt aCtrlId,
                             TBool aFullScreenFlag)
                            :CHwrWndBase(aRect,aUiLayout,aCtrlId),
                             iIsFullScreen(aFullScreenFlag),
                             iTraceOutsideWindowFlag(EFalse),
                             iTransparencyFactor(KDefaultTransparency)
    {
    SetControlType(ECtrlTransparentHwrWnd);

    const TRgb KDefaultMaskCol(200,200,200);

    if(iIsFullScreen)
        {
        SetMaskBkCol( TRgb(KTransparentColor)); //full transparent
        }
    else
        SetMaskBkCol(KDefaultMaskCol);
    
    SetBorderColor(TRgb(KOpaqueColor));
    //transparent window allow dragging on top of it
    SetAllowOverlap(ETrue);
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::~CTransparentHwrWnd
// Destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CTransparentHwrWnd::~CTransparentHwrWnd()
    {
    if(iTraceOutsideWindowFlag)
        {
        TRAP_IGNORE(RequireRegionUpdateL(EFalse));
        }

    if ( iPenTraceDecorator )
        {
        delete iPenTraceDecorator;
        iPenTraceDecorator = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::NewL
// Factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CTransparentHwrWnd* CTransparentHwrWnd::NewL(const TRect& aRect,
                                    CFepUiLayout* aUiLayout,TInt aCtrlId,
                                    TBool aFullScreenFlag)
    {
    CTransparentHwrWnd* wnd=new(ELeave) CTransparentHwrWnd(aRect,aUiLayout,
                                                        aCtrlId,aFullScreenFlag);
    CleanupStack::PushL(wnd);
    wnd->ConstructL ();
    CleanupStack::Pop(wnd);
    return wnd;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::SetWndTransparencyFactor
// Set the transparency factor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void EXPORT_C CTransparentHwrWnd::SetWndTransparencyFactor(TUint8 aFactor)
    {
    if(iTransparencyFactor != aFactor)
        {
        //change mask col        
        TUint8 col = aFactor;
        
        SetMaskBkCol(TRgb( 255- col, 255 - col, 255 - col));     
        
        iTransparencyFactor = aFactor;
        Draw();
        UpdateArea(Rect(),ETrue);
        UiLayout()->LayoutOwner()->FlushUi();
        }
    }


// -----------------------------------------------------------------------------
// CTransparentHwrWnd::EnableTraceOutsideWindow
// Enable/disable trace outside of hwr window.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
EXPORT_C void CTransparentHwrWnd::EnableTraceOutsideWindow(TBool aFlag)
    {
    iTraceOutsideWindowFlag = aFlag;
    TRAP_IGNORE(RequireRegionUpdateL(aFlag));
    }


// ---------------------------------------------------------------------------
// CTransparentHwrWnd::Draw
// Draw hwr window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CTransparentHwrWnd::Draw()
    {
	if(!AbleToDraw())
        return;
    
    if(iIsFullScreen)
        {
        DrawStroke(ETrue);
        return;
        }       

    CFbsBitGc* gc= static_cast<CFbsBitGc*> (BitGc());
    
    DrawMaskBackground();
    // ----- draw bitmaps -----
    DrawBackground();
    DrawFrame( Rect() );
    //draw stroke
    gc->SetPenSize( PenSize() ); 
    gc->SetPenColor( PenColor() );
    
    if ( PenTraceDecorationActivated() )
        {
        //clear decoration buffer
        if ( StrokeList().Count() > 0 )
            {
            iPenTraceDecorator->Clear();
            }
        }

    DrawStroke(ETrue);
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::DrawStroke
// Draw stroke
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TRect CTransparentHwrWnd::DrawStroke(TBool aDrawFlag)
    {
    TRect DirtyRect;
    TRect rect;
    TPoint pt;
    TPoint prevPt;
    
    //rect = DrawPoint(pt,aDrawFlag);
    //DirtyRect = rect; 
    TBool bBeginStroke = ETrue;
    for(TInt i = 0; i < StrokeList().Count(); ++i)
        {
        pt = StrokeList()[i] ;
        //skip the stroke end mark, plot the start point for the stroke
        if(pt == StrokeEndMark())
            {
            bBeginStroke = ETrue;
            continue;            
            }

        pt += Rect().iTl;        
        if(bBeginStroke)    
            {
            //plot the start point for the new line
            rect = DrawPoint(pt,aDrawFlag);
            bBeginStroke = EFalse;            
            }
        else
            {
            rect = DrawLine(prevPt,pt,aDrawFlag);
            }
        DirtyRect.BoundingRect(rect); //find the bounding rect
        prevPt = pt;
        }
    
    return DirtyRect;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::DrawPoint
// Draw a point
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
TRect CTransparentHwrWnd::DrawPoint(const TPoint& aPoint,TBool aDrawFlag)
    {
    TRect affectedRect(0,0,0,0);

    if ( PenTraceDecorationActivated( ) )
        {
        TRect wndRect = Rect();
        
        TPoint pt1 = aPoint - wndRect.iTl; //decorator's coordinate system
        TRect changedRect = iPenTraceDecorator->DrawPoint( pt1 ); //decorator's coordinate system
        TPoint drawpos = changedRect.iTl + wndRect.iTl;

        CFbsBitGc* gc = static_cast<CFbsBitGc*>( BitGc() );

        gc->Activate( BitmapDevice() );
        gc->SetBrushColor( BkColor() );
        gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        
        gc->BitBltMasked( drawpos, iPenTraceDecorator->BitsBitmap(), 
                changedRect, iPenTraceDecorator->MaskBitmap(), EFalse );
        
        gc->RectDrawnTo( affectedRect ); //get the affected rect since last draw

        //gc->Activate( MaskBitmapDevice() );
        //gc->BitBlt( drawpos, iPenTraceDecorator->MaskBitmap(), changedRect );
        }
    else
        {
        CFbsBitGc* gc= static_cast<CFbsBitGc*> (BitGc());
    
        gc->Activate( BitmapDevice() );
        gc->SetPenSize( PenSize());
        if(aDrawFlag)
            gc->SetPenColor( PenColor() );
        else    //erase the point            
            gc->SetPenColor( BkColor());
        gc->Plot( aPoint );
        gc->RectDrawnTo(affectedRect); //get the affected rect since last draw

        TRect maskRect;

        gc->Activate( MaskBitmapDevice());
        gc->SetPenColor( KOpaqueColor );
        //use same color to get opague point
        gc->Plot( aPoint );
        gc->RectDrawnTo(maskRect);

        affectedRect.BoundingRect(maskRect); //find the bounding rect
        if(!iTraceOutsideWindowFlag)
            affectedRect.Intersection(iRect); //drawing not allowed outside of 
        
        }
    return affectedRect;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::DrawLine
// Draw a line
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
TRect CTransparentHwrWnd::DrawLine(const TPoint& aPt1,const TPoint& aPt2,TBool aDrawFlag)
    {
    TRect affectedRect(0,0,0,0);

    if ( PenTraceDecorationActivated() )
        {
        TRect wndRect = Rect();
        
        TPoint pt1 = aPt1 - wndRect.iTl; //decorator's coordinate system
        TPoint pt2 = aPt2 - wndRect.iTl; //decorator's coordinate system
        
        TRect changedRect = iPenTraceDecorator->DrawNextLine( pt1, pt2 ); //decorator's coordinate system
        
        TPoint drawpos = changedRect.iTl + wndRect.iTl;

        CFbsBitGc* gc= static_cast<CFbsBitGc*>( BitGc() );

        gc->Activate( BitmapDevice() );
        gc->SetBrushColor( BkColor() );
        gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        
        gc->BitBltMasked( drawpos, iPenTraceDecorator->BitsBitmap(), 
                changedRect, iPenTraceDecorator->MaskBitmap(), EFalse );
        
        gc->RectDrawnTo( affectedRect ); //get the affected rect since last draw

        //gc->Activate( MaskBitmapDevice() );
        //gc->BitBlt( drawpos, iPenTraceDecorator->MaskBitmap(), changedRect );        
        }
    
    else
        {
        CFbsBitGc* gc= static_cast<CFbsBitGc*> (BitGc());
    
        gc->Activate( BitmapDevice() );
        SetClipRegion(); 
        gc->SetPenSize( PenSize());
        if(aDrawFlag)
            gc->SetPenColor( PenColor() );
        else            
            gc->SetPenColor( BkColor());
        gc->SetPenStyle(CGraphicsContext::ESolidPen);       
        gc->DrawLine( aPt1,aPt2 );

        gc->RectDrawnTo(affectedRect); //get the affected rect since last draw
        CancelClipRegion();
         
        TRect maskRect;

        gc->Activate( MaskBitmapDevice() );
        SetClipRegion();        
        gc->SetPenColor( KOpaqueColor );
       
        //use same color as in bitmap to get opague line
        //gc->SetPenStyle(CGraphicsContext::ESolidPen);        
        gc->DrawLine( aPt1,aPt2 );
        gc->RectDrawnTo(maskRect);

        //affectedRect.BoundingRect(maskRect); //find the bounding rect
/*        if(!iTraceOutsideWindowFlag)
            {
            TRect r = affectedRect;
            affectedRect.Intersection(iRect); //drawing not allowed outside of 
            
            }*/
        CancelClipRegion();                
        }
    return affectedRect;
    } 

void CTransparentHwrWnd::DrawFrame( const TRect& aFrameRect )
	{
    CFbsBitGc* gc= static_cast<CFbsBitGc*> ( BitGc() );        
	
    gc->Activate( MaskBitmapDevice() );    
    gc->SetPenColor( KOpaqueColor );
    gc->SetPenSize( BorderSize() );
    gc->SetPenStyle( CGraphicsContext::ESolidPen ); 
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    gc->DrawRect( aFrameRect );
	
    gc->Activate( BitmapDevice() );
    gc->SetPenColor( BorderColor() );
    gc->SetPenSize( BorderSize() );
    gc->SetPenStyle(CGraphicsContext::ESolidPen); 
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    gc->DrawRect( aFrameRect );	    		
	}

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::HandlePointerDownL
// handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CTransparentHwrWnd::HandlePointerDownEventL(const TPoint& aPoint)
    {
    if ( PenTraceDecorationActivated() )
        {
        iPenTraceDecorator->SetPenTraceColor( PenColor() );
        iPenTraceDecorator->SetDisplayModeL( BitmapDevice()->DisplayMode(), EFalse );
        iPenTraceDecorator->ResizeL( Rect().Size() );
        }
    
    // handle drawing in first box
	//capture pointer
    CHwrWndBase::HandlePointerDownEventL(aPoint);
    CapturePointer();    
    return this;
    }


// ---------------------------------------------------------------------------
// CTransparentHwrWnd::HandlePointerUpL
// Handle point up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CTransparentHwrWnd::HandlePointerUpEventL(
                                                        const TPoint& aPoint)
    {
	//release capture
    CHwrWndBase::HandlePointerUpEventL(aPoint);
    CapturePointer(EFalse);    
    return this;
    }
// ---------------------------------------------------------------------------
// CTransparentHwrWnd::HandlePointerMoveEventL
// Handle point move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CTransparentHwrWnd::HandlePointerMoveEventL(
                                                         const TPoint& aPoint)
    {   
    if(!PointerDown())
       return this;
    
	//ignore events outside of window if it's TraceOutSideWindow flag is not set
	TRect r;
	TSize s(PenSize());
	TSize s2(3*s.iWidth/2,3*s.iHeight/2);
	
	r.SetRect(TPoint(aPoint-TSize(s.iWidth/2,s.iHeight/2)),s2);
	TRect r1 = r;
	r.Intersection(Rect());
	TBool inside = (r == r1);

    //if(iTraceOutsideWindowFlag || inside)//Rect().Contains(aPoint))
    if(iTraceOutsideWindowFlag || Rect().Contains(aPoint))
        {
        return CHwrWndBase::HandlePointerMoveEventL(aPoint);
        }
            
    return this;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::UpdateValidRegion
// Update the valid region when one control hiden or shown
// ---------------------------------------------------------------------------
//
EXPORT_C void CTransparentHwrWnd::UpdateValidRegion(CFepUiBaseCtrl* aCtrl,
                                                    TBool aRemovedFlag)
    {
    RRegion region(8);
    region.Copy(Region());   
    if(iTraceOutsideWindowFlag) //Ignore if trace can not be outside of hwr window
        {
        if(aCtrl)
            {                        
            if(aCtrl->Hiden() || aRemovedFlag)
                {
                region.Union(aCtrl->Region());
                }
            else
                region.SubRegion(aCtrl->Region());
            
            }
        else // recalculate the region
            {

            region.Clear();
            region.AddRect(UiLayout()->Rect());
            CFepUiBaseCtrl* ctrl;
            for(TInt i = 0; i < RootControl()->ControlList().Count(); i++)
                {
                ctrl = RootControl()->ControlList()[i];
                if(ctrl->Ready() && !ctrl->Hiden() && 
                            !ctrl->IsKindOfControl(ECtrlTransparentHwrWnd))
                    {                    
                    region.SubRect(ctrl->Rect());
                    }
                }
            }
        }
    else
        {
        region.Clear();
        region.AddRect(Rect());    
        }
        
    SetRegion(region);        
    }   

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::ClearStroke
// Clear all strokes.
// ---------------------------------------------------------------------------
//
EXPORT_C void CTransparentHwrWnd::ClearStroke()
    {
    if ( PenTraceDecorationActivated() )
        {
        iPenTraceDecorator->Clear();
        }
    
    CHwrWndBase::ClearStroke();
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::InstallPenTraceDecoratorL
// Install a new pen trace decorator.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CTransparentHwrWnd::InstallPenTraceDecoratorL(const TDesC& /*aDecoratorName*/, TBool aEnable )
    {
    if ( iPenTraceDecorator )
        {
        delete iPenTraceDecorator;
        iPenTraceDecorator = NULL;
        }
    
    iPenTraceDecorator = CPeninputPenTraceDecorator::NewL();
    if ( aEnable )
        {
        iPenTraceDecorator->Enable();
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::SupportPenTraceDecoration
// Test whether this window support trace decoration.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CTransparentHwrWnd::SupportPenTraceDecoration()
    {
    return iPenTraceDecorator ? ETrue : EFalse;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::PenTraceDecorationActivated
// Test whether pen trace decoration is activated.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CTransparentHwrWnd::PenTraceDecorationActivated()
    {
    if ( iPenTraceDecorator )
        {
        return iPenTraceDecorator->Enabled();
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWnd::ActivatePenTraceDecoration
// Activate/deactivate pen trace decoration.
// ---------------------------------------------------------------------------
//
EXPORT_C void CTransparentHwrWnd::ActivatePenTraceDecoration(TBool aActive, TBool aReleaseBuffer )
    {
    if ( iPenTraceDecorator )
        {
        if ( aActive )
            {
            iPenTraceDecorator->Enable();
            }
        else
            {
            iPenTraceDecorator->Disable( aReleaseBuffer );
            }
        }
    }
// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::CTransparentHwrWndExt
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CTransparentHwrWndExt::CTransparentHwrWndExt( const TRect& aRect,
                                                       CFepUiLayout* aUiLayout,
                                                       TInt aControlId,
                                                       TBool aFullScreenFlag, 
                                                       TBool aShowGuideLine )
    :CTransparentHwrWnd( aRect, aUiLayout, aControlId, aFullScreenFlag ),
     iShowGuideLine( aShowGuideLine ), iGuideLineStyle( EGuideLineNone ),
     iGuideLineColor( TRgb(0) ), iGuideLineWidth( 1 ), iEnableFade( EFalse )
    {
        
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::~CTransparentHwrWndExt
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//             
EXPORT_C CTransparentHwrWndExt* CTransparentHwrWndExt::NewL( const TRect& aRect,
                                                             CFepUiLayout* aUiLayout,
                                                             TInt aControlId,
                                                             TBool aFullScreenFlag, 
                                                             TBool aShowGuideLine )
    {
    CTransparentHwrWndExt* wnd=new (ELeave) CTransparentHwrWndExt( aRect,
                                                                   aUiLayout,
                                                                   aControlId,
                                                                   aFullScreenFlag,
                                                                   aShowGuideLine );
    CleanupStack::PushL(wnd);
    wnd->ConstructL ();
    CleanupStack::Pop(wnd);
    return wnd;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CTransparentHwrWndExt::ConstructL()
    {
    iStrokeEndMark=TPoint(0xffff,0);
    iFadeTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    // Construct GS CenRep
    if(iGSRepository == NULL)
        {
        TRAP_IGNORE(iGSRepository
                = CRepository::NewL(KCRUidPersonalizationSettings) );
        }
    TInt newValue = 3;
    if ( iGSRepository )
        {
        iGSRepository->Get( KSettingsWritingSpeed, newValue );  
        }    
    SetFadingSpeed(newValue);
    iFadeBeginPos = Rect().iTl.iX;
    iFadeEndPos = Rect().iTl.iX;
    CHwrWndBase::ConstructL();
    }
// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::~CTransparentHwrWndExt
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C CTransparentHwrWndExt::~CTransparentHwrWndExt()
    {
    iListOfShownPoints.Close();
    delete iFadeTimer;
    delete iGSRepository;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::HideGuideLine
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CTransparentHwrWndExt::HideGuideLine( TBool aHideFlag )
    {
    iShowGuideLine = aHideFlag;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::SetTopGuideLinePosition
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CTransparentHwrWndExt::SetTopGuideLinePosition( const TPoint& aLeftTop, 
                                                              const TPoint& aRightBottom )
    {
    iTlOfTopGuideLine = aLeftTop;
    iBrOfTopGuideLine = aRightBottom;        
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::SetBottomGuideLinePosition
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CTransparentHwrWndExt::SetBottomGuideLinePosition( const TPoint& aLeftTop, 
                                                                 const TPoint& aRightBottom )
    {
    iTlOfBottomGuideLine = aLeftTop;
    iBrOfBottomGuideLine = aRightBottom;        
    }
    
// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::SetGuideLineStyle
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CTransparentHwrWndExt::SetGuideLineStyle( TInt aGuideLineStyle )
    {
    iGuideLineStyle = aGuideLineStyle;
    }    

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::RefreshUI
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CTransparentHwrWndExt::RefreshUI()
    {
    if( !Hiden() )
        {
        Draw();
        UpdateArea(Rect(), ETrue);        
        }    
    }    

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::SetGuideLineColor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CTransparentHwrWndExt::SetGuideLineColor( const TRgb& aColor )
    {
    iGuideLineColor = aColor;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::SetGuideLineWidth
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CTransparentHwrWndExt::SetGuideLineWidth( TInt aWidth )
    {
    iGuideLineWidth = aWidth;
    }		
  
// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::Draw
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CTransparentHwrWndExt::Draw()
    {
    //if(Hiden() || !Ready() || !UiLayout()->LayoutReady())
      //  return;
    if(!AbleToDraw())
        return;
    CTransparentHwrWnd::Draw();
    
    if ( iShowGuideLine && iShowGuideLine != EGuideLineNone )
        {
        DrawGuidingLine();
        }
    }   

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::DrawStroke
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C TRect CTransparentHwrWndExt::DrawStroke(TBool aDrawFlag)
    {
    TRect DirtyRect;
    if(iListOfShownPoints.Count())
        {
        TRect rect;
        TPoint pt;
        TPoint prevPt;

        TBool beginStroke = ETrue;
        for(TInt i=0;i<iListOfShownPoints.Count();i++)
            { 
            pt=iListOfShownPoints[i] ;
            
            if(pt == StrokeEndMark() )
                {
                beginStroke = ETrue;
                continue;
                }
            else
                {
                if ( beginStroke )
                    {
                    rect = DrawPoint(pt,aDrawFlag);
                    beginStroke = EFalse;
                    }
                else
                    {
                    rect = DrawLine(prevPt,pt,aDrawFlag);
                    }
                
                DirtyRect.BoundingRect(rect);
                prevPt=pt;
                }
            }
        }
    return DirtyRect;
    }
    
// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::DrawPoint
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
TRect CTransparentHwrWndExt::DrawPoint(const TPoint& aPoint,TBool aDrawFlag)
    {
    TRect affectedRect(0,0,0,0);
        {
        CFbsBitGc* gc= static_cast<CFbsBitGc*> (BitGc());
    
        gc->Activate( BitmapDevice() );
        gc->SetPenSize( PenSize());
        if(aDrawFlag)
            gc->SetPenColor( PenColor() );
        else    //erase the point            
            gc->SetPenColor( BkColor());
        gc->Plot( aPoint );
        gc->RectDrawnTo(affectedRect); //get the affected rect since last draw

        TRect maskRect;

        gc->Activate( MaskBitmapDevice());
        if ( ( aPoint.iX > iFadeBeginPos ) && ( aPoint.iX <= iFadeEndPos ) )
            {
            
            TRgb rgb( 128 - TransparencyFactor()/2,
                      128 - TransparencyFactor()/2,
                      128 - TransparencyFactor()/2 );
            gc->SetPenColor( rgb );
            }
        else
            {
            gc->SetPenColor( KOpaqueColor );
            }
        //use same color to get opague point
        gc->Plot( aPoint );
        gc->RectDrawnTo(maskRect);

        affectedRect.BoundingRect(maskRect); //find the bounding rect
        //if(!TraceOutsideWindowFlag())
            affectedRect.Intersection(iRect); //drawing not allowed outside of 
        
        }
    return affectedRect;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::DrawLine
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
TRect CTransparentHwrWndExt::DrawLine(const TPoint& aPt1,const TPoint& aPt2,TBool aDrawFlag)
    {
    TRect affectedRect(0,0,0,0);
        {
        CFbsBitGc* gc= static_cast<CFbsBitGc*> (BitGc());
    
        gc->Activate( BitmapDevice() );
        gc->SetClippingRect(Rect()); 
        gc->SetPenSize( PenSize());
        if(aDrawFlag)
            gc->SetPenColor( PenColor() );
        else            
            gc->SetPenColor( BkColor());
        gc->SetPenStyle(CGraphicsContext::ESolidPen);       
        gc->DrawLine( aPt1,aPt2 );

        gc->RectDrawnTo(affectedRect); //get the affected rect since last draw
        gc->CancelClippingRect();
         
        TRect maskRect;

        gc->Activate( MaskBitmapDevice() );
        gc->SetClippingRect(Rect());   
        if ( ( Min(aPt1.iX, aPt2.iX) > iFadeBeginPos ) && 
            ( Max(aPt1.iX, aPt2.iX) <= iFadeEndPos ) )
            {
            TRgb rgb( 128 - TransparencyFactor()/2,
                      128 - TransparencyFactor()/2,
                      128 - TransparencyFactor()/2 )       ;
            gc->SetPenColor( rgb );
//            gc->SetPenColor( KRgbRed );
            }  
        else
            {
            gc->SetPenColor( KOpaqueColor );
            } 

        //use same color as in bitmap to get opague line
        //gc->SetPenStyle(CGraphicsContext::ESolidPen);        
        gc->DrawLine( aPt1,aPt2 );
        gc->RectDrawnTo(maskRect);

        //affectedRect.BoundingRect(maskRect); //find the bounding rect
/*        if(!iTraceOutsideWindowFlag)
            {
            TRect r = affectedRect;
            affectedRect.Intersection(iRect); //drawing not allowed outside of 
            
            }*/
        gc->CancelClippingRect();             
        }
    return affectedRect;
    } 

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::OnDeActivate
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C void CTransparentHwrWndExt::OnDeActivate()
    {
    CHwrWndBase::OnDeActivate();
    iFadeTimer->Cancel();
    iListOfShownPoints.Reset();
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::OnLayoutDraggingStart
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C void CTransparentHwrWndExt::OnLayoutDraggingStart()
    {
    iFadeTimer->Cancel();
    iListOfShownPoints.Reset();                
    CHwrWndBase::OnLayoutDraggingStart();
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::EndStrokeAndClearL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
void CTransparentHwrWndExt::EndStrokeAndClearL()
    {
    iFadeTimer->Cancel();
    iListOfShownPoints.Reset();
    iFadeBeginPos = Rect().iTl.iX;
    iFadeEndPos = Rect().iTl.iX;
    CHwrWndBase::EndStrokeAndClearL();
    } 

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::CancelPointerDownL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
void CTransparentHwrWndExt::CancelPointerDownL()
    {
    if( !PointerDown() )
        {
        return;
        }
	//allow pen up in other control. Do us normal pen up
	if( iListOfShownPoints.Count() > 0 )
        {
        iListOfShownPoints.AppendL(iStrokeEndMark);
        iFadeTimer->Cancel();
        }
    
    CHwrWndBase::CancelPointerDownL();
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::HandlePointerDownEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//               
EXPORT_C CFepUiBaseCtrl* CTransparentHwrWndExt::HandlePointerDownEventL(const TPoint& aPoint)
    {
    iListOfShownPoints.AppendL(aPoint);
    iFadeMaxPos = aPoint.iX;
    
    if ( aPoint.iX < iFadeEndPos )
        {
        iFadeTimer->Cancel();
        EndStrokeAndClearL();
        }
    if( !iFadeTimer->IsActive() && iEnableFade)
        {
        iFadeTimer->Start( iFadeBeginTime, iFadeIntervalTime, TCallBack( UpdateShownArea, this ) );
        }
        
    return CTransparentHwrWnd::HandlePointerDownEventL(aPoint);
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::HandlePointerMoveEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C CFepUiBaseCtrl* CTransparentHwrWndExt::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    if(!PointerDown())
       return this;
    
    iListOfShownPoints.AppendL(aPoint);
    
    if( aPoint.iX < iFadeMaxPos )
        {
        iFadeMaxPos = aPoint.iX;
        }

    return CTransparentHwrWnd::HandlePointerMoveEventL(aPoint);
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::HandlePointerUpEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C CFepUiBaseCtrl* CTransparentHwrWndExt::HandlePointerUpEventL(const TPoint& aPoint)
    {
    iListOfShownPoints.AppendL(iStrokeEndMark);
    iFadeMaxPos = Rect().iBr.iX;	
    return CTransparentHwrWnd::HandlePointerUpEventL(aPoint);
    }
    
// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::UpdateShownArea
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
TInt CTransparentHwrWndExt::UpdateShownArea( TAny* aAny )
    {
    CTransparentHwrWndExt* self = static_cast<CTransparentHwrWndExt*>(aAny);
    self->DoUpdateShownArea();
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTransparentHwrWndExt::DoUpdateShownArea
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
void CTransparentHwrWndExt::DoUpdateShownArea()
    {
    iFadeBeginPos = iFadeEndPos > iFadeMaxPos ? iFadeMaxPos : iFadeEndPos;
    iFadeEndPos = iFadeBeginPos + Rect().Width()/KFadeAreaCount;
    iFadeEndPos = iFadeEndPos > iFadeMaxPos ? iFadeMaxPos : iFadeEndPos;
    
    TRect eraseRect(Rect());
    eraseRect.iTl.iX = eraseRect.iBr.iX = iFadeBeginPos;
    
    for( TInt i = iListOfShownPoints.Count()-1; i >= 0; i-- )
        {
        if ( iListOfShownPoints[i].iX <= iFadeBeginPos )
            {
            if( iListOfShownPoints[i].iX < eraseRect.iTl.iX )
                {
                eraseRect.iTl.iX = iListOfShownPoints[i].iX;
                }
            iListOfShownPoints.Remove(i);
            }
        }
    eraseRect.iTl.iX -= PenSize().iWidth;
    eraseRect.iBr.iX += PenSize().iWidth;
    Draw();
    UpdateArea( eraseRect, EFalse );      
    }
    
EXPORT_C void CTransparentHwrWndExt::CancelCharWriting()
    {
    iFadeTimer->Cancel();
    iListOfShownPoints.Reset();
    iFadeBeginPos = Rect().iTl.iX;
    iFadeEndPos = Rect().iTl.iX;
    CHwrWndBase::CancelCharWriting();
    }
    
void CTransparentHwrWndExt::DrawGuidingLine()
    {
    CFbsBitGc* gc= static_cast<CFbsBitGc*> ( BitGc() );        

    gc->Activate( MaskBitmapDevice() );
    gc->SetPenSize( TSize( iGuideLineWidth, iGuideLineWidth ) );
    gc->SetPenColor( KOpaqueColor );
    gc->SetPenStyle(CGraphicsContext::ESolidPen); 
                                      
    switch ( iGuideLineStyle )
        {
        case  EGuideLineTop:
            {
            gc->DrawLine( iTlOfTopGuideLine, iBrOfTopGuideLine );    
            }                    
            break;
        case  EGuideLineBottom:
            {
            gc->DrawLine( iTlOfBottomGuideLine, iBrOfBottomGuideLine );                        
            }
            break;
        case  EGuideLineBoth:
            {
            gc->DrawLine( iTlOfTopGuideLine, iBrOfTopGuideLine );
            gc->DrawLine( iTlOfBottomGuideLine, iBrOfBottomGuideLine );
            }
            break;
        default:
            // Do nothing
            break;
        }                

    gc->Activate( BitmapDevice() );
    gc->SetPenSize( TSize( iGuideLineWidth, iGuideLineWidth ) );
    gc->SetPenColor( iGuideLineColor );
    gc->SetPenStyle(CGraphicsContext::ESolidPen); 
                                      
    switch ( iGuideLineStyle )
        {
        case  EGuideLineTop:
            {
            gc->DrawLine( iTlOfTopGuideLine, iBrOfTopGuideLine );    
            }                    
            break;
        case  EGuideLineBottom:
            {
            gc->DrawLine( iTlOfBottomGuideLine, iBrOfBottomGuideLine );                        
            }
            break;
        case  EGuideLineBoth:
            {
            gc->DrawLine( iTlOfTopGuideLine, iBrOfTopGuideLine );
            gc->DrawLine( iTlOfBottomGuideLine, iBrOfBottomGuideLine );
            }
            break;
        default:
            // Do nothing
            break;
        }                        
    }
// end of file

