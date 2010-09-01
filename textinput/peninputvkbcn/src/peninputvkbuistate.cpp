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
* Description:  Implementation of the vkb ui state machine
*
*/


// user includes
#include "peninputvkbuistate.h"
#include "peninputvkbevent.h"


// ======== MEMBER FUNCTIONS ========

// Implementation of Class CAknFepVkbUiState

// -----------------------------------------------------------------------------
// CAknFepVkbUiState::CAknFepVkbUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiState::CAknFepVkbUiState(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                     MAknFepVkbLayoutContext* aContext)
    :iUiStateMgr(aUiStateMgr),iContext(aContext)
    {
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiState::ConstructL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepVkbUiState::ConstructL()
    {
    }  
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiState::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiState* CAknFepVkbUiState::NewL(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                           MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiState* self = new ( ELeave ) CAknFepVkbUiState(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CAknFepVkbUiState::~CAknFepVkbUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiState::~CAknFepVkbUiState()
    {    
    }        

// -----------------------------------------------------------------------------
// CAknFepVkbUiState::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return EFalse;
    }    

// -----------------------------------------------------------------------------
// CAknFepVkbUiState::HandleControlEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::HandleControlEvent(TInt aEventType, const TDesC& aEventData)
    {    
    if (aEventType == EEventVirtualKeyUp)
        {
        return HandleVkbEvent(aEventType, aEventData);
        }
    else if (aEventType == EPeninputLayoutEventBack)
        {
        return HandleBackEvent(aEventType, aEventData);
        }
	else if (aEventType == EVkbEventLeftArrow || aEventType == EVkbEventRightArrow)
		{
		return HandleArrowKeyEvent(aEventType, aEventData);
		}
    else if ((aEventType == EVkbEventKeyEnter) || 
             (aEventType == EVkbEventKeySpace))
        {
        return HandleEnterSpaceEvent(aEventType, aEventData);
        }
    else
        {
        return EFalse;   
        } 
    }      

// -----------------------------------------------------------------------------
// CAknFepVkbUiState::HandleArrowKeyEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::HandleArrowKeyEvent(TInt /*aEventType*/, 
                                        const TDesC& /*aEventData*/) 
    {
    return ETrue;    
    }    

// -----------------------------------------------------------------------------
// CAknFepVkbUiState::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::HandleVkbEvent(TInt /*aEventType*/, 
                                        const TDesC& /*aEventData*/) 
    {
    return ETrue;    
    }    
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiState::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::HandleBackEvent(TInt /*aEventType*/, 
                                         const TDesC& /*aEventData*/) 
    {
    return ETrue;    
    }    
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiState::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::HandleEnterSpaceEvent(TInt /*aEventType*/,  
                                                  const TDesC& /*aEventData*/)  
    {
    return ETrue;    
    }    
                
// -----------------------------------------------------------------------------
// CAknFepVkbUiState::IsFilterUnicode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::IsFilterUnicode(TUint16 aUnicode)
    {
    for (TInt i=0; i < KLengthSymbolNeedToFilter; i++)
        {
        if ( aUnicode == KSymbolsNeedToFilter[i] )
            {
            return ETrue;
            }       
        }

    return EFalse;
    }            

// -----------------------------------------------------------------------------
// CAknFepVkbUiState::SendKey
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiState::SendKey(const TDesC& aEventData)
    {
    TPtrC ptr = aEventData.Left(1);
    iContext->Sendkey(ESignalKeyEvent,ptr); 
    return ETrue; 
    }

// End Of File
