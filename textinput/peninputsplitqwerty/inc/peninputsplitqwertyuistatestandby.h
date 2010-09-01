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

#ifndef C_PENINPUTSPLITQWERTYUISTATESTANDBY_H
#define C_PENINPUTSPLITQWERTYUISTATESTANDBY_H

// System includes
#include <peninputuistatebase.h>

// Forward declarations
class MPeninputUiStateMgr;
class MPeninputLayoutContext;

/**
 *  class CPeninputSplitQwertyUiStateStandby
 *
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */
class CPeninputSplitQwertyUiStateStandby : public CPeninputUiStateBase
    {

public:
    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return The pointer to CPeninputUiStateVkbStandby object
     */
    static CPeninputSplitQwertyUiStateStandby* NewL(
                                        MPeninputUiStateMgr* aUiStateMgr,
                                        MPeninputLayoutContext* aContext );

    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputSplitQwertyUiStateStandby();

public:
    // From base class CPeninputUiStateBase  

    /**
     * From CPeninputUiStateBase
     * Process key event
     *
     * @since S60 v5.0
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleKeyEventL( const TRawEvent& aData );

    /**
     * From CPeninputUiStateBase
     * process internal event
     *
     * @since S60 v5.0
     * @param aEventType The internal event type
     * @param aEventData The internal event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleControlEvent( TInt aEventType, const TDesC& aEventData );

protected:

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return None
     */
    CPeninputSplitQwertyUiStateStandby( MPeninputUiStateMgr* aUiStateMgr,
                                        MPeninputLayoutContext* aContext );

    /**
     * Process virtual key event
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleVkbEvent( TInt aEventType, const TDesC& aEventData );

    /**
     * Process backspace key event
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleBackEvent( TInt aEventType, const TDesC& aEventData );

    /**
     * Process enter, space key event
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleEnterSpaceEvent( TInt aEventType,
                                         const TDesC& aEventData );

    /**
     * Process tab key event
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleTabEvent( TInt aEventType, const TDesC& aEventData );

    };

#endif  // C_PENINPUTSPLITQWERTYUISTATESTANDBY_H
