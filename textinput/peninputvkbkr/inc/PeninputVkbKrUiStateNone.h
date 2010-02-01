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
* Description:  peninput VKB data manager
*
*/

#ifndef PENINPUTVKBKRUISTATENONE_H
#define PENINPUTVKBKRUISTATENONE_H

#include "PeninputVkbKrUiStateBase.h"

class CPtiEngine;

class CPeninputVkbKrUiStateNone : public CPeninputVkbKrUiStateBase
    {
public:
    
    static CPeninputVkbKrUiStateNone* NewLC(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    static CPeninputVkbKrUiStateNone* NewL(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    virtual ~CPeninputVkbKrUiStateNone();

private:

    CPeninputVkbKrUiStateNone(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    void ConstructL();
    
private: // from CPeninputVkbKrUiStateBase 
    
    virtual TBool HandleKeyEventL(
        const TRawEvent& aData);
    
    virtual TBool HandleControlEvent(
        TInt aEventType,
        const TDesC& aEventData);

    };

#endif // PENINPUTVKBKRUISTATENONE_H
