/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Enum values of events broadcasted by control to observers
*
*/


#ifndef T_PENINPUTHWRFSCNEVENT_H
#define T_PENINPUTHWRFSCNEVENT_H

#include <peninputlayoutbasecontrol.h>

/**
 *  TPeninputHwrfscnEvent
 *
 *  defines the events from internal controls. Those events are sent 
 *  by controls via calling ReportEvent() 
 *
 *  @lib peninputhwrfscn.lib
 *  @since S60 v5.0
 */
enum TPeninputHwrfscnEvent
    {
    //This definition is user close hwrwnd manully,
    //and must be reopened by EHwrCmdWindowOpen
    EHwrEventWindowClose = EEventControlUserBase,
    
    //This definition is user begin writing on box
    EHwrEventBeginWriting,
    
    //This definition is user begin writing on a non-active box
    EHwrEventNewWriting,

    //This definition is one stroke writing end on box
    EHwrEventEndStroke,

    //This definition is user finish writing on box for a while
    EHwrEventEndWriting,
    
    //This definition is user select a candidate
    EHwrEventCandidateSelected,
    
    //This definition is user select a predictive
    EHwrEventPredictiveSelected,
    
    //This definition is user click the backspace key
    EHwrEventKeyBack,
    
    // This definition is user click the enter key
    EHwrEventKeyEnter,
        
    //This definition is user click the space key
    EHwrEventKeySpace,
    
    //This definition is user click the more function button
    EHwrEventBtnMoreFunction,
    
    //This definition is user click the input language swithch button
//    EHwrEventInputLangSwitch,
    
    //This definition is user click the vkb button
    EHwrEventVkbSwitch,
    
    //This definition is user click the option button
    EHwrEventTouchInputOption,
    
    //This definition is user click somewhere outside the hwr window
    //EHwrEventPointerOutsideWnd,
    
    //This definition is user click the range button
    EHwrEventSetRange,
    
    //this definition is user click the chinese range button
    EHwrEventSetRangeChinese,
    
    //this definition is user click the chinese range button
    EHwrEventSetRangeEnglish,
    
    //this definition is user click the chinese range button
    EHwrEventSetRangeNumber,
    
    //this definition is user click the range control
    EHwrEventRangeClick,
    
    //This definition is user click the english button when it is current range
    EHwrEventSetCase,
    
    //This definition is user click out of window
    EHwrEventPointerOutsideWnd,
    
    //This definition is cusor position changed 
    EHwrEventCursorPosChanged,
    
    //This defintion is user click arrow-down of drop down list
    EHwrEventGetNextCandidatePage,
    
    //This defintion is user click arrow-up of drop down list
    EHwrEventGetPreviousCandidatePage,

    //This defintion is judge candidate exist
    EHwrEventCandidateExisted,
    
    EHwrEventClose,

    //This definition is the last event
    EHwrEventLast
    };

#endif //T_PENINPUTHWRFSCNEVENT_H

// End Of File
