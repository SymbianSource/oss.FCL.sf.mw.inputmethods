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
* Description:  Implementation of the hwr ui state
*
*/


// System includes
#include <peninputlayoutbasecontrol.h>  // EEventVirtualKeyUp, ...
#include <peninputcommonlayoutglobalenum.h>  // EPeninputLayoutEventBack, ...

// user includes
#include "peninputhwrboxuistate.h"

// constant definition

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiState

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiState::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiState* CPeninputHwrBoxUiState::NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiState* self = new ( ELeave ) CPeninputHwrBoxUiState(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiState::~CPeninputHwrBoxUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiState::~CPeninputHwrBoxUiState()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiState::HandleControlEvent
// (other items were commented in a header).
// return False --->iHiraganaKanji->HandleControlEventJpL() by UiLayout
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiState::HandleControlEvent(TInt aEventType, const TDesC& aEventData)
    {
    if (aEventType == EEventVirtualKeyUp)   // pseudo virual key (hwr 1 char)
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

// ---------------------------------------------------------------------------
// CPeninputHwrBoxUiState::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxUiState::HandleControlEventPenL(TInt /*aEventType*/
                                                    , const TDesC& /*aEventData*/
                                                    , CPeninputHwrBoxLayout& /*aUiLayout*/
                                                                   , TDes& /*aCharCode*/
                                                                   , TBool& aHandled)
    {
    aHandled = EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiState::CPeninputHwrBoxUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiState::CPeninputHwrBoxUiState(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext)
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiState::HandleVkbEvent
// pseudo virual key (hwr 1 char)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiState::HandleVkbEvent(TInt /*aEventType*/,
                                        const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiState::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiState::HandleBackEvent(TInt /*aEventType*/,
                                         const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiState::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiState::HandleEnterSpaceTabEvent(TInt /*aEventType*/,
                                                  const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// End Of File
