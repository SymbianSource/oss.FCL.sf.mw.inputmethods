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
* Description:  Implementation of the vkb ui state machine (compositionnochars)
*
*/


// user includes
#include "peninputvkbuistatecompositionnochars.h"
#include "peninputvkbevent.h"
#include "peninputvkblayout.h"
#include "peninputvkbwindow.h"
// ======== MEMBER FUNCTIONS ========

// Implementation of Class CAknFepVkbUiStateCompositionNoChars

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::CAknFepVkbUiStateCompositionNoChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateCompositionNoChars::CAknFepVkbUiStateCompositionNoChars(
                                       MAknFepVkbUiStateMgr* aUiStateMgr, 
                                       MAknFepVkbLayoutContext* aContext)
    :CAknFepVkbUiState(aUiStateMgr,aContext)
    {
    }
       
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateCompositionNoChars* CAknFepVkbUiStateCompositionNoChars::
                        NewL(MAknFepVkbUiStateMgr* aUiStateMgr, MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiStateCompositionNoChars* self = new ( ELeave ) 
                        CAknFepVkbUiStateCompositionNoChars(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::~CAknFepVkbUiStateCompositionNoChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateCompositionNoChars::~CAknFepVkbUiStateCompositionNoChars()
    {
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionNoChars::HandleKeyEventL(const TRawEvent& aData)     
    {
    if ( static_cast<CAknFepVkbLayout*>(iContext)->Window()->FocusedControlID()
          == EAknFepVkbCtrlIdCompositionField)
        {
        if (aData.ScanCode() == EStdKeyLeftArrow || aData.ScanCode() == EStdKeyRightArrow)
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionNoChars::HandleVkbEvent(TInt /*aEventType*/,
                                                          const TDesC& aEventData)
    {
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
    if ( IsFilterUnicode(event->iScanCode) )
        {
        iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());
        iContext->ForceEditGainFocus(EInputContextFieldGetFocus);    
        return SendKey(aEventData);                
        }
    else
        {
        iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateCompositionWithChars());  
        iContext->SendEventToVkbControl(EVkbEventStandby2Composition,NULL);                
        return EFalse;  
        }
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionNoChars::HandleBackEvent(TInt /*aEventType*/,
                                                           const TDesC& aEventData)
    {
    iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn()); 
    iContext->ForceEditGainFocus(EInputContextFieldGetFocus);   
    return SendKey(aEventData); 
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::HandleEnterSpaceEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionNoChars::HandleEnterSpaceEvent(TInt /*aEventType*/, 
                                                                 const TDesC& aEventData)  
    {           
    iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());
    iContext->ForceEditGainFocus(EInputContextFieldGetFocus);    
    return SendKey(aEventData);      
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::HandleETabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionNoChars::HandleTabEvent(TInt /*aEventType*/, 
                                                          const TDesC& aEventData)  
    {           
    iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());
    iContext->ForceEditGainFocus(EInputContextFieldGetFocus);    
    return SendKey(aEventData);      
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::HandleArrowKeyEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionNoChars::HandleArrowKeyEvent(TInt /*aEventType*/, 
                                                               const TDesC& aEventData)
    {
    return ETrue;
    }       
// End Of File
