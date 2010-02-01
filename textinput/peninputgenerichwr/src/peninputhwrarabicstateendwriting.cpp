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
#include <peninputcommonlayoutglobalenum.h>
#include <peninputdataconverter.h>
#include <aknfeppeninputenums.h>    // ERangeNumber
// user includes
//#include "peninputhwrevent.h"
#include "peninputhwrarabic.hrh"
#include "peninputhwrarabiclayout.h"
#include "peninputhwrarabicdatamgr.h"       // use data mgr
#include "peninputhwrarabicwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrarabicstateendwriting.h"

// constant definition
//const TUint16 KBackRep = 0x2408;
// convert displayable backspace
const TUint16 KBackRep = 0x2190;
const TUint16 KEnterRep = 0xF801;
const TInt KMaxCandidateLen = 128;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBxAbStateEndWriting

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateEndWriting::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateEndWriting* CPeninputHwrBxAbStateEndWriting::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBxAbStateEndWriting* self = new ( ELeave ) CPeninputHwrBxAbStateEndWriting(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateEndWriting::~CPeninputHwrBxAbStateEndWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateEndWriting::~CPeninputHwrBxAbStateEndWriting()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbStateEndWriting::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBxAbStateEndWriting::HandleControlEventPenL( TInt aEventType, 
    const TDesC& aEventData, CPeninputHwrBxAbLayout& aUiLayout, TDes& aCharCode, TBool& aHandled )
    {
    aHandled = EFalse;
    switch (aEventType)
        {
        case EEventHwrStrokeStarted:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateBeginWriting);
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->OnStrokeStarted();
            aHandled = ETrue;
            }
            break;
        case EEventHwrStrokeFinished:
            {
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->GetCandidate(0, aCharCode);
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->OnStrokeFinished();

            //aUiLayout.SubmitL(aCharCode);

            TPtrC16 ptrBack;
            TPtrC16 ptrEnter;
            ptrBack.Set(&KBackRep, sizeof(KBackRep)/2);
            ptrEnter.Set(&KEnterRep, sizeof(KEnterRep)/2);

            //if char code is backspace or enter
            if (aCharCode.CompareC(ptrBack) == 0
                    || aCharCode.CompareC(ptrEnter) == 0)
                {
                aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateStandby);
                }
            else
                {
                aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateCandidateSelecting);
                }
            aHandled = ETrue;
            }
            break;
        case EEventHwrCharacterTimerOut:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateCandidateSelecting);
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->OnStrokeCharacterTimerOut();
            aHandled = ETrue;
            }
            break;
        case EPeninputLayoutHwrEventCandidateSelected:
            {
            //cancel writing timer
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->CancelWriting();
            //There must be a charcode and length at least
            if (aEventData.Length() > 1)
                {
                //get the candidate index
                const TInt cellNo = aEventData[aEventData.Length()-1];

                //get and submit the candidate to fep
                TBuf<KMaxCandidateLen> charCode;
                static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->GetCandidate(cellNo, charCode);
                aUiLayout.SubmitL(charCode);
                
                TInt dataType = CPeninputDataConverter::AnyToInt(aUiLayout.RequestData( EPeninputDataTypeCurrentRange ));
                if (dataType == ERangeNative)
                    {
                    static_cast<CPeninputHwrBxAbWnd *>(aUiLayout.LayoutWindow())->ResetAndShowDropdownList();
                    }
                aHandled = ETrue;
                }
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
// CPeninputHwrBxAbStateEndWriting::CPeninputHwrBxAbStateEndWriting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateEndWriting::CPeninputHwrBxAbStateEndWriting(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBxAbStateBase(aUiStateMgr,aContext)
    {
    }

void CPeninputHwrBxAbStateEndWriting::ToCandidateSelecting(CPeninputHwrBxAbLayout& /*aUiLayout*/)
    {
    }

// End Of File
