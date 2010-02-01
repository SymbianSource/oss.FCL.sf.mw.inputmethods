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
#include <peninputuistatemgr.h>
#include <aknfeppeninputenums.h>    // ERangeNumber
#include <peninputlayoutcontext.h>
#include <peninputdataconverter.h>
#include <peninputcommonlayoutglobalenum.h>
#include <peninputlayoutbasecontrol.h>
// user includes
//#include "peninputhwrevent.h"
#include "peninputhwrarabic.hrh"
#include "peninputhwrarabiclayout.h"
#include "peninputhwrarabicdatamgr.h"       // use data mgr
#include "peninputhwrarabicwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrarabicstatestandby.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBxAbStateStandby

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateStandby::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateStandby* CPeninputHwrBxAbStateStandby::NewL(MPeninputUiStateMgr* aUiStateMgr,
                                                         MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBxAbStateStandby* self = new ( ELeave ) CPeninputHwrBxAbStateStandby(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateStandby::~CPeninputHwrBxAbStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateStandby::~CPeninputHwrBxAbStateStandby()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateStandby::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateStandby::HandleKeyEventL(const TRawEvent& /*aData*/)
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateStandby::CPeninputHwrBxAbStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBxAbStateStandby::CPeninputHwrBxAbStateStandby(MPeninputUiStateMgr* aUiStateMgr,
                                                   MPeninputLayoutContext* aContext)
    :CPeninputHwrBxAbStateBase(aUiStateMgr,aContext)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateStandby::HandleVkbEvent
// pseudo virual key (hwr 1 char)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateStandby::HandleVkbEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    TBool rs = SendKey(aEventData);
    return rs;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateStandby::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateStandby::HandleBackEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    return SendKey(aEventData);

    }

// --------------------------------------------------------------------------
// CPeninputUiStateHwrStandby::HandleControlEventPenL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbStateStandby::HandleControlEventPenL( TInt aEventType, 
    const TDesC& aEventData, CPeninputHwrBxAbLayout& aUiLayout, TDes& /*aCharCode*/, TBool& aHandled )
    {
    aHandled = EFalse;
    switch (aEventType)
        {
        case EPeninputLayoutEventBack:
            {
            HandleBackEvent( aEventType, aEventData );	
            aHandled = ETrue;
            }
            break;

        case EEventHwrStrokeStarted:
            {
            aUiLayout.UiStateMgr()->SetCurrentUiState(EPeninputHwrBxAbStateBeginWriting);
            static_cast<CPeninputHwrBxAbWnd*>(aUiLayout.LayoutWindow())->OnStrokeStarted();
            aHandled = ETrue;
            }
            break;	
        case EEventIcfPointerUpEvent:
            {
            TInt dataType = CPeninputDataConverter::AnyToInt(aUiLayout.RequestData( EPeninputDataTypeCurrentRange ));
            if (dataType == ERangeNative)
                {
                static_cast<CPeninputHwrBxAbWnd *>(aUiLayout.LayoutWindow())->ResetAndShowDropdownList();
                aHandled = ETrue;
                }
            }
            break;
        default:
            break;
        }	
    }      

// -----------------------------------------------------------------------------
// CPeninputHwrBxAbStateStandby::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBxAbStateStandby::HandleEnterSpaceTabEvent(TInt /*aEventType*/, const TDesC& aEventData)
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
