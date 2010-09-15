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
* Description:  This file contains classes of phrase creation layout data management
*
*/


#ifndef C_PHRASECREATIONLAYOUTDATAMGR_H
#define C_PHRASECREATIONLAYOUTDATAMGR_H

#include <e32std.h>
#include <e32base.h>
#include <badesca.h>
#include <coecntrl.h>
#include <AknUtils.h>
#include <AknLayout2ScalableDef.h>

enum TAknFepChineseUiLafDataType
    {
    EPopupRectEntry,
    EPopupRectCandatate,
    EPopupRectEEP,
    EPopupHeightEntry,
    EPopupHeightEEP,
    EEntryPaneWindowLine,
    ECandidatePaneWindowLine,
    EEEPWindowLayout,
    EEEPWindowRect,
    ELayoutEntryItem,
    ERectCoverMainPaneInputEntry,
    ERectFirstShadowInputEntry,
    ERectOutlineFrameInputEntry,
    ERectInsideAreaInputEntry, 
    ERectCoverMainPaneInputCandidate,
    ERectFirstShadowInputCandidate,
    ERectOutlineFrameInputCandidate,
    ERectInsideAreaInputCandidate, 
    ERectCoverMainPaneInputEEP,
    ERectFirstShadowInputEEP,
    ERectOutlineFrameInputEEP,
    ERectInsideAreaInputEEP,
    ELayoutInputPaneText,
    ELayoutLineRectEntryPane,
    EEntryTextLayout,
    EEntryPaneRect,
    ERectCoverMainPaneInput,
    ERectFirstShadowInput,
    ERectOutlineFrameInput,
    ERectInsideAreaInput,
    EEEPTextLine,
    EEEPCursorHeight,
    EEEPTextColor,
    EHorizLine,
    EEEPFirstHorizLine,
    EEEPSecondHorizLine,
    EEntryHorizLineWindow,
    EEEPSecondHorizLineWindow,
    EEEPFirstHorizLineWindow,
    EECPLayout,
    ECandidateLayout,
    EECandidateLayout,
    EEPCandidateLayout,
    ECharacterLayout,
    ECharacterTextLayout,
    EPopupHeightEP,
    EUpArrowLayout,
    EDownArrowLayout,
    ECloseLayout,
    EScrollLayout,
    EEntryCLayout,
    EEntryCPLayout,
    EECWindowLayout,
    EInputLayout,
    EInputTextLayout
    };
//class CAknFepUiLayoutDataMgr;
/**
 *  CAknFepUiLayoutDataMgr
 *  This class provides the phrase creation layout 
 *  data interface management
 *  to the program other component
 */
/**
* CAknFepUiLayoutDataMgr is a static fascade interface to the singleton layout data mgr object.
* There is at most one CAknFepUiLayoutDataMgr per thread. This static interface ensures that this
* fact can not be changed.
*/
class CAknFepUiLayoutDataMgr  : public CBase
    {
public:
    // General public API

    /**
     * First phase construction
     */
    static CAknFepUiLayoutDataMgr* NewL();
    
    /**
     * Read LAF info 
     * @return None.
     */
    void ReadLafInfo();

    /**
     * Get LAF info 
     */
    TAny* RequestData(TInt aDataType);
    
private:
    
    /**
     * Constructor
     */
    CAknFepUiLayoutDataMgr();
    
    /**
     * Second phase construction
     */
    void ConstructL();
    
private:
    TSize iScreenSize;
    TBool iLandScape;
    
    TRect iPopupRectEntry;
    TRect iPopupRectCandidate;
    TRect iPopupRectEEP;
    TInt iPopupHeightEntry;
    TInt iPopupHeightEEP;
    TInt16 iEEPHeight;
    
    TAknWindowLineLayout iEntryPaneWindowLine;
    TAknWindowLineLayout iCandidatePaneWindowLine;
    
    TAknWindowLineLayout iEEPWindowLayout;
    TAknWindowLineLayout iECWindowLayout;
    TRect iEEPWindowRect;
    TRect iECWindowRect;
    TRect iEntryPaneRect;

    TAknWindowLineLayout iLayoutEntryItem;

    TAknTextLineLayout iEntryTextLayout;
    
    TAknLayoutRect iRectCoverMainPaneInputEntry;
    TAknLayoutRect iRectFirstShadowInputEntry;
    TAknLayoutRect iRectOutlineFrameInputEntry;
    TAknLayoutRect iRectInsideAreaInputEntry;
    
    TAknLayoutRect iRectCoverMainPaneInputCandidate;
    TAknLayoutRect iRectFirstShadowInputCandidate;
    TAknLayoutRect iRectOutlineFrameInputCandidate;
    TAknLayoutRect iRectInsideAreaInputCandidate;
    
    TAknLayoutRect iRectCoverMainPaneInputEEP;
    TAknLayoutRect iRectFirstShadowInputEEP;
    TAknLayoutRect iRectOutlineFrameInputEEP;
    TAknLayoutRect iRectInsideAreaInputEEP;
    
    TAknWindowLineLayout iEntryHorizLineWindow;
    TAknWindowLineLayout iEEPSecondHorizLineWindow;
    TAknWindowLineLayout iEEPFirstHorizLineWindow;
    TAknLayoutRect iHorizLine;
    TAknLayoutRect iEEPFirstHorizLine;
    TAknLayoutRect iEEPSecondHorizLine;
    
    TAknLayoutText iLayoutInputPaneText;
    TAknLayoutRect iLayoutLineRectEntryPane;
    
   	TAknLayoutRect iRectCoverMainPaneInput;
    TAknLayoutRect iRectFirstShadowInput;
    TAknLayoutRect iRectOutlineFrameInput;
    TAknLayoutRect iRectInsideAreaInput;
    
    TAknWindowLineLayout iEntryWithCandidateAndPopUp;    
    
    TAknWindowLineLayout iCandidate;
    TAknWindowLineLayout iECandidate;
    TAknWindowLineLayout iEPCandidate;
    TAknWindowLineLayout iCharacterLineLayout;
    TAknTextLineLayout iCharacterTextLayout;
    TAknWindowLineLayout iUpArrowLayout;
    TAknWindowLineLayout iDownArrowLayout;
    TAknWindowLineLayout iCloseLayout;
    TAknWindowLineLayout iScrollLayout;
    TAknWindowLineLayout iEntryCLayout;
    TAknWindowLineLayout iEntryCPLayout;
    
    TAknTextLineLayout iInputTextLayout;
    TAknWindowLineLayout iInputLayout;
    
    TAknTextLineLayout iEEPTextLine;
    TInt iEEPCursorHeight;
    TRgb iEEPTextColor;
    
    TInt iPopupRectCandidateHeight;
    };

#endif // C_PHRASECREATIONLAYOUTDATAMGR_H

// End of File
