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
* Description:  Implement of class CTruiApplication
*
*/


#include <eikapp.h>

#include "truidocument.h"
#include "truiapplication.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// To create document
// ---------------------------------------------------------------------------
//
CApaDocument* CTruiApplication::CreateDocumentL()
    {  
    // Create an trui document, and return a pointer to it    
    CApaDocument* document = new (ELeave) CTruiDocument( *this );
    return document;
    }

// ---------------------------------------------------------------------------
// Get Uid of AppDll
// ---------------------------------------------------------------------------
//
TUid CTruiApplication::AppDllUid() const
    {
    // Return the UID for the trui application
    return KUidtruiApp;
    }
    
// ---------------------------------------------------------------------------
// Opens the .ini file associated with the application.
// ---------------------------------------------------------------------------
//
CDictionaryStore* CTruiApplication::OpenIniFileLC( RFs& aFs ) const
    {
    return CEikApplication::OpenIniFileLC( aFs );
    }
