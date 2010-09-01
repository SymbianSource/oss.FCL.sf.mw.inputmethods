/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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


// System includes

// user includes
#include "peninputvkbuistatewithtransitorychars.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputVkbUiStateWithTransitoryChars

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateWithTransitoryChars::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateWithTransitoryChars* CPeninputVkbUiStateWithTransitoryChars::NewL(MPeninputUiStateMgr* aUiStateMgr, 
                                                                                   MPeninputLayoutContext* aContext)
    {
    CPeninputVkbUiStateWithTransitoryChars* self = new ( ELeave ) CPeninputVkbUiStateWithTransitoryChars(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct(); 
    CleanupStack::Pop(self);
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CPeninputVkbUiStateWithTransitoryChars::~CPeninputVkbUiStateWithTransitoryChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateWithTransitoryChars::~CPeninputVkbUiStateWithTransitoryChars()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateWithTransitoryChars::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateWithTransitoryChars::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return ETrue;  // by HiraganaKanji
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateWithTransitoryChars::CPeninputVkbUiStateWithTransitoryChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputVkbUiStateWithTransitoryChars::CPeninputVkbUiStateWithTransitoryChars(MPeninputUiStateMgr* aUiStateMgr, 
                                                                             MPeninputLayoutContext* aContext)
    :CPeninputVkbUiState(aUiStateMgr,aContext)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputVkbUiStateWithTransitoryChars::HandleVkbEvent
// (other items were commented in a header).
// return False --->iVkbWindow->HandleControlEvent() by UiLayout
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateWithTransitoryChars::HandleVkbEvent(TInt /*aEventType*/,
                                                            const TDesC& /*aEventData*/)
    {
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CPeninputVkbUiStateWithTransitoryChars::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateWithTransitoryChars::HandleBackEvent(TInt /*aEventType*/,
                                                             const TDesC& /*aEventData*/)
    {
    return EFalse;
    }
// -----------------------------------------------------------------------------
// CPeninputVkbUiStateWithTransitoryChars::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbUiStateWithTransitoryChars::HandleEnterSpaceTabEvent(TInt /*aEventType*/, 
                                                                      const TDesC& /*aEventData*/)  
    {
    return EFalse;
    }        
                  
// End Of File
