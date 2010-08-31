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
* Description:  Implementation for choice list control
*
*/



#include <AknsDrawUtils.h>
#include "peninputlayoutrootctrl.h"
#include "peninputlayoutpopupwnd.h"
#include "peninputlayout.h"
#include "peninputlayoutchoicelist.h"
#include <coecntrl.h>
#include <eiklbx.h>

//after display, freeze some time
const TInt KFreezePerioid = 400000; // 400ms
// ---------------------------------------------------------------------------
// CFepLayoutChoiceList::NewL
// Factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutChoiceList* CFepLayoutChoiceList::NewL(CFepUiLayout* aUiLayout,
                                                          TInt aControlId)
    {
    CFepLayoutChoiceList* self = new(ELeave) CFepLayoutChoiceList(aUiLayout, aControlId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CFepLayoutChoiceList::~CFepLayoutChoiceList
// Destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutChoiceList::~CFepLayoutChoiceList()
    {
    delete iFreezeTimer;
    delete iItemFocusBmp;
    delete iItemFocusBmpMask;
    iItemList.ResetAndDestroy();
    iItemList.Close();
    }
    
// ---------------------------------------------------------------------------
// CFepLayoutChoiceList::SetItemRect
// Set choice list item rectangle.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::SetItemRect(const TRect& aItemRect, const TRect& aFocusRect)
    {
    iItemRect = aItemRect;
    iItemFocusRect = aFocusRect;
    ReCalcLayout();    
    }

// ---------------------------------------------------------------------------
// CFepLayoutChoiceList::SetFocusBmp
// Set bitmap for focus.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::SetFocusBmp(CFbsBitmap *aItemFocusBmp)
    {
    delete iItemFocusBmp;
    iItemFocusBmp = aItemFocusBmp;
    }

// CFepLayoutChoiceList::SetFocusBmpMask
// Set mask bitmap for focus.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::SetFocusBmpMask(CFbsBitmap *aItemFocusBmpMask)
    {
    delete iItemFocusBmpMask;
    iItemFocusBmpMask = aItemFocusBmpMask;
    }

// CFepLayoutChoiceList::SetItemsL
// Set choice list items.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::SetItemsL(
                          const RPointerArray<CFepLayoutChoiceList::SItem>& aItemList)
    {
    //remove all items
    iItemList.ResetAndDestroy();
    for(int ii = 0; ii < aItemList.Count(); ++ii)
        {
        AddItemL(*aItemList[ii]);
        }
    }

// CFepLayoutChoiceList::AddItemL
// Append a choice list item.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::AddItemL(const CFepLayoutChoiceList::SItem& aItem)
    {
    CFepLayoutChoiceList::SItem* item;
    item = new(ELeave)CFepLayoutChoiceList::SItem;
    item->iCommand = aItem.iCommand;
    item->iText.Copy( aItem.iText );
    CleanupStack::PushL(item);
    iItemList.AppendL(item);
    CleanupStack::Pop(item);
    //ReCalcLayout();
    }

// CFepLayoutChoiceList::InsertItemL
// Append a choice list item.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::InsertItemL(TInt aPostion, 
                                                const CFepLayoutChoiceList::SItem& aItem)
    {
    CFepLayoutChoiceList::SItem* item;
    item = new(ELeave)CFepLayoutChoiceList::SItem;
    item->iCommand = aItem.iCommand;
    item->iText.Copy( aItem.iText );
    CleanupStack::PushL(item);
    iItemList.InsertL(item, aPostion);
    CleanupStack::Pop(item);
    //ReCalcLayout();
    }

// CFepLayoutChoiceList::RemoveItemByCommand
// Remove a choice list item by command.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::RemoveItemByCommand(TInt aCommand)
    {
    for(int ii = 0; ii < iItemList.Count(); ++ii)
        {
        if( iItemList[ii]->iCommand == aCommand )
            {
            RemoveItemByIndex(ii);
            }
        }
    //ReCalcLayout();
    }

// CFepLayoutChoiceList::RemoveItemByIndex
// Remove a choice list item by index.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::RemoveItemByIndex(TInt aIndex)
    {
    CFepLayoutChoiceList::SItem* item;
    item = iItemList[aIndex];
    iItemList.Remove(aIndex);
    delete item;
    //ReCalcLayout();
    }

// CFepLayoutChoiceList::RemoveItemByIndex
// Clear all choice list items.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::ClearItemsL()
    {
    iItemList.ResetAndDestroy();
    //ReCalcLayout();
    }

// CFepLayoutChoiceList::RemoveItemByIndex
// Clear all choice list items.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepLayoutChoiceList::ItemsCount()
    {
    return iItemList.Count();
    }

// CFepLayoutChoiceList::ItemByIndex
// Retrieve a choice list item by index.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C const CFepLayoutChoiceList::SItem* CFepLayoutChoiceList::ItemByIndex(TInt aIndex)
    {
    return iItemList[aIndex];
    }

// CFepLayoutChoiceList::ItemByCommand
// Retrieve a choice list item by item's command.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepLayoutChoiceList::FindCommand(TInt aCommand)
    {
    TInt ret = -1;
    for(int ii = 0; ii < iItemList.Count(); ++ii)
        {
        if( iItemList[ii]->iCommand == aCommand )
            {
            ret = ii;
            break;
            }
        }
    return ret;
    }

// Constructor
// 
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutChoiceList::CFepLayoutChoiceList(CFepUiLayout* aUiLayout,
                                                    TInt aControlId)
    :CFepLayoutPopupWnd(TSize(0,0), aUiLayout, aControlId)
    {
    SetControlType(ECtrlPopupChoiceList);
	iSubItemSkinID 		= KAknsIIDNone;
	iBackgroundSkinID   = KAknsIIDNone;
    }

// CFepLayoutChoiceList::HitTest
// Get item whose region contains the point.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepLayoutChoiceList::HitTest(const TPoint& aPoint)
    {
    if( !Rect().Contains(aPoint) )
        {
        return EListOutside;
        }
    if(iWndControl)
        {        
        return static_cast<CEikListBox*>(iWndControl)->CurrentItemIndex();
        }
    TInt index = (aPoint.iY - Rect().iTl.iY)/iItemRect.Height();
    if( index >= iItemList.Count() )
        {
        return EListOutside;
        }
    else
        {
        return index;
        }
    }

// CFepLayoutChoiceList::HandlePointerDownEventL
// Handle pointer down event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepLayoutChoiceList::HandlePointerDownEventL(const TPoint& aPoint)
    {
    if( iFreezeTimer && iFreezeTimer->IsActive() )
        {
        return this;
        }
    CFepUiBaseCtrl* ctrl = CFepLayoutPopupWnd::HandlePointerDownEventL(aPoint);
    if( ctrl )
        {
        return ctrl;
        }
    TInt index = HitTest(aPoint);
    if( index == EListOutside )
        {
        SetPointerDown(EFalse);
        CloseWindow();
        CFepLayoutChoiceList::SEvent event;
        event.iIndex = -1;
        event.iCommand = 0;
        ReportChoiceEvent(event);
		#ifdef RD_TACTILE_FEEDBACK
        if (UiLayout()->SupportTactileFeedback())
        	{
        	UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveKeypad, ETrue, EFalse);
        	}
		#endif //RD_TACTILE_FEEDBACK     
        }
    else
        {
        SetPointerDown( ETrue );      
        if(  index >= EListItemFirst &&  index != iCurFocusItem )
            {
            //TInt prev = iCurFocusItem;
            iCurFocusItem = index;
       		Draw();
            UpdateArea(Rect(),EFalse);
			#ifdef RD_TACTILE_FEEDBACK
            if (UiLayout()->SupportTactileFeedback())
            	{
            	UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveKeypad);
            	}
			#endif //RD_TACTILE_FEEDBACK
            }
        }
    return this;
    }

// CFepLayoutChoiceList::HandlePointerMoveEventL
// Handle pointer move event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepLayoutChoiceList::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    if( iFreezeTimer && iFreezeTimer->IsActive() )
        {
        return this;
        }
    CFepLayoutPopupWnd::HandlePointerMoveEventL(aPoint);
    if( PointerDown() )
        {
        TInt index = HitTest(aPoint);
        if( index != iCurFocusItem && index >= 0)
            {
            //TInt prev = iCurFocusItem;
            iCurFocusItem = index;
/*        
            if( prev != -1 )
                {
                DrawItem(prev, ETrue);
                }

            if( iCurFocusItem != -1 )
                {
                DrawItem(iCurFocusItem, ETrue);
                }
 */               
    #ifdef RD_TACTILE_FEEDBACK
        if (UiLayout()->SupportTactileFeedback())
            {
            if ( iLastSelIndex != iCurFocusItem) 
                {
                UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveKeypad);
                iLastSelIndex = iCurFocusItem;
                }
            }
    #endif //RD_TACTILE_FEEDBACK

           	Draw();
            UpdateArea(Rect(),EFalse);
            }
        }
    return this;
    }

// CFepLayoutChoiceList::HandlePointerUpEventL
// Handle pointer up event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepLayoutChoiceList::HandlePointerUpEventL(const TPoint& aPoint)
    {
    if( iFreezeTimer && iFreezeTimer->IsActive() || !PointerDown())
        {
        return this;
        }
    CFepUiBaseCtrl* ctrl = CFepLayoutPopupWnd::HandlePointerUpEventL(aPoint);
    
    TInt index = HitTest(aPoint);
    if( index >= EListItemFirst )
        {
        CFepLayoutChoiceList::SEvent event;
        event.iIndex = index;
        event.iCommand = iItemList[index]->iCommand;
        CloseWindow();
        ReportChoiceEvent(event);
        
		#ifdef RD_TACTILE_FEEDBACK
		if (UiLayout()->SupportTactileFeedback())
			{
			UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveKeypad, ETrue, EFalse);
			}
		#endif //RD_TACTILE_FEEDBACK	
        }
    else if( index == EListOutside )
    	{
    	SetPointerDown( EFalse );	
        }
    return this;
    }

// CFepLayoutChoiceList::DrawItem
// Draw a choice list item.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutChoiceList::DrawItem(TInt aIndex, TBool aErase)
    {
    //
    if(iWndControl)
        return;
    TRect rtItem;
    rtItem.iTl.iY = aIndex * iItemRect.Height();
    rtItem.iBr.iX = iItemRect.Width();
    rtItem.iBr.iY = rtItem.iTl.iY + iItemRect.Height();
    rtItem.Move(Rect().iTl);
    DrawItem(rtItem, *iItemList[aIndex], aErase, aIndex == iCurFocusItem );
    }

// CFepLayoutChoiceList::DrawItem
// Draw a choice list item.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutChoiceList::DrawItem(const TRect& aRect, const CFepLayoutChoiceList::SItem& aItem, 
                                    TBool aErase, TBool aFocus)
    {
    if(iWndControl)
        return;

    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    if( aErase )
        {
        SetBorderColor( BkColor() );
        //DrawChoiceListBackground( aRect );//, EFalse);
        }
    if( aFocus )
        {
        //draw focus bitmap
        TRect rtFocusRect(iItemFocusRect);
        TRect rtFocusInBmp(TPoint(0,0), iItemFocusRect.Size());

        rtFocusRect.Move(aRect.iTl);
        TRect rtInnerRect( rtFocusRect );
        rtInnerRect.Shrink( 5, 5 );
        
       	if( iSubItemSkinID.iMajor != EAknsMajorNone && 
       	    iSubItemSkinID.iMinor != EAknsMinorNone)
       		{
       		gc->Activate( BitmapDevice() ); 
			AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                         *gc, 
                         rtFocusRect, 
                         rtInnerRect,
                         iSubItemSkinID,
                         KAknsIIDDefault );       	
       		}
       	else
       		{
	        //mask bitmaps-------
	        gc->Activate( MaskBitmapDevice() );
	        if( iItemFocusBmpMask )
	            {
	            DrawBitmap(rtFocusRect, rtFocusInBmp, iItemFocusBmpMask,
	                       iItemFocusBmpMask->SizeInPixels() == rtFocusInBmp.Size());
	            }

	        //front bitmaps-------
	        gc->Activate( BitmapDevice() );
	        
	        if( iItemFocusBmp )
	            {
	            DrawBitmap(rtFocusRect, rtFocusInBmp, iItemFocusBmp,
	                       iItemFocusBmp->SizeInPixels() == rtFocusInBmp.Size());
	            }       			
       		}        
        }
    //draw text
    if (iFont)
        {
        gc->UseFont(iFont);

        gc->SetBrushStyle( CGraphicsContext::ENullBrush );
        gc->SetPenColor(iFontColor);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);

        gc->DrawText(aItem.iText, aRect, iBaseline, CGraphicsContext::ELeft, iMargin);

        gc->DiscardFont();
        }
    }

// CFepLayoutChoiceList::DrawChoiceListBackground
// Draw choice list background.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutChoiceList::DrawChoiceListBackground(const TRect& aRect)//, TBool aDrawBorder)
    {
 	if( iBackgroundSkinID.iMajor != EAknsMajorNone && 
 		iBackgroundSkinID.iMinor != EAknsMinorNone )
 		{	
 		//draw bitmap 		
		DrawOpaqueMaskBackground( aRect );
	    TRect rtInnerRect( aRect );
	    rtInnerRect.Shrink( 1, 1 );
	    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
	    
	    gc->Activate( BitmapDevice() ); 
		AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                     *gc, 
	                     aRect, 
	                     rtInnerRect,
	                     iBackgroundSkinID,
	                     KAknsIIDDefault );       
 		}
 	else
 		{
    	//draw bitmap 		
		DrawOpaqueMaskBackground();
	    //front bitmaps-------
	    DrawBackground();			
 		}	
    }

// CFepLayoutChoiceList::DrawBitmap
// Draw bitmap helper function.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutChoiceList::DrawBitmap(const TRect& aDestRect, const TRect& aSrcRect, 
                                      CFbsBitmap* aBmp, TBool aFast)
    {
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    if( aFast )
        {
         gc->BitBlt(aDestRect.iTl, aBmp, aSrcRect);
        }
    else
        {
        gc->DrawBitmap(aDestRect, aBmp, aSrcRect);
        }
    }

EXPORT_C void CFepLayoutChoiceList::ReportChoiceEvent(CFepLayoutChoiceList::SEvent& aEvent)
    {
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&aEvent), sizeof(aEvent));
    ReportEvent(EEventChoiceSelected, ptr);
    }

// CFepLayoutChoiceList::Draw
// Draw a choice list.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::Draw()
    {
    if(!AbleToDraw())
    	return;    
    
    SetBorderColor( PenColor() );

    DrawChoiceListBackground(Rect()); //draw with border

    if(iWndControl)
        {
        iWndControl->DrawNow();
        return;        
        }
    //draw background
    for(int ii = 0; ii < iItemList.Count(); ++ii)
        {
        DrawItem(ii, EFalse);
        }
    }

// CFepLayoutChoiceList::OnDisplay
// Prepare display.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::OnDisplay()
    {
    ReCalcLayout();
    }

// CFepLayoutChoiceList::ReCalcLayout
// Calculates the displaying rect.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutChoiceList::ReCalcLayout()
    {
    TRect poprect;
    poprect.iBr.iX = iItemRect.Width();
    poprect.iBr.iY = iItemRect.Height() * iItemList.Count();
    SetRect(poprect);
    }
    
// CFepLayoutChoiceList::SetSkinID
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepLayoutChoiceList::SetSkinID( TAknsItemID aSubItemID, 
									  TAknsItemID aBackgroundID )
	{
	iSubItemSkinID = aSubItemID;
	iBackgroundSkinID = aBackgroundID;
	}
	
// CFepLayoutChoiceList::CurrentSelectedIndex
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 	
EXPORT_C TInt CFepLayoutChoiceList::CurrentFocusedIndex( ) const
	{
	return iCurFocusItem;
	}	
	
// CFepLayoutChoiceList::SetCurrentSelectedIndex
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 	
EXPORT_C void CFepLayoutChoiceList::SetCurrentFocusedIndex(const TInt aIndex)
	{
	iCurFocusItem = aIndex;
	}	
TInt CFepLayoutChoiceList::FreezeCallBack( TAny *aPtr )
    {
    CFepLayoutChoiceList* pThis = (CFepLayoutChoiceList*)aPtr;
    pThis->iFreezeTimer->Cancel();
    return 0;
    }

void CFepLayoutChoiceList::AfterDisplayedL()
    {
    if( !iFreezeTimer )
        {
        iFreezeTimer = CPeriodic::NewL(CActive::EPriorityStandard);
        }
    if( !iFreezeTimer->IsActive() )
        {
        iFreezeTimer->Start(KFreezePerioid, 
                            KFreezePerioid, 
                            TCallBack(FreezeCallBack, this));  
        }
    }

EXPORT_C void CFepLayoutChoiceList::AfterDisplayed()
    {
    TRAP_IGNORE(AfterDisplayedL());
    }
//End Of File
