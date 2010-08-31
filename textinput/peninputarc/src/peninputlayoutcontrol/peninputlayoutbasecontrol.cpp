/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for base control
*
*/


#include <AknIconUtils.h>
#include "peninputlayouttimer.h"
#include "peninputlayoutbasecontrol.h"
#include "peninputlayoutbutton.h"
#include "peninputlayouthwrwnd.h"
#include "peninputlayoutcandidatewnd.h"
#include "peninputlayoutrootctrl.h"
#include "peninputlayout.h"
#include <coecntrl.h>


// ============================ MEMBER FUNCTIONS ===============================

// Implementation of Class CFepUiBaseCtrl 

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::CFepUiBaseCtrl
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl::CFepUiBaseCtrl(const TRect& aRect,CFepUiLayout* aUiLayout,
                        TInt aControlId):
                        iPointerLeft(EFalse),
                        iRect(aRect),
                        iShadowRect(TRect(0,0,0,0)),
                        iResourceId(-1),
                        iBorderColor(KRgbBlack),
                        iUpdateFlagEnabled(ETrue),
                        iControlId(aControlId),
                        iControlType( ECtrlBaseCtrl),
                        iBackgroundCol( KRgbWhite),
                        iMaskBkCol(KOpaqueColor),
                        iPenColor(KRgbBlack),
                        iHiden(EFalse),
                        iPointerDown(EFalse),
                        iReady( EFalse),
                        iIsActive(EFalse),
                        iAllowOverlap(EFalse),
                        iUiLayout(aUiLayout),
                        iOwnWndCtrl(ETrue),
                        iOrdinalPos(EOrderNomal)
    {
#ifdef RD_TACTILE_FEEDBACK
    iReservered1 = reinterpret_cast<TInt>(new CFepUiBaseCtrlExtension(ETouchFeedbackNone));
#endif // RD_TACTILE_FEEDBACK    
    iPenSize.SetSize(1,1);        
    iBorderSize.SetSize(KDefaultFrameWidth,KDefaultFrameWidth);
    iValidRegion.AddRect(iRect);
    iClipRegion.AddRect(iRect);
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::CFepUiBaseCtrl
// C++  constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl::CFepUiBaseCtrl(CCoeControl* aControl, CFepUiLayout* aUiLayout,
                                TBool aOwnership)
                  : iPointerLeft(EFalse),
                    //iRect(aRect),
                    iShadowRect(TRect(0,0,0,0)),
                    iWndControl(aControl),
                    iBorderColor(KRgbBlack),                    
                    //iControlId(aControlId),
                    iControlType( ECtrlBaseCtrl|ECtrlWndBasedCtrl),
                    iBackgroundCol( KRgbWhite),
                    iMaskBkCol(KOpaqueColor),
                    iPenColor(KRgbBlack),
                    iHiden(EFalse),
                    iPointerDown(EFalse),
                    iReady( EFalse),
                    iIsActive(EFalse),
                    iAllowOverlap(EFalse),
                    iUiLayout(aUiLayout),
                    iOwnWndCtrl(aOwnership)
    {
#ifdef RD_TACTILE_FEEDBACK
    iReservered1 = reinterpret_cast<TInt>(new CFepUiBaseCtrlExtension(ETouchFeedbackNone));
#endif // RD_TACTILE_FEEDBACK
    iRect = aControl->Rect();
    iControlId = aControl->UniqueHandle();
    iValidRegion.AddRect(iRect);
    iClipRegion.AddRect(iRect);
    }
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::~CFepUiBaseCtrl
// C++  destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl::~CFepUiBaseCtrl()
    {
    iEventObserverList.Close();
    iValidRegion.Close();
    iClipRegion.Close();
    iClipRegionWithoutLockedArea.Clear();
    iClipRegionWithoutLockedArea.Close();
    delete iBackgroundBmp;
    delete iBkMaskBmp;
    //delete [] iEventData;
    for(TInt i = 0; i< 2*KShadowBitmapTotalNum; i++)
        delete iShadowBmp[i];    
        
    if(iOwnWndCtrl)    
        delete iWndControl;
    
#ifdef RD_TACTILE_FEEDBACK     
    //de-register the area for tactile feedback
    //if(aUiLayout) //there must be aUiLayout
    UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(this), Rect());   
	delete reinterpret_cast<CFepUiBaseCtrlExtension*>(iReservered1);       
#endif // RD_TACTILE_FEEDBACK 
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::BaseConstructL
// Handle UI event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiBaseCtrl::BaseConstructL()
    {
    __ASSERT_DEBUG(iUiLayout,User::Leave(EUiLayoutNotReady));    
    
    iRootCtrl = iUiLayout->RootControl();
    iLayoutOwner = iUiLayout->LayoutOwner();
    
    //__ASSERT_DEBUG(iLayoutOwner,User::Leave(EUiLayoutOwnerNotReady));        

    iReady = ETrue;            
    
    for(TInt i = 0; i< 2*KShadowBitmapTotalNum; i++)
        iShadowBmp[i] = NULL;
    __ASSERT_DEBUG(iControlType != ECtrlBaseCtrl,
                                        User::Leave(EUiLayoutBadControlType));
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::BaseConstructL
// Handle UI event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepUiBaseCtrl::BaseConstructL(CFepUiLayoutRootCtrl* aRoot)
    {   
    iRootCtrl = aRoot;
    iLayoutOwner = iUiLayout->LayoutOwner();
    
    __ASSERT_DEBUG(iLayoutOwner,User::Leave(EUiLayoutOwnerNotReady));        
    iReady = ETrue;            
    
    for(TInt i = 0; i< 2*KShadowBitmapTotalNum; i++)
        iShadowBmp[i] = NULL;
    __ASSERT_DEBUG(iControlType != ECtrlBaseCtrl,
                                        User::Leave(EUiLayoutBadControlType));
    }


// ---------------------------------------------------------------------------
// Set tactile feedback type
// Advanced Tactile feedback REQ417-47932
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiBaseCtrl::SetTactileFeedbackType(TInt aTactileType)
	{
#ifdef RD_TACTILE_FEEDBACK
	reinterpret_cast<CFepUiBaseCtrlExtension*>(iReservered1)->SetTactileFeedbackType(aTactileType);
#endif // RD_TACTILE_FEEDBACK 
	}


// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::CapturePointer
// Capture or release the pointer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CFepUiBaseCtrl::CapturePointer(TBool aFlag)
    {
    return RootControl()->CapturePointer(this,aFlag);
    }


// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::Clear
// Clear the whole control area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiBaseCtrl::Clear()
    {
    if(iShadowRect.Size().iWidth > iRect.Size().iWidth ||
            iShadowRect.Size().iHeight > iRect.Size().iHeight)
        ClearRect(iShadowRect);
    else               
        ClearRect(iRect);
    }
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawBackgroundToDevice
// Draw background to a device
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CFepUiBaseCtrl::DrawBackgroundToDevice(const TRect aRect,CFbsDevice *aDevice, 
                                            CFbsBitmap* aBmp, const TRgb& aBrushCol,
                                            const TRgb& aPenCol,TBool aBmpStretchFlag)
	{
	CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
	gc->Activate( aDevice);
	if(aBmp)
		{
		DrawBitmap(aRect,aBmp,aBmpStretchFlag);
		}
	else
		{
        gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc->SetBrushColor( aBrushCol);
        gc->SetPenSize(iBorderSize);
        gc->SetPenColor( aPenCol);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->DrawRect(aRect);
	    }
	
	}

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawBackground
// Draw background
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::DrawBackground(TBool aBmpStretchFlag)
	{
	DrawBackgroundToDevice(iRect,BitmapDevice(), iBackgroundBmp, iBackgroundCol,
	                       iBorderColor, aBmpStretchFlag);
	}

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawBackground
// Draw background
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::DrawBackground(const TRect aRect, TBool /*aBmpStretchFlag*/)
	{
	if( !AbleToDraw() )
	    {
	    return;
	    }
	TRect rtDraw = Rect();
	rtDraw.Intersection( aRect );
	if( rtDraw.IsEmpty() )
	    {
	    return;
	    }
	
	//draw mask    
	CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
	gc->Activate( MaskBitmapDevice() );
	gc->SetClippingRect( rtDraw );
	
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetBrushColor( iMaskBkCol);
    gc->SetPenSize(iBorderSize);
    gc->SetPenColor( iMaskBkCol);
    gc->SetPenStyle(CGraphicsContext::ESolidPen);
    gc->DrawRect(rtDraw);
    //draw foreground
    gc->Activate( BitmapDevice() );
    gc->SetClippingRect( rtDraw );

    if(!BackgroundBmp())
        {
        gc->SetBrushColor( iBackgroundCol );
        gc->SetPenColor( iPenColor );
        gc->SetPenSize(iBorderSize);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->DrawRect( Rect() );
        }
    else
        {
        // ----- draw bitmaps -----        
        if(BkMaskBmp())
            {
            TRect bmpRect(TPoint(0,0),BkMaskBmp()->SizeInPixels());
            gc->DrawBitmapMasked( Rect(), BackgroundBmp(),bmpRect,BkMaskBmp(),EFalse);
            }
        else        
            {
            gc->DrawBitmap( Rect(),BackgroundBmp());
            }
        }    
	}
		
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawMaskBackground
// Draw mask background
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    	
EXPORT_C void CFepUiBaseCtrl::DrawMaskBackground(TBool aBmpStretchFlag)
	{
	DrawBackgroundToDevice(iRect,MaskBitmapDevice(), iBkMaskBmp, iMaskBkCol,
	                       iBorderColor,aBmpStretchFlag);
	}

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawOpaqueMaskBackground
// Draw opaque mask background
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    		
EXPORT_C void CFepUiBaseCtrl::DrawOpaqueMaskBackground(TBool aBmpStretchFlag)
	{
	const TRgb KOpaqueMask = TRgb(KOpaqueColor);
	DrawBackgroundToDevice(iRect,MaskBitmapDevice(), iBkMaskBmp, KOpaqueMask,
	                       KOpaqueMask,aBmpStretchFlag);
	}

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawOpaqueMaskBackground
// Draw opaque mask background
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    		
EXPORT_C void CFepUiBaseCtrl::DrawOpaqueMaskBackground(const TRect aRect, TBool aBmpStretchFlag)
	{
	const TRgb KOpaqueMask = TRgb(KOpaqueColor);
	DrawBackgroundToDevice(aRect, MaskBitmapDevice(), iBkMaskBmp,
	                       KOpaqueMask, KOpaqueMask, aBmpStretchFlag);
	}

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawTransparentMaskBackground
// Draw transparent mask background
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    		
EXPORT_C void CFepUiBaseCtrl::DrawTransparentMaskBackground(const TRect& aRect,
                                                            TBool aBmpStretchFlag)
	{
	const TRgb KOpaqueMask = TRgb(KTransparentColor);
	DrawBackgroundToDevice(aRect, MaskBitmapDevice(), iBkMaskBmp, KTransparentColor,
	                       KTransparentColor, aBmpStretchFlag);
	}
	
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::ClearRect
// Clear a rect in control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C void CFepUiBaseCtrl::ClearRect(const TRect& aRect)    
    {
    if(!AbleToDraw())
        return;
    TRect r = iRect;
    r.BoundingRect(iShadowRect);
    r.Intersection(aRect);
    //only clears the rect belongs to this control
    if(!r.IsEmpty())
        {            
        DrawBackgroundToDevice(r,BitmapDevice(),NULL,KRgbWhite,KRgbWhite,EFalse);
        DrawTransparentMaskBackground(r);        
        return;
        }    
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::BringToTop
// Show the control and brings to top most in current control group
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::BringToTop()
    {
    CControlGroup* grp = RootControl(); 
    if(ParentCtrl()) 
        grp = static_cast<CControlGroup*>(ParentCtrl());
    grp->BringToTopInGroup(this);        
    }
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::BringToBack
// Bring the control to the back
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::BringToBack()
    {
    CControlGroup* grp = RootControl(); 
    if(ParentCtrl())
        grp = static_cast<CControlGroup*>(ParentCtrl());
    grp->BringToBackInGroup(this);        
    }
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::Hide
// Hide the control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::Hide(TBool aFlag)
    {
    if(iHiden == aFlag)
        return;

    iHiden = aFlag;

    if(!iReady)
        return;
    
    if(iHiden)
        {            
        TBool needClear;
        if(!iParentCtrl)
            {
            needClear = ETrue;
            }
        else 
            {
            //no need to clear again if group is alreay hiden. There won't be the case
            // that a group is hiden but sub control is shown.            
            needClear = iParentCtrl->Hiden() ? EFalse : ETrue; 
            }
        if(needClear)
            { 
            /*Hide a control should remove its old image, however in fact, the clear sounds reduntant, as each  
              control inside one group, and the background will be redrawn when ReDrawRect() is called, so in  
              order to ensure performance, comments it now*/
            /*iHiden = EFalse;           
            Clear();
            iHiden = ETrue;
            */
            //must redraw the cleared rect as it may clear overlapped control also.
            iUiLayout->RootControl()->ReDrawRect(Rect());            
            UpdateArea(iRect, EFalse); 
            }
        }
    else
        {
        if(!iParentCtrl || !iParentCtrl->Hiden())        
            {            
            Draw();
            UpdateArea(iRect,EFalse);          
            }
        }
    
    //rect in layout might be changed, update it.
    //RootControl()->UpdateValidRegion(this,aFlag);
    if(iParentCtrl)
        iParentCtrl->UpdateValidRegion(this,aFlag);
    
    //report size chage event
    ReportEvent(EEventSizeChanged);    
    
    
#ifdef RD_TACTILE_FEEDBACK   
    if(IsKindOfControl(ECtrlTactileFeedback))
        {        
        if(aFlag)
            UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect());
        else if (iReady)
            UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect(),TactileFeedbackType());            
        }
#endif    
            
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::SetBackgroundBitmapL
// Set back ground bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C void CFepUiBaseCtrl::SetBackgroundBitmapL(CFbsBitmap* aBmp)
    {
    if(!aBmp)
        return;
/*    
    if(IsKindOfControl(ECtrlButton))
        {
        //special handling to button as button does not use iBackgroundBmp directly.        
        return;
        }
        */
    delete iBackgroundBmp;
    iBackgroundBmp = aBmp;             
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::SetBackgroundMaskBitmapL
// Set back ground mask bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepUiBaseCtrl::SetBackgroundMaskBitmapL(CFbsBitmap* aBmp)
    {
    if(!aBmp)
        return;    
/*    
    if(IsKindOfControl(ECtrlButton))
        {
        //special handling to button as button does not use iBackgroundBmp directly.
        return;
        }*/
    delete iBkMaskBmp;    
    iBkMaskBmp = aBmp;          
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::Contains
// Check whether the point is in control region
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C TBool CFepUiBaseCtrl::Contains(const TPoint& aPt)
    {
    return iRect.Contains(aPt);
    //return iValidRegion.Contains(aPt);
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::SetRect
// Set the control rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::SetRect(const TRect& aRect)
    {
    if(iRect == aRect)
        return;
    TRect oldRect = iRect;
    iRect = aRect;
    iValidRegion.Clear();
    iClipRegion.Clear();
    iValidRegion.AddRect(iRect);
    iClipRegion.AddRect(iRect);
    if(iParentCtrl && iParentCtrl->IsKindOfControl(ECtrlControlGroup))
        {
        iParentCtrl->UpdateValidRegion(this,EFalse);
        //iParentCtrl->UpdateClipRegion(this);
        }
    
    if(iWndControl)
        iWndControl->SetRect(iRect);        
    ReportEvent(EEventSizeChanged);
    
#ifdef RD_TACTILE_FEEDBACK    
    if(IsKindOfControl(ECtrlTactileFeedback) && !Hiden())
        {        
        UiLayout()->ChangeFeedbackArea(reinterpret_cast<TInt>(this),iRect,oldRect);        
        }
#endif        
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::SetShadowRect
// Set the control shadow rect
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiBaseCtrl::SetShadowRect(const TRect& aRect)
    {
    iShadowRect = aRect;
    //calculate the shadow area rect
    TRect shadowAreaRect;
    if(!iRect.IsEmpty())
        {        
        for (TInt i = 0; i < KShadowBitmapTotalNum; i ++)
            {
            switch (i)
                {
                case KShadowBmpTl:
                    iShadowAreaRect[i] = TRect( iShadowRect.iTl, iRect.iTl );
                    break;
                case KShadowBmpTr:
                    iShadowAreaRect[i] = TRect( iRect.iBr.iX, iShadowRect.iTl.iY, 
                                           iShadowRect.iBr.iX, iRect.iTl.iY );
                    break;
                case KShadowBmpBl:
                    iShadowAreaRect[i] = TRect( iShadowRect.iTl.iX, iRect.iBr.iY,
                                           iRect.iTl.iX, iShadowRect.iBr.iY );
                    break;
                case KShadowBmpBr:
                    iShadowAreaRect[i] = TRect( iRect.iBr, iShadowRect.iBr );
                    break;
                case KShadowBmpTm:
                    iShadowAreaRect[i] = TRect( iRect.iTl.iX, iShadowRect.iTl.iY, 
                                           iRect.iBr.iX, iRect.iTl.iY );
                    break;
                case KShadowBmpBm:
                    iShadowAreaRect[i] = TRect( iRect.iTl.iX, iRect.iBr.iY, 
                                            iRect.iBr.iX, iShadowRect.iBr.iY );
                    break;
                case KShadowBmpLm:
                    iShadowAreaRect[i] = TRect( iShadowRect.iTl.iX, iRect.iTl.iY, 
                                            iRect.iTl.iX, iRect.iBr.iY );
                    break;
                case KShadowBmpRm:
                    iShadowAreaRect[i] = TRect( iRect.iBr.iX, iRect.iTl.iY, 
                                            iShadowRect.iBr.iX, iRect.iBr.iY );
                    break;
                default:
                    break;
                }
            }
        }
    else
        {
        //empty the shadow area since we can't calculate it right now.
        for (TInt i = 0; i < KShadowBitmapTotalNum; i ++)
            {
            iShadowAreaRect[i].SetRect(0,0,0,0);
            }
        }
    
    RootControl()->UpdateValidRegion(this,EFalse);
    if(iShadowRect.Size().iWidth > UiLayout()->Rect().Size().iWidth ||
        iShadowRect.Size().iHeight > UiLayout()->Rect().Size().iHeight)
        {
        UiLayout()->LayoutOwner()->LayoutSizeChanged(iShadowRect.Size());
        }    
    }
// CFepUiBaseCtrl::AddEventObserver
// Add a event observer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CFepUiBaseCtrl::AddEventObserver(MEventObserver* aObserver)
    {
    if(!aObserver)
        return;
    //add only when it's not an observer 
    if(KErrNotFound == iEventObserverList.Find(aObserver))
        iEventObserverList.Append(aObserver);
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::RemoveEventObserver
// remove a event observer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CFepUiBaseCtrl::RemoveEventObserver(MEventObserver* aObserver)
    {
    TInt index = iEventObserverList.Find(aObserver);
    if(KErrNotFound != index)
        iEventObserverList.Remove(index);
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::ReportEvent
// Report event to observer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CFepUiBaseCtrl::ReportEvent(TInt aEventType, const TDesC& aEventData)
    {
    for(TInt i = 0; i < iEventObserverList.Count(); i++)    
        {        
        iEventObserverList[i]->HandleControlEvent(aEventType,this,aEventData);
        }
    }

TBool CFepUiBaseCtrl::CompareOrderInSameGroup(CFepUiBaseCtrl* aCtrl)
    {
    return IsOnTopOf(aCtrl);    
    }
        
TBool CFepUiBaseCtrl::CompareOrder(CFepUiBaseCtrl* aCtrl)
    {
    return IsOnTopOf(aCtrl);
    }

TBool CFepUiBaseCtrl::IsOnTopOf(CFepUiBaseCtrl* aCtrl)
    {
    __ASSERT_DEBUG(aCtrl,EUiNullParam);
    
    return OrderPos() < aCtrl->OrderPos();
    }
    
    
TBool CFepUiBaseCtrl::Overlapped(CFepUiBaseCtrl* aCtrl)
    {
    return iRect.Intersects(aCtrl->Rect());     
    }
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::UpdateValidRegion
// Update control's valid region when other control show or hide.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiBaseCtrl::UpdateValidRegion(CFepUiBaseCtrl* /*aCtrl*/,TBool /*aRemoveFlag*/)
    {
    if(!iUpdateFlagEnabled)
        return;
    iValidRegion.Clear();
    iValidRegion.AddRect(iRect);
    return;
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::Move
// Move control and component's rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CFepUiBaseCtrl::Move(const TPoint& aOffset)
    {
    TRect oldRect = iRect;
    iShadowRect.Move(aOffset);
    iRect.Move(aOffset);
    if(iWndControl)
        iWndControl->SetRect(iRect);
    iValidRegion.Clear();
    iValidRegion.AddRect(iRect);    
    if(iParentCtrl && iUpdateFlagEnabled)   
        iParentCtrl->UpdateValidRegion(this,EFalse);        
    for (TInt i = 0; i < KShadowBitmapTotalNum; i ++)    
        {
        iShadowAreaRect[i].Move(aOffset);
        }
      
#ifdef RD_TACTILE_FEEDBACK    
    if(IsKindOfControl(ECtrlTactileFeedback) && !Hiden())
        {        
        UiLayout()->ChangeFeedbackArea(reinterpret_cast<TInt>(this),iRect,oldRect);        
        }        
#endif             
    }    
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::SetActive
// Set control active
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
EXPORT_C  void CFepUiBaseCtrl::SetActive(TBool aFlag)
    {
    iIsActive = aFlag;
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
EXPORT_C CFepUiBaseCtrl* CFepUiBaseCtrl::HandlePointerDownEventL(const TPoint& aPoint)
    {
    iPointerDown = ETrue;
    if(iWndControl)
        {
        TPointerEvent event;
        event.iType = TPointerEvent::EButton1Down;
        event.iPosition = aPoint;
        event.iModifiers = 0;
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
        gc->Activate( BitmapDevice() );
        
        iWndControl->HandlePointerEventL(event);
        }
    return this;    
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerUpEventL
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CFepUiBaseCtrl::HandlePointerUpEventL(
                                                         const TPoint& aPoint)
    {
    iPointerDown = EFalse;
    if(iWndControl)
        {
        TPointerEvent event;
        event.iType = TPointerEvent::EButton1Up;
        event.iPosition = aPoint;
        event.iModifiers = 0;
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
        gc->Activate( BitmapDevice() );
        
        iWndControl->HandlePointerEventL(event);
        Draw();
        }
    
    return this;
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerMoveEventL
// Handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepUiBaseCtrl::HandlePointerMoveEventL(
                                                        const TPoint& aPoint)
    {
    //do nothing if not a CCoeControl
    if(iWndControl)
        {
        TPointerEvent event;
        if(iPointerDown)
            event.iType = TPointerEvent::EDrag; //drag event
        else        
            event.iType = TPointerEvent::EMove; //move event
        event.iPosition = aPoint;
        event.iModifiers = 0;
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
        gc->Activate( BitmapDevice() );

        iWndControl->HandlePointerEventL(event);
        Draw();
        }
    
    return this;
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::CancelPointerDownL
// Cancel pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::CancelPointerDownL()
    {
    if (PointerDown())
        {
        iPointerDown = EFalse;
        iPointerLeft = EFalse;
        //HandlePointerLeave(TPoint(0, 0));
        ReportEvent(EEventButtonDownCanceled);        
        Draw();
        UpdateArea(Rect(), EFalse);        
        }
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePositionChange
// Draw control's frame when control moving
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepUiBaseCtrl::HandlePositionChange(const TPoint& aOffset)
    {
    TRect rect = iRect;

    Move(aOffset);
    rect.BoundingRect(iRect);
    Draw();

    UpdateArea(rect,EFalse);
    
    return EFalse ;//let others handle the event
    }
    
// -----------------------------------------------------------------------------
// CFepUiBaseCtrl::MovingArea
// Return observer's moving area
// -----------------------------------------------------------------------------
//    
EXPORT_C const TRect& CFepUiBaseCtrl::MovingArea()    
    {
    return iRect;
    }

// -----------------------------------------------------------------------------
// CFepUiBaseCtrl::IsFepBaseControl
// -----------------------------------------------------------------------------
//        
EXPORT_C TBool CFepUiBaseCtrl::IsFepBaseControl()
    {
    return ETrue;
    }        
        
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawBitmap
// Draw bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiBaseCtrl::DrawBitmap(const CFbsBitmap* aBitmap,TBool aStretchFlag)
	{
	TRect bmpRect(TPoint(0,0),aBitmap->SizeInPixels());
	CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
	if(aStretchFlag && Rect() != bmpRect)
		{
		gc->DrawBitmap(Rect(),aBitmap);
		}
	else
		{            
         gc->BitBlt(Rect().iTl,aBitmap,bmpRect);
		}
	}
        
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawBitmap
// Draw bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiBaseCtrl::DrawBitmap(const TRect aRect,const CFbsBitmap* aBitmap,
                                         TBool aStretchFlag)
	{
	TRect bmpRect(TPoint(0,0),aBitmap->SizeInPixels());
	CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
	if(aStretchFlag && aRect != bmpRect)
		{
		gc->DrawBitmap(aRect,aBitmap);
		}
	else
		{            
        gc->BitBlt(aRect.iTl,aBitmap,bmpRect);
		}
	}
	        
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::UpdateArea
// Update layout area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C void CFepUiBaseCtrl::UpdateArea(const TRect& aRect,TBool aUpdateFlag)
    {
    if( !Hiden() && ParentCtrl() && (this != UiLayout()->LockareaOwner()) )
        static_cast<CControlGroup*>(ParentCtrl())->UpdateAreaInGrp(this,aRect,aUpdateFlag);
    else
        {
        iUiLayout->UpdateArea(this,aRect,aUpdateFlag);
        }
    }    
    

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::UpdateAreaImmed
// Update layout area immediately
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C void CFepUiBaseCtrl::UpdateAreaImmed(const TRect& aRect,TBool aUpdateFlag)
    {
    if( !Hiden() && ParentCtrl() && (this != UiLayout()->LockareaOwner()) )
        {
        static_cast<CControlGroup*>(ParentCtrl())->UpdateAreaInGrpImmed(this,aRect,aUpdateFlag);
        }
    else
        {
        iUiLayout->UpdateArea(this,aRect,aUpdateFlag,ETrue); 
        }
    }
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::RequireRegionUpdateL
// Register the control to be region change awared
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepUiBaseCtrl::RequireRegionUpdateL(TBool aRequiredFlag)
    {
    iRootCtrl->RequireRegionUpdateL(this,aRequiredFlag);    
    }
    
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepUiBaseCtrl::HandlePointerLeave(const TPoint& /*aPt*/)
    {
    iPointerDown = EFalse;
    iPointerLeft = ETrue;
    //report event
    ReportEvent(EEventPointerLeftControl);
    Draw();
    }
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerEnter
// Handle pointer enter event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepUiBaseCtrl::HandlePointerEnter(const TPoint& aPt)
    {
    iPointerDown = ETrue;
    iPointerLeft = EFalse;
    TRAP_IGNORE(HandlePointerMoveEventL(aPt));
    //report event
    ReportEvent(EEventPointerEnterControl);
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::ReDrawRect
// Re draw control rect
// ---------------------------------------------------------------------------
//                   
EXPORT_C void CFepUiBaseCtrl::ReDrawRect(const TRect& /*aRect*/)
    {
    Draw();
    }
    
    
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawShadowBitmap
// Draw shadow
// ---------------------------------------------------------------------------

void CFepUiBaseCtrl::DrawShadowBitmap(const TRect& aRect,TInt aIndex)
    {
    CFbsBitmap* bmp = iShadowBmp[aIndex];
    
    if(!bmp)
        return;
        
    //shadow bitmap is always opaque
    //mask bitmaps
    CFbsBitmap* maskbmp = iShadowBmp[aIndex + KShadowBitmapTotalNum];

    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    if (maskbmp)
        {           
        gc->Activate( MaskBitmapDevice() );
        
        //clear the rect
        gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc->SetBrushColor( TRgb(KTransparentColor) );//transparency
        gc->SetPenStyle(CGraphicsContext::ENullPen);    
        gc->DrawRect(aRect);
        
        gc->DrawRect( aRect );         
        if ((aRect.Size() != bmp->SizeInPixels()) ||
            (aRect.Size() != maskbmp->SizeInPixels()))
            { 
            AknIconUtils::SetSize(bmp, aRect.Size(), EAspectRatioNotPreserved);
            AknIconUtils::SetSize(maskbmp, aRect.Size(), EAspectRatioNotPreserved);
            }
        gc->DrawBitmap(aRect, maskbmp, maskbmp->SizeInPixels());
        
        gc->Activate(BitmapDevice());
    
        gc->DrawBitmap(aRect, bmp, bmp->SizeInPixels());
        }
    
    else
        {
        gc->Activate( MaskBitmapDevice() );
        
        //clear the rect
        gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc->SetBrushColor( TRgb(KOpaqueColor) );//non transparency
        gc->SetPenStyle(CGraphicsContext::ENullPen);    
        gc->DrawRect(aRect);
        if (aRect.Size() != bmp->SizeInPixels())
            { 
            AknIconUtils::SetSize(bmp, aRect.Size(), EAspectRatioNotPreserved);
            }
            
        gc->Activate(BitmapDevice());
        gc->DrawBitmap(aRect, bmp, bmp->SizeInPixels());
        }
        
    }
        
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::DrawShadow
// Draw shadow
// ---------------------------------------------------------------------------
//                       
EXPORT_C void CFepUiBaseCtrl::DrawShadow()
    {
    for (TInt i = 0; i < KShadowBitmapTotalNum; i ++)
        {
        DrawShadowBitmap(iShadowAreaRect[i], i);
        }    
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::OnDeActivate
// Response to layout de activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiBaseCtrl::OnDeActivate()
    {    
    iReady= EFalse;
    if(iPointerDown)
        {
        TRAP_IGNORE(CancelPointerDownL());
        iPointerDown = EFalse;
        }
    iPointerLeft = EFalse;
    if(iWndControl && !Hiden())
        iWndControl->MakeVisible(EFalse);
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::OnActivate
// Response to layout activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiBaseCtrl::OnActivate()
    {
    //do nothing   
    iReady= ETrue; 
    if(iWndControl && !Hiden())
        iWndControl->MakeVisible(ETrue);
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::OnLayoutDraggingStart
// Response to layout dragging start event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiBaseCtrl::OnLayoutDraggingStart()
    {
    //do nothing        
    }
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::OnLayoutDraggingEnd
// Response to layout dragging end event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiBaseCtrl::OnLayoutDraggingEnd()
    {
    //do nothing        
    }    
    
// ---------------------------------------------------------------------------
// Set shadow area rect
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::SetShadowAreaRect(const TRect& aRect,
                                                    TShadowBitmapIndex aIndex)
    {
    iShadowAreaRect[aIndex] = aRect;
    if(iShadowRect.IsEmpty())
        iShadowRect = aRect;
    else
        iShadowRect.BoundingRect(aRect);
    
    if(iShadowRect.Size().iWidth > UiLayout()->Rect().Size().iWidth ||
        iShadowRect.Size().iHeight > UiLayout()->Rect().Size().iHeight)
        {
        UiLayout()->LayoutOwner()->LayoutSizeChanged(iShadowRect.Size());
        }
    }
    

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::Draw
// Draw control
// ---------------------------------------------------------------------------
//        
EXPORT_C void CFepUiBaseCtrl::Draw()
    {    
    //only draw when it's window based control
    if(iWndControl && AbleToDraw())
        {
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
        
  		DrawOpaqueMaskBackground();              	   		
        gc->Activate( BitmapDevice() );
        //gc->SetClippingRegion(&ClipRegion());        
        iWndControl->DrawNow();
        gc->CancelClipping();        
        UpdateArea(iRect, EFalse);        
        }
    }
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::ConstructFromResourceL
// handle resource change
// ---------------------------------------------------------------------------
//        
EXPORT_C void CFepUiBaseCtrl::ConstructFromResourceL()
    {
    // do nothing
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::SetResourceId
// Set resurce Id
// ---------------------------------------------------------------------------
//        
EXPORT_C void CFepUiBaseCtrl::SetResourceId(TInt32 aResId)
    {
    iResourceId = aResId;
    }
    
// ---------------------------------------------------------------------------
// Set shadow bitmap
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::SetShadowBmp(CFbsBitmap* aBmp,
                               CFbsBitmap* aMaskBmp,TShadowBitmapIndex aIndex)
    {
    //remove previous one
    delete iShadowBmp[aIndex];
    delete iShadowBmp[aIndex+KShadowBitmapTotalNum];
    iShadowBmp[aIndex] = aBmp;
    iShadowBmp[aIndex+KShadowBitmapTotalNum] = aMaskBmp;
    }

EXPORT_C TBool CFepUiBaseCtrl::AbleToDraw()
	{
    return UiLayout()->LayoutReady() && Ready() && !WholeHiden() && Rect().Size() != TSize(0,0);
	}

// ---------------------------------------------------------------------------
// Get extension
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CFepUiBaseCtrl::Extension_(TUint aExtensionId, TAny *&a0, TAny *a1)
    {
    //not implemented, use CBase's
    return CBase::Extension_(aExtensionId, a0, a1);
    }
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::WholeHiden
// Return if hiden all controls
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CFepUiBaseCtrl::WholeHiden()
    {
    if ( Hiden() )
        {
        return ETrue;
        }
        
    CFepUiBaseCtrl* parentCtrl = ParentCtrl();
    while( parentCtrl )
        {
        if ( parentCtrl->Hiden() )
            {
            return ETrue;
            }
        parentCtrl = parentCtrl->ParentCtrl();
        }
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::ValidClipRegion
// Get the clipping region
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C const TRegion& CFepUiBaseCtrl::ValidClipRegion()
    {
    iClipRegionWithoutLockedArea.Clear();
    iClipRegionWithoutLockedArea.Copy(iClipRegion);
    /***********************************************************************/
    /*!!!! Seems a problem here, what if this area is locked by myself?    */
    /***********************************************************************/
    iClipRegionWithoutLockedArea.SubRect(UiLayout()->LockedArea());
    return iClipRegionWithoutLockedArea;
    }

// -----------------------------------------------------------------------------
// CFepUiBaseCtrl::OnResourceChange
// Handle system resource change 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
EXPORT_C void CFepUiBaseCtrl::HandleResourceChange(TInt aType)
    {
    if(iWndControl)
        iWndControl->HandleResourceChange(aType);
    }
// -----------------------------------------------------------------------------
// CFepUiBaseCtrl::GraphicDeviceSizeChanged
// Handle pen UI graphic device size change event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
EXPORT_C void CFepUiBaseCtrl::GraphicDeviceSizeChanged()
    {
    //do nothing
    }
							  
							  
// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::SimulatePointerEvent
// Simulates an internal pointer event
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepUiBaseCtrl::SimulateRawEvent(const TRawEvent& aEvent)
    {
    TRAP_IGNORE(RootControl()->HandleEventL(ERawEvent,&aEvent));
    }


EXPORT_C void CFepUiBaseCtrl::SetParent(CFepUiBaseCtrl* aParent)
    {
    //parent control must be type of control group.
    if(aParent)
        {
        __ASSERT_DEBUG(aParent->IsKindOfControl(ECtrlControlGroup),EUiLayoutBadParentType);
        }
    iParentCtrl = aParent;
    }

EXPORT_C void CFepUiBaseCtrl::SetOrdinalPos(TInt aPos)
    {
    iOrdinalPos = aPos;
    }
    
EXPORT_C TInt CFepUiBaseCtrl::AbsOrderPos()
    {
    TInt order = OrderPos();
    if(ParentCtrl())
        order += ParentCtrl()->AbsOrderPos();
    return order;    
    }



// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::CFepUiBaseCtrlExtension
// 
// ---------------------------------------------------------------------------
//

CFepUiBaseCtrl::CFepUiBaseCtrlExtension::CFepUiBaseCtrlExtension(TInt aTactileType)
	{
#ifdef RD_TACTILE_FEEDBACK
	iTactileType = aTactileType;
#endif // RD_TACTILE_FEEDBACK 
	}

void CFepUiBaseCtrl::CFepUiBaseCtrlExtension::SetTactileFeedbackType(TInt aTactileType)
	{
#ifdef RD_TACTILE_FEEDBACK
	iTactileType = aTactileType;
#endif // RD_TACTILE_FEEDBACK 
	}

EXPORT_C TInt CFepUiBaseCtrl::CFepUiBaseCtrlExtension::TactileFeedbackType()
	{
#ifdef RD_TACTILE_FEEDBACK
	return iTactileType;
#else
	return 0;
#endif // RD_TACTILE_FEEDBACK 
	}

