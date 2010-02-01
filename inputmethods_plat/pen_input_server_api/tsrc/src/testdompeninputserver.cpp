/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               test peninputclient.h
*
*/
















// INCLUDE FILES
#include <StifTestInterface.h>
#include <SettingServerClient.h>
#include <ScreensaverInternalPSKeys.h>
#include <e32property.h>

#include "testdompeninputserver.h"

// CONSTANTS
_LIT( KModuleName, "testdompeninputserver.dll" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::CTestDOMPenInputServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestDOMPenInputServer::CTestDOMPenInputServer( CTestModuleIf& aTestModuleIf ):
    CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::~CTestDOMPenInputServer
// Destructor.
// -----------------------------------------------------------------------------
//
CTestDOMPenInputServer::~CTestDOMPenInputServer()
    {
    // Delete logger
    delete iLog; 

    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestDOMPenInputServer::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    CleanupClosePushL( settingServer );
    TInt ret = settingServer.Connect();
    if ( ret != KErrNone )
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings( loggerSettings );
    if ( ret != KErrNone )
        {
        User::Leave( ret );
        } 
    // Close Setting server session
    settingServer.Close();
    CleanupStack::PopAndDestroy( &settingServer );

    TFileName logFileName;
    
    if ( loggerSettings.iAddTestCaseTitle )
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL( title );
        logFileName.Format( KtestdompeninputserverLogFileWithTitle, &title );
        }
    else
        {
        logFileName.Copy( KtestdompeninputserverLogFile );
        }

    iLog = CStifLogger::NewL( KtestdompeninputserverLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    
    TurnOffScreenSaver();
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestDOMPenInputServer* CTestDOMPenInputServer::NewL( CTestModuleIf& aTestModuleIf )
    {
    CTestDOMPenInputServer* self = new( ELeave ) CTestDOMPenInputServer( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;

    }

//-----------------------------------------------------------------------------
// CTestDOMPenInputServer::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CTestDOMPenInputServer::SendTestClassVersion()
    {
    TVersion moduleVersion;
    moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
    moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
    moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;

    TFileName moduleName;
    moduleName = KModuleName;

    TBool newVersionOfMethod = ETrue;
    TestModuleIf().SendTestModuleVersion( moduleVersion, moduleName, 
        newVersionOfMethod );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {
    return ( CScriptBase* ) CTestDOMPenInputServer::NewL( aTestModuleIf );
    }

// -----------------------------------------------------------------------------
// Turn off ScreenSaver
// -----------------------------------------------------------------------------
//
void CTestDOMPenInputServer::TurnOffScreenSaver()
    {
    TInt err1 = RProperty::Get( KPSUidScreenSaver, KScreenSaverAllowScreenSaver, 
        iOldScreenSaverProperty );
    TInt err2 = RProperty::Set( KPSUidScreenSaver, KScreenSaverAllowScreenSaver, 
        KScreenSaverAllowScreenSaver );    
    RDebug::Printf( "screensaver property=%d err1=%d err2=%d\n", 
        iOldScreenSaverProperty, err1, err2 );
    }

// -----------------------------------------------------------------------------
// Restore ScreenSaver
// -----------------------------------------------------------------------------
//
void CTestDOMPenInputServer::RestoreScreenSaver()
    {
    RProperty::Set( KPSUidScreenSaver, KScreenSaverAllowScreenSaver, 
        iOldScreenSaverProperty );
    User::ResetInactivityTime();
    }


//  End of File
