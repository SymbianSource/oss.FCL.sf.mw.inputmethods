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
* Description:  Implementation of the hwr ui state machine (Pen Standby State)
*
*/


// System includes
#include <peninputuistatemgr.h>

// user includes
#include "peninputhwrevent.h"
#include "peninputhwrboxlayout.h"
#include "peninputhwrboxuistatetype.h"         // UI state
#include "peninputhwrboxwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrboxuistatepenstandby.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiStatePenStandby

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenStandby::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenStandby* CPeninputHwrBoxUiStatePenStandby::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiStatePenStandby* self = new ( ELeave ) CPeninputHwrBoxUiStatePenStandby(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenStandby::~CPeninputHwrBoxUiStatePenStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenStandby::~CPeninputHwrBoxUiStatePenStandby()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenStandby::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxUiStatePenStandby::HandleControlEventPenL(TInt aEventType
                                                                   , const TDesC& /*aEventData*/
                                                                   , CPeninputHwrBoxLayout& aUiLayout
                                                                   , TDes& /*aCharCode*/
                                                                   , TBool& aHandled)
    {
    aHandled = ETrue;
    switch (aEventType)
        {
        case EPeninputLayoutHwrEventBeginWriting:
            {
            aUiLayout.CompleteIfConvertingL();
            iUiStateMgr->SetCurrentUiState(EPeninputHwrUiStatePenBeginWriting);
            aUiLayout.HwrBoxWindow()->DimInputContextField(ETrue);    // BeginWriting::OnEntry()
            }
            break;
        default:
            aHandled = EFalse;
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenStandby::CPeninputHwrBoxUiStatePenStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenStandby::CPeninputHwrBoxUiStatePenStandby(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBoxUiState(aUiStateMgr,aContext)
    {
    }

// End Of File
