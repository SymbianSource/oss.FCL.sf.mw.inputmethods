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
* Description:  header file of choice list window
*
*/

#ifndef C_PENINPUTCOMMONCHOICELIST_H
#define C_PENINPUTCOMMONCHOICELIST_H

// INCLUDES
#include <w32std.h>
#include <e32base.h>
#include <peninputlayoutchoicelist.h>
#include <AknsConstants.h>

/** class forward decalaration */
class CFepUiLayout;

enum ECommonChoiceListStyle
	{
	EFixedLength,
	EAutoFitLength
	};
	
/**
 *  CPeninputCommonChoiceList class 
 *
 *  This class for touch input common choice list. The prop. of 
 *  this control is depend on laf data.
 *
 *  @lib 
 *  @since S60 v5.0
 */
class CPeninputCommonChoiceList : public CFepLayoutChoiceList
    {
public: 

    /**
     * Symbian Constructor
     *
     * @since S60 v5.0
     * @param aUiLayout 		Pointer to uilayout
     * @param aControlId 		Choice list id
     * @param aItemSkinID 		Item skin id
     * @param aBackgroundSkinID Background skin id
     * @return Pointer to created CPeninputCommonChoiceList object
     */
    IMPORT_C static CPeninputCommonChoiceList* NewL( CFepUiLayout* aUiLayout, 
    											     TInt aControlId,
    											     TAknsItemID aItemSkinID,
    											     TAknsItemID aBackgroundSkinID,
    											     ECommonChoiceListStyle aListStyle = EAutoFitLength);

    /**
     * Destructor
     *
     * @since S60 v5.0
     * @return None
     */
    IMPORT_C virtual ~CPeninputCommonChoiceList();
    
    /**
     * Add rect of item to the end of list
     *
     * @param aRect 	The rect of the item
     * @since S60 v5.0
     * @return None
     */    
    IMPORT_C void AddItemRectL( const TRect& aRect );
    
    /**
     * Remove all the rects of item
     *
     * @since S60 v5.0
     * @return None
     */    
    IMPORT_C void RemoveAllItemsRect();
    
    /**
     * Set text layout data
     *
     * @param aTextLayout 	Text layout from laf data
     * @since S60 v5.0
     * @return None
     */    
    IMPORT_C void SetTextLayout( const TAknTextLineLayout& aTextLayout );
    
    /**
     * Set list column count. The maxium column of the control is based on the laf
     * Any number exceeds the maxium will be ignored.
     *
     * @param aMaxCount 
     * @since S60 v5.0
     * @return None
     */   
    IMPORT_C void SetListColumnNumWithLafL( TInt aMaxCount);
    
    /**
     * Set skin ID
     *
     * @param aItemSkinID  			item skin ID
     * @param aBackgroundSkinID  	background skin ID
     * @since S60 v5.0
     * @return None
     */   
    IMPORT_C void SetListSkinID( TAknsItemID aItemSkinID, TAknsItemID aBackgroundSkinID );
    /**
     * Hit Test
     *
     * @param aPoint  			touched point
     * @return Index of list.
     */       
    IMPORT_C TInt HitTest(const TPoint& aPoint);    							 
protected:
    /**
     * Constructor.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @param aItemSkinID 		Item skin id
     * @param aBackgroundSkinID Background skin id
     */  
	IMPORT_C CPeninputCommonChoiceList( CFepUiLayout* aUiLayout, 
							   TInt aControlId,
							   TAknsItemID aItemSkinID,
							   TAknsItemID aBackgroundSkinID,
							   ECommonChoiceListStyle aListStyle );

	TInt MaxWidthOfListItem();
private:
	/**
     * Choice list draw function
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C virtual void Draw();
    
	/**
     * Calculate rect based on item count. (only length, suppose the count is fixed)
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C virtual void ReCalcLayout();  
     
    void DrawBackground();
    void DrawItem( TInt aIndex, TBool aIsFocus );
private: // data
    /**
     * Item rect array.
     */
	RArray<TRect> iItemRects;
	
    /**
     * Item text layout
     */	
	TAknTextLineLayout iTextLayout;
	
    /**
     * The skin ID of sub item and background
     */
    TAknsItemID iItemSkinID;
    TAknsItemID iBackgroundSkinID;
    
    /**
     * The style of the list
     */
	ECommonChoiceListStyle iListStyle;
	
    /**
     * The last column number of the list
     */
	TInt iLastColumnNum;
    };

#endif //C_PENINPUTCOMMONCHOICELIST_H
//End Of File
