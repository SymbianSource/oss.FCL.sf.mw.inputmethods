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
#include "peninputgenericitutlayoutimp.h"
#include "peninputgenericitutlayout.h"

// ---------------------------------------------------------------------------
// CVirtualItutLayoutImp::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CVirtualItutLayoutImp* CVirtualItutLayoutImp::NewL(TAny* /*aInitParams*/)
    {
    CVirtualItutLayoutImp* self = new (ELeave) CVirtualItutLayoutImp();
    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CVirtualItutLayoutImp::~CVirtualItutLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CVirtualItutLayoutImp::~CVirtualItutLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CVirtualItutLayoutImp::CVirtualItutLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CVirtualItutLayoutImp::CVirtualItutLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CVirtualItutLayoutImp::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CVirtualItutLayoutImp::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CVirtualItutLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CFepUiLayout* CVirtualItutLayoutImp::CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner, 
                                                        const TAny* aData)
    {
    CGenericItutUiLayout* uiLayout = CGenericItutUiLayout::NewL(aLayoutOwner, aData);
    return uiLayout;
    }

// End Of File
