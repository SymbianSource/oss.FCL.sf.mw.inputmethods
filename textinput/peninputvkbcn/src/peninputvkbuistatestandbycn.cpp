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
* Description:  Implementation of the vkb ui state machine (Chinese Standby State)
*
*/


// user includes
#include "peninputvkbuistatestandbycn.h"
#include "peninputvkbevent.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CAknFepVkbUiStateStandbycn

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::CAknFepVkbUiStateStandbycn
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandbycn::CAknFepVkbUiStateStandbycn(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                                       MAknFepVkbLayoutContext* aContext)
    :CAknFepVkbUiState(aUiStateMgr,aContext)
    {
    }
       
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandbycn* CAknFepVkbUiStateStandbycn::NewL(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                                             MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiStateStandbycn* self = 
                                  new ( ELeave ) CAknFepVkbUiStateStandbycn(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::~CAknFepVkbUiStateStandbycn
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandbycn::~CAknFepVkbUiStateStandbycn()
    {
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycn::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycn::HandleVkbEvent(TInt /*aEventType*/,
                                                 const TDesC& aEventData)
    {
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();      
    if ( IsFilterUnicode(event->iScanCode) )
        {
        iContext->ClearTriggerString();
        iContext->SendEventToVkbControl(EVkbEventClearContent,NULL);
        return SendKey(aEventData);
        }
    else
        {
        iContext->SendEventToVkbControl(EVkbEventStandby2Composition,NULL); 
        iContext->ForceEditGainFocus(ECompotisionFieldGetFocus);       
        iUiStateMgr->SetCurrentUiState( iUiStateMgr->UiStateCompositionWithChars() ); 
        return EFalse;                
        }
    
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycn::HandleBackEvent(TInt /*aEventType*/,
                                                  const TDesC& aEventData)
    {
    TInt candidateCount = iContext->RequestData(EAknFepDataTypeCandidateCount);
    if(candidateCount)
        {
        TRAP_IGNORE(iContext->GetCandidatesL(aEventData,ETrue));
        }
    candidateCount = iContext->RequestData(EAknFepDataTypeCandidateCount);
    if ( candidateCount == 0 )
        {
        iContext->SendEventToVkbControl(EVkbEventClearContent,NULL);
        }

    return SendKey(aEventData);  
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::HandleEnterSpaceEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycn::HandleEnterSpaceEvent(TInt /*aEventType*/,
                                                           const TDesC& aEventData)  
    {
    iContext->ClearTriggerString();
    iContext->SendEventToVkbControl(EVkbEventClearContent,NULL);
    return SendKey(aEventData);
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycn::HandleEnterSpaceEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycn::HandleTabEvent(TInt /*aEventType*/,
                                                           const TDesC& aEventData)  
    {
    return SendKey(aEventData);
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleArrowKeyEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycn::HandleArrowKeyEvent(TInt /*aEventType*/, const TDesC& aEventData)
    {
    iContext->ClearTriggerString();//new added
    iContext->SendEventToVkbControl(EVkbEventClearContent,NULL); //new added
    return SendKey(aEventData);
    }
    
TBool CAknFepVkbUiStateStandbycn::HandleControlEvent(TInt aEventType, const TDesC& aEventData)
	{
	if( aEventType == EEventSetAppCursorSelection && 
		iUiStateMgr->CurrentUiState() == iUiStateMgr->UiStateStandbycn() )	
		{
		// clear condidate list when cursor moves
	    iContext->ClearTriggerString();
	    iContext->SendEventToVkbControl(EVkbEventClearContent,NULL);
		return true;		
		}
	else
		return CAknFepVkbUiState::HandleControlEvent( aEventType, aEventData );	
	}
// End Of File
