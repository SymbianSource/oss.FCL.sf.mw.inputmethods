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

// user includes
#include "peninputhwrevent.h"
#include "peninputhwrboxlayout.h"
#include "peninputhwrboxdatamgr.h"       // use data mgr
#include "peninputhwrboxwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrboxuistatetype.h"         // UI state
#include "peninputhwrboxuistatepenbeginwriting.h"

// constant definition
const TUint16 KEWBackRep = 0x2408;
const TUint16 KEWEnterRep = 0xF801;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiStatePenBeginWriting

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenBeginWriting::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenBeginWriting* CPeninputHwrBoxUiStatePenBeginWriting::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiStatePenBeginWriting* self = new ( ELeave ) CPeninputHwrBoxUiStatePenBeginWriting(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenBeginWriting::~CPeninputHwrBoxUiStatePenBeginWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenBeginWriting::~CPeninputHwrBoxUiStatePenBeginWriting()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenBeginWriting::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxUiStatePenBeginWriting::HandleControlEventPenL(TInt aEventType
                                                                   , const TDesC& aEventData
                                                                   , CPeninputHwrBoxLayout& aUiLayout
                                                                   , TDes& aCharCode
                                                                   , TBool& aHandled)
    {
    switch (aEventType)
        {
        case EPeninputLayoutHwrEventEndStroke:
            {
            const RArray<TPoint>* aTraceData = (RArray<TPoint>*)aEventData.Ptr();
            aUiLayout.DataMgr(0)->DoRecognize(*aTraceData);
            iUiStateMgr->SetCurrentUiState(EPeninputHwrUiStatePenEndWriting);
            aUiLayout.DataMgr(0)->GetCandidate(0, aCharCode);
            aUiLayout.SubmitL(aCharCode);
            if (aUiLayout.DataMgr(0)->CandidateCount() > 0)
                {
                //hight the first
                aUiLayout.HwrBoxWindow()->HighlightCell(0);
                }
            TPtrC16 ptrBack;
            TPtrC16 ptrEnter;
            ptrBack.Set(&KEWBackRep, sizeof(KEWBackRep)/sizeof(TUint16));
            ptrEnter.Set(&KEWEnterRep, sizeof(KEWEnterRep)/sizeof(TUint16));
            //if char code is backspace or enter
            if (aCharCode.CompareC(ptrBack) == 0 || aCharCode.CompareC(ptrEnter) == 0)
                {
                aUiLayout.ToPenStandBy();
                }
            aUiLayout.HwrBoxWindow()->DimInputContextField(EFalse);   // BeginWriting::OnExit()
            }
            break;
        default:
            break;
        }
    aHandled = ETrue;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenBeginWriting::CPeninputHwrBoxUiStatePenBeginWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenBeginWriting::CPeninputHwrBoxUiStatePenBeginWriting(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBoxUiState(aUiStateMgr,aContext)
    {
    }

// End Of File
