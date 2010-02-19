/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  CPeninputArabicFingerHwrIndicator
*
*/

#include <AknsUtils.h>
#include <barsread.h>

#include "peninputfingerhwrarindicator.h"
#include "peninputlayout.h"
const TInt KInvalidResId = -1;
const TInt KInvalidBmp = -1 ;
const TInt KInvalidColorGroup = -1;


// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrIndicator::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrIndicator* CPeninputArabicFingerHwrIndicator::NewL(CFepUiLayout* aUiLayout, TInt aCtrlId)
    {
    CPeninputArabicFingerHwrIndicator* lable=new (ELeave) CPeninputArabicFingerHwrIndicator(aUiLayout,aCtrlId);

    CleanupStack::PushL(lable);
    lable->BaseConstructL();
    CleanupStack::Pop(lable);

    return lable;    
    }
    
// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrIndicator::CPeninputArabicFingerHwrIndicator
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrIndicator::CPeninputArabicFingerHwrIndicator(CFepUiLayout* aUiLayout, TInt aCtrlId)
    :CFepUiBaseCtrl(TRect(0,0,0,0),aUiLayout,aCtrlId)
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
// CPeninputArabicFingerHwrIndicator::~CPeninputArabicFingerHwrIndicator
// Destructor
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrIndicator::~CPeninputArabicFingerHwrIndicator()
    {  
     
    }

void CPeninputArabicFingerHwrIndicator::ConstructFromResourceL( TInt aResID )
	{
	if (aResID == KInvalidResId)
		{
	    User::Leave( KErrArgument );
		}
	//DestroyRes();
		
	TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, aResID);
    
    // Read the file name of the bmps
    TPtrC bmpFileName = reader.ReadTPtrC();    
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TInt colorGroup = reader.ReadInt16();
    TAknsItemID id;

	// Get the image ids and mask ids from resource
    TInt bmpId = reader.ReadInt16(); 
    TInt bmpMskId = reader.ReadInt16();
    
    // Read skin item id
    const TInt skinitemid = reader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);
	
    if ( bmpId == KInvalidBmp || 
    	 bmpMskId == KInvalidBmp ||
    	 colorGroup == KInvalidColorGroup )
    	{
    	User::Leave( KErrGeneral );
    	}
    
	CFbsBitmap* bmp;
	CFbsBitmap* bmpMask;	
    AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(),
				                 id,
				                 KAknsIIDQsnIconColors,
				                 colorGroup,
				                 bmp,
				                 bmpMask,
				                 bmpFileName,
				                 bmpId,
				                 bmpMskId,
				                 TRgb() );   	
    
	SetBitmapL(bmp);
	SetMaskBitmapL(bmpMask);
    CleanupStack::PopAndDestroy(); // reader
	}
	
// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrIndicator::Draw
// Draw the control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CPeninputArabicFingerHwrIndicator::Draw()
    {
	if(!AbleToDraw())
        return;

    TRect rect = Rect();
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    gc->Activate(MaskBitmapDevice());

    //clear the rect
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetBrushColor(TRgb(KTransparentColor));//transparency
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
    }

// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrIndicator::SetBitmapL
// Set bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CPeninputArabicFingerHwrIndicator::SetBitmapL(CFbsBitmap* aBitmap)
    {
    if(!aBitmap)
        {
        return;
        }
        
    SetBackgroundBitmapL(aBitmap);    
    }  
   
// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrIndicator::SetMaskBitmapL
// Set mask bitmap for given status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CPeninputArabicFingerHwrIndicator::SetMaskBitmapL(CFbsBitmap* aBmp)
    {   
    if(!aBmp)
        {
        return;   
        }
        
    SetBackgroundMaskBitmapL(aBmp); 
    }
    
// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrIndicator::SizeChanged
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrIndicator::SizeChanged(TRect aNewRect, TBool aIsReloadImages)
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
// CPeninputArabicFingerHwrIndicator::ResizeBitmaps
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrIndicator::ResizeBitmaps(TSize aSize)
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

// End Of File
