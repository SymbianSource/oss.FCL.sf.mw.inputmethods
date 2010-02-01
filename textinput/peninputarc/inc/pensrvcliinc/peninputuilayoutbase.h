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
* Description:  interface for pen input ui layout base
*
*/



#ifndef M_PENUILAYOUTBASE_H
#define M_PENUILAYOUTBASE_H

#include <peninputcmdparam.h>

//event type
enum TEventType
    {
    EPointerEvent,          //Pointer event
    EPointerBufferEvent,    //Pointer buffer event. This is used only for window based.
    ERawEvent,              //Raw event from window server
    //Event indicates the pointer up event happens to another control 
    //Which does not has the pointer down event. 
    EEventCancelPointerDown,    //Pointer is up in other control, cancel the down event
    ERawEventBuffer,    //raw event buffer. A collection of event buffered by server.
                        //The event is starts by a point down event, then followed 
                        // by serveral move events. It will ends either by a move event
                        // or a point up event.
                        // The count of event number is set in the buffer header.
                        // The buffer structure is:
                        /*
                           {
                           TInt nEventCount;
                           TRawEvent event[1];
                           }
                        */
    };


/**
 * Class MPenUiLayoutBase.
 *
 * Interface for fep layout base
 *
 * @since S60 V4.0
 */
class MPenUiLayoutBase
    {
public:
    /**
     * InitL. 
     * Do layout initialization. It's called by layout owner
     *
     * @since S60 v4.0
     * @retutn The layout rect including the shadow.
     */
    virtual TRect Init() = 0;

    /**
     * HandleEventL. Handle UI event.
     * There are only 3 kinds of UI event right now: raw event, 
     * pointer and pointer buffer event. 
     *
     * @since S60 v4.0
     * @param aType The event type. See TEventType
     * @param aEventData The event data.
     * @return ETrue if layout has processed the event, otherwise EFalse
     */
    virtual TBool HandleEventL(TEventType aType, 
                                                const TAny* aEventData) = 0;

    /**
     * HandleCommand. Handle layout command event.
     *
     * @since S60 v4.0
     * @param aCmd Command Id.
     * @param aData Data for command.
     * @return precessing result. Errors when return value small than 0.		
     */
    virtual TInt HandleCommand(TInt aCmd, TUint8* aData) = 0;
                                                    
    /**
     * Draw layout.
     *
     * @since S60 v4.0    
     */                                                          
    virtual void Draw() = 0;
    
    /**
     * SizeChanged. Inform layout the size changing event.
     *
     * @since S60 v4.0
     * @param aType The event type
     * @return KErrNone if successfully size changing. 
     */
    virtual TInt OnResourceChange(TInt aEventType) = 0;     

    /**
     * Handle editor text coming
     * Called by animation dll when app editor text is coming.
    
    /**
     * Destroy layout
     *
     * @since S60 v4.0     
     */    
    virtual void Destroy() = 0;
    
    /**
     * OnActivate. Called by owner when the layout is going to be shown.
     *
     * @since S60 v4.0
     */        
    virtual void OnActivate() = 0;
    
    /**
     * OnDeActivate. Called by owner when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    virtual void OnDeActivate() = 0;    

    /**
     * SemiTransparencyRequired. Tell whether this layout requires semi-transparency.
     * This is to save memory used in mask bitmap. If semi-transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v4.0
     * @return ETrue if layout require transparency, otherwise EFalse.
     */        
    virtual TBool SemiTransparencyRequired() = 0;     
    
    /**
     * Get current input UI type
     *
     * @since S60 v4.0
     * @return Current input UI type
     */
    virtual TInt PenInputType() = 0;
    
    /**
     * HandleAppInfoChange. Inform UI that application infomation changed.
     *
     * @since S60 v5.0
     * @param aInfo The information.
     * @param aType The information type
     */
    virtual void HandleAppInfoChange(const TDesC& aInfo, TPeninputAppInfo aType) = 0;    
    
    /**
     * SetPositionFromOutside. Inform UI that the position is change from outside of UI.
     *
     * @since S60 v5.0
     * @param aInfo The information.
     * @param aType The information type
     */
    virtual void SetPositionFromOutside(const TPoint& aNewPos) = 0;    
    };

#endif //M_PENUILAYOUTBASE_H

