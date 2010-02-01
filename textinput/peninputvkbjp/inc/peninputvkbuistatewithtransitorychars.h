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
* Description:  Interface of vkb composition with chars ui state
*
*/


#ifndef C_PENINPUTVKBUISTATEWITHTRANSITORYCHARS_H
#define C_PENINPUTVKBUISTATEWITHTRANSITORYCHARS_H

// user includes
#include "peninputvkbuistate.h"

// class declarations
/**
 *  with  transitory chars ui state
 *  It provides key event handling and internal event handling in with  transitory chars state
 *
 *  @lib peninputvkbjp.lib
 *  @since S60 v3.2
 */
class CPeninputVkbUiStateWithTransitoryChars : public CPeninputVkbUiState
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     * @return The pointer point to CPeninputVkbUiStateWithTransitoryChars type object
     */
    static CPeninputVkbUiStateWithTransitoryChars* NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CPeninputVkbUiStateWithTransitoryChars();

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
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     */
    CPeninputVkbUiStateWithTransitoryChars(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);

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
    virtual TBool HandleEnterSpaceTabEvent(TInt aEventType, const TDesC& aEventData);   
    };

#endif  // C_PENINPUTVKBUISTATEWITHTRANSITORYCHARS_H

// End Of File
