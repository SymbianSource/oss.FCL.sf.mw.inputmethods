/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for peninput server application document
*
*/


// INCLUDE FILES
#include "peninputserverdocument.h"
#include "peninputserverappui.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::CPeninputServerDocument
// ---------------------------------------------------------------------------
//
CPeninputServerDocument::CPeninputServerDocument(CEikApplication& aApp)
                                                    : CAknDocument(aApp)    
    {
    }

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::~CPeninputServerDocument
// ---------------------------------------------------------------------------
//
CPeninputServerDocument::~CPeninputServerDocument()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::ConstructL
// ---------------------------------------------------------------------------
//
void CPeninputServerDocument::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::NewL
// ---------------------------------------------------------------------------
//
CPeninputServerDocument* CPeninputServerDocument::NewL(CEikApplication& aApp)
    {
    CPeninputServerDocument* self = new (ELeave) CPeninputServerDocument(aApp);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPeninputServerDocument::CreateAppUiL()
// constructs CPeninputServerAppUi
// ---------------------------------------------------------------------------
//
CEikAppUi* CPeninputServerDocument::CreateAppUiL()
    {
    return new (ELeave) CPeninputServerAppUi;
    }

// End of File  
