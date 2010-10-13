/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for pen ui window event handler
*
*/

#include "peninputserver.h"
#include "peninputuilayoutowner.h"
#include "penuiwndeventobserver.h"
#include "penuiwndeventhandler.h"

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPenUiWndEventHandler* CPenUiWndEventHandler::NewL( CPeninputServer* aServer )
    {
    CPenUiWndEventHandler* self = CPenUiWndEventHandler::NewLC( aServer );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPenUiWndEventHandler* CPenUiWndEventHandler::NewLC( CPeninputServer* aServer )
    {
    CPenUiWndEventHandler* self = new (ELeave) CPenUiWndEventHandler( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CPenUiWndEventHandler::~CPenUiWndEventHandler()
    {
    }

// ---------------------------------------------------------------------------
// Signal owner that there is an event
// ---------------------------------------------------------------------------
//
void CPenUiWndEventHandler::SignalOwner( TInt aEventType, 
                                         const TDesC& aEventData )
    {
    if ( iServer )
        {
        iServer->SignalOwner( aEventType, aEventData );
        }
    }

// ---------------------------------------------------------------------------
// Handle layout command event.
// ---------------------------------------------------------------------------
//
void CPenUiWndEventHandler::HandleCommand( TInt aCmd, TUint8* aData )
    {
    if ( iServer && iServer->PenUiLayout() )
        {
        iServer->PenUiLayout()->HandleCommand( aCmd, aData );
        }
    }

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CPenUiWndEventHandler::CPenUiWndEventHandler( CPeninputServer* aServer ) 
                        : iServer( aServer )
    {
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CPenUiWndEventHandler::ConstructL()
    {
    }

// End of File
