/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of peninput fullscreen HWR layout base
*
*/


#ifndef __PENINPUTFULLSCRLAYOUTBASE_H__
#define __PENINPUTFULLSCRLAYOUTBASE_H__

// INCLUDES
#include "peninputlayout.h"

class CPenInputFullScreenLayoutBase : public CFepUiLayout
    {
public:
    /**
     * Destructor.
     *
     * @since S60 v4.0
     */
    virtual ~CPenInputFullScreenLayoutBase();

    /**
     * BaseConstructL
     * Does the base construction. Derivated class must call this when constructed.
     *
     * @since S60 v4.0
     */
    virtual void BaseConstructL();

    /**
     * Constructor
     *
     * @since S60 v4.0
     * @param aLayoutOwner The layout owner
     * @param aDuration The maximum time to buffer the event.
     */
    CPenInputFullScreenLayoutBase( MLayoutOwner* aLayoutOwner );

    /**
     * From MFepLayoutBase
     * HandleEventL. Handle UI event.
     * There are only 3 kinds of UI event right now: raw event,
     * pointer and pointer buffer event.
     *
     * @since S60 v4.0
     * @param aType The event type. See TEventType
     * @param aEventData The event data.
     * @return The control which processes the event
     */
    TBool HandleEventL( TEventType aType, const TAny* aEventData );

    /**
     * SemiTransparencyRequired. Tell whether this layout requires semi-transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v5.0
     * @return ETrue if layout require transparency, otherwise EFalse.
     */
    TBool SemiTransparencyRequired();

    /**
     * Set stroke filter.
     *
     * @since S60 v4.0
     * @param aEnable stroke filter
     */
    void EnableStrokeFilter( TBool aEnable );
    
    /**
     * Get current stroke filter status
     *
     * @since S60 v5.0
     * @return stroke filter status
     */
    TBool StrokeFilter() const;
    
protected:
	/**
	* PreHandleEvent
	*
	* @since S60 v5.0
	*/
	virtual void PreHandleEvent( TEventType aType, const TRawEvent &aEvent );
    
    /**
    * Notify the subclass to do someting before sending the events to background
    *
    * @since S60 v5.0
    */
    virtual void BeforeSendingEventsToBackground();
    
private:
    /**
     * PeriodicCallBack. call back function for periodic timer object.
     *
     * @since S60 v5.0
     * @return KErrNone if no error.
     */
    static TInt PeriodicCallBack( TAny *aAnyPtr );

    /**
     * OnLongTapTimeOut. called when longtap periodic time-out.
     *
     * @since S60 v5.0
     * @return KErrNone if no error.
     */
    TInt OnLongTapTimeOut();

    /**
     * OnRawEvent_Button1Down. handle TRawEven::EButton1Down event.
     *
     * @since S60 v5.0
     * @return ETrue if event is handled, or return EFalse.
     */
    TBool OnRawEvent_Button1Down( const TRawEvent &event );

    /**
     * OnRawEvent_Button1Up. handle TRawEven::EButton1Up event.
     *
     * @since S60 v5.0
     * @return ETrue if event is handled, or return EFalse.
     */
	TBool OnRawEvent_Button1Up( const TRawEvent &event );

    /**
     * OnRawEvent_PointerMove. handle TRawEven::EPointerMove event.
     *
     * @since S60 v5.0
     * @return ETrue if event is handled, or return EFalse.
     */
	TBool OnRawEvent_PointerMove( const TRawEvent &event );

    /**
     * StartLongTapTimer. start long tap timer.
     *
     * @since S60 v5.0
     */
	void StartLongTapTimer();

    /**
     * StopLongTapTimer. stop long tap timer.
     *
     * @since S60 v5.0
     */
	void StopLongTapTimer();

    /**
     * RecordPointerEvent. add aEvent to iEventBuf.
     *
     * @since S60 v5.0
     */
	void RecordPointerEvent( const TRawEvent &aEvent );

    /**
     * SendBufferEventToLayout. send event in buffer to peninput layout and reset buffer.
     *
     * @since S60 v5.0
     */
	void SendBufferEventToLayoutL();

    /**
     * SendBufferEventToBackground. send event in buffer to background application and reset buffer.
     *
     * @since S60 v5.0
     */
	void SendBufferEventToBackground();

    /**
     * IsStroke. test if the pointer event in buffer is a stroke.
     *
     * @since S60 v5.0
     * @return ETure if it is a stroke, or return EFalse.
     */
	TBool IsStroke();

    /**
     * CalcDiagonalDistance. calculate the diagonal distance of the events in buffer.
     *
     * @since S60 v5.0
     * @return the diagonal distance value.
     */
	TReal CalcDiagonalDistance();

    /**
     * CalcAngle. calculate the anti-clockwise angle of the first and last pointer event pos in the buffer.
     *
     * @since S60 v5.0
     * @return the angle value.
     */
	TInt CalcAngle();

private:
	/**
	* pointer/pen state
	*/
	enum TPenState
		{
		EPenStateUnknown,
		EPenStateStroke,
		EPenStateLongTap,
		};
	TPenState iPenState;

    /**
     * Timer to check whether event needs simulatation or handling.
     * Own.
     */
    CPeriodic* iLongTapPeriodic;

    /*
     * Event buffer.
     */
    RArray<TRawEvent> iEventBuf;
    
    /*
     * Stroke filter
     */
    TBool iEnableStrokeFilter;
    };

#endif	//__PENINPUTFULLSCRLAYOUTBASE_H__
// End of file
