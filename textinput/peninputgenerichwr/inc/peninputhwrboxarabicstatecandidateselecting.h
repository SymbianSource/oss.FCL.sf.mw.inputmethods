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
* Description:  Interface of hwr pen candidateselecting ui state
*
*/


#ifndef C_PENINPUTHWRBOXUISTATEPENCANDIDATESELECTING_H
#define C_PENINPUTHWRBOXUISTATEPENCANDIDATESELECTING_H

// user includes
#include "peninputhwrarabicstate.h"

/**
 *  Hwr pen candidateselecting ui state
 *  It provides key event handling and internal event handling in pen candidateselecting ui state
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBxAbStateCandidateSelecting : public CPeninputHwrBxAbStateBase
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     * @return The pointer point to CPeninputHwrBxAbStateCandidateSelecting type object
     */
    static CPeninputHwrBxAbStateCandidateSelecting* NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CPeninputHwrBxAbStateCandidateSelecting();

    /**
     * From CPeninputUiStateBase
     * process internal event
     *
     * @since S60 v3.2
     * @param aEventType The internal event type
     * @param aEventData The internal event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleControlEvent(TInt /*aEventType*/, const TDesC& /*aEventData*/) {return EFalse;}
     
    void HandleControlEventPenL( TInt aEventType, 
                                      const TDesC& aEventData, CPeninputHwrBxAbLayout& aUiLayout, TDes& aCharCode, TBool& aHandled  );
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The common ui state manager
     * @param aContext The common layout context
     */
    CPeninputHwrBxAbStateCandidateSelecting(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext);
    };

#endif  // C_PENINPUTHWRBOXUISTATEPENCANDIDATESELECTING_H

// End Of File
