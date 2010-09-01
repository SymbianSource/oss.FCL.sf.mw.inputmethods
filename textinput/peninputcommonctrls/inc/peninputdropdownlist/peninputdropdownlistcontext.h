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
* Description:  context for list
*
*/


#ifndef M_DROP_DOWN_LIST_CONTEXT_H
#define M_DROP_DOWN_LIST_CONTEXT_H

#include <e32base.h>
#include <w32std.h>
#include <AknsDrawUtils.h> 

class CCandidate;
class CPages;
class CBitmapDb;
class CList;

struct TDropdownListDrawInfo
    {
    TAknsItemID iInactiveBtnImgID;
    TAknsItemID iNormalBtnImgID;
    TAknsItemID iPressedBtnImgID;

    TAknsItemID iCandidateLeftImgID;
    TAknsItemID iCandidateMiddleImgID;
    TAknsItemID iCandidateRightImgID;
    
    TAknsItemID iCandidatePressedLeftImgID;
    TAknsItemID iCandidatePressedMiddleImgID;
    TAknsItemID iCandidatePressedRightImgID;
    
    TRgb iCandidatesHighlightColor;
    
    TBool iSupportSkin;
    
    TDropdownListDrawInfo(TAknsItemID aInactiveBtnImgID,
                          TAknsItemID aNormalBtnImgID,
                          TAknsItemID aPressedBtnImgID,
                          
                          TAknsItemID aCandidateLeftImgID,
                          TAknsItemID aCandidateMiddleImgID,
                          TAknsItemID aCandidateRightImgID,
                      
                          TAknsItemID aCandidatePressedLeftImgID,
                          TAknsItemID aCandidatePressedMiddleImgID,
                          TAknsItemID aCandidatePressedRightImgID,

                          TRgb aCandidatesHighlightColor,
                          TBool aSupportSkin = EFalse)
        {
        
        iInactiveBtnImgID = aInactiveBtnImgID;
        iNormalBtnImgID = aNormalBtnImgID;
        iPressedBtnImgID = aPressedBtnImgID;
            
        iCandidateLeftImgID = aCandidateLeftImgID;
        iCandidateMiddleImgID = aCandidateMiddleImgID;
        iCandidateRightImgID = aCandidateRightImgID;

        iCandidatePressedLeftImgID = aCandidatePressedLeftImgID;
        iCandidatePressedMiddleImgID = aCandidatePressedMiddleImgID;
        iCandidatePressedRightImgID = aCandidatePressedRightImgID;      
        
        iCandidatesHighlightColor = aCandidatesHighlightColor;
        
        iSupportSkin = aSupportSkin;
            
        };
        
    TDropdownListDrawInfo()
        {
        iSupportSkin = EFalse;
        };
    };    



/**
 *  Context for list
 *
 *  it provide all functions used by list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */
class MFepCtrlDropdownListContext
    {
public:  // Method

    /**
     * Draw mask for the appointed rect
     * @since S60 v3.2
     * @param aRect the rect which will be redrawn
     * @return None
     */     
    virtual void DrawMask(TRect aRect) = 0;

    /**
     * Active bitmap context
     * @since S60 v3.2
     * @return None
     */         
    virtual void ActivateBitmapContext() = 0;
    
    /**
     * Get the graphics context
     * @since S60 v3.2
     * @return The graphics context
     */ 
    virtual CFbsBitGc* Gc() = 0;

    /**
     * Get the candidate count
     *
     * @since S60 v3.2
     * @return The candidate count.
     */      
    virtual TInt CandidateCount() const = 0;

    /**
     * Get the first candidate visible
     *
     * @since S60 v3.2
     * @return The first candidate index.
     */
    virtual TInt GetFirstCandidateIndex() = 0;

    /**
     * Get the first candidate visible
     *
     * @since S60 v3.2
     * @return The last candidate index.
     */
    virtual TInt GetLastCandidateIndex() = 0;

    /**
     * Set the first candidate visible
     *
     * @since S60 v3.2
     * @param aFirstIndex the first candidate index
     * @return None.
     */
    virtual void SetFirstCandidateIndex(TInt aFirstIndex) = 0;

    /**
     * Set the first candidate visible
     *
     * @since S60 v3.2
     * @param aLastIndex the last candidate index
     * @return None.
     */
    virtual void SetLastCandidateIndex(TInt aLastIndex) = 0;

    /**
     * Get the appointed candidate
     * @since S60 v3.2
     * @param aIndex The index which will be check out
     * @return The appointed candidate
     */ 
    virtual CCandidate* GetCandidate(TInt aIndex) = 0;
    
    /**
     * Get top left point
     * @since S60 v3.2
     * @return The top left point
     */ 
    virtual TPoint GetTopLeftPoint() = 0;
    
    /**
     * Get cell width
     * @since S60 v3.2
     * @return Cell width
     */ 
    virtual TInt GetCellWidth() = 0;
    
    /**
     * Get cell height
     * @since S60 v3.2
     * @return Cell height
     */ 
    virtual TInt GetCellHeight() = 0;
    
    /**
     * Get navigation width
     * @since S60 v3.2
     * @return Navigation width
     */ 
    virtual TInt GetNaviWidth() = 0;
    virtual TInt GetSpinBtnHeight() = 0;
    virtual TInt GetCellHorizontalMargin() = 0;
    virtual TInt GetCellVerticalMargin() = 0;
    virtual TInt ListType() = 0;
    virtual TInt OneRowStartCandIdx() = 0;
    virtual void SetOneRowStartCandIdx(TInt aOriginalCandIdx) = 0;
    virtual TInt GetOriginalFirstCandIdx() = 0;
    virtual void SetOriginalFirstCandIdx(TInt aOriginalFirstCandIdx) = 0;
    virtual TInt CandTruncateType() = 0;
    virtual CList* ActiveList() = 0;
    virtual void SetUpdateListReDrawFlag(TBool aReDraw) = 0;
    virtual TBool GetUpdateListRedrawFlag() = 0;
    // if set: when list expanded, and user press on region between
    // list's rect and outerrect (list's rect plus fuzzy boundry)
    // it still think that press inside list
    virtual TInt FuzzyBoundry() = 0;
    virtual TInt CandExpandType() = 0;
    virtual TInt CandHDirection() = 0;
    virtual TBool PageCtrlShown() = 0;

    /**
     * Get columns number
     * @since S60 v3.2
     * @return The top left point
     */ 
    virtual TInt GetColNum() = 0;

    /**
     * Get rows number
     * @since S60 v3.2
     * @return The top left point
     */ 
    virtual TInt GetRowNum() = 0;

    /**
     * Get the font used to draw text
     * @since S60 v3.2
     * @return The font used to draw text
     */ 
    virtual const CFont* GetFont() = 0;
    
    /**
     * Get alignment used to draw the text
     * @since S60 v3.2
     * @return The alignment used to draw the text
     */ 
    virtual CGraphicsContext::TTextAlign GetAlignment() = 0;
    
    /**
     * Get text margin used to draw the text
     * @since S60 v3.2
     * @return The margin used to draw the text
     */ 
    virtual TInt GetTextMargin() = 0;

    /**
     * Get background color
     * @since S60 v3.2
     * @return The background color
     */ 
    virtual TRgb GetBgColor() = 0;
    
    /**
     * Get button down color
     * @since S60 v3.2
     * @return The background color
     */ 
    virtual TRgb GetButtonDownBgColor() = 0;    
    
    /**
     * Get border color
     * @since S60 v3.2
     * @return The border color
     */ 
    virtual TRgb GetBorderColor() = 0;

    /**
     * Get text color
     * @since S60 v3.2
     * @return The text color
     */ 
    virtual TRgb GetTextColor() = 0;    
    
    /**
     * Get background color for highlight area
     * @since S60 v3.2
     * @return The background color for highlight area
     */ 
    virtual TRgb GetHighLightBgColor() = 0;
    
    /**
     * Get high light text color
     * @since S60 v3.2
     * @return The high light text color
     */ 
    virtual TRgb GetHighLightTextColor() = 0;
    
    /**
     * Get special text color
     * @since S60 v3.2
     * @return The special text color
     */ 
    virtual TRgb GetSpecialTextColor() = 0;
    
    /**
     * Get fixed text
     * @since S60 v3.2
     * @return The fixed text
     */ 
    virtual TDesC GetFixedText() = 0;
    
    /**
     * Invalidate some rect and redraw it
     * @since S60 v3.2
     * @param aRect The rect which will be redrawn
     * @param aUpdateAll Update all or not
     * @return None
     */
    virtual void Invalidate(const TRect& aRect, TBool aUpdateAll = EFalse) = 0;   

    /**
     * Get secondary list
     * @since S60 v3.2
     * @return The secondary list
     */     
    virtual const RArray<TInt>& GetSecondaryList() const = 0;

    /**
     * Get the flag of enable or disable multi color
     *
     * @since S60 v3.2
     * @return ETrue if enable, or EFalse.
     */     
    virtual TBool IsEnableSecondary() const= 0;

    /**
     * Get highlight cell list
     * @since S60 v3.2
     * @return The highlight cell list
     */    
    virtual const RArray<TInt>& GetHighlightList() const= 0;

    /**
     * Report event when candidate is selected
     * @since S60 v3.2
     * @param aCandidate The candidate string
     * @param aSelectedIndex The index of the selected candidate
     * @return None.
     */
    virtual void ReportCandidateSelectEventL(const TDesC& aCandidate, const TInt aSelectedIndex)= 0;
    

    /**
     * Get page manager
     * @since S60 v3.2
     * @return page manager
     */
    virtual CPages* GetPages() = 0;

    /**
     * Get bitmap manager
     * @since S60 v3.2
     * @return The bitmap manager
     */    
    virtual CBitmapDb* GetBitmapDb() = 0;

    /**
     * Change active list
     *
     * @since S60 v3.2
     * @param aList The new active list
     * @param aLastIndex The last visible candidate index  
     * @return None.
     */
    virtual void ChangeActiveList(CList* aList, TInt aLastIndex = 0) = 0; 
 
     /**
     * Get pen size
     *
     * @since S60 v3.2
     * @return The pen size.
     */
    virtual const TSize GetPenSize() = 0;
    
    /**
     * Set pointer capture
     *
     * @since S60 v3.2
     * @param aFlag ETrue to capture pointer, otherwise EFalse
     * @return None.
     */
    virtual void SetCapture(TBool aFlag ) = 0;
    
    /**
     * Get next candidate page
     *
     * @since S60 v3.2
     * @return None.
     */
    virtual void NotifyGetNextPage() = 0;
    
    /**
     * Get previous candidate page
     *
     * @since S60 v3.2
     * @return None.
     */
    virtual void NotifyGetPreviousPage() = 0;
    
    /**
     * Notify close candidate page
     *
     * @since S60 v3.2
     * @return None.
     */
    virtual void NotifyClosePage() = 0;
    
    /**
     * Judge if next candidate existed
     *
     * @since S60 v3.2
     * @return ETrue if candidate exist, otherwise EFalse
     */
    virtual TBool NextCandidateExist() = 0;

    /**
     * Auto change active list
     *
     * @since S60 v3.2
     * @param aLastIndex The last visible candidate index  
     * @return None.
     */
    virtual void AutoChangeActiveList( TInt aLastIndex = 0 ) = 0; 
    
    /**
     * Show preview bubble
     *
     * @since S60 v3.2
     * @param aRect Bubble rect
     * @param aText Review bubble text  
     * @return None.
     */
    virtual void DrawBubble(const TRect aRect, const TDesC& aText) = 0;
    
    /**
     * Hide preview bubble
     *
     * @since S60 v3.2
     * @return None.
     */    
    virtual void ClearBubble() = 0;

    /**
     * Redraw preview bubble
     *
     * @since S60 v3.2
     * @return None.
     */    
    virtual void ReDrawBubble() = 0;
    };

#endif // M_DROP_DOWN_LIST_CONTEXT_H

// End Of File
