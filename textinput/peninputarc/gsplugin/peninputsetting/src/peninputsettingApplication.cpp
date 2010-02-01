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
* Description:  Implement of class PenInputSettingApplication
*
*/


#include "peninputsettingDocument.h"
#include "peninputsettingApplication.h"

static const TUid KUidPenInputSettingApp = { 0x2001959B };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPenInputSettingApplication::CreateDocumentL()
// To create document
// ---------------------------------------------------------------------------
//
CApaDocument* CPenInputSettingApplication::CreateDocumentL()
    {  
    // Create an peninputsetting document, and return a pointer to it
    CApaDocument* document = CPenInputSettingDocument::NewL(*this);
    return document;
    }
    
// ---------------------------------------------------------------------------
// CPenInputSettingApplication::AppDllUid()
// Get Uid of AppDll
// ---------------------------------------------------------------------------
//
TUid CPenInputSettingApplication::AppDllUid() const
    {
    // Return the UID for the peninputsetting application
    return KUidPenInputSettingApp;
    }

