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
* Description:  test functions in aknfep uiinterface api
*
*/



#ifndef C_TESTPLATAU_H
#define C_TESTPLATAU_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <aknfepuiinterface.h>

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KtestplatauLogPath, "\\logs\\testframework\\testplatau\\" ); 
// Log file
_LIT( KtestplatauLogFile, "testplatau.txt" ); 
_LIT( KtestplatauLogFileWithTitle, "testplatau_[%S].txt" );

/**
*  CTestPlatAU test class for STIF Test Framework TestScripter.
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CTestPlatAU ) : public CScriptBase
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CTestPlatAU* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CTestPlatAU();

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
    CTestPlatAU( CTestModuleIf& aTestModuleIf );

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

private: // [TestMethods]
	
	//AknFepUiMenus.h
    /**
    * TestAUDynInitMenuPaneL test method of MAknFepUiInterfaceMenuObserver.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestAUDynInitMenuPaneL( CStifItemParser& /*aItem*/ );
    
    /**
    * TestAUDynInitMenuBarL test method of MAknFepUiInterfaceMenuObserver.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestAUDynInitMenuBarL( CStifItemParser& /*aItem*/ );
    
    //AknFepUiInterface.h
    /**
    * TestCAknUINewL test method of CAknFepUIInterface.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestCAknUINewL( CStifItemParser& /*aItem*/ );
    
private:    // Data

    /**
     * ScreenSaver Property
     */
    TInt iOldScreenSaverProperty;
    


    };

#endif      // C_TESTPLATAU_H

// End of File
