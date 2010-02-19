/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* @since S60 v5.0
*/
class CPeninputFingerHwrArLafManager : public CBase 
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object.
     */    
    static CPeninputFingerHwrArLafManager* NewL();
    
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object.
     */  
    static CPeninputFingerHwrArLafManager* NewLC();
    
    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */    
    ~CPeninputFingerHwrArLafManager();
   
public:
    /**
     * read laf data.
     *
     * @since S60 v5.0
     * @return None
     */    
    void ReadLafInfo();
    
    /**
     * get screen orientation.
     * 
     * @since S60 v5.0
     * @return ETrue if is landscape.
     */
    TBool IsLandscape();

    /**
     * get rect of layout.
     * 
     * @since S60 v5.0
     * @return rect of layout.
     */    
    TRect LayoutRect();
    
    /**
     * get rect of specified control.
     * 
     * @since S60 v5.0
     * @param aCtrlId specifies control id.
     * @return rect of specified control.
     */
    TRect CtrlRect( TInt aCtrlId );
    
public: // icf
    /**
     * get rect of ICF control.
     * 
     * @since S60 v5.0
     * @return rect of icf.
     */      
    TRect IcfRect();
    
    /**
     * get left margin of icf.
     * 
     * @since S60 v5.0
     * @return left margin of icf.
     */
    TInt IcfLeftMargin();
    
    /**
     * get Right margin of icf.
     * 
     * @since S60 v5.0
     * @return Right margin of icf.
     */    
    TInt IcfRightMargin();
    
    /**
     * get top margin of icf.
     * 
     * @since S60 v5.0
     * @return top margin of icf.
     */    
    TInt IcfTopMargin();
    
    /**
     * get bottom margin of icf.
     * 
     * @since S60 v5.0
     * @return bottom margin of icf.
     */    
    TInt IcfBottomMargin();
    
    /**
     * get line spacing of icf.
     * 
     * @since S60 v5.0
     * @return line spacing of icf.
     */      
    TInt IcfLineSpaceMargin();
  
    /**
     * get font of icf.
     * 
     * @since S60 v5.0
     * @return font of icf.
     */       
    CFont* IcfFont();
    
    /**
     * get text height of icf.
     * 
     * @since S60 v5.0
     * @return text height of icf.
     */       
    TInt IcfTextHeight();
    
public: //buttons
    
    /**
     * get inner padding of buttons.
     * 
     * @since S60 v5.0
     * @return inner padding of buttons.
     */      
    TSize ButtonInnerPadding();
    
public: //num pad
    /**
     * get cell size of virtual numpad.
     * 
     * @since S60 v5.0
     * @return cell size of virtual numpad.
     */    
    TRect VirtualNumpadCellSize();
        
    /**
     * get text layout of virtual numpad.
     * 
     * @since S60 v5.0
     * @return a TAknTextLineLayout object to layout cell text.
     */
    TAknTextLineLayout NumpadKeyTxtLayout();
    
public: //sct pad
  
    /**
     * get row count of virtual sctpad.
     * 
     * @since S60 v5.0
     * @return row count of virtual sctpad.
     */      
    TInt VirtualSctpadRowCount();
    
    /**
     * get column count of virtual sctpad.
     * 
     * @since S60 v5.0
     * @return column count of virtual sctpad.
     */        
    TInt VirtualSctpadColCount();
    
    /**
     * get cell size of virtual sctpad.
     * 
     * @since S60 v5.0
     * @return cell size of virtual sctpad.
     */     
    TRect VirtualSctpadCellSize();
    
    /**
     * Get cell size of Space and Enter
     * 
     * @since S60 v5.0
     * @return cell size of virtual sctpad.
     */     
    TRect FixVirtualSctpadCellSize();
    
    /**
     * get cell rect of virtual sctpad.
     * 
     * @since S60 v5.0
     * @param aArrayRects a TRect array to receive cell rect.
     * @return none.
     */     
    void GetVirtualKeyRects( RArray<TRect>& aArrayRects );
    
    /**
     * get cell rect of Space and Enter.
     * 
     * @since S60 v5.0
     * @param aArrayRects a TRect array to receive cell rect.
     * @return none.
     */     
    void GetFixVirtualKeyRects( RArray<TRect>& aArrayRects );
    
    
    /**
     * get text layout of virtual sctpad.
     * 
     * @since S60 v5.0
     * @return a TAknTextLineLayout object to layout cell text.
     */    
    TAknTextLineLayout SctpadKeyTxtLayout();
    
    /**
     * Get text layout about Space and Enter
     * 
     * @since S60 v5.0
     * @return a TAknTextLineLayout object to layout cell text.
     */    
    TAknTextLineLayout FixSctpadKeyTxtLayout();
    
    /**
     * get preview bubble rect.
     * 
     * @since S60 v5.0
     * @return a TRect object to preview bubble rect.
     */    
    TRect PreviewBubbleRect();
    
    /**
     * get preview bubble inner rect.
     * 
     * @since S60 v5.0
     * @return a TRect object to preview bubble inner rect.
     */    
    TRect PreviewBubbleInnerRect();
    
    /**
     * get preview bubble font.
     * 
     * @since S60 v5.0
     * @return a CFont pointer to preview bubble font.
     */    
    const CFont* PreviewBubbleFont();
    
    /**
     * get preview bubble text layout.
     * 
     * @since S60 v5.0
     * @return a TAknTextLineLayout object to preview bubble text layout.
     */    
    TAknTextLineLayout PreviewBubbleTextLayout();
    
public: // candidate list
    
    /**
     * get width unit of candidate list.
     * 
     * @since S60 v5.0
     * @return width unit of candidate list.
     */      
    TInt CandidateUnitWidth();
    
    /**
     * get height unit of candidate list.
     * 
     * @since S60 v5.0
     * @return height unit of candidate list.
     */ 
    TInt CandidateUnitHeight();  
    

    /**
     * get horizontal margin of candidate list.
     * 
     * @since S60 v5.0
     * @return horizontal margin of candidate list.
     */    
    TInt CandidateHorizontalMargin();

    /**
     * get vertical margin of candidate list.
     * 
     * @since S60 v5.0
     * @return vertical margin of candidate list.
     */      
    TInt CandidateVerticalMargin();

    /**
     * get top-left position of candidate list.
     * 
     * @since S60 v5.0
     * @return top-left position of candidate list.
     */     
    TPoint CandidateLTPos();

    /**
     * get top-left position of predictive list.
     * 
     * @since S60 v5.0
     * @return top-left position of predictive list.
     */     
    TPoint PredictiveLTPos();
    
    /**
     * get font of candidate list.
     * 
     * @since S60 v5.0
     * @return font of candidate list.
     */     
    const CFont* CandidateFont();
    
public: //sct paging buttons
    /**
     * get text layout of sct paging button.
     * 
     * @since S60 v5.0
     * @return a TAknTextLineLayout object to layout text.
     */    
    TAknTextLineLayout SctPageBtnTxtLayout();
	TSize SymbolGroupButtonSize();
private:
    
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    CPeninputFingerHwrArLafManager();
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();    
    
private: //datas
    
    TBool iIsLandscape;
    
    // constrols position
    TRect iRectIcf;
    TRect iRectWritingBox;
    TRect iRectNumpad;
    TRect iRectSctpad;
    TRect iRectFixSctpad;
    TRect iRectSpase;
    TRect iRectEnter;
        
    TRect iRectBtnClose;
    TRect iRectBtnRangeChn;
    TRect iRectBtnRangeEng;
    TRect iRectBtnRangeNum;
    TRect iRectBtnRangeSmb;
    TRect iRectBtnBackspace;
    TRect iRectBtnArrowLeft;
    TRect iRectBtnArrowRight;
    TRect iRectBtnArrowUp;
    TRect iRectBtnArrowDown;
    TRect iRectBtnOption;
    TRect iRectBtnImeSwitch;
    TRect iRectBtnSctPage;
    
    TSize iSizeBtnPadding;
    TSize iSizeNumpadCell;
    
    TInt iSctpadRowCount;
    TInt iSctpadColCount;
    TSize iSizeSctpadCell;
    TSize iSizeFixSctpadCell;

    TRect iLayoutRect;
    TPoint iLayoutOffset;
    
	TInt iIcfTextLeftMarginCn;
	TInt iIcfTextRightMarginCn;
	TInt iIcfTextTopMarginCn;
	TInt iIcfTextBottomMarginCn;
	TInt iIcfTextLineSpaceMarginCn;
	TInt iIcfTextHeightCn;
    TInt iIcfTextAlignment;
    CFont* iIcfFont;
    
    TInt iCandsHorizontalMargin;
    TInt iCandsVerticalMargin;
    TInt iCandsUnitWidth;
    TInt iCandsUnitHeight;
    TInt iCandsNaviHeight;
    TPoint iCandidateLTPos;
    TPoint iPredictiveLTPos;
    CFont* iCandsFont;
    TInt iCandsTextMargin;
    
    TSize iScreenSize;
    
    RArray<TRect> iSCTrectArray;
    
    // preview bubble
    TRect iPreviewWndRect;
    TRect iPreviewWndInnerRect;
    const CFont* iBubbleFont;
	TAknTextLineLayout iPreviewWndText;
	TSize iSymButtonSize;
	TRect iRectOfSymbolTable;
    };

#endif // C_PENINPUTFINGERHWRARLAFMANAGER_H

// End Of File
