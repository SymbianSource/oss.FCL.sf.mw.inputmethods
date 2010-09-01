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
* Description:  header file of button and dragbar
*
*/

#ifndef C_CAKNFEPCTRLLABEL_H
#define C_CAKNFEPCTRLLABEL_H

#include <peninputlayoutbasecontrol.h>
/**
 *  CAknFepCtrlLabel
 *
 *  Label control. This class could be used directly by user.
 *
 *  @lib peninputlayoutcontrol.lib
 *  @since S60 V5.0
 */
class CAknFepCtrlLabel: public CFepUiBaseCtrl
    {
    
public: 
    //constructor and destructor
    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    IMPORT_C virtual ~CAknFepCtrlLabel();

    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */                               
    IMPORT_C static CAknFepCtrlLabel* NewL(CFepUiLayout* aUiLayout,TInt aControlId);

    /**
     * Set bitmap for given status .        
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to button
     * @param aType The bitmap type used for
     */                                 
    IMPORT_C void SetBitmapL(CFbsBitmap* aBmp);

    /**
     * Set mask bitmap for given status.        
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to button
     * @param aType The bitmap type used for
     */                                 
    IMPORT_C void SetMaskBitmapL(CFbsBitmap* aBmp);

    //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                               
    IMPORT_C virtual void Draw();    

    /* Set the font for caption
     *
     * @since S60 V4.0
     * @param aFontSpec the font spec to be used.
     */                                                   
    IMPORT_C virtual void SetFont(const TFontSpec& aFontSpec);
    
    /**
     * Set font information
     *
     * @since S60 V4.0          
     * @param aFont The font info for key drawing.Ownership not transferred
     */
    IMPORT_C virtual void SetFont(const CFont* aFont);    

    /**
     * Set font text color
     *
     * @since S60 V4.0
     * @param aColor Color for font text
     */
     IMPORT_C virtual void SetFontColor(const TRgb aColor);
     
     
    /**
     * Set text to label
     *
     * @since S60 v3.2
     * @param aText Display text in lable
     * @return None
     */
    IMPORT_C void SetText(const TDesC& aText); 
    
    /**
     * Label size changed
     *
     * @since S60 v3.2
     * @param aNewRect New rect for label
     * @param aIsReloadImages resize image     
     * @return None
     */
    IMPORT_C virtual void SizeChanged(TRect aNewRect, TBool aIsReloadImages);
    
    /**
     * Set aligment
     *
     * @since S60 v3.2
     * @param aAlignment Text alignment
     * @return None
     */
    IMPORT_C virtual void SetTextAlignment(CGraphicsContext::TTextAlign aAlignment);
        
    /**
     * Get aligment
     *
     * @since S60 v3.2
     * @return Text alignment
     */
    IMPORT_C virtual CGraphicsContext::TTextAlign TextAlignment();
    
protected:   

    /**
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     */           
    CAknFepCtrlLabel(CFepUiLayout* aUiLayout,TInt aControlId);

    /**
     * Bitmaps size changed
     *
     * @since S60 v3.2
     * @param aSize New size for bitmaps
     * @return None
     */
    void ResizeBitmaps(TSize aSize);            

private:    

    /**
     * bitmap for dimmed state
     * Own.
     */
    CFbsBitmap* iBmp;   
         
    /**
     * mask bitmap for dim stute
     * Own.
     */
    CFbsBitmap* iMaskBmp;
    
    /**
     * lable caption
     */
    HBufC* iText;
    
    /**
     * Font object for key drawing
     * Ownership depends on flag iFontOwnership.
     */          
    CFont* iFont;

    /**
     * Font information for key drawing
     */     
    TFontSpec iFontSpec;

    /**
     *  Flag ownership flag. ETrue if CFont object is owned by this control.
     */
    TBool iFontOwnership;
        
    /**
     * Baseline for caption
     */
    TInt iBaseline;
    
    /**
     * Pen color for first level caption
     */     
    TRgb iFontColor;

    /**
     * Text alignment
     */     
    CGraphicsContext::TTextAlign iAlignment;

    };

#endif // C_CAKNFEPCTRLLABEL_H