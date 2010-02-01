/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  test ocrsrv.h
*
*/



#ifndef C_TESTSDKOCRECOGNITION_H
#define C_TESTSDKOCRECOGNITION_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KtestsdkocrecognitionLogPath, "\\logs\\testframework\\testsdkocrecognition\\" ); 
// Log file
_LIT( KtestsdkocrecognitionLogFile, "testsdkocrecognition.txt" ); 
_LIT( KtestsdkocrecognitionLogFileWithTitle, "testsdkocrecognition_[%S].txt" );

/**
*  CTestSDKOCRecognition test class for STIF Test Framework TestScripter.
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CTestSDKOCRecognition ) : public CScriptBase
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CTestSDKOCRecognition* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CTestSDKOCRecognition();

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
    CTestSDKOCRecognition( CTestModuleIf& aTestModuleIf );

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

private: // Test ocrsrv.h
/*                               class OCREngineFactory                       */
    /**
     * TestORCCreateOCREngineL test function for testing the 
     *     CreateOCREngineL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestORCCreateOCREngineL( CStifItemParser& aItem );
    
    /**
     * TestORCReleaseOCREngineL test function for testing the 
     *     ReleaseOCREngine function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestORCReleaseOCREngineL( CStifItemParser& aItem );

private:    // Data

    /**
     * ScreenSaver Property
     */
    TInt iOldScreenSaverProperty;

    };

#endif      // C_TESTSDKOCRECOGNITION_H

// End of File
