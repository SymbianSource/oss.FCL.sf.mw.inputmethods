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
#include "peninputsplitqwerty.hrh"
#include "peninputsplitqwertyuistatestandby.h"
#include "peninputsplitqwertyenum.h"

// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// Symbian constructor
// --------------------------------------------------------------------------
//
CPeninputSplitQwertyUiStateStandby* CPeninputSplitQwertyUiStateStandby::NewL(
                                            MPeninputUiStateMgr* aUiStateMgr, 
                                            MPeninputLayoutContext* aContext )
    {
    CPeninputSplitQwertyUiStateStandby* self = NULL;
    self = new ( ELeave ) CPeninputSplitQwertyUiStateStandby( aUiStateMgr,
                                                              aContext );
    CleanupStack::PushL( self );
    self->Construct();
    CleanupStack::Pop( self );

    return self;
    }

// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::CPeninputUiStateVkbStandby
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputSplitQwertyUiStateStandby::CPeninputSplitQwertyUiStateStandby(
                                            MPeninputUiStateMgr* aUiStateMgr, 
                                            MPeninputLayoutContext* aContext ) 
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
//
CPeninputSplitQwertyUiStateStandby::~CPeninputSplitQwertyUiStateStandby()
    {
    }

// --------------------------------------------------------------------------
// Process key event
// --------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStateStandby::HandleKeyEventL( const TRawEvent& /*aData*/)
    {
    return EFalse;
    }

// --------------------------------------------------------------------------
// process internal event
// --------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStateStandby::HandleControlEvent( TInt aEventType, 
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
// Process virtual key event
// --------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStateStandby::HandleVkbEvent(TInt /*aEventType*/, 
                                                     const TDesC& aEventData )
    {
    return SendKey( aEventData );
    }

// --------------------------------------------------------------------------
// Process backspace key event
// --------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStateStandby::HandleBackEvent( TInt /*aEventType*/, 
                                                      const TDesC& aEventData )
    {
    TInt latchedFlag = CPeninputDataConverter::AnyToInt(
            iContext->RequestData( EAkninputDataTypeLatchedSet ) );
    if ( !latchedFlag )
        {
        SendKey( aEventData );
        }
    return EFalse;
    }

// --------------------------------------------------------------------------
// Process enter, space key event
// --------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStateStandby::HandleEnterSpaceEvent( TInt /*aEventType*/,
                                                      const TDesC& aEventData )
    {
    TInt latchedFlag = CPeninputDataConverter::AnyToInt(
            iContext->RequestData( EAkninputDataTypeLatchedSet ) );
    
    if ( !latchedFlag )
        {
        SendKey( aEventData );
        }

    return EFalse;
    }

// --------------------------------------------------------------------------
// Process tab key event
// --------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStateStandby::HandleTabEvent( TInt /*aEventType*/, 
                                                      const TDesC& aEventData )
    {
    SendKey( aEventData );
    return EFalse;
    }

