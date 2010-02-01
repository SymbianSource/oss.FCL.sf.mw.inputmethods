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
#include "peninputvkbevent.h"
#include "peninputvkbuistatecompositionwithchars.h"
#include "peninputvkblayout.h"
#include "peninputvkbwindow.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CAknFepVkbUiStateCompositionWithChars

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::CAknFepVkbUiStateCompositionWithChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateCompositionWithChars::CAknFepVkbUiStateCompositionWithChars(
                                            MAknFepVkbUiStateMgr* aUiStateMgr, 
                                            MAknFepVkbLayoutContext* aContext)
    :CAknFepVkbUiState(aUiStateMgr,aContext)
    {
    }
       
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateCompositionWithChars* CAknFepVkbUiStateCompositionWithChars::
                                       NewL(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                            MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiStateCompositionWithChars* self = new ( ELeave ) 
                                      CAknFepVkbUiStateCompositionWithChars(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::~CAknFepVkbUiStateCompositionWithChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateCompositionWithChars::~CAknFepVkbUiStateCompositionWithChars()
    {
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionWithChars::HandleKeyEventL(const TRawEvent& aData)     
    {
    if ( static_cast<CAknFepVkbLayout*>(iContext)->Window()->FocusedControlID()
          == EAknFepVkbCtrlIdCompositionField && aData.Type() == TRawEvent::EKeyUp)
        {
        if (aData.ScanCode() == EStdKeyLeftArrow)
            {
            //static_cast<CAknFepVkbLayout*>(iContext)->Window()->UpdateCompositionFieldCursor(ETrue);
            return ETrue;
            }
        else if (aData.ScanCode() == EStdKeyRightArrow)
            {
            //static_cast<CAknFepVkbLayout*>(iContext)->Window()->UpdateCompositionFieldCursor(EFalse);
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::HandleVkbEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionWithChars::HandleVkbEvent(TInt /*aEventType*/,
                                                            const TDesC& aEventData)
    {
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
    if ( ! IsFilterUnicode(event->iScanCode) )
        {
        if ( !iContext->CompositionFieldStringLength() ) // if no chars
            {
            iContext->SendEventToVkbControl(EVkbEventStandby2Composition,NULL);        
            }
            
        return EFalse; 
        }
    else 
        {
        if ( !iContext->CompositionFieldStringLength() ) // if no chars
            {
            iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());
            iContext->ForceEditGainFocus(EInputContextFieldGetFocus);    
            return SendKey(aEventData);        
            }        
        return ETrue;    
        }    
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionWithChars::HandleBackEvent(TInt /*aEventType*/,
                                                             const TDesC& aEventData)
    {
    TInt lengthPendown = iContext->RequestData(EAknFepDataTypeLengthWhenButtondownOnBack);
    
    TInt* data = (TInt*) aEventData.Ptr();
    
    //Get the flag of pen up or timer out, true when timer out, false when pen up
    data++;
    
    if ( 1 >= iContext->CompositionFieldStringLength() ) // if no chars
        {
        iContext->SendEventToVkbControl(EVkbEventComposition2Standby,NULL);
        
        if ( *data == EFalse )  // pen up
            {
            iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());                       
            iContext->ForceEditGainFocus(EInputContextFieldGetFocus);              
            }            
        }
    return EFalse;  // If no chars, needn't send to composition again
        
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::HandleEnterSpaceEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionWithChars::HandleEnterSpaceEvent(TInt aEventType, 
                                                                   const TDesC& aEventData)  
    {
    TInt layoutType = iContext->RequestData(EAknFepDataTypeIMLayout);
    
    if ( !iContext->CompositionFieldStringLength() )  // If no chars
        {
        iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());
        iContext->ForceEditGainFocus(EInputContextFieldGetFocus);    
        return SendKey(aEventData);
        }
    else  // If have chars
        {
        if ( aEventType == EVkbEventKeyEnter)
            {
            return EFalse;
            }
        if  ( ( layoutType == EAknFepVkbImCnZhuyin || layoutType == EAknFepVkbImCnStroke || 
                layoutType == EAknFepVkbImCnPinyin || layoutType == EAknFepVkbImCnCangjie) && 
                ( aEventType == EVkbEventKeySpace))
            {
            return EFalse;
            }
        else{
            return ETrue;    
            }            
        }        
    }        
           
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionWithChars::HandleTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionWithChars::HandleTabEvent(TInt /*aEventType*/, 
                                                                      const TDesC& aEventData)  
    {
    TInt layoutType = iContext->RequestData(EAknFepDataTypeIMLayout);
    
    if ( !iContext->CompositionFieldStringLength() )  // If no chars
        {
        iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());
        iContext->ForceEditGainFocus(EInputContextFieldGetFocus);    
        return SendKey(aEventData);
        }
    return ETrue;
    }     
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateCompositionNoChars::HandleArrowKeyEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepVkbUiStateCompositionWithChars::HandleArrowKeyEvent(TInt /*aEventType*/, 
                                                                 const TDesC& aEventData)
    {
    return EFalse;
    }          
// End Of File
