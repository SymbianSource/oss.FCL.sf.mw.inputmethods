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
* Description: Implementation of popup state of IME Split-Qwerty
*
*/


// System includes
#include <peninputcmd.h>
#include <aknfeppeninputenums.h>
#include <peninputdataprovider.h>
#include <peninputlayoutcontext.h>
#include <peninputdataconverter.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "peninputsplitqwerty.hrh"
#include "peninputsplitqwertyuistatepopup.h"
#include "peninputsplitqwertyenum.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyUiStatePopup* CPeninputSplitQwertyUiStatePopup::NewL( 
                                            MPeninputUiStateMgr* aUiStateMgr, 
                                            MPeninputLayoutContext* aContext )
    {
    CPeninputSplitQwertyUiStatePopup* self = NULL;
    self = new ( ELeave ) CPeninputSplitQwertyUiStatePopup( aUiStateMgr, 
                                                            aContext );
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyUiStatePopup::~CPeninputSplitQwertyUiStatePopup()
    {
    }

// ---------------------------------------------------------------------------
// Process key event
// ---------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStatePopup::HandleKeyEventL( const TRawEvent& aData )
    {
    TInt scanCode = aData.ScanCode();
    iContext->SendEventToWindow( EPenInputLayoutEventClosePopup, NULL );
    
    if(scanCode == EStdKeyYes) // send key
        {
        return EFalse;        
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// process internal event
// ---------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyUiStatePopup::HandleControlEvent( TInt /*aEventType*/, 
                                                const TDesC& /*aEventData*/ )
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyUiStatePopup::CPeninputSplitQwertyUiStatePopup( 
                                          MPeninputUiStateMgr* aUiStateMgr, 
                                          MPeninputLayoutContext* aContext )
    : CPeninputUiStateBase( aUiStateMgr, aContext )
    {
    }

