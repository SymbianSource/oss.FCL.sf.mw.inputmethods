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
* Description:  Interface of normal button and multidisplay button
*
*/


#ifndef C_CAKNFEPCTRLMULTIIMAGEBUTTON_H
#define C_CAKNFEPCTRLMULTIIMAGEBUTTON_H

// system includes
#include <AknsConstants.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>
#include <peninputcommonbutton.h>

// forward declarations
class TResourceReader;


// class declarations
/**
 *  Basic functionality for graphic buttons
 *  It provides creation from resource and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlMultiImageButton : public CAknFepCtrlCommonButton
    {
public: 
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlMultiImageButton type object
     */
    IMPORT_C static CAknFepCtrlMultiImageButton* NewL(CFepUiLayout* aUiLayout, 
    						TInt aControlId,
    						TAknsItemID aNormalID,
    						TAknsItemID aPressedID,
    						TAknsItemID aInactiveID);
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlMultiImageButton type object
     */
    IMPORT_C static CAknFepCtrlMultiImageButton* NewLC(CFepUiLayout* aUiLayout, 
    						TInt aControlId,
							TAknsItemID aNormalID,
    						TAknsItemID aPressedID,
    						TAknsItemID aInactiveID);
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    IMPORT_C virtual ~CAknFepCtrlMultiImageButton();

    /**
     * This function creates the input range button from resource.
     *
     * @since S60 v3.2
     * @param aReader A Reader which is created against the AKN_FEP_BUTTON resource type
     * @return None
     */
    IMPORT_C void ConstructFromResourceL(TResourceReader& aReader);
    
    /**
     * This function creates the input range button from resource,
     * before calling this function, caller should set resource id of 
     * range button first
     * 
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ConstructFromResourceL();

    /**
     * This function handles the SizeChanged event, it resizes all images and rects
     *
     * @since S60 v3.2
     * @param aRect New button extent
     * @param aInnerRect New button inner
     * @param aIsReloadImages Whether resize images as well, 
     * if only the position of the button changed, no need to reset the images as well
     * @return Nothing
     */
    IMPORT_C void SizeChanged(TRect aRect, TRect aInnerRect, TBool aIsReloadImages = EFalse);

    /**
     * Draws UI
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Draw();
                   
	/**
     * Set text line layout
     *
     * @since S60 V4.0
     * @param aTextFormat Text line layout to be set
 	 * @return None
     */                  	
    inline void SetTextFormat(TAknTextLineLayout aTextFormat);
    
	/**
     * Set text to be shown on button
     *
     * @since S60 V4.0
     * @param aText Text to be shown on button
 	 * @return None
     */
    inline void SetTextL(const TDesC& aText);

    inline void SetImageFrameId( TAknsItemID aNormalID,
    							 TAknsItemID aPressedID,
    							 TAknsItemID aInactiveID );
    							 
	IMPORT_C void Move(const TPoint& aOffset);    
								 
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     */
    IMPORT_C CAknFepCtrlMultiImageButton(CFepUiLayout* aUiLayout, 
    								 TInt aControlId,
    								 TAknsItemID aNormalID,
    								 TAknsItemID aPressedID,
    								 TAknsItemID aInactiveID);
			
    
    void ResizeBitmaps(TSize aInnerSize);
    
    void DeleteForgroundRes();
    
    void CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                            const CFbsBitmap* aMask
                        	/*TScaleMode aScaleMode*/ );
                        	
private:
    /**
     * Froground bitmap and it's mask
     */
    CFbsBitmap* iForground1Bmp;
    CFbsBitmap* iForground1BmpMask;
    
    /**
     * Froground Dimmed bitmap's mask
     */
    CFbsBitmap* iForground1DimBmpMask;
    
    /**
     * Froground bitmap's position
     */
    TRect iForground1BmpRect;
    
    /**
     * Froground bitmap and it's mask
     */
    CFbsBitmap* iForground2Bmp;
    CFbsBitmap* iForground2BmpMask;
    
    /**
     * Froground Dimmed bitmap's mask
     */
    CFbsBitmap* iForground2DimBmpMask;
    
    /**
     * Froground bitmap's position
     */
    TRect iForground2BmpRect;    
    
    /**
     * The img ID of button in various states
     */
    TAknsItemID iNormalImgID;
    TAknsItemID iPressedImgID;
    TAknsItemID iInactiveImgID;

    HBufC* iText;
    TAknTextLineLayout iTextFormat;
    };

inline void CAknFepCtrlMultiImageButton::SetTextFormat(TAknTextLineLayout aTextFormat)
    {
    iTextFormat = aTextFormat;
    }

inline void CAknFepCtrlMultiImageButton::SetTextL(const TDesC& aText)
    {
    delete iText;
    iText = NULL;
    iText = aText.AllocL();
    }

inline void CAknFepCtrlMultiImageButton::SetImageFrameId( TAknsItemID aNormalID,
                                                      TAknsItemID aPressedID,
                                                      TAknsItemID aInactiveID )
    {
    iNormalImgID = aNormalID;
    iPressedImgID = aPressedID;
    iInactiveImgID = aInactiveID;
    }
#endif // C_CAKNFEPCTRLMULTIIMAGEBUTTON_H
            
// End Of File
