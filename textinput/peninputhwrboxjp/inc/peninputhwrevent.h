/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef T_TAKNFEPHWRBXEVENT_H
#define T_TAKNFEPHWRBXEVENT_H

/**
 *  TPeninputLayoutHwrEvent
 *
 *  defines the events from internal controls. Those events are sent 
 *  by controls via calling ReportEvent() 
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
// EPenInputEventJpLast = 3000      // hwr event starts from here
enum TPeninputLayoutHwrEvent
    {
    //This definition is user begin writing on box
    EPeninputLayoutHwrEventBeginWriting = 3000,
    
    //This definition is user begin writing on a non-active box
    EPeninputLayoutHwrEventNewWriting,

    //This definition is one stroke writing end on box
    EPeninputLayoutHwrEventEndStroke,

    //This definition is user finish writing on box for a while
    EPeninputLayoutHwrEventEndWriting,
    
    //This definition is user select a candidate
    EPeninputLayoutHwrEventCandidateSelected,

    //This definition is user click out of window
    EPeninputLayoutHwrEventPointerOutsideWnd,

    EPeninputLayoutHwrEventStrokeEndMark,

    // set candidate list to the dropdownlist control
    EPeninputLayoutHwrEventCandidates,

    // dropdownlist expanded
    EPeninputLayoutHwrEventDropDownListExpanded,

    // show character range switch popup
    EPeninputLayoutEventSwitchCharacterRange,

    // close character range switch popup
    EPeninputLayoutEventCloseCharacterRange,

    EPeninputLayoutEventSetCharacterRangeText,

    // Conversion button disable/enable
    EPeninputLayoutEventSetConversion,

    // from generic
    EPeninputLayoutHwrEventPenColorChange,
    EPeninputLayoutHwrEventPenSizeChange,    
    EPeninputLayoutHwrEventWritingSpeedChange,

    //This definition is the last event
    EPeninputLayoutHwrEventLast
    };

/**  
 * Hwr Character Range id
 */
enum TPeninputCharacterRangeId
    {
    EPeninutCharacterRangeIdAll,
    EPeninutCharacterRangeIdKanaKanji,
    EPeninutCharacterRangeIdLatin,
    EPeninutCharacterRangeIdNumber,
    EPeninutCharacterRangeIdLast
    };

#endif //T_TAKNFEPHWRBXEVENT_H

// End Of File
