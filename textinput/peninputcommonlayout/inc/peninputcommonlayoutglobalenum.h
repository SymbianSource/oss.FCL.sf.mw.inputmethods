/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  the enumeration of peninputcommonlayout
*
*/


#ifndef PENINPUTCOMMONLAYOUTGLOBALENUM_H
#define PENINPUTCOMMONLAYOUTGLOBALENUM_H

/**  
 * Pen input range button response style
 */
enum TPeninputRangeResponseStyle
    {
    EPeninputRangeResponseNoAction           = 0,
    EPeninputRangeResponseSwitchRange        = 1,
    EPeninputRangeResponsePopupChoiceItem    = 2
    };

/**  
 * Pen input range extention
 */
enum TPeninputLayoutExtRange
    {
    ERangeNativeNumber          = 0x0020,
    ERangeMixedText             = 0x0040,  // e.g. arabic native text mixed with latin text 
    ERangeMixedNumber           = 0x0080   // e.g. arabic native number mixed with latin number    
    };

/**  
 * Pen input layout event
 */
enum TPeninputLayoutEvent
    {
    EPeninputLayoutEventClose = 1001, //starts from control event extended base
    EPeninputLayoutEventOpen,
    EPeninputLayoutEventOption,
    EPeninputLayoutEventEnter,
    EPeninputLayoutEventBack,
    EPeninputLayoutEventSwitchLanguage,
    EPeninputLayoutEventToHwr,
    EPeninputLayoutEventToVkb,
    EPeninputLayoutEventDataQueryPosition,
    EPeninputLayoutEventMovePosition,
    EPeninputLayoutEventRange,
    EPeninputLayoutEventRangeLoop,
    EPeninputLayoutEventShift,
    EPeninputLayoutEventCapslock,
    EPeninputLayoutEventSpace,
    EPeninputLayoutEventTab,

    EPeninputLayoutEventSetPermittedRange,
    EPeninputLayoutEventSetPermittedCase,
    EPeninputLayoutEventSetCase,
    EPeninputLayoutEventCloseWindow,
    EPeninputLayoutEventChangeInputMode,
    
    EPeninputLayoutEventLeftArrow,
    EPeninputLayoutEventRightArrow,    
    
	EPeninputLayoutEventMultiRange,
    EPeninputLayoutEventLast = 2000  // user event starts from here, same as base control
    };

/**  
 * Pen input common control id
 */
enum TPeninputCommonCtrlID
    {
    EPeninutWindowCtrlIdBaseWindow = 0,

    // Controls in the titlepane
    EPeninutWindowCtrlIdMoveBtn,
    EPeninupWindowCtrlIdArrowLeftBtn,// new added by vv
    EPeninupWindowCtrlIdArrowRightBtn,// new added by vv
    EPeninutWindowCtrlIdCloseBtn,
    EPeninutWindowCtrlIdOptionBtn,
    EPeninutWindowCtrlIdInputContextField,

    EPeninutWindowCtrlIdVkbCtrl,
    EPeninutWindowCtrlIdBackspaceBtn,
    EPeninutWindowCtrlIdEnterBtn,    
    EPeninutWindowCtrlIdInputLangSwitcherBtn,

    // Controls in the next line    
    EPeninutWindowCtrlIdTabBtn,
    EPeninutWindowCtrlIdCapslockBtn,
    EPeninutWindowCtrlIdShiftBtn,
    EPeninutWindowCtrlIdSpaceBtn,
    EPeninutWindowCtrlIdSwitchToHwrBtn,
    EPeninutWindowCtrlIdSwitchToVkbBtn,

    EPeninutWindowCtrlIdRangeBar,

    // Button Id in range bar
    EPeninutWindowCtrlIdNativeCharBtn,
    EPeninutWindowCtrlIdNativeNumberBtn,
    EPeninutWindowCtrlIdLatinCharBtn,
    EPeninutWindowCtrlIdLatinNumberBtn,
    EPeninutWindowCtrlIdSymbolBtn,
    EPeninutWindowCtrlIdAccentedBtn,   

    // Hwr box
    EPeninutWindowCtrlIdHwrBox,
    
    // Pop up window
    EPeninutWindowCtrlIdAccentPopupWindow,
    EPeninutWindowCtrlIdSwitcherPopupWindow,
    EPeninputWindowCtrlIdMultiLineICF,
    //candidate list
    EPentinputHwrCandidateList,    
	EPeninutWindowCtrlIdMultiRangeBtn,
    EPeninputWindowCtrlIdLastOne,
    
    EPeninutWindowCtrlIdRangeBtn //todo split view
    };

/**  
 * Pen input postion change style
 */
enum TPeninputPositionChangeStyle
    {
    EPeninputPositionChangeNone      = 0,
    EPeninputPositionChangeTlJustify = 1,
    EPeninputPositionChangeBrJustify = 2,
    EPeninputPositionChangeDataQuery = 3    
    };

enum TPeninputGuideLineStyle
    {
    EPeninputGuideLineNone               = 0,
    EPeninputGuideLineTop                = 1,
    EPeninputGuideLineBottom             = 2,
    EPeninputGuideLineBoth               = 3
    };
            
#endif // PENINPUTCOMMONLAYOUTGLOBALENUM_H
