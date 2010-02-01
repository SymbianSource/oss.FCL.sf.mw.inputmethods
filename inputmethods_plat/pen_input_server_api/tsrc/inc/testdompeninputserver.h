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
















#ifndef C_TESTDOMPENINPUTSERVER_H
#define C_TESTDOMPENINPUTSERVER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <peninputsrveventhandler.h>

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KtestdompeninputserverLogPath, "\\logs\\testframework\\testdompeninputserver\\" ); 
// Log file
_LIT( KtestdompeninputserverLogFile, "testdompeninputserver.txt" ); 
_LIT( KtestdompeninputserverLogFileWithTitle, "testdompeninputserver_[%S].txt" );

/**
*  CTestDOMPenInputServer test class for STIF Test Framework TestScripter.
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CTestDOMPenInputServer ) : public CScriptBase
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CTestDOMPenInputServer* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CTestDOMPenInputServer();

public: // Functions from base classes

    /**
    * From CScriptBase Runs a script line.
    * @since S60 5.0
    * @param aItem Script line containing method name and parameters
    * @return Symbian OS error code
    */
    virtual TInt RunMethodL( CStifItemParser& aItem );

private:

    /**
    * C++ default constructor.
    */
    CTestDOMPenInputServer( CTestModuleIf& aTestModuleIf );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();
    
    /**
     * Method used to log version of test class
     */
    void SendTestClassVersion();
    
        /**
     * Turn off ScreenSaver
     * @since S60 5.0
     * @return Symbian OS error code.
     */
    void TurnOffScreenSaver();

    /**
     * Restore ScreenSaver
     * @since S60 5.0
     * @return Symbian OS error code.
     */
    void RestoreScreenSaver();

private: // Test peninputclient.h
/*                               class RPeninputServer                        */
    /**
     * TestPISRPeninputServerL test function for testing the 
     *     RPeninputServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISRPeninputServerL( CStifItemParser& aItem );
    
    /**
     * TestPISConnectL test function for testing the 
     *     Connect function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISConnectL( CStifItemParser& aItem );
    
    /**
     * TestPISVersionL test function for testing the 
     *     Version function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISVersionL( CStifItemParser& aItem );
    
    /**
     * TestPISCloseL test function for testing the 
     *     Close function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISCloseL( CStifItemParser& aItem );
    
    /**
     * TestPISHandleCommandTL test function for testing the 
     *     HandleCommand function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHandleCommandTL( CStifItemParser& aItem );
    
    /**
     * TestPISHandleCommandL test function for testing the 
     *     HandleCommand function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHandleCommandL( CStifItemParser& aItem );
    
    /**
     * TestPISHandleCommandML test function for testing the 
     *     HandleCommand function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHandleCommandML( CStifItemParser& aItem );
    
    /**
     * TestPISIsVisibleL test function for testing the 
     *     IsVisible function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISIsVisibleL( CStifItemParser& aItem );
    
    /**
     * TestPISIsDimmedL test function for testing the 
     *     IsDimmed function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISIsDimmedL( CStifItemParser& aItem );
    
    /**
     * TestPISSetUiLayoutIdL test function for testing the 
     *     SetUiLayoutId function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetUiLayoutIdL( CStifItemParser& aItem );
    
    /**
     * TestPISSetUiLayoutIdTL test function for testing the 
     *     SetUiLayoutId function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetUiLayoutIdTL( CStifItemParser& aItem );
    /**
     * TestPISSetUiLayoutPosL test function for testing the 
     *     SetUiLayoutPos function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetUiLayoutPosL( CStifItemParser& aItem );
    
    /**
     * TestPISUiLayoutPosL test function for testing the 
     *     UiLayoutPos function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISUiLayoutPosL( CStifItemParser& aItem );
    
    /**
     * TestPISUiLayoutSizeL test function for testing the 
     *     UiLayoutSize function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISUiLayoutSizeL( CStifItemParser& aItem );
    
    /**
     * TestPISResourceChangedL test function for testing the 
     *     ResourceChanged and SetResourceChange function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISResourceChangedL( CStifItemParser& aItem );
    
    /**
     * TestPISActivateLayoutL test function for testing the 
     *     ActivateLayout function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISActivateLayoutL( CStifItemParser& aItem );
    
    /**
     * TestPISGetServerEventDataL test function for testing the 
     *     GetServerEventData, SetEditorPriority, SetInternalPopUp and 
     * SetGlobalNotes function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISGetServerEventDataL( CStifItemParser& aItem );
    
    /**
     * TestPISServerEventDataNumL test function for testing the 
     *     ServerEventDataNum function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISServerEventDataNumL( CStifItemParser& aItem );
    
    /**
     * TestPISAddPeninputServerObserverL test function for testing the 
     *     AddPeninputServerObserverL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISAddPeninputServerObserverL( CStifItemParser& aItem );
    
    /**
     * TestPISRemovePeninputServerObserverL test function for testing the 
     *     RemovePeninputServerObserver function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISRemovePeninputServerObserverL( CStifItemParser& aItem );
    
    /**
     * TestPISRemovePeninputServerObserverPL test function for testing the 
     *     RemovePeninputServerObserver function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISRemovePeninputServerObserverPL( CStifItemParser& aItem );
    /**
     * TestPISSetDisplayModeL test function for testing the 
     *     SetDisplayMode function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetDisplayModeL( CStifItemParser& aItem );
    
    /**
     * TestPISClearServerEventL test function for testing the 
     *     ClearServerEvent function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISClearServerEventL( CStifItemParser& aItem );
    
    /**
     * TestPISGetImePluginIdListL test function for testing the 
     *     GetImePluginIdListL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISGetImePluginIdListL( CStifItemParser& aItem );
    
    /**
     * TestPISPenSupportLanguagesL test function for testing the 
     *     PenSupportLanguagesL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISPenSupportLanguagesL( CStifItemParser& aItem );
    
    /**
     * TestPISSetForegroundL test function for testing the 
     *     SetForeground function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetForegroundL( CStifItemParser& aItem );
    
    /**
     * TestPISLoseForegroundL test function for testing the 
     *     LoseForeground function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISLoseForegroundL( CStifItemParser& aItem );
    
    /**
     * TestPISIsForegroundL test function for testing the 
     *     IsForeground function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISIsForegroundL( CStifItemParser& aItem );
    
    /**
     * TestPISAddPenUiActivationHandlerL test function for testing the 
     *     AddPenUiActivationHandler function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISAddPenUiActivationHandlerL( CStifItemParser& aItem );
    
    /**
     * TestPISRemovePenUiActivationHandlerL test function for testing the 
     *     RemovePenUiActivationHandler function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISRemovePenUiActivationHandlerL( CStifItemParser& aItem );
    
    /**
     * TestPISRemovePenUiActivationHandlerPL test function for testing the 
     *     RemovePenUiActivationHandler function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISRemovePenUiActivationHandlerPL( CStifItemParser& aItem );
    /**
     * TestPISActivatePeninputInNotesL test function for testing the 
     *     ActivatePeninputInNotesL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISActivatePeninputInNotesL( CStifItemParser& aItem );
    
    /**
     * TestPISDimUiLayoutL test function for testing the 
     *     DimUiLayout and ClearTouchUI function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISDimUiLayoutL( CStifItemParser& aItem );
    
    /**
     * TestPISSetDisabledLayoutL test function for testing the 
     *     SetDisabledLayout function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetDisabledLayoutL( CStifItemParser& aItem );
    
    /**
     * TestPISDisabledLayoutL test function for testing the 
     *     DisabledLayout function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISDisabledLayoutL( CStifItemParser& aItem );
    
    /**
     * TestPISUpdateAppInfoL test function for testing the 
     *     UpdateAppInfo function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISUpdateAppInfoL( CStifItemParser& aItem );
    
    /**
     * TestPISSetAutoOpenL test function for testing the 
     *     SetAutoOpen function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetAutoOpenL( CStifItemParser& aItem );
    
    /**
     * TestPISAutoOpenL test function for testing the 
     *     AutoOpen function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISAutoOpenL( CStifItemParser& aItem );
    
    /**
     * TestPISBackgroudDefaultOriL test function for testing the 
     *     BackgroudDefaultOri function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISBackgroudDefaultOriL( CStifItemParser& aItem );
    
    /**
     * TestPISSetPreferredUiModeL test function for testing the 
     *     SetPreferredUiMode function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISSetPreferredUiModeL( CStifItemParser& aItem );
    
    /**
     * TestPISPreferredUiModeL test function for testing the 
     *     PreferredUiMode function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISPreferredUiModeL( CStifItemParser& aItem );

private: // Test peninputserverhandler.h
/*                          class CPenInputServerHandler                      */
    /**
     * TestPISHNewL test function for testing the 
     *     NewL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHNewL( CStifItemParser& aItem );

    /**
     * TestPISHDeconstructorL test function for testing the 
     *     ~CPenInputServerHandler function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHDeconstructorL( CStifItemParser& aItem );

    /**
     * TestPISHConnectServerL test function for testing the 
     *     ConnectServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHConnectServerL( CStifItemParser& aItem );

    /**
     * TestPISHSetUiLayoutL test function for testing the 
     *     SetUiLayout function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHSetUiLayoutL( CStifItemParser& aItem );

    /**
     * TestPISHDisconnectServerL test function for testing the 
     *     DisconnectServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHDisconnectServerL( CStifItemParser& aItem );

    /**
     * TestPISHActivateUiLayoutL test function for testing the 
     *     ActivateUiLayout function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHActivateUiLayoutL( CStifItemParser& aItem );

    /**
     * TestPISHSendCommandToServerL test function for testing the 
     *     SendCommandToServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHSendCommandToServerL( CStifItemParser& aItem );

    /**
     * TestPISHSendCommandToServerPL test function for testing the 
     *     SendCommandToServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHSendCommandToServerPL( CStifItemParser& aItem );

    /**
     * TestPISHSendCommandToServerTPL test function for testing the 
     *     SendCommandToServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHSendCommandToServerTPL( CStifItemParser& aItem );

    /**
     * TestPISHSendNewKeyToServerL test function for testing the 
     *     SendNewKeyToServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHSendNewKeyToServerL( CStifItemParser& aItem );

    /**
     * TestPISHSetIcfTextL test function for testing the 
     *     SetIcfText function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHSetIcfTextL( CStifItemParser& aItem );

    /**
     * TestPISHSendPromptTextToServerL test function for testing the 
     *     SendPromptTextToServer function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestPISHSendPromptTextToServerL( CStifItemParser& aItem );
    
private:    // Data

    /**
     * ScreenSaver Property
     */
    TInt iOldScreenSaverProperty;


    };

    /**
     * class TTestObserver derive from MPeninputServerEventHandler
     */
class TTestObserver : public MPeninputServerEventHandler
    {
public:
    /**
     * from MPeninputServerEventHandler
     */
    TBool HandleServerEventL( TInt /*aEventId*/ )
        {
        return ETrue;
        }
    };

/**
 * class TTestHandleObserver derive from MPenUiActivationHandler
 */
class TTestHandleObserver : public MPenUiActivationHandler
    {
public:
    /**
     * from MPenUiActivationHandler
     */
    void OnPeninputUiDeactivated()
        {
        
        }
    
    /**
     * from MPenUiActivationHandler
     */
    void OnPeninputUiActivated()
        {
        
        }
    };

#endif      // C_TESTDOMPENINPUTSERVER_H

// End of File

