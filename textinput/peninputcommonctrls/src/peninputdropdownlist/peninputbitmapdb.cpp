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
* Description:  bitmap db manager,which is used by drop down list
*
*/


#include <e32base.h>
#include <w32std.h>
#include <fbs.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayout.h>
#include <aknenv.h>
#include "peninputbitmapdb.h"

const TUint32 KDefaultTextColor = 0x000000;
const TUint32 KDefaultFrameColor = 0x000000;

const TInt KInvalidBmp = -1;

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

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CBitmapDb 

// -----------------------------------------------------------------------------
// CBitmapDb::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CBitmapDb* CBitmapDb::NewL(TResourceReader& aReader, 
                           TSize aBitmapSize,
                           TSize aExpandSize,
                           TSize aSpinBtnSize)
    {
    CBitmapDb* self = new (ELeave) CBitmapDb();
    CleanupStack::PushL(self);
    self->ConstructL(aReader, 
                     aBitmapSize,
                     aExpandSize,
                     aSpinBtnSize);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CBitmapDb::~CBitmapDb
// destructor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
CBitmapDb::~CBitmapDb() 
    {
    // Delete all bitmap
    iBmpList.ResetAndDestroy();
    iBmpList.Close();    
    }

// -----------------------------------------------------------------------------
// CBitmapDb::GetBitMap
// get bitmap according to the assigned type
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
const CFbsBitmap* CBitmapDb::GetBitMap(const TBitmapType aType)
    {
    // Return corresponding bitmap according aType 
    return iBmpList[aType];
    }
        
// -----------------------------------------------------------------------------
// CBitmapDb::CreateIconL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CBitmapDb::CreateIconL(const TDesC& aBmpFileName,
                            TAknsItemID aId,
                            CFbsBitmap** aBmp,
                            CFbsBitmap** aMaskBmp,
                            TInt aBmpId,
                            TInt aMaskBmpId,
                            TSize aSize,
                            TBool aColorIcon)
    {
    if (aBmpId != KInvalidBmp)
        {
        if (aMaskBmpId != KInvalidBmp)
            {
            if (aColorIcon)
                {
                AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(),
                             aId,
                             KAknsIIDQsnIconColors,
                             EAknsCIQsnIconColorsCG30,
                             *aBmp,
                             *aMaskBmp,
                             aBmpFileName,
                             aBmpId,
                             aMaskBmpId,
                             AKN_LAF_COLOR( 0 ) ); 
                }
            else
                {
                AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
                                       aId,
                                       *aBmp,
                                       *aMaskBmp,
                                       aBmpFileName,
                                       aBmpId,
                                       aMaskBmpId);
                }

            AknIconUtils::SetSize(*aMaskBmp, aSize, EAspectRatioNotPreserved);
            }
        else
            {
            AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
                                   aId,
                                   *aBmp,
                                   aBmpFileName,
                                   aBmpId);
            }
        
        AknIconUtils::SetSize(*aBmp, aSize, EAspectRatioNotPreserved);    
        }
    }

void CBitmapDb::ConstructFromResourceL(TResourceReader& aReader, 
                                       TSize aBitmapSize,
                                       TSize aExpandSize,
                                       TSize aSpinBtnSize)
    {
    for (TInt i = 0; i < EBitmapLastType + 1; ++i)
        {
        delete iBmpList[i];
        iBmpList[i] = NULL;
        }

    // Read mbm file name from the aReader
    TPtrC bmpFile = aReader.ReadTPtrC();
    TInt imgMajorSkinId = aReader.ReadInt32();

    TAknsItemID id;

    for (TInt i = 0; i < EBitmapLastType + 1; i+=2)
        {
        TInt bmpId = aReader.ReadInt16();
        TInt maskId = aReader.ReadInt16();
        TInt minorSkinId = aReader.ReadInt16();
        
        TSize bmpsize = aBitmapSize;
        
        if ((i >= EBitmapNextDisable && i < EBitmapNextPressMask) ||
            (i >= EBitmapPrevious && i < EBitmapPreviousPressMask))
            {
            bmpsize = aSpinBtnSize;
            }
        else if (i >= EBitmapExpand && i < EBitmapExpandPressMask)
            {
            bmpsize = aExpandSize;
            }
            
        id.Set(imgMajorSkinId, minorSkinId);
        
        TBool colorIcon;
        if ( i == EBitmapNext || i == EBitmapNextMask ||
             i == EBitmapClose || i == EBitmapCloseMask ||
             i == EBitmapPrevious || i == EBitmapPreviousMask ||
             i == EBitmapExpand || i == EBitmapExpandMask)
            {
            colorIcon = ETrue;
            }
        else
            {
            colorIcon = EFalse;
            }
        
        CreateIconL(bmpFile,
                    id,
                    &(iBmpList[i]),
                    &(iBmpList[i+1]),
                    bmpId,
                    maskId,
                    bmpsize,
                    colorIcon);
        
        if( i == EBitmapNextDisable )
        	{
        	CFbsBitmap* newMask = NULL;
        	CreateDimmedMaskL( newMask, iBmpList[i+1]);
        	delete iBmpList[i+1];
        	iBmpList[i+1] = newMask;
        	}
        }
    
    TInt colorMajorSkinId = aReader.ReadInt32();
    TInt skinitemid = aReader.ReadInt16();
    TInt textcoloridx = aReader.ReadInt16();
    
    id.Set(colorMajorSkinId, skinitemid);
    TInt error = AknsUtils::GetCachedColor(AknsUtils::SkinInstance(),
                                           iTextColor,
                                           id,
                                           textcoloridx);

    if (error != KErrNone)
        {
    	iTextColor = TRgb(KDefaultTextColor);
        }
        
    TInt frameMajorSkinId = aReader.ReadInt32();
    skinitemid = aReader.ReadInt16();
    TInt framecoloridx = aReader.ReadInt16();
    
    id.Set(frameMajorSkinId, skinitemid);
    error = AknsUtils::GetCachedColor(AknsUtils::SkinInstance(),
                                      iFrameColor,
                                      id,
                                      framecoloridx);
    
    if (error != KErrNone)
        {
    	iFrameColor = TRgb(KDefaultFrameColor);
        }
    }

// -----------------------------------------------------------------------------
// CBitmapDb::ConstructL
// 2nd construct function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CBitmapDb::ConstructL(TResourceReader& aReader, 
                           TSize aBitmapSize,
                           TSize aExpandSize,
                           TSize aSpinBtnSize)
    {
    for (TInt i = 0; i < EBitmapLastType + 1; ++i)
        {
        iBmpList.AppendL(NULL);
        }

    ConstructFromResourceL(aReader, 
                           aBitmapSize,
                           aExpandSize,
                           aSpinBtnSize);
    }
    
// -----------------------------------------------------------------------------
// CBitmapDb::TextColor
// Get text color of drop down list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TRgb CBitmapDb::TextColor()
    {
    return iTextColor;
    }

// -----------------------------------------------------------------------------
// CBitmapDb::FrameColor
// Get frame color of drop down list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TRgb CBitmapDb::FrameColor()
    {
	return iFrameColor;
    }
// ---------------------------------------------------------------------------
// CBitmapDb::CreateDimmedMaskL
// This methods shall be called by the container's SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//	
void CBitmapDb::CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
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
	
void CBitmapDb::SizeChanged(TSize aSize)
	{
	for (TInt i = 0; i < iBmpList.Count(); i++)
		{
		AknIconUtils::SetSize(iBmpList[i], aSize, EAspectRatioNotPreserved);
		}
	// dimmed mask can't use AknIconUtils::SetSize change its size.
	if( iBmpList[EBitmapNextDisableMask] && iBmpList[EBitmapNextMask] )
		{
		CFbsBitmap* newMask = NULL;
		TRAP_IGNORE(CreateDimmedMaskL( newMask, iBmpList[EBitmapNextMask]));
		delete iBmpList[EBitmapNextDisableMask];
		iBmpList[EBitmapNextDisableMask] = newMask;		
		}
	}
// End Of File
