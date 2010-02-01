/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include "peninputsplititutlayoutimp.h"
#include "peninputsplititutlayout.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSplitVirtualItutLayoutImp::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitVirtualItutLayoutImp* CSplitVirtualItutLayoutImp::NewL(TAny* /*aInitParams*/)
    {
    CSplitVirtualItutLayoutImp* self = new (ELeave) CSplitVirtualItutLayoutImp();
    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop( self );
    return self;
    }

// ---------CSplitVirtualItutLayoutImp----------------------------------------
// CSplitVirtualItutLayoutImp::~CSplitVirtualItutLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitVirtualItutLayoutImp::~CSplitVirtualItutLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CSplitVirtualItutLayoutImp::CVirtualItutLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitVirtualItutLayoutImp::CSplitVirtualItutLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CSplitVirtualItutLayoutImp::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitVirtualItutLayoutImp::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CSplitVirtualItutLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CFepUiLayout* CSplitVirtualItutLayoutImp::CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner, 
                                                        const TAny* aData)
    {
    CSplitItutUiLayout* uiLayout = CSplitItutUiLayout::NewL(aLayoutOwner, aData);
    return uiLayout;
    }

// End Of File
