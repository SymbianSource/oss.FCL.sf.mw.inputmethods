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

#include "PeninputVkbKrDbg.h"
#include "PeninputVkbKrUiStateNone.h"

// ---------------------------------------------------------------------------
// Constructors & Destructors
// ---------------------------------------------------------------------------
//

CPeninputVkbKrUiStateNone* CPeninputVkbKrUiStateNone::NewLC(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    {
    CPeninputVkbKrUiStateNone* self =
        new (ELeave) CPeninputVkbKrUiStateNone(aUiStateMgr,aContext,aPtiEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPeninputVkbKrUiStateNone* CPeninputVkbKrUiStateNone::NewL(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    {
    CPeninputVkbKrUiStateNone* self = 
        CPeninputVkbKrUiStateNone::NewLC(aUiStateMgr,aContext,aPtiEngine);
    CleanupStack::Pop(); // self;
    return self;
    }

CPeninputVkbKrUiStateNone::CPeninputVkbKrUiStateNone(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    : CPeninputVkbKrUiStateBase(aUiStateMgr,aContext,aPtiEngine)
    {
    // No implementation required
    }

void CPeninputVkbKrUiStateNone::ConstructL()
    {
    CPeninputVkbKrUiStateBase::ConstructL();
    }

CPeninputVkbKrUiStateNone::~CPeninputVkbKrUiStateNone()
    {
    }

// ---------------------------------------------------------------------------
// from CPeninputVkbKrUiStateBase
// ---------------------------------------------------------------------------
//

TBool CPeninputVkbKrUiStateNone::HandleKeyEventL(
        const TRawEvent& aData)
    {
    LOG2(
        "Vkb.UiState.None.HandleKeyEventL %d %d",
        aData.Modifiers(),
        aData.ScanCode());
    TBool ret(EFalse);
    return ret;
    }

TBool CPeninputVkbKrUiStateNone::HandleControlEvent(
        TInt aEventType,
        const TDesC& /*aEventData*/)
    {
    LOG1(
        "Vkb.UiState.None.HandleControlEvent %d",
        aEventType);
    TBool ret(EFalse);
    return ret;    
    }
