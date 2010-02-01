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
* Description:               test peninputserverhandler.h
*
*/














#include <peninputserverhandler.h>

#include "testdompeninputserver.h"

const TInt KLength = 10;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHNewL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHNewL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHDeconstructorL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHDeconstructorL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    CleanupStack::Pop( handler );
    delete handler;
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHConnectServerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHConnectServerL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHSetUiLayoutL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHSetUiLayoutL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    handler->SetUiLayout( 0 );
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHDisconnectServerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHDisconnectServerL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHActivateUiLayoutL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHActivateUiLayoutL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    handler->SetUiLayout( 0 );
    handler->ActivateUiLayout( ETrue );
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHSendCommandToServerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHSendCommandToServerL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    handler->SendCommandToServer( 0 );
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHSendCommandToServerPL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHSendCommandToServerPL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    handler->SendCommandToServer( 0, 0 );
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHSendCommandToServerTPL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHSendCommandToServerTPL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    TFileName buf;
    handler->SendCommandToServer( 0, buf );
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHSendNewKeyToServerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHSendNewKeyToServerL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    TItutBmp bmp;
    TAknTextLineLayout textFormat;
    _LIT( KTest, "tset" );
    TBufC<KLength> bufc( KTest );
    TItutKey key =  { 0, bmp, 0, 0, bufc, textFormat };
    handler->SendNewKeyToServer( 0, key );
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHSetIcfTextL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHSetIcfTextL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    TFepInputContextFieldData data;
    handler->SetIcfText( data );
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHSendPromptTextToServerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHSendPromptTextToServerL( CStifItemParser& /*aItem*/ )
    {
    CPenInputServerHandler* handler = CPenInputServerHandler::NewL();
    CleanupStack::PushL( handler );
    STIF_ASSERT_NOT_NULL( handler );
    
    handler->ConnectServer();
    TFepPromptText text;
    handler->SendPromptTextToServer( 0, text );
    handler->DisconnectServer();
    
    CleanupStack::PopAndDestroy( handler );
    
    return KErrNone;
    
    }

