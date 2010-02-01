/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for latchable button
*
*/


#include "peninputlayoutlatchbutton.h"

// ============================ MEMBER FUNCTIONS ===============================
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutLatchButton* CFepLayoutLatchButton::NewL(const TRect& aRect,
											CFepUiLayout* aUiLayout,TInt aCtrlId)
    {
    CFepLayoutLatchButton* btn=new (ELeave)CFepLayoutLatchButton(aRect,
    														aUiLayout,aCtrlId);
    CleanupStack::PushL(btn);
    btn->BaseConstructL();
    CleanupStack::Pop(btn);
    return btn;    
    }
    
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::CFepLayoutLatchButton
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutLatchButton::CFepLayoutLatchButton(const TRect& aRect,
								CFepUiLayout* aUiLayout,TInt aCtrlId)
                        			:CButtonBase(aRect,aUiLayout,aCtrlId),
                         			iLatched(EFalse)
    {
    //do nothing right now.
    }

// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::~CFepLayoutLatchButton
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutLatchButton::~CFepLayoutLatchButton()
    {  
    delete iLatchedBmp;
    delete iLatchedMaskBmp;
    delete iUnLatchedBmp;
    delete iUnLatchedMaskBmp;
    
    iActiveBmp = NULL;
    iActiveBmpPressed= NULL;
    iNonActiveBkBmp= NULL;
    iNonActiveBkBmpPressed= NULL;
    
    
    iActiveMaskBmp= NULL;
    iActiveMaskBmpPressed= NULL;
    iNonActiveBkMaskBmp= NULL;
    iNonActiveBkMaskBmpPressed= NULL; 
    }


// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::SetLatchedBitmapL
// Set lathed bitmap
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutLatchButton::SetLatchedBitmap(CFbsBitmap* aBmp,
														CFbsBitmap* aMaskBmp)
	{
	iLatchedBmp = aBmp;
	iLatchedMaskBmp = aMaskBmp;
	if(iLatched)
		{
		SetBackgroundBmp(iLatchedBmp);
		SetBackgroundMaskBmp(iLatchedMaskBmp);
		}
	//set active pressed and non active pressed bitmap to latched bitmap
	iActiveBmpPressed = aBmp;
	iNonActiveBkBmpPressed = aBmp;
	iActiveMaskBmpPressed = aMaskBmp;
	iNonActiveBkMaskBmpPressed = aMaskBmp;
	
	}

// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::SetUnLatchedBitmapL
// Set unlathed bitmap
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutLatchButton::SetUnLatchedBitmap(CFbsBitmap* aBmp,
														CFbsBitmap* aMaskBmp)
	{
	iUnLatchedBmp = aBmp;
	iUnLatchedMaskBmp = aMaskBmp;
	if(!iLatched)
		{
		SetBackgroundBmp(iUnLatchedBmp);
		SetBackgroundMaskBmp(iUnLatchedMaskBmp);
		}
	//set active and non active bitmap to latched bitmap
	iActiveBmp = aBmp;
	iNonActiveBkBmp = aBmp;
	iActiveMaskBmp = aMaskBmp;
	iNonActiveBkMaskBmp = aMaskBmp;		
	}
	
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::HandlePointerDownEventL
// Handle latch button down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CFepLayoutLatchButton::HandlePointerDownEventL(
															const TPoint& aPt)
    {
    
    if(IsDimmed())
        return NULL;
	CFepUiBaseCtrl::HandlePointerDownEventL(aPt);    	
	ChangeState();
    ReportEvent(EEventButtonDown);
    return this;
    }
	
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::HandlePointerUpEentL
// Handle latch button up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CFepLayoutLatchButton::HandlePointerUpEventL(
															const TPoint& aPt)
    {
	//do nothing special
	CFepUiBaseCtrl::HandlePointerUpEventL(aPt);   	 
    ReportEvent(EEventButtonUp);
    return this;
    }	
    
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::HandlePointerEnter
// Handle pointer enter event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepLayoutLatchButton::HandlePointerEnter(const TPoint& /*aPt*/)
    {
    if(IsDimmed())
        return;
    ChangeState();
    ReportEvent(EEventPointerEnterControl);
    }
    
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepLayoutLatchButton::HandlePointerLeave(const TPoint& /*aPt*/)
    {
    if(IsDimmed())
        return;
	ChangeState();
	ReportEvent(EEventPointerLeftControl);
    }
    
    
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::ChangeState
// Change latch state when button down/up
// ---------------------------------------------------------------------------
//           
void CFepLayoutLatchButton::ChangeState()
	{
	iLatched = !iLatched; //change state
    Draw();
    UpdateArea(Rect(),EFalse);	
	}
	
	
// ---------------------------------------------------------------------------
// CFepLayoutLatchButton::Draw
// Draw the control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepLayoutLatchButton::Draw()
    {
    //change backgroud according to latch state
	if(iLatched)
		{
		SetBackgroundBmp(iLatchedBmp);
		SetBackgroundMaskBmp(iLatchedMaskBmp);
		}
	else
		{
		SetBackgroundBmp(iUnLatchedBmp);
		SetBackgroundMaskBmp(iUnLatchedMaskBmp);
		}
	CButtonBase::Draw();    
    }
	
//end of file
