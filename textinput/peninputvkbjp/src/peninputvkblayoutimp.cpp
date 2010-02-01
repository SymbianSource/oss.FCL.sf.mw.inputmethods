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
* Description:  Implementation of the vkb layout
*
*/


// system includes

// user includes
#include "peninputvkblayoutimp.h"
#include "peninputvkblayout.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputVkbLayoutImp::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbLayoutImp* CPeninputVkbLayoutImp::NewL( TAny* /*aInitParams*/ )
    {
    CPeninputVkbLayoutImp* self = new(ELeave) CPeninputVkbLayoutImp();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayoutImp::~CPeninputVkbLayoutImp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbLayoutImp::~CPeninputVkbLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiLayout* CPeninputVkbLayoutImp::CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,const TAny* aData)
    {
    CPeninputVkbLayout* aUiLayout = CPeninputVkbLayout::NewL( aLayoutOwner, aData );

    return aUiLayout;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayoutImp::CPeninputVkbLayoutImp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbLayoutImp::CPeninputVkbLayoutImp()
    {
    // See ConstructL() for initialisation completion.
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayoutImp::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbLayoutImp::ConstructL()
    {
    }

// End Of File
