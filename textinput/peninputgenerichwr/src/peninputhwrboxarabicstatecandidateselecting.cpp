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
#include <peninputcommonlayoutglobalenum.h>
#include <peninputdataconverter.h>
#include <aknfeppeninputenums.h>    // ERangeNumber
// user includes
//#include "peninputhwrevent.h"
#include "peninputhwrarabic.hrh"
#include "peninputhwrarabiclayout.h"
#include "peninputhwrarabicdatamgr.h"       // use data mgr
#include "peninputhwrarabicwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrboxarabicstatecandidateselecting.h"
#include "peninputlayouthwrwnd.h"

// constant definition
const TInt KMaxCandidateLen = 128;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBxAbStateCandidateSelecting

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateCandidateSelecting::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateCandidateSelecting* CPeninputHwrBxAbStateCandidateSelecting::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBxAbStateCandidateSelecting* self = new ( ELeave ) CPeninputHwrBxAbStateCandidateSelecting(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateCandidateSelecting::~CPeninputHwrBxAbStateCandidateSelecting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateCandidateSelecting::~CPeninputHwrBxAbStateCandidateSelecting()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxUiStatePenBeginWriting::HandleControlEventPenL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBxAbStateCandidateSelecting::HandleControlEventPenL( TInt aEventType, 
    const TDesC& aEventData, CPeninputHwrBxAbLayout& aUiLayout, TDes& /*aCharCode*/, TBool& aHandled )
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

        case EPeninputLayoutHwrEventCandidateSelected:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateStandby);

            //cancel writing timer
            CTransparentHwrWndExt * transparentHwrWndExt =
                    static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->getCTransparentHwrWndExt();
            //transparentHwrWndExt->SetWndTransparencyFactor(iInactiveTrans);
            transparentHwrWndExt->CancelCharWriting();
            //submit text.
            if (aEventData.Length() > 1)
                {
                //get the candidate index
                const TInt cellNo = aEventData[aEventData.Length()-1];

                //not the first candidate, replace previouse char
                if (cellNo > 0)
                    {
                    //get the first candidate 
                    TBuf<KMaxCandidateLen> oldCharCode;
                    static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->GetCandidate(0, oldCharCode);

                    //get the new candidate 
                    TBuf<KMaxCandidateLen> newCharCode;
                    static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->GetCandidate(cellNo, newCharCode);

                    aUiLayout.TranslateCharCode(newCharCode);

                    //delete the first candidate submitted and submit new
                    aUiLayout.ReplaceL(oldCharCode, newCharCode);
                    }
                }
                
                TInt dataType = CPeninputDataConverter::AnyToInt(aUiLayout.RequestData( EPeninputDataTypeCurrentRange ));
                if (dataType == ERangeNative)
                {
                static_cast<CPeninputHwrBxAbWnd *>(aUiLayout.LayoutWindow())->ResetAndShowDropdownList();
                }
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
        case EPeninputLayoutEventRange:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateStandby);
//            aHandled = ETrue;
            }
            break;            
        case EEventIcfPointerUpEvent:
            {            
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateStandby);
            TInt dataType = CPeninputDataConverter::AnyToInt(aUiLayout.RequestData( EPeninputDataTypeCurrentRange ));
            if (dataType == ERangeNative)
                {
                static_cast<CPeninputHwrBxAbWnd *>(aUiLayout.LayoutWindow())->ResetAndShowDropdownList();
                aHandled = ETrue;
                }
            }
            break;
        case EPeninputLayoutEventBack:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateStandby);
            TInt dataType = CPeninputDataConverter::AnyToInt(aUiLayout.RequestData( EPeninputDataTypeCurrentRange ));
            if (dataType == ERangeNative)
                {
                static_cast<CPeninputHwrBxAbWnd *>(aUiLayout.LayoutWindow())->ResetAndShowDropdownList();
                }
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->HandleBackspaceKeyEvent();
            aHandled = ETrue;
            }
            break;
        default:
            break;
        } 
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateCandidateSelecting::CPeninputHwrBxAbStateCandidateSelecting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateCandidateSelecting::CPeninputHwrBxAbStateCandidateSelecting(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBxAbStateBase(aUiStateMgr,aContext)
    {
    }

// End Of File
