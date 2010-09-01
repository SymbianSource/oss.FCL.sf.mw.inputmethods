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
* Description:  Implementation of the vkb ui state machine (Chinese Standby State with Chars)
*
*/


// user includes
#include "peninputvkbuistatestandbycnwithchars.h"
#include "peninputvkbevent.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CAknFepVkbUiStateStandbycnWithChars

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycnWithChars::CAknFepVkbUiStateStandbycnWithChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandbycnWithChars::CAknFepVkbUiStateStandbycnWithChars(
                                       MAknFepVkbUiStateMgr* aUiStateMgr, 
                                       MAknFepVkbLayoutContext* aContext)
    :CAknFepVkbUiStateStandbycn(aUiStateMgr,aContext)
    {
    }
       
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycnWithChars::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandbycnWithChars* CAknFepVkbUiStateStandbycnWithChars::
                       NewL(MAknFepVkbUiStateMgr* aUiStateMgr, MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiStateStandbycnWithChars* self = 
                        new ( ELeave ) CAknFepVkbUiStateStandbycnWithChars(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycnWithChars::~CAknFepVkbUiStateStandbycnWithChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateStandbycnWithChars::~CAknFepVkbUiStateStandbycnWithChars()
    {
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycnWithChars::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycnWithChars::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycnWithChars::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycnWithChars::HandleVkbEvent(TInt /*aEventType*/, 
                                                          const TDesC& aEventData)
    {
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
    if ( IsFilterUnicode(event->iScanCode) )
        {
        return TRUE;
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
// CAknFepVkbUiStateStandbycnWithChars::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycnWithChars::HandleBackEvent(TInt /*aEventType*/, 
                                                           const TDesC& aEventData)
    {
    /*TInt candidateCount = iContext->RequestData(EAknFepDataTypeCandidateCount);
    if ( candidateCount )
        {
        iContext->SendEventToVkbControl(EVkbEventCompFieldNoChars,NULL);
        return ETrue;
        }
    else
        {*/
        return SendKey(aEventData);  
        //}    
            
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycnWithChars::HandleEnterSpaceEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycnWithChars::HandleEnterSpaceEvent(TInt /*aEventType*/, 
                                                                 const TDesC& aEventData)  
    {           
    return SendKey(aEventData);
    }          
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandbycnWithChars::HandleTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycnWithChars::HandleTabEvent(TInt /*aEventType*/, 
                                                          const TDesC& aEventData)  
    {           
    return SendKey(aEventData);
    }  
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateStandby::HandleArrowKeyEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateStandbycnWithChars::HandleArrowKeyEvent(TInt /*aEventType*/, 
                                                               const TDesC& aEventData)
    {
    return SendKey(aEventData);
    }      
// End Of File
