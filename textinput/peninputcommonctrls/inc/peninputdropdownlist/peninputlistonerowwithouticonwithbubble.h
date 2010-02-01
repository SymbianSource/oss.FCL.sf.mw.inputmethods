/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#ifndef C_CLISTONEROWWITHOUTICONWITHBUBBLE_H
#define C_CLISTONEROWWITHOUTICONWITHBUBBLE_H

//  INCLUDES
#include <e32std.h>
#include <w32std.h>

#include "peninputlist.h"
#include "peninputlistonerowwithouticon.h"
#include "peninputcandidate.h"

// class forward decalaration
class MFepCtrlDropdownListContext;

/**
 *  One rows list with icon for drop-down list
 *
 *  One rows list with icon for drop-down list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */  
NONSHARABLE_CLASS(CListOneRowWithoutIconWithBubble) : public CListOneRowWithoutIcon
    {
  
public:  // Methods

    /**
     * Create a object of CListOneRowWithoutIcon, static method
     *
     * @since S60 v3.2
     * @param iOwner The drop down list
     * @return The CListOneRowWithoutIcon pointer.
     */
    static CListOneRowWithoutIconWithBubble* NewL(CListManager* aManager, 
                                        MFepCtrlDropdownListContext* iOwner);

    /**
     * Destructor
     *
     * @since S60 v3.2
     */    
    virtual ~CListOneRowWithoutIconWithBubble();

// from base class CList

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


 // from base class CList    
    /**
     * Deal with the pointer leave event.
     *
     * @since S60 v3.2
     * @param aPoint The point of the mouse
     * @return None.
     */     
    virtual void HandlePointerLeave(const TPoint& aPoint);  
                   
protected:  // Methods

    /**
     * Constructor.
     *
     * @param aManager The list manager
     * @param aOwner The context of the drop down list           
     * @since S60 v3.2
     */  
    CListOneRowWithoutIconWithBubble(CListManager* aManager, MFepCtrlDropdownListContext* aOwner);
  
// from base class CList

    /**
     * Compare rects equal or not.
     *
     * @since S60 v3.2
     * @param aSrcRect The source rect
     * @param aDesRect The destination rect
     * @return ETrue If the two rects are equal.     
     */  
    virtual TBool CompareRect(const TRect& aSrcRect, const TRect& aDesRect);  
                   
private: //Methods


    };

#endif //C_CLISTONEROWWITHOUTICON_H

// End Of File
