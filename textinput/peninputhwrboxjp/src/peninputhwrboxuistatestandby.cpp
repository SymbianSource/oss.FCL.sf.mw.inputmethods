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
* Description:  Implementation of the hwr ui state machine (Standby State)
*
*/


// System includes
#include <aknfeppeninputenums.h>    // ERangeNumber
#include <peninputlayoutcontext.h>
#include <peninputdataconverter.h>

// user includes
#include "peninputhwrboxuistatestandby.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiStateStandby

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandby::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateStandby* CPeninputHwrBoxUiStateStandby::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiStateStandby* self = new ( ELeave ) CPeninputHwrBoxUiStateStandby(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandby::~CPeninputHwrBoxUiStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateStandby::~CPeninputHwrBoxUiStateStandby()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandby::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandby::HandleKeyEventL(const TRawEvent& /*aData*/)
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandby::CPeninputHwrBoxUiStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateStandby::CPeninputHwrBoxUiStateStandby(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBoxUiState(aUiStateMgr,aContext)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandby::HandleVkbEvent
// pseudo virual key (hwr 1 char)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandby::HandleVkbEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    TBool rs = SendKey(aEventData);
    return rs;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandby::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandby::HandleBackEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    return SendKey(aEventData);

    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateStandby::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateStandby::HandleEnterSpaceTabEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    TInt permittedRange = CPeninputDataConverter::AnyToInt
        ( iContext->RequestData( EPeninputDataTypePermittedRange ) );
    if (permittedRange == ERangeNumber)
        {
        return ETrue;
        }
    else
        {
        return SendKey(aEventData);
        }
    }

// End Of File
