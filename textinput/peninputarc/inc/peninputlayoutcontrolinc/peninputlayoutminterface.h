/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of layout interface
*
*/


#ifndef M_LAYOUTMINTERFACE_H
#define M_LAYOUTMINTERFACE_H
//#include <gdi.h>
//#include <bitdev.h>
//#include <peninputcmdparam.h>

/**
 * Class MTimerHandler
 *
 * Interface for sending time event handling
 *
 * @lib fepbasecontrol.lib
 *  @since S60 v4.0
 */
class MTimerHandler
    {
public:
    /**
     * HandleTimerOut. Handling time out event
     * @since S60 v4.0
     * @param aTimeType The timer type
     */                
    virtual void HandleTimerOut(TInt aTimeType) = 0;
    };


/**
 * MPositionObserver
 *
 * Interface for dragbar moving event. There can be lots of observer for a dragbar.
 * But when one observer processs the event, others wont't have a chance to process it.  
 * The processing order is revesed to the order which observers added. 
 * i.e.The last added observer will precess the event first
 *
 * @lib fepbasecontrol.lib
 * @since S60 v4.0
 */
class MPositionObserver
    {
public:
    /**
     * Handle position change event
     *
     * @since S60 v4.0
     * @param aOffset The moving offset
     * @return The flag whether this observer has processed the event.
     *         ETrue if processed.Otherwise EFalse.
     */            
    virtual TBool HandlePositionChange(const TPoint& aOffset) = 0;
    
     /**
      * return observer's area to be moved when position changed
      *
      * @since S60 v4.0
      * @return The rect of observer's moving area
      */            
    virtual const TRect& MovingArea() = 0;
    
    /**
     * Tell whether this observer is a fep base control
     *
     * @since S60 v4.0
     * @return ETrue if this observer is a control, otherwise, EFalse.
     */
    virtual TBool IsFepBaseControl() = 0;
    };

/**
 * MCandidateBuffer
 *
 * Interface for candidate data buffer
 *
 * @lib fepbasecontrol.lib 
 * @since S60 v4.0
 */
class MCandidateBuffer
    {
public:
    /**
     * Set candidate buffer 
     *
     * @since S60 v4.0
     * @param aCandidateData Candidate data
     */        
    virtual void SetCandidateL(const RPointerArray<HBufC>& aCandidateData) = 0;
    };

/**
 * MEventObserver
 *
 * Interface for event observer
 *
 * @lib fepbasecontrol.lib
 * @since S60 v4.0
 */
class CFepUiBaseCtrl;
class MEventObserver
    {
public:
    /**
     * Handle control event 
     *
     * @since S60 v4.0
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     */            
    virtual void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl,
                                                const TDesC& aEventData) = 0;
    };

/**
 * MOnResourceChangFunctions
 *
 * Interface for handling resource change
 *
 * @lib fepbasecontrol.lib
 * @since S60 v4.0
 */    
 class MOnResourceChangFunctions
    {
public:
    /**
     * Set the resource Id.
     *
     * @since S60 v4.0
     * @param aId The control resource Id
     */                 
    virtual void SetResourceId(TInt32 aResId) = 0;
    
    /**
     * Construct the control from resource again due to resource change.
     *
     * @since S60 v4.0
     */            
    virtual void ConstructFromResourceL() = 0;
    };

#endif //M_LAYOUTMINTERFACE_H