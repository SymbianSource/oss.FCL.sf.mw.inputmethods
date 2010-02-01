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
* Description:  Interface of hwr standby jp ui state
*
*/


#ifndef C_PENINPUTHWRBOXUISTATESTANDBYJP_H
#define C_PENINPUTHWRBOXUISTATESTANDBYJP_H

// user includes
#include "peninputhwrboxuistate.h"

/**
 *  Hwr standby jp ui state
 *  It provides key event handling and internal event handling in standby jp ui state
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxUiStateStandbyjp : public CPeninputHwrBoxUiState
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     * @return The pointer point to CPeninputHwrBoxUiStateStandbyjp type object
     */
    static CPeninputHwrBoxUiStateStandbyjp* NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);
        
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CPeninputHwrBoxUiStateStandbyjp();
        
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
    CPeninputHwrBoxUiStateStandbyjp(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);

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

#endif  // C_PENINPUTHWRBOXUISTATESTANDBYJP_H

// End Of File
