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
* Description:  Multi rows list without icon for drop-down list objects
*
*/


#ifndef C_LISTMULTIROWWITHOUTICON_H
#define C_LISTMULTIROWWITHOUTICON_H

/**
 *  Multi rows list without icon for drop-down list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */  
NONSHARABLE_CLASS(CListMultiRowWithoutIcon) : public CList
    {
    	
public: // Methods

    /**
     * Create a object of CListMultiRowWithoutIcon, static method
     *
     * @since S60 v3.2
     * @param aManager Pointer to the list manager
     * @param iOwner The drop down list
     * @return The CListMultiRowWithoutIcon pointer.
     */
    static CListMultiRowWithoutIcon* NewL(CListManager* aManager, 
                                       MFepCtrlDropdownListContext* iOwner);
    
    /**
     * Destructor
     *
     * @since S60 v3.2
     */    
    virtual ~CListMultiRowWithoutIcon();

// from base class CList
    
     /**
     * Draw the list
     *
     * @since S60 v3.2
     */  
    virtual void Draw();

    /**
     * Update the list, including re-calculate the rect of candidates.
     *
     * @since S60 v3.2
     * @param aRecalculate Re-calculate or not calculate 
	 *		  the rect of candidates, the default is re-calculate
     */
    virtual void UpdateListL(TBool aDrawFlag = ETrue, TBool aRecalculate = ETrue);

    /**
     * Deal with the pointer up event.
     *
     * @since S60 v3.2
     * @param aPoint The point of the mouse
     * @return None.     
     */  
    virtual void HandlePointerUpL(const TPoint& aPoint);

    /**
     * Deal with the pointer drag event.
     *
     * @since S60 v3.2
     * @param aPoint The point of the mouse
     * @return None.
     */ 
    virtual void HandlePointerDragL(const TPoint& aPoint);
    
protected: // Methods

    /**
     * Constructor.
     *
     * @since S60 v3.2
     * @param aManager The list manager
     * @param aOwner The context of the drop down list     
     */  
    CListMultiRowWithoutIcon( CListManager* aManager, 
							  MFepCtrlDropdownListContext* aOwner);
    
private: //Methods

    /**
     * Calculate the position of current visible candidates.
     *
     * @since S60 v3.2
     * @return the last candidate visible.
     */  
    virtual TInt CalculatePositionsL();

    /**
     * Calculate all kinds of rectangles of list.
     *
     * @since S60 v3.2
     */  
    virtual void CalculateAreas();

    };

#endif // C_LISTMULTIROWWITHOUTICON_H

// End Of File
