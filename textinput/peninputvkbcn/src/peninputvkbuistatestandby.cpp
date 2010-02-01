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
* Description:  Implementation of the vkb ui state machine (Standby State)
*
*/


// user includes
#include "aknfeppeninputenums.h"
#include "peninputvkbuistatestandby.h"
#include "peninputvkbevent.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CAknFepVkbUiStateStandby

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::CAknFepVkbUiStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandby::CAknFepVkbUiStateStandby(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                                   MAknFepVkbLayoutContext* aContext)
    :CAknFepVkbUiState(aUiStateMgr,aContext)
    {
    }
       
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandby* CAknFepVkbUiStateStandby::NewL(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                                         MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiStateStandby* self = new ( ELeave ) CAknFepVkbUiStateStandby(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;
    }
      
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::~CAknFepVkbUiStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandby::~CAknFepVkbUiStateStandby()
    {
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandby::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandby::HandleVkbEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    TBool rs = SendKey(aEventData);
    
    // If shift button is down
    TBool shiftDown = iContext->RequestData(EAknFepDataTypeShfitDown); 
    TInt curRange = iContext->RequestData(EAknFepDataTypeCurrentRange); 
    if ( shiftDown && curRange != ERangeNumber )
        {
    //    iContext->SetData(EAknFepDataTypeShfitDown,0);
        iContext->SendEventToVkbControl(EVkbEventResetShift,NULL);
        }    

    return rs;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandby::HandleBackEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    return SendKey(aEventData);
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleEnterSpaceEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandby::HandleEnterSpaceEvent(TInt /*aEventType*/, const TDesC& aEventData)  
    {
  	return SendKey(aEventData);   
    }    

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleEnterTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandby::HandleTabEvent(TInt /*aEventType*/, const TDesC& aEventData)  
    {
    return SendKey(aEventData);   
  	}   
  	
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleArrowKeyEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandby::HandleArrowKeyEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    return SendKey(aEventData);
    }  	
    
// End Of File
