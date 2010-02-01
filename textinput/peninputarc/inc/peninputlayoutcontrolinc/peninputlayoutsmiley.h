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
* Description: header smiley classes 
*
*/

#ifndef C_PENINPUTLAYOUTSMILEY_H
#define C_PENINPUTLAYOUTSMILEY_H

// INCLUDES

#include <e32base.h>
#include <e32std.h>
#include <e32cmn.h> 
#include <AknIconObserver.h> 
#include <e32hashtab.h>

// FORWARD DECLARATIONS

//CLASS DECLARATIONS

/**
 *  class CPeninputSmileyImage 
 *
 *  @lib peninputlayoutcontrol.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CPeninputSmileyImage) : public CBase
    {
public: //constructors
    
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @param aSmileyId Unique id of smiley 
     * @param aBitmapId Resource id of image bitmap
     * @param aMaskId  Resource id of mask bitmap
     * @param aImageSize Size(in pixel) of smiley
     * @return Pointer to created object
     */    
    static CPeninputSmileyImage* NewL( TInt aSmileyId, TInt aBitmapId,
                                       TInt aMaskId, const TSize& aImageSize );
    
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @param aSmileyId Unique id of smiley
     * @param aBitmapId Resource id of image bitmap
     * @param aMaskId  Resource id of mask bitmap
     * @param aImageSize Size(in pixel) of smiley
     * @return Pointer to created object
     */      
    static CPeninputSmileyImage* NewLC( TInt aSmileyId, TInt aBitmapId,
                                        TInt aMaskId, const TSize& aImageSize );

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */    
    virtual ~CPeninputSmileyImage();

public:
    
    /**
     * Create image and mask of smiley
     *
     * @since S60 v5.0
     * @return None
     */     
    void CreateImageL();
    
    /**
     * Destroy image and mask of smiley
     *
     * @since S60 v5.0
     * @return None
     */     
    void DestroyImage();

    /**
     * Get image of smiley. 
     *
     * @since S60 v5.0
     * @return bitmap
     */         
    CFbsBitmap* ImageBitmap();

    /**
     * Get mask of smiley. 
     *
     * @since S60 v5.0
     * @return mask bitmap
     */ 
    CFbsBitmap* MaskBitmap();
   
    /**
     * Get smiley id. 
     *
     * @since S60 v5.0
     * @return Smiley id
     */     
    TInt SmileyId();
   
    /**
     * Get smiley size. 
     *
     * @since S60 v5.0
     * @return Size of smiley
     */     
    TSize SmileyImageSize();
    
    /**
     * Change size of smiley 
     *
     * @since S60 v5.0
     * @param aSize New size of smiley
     * @return none
     */     
    void SetImageSize( const TSize& aSize );

private: //constructors
    
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aSmileyId Unique id of smiley 
     * @param aBitmapId Resource id of image bitmap
     * @param aMaskId  Resource id of mask bitmap
     * @param aImageSize Size(in pixel) of smiley
     */    
    CPeninputSmileyImage( TInt aSmileyId, TInt aBitmapId, TInt aMaskId, 
                          const TSize& aImageSize );
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();
     
private: //data
    
    /**
     * Smiley id
     */
    TInt iSmileyId;
    
    /**
     * Resource id of bitmap
     */
    TInt iImageBitmapId;
    
    /**
     * Resource id of mask
     */
    TInt iMaskBitmapId;
           
    /**
     * Size of smiley, pixel
     */
    TSize iImageSize;

    /**
     * Image bitmap 
     * Own
     */
    CFbsBitmap* iImageBitmap;
    
    /**
     * Mask bitmap
     * Own
     */
    CFbsBitmap* iMaskBitmap;
           
    };

/**
 *  class CPeninputSmileyManager 
 *
 *  @lib peninputlayoutcontrol.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CPeninputSmileyManager) : public CBase
    {
public: //constructors
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object
     */    
    static CPeninputSmileyManager* NewL();

    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object
     */        
    static CPeninputSmileyManager* NewLC();

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */      
    virtual ~CPeninputSmileyManager();

public:
    /**
     * Get smiley object by id
     * 
     * @since S60 v5.0
     * @param aSmileyId smiley id 
     * @return smiley object Specified by aSmileyId, NULL if not found
     */
    CPeninputSmileyImage* SmileyImage( TInt aSmileyId );
    
    /**
     * Detech if Specified character is smiley
     * 
     * @since S60 v5.0
     * @param aSmileyId smiley id
     * @return ETrue if aTextCode is a smiley
     */
    TBool IsSmileyCode( TUint16 aTextCode );

    /**
     * Get smiley template
     * 
     * @since S60 v5.0 
     * @return smiley template, The size of smiley is determined by this value 
     */
    const TDesC& SmileySizeTemplate();
    
    /**
     * Get invisible character as place holder
     * 
     * @since S60 v5.0 
     * @return character code of place holder 
     */
    TUint16 SmileyPlaceHolderCharacter();
    
private:
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return none
     */ 
    CPeninputSmileyManager();
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */       
    void ConstructL();

private:
    /**
     * Read smiley info from resource
     *
     * @since S60 v5.0
     * @param aResourceId resource id of smiley info
     * @return None
     */       
    void ReadSmileyInfoL( TInt aResourceId );
    
    /**
     * Initalize all smily images
     *
     * @since S60 v5.0
     * @return None
     */       
    void PreLoadSmileyImageL();

private:
    /**
     * Available smileies
     */
    RPointerArray<CPeninputSmileyImage> iSmileyArray;
    
    /**
     * Hash table of smileies
     */
    RHashMap<TInt,CPeninputSmileyImage*> iSmileyHash;
    };

#endif /*C_PENINPUTLAYOUTSMILEY_H*/
