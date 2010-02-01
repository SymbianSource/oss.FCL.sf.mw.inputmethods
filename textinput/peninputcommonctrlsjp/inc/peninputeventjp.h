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
* Description:  the enumeration of peninputcommonctrlsjp
*
*/

#ifndef T_TPENINPUTBEVENTJP_H
#define T_TPENINPUTBEVENTJP_H

#define KPENINPUT_LAYOUT_VKB_POPUP_MAX_ROW_COUNT 5
#define KPENINPUT_LAYOUT_HWR_POPUP_MAX_ROW_COUNT 6
#define KPENINPUT_LAYOUT_PREDICTIONPANE_ROW_COUNT 2

#define KPENINPUT_LAYOUT_VALUE_2 2
#define KPENINPUT_LAYOUT_VALUE_3 3
#define KPENINPUT_LAYOUT_VALUE_4 4
#define KPENINPUT_LAYOUT_VALUE_5 5
#define KPENINPUT_LAYOUT_VALUE_6 6
#define KPENINPUT_LAYOUT_VALUE_7 7
#define KPENINPUT_LAYOUT_VALUE_16 16

// EEventControlUserBase = 2000    //user event starts from here
enum TPenInputEventJp
    {
    //Event notify StdKeyLeftArrow
    EEventStdKeyLeftArrow = 2000,

    //Event notify StdKeyRightArrow
    EEventStdKeyRightArrow,

    //Event notify StdKeyRightArrow
    EEventStdKeyUpArrow,

    //Event notify StdKeyRightArrow
    EEventStdKeyDownArrow,

    //Event CancelHiraganaKanji
    EEventCancelHiraganaKanji,

    //Event CompleteAllHiraganaKanji
    EEventCompleteAllHiraganaKanji,

    //Event Status Change to StandbyJP
    EEventHiraganaKanjiWithTransitoryChars2Standbyjp,

    //Event Status is WithTransitoryChars but no TransitoryChars exist(predictive on)
    EEventHiraganaKanjiWithoutTransitoryChars,

    //Event Show Candidate
    EEventShowCandidateWnd,

    //Event Hide Candidate
    EEventHideCandidateWnd,

    //Event wait OnAppEditorTextComing
    EEventWaitforEditorTextComing,

    // SCT Button
    EPeninputLayoutEventSct,

    // ScrollUp Button
    EPeninputLayoutEventScrollUp, // prediction pane

    // ScrollDown Button
    EPeninputLayoutEventScrollDown, // prediction pane

    // Pointer up event for Japanese Input Context Field
    EEventIcfPointerUp,

    // Complete all event for Japanese Input Context Field
    EEventIcfCompleteAll,

    // for Hwr
    //Event Complete only if EStatusConversion or EStatusChangeDivision
    EEventCompleteIfConvertingHiraganaKanji,

    //Event SetDimmed Sct button
    EEventSetDimmedSctBtn,

    //Event Setect Item
    EPeninputLayoutEventSelectItem,

    //This definition is the last event
    EPenInputEventJpLast = 3000

    };

/**
 * Pen input control id for japanese
 */
enum TPeninputJpCtrlID
    {
    EPeninutWindowCtrlIdCandidatePopupWindow = 1000,
    EPeninutWindowCtrlIdPredictionPane,
    EPeninutWindowCtrlIdConvertToKanjiBtn,
    EPeninutWindowCtrlIdHiraganaBtn,
    EPeninutWindowCtrlIdKatakanaBtn,
    EPeninutWindowCtrlIdDropDownList,
    EPeninutWindowCtrlIdClear1Btn,   // 3 Unit
    EPeninutWindowCtrlIdClear2Btn,   // 1 Unit
    EPeninutWindowCtrlIdCharacterRangePopupWindow,
    EPeninutWindowCtrlIdScrollUpBtn,  // prediction pane
    EPeninutWindowCtrlIdScrollDownBtn // prediction pane
    };
#endif //T_TPENINPUTBEVENTJP_H

// End Of File
