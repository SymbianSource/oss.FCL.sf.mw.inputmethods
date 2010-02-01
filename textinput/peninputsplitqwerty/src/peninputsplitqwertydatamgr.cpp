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
#include <SettingsInternalCRKeys.h>
#include <peninputlayoutcontext.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "peninputsplitqwertydatamgr.h"

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyDataMgr* CPeninputSplitQwertyDataMgr::NewL( 
                                            MPeninputLayoutContext* aContext )
    {
    CPeninputSplitQwertyDataMgr* self = NewLC( aContext );
    CleanupStack::Pop(self);
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyDataMgr* CPeninputSplitQwertyDataMgr::NewLC(
                                            MPeninputLayoutContext* aContext )
    {
    CPeninputSplitQwertyDataMgr* 
        self = new ( ELeave ) CPeninputSplitQwertyDataMgr( aContext );
    
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyDataMgr::~CPeninputSplitQwertyDataMgr()
    {
    }

// ---------------------------------------------------------------------------
// Additional init
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyDataMgr::InitMore()
    {
    }

// ---------------------------------------------------------------------------
// Call back function, which is called when some key changed
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyDataMgr::HandleGSRepositoryChange( TInt aChangedKey )
    {
    if ( aChangedKey == KSettingsPenInputLang )
        {
        }
    }

// ---------------------------------------------------------------------------
// Reset data contained in data manager
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyDataMgr::Reset()
    {
    iShiftDown = 0;
    iCapslockDown = 0;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyDataMgr::CPeninputSplitQwertyDataMgr(
                                            MPeninputLayoutContext* aContext ) 
    : CPeninputDataMgr( aContext )
    {
    }
