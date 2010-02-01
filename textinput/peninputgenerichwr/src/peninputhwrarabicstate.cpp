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
#include "peninputhwrarabicstate.h"
#include "peninputhwrarabiclayout.h"

// constant definition

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBxAbStateBase

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateBase* CPeninputHwrBxAbStateBase::NewL(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBxAbStateBase* self = new ( ELeave ) CPeninputHwrBxAbStateBase(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::~CPeninputHwrBxAbStateBase
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateBase::~CPeninputHwrBxAbStateBase()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::HandleControlEvent
// (other items were commented in a header).
// return False --->iHiraganaKanji->HandleControlEventJpL() by UiLayout
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateBase::HandleControlEvent(TInt aEventType, const TDesC& aEventData)
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

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::CPeninputHwrBxAbStateBase
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateBase::CPeninputHwrBxAbStateBase(MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext)
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::HandleVkbEvent
// pseudo virual key (hwr 1 char)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateBase::HandleVkbEvent(TInt /*aEventType*/,
                                        const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateBase::HandleBackEvent(TInt /*aEventType*/,
                                         const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateBase::HandleEnterSpaceTabEvent(TInt /*aEventType*/,
                                                  const TDesC& /*aEventData*/)
    {
    return ETrue;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbStateBase::HandleControlEventPenL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbStateBase::HandleControlEventPenL( TInt /*aEventType*/, 
    const TDesC& /*aEventData*/, CPeninputHwrBxAbLayout& /*aUiLayout*/, TDes& /*aCharCode*/, TBool& aHandled)
    {
    aHandled = EFalse;
    }

// End Of File
