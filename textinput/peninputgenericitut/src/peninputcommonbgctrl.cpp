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

// ======== MEMBER FUNCTIONS ========

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
	
TBool CAknFepCtrlCommonBgCtrl::Contains(const TPoint& /*aPt*/)
	{
	return EFalse;	
	}

CAknFepCtrlCommonBgCtrl::CAknFepCtrlCommonBgCtrl(CFepUiLayout* aUiLayout, 
                                                 TInt aControlId, 
                                                 TAknsItemID aFrameID,
    								 			 TAknsItemID aCenterID)
    : CFepUiBaseCtrl(TRect(), aUiLayout, aControlId),
   	  iFrameID(aFrameID), iCenterID(aCenterID)
    {        
    SetControlType( ECtrlBackgroundCtrl );
    }

CAknFepCtrlCommonBgCtrl::~CAknFepCtrlCommonBgCtrl()
    {
    iSubItems.Reset();
    iSubItems.Close();
    }

void CAknFepCtrlCommonBgCtrl::SetFrameID( TAknsItemID aFrameID, TAknsItemID aCenterID )
	{
	iFrameID 	= aFrameID;
	iCenterID 	= aCenterID;	
	}

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
	
void CAknFepCtrlCommonBgCtrl::AddSubBgItemL( const TCommonBgCtrlSubItem& aSubItem )
	{
	iSubItems.AppendL( aSubItem );	
	}

void CAknFepCtrlCommonBgCtrl::ShowSubBgItem( TInt aIndex, TBool aShow )
	{
	iSubItems[aIndex].iIsShow = aShow;
	}

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
	
TCommonBgCtrlSubItem& CAknFepCtrlCommonBgCtrl::GetSubBgItem( TInt aIndex )	
	{
	return iSubItems[aIndex];	
	}
//  End Of File
