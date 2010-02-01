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
* Description:  Interface of hwr ui state
*
*/


#ifndef C_PENINPUTHWRBOXUISTATE_H
#define C_PENINPUTHWRBOXUISTATE_H

// system includes
#include <peninputuistatebase.h>

class CPeninputHwrBoxLayout;

// user includes

// class declarations

/**
 *  Basic class of hwr ui state
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxUiState : public CPeninputUiStateBase
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     * @return The pointer point to CPeninputHwrBoxUiState type object
     */
    static CPeninputHwrBoxUiState* NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CPeninputHwrBoxUiState();

    // from MPeninputHwrBoxUiState  

    /**
     * process internal event
     *
     * @since S60 v3.2
     * @param aEventType The internal event type
     * @param aEventData The internal event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleControlEvent(TInt aEventType, const TDesC& aEventData);

    /**
     * Handle the event for Pen from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @param aUiLayout  The UiLayout Control
     * @param aCharCode  character code buffer
     * @param aHandled   output buffer:ETrue means event was responsed, otherwise EFalse
     * @return None
     */
    virtual void HandleControlEventPenL(TInt aEventType, const TDesC& aEventData, CPeninputHwrBoxLayout& aUiLayout, TDes& aCharCode, TBool& aHandled);

protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     */
    CPeninputHwrBoxUiState(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);

    /**
     * Process pseudo virtual key (hwr 1 char) event
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

#endif  // C_PENINPUTHWRBOXUISTATE_H

// End Of File
