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
* Description:  Implementation of the hwr layout
*
*/


// system includes

// user includes
#include "peninputhwrboxlayoutimp.h"
#include "peninputhwrboxlayout.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayoutImp::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxLayoutImp* CPeninputHwrBoxLayoutImp::NewL( TAny* /*aInitParams*/ )
    {
    CPeninputHwrBoxLayoutImp* self = new(ELeave) CPeninputHwrBoxLayoutImp();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayoutImp::~CPeninputHwrBoxLayoutImp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxLayoutImp::~CPeninputHwrBoxLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiLayout* CPeninputHwrBoxLayoutImp::CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,const TAny* aData)
    {
    CPeninputHwrBoxLayout* aUiLayout = CPeninputHwrBoxLayout::NewL( aLayoutOwner, aData );

    return aUiLayout;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayoutImp::CPeninputHwrBoxLayoutImp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxLayoutImp::CPeninputHwrBoxLayoutImp()
    {
    // See ConstructL() for initialisation completion.
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayoutImp::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayoutImp::ConstructL()
    {
    }

// End Of File
