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
* Description:  Implementation of the hwr ui state machine (compositionnochars)
*
*/


// System includes

// user includes
#include "peninputhwrboxuistatewithtransitorychars.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPeninputHwrBoxUiStateWithTransitoryChars

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateWithTransitoryChars::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateWithTransitoryChars* CPeninputHwrBoxUiStateWithTransitoryChars::NewL(MPeninputUiStateMgr* aUiStateMgr, 
                                                                                   MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxUiStateWithTransitoryChars* self = new ( ELeave ) CPeninputHwrBoxUiStateWithTransitoryChars(aUiStateMgr,aContext);
    CleanupStack::PushL(self);
    self->Construct(); 
    CleanupStack::Pop(self);
    return self;
    }  
      
// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateWithTransitoryChars::~CPeninputHwrBoxUiStateWithTransitoryChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateWithTransitoryChars::~CPeninputHwrBoxUiStateWithTransitoryChars()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateWithTransitoryChars::HandleKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateWithTransitoryChars::HandleKeyEventL(const TRawEvent& /*aData*/)     
    {
    return ETrue;  // by HiraganaKanji
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateWithTransitoryChars::CPeninputHwrBoxUiStateWithTransitoryChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxUiStateWithTransitoryChars::CPeninputHwrBoxUiStateWithTransitoryChars(MPeninputUiStateMgr* aUiStateMgr, 
                                                                             MPeninputLayoutContext* aContext)
    :CPeninputHwrBoxUiState(aUiStateMgr,aContext)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateWithTransitoryChars::HandleVkbEvent
// pseudo virual key (hwr 1 char)
// (other items were commented in a header).
// return False --->iHiraganaKanji->HandleControlEventJpL() by UiLayout
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateWithTransitoryChars::HandleVkbEvent(TInt /*aEventType*/,
                                                            const TDesC& /*aEventData*/)
    {
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateWithTransitoryChars::HandleBackEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateWithTransitoryChars::HandleBackEvent(TInt /*aEventType*/,
                                                             const TDesC& /*aEventData*/)
    {
    return EFalse;
    }
// -----------------------------------------------------------------------------
// CPeninputHwrBoxUiStateWithTransitoryChars::HandleEnterSpaceTabEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrBoxUiStateWithTransitoryChars::HandleEnterSpaceTabEvent(TInt /*aEventType*/, 
                                                                      const TDesC& /*aEventData*/)  
    {
    return EFalse;
    }

// End Of File
