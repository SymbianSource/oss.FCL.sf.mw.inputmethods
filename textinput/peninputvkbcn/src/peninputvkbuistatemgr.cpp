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
* Description:  Implementation of the vkb ui state machine manager
*
*/


// user includes
#include "peninputvkbuistatemgr.h"
#include "peninputvkblayoutcontext.h"

#include "peninputvkb.hrh"
#include "peninputvkbevent.h"
#include "peninputvkbuistate.h"
#include "peninputvkbuistatecompositionnochars.h"
#include "peninputvkbuistatecompositionwithchars.h"
#include "peninputvkbuistatestandby.h"
#include "peninputvkbuistatestandbycn.h"
#include "peninputvkbuistatestandbycnwithchars.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CAknFepVkbUiStateMgr

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::CAknFepVkbUiStateMgr
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateMgr::CAknFepVkbUiStateMgr(MAknFepVkbLayoutContext* aContext)
    :iContext(aContext)
    {
    }

// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateMgr* CAknFepVkbUiStateMgr::NewL(MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiStateMgr*  self = NewLC(aContext);
    CleanupStack::Pop();
    return self;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::NewLC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateMgr* CAknFepVkbUiStateMgr::NewLC(MAknFepVkbLayoutContext* aContext)
    {
    CAknFepVkbUiStateMgr* self = new ( ELeave ) CAknFepVkbUiStateMgr(aContext);
    CleanupStack::PushL(self);
    self->ConstructL(aContext);
 
    return self;
    }
      
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::~CAknFepVkbUiStateMgr
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepVkbUiStateMgr::~CAknFepVkbUiStateMgr()
    {
    delete iUiStateInitial;
    delete iUiStateStandby;
    delete iUiStateStandbycn;
    delete iUiStateStandbyWitchChars;
    delete iUiStateCompositionNoChars;
    delete iUiStateCompositionWithChars;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::CurrentUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepVkbUiState* CAknFepVkbUiStateMgr::CurrentUiState()
    {
    return iCurrentState;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::SetCurrentUiState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepVkbUiStateMgr::SetCurrentUiState(MAknFepVkbUiState* aUiState)
    {
    //First set the current state       
    iCurrentState = aUiState;   
    
    //Then change client vkb if current state is standbycn
    if ( iCurrentState == UiStateStandbycn() )
        {
        iContext->SendEventToVkbControl(EVkbEventComposition2Standby,NULL);
        }
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::UiStateInitial
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepVkbUiState* CAknFepVkbUiStateMgr::UiStateInitial()
    {
    return iUiStateInitial;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::UiStateStandby
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepVkbUiState* CAknFepVkbUiStateMgr::UiStateStandby()
    {
    return iUiStateStandby;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::UiStateStandbycn
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepVkbUiState* CAknFepVkbUiStateMgr::UiStateStandbycn()
    {
    return iUiStateStandbycn;
    }   
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::UiStateStandbycnWithChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepVkbUiState* CAknFepVkbUiStateMgr::UiStateStandbycnWithChars()
    {
    return iUiStateStandbyWitchChars;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::UiStateCompositionNoChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepVkbUiState* CAknFepVkbUiStateMgr::UiStateCompositionNoChars()
    {
    return iUiStateCompositionNoChars;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::UiStateCompositionWithChars
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepVkbUiState* CAknFepVkbUiStateMgr::UiStateCompositionWithChars()
    {
    return iUiStateCompositionWithChars;
    }
    
// -----------------------------------------------------------------------------
// CAknFepVkbUiStateMgr::ConstructL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepVkbUiStateMgr::ConstructL(MAknFepVkbLayoutContext* aContext)
    {
    iUiStateInitial = CAknFepVkbUiState::NewL(this,aContext);
    iUiStateStandby = CAknFepVkbUiStateStandby::NewL(this,aContext);
    iUiStateStandbycn = CAknFepVkbUiStateStandbycn::NewL(this,aContext);
    iUiStateStandbyWitchChars = CAknFepVkbUiStateStandbycnWithChars::NewL(this,aContext);
    iUiStateCompositionNoChars = CAknFepVkbUiStateCompositionNoChars::NewL(this,aContext);
    iUiStateCompositionWithChars = CAknFepVkbUiStateCompositionWithChars::NewL(this,aContext);          
    }

// End Of File
