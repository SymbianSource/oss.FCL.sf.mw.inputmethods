/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for phrase creation layout data managment
 *
*/


// INCLUDE FILES
#include <gulfont.h> 
#include <eiklabel.h>
#include <AknLayout.lag>
#include <AknsDrawUtils.h>
#include <skinlayout.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>

#include "aknfepuilayoutdatamgr.h"

// -----------------------------------------------------------------------------
// CAknFepUiLayoutDataMgr::GetPinyinPhraseCreationContainerHeight()
// Get the pinyin phrase creation container Height data 
// -----------------------------------------------------------------------------
//
CAknFepUiLayoutDataMgr* CAknFepUiLayoutDataMgr::NewL()
    {
    CAknFepUiLayoutDataMgr* self = new(ELeave)CAknFepUiLayoutDataMgr();
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( ); // self
    return self;
    }

CAknFepUiLayoutDataMgr::CAknFepUiLayoutDataMgr()
    {
    }

void CAknFepUiLayoutDataMgr::ConstructL()
    {
    ReadLafInfo();
    }

void CAknFepUiLayoutDataMgr::ReadLafInfo()
    {	
	// Screen
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, rect );
    iScreenSize = rect.Size();

    iLandScape = iScreenSize.iWidth < iScreenSize.iHeight ? EFalse : ETrue;
    
    // 1. Container With Candidate list
    TAknWindowLineLayout layoutLineEntry;
    TAknLayoutRect layoutRectEntry;
      
    // Get container Rect & height
    layoutLineEntry = AknLayoutScalable_Avkon::popup_hyb_candi_window(0);
    layoutRectEntry.LayoutRect( rect, layoutLineEntry );    
    iPopupRectEntry = layoutRectEntry.Rect();
    iPopupHeightEntry = layoutLineEntry.iH;
    
    // Candidate layout
    iCandidate = AknLayoutScalable_Avkon::grid_hyb_candi_pane(0);
    
    // Scroll layout
    iScrollLayout = AknLayoutScalable_Avkon::cell_hyb_candi_scroll_pane(0); 
   
    // Up arrow & down arrow & close in scroll pane
    iUpArrowLayout = AknLayoutScalable_Avkon::cell_hyb_candi_scroll_pane_g1_aid(0);   
    iDownArrowLayout = AknLayoutScalable_Avkon::cell_hyb_candi_scroll_pane_g2_aid(0);    
    iCloseLayout = AknLayoutScalable_Avkon::cell_hyb_candi_scroll_pane_g3(0);
    
    // Character in candidate pane
    iCharacterLineLayout = AknLayoutScalable_Avkon::cell_hyb_candi_pane(0);    
    iCharacterTextLayout = AknLayoutScalable_Avkon::cell_hyb_candi_pane_t1(0);
    iEntryPaneWindowLine = AknLayoutScalable_Avkon::input_focus_pane_cp06( 0 );
    iEntryPaneWindowLine.il = 3;
    iEntryPaneWindowLine.it = 3;
    iEntryPaneWindowLine.ir = 5;
    iEntryPaneWindowLine.iH = 30;
    iLayoutEntryItem = AKN_LAYOUT_WINDOW_fep_entry_item_pane;    
    iLayoutLineRectEntryPane.LayoutRect( iPopupRectEntry, iEntryPaneWindowLine );
    
    // 2. EC( With Candidate list & Entry )
    TAknWindowLineLayout layoutLineCandidate;
    TAknLayoutRect layoutRectCandidate, layoutEC;
    
    // Container Rect & height
    layoutLineCandidate = AknLayoutScalable_Avkon::popup_hyb_candi_window(1);
    layoutRectCandidate.LayoutRect( rect,layoutLineCandidate );
    iPopupRectCandidate = layoutRectCandidate.Rect();
    iPopupRectCandidateHeight = layoutLineCandidate.iH;
    
    // Candidate layout
    iECandidate = AknLayoutScalable_Avkon::grid_hyb_candi_pane(1);    
    iEntryCLayout = AknLayoutScalable_Avkon::entry_hyb_candi_pane(0);
    
    // Phrase pane layout
    iECWindowLayout = AknLayoutScalable_Avkon::grid_hyb_phrase_pane(0);     
       
    // 3. ECP( With Candidate list & Entry & PopUp )
    TAknWindowLineLayout layoutLineEEP;
    TAknLayoutRect layoutEEP, layoutRectEEP;
    
    // Container Rect & height
    layoutLineEEP = AknLayoutScalable_Avkon::popup_hyb_candi_window(2);
    layoutRectEEP.LayoutRect(rect,layoutLineEEP);
    iPopupRectEEP = layoutRectEEP.Rect();
    iPopupHeightEEP = layoutLineEEP.iH;
    
    //Entry
    iEntryCPLayout = AknLayoutScalable_Avkon::entry_hyb_candi_pane(1);
    iEntryWithCandidateAndPopUp = AknLayoutScalable_Avkon::grid_hyb_phrase_pane(1);

    // Candidate layout
    iEPCandidate = AknLayoutScalable_Avkon::grid_hyb_candi_pane(2);
       
    //EEP
    iEEPWindowLayout = AknLayoutScalable_Avkon::grid_hyb_phrase_pane(1);     
    layoutEEP.LayoutRect( iPopupRectEEP, iEEPWindowLayout );
    iEEPWindowRect = layoutEEP.Rect();
    
    iRectCoverMainPaneInputEntry.LayoutRect(iPopupRectEntry, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_1(iPopupRectEntry));
    iRectFirstShadowInputEntry.LayoutRect(iPopupRectEntry, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_2(iPopupRectEntry));
    iRectOutlineFrameInputEntry.LayoutRect(iPopupRectEntry, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_3(iPopupRectEntry));
    iRectInsideAreaInputEntry.LayoutRect(iPopupRectEntry, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_4(iPopupRectEntry));
    
    iRectCoverMainPaneInputCandidate.LayoutRect(iPopupRectCandidate, 
                                                AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_1(iPopupRectCandidate));
    iRectFirstShadowInputCandidate.LayoutRect(iPopupRectCandidate, 
                                              AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_2(iPopupRectCandidate));
    iRectOutlineFrameInputCandidate.LayoutRect(iPopupRectCandidate,
                                               AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_3(iPopupRectCandidate));
    iRectInsideAreaInputCandidate.LayoutRect(iPopupRectCandidate, 
                                             AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_4(iPopupRectCandidate));
    
  
    iRectCoverMainPaneInputEEP.LayoutRect(iPopupRectEEP, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_1(iPopupRectEEP));
    iRectFirstShadowInputEEP.LayoutRect(iPopupRectEEP, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_2(iPopupRectEEP));
    iRectOutlineFrameInputEEP.LayoutRect(iPopupRectEEP, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_3(iPopupRectEEP));
    iRectInsideAreaInputEEP.LayoutRect(iPopupRectEEP, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_4(iPopupRectEEP));    
    
    iEntryHorizLineWindow = AknLayoutScalable_Avkon::popup_fep_china_uni_window_g(0,0).LayoutLine();
    iEEPSecondHorizLineWindow = AknLayoutScalable_Avkon::popup_fep_china_uni_window_g(0,1).LayoutLine();
    iEEPFirstHorizLineWindow = AknLayoutScalable_Avkon::popup_fep_china_uni_window_g(1,0).LayoutLine();
    iHorizLine.LayoutRect(iPopupRectEntry, iEntryHorizLineWindow);
    iEEPFirstHorizLine.LayoutRect(iPopupRectEEP, iEEPFirstHorizLineWindow);
    iEEPSecondHorizLine.LayoutRect(iPopupRectEEP, iEEPSecondHorizLineWindow);
    iEntryPaneRect = iLayoutLineRectEntryPane.Rect( );

    TAknLayoutRect labelRect;
    labelRect.LayoutRect( iEntryPaneRect, AknLayoutScalable_Avkon::entry_hyb_candi_pane(0));
    iEntryTextLayout = AknLayoutScalable_Avkon::entry_hyb_candi_pane_t1( 0 );
    iLayoutInputPaneText.LayoutText( labelRect.Rect( ), iEntryTextLayout );

    TRect windowRect = layoutRectEntry.Rect();    
    TInt height = windowRect.Height();
    TInt width = windowRect.Width();
    
    windowRect.iTl.iY = 0;
    windowRect.iTl.iX = 0;
    windowRect.SetHeight(height);
    windowRect.SetWidth(width);
      
    TAknLayoutRect layoutLineRectEntryPane;
    layoutLineRectEntryPane.LayoutRect(windowRect, iEntryPaneWindowLine);
    TRect entryPane = layoutLineRectEntryPane.Rect();
     
    iRectCoverMainPaneInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_1(windowRect));
    iRectFirstShadowInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_2(windowRect));
    iRectOutlineFrameInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_3(windowRect));
    iRectInsideAreaInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_4(windowRect));
      
    //eep text
    TAknTextComponentLayout textLayout = AknLayoutScalable_Avkon::entry_hyb_candi_pane_t1(0);
    iEEPTextLine = textLayout.LayoutLine();
    TAknLayoutText eepTextLayout;
    eepTextLayout.LayoutText( iEEPWindowRect, iEEPTextLine );
    iEEPTextColor = eepTextLayout.Color();
    const CFont* font = AknLayoutUtils::FontFromId(iEEPTextLine.FontId());
    iEEPCursorHeight = font->HeightInPixels();    
    iInputLayout = AknLayoutScalable_Avkon::cell_hyb_phrase_pane(0);
    iInputTextLayout = AknLayoutScalable_Avkon::cell_hyb_phrase_pane_t1(0);
    }

TAny* CAknFepUiLayoutDataMgr::RequestData(TInt aDataType)
    {
    switch ( aDataType )
        {
        case EPopupRectEntry:
            return &iPopupRectEntry;
        case EPopupRectCandatate:
            return &iPopupRectCandidate;
        case EPopupRectEEP:
            return &iPopupRectEEP;
        case EPopupHeightEntry:
            return &iPopupHeightEntry;
        case EPopupHeightEEP:
            return &iPopupHeightEEP;
        case EEntryPaneWindowLine:
            return &iEntryPaneWindowLine;
        case ECandidatePaneWindowLine:
            return &iCandidatePaneWindowLine;
        case ELayoutEntryItem:
            return &iLayoutEntryItem;
        case EEEPWindowRect:
            return &iEEPWindowRect;
        case EEEPWindowLayout:
            return &iEEPWindowLayout;
        case ERectCoverMainPaneInputEntry:
            return &iRectCoverMainPaneInputEntry;
        case ERectFirstShadowInputEntry:
            return &iRectFirstShadowInputEntry;
        case ERectOutlineFrameInputEntry:
            return &iRectOutlineFrameInputEntry;
        case ERectInsideAreaInputEntry:
            return &iRectInsideAreaInputEntry;
        case ERectCoverMainPaneInputCandidate:
            return &iRectCoverMainPaneInputCandidate;
        case ERectFirstShadowInputCandidate:
            return &iRectFirstShadowInputCandidate;
        case ERectOutlineFrameInputCandidate:
            return &iRectOutlineFrameInputCandidate;
        case ERectInsideAreaInputCandidate:
            return &iRectInsideAreaInputCandidate;
        case ERectCoverMainPaneInputEEP:
            return &iRectCoverMainPaneInputEEP;
        case ERectFirstShadowInputEEP:
            return &iRectFirstShadowInputEEP;
        case ERectOutlineFrameInputEEP:
            return &iRectOutlineFrameInputEEP;
        case ERectInsideAreaInputEEP:
            return &iRectInsideAreaInputEEP;
        case ELayoutInputPaneText:
            return &iLayoutInputPaneText;
        case ELayoutLineRectEntryPane:
            return &iLayoutLineRectEntryPane;
        case EEntryTextLayout:
            return &iEntryTextLayout;
        case EEntryPaneRect:
            return &iEntryPaneRect;
        case ERectCoverMainPaneInput:
            return &iRectCoverMainPaneInput;
        case ERectFirstShadowInput:
            return &iRectFirstShadowInput;
        case ERectOutlineFrameInput:
            return &iRectOutlineFrameInput;
        case ERectInsideAreaInput:
            return &iRectInsideAreaInput;
        case EEEPTextLine:
            return &iEEPTextLine;
        case EEEPCursorHeight:
            return &iEEPCursorHeight;
        case EHorizLine:
            return &iHorizLine;
        case EEEPFirstHorizLine:
            return &iEEPFirstHorizLine;
        case EEEPSecondHorizLine:
            return &iEEPSecondHorizLine;
        case EEntryHorizLineWindow:
            return &iEntryHorizLineWindow;
        case EEEPSecondHorizLineWindow:
            return &iEEPSecondHorizLineWindow;
        case EEEPFirstHorizLineWindow:
            return &iEEPFirstHorizLineWindow;
        case EEEPTextColor:
            return &iEEPTextColor;
        case EECPLayout:
        	return &iEntryWithCandidateAndPopUp;//Rect;
        case ECandidateLayout:
        	return &iCandidate;
        case EECandidateLayout:
        	return &iECandidate;
        case EEPCandidateLayout:
        	return &iEPCandidate;
        case ECharacterLayout:
        	return &iCharacterLineLayout;
        case ECharacterTextLayout:
        	return &iCharacterTextLayout;
        case EPopupHeightEP:
        	return &iPopupRectCandidateHeight;
        case EUpArrowLayout:
        	return &iUpArrowLayout;
        case EDownArrowLayout:
        	return &iDownArrowLayout;
        case EScrollLayout:
        	return &iScrollLayout;
        case EEntryCLayout:
        	return &iEntryCLayout;
        case EEntryCPLayout:
        	return &iEntryCPLayout;
        case EECWindowLayout:
        	return &iECWindowLayout;
        case ECloseLayout:
        	return &iCloseLayout;
        case EInputLayout:
        	return &iInputLayout;
        case EInputTextLayout:
        	return &iInputTextLayout;
        default:
            break;
        }
    return NULL;
    }

// End of File
