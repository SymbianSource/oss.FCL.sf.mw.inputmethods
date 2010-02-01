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
* Description:  Multi rows list with icon for drop-down list objects
*
*/


#ifndef C_CLISTMULTIROWWITHICON_H
#define C_CLISTMULTIROWWITHICON_H

//  INCLUDES
#include <e32std.h>
#include <w32std.h>

#include "peninputlist.h"
#include "peninputcandidate.h"

// class forward decalaration
class MFepCtrlDropdownListContext;

/**
 *  Multi rows list with icon for drop-down list
 *
 *  Multi rows list with icon for drop-down list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */  
NONSHARABLE_CLASS(CListMultiRowWithIcon) : public CList
    {
    	
public: // Methods

    /**
     * Create a object of CListMultiRowWithIcon, static method
     *
     * @since S60 v3.2
     * @param aManager Pointer to the list manager
     * @param iOwner The drop down list
     * @return The CListMultiRowWithIcon pointer.
     */
    static CListMultiRowWithIcon* NewL(CListManager* aManager, 
                                       MFepCtrlDropdownListContext* iOwner);
    
    /**
     * Destructor
     *
     * @since S60 v3.2
     */    
    virtual ~CListMultiRowWithIcon();

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
     * @param aRecalculate Re-calculate or not calculate the rect of candidates, the default is re-calculate
     */
    virtual void UpdateListL(TBool aDrawFlag = ETrue, TBool aRecalculate = ETrue);

    /**
     * Deal with the pointer down event.
     *
     * @since S60 v3.2
     * @param aPoint The point of the mouse
     * @return None.
     */ 
    virtual void HandlePointerDownL(const TPoint& aPoint);

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
     * @param aManager The list manager
     * @param aOwner The context of the drop down list     
     * @since S60 v3.2
     */  
    CListMultiRowWithIcon(CListManager* aManager, MFepCtrlDropdownListContext* aOwner);

// from base class CList

    /**
     * Draw navigation button when pressed.
     *
     * @since S60 v3.2
     * @param aRect The rect which will be drawn
     * @param aIndicate indicate the button is pressed or not
     */ 
    TBool DrawNavigationPress(const TRect& aRect, TBool aIndicate);        

protected:

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

    virtual void RefreshListL(TBool aRecalculate);

protected:

    /**
     * the bitmap for close button
     * Not own
     */  
    const CFbsBitmap* iBButtonClose;

    /**
     * the mask bitmap for close button
     * Not own
     */  
    const CFbsBitmap* iBButtonMaskClose;

    /**
     * the bitmap for next button
     * Not own
     */  
    const CFbsBitmap* iBButtonDown;

    /**
     * the mask bitmap for next button
     * Not own
     */  
    const CFbsBitmap* iBButtonMaskDown;

    /**
     * the bitmap for up button
     * Not own
     */  
    const CFbsBitmap* iBButtonUp;

    /**
     * the mask bitmap for up button
     * Not own
     */  
    const CFbsBitmap* iBButtonMaskUp;
    };

#endif //C_CLISTMULTIROWWITHICON_H

// End Of File
