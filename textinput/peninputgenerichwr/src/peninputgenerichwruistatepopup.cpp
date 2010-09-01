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
* Description:  Implementation of hwr ui popup state
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
#include "peninputgenerichwr.hrh"
#include "peninputgenerichwruistatepopup.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputUiStateHwrPopup::CPeninputUiStateHwrPopup
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateHwrPopup::CPeninputUiStateHwrPopup( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }
       
// --------------------------------------------------------------------------
// CPeninputUiStateHwrPopup::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateHwrPopup* CPeninputUiStateHwrPopup::NewL( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    {
    CPeninputUiStateHwrPopup* self = 
        new ( ELeave ) CPeninputUiStateHwrPopup( aUiStateMgr, aContext );
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );

    return self;
    }  
      
// --------------------------------------------------------------------------
// CPeninputUiStateHwrPopup::~CPeninputUiStateHwrPopup
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateHwrPopup::~CPeninputUiStateHwrPopup()
    {
    }

// --------------------------------------------------------------------------
// CPeninputUiStateHwrPopup::HandleKeyEventL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateHwrPopup::HandleKeyEventL( 
    const TRawEvent& /*aData*/ )
    {
    iContext->SendEventToWindow( EPeninputLayoutHwrEventPopupClose, 
                                 NULL );
    return ETrue;
    }    

// --------------------------------------------------------------------------
// CPeninputUiStateHwrPopup::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateHwrPopup::HandleControlEvent( TInt /*aEventType*/, 
    const TDesC& /*aEventData*/ )
    {
    return EFalse;   
    }      
