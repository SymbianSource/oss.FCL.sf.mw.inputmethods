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
* Description:            VKB layout UI class implementation file.
*
*/












// System includes
#include <aknedsts.h>               // CAknEdwinState
#include <eikon.hrh>

// User includes
#include "pluginfepmanagervkb.h"
#include "peninputimeplugincn.h"

// ======== MEMBER FUNCTIONS ========


// -----------------------------------------------------------------------------
// CPluginFepManagerVkb::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerVkb* CPluginFepManagerVkb::NewL(CPenInputImePluginCn& aOwner,
                                                 RPeninputServer* aPenInputServer)
    {
    CPluginFepManagerVkb* self = new(ELeave)CPluginFepManagerVkb(aOwner, aPenInputServer);
    CleanupStack::PushL(self);
    self->BaseConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerVkb::CPluginFepManagerVkb
// constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerVkb::CPluginFepManagerVkb(CPenInputImePluginCn& aOwner,
                                           RPeninputServer* aPenInputServer)
    :CPluginFepManagerBase(aOwner, aPenInputServer)
    {
    }

// End Of File
