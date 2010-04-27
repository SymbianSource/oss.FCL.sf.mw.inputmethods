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
* Description:  Implementation of LAF manager.
*
*/

// INCLUDES
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknUtils.h>

#include "peninputfingerhwrlafmanager.h"
#include "peninputfingerhwrcontrolid.h"

#define LAF_MODIFY 1
 

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrLAFManager* CPeninputFingerHwrLAFManager::NewL()
    {
    CPeninputFingerHwrLAFManager* self = CPeninputFingerHwrLAFManager::NewLC();
    CleanupStack::Pop(); // self;
    return self;   
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrLAFManager* CPeninputFingerHwrLAFManager::NewLC()
    {
    CPeninputFingerHwrLAFManager* self = new( ELeave ) CPeninputFingerHwrLAFManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrLAFManager::~CPeninputFingerHwrLAFManager()
    {
    //nothing
    iSCTrectArray.Reset();
    iSCTrectArray.Close();
    }


// ---------------------------------------------------------------------------
// read laf data.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLAFManager::ReadLafInfo()
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

    icflayoutcn = AknLayoutScalable_Avkon::fshwr2_icf_pane(0).LayoutLine();
    
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

    candlistpane = AknLayoutScalable_Avkon::fshwr2_func_candi_pane(0).
										LayoutLine();

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
    iRectBtnImeSwitch = btnbgRect.Rect();
    
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
    
    iRectFixSctpad = btnrowRect.Rect();
  
    iRectSpase = btnbgRect.Rect();
    iRectSpase.Move(-iRectFixSctpad.iTl );
    
    // Enter button
    btnpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_pane( 0 , 1 , 0 ).
										LayoutLine();
	btnRect.LayoutRect(btnrowRect.Rect(), btnpane);																		
    btnbgpane = AknLayoutScalable_Avkon::fshwr2_func_candi_cell_bg_pane(0).
										LayoutLine();									
	btnbgRect.LayoutRect(btnRect.Rect(), btnbgpane);
										    
    iRectEnter = btnbgRect.Rect();
    iRectEnter.Move(-iRectFixSctpad.iTl);

    iSizeFixSctpadCell = iRectSpase.Size();
    
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
  	TAknWindowLineLayout previewWnd, previewBackground, previewWndInner;
  	TAknLayoutRect previewWndRect, previewBackgroundRect, previewWndInnerRect;
  	TAknLayoutText previewWndText;
  	previewWnd = AknLayoutScalable_Avkon::popup_fshwr2_char_preview_window(0).LayoutLine();	
  	            
  	previewWndRect.LayoutRect( iLayoutRect, previewWnd );
  	previewBackground = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_cp01().LayoutLine();
  	previewBackgroundRect.LayoutRect( previewWndRect.Rect(), previewBackground );
  	iPreviewWndRect = previewBackgroundRect.Rect();
  	
  	previewWndInner = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_g9().LayoutLine();
  	previewWndInnerRect.LayoutRect( previewBackgroundRect.Rect(), previewWndInner );
  	
  	iPreviewWndInnerRect = previewWndInnerRect.Rect();
  	iPreviewWndText = 
        AknLayoutScalable_Avkon::popup_fshwr2_char_preview_window_t1(0).LayoutLine();
    previewWndText.LayoutText(previewWndRect.Rect(), iPreviewWndText);
    iBubbleFont = previewWndText.Font();
    }
        

// ---------------------------------------------------------------------------
// get screen orientation.
// ---------------------------------------------------------------------------
//
TBool CPeninputFingerHwrLAFManager::IsLandscape()
    {
    return iIsLandscape;
    }
    
    
// ---------------------------------------------------------------------------
// get rect of layout.
// ---------------------------------------------------------------------------
//    
TRect CPeninputFingerHwrLAFManager::LayoutRect()
    {        
    return iLayoutRect;
    }


// ---------------------------------------------------------------------------
// get rect of specified control.
// ---------------------------------------------------------------------------
//  
TRect CPeninputFingerHwrLAFManager::CtrlRect( TInt aCtrlId )
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
        case EHwrCtrlIdNumpad:
            {
            return iRectNumpad;
            }
        case EHwrCtrlIdSctpad:
            {
            return iRectSctpad;
            }
        case EHwrCtrlIdSctpadFix:
            {
            return iRectFixSctpad;
            }            
        case EHwrCtrlIdClose:
            {
            return iRectBtnClose;
            }      
        case EHwrCtrlIdRangeSelector:
            {
            return iRectBtnRangeChn;
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
        case EHwrCtrlIdVkbSwitcher:
            {
            return iRectBtnImeSwitch;
            }
        case EHwrCtrlIdSctPage0Btn:
        case EHwrCtrlIdSctPage1Btn:
        case EHwrCtrlIdSctPage2Btn:
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
TRect CPeninputFingerHwrLAFManager::IcfRect()
    {    
    return iRectIcf;    
    }

// ---------------------------------------------------------------------------
// get left margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLAFManager::IcfLeftMargin()
    {
    return iIcfTextLeftMarginCn;
    }
    
// ---------------------------------------------------------------------------
// get right margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLAFManager::IcfRightMargin()
    { 
    return iIcfTextRightMarginCn;
    }    

// ---------------------------------------------------------------------------
// get top margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLAFManager::IcfTopMargin()
    {
    return iIcfTextTopMarginCn;
    }
    
// ---------------------------------------------------------------------------
// get bottom margin of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLAFManager::IcfBottomMargin()
    {      
    return iIcfTextBottomMarginCn;
    }
    
// ---------------------------------------------------------------------------
// get line spacing of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLAFManager::IcfLineSpaceMargin()
    {  
    return iIcfTextLineSpaceMarginCn;
    }    
    
// ---------------------------------------------------------------------------
// get font of icf.
// ---------------------------------------------------------------------------
// 
CFont* CPeninputFingerHwrLAFManager::IcfFont()
    {
    return iIcfFont;
    }
   
// ---------------------------------------------------------------------------
// get text height of icf.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLAFManager::IcfTextHeight()
    { 
    return iIcfTextHeightCn;
    }
       

// ---------------------------------------------------------------------------
// get inner padding of buttons.
// ---------------------------------------------------------------------------
//
TSize CPeninputFingerHwrLAFManager::ButtonInnerPadding()
    {
    return iSizeBtnPadding;
    }


// ---------------------------------------------------------------------------
// get cell size of virtual numpad.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrLAFManager::VirtualNumpadCellSize()
    {
    return iSizeNumpadCell;
    }
    

// ---------------------------------------------------------------------------
// get text layout of virtual numpad.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrLAFManager::NumpadKeyTxtLayout()
    {
    TAknTextLineLayout layout = AknLayoutScalable_Avkon::
        cell_fshwr2_syb_bg_pane_t1(0).LayoutLine();
    return layout;    
    }

// ---------------------------------------------------------------------------
// get row count of virtual sctpad.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLAFManager::VirtualSctpadRowCount()
    {
    return iSctpadRowCount;
    }

// ---------------------------------------------------------------------------
// get column count of virtual sctpad.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLAFManager::VirtualSctpadColCount()
    {
    return iSctpadColCount;
    }

// ---------------------------------------------------------------------------
// get cell size of virtual sctpad.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrLAFManager::VirtualSctpadCellSize()
    {
    return iSizeSctpadCell;
    }

// ---------------------------------------------------------------------------
// get cell size of virtual sctpad.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrLAFManager::FixVirtualSctpadCellSize()
    {
    return iSizeFixSctpadCell;
    }
    
    
// ---------------------------------------------------------------------------
// get preview bubble rect.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrLAFManager::PreviewBubbleRect()
    {
    return iPreviewWndRect;
    }

// ---------------------------------------------------------------------------
// get preview bubble inner rect
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrLAFManager::PreviewBubbleInnerRect()
    {
    return iPreviewWndInnerRect;
    }
 
// ---------------------------------------------------------------------------
// get preview bubble font
// ---------------------------------------------------------------------------
//
const CFont* CPeninputFingerHwrLAFManager::PreviewBubbleFont()
    {
    return iBubbleFont;
    }

// ---------------------------------------------------------------------------
// get preview bubble text layout
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrLAFManager::PreviewBubbleTextLayout()
    {
    return iPreviewWndText;
    }

// ---------------------------------------------------------------------------
// get cell rect of virtual sctpad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLAFManager::GetVirtualKeyRects( RArray<TRect>& aArrayRects )
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
// get space and enter key rect.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLAFManager::GetFixVirtualKeyRects( RArray<TRect>& aArrayRects )
    {   
    aArrayRects.Reset();
    aArrayRects.Append( iRectSpase );
    aArrayRects.Append( iRectEnter );
        
    }

// ---------------------------------------------------------------------------
// get text layout of virtual sctpad.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrLAFManager::SctpadKeyTxtLayout()
    {
    TAknTextLineLayout layout = AknLayoutScalable_Avkon::
        cell_fshwr2_syb_bg_pane_t1(0).LayoutLine();
    return layout;
    }

// ---------------------------------------------------------------------------
// get text layout of space and enter.
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputFingerHwrLAFManager::FixSctpadKeyTxtLayout()
    {
    //TAknTextLineLayout layout = AknLayoutScalable_Avkon::
    //    cell_fshwr2_syb_bg_pane_t1(0).LayoutLine();
    TAknTextLineLayout layout = AknLayoutScalable_Avkon::
    	fshwr2_func_candi_cell_pane_t1(0).LayoutLine();
    return layout;
    }

// ---------------------------------------------------------------------------
// get width unit of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLAFManager::CandidateUnitWidth()
    {
    return iCandsUnitWidth;
    }

// ---------------------------------------------------------------------------
// get height unit of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLAFManager::CandidateUnitHeight()
    {
    return iCandsUnitHeight;
    }

// ---------------------------------------------------------------------------
// get horizontal margin of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLAFManager::CandidateHorizontalMargin()
    {
    return iCandsHorizontalMargin;
    }    

// ---------------------------------------------------------------------------
// get vertical margin of candidate list.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLAFManager::CandidateVerticalMargin()
    {
    return iCandsVerticalMargin;
    }    
   
// ---------------------------------------------------------------------------
// get top-left position of candidate list.
// ---------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrLAFManager::CandidateLTPos()
    {
    return iCandidateLTPos;
    }

// ---------------------------------------------------------------------------
// get top-left position of predictive list.
// ---------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrLAFManager::PredictiveLTPos()
    {
    return iPredictiveLTPos;
    }
   
// ---------------------------------------------------------------------------
// get font of candidate list.
// ---------------------------------------------------------------------------
//
const CFont* CPeninputFingerHwrLAFManager::CandidateFont()
    {   
    return iCandsFont;
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrLAFManager::CPeninputFingerHwrLAFManager()
    {
    
    }
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLAFManager::ConstructL()
    {
    }

// End Of File
