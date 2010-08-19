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
* Description:  Implementation of LAF manager.
*
*/


// INCLUDES
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknUtils.h>

#include "peninputfingerhwrarlafmanager.h"
#include "peninputfingerhwrarcontrolid.h"

//#define HackerVersion 
#define ARABIC_LAF

const TInt KSymButtonNum = 3;
const TInt KFuncButtonRowNum = 2;
const TInt KFuncButtonColNum = 4;
const TInt KLandscapeVirtualKeypadRow = 4;
const TInt KLandscapeVirtualKeypadCol = 6;
const TInt KLandscapeNumpadRow = 4;
const TInt KLandscapeNumpadCol = 4;

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
	
// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLafManager::~CPeninputFingerHwrArLafManager()
    {
    iSCTrectArray.Close();
	iSymBtnRectArray.Close();
    iNumSCTrectArray.Close();
//    iNumSymBtnRectArray.Close();
    }

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLayoutData()
// Retrieve layout data
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLayoutData()
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
	}

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLafDataForICF()
// Retrieve laf data for ICF editor
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLafDataForICF()
    {
	// ICF laf data
    TAknWindowLineLayout icfLayout;
    TAknLayoutRect icfLayoutRect;
	
    // read the icf layout 
    icfLayout = AknLayoutScalable_Avkon::fshwr2_icf_pane(1).LayoutLine();

    icfLayoutRect.LayoutRect(iLayoutRect, icfLayout);
	iRectIcf = icfLayoutRect.Rect();
    
	// read icf text line layout
    TAknTextLineLayout icfTextLineLayoutRow1 = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1(0, 0, 0).LayoutLine();   
    TAknTextLineLayout icfTextLineLayoutRow2 = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1(0, 0, 1).LayoutLine();
	TInt maxRow = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1_ParamLimits(0).LastRow();
    TAknTextLineLayout icfTextLineLayoutLastRow = AknLayoutScalable_Avkon::fshwr2_icf_pane_t1(0, 0, maxRow-1).LayoutLine();
    
	// read icf layout text
	TAknLayoutText icfLayoutText1;
	TAknLayoutText icfLayoutText2;
	TAknLayoutText icfLayoutTextLastRow;
  
    icfLayoutText1.LayoutText( iRectIcf, icfTextLineLayoutRow1 );
    icfLayoutText2.LayoutText( iRectIcf, icfTextLineLayoutRow2 );
    icfLayoutTextLastRow.LayoutText( iRectIcf, icfTextLineLayoutLastRow );
	
	// get the margins
	iIcfTextLeftMargin      = icfTextLineLayoutRow1.il;
    iIcfTextRightMargin     = iIcfTextLeftMargin;
    iIcfTextTopMargin       = icfLayoutText1.TextRect().iTl.iY - iRectIcf.iTl.iY;
    iIcfTextLineSpaceMargin = icfLayoutText2.TextRect().iTl.iY - icfLayoutText1.TextRect().iBr.iY;
    iIcfTextBottomMargin    = iRectIcf.iBr.iY - icfLayoutTextLastRow.TextRect().iBr.iY - iIcfTextLineSpaceMargin;
    iIcfTextHeight          = icfLayoutText1.TextRect().Height();    
 	
	// get font for text
    iIcfFont = const_cast<CFont*>(AknLayoutUtils::FontFromId(icfTextLineLayoutRow1.iFont, NULL));
    
    //indicator bubble.
#ifdef ARABIC_LAF
    if(iIsLandscape)
        {
        iRectIndicatorBubble.SetRect(543,17,624,41);
        }
    else
        {
        iRectIndicatorBubble.SetRect(263,17,344,41);
        }
#endif    
	}

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLafDataForCandidateList()
// Retrieve the laf data for candidate list
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLafDataForCandidateList()
    {
	// retrieve the cell flow:
	// fshwr2_func_candi_pane->fshwr2_func_candi_row_pane->fshwr2_func_candi_cell_pane
	// ->fshwr2_func_candi_cell_bg_pane
	
	// candidate list
	TAknWindowLineLayout candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(1).LayoutLine();        
    
	TAknLayoutRect candpaneRect;
    candpaneRect.LayoutRect( iLayoutRect, candlistpane );
    TRect rect = candpaneRect.Rect();
	
	// get candiate row1 pane
    TAknWindowLineLayout candRow1Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 1).
										LayoutLine();
    TAknLayoutRect candrow1paneRect;
	candrow1paneRect.LayoutRect( rect, candRow1Pane );
     
	// get candidate cell pane with row 1 and col 0
    TAknWindowLineLayout cellPane1Row1 = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , 0 , 1 ).
										LayoutLine();
    TAknLayoutRect cellPane1Row1Rect;
	cellPane1Row1Rect.LayoutRect( candrow1paneRect.Rect(), cellPane1Row1 );
    
	// get candidate cell bg pane 
    TAknWindowLineLayout candbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).LayoutLine();
	TAknLayoutRect cellPane1Row1BgRect;
    cellPane1Row1BgRect.LayoutRect( cellPane1Row1Rect.Rect(), candbgpane );   
	
	// get candidate cell pane with row 1 and col 1
    TAknWindowLineLayout cellPane2Row1 = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , 1 , 1 ).
										LayoutLine();
    TAknLayoutRect cellPane2Row1Rect;
	cellPane2Row1Rect.LayoutRect( candrow1paneRect.Rect(), cellPane2Row1 );
	
	// get candidate cell2 bg pane
	candbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).LayoutLine();
    TAknLayoutRect cellPane2Row1BgRect;
	cellPane2Row1BgRect.LayoutRect( cellPane2Row1Rect.Rect(), candbgpane );
	
	// ====================================================================================================
	// get candidate row2 pane
    TAknWindowLineLayout candRow2Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 2).
										LayoutLine();
    TAknLayoutRect candrow2paneRect;
	candrow2paneRect.LayoutRect( rect, candRow2Pane );
	
	// get candidate cell pane with row 2 and col 0
    TAknWindowLineLayout cellPane1Row2 = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , 0 , 0 ).
										LayoutLine();
    TAknLayoutRect cellPane1Row2Rect;
	cellPane1Row2Rect.LayoutRect( candrow2paneRect.Rect(), cellPane1Row2 );
    
	// get the candidate cell bg pane with row 2 and col 0
    candbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).LayoutLine();
	TAknLayoutRect cellPane1Row2BgRect;
	cellPane1Row2BgRect.LayoutRect( cellPane1Row2Rect.Rect(), candbgpane );   
	
    // calculate the margins	
    iCandsHorizontalMargin = cellPane2Row1BgRect.Rect().iTl.iX - cellPane1Row1BgRect.Rect().iBr.iX;
    iCandsVerticalMargin = cellPane1Row2BgRect.Rect().iTl.iY - cellPane1Row1BgRect.Rect().iBr.iY;
	
    iCandsUnitWidth = cellPane1Row1BgRect.Rect().Width();
    iCandsUnitHeight = cellPane1Row1BgRect.Rect().Height();
    
    iCandidateLTPos = candrow1paneRect.Rect().iTl;
    
	// get candidate text laf data
    TAknTextLineLayout candsTextLayout = 
                     AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane_t1(1).LayoutLine();
    TAknLayoutText candsText;
    candsText.LayoutText( cellPane1Row1Rect.Rect(), candsTextLayout );
    iCandsFont = const_cast<CFont*>( AknLayoutUtils::FontFromId( candsTextLayout.iFont, NULL ) );
    iCandsTextMargin = candsText.TextRect().iTl.iX - cellPane1Row1Rect.Rect().iTl.iX;
	}

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLafDataForFunctionalButton()
// Retrieve the laf data for functional group
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLafDataForFunctionalButton()
    {
	// retrieve the button laf flow:
	// fshwr2_func_candi_pane->fshwr2_func_candi_row_pane->fshwr2_func_candi_cell_pane
	// ->fshwr2_func_candi_cell_bg_pane
	
	// candidate list
	TAknWindowLineLayout candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(1).LayoutLine();        
    
	TAknLayoutRect candpaneRect;
    candpaneRect.LayoutRect( iLayoutRect, candlistpane );
    TRect rect = candpaneRect.Rect();
	
	TAknWindowLineLayout candRowPane;
	TAknLayoutRect candRowPaneRect;
	
	TAknWindowLineLayout cellPane;
    TAknLayoutRect cellPaneRect;
	
	TAknWindowLineLayout buttonBgPane;
	TAknLayoutRect cellPaneRowBgRect;
	
	RArray<TRect> buttonRect;
	for(TInt row = 0; row < KFuncButtonRowNum; row++)
	    {
		candRowPane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, row).LayoutLine();
		candRowPaneRect.LayoutRect( rect, candRowPane );

		for(TInt col = 0; col < KFuncButtonColNum; col++)
            {
			// cell pane
			cellPane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 2 , col , row ).LayoutLine();
			cellPaneRect.LayoutRect(candRowPaneRect.Rect(),cellPane);
			
			// cell bg pane
			buttonBgPane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(2).LayoutLine();
            cellPaneRowBgRect.LayoutRect( cellPaneRect.Rect(), buttonBgPane );
			buttonRect.Append(cellPaneRowBgRect.Rect());
			}			
		}
	
	// assign the functional button
    TInt i = 0;	
	iRectBtnClose =      buttonRect[i++];
    iRectBtnOption =     buttonRect[i++];
    iRectBtnRangeSmb = 	 buttonRect[i++];
	iRectBtnBackspace =  buttonRect[i++];
	iRectBtnArrowLeft =    buttonRect[i++];
	iRectBtnArrowRight =  buttonRect[i++];
	iRectBtnSpace =  buttonRect[i++];
	iRectBtnEnter = buttonRect[i++];
	buttonRect.Close();
	
	// padding size of Btn
	TInt xpad = (iRectBtnOption.iTl.iX - iRectBtnClose.iBr.iX)/2;
	TInt ypad = (iRectBtnArrowLeft.iTl.iY - iRectBtnClose.iBr.iY)/2;
	
	iSizeBtnPadding = TSize(xpad, ypad);
	
	// padding size of arrow button
	if(iIsLandscape)
	    {
	    iArrowPaddingSize = TSize(8,8);	
		}
	else
        {
		iArrowPaddingSize = TSize(12,12);
		}	
	}

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLafDataForHwrBox()
// Retrieve the laf data for hwr writing area
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLafDataForHwrBox()
    {
	TAknWindowLineLayout writingBoxPane;
    TAknLayoutRect boxRect;
    writingBoxPane = AknLayoutScalable_Avkon::fshwr2_hwr_syb_pane(1).LayoutLine();
    boxRect.LayoutRect(iLayoutRect, writingBoxPane);
    iRectWritingBox = boxRect.Rect();
	
	// need read laf data?
	iIndicatorRect.iTl = iRectWritingBox.iTl;
	iIndicatorRect.iTl = iIndicatorRect.iTl + TPoint(20,20);
	iIndicatorRect.SetSize(TSize(80,80));
	}
	
// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLafDataForSymbolTable()
// Retrieve the laf data for symbol table area
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLafDataForSymbolTable()
    {
	if(iIsLandscape)
	    {
	    RetrieveLandscapeLafDataForSymbolTable();
		}
	else
        {
		RetrievePortraitLafDataForSymbolTable();
		}	    	
    }

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLafDataForNumSymbolTable()
// Retrieve the laf data for number symbol table area
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrArLafManager::RetrieveLafDataForNumSymbolTable()
    {
    if(iIsLandscape)
        {
        RetrieveLandscapeLafDataForNumSymbolTable();
        }
    else
        {
        RetrievePortraitLafDataForNumSymbolTable();
        }           
    }

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLandscapeLafDataForSymbolTable()
// Retrieve the landscape laf data for symbol table area
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLandscapeLafDataForSymbolTable()
    {
	TAknLayoutScalableParameterLimits candiPaneVariety = AknLayoutScalable_Avkon::fshwr2_func_candi_pane_ParamLimits(2);
    
	iSctpadRowCount = KLandscapeVirtualKeypadRow;//candiPaneVariety.LastRow();
	iSctpadColCount = KLandscapeVirtualKeypadCol;//candiPaneVariety.LastColumn() + 1;

	// get the functional buttons laf data for symbol table
	// candidate list
	TAknWindowLineLayout candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(2).LayoutLine(); 
	TAknLayoutRect candpaneRect;
	candpaneRect.LayoutRect( iLayoutRect, candlistpane );
	iRectOfSymbolTable = candpaneRect.Rect();
	
	// get 1st row pane in button area
	TAknWindowLineLayout candRow1Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(1, 0, 0).LayoutLine();
	TAknLayoutRect candrow1paneRect;
	candrow1paneRect.LayoutRect( iRectOfSymbolTable, candRow1Pane );
	
	iSymBtnRectArray.Reset();
    TAknWindowLineLayout cellPane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 5, 0, 0 ).
                                        LayoutLine();
    TAknLayoutRect cellPaneRect;
    cellPaneRect.LayoutRect( candrow1paneRect.Rect(), cellPane );

    // get the candidate cell bg pane with row 0 and col i
    TAknWindowLineLayout cellbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(5).LayoutLine();
    TAknLayoutRect cellbgRect;
    cellbgRect.LayoutRect( cellPaneRect.Rect(), cellbgpane ); 
    iSymBtnRectArray.Append(cellbgRect.Rect());
	//get 1st row pane in SCT area
	TAknWindowLineLayout row1Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(1, 0, 1).LayoutLine();
	TAknLayoutRect row1Rect;
	row1Rect.LayoutRect(iRectOfSymbolTable,row1Pane);
	iRectSctpad.iTl = row1Rect.Rect().iTl;
//	iRectSctpad.iBr = iRectOfSymbolTable.iBr;
    
    TAknWindowLineLayout key1bgpane, keypane;
    TAknLayoutRect key1bgRect, keyRect;
    
	
	keypane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 4, 0 , 0 ).
																LayoutLine();
	keyRect.LayoutRect(row1Rect.Rect(), keypane);
	
	key1bgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(4).LayoutLine();
	key1bgRect.LayoutRect(keyRect.Rect(), key1bgpane);
	
	keypane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 4, 1 , 0 ).
																		LayoutLine();
	keyRect.LayoutRect(row1Rect.Rect(), keypane);
	
	TAknWindowLineLayout key2bgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(4).LayoutLine();
	TAknLayoutRect key2bgRect;
	key2bgRect.LayoutRect(keyRect.Rect(), key2bgpane);
	
	TAknWindowLineLayout row2Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(1, 0, 2).LayoutLine();
	TAknLayoutRect row2Rect;
	row2Rect.LayoutRect(iRectOfSymbolTable,row2Pane);
	
	keypane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 4, 0 , 0 ).LayoutLine();
	keyRect.LayoutRect(row2Rect.Rect(), keypane);
	
	TAknWindowLineLayout key3bgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(4).LayoutLine();
	TAknLayoutRect key3bgRect;
	key3bgRect.LayoutRect(keyRect.Rect(), key3bgpane);
			
	TInt keyHorizontalGap = key2bgRect.Rect().iTl.iX - key1bgRect.Rect().iBr.iX;
	TInt keyVerticalGap = key3bgRect.Rect().iTl.iY - key1bgRect.Rect().iBr.iY;
	
	TRect cursorRect(key1bgRect.Rect());
	TRect originRect(key1bgRect.Rect());
	TInt rectHeight = cursorRect.Height();
	TInt rectWidth = cursorRect.Width();
	
	iSCTrectArray.Reset();
	for(TInt i = 0; i < iSctpadRowCount; i++)
		{
		cursorRect = originRect;
		cursorRect.Move(0,i*(rectHeight+keyVerticalGap));
		iSCTrectArray.Append(cursorRect);
		for(TInt j = 1; j < iSctpadColCount; j++)
			{
			cursorRect.Move((rectWidth+keyHorizontalGap),0);
			iSCTrectArray.Append(cursorRect);
			}
		}
	iRectOfSymbolTable.iBr = cursorRect.iBr;
    iRectSctpad.iBr = iRectOfSymbolTable.iBr;
	
	}

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrievePortraitLafDataForSymbolTable()
// Retrieve the portrait laf data for symbol table area
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrievePortraitLafDataForSymbolTable()
    {
	// get the functional buttons laf data for symbol table
	// candidate list
	TAknWindowLineLayout candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(1).LayoutLine();        
	
	TAknLayoutRect candpaneRect;
	candpaneRect.LayoutRect( iLayoutRect, candlistpane );
	TRect rect = candpaneRect.Rect();
	
	// get candidate row2 pane
	TAknWindowLineLayout candRow2Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(0, 0, 1).
										LayoutLine();
	TAknLayoutRect candrow2paneRect;
	candrow2paneRect.LayoutRect( rect, candRow2Pane );
	iRectOfSymbolTable.iTl = candrow2paneRect.Rect().iTl;
	
	iSymBtnRectArray.Reset();
	for(TInt i = 0; i < KSymButtonNum; i++)
		{
		// get candidate cell pane with row 0 and col i
		TAknWindowLineLayout cellPaneRow = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 1, i , 0 ).
											LayoutLine();
		TAknLayoutRect cellPaneRect;
		cellPaneRect.LayoutRect( candrow2paneRect.Rect(), cellPaneRow );

		// get the candidate cell bg pane with row 0 and col i
		TAknWindowLineLayout candbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(1).LayoutLine();
		TAknLayoutRect BgRect;
		BgRect.LayoutRect( cellPaneRect.Rect(), candbgpane ); 
		iSymBtnRectArray.Append(BgRect.Rect());
		}
	
	// get the virtual key rects
	TAknWindowLineLayout writingBoxPane;
	TAknLayoutRect boxRect;
	writingBoxPane = AknLayoutScalable_Avkon::fshwr2_hwr_syb_pane(0).LayoutLine();
	boxRect.LayoutRect(iLayoutRect, writingBoxPane);
	iRectSctpad = boxRect.Rect();
	
	TAknWindowLineLayout keypane, keybgpane;
	TAknLayoutRect keyRect, keybgRect;
  
	TAknLayoutScalableParameterLimits sctPaneVariety =
		AknLayoutScalable_Avkon::cell_fshwr2_syb_pane_ParamLimits(0);
	
	iSctpadRowCount = sctPaneVariety.LastRow() + 1;
	iSctpadColCount = sctPaneVariety.LastColumn() + 1;    
  
	iSCTrectArray.Reset();
	for(TInt i = 0; i < iSctpadRowCount; i++)
		{
		for ( TInt j = 0; j < iSctpadColCount; j++ )
			{
			keypane = AknLayoutScalable_Avkon::cell_fshwr2_syb_pane(0, j, i).LayoutLine();
			keyRect.LayoutRect(iRectSctpad, keypane);
			keybgpane = AknLayoutScalable_Avkon::cell_fshwr2_syb_bg_pane(0).LayoutLine();
			keybgRect.LayoutRect(keyRect.Rect(), keybgpane);
			iSCTrectArray.Append( keybgRect.Rect());
			}
		}
	iSizeSctpadCell = keybgRect.Rect().Size();

	// calculate the symbol table rect	 
	iRectOfSymbolTable.iBr = iRectSctpad.iBr;
	}
	
// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLandscapeLafDataForNumSymbolTable()
// Retrieve the landscape laf data for number symbol table area
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrArLafManager::RetrieveLandscapeLafDataForNumSymbolTable()
    {
//    TAknLayoutScalableParameterLimits candiPaneVariety = AknLayoutScalable_Avkon::fshwr2_func_candi_pane_ParamLimits(2);
    
    iNumSctpadRowCount = KLandscapeNumpadRow;
    iNumSctpadColCount = KLandscapeNumpadCol;

    // get the functional buttons laf data for symbol table
    // candidate list
    TAknWindowLineLayout candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(2).LayoutLine(); 
    TAknLayoutRect candpaneRect;
    candpaneRect.LayoutRect( iLayoutRect, candlistpane );
    iRectOfNumSymbolTable = candpaneRect.Rect();
    
    // get candidate row1 pane
    TAknWindowLineLayout candRow1Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(2, 0, 0).LayoutLine();
    TAknLayoutRect candrow1paneRect;
    candrow1paneRect.LayoutRect( candpaneRect.Rect(), candRow1Pane );
    
    TAknWindowLineLayout rowPane, key1bgpane, keypane;
    TAknLayoutRect rowRect, key1bgRect, keyRect;
    
    iRectNumpad.iTl = candpaneRect.Rect().iTl;
    
    keypane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 6, 0 , 0 ).
                                                                LayoutLine();
    keyRect.LayoutRect(candrow1paneRect.Rect(), keypane);
    
    key1bgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(6).LayoutLine();
    key1bgRect.LayoutRect(keyRect.Rect(), key1bgpane);
    
    keypane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 6, 1 , 0 ).
                                                                        LayoutLine();
    keyRect.LayoutRect(candrow1paneRect.Rect(), keypane);
    
    TAknWindowLineLayout key2bgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(6).LayoutLine();
    TAknLayoutRect key2bgRect;
    key2bgRect.LayoutRect(keyRect.Rect(), key2bgpane);

    TAknWindowLineLayout row2Pane = AknLayoutScalable_Avkon::fshwr2_func_candi_row_pane(2, 0, 1).LayoutLine();
    TAknLayoutRect row2Rect;
    row2Rect.LayoutRect(candpaneRect.Rect(),row2Pane);
    
    keypane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 6, 1, 0 ).LayoutLine();
    keyRect.LayoutRect(row2Rect.Rect(), keypane);
    
    TAknWindowLineLayout key3bgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(6).LayoutLine();
    TAknLayoutRect key3bgRect;
    key3bgRect.LayoutRect(keyRect.Rect(), key3bgpane);
            
    TInt keyHorizontalGap = key2bgRect.Rect().iTl.iX - key1bgRect.Rect().iBr.iX;
    TInt keyVerticalGap = key3bgRect.Rect().iTl.iY - key1bgRect.Rect().iBr.iY;
    
    TRect cursorRect(key1bgRect.Rect());
    TRect originRect(key1bgRect.Rect());
    TInt rectHeight = cursorRect.Height();
    TInt rectWidth = cursorRect.Width();
    
    iNumSCTrectArray.Reset();
    for(TInt i = 0; i < iNumSctpadRowCount; i++)
        {
        cursorRect = originRect;
        cursorRect.Move(0,i*(rectHeight+keyVerticalGap));
        iNumSCTrectArray.Append(cursorRect);
        for(TInt j = 1; j < iNumSctpadColCount; j++)
            {
            cursorRect.Move((rectWidth+keyHorizontalGap),0);
            iNumSCTrectArray.Append(cursorRect);
            }
        }    

    // calculate the symbol table rect   
    iRectOfNumSymbolTable.iBr = cursorRect.iBr;        
    iRectNumpad.iBr = iRectOfNumSymbolTable.iBr;    
    }

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrievePortraitLafDataForNumSymbolTable()
// Retrieve the portrait laf data for number symbol table area
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrArLafManager::RetrievePortraitLafDataForNumSymbolTable()
    {
    
    // get the virtual key rects
    TAknWindowLineLayout writingBoxPane;
    TAknLayoutRect boxRect;
    writingBoxPane = AknLayoutScalable_Avkon::fshwr2_hwr_syb_pane(2).LayoutLine();
    boxRect.LayoutRect(iLayoutRect, writingBoxPane);
    iRectNumpad = boxRect.Rect();
    iRectOfNumSymbolTable.iTl = iRectNumpad.iTl;


    TAknWindowLineLayout keypane, keybgpane;
    TAknLayoutRect keyRect, keybgRect;
  
    TAknLayoutScalableParameterLimits sctPaneVariety =
        AknLayoutScalable_Avkon::cell_fshwr2_syb_pane_ParamLimits(0);
    
    iNumSctpadRowCount = sctPaneVariety.LastRow() + 1;
    iNumSctpadColCount = sctPaneVariety.LastColumn() + 1;    
  
    iNumSCTrectArray.Reset();
    for(TInt i = 0; i < iNumSctpadRowCount; i++)
        {
        for ( TInt j = 0; j < iNumSctpadColCount; j++ )
            {
            keypane = AknLayoutScalable_Avkon::cell_fshwr2_syb_pane(1, j, i).LayoutLine();
            keyRect.LayoutRect(iRectNumpad, keypane);
            keybgpane = AknLayoutScalable_Avkon::cell_fshwr2_syb_bg_pane(1).LayoutLine();
            keybgRect.LayoutRect(keyRect.Rect(), keybgpane);
            TRect rect = keybgRect.Rect();
            iNumSCTrectArray.Append( rect);
            }
        }
    iSizeNumSctpadCell = keybgRect.Rect().Size();

    // calculate the symbol table rect   
    iRectOfNumSymbolTable.iBr = iRectNumpad.iBr;        
    }

// ---------------------------------------------------------------------------
// CPeninputFingerHwrArLafManager::RetrieveLafDataForPreviewBubble()
// Retrieve the laf data for preview bubble
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArLafManager::RetrieveLafDataForPreviewBubble()
    {
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
	}
   
// ---------------------------------------------------------------------------
// read laf data.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLafManager::ReadLafInfo()
    {
    // retrieve the layout rect
	RetrieveLayoutData();
	
	// retrieve the laf data of icf
	RetrieveLafDataForICF();
	
    #ifdef HackerVersion
    #else 
	// retrieve the laf data of functional button
	RetrieveLafDataForFunctionalButton();
    #endif
	// retrieve the laf data of candidate list
	RetrieveLafDataForCandidateList();
	
    #ifdef HackerVersion
    #else
	// retrieve the laf data of symbol table
    RetrieveLafDataForSymbolTable();
    
    // retrieve the laf data of number symbol table
    RetrieveLafDataForNumSymbolTable();
    
	// retrieve the laf data of hwr box
    RetrieveLafDataForHwrBox();	 
    #endif
    
    // retrieve the laf data of preview bubble
    RetrieveLafDataForPreviewBubble();

    //==========================================================================
    // going to be removed if the LAF data is ready
    //
    #ifdef HackerVersion
	
	const TUint KMarginLayout = 4;
	// hardcode those and remove them after the laf data is ready
	// do we really need to read ui data from laf system?
	// icf margins
	iIcfTextLeftMargin = 14;
	iIcfTextRightMargin = 14;
    iIcfTextTopMargin = 14;
    iIcfTextBottomMargin = 20;
    iIcfTextLineSpaceMargin = 5;	
	
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
        iArrowPaddingSize = TSize(8,8);
	    
	    iRectBtnSpace = iRectBtnClose;
	    iRectBtnSpace.Move(TPoint(0,buttonHight));
	    
	    iRectBtnEnter = iRectBtnSpace;
	    iRectBtnEnter.Move(TPoint(buttonWidth,0));
	    
        // candate position
        iCandidateLTPos = iRectBtnClose.iTl + TPoint(0,buttonHight);
    	iCandsUnitWidth = buttonWidth-iCandsVerticalMargin;
        iCandsUnitHeight = buttonHight;
		
    	// icf editor rect
    	iRectIcf = TRect(TPoint(4*buttonWidth+KMarginLayout,KMarginLayout),TPoint(iLayoutRect.iBr.iX-KMarginLayout,2*buttonHight+KMarginLayout));
    	
    	// iwriting box
    	iRectWritingBox	= TRect(TPoint(KMarginLayout,2*buttonHight+KMarginLayout),iLayoutRect.iBr-TPoint(KMarginLayout,KMarginLayout));
    	
		iIndicatorRect.iTl = iRectWritingBox.iTl;
		iIndicatorRect.iTl = iIndicatorRect.iTl + TPoint(20,20);
		iIndicatorRect.SetSize(TSize(80,80));
		
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
		iSymBtnRectArray.Reset();
		TRect btnRect;
		btnRect.iTl = iRectOfSymbolTable.iTl;
		btnRect.SetSize(TSize(2*buttonWidth,buttonHight));
		iSymBtnRectArray.Append(btnRect);
		
		btnRect.Move(TPoint(btnRect.Width(),0));
		iSymBtnRectArray.Append(btnRect);
		
		btnRect.Move(TPoint(btnRect.Width(),0));
		iSymBtnRectArray.Append(btnRect);
		
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
		iArrowPaddingSize = TSize(12,12);
		
        // candate position
        iCandidateLTPos = iRectBtnClose.iTl + TPoint(0,buttonHight);
        iCandsUnitWidth = buttonWidth-iCandsVerticalMargin;
        iCandsUnitHeight = buttonHight;
		
        // HWR Box Rect
        iRectWritingBox = TRect(TPoint(iRectBtnArrowUp.iTl.iX,iRectBtnArrowUp.iBr.iY),iLayoutRect.iBr-TPoint(KMarginLayout,KMarginLayout));
		
		iIndicatorRect.iTl = iRectWritingBox.iTl;
		iIndicatorRect.iTl = iIndicatorRect.iTl + TPoint(20,20);
		iIndicatorRect.SetSize(TSize(80,80));
		
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
		iSymBtnRectArray.Reset();
		TRect btnRect;
		btnRect.iTl = iRectOfSymbolTable.iTl;
		btnRect.SetSize(TSize((4*buttonWidth)/3,buttonHight));
		iSymBtnRectArray.Append(btnRect);
		
		btnRect.Move(TPoint(btnRect.Width(),0));
		iSymBtnRectArray.Append(btnRect);
		
		btnRect.Move(TPoint(btnRect.Width(),0));
		iSymBtnRectArray.Append(btnRect);
		
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
        case EHwrCtrlIdNumSymbolTableVkbGroup: 
            {
            return iRectOfNumSymbolTable;
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
        case EHwrCtrlIdArrowLeft:
            {
            return iRectBtnArrowLeft;
            }
        case EHwrCtrlIdArrowRight:
            {
            return iRectBtnArrowRight;
            }
        case EHwrCtrlIdBtnEnter:
            {
            return iRectBtnEnter;
            }
        case EHwrCtrlIdBtnSpace:
            {
            return iRectBtnSpace;
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
    return iIcfTextLeftMargin;
    }
    
// ---------------------------------------------------------------------------
// get right margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfRightMargin()
    { 
    return iIcfTextRightMargin;
    }    

// ---------------------------------------------------------------------------
// get top margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfTopMargin()
    {
    return iIcfTextTopMargin;
    }
    
// ---------------------------------------------------------------------------
// get bottom margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfBottomMargin()
    {      
    return iIcfTextBottomMargin;
    }
    
// ---------------------------------------------------------------------------
// get line spacing of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfLineSpaceMargin()
    {  
    return iIcfTextLineSpaceMargin;
    }    
    
// ---------------------------------------------------------------------------
// get font of icf.
// ---------------------------------------------------------------------------
// 
CFont* CPeninputFingerHwrArLafManager::IcfFont()
    {
    return iIcfFont;
    }
   
// ---------------------------------------------------------------------------
// get text height of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLafManager::IcfTextHeight()
    { 
    return iIcfTextHeight;
    }
       
// ---------------------------------------------------------------------------
// get indicator bubble rect.
// ---------------------------------------------------------------------------
// 
TRect CPeninputFingerHwrArLafManager::IndicatorBubbleRect()
    {
    return iRectIndicatorBubble;
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
// get row count of virtual number sctpad.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::VirtualNumSctpadRowCount()
    {
    return iNumSctpadRowCount;
    }

// ---------------------------------------------------------------------------
// get column count of virtual number sctpad.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLafManager::VirtualNumSctpadColCount()
    {
    return iNumSctpadColCount;
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
// get cell size of virtual numpad.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrArLafManager::VirtualNumpadCellSize()
    {
    return iSizeNumSctpadCell;
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
// get cell rect of virtual sctpad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLafManager::GetNumVirtualKeyRects( RArray<TRect>& aArrayRects )
    {   
    aArrayRects.Reset();
    
    for(TInt i = 0; i < iNumSCTrectArray.Count(); i++)
        {
        TRect absoluteRect = iNumSCTrectArray[i];
        absoluteRect.Move(-iRectNumpad.iTl.iX, -iRectNumpad.iTl.iY);
        aArrayRects.Append(absoluteRect);
        } 
    }

// ---------------------------------------------------------------------------
// get text layout of virtual sctpad.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrArLafManager::SctpadKeyTxtLayout()
    {
    TAknTextLineLayout layout;
    if(iIsLandscape)
        {
        layout = AknLayoutScalable_Avkon::
                fshwr2_func_candi_cell_pane_t1(4).LayoutLine();
        }
    else
        {
        layout = AknLayoutScalable_Avkon::
            cell_fshwr2_syb_bg_pane_t1(0).LayoutLine();
        }
    return layout;
    }

// ---------------------------------------------------------------------------
// get text layout of virtual sctpad.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrArLafManager::NumpadKeyTxtLayout()
    {
    TAknTextLineLayout layout;
    if(iIsLandscape)
        {
        layout = AknLayoutScalable_Avkon::
                fshwr2_func_candi_cell_pane_t1(6).LayoutLine();
        }
    else
        {
        layout = AknLayoutScalable_Avkon::
            cell_fshwr2_syb_bg_pane_t1(1).LayoutLine();
        }
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
// get font for candidate list.
// ---------------------------------------------------------------------------
//
const CFont* CPeninputFingerHwrArLafManager::CandidateFont()
    {   
    return iCandsFont;
    }

// ---------------------------------------------------------------------------
// get rects for symbol buttons
// ---------------------------------------------------------------------------
//	
RArray<TRect>& CPeninputFingerHwrArLafManager::GetSymBtnArray()
    {
	return iSymBtnRectArray;
	}

// ---------------------------------------------------------------------------
// get rect for virtual key
// ---------------------------------------------------------------------------
//	
TRect CPeninputFingerHwrArLafManager::GetVirtualKeyRect()
    {
	return iRectSctpad;
	} 	
    	
// ---------------------------------------------------------------------------
// get rect for virtual number keypad
// ---------------------------------------------------------------------------
//  
TRect CPeninputFingerHwrArLafManager::GetVirtualNumKeyRect()
    {
    return iRectNumpad;
    }   

// ---------------------------------------------------------------------------
// get rect for hwr indicator
// ---------------------------------------------------------------------------
//		
TRect CPeninputFingerHwrArLafManager::GetIndicatorRect()
    {
    return iIndicatorRect;
    }

// ---------------------------------------------------------------------------
// get padding size for arrow button
// ---------------------------------------------------------------------------
//	
TSize CPeninputFingerHwrArLafManager::GetArrowPaddingSize()
    {
	return iArrowPaddingSize;
	}
   
// End Of File
