/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for button base and dragbar
*
*/

#include <AknFepGlobalEnums.h>
#include "peninputlayoutbutton.h"
#include "peninputlayouttimer.h"
#include "peninputlayout.h"

#ifdef __WINS__
const TInt KSpriteMoveDrawInterval    = 10000; // 1/100 second
#else
const TInt KSpriteMoveDrawInterval    = 1000; // 1/1000 second
#endif

#define __USING_MOVING_FRAME__	//flag to use the moving frame when dragging the layout
// ============================ MEMBER FUNCTIONS ===============================


/******************** start of CButtonBase **********************************/

// ---------------------------------------------------------------------------
// CButtonBase::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CButtonBase* CButtonBase::NewL(const TRect& aRect,
                                        CFepUiLayout* aUiLayout,TInt aCtrlId)
    {
    CButtonBase* btn=new (ELeave)CButtonBase(aRect,aUiLayout,aCtrlId);
    CleanupStack::PushL(btn);
    btn->BaseConstructL();
    CleanupStack::Pop(btn);
    return btn;    
    }
    
// ---------------------------------------------------------------------------
// CButtonBase::CButtonBase
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CButtonBase::CButtonBase(const TRect& aRect,CFepUiLayout* aUiLayout,
                                                                TInt aCtrlId)
                        :CFepUiBaseCtrl(aRect,aUiLayout,aCtrlId),
                         iPressed(EFalse),
                         iStretchable(ETrue),
                         iBaseline(-1),
                         iCaptionColor(KRgbWhite), //white caption
                         iCaptionShadowColor(KRgbBlack) //black caption shadow
    {
    SetControlType(ECtrlButton | ECtrlTactileFeedback);
    iDimmed = EFalse;
#ifdef __ALPHA_TRANCEPARENCY__
    const TRgb KDefaultButtonBackCol = TRgb(30,30,30);
#else
    const TRgb KDefaultButtonBackCol = TRgb(230,230,230);
#endif
    
    SetBkColor(KDefaultButtonBackCol);

#ifdef RD_TACTILE_FEEDBACK     
    //Advanced Tactile feedback REQ417-47932
    if(aUiLayout)
    	{
    	if (aUiLayout->PenInputType() == EPluginInputModeFSQ||
    		aUiLayout->PenInputType() == EPluginInputModeVkb)
    		{
    		SetTactileFeedbackType(ETouchFeedbackBasicButton);
    		aUiLayout->RegisterFeedbackArea(reinterpret_cast<TInt>(this),aRect,ETouchFeedbackBasicButton);
    		}
    	else
    		{
    		SetTactileFeedbackType(ETouchFeedbackSensitiveKeypad);
    		aUiLayout->RegisterFeedbackArea(reinterpret_cast<TInt>(this),aRect,ETouchFeedbackSensitiveKeypad);
    		}
    	}       
#endif //RD_TACTILE_FEEDBACK     
    }

// ---------------------------------------------------------------------------
// CButtonBase::~CButtonBase
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CButtonBase::~CButtonBase()
    {  
    delete iCaption;
    
    //only free the bitmaps in the pool
    iBmpPool.ResetAndDestroy();    
    
    //must reset the background bmp otherwise CFepUiBaseCtrl will re-delete it.    
    SetBackgroundBmp(NULL);
    SetBackgroundMaskBmp(NULL);    
    //font
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }        
    delete iNoUsedBkbmp;
    delete iNoUsedMaskBkbmp;      

#ifdef RD_TACTILE_FEEDBACK     
    //de-register the area for tactile feedback
    //if(aUiLayout) //there must be aUiLayout
    UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect());          
#endif // RD_TACTILE_FEEDBACK     
    }

// ---------------------------------------------------------------------------
// CButtonBase::Draw
// Draw the control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CButtonBase::Draw()
    {
    if(!AbleToDraw())
    	return;    
        
    if(iWndControl)
        {
        CFepUiBaseCtrl::Draw();
        return;
        }
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());        
	DrawOpaqueMaskBackground(iStretchable);
        
        //mask bitmaps
		DrawBackground();            
        //draw caption if there is
    //gc->SetClippingRegion(&ValidClipRegion());  //this cause crash in generic vkb
        if(iCaption && iFont)
            {
            //use font
            gc->UseFont(iFont);
            gc->SetBrushStyle( CGraphicsContext::ENullBrush ); 
            gc->SetPenStyle(CGraphicsContext::ESolidPen);                        
            
           
            TRect rect(iRect);
            
            //draw text shadow first
            rect.Move(1,1);
            
            TInt baseLine = iBaseline > 0 ? iBaseline : 
                                rect.Height()*KDEFAULTBASELINERATIO;      
            //set pen color for text shadow
            gc->SetPenColor(iCaptionShadowColor);                                      
            gc->DrawText(*iCaption,rect,baseLine,CGraphicsContext::ECenter );

            //Now draw caption on top of the shadow
            //re-cal the baseline
            baseLine = iBaseline > 0 ? iBaseline : 
                                iRect.Height()*KDEFAULTBASELINERATIO;
            gc->SetPenColor(iCaptionColor);              
            gc->DrawText(*iCaption,iRect,baseLine,CGraphicsContext::ECenter );
            
        gc->DiscardFont();
        }
    gc->CancelClipping();
    }

// ---------------------------------------------------------------------------
// CButtonBase::HandleButtonDown
// Handle button down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CButtonBase::HandlePointerDownEventL(const TPoint& aPt)
    {    
    
    if(iDimmed)
        return NULL;
    
    CFepUiBaseCtrl::HandlePointerDownEventL(aPt);
    if(IsActive())
        {
        SetBackgroundBmp(iActiveBmpPressed);
        SetBackgroundMaskBmp(iActiveMaskBmpPressed);
        }        
    else
        {
        SetBackgroundBmp(iNonActiveBkBmpPressed);
        SetBackgroundMaskBmp(iNonActiveBkMaskBmpPressed);
        }        

    Draw();
    UpdateArea(Rect(), EFalse);
    ReportEvent(EEventButtonDown);
    return this;
    }
    
// ---------------------------------------------------------------------------
// CButtonBase::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CButtonBase::HandlePointerLeave(const TPoint& aPt)
    {
    if(iDimmed)
        return;
    if(IsActive())
        {
        SetBackgroundBmp(iActiveBmp);
        SetBackgroundMaskBmp(iActiveMaskBmp);
        }
        
    else
        {
        SetBackgroundBmp(iNonActiveBkBmp);
        SetBackgroundMaskBmp(iNonActiveBkMaskBmp);
        }
    Draw();
    UpdateArea(Rect(), EFalse);   
    CFepUiBaseCtrl::HandlePointerLeave(aPt);
    return ;
    }
    
// ---------------------------------------------------------------------------
// CButtonBase::HandlePointerEnter
// Handle pointer enter event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CButtonBase::HandlePointerEnter(const TPoint& aPt)
    {
    if(iDimmed)
        return;
    if(IsActive())
        {
        SetBackgroundBmp(iActiveBmpPressed);
        SetBackgroundMaskBmp(iActiveMaskBmpPressed);
        }        
        
    else
        {
        SetBackgroundBmp(iNonActiveBkBmpPressed);
        SetBackgroundMaskBmp(iNonActiveBkMaskBmpPressed);
        }
    Draw();
    UpdateArea(Rect(), EFalse);   
    CFepUiBaseCtrl::HandlePointerEnter(aPt);
    }

    
    
// ---------------------------------------------------------------------------
// CButtonBase::HandleButtonUpEventL
// Handle button up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C CFepUiBaseCtrl* CButtonBase::HandlePointerUpEventL(const TPoint& /*aPt*/)        
    {
    if(iDimmed)
        return NULL;
    if(IsActive())
        {
        SetBackgroundBmp(iActiveBmp);
        SetBackgroundMaskBmp(iActiveMaskBmp);
        }        
    else
        {
        SetBackgroundBmp(iNonActiveBkBmp);
        SetBackgroundMaskBmp(iNonActiveBkMaskBmp);
        }
    SetActive(ETrue);
    Draw();
    UpdateArea(Rect(), EFalse);    
    
    ReportEvent(EEventButtonUp);
    return this;
    }


// ---------------------------------------------------------------------------
// CButtonBase::HandleButtonMoveEventL
// Handle button move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CButtonBase::HandlePointerMoveEventL(const TPoint& /*aPt*/)
    {
    //do nothing
    return this;
    }

// ---------------------------------------------------------------------------
// CButtonBase::SetActive
// Set button active
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CButtonBase::SetActive(TBool aActiveFlag)
    {
    if(iDimmed)
        return;
    if(IsActive() != aActiveFlag)
        {
        CFepUiBaseCtrl::SetActive(aActiveFlag);
        if(aActiveFlag)
            {               
            SetBackgroundBmp(iActiveBmp);
            SetBackgroundMaskBmp(iActiveMaskBmp);
            }
        else
            {
            SetBackgroundBmp(iNonActiveBkBmp);
            SetBackgroundMaskBmp(iNonActiveBkMaskBmp);
            }
        if(BitGc())
            {       
            Draw();
            UpdateArea(Rect(),EFalse);
            }
        }
    }

// ---------------------------------------------------------------------------
// CButtonBase::SetDimmed
// Dim button
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CButtonBase::SetDimmed(TBool aDimFlag)
    {
    //Remove its active flag if dim an active button.
    if(aDimFlag && IsActive())
        SetActive(EFalse);
    iDimmed = aDimFlag;
    if(iDimmed)
        {        
        SetBackgroundBmp(iDimmedBmp);
        SetBackgroundMaskBmp(iDimmedMaskBmp);
        }
    else
        {        
        // set the bmp according to current status
        if(IsActive())
            {               
            SetBackgroundBmp(iActiveBmp);
            SetBackgroundMaskBmp(iActiveMaskBmp);
            }
        else
            {
            SetBackgroundBmp(iNonActiveBkBmp);
            SetBackgroundMaskBmp(iNonActiveBkMaskBmp);
            }        
        }
    
    if(BitGc())
        {       
        Draw();
        UpdateArea(Rect(), EFalse);
        }

#ifdef RD_TACTILE_FEEDBACK 
    if(IsKindOfControl(ECtrlTactileFeedback))
        {        
        //if(aDimFlag)
            UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect());
        if(!aDimFlag && !Hiden())
            UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect(),TactileFeedbackType());            
        }
#endif        
    }
  


// ---------------------------------------------------------------------------
// CButtonBase::SetBitmapL
// Set bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CButtonBase::SetBitmapL(CFbsBitmap* aBitmap, TButtonBmpType aType)
    {
    CFbsBitmap* bmp = aBitmap;
    
    if(!aBitmap)
        return;   
    AddBmpToPool(aBitmap);
    
    switch(aType)
        {
        case EBtnBmpNonActive :
            {            
            iNonActiveBkBmp = bmp;        
            if(!IsActive())
                SetBackgroundBmp(iNonActiveBkBmp);
            }
            break;
        case EBtnBmpNonActivePressed :
            iNonActiveBkBmpPressed = bmp;
            break;
        case EBtnBmpActive :
            iActiveBmp = bmp;    
            if(IsActive())
                SetBackgroundBmp(iActiveBmp);

            break;
        case EBtnBmpActivePressed :
            iActiveBmpPressed = bmp;        
            break;
        case EBtnBmpDimmed :
            iDimmedBmp = bmp;
            break;
        default:
            break;

        }
    }  
   
// ---------------------------------------------------------------------------
// CButtonBase::Bitmap
// get bitmap for given type
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFbsBitmap* CButtonBase::Bitmap(TButtonBmpType aType, TBool aMaskFlag)
    {
    CFbsBitmap* bmp = NULL;
    if(!aMaskFlag)
        {
        
        switch(aType)
            {
            case EBtnBmpNonActive :
                {            
                bmp = iNonActiveBkBmp;
                }
                break;
            case EBtnBmpNonActivePressed :
                bmp = iNonActiveBkBmpPressed;
                break;
            case EBtnBmpActive :
                bmp = iActiveBmp ;
                break;
            case EBtnBmpActivePressed :
                bmp = iActiveBmpPressed;
                break;
            case EBtnBmpDimmed :
                bmp = iDimmedBmp;
                break;
            default:
                break;
            }
        }
    else //get mask bitmap
        {
        switch(aType)
            {
            case EBtnBmpNonActive :
                bmp = iNonActiveBkMaskBmp;
                break;
            case EBtnBmpNonActivePressed :
                bmp = iNonActiveBkMaskBmpPressed;
                break;
            case EBtnBmpActive :
                bmp = iActiveMaskBmp;
                break;
            case EBtnBmpActivePressed :
                bmp = iActiveMaskBmpPressed;
                break;
            case EBtnBmpDimmed :
                bmp = iDimmedMaskBmp;
                break;
            default:
                break;
            }        
        }
    return bmp;
    }  
// ---------------------------------------------------------------------------
// CButtonBase::SetMaskBitmapL
// Set mask bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CButtonBase::SetMaskBitmapL(CFbsBitmap* aBmp, TButtonBmpType aType)
    {   
    if (!aBmp)
        return;
    
    AddBmpToPool(aBmp);

    switch(aType)
        {
        case EBtnBmpNonActive :
            iNonActiveBkMaskBmp = aBmp;        
            if(BkMaskBmp())
                SetBackgroundMaskBmp(iNonActiveBkMaskBmp);  
            break;
        case EBtnBmpNonActivePressed :
            iNonActiveBkMaskBmpPressed = aBmp;
            break;
        case EBtnBmpActive :
            iActiveMaskBmp = aBmp;    
            break;
        case EBtnBmpActivePressed :
            iActiveMaskBmpPressed = aBmp;        
            break;
        case EBtnBmpDimmed :
            iDimmedMaskBmp = aBmp;
            break;
        default:
            break;
        }
    }
   
   
// ---------------------------------------------------------------------------
// CButtonBase::SetCaptionL
// Set mask bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
EXPORT_C void CButtonBase::SetCaptionL(const TDesC& aText)
    {
    TBool bRedraw(EFalse);
    if(iCaption)
        {        
        delete iCaption;
        //already has a caption, redraw may be needed.
        bRedraw = ETrue;
        iCaption = NULL;        
        }
    iCaption = aText.AllocL();
    if(bRedraw)
        {
        Draw();
        UpdateArea(iRect, EFalse);
        }
    }
    
    
// ---------------------------------------------------------------------------
// CButtonBase::SetFont
// Set caption font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CButtonBase::SetFont(const TFontSpec& aFontSpec)
    {
    iFontSpec = aFontSpec;
    //release old font
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }    
    iFontOwnership = ETrue;    
    if(BitmapDevice())
        {
        if (KErrNone != BitmapDevice()->GetNearestFontInPixels(iFont,iFontSpec))
            iFont = NULL;
        }      
    }        
    
// ---------------------------------------------------------------------------
// CButtonBase::SetFont
// Set caption font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CButtonBase::SetFont(const CFont* aFont)
    {
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        }    
    iFontOwnership = EFalse;    
    iFont = const_cast<CFont*>(aFont);
    if(iFont)
        iFontSpec = iFont->FontSpecInTwips();
    }        

// ---------------------------------------------------------------------------
// CButtonBase::ResetBmpPool
// Rest bitmap pool
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CButtonBase::ResetBmpPool()
    {
    iBmpPool.ResetAndDestroy();        
    //must reset the background bmp otherwise CFepUiBaseCtrl will re-delete it.    
    SetBackgroundBmp(NULL);
    SetBackgroundMaskBmp(NULL);        
    }
    
    
// ---------------------------------------------------------------------------
// CButtonBase::SetBackgroundBitmapL
// Set back ground bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C void CButtonBase::SetBackgroundBitmapL(CFbsBitmap* aBmp)
    {
    if(!aBmp)
        return;
    AddBmpToPool(aBmp);
    SetBackgroundBmp(aBmp);    
    }

// ---------------------------------------------------------------------------
// CButtonBase::SetBackgroundMaskBitmapL
// Set back ground mask bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C void CButtonBase::SetBackgroundMaskBitmapL(CFbsBitmap* aBmp)
    {
    if(!aBmp)
        return;   
    AddBmpToPool(aBmp);
    SetBackgroundMaskBmp(aBmp);          
    }
    
EXPORT_C void CButtonBase::Hide(TBool aFlag)
	{
	CFepUiBaseCtrl::Hide(aFlag);
#ifdef RD_TACTILE_FEEDBACK   
    if(IsKindOfControl(ECtrlTactileFeedback))
        {        
        if(aFlag || iDimmed)
            UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect());
        }
#endif    
	}
    
// ---------------------------------------------------------------------------
// CButtonBase::AddBmpToPool
// add a bitmap to pool
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//               
void CButtonBase::AddBmpToPool(CFbsBitmap* aBmp)
    {
    //do we already has the bitmap?
    if(KErrNotFound == iBmpPool.Find(aBmp))
        {
        TRAP_IGNORE(iBmpPool.AppendL(aBmp));
        }        
    }    
    
/**********************    end of CButtonBase     ****************************/


/********************   Implementation of CDragBar   ************************/

// ---------------------------------------------------------------------------
// CDragBar::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CDragBar* CDragBar::NewL(const TRect& aRect,CFepUiLayout* aUiLayout,
                                                                 TInt aCtrlId)
    {
    CDragBar* btn = new (ELeave)CDragBar(aRect,aUiLayout,aCtrlId);
    CleanupStack::PushL(btn);
    btn->ConstructL ();
    CleanupStack::Pop(btn);
    return btn;    
    }

// ---------------------------------------------------------------------------
// CDragBar::~CDragBar
// Destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CDragBar::~CDragBar()
    {
    iPosObserverList.Close();       
    delete iLongPressTimer;
    delete iMoveTimer;    
    iLongPressTimer = NULL;
    iDraggingCtrlList.Close();
    }
 
// ---------------------------------------------------------------------------
// CDragBar::CDragBar
// Constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CDragBar::CDragBar(const TRect& aRect,CFepUiLayout* aUiLayout,
                                                                 TInt aCtrlId)
                :CButtonBase(aRect,aUiLayout,aCtrlId),
                 iIsDragging(EFalse),iMoved(EFalse),
                 iLayoutBeingDraged(EFalse),iNeedLongtimePress(EFalse),
                 iDraggingEnabled(ETrue),iDrawInterval(KSpriteMoveDrawInterval)
    {    
    SetControlType(ECtrlDragBar);
    iDragFrameRect = Rect();
    SetPenSize(TSize(KDefaultFrameWidth,KDefaultFrameWidth));
    iMovingIndicatorPos = aRect.iTl + PenSize();
    }

// ---------------------------------------------------------------------------
// CDragBar::ConstructL
// Second phase constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CDragBar::ConstructL()
    {
    BaseConstructL();
    iLongPressTimer = CLayoutTimer::NewL(this,CLayoutTimer::ELongPressTimer);
    iMoveTimer = CLayoutTimer::NewL(this,CLayoutTimer::EDragbarMoveTimer,ETrue);
    iMoveTimer->SetPriority(CActive::EPriorityHigh);
    AddPositionObserver(this);//add itself to be an position change observer        
    }

// ---------------------------------------------------------------------------
// CDragBar::HandleButtonDownEventL
// Handle pen down event in dragbar
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CDragBar::HandlePointerDownEventL(const TPoint& aPt)
    {    
    if(!iDraggingEnabled) //not support dragging
        {
        CButtonBase::HandlePointerDownEventL(aPt);
        return this;
        }
    //call base class to set state
    CFepUiBaseCtrl::HandlePointerDownEventL(aPt);        
    //init status
    iLongPressed = EFalse;    
    iMoved = EFalse;
    iMoveOffset = TPoint(0,0);    
    
    iDraggingStartPt = aPt;
    if(iLayoutBeingDraged)
        {
        //change draging start point to absolute point in layout dragging
        iDraggingStartPt += UiLayout()->Position();
        }
        
    
    iCurrentPointerPos = aPt;       
    CapturePointer();
    if(iNeedLongtimePress)
        {
        iIsDragging = EFalse;
        iLongPressTimer->SetTimer(iLongtimePressTimeout);        
        }
    else
        {
        //report event first, so other controls can prepare for dragging
        ReportEvent(EEventDraggingStart);                
        
        iIsDragging = ETrue;
        PrepareDragging();
#ifdef __USING_MOVING_FRAME__        
        DrawFrame(iDragFrameRect);
        ShowMovingIndicator();
        RootControl()->UpdateArea(iDragFrameRect, EFalse);        
#endif        
        iMoveTimer->SetTimer(iDrawInterval);        
        }
        
    return this;
    }

// ---------------------------------------------------------------------------
// CDragBar::HandleButtonUpEventL
// Handle pen up event in dragbar
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CDragBar::HandlePointerUpEventL(const TPoint& aPt)
    {
    CapturePointer(EFalse);
    if(!PointerDown())
        return this;

    if(!iDraggingEnabled)
        {
        return CButtonBase::HandlePointerUpEventL(aPt);        
        }    
    
    SetPointerDown(EFalse);
    if(iNeedLongtimePress)
        {
        iLongPressTimer->Cancel();    
        iLongPressed = EFalse;        
        }
    
    if(iIsDragging)
       {
       
       iIsDragging=EFalse;       
       iMoveTimer->Cancel();
       
       AfterDragging();
       ReportEvent(EEventDraggingEnd);       
       //Clear the frame of the last drawing
       TRect rect;
#ifdef __USING_MOVING_FRAME__       
       rect = DrawFrame(iDragFrameRect,EFalse);
       rect.BoundingRect(rect);
#endif       

       
       for(TInt i = iPosObserverList.Count()-1; i >= 0; i--)
            {
            if(iPosObserverList[i]->HandlePositionChange(iMoveOffset))
                break;    
            }
#ifdef __USING_MOVING_FRAME__            
       if(iLayoutBeingDraged) //special handling when layout is Dragging            
            {
             RootControl()->Draw();
            }
       else
           {
           //redraw the component.
            RootControl()->DrawRect(rect,EFalse); //not draw the frame
            UpdateArea(rect, EFalse);
           }

       RootControl()->UpdateValidRegion(NULL,EFalse);
#endif       
       iMoveOffset = TPoint(0,0);    
       }
    else
        {
        if(iNeedLongtimePress) 
            {            
            ReportEvent(EEventButtonUp);            
            }                           
        }
        
    return this;
    }
    
// ---------------------------------------------------------------------------
// CDragBar::HandleButtonMoveEventL
// Handle dragbar move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CDragBar::HandlePointerMoveEventL(const TPoint& aPt)       
    {
    if(!iDraggingEnabled) //dragging not allowed
        {
        CButtonBase::HandlePointerMoveEventL(aPt);
        return this;
        }
    
    if(!iIsDragging)
        {
        if(PointerDown())
            {
            iCurrentPointerPos = aPt;   
            }
        }        
    else    // being dragging
        {            
        TBool verticalMove(EFalse);
        TBool horizontalMove(EFalse);
        if(iLayoutBeingDraged) //special handling when layout is Dragging
            {            
            TRect dragRect(Rect());
            
            //convert to screen coordinate
            dragRect.Move(UiLayout()->Position());
            TPoint pt = aPt + UiLayout()->Position();            
            
            TPoint offset = pt-iDraggingStartPt;
            
            dragRect.Move(offset + iMoveOffset);
            
            //check whether dragRect is a valid position
            if(!UiLayout()->IsValidDestination(dragRect,this,verticalMove,
                                                horizontalMove,ETrue))
                {
                // Not valid, then check H and V direction separately
                if(verticalMove) //V is valid then H is not.
                    {
                    //set x offset to 0
                    offset.iX = 0;
                    }
                else
                    {
                    if(horizontalMove)//H is valid then V is not.
                        {
                        // set y offset to 0
                        offset.iY = 0;
                        }
                    else
                        {                        
                        offset = TPoint(0,0);//return this; 
                        //not valid in both direction. Can't move
                        }
                    }                
                }
            
            iDraggingStartPt += offset;                
            iMoveOffset += offset;                        
            ReportEvent(EEventDragging);
            return  this;
            }
        else    //component moving inside layout area
            {            
            if(IsValidDestination(aPt,verticalMove,horizontalMove))
                {
                iCurrentPointerPos = aPt;
                ReportEvent(EEventDragging);
                }
            else
                {
                // Not valid, then check H and V direction separately
                if(verticalMove)
                    {
                    //vertical move is allowed
                    iCurrentPointerPos.iY = aPt.iY;
                    }
                else
                    {
                    if(horizontalMove)
                        {
                        //horizontal move is allowed
                        iCurrentPointerPos.iX = aPt.iX;
                        }
                    }
                }
            }
        }
    //When Dragging, eats the move event even it's not a valid destination
    return this;
    }

// ---------------------------------------------------------------------------
// CDragBar::IsValidDestination
// Test whether the destination is valid for dragbar
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CDragBar::IsValidDestination(const TPoint& aPt,TBool& aVInfo,
                                                               TBool& aHInfo)
    {
    TRect rect(iDragFrameRect);
    TPoint offset(aPt-iDraggingStartPt);
    
    rect.Move(offset);
    
    return UiLayout()->IsValidDestination(rect,this,aVInfo,aHInfo,
                                                          iLayoutBeingDraged);
    }

// ---------------------------------------------------------------------------
// CDragBar::Move
// Move control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CDragBar::Move(const TPoint& aOffset)
    {
    CFepUiBaseCtrl::Move(aOffset);
    iMovingIndicatorPos += aOffset;
    iMoved = ETrue;
    
    } 

// ---------------------------------------------------------------------------
// CDragBar::AddPositionObserver
// Add position change observer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CDragBar::AddPositionObserver(MPositionObserver* aObserver)
    {
    if(!aObserver)
        return;    
    
     //is it already an observer?
    if(KErrNotFound != iPosObserverList.Find(aObserver))
        {
        return; // do nothing
        }
    
    if(aObserver == static_cast<MPositionObserver*>(RootControl()) )
        {        
        iLayoutBeingDraged = ETrue;        
        }
    
    iPosObserverList.Append(aObserver);
    
    iDragFrameRect.BoundingRect(aObserver->MovingArea());    
    
    if(aObserver->IsFepBaseControl())
        {
        CFepUiBaseCtrl* ctrl=static_cast<CFepUiBaseCtrl*>(aObserver);
        ctrl->AddEventObserver(this);                 
        }
    }

// ---------------------------------------------------------------------------
// CDragBar::AddToDraggingCtrlList
// Add a control to dragging list
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
void CDragBar::AddToDraggingCtrlList(CFepUiBaseCtrl* aCtrl)
    {
    if(!aCtrl->Ready())
        return;
    
    if(aCtrl->IsKindOfControl(ECtrlControlGroup))
        {
        CControlGroup* group = reinterpret_cast<CControlGroup*>(aCtrl);
        RPointerArray<CFepUiBaseCtrl> ctrlList = group->ControlList();
        for(TInt i = 0; i < ctrlList.Count(); i++)
            {
            if(ctrlList[i]->Ready())
                AddToDraggingCtrlList(ctrlList[i]);
            }
        }
    else
        {
        if(aCtrl->Ready()) //root control is not included
            iDraggingCtrlList.Append(aCtrl);
        }
    }


// ---------------------------------------------------------------------------
// CDragBar::PrepareDragging
// Change dragging control's state before dragging
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
void CDragBar::PrepareDragging()
    {
    TInt i;
    //re-calculate the dragging area
    ReCalculateDragArea();
    
    iDraggingCtrlList.Reset();
    for( i = 0; i < iPosObserverList.Count(); i++)
        {
        if(iPosObserverList[i]->IsFepBaseControl())
            {
            CFepUiBaseCtrl* ctrl=static_cast<CFepUiBaseCtrl*>(
                                                        iPosObserverList[i]);
            if(ctrl != RootControl())
                {
                AddToDraggingCtrlList(ctrl);
                }
            }
        }
        
    for(i = 0; i < iDraggingCtrlList.Count(); i++)
        {
        iDraggingCtrlList[i]->SetReady(EFalse);
        }
    }

// ---------------------------------------------------------------------------
// CDragBar::AfterDragging
// Change dragging control's state after dragging 
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
void CDragBar::AfterDragging()
    {
    TInt i;        
    for(i = 0; i < iDraggingCtrlList.Count(); i++)
        {
        iDraggingCtrlList[i]->SetReady(ETrue);
        }
    }

// ---------------------------------------------------------------------------
// CDragBar::RemovePositionObserver
// Remove position change observer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CDragBar::RemovePositionObserver(MPositionObserver* aObserver)
    {
    TInt index = iPosObserverList.Find(aObserver);
    if(KErrNotFound != index)
        {       
        if(aObserver == RootControl())
            iLayoutBeingDraged = EFalse;
    
        iPosObserverList.Remove(index);
        //re-calculate the moving rect
        iDragFrameRect = Rect();
        for(TInt i = 0; i < iPosObserverList.Count(); i++)
            iDragFrameRect.BoundingRect(iPosObserverList[i]->MovingArea());        
        }            
    }

// ---------------------------------------------------------------------------
// CDragBar::SetLongpressTimer
// Set long time press time out
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CDragBar::SetLongpressTimer(TTimeIntervalMicroSeconds32 aTime)
    {
    if(aTime.Int() <= 0)
        iNeedLongtimePress = EFalse;
    else
        {
        iNeedLongtimePress = ETrue;
        iLongtimePressTimeout = aTime;
        }
    }

// ---------------------------------------------------------------------------
// CDragBar::HandleTimerOut
// Handle long press time out event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                
EXPORT_C void CDragBar::HandleTimerOut(TInt aTimeType)
    {
    switch(aTimeType)
        {
        //long press timeout        
        case CLayoutTimer::ELongPressTimer:
            if(PointerDown() && Contains(iCurrentPointerPos))
                {
                //report event
                ReportEvent(EEventButtonLongPress);
                
                //report event first                
                ReportEvent(EEventDraggingStart);                        
                
                iLongPressed = ETrue;
                iIsDragging = ETrue;            
                //need redraw to show that moving starts
                PrepareDragging();
#ifdef __USING_MOVING_FRAME__                
                TRect dirtyRect = DrawFrame(iDragFrameRect);
                ShowMovingIndicator();

                UpdateArea(dirtyRect, EFalse);        
#endif                
                iMoveTimer->SetTimer(iDrawInterval);                        
                }
            break;
        case CLayoutTimer::EDragbarMoveTimer:
            if(iLayoutBeingDraged)
                {                        
                
                static_cast<MPositionObserver*>(RootControl())->
                                            HandlePositionChange(iMoveOffset);
                iMoveOffset = TPoint(0,0);
                }
            else
                {                
                //clear previous frame rect
                TRect rect = DrawFrame(iDragFrameRect,EFalse);          
                TPoint offset = iCurrentPointerPos - iDraggingStartPt;
                iMoveOffset += offset;        
                
                iDragFrameRect.Move(offset);      
                
#ifdef __USING_MOVING_FRAME__                
                TRect dirtyRect = DrawFrame(iDragFrameRect);        
                ShowMovingIndicator();
                rect.BoundingRect(dirtyRect);
#else
                rect = iDragFrameRect;                
#endif                
                iDraggingStartPt = iCurrentPointerPos;
                
                UpdateArea(rect, EFalse);
                
                }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CDragBar::ReCalculateDragArea
// Update moving area. 
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                 
void CDragBar::ReCalculateDragArea()
    {
    iDragFrameRect = Rect();
    for(TInt i = 0; i < iPosObserverList.Count(); i++)
        {
        iDragFrameRect.BoundingRect(
                        iPosObserverList[i]->MovingArea());
        }                                        
    }
// ---------------------------------------------------------------------------
// CDragBar::HandleControlEvent
// Handle event from control which being observed
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                
EXPORT_C void CDragBar::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl,
                                                    const TDesC& /*aEventData*/)
    {
    //only handle control size event change
    switch(aEventType)
        {
        case EEventSizeChanged:
            {
            if(aCtrl && !aCtrl->Hiden())
                {
/*                TInt index = iPosObserverList.Find(aCtrl);
                if(KErrNotFound != index)
                    {
                    //recaculate the bounding rect
                    ReCalculateDragArea();    
                    }              */
                if(aCtrl == this)
                    {
                    iMovingIndicatorPos = Rect().iTl + PenSize();
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// CDragBar::SetBitmapL
// Set bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CDragBar::SetBitmapL(CFbsBitmap* aBitmap, TButtonBmpType aType)
    {
    CFbsBitmap* bmp = aBitmap;
    
    if(!aBitmap)
        return;
    
    if(EBtnBmpMovingIndicator == aType)
        {            
        iMovingIndicatorBmp = bmp;

        iMovingIndicatorPos = Rect().iTl + PenSize();            
        }

    CButtonBase::SetBitmapL(bmp,aType);
    }  

// ---------------------------------------------------------------------------
// CDragBar::ShowMovingIndicator
// Show dragging indicator
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CDragBar::ShowMovingIndicator(TBool aDrawFlag)
    {
    if(iMovingIndicatorBmp && aDrawFlag)
        {
        
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());                     
        
        TSize bmpSize = iMovingIndicatorBmp->SizeInPixels();
        bmpSize -= PenSize();
        TRect bmpRect(TPoint(0,0),bmpSize);
        TPoint bmpPos = iMovingIndicatorPos + iMoveOffset;
        
      
        gc->Activate( MaskBitmapDevice() );
        
        gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc->SetBrushColor( TRgb(KOpaqueColor) );
        TRect rect = bmpRect;
        rect.Move(bmpPos);
        gc->DrawRect(rect);
    
        //mask will not be used if no bmp
        if(iMovingIndicatorMaskBmp) 
            {            
            gc->BitBlt(bmpPos,iMovingIndicatorMaskBmp,bmpRect);            
            }        
        
        gc->Activate( BitmapDevice() );            
        gc->SetBrushColor( KRgbWhite );        
        gc->BitBlt(bmpPos,iMovingIndicatorBmp,bmpRect);       
        }
       
    }

// ---------------------------------------------------------------------------
// CDragBar::DrawFrame
// Draw moving frame
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
TRect CDragBar::DrawFrame(const TRect& aFrameRect, TBool aDrawFlag)
    {   
    return RootControl()->DrawFrame(aFrameRect,aDrawFlag);     
    }



// ---------------------------------------------------------------------------
// CDragBar::SetMaskBitmapL
// Set bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CDragBar::SetMaskBitmapL(CFbsBitmap* aBitmap, TButtonBmpType aType)
    {   
    if(!aBitmap)
        return;
    
    switch(aType)
        {
        case EBtnBmpMovingIndicator:
            iMovingIndicatorMaskBmp = aBitmap;
            break;
        default:
            CButtonBase::SetBitmapL(aBitmap,aType);
            break;
        }
    }  
   
// ---------------------------------------------------------------------------
// CDragBar::IsDraggingComponent
// Test whether this control belongs to a dragbar
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C TBool CDragBar::IsDraggingComponent(CFepUiBaseCtrl* aControl)
    {
    for(TInt i = 0; i < iPosObserverList.Count(); i++)
        {
        if(aControl == iPosObserverList[i])
            return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CDragBar::OnDeActivate
// Response to layout de activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CDragBar::OnDeActivate()
    {
    //simulates a pointer up event to end the draging.
    TRAP_IGNORE(HandlePointerUpEventL(TPoint(0,0)));
    CFepUiBaseCtrl::OnDeActivate();        
    }
    
// ---------------------------------------------------------------------------
// CDragBar::CancelPointerDownL
// Cancel pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CDragBar::CancelPointerDownL()
    {
    HandlePointerUpEventL(TPoint(0,0));
    }
        
//end of CDragBar
