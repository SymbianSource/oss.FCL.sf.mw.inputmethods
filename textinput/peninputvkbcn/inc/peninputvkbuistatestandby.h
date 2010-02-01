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
* Description:  Interface of vkb standby ui state
*
*/


#ifndef C_AKNFEPVKBUISTATESTANDBY_H
#define C_AKNFEPVKBUISTATESTANDBY_H

// user includes
#include "peninputvkbuistate.h"

/**
 *  Vkb standby ui state
 *  It provides key event handling and internal event handling in standby ui state
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbUiStateStandby : public CAknFepVkbUiState
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return The pointer point to CAknFepVkbUiStateStandby type object
     */
    static CAknFepVkbUiStateStandby* NewL(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                          MAknFepVkbLayoutContext* aContext);
        
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CAknFepVkbUiStateStandby();
        
    /**
     * Process key event
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleKeyEventL(const TRawEvent &aData);     
        
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     */
    CAknFepVkbUiStateStandby(MAknFepVkbUiStateMgr* aUiStateMgr, 
                             MAknFepVkbLayoutContext* aContext);
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    //void ConstructL();
    
    /**
     * Process virtual key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleVkbEvent(TInt aEventType, const TDesC& aEventData);

    /**
     * Process backspace key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleBackEvent(TInt aEventType, const TDesC& aEventData);

    /**
     * Process enter, space, tab key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleEnterSpaceEvent(TInt aEventType, const TDesC& aEventData);   
    
    /**
     * Process tab key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleTabEvent(TInt aEventType, const TDesC& aEventData);
    
	virtual TBool HandleArrowKeyEvent(TInt aEventType, const TDesC& aEventData);        
    };

#endif  // C_AKNFEPVKBUISTATESTANDBY_H

// End Of File
