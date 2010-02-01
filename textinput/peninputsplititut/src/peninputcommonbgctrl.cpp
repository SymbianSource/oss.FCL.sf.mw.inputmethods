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

// system includes
#include <peninputlayout.h>
#include <AknIconUtils.h>
#include <coemain.h>
#include <AknsUtils.h>
#include <AknsDrawUtils.h> 
// user includes
#include "peninputcommonbgctrl.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::NewL
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonBgCtrl* CAknFepCtrlCommonBgCtrl::NewL(CFepUiLayout* aUiLayout, 
                                                       TInt aControlId, 
                                                       TAknsItemID aFrameID,
    								 				   TAknsItemID aCenterID)
    { 
    CAknFepCtrlCommonBgCtrl* self = NewLC(aUiLayout, 
                                          aControlId, 
                                          aFrameID, 
                                          aCenterID);
    CleanupStack::Pop(self);
    
    return self;                        
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::NewLC
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonBgCtrl* CAknFepCtrlCommonBgCtrl::NewLC(CFepUiLayout* aUiLayout, 
                                                        TInt aControlId, 
                                                        TAknsItemID aFrameID,
    								 					TAknsItemID aCenterID)
    { 
    CAknFepCtrlCommonBgCtrl* self = new (ELeave) CAknFepCtrlCommonBgCtrl(aUiLayout, 
                                                                         aControlId,
                                                                         aFrameID,
                                                                         aCenterID);
    CleanupStack::PushL(self);
    self->BaseConstructL();
	self->SetOrdinalPos(CFepUiBaseCtrl::EOrderBackground); 
    return self;                      
    }
    	
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::SizeChanged
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonBgCtrl::SizeChanged(const TRect& aRect)
	{
    if (aRect.Size().iWidth == 0 || aRect.Size().iHeight == 0)
    {
    return;
    }
	
	SetRect( aRect );
	
	Draw();
    UpdateArea(Rect(), EFalse);
	}
	
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::Contains
// ---------------------------------------------------------------------------
//
TBool CAknFepCtrlCommonBgCtrl::Contains(const TPoint& /*aPt*/)
	{
	return EFalse;	
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::CAknFepCtrlCommonBgCtrl
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonBgCtrl::CAknFepCtrlCommonBgCtrl(CFepUiLayout* aUiLayout, 
                                                 TInt aControlId, 
                                                 TAknsItemID aFrameID,
    								 			 TAknsItemID aCenterID)
    : CFepUiBaseCtrl(TRect(), aUiLayout, aControlId),
   	  iFrameID(aFrameID), iCenterID(aCenterID)
    {        
    SetControlType( ECtrlBackgroundCtrl );
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::~CAknFepCtrlCommonBgCtrl
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonBgCtrl::~CAknFepCtrlCommonBgCtrl()
    {
    iSubItems.Reset();
    iSubItems.Close();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::SetFrameID
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonBgCtrl::SetFrameID( TAknsItemID aFrameID, TAknsItemID aCenterID )
	{
	iFrameID 	= aFrameID;
	iCenterID 	= aCenterID;	
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::Draw
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonBgCtrl::Draw()
    {
    if(Hiden() || !Ready() || Rect().Size() == TSize(0,0))
	    {
	    return;
	    }
	
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    //mask bitmaps
  	DrawOpaqueMaskBackground();  
    TRect rect = Rect();        
    // ----- draw frame -----
    gc->Activate( BitmapDevice() );                
    gc->Clear(rect);
	DrawFrame( gc, rect, iFrameID, iCenterID);
	
	for( TInt i = 0; i < iSubItems.Count(); i++ )
		{
		if( iSubItems[i].iIsShow )
			{
			DrawFrame( gc, 
					   iSubItems[i].iRect, 
					   iSubItems[i].iFrameID, 
					   iSubItems[i].iCenterID );				
			}
		}
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::ReDrawRect
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonBgCtrl::ReDrawRect(const TRect& aRect)
	{
    if(Hiden() || !Ready() || Rect().Size() == TSize(0,0))
	    {
	    return;
	    }	
	    
	CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
	DrawOpaqueMaskBackground();  
	gc->Activate( BitmapDevice() );                
    gc->Clear(aRect);
    
	gc->SetClippingRect( aRect );
    TRect rect = Rect();        
	
	DrawFrame( gc, rect, iFrameID, iCenterID);
	
	for( TInt i = 0; i < iSubItems.Count(); i++ )
		{
		if( iSubItems[i].iIsShow )
			{
			DrawFrame( gc, 
					   iSubItems[i].iRect, 
					   iSubItems[i].iFrameID, 
					   iSubItems[i].iCenterID );				
			}
		}	    
    gc->CancelClippingRect();
	}
	
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::AddSubBgItemL
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonBgCtrl::AddSubBgItemL( const TCommonBgCtrlSubItem& aSubItem )
	{
	iSubItems.AppendL( aSubItem );	
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::ShowSubBgItem
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonBgCtrl::ShowSubBgItem( TInt aIndex, TBool aShow )
	{
	iSubItems[aIndex].iIsShow = aShow;
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::DrawFrame
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonBgCtrl::DrawFrame( CFbsBitGc* aGc, 
										 const TRect& aRect,
										 TAknsItemID aFrameID,
										 TAknsItemID aCenterID )
	{
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRect innerRect = aRect;
    innerRect.Shrink( 4, 4 );
	
    AknsDrawUtils::DrawFrame(
						    skin, *aGc, 
						    aRect, innerRect, 
						    aFrameID, aCenterID);	
	}
	
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonBgCtrl::GetSubBgItem
// ---------------------------------------------------------------------------
//
TCommonBgCtrlSubItem& CAknFepCtrlCommonBgCtrl::GetSubBgItem( TInt aIndex )	
	{
	return iSubItems[aIndex];	
	}
	
//  End Of File
