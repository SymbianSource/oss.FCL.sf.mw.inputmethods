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


#ifndef C_CAKNFEPCTRLCOMMONBUTTON_H
#define C_CAKNFEPCTRLCOMMONBUTTON_H

// system includes
#include <AknsConstants.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>
#include <AknLayout2Def.h>
#include <peninputpluginutils.h>

// forward declarations
class TResourceReader;

struct TDraw3PiecesFrame
	{
    TAknsItemID iImgLeftID;
    TAknsItemID iImgMiddleID;
    TAknsItemID iImgRightID;
	TRect iInnerRect;
	    
    TDraw3PiecesFrame(TAknsItemID aImgLeftID,
    					  TAknsItemID aImgMiddleID,
    					  TAknsItemID aImgRightID,
    					  TRect aInnerRect)
    	{
    	
    	iImgLeftID = aImgLeftID;
    	iImgMiddleID = aImgMiddleID;
    	iImgRightID = aImgRightID;
    		 
    	iInnerRect = aInnerRect;
    		
    	};
    	
    TDraw3PiecesFrame()
    	{
    	
    	};
	};    

// class declarations
/**
 *  Basic functionality for graphic buttons
 *  It provides creation from resource and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlCommonButton : public CButtonBase
    {
public: 
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlCommonButton type object
     */
    IMPORT_C static CAknFepCtrlCommonButton* NewL(CFepUiLayout* aUiLayout, 
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
     * @return The pointer point to CAknFepCtrlCommonButton type object
     */
    IMPORT_C static CAknFepCtrlCommonButton* NewLC(CFepUiLayout* aUiLayout, 
    						TInt aControlId,
							TAknsItemID aNormalID,
    						TAknsItemID aPressedID,
    						TAknsItemID aInactiveID);
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    IMPORT_C virtual ~CAknFepCtrlCommonButton();

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
     * Set active for this button
     *
     * @since S60 v3.2
     * @param aActiveFlag active the button or desactive it
     * @return None
     */
    IMPORT_C void SetActive(TBool aActiveFlag);

    /**
     * Set high light for the button
     *
     * @since S60 v3.2
     * @param aIsHighlight highlight the button or cancel high light it
     * @return None
     */
    IMPORT_C void SetHighlight(TBool aIsHighlight);
	
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
	         
    IMPORT_C void SetDraw3PieceFrameInfo(TDraw3PiecesFrame a3PiecesFrameInfo);
    
	/**
     * Cancel pointer down state
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void CancelPointerDownL();
    
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
    
    							 
	inline void Set3PiecesColorIcons( CPenInput3PiecesColorIcons* aColorIcons ); 
	
	/**
     * Set text color group
     *
     * @since S60 V4.0
     * @param aColorGroup Text color group
 	 * @return None
     */
	inline void SetTextColorIndex( TInt aColorGroup );   							 
	inline TInt TextColorIndex() const;
	
	inline CFbsBitmap* ForgroundBmp();
    inline CFbsBitmap* ForgroundBmpMask();
    inline const TRect& ForgroundBmpRect() const;
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     */
    IMPORT_C CAknFepCtrlCommonButton(CFepUiLayout* aUiLayout, 
    								 TInt aControlId,
    								 TAknsItemID aNormalID,
    								 TAknsItemID aPressedID,
    								 TAknsItemID aInactiveID);
			
    
    void ResizeBitmaps(TSize aInnerSize);
    
    void DeleteForgroundRes();
    
    void CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                            const CFbsBitmap* aMask
                        	/*TScaleMode aScaleMode*/ );
                        	
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
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
 	 * @return The control which handles the event
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
	
	/**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
 	 * @return The control which handles the event
     */                  	
    IMPORT_C virtual void HandlePointerLeave(const TPoint& aPt);
    
    IMPORT_C virtual void HandlePointerEnter(const TPoint& aPt);
	
    IMPORT_C void Move(const TPoint& aOffset);
protected:

    /**
     * The first time construct flag of button
     */
    TBool iFirstTimeConstruct;

    /**
     * The highlight flag of the button
     */
    TBool iHighlight;
    
    TBool iPressed;

private:

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
    
    TDraw3PiecesFrame iDraw3PiecesInfo;
    /**
     * The 3 pieces color icon (owned)
     */    
    CPenInput3PiecesColorIcons* iColorIcons;
    /**
     * Text color index
     */
    TInt iTextColorIndex;
    };

inline void CAknFepCtrlCommonButton::SetTextFormat(TAknTextLineLayout aTextFormat)
    {
    iTextFormat = aTextFormat;
    }

inline void CAknFepCtrlCommonButton::SetTextL(const TDesC& aText)
    {
    delete iText;
    iText = NULL;
    iText = aText.AllocL();
    }

inline void CAknFepCtrlCommonButton::SetImageFrameId( TAknsItemID aNormalID,
                                                      TAknsItemID aPressedID,
                                                      TAknsItemID aInactiveID )
    {
    iNormalImgID = aNormalID;
    iPressedImgID = aPressedID;
    iInactiveImgID = aInactiveID;
    }
    
inline void CAknFepCtrlCommonButton::
			Set3PiecesColorIcons( CPenInput3PiecesColorIcons* aColorIcons )
	{
	delete iColorIcons;
	iColorIcons = aColorIcons;
	}

inline void CAknFepCtrlCommonButton::SetTextColorIndex( TInt aColorGroup )
	{
	iTextColorIndex = aColorGroup;
	}
	
inline TInt CAknFepCtrlCommonButton::TextColorIndex() const
	{
	return iTextColorIndex;
	}
	
inline CFbsBitmap* CAknFepCtrlCommonButton::ForgroundBmp()
    {
    return iForgroundBmp;
    }

inline CFbsBitmap* CAknFepCtrlCommonButton::ForgroundBmpMask()
    {
    return iForgroundBmpMask;
    }
    		
inline const TRect& CAknFepCtrlCommonButton::ForgroundBmpRect() const
	{
	return iForgroundBmpRect;
	}    		
#endif // C_CAKNFEPCTRLCOMMONBUTTON_H
            
// End Of File
