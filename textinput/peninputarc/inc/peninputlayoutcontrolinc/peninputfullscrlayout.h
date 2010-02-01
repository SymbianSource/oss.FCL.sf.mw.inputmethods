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
* Description:  header file of peninput fullscreen HWR ui layout
*
*/


#ifndef C_CFEPUI_FULLSCREEN_LAYOUT_H
#define C_CFEPUI_FULLSCREEN_LAYOUT_H

#include "peninputlayout.h"
// INCLUDES
class CLayoutTimer;
class CHwrWndBase;

const TInt KMaxBufferEventNum = 30;

class CFepFullScreenLayout : public CFepUiLayout,public MTimerHandler
    {
public:   
    /**
     * Destructor.
     *
     * @since S60 v4.0
     */
    IMPORT_C virtual ~CFepFullScreenLayout();

    /**
     * BaseConstructL
     * Does the base construction. Derivated class must call this when constructed.
     *
     * @since S60 v4.0
     */
    IMPORT_C virtual void BaseConstructL();    
        
    /**
     * Constructor
     *
     * @since S60 v4.0
     * @param aLayoutOwner The layout owner
     * @param aDuration The maximum time to buffer the event.
     */
    IMPORT_C CFepFullScreenLayout(MLayoutOwner* aLayoutOwner,
                                                    TInt aDuration = 0);
    
    /**
     * Get the buffered event
     *
     * @since S60 v4.0
     * @return The buffered event
     */
    inline const RArray<TRawEvent>& BufferedEvent();    
    
    /**
     * From MPenUiLayoutBase    
     * Init. 
     * Do layout initialization. It's called by layout owner
     *
     * @since S60 v4.0
     * @return The layout rect including the shadow rect
     */
    IMPORT_C TRect Init();
    
    /**
     * From MPenUiLayoutBase        
     * HandleEventL. Handle UI event.
     * There are only 3 kinds of UI event right now: raw event, 
     * pointer and pointer buffer event. 
     *
     * @since S60 v4.0
     * @param aType The event type. See TEventType
     * @param aEventData The event data.
     * @return The control which processes the event
     */
    IMPORT_C TBool HandleEventL(TEventType aType, const TAny* aEventData);

    /**
     * SemiTransparencyRequired. Tell whether this layout requires semi-transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v4.0
     * @return ETrue if layout require transparency, otherwise EFalse.
     */        
    IMPORT_C TBool SemiTransparencyRequired();        
    
    //from base class MTimerHandler
    /**
     * From MTimerHandler
     * HandleTimerOut. Handling time out event
     * @since S60 v4.0
     * @param aTimeType The timer type
     */                    
    IMPORT_C void HandleTimerOut(TInt aTimeType);
        
protected:
    /**
     * Test whether the buffered event is valid for a trace
     *
     * @since S60 v4.0
     * @return ETrue if buffered event consists of a valid stroke. Otherwise EFalse.
     */                    
    IMPORT_C virtual TBool IsValidStroke();

    /**
     * Test whether a new character starts. Default implementation is checking the 
     * HWR window stroke list
     *     
     * @since S60 v4.0
     * @return ETrue if a new character starts. Otherwise EFalse.
     */     
    IMPORT_C virtual TBool IsHwrNewCharacter();
        
private:
    
    /**
     * Start to buffer event
     *
     * @since S60 v4.0
     * @param aEvent The event to be buffered
     */                    
    void StartBufferEvent(TRawEvent& aEvent);
    
    /**
     * buffer event
     *     
     * @since S60 v4.0
     * @param aEvent The event to be buffered
     */     
    void BufferEventL(TRawEvent& aEvent);
    
    /**
     * Forward the bufferred event to input UI.
     *     
     * @since S60 v4.0
     */ 
    void HandleBufferedEventL();
    
    /**
     * Simulates the buffered event to other apps
     *     
     * @since S60 v4.0
     */ 
    void SimulateEvent();        
    
private:
    /**
     * Timer to check whether event needs simulatation or handling.
     * Own.
     */	
    CLayoutTimer* iBufferTimer;
    
    /**
     * Full-screen HWR window.
     * Not own.
     */	    
    CHwrWndBase* iHwrWnd;
    
    /*
     * Flag to tell whether current event should be buffered.
     */
    TBool iBufferEvent;
    
    /*
     * The left most position in current event buffer.
     */    
    TInt iXLeftPos;
    
    /*
     * The right most position in current event buffer.
     */        
    TInt iXRightPos;
    
    /*
     * The top most position in current event buffer.
     */        
    TInt iYTopPos;
    
    /*
     * The bottom most position in current event buffer.
     */        
    TInt iYBottomPos;     

    /*
     * pen size for Full-screen HWR window. It's used to keep the value.
     * pen size is set to 0 when event is bufferring.
     */    
    TSize iHwrWndPenSize;

    /*
     * Event buffer.
     */         
    RArray<TRawEvent> iEventBuf;//[KMaxBufferEventNum+1];
    
    /*
     * The buffering duration after pen down event. If pen is up within this duration,
     * The pen operation is considering to be a clicking instead of a writting.
     * The value is in micro-second unit.
     */        
    TInt iEventBufDruation;
    
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;	           
    };
    
#include "peninputfullscrlayout.inl"    
#endif  C_CFEPUI_FULLSCREEN_LAYOUT_H

// End of file
