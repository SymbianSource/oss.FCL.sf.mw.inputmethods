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
* Description:            ITUT layout UI class implementation file.
*
*/












// System includes
#include <aknedsts.h>               // CAknEdwinState
#include <peninputclient.h>

// User includes
#include "pluginfepmanageritut.h"
#include "peninputimepluginitut.h"
#include "eikon.hrh"

// ======== MEMBER FUNCTIONS ========


// -----------------------------------------------------------------------------
// CPluginFepManagerItut::NewL
// Factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerItut* CPluginFepManagerItut::NewL( 
    CPenInputImePluginItut& aOwner, RPeninputServer* aPenInputServer )
    {
    CPluginFepManagerItut* self = new ( ELeave ) CPluginFepManagerItut(
         aOwner, aPenInputServer );
    CleanupStack::PushL( self );
    self->BaseConstructL(); 
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerItut::CPluginFepManagerItut
// constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerItut::CPluginFepManagerItut( CPenInputImePluginItut& aOwner,
                                            RPeninputServer* aPenInputServer )
    :CPluginFepManagerBase( aOwner, aPenInputServer )
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerItut::HandleKeyL
// Handle key event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerItut::HandleKeyL( TInt aKey, TKeyPressLength aLength, TEventCode /*aEventCode*/ )
    {
    TInt ret;
    TPckgC<TInt> msgdata(aKey);
    
    if (aLength == ELongKeyPress)
        {
        iPenInputServer->HandleCommand(ECmdPenInputFingerLongKeyPress, msgdata, ret);
        }
    else
        {
        iPenInputServer->HandleCommand(ECmdPenInputFingerKeyPress, msgdata, ret);
        }
    
    return ret == KErrNone ? ETrue:EFalse;
    }

// End Of File
