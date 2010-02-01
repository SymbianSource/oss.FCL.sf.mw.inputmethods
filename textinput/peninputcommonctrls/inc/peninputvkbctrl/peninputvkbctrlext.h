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
* Description:  vkb extenting control 
*
*/


#ifndef C_CPENINPUTVKBCTRLEXT_H
#define C_CPENINPUTVKBCTRLEXT_H

// System includes
#include <peninputlayoutvkb.h>

// Forward decalaration
class CPeninputVkbLayoutInfo;

/**
 *  vkb extenting control class
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputVkbCtrlExt : public CVirtualKeyboard
    {
   
public:

    /**
     * Create one CPeninputVkbCtrlExt object
     *
     * @since S60 v3.2
     * @param aRect The rectangle of the control
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aFont The pointer to CFont object
     * @return The pointer to CPeninputVkbCtrlExt object
     */                                                                      
    IMPORT_C static CPeninputVkbCtrlExt* NewL( const TRect& aRect, 
                                               CFepUiLayout* aUiLayout,
                                               TInt aControlId,
                                               const CFont* aFont,
                                               TBool aIrregular = EFalse);
                                   
    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */                                    
    IMPORT_C virtual ~CPeninputVkbCtrlExt();
    
    /**
     * Handle size changing of vkb extenting control
     *
     * @since S60 v3.2
     * @param aUnitWidth Width of unit of vkb extenting control
     * @param aUnitHeight Height of unit of vkb extenting control
     * @param aLeftTopPoint The left top point of vkb extenting control
     * @return None
     */     
    IMPORT_C void SizeChanged( const TInt aUnitWidth,
                               const TInt aUnitHeight,
                               const TPoint& aLeftTopPoint ); 
                               
    /**
     * Handle size changing of vkb extenting control
     *
     * @since S60 v3.2
     * @param aUnitWidth Width of unit of vkb extenting control
     * @param aUnitHeight Height of unit of vkb extenting control
     * @param aLeftTopPoint The left top point of vkb extenting control
     * @return None
     */     
    IMPORT_C void SizeChanged( const TRect& aRect );                                

    /**
     * Reorganize Vkb Keys
     *
     * @since S60 v3.2
     * @param aVkbLayout The vkb layout info
     * @return None
     */     
    IMPORT_C void ReorganizeVkbKeys( CPeninputVkbLayoutInfo* aVkbLayoutInfo );
    
    /**
     * Move the vkb extenting control
     *
     * @since S60 v3.2
     * @param aOffset The offset to be moved
     * @return None
     */     
    IMPORT_C void Move( const TPoint& aOffset );
    
    /**
     * Reset some value of the control
     *
     * @since S60 v3.2
     * @return None
     */    
    IMPORT_C void Reset();
    
    /**
     * Dim keys in vkb extenting control
     *
     * @since S60 v3.2
     * @param aPermitted The permitted unicodes
     * @return None
     */     
    IMPORT_C void DimKeys( const HBufC* aPermitted );

    /**
     * Cancel dim of all keys
     *
     * @since S60 v3.2
     * @return None
     */  
    IMPORT_C void CancelDims();

    /**
     * Dim keys in vkb extenting control
     *
     * @since S60 v3.2
     * @param aSet The unicodes set
     * @param aDim The flag of dim or not
     * @return None
     */   
    IMPORT_C void DimKeySet( const HBufC* aSet, TBool aDim );  
    
    /**
     * Read resource and construct
     *
     * @since S60 v5.0
     * @return None
     */  
     IMPORT_C void ConstructFromResourceL();               
protected:

    /* 
     * Constructor
     *
     * @since S60 v3.2
     * @param aRect The rectangle of the control
     * @param aUiLayout The layout plugin
     * @param aControlId The control ID
     * @param aFontSpec The specification of font
     * @return None
     */ 
    CPeninputVkbCtrlExt( const TRect& aRect, 
                                  CFepUiLayout* aUiLayout,
                                  TInt aControlId,
                                  const TFontSpec& aFontSpec,
                                  TBool aIrregular = EFalse);

protected: // Data
    /**
     * The vkb layout info
     * Not own
     */
    CPeninputVkbLayoutInfo* iVkbLayoutInfo;

private: // Data

    
    /**
     * The unit width
     */
    TInt iUnitWidth;

    /**
     * The unit height
     */
    TInt iUnitHeight;

    /**
     * The left top point
     */
    TPoint iLeftTopPoint;
    };  

#endif // C_CPENINPUTVKBCTRLEXT_H
