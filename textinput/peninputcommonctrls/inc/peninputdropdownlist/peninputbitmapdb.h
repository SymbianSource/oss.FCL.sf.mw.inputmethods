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


#ifndef C_BITMAPDB_H
#define C_BITMAPDB_H

#include <e32base.h>
#include <w32std.h>
#include <barsread.h>
#include <AknsUtils.h>

/**
 *  Bitmap db manager
 *
 *  Bitmap db manager used by drop down list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */    
NONSHARABLE_CLASS(CBitmapDb) : public CBase
    {

public:
    /**
     *  bitmap type
     *
     *  defines the types from bitmap database used by drop down list. 
     *
     *  @lib FepCtrlDropdownList.lib
     *  @since S60 v3.2
     */
    enum TBitmapType
        {
        EBitmapListBg,
        
        EBitmapListBgMask,
        
        // the primary background bitmap
        EBitmapPrimary,
        
        // the primary Mask background bitmap
        EBitmapPrimaryMask,

        // the secondary background bitmap
        EBitmapSecondary,

        // the secondary Mask background bitmap
        EBitmapSecondaryMask,

        // the highlight cell background bitmap
        EBitmapHighlight,        
        
        // the highlight cell Mask background bitmap
        EBitmapHighlightMask,        

        // the pen down cell background bitmap
        EBitmapPenDown,        
        
        // the pen down cell Mask background bitmap
        EBitmapPenDownMask,         
        
        // disable bitmap for page down button
        EBitmapNextDisable,
        
        // disable mask bitmap for page down button
        EBitmapNextDisableMask,

        // bitmap for page down button
        EBitmapNext,

        // mask bitmap for page down button
        EBitmapNextMask,

        // pressed bitmap for page down button
        EBitmapNextPress,

        // pressed mask bitmap for page down button
        EBitmapNextPressMask,

        // bitmap for page close button
        EBitmapClose,

        // mask bitmap for page close button
        EBitmapCloseMask,

        // pressed bitmap for page close button
        EBitmapClosePress,

        // pressed mask bitmap for page close button
        EBitmapClosePressMask,

        // bitmap for page up button
        EBitmapPrevious,

        // mask bitmap for page up button
        EBitmapPreviousMask,

        // pressed bitmap for page up button
        EBitmapPreviousPress,

        // pressed mask bitmap for page up button
        EBitmapPreviousPressMask,
        EBitmapExpand,
        
        EBitmapExpandMask,
        
        EBitmapExpandPress,
        
        EBitmapExpandPressMask,
        
        EBitmapLastType = EBitmapExpandPressMask
        };    

public:  // Method

    /**
     * Create a object of CBitmapDb, static method
     *
     * @since S60 v3.2
     * @param aReader resource reader which provide the bitmap(mask) id
     * @param aBitmapSize the size needed to set for the bitmap
     * @return CBitmapDb pointer.
     */
     static CBitmapDb* NewL(TResourceReader& aReader, 
                            TSize aBitmapSize,
                            TSize aExpandSize,
                            TSize aSpinBtnSize);

    /**
     * Destructor
     *
     * @since S60 v3.2
     */  
    virtual ~CBitmapDb();
    
    /**
     * Get bitmap from the database
     *
     * @since S60 v3.2
     * @param aType bitmap type
     * @return the destination bitmap, Null if can't find
     */    
    const CFbsBitmap* GetBitMap(const TBitmapType aType);
    
    void ConstructFromResourceL(TResourceReader& aReader, 
                                TSize aBitmapSize,
                                TSize aExpandSize,
                                TSize aSpinBtnSize);

    /**
     * Get text color of drop down list
     *
     * @since S60 v3.2
     * @return Text color of drop down list
     */    
    const TRgb TextColor();

    /**
     * Get frame color of drop down list
     *
     * @since S60 v3.2
     * @return Frame color of drop down list
     */    
    const TRgb FrameColor();
    
    static void CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                                        		 const CFbsBitmap* aMask
                                        		 /*TScaleMode aScaleMode*/ );
                                        		 
	void SizeChanged(TSize aSize);
private: // Data

    /**
     * 2nd construct method
     *
     * @since S60 v3.2
     * @param aReader resource reader which provide the bitmap(mask) id
     * @param aBitmapSize the size needed to set for the bitmap
     * @return None
     */    
    void ConstructL(TResourceReader& aReader, 
                    TSize aBitmapSize,
                    TSize aExpandSize,
                    TSize aSpinBtnSize);

    // txin
    void CreateIconL(const TDesC& aBmpFileName,
                     TAknsItemID aId,
                     CFbsBitmap** aBmp,
                     CFbsBitmap** aMaskBmp,
                     TInt aBmpId,
                     TInt aMaskBmpId,
                     TSize aSize,
                     TBool aColorIcon);
    // end
    
private: // Data

    /**
     * the bitmap array
     */    
    RPointerArray<CFbsBitmap> iBmpList;
    
    /**
     * the text color of drop down list
     */    
    TRgb iTextColor;
    
    /**
     * the frame color of drop down list
     */    
    TRgb iFrameColor;
    };

#endif // C_BITMAPDB_H

// End Of File
