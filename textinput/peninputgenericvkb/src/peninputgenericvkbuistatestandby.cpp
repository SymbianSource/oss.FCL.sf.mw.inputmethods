/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of vkb ui standby state
*
*/


// System includes
#include <peninputcmd.h>
#include <aknfeppeninputenums.h>
#include <peninputdataprovider.h>
#include <peninputlayoutcontext.h>
#include <peninputdataconverter.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "peninputgenericvkb.hrh"
#include "peninputgenericvkbuistatestandby.h"
#include "peninputgenericvkbenum.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::CPeninputUiStateVkbStandby
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateVkbStandby::CPeninputUiStateVkbStandby( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }
       
// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateVkbStandby* CPeninputUiStateVkbStandby::NewL( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    {
    CPeninputUiStateVkbStandby* self = 
        new ( ELeave ) CPeninputUiStateVkbStandby( aUiStateMgr, aContext );
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );

    return self;
    }  
      
// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::~CPeninputUiStateVkbStandby
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateVkbStandby::~CPeninputUiStateVkbStandby()
    {
    }

// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::HandleKeyEventL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateVkbStandby::HandleKeyEventL( 
    const TRawEvent& /*aData*/ )
    {
    return EFalse;
    }    

// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateVkbStandby::HandleControlEvent( TInt aEventType, 
    const TDesC& aEventData )
    {
    switch ( aEventType )
        {
        case EEventVirtualKeyUp:
            {
            return HandleVkbEvent( aEventType, aEventData );
            }
        case EPeninputLayoutEventBack:
            {
            return HandleBackEvent( aEventType, aEventData );
            }
            
        case EPeninputLayoutEventLeftArrow:
        case EPeninputLayoutEventRightArrow:
            {
            SendKey( aEventData );
            return EFalse;
            }            
        case EPeninputLayoutEventTab:
            {
            return HandleTabEvent( aEventType, aEventData ); 
            }
        case EPeninputLayoutEventEnter:
        case EPeninputLayoutEventSpace:
            {
            return HandleEnterSpaceEvent( aEventType, aEventData );
            }
        default:
            return EFalse;
        }
    }      

// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::HandleVkbEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateVkbStandby::HandleVkbEvent( TInt /*aEventType*/, 
    const TDesC& aEventData )
    {
    return SendKey( aEventData );
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::HandleBackEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateVkbStandby::HandleBackEvent( TInt /*aEventType*/, 
    const TDesC& aEventData )
    {
    TInt latchedFlag = CPeninputDataConverter::AnyToInt
                       (iContext->RequestData(EAkninputDataTypeLatchedSet));
    if(!latchedFlag)
        {
        SendKey( aEventData );
        }
    return EFalse;
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::HandleEnterSpaceEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateVkbStandby::HandleEnterSpaceEvent( 
    TInt /*aEventType*/, const TDesC& aEventData )
    {
    TInt latchedFlag = CPeninputDataConverter::AnyToInt
                       (iContext->RequestData(EAkninputDataTypeLatchedSet));
    if(!latchedFlag)
        {
        SendKey( aEventData );
        }
    
    return EFalse;
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::HandleTabEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
 TBool CPeninputUiStateVkbStandby::HandleTabEvent( 
    TInt /*aEventType*/,  const TDesC& aEventData )
    {
    SendKey( aEventData );
    return EFalse;
    }
    
