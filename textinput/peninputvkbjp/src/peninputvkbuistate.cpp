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
* Description:  Implementation of the vkb ui state machine
*
*/


// System includes
#include <peninputlayoutbasecontrol.h>  // EEventVirtualKeyUp
#include <peninputcommonlayoutglobalenum.h>

// user includes
#include "peninputvkbuistate.h"

// constant definition

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputVkbUiState

// -----------------------------------------------------------------------------
// CPeninputVkbUiState::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiState* CPeninputVkbUiState::NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext)
    {
    CPeninputVkbUiState* self = new ( ELeave ) CPeninputVkbUiState(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiState::~CPeninputVkbUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiState::~CPeninputVkbUiState()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiState::HandleControlEvent
// (other items were commented in a header).
// return False --->iVkbWindow->HandleControlEvent() by UiLayout
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiState::HandleControlEvent(TInt aEventType, const TDesC& aEventData)
    {
    if (aEventType == EEventVirtualKeyUp)
        {
        return HandleVkbEvent(aEventType, aEventData);
        }
    else if (aEventType == EPeninputLayoutEventBack)
        {
        return HandleBackEvent(aEventType, aEventData);
        }
    else if ((aEventType == EPeninputLayoutEventTab) ||
             (aEventType == EPeninputLayoutEventEnter) ||
             (aEventType == EPeninputLayoutEventSpace))
        {
        return HandleEnterSpaceTabEvent(aEventType, aEventData);
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiState::CPeninputVkbUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiState::CPeninputVkbUiState(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext)
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiState::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiState::HandleVkbEvent(TInt /*aEventType*/,
                                        const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiState::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiState::HandleBackEvent(TInt /*aEventType*/,
                                         const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiState::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiState::HandleEnterSpaceTabEvent(TInt /*aEventType*/,
                                                  const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// End Of File
