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
* Description:  peninput generic vkb ui state standby class definition
*
*/


#ifndef C_PNEINPUTGENERICVKBUISTATESTANDBY_H
#define C_PNEINPUTGENERICVKBUISTATESTANDBY_H

// System includes
#include <peninputuistatebase.h>

// Forward declarations
class MPeninputUiStateMgr;
class MPeninputLayoutContext;

/**
 *  Vkb's standby ui state class
 *  This class define methods to handle event received at this ui state
 *
 *  @lib peninputgenericvkb.lib
 *  @since S60 v3.2
 */
class CPeninputUiStateVkbStandby : public CPeninputUiStateBase
    {

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return The pointer to CPeninputUiStateVkbStandby object
     */
    static CPeninputUiStateVkbStandby* NewL( MPeninputUiStateMgr* aUiStateMgr,
        MPeninputLayoutContext* aContext );
        
    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputUiStateVkbStandby();
        
// From base class CPeninputUiStateBase  

    /**
     * From CPeninputUiStateBase
     * Process key event
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleKeyEventL( const TRawEvent &aData );
    
    /**
     * From CPeninputUiStateBase
     * process internal event
     *
     * @since S60 v3.2
     * @param aEventType The internal event type
     * @param aEventData The internal event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleControlEvent( TInt aEventType, 
                                      const TDesC& aEventData );
    
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return None
     */
    CPeninputUiStateVkbStandby( MPeninputUiStateMgr* aUiStateMgr, 
                                MPeninputLayoutContext* aContext );

    /**
     * Process virtual key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleVkbEvent( TInt aEventType, const TDesC& aEventData );
    
    /**
     * Process backspace key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleBackEvent( TInt aEventType, 
                                   const TDesC& aEventData );
    
    /**
     * Process enter, space key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleEnterSpaceEvent( TInt aEventType, 
                                            const TDesC& aEventData );
    
    /**
     * Process tab key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleTabEvent( TInt aEventType, 
                                            const TDesC& aEventData );

    };
                
#endif  // C_PNEINPUTGENERICVKBUISTATESTANDBY_H
