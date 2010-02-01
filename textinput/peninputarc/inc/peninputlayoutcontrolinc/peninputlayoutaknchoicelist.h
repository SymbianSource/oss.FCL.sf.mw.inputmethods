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
* Description:  header file of choice list window for avkon version
*
*/

#ifndef C_AKN_FEPLAYOUTCHOICELIST_H
#define C_AKN_FEPLAYOUTCHOICELIST_H

#include "peninputlayoutchoicelist.h"
#include <eikclb.h>
#include <aknlists.h>
class CAknSingleStyleListBox;
class CAknsFrameBackgroundControlContext;

/**
 *  class CFepLayoutAknChoiceList
 *
 *  Popup choick list control. This class could be used directly by user.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */
class CFepLayoutAknChoiceList : public CFepLayoutChoiceList
    {
public:
    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CFepLayoutChoiceList class        
     */                               
    IMPORT_C static CFepLayoutAknChoiceList* NewL(CFepUiLayout* aUiLayout,
                                         TInt aControlId,CCoeControl* aParent);

    //IMPORT_C static CFepLayoutAknChoiceList* NewL(CFepUiLayout* aUiLayout,
      //                                     TInt aControlId,CAknChoiceListPopup* aList);

    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    //IMPORT_C  ~CFepLayoutAknChoiceList();
    

    /**
     * Set choice list items.
     *
     * @since S60 V4.0        
     * @param aItemList Item list array. The function copies them internally.
     * @return None        
     */                                 
    IMPORT_C void SetItemsL(const RPointerArray<CFepLayoutChoiceList::SItem>& aItemList);

    /**
     * Append a choice list item.
     *
     * @since S60 V4.0        
     * @param aItem Item to add.
     * @return None        
     */                                 
    IMPORT_C void AddItemL(const CFepLayoutChoiceList::SItem& aItem);

    /**
     * Insert a choice list item at specified postion.
     *
     * @since S60 V4.0        
     * @param aPostion The position is to be inserted.
     * @param aItem Item to insert.
     * @return None        
     */                                 
    IMPORT_C void InsertItemL(TInt aPostion, const CFepLayoutChoiceList::SItem& aItem);

    /**
     * Remove a choice list item by command
     * Only the first found item is removed.
     *
     * @since S60 V4.0        
     * @param aCommand Item command.
     * @return None        
     */                                 
    IMPORT_C void RemoveItemByCommand(TInt aCommand);

    /**
     * Remove a choice list item by index
     *
     * @since S60 V4.0        
     * @param aIndex Item index
     * @return None        
     */                                 
    IMPORT_C void RemoveItemByIndex(TInt aIndex);

    /**
     * Clear all choice list items
     *
     * @since S60 V4.0        
     * @return None        
     */                                 
    IMPORT_C void ClearItemsL();
    
    /**
     * Clear all choice list items
     *
     * @since S60 V4.0        
     * @return None        
     */                                 
    IMPORT_C TInt ItemsCount();
    
    /**
     * Retrieve a choice list item by index
     *
     * @since S60 V4.0        
     * @param aIndex Item index
     * @return choice list item pointer of specified index
     */                                 
    //const CFepLayoutChoiceList::SItem* ItemByIndex(TInt aIndex);

    /**
     * Retrieve a choice list item by item's command
     * Only first found item is returned.
     *
     * @since S60 V4.0        
     * @param aCommand Item command
     * @return choice list item pointer. NULL if not found.
     */                                 
    //const CFepLayoutChoiceList::SItem*  ItemByCommand(TInt aCommand);

    IMPORT_C CEikColumnListBox* ListBox();
    
    IMPORT_C CEikFormattedCellListBox* FormatedCellListBox();
    
    
    IMPORT_C void SetItemRect(const TRect& aItemRect, const TRect& aFocusRect);     

    //from CFepUiBaseCtrl
    IMPORT_C void SetRect(const TRect& aRect);    
    
    IMPORT_C ~CFepLayoutAknChoiceList();
protected:

   /**
     * Constructor.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     */                  
    IMPORT_C CFepLayoutAknChoiceList(CFepUiLayout* aUiLayout, 
                                  TInt aControlId,CCoeControl* aParent);
        
    IMPORT_C void ConstructL(); 
    /**
     * Get item whose region contains the point.
     *
     * @since S60 V4.0
     * @param aPoint The point position the layout
     * @return The index of choice item. -1 if no item contains
     *         the point
     */                                        
     //IMPORT_C TInt HitTest(const TPoint& aPoint);

    /**
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
private:
    /**
     * Draw a choice list item by index.
     *
     * @since S60 V4.0
     * @param aIndex The index of item to draw.
     * @param aErase ETrue, draw background 
                     EFalse, don't draw background
     * @return None.     
     */                  
//    void DrawItem(TInt aIndex, TBool aErase);

    /**
     * Draw a choice list item.
     *
     * @since S60 V4.0
     * @param aRect The position of the item.
     * @param aItem The item to draw.
     * @param aFocus Whether the item is focused or not
     * @param aErase ETrue, draw background 
                     EFalse, don't draw background
     * @return None.     
     */                  
  //  void DrawItem(const TRect& aRect, const CFepLayoutChoiceList::SItem& aItem, 
    //              TBool aErase, TBool aFocus);

    /**
     * Draw choice list background
     *
     * @since S60 V4.0
     * @param aRect The area to draw
     * @param aDrawBorder ETrue, draw border 
                          EFalse, don't draw border
     * @return None.     
     */                  
    //void DrawChoiceListBackground(const TRect& aRect);//, TBool aDrawBorder = ETrue);

    /**
     * Report event to observer
     *
     * @since S60 V4.0
     * @param aEvent Event to report
     * @return None.      
     */                  
   // void ReportChoiceEvent(CFepLayoutChoiceList::SEvent& aEvent);

    /**
     * Choice list draw function
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C void Draw();

    /**
     * Prepare display.
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C void OnDisplay(const TPoint& aOffset);
    
    IMPORT_C TBool ScrollbarVisible();
    /**
     * Calculate rect based on item count.
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C void ReCalcLayout();
    
    IMPORT_C TInt MinItemWidthInPixels();
private:
    /**
     * Adjust the txt which is appended to column list.
     * As we use column list there, we need insert "\t" to the txt in order to
     * display the txt.          
     *
     * @since S60 V5.0
     * @param aTxt The text to be added.
     * @return The txt after insert "\t" to the header.     
     */
    const TDesC& AdjustColumnText(const TDesC& aTxt);
    
    void UpdateControlContext();
    void UpdateTextColors();
    void RemoveItemByIndexL(TInt aIndex);
private:

    //CAknChoiceListPopup* iListBoxCtrl;
    //CAknSingleStyleListBox* iListBox; 
    CAknSinglePopupMenuStyleListBox* iListBox;
    CCoeControl* iParent;  
    //There must be a seperate scrollbar control to handle scroll bar pointer event.
    //because pointer event is sent to RWindow directly.
    CFepUiBaseCtrl* iScrollbarCtrl;
    
    TBool iScrollbarCaptured;
    
    //buffer used to insert '\t' to the colum list box
    HBufC* iTxtTransferBuf;
    
    CAknsFrameBackgroundControlContext* iBgContext;        
    };
#endif //C_AKN_FEPLAYOUTCHOICELIST_H
//End Of File
