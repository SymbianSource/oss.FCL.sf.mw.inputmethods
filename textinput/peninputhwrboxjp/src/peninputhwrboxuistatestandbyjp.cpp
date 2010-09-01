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
* Description:  Implementation of the hwr ui state machine (Chinese Standby State)
*
*/


// System includes
#include <peninputuistatemgr.h>

// user includes
#include "peninputhwrboxuistatestandbyjp.h"
#include "peninputhwrboxuistatetype.h"         // UI state

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiStateStandbyjp

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandbyjp::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateStandbyjp* CPeninputHwrBoxUiStateStandbyjp::NewL(MPeninputUiStateMgr* aUiStateMgr, 
                                                             MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiStateStandbyjp* self = new ( ELeave ) CPeninputHwrBoxUiStateStandbyjp(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct(); 
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandbyjp::~CPeninputHwrBoxUiStateStandbyjp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateStandbyjp::~CPeninputHwrBoxUiStateStandbyjp()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandbyjp::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandbyjp::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandbyjp::CPeninputHwrBoxUiStateStandbyjp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateStandbyjp::CPeninputHwrBoxUiStateStandbyjp(MPeninputUiStateMgr* aUiStateMgr, 
                                                       MPeninputLayoutContext* aContext)
    :CPeninputHwrBoxUiState(aUiStateMgr,aContext)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandbyjp::HandleVkbEvent
// pseudo virual key (hwr 1 char)
// (other items were commented in a header).
// return False --->iHiraganaKanji->HandleControlEventJpL() by UiLayout
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandbyjp::HandleVkbEvent(TInt /*aEventType*/,
                                                 const TDesC& /*aEventData*/)
    {
    // change state only
    iUiStateMgr->SetCurrentUiState( iUiStateMgr->GetUiState(EPeninputHwrUiStateWithTransitoryChars) );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandbyjp::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandbyjp::HandleBackEvent(TInt /*aEventType*/,
                                                  const TDesC& aEventData)
    {
    return SendKey(aEventData);
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandbyjp::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandbyjp::HandleEnterSpaceTabEvent(TInt /*aEventType*/,
                                                           const TDesC& aEventData)
    {
    return SendKey(aEventData);
    }

// End Of File
