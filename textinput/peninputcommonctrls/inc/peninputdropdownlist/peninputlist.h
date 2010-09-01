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
* Description:  Abstract base class for drop-down list objects
*
*/


#ifndef C_CLIST_H
#define C_CLIST_H

//  INCLUDES
#include <e32std.h>
#include <w32std.h>
#include <coedef.h>
#include <peninputlayoutbasecontrol.h>

#include "peninputbitmapdb.h"
#include "peninputcandidate.h"
#include "peninputpages.h"
#include "peninputdropdownlistcontext.h"
#include "peninputlistmanager.h"

// class forward decalaration
class CFont;

// Empty rect 
#define KEmptyRect TRect(0, 0, 0, 0);

// Dummy rect, which is used to identify mouse down, but not click in valid area
#define KDummyRect TRect(0, 0, 1, 1);


/**
 *  abstract base list
 *
 *  abstract base list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */   
NONSHARABLE_CLASS(CList) : public CBase
    {
    

    
public: //Methods

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None.     
     */  
    virtual ~CList();

    /**
     * Draw the list
     *
     * @since S60 v3.2
     * @return None.     
     */  
    virtual void Draw();

    /**
     * Get rect of the list
     *
     * @since S60 v3.2
     * @return the rect of the list
     */               
    virtual const TRect& Rect();

    /**
     * Update the list, including re-calculate the rect of candidates.
     *
     * @since S60 v3.2
     * @param aRecalculate Re-calculate or not calculate the rect of candidates, the default is re-calculate
     * @return None.     
     */
    virtual void UpdateListL(TBool aDrawFlag = ETrue, TBool aRecalculate = ETrue);

    /**
     * Deal with size change, it will be called when the size change
     *
     * @since S60 v3.2
     * @return None.     
     */    
    virtual void SizeChanged();
    
    /**
     * Deal with move event, it will be called when the drop down list is moved.
     *
     * @since S60 v3.2
     * @param aOffsetPoint The offset point
     * @return None.     
     */    
    virtual void Move(const TPoint& aOffsetPoint);

    /**
     * Set some cell to be high light.
     *
     * @since S60 v3.2
     * @param aCellIndex The which cell would be set
     * @param aHighlight The flag of highlight, set to highlight if ETrue, else cancel highlight
     * @return None.     
     */     
    virtual  void SetHighlightCell(const TInt aCellIndex, TBool aHighlight, 
                                   TBool aForceDraw = ETrue);

    virtual void SetNextPageHighlightCellAfterOnePageL(const TInt aCellIndex);

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
    
    /**
     * Deal with the pointer leave event.
     *
     * @since S60 v3.2
     * @param aPoint The point of the mouse
     * @return None.
     */     
    virtual void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * Reset some status of the list.
     *
     * @since S60 v3.2
     * @return None.
     */       
    virtual void ResetStatus();
    
    virtual TInt StartCandIdxOfLine(TInt aCandIndex);
    
    
    void SetDropdownListImgID(TDropdownListDrawInfo aDropdownListDrawInfo);
    						  
protected: // Methods

    /**
     * Constructor.
     *     
     * @since S60 v3.2
     * @param aManager The list manager
     * @param aOwner The context of the drop down list     
     * @return None.     
     */  
    CList(CListManager* aManager,MFepCtrlDropdownListContext *aOwner);

    /**
     * 2nd construct function.
     *
     * @since S60 v3.2
     * @return None.          
     */  
    void ConstructL();

    /**
     * Find candidate according the assigned point.
     *
     * @since S60 v3.2
     * @aPosition The position assigned
     * @aIndex The index of the candidate which has been found
     * @return The candidate which has been found, Null if not found.     
     */  
    CCandidate* FindCandidate(const TPoint& aPosition, TInt& aIndex);

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
     * @return None.          
     */  
    virtual void CalculateAreas();
   
    /**
     * Draw select cell, which can be candidates or buttons.
     *
     * @since S60 v3.2
     * @param aRect The rect which will be drawn
     * @param aIndicate Indicate the cell is pressed or not
     * @return None.
     */  
    virtual void DrawSelection(const TRect& aRect, TBool aIndicate); 

    /**
     * Draw candidate when pressed.
     *
     * @since S60 v3.2
     * @param aRect The rect which will be drawn
     * @param aIndicate Indicate the cell is pressed or not
     * @return None.
     */  
    virtual void DrawCandidatePress(const TRect& aRect, TBool aIndicate);

    /**
     * Draw navigation button when pressed.
     *
     * @since S60 v3.2
     * @param aRect The rect which will be drawn
     * @param aIndicate Indicate the button is pressed or not
     * @return None.
     */ 
    virtual TBool DrawNavigationPress(const TRect& aRect, TBool aIndicate);

    /**
     * Draw all visible cells.
     *
     * @since S60 v3.2
     * @return None.
     */  
    virtual void DrawCandidates();

    /**
     * Compare rects equal or not.
     *
     * @since S60 v3.2
     * @param aSrcRect The source rect
     * @param aDesRect The destination rect
     * @return ETrue If the two rects are equal.     
     */  
    virtual TBool CompareRect(const TRect& aSrcRect, const TRect& aDesRect);

    /**
     * Draw highlight cell.
     *
     * @since S60 v3.2
     * @param aIndicate Indicate the button is pressed or not
     * @return None 
     */  
    virtual void DrawHighightCell(const TRect& aSrcRect, const TBool aIndidate);
    
protected: // Data

    /**
     * The drop down list
     * Not own.
     */  
    MFepCtrlDropdownListContext *iOwner;

    /**
     * The list manager
     * Not own.
     */  
    CListManager* iManager;
    
    /**
     * the rectangle of the list
     */  
    TRect iRect;
    
    /**
     * the content rectangle of the list
     */  
    TRect iContentRect;

    /**
     * the navigation rectangle of the list
     */  
    TRect iNavigationRect;

    /**
     * the next button rectangle of the list
     */  
    TRect iNavigationNextRect;

    /**
     * the previous button rectangle of the list
     */  
    TRect iNavigationPreviousRect;

    /**
     * the close button rectangle of the list
     */  
    TRect iNavigationCloseRect;
    
    /**
     * the click rectangle of the list
     */  
    TRect iClickedRect;
    
    /**
     * the move rectangle of cell, when mouse moved
     */  
    TRect iMoveLatestRect;

    /**
     * the candidate index clicked
     */  
    TInt iClickedCandidateIndex;
    
    /**
     * the flag indicating the button is click or not
     */  
    TBool iIndicateButtonClicked;
    
    /**
     * the flag indicating the next button is enable or disable
     */  
    TBool iNextButtonDisable;

    /**
     * the flag indicating the close button is enable or disable
     */  
    TBool iCloseButtonDisable;

    /**
     * the flag indicating the previous button is enable or disable
     */  
    TBool iPreviousButtonDisable;
    
    /**
     * the flag indicating the list is multi rows or one row
     */  
    TBool iIsMultiRows;
    
    /**
     * the flag indicating the list is multi rows or one row
     */  
    TBool iPenDownInCandidate;
    
    TDropdownListDrawInfo iDropdownListDrawInfo;
    };

#endif // C_CLIST_H

// End Of File
