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
















// [INCLUDE FILES]
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>

#include "testdompeninputserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestDOMPenInputServer::RunMethodL( CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "TestPISRPeninputServerL", CTestDOMPenInputServer::TestPISRPeninputServerL ),
        ENTRY( "TestPISConnectL", CTestDOMPenInputServer::TestPISConnectL ),
        ENTRY( "TestPISVersionL", CTestDOMPenInputServer::TestPISVersionL ),
        ENTRY( "TestPISCloseL", CTestDOMPenInputServer::TestPISCloseL ),
        ENTRY( "TestPISHandleCommandTL", CTestDOMPenInputServer::TestPISHandleCommandTL ),
        ENTRY( "TestPISHandleCommandL", CTestDOMPenInputServer::TestPISHandleCommandL ),
        ENTRY( "TestPISHandleCommandML", CTestDOMPenInputServer::TestPISHandleCommandML ),
        ENTRY( "TestPISIsVisibleL", CTestDOMPenInputServer::TestPISIsVisibleL ),
        ENTRY( "TestPISIsDimmedL", CTestDOMPenInputServer::TestPISIsDimmedL ),
        ENTRY( "TestPISSetUiLayoutIdL", CTestDOMPenInputServer::TestPISSetUiLayoutIdL ),
        ENTRY( "TestPISSetUiLayoutIdTL", CTestDOMPenInputServer::TestPISSetUiLayoutIdTL ),
        ENTRY( "TestPISSetUiLayoutPosL", CTestDOMPenInputServer::TestPISSetUiLayoutPosL ),
        ENTRY( "TestPISUiLayoutPosL", CTestDOMPenInputServer::TestPISUiLayoutPosL ),
        ENTRY( "TestPISUiLayoutSizeL", CTestDOMPenInputServer::TestPISUiLayoutSizeL ),
        ENTRY( "TestPISResourceChangedL", CTestDOMPenInputServer::TestPISResourceChangedL ),
        ENTRY( "TestPISActivateLayoutL", CTestDOMPenInputServer::TestPISActivateLayoutL ),
        ENTRY( "TestPISGetServerEventDataL", CTestDOMPenInputServer::TestPISGetServerEventDataL ),
        ENTRY( "TestPISServerEventDataNumL", CTestDOMPenInputServer::TestPISServerEventDataNumL ),
        ENTRY( "TestPISAddPeninputServerObserverL", CTestDOMPenInputServer::TestPISAddPeninputServerObserverL ),
        ENTRY( "TestPISRemovePeninputServerObserverL", CTestDOMPenInputServer::TestPISRemovePeninputServerObserverL ),
        ENTRY( "TestPISRemovePeninputServerObserverPL", CTestDOMPenInputServer::TestPISRemovePeninputServerObserverPL ),
        ENTRY( "TestPISSetDisplayModeL", CTestDOMPenInputServer::TestPISSetDisplayModeL ),
        ENTRY( "TestPISClearServerEventL", CTestDOMPenInputServer::TestPISClearServerEventL ),
        ENTRY( "TestPISGetImePluginIdListL", CTestDOMPenInputServer::TestPISGetImePluginIdListL ),
        ENTRY( "TestPISPenSupportLanguagesL", CTestDOMPenInputServer::TestPISPenSupportLanguagesL ),
        ENTRY( "TestPISSetForegroundL", CTestDOMPenInputServer::TestPISSetForegroundL ),
        ENTRY( "TestPISLoseForegroundL", CTestDOMPenInputServer::TestPISLoseForegroundL ),
        ENTRY( "TestPISIsForegroundL", CTestDOMPenInputServer::TestPISIsForegroundL ),
        ENTRY( "TestPISAddPenUiActivationHandlerL", CTestDOMPenInputServer::TestPISAddPenUiActivationHandlerL ),
        ENTRY( "TestPISRemovePenUiActivationHandlerL", CTestDOMPenInputServer::TestPISRemovePenUiActivationHandlerL ),
        ENTRY( "TestPISRemovePenUiActivationHandlerPL", CTestDOMPenInputServer::TestPISRemovePenUiActivationHandlerPL ),
        ENTRY( "TestPISActivatePeninputInNotesL", CTestDOMPenInputServer::TestPISActivatePeninputInNotesL ),
        ENTRY( "TestPISDimUiLayoutL", CTestDOMPenInputServer::TestPISDimUiLayoutL ),
        ENTRY( "TestPISSetDisabledLayoutL", CTestDOMPenInputServer::TestPISSetDisabledLayoutL ),
        ENTRY( "TestPISDisabledLayoutL", CTestDOMPenInputServer::TestPISDisabledLayoutL ),
        ENTRY( "TestPISUpdateAppInfoL", CTestDOMPenInputServer::TestPISUpdateAppInfoL ),
        ENTRY( "TestPISSetAutoOpenL", CTestDOMPenInputServer::TestPISSetAutoOpenL ),
        ENTRY( "TestPISAutoOpenL", CTestDOMPenInputServer::TestPISAutoOpenL ),
        ENTRY( "TestPISBackgroudDefaultOriL", CTestDOMPenInputServer::TestPISBackgroudDefaultOriL ),
        ENTRY( "TestPISSetPreferredUiModeL", CTestDOMPenInputServer::TestPISSetPreferredUiModeL ),
        ENTRY( "TestPISPreferredUiModeL", CTestDOMPenInputServer::TestPISPreferredUiModeL ),
        
        ENTRY( "TestPISHNewL", CTestDOMPenInputServer::TestPISHNewL ),
        ENTRY( "TestPISHDeconstructorL", CTestDOMPenInputServer::TestPISHDeconstructorL ),
        ENTRY( "TestPISHConnectServerL", CTestDOMPenInputServer::TestPISHConnectServerL ),
        ENTRY( "TestPISHSetUiLayoutL", CTestDOMPenInputServer::TestPISHSetUiLayoutL ),
        ENTRY( "TestPISHDisconnectServerL", CTestDOMPenInputServer::TestPISHDisconnectServerL ),
        ENTRY( "TestPISHActivateUiLayoutL", CTestDOMPenInputServer::TestPISHActivateUiLayoutL ),
        ENTRY( "TestPISHSendCommandToServerL", CTestDOMPenInputServer::TestPISHSendCommandToServerL ),
        ENTRY( "TestPISHSendCommandToServerPL", CTestDOMPenInputServer::TestPISHSendCommandToServerPL ),
        ENTRY( "TestPISHSendCommandToServerTPL", CTestDOMPenInputServer::TestPISHSendCommandToServerTPL ),
        ENTRY( "TestPISHSendNewKeyToServerL", CTestDOMPenInputServer::TestPISHSendNewKeyToServerL ),
        ENTRY( "TestPISHSetIcfTextL", CTestDOMPenInputServer::TestPISHSetIcfTextL ),
        ENTRY( "TestPISHSendPromptTextToServerL", CTestDOMPenInputServer::TestPISHSendPromptTextToServerL ),

        // [test cases entries]

        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }




//  [End of File]
