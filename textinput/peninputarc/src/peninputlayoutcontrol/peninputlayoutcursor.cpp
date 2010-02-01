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



#include "peninputlayoutcursor.h"
#include "peninputlayouteditareabase.h"
#include "peninputlayouttimer.h"
#include "peninputlayoutrootctrl.h"

const TInt KDefaultCursorHeight = 10;
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CFepUiCursor::CFepUiCursor
// C++ default constructor can NOT contain any code, that might leave
// ---------------------------------------------------------------------------
//
CFepUiCursor::CFepUiCursor(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aId)
                :CFepUiBaseCtrl(aRect,aUiLayout,aId),
                iPosition(0, 0),
                 iHeight(KDefaultCursorHeight),
                 iIsOn(EFalse),
                 iIsVisible(EFalse),
                 //iEditor(aOwner),
                 //iBitmapDevice(aBitmapDevice),
                 //iGc(aGc),
                 iCursorTempDisabled(EFalse)
    {
    SetControlType(ECtrlCursor);
    }
 
// ---------------------------------------------------------------------------
// CFepUiCursor::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CFepUiCursor::ConstructL()
    {
    CFepUiBaseCtrl::BaseConstructL();
    iCursorBlinkingTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    iRestoreCursorTimer = CLayoutTimer::NewL(this,CLayoutTimer::EOthers);
    }

// ---------------------------------------------------------------------------
// CFepUiCursor::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CFepUiCursor* CFepUiCursor::NewL(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aId)
    {
    CFepUiCursor* self = new(ELeave) CFepUiCursor(aRect, aUiLayout,aId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); // insertionPoint
    return self;
    }

// ---------------------------------------------------------------------------
// CFepUiCursor::~CFepUiCursor
// Destructor
// ---------------------------------------------------------------------------
//
CFepUiCursor::~CFepUiCursor()
    {
    if(iCursorBlinkingTimer)
        {        
        iCursorBlinkingTimer->Cancel();
        delete iCursorBlinkingTimer;
        }    
   
    delete iRestoreCursorTimer;
    }


// ---------------------------------------------------------------------------
// CFepUiCursor::SetPosition
// Sets the position of the sprite if it has been turned on
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiCursor::SetPosition(const TPoint& aPosition)
    {
    
    TBool isOn = iIsOn;
    if (isOn)
        {
        SetOn(EFalse);
        }
    iPosition=aPosition;
    TPoint correction(KCursorPosCorrectionX,KCursorPosCorrectionY );
    TRect rect = TRect(iPosition + correction , TSize(KCursorWidth,iHeight));
    if(rect != iCursorRect)
        {
        SetRect(rect);
        iCursorRect = rect;
        //must update clip region again.
        for(TInt i = 0 ; i < RootControl()->PopCtrlList().Count(); ++i)
            {
            UpdateValidRegion(RootControl()->PopCtrlList()[i],EFalse);
            }
        }
        
    if (isOn)
        {
        SetOn(ETrue);
        }
    }

// ---------------------------------------------------------------------------
// CFepUiCursor::Position
// get the cursor position
// ---------------------------------------------------------------------------
//
EXPORT_C TPoint CFepUiCursor::Position()
    {
    return iPosition;
    }

// ---------------------------------------------------------------------------
// CFepUiCursor::SetLayoutOwner
// Sets the layout owner
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
/*void CFepUiCursor::SetLayoutOwner(MLayoutOwner* aOwner)
    {
    iLayoutOwner = aOwner;
    }
*/

// ---------------------------------------------------------------------------
// CFepUiCursor::SetOn
// Turns the sprite on and off
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiCursor::SetOn(TBool aOn, TBool aImmediately)
    {
    //aOn = EFalse;
    iIsOn = aOn;    

    // If cursor will bee set off and timer is active, Cancel timer
    if ( !iIsOn )
        {
        CancelDelayCursorShown();
        if ( iCursorBlinkingTimer->IsActive())
            {
            iCursorBlinkingTimer->Cancel();    
            }
        }
    else
        {
        if(iCursorTempDisabled)
            {
            return;
            }
        // Activate blinking timer, if it is not already activated
        if ( !iCursorBlinkingTimer->IsActive() )
            {
            iCursorBlinkingTimer->Start(aImmediately ? 0: KCursorBlinkPerioid,
                   KCursorBlinkPerioid,TCallBack(CursorBlinkCallBack , this));
            }
        }
        
        // Invalidate InsertionPoint's rect, so it will be removed from editarea
    InvalidateInsertionPoint();
    }

// ---------------------------------------------------------------------------
// CFepUiCursor::SetHeight
// Sets caret height
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiCursor::SetHeight(TInt aHeight)
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

EXPORT_C void CFepUiCursor::ReDraw(TBool aReset)
    {
    if(aReset)
        iIsVisible = EFalse;
    Draw();
    UpdateArea(iCursorRect,EFalse);
    }
    

// ---------------------------------------------------------------------------
// CFepUiCursor::Draw
// Draws insertion point's Rect and starts blinking timer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepUiCursor::Draw()//CBitmapContext* aGc, TBool aReset)
    {
    if(!AbleToDraw())
        {
        return;
        }
    
    /*if (aReset)    
        {
        iIsVisible = EFalse;
        }
      */  

    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());                
    gc->Activate( BitmapDevice());    
    if(iIsOn)
        {        
        // draw cursor by inverting colors in the selected text rectancle
        gc->SetClippingRegion(ValidClipRegion());        	        
        
        gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
        gc->SetBrushColor(KRgbBlack);
        gc->SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);

        gc->SetPenColor(KRgbBlack);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->SetPenSize( TSize(1,1));

        // When the blink timer out and cursor is visible, do nothing
        // else draw the cursor and set the visible flag
        gc->DrawRect(iCursorRect);
        iIsVisible = !iIsVisible;

        // restore normal draw mode
        gc->SetDrawMode(CGraphicsContext::EDrawModePEN);
        gc->SetBrushStyle(CGraphicsContext::ENullBrush);
        
        //if (iEditor)
            {
          //  iEditor->UpdateArea(iCursorRect,EFalse);
            }
        
     //   UpdateArea(iCursorRect,EFalse);
        } 
    else
        {
        if(iIsVisible) //only do when already shown
            {                
            //CFbsBitmapDevice* bitmapDevice = iEditor->BitmapDevice();  
            //static_cast<CFbsBitGc*>(gc)->Activate(iBitmapDevice);
            gc->SetClippingRegion(ValidClipRegion());        	    
            gc->SetPenColor(KRgbBlack);
            gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
            gc->SetBrushColor(KRgbBlack);
            gc->SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);
            gc->SetPenStyle(CGraphicsContext::ESolidPen);
            gc->SetPenSize( TSize(1,1));
            gc->DrawRect(iCursorRect);

            // restore normal draw mode
            gc->SetDrawMode(CGraphicsContext::EDrawModePEN);
            gc->SetBrushStyle(CGraphicsContext::ENullBrush);
            
            iIsVisible = EFalse;                       

            /*if (iEditor)
                {
                iEditor->UpdateArea(iCursorRect,EFalse);
                }*/
       //     UpdateArea(iCursorRect,EFalse);                
            }   
        }
    gc->CancelClippingRegion();        	        
    }
    
// ---------------------------------------------------------------------------
// CFepUiCursor::CursorBlinkCallBack
// Invalidates cursor rect, when timer gets out. 
// With periodic caused draw/undraw of cursor will cause cursor blinking
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CFepUiCursor::CursorBlinkCallBack(TAny *aPtr)
    {
    CFepUiCursor* self = static_cast<CFepUiCursor*>(aPtr); 
           
    self->InvalidateInsertionPoint();
        
    return(ETrue);        
    }

// ---------------------------------------------------------------------------
// CFepUiCursor::InvalidateCursor
// Invalidates InsertionPoint's rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepUiCursor::InvalidateInsertionPoint()
    {    
    //if(iEditor)
        {        
        Draw();
        UpdateArea(iCursorRect,EFalse);
        }
    }
  
// ---------------------------------------------------------------------------
// CFepUiCursor::DelayCursorWhileDraging
// Disable the cursor while pen is draging
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiCursor::DelayCursorWhileDraging()
    {
    DelayCursorShown();
    }
  
// ---------------------------------------------------------------------------
// CFepUiCursor::DelayCursorShown
// Disable the cursor while pen is draging
// (other items were commented in a header).
// ---------------------------------------------------------------------------
EXPORT_C void CFepUiCursor::DelayCursorShown()
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
// cancel delay shown process
// ---------------------------------------------------------------------------
//
void CFepUiCursor::CancelDelayCursorShown()
    {
    iRestoreCursorTimer->Cancel();
    iCursorTempDisabled = EFalse;
    }
// ---------------------------------------------------------------------------
// CFepUiCursor::HandleTimerOut
// Set the cursor on
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepUiCursor::HandleTimerOut(TInt /*aTimeType*/)
    {
    iCursorTempDisabled = EFalse;
    iIsOn = ETrue;
    //To avoid the ugly first shown, we draw the cursor directly here.
    InvalidateInsertionPoint();
    SetOn(ETrue, EFalse);    
    }
    
    
// ---------------------------------------------------------------------------
// CFepUiCursor::UpdateValidRegion
// Update control's valid region when other control show or hide.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiCursor::UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag)
    {
    if(!iUpdateFlagEnabled)
        return;
   
    if(aRemoveFlag)
        {
        }
    else
        {
        if(aCtrl->Hiden())
            return;
        TRect rect = Rect();
        rect.Intersection(aCtrl->Rect());
        iValidRegion.SubRect(rect);
        iClipRegion.SubRect(rect);
        }
        //iValidRegion.AddRect(iRect);
    return;
    }

TInt CFepUiCursor::OrderPos()
    {
    TInt ret = 0;
    TRAP_IGNORE(ret = OrderPosL());
    return ret;
    }

TInt CFepUiCursor::OrderPosL()
    {
    __ASSERT_DEBUG(iEditor,User::Leave(EUiCursorNoOwner));
    return iEditor->OrderPos();    
    }
    
    
void CFepUiCursor::ResetValidClipRegion()
    {
    iValidRegion.Clear();
    iClipRegion.Clear();
    iValidRegion.AddRect(iRect);
    iClipRegion.AddRect(iRect);
    }
    
void CFepUiCursor::UpdateCursorArea(const TRect& aRect, TBool aFlag)
    {
    CFepUiBaseCtrl* parent = iEditor->ParentCtrl();
    
    if(parent)
        {
        if(parent->IsKindOfControl(ECtrlControlGroup))     
            static_cast<CControlGroup*>(parent)->UpdateAreaInGrp(iEditor,aRect,aFlag);
        else
            iEditor->ParentCtrl()->UpdateArea(aRect,aFlag);
        }
    else
        UpdateArea(aRect,aFlag);    
    }
//  End of File  
