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
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
    iScreenSize = rect.Size();

    iLandScape = iScreenSize.iWidth < iScreenSize.iHeight ? EFalse : ETrue;
    
    TAknWindowLineLayout layoutLineEntry, layoutLineCandidate, layoutLineEEP;
    TAknLayoutRect layoutRectEntry, layoutRectCandidate, layoutRectEEP;
    
    layoutLineEntry = AKN_LAYOUT_WINDOW_popup_fep_china_uni_window(0,0);
    layoutRectEntry.LayoutRect(iScreenSize,layoutLineEntry);
    iPopupRectEntry = layoutLineEntry.Rect();
    iPopupHeightEntry = layoutLineEntry.iH;
    
    //PinyinPopupWindowRect
    iEntryPaneWindowLine = AKN_LAYOUT_WINDOW_fep_china_uni_entry_pane;
    iLayoutEntryItem = AKN_LAYOUT_WINDOW_fep_entry_item_pane;
    
    iLayoutLineRectEntryPane.LayoutRect(iPopupRectEntry, iEntryPaneWindowLine);
    
    layoutLineCandidate = AKN_LAYOUT_WINDOW_popup_fep_china_uni_window(0,1);
    layoutRectCandidate.LayoutRect(iScreenSize,layoutLineCandidate);
    iPopupRectCandidate = layoutLineCandidate.Rect();
    
    layoutLineEEP = AKN_LAYOUT_WINDOW_popup_fep_china_uni_window(0,2);//need updated
    layoutRectEEP.LayoutRect(iScreenSize,layoutLineEEP);
    iPopupRectEEP = layoutLineEEP.Rect();
    iPopupHeightEEP = layoutLineEEP.iH;
    
    
    iCandidatePaneWindowLine = AKN_LAYOUT_WINDOW_fep_china_uni_candidate_pane( 0 );
    
    //Candidate
    
    iCandidatePaneTextLine = AKN_LAYOUT_TEXT_Chinese_universal_FEP_candidate_pane_texts_Line_1;
    iOrdinalCandidateTextLayout = AKN_LAYOUT_TEXT_Chinese_universal_FEP_candidate_pane_texts_Line_3;
    iCandidateWindowLine1 = AKN_LAYOUT_WINDOW_Chinese_universal_FEPcandidate_pane_elements_Line_1;
    iCandidateWindowLine2 = AKN_LAYOUT_WINDOW_Chinese_universal_FEPcandidate_pane_elements_Line_2;
    iCandidateWindowLine3 = AKN_LAYOUT_WINDOW_Chinese_universal_FEPcandidate_pane_elements_Line_3;
    iCandidateWindowLine4 = AKN_LAYOUT_WINDOW_Chinese_universal_FEPcandidate_pane_elements_Line_4;
    
    iLayoutCandidateItem = AKN_LAYOUT_WINDOW_fep_candidate_item_pane;
    
    //EEP   
    TAknWindowComponentLayout EEPWindowComponentLayout = AknLayoutScalable_Avkon::fep_china_uni_eep_pane();     
    iEEPWindowLayout = EEPWindowComponentLayout.LayoutLine();
    TAknLayoutRect EEPLayout;
    EEPLayout.LayoutRect( iPopupRectEEP, iEEPWindowLayout );
    iEEPWindowRect = EEPLayout.Rect();
    //
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
       
    //HorizLine

    iEntryHorizLineWindow = AknLayoutScalable_Avkon::popup_fep_china_uni_window_g(0,0).LayoutLine();
    iEEPSecondHorizLineWindow = AknLayoutScalable_Avkon::popup_fep_china_uni_window_g(0,1).LayoutLine();
    iEEPFirstHorizLineWindow = AknLayoutScalable_Avkon::popup_fep_china_uni_window_g(1,0).LayoutLine();
    iHorizLine.LayoutRect(iPopupRectEntry, iEntryHorizLineWindow);
    iEEPFirstHorizLine.LayoutRect(iPopupRectEEP, iEEPFirstHorizLineWindow);
    iEEPSecondHorizLine.LayoutRect(iPopupRectEEP, iEEPSecondHorizLineWindow);

    iEntryPaneRect = iLayoutLineRectEntryPane.Rect( );
    iEntryTextLayout
        = AKN_LAYOUT_TEXT_Chinese_universal_FEP_entry_pane_texts_Line_1(0);
    TAknLayoutRect labelRect;
    labelRect.LayoutRect( iEntryPaneRect,
        AKN_LAYOUT_WINDOW_fep_entry_item_pane );
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
    // left and right arrow
    TAknWindowLineLayout leftArrowLayoutLine, rithgArrowLayoutLine;	
    leftArrowLayoutLine = AKN_LAYOUT_WINDOW_Chinese_universal_FEP_entry_pane_elements_Line_1;
    iIndiFepArrowLeft.LayoutRect(entryPane,leftArrowLayoutLine);
    rithgArrowLayoutLine = AKN_LAYOUT_WINDOW_Chinese_universal_FEP_entry_pane_elements_Line_2;
    iIndiFepArrowRight.LayoutRect(entryPane,rithgArrowLayoutLine);   	
    
    iRectCoverMainPaneInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_1(windowRect));
    iRectFirstShadowInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_2(windowRect));
    iRectOutlineFrameInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_3(windowRect));
    iRectInsideAreaInput.LayoutRect(windowRect, AKN_LAYOUT_WINDOW_Chinese_universal_FEP_pop_up_window_graphics_Line_4(windowRect));
    
    //eep text
    TAknTextComponentLayout textLayout = AknLayoutScalable_Avkon::fep_china_uni_eep_pane_t1();
    iEEPTextLine = textLayout.LayoutLine();
    TAknLayoutText eepTextLayout;
    eepTextLayout.LayoutText( iEEPWindowRect, iEEPTextLine );
    iEEPTextColor = eepTextLayout.Color();
    const CFont* font = AknLayoutUtils::FontFromId(iEEPTextLine.FontId());
    iEEPCursorHeight = font->HeightInPixels();    
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
        case ECandidatePaneTextLine:
            return &iCandidatePaneTextLine;
        case ECandidateWindowLine1:
            return &iCandidateWindowLine1;
        case ECandidateWindowLine2:
            return &iCandidateWindowLine2;
        case ECandidateWindowLine3:
            return &iCandidateWindowLine3;
        case ECandidateWindowLine4:
            return &iCandidateWindowLine4;
        case ELayoutCandidateItem:
            return &iLayoutCandidateItem;
        case ELayoutEntryItem:
            return &iLayoutEntryItem;
        case EOrdinalCandidateTextLayout:
            return &iOrdinalCandidateTextLayout;
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
        case EIndiFepArrowLeft:
            return &iIndiFepArrowLeft;
        case EIndiFepArrowRight:
            return &iIndiFepArrowRight;
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
        default:
            break;
        }
    return NULL;
    }

// End of File
