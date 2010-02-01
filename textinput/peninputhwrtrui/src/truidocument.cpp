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
* Description:  Implement of class CTruiDocument
*
*/


#include "truiappui.h"
#include "truidocument.h"
#include "truiapplication.h"

// ======== MEMBER FUNCTIONS ======== 

// ---------------------------------------------------------------------------
// Standard Symbian OS construction
// ---------------------------------------------------------------------------
//
CTruiDocument::CTruiDocument( CAknApplication& aApp ) : CAknDocument( aApp ) 
    {
    }

// ---------------------------------------------------------------------------
// Standard Symbian OS construction
// ---------------------------------------------------------------------------
//
CTruiDocument::~CTruiDocument()
    {
    }

// ---------------------------------------------------------------------------
// Create the application user interface, and return a pointer to it,
// the framework takes ownership of this object
// ---------------------------------------------------------------------------
//
CEikAppUi* CTruiDocument::CreateAppUiL()
    {
    CEikAppUi* appUi = new (ELeave) CTruiAppUi;
    return appUi;
    }

