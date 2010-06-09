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
#include <AknBidiTextUtils.h>

#include "peninputlayoutbubblectrl.h"
#include "peninputlayout.h"
#include <peninputpluginutils.h>
#include <imageconversion.h> 
#include "peninputcmd.h"

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
	delete iBitmap;
	delete iMaskBitmap; 
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
        /*
        if(UiLayout()->NotDrawToLayoutDevice())
            {
            struct SData
                {
                TRect rr;
                TBool flag;
                } data;
            data.rr = aRect;
            data.flag = ETrue;
            TPtrC ptr;
            ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
        
            UiLayout()->SignalOwner(ESignalPopupArea,ptr);
            }  
        */
        return;
        }
    if( !iShowing || iNeedRedraw )
        {
        iShowing = ETrue;  
        TRect rect = Rect();
        //SetRect(aRect);
        SetHidenFlag(EFalse);        
        BringToTop();
        
        /*
        //redraw the control under bubble control
        if(aRect != rect)        
            {
            // signal the server to remove the pop area added before since the rect 
			// has been changed
		    if(UiLayout()->NotDrawToLayoutDevice())
		        {
		        struct SData
		            {
		            TRect rr;
		            TBool flag;
		            } data;
            
		        data.rr = rect;
		        data.flag = EFalse;
		        TPtrC ptr;
        
		        ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
		        UiLayout()->SignalOwner(ESignalPopupArea,ptr);
		        }
		    else
			    {
				//redraw the control under bubble control
				RootControl()->ReDrawRect(rect); 
				}
            }
            */
		/*
        // signal server to add the pop area		
        if(UiLayout()->NotDrawToLayoutDevice())
            {
            struct SData
                {
                TRect rr;
                TBool flag;
                } data;
                
            data.rr = aRect;
            data.flag = ETrue;
            TPtrC ptr;
            ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
    
            UiLayout()->SignalOwner(ESignalPopupArea,ptr);
            }
            */
        
		// signal server to copy the background as background bitmap for its own bitmap
        if(UiLayout()->NotDrawToLayoutDevice())
            {
            //copy backgroud to bitmap. DrawFrame seems to have transparent effect, 
            //so we need copy the background first.  
            if(!iBitmap)
                {
                TRAP_IGNORE(CreateBmpDevL());
                }
        
            CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
            gc->Activate( BitmapDevice() ); 
            CFbsBitGc* layoutGc = static_cast<CFbsBitGc*>(
                                UiLayout()->LayoutOwner()->BitmapContext()); 
            gc->BitBlt(TPoint(0,0),*layoutGc,Rect());
            
            //ensure other pop up draws correctly on the background
            struct SData
                {
                CFbsBitGc* gc;
                CFbsBitmap* bmp;
                TRect rect;
                } data;
            
            data.gc = gc;
            data.bmp = iBitmap;
            data.rect = Rect();
            TPtrC ptr;
            ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
            
            UiLayout()->SignalOwner(ESignalDrawBackground,ptr);
            }

        Draw();
        
		// notify sever to draw the bubble bitmap on the screen
        if(UiLayout()->NotDrawToLayoutDevice())
            {
            struct SData
                {
                TBool flag;
                TRect pos;
                CFbsBitmap* bmp;
                CFbsBitmap* mask;
                } data;
                
            data.flag = ETrue;
            data.pos = Rect();
            data.bmp = iBitmap;
            data.mask = iMaskBitmap;
            TPtrC ptr;
            ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));            
            UiLayout()->SignalOwner(ESignalUpdateBubble,ptr);
            }
        else
            {
            rect.BoundingRect(aRect);
            UpdateAreaImmed(rect,ETrue);     
            }   
        
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
    if(UiLayout()->NotDrawToLayoutDevice())
	{
	SetHidenFlag(ETrue);
    struct SData
        {
        TRect rr;
        TBool flag;
        } data;
    data.rr = rect;
    data.flag = EFalse;
    TPtrC ptr;
    ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));

    UiLayout()->SignalOwner(ESignalPopupArea,ptr);
	}

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
        if(UiLayout()->NotDrawToLayoutDevice())
            {
            struct SData
                {
                TBool flag;
                TRect pos;
                CFbsBitmap* bmp;
                } data;
                
            data.flag = EFalse;
            data.pos = Rect();
            data.bmp = iBitmap;
            
            TPtrC ptr;
            ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
            UiLayout()->SignalOwner(ESignalUpdateBubble,ptr);
            
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
    if(UiLayout()->NotDrawToLayoutDevice() && !iBitmap)
        {
        TRAP_IGNORE(CreateBmpDevL());
        }
		
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    //mask bitmaps
  	DrawOpaqueMaskBackground();  
  	
    TRect rect = GetRect();          
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
	
    	AknsDrawUtils::DrawFrame( UiLayout()->SkinInstance(), //UiLayout()->SkinInstace()
                         *gc,
                         rect,
                         innerRect,
                         iBgSkinId,
                         KAknsIIDDefault ); 	        
	    }
	else if( iFirstBmpId.iMajor && iMiddleBmpId.iMajor && iLastBmpId.iMajor )
		{
		AknPenInputDrawUtils::Draw3PiecesFrame( UiLayout()->SkinInstance(),
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
        TRect iconRect;
        iconRect.iTl = TPoint( innerRect.iTl.iX + iIconOffset.iWidth, 
                innerRect.iTl.iY + iIconOffset.iHeight );
        iconRect.SetWidth( iIconSize.iWidth );
        iconRect.SetHeight( iIconSize.iHeight );
        
        if( iForgroundBmp->SizeInPixels() != iconRect.Size() )
        	{
        	AknIconUtils::SetSize( iForgroundBmp, iconRect.Size(), EAspectRatioNotPreserved );
        	}
        	
    	TRect srcRect( TPoint( 0, 0 ), iForgroundBmp->SizeInPixels() );
    	
    	if( iForgroundBmpMask )
    		{
            if( iForgroundBmpMask->SizeInPixels() != iconRect.Size() )
            	{
            	AknIconUtils::SetSize( iForgroundBmpMask, iconRect.Size(), EAspectRatioNotPreserved);
            	}
            	        
            gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    		gc->BitBltMasked( iconRect.iTl, 
    						  iForgroundBmp, 
    						  srcRect,
    						  iForgroundBmpMask,
    						  EFalse);
    		}
        else
            {
    		gc->BitBlt( iconRect.iTl,
    					iForgroundBmp,
    					srcRect );
            }            
        }
        
    if ( iText && iText->Length() > 0 )
        {
        gc->SetBrushStyle( CGraphicsContext::ENullBrush );
        TAknLayoutText textLayout;
        textLayout.LayoutText(GetRect(), iTextFormat);
        TRgb color( KRgbBlack );  // sane default for nonskinned case
	    if ( AknsUtils::AvkonSkinEnabled() )
	        {
	        AknsUtils::GetCachedColor( UiLayout()->SkinInstance(),
	                               color, KAknsIIDQsnTextColors, iTextColorIndex );
	        }
         if ( iLangCode == ELangArabic ||
              iLangCode == ELangHebrew ||
              iLangCode == ELangFarsi  ||
              iLangCode == ELangUrdu )
             {
              const CFont* font = textLayout.Font();
              HBufC* visualBuf = HBufC::New( iText->Length() + KAknBidiExtraSpacePerLine );
              *visualBuf = *iText;
              TPtr buf = visualBuf->Des();

              AknBidiTextUtils::ConvertToVisualAndClip(*iText, buf, *font,
                                          textLayout.TextRect().Size().iWidth,
                                          textLayout.TextRect().Size().iWidth,
                                          AknBidiTextUtils::ERightToLeft );
              textLayout.DrawText(*gc, buf, EFalse, color);
              delete visualBuf;
              visualBuf = NULL;
             }
         else
             {
             textLayout.DrawText(*gc, *iText, EFalse, color);
             }
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

TRect CBubbleCtrl::GetRect()
    {
    TRect r(Rect());
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        r.Move(-Rect().iTl.iX, -Rect().iTl.iY);
        }
    return r;
    }


void CBubbleCtrl::SetBmpDevice(CFbsBitGc* aGc,CFbsBitmapDevice* aDevice)
    {
    TAny* extension;
   
    if(KErrNone == Extension_(KFepCtrlExtId,extension,0))
        {
        static_cast<CFepUiBaseCtrl::CFepUiBaseCtrlExtension*>(extension)->SetGc(aGc);
        static_cast<CFepUiBaseCtrl::CFepUiBaseCtrlExtension*>(extension)->SetBmpDevice(aDevice);
        }
    }
void CBubbleCtrl::CreateBmpDevL()
    {
    iBitmap = new ( ELeave ) CFbsBitmap;   
    //iMaskBitmap = new ( ELeave ) CFbsBitmap;
    CreateOwnDeviceL(iBitmap,0);
    }

EXPORT_C void CBubbleCtrl::HandleResourceChange(TInt aType)
    {
    if(aType == KPenInputOwnDeviceChange)
        {
        if(UiLayout()->NotDrawToLayoutDevice() && !iBitmap)
            {
            TRAP_IGNORE(CreateBmpDevL());
            }
        }
    else
        CFepUiBaseCtrl::HandleResourceChange(aType);
    }

EXPORT_C void CBubbleCtrl::SetRect(const TRect& aRect)
    {
    if( aRect == Rect())
        {
        return;
        }
    else
        {
        if( UiLayout()->NotDrawToLayoutDevice())
            {
            struct SData
                {
                TRect rr;
                TBool flag;
                } data;
        
            // Remove the original region from list
            data.rr = Rect();
            data.flag = EFalse;
            TPtrC ptrForRemove;
            ptrForRemove.Set( 
                    reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
            UiLayout()->SignalOwner( ESignalPopupArea, ptrForRemove );
            
            // Add the new region to list
            data.rr = aRect;
            data.flag = ETrue;
            TPtrC ptrForAdd;
            ptrForAdd.Set( 
                    reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
            UiLayout()->SignalOwner( ESignalPopupArea, ptrForAdd );
            }
        }
    CFepUiBaseCtrl::SetRect(aRect);
    ResizeDeviceL();
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
		SetTactileFeedbackType(ETouchFeedbackSensitiveInput);
        UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(this),aRect,ETouchFeedbackSensitiveInput);
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
