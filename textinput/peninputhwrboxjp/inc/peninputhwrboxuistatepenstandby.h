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
* Description:  Interface of hwr pen standby ui state
*
*/


#ifndef C_PENINPUTHWRBOXUISTATEPENSTANDBY_H
#define C_PENINPUTHWRBOXUISTATEPENSTANDBY_H

// user includes
#include "peninputhwrboxuistate.h"

/**
 *  Hwr pen standby ui state
 *  It provides key event handling and internal event handling in pen standby ui state
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxUiStatePenStandby : public CPeninputHwrBoxUiState
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     * @return The pointer point to CPeninputHwrBoxUiStatePenStandby type object
     */
    static CPeninputHwrBoxUiStatePenStandby* NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CPeninputHwrBoxUiStatePenStandby();

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
    void HandleControlEventPenL(TInt aEventType, const TDesC& aEventData, CPeninputHwrBoxLayout& aUiLayout, TDes& aCharCode, TBool& aHandled);

protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     */
    CPeninputHwrBoxUiStatePenStandby(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);
    };

#endif  // C_PENINPUTHWRBOXUISTATEPENSTANDBY_H

// End Of File
