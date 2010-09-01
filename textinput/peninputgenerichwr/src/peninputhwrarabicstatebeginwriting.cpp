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
* Description:  Implementation of the hwr ui state machine (Pen Beginwriting State)
*
*/


// System includes
#include <peninputuistatemgr.h>
#include <peninputcommonlayoutglobalenum.h>
// user includes
//#include "peninputhwrevent.h"
#include "peninputhwrarabic.hrh"
#include "peninputhwrarabiclayout.h"
#include "peninputhwrarabicdatamgr.h"       // use data mgr
#include "peninputhwrarabicwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrarabicstatebeginwriting.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBxAbStateBeginWriting

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBeginWriting::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateBeginWriting* CPeninputHwrBxAbStateBeginWriting::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBxAbStateBeginWriting* self = new ( ELeave ) CPeninputHwrBxAbStateBeginWriting(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBeginWriting::~CPeninputHwrBxAbStateBeginWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateBeginWriting::~CPeninputHwrBxAbStateBeginWriting()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbStateBeginWriting::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBxAbStateBeginWriting::HandleControlEventPenL( TInt aEventType, 
    const TDesC& /*aEventData*/, CPeninputHwrBxAbLayout& aUiLayout, TDes& aCharCode, TBool& aHandled )
    {
    aHandled = EFalse;
    switch (aEventType)
        {
        case EEventHwrStrokeFinished:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateEndWriting);
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->OnStrokeFinished();
            aHandled = ETrue;
            }
            break;
        case EEventHwrStrokeCanceled:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateStandby);
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->OnStrokeCanceled();
            aHandled = ETrue;
            }
            break;
        case EPeninputLayoutEventBack:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateStandby);
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->HandleBackspaceKeyEvent();
            aHandled = ETrue;
            }
            break;
        default:
            aHandled = EFalse;
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateBeginWriting::CPeninputHwrBxAbStateBeginWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateBeginWriting::CPeninputHwrBxAbStateBeginWriting(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBxAbStateBase(aUiStateMgr,aContext)
    {
    }

// End Of File
