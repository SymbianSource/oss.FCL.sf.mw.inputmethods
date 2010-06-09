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
* Description:  Implementation for pop up window
*
*/

#include "peninputlayoutrootctrl.h"
#include "peninputlayoutpopupwnd.h"
#include "peninputlayout.h"
#include <coemain.h>
#include <coecntrl.h>
#include "peninputcmd.h"
// ============================ MEMBER FUNCTIONS =============================

// Implementation of Class CFepLayoutPopupWnd

// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::CFepLayoutPopupWnd
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
EXPORT_C CFepLayoutPopupWnd::CFepLayoutPopupWnd(const TSize& /*aSize*/,
                                    CFepUiLayout* aUiLayout,TInt aControlId)
                            : CControlGroup(aUiLayout, aControlId), iIsShowing(EFalse)
	{
	SetControlType(ECtrlPopupWindow | ECtrlControlGroup);
	Hide(ETrue);
	}
	
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::~CFepLayoutPopupWnd
// Destructor
// ---------------------------------------------------------------------------
EXPORT_C CFepLayoutPopupWnd::~CFepLayoutPopupWnd()
	{
	if(iIsShowing) // is showing
		{		
		//do nothing
		}
    delete iExt;
	}	
	
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::ConsructL()
// Symbian 2nd phase constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
EXPORT_C void CFepLayoutPopupWnd::ConstructL()
	{
	BaseConstructL();
    iExt = CPopupWndExt::NewL();    
    //check whether we need create backup bmp. Some pop up window is created after UI layout is created
    //If a pop up window is created during layout constructing, it will create the backup bitmap in Handleresourcechange
    if (UiLayout()->NotDrawToLayoutDevice())
        {
            iExt->ContructBkDeviceL(Rect().Size(),
                        UiLayout()->LayoutOwner()->BitmapDevice()->DisplayMode());
        }
	}	

EXPORT_C void CFepLayoutPopupWnd::ReDrawRect(const TRect& /*aRect*/)
    {
    Draw();
    }
    

/*

                                                   
Four directions description                                                 
                                                                            
                                                                            
                       +-----------+     +-----------+                      
                       |           |     |           |                      
  EDispTopLeft;        | PopupWnd  |     | PopupWnd  |  EDispTopRight;      
                       |           |     |           |                      
                       +-------+---+     +---+---+---+                      
                               |   |     |   |                              
                               +---+     +---+                              
                                                                            
                               +---+     +---+                              
                               |   |     |   |                              
                       +-------+---+     +---+-------+                      
                       |           |     |           |                      
  EDispBottomLeft;     | PopupWnd  |     | PopupWnd  |  EDispBottomRight;   
                       |           |     |           |                      
                       +-------+---+     +-------+---+                      
                                                                            
                                                                      
The method to determine the direction                                 
                                                                      
    1.Convert coordinates to screen based                             
    2.Determine left-right direction                                  
      2.1 First consider screen                                       
      2.2 if both left & right are in screen, consider layout         
      2.3 if both left & right are neigher in screen nor in layout,   
          use the less difference                                     
    3.Determine top-bottom direction                                  
                                                                      
                                                                      

*/	
CFepLayoutPopupWnd::TDisplayPosition CFepLayoutPopupWnd::DeterminePosition(const TRect& aSrcRect)
    {
	TBool down = ETrue;
	TBool right = ETrue;
	
	TSize szPopup = iRect.Size();
	TRect rtLayout = UiLayout()->Rect();
	TPoint ptLayout = UiLayout()->Position();
	rtLayout.Move( ptLayout );
	
    TPixelsTwipsAndRotation ptScreenSize;             
    CCoeEnv::Static()->ScreenDevice()->
                            GetDefaultScreenSizeAndRotation(ptScreenSize);
    TRect rtScreen (TPoint(), ptScreenSize.iPixelSize );
    
    //determine right or left
    TInt rsRight = aSrcRect.iTl.iX + ptLayout.iX + szPopup.iWidth;
    TInt lsLeft = aSrcRect.iBr.iX + ptLayout.iX - szPopup.iWidth;
    
    do 
        {
        if( rsRight > rtScreen.iBr.iX && lsLeft > 0 )
            {
            right = EFalse;
            break;
            }

        if( rsRight < rtScreen.iBr.iX && lsLeft < 0 )
            {
            right = ETrue;
            break;
            }
            
        if( rsRight > rtScreen.iBr.iX && lsLeft < 0 )
            {
            if( rsRight - rtScreen.iBr.iX < 0 - lsLeft )
                {
                right = ETrue;
                }
            else
                {
                right = EFalse;
                }
            break;
            }
           
        if( rsRight < rtLayout.iBr.iX )
            {
            right = ETrue;
            break;
            }
            
        if( lsLeft > rtLayout.iTl.iX )    
            {
            right = EFalse;
            break;
            }
        
        if( rtLayout.iTl.iX - lsLeft < rsRight - rtLayout.iBr.iX )    
            {
            right = EFalse;
            break;
            }
        
        right = ETrue;
        }while( EFalse );
    //determin down or up
    TInt dsBottom = aSrcRect.iBr.iY + ptLayout.iY + szPopup.iHeight;
    TInt usTop = aSrcRect.iTl.iY + ptLayout.iY - szPopup.iHeight;
    
    do 
        {
        if( dsBottom > rtScreen.iBr.iY && usTop > 0 )
            {
            down = EFalse;
            break;
            }

        if( dsBottom < rtScreen.iBr.iY && usTop < 0 )
            {
            down = ETrue;
            break;
            }
            
        if( dsBottom > rtScreen.iBr.iY && usTop < 0 )
            {
            if( dsBottom - rtScreen.iBr.iY < 0 - usTop )
                {
                down = ETrue;
                }
            else
                {
                down = EFalse;
                }
            break;
            }
           
        if( dsBottom < rtLayout.iBr.iY )
            {
            down = ETrue;
            break;
            }
            
        if( usTop > rtLayout.iTl.iY )    
            {
            down = EFalse;
            break;
            }
        
        if( rtLayout.iTl.iY - usTop < dsBottom - rtLayout.iBr.iY )    
            {
            down = EFalse;
            break;
            }
        
        down = ETrue;
        }while( EFalse );

    TDisplayPosition disPostion;
    if( down )
        {
        if( right )
            {
            disPostion = EDispBottomRight;
            }
        else
            {
            disPostion = EDispBottomLeft;
            }
        }
    else
        {
        if( right )
            {
            disPostion = EDispTopRight;
            }
        else
            {
            disPostion = EDispTopLeft;
            }
        }
    return disPostion;
    }

// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::Display()
// display pop up window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
EXPORT_C void CFepLayoutPopupWnd::Display( const TRect& aOffsetRect, TDisplayPosition aDisPostion)
    {
	if(iIsShowing)
	    return;
	RootControl()->SetActiveCtrl(this);
    ReportEvent(EEventCapturePointer);

    //When preparing the pop up window, it causes redraw the underlying control sometimes.
    DisableDrawingInGroup();//disable any drawing during preparation to avoid it dirty the background

	//let pop up window do preparation
	OnDisplay();	
	
	//determine the directoin
	TRect rtPopup; //final position 
	TSize szPopup = iRect.Size();
	TRect rtLayout = UiLayout()->Rect();
	TPoint ptLayout = UiLayout()->Position();
	rtLayout.Move( ptLayout );


	TRect layoutRect = UiLayout()->Rect();//relative layout
	
	if( aDisPostion == EDispAutomatic )
	    {
        aDisPostion = DeterminePosition( aOffsetRect );
	    }
	    
    //reposition if the rect is out of screen   
    TPixelsTwipsAndRotation ptScreenSize;             
    CCoeEnv::Static()->ScreenDevice()->
                            GetDefaultScreenSizeAndRotation(ptScreenSize);

    TRect offsetRect( aOffsetRect );
    offsetRect.Move( ptLayout );
    
    switch( aDisPostion )
        {
        case EDispBottomRight:
            rtPopup.SetRect( TPoint( offsetRect.iTl.iX, offsetRect.iBr.iY ), szPopup );            
            if( rtPopup.iBr.iY > ptScreenSize.iPixelSize.iHeight )
                {
                rtPopup.Move( 0, ptScreenSize.iPixelSize.iHeight - rtPopup.iBr.iY );
                }            
            if(rtPopup.iBr.iX > ptScreenSize.iPixelSize.iWidth )
                {
                rtPopup.Move ( ptScreenSize.iPixelSize.iWidth - rtPopup.iBr.iX, 0 );
                }
            break;
        case EDispBottomLeft:
            rtPopup.SetRect( TPoint( offsetRect.iBr.iX - szPopup.iWidth, offsetRect.iBr.iY), 
                             szPopup );
            if( rtPopup.iBr.iY > ptScreenSize.iPixelSize.iHeight )
                {
                //we need move the pop wnd up to ensure it not to be out of screen
                rtPopup.Move( 0, ptScreenSize.iPixelSize.iHeight - rtPopup.iBr.iY );
                }
            if( rtPopup.iTl.iX < 0 )
                {
                rtPopup.Move ( -rtPopup.iTl.iX, 0 );
                }
            break;
        case EDispTopRight:
            rtPopup.SetRect( TPoint( offsetRect.iTl.iX, offsetRect.iTl.iY - szPopup.iHeight ), 
                             szPopup );            
            if( rtPopup.iTl.iY < 0 )
               {
               rtPopup.Move ( 0, -rtPopup.iTl.iY );
               }                       
            if(rtPopup.iBr.iX > ptScreenSize.iPixelSize.iWidth )
                {
                rtPopup.Move ( ptScreenSize.iPixelSize.iWidth - rtPopup.iBr.iX, 0 );
                }
               
            break;
        case EDispTopLeft:
            rtPopup.SetRect( TPoint( offsetRect.iBr.iX - szPopup.iWidth, 
                                     offsetRect.iTl.iY - szPopup.iHeight ), szPopup );
                                     
            if( rtPopup.iTl.iY < 0 )
               {
               rtPopup.Move ( 0, -rtPopup.iTl.iY );
               }
               
            if( rtPopup.iTl.iX < 0 )
                {
                rtPopup.Move ( -rtPopup.iTl.iX, 0 );
                }
            break;
        }
                
    MLayoutOwner* layoutOwner = UiLayout()->LayoutOwner();
    
	//calculate the bound rect of current layout + popup window in screen coordination
	TRect totalRect(rtPopup);

	totalRect.BoundingRect(rtLayout);

    TRect rtFinal( totalRect );
    
	rtPopup.Move( -ptLayout );
	if( rtFinal != rtLayout )
	    {//resize
	    iPrevLayoutRect = rtLayout;
	    iLayoutMoveOff = rtLayout.iTl - rtFinal.iTl;
	    rtPopup.Move( iLayoutMoveOff );
        layoutOwner->LayoutSizeChanged(rtFinal.Size(), EFalse);
        layoutOwner->SetPosition(rtFinal.iTl);
        //RootControl()->Clear();
        DrawTransparentMaskBackground( TRect( TPoint(), rtFinal.Size() ) );
        RootControl()->Move(iLayoutMoveOff);
        EnableDrawingInGroup();
        RootControl()->Draw();
        DisableDrawingInGroup();	    
    	//UpdateArea( TRect( TPoint(), rtFinal.Size()), ETrue); 	
	    }
	else
	    {
	    iPrevLayoutRect.SetSize(TSize(0,0));	        
	    }
	        
    //capture pointer
    iPreCaptureCtrl= CapturePointer();

	if(iWndControl)
	    {
	    iWndControl->MakeVisible(ETrue);    
	    }
	    
    //iRect = rtPopup;
    Move( rtPopup.iTl - Rect().iTl);
    //copy and save the bitmap 
    if (UiLayout()->NotDrawToLayoutDevice())
        {    
        iExt->SetRect(Rect());
        
        CFbsBitGc* gc = iExt->BackupGc();
        gc->Activate( iExt->BackupDevice() );
        CFbsBitGc* layoutGc = static_cast<CFbsBitGc*>(
                            UiLayout()->LayoutOwner()->BitmapContext()); 
        gc->BitBlt(TPoint(0,0),*layoutGc,Rect());
        
        Hide(EFalse); 
        }
    
    EnableDrawingInGroup();

    UiLayout()->LockArea(iRect,this);

    SetReady(ETrue);//enalbe it as in Close, the flag is reset.
    if (UiLayout()->NotDrawToLayoutDevice())
        Draw();//force draw
    else
        Hide(EFalse);
    UpdateArea(iRect, EFalse); 
        
    iIsShowing = ETrue;
    AfterDisplayed();
    
	TRect rectOfPopup =Rect(); //in new layout coord-sys
	TRect rectTrimLayout = RootControl()->Rect(); // in new layout coord-sys
	
	TInt rectsz = sizeof(TRect)/sizeof(TUint16);
	RBuf buf;
	buf.Create( rectsz * 2 );
	buf.Append( reinterpret_cast<const TUint16*>(&rectOfPopup), rectsz );
	buf.Append( reinterpret_cast<const TUint16*>(&rectTrimLayout), rectsz );
	UiLayout()->SignalOwner(ESignalPopupWnd, buf );
	buf.Close();
    }
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::Display()
// display pop up window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
EXPORT_C void CFepLayoutPopupWnd::Display( const TRect& aOffsetRect )
    {
        Display( aOffsetRect, EDispAutomatic);
    }

// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::Display()
// display pop up window
// (other items were commented in a header).
// ---------------------------------------------------------------------------    
EXPORT_C void CFepLayoutPopupWnd::Display(const TPoint& aOffset)
	{
	Display ( TRect( aOffset, aOffset ) , EDispAutomatic);	
	}		

// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::CloseWindow()
// display window
// ---------------------------------------------------------------------------
EXPORT_C void CFepLayoutPopupWnd::CloseWindow()
	{
	if( !iIsShowing )
	    {
	    return;
	    }
	
    if(iPreCaptureCtrl)
        iPreCaptureCtrl->CapturePointer();
    else
        CapturePointer(EFalse);   	
	UiLayout()->SignalOwner(ESignalPopupWndClosed); 		    
	
	iIsShowing = EFalse;
	if(iWndControl)
	    iWndControl->MakeVisible(EFalse);    
    SetReady(EFalse);//disable redraw
    TRect popRect = iRect;
    UiLayout()->UnLockArea(iRect, this);

    //restore the background bitmap
    TBool bRedraw = ETrue;
    if (UiLayout()->NotDrawToLayoutDevice())
        {
        CFbsBitGc* layoutGc = static_cast<CFbsBitGc*>(
                    UiLayout()->LayoutOwner()->BitmapContext()); 
 
                iExt->BackupGc();
        layoutGc->Activate( UiLayout()->LayoutOwner()->BitmapDevice());
        TRect r;
        r.SetSize(iExt->Rect().Size());
        layoutGc->BitBlt(iExt->Rect().iTl,iExt->BackupBitmap(),r);
        bRedraw = EFalse;
        }
    
    Hide(ETrue);
	UiLayout()->UnLockArea(iRect,this);
	iRect.Move(-iRect.iTl);	
	MLayoutOwner* layoutOwner = UiLayout()->LayoutOwner();
	if(!iPrevLayoutRect.IsEmpty())
	    {
	    RootControl()->Move(-iLayoutMoveOff);
	    TRect rt;
	    rt.SetSize( layoutOwner->LayoutRect().Size() );
        layoutOwner->SetPosition(iPrevLayoutRect.iTl);
       	layoutOwner->LayoutSizeChanged(iPrevLayoutRect.Size());
        RootControl()->GraphicDeviceSizeChanged();
        bRedraw = ETrue;
	    }
	    
    if (bRedraw)    
	    {
        RootControl()->Draw();
		}

	iIsShowing = EFalse;
	
    //TRect rect(TPoint(0, 0), iPrevLayoutRect.Size());
    UpdateArea(popRect, EFalse);     	
	}		
		
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::SetRect
// Set the pop up window's rect
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepLayoutPopupWnd::SetRect(const TRect& aRect)
    {    
    iRect = aRect;
    //Ensure that the top left of pop up window is always (0,0)
    iRect.Move(-aRect.iTl.iX,-aRect.iTl.iY);    
    }
	
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::OnDeActivate
// DeActivate popup window
// ---------------------------------------------------------------------------
//  
EXPORT_C void CFepLayoutPopupWnd::OnDeActivate()	
    {
    CloseWindow();
    }
    
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::OnDisplay
// Prepare for window displaying
// ---------------------------------------------------------------------------
//    	
EXPORT_C void CFepLayoutPopupWnd::OnDisplay()
    {
    //do nothing
    }
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::AfterDisplayed
// Notify pops up window displayed
// ---------------------------------------------------------------------------
//    	
EXPORT_C void CFepLayoutPopupWnd::AfterDisplayed()
    {
    //do nothing
    }
EXPORT_C void CFepLayoutPopupWnd::HandleResourceChange(TInt aType)
    {
    if (aType == KPenInputOwnDeviceChange)
        {
        if (UiLayout()->NotDrawToLayoutDevice())
            {
            TRAP_IGNORE(iExt->ContructBkDeviceL(Rect().Size(),
                    UiLayout()->LayoutOwner()->BitmapDevice()->DisplayMode()));
            }
        }
    else
        CControlGroup::HandleResourceChange(aType);
    }
void CFepLayoutPopupWnd::DisableDrawingInGroup()
    {

    UiLayout()->DisableLayoutDrawing(ETrue);
    SetReady(EFalse);
    for(TInt i = 0; i < NumOfControls(); ++i)
        {
        //There is potential defect that if the child control is a group control, its children not set.
        //This can be solved by locking the area so no one can update until we unlock it in Enalbegroup
        ControlList()[i]->SetReady(EFalse); 
        }
    }
void CFepLayoutPopupWnd::EnableDrawingInGroup()
    {
    SetReady(ETrue);
    for(TInt i = 0; i < NumOfControls(); ++i)
        {
        //There is potential defect that if the child control is a group control, its children not set.
        //This can be solved by locking the area so no one can update until we unlock it in Enalbegroup
        ControlList()[i]->SetReady(ETrue); 
        }
    UiLayout()->DisableLayoutDrawing(EFalse);    
    }



CFepLayoutPopupWnd::CPopupWndExt* CFepLayoutPopupWnd::CPopupWndExt::NewL()
    {
    CPopupWndExt* self = new (ELeave)CPopupWndExt();
    return self;
    }

void CFepLayoutPopupWnd::CPopupWndExt::ContructBkDeviceL(const TSize& aSize,const TDisplayMode& aMode)
    {
    TBool bNewBmp = EFalse;
    if(!iBitmap || iBitmap->DisplayMode() != aMode)
        {
        delete iBitmap;
        iBitmap = 0;
        iBitmap = new (ELeave) CFbsBitmap;    
        User::LeaveIfError( iBitmap->Create( aSize,aMode));
        delete iBitmapDevice;
        iBitmapDevice = 0;
        iBitmapDevice = CFbsBitmapDevice::NewL(iBitmap);
        bNewBmp = ETrue;
        }
    if(!iGc)
        iGc = CFbsBitGc::NewL();    

    if(!bNewBmp && aSize != iBitmap->SizeInPixels())
        {
        ResizeBackupDeviceL(aSize);
        }
    }
void CFepLayoutPopupWnd::CPopupWndExt::ResizeBackupDeviceL(const TSize& aSize)
    {
    
    iBitmap->Resize(aSize);
    iBitmapDevice->Resize( aSize);
    iGc->Activate(iBitmapDevice);
    iGc->Resized();    
    }
void CFepLayoutPopupWnd::CPopupWndExt::SetRect(const TRect& aRect)
    {
    iRect = aRect;
    if(iBitmap && iBitmap->SizeInPixels() != aRect.Size())
        {            
        ResizeBackupDeviceL(aRect.Size());
        }
    }
//  End of File  
