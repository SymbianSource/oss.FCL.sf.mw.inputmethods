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
* Description:  Enum values of events broadcasted by control to observers
*
*/

#ifndef PENINPUTFINGERHWREVENT_H
#define PENINPUTFINGERHWREVENT_H

#include <peninputlayoutbasecontrol.h>

/**
 *  TPeninputFingerHwrEvent
 *
 *  defines the events from internal controls. Those events are sent 
 *  by controls via calling ReportEvent() 
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
enum TPeninputFingerHwrEvent
    {
    //This definition is user close hwrwnd manully,
    //and must be reopened by EHwrCmdWindowOpen
    EHwrEventWindowClose = EEventControlUserBase,
    
    //This definition is user select a candidate
    EHwrEventCandidateSelected,
    
    //This definition is user select a predictive
    EHwrEventPredictiveSelected,
    
    //This definition is user click the backspace key
    EHwrEventKeyBack,
    
    //This defintion is user click arrow-down of drop down list
    EHwrEventGetNextCandidatePage,
    
    
    EHwrEventClose,

    EHwrEventNavKeyClicked,
    
    //This definition is the last event
    EHwrEventLast
    };

#endif //PENINPUTFINGERHWREVENT_H

// End Of File