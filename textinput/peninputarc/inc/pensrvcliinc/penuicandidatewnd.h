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

#ifndef PENUICANDIDATEWND_H
#define PENUICANDIDATEWND_H

#include <coecntrl.h>
#include <AknsItemID.h> 
#include <AknLayoutDef.h>

class MPenUiWndEventObserver;

/**
 *  The constant for max text length of 
 *  choice list item title.
 */
const TInt KMaxItemTextLength = 100;

/**
 *  CPenUiCandidateWnd
 *  
 *  CCoeControl based class used for candidate list
 * 
 */
class CPenUiCandidateWnd : public CCoeControl
    {
public:
   /**
    * Store LAF data for candidate list
    */
    class TPenUiCandidateLafData
        {
    public:
        TSize iItemSize;    
        TInt iHorMargin;
        TInt iVerMargin;
        TSize iNaviSize;
        TSize iNaviInnerSize;
        TAknTextLineLayout iTextLine;
        TAknTextLineLayout iPageTextLine;
        const CFont* iFont;
        };

    /**
     * Choice list item data structure.
     */
    class TPenUiCandidateItem
        {
    public:
        /*
         * Command of choice list item.
         */
        TInt iCommand;        
        /*
         * Title of choice list item.
         */
        TBuf<KMaxItemTextLength> iText;
        };
    
    /**
     * Two-phased constructor.
     * 
     * @since symbian^3
     * @param aServer the pointer to the owner of the control     
     * @return Pointer to CPenUiCandidateWnd's instance
     */
    static CPenUiCandidateWnd* NewL( MPenUiWndEventObserver* aObserver );

    /**
     * Two-phased constructor.
     * 
     * @since symbian^3
     * @param aServer the pointer to the owner of the control     
     * @return Pointer to CPenUiCandidateWnd's instance
     */
    static CPenUiCandidateWnd* NewLC( MPenUiWndEventObserver* aObserver );
    
    /**
     * Destructor
     */
    ~CPenUiCandidateWnd();
    
    /**
     * Handle raw events of pen input.
     *
     * @since Symbian^3 
     * @param aEventData The event data.
     * @return ETrue if layout has processed the event, otherwise EFalse
     */
    TBool HandleRawEventL( const TAny* aEventData ); 

    /**
     * Set window priority.
     *
     * @since Symbian^3     
     * @param aPriority the priority of candidate
     * @return None     
     */
    inline void SetPriority( TInt aPriority );
    
    /**
     * Set the alienment of text in candidate.
     *
     * @since Symbian^3     
     * @param aAlign The alignment to be used.
     * @return None
     */  
    inline void SetAlign( CGraphicsContext::TTextAlign aAlign );
    
    /**
     * Set the init rect used for the top-left point of candidate.
     *
     * @since Symbian^3     
     * @param aInitRect The initial rect used for the top-left position
     *                  of candidate.
     * @return None     
     */  
    inline void SetInitRect( const TRect& aInitRect );
    
    /**
     * Enable spell functionality of candidate
     *
     * @since Symbian^3     
     * @param aSpellEnabled ETrue: enable spell;
     *                      EFalse: disable spell.
     * @return None     
     */     
    inline void EnableSpell( TBool aSpellEnabled );

    /**
     * Enable minimal text width when calculates item width.
     *
     * @since Symbian^3     
     * @param aTextWidthEnabled ETrue: enable text line;
     *                      EFalse: disable text line.
     * @return None     
     */     
    inline void EnableTextWidth( TBool aTextWidthEnabled );
    
    /**
     * Show candidate list
     * 
     * @since Symbain^3
     * @param aItemArray, the contents to be shown
     * @param aActiveIndex, the default index
     * @return None
     */    
    void ShowCandidateListL( const CDesCArray* aItemArray, TInt aActiveIndex );
    
    /**
     * Hide candidate list
     * 
     * @since Symbian^3
     * @return None
     */        
    void HideCandidateList();

// From CCoeControl    
    /**
     * Handles a change to the control's resources.
     *  
     * @since symbian^3
     * @param aType, a message UID value.
     * @return None
     */    
    void HandleResourceChange( TInt aType );
    
private:

    /**
     * C++ Default Constructor 
     */
    CPenUiCandidateWnd( MPenUiWndEventObserver* aObserver );

    /**
     * 2nd phase constructor. 
     * 
     * @since Symbain^3
     * @return None
     */
    void ConstructL();
    
    /**
     * Set all kinds of frame ids 
     * 
     * @since Symbain^3
     * @param aBgFrId, background frame id;
     * @param aNaviFrId, normal navi button frame id;
     * @param aNaviActiveFrId, pressed navi button frame id;
     * @param aNaviDimFrId, dimmed navi button frame id;
     * @param aItemActiveFrId, heightligh item frame id; 
     * @return None
     */
    void SetImgFrId( TAknsItemID aBgFrId, 
                     TAknsItemID aNaviFrId,
                     TAknsItemID aNaviActiveFrId,
                     TAknsItemID aNaviDimFrId,                     
                     TAknsItemID aItemActiveFrId );
    
    /**
     * Construct candidate list from resource. 
     * 
     * @return None
     */    
    void ConstructFromResourceL();
    
    /**
     * Construct up / down arrow buttons from resource. 
     * 
     * @since Symbain^3
     * @param aReader: the resource reader;
     * @param aUpDown, ETrue, construct up button;
     *             EFalse, construct down button;  
     * @return None
     */  
    void ConstructPageInfoFromResourceL( TResourceReader& aReader, TBool aUpDown );
    
    
    /**
     * Read LAF data for candidate list 
     *  
     * @since Symbian^3     
     * @return None
     */    
    void ReadLafInfo();
    
    /**
     * Set size for bitmaps
     * 
     * @since Symbian^3
     * @return None
     */    
    void SetBitmapSize();
    
    /**
     * Create mask bitmap for dimmed icons 
     *  
     * @since Symbian^3
     * @param aDimmedMask, store the generated mask bitmap for dimmed icons.
     * @param aMask. the source bitmap
     * @return None
     */    
    void CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                            const CFbsBitmap* aMask ); 

    /**
     * Capture all poninters of full screen when candidate list is opened. 
     * 
     * @since Symbain^3
     * @param aFlag, ETrue: capture all pointers of full screen.
     *               EFalse: don't capture all pointers of full screen.
     * @return None
     */       
    void SetCapturePointer( TBool aFlag );   
    
     /**
      * Get page info and transfer it to descritor.
      *
      * @since Symbian^3      
      * @return None.     
      */      
    void GetPageInfoBufL();
    
    /**
     * Set the text item array to candidate list
     * 
     * @since Symbian^3
     * @param aItemArray: The text item array to be shown.
     * @return None
     */        
    void SetItemArrayL( const CDesCArray* aItemArray );
    
    /**
     * Add spell item
     * 
     * @since Symbian^3
     * @return None
     */        
    void AddSpellItemL();
    
    /**
     * Calculate the displaying rect of candidate.
     * 
     * @since Symbian^3
     * @return None
     */         
    void ReCalcLayout();
    
    /**
     * Return the dynamical width for displaying text.
     * 
     * @since Symbian^3
     * @return The fittable width of candidate.
     */            
    TInt WidthToFitText(); 
    
    /**
     * Return the rect to fit the screen size.
     * 
     * @since Symbian^3
     * @param aRect: the source rect to be checked
     * @return the rect of fittable to screen size
     */            
    TRect RectToFitScreen( const TRect& aRect );
    
    /**
     * Draw each item.
     * 
     * @since Symbian^3
     * @param aGc: the pointer of window gc
     * @param aIndex: the index of the item to be drawn.
     * @return None
     */    
    void DrawItem( CWindowGc* aGc, TInt aIndex ) const;
    
    /**
     * Draw page down / up arrow button.
     * 
     * @since Symbian^3
     * @param aGc: the pointer of window gc
     * @param aUpDown: ETrue, the up arrow button,
     *                 EFalse, the down arrow button
     * @return None
     */    
    void DrawPageButton( CWindowGc* aGc, TBool aUpDown ) const;
    
    /**
     * Draw page into text.
     * 
     * @since Symbian^3
     * @param aGc: the pointer of window gc
     * @return None
     */
    void DrawPageInfo( CWindowGc* aGc ) const;
  
    /**
     * Handle pointer down event
     *
     * @since Symbian^3 
     * @param aPoint The point position relative the layout
     * @return None.
     */                                        
     void HandlePointerDownEvent(const TPoint& aPoint);
    
    /**
     * Handle pointer move event
     *
     * @since Symbian^3
     * @param aPoint The point position relative the layout
     * @return None.       
     */                  
     void HandlePointerMoveEvent(const TPoint& aPoint);
    
    /**
     * Handle pointer up event
     *
     * @since Symbian^3
     * @return None.      
     */                  
     void HandlePointerUpEventL();  
     
     /**
      * Check pointer area of pointer event 
      *
      * @param aPoint The point position relative the layout
      * @return None.     
      */                  
     void CheckPointerEvent(const TPoint& aPoint);
      
     /**
      * Return the index of pointed text item.
      *  
      * @since symbian^3
      * @param aPoint, the position of pointer down.
      * @return the index of the pointed text item.
      */    
     TInt ItemIndexByPoint( const TPoint& aPoint );
     
     /**
      * Report item selected event to ui layout
      *
      * @since Symbian^3
      * @return None.     
      */
     void ReportItemSelectedEventL();
     
     /**
      * Delete all bitmaps.
      *
      * @since Symbian^3
      * @return None.     
      */ 
     void DeleteAllBitmaps();
     
     /**
      * Handle skin change.
      *
      * @since Symbian^3
      * @return None.     
      */ 
     void HandleSkinChangeL(); 
     
 // From CCoeControl
     /**
      * Responds to changes to the size and position of the contents of this control.
      * 
      * @since Symbian^3
      * @return None
      */         
     void SizeChanged();
     
     /**
      * Draw candidate control.
      * 
      * @since Symbian^3
      * @param aRect: the rect to be drawn.
      * @return None
      */    
     void Draw( const TRect& aRect ) const;       
     
private:
    /**
     * The alignment of text in candidate list
     *      
     */
    CGraphicsContext::TTextAlign iAlign;
    
    /**
     * The initial rect for the top-left point
     *      
     */
    TRect iInitRect;
    
    /**
     * If enabling the spell functionality
     *      
     */
    TBool iSpellEnabled;
    
    /**
     * If enabling text width
     *      
     */
    TBool iTextWidthEnabled;

    /**
     * Store Laf Data.
     *      
     */
    CPenUiCandidateWnd::TPenUiCandidateLafData iLafData;
    
    /**
     * Store the item list.
     *      
     */
    RPointerArray<CPenUiCandidateWnd::TPenUiCandidateItem> iItemList;
    
    /**
     * Background frame id.
     *
     */
    TAknsItemID iBgFrId;    

    /**
     * Hilight frame id.
     *
     */
    TAknsItemID iItemActiveFrId;
    
    /**
     * Navi buttons normal frame id.
     *
     */    
    TAknsItemID iNaviFrId;

    /**
     * Navi buttons pressed frame id.
     *
     */
    TAknsItemID iNaviActiveFrId;
    
    /**
     * Navi buttons dim frame id.
     *
     */    
    TAknsItemID iNaviDimFrId;
    
    /**
     * The width of text area
     *
     */
    TInt iItemWidth;
    
    /**
     * The rect of list pane
     *
     */
    TRect iListRect;
    
    /**
     * The rect of scroll pane
     *
     */
    TRect iScrollRect;

    /**
     * The rect of up arrow button
     *
     */
    TRect iPageUpRect;

    /**
     * The rect of down arrow button
     *
     */    
    TRect iPageDownRect;
    
    /**
     * The rect of each item.
     *
     */
    RArray<TRect> iItemRects;
    
    /**
     * The size of separator line.
     *
     */    
    TSize iSeparatorSize;
    
    /**
     * The index of current page.
     *
     */
    TInt iCurrentPage;
    
    /**
     * The number of total page.
     *
     */    
    TInt iTotalPages;

    /**
     * The index of focused item.
     *
     */ 
    TInt iFocusItem;
    
    /**
     * The text of page info
     *
     * Owned
     */     
    HBufC* iPageInfo; 
    
    /**
     * The area type of pointer 
     *
     */    
    TInt iPointerArea;
    
    /**
     * If navi buttons need to be shown
     *
     */
    TBool iNaviBtnShown;

    /**
     * The bitmap of page up button.
     *
     * Owned.
     */
    CFbsBitmap* iPageUpBmp;
    
    /**
     * The mask bitmap of page up button.
     *
     * Owned.
     */
    CFbsBitmap* iPageUpBmpMask;

    /**
     * The mask bitmap of dimmed page up button.
     *
     * Owned.
     */
    CFbsBitmap* iPageUpDimBmpMask;
    
    /**
     * The mask bitmap of dimmed page down button.
     *
     * Owned.
     */
    CFbsBitmap* iPageDownBmp;

    /**
     * The bitmap of page down button.
     *
     * Owned.
     */
    CFbsBitmap* iPageDownBmpMask;
    
    /**
     * The mask bitmap of page down button.
     *
     * Owned.
     */
    CFbsBitmap* iPageDownDimBmpMask;
    
    /**
     * The bitmap of separator.
     *
     * Owned.
     */
    CFbsBitmap* iSeperationBmp;

    /**
     * The mask bitmap of separator.
     *
     * Owned.
     */    
    CFbsBitmap* iSeperationMaskBmp;
    
    /**
     * The observer of the control
     * 
     * Now own
     */
    MPenUiWndEventObserver* iObserver;    
    };


/**
* Set window priority.
*/
inline void CPenUiCandidateWnd::SetPriority( TInt aPriority )
    {
    Window().SetOrdinalPosition( 0, aPriority );    
    }

/**
* Set the alienment of text in candidate.
*/ 
inline void CPenUiCandidateWnd::SetAlign(CGraphicsContext::TTextAlign aAlign)
    {
    iAlign = aAlign;
    }

/**
* Set the init rect used for the top-left point of candidate.
*/ 
inline void CPenUiCandidateWnd::SetInitRect( const TRect& aInitRect )
    {
    iInitRect = aInitRect;
    }
    
/**
* Enable spell functionality of candidate
*/ 
inline void CPenUiCandidateWnd::EnableSpell( TBool aSpellEnabled )
    {
    iSpellEnabled = aSpellEnabled;
    }

/**
* Enable minimal text width when calculates item width.
*/ 
inline void CPenUiCandidateWnd::EnableTextWidth( TBool aTextWidthEnabled )
    {
    iTextWidthEnabled = aTextWidthEnabled;
    }

#endif //R_RPENINPUTANIM_H

// End of File
