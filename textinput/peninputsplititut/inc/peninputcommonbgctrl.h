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


#ifndef C_CAKNFEPCTRLCOMMONBGCTRL_H
#define C_CAKNFEPCTRLCOMMONBGCTRL_H

// system includes
#include <AknsConstants.h>
#include <peninputlayoutbasecontrol.h>

struct TCommonBgCtrlSubItem
	{
	TRect iRect;	
    TAknsItemID iFrameID;
    TAknsItemID iCenterID;
    TBool iIsShow;		
	};

// class declarations
/**
 *  Basic functionality for background display
 *  It provides creation from Frame ID and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlCommonBgCtrl : public CFepUiBaseCtrl
    {
public: 
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlCommonBgCtrl type object
     */
    static CAknFepCtrlCommonBgCtrl* NewL(CFepUiLayout* aUiLayout, 
    						TInt aControlId,
    						TAknsItemID aFrameID,
    						TAknsItemID aCenterID);
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlCommonBgCtrl type object
     */
    static CAknFepCtrlCommonBgCtrl* NewLC(CFepUiLayout* aUiLayout, 
    						TInt aControlId,
							TAknsItemID aFrameID,
    						TAknsItemID aCenterID);
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CAknFepCtrlCommonBgCtrl();
    
    /**
     * This function handles the SizeChanged event, it resizes all images and rects
     *
     * @since S60 v3.2
     * @param aRect New button extent
     * if only the position of the button changed, no need to reset the images as well
     * @return Nothing
     */
    void SizeChanged(const TRect& aRect);
	
	/**
     * Check whether this control contains the point
     *
     * @since S60 V4.0
     * @param aPoint The point to be checked
     * @return ETrue if control valid region contains the point, otherwise EFalse
     */
    virtual TBool Contains(const TPoint& aPt);
	
	/**
     * Redraw the defined area of background
     *
     * @since S60 V4.0
     * @param aPoint The point to be checked
     * @return ETrue if control valid region contains the point, otherwise EFalse
     */	
	virtual void ReDrawRect(const TRect& aRect);
		
	/**
     * Add sub background item
     *
     * @since S60 V4.0
     * @param aSubItem sub background which will show above the background
     * @return None
     */		
    void AddSubBgItemL( const TCommonBgCtrlSubItem& aSubItem );
    
	/**
     * Show sub background item
     *
     * @since S60 V4.0
     * @param aIndex item index
     * @param aShow  is show or hide
     * @return None
     */		
    void ShowSubBgItem( TInt aIndex, TBool aShow );
    
	/**
     * get sub background item
     *
     * @since S60 V4.0
     * @param aIndex item index
     * @return sut item
     */		
    TCommonBgCtrlSubItem& GetSubBgItem( TInt aIndex );
    
    /**
     * Draws UI
     *
     * @since S60 v3.2
     * @return None
     */
    void Draw();
	
	/**
     * Set Frame ID
     *
     * @since S60 v3.2
     * @param aFrameID   The frame ID
     * @param aCenterID  Center bitmap ID
     * @return None
     */
	void SetFrameID( TAknsItemID aFrameID, TAknsItemID aCenterID );
	
protected:
    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @param aFrameID   The frame ID
     * @param aCenterID  Center bitmap ID
     */
    CAknFepCtrlCommonBgCtrl(CFepUiLayout* aUiLayout, 
    						TInt aControlId,
    						TAknsItemID aFrameID,
    						TAknsItemID aCenterID);
   
    /**
     * draw frame
     *
     * @since S60 v3.2
     * @param aGc
     * @param aRect The control rect
     * @param aFrameID   The frame ID
     * @param aCenterID  Center bitmap ID
     */
    void DrawFrame( CFbsBitGc* aGc, 
    				const TRect& aRect,
    				TAknsItemID aFrameID, 
    				TAknsItemID aCenterID );
private: 
    /**
     * The img ID of frame
     */
    TAknsItemID iFrameID;
    TAknsItemID iCenterID;
    
    /**
     * The sub items of the background 
     */
    RArray<TCommonBgCtrlSubItem> iSubItems;
    };    

#endif // C_CAKNFEPCTRLCOMMONBGCTRL_H
            
// End Of File
