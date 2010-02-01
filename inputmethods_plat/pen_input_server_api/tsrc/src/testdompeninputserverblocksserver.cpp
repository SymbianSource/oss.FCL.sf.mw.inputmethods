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


#include <peninputclient.h>
#include <e32cmn.h>
#include <gdi.h>

#include "testdompeninputserver.h"

const TInt KLength = 10;
// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISRPeninputServerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISRPeninputServerL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    server.Close();
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISConnectL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISConnectL( CStifItemParser& aItem )
    {
    return TestPISRPeninputServerL( aItem );
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISVersionL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISVersionL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    TVersion version = server.Version();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISCloseL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISCloseL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHandleCommandTL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHandleCommandTL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    _LIT8( KBuf, "server" );
    TBuf8<KLength> buf( KBuf );
    
    TInt flag = server.HandleCommand( 0, buf );
    STIF_ASSERT_EQUALS( KErrNone, flag );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHandleCommandL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHandleCommandL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    TInt flag = server.HandleCommand( 0 );
    STIF_ASSERT_EQUALS( KErrNone, flag );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISHandleCommandML
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISHandleCommandML( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    _LIT8( KBuf, "server" );
    TBuf8<KLength> buf( KBuf );
    TInt resault;
    
    TInt flag = server.HandleCommand( 0, buf, resault );
    STIF_ASSERT_EQUALS( KErrNone, flag );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISIsVisibleL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISIsVisibleL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    server.IsVisible();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISIsDimmedL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISIsDimmedL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    server.IsDimmed();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetUiLayoutIdL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetUiLayoutIdL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    TUid uid = TUid::Uid( 0 );
    TInt flag = server.SetUiLayoutId( uid );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetUiLayoutIdTL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetUiLayoutIdTL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    TUid uid = TUid::Uid( 0 );
    _LIT8( KBuf, "server" );
    TBuf8<KLength> buf( KBuf );
    
    TInt flag = server.SetUiLayoutId( uid, buf );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetUiLayoutPosL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetUiLayoutPosL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    TPoint point( 0, 0 );
    
    TInt flag = server.SetUiLayoutPos( point );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISUiLayoutPosL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISUiLayoutPosL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    TPoint pointGet = server.UiLayoutPos();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISUiLayoutSizeL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISUiLayoutSizeL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    TSize sizeGet = server.UiLayoutSize();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISResourceChangedL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISResourceChangedL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    server.SetResourceChange( ETrue );
    server.ResourceChanged( 0 );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISActivateLayoutL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISActivateLayoutL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    server.ActivateLayout( ETrue );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISGetServerEventDataL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISGetServerEventDataL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    TBuf<KLength> buf;
    TInt flag = server.GetServerEventData( buf );
    
    server.SetEditorPriority( 0 );
    server.SetInternalPopUp( ETrue );
    server.SetGlobalNotes( ETrue );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISServerEventDataNumL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISServerEventDataNumL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    server.ServerEventDataNum();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISAddPeninputServerObserverL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISAddPeninputServerObserverL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    TTestObserver obsersver;
    server.AddPeninputServerObserverL( &obsersver );
    server.RemovePeninputServerObserver();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISRemovePeninputServerObserverL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISRemovePeninputServerObserverL( CStifItemParser& aItem )
    {
    return TestPISAddPeninputServerObserverL( aItem );
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISRemovePeninputServerObserverPL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISRemovePeninputServerObserverPL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    TTestObserver obsersver;
    server.AddPeninputServerObserverL( &obsersver );
    server.RemovePeninputServerObserver( &obsersver );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetDisplayModeL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetDisplayModeL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    server.SetDisplayMode( EGray2, ENone );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISClearServerEventL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISClearServerEventL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    server.ClearServerEvent();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISGetImePluginIdListL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISGetImePluginIdListL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    RArray<TInt> implmentationIds;
    CleanupClosePushL( implmentationIds );
    server.GetImePluginIdListL( 1, 0, implmentationIds );
    
    CleanupStack::PopAndDestroy( &implmentationIds );
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISPenSupportLanguagesL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISPenSupportLanguagesL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    RArray<TInt> implmentationIds;
    CleanupClosePushL( implmentationIds );
    server.PenSupportLanguagesL( implmentationIds );
    
    CleanupStack::PopAndDestroy( &implmentationIds );
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetForegroundL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetForegroundL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    server.SetForeground( ETrue );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISLoseForegroundL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISLoseForegroundL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    server.SetForeground( ETrue );
    server.LoseForeground();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISIsForegroundL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISIsForegroundL( CStifItemParser& aItem )
    {
    return TestPISSetForegroundL( aItem );
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISAddPenUiActivationHandlerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISAddPenUiActivationHandlerL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    TTestHandleObserver observer;

    server.AddPenUiActivationHandler( &observer, 0 );
    server.RemovePenUiActivationHandler();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISRemovePenUiActivationHandlerL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISRemovePenUiActivationHandlerL( CStifItemParser& aItem )
    {
    return TestPISAddPenUiActivationHandlerL( aItem );
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISRemovePenUiActivationHandlerPL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISRemovePenUiActivationHandlerPL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    
    TTestHandleObserver observer;
    TTestHandleObserver observer1;
    
    server.AddPenUiActivationHandler( &observer, 0 );
    server.RemovePenUiActivationHandler( &observer );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISActivatePeninputInNotesL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISActivatePeninputInNotesL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    server.ActivatePeninputInNotesL();
    server.ClearTouchUI();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISDimUiLayoutL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISDimUiLayoutL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();

    server.DimUiLayout( ETrue );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetDisabledLayoutL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetDisabledLayoutL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    server.SetDisabledLayout( 0 );
    server.DisabledLayout();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISDisabledLayoutL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISDisabledLayoutL( CStifItemParser& aItem )
    {
    return TestPISSetDisabledLayoutL( aItem );
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISUpdateAppInfoL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISUpdateAppInfoL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    _LIT( KBuf, "server" );
    TBuf<KLength> buf( KBuf );
    server.UpdateAppInfo( buf, EAppTitle );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetAutoOpenL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetAutoOpenL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    server.SetAutoOpen( ETrue );
    server.AutoOpen();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISAutoOpenL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISAutoOpenL( CStifItemParser& aItem )
    {
    return TestPISSetAutoOpenL( aItem );
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISBackgroudDefaultOriL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISBackgroudDefaultOriL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    server.BackgroudDefaultOri( 0 );
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISSetPreferredUiModeL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISSetPreferredUiModeL( CStifItemParser& /*aItem*/ )
    {
    RPeninputServer server;
    server.Connect();
    server.SetPreferredUiMode( EPluginInputModeNone );
    server.PreferredUiMode();
    
    server.Close();
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPenInputServer::TestPISPreferredUiModeL
// -----------------------------------------------------------------------------
TInt CTestDOMPenInputServer::TestPISPreferredUiModeL( CStifItemParser& aItem )
    {
    return TestPISSetPreferredUiModeL( aItem );
    
    }

// End of file
