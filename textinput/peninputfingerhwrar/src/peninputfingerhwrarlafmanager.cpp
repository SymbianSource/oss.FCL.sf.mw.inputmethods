/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of LAF manager.
*
*/


// INCLUDES
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <aknutils.h>

#include "peninputfingerhwrarlafmanager.h"
#include "peninputfingerhwrarcontrolid.h"

#define LAF_MODIFY 1
#define HackerVersion 

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLafManager* CPeninputFingerHwrArLafManager::NewL()
    {
    CPeninputFingerHwrArLafManager* self = CPeninputFingerHwrArLafManager::NewLC();
    CleanupStack::Pop(); // self;
    return self;   
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLafManager* CPeninputFingerHwrArLafManager::NewLC()
    {
    CPeninputFingerHwrArLafManager* self = new( ELeave ) CPeninputFingerHwrArLafManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLafManager::~CPeninputFingerHwrArLafManager()
    {
    //nothing
    iSCTrectArray.Reset();
    iSCTrectArray.Close();
    }


// ---------------------------------------------------------------------------
// read laf data.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLafManager::ReadLafInfo()
    {
    // Screen
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
    iScreenSize = rect.Size();

    iIsLandscape = iScreenSize.iWidth < iScreenSize.iHeight ? EFalse : ETrue;
    
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EApplicationWindow, rect);

    // finger Layout
    TAknWindowLineLayout fshwrlayout;
    TAknLayoutRect fshwrRect;
        
    fshwrlayout = AknLayoutScalable_Avkon::main_fep_fshwr2_pane(0).LayoutLine();
    fshwrRect.LayoutRect(rect, fshwrlayout);

    iLayoutRect = fshwrRect.Rect();
    iLayoutOffset = iLayoutRect.iTl;
    
    // ICF & Keypad
    TAknWindowLineLayout icflayoutcn,icflayoutbgcn;
    TAknTextLineLayout icflefttext;    
    TAknTextLineLayout icflefttext1, icflefttext2, icflefttext3; 
    TAknLayoutRect icfrectlatin, icfrectcn;

    if (iIsLandscape)
        {
        icflayoutcn = AknLayoutScalable_Avkon::fshwr2_icf_pane(1).LayoutLine();
        }
    else
        {
        icflayoutcn = AknLayoutScalable_Avkon::fshwr2_icf_pane(0).LayoutLine();
        }
    
    icfrectcn.LayoutRect(fshwrRect.Rect(), icflayoutcn);
	
    TAknLayoutScalableParameterLimits entryPaneVariety = 
                AknLayoutScalable_Avkon::fshwr2_icf_pane_t1_ParamLimits(0);
    TInt maxRow = entryPaneVariety.LastRow();
    
    icflefttext = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1(0, 0, 0).LayoutLine();
    
    icflefttext1 = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1(0, 0, 1).LayoutLine();
    icflefttext2 = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1(0, 0, 2).LayoutLine();
    icflefttext3 = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1(0, 0, maxRow).LayoutLine();
    iIcfTextLeftMarginCn = icflefttext.il;
    iIcfTextRightMarginCn = iIcfTextLeftMarginCn;
    
    TAknLayoutText ctxt, ctxt1, ctxt2, ctxt3;
    ctxt.LayoutText( icfrectcn.Rect(), icflefttext );
    ctxt1.LayoutText( icfrectcn.Rect(), icflefttext1 );
    ctxt2.LayoutText( icfrectcn.Rect(), icflefttext2 );
    ctxt3.LayoutText( icfrectcn.Rect(), icflefttext3 );
	
    iIcfTextTopMarginCn = ctxt.TextRect().iTl.iY - icfrectcn.Rect().iTl.iY;
    iIcfTextLineSpaceMarginCn = ctxt1.TextRect().iTl.iY - ctxt.TextRect().iBr.iY;
    iIcfTextBottomMarginCn = icfrectcn.Rect().iBr.iY - ctxt3.TextRect().iBr.iY 
                                                     - iIcfTextLineSpaceMarginCn;
    iIcfTextHeightCn = ctxt.TextRect().Height();    
 	
    iIcfFont = const_cast<CFont*>(AknLayoutUtils::FontFromId(icflefttext.iFont, NULL));

    iRectIcf = icfrectcn.Rect();
    
	// candidate list
    TAknWindowLineLayout candrowpane, candrowpane2, candlistpane, cellpane, candbgpane;
    TAknLayoutRect candpaneRect, candrowpaneRect, candrowpaneRect2, cellpaneRect1, cellpaneRect2;
    TAknLayoutRect cellpaneRect3, cellpaneRect;
	
    if (iIsLandscape)
        {
		candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(1).
										LayoutLine();        
        }
    else
        {
       	candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(0).
										LayoutLine();
        }
    candpaneRect.LayoutRect( fshwrRect.Rect(), candlistpane );
    rect = candpaneRect.Rect();
    candrowpane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 1).
										LayoutLine();
    candrowpaneRect.LayoutRect( rect, candrowpane );
    
    candrowpane2 = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 2).
										LayoutLine();
    candrowpaneRect2.LayoutRect( rect, candrowpane2 );
     
	
    cellpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 3 , 0 , 0 ).
										LayoutLine();
    cellpaneRect.LayoutRect( candrowpaneRect.Rect(), cellpane );
    
    candbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(3).LayoutLine();
    cellpaneRect1.LayoutRect( cellpaneRect.Rect(), candbgpane );   
    
    cellpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 3 , 1 , 0 ).
										LayoutLine();
    cellpaneRect.LayoutRect( candrowpaneRect.Rect(), cellpane );
    
    candbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(3).LayoutLine();
    cellpaneRect2.LayoutRect( cellpaneRect.Rect(), candbgpane );   
    
    
    
    cellpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 3 , 0 , 1 ).
										LayoutLine();
    cellpaneRect.LayoutRect( candrowpaneRect2.Rect(), cellpane );
    
    candbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(3).LayoutLine();
    cellpaneRect3.LayoutRect( cellpaneRect.Rect(), candbgpane );   
    
									
    iCandsHorizontalMargin = cellpaneRect2.Rect().iTl.iX - cellpaneRect1.Rect().iBr.iX;
    iCandsVerticalMargin = cellpaneRect3.Rect().iTl.iY - cellpaneRect1.Rect().iBr.iY;
	
    iCandsUnitWidth = cellpaneRect1.Rect().Width();
    iCandsUnitHeight = cellpaneRect1.Rect().Height() ;
    
    iCandsNaviHeight = cellpaneRect1.Rect().Height();
    iCandidateLTPos = candrowpaneRect.Rect().iTl;
    iPredictiveLTPos = candrowpaneRect2.Rect().iTl;
    
    TAknTextLineLayout candstxtlayout = 
                     AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane_t1(3).LayoutLine();
    TAknLayoutText candstxt;
    candstxt.LayoutText( cellpaneRect1.Rect(), candstxtlayout );
    iCandsFont = const_cast<CFont*>( AknLayoutUtils::FontFromId( candstxtlayout.iFont, NULL ) );
    iCandsTextMargin = candstxt.TextRect().iTl.iX - cellpaneRect1.Rect().iTl.iX;
    
    
    TAknWindowLineLayout btnrowpane, btnpane, btnbgpane;
    TAknLayoutRect btnrowRect, btnRect, btnbgRect;
    
    // Get the first row rect *************************************************
    btnrowpane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 0).
                                            LayoutLine();
    btnrowRect.LayoutRect(candpaneRect.Rect(), btnrowpane);
    // Close Button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , 0 , 0 ).
										LayoutLine();    
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane); 
    iRectBtnClose = btnbgRect.Rect();
    
    // Range Button with mark
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , 1 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane); 
    iRectBtnRangeChn = btnbgRect.Rect();
    iRectBtnRangeEng = iRectBtnRangeChn;
    iRectBtnRangeNum = iRectBtnRangeEng;
    
    // SCT button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , 2 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane); 
    iRectBtnRangeSmb = btnbgRect.Rect();
    
    // Backspace button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , 3 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);
    iRectBtnBackspace = btnbgRect.Rect();    
    
    // Get the second row rect **************************************************
    btnrowpane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 1).
                                            LayoutLine();
    btnrowRect.LayoutRect(candpaneRect.Rect(), btnrowpane);
    
    // Option button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 1 , 0 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(1).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);
    iRectBtnOption = btnbgRect.Rect();     
    
    // Switch mode button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 1 , 1 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(1).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane); 
    
    // Arrow up button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 1 , 2 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(1).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);  
    iRectBtnArrowUp = btnbgRect.Rect();
    iRectBtnSctPage = iRectBtnArrowUp;  


    // Get the third row rect **************************************************
    btnrowpane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 2).
                                            LayoutLine();
    btnrowRect.LayoutRect(candpaneRect.Rect(), btnrowpane);

    // Arrow left button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 1 , 0 , 0 ).
										LayoutLine();									
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(1).
										LayoutLine();
    btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane); 
    iRectBtnArrowLeft = btnbgRect.Rect();    
    
    // Arrow right button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 1 , 1 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(1).
										LayoutLine();
	btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);									    
    iRectBtnArrowRight = btnbgRect.Rect();
    
    // Arrow down button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 1 , 2 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(1).
										LayoutLine();
	btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);									    
    iRectBtnArrowDown = btnbgRect.Rect();
    
    // Space button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 0 , 0 , 0 ).
										LayoutLine();
    btnRect.LayoutRect(btnrowRect.Rect(), btnpane);
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(0).
										LayoutLine();
	btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);									    
    


    // Enter button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 0 , 1 , 0 ).
										LayoutLine();
	btnRect.LayoutRect(btnrowRect.Rect(), btnpane);																		
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(0).
										LayoutLine();									
	btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);
										    
    iRectEnter = btnbgRect.Rect();

    
    // Writing box
    TAknWindowLineLayout writingboxpane;
    TAknLayoutRect boxRect;
    writingboxpane = AknLayoutScalable_Avkon::fshwr2_hwr_syb_pane(0).LayoutLine();
    boxRect.LayoutRect(fshwrRect.Rect(), writingboxpane);
    iRectWritingBox = boxRect.Rect();
    iRectNumpad = iRectWritingBox;
    iRectSctpad = iRectWritingBox;
    
    // virtual key size
    TAknWindowLineLayout keypane, keybgpane;
    TAknLayoutRect keyRect, keybgRect;
  
    TAknLayoutScalableParameterLimits sctPaneVariety =
        AknLayoutScalable_Avkon::cell_fshwr2_syb_pane_ParamLimits(0);
    
    iSctpadRowCount = sctPaneVariety.LastRow() + 1;
    iSctpadColCount = sctPaneVariety.LastColumn() + 1;    
  
    iSizeBtnPadding = TSize(6, 6);
  
    iSCTrectArray.Reset();
    for(TInt i = 0; i < iSctpadRowCount; i++)
        {
        for ( TInt j = 0; j < iSctpadColCount; j++ )
            {
            keypane = AknLayoutScalable_Avkon::cell_fshwr2_syb_pane(0, j, i).LayoutLine();
            keyRect.LayoutRect(boxRect.Rect(), keypane);
            keybgpane = AknLayoutScalable_Avkon::cell_fshwr2_syb_bg_pane(0).LayoutLine();
            keybgRect.LayoutRect(keyRect.Rect(), keybgpane);
            iSCTrectArray.Append( keybgRect.Rect());
            }
        }
    iSizeNumpadCell = keybgRect.Rect().Size();
    iSizeSctpadCell = iSizeNumpadCell;
    
    // preview popup window 
  	TAknWindowLineLayout previewWnd, previewWndInner;
  	TAknLayoutRect previewWndRect, previewWndInnerRect;
  	TAknLayoutText previewWndText;
  	previewWnd = AknLayoutScalable_Avkon::popup_fep_char_preview_window(0).LayoutLine();
  	previewWndRect.LayoutRect( TRect(0,0,50,50), previewWnd );
  	previewWndInner = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_g9().LayoutLine();
  	previewWndInnerRect.LayoutRect( previewWndRect.Rect(), previewWndInner );
  	iPreviewWndRect = previewWndRect.Rect();
  	iPreviewWndInnerRect = previewWndInnerRect.Rect();
  	iPreviewWndText = 
        AknLayoutScalable_Avkon::popup_fep_char_preview_window_t1(0).LayoutLine();
    previewWndText.LayoutText(previewWndRect.Rect(), iPreviewWndText);
    iBubbleFont = previewWndText.Font();
    //==========================================================================
    // going to be removed if the LAF data is ready
    //
    #ifdef HackerVersion
	const TUint KMarginLayout = 4;
	// hardcode those and remove them after the laf data is ready
	// do we really need to read ui data from laf system?
	// icf margins
	iIcfTextLeftMarginCn = 14;
	iIcfTextRightMarginCn = 14;
    iIcfTextTopMarginCn = 14;
    iIcfTextBottomMarginCn = 20;
    iIcfTextLineSpaceMarginCn = 5;	
	
	// candiate margin
	iCandsHorizontalMargin = 1;
    iCandsVerticalMargin = 1;
	
    if(iIsLandscape)
    	{
    	TInt buttonHight = (iLayoutRect.Height()-2*KMarginLayout)/6;//-6; // 1mm ~= 6 pixls
        TInt buttonWidth = (iLayoutRect.Width()-2*KMarginLayout)/8;//-8;
    	
    	iRectBtnClose = TRect(iLayoutRect.iTl+TPoint(KMarginLayout,KMarginLayout),TPoint(buttonWidth+KMarginLayout,buttonHight+KMarginLayout));
    	
    	iRectBtnOption = iRectBtnClose;
    	iRectBtnOption.Move(TPoint(buttonWidth,0));
    	
    	iRectBtnRangeSmb = iRectBtnOption;
    	iRectBtnRangeSmb.Move(TPoint(buttonWidth,0));
    	
    	iRectBtnBackspace = iRectBtnRangeSmb;
    	iRectBtnBackspace.Move(TPoint(buttonWidth,0));
    	
    	// button on second row
        iRectBtnArrowUp = iRectBtnClose;
        iRectBtnArrowUp.Move(TPoint(0,buttonHight));
        
        iRectBtnArrowDown = iRectBtnArrowUp;
        iRectBtnArrowDown.Move(TPoint(buttonWidth,0));
        
        iRectBtnArrowLeft = iRectBtnArrowDown;
        iRectBtnArrowLeft.Move(TPoint(buttonWidth,0));
        
        iRectBtnArrowRight = iRectBtnArrowLeft;
        iRectBtnArrowRight.Move(TPoint(buttonWidth,0));
        
		iSizeBtnPadding = TSize(6,6);

	    
	    iRectSpase = iRectBtnClose;
	    iRectSpase.Move(TPoint(0,buttonHight));
	    
	    iRectEnter = iRectSpase;
	    iRectEnter.Move(TPoint(buttonWidth,0));
	            

	    
        // candate position
        iCandidateLTPos = iRectBtnClose.iTl + TPoint(0,buttonHight);
    	iCandsUnitWidth = buttonWidth-iCandsVerticalMargin;
        iCandsUnitHeight = buttonHight;
		
    	// icf editor rect
    	iRectIcf = TRect(TPoint(4*buttonWidth+KMarginLayout,KMarginLayout),TPoint(iLayoutRect.iBr.iX-KMarginLayout,2*buttonHight+KMarginLayout));
    	
    	// iwriting box
    	iRectWritingBox	= TRect(TPoint(KMarginLayout,2*buttonHight+KMarginLayout),iLayoutRect.iBr-TPoint(KMarginLayout,KMarginLayout));
    	
    	// construct symbol table
    	iRectOfSymbolTable = iRectWritingBox;
    	iRectOfSymbolTable.Move(TPoint(buttonWidth,-buttonHight));
    	iRectOfSymbolTable.SetHeight(5*buttonHight);
    	iRectOfSymbolTable.SetWidth(6*buttonWidth);
    	
    	iRectSctpad = iRectOfSymbolTable;
    	iRectSctpad.Move(TPoint(0,buttonHight));
    	iRectSctpad.SetHeight(4*buttonHight);
    	iRectSctpad.SetWidth(6*buttonWidth);
		
		// the button size for symbol table control 
		iSymButtonSize = TSize(2*buttonWidth,buttonHight);
		iSctpadRowCount = 4;
		iSctpadColCount = 6;
		
    	iSCTrectArray.Reset();
    	for(TInt i = 0; i <  iSctpadRowCount; i++)
    	    {	
    	    for(TInt j = 0; j < iSctpadColCount; j++)
    	        {
    	        TPoint rectTl = iRectSctpad.iTl+TPoint(j*buttonWidth,i*buttonHight);
    	        TPoint rectBr = rectTl+ TPoint(buttonWidth,buttonHight);
    	        iSCTrectArray.Append(TRect(rectTl,rectBr));
    	        }
    	    }
        }
    else
    	{
        // icf rect
        iRectIcf = TRect(iLayoutRect.iTl+TPoint(KMarginLayout,KMarginLayout),TPoint(iLayoutRect.iBr.iX-KMarginLayout,22.5*9));
        
		TInt buttonHight = (iLayoutRect.Height()-iRectIcf.Height()-2*KMarginLayout)/6;//(15*9)/2; // 1mm ~= 9pixls
        TInt buttonWidth = (iLayoutRect.Width()-2*KMarginLayout)/4;
		
        // the button on second row
		TPoint firstButtonTl = iRectIcf.iTl+TPoint(0,iRectIcf.Height());
		iRectBtnClose = TRect(firstButtonTl,firstButtonTl+TPoint(buttonWidth,buttonHight));

        iRectBtnOption = iRectBtnClose;
        iRectBtnOption.Move(TPoint(buttonWidth,0));
        
		iRectBtnRangeSmb = iRectBtnOption;
        iRectBtnRangeSmb.Move(TPoint(buttonWidth,0));
        
		iRectBtnBackspace = iRectBtnRangeSmb;
        iRectBtnBackspace.Move(TPoint(buttonWidth,0));
        
        // arrow button: on third row
        iRectBtnArrowUp = iRectBtnClose;
        iRectBtnArrowUp.Move(TPoint(0,buttonHight));
        
        iRectBtnArrowDown = iRectBtnArrowUp;
        iRectBtnArrowDown.Move(TPoint(buttonWidth,0));
        
        iRectBtnArrowLeft = iRectBtnArrowDown;
        iRectBtnArrowLeft.Move(TPoint(buttonWidth,0));
        
        iRectBtnArrowRight = iRectBtnArrowLeft;
        iRectBtnArrowRight.Move(TPoint(buttonWidth,0));
        
        // button padding
		iSizeBtnPadding = TSize(6,6);
		
        // candate position
        iCandidateLTPos = iRectBtnClose.iTl + TPoint(0,buttonHight);
        iCandsUnitWidth = buttonWidth-iCandsVerticalMargin;
        iCandsUnitHeight = buttonHight;
		
        // HWR Box Rect
        iRectWritingBox = TRect(TPoint(iRectBtnArrowUp.iTl.iX,iRectBtnArrowUp.iBr.iY),iLayoutRect.iBr-TPoint(KMarginLayout,KMarginLayout));
		
		// construct symbol table
    	iRectOfSymbolTable = iRectWritingBox;
    	iRectOfSymbolTable.Move(TPoint(0,-buttonHight));
    	iRectOfSymbolTable.SetHeight(5*buttonHight);
    	iRectOfSymbolTable.SetWidth(4*buttonWidth);
    	
    	iRectSctpad = iRectOfSymbolTable;
    	iRectSctpad.Move(TPoint(0,buttonHight));
    	iRectSctpad.SetHeight(4*buttonHight);
    	iRectSctpad.SetWidth(4*buttonWidth);
		
		// the button size for symbol table control 
		iSymButtonSize = TSize((4*buttonWidth)/3,buttonHight);
		
		iSctpadRowCount = 4;
		iSctpadColCount = 4;
		iSCTrectArray.Reset();
    	for(TInt i = 0; i <  iSctpadRowCount; i++)
    	    {	
    	    for(TInt j = 0; j < iSctpadColCount; j++)
    	        {
    	        TPoint rectTl = iRectSctpad.iTl+TPoint(j*buttonWidth,i*buttonHight);
    	        TPoint rectBr = rectTl+ TPoint(buttonWidth,buttonHight);
    	        iSCTrectArray.Append(TRect(rectTl,rectBr));
    	        }
    	    }
        }
    #endif        
    //
    //==========================================================================
    
    }
        

// ---------------------------------------------------------------------------
// get screen orientation.
// ---------------------------------------------------------------------------
//
TBool CPeninputFingerHwrArLafManager::IsLandscape()
    {
    return iIsLandscape;
    }
    
    
// ---------------------------------------------------------------------------
// get rect of layout.
// ---------------------------------------------------------------------------
//    
TRect CPeninputFingerHwrArLafManager::LayoutRect()
    {        
    return iLayoutRect;
    }


// ---------------------------------------------------------------------------
// get rect of specified control.
// ---------------------------------------------------------------------------
//  
TRect CPeninputFingerHwrArLafManager::CtrlRect( TInt aCtrlId )
    {
    switch ( aCtrlId )
        {
        case EHwrCtrlIdInputContextField:
            {
            return iRectIcf;
            }
        case EHwrCtrlIdWritingBox:
            {
            return iRectWritingBox;
            }
		case EHwrCtrlIdSymbolTableVkbGroup:	
		    {
			return iRectOfSymbolTable;
			}         
        case EHwrCtrlIdClose:
            {
            return iRectBtnClose;
            }          
        case EHwrCtrlIdSymbolButton:
            {
            return iRectBtnRangeSmb;
            }
        case EHwrCtrlIdBackspace:
            {
            return iRectBtnBackspace;
            }
        case EHwrCtrlIdArrowUp:
            {
            return iRectBtnArrowUp;
            }
        case EHwrCtrlIdArrowDown:
            {
            return iRectBtnArrowDown;
            }  
        case EHwrCtrlIdArrowLeft:
            {
            return iRectBtnArrowLeft;
            }
        case EHwrCtrlIdArrowRight:
            {
            return iRectBtnArrowRight;
            }
        case EHwrCtrlIdLanguageOption:
            {
            return iRectBtnOption;
            }
        case EHwrCtrlId3Page1Btn:
        case EHwrCtrlId3Page2Btn:
        case EHwrCtrlId3Page3Btn:
		case EHwrCtrlId2Page1Btn:
        case EHwrCtrlId2Page2Btn:
            {
            return iRectBtnSctPage;
            }    
        }
    return TRect();
    }

// ---------------------------------------------------------------------------
// get rect of ICF control.
// ---------------------------------------------------------------------------
// 
TRect CPeninputFingerHwrArLafManager::IcfRect()
    {    
    return iRectIcf;    
    }

// ---------------------------------------------------------------------------
// get left margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfLeftMargin()
    {
    return iIcfTextLeftMarginCn;
    }
    
// ---------------------------------------------------------------------------
// get right margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfRightMargin()
    { 
    return iIcfTextRightMarginCn;
    }    

// ---------------------------------------------------------------------------
// get top margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfTopMargin()
    {
    return iIcfTextTopMarginCn;
    }
    
// ---------------------------------------------------------------------------
// get bottom margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfBottomMargin()
    {      
    return iIcfTextBottomMarginCn;
    }
    
// ---------------------------------------------------------------------------
// get line spacing of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfLineSpaceMargin()
    {  
    return iIcfTextLineSpaceMarginCn;
    }    
    
// ---------------------------------------------------------------------------
// get font of icf.
// ---------------------------------------------------------------------------
// 
CFont* CPeninputFingerHwrArLafManager::IcfFont()
    {
//    //workaround
//    TAknTextLineLayout textPaneTextLayout;
//    TAknLayoutText txt;
//    textPaneTextLayout = 
//        AknLayoutScalable_Avkon::fep_hwr_top_text_pane_t1().LayoutLine();
//    const CFont* icffont = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, NULL );
//    
//    return const_cast<CFont*>(icffont);
    return iIcfFont;
    }
   
// ---------------------------------------------------------------------------
// get text height of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfTextHeight()
    { 
    return iIcfTextHeightCn;
    }
       

// ---------------------------------------------------------------------------
// get inner padding of buttons.
// ---------------------------------------------------------------------------
//
TSize CPeninputFingerHwrArLafManager::ButtonInnerPadding()
    {
    return iSizeBtnPadding;
    }
    

// ---------------------------------------------------------------------------
// get row count of virtual sctpad.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::VirtualSctpadRowCount()
    {
    return iSctpadRowCount;
    }

// ---------------------------------------------------------------------------
// get column count of virtual sctpad.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::VirtualSctpadColCount()
    {
    return iSctpadColCount;
    }

// ---------------------------------------------------------------------------
// get cell size of virtual sctpad.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrArLafManager::VirtualSctpadCellSize()
    {
    return iSizeSctpadCell;
    }
    
    
// ---------------------------------------------------------------------------
// get preview bubble rect.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrArLafManager::PreviewBubbleRect()
    {
    return iPreviewWndRect;
    }

// ---------------------------------------------------------------------------
// get preview bubble inner rect
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrArLafManager::PreviewBubbleInnerRect()
    {
    return iPreviewWndInnerRect;
    }
 
// ---------------------------------------------------------------------------
// get preview bubble font
// ---------------------------------------------------------------------------
//
const CFont* CPeninputFingerHwrArLafManager::PreviewBubbleFont()
    {
    return iBubbleFont;
    }

// ---------------------------------------------------------------------------
// get preview bubble text layout
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrArLafManager::PreviewBubbleTextLayout()
    {
    return iPreviewWndText;
    }

// ---------------------------------------------------------------------------
// get cell rect of virtual sctpad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLafManager::GetVirtualKeyRects( RArray<TRect>& aArrayRects )
    {   
    aArrayRects.Reset();
    
    for(TInt i = 0; i < iSCTrectArray.Count(); i++)
        {
        TRect absoluteRect = iSCTrectArray[i];
        absoluteRect.Move(-iRectSctpad.iTl.iX, -iRectSctpad.iTl.iY);
        aArrayRects.Append(absoluteRect);
        } 
    }

// ---------------------------------------------------------------------------
// get text layout of virtual sctpad.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrArLafManager::SctpadKeyTxtLayout()
    {
    TAknTextLineLayout layout = AknLayoutScalable_Avkon::
        cell_fshwr2_syb_bg_pane_t1(0).LayoutLine();
    return layout;
    }

// ---------------------------------------------------------------------------
// get text layout of space and enter.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrArLafManager::FixSctpadKeyTxtLayout()
    {
    TAknTextLineLayout layout = AknLayoutScalable_Avkon::
        cell_fshwr2_syb_bg_pane_t1(0).LayoutLine();
    return layout;
    }

// ---------------------------------------------------------------------------
// get width unit of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::CandidateUnitWidth()
    {
    return iCandsUnitWidth;
    }

// ---------------------------------------------------------------------------
// get height unit of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::CandidateUnitHeight()
    {
    return iCandsUnitHeight;
    }

// ---------------------------------------------------------------------------
// get horizontal margin of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::CandidateHorizontalMargin()
    {
    return iCandsHorizontalMargin;
    }    

// ---------------------------------------------------------------------------
// get vertical margin of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::CandidateVerticalMargin()
    {
    return iCandsVerticalMargin;
    }    
   
// ---------------------------------------------------------------------------
// get top-left position of candidate list.
// ---------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrArLafManager::CandidateLTPos()
    {
    return iCandidateLTPos;
    }

// ---------------------------------------------------------------------------
// get top-left position of predictive list.
// ---------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrArLafManager::PredictiveLTPos()
    {
    return iPredictiveLTPos;
    }
   
// ---------------------------------------------------------------------------
// get font of candidate list.
// ---------------------------------------------------------------------------
//
const CFont* CPeninputFingerHwrArLafManager::CandidateFont()
    {   
    return iCandsFont;
    }


// ---------------------------------------------------------------------------
// get text layout of sct paging button.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrArLafManager::SctPageBtnTxtLayout()
    {
    TAknTextLineLayout layout =  AknLayoutScalable_Apps::
        field_vitu2_entry_pane_t1(0, 0, 0).LayoutLine();
    return layout;
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLafManager::CPeninputFingerHwrArLafManager()
    {
    
    }
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLafManager::ConstructL()
    {
    }

TSize CPeninputFingerHwrArLafManager::SymbolGroupButtonSize()
   {
   return iSymButtonSize;
   }
// End Of File
