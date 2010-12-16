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
* Description:  drop-down list control
*
*/


#ifndef C_CFEPCTRLDROPDOWNLIST_H
#define C_CFEPCTRLDROPDOWNLIST_H

// INCLUDES
#include <e32std.h>
#include <w32std.h>
#include <barsread.h>
#include <peninputdropdownlistcontext.h>
#include <peninputlayoutbasecontrol.h>
#include <AknsUtils.h>


/** class forward decalaration */
class CCandidate;
class CPages;
class CBitmapDb;
class CListManager;
class MEventObserver;
class CBubbleCtrl;
class TAknTextLineLayout;

/** The max length for fixed text */
const TInt KMaxFixedTextNumber = 32;
const TInt KDropDownListCtrlType = EUserCtrlBase | ECtrlBaseCtrl;

/**
 *  The interface dealing with outer event from any control except the list
 *
 *  The interface dealing with outer event from any control except the list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */
class MListEventHandler
    {
    
public:

    /**
     * The method will be called when there is event from any control except from the list
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends out the event
     * @param aEventData The data relative with the event
     */    
    virtual void HandleListOuterEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                      const TDesC& aEventData);  
    
    };

/**
 *  The drop-down list control
 *
 *  The drop-down list control
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */
class CFepCtrlDropdownList : public CFepUiBaseCtrl,  
                             public MFepCtrlDropdownListContext,
                             public MEventObserver
    { 

public: // internal type definition
    /**
     *  The type of list, expandable or no-expandable
     *
     *  The type of list, expandable or no-expandable
     *
     *  @lib FepCtrlDropdownList.lib
     *  @since S60 v3.2
     */ 
    enum TListType
        {
        EListExpandable,
        EListExpandableWithBubble,
        EListNoExpandable,
        EListNoExpandableWithBubble,
        EListExpandableWithoutIcon,
        EListExpandableRollWithIcon,
		EListExpandableRollWithIconWithBubble,
        EListExpandableMultiRowWithoutIcon,
        EListExpandableMultiRowWithoutIconWithBubble      
        };

    enum TCandTruncateType
        {
        // beginning part of candidate are hide
        ECandTruncateFromBeginning,
        // end part of candidate are shown as ellipsis
        ECandTruncateAsEllipsis,
        // end part of candidate are hide
        ECandTruncateFromEnd
        };

    enum TExpandType
        {
        ECandExpandUpward,
        ECandExpandDownward
        };

    // horizontal direction of candidate list: shrink and expanded
    enum THDirection
        {
        ECandFromLToR,
        ECandFromRToL
        };

public: //Method

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None.
     */ 
    IMPORT_C virtual ~CFepCtrlDropdownList();

    /**
     * Create one CFepCtrlDropdownList object
     *
     * @since S60 v3.2
     * @param aTopLeftPoint The top left point of the drop down list
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aReader The reader from which the bitmaps ID can be read
     * @param aCellWidth The width of one basic cell
     * @param aCellHeight The height of one basic cell
     * @param aNaviWidth The width of navidation
     * @param aColNum The number of columns in one row
     * @param aRowNum The number of rows
     * @param aFont The font used to draw the text
     * @param aHandler The handle which will deal with outer event to this control
     * @param aAlignment The alighment used to draw text
     * @param aTextMargin if alignment is not ECenter, the margin is used      
     * @param aBorderColor The color of border
     * @param aBgColor The color of background
     * @param aNavigationBgColor The background color of navigation area
     * @param aTextColor The color of text
     * @param aSpecialTextColor The color of special text
     * @param aHighLightTextColor The color of highlight text
     * @param aHighLightBgColor The background color of high light
     * @param aButtonDownBgColor The background color when button pressing
     * @return The CFepCtrlDropdownList object.
     */        
    IMPORT_C static CFepCtrlDropdownList* NewL(const TPoint& aTopLeftPoint,
                                                CFepUiLayout* aUiLayout,
                                                TInt aControlId,
                                                TResourceReader& aReader,
                                                const TInt aCellWidth,
                                                const TInt aCellHeight,
                                                const TInt aNaviWidth,
                                                const TInt aColNum,
                                                const TInt aRowNum,
                                                const CFont* aFont = NULL,
                                                MListEventHandler* aHandler = NULL,
                                                const CGraphicsContext::TTextAlign aAlignment = CGraphicsContext::ECenter,
                                                const TInt aTextMargin = 3,
                                                const TRgb aBorderColor = KRgbBlack,
                                                const TRgb aBgColor = TRgb(0xeeeeee),
                                                const TRgb aNavigationBgColor = TRgb(0xeeeeee), 
                                                const TRgb aTextColor = KRgbBlack,
                                                const TRgb aSpecialTextColor = KRgbBlack,
                                                const TRgb aHighLightTextColor = KRgbBlack,
                                                const TRgb aHighLightBgColor = TRgb(0xcccccc),
                                                const TRgb aButtonDownBgColor = KRgbGray);
    IMPORT_C static CFepCtrlDropdownList* NewL(CFepUiLayout* aUiLayout,
                                               TInt aControlId,
                                               TInt aResId,
                                               TPoint aTopLeftPoint,
                                               TSize aCellSize,
                                               TInt aColNum,
                                               TInt aRowNum);

    IMPORT_C static CFepCtrlDropdownList* NewL(CFepUiLayout* aUiLayout,
                                               TInt aControlId,
                                               TInt aResId,
                                               TPoint aTopLeftPoint,
                                               const CFont* aFont,
                                               TSize aCellSize,
                                               TInt aNaviWidth,
                                               TInt aColNum,
                                               TInt aRowNum,
                                               TInt aSpinBtnHeight,
                                               TInt aHorizontalCellMargin = 0,
                                               TInt aVerticalCellMargin = 0,
                                               THDirection aDirection = ECandFromLToR,
                                               TBool aPageShown = ETrue);

    /**
     * Create one CFepCtrlDropdownList object, need to pop up yourself
     *
     * @since S60 v3.2
     * @param aTopLeftPoint The top left point of the drop down list
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aReader The reader from which the bitmaps ID can be read
     * @param aCellWidth The width of one basic cell
     * @param aCellHeight The height of one basic cell
     * @param aNaviWidth The width of navidation
     * @param aColNum The number of columns in one row
     * @param aRowNum The number of rows
     * @param aFont The font used to draw the text
     * @param aHandler The handle which will deal with outer event to this control
     * @param aAlignment The alighment used to draw text
     * @param aTextMargin if alignment is not ECenter, the margin is used      
     * @param aBorderColor The color of border
     * @param aBgColor The color of background
     * @param aNavigationBgColor The background color of navigation area
     * @param aTextColor The color of text
     * @param aSpecialTextColor The color of special text
     * @param aHighLightTextColor The color of highlight text
     * @param aHighLightBgColor The background color of high light
     * @param aButtonDownBgColor The background color when button pressing
     * @return The CFepCtrlDropdownList object.
     */        
    IMPORT_C static CFepCtrlDropdownList* NewLC(const TPoint& aTopLeftPoint,
                                                CFepUiLayout* aUiLayout,
                                                TInt aControlId,
                                                TResourceReader& aReader,
                                                const TInt aCellWidth,
                                                const TInt aCellHeight,
                                                const TInt aNaviWidth,
                                                const TInt aColNum,
                                                const TInt aRowNum,
                                                const CFont* aFont = NULL,
                                                MListEventHandler* aHandler = NULL,
                                                const CGraphicsContext::TTextAlign aAlignment = CGraphicsContext::ECenter,
                                                const TInt aTextMargin = 3,
                                                const TRgb aBorderColor = KRgbBlack,
                                                const TRgb aBgColor = TRgb(0xeeeeee),
                                                const TRgb aNavigationBgColor = TRgb(0xeeeeee), 
                                                const TRgb aTextColor = KRgbBlack,
                                                const TRgb aSpecialTextColor = KRgbBlack,
                                                const TRgb aHighLightTextColor = KRgbBlack,
                                                const TRgb aHighLightBgColor = TRgb(0xcccccc),
                                                const TRgb aButtonDownBgColor = KRgbGray);

    /**
     * Size change function
     *
     * @since S60 v3.2
     * @param aCellWidth The width of one basic cell
     * @param aCellHeight The height of one basic cell
     * @param aNaviWidth The width of navidation
     * @param aTopLeftPoint The top left point of the drop down list
     * @param aColNum The number of columns in one row
     * @param aRowNum The number of rows
     * @return None.
     */                                                   
    IMPORT_C void SizeChanged(const TInt aCellWidth, const TInt aCellHeight, 
                               const TInt aNaviWidth, const TPoint aTopLeftPoint = TPoint(0,0),
                               const TInt aColNum = 0, const TInt aRowNum = 0);
    
    /**
     * Set top left point of the list
     *
     * @since S60 v3.2
     * @param aTopLeftPoint The top left point of the drop down list
     * @return None.
     */                                                   
    IMPORT_C void SetTopLeftPoint(const TPoint aTopLeftPoint);
    
    /**
     * Set the width of one basic cell
     *
     * @since S60 v3.2
     * @param aCellWidth The width of one basic cell
     * @return None.
     */                                                   
    IMPORT_C void SetCellWidth(const TInt aCellWidth);

    /**
     * Set the height of one basic cell
     *
     * @since S60 v3.2
     * @param aCellHeight The height of one basic cell
     * @return None.
     */                                                   
    IMPORT_C void SetCellHeight(const TInt aCellHeight);

    /**
     * Set width of navigation 
     *
     * @since S60 v3.2
     * @param aNaviWidth The width of navidation
     * @return None.
     */                                                   
    IMPORT_C void SetNaviWidth(const TInt aNaviWidth);
    IMPORT_C void SetSpinBtnHeight(const TInt aHeight);
    IMPORT_C void SetCellMargin(const TInt aHorizontalMargin, const TInt aVerticalMargin);
   
    /**
     * Set column numbers
     *
     * @since S60 v3.2
     * @param aColNum The number of columns in one row
     * @return None.
     */                                                   
    IMPORT_C void SetColNum(const TInt aColNum);
    
    /**
     * Set or unset highlight for cells.
     *
     * @since S60 v3.2
     * @param aCellIndex Which cell would be set
     * @param aHighlight The flag of highlight, set highlight if ETrue, otherwise cancel highlight
     * @return None.
     */  
    IMPORT_C void SetHighlightCell(const TInt aCellIndex, TBool aHighlight = ETrue,
                                   const TBool aIsDraw = ETrue );

    
    /**
     * Set row numbers
     *
     * @since S60 v3.2
     * @param aRowNum The number of rows
     * @return None.
     */                                                  
    IMPORT_C void SetRowNum(const TInt aRowNum);
    
    /**
     * Set border color
     *
     * @since S60 v3.2
     * @param aBorderColor The border color
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetBorderColor(TRgb aBorderColor, TBool aReDraw = EFalse);

    /**
     * Set background color for the whole area
     *
     * @since S60 v3.2
     * @param aBgColor The background color
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetBgColor(TRgb aBgColor, TBool aReDraw = EFalse);

    /**
     * Set background for navigation area
     *
     * @since S60 v3.2
     * @param aNavigationBgColor The background color of navigation area
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetNavigationBgColor(TRgb aNavigationBgColor, TBool aReDraw = EFalse);

    /**
     * Set text color
     *
     * @since S60 v3.2
     * @param aTextColor The color used to draw text
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetTextColor(TRgb aTextColor, TBool aReDraw = EFalse);

    /**
     * Set color for special text
     *
     * @since S60 v3.2
     * @param aSpecialTextColor The color used to draw special text
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetSpecialTextColor(TRgb aSpecialTextColor, TBool aReDraw = EFalse);

    /**
     * Set color for high light text
     *
     * @since S60 v3.2
     * @param aHighLightTextColor The color used to draw high light text
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetHighLightTextColor(TRgb aHighLightTextColor, TBool aReDraw = EFalse);

    /**
     * Set background color for highlight
     *
     * @since S60 v3.2
     * @param aHighLightBgColor The background color of high light area
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetHighLightBgColor(TRgb aHighLightBgColor, TBool aReDraw = EFalse);
   
    /**
     * The method will be called when there is event from any control except from the list
     *
     * @since S60 v3.2
     * @param aFont The font used to draw text
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetFont(const CFont* aFont, TBool aReDraw = EFalse);

    /**
     * Set text alignment
     *
     * @since S60 v3.2
     * @param aAlignment The alignment used to draw text
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetTextAlighment(const CGraphicsContext::TTextAlign aAlignment, 
                                   TBool aReDraw = EFalse);

    /**
     * Set text margin if alignment is not ECenter
     *
     * @since S60 v3.2
     * @param aTextMargin the margin used to draw text
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetTextMargin(const TInt aTextMargin, TBool aReDraw = EFalse);
    
    /**
     * set secondary list
     *
     * @since S60 v3.2
     * @param aIndex the cell index need to set new color
     * @param aBgColor The background color
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void SetSecondaryList(RArray<TInt>& aIndexList, TBool aReDraw = EFalse);
    
    /**
     * Set event handler when there is out event coming
     *
     * @since S60 v3.2
     * @param aHandler The event handler, which has implemented the interface of MListEventHandler
     * @return None.
     */                                                   
    IMPORT_C void SetListEventHandler(MListEventHandler* aHandler);

    /**
     * Set event id when candidate is selected
     *
     * @since S60 v3.2
     * @param aEventId The event id when candidate is selected
     * @return The CListMultiRowWithIcon pointer.
     */                                                   
    IMPORT_C void SetEventIdForCandidateSelected(TInt aEventId);
    
    /**
     * Set event id when need to get next page candidate
     *
     * @since S60 v3.2
     * @param aEventId The event id need next page candidate
     * @return None
     */
    IMPORT_C void SetEventIdForNextPageCandidate(TInt aEventId);
    
    /**
     * Set event id when need to get previous page candidate
     *
     * @since S60 v3.2
     * @param aEventId The event id need previous page candidate
     * @return None
     */
    IMPORT_C void SetEventIdForPreviousPageCandidate(TInt aEventId);
    
    /**
     * Set event id when need to judge if candidate exist
     *
     * @since S60 v3.2
     * @param aEventId The event id judge if candidate exist
     * @return None
     */
    IMPORT_C void SetEventIdForCandidateExisted(TInt aEventId);
    
    /**
     * Set candidates for the list
     *
     * @since S60 v3.2
     * @param aCandidates the candidates array
     * @param aListType The list type used when redraw
     * @param aFixedText The fixed text(only used in English)
     * @return None.
     */                                                   
    IMPORT_C void SetCandidatesL(const RPointerArray<HBufC>& aCandidates, 
                                 const TListType aListType  = EListExpandable,
                                 const TDesC& aFixedText = KNullDesC16);

    IMPORT_C void SetCandTruncateType(
                                const TCandTruncateType aTruncateType = ECandTruncateFromEnd);
                                 
    /**
     * Add candidates for the list
     *
     * @since S60 v3.2
     * @param aCandidates the candidates array
     * @param aListType The list type used when redraw
     * @return None.
     */
    IMPORT_C void AddCandidatesL(const RPointerArray<HBufC>& aCandidates,
                                 const TListType aListType  = EListExpandable);

    /**
     * Reset and clear all candidates in the list and redraw the list
     *
     * @since S60 v3.2
     * @param aListType The list type used when redraw
     * @return None.
     */                                              
    IMPORT_C void ResetAndClear(const TListType aListType = EListNoExpandable);


    /**
     * Fold the expanding list
     *
     * @since S60 v3.2
     * @return None.
     */                                                   
    IMPORT_C void FoldList();

// from base class CFepUiBaseCtrl  

    /**
     * Draw the list
     *
     * @since S60 v3.2
     * @return None
     */                         
    IMPORT_C void Draw();                                

    /**
     * Active the control
     *
     * @since S60 v3.2
     * @return None
     */                         
    IMPORT_C void SetActive(TBool aActiveFlag);
    
    /**
     * Get rect of the drop down list
     *
     * @since S60 v3.2
     * @return The rect of the drop down list
     */  
    IMPORT_C const TRect& Rect();

    /**
     * Handle pointer down event
     *
     * @since S60 v3.2
     * @param aPoint The point position relative the layout
     */                                        
    IMPORT_C CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * Handle pointer move event
     *
     * @since S60 v3.2
     * @param aPoint The point position relative the layout
     */                  
    IMPORT_C CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * Handle pointer up event
     *
     * @since S60 v3.2
     * @param aPoint The point position relative the layout
     */                  
    IMPORT_C CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);

    /**
     * Handle pointer leave event
     *
     * @since S60 v3.2
     * @param aPoint The point position relative the layout
     */                  
    IMPORT_C void HandlePointerLeave(const TPoint& aPoint);

// from interface MEventObserver  

    /**
     * Handle event 
     * @since Series 60 3.1
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */  
    IMPORT_C void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                     const TDesC& aEventData);

// from base class CFepUiBaseCtrl   
   
    /**
     * Move the drop down list
     * @since Series 60 3.1
     * @param aOffset The offset point
     * @return None
     */     
    IMPORT_C void Move(const TPoint& aOffset);

    /**
     * Hide the control
     * @since Series 60 3.1
     * @return None
     */     
    IMPORT_C void OnDeActivate();  

    /**
     * Hide or show dropdown list.
     *     
     * @param aFlag ETrue if want to hide the control.
     */                                               
    IMPORT_C void Hide( TBool aFlag );      
    
// from interface MContext

    /**
     * Get the candidate count
     *
     * @since S60 v3.2
     * @return the candidate count.
     */                                                   
    IMPORT_C TInt CandidateCount() const;

    /**
     * Enable multi color or not
     *
     * @since S60 v3.2
     * @param aEnable Enable mutil color if ETrue, otherwise disable
     * @param aReDraw Redraw if ETrue, otherwise not redraw
     * @return None.
     */                                                   
    IMPORT_C void EnableSecondary(TBool aEnable, TBool aReDraw = EFalse);

    /**
     * Get the flag of enable or disable multi color
     *
     * @since S60 v3.2
     * @return ETrue if enable, or EFalse.
     */                                                     
    IMPORT_C TBool IsEnableSecondary() const;
    
    /**
     * This function creates drop down list from resource, before calling 
     * this function, caller should set resource id of drop down list first
     * 
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ConstructFromResourceL();

    /**
     * Get secondary list
     *
     * @since S60 v3.2
     * @return color array.
     */                                                   
    const RArray<TInt>& GetSecondaryList() const;

    /**
     * Draw mask for the appointed rect
     * @since Series 60 3.1
     * @param aRect The rect which will be redrawn
     * @return None
     */     
    void DrawMask(TRect aRect);

    /**
     * Active bitmap context
     * @since Series 60 3.1
     * @return None
     */         
    void ActivateBitmapContext();
    
    /**
     * Get the graphics context
     * @since Series 60 3.1
     * @return The graphics context
     */ 
    CFbsBitGc* Gc();

    /**
     * Get the first candidate visible
     *
     * @since S60 v3.2
     * @return The first candidate index.
     */
    TInt GetFirstCandidateIndex();

    /**
     * Get the first candidate visible
     *
     * @since S60 v3.2
     * @return The last candidate index.
     */
    TInt GetLastCandidateIndex();

    /**
     * Set the first candidate visible
     *
     * @since S60 v3.2
     * @return None.
     */
    void SetFirstCandidateIndex(TInt aFirstIndex);

    /**
     * Set the first candidate visible
     *
     * @since S60 v3.2
     * @return None.
     */
    void SetLastCandidateIndex(TInt aLastIndex);
    
    /**
     * Get the appointed candidate
     * @since Series 60 3.1
     * @param aIndex The index which will be check out
     * @return The appointed candidate
     */ 
    CCandidate* GetCandidate(TInt aIndex);
    
    /**
     * Get top left point
     * @since Series 60 3.1
     * @return The top left point
     */ 
    TPoint GetTopLeftPoint();
    
    /**
     * Get cell width
     * @since Series 60 3.1
     * @return The cell width
     */ 
    TInt GetCellWidth();
    
    /**
     * Get cell height
     * @since Series 60 3.1
     * @return The cell height
     */ 
    TInt GetCellHeight();
    
    /**
     * Get navigation width
     * @since Series 60 3.1
     * @return The navigation width
     */ 
    TInt GetNaviWidth();
    
    TInt GetSpinBtnHeight();
    TInt GetCellHorizontalMargin();
    TInt GetCellVerticalMargin();
    TInt ListType();
    TInt OneRowStartCandIdx();
    void SetOneRowStartCandIdx(TInt aOriginalCandIdx);
    TInt GetOriginalFirstCandIdx();
    void SetOriginalFirstCandIdx(TInt aOriginalFirstCandIdx);
    TInt CandTruncateType();
    CList* ActiveList();
    void SetUpdateListReDrawFlag(TBool aReDraw);
    TBool GetUpdateListRedrawFlag();
    IMPORT_C void SetFuzzyBoundry(TInt aFuzzyBoundry);
    TInt FuzzyBoundry();
    TInt CandExpandType();
    IMPORT_C void SetCandExpandType(const TExpandType aType = ECandExpandDownward);
    TInt CandHDirection();
    inline void SetHDirection(const THDirection aDirection = ECandFromLToR);

    // when drop down list is expandable, decide pageup/pagedown shown or not 
    TBool PageCtrlShown();
    inline void SetPageCtrlShown(TBool aShown = ETrue);

    /**
     * Get columns number
     * @since Series 60 3.1
     * @return The top left point
     */ 
    TInt GetColNum();

    /**
     * Get rows number
     * @since Series 60 3.1
     * @return The top left point
     */ 
    TInt GetRowNum();

    /**
     * Get the font used to draw text
     * @since Series 60 3.1
     * @return The font used to draw text
     */ 
    const CFont* GetFont();
    
    /**
     * Get alignment used to draw the text
     * @since Series 60 3.1
     * @return The alignment used to draw the text
     */ 
    CGraphicsContext::TTextAlign GetAlignment();

    /**
     * Get text margin used to draw the text
     * @since S60 v3.2
     * @return The margin used to draw the text
     */ 
    TInt GetTextMargin();
        
    /**
     * Get background color
     * @since Series 60 3.1
     * @return The background color
     */ 
    TRgb GetBgColor();

    /**
     * Get button down color
     * @since Series 60 3.1
     * @return The background color
     */ 
    TRgb GetButtonDownBgColor();    
        
    /**
     * Get border color
     * @since Series 60 3.1
     * @return The border color
     */ 
    TRgb GetBorderColor();
    
    /**
     * Get text color
     * @since Series 60 3.1
     * @return The text color
     */ 
    TRgb GetTextColor();    
        
    /**
     * Get background color for highlight area
     * @since Series 60 3.1
     * @return The background color for highlight area
     */ 
    TRgb GetHighLightBgColor();
    
    /**
     * Get high light text color
     * @since Series 60 3.1
     * @return The high light text color
     */ 
    TRgb GetHighLightTextColor();
    
    /**
     * Get special text color
     * @since Series 60 3.1
     * @return The special text color
     */ 
    TRgb GetSpecialTextColor();
    
    /**
     * Get fixed text
     * @since Series 60 3.1
     * @return The fixed text
     */ 
    TDesC GetFixedText();

    /**
     * Get highlight cell list
     * @since Series 60 3.1
     * @return The highlight cell list
     */
    const RArray<TInt>& GetHighlightList() const;

    /**
     * Get page manager
     * @since Series 60 3.1
     * @return The page manager
     */
    CPages* GetPages();

    /**
     * Get bitmap manager
     * @since Series 60 3.1
     * @return The bitmap manager
     */    
    CBitmapDb* GetBitmapDb();
        
    /**
     * Report event when candidate is selected
     * @since Series 60 3.1
     * @param aCandidate The candidate string
     * @param aSelectedIndex The index of the selected candidate
     * @return None
     */
    void ReportCandidateSelectEventL(const TDesC& aCandidate, const TInt aSelectedIndex);
    
    /**
     * Invalidate some rect and redraw it
     * @since Series 60 3.1
     * @param aRect The rect which will be redrawn
     * @param aUpdateAll Update all or not
     * @return None
     */
    void Invalidate(const TRect& aRect,TBool aUpdateAll = EFalse); 

    /**
     * Change active list
     *
     * @since S60 v3.2
     * @param aList The new active list
     * @param aLastIndex The last visible candidate index  
     * @return None.
     */
    void ChangeActiveList(CList* aList, TInt aLastIndex = 0); 
    
    /**
     * Get pen size
     *
     * @since S60 v3.2
     * @return The pen size.
     */
    const TSize GetPenSize();
                                                                  
    /**
     * Set pointer capture
     *
     * @since S60 v3.2
     * @param aFlag ETrue to capture pointer, otherwise EFalse
     * @return None.
     */
    void SetCapture(TBool aFlag);
    
    /**
     * Notify get next candidate page
     *
     * @since S60 v3.2
     * @return None.
     */
    void NotifyGetNextPage();
    
    /**
     * Notify get previous candidate page
     *
     * @since S60 v3.2
     * @return None.
     */
    void NotifyGetPreviousPage();
    
    /**
     * Notify close candidate page
     *
     * @since S60 v3.2
     * @return None.
     */
    void NotifyClosePage();    
    
    /**
     * Set flag of if next candidate after last page eixsted
     *
     * @since S60 v3.2
     * @param aFlag ETrue if candidate exist, otherwise EFalse
     * @return None
     */
    IMPORT_C void SetFlagCandidateExist(TBool aFlag);

    /**
     * Judge if next candidate existed
     *
     * @since S60 v3.2
     * @return ETrue if candidate exist, otherwise EFalse
     */
    TBool NextCandidateExist();

    /**
     * Auto change active list
     *
     * @since S60 v3.2
     * @param aLastIndex The last visible candidate index  
     * @return None.
     */
    void AutoChangeActiveList( TInt aLastIndex = 0 ); 
    
    /**
     * Set dropdown list image ID
     *
     * @since S60 v3.2
     * @param TDropdownListDrawInfo image information
     * @return None.
     */
	IMPORT_C void SetDropdownListImgID(TDropdownListDrawInfo aDropdownListDrawInfo);
	
    /**
     * Show preview bubble
     *
     * @since S60 v3.2
     * @param aFlag Show/Hide flag
     * @return None.
     */	
	IMPORT_C void ShowBubble(TBool aFlag);
	
    /**
     * Set bubble bitmap param
     *
     * @since S60 v3.2
     * @param aBmpId 
     * @param aMaskBmpId
     * @param aBgSkinId
     * @return None.
     */		
	IMPORT_C void SetBubbleBitmapParam(CFbsBitmap* aBmpId,
                                       CFbsBitmap* aMaskBmpId ,
                                       TAknsItemID aBgSkinId );
    
    /**
     * Set bubble text font
     *
     * @since S60 v3.2
     * @param aFont Text font
     * @return None.
     */		                                   
    IMPORT_C void SetBubbleTextFont(const CFont* aFont);                                  
                                       
    /**
     * Set bubble text format
     *
     * @since S60 v3.2
     * @param aTextFormat Text format
     * @return None.
     */		                                   	
	IMPORT_C void SetTextFormat(TAknTextLineLayout aTextFormat);
		
    /**
     * Set bubble text format
     *
     * @since S60 v3.2
     * @param aLeftDiff
     * @param aTopDiff
     * @param aRightDiff
     * @param aBottomDiff
     * @return None.
     */		                                   	
	IMPORT_C void SetFrameDiff(TInt aLeftDiff, TInt aTopDiff,
                               TInt aRightDiff,TInt aBottomDiff);
	
    /**
     * Set bubble size
     *
     * @since S60 v3.2
     * @param aSize
     * @return None.
     */		                                   		
	IMPORT_C void SetBubbleSize(const TSize& aSize);
	
    /**
     * show preview bubble
     *
     * @since S60 v3.2
     * @param aRect
     * @param aText
     * @return None.
     */		                                   		
	void DrawBubble(const TRect aRect, const TDesC& aText);
	
    /**
     * Hide preview bubble
     *
     * @since S60 v3.2
     * @return None.
     */		                                   		
	void ClearBubble();

    /**
     * ReDraw preview bubble
     *
     * @since S60 v3.2
     * @return None.
     */		                                   		
	void ReDrawBubble();
	
	CFepUiLayout* UiLayout();

    /**
     * Redraw dropdown list
     *
     * @since S60 v3.2
     * @param aRect Redraw rect.
     * @return None.
     */		                                   		
    IMPORT_C void ReDrawRect(const TRect& aRect);
protected:    

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aTopLeftPoint The top left point of the drop down list
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aReader The reader from which the bitmaps ID can be read
     * @param aCellWidth The width of one basic cell
     * @param aCellHeight The height of one basic cell
     * @param aNaviWidth The width of navidation
     * @param aColNum The number of columns in one row
     * @param aRowNum The number of rows
     * @param aFont The font used to draw the text
     * @param aHandler The handle which will deal with outer event to this control
     * @param aAlignment The alignment used to draw text
     * @param aTextMargin if alignment is not ECenter, the margin is used 
     * @param aBorderColor The color of border
     * @param aBgColor The color of background
     * @param aNavigationBgColor The background color of navigation area
     * @param aTextColor The color of text
     * @param aSpecialTextColor The color of special text
     * @param aHighLightTextColor The color of highlight text
     * @param aHighLightBgColor The background color of high light
     * @param aButtonDownBgColor The background color when button pressing
     */
    CFepCtrlDropdownList(const TPoint& aTopLeftPoint,
                         CFepUiLayout* aUiLayout,
                         TInt aControlId,
                         const TInt aCellWidth,
                         const TInt aCellHeight,
                         const TInt aNaviWidth,
                         const TInt aColNum,
                         const TInt aRowNum,
                         const CFont* aFont,
                         MListEventHandler* aHandler,                               
                         const CGraphicsContext::TTextAlign aAlignment,
                         const TInt aTextMargin,
                         const TRgb aBorderColor,
                         const TRgb aBgColor,
                         const TRgb aNavigationBgColor, 
                         const TRgb aTextColor,
                         const TRgb aSpecialTextColor,
                         const TRgb aHighLightTextColor,
                         const TRgb aHighLighttBgColor,
                         const TRgb aButtonDownBgColor);
                         
    CFepCtrlDropdownList(CFepUiLayout* aUiLayout,
                         TInt aControlId,
                         TPoint aTopLeftPoint,
                         TSize aCellSize,
                         TInt aColNum,
                         TInt aRowNum);
                         
    CFepCtrlDropdownList(CFepUiLayout* aUiLayout,
                         TInt aControlId,
                         TInt aResId,
                         TPoint aTopLeftPoint,
                         const CFont* aFont,
                         TSize aCellSize,
                         TInt aNaviWidth,
                         TInt aColNum,
                         TInt aRowNum,
                         TInt aSpinBtnHeight,
                         TInt aHorizontalCellMargin,
                         TInt aVerticalCellMargin,
                                               THDirection aDirection,
                                               TBool aPageShown);
                         

        
    /**
     * 2nd construct function
     * @since Series 60 3.1
     * @param aTopLeftPoint The top left point
     * @param aReader The reader from which the bitmaps ID can be read
     * @return None
     */ 
    void ConstructL(const TPoint& aTopLeftPoint, TResourceReader& aReader);

    void ConstructL(TPoint aTopLeftPoint, TInt aResId);

private:
    /**
     * Move point to center of respone area if tap accuracy adjustment is activated
     * 
     * 
     * @since Symbian^3
     * @param aPoint point to be adjusted
     * @return None
     */
    void AdjustPointerPosition( TPoint& aPoint );
    
private:

    /**
     * The list manager
     * Not own
     */
    CListManager* iListManager;
        
    /**
     * The bitmap database
     * Own
     */
    CBitmapDb* iBitmapDb;

    /**
     * The pages manager
     * Own
     */
    CPages* iPages;

    /**
     * The candidate list
     * Own
     */
    RPointerArray<CCandidate> iCandidatelist;
    
    /**
     * The candidate color list
     * Own
     */
    RArray<TInt> iSecondaryList;

    /**
     * The high light list
     * Own
     */
    RArray<TInt> iHighlightList;

    /**
     * The flag of multi color enable or disable
     */
    TBool iEnableSecondary;
      
    /**
     * The first candidate index in the list
     */
    TInt iFirstCandidateIndex;

    /**
     * The last candidate index in the list
     */
    TInt iLastCandidateIndex;

    /**
     * The font used to draw the text
     * Own
     */
    const CFont* iFont;
    
    const CFont* iBubbleFont;

    /**
     * Cell width
     */
    TInt iCellWidth;

    /**
     * Cell height
     */
    TInt iCellHeight;

    /**
     * navigation width
     */
    TInt iNaviWidth;
    
    TInt iSpinBtnHeight;
    TInt iCellHorizontalMargin;
    TInt iCellVerticalMargin;
    TInt iOriginalCandIdx;
    TInt iOriginalFirstCandIdx;
    TBool iUpdateListReDraw;

    /**
     * alignment used to draw the text
     */
    CGraphicsContext::TTextAlign iAlignment;

    /**
     * the margin used to draw the text if alignment is not ECenter
     */
    TInt iTextMargin;

    /**
     * The columns number
     */
    TInt iColNum;

    /**
     * The rows number
     */
    TInt iRowNum;

    /**
     * The border color
     */
    TRgb iBorderColor;

    /**
     * The background color of the whole list
     */
    TRgb iBgColor;

    /**
     * The navigation background color
     */
    TRgb iNavigationBgColor;

    /**
     * The text color
     */
    TRgb iTextColor;

    /**
     * The special text color
     */
    TRgb iSpecialTextColor;

    /**
     * The highlight text color
     */
    TRgb iHighLightTextColor;

    /**
     * The background color of highlight area 
     */
    TRgb iHighLightBgColor;

    /**
     * The background color when button down
     */
    TRgb iButtonDownBgColor;

    /**
     * The top left point the list
     */
    TPoint iTopLeftPoint;

    /**
     * The fixed text
     */
    TBuf<KMaxFixedTextNumber> iFixedText;
    
    /**
     * The event ID for candidate selected
     */
    TInt iEventIdForCandidateSelected;
    
    /**
     * The event ID for next page candidate
     */
    TInt iEventIdForNextPageCandidate;
    
    /**
     * The event ID for previous page candidate
     */
    TInt iEventIdForPreviousPageCandidate;
    
    /**
     * The event ID for judge if candidate existed
     */
    TInt iEventIdForCandidateExisted;

    /**
     * The event handler when out event coming
     * Not own
     */
    MListEventHandler* iListEventHandler;

    /**
     * The first time construct flag of drop down list
     */
    TBool iFirstTimeConstruct;
    
    /**
     * The flag to indicate that candidate existed or not at given position
     */
    TBool iCandidateExisted;
    
    /**
     * The type of drop down list.
     */
    TListType iListType;

    TCandTruncateType iCandTruncateType;

    TInt iFuzzyBoundry;
    
    TExpandType iCandExpandType;
    
    THDirection iCandHDirection;
    
    TBool iPageCtrlShown;
    
    CBubbleCtrl* iBubbleCtrl;
    
    TBool iIsShowBubble;
    
    TSize iBubbleSize;
    
    TInt iBubbleVerticalMargin;
    
    /**
     * activity flag for extra response area
     */
    TBool iExtResponseAreaEnabled;
    
    /**
     * margin size of extra response area 
     */
    TInt  iExtResponseAreaSize;
    };    
        
#endif // C_CFEPCTRLDROPDOWNLIST_H

inline void CFepCtrlDropdownList::SetHDirection(const THDirection aDirection)
    {
    iCandHDirection = aDirection;
    }

inline void CFepCtrlDropdownList::SetPageCtrlShown(TBool aShown)
    {
    iPageCtrlShown = aShown;
    }
// End Of File
