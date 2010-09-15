/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of button
*
*/


// system includes
#include <barsread.h>
#include <peninputlayout.h>
#include <AknIconUtils.h>
#include <coemain.h>
#include <AknsUtils.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h> 
// user includes
#include <peninputbutton.h>
#include <peninputdragbutton.h>
#include <peninputeventbutton.h>
#include <peninputmultimodebutton.h>
#include <peninputrepeatbutton.h>
#include <peninputrawkeybutton.h>
#include <peninputcommonbutton.h>
#include <peninputmultiimagebutton.h>
#include <peninputcommonctrls.hrh>
#include <peninputcommonlayoutglobalenum.h>
#include <peninputlongpressbutton.h>

const TInt KInvalidBmp = -1 ;

//default value for long press timer
const TInt KLongPressInterval = 600000;

//default value for repeat timer
const TInt KRepeatInterval = 100000;

//default text color and shadow text color
const TUint32 KDefaultTextColor = 0x000000;
const TUint32 KDefaultShadowTextColor = 0xffffff;
const TInt32 KInvalidResId = -1;
const TInt KInvalidEventId = 0xffff;
//const TInt KFadingParamBlack = 255;
//const TInt KFadingParamWhite = 0;

const TInt KTransparency[256] =
    { 
    0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7,
    8, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13,
    14, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 19,
    19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23, 24, 24,
    24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 29,
    30, 30, 30, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35,
    35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 40, 40, 40,
    41, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 44, 44, 45, 45, 45, 46,
    46, 46, 47, 47, 47, 47, 48, 48, 48, 49, 49, 49, 50, 50, 50, 50, 51, 51,
    51, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 55, 55, 55, 56, 56, 56, 56,
    57, 57, 57, 58, 58, 58, 59, 59, 59, 59, 60, 60, 60, 61, 61, 61, 62, 62,
    62, 62, 63, 63, 63, 64, 64, 64, 65, 65, 65, 65, 66, 66, 66, 67, 67, 67,
    68, 68, 68, 68, 69, 69, 69, 70, 70, 70, 71, 71, 71, 71, 72, 72, 72, 73,
    73, 73, 74, 74, 74, 74, 75, 75, 75, 76, 76, 76, 77, 77 
    };
// Invalid function icon color group
const TInt KInvalidColorGroup = -1;

// ======== MEMBER FUNCTIONS ========
   
// ---------------------------------------------------------------------------
// CAknFepCtrlButton::CAknFepCtrlButton
// C++ default constructor can NOT contain any code, that
// might leave.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlButton::CAknFepCtrlButton(CFepUiLayout* aUiLayout, TInt aControlId)
: CButtonBase(TRect(), aUiLayout, aControlId), iFirstTimeConstruct(ETrue), iHighlight(EFalse) 
    {
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlButton::NewL
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlButton* CAknFepCtrlButton::NewL(CFepUiLayout* aUiLayout, TInt aControlId)
    {
    CAknFepCtrlButton* self = NewLC(aUiLayout, aControlId);
    CleanupStack::Pop(self);
    
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlButton::NewLC
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlButton* CAknFepCtrlButton::NewLC(CFepUiLayout* aUiLayout, TInt aControlId)
    {
    CAknFepCtrlButton* self = new (ELeave) CAknFepCtrlButton(aUiLayout, aControlId);
    CleanupStack::PushL(self);
    self->BaseConstructL();

    return self;
    }    
    
// -----------------------------------------------------------------------------
// Destructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlButton::~CAknFepCtrlButton()
    {
    iBmpList.Close();
    iMaskBmpList.Close();
    }
// ---------------------------------------------------------------------------
// CAknFepCtrlButton::ConstructFromResourceL
// The order of the image ids in the CButtonBase class must not be changed
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlButton::ConstructFromResourceL(TResourceReader& aReader)
    {
	if (!iFirstTimeConstruct)
		{
        iBmpList.Close();
        iMaskBmpList.Close();
        ResetBmpPool();
		}
    
    TPtrC bmpFileName = aReader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = aReader.ReadInt32();
   	TInt colorgroup = aReader.ReadInt16();
    TAknsItemID id;

    for (TInt ii = 0; ii < EBtnBmpLastType + 1; ++ii)
        {
        // Get the image ids and mask ids from resource
        TInt bmpId = aReader.ReadInt16(); 
        TInt bmpMskId = aReader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = aReader.ReadInt16();
        id.Set(TInt(imgMajorSkinId), skinitemid);
        
        if (bmpId != KInvalidBmp)
            {
        	CFbsBitmap* bmp = NULL;
        	CFbsBitmap* maskbmp = NULL;

        	if (bmpMskId != KInvalidBmp)
        		{
        		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        		                       id,
        		                       bmp,
        		                       maskbmp,
        		                       bmpFileName,
        		                       bmpId,
        		                       bmpMskId);
        		
       		    // set maskbmp and size
                SetMaskBitmapL(maskbmp, TButtonBmpType(EBtnBmpNonActive + ii));
                iMaskBmpList.Append(maskbmp);
                }
            else
                {
        	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        	                           id,
        	                           bmp,
        	                           bmpFileName,
        	                           bmpId);
                }

            // set bmp and size
            SetBitmapL(bmp, TButtonBmpType(EBtnBmpNonActive + ii));
            iBmpList.Append(bmp);
            }
        }

    if (!iFirstTimeConstruct)
    	{
        ResizeBitmaps(Rect().Size());
    	Draw();
    	}
    
    iFirstTimeConstruct = EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlButton::ConstructFromResourceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlButton::ConstructFromResourceL()
	{
	if (iResourceId == KInvalidResId)
		{
        User::Leave(KErrArgument);
		}
	
	if (!iFirstTimeConstruct)
		{
		// need to remove original bmp and maskbmp
        iBmpList.Close();
        iMaskBmpList.Close();
        ResetBmpPool();
		}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TPtrC bmpFileName = reader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TInt colorgroup = reader.ReadInt16();
    TAknsItemID id;

    for (TInt ii = 0; ii < EBtnBmpLastType + 1; ++ii)
        {
        // Get the image ids and mask ids from resource
        TInt bmpId = reader.ReadInt16(); 
        TInt bmpMskId = reader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = reader.ReadInt16();
        id.Set(TInt(imgMajorSkinId), skinitemid);
        
        if (bmpId != KInvalidBmp)
            {
        	CFbsBitmap* bmp = NULL;
        	CFbsBitmap* maskbmp = NULL;

        	if (bmpMskId != KInvalidBmp)
        		{
        		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        		                       id,
        		                       bmp,
        		                       maskbmp,
        		                       bmpFileName,
        		                       bmpId,
        		                       bmpMskId);
        		
       		    // set maskbmp and size
                SetMaskBitmapL(maskbmp, TButtonBmpType(EBtnBmpNonActive + ii));
                iMaskBmpList.Append(maskbmp);
                }
            else
                {
        	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        	                           id,
        	                           bmp,
        	                           bmpFileName,
        	                           bmpId);
                }

            // set bmp and size
            SetBitmapL(bmp, TButtonBmpType(EBtnBmpNonActive + ii));
            iBmpList.Append(bmp);
            }
        }
        
    CleanupStack::PopAndDestroy(); // reader
    
    if (!iFirstTimeConstruct)
    	{
        ResizeBitmaps(Rect().Size());
    	Draw();
    	}

    iFirstTimeConstruct = EFalse;    	
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlButton::Draw
// Draws UI
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlButton::Draw()
    {
	if(!AbleToDraw())
        return;
    
    if ( !BackgroundBmp() )
        {
        CButtonBase::Draw();
        }

    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());        
    
    //mask bitmaps
    gc->Activate( MaskBitmapDevice() );
   
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetBrushColor( TRgb(KOpaqueColor));//Non transparent at all
    gc->SetPenStyle(CGraphicsContext::ESolidPen );
    gc->SetPenSize( PenSize());
    gc->SetPenColor( TRgb(KOpaqueColor) );
    gc->DrawRect(Rect());   

    // ----- draw bitmaps -----
    gc->Activate( BitmapDevice() );   
    if ( BackgroundBmp() )
        {
        TSize size=BackgroundBmp()->SizeInPixels();
    
        gc->DrawBitmap(Rect(), BackgroundBmp(),size);
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlButton::SetActive
// Set active for the button
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlButton::SetActive(TBool aActiveFlag)
    {
    if( IsDimmed() )
        {
        return;
        }
        
    if(IsActive() != aActiveFlag)
        {
        CFepUiBaseCtrl::SetActive(aActiveFlag);
        if(aActiveFlag || iHighlight)
            {               
            SetBackgroundBmp(Bitmap(EBtnBmpActive,EFalse));
            SetBackgroundMaskBmp(Bitmap(EBtnBmpActive,ETrue));
            //Hide(EFalse);   //active button won't be Hiden ???
            }
        else
            {
            SetBackgroundBmp(Bitmap(EBtnBmpNonActive,EFalse));
            SetBackgroundMaskBmp(Bitmap(EBtnBmpNonActive,ETrue));
            }
        if(BitGc())
            {       
            Draw();
            UpdateArea(Rect(),EFalse);
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlButton::SetHighlight
// Set highlight for the button
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlButton::SetHighlight(TBool aIsHighlight)
    {
    iHighlight = aIsHighlight;

    if ( iHighlight )
        {
        SetBackgroundBmp(Bitmap(EBtnBmpActive,EFalse));
        SetBackgroundMaskBmp(Bitmap(EBtnBmpActive,ETrue));            
        }
    else
        {
        SetBackgroundBmp(Bitmap(EBtnBmpNonActive,EFalse));
        SetBackgroundMaskBmp(Bitmap(EBtnBmpNonActive,ETrue));
        }
    Draw();
    UpdateArea(Rect(), EFalse);         
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlButton::ResizeBitmaps
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlButton::ResizeBitmaps(TSize aSize)
    {
    if (iMaskBmpList.Count() >= 1 && 
        iMaskBmpList[0]->SizeInPixels() != aSize)
        {
        for( int ii = 0; ii < iMaskBmpList.Count(); ++ii )
            {
          	AknIconUtils::SetSize(iMaskBmpList[ii], aSize, EAspectRatioNotPreserved);
            }
        }

    if (iBmpList.Count() >= 1 &&
        iBmpList[0]->SizeInPixels() != aSize)
        {
        for( int ii = 0; ii < iBmpList.Count(); ++ii )
            {
       	    AknIconUtils::SetSize(iBmpList[ii], aSize, EAspectRatioNotPreserved);
            }
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlButton::SizeChanged
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlButton::SizeChanged(TRect aNewRect, TBool aIsReloadImages)
    {
    if ((aNewRect.Size().iWidth == 0 || aNewRect.Size().iHeight == 0) &&
        (ControlId() != EPeninupWindowCtrlIdArrowLeftBtn && 
         ControlId() != EPeninupWindowCtrlIdArrowRightBtn))
        {
   	    return;
        }
    
    SetRect(aNewRect);

    //if (aIsReloadImages)
        {
        ResizeBitmaps(aNewRect.Size());	
        }

    Draw();
    UpdateArea(Rect(), EFalse);
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlButton::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
void CAknFepCtrlButton::HandlePointerLeave(const TPoint& aPt)
    {
    if( IsDimmed() )
        return;
    if( iHighlight )
        {
        SetBackgroundBmp(iActiveBmp);
        SetBackgroundMaskBmp(iActiveMaskBmp);
        }
        
    else
        {
        SetBackgroundBmp(iNonActiveBkBmp);
        SetBackgroundMaskBmp(iNonActiveBkMaskBmp);
        }
    Draw();
    UpdateArea(Rect(), EFalse);   
    CFepUiBaseCtrl::HandlePointerLeave(aPt);
    return ;
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlDragButton::CAknFepCtrlDragButton
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlDragButton::CAknFepCtrlDragButton(CFepUiLayout* aUiLayout, TInt aControlId)
: CDragBar(TRect(), aUiLayout, aControlId), iFirstTimeConstruct(ETrue)
    {
    }
    
// -----------------------------------------------------------------------------
// Destructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlDragButton::~CAknFepCtrlDragButton()
    {
    iBmpList.Close();
    iMaskBmpList.Close();
    
	DeleteForgroundRes();
    delete iText;    
    }

    
// -----------------------------------------------------------------------------
// CAknFepCtrlDragButton::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlDragButton* CAknFepCtrlDragButton::NewL(CFepUiLayout* aUiLayout, 
                                                            TInt aControlId)
    {
    CAknFepCtrlDragButton* self = NewLC(aUiLayout, aControlId);
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlDragButton::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlDragButton* CAknFepCtrlDragButton::NewLC(CFepUiLayout* aUiLayout, 
                                                             TInt aControlId)
    {
    CAknFepCtrlDragButton* self = new (ELeave) CAknFepCtrlDragButton(aUiLayout, aControlId);
    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }    
    
// -----------------------------------------------------------------------------
// CAknFepCtrlDragButton::ConstructFromResourceL
// The order of the image ids in the CButtonBase class must not be changed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlDragButton::ConstructFromResourceL(TResourceReader& aReader)
    {
    /*
	if (!iFirstTimeConstruct)
		{
        iBmpList.Close();
        iMaskBmpList.Close();
        ResetBmpPool();
		}
    
    TPtrC bmpFileName = aReader.ReadTPtrC();
    TInt32 imgMajorSkinId = aReader.ReadInt32();
    TAknsItemID id;

    for (TInt ii = 0; ii < EBtnBmpLastType + 1; ++ii)
        {
        const TInt16 bmpId = aReader.ReadInt16();
        const TInt16 bmpMskId = aReader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = aReader.ReadInt16();
        id.Set(TInt(imgMajorSkinId), skinitemid);
        
        if (bmpId != KInvalidBmp)
            {
        	CFbsBitmap* bmp = NULL;
        	CFbsBitmap* maskbmp = NULL;

        	if (bmpMskId != KInvalidBmp)
        		{
        		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        		                       id,
        		                       bmp,
        		                       maskbmp,
        		                       bmpFileName,
        		                       bmpId,
        		                       bmpMskId);
        		
                SetMaskBitmapL(maskbmp, TButtonBmpType(EBtnBmpNonActive + ii));
                iMaskBmpList.Append(maskbmp);
                }
            else
                {
        	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        	                           id,
        	                           bmp,
        	                           bmpFileName,
        	                           bmpId);
                }

            SetBitmapL(bmp, TButtonBmpType(EBtnBmpNonActive + ii));
            iBmpList.Append(bmp);
            }
        }
    
    if (!iFirstTimeConstruct)
    	{
        ResizeBitmaps(Rect().Size());
    	Draw();
    	}
    
    iFirstTimeConstruct = EFalse;*/
	if (!iFirstTimeConstruct)
		{
        DeleteForgroundRes();
		}
    
    TPtrC bmpFileName = aReader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = aReader.ReadInt32();
    TInt colorGroup = aReader.ReadInt16();
    TAknsItemID id;
	
    // Get the image ids and mask ids from resource
    TInt bmpId = aReader.ReadInt16(); 
    TInt bmpMskId = aReader.ReadInt16();
    
    // read skin item id
    const TInt skinitemid = aReader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);
    
    if (bmpId != KInvalidBmp)
        {
    	if (bmpMskId != KInvalidBmp)
    		{
    		if( colorGroup == KInvalidColorGroup )
    			{
	    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                       id,
                       iForgroundBmp,
                       iForgroundBmpMask,
                       bmpFileName,
                       bmpId,
                       bmpMskId);	
    			}
			else
				{
			    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                             id,
                             KAknsIIDQsnIconColors,
                             colorGroup,
                             iForgroundBmp,
                             iForgroundBmpMask,
                             bmpFileName,
                             bmpId,
                             bmpMskId,
                             AKN_LAF_COLOR( 0 ) );   	
				}

        AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(iForgroundBmpMask, iForgroundBmpRect.Size(), 
                              EAspectRatioNotPreserved);
    		                       
    		/*CreateDimmedMaskL( iForgroundDimBmpMask, 
    						   iForgroundBmpMask );
   			AknIconUtils::SetSize(iForgroundDimBmpMask, iForgroundBmpRect.Size(), 
                              EAspectRatioNotPreserved);*/
    		}
    		else
	        {
		    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
		                           id,
		                           iForgroundBmp,
		                           bmpFileName,
		                           bmpId);
		    AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
	        }
        }
	
    if (!iFirstTimeConstruct)
    	{
        //ResizeBitmaps(iForgroundBmpRect.Size());
    	Draw();
    	}
    
    iFirstTimeConstruct = EFalse;    
    
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlDragButton::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlDragButton::ConstructFromResourceL()
	{
	/*
	if (iResourceId == KInvalidResId)
		{
		User::Leave(KErrArgument);
		}
	
	if (!iFirstTimeConstruct)
		{
        iBmpList.Close();
        iMaskBmpList.Close();
        ResetBmpPool();
		}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TAknsItemID id;

    for (TInt ii = 0; ii < EBtnBmpLastType + 1; ++ii)
        {
        const TInt16 bmpId = reader.ReadInt16();
        const TInt16 bmpMskId = reader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = reader.ReadInt16();
        id.Set(TInt(imgMajorSkinId), skinitemid);
        
        if (bmpId != KInvalidBmp)
            {
        	CFbsBitmap* bmp = NULL;
        	CFbsBitmap* maskbmp = NULL;

        	if (bmpMskId != KInvalidBmp)
        		{
        		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        		                       id,
        		                       bmp,
        		                       maskbmp,
        		                       bmpFileName,
        		                       bmpId,
        		                       bmpMskId);
        		
                SetMaskBitmapL(maskbmp, TButtonBmpType(EBtnBmpNonActive + ii));
                iMaskBmpList.Append(maskbmp);
                }
            else
                {
        	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        	                           id,
        	                           bmp,
        	                           bmpFileName,
        	                           bmpId);
                }

            SetBitmapL(bmp, TButtonBmpType(EBtnBmpNonActive + ii));
            iBmpList.Append(bmp);
            }
        }
        
    CleanupStack::PopAndDestroy(); // reader
    
    if (!iFirstTimeConstruct)
    	{
        ResizeBitmaps(Rect().Size());
        Draw();
    	}
    	
    iFirstTimeConstruct = EFalse;*/
	if (iResourceId == KInvalidResId)
		{
        return;
		}
	
	if (!iFirstTimeConstruct)
		{
		// need to remove original bmp and maskbmp
        DeleteForgroundRes();
		}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TPtrC bmpFileName = reader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TInt colorGroup = reader.ReadInt16();
    TAknsItemID id;
	
	// Get the image ids and mask ids from resource
    TInt bmpId = reader.ReadInt16(); 
    TInt bmpMskId = reader.ReadInt16();
    
    // Read skin item id
    const TInt skinitemid = reader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);

    if (bmpId != KInvalidBmp)
        {
    	if (bmpMskId != KInvalidBmp)
    		{
    		if( colorGroup == KInvalidColorGroup )
    			{
	    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                       id,
                       iForgroundBmp,
                       iForgroundBmpMask,
                       bmpFileName,
                       bmpId,
                       bmpMskId);	
    			}
			else
				{
			    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                             id,
                             KAknsIIDQsnIconColors,
                             colorGroup,
                             iForgroundBmp,
                             iForgroundBmpMask,
                             bmpFileName,
                             bmpId,
                             bmpMskId,
                             AKN_LAF_COLOR( 0 ) );   	
				}

    		AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
    		AknIconUtils::SetSize(iForgroundBmpMask, iForgroundBmpRect.Size(), 
    		                      EAspectRatioNotPreserved);
    		                       
    		/*CreateDimmedMaskL( iForgroundDimBmpMask, iForgroundBmpMask );
   			AknIconUtils::SetSize(iForgroundDimBmpMask, iForgroundBmpRect.Size(), 
   			                      EAspectRatioNotPreserved);*/
    		}
    		else
	        {
		    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
		                           id,
		                           iForgroundBmp,
		                           bmpFileName,
		                           bmpId);
		    AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
	        }
        }

    CleanupStack::PopAndDestroy(); // reader
    
    if (!iFirstTimeConstruct)
    	{
        //ResizeBitmaps(iForgroundBmpRect.Size());
    	Draw();
    	}

    iFirstTimeConstruct = EFalse;    	
    
	}
    
    
// ---------------------------------------------------------------------------
// CAknFepCtrlDragButton::DeleteForgroundRes
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlDragButton::DeleteForgroundRes()
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
	if( iForgroundDimBmpMask )
		{
		delete iForgroundDimBmpMask;
		iForgroundDimBmpMask = NULL;
		}
	}    
    
// ---------------------------------------------------------------------------
// CAknFepCtrlDragButton::CreateDimmedMaskL
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//	
void CAknFepCtrlDragButton::CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                                        		 const CFbsBitmap* aMask
                                        		 /*TScaleMode aScaleMode*/ )
	{
	    if (aMask && aMask->DisplayMode() == EGray256)
	        {
	        delete aDimmedMask;
	        aDimmedMask = NULL;

	        aDimmedMask = new (ELeave) CFbsBitmap;

	        User::LeaveIfError(aDimmedMask->Create(aMask->SizeInPixels(), EGray256)); 
	        CleanupStack::PushL(aDimmedMask);

	        CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(aDimmedMask);
	        CleanupStack::PushL(bitmapDevice);

	        CFbsBitGc* bitGc(NULL);
	        User::LeaveIfError(bitmapDevice->CreateContext(bitGc));
	        CleanupStack::PushL(bitGc);

	        bitGc->SetPenStyle(CGraphicsContext::ESolidPen);
	        bitGc->BitBlt(TPoint(0, 0), aMask);

	        aDimmedMask->LockHeap();
	        TInt w = aMask->SizeInPixels().iWidth; 
	        TInt h = aMask->SizeInPixels().iHeight;
	        TInt dataStride = aMask->DataStride() - w; 
	        unsigned char* address = (unsigned char *)aDimmedMask->DataAddress();  

	        for ( TInt i = 0; i < h; ++i )
	            {
	            for ( TInt j = 0; j < w; ++j )
	                {
	                *address = KTransparency[*address];
	                ++address;
	                }
	            address += dataStride;         
	            }

	        aDimmedMask->UnlockHeap();

	        //AknIconUtils::SetSize(aDimmedMask, aMask->SizeInPixels(), aScaleMode);

	        CleanupStack::PopAndDestroy(2); // bitmapDevice, bitGc
	        CleanupStack::Pop(1); // aDimmedMask
	        }

	}
    
    
// ---------------------------------------------------------------------------
// CAknFepCtrlDragButton::SetForgroundBmpRect
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlDragButton::SetForgroundBmpRect(const TRect& aRect)
	{
	iForgroundBmpRect = aRect;	
	}    
    
// ---------------------------------------------------------------------------
// CAknFepCtrlDragButton::SetForegroundBmpL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CAknFepCtrlDragButton::SetForegroundBmpL(CFbsBitmap* aBmp,
                                                         CFbsBitmap* aMaskBmp)
    {
    DeleteForgroundRes();

    iForgroundBmp = aBmp;
    iForgroundBmpMask = aMaskBmp;

    ResizeBitmaps(iForgroundBmpRect.Size());
    }

EXPORT_C void CAknFepCtrlDragButton::Move(const TPoint& aOffset)
    {
    CButtonBase::Move(aOffset);
    iForgroundBmpRect.Move( aOffset );
    }    
    
// ---------------------------------------------------------------------------
// CAknFepCtrlDragButton::HandlePointerDownEventL
// Handle button down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlDragButton::HandlePointerDownEventL(const TPoint& aPt)
	{
    if( IsDimmed() )
    	return NULL;
    
    CDragBar::HandlePointerDownEventL(aPt);
		
	SetReady(ETrue);
    Draw();
    UpdateArea(Rect(),EFalse);
    ReportEvent(EEventButtonDown);
    return this;
	}    
    
// ---------------------------------------------------------------------------
// CAknFepCtrlDragButton::Draw
// Draws UI
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlDragButton::Draw()
    {
	if(!AbleToDraw())
        return;
    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    //mask bitmaps
  	DrawOpaqueMaskBackground();  
    TRect rect = Rect();        
    TRect innerRect = rect;
    innerRect.Shrink( 10, 10 );
    
    // ----- draw bitmaps -----
    gc->Activate( BitmapDevice() );                
    //gc->Clear(rect);
    	
	CFbsBitmap* bmpMask = NULL;
	
	// Draw background    
 	if( IsDimmed() ) // Dimmed state
    	{
		AknsDrawUtils::DrawFrame( UiLayout()->SkinInstance(), 
		                         *gc, 
		                         rect, 
		                         innerRect,
		                         iInactiveImgID,
		                         KAknsIIDDefault );
    	if( iForgroundDimBmpMask )
    		{
    		delete iForgroundDimBmpMask;
    		iForgroundDimBmpMask = NULL;
    		}		                         
    	TRAP_IGNORE(CreateDimmedMaskL( iForgroundDimBmpMask, 
    					   iForgroundBmpMask ));
		AknIconUtils::SetSize(iForgroundDimBmpMask, iForgroundBmpRect.Size(), 
                              EAspectRatioNotPreserved);		                         
		bmpMask = iForgroundDimBmpMask;    
    	}
    else
    	{
		// Normal state or pressed state
		TAknsItemID ImgID = (IsDragging() | PointerDown()) ? iPressedImgID : iNormalImgID;
		AknsDrawUtils::DrawFrame( UiLayout()->SkinInstance(), 
	                     *gc, 
	                     rect, 
	                     innerRect,
	                     ImgID,
	                     KAknsIIDDefault ); 
	    bmpMask = iForgroundBmpMask;            	    		
    	}
    
    // Draw forground
	if( iForgroundBmp )
		{
		TRect srcRect( TPoint( 0, 0 ), iForgroundBmp->SizeInPixels() );
		if( bmpMask )
			{
			gc->BitBltMasked( iForgroundBmpRect.iTl, 
							  iForgroundBmp, 
							  srcRect,
							  bmpMask,
							  EFalse);
			}
		else
			{
			gc->BitBlt( iForgroundBmpRect.iTl,
						iForgroundBmp,
						srcRect );	
			}
		}

    if (iText)
        {
        TAknLayoutText textLayout;
        textLayout.LayoutText(Rect(), iTextFormat);
        textLayout.DrawText(*gc, *iText);
        }
    }
    
    
// -----------------------------------------------------------------------------
// CAknFepCtrlDragButton::ResizeBitmaps
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepCtrlDragButton::ResizeBitmaps(TSize aInnerSize)
    {
    /*
    if (iMaskBmpList.Count() >= 1 &&
        iMaskBmpList[0]->SizeInPixels() != aSize)
        {
        for( TInt ii = 0; ii < iMaskBmpList.Count(); ++ii )
            {
       	    AknIconUtils::SetSize(iMaskBmpList[ii], aSize, EAspectRatioNotPreserved);
            }
        }
        
    if (iBmpList.Count() >= 1 &&
        iBmpList[0]->SizeInPixels() != aSize)
        {
        for( TInt ii = 0; ii < iBmpList.Count(); ++ii )
            {
            AknIconUtils::SetSize(iBmpList[ii], aSize, EAspectRatioNotPreserved);
            }
        }*/
    if( iForgroundBmp && ( iForgroundBmp->SizeInPixels() != aInnerSize ) )
    	{
    	AknIconUtils::SetSize( iForgroundBmp, aInnerSize, EAspectRatioNotPreserved );
    	}
    if( iForgroundBmpMask && ( iForgroundBmpMask->SizeInPixels() != aInnerSize ) )
    	{
    	AknIconUtils::SetSize( iForgroundBmpMask, aInnerSize, EAspectRatioNotPreserved );
    	}    
    	
    /*TRAP_IGNORE(CreateDimmedMaskL(iForgroundDimBmpMask, 
                                  iForgroundBmpMask));   */     
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlDragButton::SizeChanged
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlDragButton::SizeChanged(TRect aNewRect, 
												 TRect aInnerRect, 
												 TBool aIsReloadImages)
    {
    // Reset the position and the extent of the button aera
    /*if (aNewRect.Size().iWidth == 0 || aNewRect.Size().iHeight == 0)
        {
   	    return;
        }
        
    SetRect(aNewRect);

    if (aIsReloadImages)
        {
        ResizeBitmaps(aNewRect.Size());
        }

    Draw();
    UpdateArea(Rect(), ETrue);*/
    
    if (aNewRect.Size().iWidth == 0 || aNewRect.Size().iHeight == 0)
        {
   	    return;
        }
    
    TRect oriRect = Rect();
    SetRect(aNewRect);
	iForgroundBmpRect = aInnerRect;
	
    if (aIsReloadImages)
        {
        ResizeBitmaps(aInnerRect.Size() );	
        }
	RootControl()->ReDrawRect( oriRect );
    UpdateArea(oriRect, EFalse);
    Draw();
    UpdateArea(Rect(), EFalse);    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::CAknFepCtrlMultiModeButton
// C++ default constructor can NOT contain any code, that
// might leave.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiModeButton::CAknFepCtrlMultiModeButton(CFepUiLayout* aUiLayout, 
                                     TInt aControlId,TAknsItemID aNormalID,
                                     TAknsItemID aPressedID,TAknsItemID aInactiveID)
         : CAknFepCtrlCommonButton(aUiLayout, aControlId, aNormalID, aPressedID, aInactiveID)
    {
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::NewL
// Two-phased constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiModeButton* CAknFepCtrlMultiModeButton::NewL(CFepUiLayout* aUiLayout, 
                                                    TInt aControlId,TAknsItemID aNormalID,
                                                    TAknsItemID aPressedID,TAknsItemID aInactiveID)
    {
    CAknFepCtrlMultiModeButton* self = NewLC(aUiLayout, aControlId, 
                                             aNormalID, aPressedID, aInactiveID);
    CleanupStack::Pop(self);
    
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::NewLC
// Two-phased constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiModeButton* CAknFepCtrlMultiModeButton::NewLC(CFepUiLayout* aUiLayout, 
                                                    TInt aControlId,TAknsItemID aNormalID,
                                                    TAknsItemID aPressedID,TAknsItemID aInactiveID)
    {
    CAknFepCtrlMultiModeButton* self = new (ELeave) CAknFepCtrlMultiModeButton(aUiLayout, 
                                                                               aControlId, 
                                                                               aNormalID, 
                                                                               aPressedID,
                                                                               aInactiveID);
    CleanupStack::PushL(self);
    self->BaseConstructL();
    
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiModeButton::~CAknFepCtrlMultiModeButton()
    {
    iImageResIdList.Close();
    iForegroundBmpList.ResetAndDestroy();
    iForegroundBmpList.Close();
    iForegroundMaskBmpList.ResetAndDestroy();
    iForegroundMaskBmpList.Close();
        
    iModeTextList.ResetAndDestroy();
    iModeTextList.Close();  
    
    iModesList.ResetAndDestroy();
    iModesList.Close();
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::ConstructFromResourceL
// The order of the image ids in the CButtonBase class must not be changed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void CAknFepCtrlMultiModeButton::ConstructFromResourceL(TResourceReader& aReader)
    {
    if (!iFirstTimeConstruct)
    	{
        iImageResIdList.Close();
        iForegroundBmpList.ResetAndDestroy();
        iForegroundBmpList.Close();
        iForegroundMaskBmpList.ResetAndDestroy();
        iForegroundMaskBmpList.Close();
             
        iModeTextList.ResetAndDestroy();
        iModeTextList.Close();  


        iModesList.ResetAndDestroy();
        iModesList.Close();
    	
        ResetBmpPool();
    	}
    
    // read text color and shadow text color	
    TInt32 colorMajorSkinId = aReader.ReadInt32();
    const TInt skinitemid = aReader.ReadInt16();
    TInt textcoloridx = aReader.ReadInt16();
    TInt shadowtextcoloridx = aReader.ReadInt16();
    
    TAknsItemID id;
    
    id.Set(TInt(colorMajorSkinId), skinitemid);
    TInt error = AknsUtils::GetCachedColor(UiLayout()->SkinInstance(),
                                           iFontColor,
                                           id,
                                           textcoloridx);

    if (error != KErrNone)
        {
    	iFontColor = TRgb(KDefaultTextColor);
        }
    
    SetFontColor(iFontColor);    
    
    error = AknsUtils::GetCachedColor(UiLayout()->SkinInstance(),
                                      iShadowFontColor,
                                      id,
                                      shadowtextcoloridx);
                                      
    if (error != KErrNone)
        {
    	iShadowFontColor = TRgb(KDefaultShadowTextColor);
        }
    
    SetShadowFontColor(iShadowFontColor);    
    
    const TInt16 countModes = aReader.ReadInt16();      // Read the count of different cases
    
    if (countModes <= 0)
        {
        User::Leave(KErrArgument);
        }

    iNumberOfModes = 0;

    for (TInt ii = 0; ii < countModes; ii++)
        {
        AddModeL(aReader);
        iNumberOfModes++;
        }
        
    if (iFirstTimeConstruct)
    	{
        if (iImageResIdList.Count() == 1)
            {
            // set background bimap for button    
            SetButtonBmpL(EBtnBmpNonActive, 0);
            iCurrentBmpType = EBtnBmpNonActive;
            }

        const TInt16 defaultModeIndex = aReader.ReadInt16();
        SetCurrentModeL(EBtnBmpNonActive, defaultModeIndex);
    	}
    else
    	{
        SetButtonBmpL(iCurrentBmpType, iCurrentMode);
        Draw();
    	}

    iFirstTimeConstruct = EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void CAknFepCtrlMultiModeButton::ConstructFromResourceL()
	{
    if (iResourceId == KInvalidResId)
    	{
    	return;
    	}
 
    if (!iFirstTimeConstruct)
    	{
        iImageResIdList.Close();
        iForegroundBmpList.ResetAndDestroy();
        iForegroundBmpList.Close();
        iForegroundMaskBmpList.ResetAndDestroy();
        iForegroundMaskBmpList.Close();
        
        iModeTextList.ResetAndDestroy();
        iModeTextList.Close();  


        iModesList.ResetAndDestroy();
        iModesList.Close();
    	
        ResetBmpPool();
    	}
    	
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);
	
    // read text color and shadow text color	
    TInt32 colorMajorSkinId = reader.ReadInt32();
    const TInt skinitemid = reader.ReadInt16();
    TInt textcoloridx = reader.ReadInt16();
    TInt shadowtextcoloridx = reader.ReadInt16();
    
    TAknsItemID id;
    
    id.Set(TInt(colorMajorSkinId), skinitemid);
    TInt error = AknsUtils::GetCachedColor(UiLayout()->SkinInstance(),
                                           iFontColor,
                                           id,
                                           textcoloridx);

    if (error != KErrNone)
        {
    	iFontColor = TRgb(KDefaultTextColor);
        }
    
    SetFontColor(iFontColor);    
    
    error = AknsUtils::GetCachedColor(UiLayout()->SkinInstance(),
                                      iShadowFontColor,
                                      id,
                                      shadowtextcoloridx);
                                      
    if (error != KErrNone)
        {
    	iShadowFontColor = TRgb(KDefaultShadowTextColor);
        }
    
    SetShadowFontColor(iShadowFontColor);    

    const TInt16 countModes = reader.ReadInt16();
    
    if (countModes <= 0)
        {
        User::Leave(KErrArgument);
        }

   	iNumberOfModes = 0;
    
    for (TInt ii = 0; ii < countModes; ii++)
        {
        AddModeL(reader);
       	iNumberOfModes++;
        }

	
    if (iFirstTimeConstruct)
    	{
        if (iImageResIdList.Count() == 1)
            {
            // set background bimap for button    
            SetButtonBmpL(EBtnBmpNonActive, 0);
            iCurrentBmpType = EBtnBmpNonActive;
            }

        const TInt16 defaultModeIndex = reader.ReadInt16();
        SetCurrentModeL(EBtnBmpNonActive, defaultModeIndex);
    	}
    else
    	{
    	ResizeBitmaps(iForgroundBmpRect.Size());
        SetButtonBmpL(iCurrentBmpType, iCurrentMode);
        Draw();
    	}	
    	
    CleanupStack::PopAndDestroy(); // reader
    
    iFirstTimeConstruct = EFalse;

	}

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::AddModeL
// Add new display mode from resource reader
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::AddModeL(TResourceReader& aReader)
    {
    const TInt32 imageResId = aReader.ReadInt32();
    iModeTextList.Append(aReader.ReadHBufCL());
    const TInt rangeId = aReader.ReadInt16();
    const TInt modeId = aReader.ReadInt16();

    TAknFepModeIndicator* modeIndicator = new (ELeave) TAknFepModeIndicator;
    CleanupStack::PushL(modeIndicator);

    modeIndicator->iRangeId = rangeId;
    modeIndicator->iModeId = modeId;
    modeIndicator->iPermitted = ETrue;

    iModesList.AppendL(modeIndicator);
    CleanupStack::Pop(modeIndicator); // indicator        

    if ( imageResId != KInvalidBmp )
        {
        // switch mode means change image    
        TResourceReader reader;
        CCoeEnv::Static()->CreateResourceReaderLC(reader, imageResId);
        
        TPtrC bmpFileName = reader.ReadTPtrC();
        TInt32 imgMajorSkinId = reader.ReadInt32();
        TInt colorGroup = reader.ReadInt16();
        TAknsItemID id;

        for (TInt jj = 0; jj < /*EBtnBmpLastType +*/ 1; jj++)
            {
            const TInt16 bmpId = reader.ReadInt16();
            const TInt16 bmpMskId = reader.ReadInt16();
            const TInt16 skinitemid = reader.ReadInt16();
            id.Set(TInt(imgMajorSkinId), skinitemid);

            if (bmpId != KInvalidBmp)
            	{
            	CFbsBitmap* bmp = NULL;
            	CFbsBitmap* maskbmp = NULL;
            	
        	    if (bmpMskId != KInvalidBmp)
        	        {
        	        if( colorGroup == KInvalidColorGroup )
	        	    	{
	        		    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
	        		                           id,
	        		                           bmp,
	        		                           maskbmp,
	        		                           bmpFileName,
	        		                           bmpId,
	        		                           bmpMskId);
	        	    	}
        	    	else
	        	    	{
	        	    	AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
				                             id,
				                             KAknsIIDQsnIconColors,
				                             colorGroup,
				                             bmp,
				                             maskbmp,
				                             bmpFileName,
				                             bmpId,
				                             bmpMskId,
				                             AKN_LAF_COLOR( 0 ) );   	
	
	        	    	}
                    CleanupStack::PushL(maskbmp);
                    AknIconUtils::SetSize(maskbmp, iForgroundBmpRect.Size(),
                                          EAspectRatioNotPreserved);                    
                    iForegroundMaskBmpList.AppendL(maskbmp);            
                    CleanupStack::Pop(maskbmp);    
                    
        	        }
        	    else
        	        {
        	        AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        	                               id,
        	                               bmp,
        	                               bmpFileName,
        	                               bmpId);

        	        }
        	    
        	    CleanupStack::PushL(bmp);
        	    AknIconUtils::SetSize(bmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
        	    iForegroundBmpList.AppendL(bmp);        	    
        	    CleanupStack::Pop(bmp);    
                
            	}
            }
        
        CleanupStack::PopAndDestroy(); //reader
        
        iImageResIdList.Append(imageResId);
        }
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SetButtonBmp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::SetButtonBmpL(const TButtonBmpType aType , 
                                                        const TInt aMode)
    {
    if ( !ForgroundBmp() || 
         ForgroundBmp()->Handle() != iForegroundBmpList[aMode]->Handle() ||
         !ForgroundBmpMask() || 
         ForgroundBmpMask()->Handle() != iForegroundMaskBmpList[aMode]->Handle() )
        {
        CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
        if(iForegroundMaskBmpList.Count() > 0 && iForegroundBmpList[aMode])
            {
            CleanupStack::PushL( bmp );
            
            User::LeaveIfError(bmp->Duplicate(iForegroundBmpList[aMode]->Handle()));
            
            CleanupStack::Pop( bmp );            
            }
        else
            {
            delete bmp;
            bmp = NULL;
            }
        CFbsBitmap* maskbmp = new (ELeave) CFbsBitmap;
        
        if(iForegroundMaskBmpList.Count() > 0 && iForegroundMaskBmpList[aMode])
            {    
            CleanupStack::PushL( maskbmp );
            User::LeaveIfError(maskbmp->Duplicate(iForegroundMaskBmpList[aMode]->Handle()));    
            CleanupStack::Pop( maskbmp );     
            }
        else
            {
            delete maskbmp;
            maskbmp = NULL;
            }
        SetForegroundBmpL(bmp,maskbmp);
        }
    else if ( ForgroundBmp()->SizeInPixels() != iForegroundBmpList[aMode]->SizeInPixels() ||
              ForgroundBmpMask()->SizeInPixels() != iForegroundMaskBmpList[aMode]->SizeInPixels())
        {
        ResizeBitmaps(iForegroundBmpList[aMode]->SizeInPixels());
        }
        
    if(aType == EBtnBmpActivePressed
      || aType == EBtnBmpNonActivePressed)
        {
        SetHighlight(ETrue);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SetCurrentModeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::SetCurrentModeL(const TButtonBmpType aType, 
                                                          const TInt aModeIndex)
    {
    if ((aModeIndex >= 0) && 
        (aModeIndex <= NumberOfModes()-1) && 
        (iModesList[aModeIndex]->iPermitted))
        {
        iCurrentMode = aModeIndex;

        if (iImageResIdList.Count() > 1)
            {
            // change mode means change image
            SetButtonBmpL(aType, iCurrentMode);
            iCurrentBmpType = aType;
            }

        Draw();
        UpdateArea(Rect());
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::NextMode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlMultiModeButton::NextMode() const
    {
    TInt startIndex = 0;

    if (iCurrentMode < (NumberOfModes() -1))
        {
        startIndex = iCurrentMode + 1;
        }

    TBool find = EFalse;
    
    while (!find && (startIndex != iCurrentMode))
        {
        if (iModesList[startIndex]->iPermitted)
            {
            find = ETrue;
            }
        else
            {
            startIndex++;
            
            if (startIndex > (NumberOfModes()-1))
                {
                startIndex = 0;
                }
            }
        }        

    if (find)
        {
        return startIndex;
        }
    else
        {
        return iCurrentMode;
        }    
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::ImageAlreadyExist
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CAknFepCtrlMultiModeButton::ImageAlreadyExist(const TInt32 aResId)
    {
    TBool findResId = EFalse;

    for (TInt i=0; i<iImageResIdList.Count(); i++)
        {
        if (aResId == iImageResIdList[i])
            {
            findResId = ETrue;

            break;
            }
        }

    return findResId;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::DisplayText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::DisplayText()
    {
    if ((iFont) && (iShadowFont))
        {
        // draw shadow text
        BitGc()->UseFont(iShadowFont);

        BitGc()->SetBrushStyle( CGraphicsContext::ENullBrush );
        BitGc()->SetBrushColor( KRgbBlack);//Non transparent at all
        BitGc()->SetPenColor(iShadowFontColor);
        BitGc()->SetPenStyle(CGraphicsContext::ESolidPen);
        BitGc()->SetPenSize( PenSize());

        TPoint pointText;
        TPoint pointShadowText;

        TInt leftMarginOfText = 
                     (Rect().Width() - iFont->TextWidthInPixels(*(iModeTextList[iCurrentMode])))/2;

        pointText.iX = Rect().iTl.iX + leftMarginOfText;
        pointText.iY = Rect().iTl.iY + Rect().Height()*3/4;

        pointShadowText.iX = pointText.iX + 1;
        pointShadowText.iY = pointText.iY + 1;

        //BitGc()->DrawText(*(iModeTextList[iCurrentMode]), Rect(), Rect().Height()*3/4, CGraphicsContext::ECenter, Rect().Width()/2 - iFont->TextWidthInPixels(*(iModeTextList[iCurrentMode]))/2);
        BitGc()->DrawText(*(iModeTextList[iCurrentMode]), pointShadowText);

        BitGc()->DiscardFont();

        // draw text
        BitGc()->UseFont(iFont);

        BitGc()->SetBrushStyle( CGraphicsContext::ENullBrush );
        BitGc()->SetBrushColor( KRgbBlack);//Non transparent at all
        BitGc()->SetPenColor(iFontColor);
        BitGc()->SetPenStyle(CGraphicsContext::ESolidPen);
        BitGc()->SetPenSize( PenSize());

        //BitGc()->DrawText(*(iModeTextList[iCurrentMode]), Rect(), Rect().Height()*3/4+1, CGraphicsContext::ECenter, Rect().Width()/2 - iFont->TextWidthInPixels(*(iModeTextList[iCurrentMode]))/2-1);
        BitGc()->DrawText(*(iModeTextList[iCurrentMode]), pointText);

        BitGc()->DiscardFont();

        //BitGc()->SetPenStyle(CGraphicsContext::ENullPen);        
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::Draw
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::Draw()
    {
	if(!AbleToDraw())
        return;
    
    CAknFepCtrlCommonButton::Draw();
    //CButtonBase::Draw();  
    if ( iModeTextList.Count() )
        {
        if ((iModeTextList[iCurrentMode]) && (iModeTextList[iCurrentMode]->Length() > 0)) 
            {
            DisplayText();
            }        
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SetFontColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::SetFontColor(const TRgb aFontColor)
    {
    iFontColor = aFontColor;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SetShadowFontColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::SetShadowFontColor(const TRgb aShadowFontColor)
    {
    iShadowFontColor = aShadowFontColor;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SetPermittedModes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlMultiModeButton::SetPermittedModes(const TInt aRangeId,
                                                            const TInt aRealCaseId,
                                                            TBool aPermit)
    {
    for (TInt i = 0; i < iModesList.Count(); i++)
        {
    	if ((iModesList[i]->iRangeId == aRangeId) && (iModesList[i]->iModeId == aRealCaseId))
    	    {
          	iModesList[i]->iPermitted = aPermit;
            
            return KErrNone;    
    	    }
        }
    
    return KErrNotSupported;    
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SetPermittedRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlMultiModeButton::SetPermittedRange(const TInt aRangeId, TBool aPermit)
    {
    TBool findRangeId = EFalse;
    
    for (TInt i = 0; i < iModesList.Count(); i++)
        {
        if ((iModesList[i]->iRangeId == aRangeId) && (iModesList[i]->iPermitted != aPermit))
            {
            iModesList[i]->iPermitted = aPermit;
            findRangeId = ETrue;
            }
        }
    
    if (!findRangeId)
        {
        return KErrNotSupported;
        }
    else
        {
        return KErrNone;
        }    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::EnableAllModes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::EnableAllModes()
    {
    for (TInt i = 0; i < iModesList.Count(); i++)
        {
        iModesList[i]->iPermitted = ETrue;
        }
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::DisableAllModes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::DisableAllModes()
    {
    for (TInt i = 0; i < iModesList.Count(); i++)
        {
        iModesList[i]->iPermitted = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::DisableAllModes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::DisableOneModes(const TInt aRangeId)
    {
    for (TInt i = 0; i < iModesList.Count(); i++)
        {
        if(iModesList[i]->iRangeId == aRangeId)
            {
            iModesList[i]->iPermitted = EFalse;
            TRAP_IGNORE(SetCurrentModeL(EBtnBmpNonActive, NextMode()));
            }
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::IsGivenRangeSupported
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CAknFepCtrlMultiModeButton::IsGivenRangeSupported(const TInt aRange)
    {
    for (TInt i = 0; i < iModesList.Count(); i++)
        {
        // judge if aRange is supported by at least 1 mode
        if ((iModesList[i]->iRangeId == aRange) && (iModesList[i]->iPermitted))
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SetRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlMultiModeButton::SetRange(const TInt aRange)
    {
    if (CurrentRange() != aRange)
        {
        for (TInt i = 0; i < iModesList.Count(); i++)
            {
    	    if ((iModesList[i]->iRangeId == aRange) && 
    	        (iModesList[i]->iPermitted))
    	        {
                TRAP_IGNORE(SetCurrentModeL(EBtnBmpActive, i));
                return KErrNone;
    	        }
            }
        }
    
    return KErrNotSupported;    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::CurrentRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlMultiModeButton::CurrentRange() const
    {
    if ((iCurrentMode >= 0) && (iCurrentMode <= (NumberOfModes()-1)))
        {
        return iModesList[iCurrentMode]->iRangeId;
        }
    else
        {
        return KErrNotFound;
        }    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::RangeOfGivenMode()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlMultiModeButton::RangeOfGivenMode(TInt aModeIndex) const
    {
    if ((aModeIndex >= 0) && (aModeIndex <= NumberOfModes()-1))
        {
        return iModesList[aModeIndex]->iRangeId; 
        }
    
    return KErrNotSupported;    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::GetRealCaseByMode()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlMultiModeButton::GetRealCaseByMode(TInt aModeIndex) const
    {
    if ((aModeIndex >= 0) && (aModeIndex <= NumberOfModes() -1))	
        {
        return iModesList[aModeIndex]->iModeId;
        }
    
    return KErrNotFound;    
    }

// ------------------------------------------------
// CAknFepCtrlMultiModeButton::ModeIndexByModeId
// ------------------------------------------------
EXPORT_C TInt CAknFepCtrlMultiModeButton::ModeIndexByModeId( TInt aModeId )
    {
    TInt count = NumberOfModes();
    for ( TInt i = 0; i < count; i++ )
         {
         if (iModesList[i]->iModeId == aModeId )
             {
             return i;
             }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::GetRangeListL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepCtrlMultiModeButton::GetRangeListL(RArray<TInt>& aList)
    {
	CleanupClosePushL( aList );
    for (TInt i = 0; i < NumberOfModes(); i++)
        {
        if (aList.Find(iModesList[i]->iRangeId) == KErrNotFound)
            {
            aList.AppendL(iModesList[i]->iRangeId);
            }
        }
    CleanupStack::Pop();
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::DuplicateBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CFbsBitmap* CAknFepCtrlMultiModeButton::DuplicateBitmapL(CFbsBitmap* aSourceBmp)
    {
    CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
    CleanupStack::PushL( bmp );
    
    User::LeaveIfError(bmp->Duplicate(aSourceBmp->Handle()));
    
    CleanupStack::Pop( bmp );
    //iOwnedBitmaps.InsertInAddressOrder(bmp);
    return bmp;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::ResizeBitmaps
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepCtrlMultiModeButton::ResizeBitmaps(TSize aSize)
    {
    if(iForegroundBmpList.Count() >= 1 && iForegroundBmpList[0]->SizeInPixels() != aSize
    || iForegroundMaskBmpList.Count() >= 1 && iForegroundMaskBmpList[0]->SizeInPixels() != aSize)
        {
        for (TInt i=0; i<iForegroundBmpList.Count(); i++)
            {
          	AknIconUtils::SetSize(iForegroundBmpList[i], aSize, EAspectRatioNotPreserved);
            }        
        for (TInt i=0; i<iForegroundMaskBmpList.Count(); i++)
            {
          	AknIconUtils::SetSize(iForegroundMaskBmpList[i], aSize, EAspectRatioNotPreserved);
            }
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::SizeChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiModeButton::SizeChanged(TRect aRect, TRect aInnerRect,
                                                      TBool aIsReloadImages)
    {
    if (aRect.Size().iWidth == 0 || aRect.Size().iHeight == 0)
        {
    	  return;
        }
    SetRect(aRect);
    
    iForgroundBmpRect = aInnerRect;
    
    if (aIsReloadImages)
        {
        ResizeBitmaps(aInnerRect.Size());
        }

    TRAP_IGNORE(SetButtonBmpL(iCurrentBmpType, iCurrentMode));

    CAknFepCtrlCommonButton::SizeChanged(aRect,aInnerRect,aIsReloadImages);
/*    
    Draw();
    UpdateArea(Rect(), ETrue);
*/    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlMultiModeButton::TransferOwnership
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CAknFepCtrlMultiModeButton::TransferOwnership(CFbsBitmap* aRemoved)
    {
/*    
    TInt index = iOwnedBitmaps.FindInAddressOrder(aRemoved);
    if( index != KErrNotFound )
        {
        iOwnedBitmaps.Remove(index);
        }
*/        
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlEventButton* CAknFepCtrlEventButton::NewL(CFepUiLayout* aUiLayout, 
                                                              TInt aControlId, 
                                                              TInt aEvent,
                                                              TInt aUnicode,
                                                              TAknsItemID aNormalID,
                                                              TAknsItemID aPressedID,
                                                              TAknsItemID aInactiveID)
    { 
    CAknFepCtrlEventButton* self = NewLC(aUiLayout, aControlId, aEvent, aUnicode, 
    									 aNormalID, aPressedID, aInactiveID);
    CleanupStack::Pop(self);
    
    return self;                        
    }
    
// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::NewLC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlEventButton* CAknFepCtrlEventButton::NewLC(CFepUiLayout* aUiLayout, 
                                                               TInt aControlId, 
                                                               TInt aEvent,
                                                               TInt aUnicode,
                                                               TAknsItemID aNormalID,
                                                               TAknsItemID aPressedID,
                                                               TAknsItemID aInactiveID)
    { 
    CAknFepCtrlEventButton* self = new (ELeave) CAknFepCtrlEventButton(aUiLayout, 
                                                                       aControlId,
                                                                       aEvent,
                                                                       aUnicode,
                                                                       aNormalID,
                                                                       aPressedID,
                                                                       aInactiveID);
    CleanupStack::PushL(self);
    self->BaseConstructL();

    return self;                      
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::CAknFepCtrlEventButton
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlEventButton::CAknFepCtrlEventButton(CFepUiLayout* aUiLayout, 
                                                        TInt aControlId, 
                                                        TInt aEvent,
                                                        TInt aUnicode,
                                                        TAknsItemID aNormalID,
                                                        TAknsItemID aPressedID,
                                                        TAknsItemID aInactiveID)
    : CAknFepCtrlCommonButton(aUiLayout, aControlId, aNormalID, aPressedID, aInactiveID),
	  iEvent(aEvent),iUnicode(aUnicode),iText(NULL) 
    {        
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::~CAknFepCtrlEventButton
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//                                    
EXPORT_C CAknFepCtrlEventButton::~CAknFepCtrlEventButton()
    {
    delete iText;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::HandlePointerUpEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlEventButton::HandlePointerUpEventL(const TPoint& aPoint)
    {
    if (iEvent == 0xFFFF)
        {
        return CAknFepCtrlCommonButton::HandlePointerUpEventL(aPoint);
        }
    else
        {
        CAknFepCtrlCommonButton::HandlePointerUpEventL(aPoint);
        if(IsDimmed())
            {
            return NULL;
            }
        
        if(IsActive())
            {
            SetBackgroundBmp(Bitmap(EBtnBmpActive,EFalse));
            SetBackgroundMaskBmp(Bitmap(EBtnBmpActive,ETrue));
            }
        else
            {
            SetBackgroundBmp(Bitmap(EBtnBmpNonActive,EFalse));
            SetBackgroundMaskBmp(Bitmap(EBtnBmpNonActive,ETrue));
            }
            
        SetActive(ETrue);
        Draw();
        UpdateArea(Rect(),EFalse);         
        
        TPtrC ptr = (TUint16*)(&iUnicode);
        ReportEvent(iEvent,ptr);     
        
        return this;                                      
        }    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::DisplayText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlEventButton::DisplayText()
    {
    if ((iFont) && (iShadowFont))
        {
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());

        gc->Activate(BitmapDevice());

        // draw shadow text
        gc->UseFont(iShadowFont);

        gc->SetBrushStyle( CGraphicsContext::ENullBrush );
        gc->SetBrushColor( KRgbBlack);//Non transparent at all
        gc->SetPenColor(iShadowFontColor);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->SetPenSize( PenSize());

        TPoint pointText;
        TPoint pointShadowText;

        TInt leftMarginOfText = (Rect().Width() - iFont->TextWidthInPixels(*iText))/2;

        pointText.iX = Rect().iTl.iX + leftMarginOfText;
        pointText.iY = Rect().iTl.iY + Rect().Height()*3/4;

        pointShadowText.iX = pointText.iX + 1;
        pointShadowText.iY = pointText.iY + 1;

        gc->DrawText(*iText, pointShadowText);

        gc->DiscardFont();

        // draw text
        gc->UseFont(iFont);

        gc->SetBrushStyle( CGraphicsContext::ENullBrush );
        gc->SetBrushColor( KRgbBlack);//Non transparent at all
        gc->SetPenColor(iFontColor);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->SetPenSize( PenSize());

        gc->DrawText(*iText, pointText);

        gc->DiscardFont();
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetFontColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlEventButton::SetFontColor(const TRgb aColor)
    {
    iFontColor = aColor;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetShadowFontColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlEventButton::SetShadowFontColor(const TRgb aColor)
    {
    iShadowFontColor = aColor;
    }


// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::Draw
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlEventButton::Draw()
    {
	if(!AbleToDraw())
        return;
    
    CAknFepCtrlCommonButton::Draw();

    if ( iText && (iText->Length() > 0 ) ) 
        {
        DisplayText();
        }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlEventButton::SetText(const TDesC& aText)
    {
    if ( iText )
        {
        delete iText;
        iText = NULL;
        }
    TRAPD(err, iText = aText.AllocL());
    if (err != KErrNone)
        {
        iText = NULL;
        }
    Draw();
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetFont
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlEventButton::SetFont(const CFont* aFont)
    {
    iFont = aFont;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetShadowFont
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlEventButton::SetShadowFont(const CFont* aFont)
    {
    iShadowFont = aFont;
    }
        
// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::Event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlEventButton::Event() const
    {                           
    return iEvent;                       
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::Unicode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt CAknFepCtrlEventButton::Unicode() const
    {             
    return iUnicode;                                     
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlEventButton::SetEvent(TInt aEvent)
    {               
    iEvent = aEvent;                                   
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlEventButton::SetUnicode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlEventButton::SetUnicode(TInt aUnicode)
    {                                                
    iUnicode = aUnicode;  
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlRepeatButton* CAknFepCtrlRepeatButton::NewLC(CFepUiLayout* aUiLayout, 
                                                                 TInt aCtrlId,
                                                                 TInt aEvent,
                                                                 TInt aUnicode,
                                                                 TAknsItemID aNormalID,
                                                                 TAknsItemID aPressedID,
                                                                 TAknsItemID aInactiveID)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::NewLC(), aCtrlId = %d\n"), aCtrlId));
    CAknFepCtrlRepeatButton *self = new(ELeave) CAknFepCtrlRepeatButton(aUiLayout, 
                                                                        aCtrlId, 
                                                                        aEvent, 
                                                                        aUnicode,
                                                                        aNormalID,
                                                                        aPressedID,
                                                                        aInactiveID);

    CleanupStack::PushL(self);
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlRepeatButton* CAknFepCtrlRepeatButton::NewL(CFepUiLayout* aUiLayout, 
                                                                TInt aCtrlId,
                                                                TInt aEvent,
                                                                TInt aUnicode,
                                                                TAknsItemID aNormalID,
                                                                TAknsItemID aPressedID,
                                                                TAknsItemID aInactiveID)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::NewL(), aCtrlId = %d\n"), aCtrlId));
    CAknFepCtrlRepeatButton *self = NewLC(aUiLayout, aCtrlId, aEvent, aUnicode,
                                          aNormalID, aPressedID, aInactiveID);
    CleanupStack::Pop(self);
    
    return self;
    }
        
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlRepeatButton::~CAknFepCtrlRepeatButton()
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::~CAknFepCtrlRepeatButton()\n")));
    CancelRepeat();

    delete iLongPressTimer;
    delete iRepeatTimer;
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlRepeatButton::CAknFepCtrlRepeatButton(CFepUiLayout* aUiLayout, 
                                                          TInt aCtrlId,
                                                          TInt aEvent,
                                                          TInt aUnicode,
                                                          TAknsItemID aNormalID,
                                                          TAknsItemID aPressedID,
                                                          TAknsItemID aInactiveID)
    :CAknFepCtrlEventButton(aUiLayout, aCtrlId, aEvent, aUnicode,
														aNormalID,
														aPressedID,
														aInactiveID)
    {
    iLongPressInterval = KLongPressInterval;
    iRepeatInterval = KRepeatInterval;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRepeatButton::ConstructL()
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::ConstructL()\n")));
    BaseConstructL();
    
    iLongPressTimer = CAknFepTimer::NewL(this);
    iRepeatTimer = CAknFepTimer::NewL(this);
    }
    
// ---------------------------------------------------------
// Time out event handler of both long press timer & repeat timer
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRepeatButton::HandleTimerOut(const CAknFepTimer* aTimer)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::HandleTimerOut()--")));
    if (aTimer == iLongPressTimer)
        {
        //PRINTF((_L("long press timer timeout\n")));
        iRepeatTimer->SetTimer(iRepeatInterval);
        }
    else
        {
        if (aTimer == iRepeatTimer)
            {
            //PRINTF((_L("repeat timer timeout: EEventButtonUp reported\n")));
            if ( Unicode() )
                {
                TInt unicode = Unicode();
                TPtrC ptr = (TUint16*)(&unicode ) ;
                ReportEvent(Event(),ptr);   
#ifdef RD_TACTILE_FEEDBACK
			    if (UiLayout()->SupportTactileFeedback())
			        {
			        UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveInput);
			        }
#endif //RD_TACTILE_FEEDBACK                
                }
            else
                {
                ReportEvent(EEventButtonUp);
                }
            iRepeatTimer->SetTimer(iRepeatInterval);
            }
        }
    }

// ---------------------------------------------------------
// Handle button down start long press timer 
// ---------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlRepeatButton::HandlePointerDownEventL(const TPoint& aPt)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::HandleButtonDown()\n")));
    
    //PRINTF((_L("Set long press Timer\n")));
    if( IsDimmed() )
    	{
    	return NULL;
    	}
    iLongPressTimer->SetTimer(iLongPressInterval);
    TInt unicode = Unicode();
    TPtrC ptr = (TUint16*)(&unicode);
    ReportEvent(Event(), ptr);  
    return CAknFepCtrlCommonButton::HandlePointerDownEventL(aPt);
    }
                                                          
// ---------------------------------------------------------
// Handle button up cancel timers 
// ---------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlRepeatButton::HandlePointerUpEventL(const TPoint& aPt)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::HandleButtonUp()\n")));
    
    CancelRepeat();
    
    return CAknFepCtrlCommonButton::HandlePointerUpEventL(aPt);
    }

// ---------------------------------------------------------
// Handle pointer levae event cancel timers 
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRepeatButton::HandlePointerLeave(const TPoint& aPoint)
    {
    CAknFepCtrlCommonButton::HandlePointerLeave(aPoint);

    CancelRepeat();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRepeatButton::CancelPointerDownL
// Cancel pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlRepeatButton::CancelPointerDownL()
    {
    CAknFepCtrlCommonButton::CancelPointerDownL();     
    
    CancelRepeat();
    }

// ---------------------------------------------------------
// Set the repeat interval of repeat timer
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRepeatButton::SetRepeatInterval(const TTimeIntervalMicroSeconds32& aTime)
    {
    iRepeatInterval = aTime;
    }

// ---------------------------------------------------------
// Cancel timers 
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRepeatButton::CancelRepeat()
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::CancelRepeat()\n")));
    iLongPressTimer->Cancel();
    iRepeatTimer->Cancel();
    }
    
EXPORT_C CAknFepCtrlRawKeyButton* CAknFepCtrlRawKeyButton::NewL(CFepUiLayout* aUiLayout, 
                                                                TInt aControlId,
                                                                TAknsItemID aNormalID,
                                                                TAknsItemID aPressedID,
                                                                TAknsItemID aInactiveID, 
                                                                TInt aDownEvent,
                                                                TInt aUpEvent,
                                                                TInt aScanCode)
    { 
    CAknFepCtrlRawKeyButton* self = NewLC(aUiLayout, 
                                          aControlId,
                                          aNormalID,
                                          aPressedID,
                                          aInactiveID, 
                                          aDownEvent, 
                                          aUpEvent,
                                          aScanCode);
    CleanupStack::Pop(self);
    
    return self;                        
    }
    
EXPORT_C CAknFepCtrlRawKeyButton* CAknFepCtrlRawKeyButton::NewLC(CFepUiLayout* aUiLayout, 
                                                                 TInt aControlId, 
                                                                 TAknsItemID aNormalID,
                                                                 TAknsItemID aPressedID,
                                                                 TAknsItemID aInactiveID,
                                                                 TInt aDownEvent,
                                                                 TInt aUpEvent,
                                                                 TInt aScanCode)
    { 
    CAknFepCtrlRawKeyButton* self = new (ELeave) CAknFepCtrlRawKeyButton(aUiLayout, 
                                                                         aControlId,
                                                                         aNormalID,
                                                                         aPressedID,
                                                                         aInactiveID,
                                                                         aDownEvent,
                                                                         aUpEvent,
                                                                         aScanCode);
    CleanupStack::PushL(self);
    self->BaseConstructL();
    return self;                      
    }

EXPORT_C CAknFepCtrlRawKeyButton::CAknFepCtrlRawKeyButton(CFepUiLayout* aUiLayout, 
                                                          TInt aControlId,
                                                          TAknsItemID aNormalID,
                                                          TAknsItemID aPressedID,
                                                          TAknsItemID aInactiveID, 
                                                          TInt aDownEvent,
                                                          TInt aUpEvent,
                                                          TInt aScanCode)
    : CAknFepCtrlCommonButton(aUiLayout, aControlId, aNormalID, aPressedID, aInactiveID),
      iDownEvent(aDownEvent),
      iUpEvent(aUpEvent),
      iScanCode(aScanCode),
      iLastPntDownEvent(EFalse)
    {        
    }

EXPORT_C CAknFepCtrlRawKeyButton::~CAknFepCtrlRawKeyButton()
    {
    }

EXPORT_C CFepUiBaseCtrl* CAknFepCtrlRawKeyButton::HandlePointerDownEventL(const TPoint& aPoint)
    {
/*    if (iDownEvent == KInvalidEventId)
        {
        return CButtonBase::HandlePointerDownEventL(aPoint);
        }
    
    if(IsDimmed())
        {
        return NULL;
        }
    
    CAknFepCtrlButton::HandlePointerDownEventL(aPoint);
    if(IsActive())
        {
        SetBackgroundBmp(iActiveBmpPressed);
        SetBackgroundMaskBmp(iActiveMaskBmpPressed);
        }        
    else
        {
        SetBackgroundBmp(iNonActiveBkBmpPressed);
        SetBackgroundMaskBmp(iNonActiveBkMaskBmpPressed);
        }        

    Draw();
    UpdateArea(Rect(),ETrue);
*/
    if(IsDimmed())
        {
        return NULL;
        }

    CAknFepCtrlCommonButton::HandlePointerDownEventL(aPoint);
    TKeyEvent event= {0,0,0,0};
    TPtrC eventDataPtr;
    event.iScanCode = iScanCode;
    eventDataPtr.Set(reinterpret_cast<TUint16*>(&event),sizeof(event));
        
    ReportEvent(iDownEvent,eventDataPtr);
    
    iLastPntDownEvent = ETrue;

    return this;
    }

EXPORT_C CFepUiBaseCtrl* CAknFepCtrlRawKeyButton::HandlePointerUpEventL(const TPoint& aPoint)
    {
    if (iUpEvent == KInvalidEventId)
        {
        return CButtonBase::HandlePointerUpEventL(aPoint);
        }
    
/*    if(IsDimmed())
        {
        return NULL;
        }
        
    if(IsActive())
        {
        SetBackgroundBmp(Bitmap(EBtnBmpActive,EFalse));
        SetBackgroundMaskBmp(Bitmap(EBtnBmpActive,ETrue));
        }
    else
        {
        SetBackgroundBmp(Bitmap(EBtnBmpNonActive,EFalse));
        SetBackgroundMaskBmp(Bitmap(EBtnBmpNonActive,ETrue));
        }

    SetActive(ETrue);
    Draw();
    UpdateArea(Rect(),ETrue);    
*/
    if(IsDimmed())
        {
        return NULL;
        }
        
    CAknFepCtrlCommonButton::HandlePointerUpEventL(aPoint);
    if (iLastPntDownEvent)
        {
        // only when last event is pointer down event, there is need to simulate
        // pointer up event
    	TKeyEvent event= {0,0,0,0};
        TPtrC eventDataPtr;
        event.iScanCode = iScanCode;
        eventDataPtr.Set(reinterpret_cast<TUint16*>(&event),sizeof(event));

        ReportEvent(iUpEvent,eventDataPtr);

        iLastPntDownEvent = EFalse;
        }

    return this;
    }

EXPORT_C void CAknFepCtrlRawKeyButton::ConstructFromResourceL(TResourceReader& aReader)
    {
    TInt scancode = aReader.ReadInt16();
    SetScanCode(scancode);
    
    TInt32 btnimgResId = aReader.ReadInt32();
    TResourceReader imgreader;
    CCoeEnv::Static()->CreateResourceReaderLC(imgreader, btnimgResId);
    CAknFepCtrlCommonButton::ConstructFromResourceL(imgreader);
    CleanupStack::PopAndDestroy(); // imgreader
    
    //ResizeBitmaps(Rect().Size());
    iFirstTimeConstruct = EFalse;
    }

EXPORT_C void CAknFepCtrlRawKeyButton::ConstructFromResourceL()
    {
	if (iResourceId == KInvalidResId)
	    {
	    return;
	    }
	
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);
    
    TInt scancode = reader.ReadInt16();
    SetScanCode(scancode);
    
    TInt32 btnimgResId = reader.ReadInt32();
    TResourceReader imgreader;
    CCoeEnv::Static()->CreateResourceReaderLC(imgreader, btnimgResId);
    CAknFepCtrlCommonButton::ConstructFromResourceL(imgreader);
    CleanupStack::PopAndDestroy(2); // reader, imgreader
    
    //ResizeBitmaps(Rect().Size());
    iFirstTimeConstruct = EFalse;
    }

EXPORT_C void CAknFepCtrlRawKeyButton::HandlePointerLeave(const TPoint& aPoint)
    {
    if (!Rect().Contains(aPoint))
        {
        TRAP_IGNORE(HandlePointerUpEventL(Rect().Center()));
        }
    }
    
EXPORT_C void CAknFepCtrlRawKeyButton::OnDeActivate()
    {
    CButtonBase::OnDeActivate();
    
    if (iLastPntDownEvent)
        {
        TRAP_IGNORE(HandlePointerUpEventL(Rect().Center()));
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::CAknFepCtrlCommonButton
// C++ default constructor can NOT contain any code, that
// might leave.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlCommonButton::CAknFepCtrlCommonButton(CFepUiLayout* aUiLayout, 
                                                          TInt aControlId,
                                                          TAknsItemID aNormalID,
                                                          TAknsItemID aPressedID,
                                                          TAknsItemID aInactiveID)
: CButtonBase(TRect(), aUiLayout, aControlId),
  iFirstTimeConstruct(ETrue),
  iHighlight(EFalse),
  iPressed(EFalse),
  iTextColorIndex( KInvalidColorGroup )
    {
    iNormalImgID 	= aNormalID;
    iPressedImgID	= aPressedID;
    iInactiveImgID	= aInactiveID;
    CFepUiBaseCtrl::SetActive( ETrue );
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::NewL
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlCommonButton* CAknFepCtrlCommonButton::NewL(CFepUiLayout* aUiLayout, 
                                                                TInt aControlId,
                                                                TAknsItemID aNormalID,
                                                                TAknsItemID aPressedID,
                                                                TAknsItemID aInactiveID)
    {
    CAknFepCtrlCommonButton* self = NewLC(aUiLayout, 
    									  aControlId,
    									  aNormalID,
    									  aPressedID,
    									  aInactiveID);
    CleanupStack::Pop(self);
    
    return self;
    }
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::NewLC
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlCommonButton* CAknFepCtrlCommonButton::NewLC(CFepUiLayout* aUiLayout,
                                                                 TInt aControlId,
                                                                 TAknsItemID aNormalID,
                                                                 TAknsItemID aPressedID,
                                                                 TAknsItemID aInactiveID)
    {
    CAknFepCtrlCommonButton* self = new (ELeave) CAknFepCtrlCommonButton(aUiLayout, 
    									aControlId,
    									aNormalID,
    									aPressedID,
    									aInactiveID);
    CleanupStack::PushL(self);
    self->BaseConstructL();

    return self;
    }    
    
// -----------------------------------------------------------------------------
// Destructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlCommonButton::~CAknFepCtrlCommonButton()
    {
    DeleteForgroundRes();
    delete iColorIcons;
    delete iText;
    }
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::ConstructFromResourceL
// The order of the image ids in the CButtonBase class must not be changed
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlCommonButton::ConstructFromResourceL(TResourceReader& aReader)
    {
    if (!iFirstTimeConstruct)
		{
        DeleteForgroundRes();
        if( iColorIcons )
        	iColorIcons->ReConstructL();
		}
    
    TPtrC bmpFileName = aReader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = aReader.ReadInt32();
    TInt colorGroup = aReader.ReadInt16();    
    TAknsItemID id;
	
    // Get the image ids and mask ids from resource
    TInt bmpId = aReader.ReadInt16(); 
    TInt bmpMskId = aReader.ReadInt16();
    
    // read skin item id
    const TInt skinitemid = aReader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);
    
    if (bmpId != KInvalidBmp)
        {
    	if (bmpMskId != KInvalidBmp)
    		{
    		if( colorGroup == KInvalidColorGroup )
    			{
	    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                       id,
                       iForgroundBmp,
                       iForgroundBmpMask,
                       bmpFileName,
                       bmpId,
                       bmpMskId);	
    			}
			else
				{
			    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                             id,
                             KAknsIIDQsnIconColors,
                             colorGroup,
                             iForgroundBmp,
                             iForgroundBmpMask,
                             bmpFileName,
                             bmpId,
                             bmpMskId,
                             AKN_LAF_COLOR( 0 ) );   	
				}

    		AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
    		AknIconUtils::SetSize(iForgroundBmpMask, iForgroundBmpRect.Size(), 
    		                      EAspectRatioNotPreserved);
    		                       
    		/*CreateDimmedMaskL( iForgroundDimBmpMask, iForgroundBmpMask );
    		AknIconUtils::SetSize(iForgroundDimBmpMask, iForgroundBmpRect.Size(), 
    		                      EAspectRatioNotPreserved);*/
    		}
    	else
	        {
		    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
		                           id,
		                           iForgroundBmp,
		                           bmpFileName,
		                           bmpId);
		    AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
	        }
        }
	
    if (!iFirstTimeConstruct)
    	{
        //ResizeBitmaps(iForgroundBmpRect.Size());
    	Draw();
    	}
    
    iFirstTimeConstruct = EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::ConstructFromResourceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlCommonButton::ConstructFromResourceL()
	{
	if (iResourceId == KInvalidResId)
		{
        return;
		}
	
	if (!iFirstTimeConstruct)
		{
		// need to remove original bmp and maskbmp
        DeleteForgroundRes();
        if( iColorIcons )
        	iColorIcons->ReConstructL();
		}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TPtrC bmpFileName = reader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TInt colorGroup = reader.ReadInt16();
    TAknsItemID id;
	
	// Get the image ids and mask ids from resource
    TInt bmpId = reader.ReadInt16(); 
    TInt bmpMskId = reader.ReadInt16();
    
    // Read skin item id
    const TInt skinitemid = reader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);

    if (bmpId != KInvalidBmp)
    {
	if (bmpMskId != KInvalidBmp)
		{
		if( colorGroup == KInvalidColorGroup )
			{
    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                   id,
                   iForgroundBmp,
                   iForgroundBmpMask,
                   bmpFileName,
                   bmpId,
                   bmpMskId);	
			}
		else
			{
		    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                         id,
                         KAknsIIDQsnIconColors,
                         colorGroup,
                         iForgroundBmp,
                         iForgroundBmpMask,
                         bmpFileName,
                         bmpId,
                         bmpMskId,
                         AKN_LAF_COLOR( 0 ) );   	
			}

        AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(iForgroundBmpMask, iForgroundBmpRect.Size(), 
                              EAspectRatioNotPreserved);

		/*CreateDimmedMaskL( iForgroundDimBmpMask, 
    					   iForgroundBmpMask );   
 		AknIconUtils::SetSize(iForgroundDimBmpMask, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);*/
		}
    else
        {
	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
	                           id,
	                           iForgroundBmp,
	                           bmpFileName,
	                           bmpId);
	    AknIconUtils::SetSize(iForgroundBmp, iForgroundBmpRect.Size(), EAspectRatioNotPreserved);
        }
    }

    CleanupStack::PopAndDestroy(); // reader
    
    if (!iFirstTimeConstruct)
    	{
        //ResizeBitmaps(iForgroundBmpRect.Size());
    	Draw();
    	}

    iFirstTimeConstruct = EFalse;    	
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::Draw
// Draws UI
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlCommonButton::Draw()
    {
    if(!AbleToDraw())
        return;
    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    //mask bitmaps
  	DrawOpaqueMaskBackground();  
    TRect rect = Rect();        
    TRect innerRect = rect;
    innerRect.Shrink( 10, 10 );
    
    // ----- draw bitmaps -----
    gc->Activate( BitmapDevice() );     
   // gc->SetClippingRegion( &ValidClipRegion() );           
    //gc->Clear(rect);
    	
    CFbsBitmap* bmpMask = NULL;
	
    // Draw background    
    if( IsDimmed() ) // Dimmed state
    	{
		AknsDrawUtils::DrawFrame( UiLayout()->SkinInstance(), 
		                         *gc, 
		                         rect, 
		                         innerRect,
		                         iInactiveImgID,
		                         KAknsIIDDefault );
    	if( iForgroundDimBmpMask )
    		{
    		delete iForgroundDimBmpMask;
    		iForgroundDimBmpMask = NULL;
    		}		                         
    	TRAP_IGNORE(CreateDimmedMaskL( iForgroundDimBmpMask, 
    					   iForgroundBmpMask ));
		AknIconUtils::SetSize(iForgroundDimBmpMask, iForgroundBmpRect.Size(), 
                              EAspectRatioNotPreserved);
                              		                         
		bmpMask = iForgroundDimBmpMask;    
    	}
    else
    	{
		// Normal state or pressed state
		TAknsItemID ImgID;
		if (iHighlight)
			{
			ImgID = iPressedImgID;
			}
		else 
			{
			ImgID = /*iPressed*/PointerDown()? iPressedImgID : iNormalImgID;
			}
		
		AknsDrawUtils::DrawFrame( UiLayout()->SkinInstance(), 
	                     *gc, 
	                     rect, 
	                     innerRect,
	                     ImgID,
	                     KAknsIIDDefault ); 
	    bmpMask = iForgroundBmpMask;            	    		
    	}
    
    // Draw forground
    if ( iColorIcons ) // 3-pieces color icon
    	{
    	AknPenInputDrawUtils::Draw3PiecesColorIcon( iColorIcons, 
    												*gc, 
    												iForgroundBmpRect, 
    												iDraw3PiecesInfo.iInnerRect );
    	}
	else if( iForgroundBmp )
		{
		TRect srcRect( TPoint( 0, 0 ), iForgroundBmp->SizeInPixels() );
		if( bmpMask )
			{
			gc->BitBltMasked( iForgroundBmpRect.iTl, 
							  iForgroundBmp, 
							  srcRect,
							  bmpMask,
							  EFalse);
			}
		else
			{
			gc->BitBlt( iForgroundBmpRect.iTl,
						iForgroundBmp,
						srcRect );	
			}
		}
	
    if (iText)
        {
		//if( IsDimmed() )
		//	{
		//	gc->SetFadingParameters( KFadingParamBlack, KFadingParamWhite );
        //	gc->SetFaded( ETrue );		
		//	}
        
        TAknLayoutText textLayout;
        textLayout.LayoutText(Rect(), iTextFormat);
        if( iTextColorIndex == KInvalidColorGroup )
        	textLayout.DrawText(*gc, *iText);
        else
        	{
			TRgb color( KRgbBlack );  // sane default for nonskinned case
			if ( AknsUtils::AvkonSkinEnabled() )
				{
				AknsUtils::GetCachedColor( UiLayout()->SkinInstance(),
				               color, KAknsIIDQsnTextColors, iTextColorIndex );
				}	
			if( IsDimmed() )
				{			
				color = KRgbGray;	
				}	
			textLayout.DrawText(*gc, *iText, ETrue, color);	
        	}
        //if( IsDimmed() )
        //	gc->SetFaded( EFalse );
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::SetActive
// Set active for the button
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlCommonButton::SetActive(TBool aActiveFlag)
    {
    if( IsDimmed() )
        {
        return;
        }
        
    if(IsActive() != aActiveFlag)
        {
        CFepUiBaseCtrl::SetActive(aActiveFlag);
/*       if(BitGc())
            {       
            Draw();
            UpdateArea(Rect(),EFalse);
            }*/
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::SetHighlight
// Set highlight for the button
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlCommonButton::SetHighlight(TBool aIsHighlight)
    {
    iHighlight = aIsHighlight;

    Draw();
    UpdateArea(Rect(),EFalse);         
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::ResizeBitmaps
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonButton::ResizeBitmaps(TSize aInnerSize)
    {
    if( iForgroundBmp && ( iForgroundBmp->SizeInPixels() != aInnerSize ) )
        {
        AknIconUtils::SetSize( iForgroundBmp, aInnerSize, EAspectRatioNotPreserved );
        }
    if( iForgroundBmpMask && ( iForgroundBmpMask->SizeInPixels() != aInnerSize ) )
        {
        AknIconUtils::SetSize( iForgroundBmpMask, aInnerSize, EAspectRatioNotPreserved );
        }
    
        /*TRAP_IGNORE(CreateDimmedMaskL(iForgroundDimBmpMask, 
                                      iForgroundBmpMask));*/
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::SizeChanged
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlCommonButton::SizeChanged(TRect aNewRect, 
												   TRect aInnerRect, 
												   TBool aIsReloadImages)
    {
    if (aNewRect.Size().iWidth == 0 || aNewRect.Size().iHeight == 0)
        {
   	    return;
        }
    
    TRect oriRect = Rect();
    
    if( oriRect == aNewRect && iForgroundBmpRect == aInnerRect )
        {
        return;
        }
    
    SetRect(aNewRect);

    if (aIsReloadImages)
        {
        iForgroundBmpRect = aInnerRect;
        ResizeBitmaps(aInnerRect.Size() );	
        }
    RootControl()->ReDrawRect( Rect() );
    UpdateArea(Rect(), EFalse);
    Draw();
    UpdateArea(Rect(), EFalse);
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::SetForgroundBmpRect
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlCommonButton::SetForgroundBmpRect(const TRect& aRect)
	{
	iForgroundBmpRect = aRect;	
	}

EXPORT_C void CAknFepCtrlCommonButton::SetDraw3PieceFrameInfo(TDraw3PiecesFrame a3PiecesFrameInfo)
	{
	iDraw3PiecesInfo = a3PiecesFrameInfo;
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::DeleteForgroundRes
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlCommonButton::DeleteForgroundRes()
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
	if( iForgroundDimBmpMask )
		{
		delete iForgroundDimBmpMask;
		iForgroundDimBmpMask = NULL;
		}
	}
	
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::CreateDimmedMaskL
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//	
void CAknFepCtrlCommonButton::CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                                                 const CFbsBitmap* aMask
                                        		 /*TScaleMode aScaleMode*/ )
	{
	    if (aMask && aMask->DisplayMode() == EGray256)
	        {
	        delete aDimmedMask;
	        aDimmedMask = NULL;

	        aDimmedMask = new (ELeave) CFbsBitmap;

	        User::LeaveIfError(aDimmedMask->Create(aMask->SizeInPixels(), EGray256)); 
	        CleanupStack::PushL(aDimmedMask);

	        CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(aDimmedMask);
	        CleanupStack::PushL(bitmapDevice);

	        CFbsBitGc* bitGc(NULL);
	        User::LeaveIfError(bitmapDevice->CreateContext(bitGc));
	        CleanupStack::PushL(bitGc);

	        bitGc->SetPenStyle(CGraphicsContext::ESolidPen);
	        bitGc->BitBlt(TPoint(0, 0), aMask);

	        aDimmedMask->LockHeap();
	        TInt w = aMask->SizeInPixels().iWidth; 
	        TInt h = aMask->SizeInPixels().iHeight;
	        TInt dataStride = aMask->DataStride() - w; 
	        unsigned char* address = (unsigned char *)aDimmedMask->DataAddress();  

	        for ( TInt i = 0; i < h; ++i )
	            {
	            for ( TInt j = 0; j < w; ++j )
	                {
	                *address = KTransparency[*address];
	                ++address;
	                }
	            address += dataStride;         
	            }

	        aDimmedMask->UnlockHeap();

	        //AknIconUtils::SetSize(aDimmedMask, aMask->SizeInPixels(), aScaleMode);

	        CleanupStack::PopAndDestroy(2); // bitmapDevice, bitGc
	        CleanupStack::Pop(1); // aDimmedMask
	        }

	}

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::HandlePointerDownEventL
// Handle button down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlCommonButton::HandlePointerDownEventL(const TPoint& aPt)
	{
    if( IsDimmed() )
    	return NULL;
    
    CFepUiBaseCtrl::HandlePointerDownEventL(aPt);
    iPressed =  ETrue;
	
    Draw();
    UpdateArea(Rect(),EFalse);
    ReportEvent(EEventButtonDown);
    return this;
	}
	
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CAknFepCtrlCommonButton::HandlePointerLeave(const TPoint& aPt)
    {
    if( IsDimmed() )
        return;
    
    CFepUiBaseCtrl::HandlePointerLeave(aPt);    
    iPressed = EFalse; 
	
    RootControl()->ReDrawRect( Rect() );
    UpdateArea(Rect(),EFalse);   

    return ;
    }
	
// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::HandleButtonUpEventL
// Handle button up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlCommonButton::HandlePointerUpEventL(const TPoint& aPt)        
    {
    CFepUiBaseCtrl::HandlePointerUpEventL(aPt);    
    if( IsDimmed() )
        return NULL;
	
    iPressed = EFalse;
    SetActive(ETrue);
   	RootControl()->ReDrawRect( Rect() );
    //Draw();
    UpdateArea(Rect(),EFalse);    
    
    ReportEvent(EEventButtonUp);
    
#ifdef RD_TACTILE_FEEDBACK   
    {
    UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveInput, ETrue, EFalse);
    }
#endif // RD_TACTILE_FEEDBACK    
    
    return this;
    }

EXPORT_C void CAknFepCtrlCommonButton::HandlePointerEnter(const TPoint& aPt)
    {
    if( IsDimmed() )
        return;
    
    CFepUiBaseCtrl::HandlePointerEnter(aPt);    
    iPressed = ETrue; 
    Draw();
    UpdateArea(Rect(),EFalse);   

    return ;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::CancelPointerDownL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CAknFepCtrlCommonButton::CancelPointerDownL()
    {
    CFepUiBaseCtrl::CancelPointerDownL();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlCommonButton::SetForegroundBmpL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CAknFepCtrlCommonButton::SetForegroundBmpL(CFbsBitmap* aBmp,
                                                         CFbsBitmap* aMaskBmp)
    {
    DeleteForgroundRes();
    
    iForgroundBmp = aBmp;
    iForgroundBmpMask = aMaskBmp;

    ResizeBitmaps(iForgroundBmpRect.Size());
    }

EXPORT_C void CAknFepCtrlCommonButton::Move(const TPoint& aOffset)
    {
    CButtonBase::Move(aOffset);
    iForgroundBmpRect.Move( aOffset );
    
    if ( iColorIcons ) 
        {
        iDraw3PiecesInfo.iInnerRect.Move( aOffset );
        }
    }



// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::CAknFepCtrlMultiImageButton
// C++ default constructor can NOT contain any code, that
// might leave.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiImageButton::CAknFepCtrlMultiImageButton(CFepUiLayout* aUiLayout, 
                                                                  TInt aControlId,
                                                                  TAknsItemID aNormalID,
                                                                  TAknsItemID aPressedID,
                                                                  TAknsItemID aInactiveID)
	: CAknFepCtrlCommonButton(aUiLayout, aControlId, aNormalID, aPressedID, aInactiveID)    								
    {
    iNormalImgID = aNormalID;
    iPressedImgID = aPressedID;
    iInactiveImgID = aInactiveID;       
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::NewL
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiImageButton* CAknFepCtrlMultiImageButton::NewL(CFepUiLayout* aUiLayout, 
                                                                        TInt aControlId,
                                                                        TAknsItemID aNormalID,
                                                                        TAknsItemID aPressedID,
                                                                        TAknsItemID aInactiveID)
    {
    CAknFepCtrlMultiImageButton* self = NewLC(aUiLayout, 
    									  aControlId,
    									  aNormalID,
    									  aPressedID,
    									  aInactiveID);
    CleanupStack::Pop(self);
    
    return self;
    }
// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::NewLC
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiImageButton* CAknFepCtrlMultiImageButton::NewLC(CFepUiLayout* aUiLayout,
                                                                         TInt aControlId,
                                                                         TAknsItemID aNormalID,
                                                                         TAknsItemID aPressedID,
                                                                         TAknsItemID aInactiveID)
    {
    CAknFepCtrlMultiImageButton* self = new (ELeave) CAknFepCtrlMultiImageButton(aUiLayout, 
    									aControlId,
    									aNormalID,
    									aPressedID,
    									aInactiveID);
    CleanupStack::PushL(self);
    self->BaseConstructL();

    return self;
    }    
    
// -----------------------------------------------------------------------------
// Destructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlMultiImageButton::~CAknFepCtrlMultiImageButton()
    {
    DeleteForgroundRes();
    delete iText;
    }
// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::ConstructFromResourceL
// The order of the image ids in the CButtonBase class must not be changed
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiImageButton::ConstructFromResourceL(TResourceReader& aReader)
    {
		if (!iFirstTimeConstruct)
		{
        DeleteForgroundRes();
		}
    
    TPtrC bmpFileName = aReader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = aReader.ReadInt32();
    TInt colorGroup = aReader.ReadInt16();    
    TAknsItemID id;
	
    // Get the image ids and mask ids from resource
    TInt bmpId = aReader.ReadInt16(); 
    TInt bmpMskId = aReader.ReadInt16();
    
    // read skin item id
    const TInt skinitemid = aReader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);
    
    if (bmpId != KInvalidBmp)
        {
    	if (bmpMskId != KInvalidBmp)
    		{
    		if( colorGroup == KInvalidColorGroup )
    			{
	    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                       id,
                       iForground1Bmp,
                       iForground1BmpMask,
                       bmpFileName,
                       bmpId,
                       bmpMskId);	
    			}
			else
				{
			    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                             id,
                             KAknsIIDQsnIconColors,
                             colorGroup,
                             iForground1Bmp,
                             iForground1BmpMask,
                             bmpFileName,
                             bmpId,
                             bmpMskId,
                             AKN_LAF_COLOR( 0 ) );   	
				}

    		AknIconUtils::SetSize(iForground1Bmp, iForground1BmpRect.Size(), EAspectRatioNotPreserved);
    		AknIconUtils::SetSize(iForground1BmpMask, iForground1BmpRect.Size(), 
    		                      EAspectRatioNotPreserved);
    		                       
    		/*CreateDimmedMaskL( iForground1DimBmpMask, iForground1BmpMask );
    		AknIconUtils::SetSize(iForground1DimBmpMask, iForground1BmpRect.Size(), 
    		                      EAspectRatioNotPreserved);*/
    		}
    	else
	        {
		    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
		                           id,
		                           iForground1Bmp,
		                           bmpFileName,
		                           bmpId);
		    AknIconUtils::SetSize(iForground1Bmp, iForground1BmpRect.Size(), EAspectRatioNotPreserved);
	        }
        }
	
	
	
    TAknsItemID id2;
	
    // Get the image ids and mask ids from resource
    TInt bmp2Id = aReader.ReadInt16(); 
    TInt bmp2MskId = aReader.ReadInt16();
    
    // read skin item id
    const TInt skinitem2id = aReader.ReadInt16();
    id2.Set(TInt(imgMajorSkinId), skinitem2id);
    
    if (bmp2Id != KInvalidBmp)
        {
    	if (bmp2MskId != KInvalidBmp)
    		{
    		if( colorGroup == KInvalidColorGroup )
    			{
	    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                       id2,
                       iForground2Bmp,
                       iForground2BmpMask,
                       bmpFileName,
                       bmp2Id,
                       bmp2MskId);	
    			}
			else
				{
			    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                             id2,
                             KAknsIIDQsnIconColors,
                             colorGroup,
                             iForground2Bmp,
                             iForground2BmpMask,
                             bmpFileName,
                             bmp2Id,
                             bmp2MskId,
                             AKN_LAF_COLOR( 0 ) );   	
				}

    		AknIconUtils::SetSize(iForground2Bmp, iForground2BmpRect.Size(), EAspectRatioNotPreserved);
    		AknIconUtils::SetSize(iForground2BmpMask, iForground2BmpRect.Size(), 
                              EAspectRatioNotPreserved);
    		                       
    		/*CreateDimmedMaskL( iForground2DimBmpMask, iForground2BmpMask );
    		AknIconUtils::SetSize(iForground2DimBmpMask, iForground2BmpRect.Size(), 
                              EAspectRatioNotPreserved);*/
    		}
    	else
	        {
		    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
		                           id2,
		                           iForground2Bmp,
		                           bmpFileName,
		                           bmp2Id);
		    AknIconUtils::SetSize(iForground2Bmp, iForground2BmpRect.Size(), EAspectRatioNotPreserved);
	        }
        }	
	
    if (!iFirstTimeConstruct)
    	{
    	Draw();
    	}
    
    iFirstTimeConstruct = EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::ConstructFromResourceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiImageButton::ConstructFromResourceL()
	{
	if (iResourceId == KInvalidResId)
		{
        User::Leave(KErrArgument);
		}
	
	if (!iFirstTimeConstruct)
		{
		// need to remove original bmp and maskbmp
        DeleteForgroundRes();
		}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TPtrC bmpFileName = reader.ReadTPtrC();    // Read the file name of the bmps
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TInt colorGroup = reader.ReadInt16();
    TAknsItemID id;
	
	// Get the image ids and mask ids from resource
    TInt bmpId = reader.ReadInt16(); 
    TInt bmpMskId = reader.ReadInt16();
    
    // Read skin item id
    const TInt skinitemid = reader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);

    if (bmpId != KInvalidBmp)
    {
	if (bmpMskId != KInvalidBmp)
		{
		if( colorGroup == KInvalidColorGroup )
			{
    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                   id,
                   iForground1Bmp,
                   iForground1BmpMask,
                   bmpFileName,
                   bmpId,
                   bmpMskId);	
			}
		else
			{
		    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                         id,
                         KAknsIIDQsnIconColors,
                         colorGroup,
                         iForground1Bmp,
                         iForground1BmpMask,
                         bmpFileName,
                         bmpId,
                         bmpMskId,
                         AKN_LAF_COLOR( 0 ) );   	
			}

    AknIconUtils::SetSize(iForground1Bmp, iForground1BmpRect.Size(), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(iForground1BmpMask, iForground1BmpRect.Size(), EAspectRatioNotPreserved);

		/*CreateDimmedMaskL( iForground1DimBmpMask, 
    					   iForground1BmpMask );   
 		AknIconUtils::SetSize(iForground1DimBmpMask, iForground1BmpRect.Size(), EAspectRatioNotPreserved);*/
		}
    else
        {
	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
	                           id,
	                           iForground1Bmp,
	                           bmpFileName,
	                           bmpId);
	    AknIconUtils::SetSize(iForground1Bmp, iForground1BmpRect.Size(), EAspectRatioNotPreserved);
        }
    }












    TAknsItemID id2;
	
	// Get the image ids and mask ids from resource
    TInt bmp2Id = reader.ReadInt16(); 
    TInt bmp2MskId = reader.ReadInt16();
    
    // Read skin item id
    const TInt skinitem2id = reader.ReadInt16();
    id2.Set(TInt(imgMajorSkinId), skinitem2id);

    if (bmp2Id != KInvalidBmp)
    {
	if (bmp2MskId != KInvalidBmp)
		{
		if( colorGroup == KInvalidColorGroup )
			{
    		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
                   id2,
                   iForground2Bmp,
                   iForground2BmpMask,
                   bmpFileName,
                   bmp2Id,
                   bmp2MskId);	
			}
		else
			{
		    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
                         id,
                         KAknsIIDQsnIconColors,
                         colorGroup,
                         iForground2Bmp,
                         iForground2BmpMask,
                         bmpFileName,
                         bmp2Id,
                         bmp2MskId,
                         AKN_LAF_COLOR( 0 ) );   	
			}

		AknIconUtils::SetSize(iForground2Bmp, iForground2BmpRect.Size(), EAspectRatioPreserved);
		AknIconUtils::SetSize(iForground2BmpMask, iForground2BmpRect.Size(), EAspectRatioPreserved);

		/*CreateDimmedMaskL( iForground2DimBmpMask, iForground2BmpMask );   
 		AknIconUtils::SetSize(iForground2DimBmpMask, iForground2BmpRect.Size(), 
                          EAspectRatioNotPreserved);*/
		}
    else
        {
	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
	                           id,
	                           iForground2Bmp,
	                           bmpFileName,
	                           bmp2Id);
	    AknIconUtils::SetSize(iForground2Bmp, iForground2BmpRect.Size(), EAspectRatioPreserved);
        }
    }






    CleanupStack::PopAndDestroy(); // reader
    
    if (!iFirstTimeConstruct)
    	{
        //ResizeBitmaps(iForgroundBmpRect.Size());
    	Draw();
    	}

    iFirstTimeConstruct = EFalse;    	
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::Draw
// Draws UI
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiImageButton::Draw()
    {
	if(!AbleToDraw())
        return;
    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    //mask bitmaps
  	DrawOpaqueMaskBackground();  
    TRect rect = Rect();        
    TRect innerRect = rect;
    innerRect.Shrink( 10, 10 );
    
    // ----- draw bitmaps -----
    gc->Activate( BitmapDevice() );     
   // gc->SetClippingRegion( &ValidClipRegion() );           
    //gc->Clear(rect);
    	
		CFbsBitmap* bmpMask1 = NULL;
		CFbsBitmap* bmpMask2 = NULL;
	
		// Draw background    
		if( IsDimmed() ) // Dimmed state
    	{
		AknsDrawUtils::DrawFrame( UiLayout()->SkinInstance(), 
		                         *gc, 
		                         rect, 
		                         innerRect,
		                         iInactiveImgID,
		                         KAknsIIDDefault );
        if( iForground1DimBmpMask )
    		{
    		delete iForground1DimBmpMask;
    		iForground1DimBmpMask = NULL;
    		}		                         
        TRAP_IGNORE(CreateDimmedMaskL( iForground1DimBmpMask, 
    					   iForground1BmpMask ));
		AknIconUtils::SetSize(iForground1DimBmpMask, iForground1BmpRect.Size(), 
                              EAspectRatioNotPreserved);		                         
		bmpMask1 = iForground1DimBmpMask;
		
		if( iForground2DimBmpMask )
    		{
    		delete iForground2DimBmpMask;
    		iForground2DimBmpMask = NULL;
    		}		                         
		TRAP_IGNORE(CreateDimmedMaskL( iForground2DimBmpMask, 
    					   iForground2BmpMask ));
		AknIconUtils::SetSize(iForground2DimBmpMask, iForground2BmpRect.Size(), 
                              EAspectRatioNotPreserved);
		bmpMask2 = iForground2DimBmpMask;  
    	}
    else
    	{
		// Normal state or pressed state
		TAknsItemID ImgID;
		if (iHighlight)
			{
			ImgID = iPressedImgID;
			}
		else 
			{
			ImgID = /*iPressed*/PointerDown()? iPressedImgID : iNormalImgID;
			}
		
		AknsDrawUtils::DrawFrame( UiLayout()->SkinInstance(), 
	                     *gc, 
	                     rect, 
	                     innerRect,
	                     ImgID,
	                     KAknsIIDDefault ); 
	    bmpMask1 = iForground1BmpMask;
	    bmpMask2 = iForground2BmpMask;
    	}
    
    // Draw forground
		if( iForground1Bmp && iForground2Bmp)
		{
		TRect srcRect( TPoint( 0, 0 ), iForground1Bmp->SizeInPixels() );
		if( bmpMask1 )
			{
			gc->BitBltMasked( iForground1BmpRect.iTl, 
							  iForground1Bmp, 
							  srcRect,
							  bmpMask1,
							  EFalse);
			}
		else
			{
			gc->BitBlt( iForground1BmpRect.iTl,
						iForground1Bmp,
						srcRect );	
			}
			
		TRect src2Rect( TPoint( 0, 0 ), iForground2Bmp->SizeInPixels() );	
		if( bmpMask2 )
			{
			gc->BitBltMasked( rect.iTl, // layout of 2nd foreground is determined by image itself, but not hard code.   
							  iForground2Bmp, 
							  src2Rect,
							  bmpMask2,
							  EFalse);
			}
		else
			{
			gc->BitBlt( rect.iTl, 
						iForground2Bmp,
						src2Rect );	
			}		
		}
	
    if (iText)
        {
        TAknLayoutText textLayout;
        textLayout.LayoutText(Rect(), iTextFormat);
        textLayout.DrawText(*gc, *iText);
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::ResizeBitmaps
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlMultiImageButton::ResizeBitmaps(TSize aInnerSize)
    {
    if( iForground1Bmp && ( iForground1Bmp->SizeInPixels() != aInnerSize ) )
    	{
    	AknIconUtils::SetSize( iForground1Bmp, aInnerSize, EAspectRatioNotPreserved );
    	}
    if( iForground1BmpMask && ( iForground1BmpMask->SizeInPixels() != aInnerSize ) )
    	{
    	AknIconUtils::SetSize( iForground1BmpMask, aInnerSize, EAspectRatioNotPreserved );
    	}    
    	
    //TRAP_IGNORE(CreateDimmedMaskL(iForground1DimBmpMask, iForground1BmpMask));
    				  
    				  
    				  
    				  
    if( iForground2Bmp && ( iForground2Bmp->SizeInPixels() != Rect().Size() ) )
    	{
    	AknIconUtils::SetSize( iForground2Bmp, Rect().Size(), EAspectRatioNotPreserved );
    	}
    if( iForground2BmpMask && ( iForground2BmpMask->SizeInPixels() != Rect().Size() ) )
    	{
    	AknIconUtils::SetSize( iForground2BmpMask, Rect().Size(), EAspectRatioNotPreserved );
    	}    
    	
    //TRAP_IGNORE(CreateDimmedMaskL(iForground2DimBmpMask, iForground2BmpMask));    				  
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::SizeChanged
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlMultiImageButton::SizeChanged(TRect aNewRect, 
												   TRect aInnerRect, 
												   TBool aIsReloadImages)
    {
    if (aNewRect.Size().iWidth == 0 || aNewRect.Size().iHeight == 0)
        {
   	    return;
        }
    
    TRect oriRect = Rect();
    SetRect(aNewRect);
    iForground1BmpRect = aInnerRect;
    iForground2BmpRect = aInnerRect;
	
    if (aIsReloadImages)
        {
        ResizeBitmaps(aInnerRect.Size() );	
        }
    RootControl()->ReDrawRect( oriRect );
    UpdateArea(oriRect, EFalse);
    Draw();
    UpdateArea(Rect(), EFalse);
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::DeleteForgroundRes
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlMultiImageButton::DeleteForgroundRes()
	{
    if( iForground1Bmp )
		{
		delete iForground1Bmp;
		iForground1Bmp = NULL;
		}
	if( iForground1BmpMask )
		{
		delete iForground1BmpMask;
		iForground1BmpMask = NULL;
		}
	if( iForground1DimBmpMask )
		{
		delete iForground1DimBmpMask;
		iForground1DimBmpMask = NULL;
		}
	
	
    if( iForground2Bmp )
		{
		delete iForground2Bmp;
		iForground2Bmp = NULL;
		}
	if( iForground2BmpMask )
		{
		delete iForground2BmpMask;
		iForground2BmpMask = NULL;
		}
	if( iForground2DimBmpMask )
		{
		delete iForground2DimBmpMask;
		iForground2DimBmpMask = NULL;
		}	

	}
	
// ---------------------------------------------------------------------------
// CAknFepCtrlMultiImageButton::CreateDimmedMaskL
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//	
void CAknFepCtrlMultiImageButton::CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                                        		 const CFbsBitmap* aMask
                                        		 /*TScaleMode aScaleMode*/ )
	{
	    if (aMask && aMask->DisplayMode() == EGray256)
	        {
	        delete aDimmedMask;
	        aDimmedMask = NULL;

	        aDimmedMask = new (ELeave) CFbsBitmap;

	        User::LeaveIfError(aDimmedMask->Create(aMask->SizeInPixels(), EGray256)); 
	        CleanupStack::PushL(aDimmedMask);

	        CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(aDimmedMask);
	        CleanupStack::PushL(bitmapDevice);

	        CFbsBitGc* bitGc(NULL);
	        User::LeaveIfError(bitmapDevice->CreateContext(bitGc));
	        CleanupStack::PushL(bitGc);

	        bitGc->SetPenStyle(CGraphicsContext::ESolidPen);
	        bitGc->BitBlt(TPoint(0, 0), aMask);

	        aDimmedMask->LockHeap();
	        TInt w = aMask->SizeInPixels().iWidth; 
	        TInt h = aMask->SizeInPixels().iHeight;
	        TInt dataStride = aMask->DataStride() - w; 
	        unsigned char* address = (unsigned char *)aDimmedMask->DataAddress();  

	        for ( TInt i = 0; i < h; ++i )
	            {
	            for ( TInt j = 0; j < w; ++j )
	                {
	                *address = KTransparency[*address];
	                ++address;
	                }
	            address += dataStride;         
	            }

	        aDimmedMask->UnlockHeap();

	        //AknIconUtils::SetSize(aDimmedMask, aMask->SizeInPixels(), aScaleMode);

	        CleanupStack::PopAndDestroy(2); // bitmapDevice, bitGc
	        CleanupStack::Pop(1); // aDimmedMask
	        }

	}

EXPORT_C void CAknFepCtrlMultiImageButton::Move(const TPoint& aOffset)
    {
    CButtonBase::Move(aOffset);
    iForground1BmpRect.Move( aOffset );
    iForground2BmpRect.Move( aOffset );
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlLongPressButton* CAknFepCtrlLongPressButton::NewLC(CFepUiLayout* aUiLayout, 
                                                                 TInt aCtrlId,
                                                                 TInt aEvent,
                                                                 TInt aUnicode,
                                                                 TAknsItemID aNormalID,
                                                                 TAknsItemID aPressedID,
                                                                 TAknsItemID aInactiveID)
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::NewLC(), aCtrlId = %d\n"), aCtrlId));
    CAknFepCtrlLongPressButton *self = new(ELeave) CAknFepCtrlLongPressButton(aUiLayout, 
                                                                        aCtrlId, 
                                                                        aEvent, 
                                                                        aUnicode,
                                                                        aNormalID,
                                                                        aPressedID,
                                                                        aInactiveID);

    CleanupStack::PushL(self);
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlLongPressButton* CAknFepCtrlLongPressButton::NewL(CFepUiLayout* aUiLayout, 
                                                                TInt aCtrlId,
                                                                TInt aEvent,
                                                                TInt aUnicode,
                                                                TAknsItemID aNormalID,
                                                                TAknsItemID aPressedID,
                                                                TAknsItemID aInactiveID)
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::NewL(), aCtrlId = %d\n"), aCtrlId));
    CAknFepCtrlLongPressButton *self = NewLC(aUiLayout, aCtrlId, aEvent, aUnicode,
                                          aNormalID, aPressedID, aInactiveID);
    CleanupStack::Pop(self);
    
    return self;
    }
        
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlLongPressButton::~CAknFepCtrlLongPressButton()
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::~CAknFepCtrlLongPressButton()\n")));
    CancelTimer();

    delete iLongPressTimer;
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlLongPressButton::CAknFepCtrlLongPressButton(CFepUiLayout* aUiLayout, 
                                                          TInt aCtrlId,
                                                          TInt aEvent,
                                                          TInt aUnicode,
                                                          TAknsItemID aNormalID,
                                                          TAknsItemID aPressedID,
                                                          TAknsItemID aInactiveID)
    :CAknFepCtrlEventButton(aUiLayout, aCtrlId, aEvent, aUnicode,
                                                        aNormalID,
                                                        aPressedID,
                                                        aInactiveID)
    {
    iLongPressInterval = KLongPressInterval;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlLongPressButton::ConstructL()
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::ConstructL()\n")));
    BaseConstructL();
    iIsLongPress = EFalse;
    iLongPressTimer = CAknFepTimer::NewL(this);
    }
    
// ---------------------------------------------------------
// Time out event handler of long press timer
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlLongPressButton::HandleTimerOut(const CAknFepTimer* aTimer)
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::HandleTimerOut()--")));
    if (aTimer == iLongPressTimer)
        {
        iIsLongPress = ETrue;
        CancelTimer();
        TInt unicode = Unicode();
        TPtrC ptr = (TUint16*)(&unicode);
        ReportEvent(EPeninputLayoutEventMultiRangeLongPress, ptr);
        }
    }

// ---------------------------------------------------------
// Handle button down start long press timer 
// ---------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlLongPressButton::HandlePointerDownEventL(const TPoint& aPt)
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::HandleButtonDown()\n")));
    //PRINTF((_L("Set long press Timer\n")));
    if ( IsDimmed() )
        {
        return NULL;
        }
    
    iLongPressTimer->SetTimer(iLongPressInterval);
    iIsLongPress = EFalse;

    return CAknFepCtrlCommonButton::HandlePointerDownEventL(aPt);
    }
                                                          
// ---------------------------------------------------------
// Handle button up cancel timer
// ---------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlLongPressButton::HandlePointerUpEventL(const TPoint& aPt)
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::HandleButtonUp()\n")));

    if ( IsDimmed() )
        {
        return NULL;
        }

    CancelTimer();
    
    if (!iIsLongPress)
        {
        TInt unicode = Unicode();
        TPtrC ptr = (TUint16*)(&unicode);
        ReportEvent(EPeninputLayoutEventMultiRange, ptr);   
        }

    iIsLongPress = EFalse;
    
    
    return CAknFepCtrlCommonButton::HandlePointerUpEventL(aPt);
    }

// ---------------------------------------------------------
// Handle pointer leave event cancel timer 
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlLongPressButton::HandlePointerLeave(const TPoint& aPoint)
    {
    CAknFepCtrlCommonButton::HandlePointerLeave(aPoint);

    CancelTimer();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlLongPressButton::CancelPointerDownL
// Cancel pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlLongPressButton::CancelPointerDownL()
    {
    // No implementation needed
	CAknFepCtrlCommonButton::CancelPointerDownL();
    }

// ---------------------------------------------------------
// Cancel timer
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlLongPressButton::CancelTimer()
    {
    //PRINTF((_L("CAknFepCtrlLongPressButton::CancelTimer()\n")));
    iLongPressTimer->Cancel();
    }
//  End Of File
