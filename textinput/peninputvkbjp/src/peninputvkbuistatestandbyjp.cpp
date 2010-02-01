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
* Description:  Implementation of the vkb ui state machine (Chinese Standby State)
*
*/


// System includes
#include <peninputuistatemgr.h>

// user includes
#include "peninputvkbuistatestandbyjp.h"
#include "peninputvkbuistatetype.h"         // UI state

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputVkbUiStateStandbyjp

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandbyjp::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateStandbyjp* CPeninputVkbUiStateStandbyjp::NewL(MPeninputUiStateMgr* aUiStateMgr, 
                                                             MPeninputLayoutContext* aContext)
    {
    CPeninputVkbUiStateStandbyjp* self = new ( ELeave ) CPeninputVkbUiStateStandbyjp(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct(); 
    CleanupStack::Pop(self);
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandbyjp::~CPeninputVkbUiStateStandbyjp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateStandbyjp::~CPeninputVkbUiStateStandbyjp()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandbyjp::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandbyjp::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandbyjp::CPeninputVkbUiStateStandbyjp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateStandbyjp::CPeninputVkbUiStateStandbyjp(MPeninputUiStateMgr* aUiStateMgr, 
                                                       MPeninputLayoutContext* aContext)
    :CPeninputVkbUiState(aUiStateMgr,aContext)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandbyjp::HandleVkbEvent
// (other items were commented in a header).
// return False --->iVkbWindow->HandleControlEvent() by UiLayout
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandbyjp::HandleVkbEvent(TInt /*aEventType*/,
                                                 const TDesC& /*aEventData*/)
    {
    // change state only
    iUiStateMgr->SetCurrentUiState( iUiStateMgr->GetUiState(EPeninputVkbUiStateWithTransitoryChars) );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandbyjp::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandbyjp::HandleBackEvent(TInt /*aEventType*/,
                                                  const TDesC& aEventData)
    {
    return SendKey(aEventData);  
    }
    
// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandbyjp::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandbyjp::HandleEnterSpaceTabEvent(TInt /*aEventType*/,
                                                           const TDesC& aEventData)  
    {
    return SendKey(aEventData);
    }

// End Of File
