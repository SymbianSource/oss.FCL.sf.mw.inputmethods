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
* Description:  Implementation for peninput server application
*
*/


// INCLUDE FILES
#include "peninputserverapp.h"
#include "peninputserverdocument.h"


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CPeninputServerApp::AppDllUid()
// Returns application UID
// ---------------------------------------------------------------------------
//
TUid CPeninputServerApp::AppDllUid() const
    {
    return KUidPeninputserver;
    }

// ---------------------------------------------------------------------------
// CPeninputServerApp::~CPeninputServerApp()
// ---------------------------------------------------------------------------
//    
CPeninputServerApp::~CPeninputServerApp()
    {
    }
    
// ---------------------------------------------------------------------------
// CPeninputServerApp::CreateDocumentL()
// Creates CPenInputServerDocument object
// ---------------------------------------------------------------------------
//
CApaDocument* CPeninputServerApp::CreateDocumentL()
    {   
    // create the server (leave it on the cleanup stack)    
    return CPeninputServerDocument::NewL( *this );
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

#ifdef RD_APPS_TO_EXES

#include <eikstart.h>

//
// ---------------------------------------------------------------------------
// NewApplication()
// Constructs CPeninputServerApp
// Returns: created application object
// ---------------------------------------------------------------------------
//
LOCAL_C CApaApplication* NewApplication()
    {
    return new CPeninputServerApp;
    }

//
// ---------------------------------------------------------------------------
// NE32Main()
// Application entry point
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

#if defined(__WINS__) && !defined(EKA2)
GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }

EXPORT_C TInt WinsMain(TDesC* aCmdLine)
    {
    return EikStart::RunApplication(NewApplication, aCmdLine);
    }
#endif

#else  // !RD_APPS_TO_EXES
EXPORT_C CApaApplication* NewApplication()
    {
    return new CPeninputServerApp;
    }

// ---------------------------------------------------------------------------
// E32Dll(TDllReason)
// Entry point function for EPOC Apps
// ---------------------------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif

#endif  // RD_APPS_TO_EXES

// End of File
