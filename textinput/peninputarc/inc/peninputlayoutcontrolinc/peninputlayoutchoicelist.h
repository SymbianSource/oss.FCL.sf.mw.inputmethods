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

#ifndef C_FEPLAYOUTCHOICELIST_H
#define C_FEPLAYOUTCHOICELIST_H

#include <AknsConstants.h>
#include "peninputlayoutpopupwnd.h"

/**
 *  The constant for max text length of 
 *  choice list item title.
 */
const TInt KMaxItemTextLength = 100;

/**
 *  class CFepLayoutChoiceList
 *
 *  Popup choick list control. This class could be used directly by user.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */
class CFepLayoutChoiceList : public CFepLayoutPopupWnd
    {
public:
    enum TListHitPosition
        {
        EListInsideNoneListItem = -2,
        EListOutside = -1,
        EListItemFirst = 0
        };
    /**
     * struct SEvent
     * Event data structure reported by choice list control
     * 
     */
    struct SEvent
        {
        /*
         * selected item index, -1 if user cancel
         * popup list by click outside popup list
         */
        TInt iIndex;
        /*
         * selected item command
         */
        TInt iCommand;
        };

    /**
     * struct SItem
     * Choice list item data structure.
     * 
     */
    struct SItem
        {
        /*
         * Command of choice list item.
         */
        TInt iCommand;

        /*
         * Title of choice list item.
         */
        TBuf<KMaxItemTextLength> iText;
        };
        
public:
    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CFepLayoutChoiceList class        
     */                               
    IMPORT_C static CFepLayoutChoiceList* NewL(CFepUiLayout* aUiLayout,
                                               TInt aControlId);


    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    IMPORT_C  ~CFepLayoutChoiceList();
    
    /**
     * Set choice list item rectangle.        
     *
     * @since S60 V4.0        
     * @param aItemRect Single item rect, top left should be (0,0)
     * @param aFocusRect Focused rect, the coordinate is in aItemRect.
     * @return None        
     */                                       
    IMPORT_C void SetItemRect(const TRect& aItemRect, const TRect& aFocusRect);

    /**
     * Set choice list item text display position, relative to
     * iItemRect.        
     *
     * @since S60 V4.0        
     * @param aMargin Offset to add to the position as calculated
                      using specified rectangle
     * @param aBaseLine An offset in pixels for the baseline from
                        the normal position.
     * @return None        
     */                                       
    inline void SetMarginAndBaseline(TInt16 aMargin, TInt16 aBaseLine);

    /**
     * Set choice list item text display font.        
     *
     * @since S60 V4.0        
     * @param aFont Font used to display text. Ownership not transferred.
     */                                       
    inline void SetFont(const CFont* aFont);


    /**
     * Set choice list item text display color.        
     *
     * @since S60 V4.0        
     * @param aCol Font color.
     * @return None        
     */                                       
    inline void SetFontColor(const TRgb& aCol);

    /**
     * Set bitmap for focus.        
     *
     * @since S60 V4.0        
     * @param aItemFocusBmp The bitmap. Ownership transferred to choice list.
     * @return None        
     */                                 
    IMPORT_C void SetFocusBmp(CFbsBitmap* aItemFocusBmp);

    /**
     * Set mask bitmap for focus.
     *
     * @since S60 V4.0        
     * @param aItemFocusBmpMask The bitmap. Ownership transferred to choice list.
     * @return None        
     */                                 
    IMPORT_C void SetFocusBmpMask(CFbsBitmap *aItemFocusBmpMask);
    
    /**
     * Get bitmap for focus .        
     *
     * @since S60 V4.0        
     * @return The bitmap        
     */                                 
    inline const CFbsBitmap* FocusBmp();

    /**
     * Set mask bitmap for focus .        
     *
     * @since S60 V4.0        
     * @return The bitmap        
     */                                 
    inline const CFbsBitmap* FocusBmpMask();

    inline const RPointerArray<CFepLayoutChoiceList::SItem>& ItemArray();

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
    IMPORT_C const CFepLayoutChoiceList::SItem* ItemByIndex(TInt aIndex);

    /**
     * Retrieve a choice list item by item's command
     * Only first found item is returned.
     *
     * @since S60 V4.0        
     * @param aCommand Item command
     * @return choice list item pointer. NULL if not found.
     */                                 
    const CFepLayoutChoiceList::SItem*  ItemByCommand(TInt aCommand);
    
    /**
     * Report event to observer
     *
     * @since S60 V4.0
     * @param aEvent Event to report
     * @return None.      
     */                  
    IMPORT_C void ReportChoiceEvent(CFepLayoutChoiceList::SEvent& aEvent);


    //inline CEikListBox* ListBox();
    /**
     * Find the position for given command
    /**
     * Get item whose region contains the point.
     *
     * @since S60 V5.0
     * @param aCmd The command to be found
     * @return The index of choice item. -1 if no item contains
     *         the command
     */                                      
     IMPORT_C TInt FindCommand(TInt aCmd);    

    /**
     * Set skin ID of subitem and background
     *
     * @since S60 V4.0
     * @param aSubItemID 	Skin ID of sub item
     * @param aBackgroundID Skin ID of background
     * @return None.     
     */                  
    IMPORT_C void SetSkinID( TAknsItemID aSubItemID, TAknsItemID aBackgroundID );
    
    /**
     * Get current selected item index
     *
     * @since S60 V4.0
     * @return index.     
     */   
    IMPORT_C TInt CurrentFocusedIndex( ) const;

    /**
     * Set current selected item index
     *
     * @since S60 V4.0
     * @param aIndex selected item index
     * @return None.     
     */       
    IMPORT_C void SetCurrentFocusedIndex(const TInt aIndex);
    
protected:

    /**
     * Constructor.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     */                  
    IMPORT_C CFepLayoutChoiceList(CFepUiLayout* aUiLayout, 
                                  TInt aControlId);
    
    /**
     * Get item whose region contains the point.
     *
     * @since S60 V4.0
     * @param aPoint The point position the layout
     * @return The index of choice item. -1 if no item contains
     *         the point
     */                                        
     IMPORT_C virtual TInt HitTest(const TPoint& aPoint);

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
    
    IMPORT_C void AfterDisplayed();
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
    void DrawItem(TInt aIndex, TBool aErase);

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
    void DrawItem(const TRect& aRect, const CFepLayoutChoiceList::SItem& aItem, 
                  TBool aErase, TBool aFocus);

    /**
     * Draw choice list background
     *
     * @since S60 V4.0
     * @param aRect The area to draw
     * @param aDrawBorder ETrue, draw border 
                          EFalse, don't draw border
     * @return None.     
     */                  
    void DrawChoiceListBackground(const TRect& aRect);//, TBool aDrawBorder = ETrue);

    /**
     * Draw bitmap helper function
     *
     * @since S60 V4.0
     * @param aDestRect The destination rect to draw
     * @param aSrcRect  The source rect of bitmap
     * @param aFast ETrue, use Bitblt to draw bitmap
                    EFalse, use DrawBitmap to draw
     * @return None.     
     */                  
    void DrawBitmap(const TRect& aDestRect, const TRect& aSrcRect, CFbsBitmap* aBmp, TBool aFast);


    /**
     * Choice list draw function
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C virtual void Draw();

    /**
     * Prepare display.
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C void OnDisplay();
    
    /**
     * Calculate rect based on item count.
     *
     * @since S60 V4.0
     * @return None.     
     */
    IMPORT_C virtual void ReCalcLayout();
    
    static TInt FreezeCallBack( TAny *aPtr );    
    
    void AfterDisplayedL();
    
protected:
    /**
     * Single item rect
     */
    TRect iItemRect;

    /**
     * Focus item rect
     */
    TRect iItemFocusRect;

    /**
     * Draw text left side margin
     */
    TInt16 iMargin;

    /**
     * Draw text baseline
     */
    TInt16 iBaseline;

    /**
     * Current selecting item
     */
    TInt   iCurFocusItem;

    /**
     * Focus bitmap.
     */
    CFbsBitmap* iItemFocusBmp; 

    /**
     * Focus mask bitmap.
     */
    CFbsBitmap* iItemFocusBmpMask; 

    /**
     * Choice list items array.
     */
    RPointerArray<CFepLayoutChoiceList::SItem> iItemList;

    /**
     * Item text display font. Not Own.
     */
    const CFont* iFont;

    /**
     * Item text display color.
     */
    TRgb  iFontColor;
    
    /**
     * Update editor cursor timer
     */
    CPeriodic* iFreezeTimer;
     
    /**
     * The last selected item index
     */
     TInt iLastSelIndex;
    
    /**
     * The skin ID of sub item and background
     */
     TAknsItemID iSubItemSkinID;
     TAknsItemID iBackgroundSkinID;
    };

#include "peninputlayoutchoicelist.inl"
#endif //C_FEPLAYOUTCHOICELIST_H
//End Of File
