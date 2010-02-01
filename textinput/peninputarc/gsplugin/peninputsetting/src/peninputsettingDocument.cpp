/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  implement of class cpeninputsettingdocument
*
*/


#include "peninputsettingAppui.h"
#include "peninputsettingDocument.h"

// ======== MEMBER FUNCTIONS ======== 

// ---------------------------------------------------------------------------
// Standard Symbian OS construction
// ---------------------------------------------------------------------------
//
CPenInputSettingDocument* CPenInputSettingDocument::NewL(CEikApplication& aApp)
    {
    CPenInputSettingDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }
    
// ---------------------------------------------------------------------------
// Standard Symbian OS construction
// ---------------------------------------------------------------------------
//
CPenInputSettingDocument* CPenInputSettingDocument::NewLC(CEikApplication& aApp)
    {
    CPenInputSettingDocument* self = new (ELeave) CPenInputSettingDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// ConstructL is called by the application framework
// ---------------------------------------------------------------------------
//
void CPenInputSettingDocument::ConstructL()
    {
	// no implementation required
    }    

// ---------------------------------------------------------------------------
// ConstructL is called by the application framework
// ---------------------------------------------------------------------------
//
CPenInputSettingDocument::CPenInputSettingDocument(CEikApplication& aApp) : CAknDocument(aApp) 
    {
	// no implementation required
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CPenInputSettingDocument::~CPenInputSettingDocument()
    {
	// no implementation required
    }

// ---------------------------------------------------------------------------
// Create the application user interface, and return a pointer to it,
// the framework takes ownership of this object
// ---------------------------------------------------------------------------
//
CEikAppUi* CPenInputSettingDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CPenInputSettingAppUi;
    return appUi;
    }

