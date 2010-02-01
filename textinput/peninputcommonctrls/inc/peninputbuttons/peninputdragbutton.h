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


#ifndef C_CAKNFEPCTRLDRAGBUTTON_H
#define C_CAKNFEPCTRLDRAGBUTTON_H

// system includes
#include <AknsConstants.h>
#include <AknLayout2Def.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>

// forward declarations
class TResourceReader;

/**
 *  Basic functionality for drag button
 *  It provides creation from resource and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlDragButton : public CDragBar
    {
public:  
    
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The Ui layout
     * @param aControlId The control id
     * @return Pointer to created CAknFepCtrlDragButton object
     */
    IMPORT_C static CAknFepCtrlDragButton* NewL(CFepUiLayout* aUiLayout, TInt aControlId);
    
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The Ui layout
     * @param aControlId The control id
     * @return Pointer to created CAknFepCtrlDragButton object
     */
    IMPORT_C static CAknFepCtrlDragButton* NewLC(CFepUiLayout* aUiLayout, TInt aControlId);
   
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CAknFepCtrlDragButton();
       
    /**
     * This function creates the button from resource.
     *
     * @since S60 v3.2
     * @param aReader A Reader which is created against the AKN_FEP_BUTTON resource type
     * @return None
     */
    IMPORT_C void ConstructFromResourceL(TResourceReader& aReader);
    
    /**
     * This function creates button from resource, before calling 
     * this function, caller should set resource id of button first
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
     * @param aIsReloadImages ETrue for resizing images; otherwise EFalse
     * @return None
     */
    IMPORT_C void SizeChanged(TRect aRect, TRect aInnerRect = TRect(), 
                              TBool aIsReloadImages = EFalse);

	/**
     * Set Forground bitmap position
     *
     * @since S60 v3.2
     * @param aRect Forground bitmap position
     * @return None
     */
	IMPORT_C void SetForgroundBmpRect(const TRect& aRect);

    IMPORT_C void SetForegroundBmpL(CFbsBitmap* aBmp,
	                                  CFbsBitmap* aMaskBmp);

    /**
     * Draws UI
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Draw();

	IMPORT_C void Move(const TPoint& aOffset);

    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);

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
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The Ui layout
     * @param aControlId The control id
     * @return None
     */
    IMPORT_C CAknFepCtrlDragButton(CFepUiLayout* aUiLayout, TInt aControlId);
    
    
    
    void ResizeBitmaps(TSize aSize);
    
    void DeleteForgroundRes();
    
    void CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                            const CFbsBitmap* aMask
                        	/*TScaleMode aScaleMode*/ );
    

private:
    /**
     * Array of background bmp (not own)
     */
    RPointerArray<CFbsBitmap> iBmpList;
    
    /**
     * Array of background mask bmp (not own)
     */
    RPointerArray<CFbsBitmap> iMaskBmpList;

    /**
     * The first time construct flag of button
     */
    TBool iFirstTimeConstruct;
    
    /**
     * Froground bitmap and it's mask
     */
    CFbsBitmap* iForgroundBmp;
    CFbsBitmap* iForgroundBmpMask;
    
    /**
     * Froground Dimmed bitmap's mask
     */
    CFbsBitmap* iForgroundDimBmpMask;
    
    /**
     * Froground bitmap's position
     */
    TRect iForgroundBmpRect;
    
    /**
     * The img ID of button in various states
     */
    TAknsItemID iNormalImgID;
    TAknsItemID iPressedImgID;
    TAknsItemID iInactiveImgID;

    HBufC* iText;
    TAknTextLineLayout iTextFormat;    
    };
    

inline void CAknFepCtrlDragButton::SetTextFormat(TAknTextLineLayout aTextFormat)
    {
    iTextFormat = aTextFormat;
    }

inline void CAknFepCtrlDragButton::SetTextL(const TDesC& aText)
    {
    delete iText;
    iText = NULL;
    iText = aText.AllocL();
    }

inline void CAknFepCtrlDragButton::SetImageFrameId( TAknsItemID aNormalID,
                                                      TAknsItemID aPressedID,
                                                      TAknsItemID aInactiveID )
    {
    iNormalImgID = aNormalID;
    iPressedImgID = aPressedID;
    iInactiveImgID = aInactiveID;
    }    

#endif // C_CAKNFEPCTRLDRAGBUTTON_H

// End Of File
