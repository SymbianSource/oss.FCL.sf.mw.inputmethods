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
* Description:  Implementation of hwr ui standby state
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
#include "peninputgenerichwruistatestandby.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputUiStateHwrStandby::CPeninputUiStateHwrStandby
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateHwrStandby::CPeninputUiStateHwrStandby( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }
       
// --------------------------------------------------------------------------
// CPeninputUiStateHwrStandby::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateHwrStandby* CPeninputUiStateHwrStandby::NewL( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    {
    CPeninputUiStateHwrStandby* self = 
        new ( ELeave ) CPeninputUiStateHwrStandby( aUiStateMgr, aContext );
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );

    return self;
    }  
      
// --------------------------------------------------------------------------
// CPeninputUiStateHwrStandby::~CPeninputUiStateHwrStandby
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateHwrStandby::~CPeninputUiStateHwrStandby()
    {    
    }        

// --------------------------------------------------------------------------
// CPeninputUiStateHwrStandby::HandleKeyEventL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateHwrStandby::HandleKeyEventL( 
    const TRawEvent& /*aData*/ )     
    {
    return EFalse;
    }    

// --------------------------------------------------------------------------
// CPeninputUiStateHwrStandby::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateHwrStandby::HandleControlEvent( TInt aEventType, 
    const TDesC& aEventData )
    {
    if ( aEventType == EPeninputLayoutEventBack )
        {
        return HandleBackEvent( aEventType, aEventData );
        }
    else
        {
        return EFalse;   
        }   
    }      
   
// --------------------------------------------------------------------------
// CPeninputUiStateHwrStandby::HandleBackEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputUiStateHwrStandby::HandleBackEvent( TInt /*aEventType*/, 
                                                   const TDesC& aEventData )
    {
    return SendKey( aEventData );
    }    
