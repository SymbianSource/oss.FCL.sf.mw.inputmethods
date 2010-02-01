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
* Description:  Implementation of the hwr ui state machine (Pen Wndwriting State)
*
*/


// System includes
#include <peninputuistatemgr.h>

// user includes
#include "peninputhwrevent.h"
#include "peninputhwrboxlayout.h"
#include "peninputhwrboxdatamgr.h"       // use data mgr
#include "peninputhwrboxuistatetype.h"         // UI state
#include "peninputhwrboxwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrboxuistatepenendwriting.h"

// constant definition

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiStatePenEndWriting

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenEndWriting::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenEndWriting* CPeninputHwrBoxUiStatePenEndWriting::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiStatePenEndWriting* self = new ( ELeave ) CPeninputHwrBoxUiStatePenEndWriting(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenEndWriting::~CPeninputHwrBoxUiStatePenEndWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenEndWriting::~CPeninputHwrBoxUiStatePenEndWriting()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenEndWriting::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxUiStatePenEndWriting::HandleControlEventPenL(TInt aEventType
                                                                   , const TDesC& aEventData
                                                                   , CPeninputHwrBoxLayout& aUiLayout
                                                                   , TDes& aCharCode
                                                                   , TBool& aHandled)
    {
    aHandled = ETrue;
    switch (aEventType)
        {
        case EPeninputLayoutHwrEventNewWriting:
        case EPeninputLayoutHwrEventBeginWriting:
            aUiLayout.DataMgr(0)->ClearCandidates();
            iUiStateMgr->SetCurrentUiState(EPeninputHwrUiStatePenBeginWriting);
            aUiLayout.HwrBoxWindow()->DimInputContextField(ETrue);    // BeginWriting::OnEntry()
            break;
        case EPeninputLayoutHwrEventCandidateSelected:
            {   // EndWriting::OnCandidateSelectedL(aEventData);
            aUiLayout.HwrBoxWindow()->CancelWriting();   // Standby::OnEntry()

            //There must be a charcode and length at least
            if (aEventData.Length() > 1)
                {
                    //get the candidate index
                const TInt cellNo = aEventData[aEventData.Length()-1];

                //get and submit the candidate to fep
                aUiLayout.DataMgr(0)->GetCandidate(cellNo, aCharCode);
                aUiLayout.SubmitL(aCharCode);
                }
            aUiLayout.ToPenStandBy();
            }
            break;
        case EPeninputLayoutHwrEventPointerOutsideWnd:
            aUiLayout.ToPenStandBy();
            break;
        case EPeninputLayoutHwrEventDropDownListExpanded:
            {    // dropdown list opened
            aUiLayout.HwrBoxWindow()->CancelWriting();   // timer cancel
            aUiLayout.HwrBoxWindow()->DropdownListDraw();
            ToCandidateSelecting(aUiLayout);
            aHandled = EFalse;
            }
            break;
        case EEventDraggingEnd:
        case EPeninputLayoutHwrEventEndWriting:
            {   // character time out
            ToCandidateSelecting(aUiLayout);
            }
            break;
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
// CPeninputHwrBoxUiStatePenEndWriting::CPeninputHwrBoxUiStatePenEndWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStatePenEndWriting::CPeninputHwrBoxUiStatePenEndWriting(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBoxUiState(aUiStateMgr,aContext)
    {
    }

void CPeninputHwrBoxUiStatePenEndWriting::ToCandidateSelecting(CPeninputHwrBoxLayout& aUiLayout)
    {
    iUiStateMgr->SetCurrentUiState(EPeninputHwrUiStatePenCandidateSelecting);
    if (aUiLayout.DataMgr(0)->CandidateCount() > 0)
        {
        //hight the first
        aUiLayout.HwrBoxWindow()->HighlightCell(0);
        }
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    aUiLayout.WriteHwrStrokeLogNew();
#endif // __PENINPUT_HWR_STROKE_LOG_ON
    }

// End Of File
