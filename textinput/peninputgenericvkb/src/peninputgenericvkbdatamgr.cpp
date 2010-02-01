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
* Description:  peninput VKB data manager
*
*/


// System includes
#include <peninputdataconverter.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include <peninputlayoutcontext.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "peninputgenericvkbdatamgr.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbDataMgr* CPeninputGenericVkbDataMgr::NewL( 
    MPeninputLayoutContext* aContext )
    {
    CPeninputGenericVkbDataMgr* self = NewLC( aContext );
    CleanupStack::Pop(self);
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::NewLC
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbDataMgr* CPeninputGenericVkbDataMgr::NewLC(
    MPeninputLayoutContext* aContext )
    {
    CPeninputGenericVkbDataMgr* self = 
        new ( ELeave ) CPeninputGenericVkbDataMgr( aContext );
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::CPeninputGenericVkbDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbDataMgr::CPeninputGenericVkbDataMgr(
    MPeninputLayoutContext* aContext ) 
    : CPeninputDataMgr( aContext )
    {
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::~CPeninputGenericVkbDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbDataMgr::~CPeninputGenericVkbDataMgr()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::InitMore
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbDataMgr::InitMore()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::HandleGSRepositoryChange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbDataMgr::HandleGSRepositoryChange( TInt aChangedKey )
    {
    if ( aChangedKey == KSettingsPenInputLang )
        {
        // Read peninput language again 
        // Do what you want
        }
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::Reset
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbDataMgr::Reset()
    {
    iShiftDown = 0;
    iCapslockDown = 0;
    }
