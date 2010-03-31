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
* Description:  Implementation for control group
*
*/

#include "peninputlayoutbutton.h"
#include "peninputlayoutctrlgroup.h"
#include "peninputlayoutrootctrl.h"
#include "peninputlayout.h"
#include "coecntrl.h"
// ============================ MEMBER FUNCTIONS ===============================

/******************************************************************************/
// Implementation to Class CControlGroup
//
/******************************************************************************/

// -----------------------------------------------------------------------------
// CControlGroup::NewL
// Factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C CControlGroup* CControlGroup::NewL(CFepUiLayout* aUiLayout,
										    TInt aControlId,
										    TBool aRedrawBg /*= ETrue*/)
    {
    CControlGroup* ctrlGroup=new (ELeave)CControlGroup(aUiLayout,aControlId, aRedrawBg);   
    CleanupStack::PushL(ctrlGroup);
    ctrlGroup->ConstructL();
    CleanupStack::Pop(ctrlGroup);
    return ctrlGroup;
    }

// -----------------------------------------------------------------------------
// CControlGroup::NewL
// Factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
CControlGroup* CControlGroup::NewL(CFepUiLayout* aUiLayout,CCoeControl* aControl)
    {
    if(!aControl)
        return NULL;
    CControlGroup* ctrlGroup=new (ELeave)CControlGroup(aUiLayout,
                                                      aControl->UniqueHandle());
    
    ctrlGroup->iWndControl = aControl;
    CleanupStack::PushL(ctrlGroup);
    ctrlGroup->ConstructL();
    ctrlGroup->SetRect(aControl->Rect());    
    CleanupStack::Pop(ctrlGroup);
    return ctrlGroup;
    }
    
// -----------------------------------------------------------------------------
// CControlGroup::CControlGroup
// C++ default constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
EXPORT_C CControlGroup::CControlGroup(CFepUiLayout* aUiLayout,TInt aCtrlId)
                :CFepUiBaseCtrl(TRect(0,0,0,0),aUiLayout,aCtrlId)
    {
    iNeedRedrawBg = (TInt)ETrue;
    SetControlType(ECtrlControlGroup);
    }

// -----------------------------------------------------------------------------
// CControlGroup::CControlGroup
// C++ default constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
EXPORT_C CControlGroup::CControlGroup(CFepUiLayout* aUiLayout,
									  TInt aCtrlId,
									  TBool aRedrawBg)
                :CFepUiBaseCtrl(TRect(0,0,0,0),aUiLayout,aCtrlId)
    {
    iNeedRedrawBg = aRedrawBg;
    SetControlType(ECtrlControlGroup);
    }

// -----------------------------------------------------------------------------
// CControlGroup::ConstructL
// Second phrase constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//            
EXPORT_C void CControlGroup::ConstructL()
    {
    //do CFepUiBaseCtrl's base construction
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CControlGroup::~CControlGroup
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//            
EXPORT_C CControlGroup::~CControlGroup()
    {
    CFepUiBaseCtrl* ctrl;
    for(TInt i = iOwnedCtrlList.Count()-1;i >= 0;i--)
        {
        ctrl = iOwnedCtrlList[i];
        delete ctrl;
        }
    iOwnedCtrlList.Close();
    
    iCtrlList.Close();
    iPopCtrlList.Close();
    iNonHwrStartPtRegion.Close();    
    /*for(TInt i = 0 ; i < iSubRegionArea.Count(); ++i)
        {
         delete iSubRegionArea[i];
        }*/
    iSubRegionArea.ResetAndDestroy();    
    iSubRegionArea.Close();
    iSubCtrlShowStateBeforeHiden.Close();
    }
    
// -----------------------------------------------------------------------------
// CControlGroup::AddControlL
// Add control to group
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
EXPORT_C void CControlGroup::AddControlL(CFepUiBaseCtrl* aControl, TBool aOwnerFlag)
    {
    //do nothing if we already has the control.
    if(!aControl || KErrNotFound != iCtrlList.Find(aControl))
        return;
    CalculateClipRegion(aControl);
    
    if(aOwnerFlag)
        iOwnedCtrlList.AppendL(aControl);

    iCtrlList.AppendL(aControl);   
    
    aControl->SetParent(this);
    if(!aControl->Hiden() && !aControl->IsKindOfControl(ECtrlPopupWindow))
        {        
        //TRect rect = Rect();
        
        if(iCtrlList.Count() > 1)
            {
            iRect.BoundingRect(aControl->Rect());       
            }
        else
            iRect = aControl->Rect();
        
        //iRect = rect;

        /*RRegion region(8);
        region.Copy(Region());   

        region.AddRect(aControl->Rect());
        SetRegion(region);*/
        UpdateValidRegion(aControl,EFalse);
        }
    //since control may be added dynamically, we need draw it
//        aControl->Draw();
//        aControl->UpdateArea(aControl->Rect(),ETrue);
    TSubCtrlShownState state = {aControl,aControl->Hiden()};    
    iSubCtrlShowStateBeforeHiden.AppendL(state);
    }


// -----------------------------------------------------------------------------
// CControlGroup::Find
// Find the control in a control group
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
CFepUiBaseCtrl* CControlGroup::Find(CCoeControl* aCtrl)
    {
    if(!IsKindOfControl(ECtrlWndBasedCtrl))
        return NULL;
    
    if(aCtrl == iWndControl)
        return this;
    
    for(TInt i = 0; i < iCtrlList.Count(); i++)
        {
        CFepUiBaseCtrl* ctrl = iCtrlList[i];
        if(!ctrl->IsKindOfControl(ECtrlControlGroup))
            {
            if(aCtrl == ctrl->WndBasedControl())
                return ctrl;
            }
        else
            {
            /*
            bFound = static_cast<CControlGroup*>(ctrl)->Find(aCtrl);
            if(bFound)
                return bFound;
                */
            CControlGroup* ctrlGroup = static_cast<CControlGroup*>(ctrl);
            CFepUiBaseCtrl* foundedCtrl = ctrlGroup->Find(aCtrl);
            if(foundedCtrl)
                return foundedCtrl;
            }
        }
    
    return NULL;    
    }
    
    
// -----------------------------------------------------------------------------
// CControlGroup::AddControlL
// Add control to group
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
EXPORT_C CFepUiBaseCtrl* CControlGroup::AddControlL(CCoeControl* aControl)
    {
    //do nothing if we already has the control.
    if(!IsKindOfControl(ECtrlWndBasedCtrl) || !aControl)
        return NULL;
    
    CFepUiBaseCtrl* ctrl = Find(aControl);
    if(ctrl)
        return ctrl;   

    //if it's a compound control, we need find out its sub-control one by one
    if(aControl->CountComponentControls())
        {
        //create a control group
        CControlGroup* group = CControlGroup::NewL(UiLayout(),aControl);
        ctrl = group;
        CleanupStack::PushL(ctrl);
        for(TInt i = 0; i < aControl->CountComponentControls(); i++)
            {
            group->AddControlL(aControl->ComponentControl(i));
            }        
        group->SetControlType(ECtrlWndBasedCtrl|ECtrlControlGroup);                
        }
    else
        {
        ctrl = new(ELeave) CFepUiBaseCtrl(aControl,UiLayout());
        ctrl->SetControlType(ECtrlWndBasedCtrl|ECtrlBaseCtrl);
        CleanupStack::PushL(ctrl);
        ctrl->BaseConstructL();
        }   
    AddControlL(ctrl,ETrue);
    CleanupStack::Pop(ctrl);            
    UpdateValidRegion(ctrl,EFalse);    
    return ctrl;
    }

// ---------------------------------------------------------------------------
// CControlGroup::RemoveOwnedControl
// Remove a control from group's owned control list
// ---------------------------------------------------------------------------
//
TBool CControlGroup::RemoveOwnedControl(CFepUiBaseCtrl* aControl)
    {
    TInt index = iOwnedCtrlList.Find(aControl);
    if(KErrNotFound != index)
        {
        iOwnedCtrlList.Remove(index);
        delete aControl;
        }
    return KErrNotFound != index;
    }

// ---------------------------------------------------------------------------
// CControlGroup::RemoveControl
// Remove a control from group
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CControlGroup::RemoveControl(CFepUiBaseCtrl* aControl)
    {
    TInt i;
    TBool bFound = EFalse;
    //CControlGroup* parent = this;
    
    if(!aControl || iCtrlList.Count() <= 0)
        return EFalse;
    
    if(aControl == iActiveCtrl)
        iActiveCtrl = NULL;
    
    //iRect = iCtrlList[0]->Rect();    
    
    //find and remove the control
    for(i = 0 ; i < iCtrlList.Count() && !bFound; ++i)
        {
        if(iCtrlList[i] == aControl)
            {
            iCtrlList.Remove(i);            
            RemoveOwnedControl(aControl);
            bFound = ETrue;            
            }
        }
        
    if(bFound && aControl != NULL && !aControl->Hiden())
        {
        //change iRect
        if(iCtrlList.Count())
            {
            iRect = iCtrlList[0]->Rect();
            }
        else
            iRect = TRect(0,0,0,0);
        
        for( i = 1; i < iCtrlList.Count(); ++i)
            {            
            iRect.BoundingRect(iCtrlList[i]->Rect());            
            }
        }
    else
        return EFalse;
    
    RemoveCtrlFromSubCtrlInfoList(aControl);
    UpdateValidRegion(aControl,ETrue);
    
    CalculateClipRegion();
    return bFound;
    }

void CControlGroup::CalculateClipRegion(CFepUiBaseCtrl* aControl)
    {
    for(TInt i = 0 ; i < iCtrlList.Count(); ++i)
        {
        if(!aControl->Hiden())
            iCtrlList[i]->iClipRegion.SubRect(aControl->Rect());        
        }
    }
    
void CControlGroup::CalculateClipRegion()
    {
    //restore each control's rect first.
    for(TInt i = 0; i < iCtrlList.Count(); ++i)
        {
        CFepUiBaseCtrl* ctrl = iCtrlList[i];
        if(ctrl->IsKindOfControl(ECtrlControlGroup))
            static_cast<CControlGroup*>(ctrl)->CalculateClipRegion();
        ctrl->iClipRegion.Clear();
        ctrl->iClipRegion.AddRect(ctrl->Rect());
        }
    //re calulate clip region
    
    for(TInt curCtrlIdx = iCtrlList.Count() - 1 ; curCtrlIdx >= 0 ; --curCtrlIdx)
        {
        CFepUiBaseCtrl* curCtrl = iCtrlList[curCtrlIdx];
        for(TInt upperCtrlIdx = curCtrlIdx + 1 ;  upperCtrlIdx < iCtrlList.Count(); ++upperCtrlIdx)
            {
            if(!iCtrlList[upperCtrlIdx]->Hiden())
                curCtrl->iClipRegion.SubRect(iCtrlList[upperCtrlIdx]->Rect());
            }
        }
    }
// -----------------------------------------------------------------------------
// CControlGroup::At
// Return control in given position
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CControlGroup::At(TInt aIndex)
    {
    if(aIndex >=0 && aIndex < iCtrlList.Count())
        return iCtrlList[aIndex];
    else
        return NULL;
    }

// -----------------------------------------------------------------------------
// CControlGroup::ControlForPoints()
// Find the control which contains the point
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
CFepUiBaseCtrl* CControlGroup::ControlForPoints(const TPoint& aPt)
    {
    CFepUiBaseCtrl* ctrl = NULL;

    if(iActiveCtrl && !iActiveCtrl->Hiden() && iActiveCtrl->Contains(aPt))
        {
        return iActiveCtrl;        
        }
        
    for ( TInt i = iCtrlList.Count() - 1 ; i > -1 ; i-- )
        {
        ctrl = iCtrlList[i];
        if(!ctrl->Hiden() && ctrl->Contains(aPt))
            {
            return ctrl;
            }
        }
    return NULL;
    }


// -----------------------------------------------------------------------------
// CControlGroup::HandlePointerUpEventL()
// Handle pointer up event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CControlGroup::HandlePointerUpEventL(const TPoint& aPt)
    {
    if(iWndControl)
        {
        return CFepUiBaseCtrl::HandlePointerUpEventL(aPt);
        }
    CFepUiBaseCtrl* ctrl = ControlForPoints(aPt);
    if(!ctrl && !iRect.Contains(aPt)) // not belong our group
        return NULL;
    
    if(!PointerDown())
        {    
        return this;
        }
        
    SetPointerDown(EFalse);    
    
    if(ctrl != iCtrlWithPointerDown)
        {
        if(iCtrlWithPointerDown)
            iCtrlWithPointerDown->CancelPointerDownL();
        }
    else
        {        
        if(ctrl)
            {          // modify by vv for temp  
            if(ctrl->HandlePointerUpEventL(aPt))
                {
                if( !ctrl->Hiden() && ctrl->Ready() )
                    {
                    SetActiveCtrl(ctrl);
                    }
                }
            ctrl->SetPointerDown(EFalse);
            }
        }
    iCtrlWithPointerDown = NULL;
    return this;
    }


// -----------------------------------------------------------------------------
// CControlGroup::HandlePointerDownEventL()
// Handle pointer down event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CControlGroup::HandlePointerDownEventL(const TPoint& aPt)
    {
    if(iWndControl)
        {
        return CFepUiBaseCtrl::HandlePointerDownEventL(aPt);
        }
        
    CFepUiBaseCtrl* ctrl = ControlForPoints(aPt);
    iCtrlWithPointerDown = ctrl;
    
    if(ctrl)
        {
        SetPointerDown(ETrue);
        
        iPointerLeft = EFalse;
        iTempActiveCtrl = iActiveCtrl;
        SetTempActiveCtrl(ctrl);
        ctrl->HandlePointerDownEventL(aPt);
        return this;
        }
    return NULL;
    }


// -----------------------------------------------------------------------------
// CControlGroup::HandlePointerMoveEvent()
// Handle pointer move event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CControlGroup::HandlePointerMoveEventL(const TPoint& aPt)
    {
    if(iWndControl)
        {
        return CFepUiBaseCtrl::HandlePointerMoveEventL(aPt);
        }
    CFepUiBaseCtrl* ctrl = ControlForPoints(aPt);

    if(!ctrl && !iRect.Contains(aPt)) // not belong our group
        return NULL; // Check: This will not happen!
    
    if(!iCtrlWithPointerDown) // does not's process move event if no pointer down
        return this;
    
    if(ctrl != iCtrlWithPointerDown) 
    //pointer just move out the control for the first time
        {
        //Inform the control that pointer is out for first time
        if(!iPointerLeft)
            {
            iPointerLeft = ETrue;            
            iCtrlWithPointerDown->HandlePointerLeave(aPt);
            }
        }
    else
        {
        //Inform the control that pointer is in
        if(iPointerLeft)
            {                                    
            iCtrlWithPointerDown->HandlePointerEnter(aPt);
            iPointerLeft = EFalse; //reset the flag
            }
        }
    
    
    if(ctrl)
        ctrl->HandlePointerMoveEventL(aPt);
    else
        return NULL;
    return this;
    }


// -----------------------------------------------------------------------------
// CControlGroup::Draw
// Draw the control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CControlGroup::Draw()
    {
	if(!AbleToDraw() || WholeHiden())
        return;
    
    if(iWndControl)
        {
        CFepUiBaseCtrl::Draw();
        return;
        }
    //Clear();
        
    for(TInt i=0; i<iCtrlList.Count();i++)        
        {
        CFepUiBaseCtrl* ctrl = iCtrlList[i];
        //not draw hiden control and pop up window. Active control is drawn later
        if(!ctrl->Hiden() && !ctrl->IsKindOfControl(ECtrlPopupWindow)
            && ctrl != iActiveCtrl) 
            ctrl->Draw();        
        }
    //draw active controls
    if(iActiveCtrl)
        iActiveCtrl->Draw();
    
    //draw pop control
     for(TInt i=0; i<iPopCtrlList.Count();i++)        
        {
        CFepUiBaseCtrl* ctrl = iPopCtrlList[i];
        if(!ctrl->Hiden()) 
            ctrl->Draw();        
        }
    }

void DoRedrawInCtrlList(const TRect& aRect, CFepUiBaseCtrl* aCtrl,
                        RPointerArray<CFepUiBaseCtrl>& aList)
    {
    for(TInt i = 0 ; i < aList.Count(); ++i)
        {
        if(aList[i] == aCtrl)
            continue;
        if(!aList[i]->Hiden() && 
            aList[i]->IsOnTopOf(aCtrl) && 
            aRect.Intersects(aList[i]->Rect()))
            {
            TRect r = aRect;
            r.Intersection(aList[i]->Rect());
            aList[i]->ReDrawRect(r);
            }
        }
    }

// -----------------------------------------------------------------------------
// CControlGroup::RedrawRectIfNeeded
// ReDraw the affected control in the rect if needed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CControlGroup::RedrawRectIfNeeded(const TRect& aRect, CFepUiBaseCtrl* aCtrl)
    {
    if( aCtrl->Hiden() )
        {
        return;
        }
	if(!AbleToDraw() || WholeHiden())
        return;
    
    if(aRect.IsEmpty())
        return;
    
    //check all sub controls
    DoRedrawInCtrlList(aRect,aCtrl,iCtrlList);    
    //check pop list
    //check all sub controls
    DoRedrawInCtrlList(aRect,aCtrl,iPopCtrlList);    
    }
    
    
// -----------------------------------------------------------------------------
// CControlGroup::ReDrawRect
// ReDraw the control in the rect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CControlGroup::ReDrawRect(const TRect& aRect)
    {   
	if(!AbleToDraw() || WholeHiden())
        return;
    
    if(aRect.IsEmpty())
        return;
    if(aRect == Rect())
        {
        Draw();    
        }
    else
        {        
        //find out the controls within the rect
        if( iNeedRedrawBg )
        	DrawBackground( aRect );            
        //DrawControlGroupBackground( aRect );
        TBool needDrawActiveCtrl = EFalse;
        for(TInt i=0;i<iCtrlList.Count();i++)
            {
            CFepUiBaseCtrl* ctrl=iCtrlList[i];
            if(!ctrl->Hiden() && aRect.Intersects(ctrl->Rect()))
                {
                if(iActiveCtrl == ctrl)
                    {
                    needDrawActiveCtrl = ETrue;
                    continue;
                    }
                TRect r = aRect;
                r.Intersection(ctrl->Rect());
                
                ctrl->ReDrawRect(r);
                }
                
            }
        if(needDrawActiveCtrl)
            {
            TRect r = aRect;
            r.Intersection(iActiveCtrl->Rect());
            
            iActiveCtrl->ReDrawRect(r);            
            }
        }        
            
    }    
    

// -----------------------------------------------------------------------------
// CControlGroup::SetTempActiveCtrl
// Set temparary active control in button down state
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CFepUiBaseCtrl* CControlGroup::SetTempActiveCtrl(CFepUiBaseCtrl* aCtrl)
    {
    if( aCtrl && 
    	aCtrl != iTempActiveCtrl && 
        !(aCtrl->ControlType() & ECtrlBackgroundCtrl) )
        {             
        //iPrevActiveCtrl = iActiveCtrl;   
        CFepUiBaseCtrl* previous = iTempActiveCtrl;
        iTempActiveCtrl = aCtrl;
        iTempActiveCtrl->SetActive(ETrue);
        //We need to check whether the control news status is successfully set.   
        if( iTempActiveCtrl->IsActive() )
            {
            if(previous)
                previous->SetActive(EFalse);
            }
        else
            {
            return previous;
            }
        }
    return iTempActiveCtrl;
    }
    
// -----------------------------------------------------------------------------
// CControlGroup::SetActiveCtrl
// Set active control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CControlGroup::SetActiveCtrl(CFepUiBaseCtrl* aCtrl)
    {
    if( aCtrl &&
    	aCtrl != iActiveCtrl &&
    	!(aCtrl->ControlType() & ECtrlBackgroundCtrl) )
        {                
        CFepUiBaseCtrl* previous = iActiveCtrl;
        iActiveCtrl = aCtrl;
        //Change the previous control first, then current control
        if(previous)
            previous->SetActive(EFalse);

        iActiveCtrl->SetActive(ETrue);
        }        
    return iActiveCtrl;
    }
    
// -----------------------------------------------------------------------------
// CControlGroup::SetActiveCtrl
// Set active control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CControlGroup::SetActiveCtrl(TInt aIndex)
    {
    if(aIndex < 0 || aIndex >= iCtrlList.Count())
        return NULL;
    return SetActiveCtrl(iCtrlList[aIndex]);        
    }
  
// -----------------------------------------------------------------------------
// CControlGroup::ActiveControl
// Get active control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CControlGroup::ActiveControl()
    {
        return iActiveCtrl;
    }

// -----------------------------------------------------------------------------
// CControlGroup::NumOfControlss
// get control numbers
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt CControlGroup::NumOfControls()
    {
    return iCtrlList.Count();
    }
    

// -----------------------------------------------------------------------------
// CControlGroup::HideControl
// Hide control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//       
EXPORT_C void CControlGroup::HideControl(TInt aIndex,TBool aFlag)
    {
    if(aIndex >=0 && aIndex < iCtrlList.Count())
        {
        HideControl(iCtrlList[aIndex],aFlag);
        }
    }
    
// -----------------------------------------------------------------------------
// CControlGroup::HideControl
// Hide control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//           
EXPORT_C void CControlGroup::HideControl(CFepUiBaseCtrl* aControl,TBool aFlag)
    {
    if(!aControl  || aControl->Hiden() == aFlag)    
        return;
    
    aControl->Hide(aFlag);

    //recalculate the control group rect
    if(aFlag) //hide
        {        
        iRect.SetRect(0,0,0,0);
        for(TInt j = 0; j < iCtrlList.Count(); j++)
            {
            if(aControl != iCtrlList[j] && ! iCtrlList[j]->Hiden())
                {
                if(iRect.IsEmpty())
                    {
                    iRect = iCtrlList[j]->Rect();
                    }
                else
                    {
                    iRect.BoundingRect(iCtrlList[j]->Rect());
                    }
                }
            }
        }
    else	//show
        {
        if(iRect.IsEmpty())
            {
            iRect = aControl->Rect();
            }
        else 
            {                                   
            iRect.BoundingRect(aControl->Rect());
            }
        }

    UpdateValidRegion(aControl,ETrue);
        
    /*
    SetRect(rect);
    RRegion region(8);
    region.Copy(Region());   

    if(aFlag) //hide control
        region.SubRect(aControl->Rect());
    else
        region.AddRect(aControl->Rect());
    SetRegion(region);
    RootControl()->UpdateValidRegion(NULL,EFalse);
    */
    }


// -----------------------------------------------------------------------------
// CControlGroup::Hide
// Hide the control group and all it's controls
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CControlGroup::Hide(TBool aFlag)
    {
    if(Hiden() == aFlag)
        return;
  /*  if(aFlag) //hide
        {
        SaveSubCtrlShownState();
        }*/
    DisableRegionUpdating(); //disable updating caused by sub controls           
    /*for (TInt i = 0; i < iCtrlList.Count(); ++i)
        {
        CFepUiBaseCtrl* ctrl = iCtrlList[i];
        TBool hideFlag = aFlag;
        if( i < iSubCtrlShowStateBeforeHiden.Count() && 
                    iSubCtrlShowStateBeforeHiden[i].iSubCtrl == ctrl)
            {            
            hideFlag = hideFlag || iSubCtrlShowStateBeforeHiden[i].iHiden;
            }
        ctrl->Hide(hideFlag); //loop sub controls        
        }*/
    EnableRegionUpdating();            
    CFepUiBaseCtrl::Hide(aFlag); //hide myself               
    /*if(!aFlag)
        iSubCtrlShowStateBeforeHiden.Reset();
*/
        }

// -----------------------------------------------------------------------------
// CControlGroup::Move
// Move control and component's rect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
EXPORT_C void CControlGroup::Move(const TPoint& aOffset)
    {
    //disable region update and re-calculate it after all sub-control moved.
    DisableRegionUpdating();    
    CFepUiBaseCtrl::Move(aOffset);
    //move components
    for(TInt j = 0; j < iCtrlList.Count(); j++)
        {
        iCtrlList[j]->Move(aOffset);
        }    
    EnableRegionUpdating();
    UpdateValidRegion(NULL,ETrue); //recalculate valid region
    }
    
// -----------------------------------------------------------------------------
// CControlGroup::ControlById
// Get the sub control for given control Id. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//            
EXPORT_C CFepUiBaseCtrl* CControlGroup::ControlById(TInt aCtrlId)
    {
    CFepUiBaseCtrl* ctrl = NULL;
    
    for(TInt i = 0; i < iCtrlList.Count(); i++)
        {
        ctrl = iCtrlList[i];
        if(ctrl->ControlId() == aCtrlId)
            {
                return ctrl;
            }
        else
            {
            if(ctrl->IsKindOfControl(ECtrlControlGroup))
                {
                CFepUiBaseCtrl* subCtrl = static_cast<CControlGroup*>(ctrl)->
                                            ControlById(aCtrlId);
                if(subCtrl)
                    return subCtrl; 
                }
            }
        }

    return NULL;
    
    }
    
// ---------------------------------------------------------------------------
// CControlGroup::CancelPointerDownL
// Cancel pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CControlGroup::CancelPointerDownL()
    {
    //let the pointer downed control do the canceling
    if(iCtrlWithPointerDown)
        {
        if (iCtrlWithPointerDown != iActiveCtrl)
        	{
			iCtrlWithPointerDown->SetActive(EFalse);
			if (iActiveCtrl)
				{
				iActiveCtrl->SetActive(ETrue);        		
				}
        	}
        iCtrlWithPointerDown->CancelPointerDownL();
        iCtrlWithPointerDown = NULL;
        ReportEvent(EEventButtonDownCanceled);
        }    
    }

// ---------------------------------------------------------------------------
// CControlGroup::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CControlGroup::HandlePointerLeave(const TPoint& aPt)
    {
    SetPointerDown( EFalse );
    iPointerLeft = ETrue;

    if(iCtrlWithPointerDown)
        {
        if(iCtrlWithPointerDown != iActiveCtrl)
            {
            SetTempActiveCtrl(iActiveCtrl);
            }
        iCtrlWithPointerDown->HandlePointerLeave(aPt);        
        }
    }
    
// ---------------------------------------------------------------------------
// CControlGroup::HandlePointerEnter
// Handle pointer enter event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CControlGroup::HandlePointerEnter(const TPoint& aPt)
    {
    SetPointerDown( ETrue );
    iPointerLeft = EFalse;
    if(iCtrlWithPointerDown)
        {
        //does the pointer really enter this control?
        if(iCtrlWithPointerDown->Contains(aPt))
            {
            SetTempActiveCtrl(iCtrlWithPointerDown);
            iCtrlWithPointerDown->HandlePointerEnter(aPt);        
            }
        }
    }

// ---------------------------------------------------------------------------
// CControlGroup::ControlByType
// Get a control by type and index
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CControlGroup::ControlByType(TControlType aType,TInt aIndex)
    {
    if(aIndex < 0)
        return NULL;
    
    CFepUiBaseCtrl* ctrl = NULL;
    TInt ctrlIdx = 0;
    for(TInt i = 0;i < iCtrlList.Count();i++)
        {
        if(iCtrlList[i]->ControlType() == aType)
            {
            if(ctrlIdx == aIndex)
                return iCtrlList[i];
            else
                ctrlIdx++;
            }
        else
            {
            //If sub control is a control group, loop inside the control group
            if(iCtrlList[i]->IsKindOfControl(ECtrlControlGroup))
                {
                CControlGroup* ctrlGroup = static_cast<CControlGroup*>(iCtrlList[i]);
                ctrl = ctrlGroup->ControlByType(aType,aIndex - ctrlIdx);
                if(ctrl)
                    return ctrl;
                }
            }
        }

    return ctrl;
    }

// ---------------------------------------------------------------------------
// CControlGroup::ControlHasType
// Get a control by type and index
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CControlGroup::ControlForKindOfType(
                                        TStockBaseCtrlType aType,TInt aIndex)
    {
    
    if(aIndex < 0 || aIndex >= iCtrlList.Count())
        return NULL;
    
    CFepUiBaseCtrl* ctrl = NULL;
    TInt ctrlIdx = 0;
    for(TInt i = 0;i < iCtrlList.Count();i++)
        {
        if(iCtrlList[i]->IsKindOfControl(aType))
            {
            if(ctrlIdx == aIndex)
                return iCtrlList[i];
            else
                ctrlIdx++;
            }
        else
            {
            //If sub control is a control group, loop inside the control group
            if(iCtrlList[i]->IsKindOfControl(ECtrlControlGroup))
                {
                CControlGroup* ctrlGroup = static_cast<CControlGroup*>(iCtrlList[i]);
                ctrl = ctrlGroup->ControlForKindOfType(aType,aIndex - ctrlIdx);
                if(ctrl)
                    return ctrl;
                }
            }
        }

    return ctrl;
    }


// ---------------------------------------------------------------------------
// CControlGroup::OnDeActivate
// Response to layout de activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CControlGroup::OnDeActivate()
    {
    TRAP_IGNORE(OnDeActivateL());
    }

void CControlGroup::OnDeActivateL()
    {
    if(iCtrlWithPointerDown)
        {
        TRAP_IGNORE(iCtrlWithPointerDown->CancelPointerDownL());
        iCtrlWithPointerDown = NULL;
        }
    CFepUiBaseCtrl::OnDeActivate();
    for(TInt i = 0;i < iCtrlList.Count();i++)
        {
        CFepUiBaseCtrl* ctrl = iCtrlList[i];
        
        __ASSERT_DEBUG(ctrl,User::Leave(-1));    
        
        iCtrlList[i]->OnDeActivate();
        }
    }

// ---------------------------------------------------------------------------
// CControlGroup::OnDctivate
// Response to layout activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CControlGroup::OnActivate()
    {
    CFepUiBaseCtrl::OnActivate();
    for(TInt i = 0;i < iCtrlList.Count();i++)
        {
        iCtrlList[i]->OnActivate();
        }
    }

// ---------------------------------------------------------------------------
// CControlGroup::OnLayoutDraggingStart
// Response to layout dragging start event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CControlGroup::OnLayoutDraggingStart()
    {
    //notify child controls
    for(TInt i = 0; i < iCtrlList.Count(); i++)
        {
        iCtrlList[i]->OnLayoutDraggingStart();
        }
    }
    
// ---------------------------------------------------------------------------
// CControlGroup::OnLayoutDraggingEnd
// Response to layout dragging end event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CControlGroup::OnLayoutDraggingEnd()
    {
    //notify child controls    
    for(TInt i = 0; i < iCtrlList.Count(); i++)
        {
        iCtrlList[i]->OnLayoutDraggingEnd();
        }    
    }    

// ---------------------------------------------------------------------------
// CControlGroup::ConstructFromResourceL
// Handle resource change
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CControlGroup::ConstructFromResourceL()
    {
    //let child control hanlde resource change.
    for(TInt i = 0; i < iCtrlList.Count(); i++)
        {
        iCtrlList[i]->ConstructFromResourceL();
        }    
    }
    
const RRegion& CControlGroup::CalculateNonHwrStartPtArea()
    {
    RRegion r;
    iNonHwrStartPtRegion.Clear();
    for(TInt i = 0; i < iCtrlList.Count(); ++i)
        {						 
        CFepUiBaseCtrl* ctrl = iCtrlList[i];
        if(ctrl->Hiden() || !ctrl->Ready())
            continue;
        if(ctrl->IsKindOfControl(ECtrlControlGroup))
            {
            iNonHwrStartPtRegion.Union(static_cast<CControlGroup*>(ctrl)->
                            CalculateNonHwrStartPtArea());
            continue;
            }
        if(ctrl->IsKindOfControl(ECtrlTransparentHwrWnd))
            {            
            r.Copy(ctrl->Region());
            continue;		          
            }
        
        iNonHwrStartPtRegion.AddRect(ctrl->Rect());
        }
    r.Sort();
    
    RRegion r2;
    r2.AddRect(UiLayout()->Rect());
    r2.SubRegion(iNonHwrStartPtRegion);
    r2.Sort();
    r2.Close();
    r.Close();
    return iNonHwrStartPtRegion;    
    }    
    
// ---------------------------------------------------------------------------
// CControlGroup::UpdateValidRegion
// Update control's valid region when other control show or hide.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CControlGroup::UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag)
    {
    if(!iUpdateFlagEnabled) //do nothing if disabled.
        return;
    
    if(aCtrl) //update due to this control
        {        
        TInt idx = FindSubRegion(aCtrl);
        if(aRemoveFlag)
            {
            if(idx == KErrNotFound)
                return;
            RemoveSubRegion(idx);            
            
            delete iSubRegionArea[idx];
            iSubRegionArea.Remove(idx);        
            CalculateClipRegion();        
            }
        else
            {
            if(idx == KErrNotFound) //new one
                {            
                //no need to check whether the control is hide as updating is done when 
                //control is hiden.
                //CSubRegionArea area(aCtrl,aCtrl->Region());
                //iSubRegionArea.Append(area);
                TRAP_IGNORE(AddSubRegionL(aCtrl));
                CalculateClipRegion(aCtrl);
                }
            else //replace the old one   
                {            
                RemoveSubRegion(idx);                                            
                iSubRegionArea[idx]->iSubRegion.Copy(aCtrl->Region());
                CalculateClipRegion();
                }
            iValidRegion.Union(aCtrl->Region());                
            }   
        }
    else //recalculate valid region
        {
        iSubRegionArea.ResetAndDestroy();
        iValidRegion.Clear();
        for(TInt i = 0 ; i < iCtrlList.Count(); ++i)
            {		
            iValidRegion.Union(iCtrlList[i]->Region()); 
            //CSubRegionArea area(aCtrl,aCtrl->Region());
            //iSubRegionArea.Append(area);
            TRAP_IGNORE(AddSubRegionL(iCtrlList[i]));
            }
            
        CalculateClipRegion();
        }
        
    if(iParentCtrl)
        iParentCtrl->UpdateValidRegion(this,EFalse);    
    }

TInt CControlGroup::FindSubRegion(CFepUiBaseCtrl* aCtrl)
    {
    for(TInt i = 0; i < iSubRegionArea.Count(); ++i)
        {
        if(aCtrl == iSubRegionArea[i]->iSubCtrl)
            return i;
        }
        
    return KErrNotFound;
    }

void CControlGroup::RemoveCtrlFromSubCtrlInfoList(CFepUiBaseCtrl* aControl)
    {
    for(TInt i = 0 ; i < iSubCtrlShowStateBeforeHiden.Count(); ++i)
        {
        if(iSubCtrlShowStateBeforeHiden[i].iSubCtrl == aControl)
            {
            iSubCtrlShowStateBeforeHiden.Remove(i);
            break;
            }
        }
    }

//record each sub control's current hide/show state
void CControlGroup::SaveSubCtrlShownState()
    {   
    iSubCtrlShowStateBeforeHiden.Reset(); 
    for (TInt i = 0; i < iCtrlList.Count(); ++i)
        {
        TSubCtrlShownState state = {iCtrlList[i],iCtrlList[i]->Hiden()};
            
        TRAP_IGNORE(iSubCtrlShowStateBeforeHiden.AppendL(state));
        }                
    }

void CControlGroup::AddSubRegionL(CFepUiBaseCtrl* aCtrl)
    {
    //no need to check aCtrl    
    CSubRegionArea* area = new (ELeave) CSubRegionArea(aCtrl,aCtrl->Region());
    CleanupStack::PushL(area);
    iSubRegionArea.AppendL(area);
    CleanupStack::Pop(area);    
    }
    
void CControlGroup::RemoveSubRegion(TInt aIdx)
    {    
    //check whether this region is clipped with other region
    RRegion overlappedReg;    
    for(TInt i = 0 ; i < iSubRegionArea.Count(); ++i)
        {
        if(i == aIdx)
            continue;
        
        RRegion r;
        r.Intersection(iSubRegionArea[aIdx]->iSubRegion,
                                        iSubRegionArea[i]->iSubRegion);
        overlappedReg.Union(r);
        r.Close();           
        }						   
    iValidRegion.SubRegion(iSubRegionArea[aIdx]->iSubRegion);
    iValidRegion.Union(overlappedReg);
    overlappedReg.Close();
    
    }

// -----------------------------------------------------------------------------
// CControlGroup::BringToTop
// Bring and show a control in current group
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
TBool CControlGroup::MoveSubCtrlToTop(CFepUiBaseCtrl* aCtrl,RPointerArray<CFepUiBaseCtrl>& aList)
    {
    TInt index = aList.Find(aCtrl); 
    if(KErrNotFound == index)
        return EFalse;
    //move the control to the end of the list
    for(; index < aList.Count() -1; ++index)
        {
        aList[index] = aList[index + 1];
        }
    aList[index] = aCtrl;
    return ETrue;    
    }

TBool CControlGroup::MoveSubCtrlToBack(CFepUiBaseCtrl* aCtrl,RPointerArray<CFepUiBaseCtrl>& aList)
    {
    TInt index = aList.Find(aCtrl); 
    if(KErrNotFound == index)
        return EFalse;
    //move the control to the start of the list
    for(; index > 0; --index)
        {
        //stop if found background control
        if( iCtrlList[index-1]->IsKindOfControl(ECtrlBackgroundCtrl) ||
            iCtrlList[index-1]->IsKindOfControl(ECtrlTransparentHwrWnd) )
            {
            break;
            }
        
        aList[index] = aList[index -1];
        }
    aList[index] = aCtrl;
    return ETrue;    
    }

EXPORT_C void CControlGroup::BringToTopInGroup(CFepUiBaseCtrl* aCtrl)
    {
    if(MoveSubCtrlToTop(aCtrl,iCtrlList)) //check in iCtrlList first
        {
        aCtrl->iClipRegion.Clear();
        aCtrl->iClipRegion.AddRect(aCtrl->Rect());
        UpdateValidRegion(aCtrl,EFalse);
        //other control should update its valid/clip region    
        }
    }

EXPORT_C void CControlGroup::BringToBackInGroup(CFepUiBaseCtrl* aCtrl)
    {
    if(MoveSubCtrlToBack(aCtrl,iCtrlList)) //check in iCtrlList first
        {
        aCtrl->iClipRegion.Clear();
        aCtrl->iClipRegion.AddRect(aCtrl->Rect());
        
        UpdateValidRegion(aCtrl,EFalse);
        //other control should update its valid/clip region    
        }
    }

// -----------------------------------------------------------------------------
// CControlGroup::OnResourceChange
// Handle system resource change 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
EXPORT_C void CControlGroup::HandleResourceChange(TInt aType)
    {
    //let child control handle the resource change
    for(TInt i = 0 ; i < iCtrlList.Count(); ++i)
        {
        iCtrlList[i]->HandleResourceChange(aType);
        }
    CFepUiBaseCtrl::HandleResourceChange(aType);
    }
// -----------------------------------------------------------------------------
// CControlGroup::GraphicDeviceSizeChanged
// Handle graphic device size change event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
EXPORT_C void CControlGroup::GraphicDeviceSizeChanged()
    {
    //let child control handle the resource change
    for(TInt i = 0 ; i < iCtrlList.Count(); ++i)
        {
        iCtrlList[i]->GraphicDeviceSizeChanged();
        }
    CFepUiBaseCtrl::GraphicDeviceSizeChanged();     
    }


// ---------------------------------------------------------------------------
// CControlGroup::UpdateArea
// Update layout area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
void CControlGroup::UpdateAreaInGrp(CFepUiBaseCtrl* aControl,const TRect& aRect,TBool aUpdateFlag)
    {
    RedrawRectInGrp( aControl, aRect );
    UpdateArea(aRect,aUpdateFlag);
    }    
    
//implementation for class CControlGroup::CSubRegionArea::CSubRegionArea
CControlGroup::CSubRegionArea::CSubRegionArea(const CFepUiBaseCtrl* aCtrl,const RRegion &aRegion)
                                : iSubCtrl(aCtrl) //,iSubRegion(aRegion) 
    {
    iSubRegion.Copy(aRegion);
    //RRegion's copy constructor will make two region point to same data. !!!
    }
CControlGroup::CSubRegionArea::~CSubRegionArea()
    {
    iSubRegion.Close();
    }

TInt CControlGroup::GetNestedLevel()    
    {
    TInt level = 1;
    CFepUiBaseCtrl* parent = ParentCtrl();
    if(!parent)
        return 0; //This is the root control
 
    while(parent && parent != RootControl())
        {
        parent = ParentCtrl();
        level++;
        }
    return level;
    }


TInt CControlGroup::GetIdx(const CFepUiBaseCtrl* aCtrl)
    {
    for(TInt i = 0; i < iCtrlList.Count(); i++)
        {
        if(iCtrlList[i] == aCtrl)
            {
            return i;            
            }
        } 
    return KErrNotFound;   
    }

// ---------------------------------------------------------------------------
// CControlGroup::DrawControlGroupBackground
// Draw ControlGroup Background
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CControlGroup::DrawControlGroupBackground( const TRect& aUpdateRegion )
	{
	DrawBackground( aUpdateRegion );
	}
	
void CControlGroup::AdjustBkCtrlPos()
    {
    //No need to check the first control
    TInt firstBkCtrlPos = 1;
    for(TInt i = 1 ; i < iCtrlList.Count(); ++i)
        {
        if(iCtrlList[i]->IsKindOfControl(ECtrlBackgroundCtrl)) 
            {
            //move to first
            CFepUiBaseCtrl* ctrl = iCtrlList[firstBkCtrlPos];
            iCtrlList[firstBkCtrlPos++] = iCtrlList[i];

            for(TInt j = i ; j < firstBkCtrlPos ; --j)
                {
                iCtrlList[j] = iCtrlList[j-1];
                }
            iCtrlList[firstBkCtrlPos] = ctrl;
            }
        else
            {
            if(iCtrlList[i]->IsKindOfControl(ECtrlControlGroup))
                {
                CControlGroup* ctrlGroup = static_cast<CControlGroup*>(iCtrlList[i]);
                ctrlGroup->AdjustBkCtrlPos();
                }
            }
        }
    } 	
    
// ---------------------------------------------------------------------------
// CControlGroup::UpdateAreaInGrpImmed
// Update layout area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
void CControlGroup::UpdateAreaInGrpImmed(CFepUiBaseCtrl* aControl,const TRect& aRect,TBool aUpdateFlag)
    {
    RedrawRectInGrp( aControl, aRect );
    UpdateAreaImmed(aRect,aUpdateFlag);
    }    


// ---------------------------------------------------------------------------
// CControlGroup::RedrawRectInGrp
// Redraw rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CControlGroup::RedrawRectInGrp( CFepUiBaseCtrl* aControl,const TRect& aRect )
    {
    //check in ctrl list
    TInt idx = 0;
    for(; idx < iCtrlList.Count(); ++idx)
        {
        if(aControl == iCtrlList[idx])
            {
            ++idx;
            break;
            }
        }
       
    for(; idx < iCtrlList.Count(); ++idx)
        {
        if(aRect.Intersects(iCtrlList[idx]->Rect()) &&
                                        !iCtrlList[idx]->Hiden())
            {
            iCtrlList[idx]->ReDrawRect(aRect);
            }
        }
    //check the pop up list
    for(idx = 0 ; idx < iPopCtrlList.Count(); ++idx)
        {
        if(aRect.Intersects(iPopCtrlList[idx]->Rect()) && 
                                        !iPopCtrlList[idx]->Hiden())
            {
            if(aControl != iPopCtrlList[idx]) //not the same one
                iPopCtrlList[idx]->ReDrawRect(aRect);
            }
        }
    }    
//end of file
