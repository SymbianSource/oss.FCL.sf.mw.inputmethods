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
* Description:  vkb control extension
*
*/


// System includes
#include <barsread.h>
#include <peninputlayout.h>
#include <AknIconUtils.h>
#include <AknsUtils.h>
#include <coemain.h>

// User includes
#include "peninputvkbctrlext.h"
#include "peninputvkbctrllayout.h"

// Constants
const TUint32 KDefaultTextColor = 0x000000;
const TInt32 KInvalidResId = -1;
const TInt KInvalidImg = -1 ;
// ======== MEMBER FUNCTIONS ========



// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::CPeninputVkbCtrlExt
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbCtrlExt::CPeninputVkbCtrlExt( const TRect& aRect,
    CFepUiLayout* aUiLayout, TInt aControlId, const TFontSpec& aFontSpec,TBool aIrregular )
    : CVirtualKeyboard( aRect, aUiLayout, aControlId, aFontSpec, aIrregular )
    {
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbCtrlExt* CPeninputVkbCtrlExt::NewL( const TRect& aRect,
    CFepUiLayout* aUiLayout, TInt aControlId, const CFont* aFont,TBool aIrregular)
    {
    TFontSpec fontSpec;
    
    CPeninputVkbCtrlExt* vkb = new (ELeave)  
        CPeninputVkbCtrlExt( aRect, aUiLayout, aControlId, fontSpec,aIrregular );
    CleanupStack::PushL( vkb );
    vkb->ConstructL();
    vkb->SetFont( aFont );
    CleanupStack::Pop( vkb );
    
    return vkb;
    }
            
// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::~CPeninputVkbCtrlExt
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbCtrlExt::~CPeninputVkbCtrlExt()
    {
    Reset();
    if( iResourceId == KInvalidImg )
    	{
		SetBackgroundBmp( NULL );
		SetBackgroundMaskBmp( NULL );    		
    	}
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::SizeChanged
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::SizeChanged( const TInt aUnitWidth,
                                                const TInt aUnitHeight,
                                                const TPoint& aLeftTopPoint )
    {
    iUnitWidth = aUnitWidth;
    iUnitHeight = aUnitHeight;
    iLeftTopPoint = aLeftTopPoint;
    
    if ( iVkbLayoutInfo )
        {        
        // 1.Reset the size and position of Vkb ctrl and keys
        TRect rect = TRect( iLeftTopPoint, TSize( iUnitWidth * iVkbLayoutInfo->Columns(),
                                                  iUnitHeight * iVkbLayoutInfo->Rows() ) );
        SetRect( rect );
            
        // 2.Move all keys(Needn't change size, because it has been 
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::SizeChanged
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::SizeChanged( const TRect& aRect )
    {
    SetRect( aRect );
    }
    
    
// CPeninputVkbCtrlExt::ReorganizeVkbKeys
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::ReorganizeVkbKeys(
    CPeninputVkbLayoutInfo* aVkbLayoutInfo )
    {           
    Reset();

    iVkbLayoutInfo = aVkbLayoutInfo;

    TRect rect = TRect( iLeftTopPoint, TSize( iUnitWidth * iVkbLayoutInfo->Columns(),
                                              iUnitHeight * iVkbLayoutInfo->Rows() ) );
	if( iResourceId != KInvalidImg )
		{
		if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )                                              
			AknIconUtils::SetSize(BackgroundBmp(), Rect().Size(), EAspectRatioNotPreserved);
		if( BkMaskBmp() && BkMaskBmp()->SizeInPixels() != Rect().Size() ) 
			AknIconUtils::SetSize(BkMaskBmp(), Rect().Size(), EAspectRatioNotPreserved);
		}
	else
		{		
	    if ( iVkbLayoutInfo->BgBmp() )
	        {	        
	        SetBackgroundBmp( iVkbLayoutInfo->BgBmp() ) ;
	        }
	    
	    if ( iVkbLayoutInfo->BgMaskBmp() )
	        {
	        SetBackgroundMaskBmp( iVkbLayoutInfo->BgMaskBmp() );
	        }			
		}	                                       
//    SetRect( rect );
            
    // According the information in vkbLayout to construct all keys and
    // then add into the keys.
/*    
    if ( iVkbLayoutInfo->BgBmp() )
        {
        TRAP_IGNORE( SetBackgroundBitmapL( iVkbLayoutInfo->BgBmp() ) );
        }
    
    if ( iVkbLayoutInfo->BgMaskBmp() )
        {
        TRAP_IGNORE( SetBackgroundMaskBitmapL( iVkbLayoutInfo->BgMaskBmp() ) );
        }
*/
    if ( iVkbLayoutInfo->DimBmp() )
        {
        SetDimKeyBmp( iVkbLayoutInfo->DimBmp() );
        }
    
    if ( iVkbLayoutInfo->DimMaskBmp() )
        {
        SetDimKeyMaskBmp( iVkbLayoutInfo->DimMaskBmp() );
        }

    TInt count = iVkbLayoutInfo->HighlightBmps().Count();
    
    TRAP_IGNORE(
    for ( TInt i = 0; i < count; i++ )
        {
        AddKeyHighLightBitmapL(iVkbLayoutInfo->HighlightBmps()[i]);
        }
        );
        
    count = iVkbLayoutInfo->HighlightMaskBmps().Count();

    TRAP_IGNORE(
        for ( TInt i = 0; i < count; i++ )
            {
            AddKeyHighLightMaskBitmapL(iVkbLayoutInfo->HighlightMaskBmps()[i]);
            }
        );
    
    count = iVkbLayoutInfo->KeyInfoList().Count();
    TRAP_IGNORE(
    for ( TInt i = 0; i < count; i++ )
        {
        iVkbLayoutInfo->KeyInfoList()[i]->Key()->SetDimmed( EFalse );
        AddKeyL( iVkbLayoutInfo->KeyInfoList()[i]->Key() );
        }
    );
   
    TRgb textColor;
    TInt error = AknsUtils::GetCachedColor(AknsUtils::SkinInstance(),
                                           textColor,
                                           KAknsIIDQsnTextColors,
                                           EAknsCIQsnTextColorsCG59);
    if (error != KErrNone)
        {
    	textColor = TRgb( KDefaultTextColor );
        }  
    SetFontColor( textColor );
    
    // Remove that draw operation to improve performance
    //Draw();
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::Reset
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::Reset()
    {
    const RPointerArray<CVirtualKey>& keys = KeyArray();
    for(TInt i = keys.Count() - 1 ; i >= 0 ; --i)    
        {
        RemoveKey(keys[i]);
        }
    
    const RPointerArray<CFbsBitmap>& bmps = KeyHighlightBmpList();
    for(TInt i = bmps.Count() - 1 ; i >= 0 ; --i)
        {
        RemoveKeyHighLightBitmap(i);
        }

    const RPointerArray<CFbsBitmap>& maskbmps = KeyHighlightMaskBmpList();    
    for(TInt i = maskbmps.Count() - 1 ; i >= 0 ; --i)
        {
        RemoveKeyHighLightMaskBitmap(i);
        }    
            
    SetDimKeyBmp( NULL );
    SetDimKeyMaskBmp( NULL );    
    
    iVkbLayoutInfo = NULL;
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::Move
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::Move( const TPoint& aOffset )
    {
    TRect rect = Rect();
    rect.Move( aOffset );
    SetRect( rect );     
    }
    
// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::DimKeys
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::DimKeys( const HBufC* aPermitted )
    {
    TInt count = iVkbLayoutInfo->KeyInfoList().Count();
    
    for ( TInt i = 0; i < count ; i++ )
        {
        TChar ch = TChar( (TUint)( KeyArray()[i]->KeyCode() ) );
        TInt index = aPermitted->Locate( ch ) ;
        
        if ( index == KErrNotFound )
            {
            DimKey( KeyArray()[i], ETrue );
            }     
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::DimKeySet
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::DimKeySet( const HBufC* aSet, TBool aDim )
    {
    TInt keyCount = iVkbLayoutInfo->KeyInfoList().Count();
    TInt setCount = const_cast<HBufC*>(aSet)->Des().Length();
    TInt findNum = 0;
    
    for ( TInt i = 0; i < keyCount ; i++ )
        {
        TChar ch = TChar( (TUint)( KeyArray()[i]->KeyCode() ) );
        TInt index = aSet->Locate( ch ) ;
        
        if ( index != KErrNotFound )
            {
            findNum++;
            DimKey( KeyArray()[i], aDim );
            }     
        
        if ( findNum == setCount )
            {
            break;
            }
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::CancelDims
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::CancelDims()
    {
    TInt count = iVkbLayoutInfo->KeyInfoList().Count();
    
    for ( TInt i = 0; i < count ; i++ )
        {
        DimKey( KeyArray()[i], EFalse );
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExt::ConstructFromResourceL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbCtrlExt::ConstructFromResourceL()
	{
	if (iResourceId == KInvalidResId)
		{
	    return;
		}
	
	// Release original resource
    if( BackgroundBmp() )
    	{
    	CFbsBitmap* bkBmp = BackgroundBmp();
    	delete bkBmp;	
    	SetBackgroundBmp( NULL );
    	}
    if( BkMaskBmp() )
    	{
    	CFbsBitmap* bkMaskBmp = BkMaskBmp();
    	delete bkMaskBmp;
		SetBackgroundMaskBmp( NULL );
    	}    
    		
    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, iResourceId );                                             
     
    TPtrC bmpFileName = resReader.ReadTPtrC();
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    const TInt16 bmpId = resReader.ReadInt16();
    const TInt16 bmpMskId = resReader.ReadInt16();
    skinitemid = resReader.ReadInt16();   
    id.Set(TInt(imgMajorSkinId), skinitemid);
	
    if (bmpId != KInvalidImg)
    	{
    	CFbsBitmap* backgroundImg = NULL;
    	CFbsBitmap* backgroundMaskImg = NULL;
        if (bmpMskId != KInvalidImg)
            {
            AknsUtils::CreateIconL(skininstance, 
                                   id,
                                   backgroundImg,
                                   backgroundMaskImg,
                                   bmpFileName,
                                   bmpId,
                                   bmpMskId);
            }
        else
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   backgroundImg,
                                   bmpFileName,
                                   bmpId);
            }
       
        SetBackgroundBmp( backgroundImg );
        SetBackgroundMaskBmp( backgroundMaskImg );           
    	}
    CleanupStack::PopAndDestroy( 1 );		
	}
