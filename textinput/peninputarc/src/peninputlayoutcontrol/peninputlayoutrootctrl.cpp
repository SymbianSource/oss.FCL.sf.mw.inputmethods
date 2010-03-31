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
* Description:  Implementation for CFepUiLayoutRootCtrl
*
*/


#include "peninputlayoutrootctrl.h"
#include "peninputlayouthwrwnd.h"
#include "peninputlayoutbutton.h"
#include "peninputlayouteditareabase.h"
#include "peninputlayout.h"
#include "peninputlayoutcursor.h"
#include <peninputcmd.h>
#include <AknsConstants.h>
#include <AknsUtils.h>
const TInt KRootControlId = -1;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::NewL
// class factory function
// ---------------------------------------------------------------------------
//
CFepUiLayoutRootCtrl* CFepUiLayoutRootCtrl::NewL(CFepUiLayout* aUiLayout)
    {
    CFepUiLayoutRootCtrl* ctrl = new(ELeave) CFepUiLayoutRootCtrl(aUiLayout);
    CleanupStack::PushL(ctrl);
    ctrl->ConstructL();
    CleanupStack::Pop(ctrl);
    return ctrl;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::ConstructL
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CFepUiLayoutRootCtrl::ConstructL()
    {
    BaseConstructL(this);
    //set it not ready to avoid unnessary drawing. Ready is set afer InitControl
    SetReady(EFalse);
    }    
    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::~CFepUiLayoutRootCtrl
// Destructor
// ---------------------------------------------------------------------------
//
CFepUiLayoutRootCtrl::~CFepUiLayoutRootCtrl()
    {    
    //CFepUiBaseCtrl* ctrl;
    //free all control. iCtrlList keeps all controls for root control.
    /*for(TInt i = iCtrlList.Count()-1; i >= 0;i--)
        {
        ctrl = iCtrlList[i];
        delete ctrl;
        }
    iCtrlList.Reset();*/
    //No need to close others, CControlGroup will do that.    
    iCtrlRegionAwareList.Close();
    iCtrlOverlappingAwareList.Close();
    //iNonHwrStartPtRegion.Close();        
    delete iWindowGc;
    delete iCursor;
    }        
    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::CFepUiLayoutRootCtrl
// Constructor
// ---------------------------------------------------------------------------
//
CFepUiLayoutRootCtrl::CFepUiLayoutRootCtrl(CFepUiLayout* aUiLayout):
                            CControlGroup(aUiLayout,KRootControlId)
    {    
    SetControlType(ECtrlWndBasedCtrl|ControlType());
    SetMaskBkCol(KTransparentColor);
    }    
    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::HandleEventL
// handle event from window server
// ---------------------------------------------------------------------------
//
TBool CFepUiLayoutRootCtrl::HandleEventL(TEventType aType, const TAny* aData)
    {   

    CFepUiBaseCtrl* ctrl = NULL;    

    switch(aType)
        {
        case ERawEvent:
            {
            TRawEvent event=*((TRawEvent*)aData);
            switch(event.Type())                
                {
                case TRawEvent::EPointerMove:
                    {                        
                    TPoint point = event.Pos() - UiLayout()->Position (); 
                    
                    //if captured
                    if(iPointerCaptureCtrl)
                        {                            
                        ctrl = iPointerCaptureCtrl->HandlePointerMoveEventL(point);
                        break;
                        }
                        
                    //won't handle move event if no pointer down happens
                    if(!iCtrlWithPointerDown)
                        return EFalse;
                    
                    ctrl = HandlePointerMoveEventL(point);
                    }
                    break;
                
                case TRawEvent::EButton1Down:
                    {                        
                    
                    TPoint point1 = event.Pos() - UiLayout()->Position ();
                    //if captured
                    if(iPointerCaptureCtrl)
                        {                            
                        ctrl = iPointerCaptureCtrl->HandlePointerDownEventL(point1);
                        break;
                        }
                    
                    ctrl = HandlePointerDownEventL(point1);
                    }
                    break;     
                    
                case TRawEvent::EButton1Up:
                    {
                    
                    TPoint point = event.Pos() - UiLayout()->Position ();
                    if(iPointerCaptureCtrl)
                        {                            
                        ctrl = iPointerCaptureCtrl->HandlePointerUpEventL(point);
                        }
                    else                        
                    ctrl = HandlePointerUpEventL(point);
                    iCtrlWithPointerDown = NULL;
                    }
                    break;
                    
                default:
                    break;                    
                }                
            }        
            break;
            
        case EPointerEvent:
            {
            TPointerEvent pt= *((TPointerEvent*)aData);
            TPoint point = pt.iPosition;
            
            //same as raw event
            if(TPointerEvent::EButton1Down == pt.iType)
                {                
                if(iPointerCaptureCtrl)
                    {                            
                    ctrl = iPointerCaptureCtrl->HandlePointerDownEventL(point);
                    break;
                    }
                
                ctrl = HandlePointerDownEventL(point);                
                }
            else
                {                
                if(TPointerEvent::EButton1Up == pt.iType)
                    {
                    if(iPointerCaptureCtrl)
                        {                            
                        ctrl = iPointerCaptureCtrl->HandlePointerUpEventL(point);
                        iCtrlWithPointerDown = NULL;
                        break;
                        }                        
                    ctrl = HandlePointerUpEventL(point);                    
                    }
                else
                    {
                    if(TPointerEvent::EDrag == pt.iType ||
                            TPointerEvent::EMove == pt.iType)
                        {                        
                        //if captured
                        if(iPointerCaptureCtrl)
                            {                            
                            ctrl = iPointerCaptureCtrl->HandlePointerMoveEventL(point);
                            break;
                            }
                            
                        //won't handle move event if no pointer down happens
                        if(!iCtrlWithPointerDown)
                            return NULL;
                        
                        ctrl = HandlePointerMoveEventL(point);
                        }
                    }                    
                }                         
            
            
            }
            break;
                
        default:
            break;
        }         
        
    if(iPointerDownCanceled)
        {    
        iCtrlWithPointerDown = NULL;
        iPointerLeft = EFalse;
        }    
    return ctrl ? ETrue : EFalse;
        
    }
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::SetPointerDownCtrl
// Set pointer down control. 
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
void CFepUiLayoutRootCtrl::SetPointerDownCtrlL(CFepUiBaseCtrl* aCtrl)
    {    
    //set pointer down canceling flag
    iPointerDownCanceled = EFalse;
    if(!aCtrl) // Pointer down and up in different control
        {        
        if(iCtrlWithPointerDown) 
            {
            //cancel the pointer down event for the control which has pen down operation
            iCtrlWithPointerDown->CancelPointerDownL();
            iPointerDownCanceled = ETrue;            
            }        
        }
    else
        iCtrlWithPointerDown = aCtrl;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::RequireRegionUpdateL
// Register the control to be region change awared
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepUiLayoutRootCtrl::RequireRegionUpdateL(CFepUiBaseCtrl* aCtrl,
                                             TBool aRequiredFlag)
    {
    TInt ctrlIdx = iCtrlRegionAwareList.Find(aCtrl);
    if(aRequiredFlag)
        {
        if(KErrNotFound != ctrlIdx)
            return; 	  //already there
        
        /*for(i =0; i < iCtrlRegionAwareList.Count(); i++)
            {
            if(aCtrl == iCtrlRegionAwareList[i])
                return;
            }*/
        iCtrlRegionAwareList.AppendL(aCtrl);
        if(aCtrl->Ready())
            {
            //calculate the region for the first time
            aCtrl->UpdateValidRegion(NULL,EFalse); 
            }
        //otherwise do this during layout initialization
        }        
    else
        {
        if(KErrNotFound == ctrlIdx)
            return; 
        
        iCtrlRegionAwareList.Remove(ctrlIdx);
        /*
        for(i =0; i < iCtrlRegionAwareList.Count(); i++)
            {
            if(aCtrl == iCtrlRegionAwareList[i])
                {
                iCtrlRegionAwareList.Remove(i);
                //TRect rect = aCtrl->Rect();
                //aCtrl->SetRect(rect); //restore the region
                }                
            }*/
        }
    }    
    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::DrawRect
// Draw layout content within a rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepUiLayoutRootCtrl::DrawRect(const TRect& aRect,TBool aDrawFrameFlag)
    {
    const TSize KFramePenSize(2,2);
    
    if(aDrawFrameFlag) //only draw frame
        {        
           
        DrawOpaqueMaskBackground();
        
        DrawBackground();
        }
    else
        {
        CControlGroup::ReDrawRect(aRect);
        }
    
    

            
    }
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::RemoveControl
// Remove a control to layout
// ---------------------------------------------------------------------------
//
void CFepUiLayoutRootCtrl::RemoveControl(CFepUiBaseCtrl* aControl)
    {
    if(!aControl)
        return;
    /*
    TBool bFound = EFalse;
    if(aControl == iActiveCtrl)
        iActiveCtrl = NULL;
    for(TInt i = 0; i < iCtrlList.Count(); i++)
        {
        if(iCtrlList[i] == aControl)
            {
            bFound = ETrue;
            iCtrlList.Remove(i);            
            DoRegionUpdateForCtrls();                
            delete aControl;
            }
        }
    if(!bFound) //not found, the control must member of one control group
        {
        for(TInt i = 0; i < iCtrlList.Count(); i++)
            {
            if(iCtrlList[i]->IsKindOfControl(ECtrlControlGroup))
                {                
                static_cast<CControlGroup*>(iCtrlList[i])->
                                                RemoveControl(aControl);
                }        
            }
        }*/
    aControl->RemoveEventObserver(UiLayout());
    CControlGroup::RemoveControl(aControl);
    
    //DoRegionUpdateForCtrls();
        }


// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::RemoveControl
// Remove a control to layout
// ---------------------------------------------------------------------------
//
void CFepUiLayoutRootCtrl::RemoveControl(CCoeControl* aControl)
    {
    CFepUiBaseCtrl* foundedCtrl = Find(aControl);    
    RemoveControl(foundedCtrl);    
    }


// -----------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::SetRect
// Set layout rect
// -----------------------------------------------------------------------------
//
void CFepUiLayoutRootCtrl::SetRect(const TRect& aRect)
    {
    iRect = aRect;

    DoRegionUpdateForCtrls();
/*    for(TInt i =0; i < iCtrlRegionAwareList.Count(); i++)
        {
        //re-calculate the valid region
        iCtrlRegionAwareList[i]->UpdateValidRegion(NULL,EFalse);
        }    */
    }


// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::UpdateValidRegion
// Update control's valid region after one control Hiden or shown.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
void CFepUiLayoutRootCtrl::UpdateValidRegion(CFepUiBaseCtrl* aCtrl,
                                                             TBool aRemoveFlag)
    {
    //CControlGroup::UpdateValidRegion(aCtrl,aRemoveFlag);
    //if(!aCtrl)
    if(Ready())
        {
        //ask all region aware control to update the region
        for(TInt i =0; i < iCtrlRegionAwareList.Count(); i++)
            {
            //re-calculate the valid region
            iCtrlRegionAwareList[i]->UpdateValidRegion(NULL,EFalse);
            }            
        }
        
     //only check shadow rect when aCtrl is not NULL
    if(aCtrl)
        {
        for(TInt i = 0; i < iCtrlOverlappingAwareList.Count(); ++i)
            {            
            iCtrlOverlappingAwareList[i]->UpdateValidRegion(aCtrl,aRemoveFlag);
            }
        //pop up window will be handled dynamically
        if(!aCtrl->IsKindOfControl(ECtrlPopupWindow) && 
                                        !aCtrl->ShadowRect().IsEmpty())
            {
            TSize s(iShadowRect.Size());
            if(aCtrl->ShadowRect().Width() > s.iWidth)
                s.iWidth = aCtrl->ShadowRect().Width();
            
            if(aCtrl->ShadowRect().Height() > s.iHeight)
                s.iHeight = aCtrl->ShadowRect().Height();
            
            if(s != iShadowRect.Size())
                {
                UiLayout()->LayoutOwner()->LayoutSizeChanged(s);
                }                    
            }
            
        //update iNonHwrStartPtRegion for full screen hwr  
        if(!aCtrl->IsKindOfControl(ECtrlTransparentHwrWnd))
            {            
            CalculateNonHwrStartPtArea();  
            }
        } 
    else
        {
        CalculateNonHwrStartPtArea();        
        }
    CalculateClipRegion();
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::InitControl
// Initialize the layout
// ---------------------------------------------------------------------------
//
TRect CFepUiLayoutRootCtrl::InitControl()
    {
    TInt i=0;
    TInt count = iCtrlList.Count();
    CFepUiBaseCtrl* ctrl;
    
    //check whether there is pop up window, move it to last
    //if pop up window is encapsulated by some control group, it can't be found here.
    for(i = 0; i < count; i++)
        {        
        if(iCtrlList[i]->IsKindOfControl(ECtrlPopupWindow))
            {
            ctrl = iCtrlList[i];
            iCtrlList.Remove(i);
            iCtrlList.Append(ctrl);
            i--;
            count--;//The last one is pop wnd, no need to check.            
            }
        }

    //check and calc the shadow rect
    TBool bHasShadow = EFalse;
    if(iShadowRect.IsEmpty())
        {
        
        for(i = 0; i < iCtrlList.Count(); i++)
            {
            //pop up window doesn't affect our layout rect, ignore it.
            if(!iCtrlList[i]->IsKindOfControl(ECtrlPopupWindow))
                {                
                iShadowRect = iCtrlList[i]->ShadowRect();            
                bHasShadow = ETrue;
                break;
                }
            }
        }
    
    bHasShadow = !iShadowRect.IsEmpty();
    
    TSize s(iRect.Size());
            
    if(bHasShadow)
        {
        for(i = 0; i < iCtrlList.Count(); i++)
            {
            if(!iCtrlList[i]->IsKindOfControl(ECtrlPopupWindow) &&
                !iCtrlList[i]->ShadowRect().IsEmpty())
                {
                iShadowRect.BoundingRect(iCtrlList[i]->ShadowRect());
                }
            }

        if(iRect.Width() < iShadowRect.Width())
            s.iWidth = iShadowRect.Width();
        
        if(iRect.Height() < iShadowRect.Height())
            s.iHeight = iShadowRect.Height();  
        iRect.SetRect(TPoint(0,0),s);

        iLayoutPos = iShadowRect.iTl;
        //move iShadowAreaRect if needed
        
        //iShadowRect need be kept as SetShadowAreaRect will change iShadowRect
        if(ShadowAreaRect(KShadowBmpTl).iTl != TPoint(0,0))
            {
            TPoint offset = -ShadowAreaRect(KShadowBmpTl).iTl;
            for (TInt i = 0; i < KShadowBitmapTotalNum; i ++)
                {
                iShadowAreaRect[i].Move(offset);
                }
            }
        //The layout shadow rect starts from TPoint(0,0);        
        iShadowRect.Move(-iLayoutPos);

        s = iShadowRect.Size();
        }
    else
        {
        iLayoutPos = iRect.iTl;
        iRect.Move(-iLayoutPos);              
        }
    
    iNonHwrStartPtRegion.Clear();
    /*
    iNonHwrStartPtRegion.AddRect(iRect);
    
    //find HWR window and remove it from the region
    CFepUiBaseCtrl* hwrWnd = ControlForKindOfType(ECtrlTransparentHwrWnd);
    TInt index = 0;
    while(hwrWnd)
        {
        iNonHwrStartPtRegion.SubRegion(hwrWnd->Region());
        //find next hwr window
        hwrWnd = ControlForKindOfType(ECtrlTransparentHwrWnd,++index);
        }
      */     
      
    CalculateNonHwrStartPtArea();
    const TRegion& t = RootControl()->NonHwrStartingPtRegion();
    TInt n = t.Count();
    const TRect* r =t.RectangleList();
    
    RegisterICFEditor();
    //check each control group, whether there is background control, 
    //adjust pos if necessary
    AdjustBkCtrlPos();
    //now it's ready to be drawn
    SetReady(ETrue);    

    UpdateValidRegion(NULL,EFalse);
    return TRect(iLayoutPos,s);    
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::CapturePointer
// Capture pointer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CFepUiLayoutRootCtrl::CapturePointer(CFepUiBaseCtrl* aCtrl,
                                                                  TBool aFlag)
    {
    CFepUiBaseCtrl* ctrl = iPointerCaptureCtrl;
    if(aFlag)
        {
        if( iPointerCaptureCtrl && aCtrl && 
            iPointerCaptureCtrl->ControlId() == aCtrl->ControlId() )
            {
            return ctrl;
            }
            
        iPointerCaptureCtrl = aCtrl;    
        
        if(iCtrlWithPointerDown && aCtrl != NULL && !aCtrl->PointerDown())
            {
            //if someone else has pointer down, we must cancel the down event.
            TRAP_IGNORE(iCtrlWithPointerDown->CancelPointerDownL());
            iCtrlWithPointerDown = NULL;            
            }
        }
    else
        {
        if(!iPointerCaptureCtrl && !aFlag)
            return NULL;
        if (aCtrl == iPointerCaptureCtrl)
            {
            iPointerCaptureCtrl = NULL;    
            }
        }
    //signal the owner that pointer needs be captured
    TInt  captureCtrl = (TInt)(iPointerCaptureCtrl);

    TBuf<8> buf;
    buf.Append((const TUint16 *)(&aFlag), sizeof(TBool)/sizeof(TUint16));
    buf.Append((const TUint16 *)(&captureCtrl), sizeof(TInt)/sizeof(TUint16));
    UiLayout()->SignalOwner(ESignalCapturePointer,buf);    
    
    
    return ctrl;
    }


// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::HandleControlEvent
// Handle control event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//         
void CFepUiLayoutRootCtrl::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl,
                                                       const TDesC& /*aEventData*/)
    {    
    CFepUiBaseCtrl* ctrl;    
    switch(aEventType)
        {
        case EEventDraggingStart:
            {
            for(TInt i = 0; i < iCtrlList.Count(); i++)
                {
                iCtrlList[i]->OnLayoutDraggingStart();
                }
            }
            break;
        case EEventDraggingEnd:
            {
            for(TInt i = 0; i < iCtrlList.Count(); i++)
                {
                iCtrlList[i]->OnLayoutDraggingEnd();
                }            
            }
            break;
        case EEventRegionUpdated:
            {
                //now find the hwr wwindow
            for(TInt i = 0; i < iCtrlList.Count(); i++)
                {
                ctrl = iCtrlList[i];
                if(ctrl->IsKindOfControl(ECtrlHwrWnd))
                    {
                    static_cast<CHwrWndBase*>(ctrl)->UpdateRegion(aCtrl);
                    }                    
                }                        
            }
            break;
        
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::HandlePositionChange
// Handle layout position change
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CFepUiLayoutRootCtrl::HandlePositionChange(const TPoint& aOffset)
    {
    const TPoint KZeroOffset(0,0);
    if(aOffset != KZeroOffset)
        {        
        TPoint pos = UiLayout()->Position() + aOffset;
        UiLayout()->SetLayoutPos(pos);
        }
    return ETrue; //if layout process the event, the no control will process it.
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::GetInputContextFieldMaxLen
// Get the max editor length of the input context field
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CFepUiLayoutRootCtrl::GetInputContextFieldMaxLen()
    {
    TInt len = 0;
    //find the input context area control. At any time there is only one editor 
    //area control in the layout.
    CFepUiBaseCtrl* ctrl = ControlForKindOfType(ECtrlEditAreaBase);
    if(ctrl)
        {        
        len = static_cast<CFepLayoutEditAreaBase*>(ctrl)->
                                                     GetEditorFieldMaxLen();
        }
    
    return len;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::IsValidDestination
// Test whether the rect conflicts with other controls
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
TBool CFepUiLayoutRootCtrl::IsValidDestination(const TRect& aRect,
                                               CDragBar* aDragBar,
                                               TBool& aVInfo,
                                               TBool& aHInfo)
    {    
    aVInfo = EFalse;
    aHInfo = EFalse;
    if(!Rect().Contains(aRect.iTl) || !Rect().Contains(aRect.iBr)) //outside of the ui layout
        return EFalse;   
     
    CFepUiBaseCtrl* ctrl;

    for(TInt i = iCtrlList.Count()-1; i >= 0;i--)
        {
        ctrl = iCtrlList[i];            
        TBool bIsDragingComponent = EFalse;
        if(aDragBar)
            {
            if(aDragBar->IsDraggingComponent(ctrl))
                bIsDragingComponent = ETrue;
            }
  
        
        if(!bIsDragingComponent && aRect.Intersects(ctrl->Rect()))   //conflicts with others
            {
            //is the control one of the dragbar component.
           
            if(!ctrl->AllowOverlap()) 
                {                    
                //if the control not allow overlapping, then it's invalid pos
                return EFalse;
                }
                
            }
        }
    
    return ETrue;
}    
// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
CFepUiBaseCtrl* CFepUiLayoutRootCtrl::HandlePointerDownEventL(
                                                    const TPoint& aPoint)
    {
    return CControlGroup::HandlePointerDownEventL(aPoint);
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::HandlePointerUpEventL
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
CFepUiBaseCtrl* CFepUiLayoutRootCtrl::HandlePointerUpEventL(
                                                     const TPoint& aPoint)
    {
    CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerUpEventL(aPoint);
    if(!ctrl && iCtrlWithPointerDown)
        {                            
        iCtrlWithPointerDown->CancelPointerDownL();
        iCtrlWithPointerDown = NULL;
        }                        

    return ctrl;
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::HandlePointerMoveEventL
// Handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CFepUiLayoutRootCtrl::HandlePointerMoveEventL(
                                                    const TPoint& aPoint)
    {
    CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerMoveEventL(aPoint);
    //Moving outside of layout
    if(!ctrl && iCtrlWithPointerDown && !iPointerLeft)
        {                            
        iCtrlWithPointerDown->HandlePointerLeave(aPoint);
        iPointerLeft = ETrue;
        }
    // Moving back from outside of layout to current control
    if(ctrl == iCtrlWithPointerDown && iPointerLeft && iCtrlWithPointerDown)
        {
        iCtrlWithPointerDown->HandlePointerEnter(aPoint);
        iPointerLeft = EFalse;                            
        }                        
    
    return ctrl;
    }

// -----------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::Draw
// Draw the control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CFepUiLayoutRootCtrl::Draw()
    {

	if(!AbleToDraw())
        return;
	
    CControlGroup::Draw();

    if(!iShadowRect.IsEmpty())        
        {
        iShadowShown = ETrue;
        DrawShadow();        
        }
    
    UpdateArea(iRect, EFalse);    
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::DrawFrame
// Draw moving frame
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
TRect CFepUiLayoutRootCtrl::DrawFrame(const TRect& aFrameRect, TBool aDrawFlag)
    {   
    
    //clear shadow when moving
    TBool updateShadowArea = EFalse;
    TRect dirtyRect;
    if(iShadowShown)
        {
        ClearRect(iShadowRect);
        iShadowShown = EFalse;
        updateShadowArea = ETrue;
        }
    
   
    TRect bmpRect;
    TPoint bmpPos;
    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());       

    //draw mask background  
    TRgb maskPenCol = aDrawFlag ? TRgb(KOpaqueColor) : TRgb(KTransparentColor);
  	DrawBackgroundToDevice(aFrameRect,MaskBitmapDevice(), 0, 
    				//TRgb(KTransparentColor), TRgb(KOpaqueColor),EFalse);
    				TRgb(KTransparentColor), maskPenCol,EFalse);

	//draw background
	TRgb penCor = aDrawFlag ? KRgbBlack : KRgbWhite;
	TAknsQsnOtherColorsIndex clrIndex;
    clrIndex = EAknsCIQsnOtherColorsCG9;

    if ( AknsUtils::AvkonSkinEnabled() )
        {
        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                               penCor, KAknsIIDQsnTextColors, clrIndex );
        }

  	DrawBackgroundToDevice(aFrameRect,BitmapDevice(),0,TRgb(KRgbWhite),penCor,EFalse);
	    
    gc->RectDrawnTo(dirtyRect);        
    
    if(updateShadowArea)
        return iShadowRect;
    else
        return dirtyRect;
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::OnDeActivate
// Response to layout de activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiLayoutRootCtrl::OnDeActivate()
    {
    if ( iPointerCaptureCtrl )
        {
        if(iPointerCaptureCtrl->PointerDown())
            {
            TRAP_IGNORE(iPointerCaptureCtrl->CancelPointerDownL());
            }
        
        //CFepLayoutMultiLineIcf::CancelPointerDownL may set iPointerCaptureCtrl to be NULL
        if ( iPointerCaptureCtrl )
        	{
        	iPointerCaptureCtrl->CapturePointer(EFalse);
            iPointerCaptureCtrl = NULL;
        	}
        }
    iPointerDownCanceled = EFalse;
    CControlGroup::OnDeActivate();
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::OnActivate
// Response to layout activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiLayoutRootCtrl::OnActivate()
    {
    CControlGroup::OnActivate();    
    }
void CFepUiLayoutRootCtrl::DoRegionUpdateForCtrls()
    {
    CFepUiBaseCtrl* ctrl;    
    for(TInt i = 0; i< iCtrlRegionAwareList.Count(); i++)
        {
        ctrl = iCtrlRegionAwareList[i];
        if(ctrl->Ready())
            ctrl->UpdateValidRegion(NULL,EFalse);
        }
    
    }

void CFepUiLayoutRootCtrl::RegisterICFEditor()
    {
    TInt index = 0;        
    while(ETrue)
        {
        CFepUiBaseCtrl* ctrl = ControlForKindOfType(ECtrlEditAreaBase,index++);        
        if(!ctrl)
            break;
        iCtrlOverlappingAwareList.Append(ctrl);        
        }
    }

CWindowToBitmapMappingGc* CFepUiLayoutRootCtrl::GetRWindowGcL()
    {
    if(!iWindowGc)
        {        
        CWsScreenDevice *screenDevice = CCoeEnv::Static()->ScreenDevice();
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(UiLayout()->LayoutOwner()->BitmapContext());
        iWindowGc = CWindowToBitmapMappingGc::NewL(*screenDevice,*gc);
        }
    return iWindowGc;
    }
    
// -----------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::OnResourceChange
// Handle system resource change 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
void CFepUiLayoutRootCtrl::HandleResourceChange(TInt aType)
    {
    CControlGroup::HandleResourceChange(aType);
    }
    
// -----------------------------------------------------------------------------
// CFepUiLayoutRootCtrl::GraphicDeviceSizeChanged
// Handle graphic device size change event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
void CFepUiLayoutRootCtrl::GraphicDeviceSizeChanged()
    {
    CControlGroup::GraphicDeviceSizeChanged();
    }

CFepUiCursor* CFepUiLayoutRootCtrl::CreateCursor()
    {
    CFepUiCursor* cursor = NULL;
    TRAP_IGNORE(cursor = CreateCursorL());
    return cursor;
    }

CFepUiCursor* CFepUiLayoutRootCtrl::CreateCursorL()
    {
    if(!iCursor)
        {              
        const TInt KCursorId = -1;
        iCursor = CFepUiCursor::NewL(TRect(0,0,0,0),UiLayout(),KCursorId);
        }
    
    return iCursor;    
    }

void CFepUiLayoutRootCtrl::InsertToPopList(CFepUiBaseCtrl* aCtrl)
    {
    TInt idx = iPopCtrlList.Find(aCtrl);
    if(idx == KErrNotFound)
        {
        iPopCtrlList.Insert(aCtrl,0);
        idx = 0;
        }
    //move to desired position
    
    for(TInt i = idx + 1; i < iPopCtrlList.Count(); ++i)
        {
        if(aCtrl->OrderPos() <= iPopCtrlList[i]->OrderPos())
            {
            iPopCtrlList[i - 1] = iPopCtrlList[i];
            }
        else
            {
            iPopCtrlList[i - 1] = aCtrl;
            break;
            }
        }
    }

TBool CFepUiLayoutRootCtrl::ClipCursorForCtrlNeeded(CFepUiBaseCtrl* aCtrl,
													const TBool aBringToTopFlag,
                                                    TInt& aCursorIndex)
    {
    TBool bNeedClip = EFalse;
	TBool bNeedClipBack = EFalse;

    if(aCtrl->IsOnTopOf(iCursor))
        {
        if(aBringToTopFlag)
            {
            bNeedClip = ETrue;
            }
        else
            {
            bNeedClipBack = ETrue;
            }
        }
    else
        {
        //otherwise, we need to find the index in the same control group.
        CFepUiBaseCtrl* cursor = iCursor->iEditor;
        CFepUiBaseCtrl* overlapCtrl = aCtrl;
        CControlGroup* cursorParent = static_cast<CControlGroup*>(cursor->ParentCtrl());
        CControlGroup* overlapCtrlParent = static_cast<CControlGroup*>(overlapCtrl->ParentCtrl());
        
        if(cursorParent != overlapCtrlParent)
            {
            //find out the nest level counting starts from root control
            TInt cursorLev = cursorParent->GetNestedLevel();
            //find out the nest level counting starts from root control 
            TInt overlapCtrlLev = overlapCtrlParent->GetNestedLevel(); 
            
            if(cursorLev > overlapCtrlLev)
                {
                for(TInt i = 0 ; i < cursorLev - overlapCtrlLev; ++i)
                    {
                    cursor = cursorParent;
                    cursorParent = static_cast<CControlGroup*>(cursor->ParentCtrl());
                    }
                }
                
            if(overlapCtrlLev > cursorLev)
                {
                for(TInt i = 0 ; i < overlapCtrlLev - cursorLev; ++i)
                    {
                    overlapCtrl = overlapCtrlParent;
                    overlapCtrlParent = static_cast<CControlGroup*>(overlapCtrl->ParentCtrl());
                    }
                }
                
            //now we are in the same nest level, loop until parent is the same
            while(cursorParent != overlapCtrlParent)
                {
                cursor = cursorParent;
                overlapCtrl = overlapCtrlParent;
                cursorParent = static_cast<CControlGroup*>(cursor->ParentCtrl());
                overlapCtrlParent = static_cast<CControlGroup*>(overlapCtrl->ParentCtrl());
                }
            
            }
            
        aCursorIndex = cursorParent->GetIdx(cursor);
        TInt overlapCtrlInd= overlapCtrlParent->GetIdx(overlapCtrl);
        if(aBringToTopFlag && overlapCtrlInd > aCursorIndex )
            {
            bNeedClip = ETrue;
            }
        else if(!aBringToTopFlag && overlapCtrlInd < aCursorIndex)
            {
            bNeedClipBack = ETrue;
            }
        }

    if(aBringToTopFlag)
        {
        return bNeedClip;
        }
    else
        {
        return bNeedClipBack;
        }
    }
    
void CFepUiLayoutRootCtrl::BringToTopInGroup(CFepUiBaseCtrl* aCtrl)
    {
    if(aCtrl->ParentCtrl() == this)
        CControlGroup::BringToTopInGroup(aCtrl);
    else
        {
        //If a control has no parent, it's added to the pop list.    
        if(aCtrl->OrderPos() != EOrderFront)
            {
            aCtrl->SetOrdinalPos(EOrderFront);
            }
        InsertToPopList(aCtrl);
        }

        
    if(iCursor)
        {
        //set cursor valid/clip region
        TInt cursorIndex = 0;     
        if(aCtrl->Overlapped(iCursor) && ClipCursorForCtrlNeeded(aCtrl, ETrue, cursorIndex))
            {
            iCursor->ResetValidClipRegion();
            iCursor->UpdateValidRegion(aCtrl,EFalse);
            }
        }
    }
    
void CFepUiLayoutRootCtrl::BringToBackInGroup(CFepUiBaseCtrl* aCtrl)
    {
    if(aCtrl->ParentCtrl() == this)
        {
        CControlGroup::BringToBackInGroup(aCtrl);
        }
    else
        {
        TInt idx = iPopCtrlList.Find(aCtrl);
        if(idx == KErrNotFound)
            return;
        
        //move to desired position
        for(TInt i = idx -1 ; i >= 0 ; --i)
            {
            if(aCtrl->OrderPos() >= iPopCtrlList[i]->OrderPos())
                {
                iPopCtrlList[i+1] = iPopCtrlList[i];
                }
            else
                {
                iPopCtrlList[i+1] = aCtrl;
                break;
                }
            }
        
        }
    
    if(iCursor)
        {            
        TInt cursorIndex = 0;
        if(aCtrl->Overlapped(iCursor) && ClipCursorForCtrlNeeded(aCtrl, EFalse, cursorIndex))
            {
            // Reset the clip region, which overlap the cursor
            iCursor->ResetValidClipRegion();
            for(TInt i = cursorIndex + 1; i < iCtrlList.Count(); i++)
                {
                if(!iCtrlList[i]->Hiden())
                    iCursor->UpdateValidRegion(iCtrlList[i],EFalse);
                }
            for(TInt j = 0 ; j < iPopCtrlList.Count(); ++j)
                {
                if(!iPopCtrlList[j]->Hiden())
                    iCursor->UpdateValidRegion(iPopCtrlList[j],EFalse);
                }
            }
        }
    }
