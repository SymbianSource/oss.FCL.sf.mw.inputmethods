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
#include <AknIconUtils.h>
#include <coemain.h>
#include <AknsUtils.h>

// user includes
#include "peninputvkbctrllayout.h"
#include "peninputvkbctrlext.h"

// constants
const TInt KInvalidBmp = -1 ;


// ======== MEMBER FUNCTIONS ========
   
// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::CAknFepCtrlVkbLayout
// C++ default constructor can NOT contain any code, that
// might leave.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

  
CAknFepCtrlVkbLayout::CAknFepCtrlVkbLayout()
    : iFirstTimeConstruct(ETrue), iResourceId( -1 ), iVkbLayoutId( -1 )
    {
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::NewL
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlVkbLayout* CAknFepCtrlVkbLayout::NewL()
    {
    CAknFepCtrlVkbLayout* self = NewLC();
    CleanupStack::Pop(self);
    
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::NewLC
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlVkbLayout* CAknFepCtrlVkbLayout::NewLC()
    {
    CAknFepCtrlVkbLayout* self = new (ELeave) CAknFepCtrlVkbLayout();
    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }    
 
// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::Reset
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlVkbLayout::Reset()
    {
    iVkbLayoutId = -1;
    iFirstTimeConstruct = ETrue;
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::ConstructL
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlVkbLayout::ConstructL()
    {
    
    }
    
// -----------------------------------------------------------------------------
// Destructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlVkbLayout::~CAknFepCtrlVkbLayout()
    {
    iVkbLayoutInfoList.ResetAndDestroy();
    iVkbLayoutInfoList.Close();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::SetVkbKeyboard
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAknFepCtrlVkbLayout::SetVkbKeyboard( 
    CPeninputVkbCtrlExt* aVkbCtrl )
    {
    iVkbCtrl = aVkbCtrl;
    }

EXPORT_C void CAknFepCtrlVkbLayout::SetIrregularResourceId(TInt32 aResId)
    {
    iIrregularResId = aResId;
    }    
    
EXPORT_C void CAknFepCtrlVkbLayout::SetNonIrregularResourceId(TInt32 aResId)
    {
    iNonIrregularResId = aResId;
    }    
    

EXPORT_C void CAknFepCtrlVkbLayout::ConstructFromIrregularResourceL()          
    {
    if ( iIrregularResId < 0 )
        {
        return;
        }
    
    TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, iIrregularResId );      
    
    TPtrC bmpFileName = reader.ReadTPtrC();
      
    for (TInt index = 0; index <= EKeyRightUp; index ++)
        {
        CreateIrregularKeyBitmapsL(reader, bmpFileName, (TVirtualKeyIrregularKeyType) index);
        }
    
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );
  	for( TInt i = 0; i < iVkbLayoutInfoList.Count(); i++ )
   	    {
   	    iVkbLayoutInfoList[i]->iIrregular = ETrue;
        }
     
    }
    
EXPORT_C void CAknFepCtrlVkbLayout::ConstructFromNonIrregularResourceL()          
    {
    if ( iNonIrregularResId < 0 )
        {
        return;
        }
        		
    TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, iNonIrregularResId );      
    
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TAknsItemID id;
      
    for (TInt index = 0; index <= EKeyBmpLastType ; index += 2)
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
        		AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
        		                       id,
        		                       bmp,
        		                       maskbmp,
        		                       bmpFileName,
        		                       bmpId,
        		                       bmpMskId);
        		
       		    // set maskbmp and size
                iVkbCtrl->SetNonIrregularKeyBitmapL(TVirtualKeyBmpType(EKeyBmpNormal + index + 1), 
                                                    maskbmp );
                }
            else
                {
        	    AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
        	                           id,
        	                           bmp,
        	                           bmpFileName,
        	                           bmpId);
                }

            // set bmp and size
           	iVkbCtrl->SetNonIrregularKeyBitmapL( TVirtualKeyBmpType(EKeyBmpNormal + index), bmp );
            }       
        }
    
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );
  	for( TInt i = 0; i < iVkbLayoutInfoList.Count(); i++ )
   	    {
   	    iVkbLayoutInfoList[i]->iIrregular = EFalse;
        }
     
    }  
void CAknFepCtrlVkbLayout::CreateIrregularKeyBitmapsL(TResourceReader& reader, TPtrC& bmpFileName, 
                                                      TVirtualKeyIrregularKeyType aKeyType)
    {
    int maskid = 0;
    for(TInt index = 0; index <= EKeyStateDimmedMask; index += 2)
        {
        maskid = index + 1;
        CreateIrregularKeyBitmapL(reader, bmpFileName, aKeyType ,
                                 (TVirtualKeyIrregularKeyState)index,
                                 (TVirtualKeyIrregularKeyState)maskid);         
        }
    }

void CAknFepCtrlVkbLayout::CreateIrregularKeyBitmapL(TResourceReader& reader, TPtrC& bmpFileName, 
                                                    TVirtualKeyIrregularKeyType aKeyType ,
                                                    TVirtualKeyIrregularKeyState aKeyState, 
                                                    TVirtualKeyIrregularKeyState aKeyStateMask)
    {
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* bitmapmaks = NULL;
    TAknsItemID id;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    
    TInt bmpID = reader.ReadInt16();
    TInt bmpmaskID = reader.ReadInt16();
    TInt minorskinID = reader.ReadInt16();    
    id.Set(TInt(EAknsMajorGeneric), minorskinID);
    
    if (bmpID > 0)
        {
        
        AknsUtils::CreateIconL( skininstance,
                                id,
                                bitmap,
                                bitmapmaks,
                                bmpFileName,
                                bmpID,
                                bmpmaskID );        

        if (bitmapmaks)
            {
            AknIconUtils::DisableCompression(bitmapmaks);
            }
            
        if (bitmap && iVkbCtrl)
            {
            iVkbCtrl->AddIrregularKeyBitmapL(aKeyType, aKeyState, bitmap);
            }
            
        if (bitmapmaks && iVkbCtrl)
            {
            iVkbCtrl->AddIrregularKeyBitmapL(aKeyType, aKeyStateMask, bitmapmaks);
            }            
        }
    }
        
// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::SetResourceId
// Set resurce Id
// ---------------------------------------------------------------------------
//        
EXPORT_C void CAknFepCtrlVkbLayout::SetResourceId(TInt32 aResId)
    {
    iResourceId = aResId;
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::ConstructFromResourceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlVkbLayout::ConstructFromResourceL()
	{
    if ( iResourceId < 0 )
        {
        return;
        }
    
    TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, iResourceId );      
      
    TInt count = reader.ReadInt16();
    
    if( iFirstTimeConstruct )
        {
        iVkbLayoutInfoList.ResetAndDestroy();
        for ( TInt i = 0; i < count; i++ )
            {
            CPeninputVkbLayoutInfo* vkbLayoutInfo = 
                CPeninputVkbLayoutInfo::NewL( reader );
                
            if (iVkbCtrl)
            	{
            	vkbLayoutInfo->iIrregular = iVkbCtrl->IsIrregularKeyBoard();	
            	}
            iVkbLayoutInfoList.Append( vkbLayoutInfo );
            }
        }
    else
        {
        // vkb keyboard must reset first, because iKeyInfoList will reset and destroy later
        
        if( iVkbCtrl != NULL )
            {
            iVkbCtrl->Reset();
            }
        
        for ( TInt i = 0; i < count; i++ )
            {
            iVkbLayoutInfoList[i]->ConstructFromResourceL( reader, iVkbCtrl, iVkbLayoutId );
            iVkbLayoutInfoList[i]->ResetSize( 
                iVkbLayoutInfoList[i]->UnitWidth(),
                iVkbLayoutInfoList[i]->UnitHeight(),
                iVkbLayoutInfoList[i]->UnitCharWidth(),
                iVkbLayoutInfoList[i]->UnitCharHeight(),
                iVkbLayoutInfoList[i]->CharLeftTopPoint(),
                iVkbLayoutInfoList[i]->CharFont() );
            
            if (iVkbCtrl)
            	{                
                iVkbLayoutInfoList[i]->iIrregular = iVkbCtrl->IsIrregularKeyBoard();
            	}
            } 
        }    
              
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 ); 

    iFirstTimeConstruct = EFalse;
	}

// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::VkbLayoutInfoList
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C RPointerArray<CPeninputVkbLayoutInfo> CAknFepCtrlVkbLayout::VkbLayoutInfoList()
    {
    return iVkbLayoutInfoList;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlVkbLayout::ChangeVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlVkbLayout::ChangeVkbLayout( TInt aVkbLayoutId )
    {
    if( iVkbLayoutId == aVkbLayoutId )
        {
        return;
        }
	iVkbLayoutId = aVkbLayoutId;
	for( TInt i = 0; i < iVkbLayoutInfoList.Count(); i++ )
	    {
	    if( aVkbLayoutId == iVkbLayoutInfoList[i]->LayoutID() )
	        {
	        iVkbCtrl->ReorganizeVkbKeys( iVkbLayoutInfoList[i] );
	        
	        return;
	        }
	    }
    }

// ------------------------------------------------------------------------
// CPeninputVkbKeyInfo::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbKeyInfo* CPeninputVkbKeyInfo::NewL( 
    TResourceReader& aReader )
    {
    CPeninputVkbKeyInfo* self = new ( ELeave ) CPeninputVkbKeyInfo();

    CleanupStack::PushL( self );
    self->ConstructL( aReader );
    CleanupStack::Pop( self );
    
    return self;
    }

// ------------------------------------------------------------------------
// CPeninputVkbKeyInfo::~CPeninputVkbKeyInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbKeyInfo::~CPeninputVkbKeyInfo()
    {
    delete iKey;
    iKey = NULL;
    delete iUnicode;
    iUnicode = NULL;
    }

// ------------------------------------------------------------------------
// CPeninputVkbKeyInfo::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputVkbKeyInfo::ConstructL( TResourceReader& aReader )
    {
    ConstructFromResourceL( aReader );
    }

// ------------------------------------------------------------------------
// CPeninputVkbKeyInfo::ConstructFromResourceL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputVkbKeyInfo::ConstructFromResourceL( TResourceReader& aReader )
    {
    iUnicode = aReader.ReadHBufCL();
    iScanCode = aReader.ReadInt16();
    iBeginRow = aReader.ReadInt16();
    iBeginColumn = aReader.ReadInt16();
    iEndRow = aReader.ReadInt16();
    iEndColumn = aReader.ReadInt16();
    iHighlightBmpIndex = aReader.ReadInt16();
    iHighlightMaskBmpIndex = aReader.ReadInt16();
    TPtrC displayUnicode = aReader.ReadTPtrC();

    TInt baseLine = 0;
    iKey = CVirtualKey::NewL( *iUnicode, iScanCode, TRect(), 
                              TRect(), baseLine, iHighlightBmpIndex, iHighlightMaskBmpIndex );
 	if( displayUnicode.Length() != 0 )
 		{
 		iKey->SetDisplayUnicodeL( displayUnicode );
 		}
    }

TVirtualKeyIrregularKeyType CPeninputVkbKeyInfo::KeyTypeFromColumn(TInt aColumns)
    {
    if (iBeginColumn == 0)
        {
        return EKeyLeftDown;
        }
        
    if ((iBeginColumn == aColumns - 1) && (aColumns % 2  == 0))
        {
        return EKeyRightUp;
        }
        
    if ((iBeginColumn == aColumns - 1) && (aColumns % 2  == 1))
        {
        return EKeyRightDown;
        }
        
    if (iBeginColumn % 2  == 0)
        {
        return EKeyMiddleDown;
        }
 
    return EKeyMiddleUp;
    }
    
void CPeninputVkbKeyInfo::SetIrregularKeyType(TInt aColumns)
    {
    if (iKey)
        {
        iKey->SetIrregualrKeyType(KeyTypeFromColumn(aColumns));
        }
    }
// ------------------------------------------------------------------------
// CPeninputVkbKeyInfo::CreateKeyL
// (other items were commented in a header)
// ------------------------------------------------------------------------
// 
void CPeninputVkbKeyInfo::CreateKeyL()
    {
    TInt baseLine = 0;
    iKey = CVirtualKey::NewL( *iUnicode, iScanCode, TRect(), 
                              TRect(), baseLine, iHighlightBmpIndex, iHighlightMaskBmpIndex );
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbLayoutInfo* CPeninputVkbLayoutInfo::NewL(
    TResourceReader& aReader )
    {
    CPeninputVkbLayoutInfo* self = NewLC( aReader );
    CleanupStack::Pop( self );
    
    return self;
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::NewLC
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbLayoutInfo* CPeninputVkbLayoutInfo::NewLC( 
    TResourceReader& aReader )
    {
    CPeninputVkbLayoutInfo* self = new ( ELeave ) CPeninputVkbLayoutInfo();
    CleanupStack::PushL( self );
    self->ConstructL( aReader ); 

    return self;
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::GetRealSize
// (other items were commented in a header)
// ------------------------------------------------------------------------
//  
EXPORT_C const TSize CPeninputVkbLayoutInfo::GetRealSize( 
    const TInt aUnitWidth, const TInt aUnitHeight ) const
    {
    return TSize( aUnitWidth * iColumns, aUnitHeight * iRows );
    }

EXPORT_C void CPeninputVkbLayoutInfo::ResetLayoutInfo(const TSize& aSize)
    {
    AknIconUtils::SetSize( iBgBmp, aSize, EAspectRatioNotPreserved );
    AknIconUtils::SetSize( iBgMaskBmp, aSize, EAspectRatioNotPreserved );
    
    }

EXPORT_C void CPeninputVkbLayoutInfo::SetKeyInfo(const TInt aRow,
                                                 const TInt aColumn,
                                                 const TRect& aKeyRect,
                                                 const TRect& aCharRect, 
                                                 const CFont* aFont)
    {
    RPointerArray<CPeninputVkbKeyInfo> keyInfoList = KeyInfoList();
    TInt count = keyInfoList.Count();
   
    for (TInt i = 0; i < count; i++ )
        {
        if (keyInfoList[i] &&
            keyInfoList[i]->BeginColumn() == aColumn && 
            keyInfoList[i]->BeginRow() == aRow)
            {
            keyInfoList[i]->Key()->SetRect( aKeyRect );
        
            return; 
            }
        }
    }


// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::ResetSize
// (other items were commented in a header)
// ------------------------------------------------------------------------
//    
EXPORT_C void CPeninputVkbLayoutInfo::ResetSize( 
    const TInt aUnitWidth, 
    const TInt aUnitHeight, 
    const TInt aUnitCharWidth, 
    const TInt aUnitCharHeight, 
    const TPoint& aCharLeftTopPoint,
    const CFont* aFont )
    {
    iUnitWidth = aUnitWidth;
    iUnitHeight = aUnitHeight;
    iUnitCharWidth = aUnitCharWidth;
    iUnitCharHeight = aUnitCharHeight;
    iCharLeftTopPoint = aCharLeftTopPoint;
    iFont = aFont;
    
    iLastTopLeft = TPoint( 0, 0 );

    // 1.Resize bk bmp and mask bmp
    AknIconUtils::SetSize( iBgBmp, 
        TSize( aUnitWidth * iColumns, aUnitHeight * iRows ), 
        EAspectRatioNotPreserved );
    AknIconUtils::SetSize( iBgMaskBmp, 
        TSize( aUnitWidth * iColumns, aUnitHeight * iRows ), 
        EAspectRatioNotPreserved );

    // 2.Resize dim bmp and mask bmp
    AknIconUtils::SetSize( iDimBmp, TSize( aUnitWidth, aUnitHeight ), 
        EAspectRatioNotPreserved );               
    AknIconUtils::SetSize( iDimMaskBmp, TSize( aUnitWidth, aUnitHeight ), 
        EAspectRatioNotPreserved );                    

    // 3.Resize all highlight bmp and mask bmp
    TInt count = iHighlightBmps.Count();
    TInt i = 0;

    for ( i = 0; i < count; i++ )
        {
        AknIconUtils::SetSize( iHighlightBmps[i], 
            TSize( aUnitWidth, aUnitHeight ), EAspectRatioNotPreserved );             
        }
     
    count = iHighlightMaskBmps.Count();
    for ( i = 0; i < count; i++ )
        {
        AknIconUtils::SetSize( iHighlightMaskBmps[i], 
            TSize( aUnitWidth, aUnitHeight ), EAspectRatioNotPreserved );                 
        }
    
    // 4.For every key, reset the key rect and char rect
    RPointerArray<CPeninputVkbKeyInfo> keyInfoList = KeyInfoList();
    count = keyInfoList.Count();
    TPoint keyLeftTop;
    TInt rows;
    TInt columns;
    TRect rect;
    
    for ( i = 0; i < count; i++ )
        {
        if ( keyInfoList[i]->Key() )
            {
            if (keyInfoList[i]->BeginColumn() == 0)
                {
                keyLeftTop = TPoint( keyInfoList[i]->BeginColumn() * aUnitWidth,
                                     keyInfoList[i]->BeginRow() * aUnitHeight );
                }
            else
                {
                keyLeftTop = TPoint( keyInfoList[i]->BeginColumn() * aUnitWidth - 
                                     6 * keyInfoList[i]->BeginColumn(),
                                     keyInfoList[i]->BeginRow() * aUnitHeight );
                }
                
            if (!iIrregular)
                {
                keyLeftTop = TPoint( keyInfoList[i]->BeginColumn() * aUnitWidth,
                                     keyInfoList[i]->BeginRow() * aUnitHeight );
                }
                            

            rows = keyInfoList[i]->EndRow() - keyInfoList[i]->BeginRow();
            columns = keyInfoList[i]->EndColumn() 
                - keyInfoList[i]->BeginColumn();
            
            //Set key rect
            rect = TRect( keyLeftTop, TSize( columns * aUnitWidth, 
                                             rows * aUnitHeight ) );
            keyInfoList[i]->Key()->SetRect( rect );
            }
        }
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::~CPeninputVkbLayoutInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbLayoutInfo::~CPeninputVkbLayoutInfo()
    {
    delete iBgBmp;
    iBgBmp = NULL;
    delete iBgMaskBmp;
    iBgMaskBmp = NULL;
    delete iDimBmp;
    iDimBmp = NULL;
    delete iDimMaskBmp;
    iDimMaskBmp = NULL;

    iHighlightBmps.ResetAndDestroy();
    iHighlightBmps.Close();
    
    iHighlightMaskBmps.ResetAndDestroy();
    iHighlightMaskBmps.Close();
    
    iKeyInfoList.ResetAndDestroy();
    iKeyInfoList.Close();
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::FindKey
// (other items were commented in a header)
// ------------------------------------------------------------------------
// 
EXPORT_C CPeninputVkbKeyInfo* CPeninputVkbLayoutInfo::FindKey( 
    const TInt aUnicode ) const
    {
    const TInt count = iKeyInfoList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( (*iKeyInfoList[i]->Unicode())[0] == aUnicode )
            {
            return iKeyInfoList[i];
            }
        }

    return NULL;
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::MoveKeys
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C void CPeninputVkbLayoutInfo::MoveKeys( const TPoint& aTopLeft )
    {
    RPointerArray<CPeninputVkbKeyInfo> keyInfoList = KeyInfoList();
    const TInt count = keyInfoList.Count();
    const TPoint difference = aTopLeft - iLastTopLeft;
    TRect rect;

    for ( TInt i = 0; i < count; i++ )
        {
        if ( keyInfoList[i]->Key() )
            {
            //Set key rect
            rect = keyInfoList[i]->Key()->Rect();
            rect.SetRect( rect.iTl + difference, rect.Size() );
            keyInfoList[i]->Key()->SetRect( rect );
            }
        }

    iLastTopLeft = aTopLeft;
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::CPeninputVkbLayoutInfo
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
CPeninputVkbLayoutInfo::CPeninputVkbLayoutInfo() 
    : iCharLeftTopPoint( TPoint(0,0) )
    {
    iLastTopLeft = TPoint( 0, 0 );
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputVkbLayoutInfo::ConstructL( TResourceReader& aReader )
    {
    
    ConstructFromResourceL( aReader );
    }

// ------------------------------------------------------------------------
// CPeninputVkbLayoutInfo::ConstructFromResourceL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
void CPeninputVkbLayoutInfo::ConstructFromResourceL( TResourceReader& aReader, 
                                                     CPeninputVkbCtrlExt* aVkbCtrl, 
                                                     TInt aVkbLayoutId )
    {
    delete iBgBmp;
    iBgBmp = NULL;
    delete iBgMaskBmp;
    iBgMaskBmp = NULL;
    delete iDimBmp;
    iDimBmp = NULL;
    delete iDimMaskBmp;
    iDimMaskBmp = NULL;

    iHighlightBmps.ResetAndDestroy();
    iHighlightBmps.Close();
    
    iHighlightMaskBmps.ResetAndDestroy();
    iHighlightMaskBmps.Close();
    
    iKeyInfoList.ResetAndDestroy();
    iKeyInfoList.Close();
    
    iLayoutID = aReader.ReadInt16();

    // Read the file name of the bmps
    TPtrC bmpFileName = aReader.ReadTPtrC();
    TInt32 imgMajorSkinId = aReader.ReadInt32();
    
    TInt layoutBmpId = aReader.ReadInt16();
    TInt layoutBmpMaskId = aReader.ReadInt16();
    TInt layoutBmpSkinId = aReader.ReadInt16();

    TAknsItemID id;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    
    id.Set(TInt( imgMajorSkinId ), layoutBmpSkinId);
    
    if ( layoutBmpId != KInvalidBmp)
    	{
        if ( layoutBmpMaskId != KInvalidBmp)
            {           
            AknsUtils::CreateIconL( skininstance,
                                    id,
                                    iBgBmp,
                                    iBgMaskBmp,
                                    bmpFileName,
                                    layoutBmpId,
                                    layoutBmpMaskId );
            }
        else
            {
            AknsUtils::CreateIconL( skininstance,
                                    id,
                                    iBgBmp,
                                    bmpFileName,
                                    layoutBmpId );
            }
    	}
    	
    TInt dimBmpId = aReader.ReadInt16();
    TInt dimMaskBmpId = aReader.ReadInt16();
    TInt dimBmpSkinId = aReader.ReadInt16();
    
    id.Set( TInt( imgMajorSkinId ), dimBmpSkinId );
    
    if ( dimBmpId != KInvalidBmp )
        {
        if ( dimMaskBmpId != KInvalidBmp )
            {
            AknsUtils::CreateIconL( skininstance,
                                    id,
                                    iDimBmp,
                                    iDimMaskBmp,
                                    bmpFileName,
                                    dimBmpId,
                                    dimMaskBmpId );
            }
        else
            {
            AknsUtils::CreateIconL( skininstance,
                                    id,
                                    iDimBmp,
                                    bmpFileName,
                                    dimBmpId );
            }
        }

    iColumns = aReader.ReadInt16();
    iRows = aReader.ReadInt16();

    TInt i;
    TInt count = aReader.ReadInt16();
    TInt bmpId;
    
    for ( i = 0; i < count; i++ )
        {
        bmpId = aReader.ReadInt16();
        if ( bmpId != KInvalidBmp )
            {
            CFbsBitmap* bmp = AknIconUtils::CreateIconL( bmpFileName, 
                                                         bmpId );
            CleanupStack::PushL( bmp );
            iHighlightBmps.Append( bmp );
            CleanupStack::Pop( bmp );
            }
        }

    count = aReader.ReadInt16();
    for ( i = 0; i < count; i++ )
        {
        bmpId = aReader.ReadInt16();
        if ( bmpId != KInvalidBmp )
            {
            CFbsBitmap* bmp = AknIconUtils::CreateIconL( bmpFileName, bmpId ); 
            CleanupStack::PushL( bmp );                    
            iHighlightMaskBmps.Append( bmp );
            CleanupStack::Pop( bmp );            
            }
        }

    count = aReader.ReadInt16();
    
    for ( i = 0; i < count; i++ )
        {
        CPeninputVkbKeyInfo* keyInfo = CPeninputVkbKeyInfo::NewL( aReader );
            keyInfo->SetIrregularKeyType(iColumns);
        iKeyInfoList.Append( keyInfo );
        }   

    if ( ( iLayoutID == aVkbLayoutId ) && ( aVkbCtrl ) )
        {   
        aVkbCtrl->ReorganizeVkbKeys( this );
        }
  }
//  End Of File
