/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Header of LAF manager.
*
*/


#ifndef C_PENINPUTFINGERHWRARLAFMANAGER_H
#define C_PENINPUTFINGERHWRARLAFMANAGER_H

/**
* Class  PeninputFingerHwrArLafManager
*  
* Wrapper of L&F data.
*
* @lib peninputfingerhwrar.lib
* @since Symbian TB9.2
*/
class CPeninputFingerHwrArLafManager : public CBase 
    {
public:
    /**
     * Symbian constructor.
     *
     * @since Symbian TB9.2
     * @return Pointer to created object.
     */    
    static CPeninputFingerHwrArLafManager* NewL();
    
    /**
     * Symbian constructor.
     *
     * @since Symbian TB9.2
     * @return Pointer to created object.
     */  
    static CPeninputFingerHwrArLafManager* NewLC();
    
    /**
     * standard c++ destructor.
     *
     * @since Symbian TB9.2
     * @return None
     */    
    ~CPeninputFingerHwrArLafManager();
   
public:
    /**
     * read laf data.
     *
     * @since Symbian TB9.2
     * @return None
     */    
    void ReadLafInfo();
    
    /**
     * get screen orientation.
     * 
     * @since Symbian TB9.2
     * @return ETrue if is landscape.
     */
    TBool IsLandscape();

    /**
     * get rect of layout.
     * 
     * @since Symbian TB9.2
     * @return rect of layout.
     */    
    TRect LayoutRect();
    
    /**
     * get rect of specified control.
     * 
     * @since Symbian TB9.2
     * @param aCtrlId specifies control id.
     * @return rect of specified control.
     */
    TRect CtrlRect( TInt aCtrlId );
    
public: // icf
    /**
     * get rect of ICF control.
     * 
     * @since Symbian TB9.2
     * @return rect of icf.
     */      
    TRect IcfRect();
    
    /**
     * get left margin of icf.
     * 
     * @since Symbian TB9.2
     * @return left margin of icf.
     */
    TInt IcfLeftMargin();
    
    /**
     * get Right margin of icf.
     * 
     * @since Symbian TB9.2
     * @return Right margin of icf.
     */    
    TInt IcfRightMargin();
    
    /**
     * get top margin of icf.
     * 
     * @since Symbian TB9.2
     * @return top margin of icf.
     */    
    TInt IcfTopMargin();
    
    /**
     * get bottom margin of icf.
     * 
     * @since Symbian TB9.2
     * @return bottom margin of icf.
     */    
    TInt IcfBottomMargin();
    
    /**
     * get line spacing of icf.
     * 
     * @since Symbian TB9.2
     * @return line spacing of icf.
     */      
    TInt IcfLineSpaceMargin();
  
    /**
     * get font of icf.
     * 
     * @since Symbian TB9.2
     * @return font of icf.
     */       
    CFont* IcfFont();
    
    /**
     * get text height of icf.
     * 
     * @since Symbian TB9.2
     * @return text height of icf.
     */       
    TInt IcfTextHeight();
    
public: //buttons
    
    /**
     * get inner padding of buttons.
     * 
     * @since Symbian TB9.2
     * @return inner padding of buttons.
     */      
    TSize ButtonInnerPadding();
    
public: //symbol table
  
    /**
     * get row count of virtual sctpad.
     * 
     * @since Symbian TB9.2
     * @return row count of virtual sctpad.
     */      
    TInt VirtualSctpadRowCount();
    
    /**
     * get column count of virtual sctpad.
     * 
     * @since Symbian TB9.2
     * @return column count of virtual sctpad.
     */        
    TInt VirtualSctpadColCount();
    
    /**
     * get row count of virtual number sctpad.
     * 
     * @since Symbian TB9.2
     * @return row count of virtual number sctpad.
     */      
    TInt VirtualNumSctpadRowCount();
    
    /**
     * get column count of virtual number sctpad.
     * 
     * @since Symbian TB9.2
     * @return column count of virtual number sctpad.
     */        
    TInt VirtualNumSctpadColCount();
    /**
     * get cell size of virtual sctpad.
     * 
     * @since Symbian TB9.2
     * @return cell size of virtual sctpad.
     */     
    TRect VirtualSctpadCellSize();
    
    /**
     * get cell rect of virtual sctpad.
     * 
     * @since Symbian TB9.2
     * @param aArrayRects a TRect array to receive cell rect.
     * @return none.
     */     
    void GetVirtualKeyRects( RArray<TRect>& aArrayRects );
    
    /**
     * get cell rect of virtual sctpad.
     * 
     * @since Symbian TB9.2
     * @param aArrayRects a TRect array to receive cell rect.
     * @return none.
     */     
    void GetNumVirtualKeyRects( RArray<TRect>& aArrayRects );
    
    /**
     * get text layout of virtual sctpad.
     * 
     * @since Symbian TB9.2
     * @return a TAknTextLineLayout object to layout cell text.
     */    
    TAknTextLineLayout SctpadKeyTxtLayout();
    
    /**
     * get text layout of virtual numpad.
     * 
     * @since Symbian TB9.2
     * @return a TAknTextLineLayout object to layout cell text.
     */    
    TAknTextLineLayout NumpadKeyTxtLayout();

    /**
     * get preview bubble rect.
     * 
     * @since Symbian TB9.2
     * @return a TRect object to preview bubble rect.
     */    
    TRect PreviewBubbleRect();
    
    /**
     * get preview bubble inner rect.
     * 
     * @since Symbian TB9.2
     * @return a TRect object to preview bubble inner rect.
     */    
    TRect PreviewBubbleInnerRect();
    
    /**
     * get preview bubble font.
     * 
     * @since Symbian TB9.2
     * @return a CFont pointer to preview bubble font.
     */    
    const CFont* PreviewBubbleFont();
    
    /**
     * get preview bubble text layout.
     * 
     * @since Symbian TB9.2
     * @return a TAknTextLineLayout object to preview bubble text layout.
     */    
    TAknTextLineLayout PreviewBubbleTextLayout();
    
public: // candidate list
    
    /**
     * get width unit of candidate list.
     * 
     * @since Symbian TB9.2
     * @return width unit of candidate list.
     */      
    TInt CandidateUnitWidth();
    
    /**
     * get height unit of candidate list.
     * 
     * @since Symbian TB9.2
     * @return height unit of candidate list.
     */ 
    TInt CandidateUnitHeight();  

    /**
     * get horizontal margin of candidate list.
     * 
     * @since Symbian TB9.2
     * @return horizontal margin of candidate list.
     */    
    TInt CandidateHorizontalMargin();

    /**
     * get vertical margin of candidate list.
     * 
     * @since Symbian TB9.2
     * @return vertical margin of candidate list.
     */      
    TInt CandidateVerticalMargin();

    /**
     * get top-left position of candidate list.
     * 
     * @since Symbian TB9.2
     * @return top-left position of candidate list.
     */     
    TPoint CandidateLTPos();

    /**
     * get top-left position of predictive list.
     * 
     * @since Symbian TB9.2
     * @return top-left position of predictive list.
     */     
    TPoint PredictiveLTPos();
    
    /**
     * get font of candidate list.
     * 
     * @since Symbian TB9.2
     * @return font of candidate list.
     */     
    const CFont* CandidateFont();
	
	/**
	 * Get indicator rect
	 */
    TRect GetIndicatorRect();
	
	/**
	 * Get arrow padding size
	 */
	TSize GetArrowPaddingSize();
public: //sct paging buttons
    /**
     * get text layout of sct paging button.
     * 
     * @since Symbian TB9.2
     * @return a TAknTextLineLayout object to layout text.
     */    
    TAknTextLineLayout SctPageBtnTxtLayout();
	
	/**
	 * Get buttons rects for symbol table
	 */
	RArray<TRect>& GetSymBtnArray();
	
	/**
	 * Get virtual key pad rect
	 */
	TRect GetVirtualKeyRect(); 
    /**
     * Get virtual number key pad rect
     */
    TRect GetVirtualNumKeyRect(); 
private:
    
    /**
     * C++ constructor
     *
     * @since Symbian TB9.2
     * @return None
     */    
    CPeninputFingerHwrArLafManager();
    
    /**
     * Symbian second-phase constructor
     *
     * @since Symbian TB9.2
     * @return None
     */    
    void ConstructL();    

private:
    /**
	 * Retrieve layout data
	 */
	void RetrieveLayoutData();
	
    /**
	 * Retrieve the laf data for ICF editor
	 */
	void RetrieveLafDataForICF();
	
	/**
	 * Retrieve the laf data for function button: 
	 * Close, optons, symbol, delete, and arrows buttons.
	 */
	void RetrieveLafDataForFunctionalButton();
    
	/**
	 * Retrieve the laf data for candidate list
	 */
	void RetrieveLafDataForCandidateList();
	
    /**
	 * Retrieve the laf data for symbol table
	 */
    void RetrieveLafDataForSymbolTable();
	
    /**
     * Retrieve the laf data for symbol table
     */
    void RetrieveLafDataForNumSymbolTable();

    /**
	 * Retrieve the landscape laf data for symbol table
	 */
	void RetrieveLandscapeLafDataForSymbolTable();
	
	/**
	 * Retrieve the portrait laf data for symbol table
	 */
	void RetrievePortraitLafDataForSymbolTable();

    /**
     * Retrieve the landscape laf data for number symbol table
     */
    void RetrieveLandscapeLafDataForNumSymbolTable();
    
    /**
     * Retrieve the portrait laf data for number symbol table
     */
    void RetrievePortraitLafDataForNumSymbolTable();

    /**
	 * Retrieve the laf data for HWR writing box
	 */
    void RetrieveLafDataForHwrBox();	 
    
	 /**
	 * Retrieve the laf data for Candidate Preview bubble
	 */
    void RetrieveLafDataForPreviewBubble();
	
private: //datas
    
    TBool iIsLandscape;
    TRect iLayoutRect;
    TPoint iLayoutOffset;
	TSize iScreenSize;
    /**
	 * control rects
	 *
	 */
    TRect iRectIcf;
    TRect iRectWritingBox;
    TRect iRectNumpad;
    TRect iRectSctpad;
    TRect iRectBtnSpace;
    TRect iRectBtnEnter;
    TRect iIndicatorRect;
	
    /**
     * Functinal button laf
     */	 
    TRect iRectBtnClose;
    TRect iRectBtnRangeSmb;
    TRect iRectBtnBackspace;
    TRect iRectBtnArrowLeft;
    TRect iRectBtnArrowRight;
    TRect iRectBtnArrowUp;
    TRect iRectBtnArrowDown;
    TRect iRectBtnOption;
    TRect iRectBtnSctPage;
    TRect iRectBtnNumType;
    TSize iArrowPaddingSize;
    TSize iSizeBtnPadding;

	/**
	 * ICF laf
	 */
	TInt iIcfTextLeftMargin;
	TInt iIcfTextRightMargin;
	TInt iIcfTextTopMargin;
	TInt iIcfTextBottomMargin;
	TInt iIcfTextLineSpaceMargin;
	TInt iIcfTextHeight;
    TInt iIcfTextAlignment;
    CFont* iIcfFont;
    
	/**
	 * Candidate laf
	 */
    TInt iCandsHorizontalMargin;
    TInt iCandsVerticalMargin;
    TInt iCandsUnitWidth;
    TInt iCandsUnitHeight;
    TPoint iCandidateLTPos;
    CFont* iCandsFont;
    TInt iCandsTextMargin;
    
	/**
	 * Symbol table laf data
	 */
	TRect iRectOfSymbolTable;
	TInt iSctpadRowCount;
    TInt iSctpadColCount;
    TSize iSizeSctpadCell;
	RArray<TRect> iSymBtnRectArray;
	
	/**
	 * virtual key rects array
	 */
    RArray<TRect> iSCTrectArray;
    // Number symbol table laf data
    TRect iRectOfNumSymbolTable;
    TInt iNumSctpadRowCount;
    TInt iNumSctpadColCount;
    TSize iSizeNumSctpadCell;
//    RArray<TRect> iNumSymBtnRectArray;    
    RArray<TRect> iNumSCTrectArray;
    /**
	 * Preview bubble
	 */
    TRect iPreviewWndRect;
    TRect iPreviewWndInnerRect;
    const CFont* iBubbleFont;
	TAknTextLineLayout iPreviewWndText;
    };

#endif // C_PENINPUTFINGERHWRARLAFMANAGER_H

// End Of File
