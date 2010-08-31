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
* Description:  Implementation for PeninputLayoutControl
*
*/


#include <AknIconUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h> 

#include "peninputlayoutbubblectrl.h"
#include "peninputlayout.h"
#include <peninputpluginutils.h>

const TInt KShrinkSize = 10;
const TInt KDefaultTextColorIndex = EAknsCIQsnTextColorsCG60;
// ======== MEMBER FUNCTIONS ========

// Implementation of Class CBubbleCtrl

// ---------------------------------------------------------------------------
// CBubbleCtrl::NewL
// Factory function
// ---------------------------------------------------------------------------
//
               
EXPORT_C CBubbleCtrl* CBubbleCtrl::NewL(const TRect& aRect,
                               CFepUiLayout* aUiLayout,
                               TInt aCtrlId)
    {
    CBubbleCtrl* ctrl = new (ELeave)CBubbleCtrl(aRect,aUiLayout,aCtrlId);
    CleanupStack::PushL(ctrl);
    ctrl->BaseConstructL();
    CleanupStack::Pop(ctrl);
    return ctrl;
    }

EXPORT_C CBubbleCtrl::CBubbleCtrl(const TRect& aRect,
                         CFepUiLayout* aUiLayout,
                         TInt aCtrlId)
    :CFepUiBaseCtrl(aRect,aUiLayout,aCtrlId),
     iForgroundBmp(NULL),
     iForgroundBmpMask(NULL),
     iShowing(EFalse),
     iTextColorIndex(KDefaultTextColorIndex)
    {
    SetControlType(ECtrlButton);
    SetHidenFlag(ETrue);
    }
        
EXPORT_C CBubbleCtrl::~CBubbleCtrl()
    {
    if ( iText )
        delete iText;
    
    if( iForgroundBmp )
		{
		delete iForgroundBmp;
		iForgroundBmp = NULL;
		}
	if( iForgroundBmpMask )
		{
		delete iForgroundBmpMask;
		iForgroundBmpMask = NULL;
		}    
    }

EXPORT_C void CBubbleCtrl::SetBitmapParam(CFbsBitmap* aBmpId,
                                 CFbsBitmap* aMaskBmpId,
                                 TAknsItemID aBgSkinId)
    {
    if( iForgroundBmp )
		{
		delete iForgroundBmp;
		iForgroundBmp = NULL;
		}
	if( iForgroundBmpMask )
		{
		delete iForgroundBmpMask;
		iForgroundBmpMask = NULL;
		}    
		
    iForgroundBmp = aBmpId;
    iForgroundBmpMask = aMaskBmpId;
    iBgSkinId = aBgSkinId;    
    }

EXPORT_C void CBubbleCtrl::Popup(const TRect& aRect)
    {
    SetRect(aRect);

    if(iFreeze)
        {
        iShowing = ETrue;              
        SetHidenFlag(EFalse);        
        return;
        }
    if(!iShowing || aRect != Rect() || iNeedRedraw)
        {
        iShowing = ETrue;  
        TRect rect = Rect();
        SetRect(aRect);
        SetHidenFlag(EFalse);        
        BringToTop();
        //redraw the control under bubble control
        if(aRect != rect)        
            RootControl()->ReDrawRect(rect);    

        Draw();
        
        rect.BoundingRect(aRect);
        UpdateAreaImmed(rect,ETrue);     
        
        iNeedRedraw = EFalse;
        }
    }
EXPORT_C void CBubbleCtrl::SetBitmapParam(CFbsBitmap* aBmpId,
                                 CFbsBitmap* aMaskBmpId,
                                 TAknsItemID aFirstBmpId,
		                         TAknsItemID aMiddleBmpId,
		                         TAknsItemID aLastBmpId)
    {
    if( iForgroundBmp )
		{
		delete iForgroundBmp;
		iForgroundBmp = NULL;
		}
	if( iForgroundBmpMask )
		{
		delete iForgroundBmpMask;
		iForgroundBmpMask = NULL;
		}    
		
    iForgroundBmp = aBmpId;
    iForgroundBmpMask = aMaskBmpId;
    iFirstBmpId = aFirstBmpId;
    iMiddleBmpId =  aMiddleBmpId;
    iLastBmpId = aLastBmpId;    
    }

EXPORT_C void CBubbleCtrl::Close()
    {
    TRect rect = Rect();
    
    iShowing = EFalse;  
    
    if( iFreeze )
        {
        if(iInvalidRect.IsEmpty() )
            {
            iInvalidRect = rect;
            }
        else
            {
            iInvalidRect.BoundingRect( rect );
            }            
        }
    else
        {
        Clear();
        SetHidenFlag(ETrue);    
        BringToBack();

        RootControl()->ReDrawRect(rect);
        UpdateAreaImmed(rect); 
        }
    }

EXPORT_C void CBubbleCtrl::Hide(TBool aFlag)
    {
    if(aFlag) //hide bubble
        {
        Close();            

        }
    else
        {
        Popup(Rect());        
        }
    }


EXPORT_C TBool CBubbleCtrl::IsShowing()
    {
    return iShowing;   
    }
            
EXPORT_C void CBubbleCtrl::Draw()
    {
    if(!AbleToDraw() || iFreeze ) //|| !iShowing )
	    {
	    return;
	    }
    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    //mask bitmaps
  	DrawOpaqueMaskBackground();  
  	
    TRect rect = Rect();        
    TRect innerRect = rect;
    
    if ( ( iLeftDiff == 0 ) && ( iTopDiff == 0 )
         && ( iRightDiff == 0 ) && ( iBottomDiff == 0 ) )
        {
        innerRect.Shrink( KShrinkSize, 0 );
        }
    else
        {
        innerRect.iTl.iX = innerRect.iTl.iX + iLeftDiff;
        innerRect.iTl.iY = innerRect.iTl.iY + iTopDiff;
        innerRect.iBr.iX = innerRect.iBr.iX - iRightDiff;
        innerRect.iBr.iY = innerRect.iBr.iY - iBottomDiff;
        
        }
    
    // ----- draw bitmaps -----
    gc->Activate( BitmapDevice() );                
    //gc->Clear(rect);
    		
	// Draw background
	if ( iBgSkinId.iMajor )
	    {
    	AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                         *gc,
                         rect,
                         innerRect,
                         iBgSkinId,
                         KAknsIIDDefault ); 	        
	    }
	else if( iFirstBmpId.iMajor && iMiddleBmpId.iMajor && iLastBmpId.iMajor )
		{
		AknPenInputDrawUtils::Draw3PiecesFrame( AknsUtils::SkinInstance(),
						 *gc,
						 rect,
						 innerRect,
						 iFirstBmpId,
						 iMiddleBmpId,
						 iLastBmpId);	
		}
	else
	    {
	    DrawBackground();    
	    }
                     
    if ( iForgroundBmp )
        {
        if( iForgroundBmp->SizeInPixels() != innerRect.Size() )
        	{
        	AknIconUtils::SetSize( iForgroundBmp, innerRect.Size(), EAspectRatioNotPreserved );
        	}
        	
    	TRect srcRect( TPoint( 0, 0 ), iForgroundBmp->SizeInPixels() );
    	
    	if( iForgroundBmpMask )
    		{
            if( iForgroundBmpMask->SizeInPixels() != innerRect.Size() )
            	{
            	AknIconUtils::SetSize( iForgroundBmpMask, innerRect.Size(), EAspectRatioNotPreserved);
            	}
            	        

    		gc->BitBltMasked( innerRect.iTl, 
    						  iForgroundBmp, 
    						  srcRect,
    						  iForgroundBmpMask,
    						  EFalse);
    		}
        else
            {
    		gc->BitBlt( innerRect.iTl,
    					iForgroundBmp,
    					srcRect );
            }            
        }
        
    if ( iText )
        {
        gc->SetBrushStyle( CGraphicsContext::ENullBrush );
        TAknLayoutText textLayout;
        textLayout.LayoutText(Rect(), iTextFormat);
        TRgb color( KRgbBlack );  // sane default for nonskinned case
	    if ( AknsUtils::AvkonSkinEnabled() )
	        {
	        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
	                               color, KAknsIIDQsnTextColors, iTextColorIndex );
	        }
	     textLayout.DrawText(*gc, *iText, EFalse, color);
        }
    }
           	
EXPORT_C void CBubbleCtrl::SetTextFormat(TAknTextLineLayout aTextFormat)
    {
    iTextFormat = aTextFormat;                
    }
    
EXPORT_C void CBubbleCtrl::SetTextL(const TDesC& aText)
    {
    delete iText;
    iText = NULL;
    iText = aText.AllocL();           
    iNeedRedraw = ETrue;
    }

EXPORT_C void CBubbleCtrl::SetFrameDiff(TInt aLeftDiff, TInt aTopDiff,
                                        TInt aRightDiff,TInt aBottomDiff)
    {
    iLeftDiff = aLeftDiff;
    iTopDiff  = aTopDiff;
    iRightDiff = aRightDiff;
    iBottomDiff = aBottomDiff;      
    }

void CBubbleCtrl::Freeze()
    {
    iFreeze = ETrue;
    iInvalidRect = TRect();
    }

void CBubbleCtrl::UnFreeze( TBool aUpdate )
    {
    iFreeze = EFalse;
    if( aUpdate )
        {
        if( !iInvalidRect.IsEmpty() )
            {
            RootControl()->ReDrawRect( iInvalidRect );
            }
        Draw();
        
        if(iInvalidRect.IsEmpty() )
            {
            iInvalidRect = Rect();
            }
        else
            {
            iInvalidRect.BoundingRect( Rect() );
            }      
                  
        UpdateArea(iInvalidRect); 
        }
    
    iInvalidRect = TRect();
    }

// ---------------------------------------------------------------------------
// CTooltipBubbleCtrl::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CTooltipBubbleCtrl* CTooltipBubbleCtrl::NewL( const TRect& aRect, 
		                                            CFepUiLayout* aUiLayout, 
		                                            TInt aCtrlId )
    {
    CTooltipBubbleCtrl* self = new (ELeave) CTooltipBubbleCtrl( aRect, 
    		                                                    aUiLayout, 
    		                                                    aCtrlId );
    CleanupStack::PushL( self );
    self->BaseConstructL();
    CleanupStack::Pop( self );
    return self;    
    }
    
// ---------------------------------------------------------------------------
// CTooltipBubbleCtrl::CTooltipBubbleCtrl
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CTooltipBubbleCtrl::CTooltipBubbleCtrl( const TRect& aRect,
		                                      CFepUiLayout* aUiLayout, 
		                                      TInt aCtrlId )
    :CBubbleCtrl( aRect, aUiLayout, aCtrlId )
    {
#ifdef RD_TACTILE_FEEDBACK
    if(IsKindOfControl(ECtrlTactileFeedback) && !Hiden() && UiLayout())
        {
		SetTactileFeedbackType(ETouchFeedbackSensitiveKeypad);
        UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(this),aRect,ETouchFeedbackSensitiveKeypad);
        }  
#endif // RD_TACTILE_FEEDBACK 
    }

// ---------------------------------------------------------------------------
// CTooltipBubbleCtrl::~CTooltipBubbleCtrl
// Destructor
// ---------------------------------------------------------------------------
//
CTooltipBubbleCtrl::~CTooltipBubbleCtrl()
    {  
    }
                 

// ---------------------------------------------------------------------------
// Handle pointer up event
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CTooltipBubbleCtrl::HandlePointerUpEventL
                                                  ( const TPoint& /*aPoint*/ )
	{
	ReportEvent( EEventTooltipPointerUp );
	return this;
	}

//end of implementation of class CVirtualKeyboard
