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
* Description:  Implementation of the vkb ui state machine (Standby State)
*
*/


// System includes
#include <peninputlayoutcontext.h>
#include <peninputdataconverter.h>

// user includes
#include "peninputvkb.hrh"      // EPeninputLayoutVkbEventResetShift
#include "peninputvkbuistatestandby.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputVkbUiStateStandby

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandby::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateStandby* CPeninputVkbUiStateStandby::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputVkbUiStateStandby* self = new ( ELeave ) CPeninputVkbUiStateStandby(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandby::~CPeninputVkbUiStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateStandby::~CPeninputVkbUiStateStandby()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandby::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandby::HandleKeyEventL(const TRawEvent& /*aData*/)
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandby::CPeninputVkbUiStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateStandby::CPeninputVkbUiStateStandby(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputVkbUiState(aUiStateMgr,aContext)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandby::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandby::HandleVkbEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    TBool rs = SendKey(aEventData);
   // If shift button is down
    TInt shiftDown = CPeninputDataConverter::AnyToInt
        ( iContext->RequestData( EPeninputDataTypeShiftDown ) );

    if ( shiftDown )
        {
        iContext->SendEventToWindow( EPeninputLayoutVkbEventResetShift,
                                     NULL );
        }
    return rs;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandby::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandby::HandleBackEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    return SendKey(aEventData);
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateStandby::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateStandby::HandleEnterSpaceTabEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    return SendKey(aEventData);
    }

// End Of File
