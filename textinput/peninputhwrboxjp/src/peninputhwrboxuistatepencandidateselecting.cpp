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
* Description:  Implementation of the hwr ui state machine (Pen CandidateSelecting State)
*
*/


// System includes
#include <peninputuistatemgr.h>

#include "peninputhwrevent.h"
#include "peninputhwrboxlayout.h"
#include "peninputhwrboxdatamgr.h"       // use data mgr
#include "peninputhwrboxuistatetype.h"         // UI state
#include "peninputhwrboxwindow.h"        // use hwr window, which derived from base window
// user includes
#include "peninputhwrboxuistatepencandidateselecting.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiStatePenCandidateSelecting

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenCandidateSelecting::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenCandidateSelecting* CPeninputHwrBoxUiStatePenCandidateSelecting::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiStatePenCandidateSelecting* self = new ( ELeave ) CPeninputHwrBoxUiStatePenCandidateSelecting(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenCandidateSelecting::~CPeninputHwrBoxUiStatePenCandidateSelecting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenCandidateSelecting::~CPeninputHwrBoxUiStatePenCandidateSelecting()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenBeginWriting::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxUiStatePenCandidateSelecting::HandleControlEventPenL(TInt aEventType
                                                                   , const TDesC& aEventData
                                                                   , CPeninputHwrBoxLayout& aUiLayout
                                                                   , TDes& aCharCode
                                                                   , TBool& aHandled)
    {
    aHandled = ETrue;
    switch (aEventType)
        {
        case EPeninputLayoutHwrEventBeginWriting:
            aUiLayout.ToPenStandBy(ETrue);   // Not To PenStandBy
            iUiStateMgr->SetCurrentUiState(EPeninputHwrUiStatePenBeginWriting);
            aUiLayout.HwrBoxWindow()->DimInputContextField(ETrue);    // BeginWriting::OnEntry()
            break;
        case EPeninputLayoutHwrEventCandidateSelected:
            {   // CandidateSelecting::OnCandidateSelectedL(aEventData);
            if (aEventData.Length() > 1)
                {
                //get the candidate index
                const TInt cellNo = aEventData[aEventData.Length()-1];

                //not the first candidate, replace previouse char
                if (cellNo > 0)
                    {
                    //get the new candidate
                    aUiLayout.DataMgr(0)->GetCandidate(cellNo, aCharCode);
                    aUiLayout.SubmitL(aCharCode);
                    }
                }
            aUiLayout.ToPenStandBy();
            }
            break;
        case EPeninputLayoutHwrEventDropDownListExpanded:
            aHandled = EFalse;
            break;
        case EEventDraggingEnd:
            break; // nop
        case EPeninputLayoutEventBack:
            {   // clear
            aUiLayout.ClearInputToPenStandByL();
            }
            break;
        default:
            {  // return EFalse;
            aUiLayout.ToPenStandBy();
            aHandled = EFalse;
            }
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenCandidateSelecting::CPeninputHwrBoxUiStatePenCandidateSelecting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenCandidateSelecting::CPeninputHwrBoxUiStatePenCandidateSelecting(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBoxUiState(aUiStateMgr,aContext)
    {
    }

// End Of File
