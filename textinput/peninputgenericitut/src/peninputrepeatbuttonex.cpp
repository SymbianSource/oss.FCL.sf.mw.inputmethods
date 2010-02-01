/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

// system includes
#include <peninputlayout.h>
#include <AknIconUtils.h>
#include <coemain.h>
#include <AknsUtils.h>
#include <AknsDrawUtils.h> 

// user includes
#include "peninputrepeatbuttonex.h"

//default value for long press timer
const TInt KLongPressInterval = 600000;

//default value for repeat timer
const TInt KRepeatInterval = 100000;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepCtrlRepeatButtonEx* CAknFepCtrlRepeatButtonEx::NewLC(CFepUiLayout* aUiLayout, 
                                                                 TInt aCtrlId,
                                                                 TAknsItemID aNormalID,
    											  				 TAknsItemID aPressedID,
    											  				 TAknsItemID aInactiveID,
                                                                 TInt aEvent,
                                                                 TInt aUnicode)                                                                 
    {
    //PRINTF((_L("CAknFepCtrlRepeatButtonEx::NewLC(), aCtrlId = %d\n"), aCtrlId));
    CAknFepCtrlRepeatButtonEx *self = new(ELeave) CAknFepCtrlRepeatButtonEx(aUiLayout, 
                                                                        aCtrlId, 
                                                                       	aNormalID,
    											  						aPressedID,
    											  						aInactiveID,
                                                                        aEvent, 
                                                                        aUnicode);

    CleanupStack::PushL(self);
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepCtrlRepeatButtonEx* CAknFepCtrlRepeatButtonEx::NewL(CFepUiLayout* aUiLayout, 
                                                                TInt aCtrlId,
                                                                TAknsItemID aNormalID,
    											  				TAknsItemID aPressedID,
    											  				TAknsItemID aInactiveID,
                                                                TInt aEvent,
                                                                TInt aUnicode)                                                                 
    {
    //PRINTF((_L("CAknFepCtrlRepeatButtonEx::NewL(), aCtrlId = %d\n"), aCtrlId));
    CAknFepCtrlRepeatButtonEx *self = NewLC(aUiLayout, 
    									  aCtrlId, 
    									  aNormalID,
    									  aPressedID,
    									  aInactiveID,
    									  aEvent, 
    									  aUnicode);
    CleanupStack::Pop(self);
    
    return self;
    }
        
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepCtrlRepeatButtonEx::~CAknFepCtrlRepeatButtonEx()
    {
    //PRINTF((_L("CAknFepCtrlRepeatButtonEx::~CAknFepCtrlRepeatButtonEx()\n")));
    CancelRepeat();

    delete iLongPressTimer;
    delete iRepeatTimer;
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepCtrlRepeatButtonEx::CAknFepCtrlRepeatButtonEx(CFepUiLayout* aUiLayout, 
                                                          TInt aCtrlId,
                                                          TAknsItemID aNormalID,
    											  		  TAknsItemID aPressedID,
    											  		  TAknsItemID aInactiveID,
                                                          TInt aEvent,
                                                          TInt aUnicode)                                                                 
    :CAknFepCtrlCommonButton(aUiLayout, aCtrlId, aNormalID, aPressedID, aInactiveID),
    iEvent( aEvent ), iUnicode( aUnicode )
    {
    iLongPressInterval = KLongPressInterval;
    iRepeatInterval = KRepeatInterval;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CAknFepCtrlRepeatButtonEx::ConstructL()
    {
    //PRINTF((_L("CAknFepCtrlRepeatButtonEx::ConstructL()\n")));
    BaseConstructL();
    
    iLongPressTimer = CAknFepTimer::NewL(this);
    iRepeatTimer = CAknFepTimer::NewL(this);
    }
    
// ---------------------------------------------------------
// Time out event handler of both long press timer & repeat timer
// ---------------------------------------------------------
//
void CAknFepCtrlRepeatButtonEx::HandleTimerOut(const CAknFepTimer* aTimer)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButtonEx::HandleTimerOut()--")));
    if (aTimer == iLongPressTimer)
        {
        //PRINTF((_L("long press timer timeout\n")));
        iRepeatTimer->SetTimer(iRepeatInterval);
        }
    else
        {
        if (aTimer == iRepeatTimer)
            {
            //PRINTF((_L("repeat timer timeout: EEventButtonUp reported\n")));
            if ( Unicode() )
                {
                TInt unicode = Unicode();
                TPtrC ptr = (TUint16*)(&unicode ) ;
                ReportEvent(Event(),ptr);   
                }
            else
                {
                ReportEvent(EEventButtonUp);
                }
            
			#ifdef RD_TACTILE_FEEDBACK
			if (UiLayout()->SupportTactileFeedback())
				{
				UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveInput);
				}
			#endif //RD_TACTILE_FEEDBACK 
            iRepeatTimer->SetTimer(iRepeatInterval);
            }
        }
    }

// ---------------------------------------------------------
// Handle button down start long press timer 
// ---------------------------------------------------------
//
CFepUiBaseCtrl* CAknFepCtrlRepeatButtonEx::HandlePointerDownEventL(const TPoint& aPt)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButtonEx::HandleButtonDown()\n")));
    
    //PRINTF((_L("Set long press Timer\n")));
    iLongPressTimer->SetTimer(iLongPressInterval);
    
    return CAknFepCtrlCommonButton::HandlePointerDownEventL(aPt);
    }
                                                          
// ---------------------------------------------------------
// Handle button up cancel timers 
// ---------------------------------------------------------
//
CFepUiBaseCtrl* CAknFepCtrlRepeatButtonEx::HandlePointerUpEventL(const TPoint& aPt)
    {
    //PRINTF((_L("CAknFepCtrlRepeatButtonEx::HandleButtonUp()\n")));
    
    CancelRepeat();
    
    return CAknFepCtrlCommonButton::HandlePointerUpEventL(aPt);
    }

// ---------------------------------------------------------
// Handle pointer levae event cancel timers 
// ---------------------------------------------------------
//
void CAknFepCtrlRepeatButtonEx::HandlePointerLeave(const TPoint& aPoint)
    {
    CAknFepCtrlCommonButton::HandlePointerLeave(aPoint);

    CancelRepeat();
    }

// ---------------------------------------------------------
// Set the repeat interval of repeat timer
// ---------------------------------------------------------
//
void CAknFepCtrlRepeatButtonEx::SetRepeatInterval(const TTimeIntervalMicroSeconds32& aTime)
    {
    iRepeatInterval = aTime;
    }

// ---------------------------------------------------------
// Cancel timers 
// ---------------------------------------------------------
//
void CAknFepCtrlRepeatButtonEx::CancelRepeat()
    {
    //PRINTF((_L("CAknFepCtrlRepeatButton::CancelRepeat()\n")));
    iLongPressTimer->Cancel();
    iRepeatTimer->Cancel();
    }
    
// ---------------------------------------------------------
// Unicode
// ---------------------------------------------------------
//
TInt CAknFepCtrlRepeatButtonEx::Unicode()
	{
	return iUnicode;
	}
	
// ---------------------------------------------------------
// Event
// ---------------------------------------------------------
//
TInt CAknFepCtrlRepeatButtonEx::Event()
	{
	return iEvent;
	}	

//  End Of File
