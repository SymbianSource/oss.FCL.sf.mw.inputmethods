/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for button base and dragbar
*
*/


#include <AknsUtils.h>

#include "peninputlabel.h"
#include "peninputlayout.h"


// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlLabel* CAknFepCtrlLabel::NewL(CFepUiLayout* aUiLayout, TInt aCtrlId)
    {
    CAknFepCtrlLabel* lable=new (ELeave) CAknFepCtrlLabel(aUiLayout,aCtrlId);

    CleanupStack::PushL(lable);
    lable->BaseConstructL();
    CleanupStack::Pop(lable);

    return lable;    
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::CAknFepCtrlLabel
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepCtrlLabel::CAknFepCtrlLabel(CFepUiLayout* aUiLayout, TInt aCtrlId)
    :CFepUiBaseCtrl(TRect(0,0,0,0),aUiLayout,aCtrlId),
    iBaseline(-1),
    iFontColor(KRgbWhite),
    iAlignment(CGraphicsContext::ECenter)
    {
    SetControlType(EUserCtrlBase | ECtrlBaseCtrl);
    
#ifdef __ALPHA_TRANCEPARENCY__
    const TRgb KDefaultButtonBackCol = TRgb(30,30,30);
#else
    const TRgb KDefaultButtonBackCol = TRgb(230,230,230);
#endif
    
    SetBkColor(KDefaultButtonBackCol);
   }

// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::~CAknFepCtrlLabel
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlLabel::~CAknFepCtrlLabel()
    {  
    delete iText;
    
    //font
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }        
   }

// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::Draw
// Draw the control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlLabel::Draw()
    {
	if(!AbleToDraw())
        return;

    TRect rect = Rect();
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    gc->Activate(MaskBitmapDevice());

    //clear the rect
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetBrushColor(TRgb(KOpaqueColor));//non transparency
    gc->SetPenStyle(CGraphicsContext::ENullPen);
    gc->DrawRect(rect);

    gc->Activate(BitmapDevice());

    if (BackgroundBmp())
        {
        if (BkMaskBmp())
            {
            gc->DrawBitmapMasked(rect, BackgroundBmp(), BackgroundBmp()->SizeInPixels(), 
                                 BkMaskBmp(), ETrue);
            }
        else
            {
            gc->DrawBitmap(rect, BackgroundBmp(), BackgroundBmp()->SizeInPixels());
            }    
        }

    //draw caption if there is
    if(iText && iFont)
        {
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());        
        
        //use font
        gc->UseFont(iFont);
        gc->SetBrushStyle(CGraphicsContext::ENullBrush); 
        gc->SetPenStyle(CGraphicsContext::ESolidPen);                        
       
        TRect rect(iRect);
        
        //draw text shadow first
        rect.Move(1,1);
        
        TInt baseLine = iFont->AscentInPixels()/2 + iRect.Height()/2;
        gc->SetPenColor(iFontColor);              
        gc->DrawText(*iText, iRect, baseLine, iAlignment);
        
        gc->DiscardFont();
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::SetBitmapL
// Set bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlLabel::SetBitmapL(CFbsBitmap* aBitmap)
    {
    if(!aBitmap)
        {
        return;
        }
        
    SetBackgroundBitmapL(aBitmap);    
    }  
   
// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::SetMaskBitmapL
// Set mask bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlLabel::SetMaskBitmapL(CFbsBitmap* aBmp)
    {   
    if(!aBmp)
        {
        return;   
        }
        
    SetBackgroundMaskBitmapL(aBmp); 
    }
  
// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::SetFont
// Set caption font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CAknFepCtrlLabel::SetFont(const TFontSpec& aFontSpec)
    {
    iFontSpec = aFontSpec;
    //release old font
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }    
        
    iFontOwnership = ETrue;    
    if(BitmapDevice())
        {
        if (KErrNone != BitmapDevice()->GetNearestFontInPixels(iFont,iFontSpec))
            iFont = NULL;
        }      
    }        
    
// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::SetFont
// Set caption font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CAknFepCtrlLabel::SetFont(const CFont* aFont)
    {
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        }    

    iFontOwnership = EFalse;    
    iFont = const_cast<CFont*>(aFont);

    if(iFont)
        {
        iFontSpec = iFont->FontSpecInTwips();
        }
    }        

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetFontColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlLabel::SetFontColor(const TRgb aColor)
    {
    iFontColor = aColor;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlLabel::SetText(const TDesC& aText)
    {
 
    delete iText;
    iText = NULL;
    
    TRAP_IGNORE( iText = aText.AllocL());
    
    Draw();
    }    
    
// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::SizeChanged
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlLabel::SizeChanged(TRect aNewRect, TBool aIsReloadImages)
    {
    if (aNewRect.Size().iWidth == 0 || aNewRect.Size().iHeight == 0)
        {
   	    return;
        }
    
    SetRect(aNewRect);

    if (aIsReloadImages)
        {
        ResizeBitmaps(aNewRect.Size());	
        }

    Draw();
    UpdateArea(Rect(), EFalse);
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::ResizeBitmaps
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlLabel::ResizeBitmaps(TSize aSize)
    {
    if (BkMaskBmp() && 
        BkMaskBmp()->SizeInPixels() != aSize)
        {
       	AknIconUtils::SetSize(BkMaskBmp(), aSize, EAspectRatioNotPreserved);
        }

    if (BackgroundBmp() && 
        BackgroundBmp()->SizeInPixels() != aSize)
        {
       	AknIconUtils::SetSize(BackgroundBmp(), aSize, EAspectRatioNotPreserved);
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::SetTextAlignment
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlLabel::SetTextAlignment(CGraphicsContext::TTextAlign aAlignment)
    {
    iAlignment = aAlignment;
    }        

// ---------------------------------------------------------------------------
// CAknFepCtrlLabel::TextAlignment
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CGraphicsContext::TTextAlign CAknFepCtrlLabel::TextAlignment()    
    {
    return iAlignment;
    }    

// End Of File
