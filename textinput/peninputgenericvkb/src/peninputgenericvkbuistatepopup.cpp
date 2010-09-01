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
* Description:  Implementation of vkb ui popup state
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
#include "peninputgenericvkbuistatepopup.h"
#include "peninputgenericvkbenum.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputUiStateVkbPopup::CPeninputUiStateVkbPopup
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateVkbPopup::CPeninputUiStateVkbPopup( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }
       
// --------------------------------------------------------------------------
// CPeninputUiStateVkbPopup::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateVkbPopup* CPeninputUiStateVkbPopup::NewL( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    {
    CPeninputUiStateVkbPopup* self = 
        new ( ELeave ) CPeninputUiStateVkbPopup( aUiStateMgr, aContext );
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );

    return self;
    }  
      
// --------------------------------------------------------------------------
// CPeninputUiStateVkbPopup::~CPeninputUiStateVkbPopup
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateVkbPopup::~CPeninputUiStateVkbPopup()
    {
    }

// --------------------------------------------------------------------------
// CPeninputUiStateVkbPopup::HandleKeyEventL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateVkbPopup::HandleKeyEventL( 
    const TRawEvent& aData )
    {
    TInt scanCode = aData.ScanCode();
    iContext->SendEventToWindow( EPenInputLayoutEventClosePopup, 
                                 NULL );
    if(scanCode == EStdKeyYes) // send key
        {
        return EFalse;        
        }
    else
        {                                 
        return ETrue;
        }
    }    

// --------------------------------------------------------------------------
// CPeninputUiStateVkbPopup::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateVkbPopup::HandleControlEvent( TInt /*aEventType*/, 
    const TDesC& /*aEventData*/ )
    {
    return EFalse;   
    }      
