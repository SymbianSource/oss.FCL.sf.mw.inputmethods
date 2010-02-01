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
* Description:
*
*/
#ifndef T_TAKNFEPVKBEVENT_H
#define T_TAKNFEPVKBEVENT_H

#include <peninputlayoutbasecontrol.h>

enum TAknFepVkbEvent
    {
    //This definition is user close manully,
    //and must be reopened by EVkbCmdWindowOpen
    EVkbEventWindowClose = EEventControlUserBase + 1,

    //This definition is user click the option button
    EVkbEventTouchInputOption,
            
    //This event is sent when there is no any chars in composition field
    EVkbEventCompFieldNoChars,

    //This event is sent when composition field submits its string
    EVkbEventCompFieldAnalysisReq,

      //Send recognize string to compostion field
    EVkbEventCompFieldAnalysisResponse,
       
    //submit all candidates when no char code left to convert
    EVkbEventCompFieldSubmit,

    //flush data in composition to fep
    EVkbEventCompFieldDataFlush,
    
    //This event is sent when the cursor changes in composition field
    EVkbEventCompFieldCursorChange,

    //This definition is user select a candidate
    EVkbEventCandidateSelected,

    //This definition is user click the backspace key

    //This definition is user click the enter key
    EVkbEventKeyEnter,

    //This definition is user click the tab button

    //This definition is user click the space button
    EVkbEventKeySpace,
    
    //This definition is user click the capslock button
    EVkbEventKeyCapslock,

    //This definition is user click the shift button
    EVkbEventKeyShift,

    //This definition is user click the input language swithch button
    EVkbEventInputLangSwitch,
    
    //This definition is user click the hwr button
    EVkbEventHwr,

    //This definition is user click the range button
    EVkbEventSetRange,

    //Set permitted range
    EVkbEventSetPermittedRange,

    // Notify control(such as composition field and drop down list) to clear text, if needed
    EVkbEventClearContent,
    
    //Event when candidate list from engine changed
    EVkbEventCandidatesChanged,
    
    //Event notify reset shift and capslock
    EVkbEventResetShiftCapslock,
    
    //Event notify reset shift
    EVkbEventResetShift,
    
    //Evnet notify language change
    EVkbEventLanguageChanged,
    
    //Evnet notify position changed
    EVkbEventPositionChanged,

    //Event notify vkb layout change from standby to composition
    EVkbEventStandby2Composition,
    
    //Event notify vkb layout change from composition to standby
    EVkbEventComposition2Standby,
    
    //Event when user press arrow-down of drop down list
    EVkbEventGetNextPageCandidate,
    
    //Event when user press arrow-up of drop down list
    EVkbEventGetPreviousPageCandidate,
    
    //Event to query if next candidate exist
    EVkbEventQueryCandidateExisted,
    
    //Event to left move cursor
    EVkbEventLeftArrow,
    
    //Event to right move cursor
    EVkbEventRightArrow,
    
    //Event to up move cursor
    EVkbEventUpArrow,
    
    //Event to down move cursor
    EVkbEventDownArrow,
    
    //This definition is the last event
    EVkbEventLast
    };

#endif //T_TAKNFEPVKBEVENT_H

// End Of File
