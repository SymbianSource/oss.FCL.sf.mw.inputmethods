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
* Description:  test AknInputLanguageInfo.h
*
*/



#ifndef C_TESTPUBINPUTLANGUAGEINFO_H
#define C_TESTPUBINPUTLANGUAGEINFO_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <AknInputLanguageInfo.h>

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KtestpubinputlanguageinfoLogPath, "\\logs\\testframework\\testpubinputlanguageinfo\\" ); 
// Log file
_LIT( KtestpubinputlanguageinfoLogFile, "testpubinputlanguageinfo.txt" ); 
_LIT( KtestpubinputlanguageinfoLogFileWithTitle, "testpubinputlanguageinfo_[%S].txt" );

/**
*  CTestPubInputLanguageInfo test class for STIF Test Framework TestScripter.
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CTestPubInputLanguageInfo ) : public CScriptBase
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CTestPubInputLanguageInfo* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CTestPubInputLanguageInfo();

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
    CTestPubInputLanguageInfo( CTestModuleIf& aTestModuleIf );

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

private: // Testing for TAknInputLanguageCapabilities
    /**
    * TestTAknInputLanguageCapabilitiesL test method of TAknInputLanguageCapabilities.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestTAknInputLanguageCapabilitiesL( CStifItemParser& /*aItem*/ );
    /**
    * TestAssignCapabilityL test method of TAknInputLanguageCapabilities.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestAssignCapabilityL( CStifItemParser& /*aItem*/ );
    /**
    * TestHasCapabilityL test method of TAknInputLanguageCapabilities.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestHasCapabilityL( CStifItemParser& /*aItem*/ );
    /**
    * TestSetAllCapabilitiesL test method of TAknInputLanguageCapabilities.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestSetAllCapabilitiesL( CStifItemParser& /*aItem*/ );
    /**
    * TestHasAnySupportL test method of TAknInputLanguageCapabilities.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestHasAnySupportL( CStifItemParser& /*aItem*/ );
    
private: // testing for CAknInputLanguageList
    /**
    * TestCAknInputLanguageListL test method of CAknInputLanguageList.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestCAknInputLanguageListL( CStifItemParser& /*aItem*/ );
    /**
    * TestMdcaCountL test method of CAknInputLanguageList.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestMdcaCountL( CStifItemParser& /*aItem*/ );
    /**
    * TestMdcaPointL test method of CAknInputLanguageList.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestMdcaPointL( CStifItemParser& /*aItem*/ );
    
private: // testing for AknInputLanguageInfoFactory
    /**
    * TestCreateInputLanguageInfoLL test method of AknInputLanguageInfoFactory.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestCreateInputLanguageInfoLL( CStifItemParser& /*aItem*/ );
    
private: // testing for CAknInputLanguageItem
    /**
    * TestLanguageCodeL test method of CAknInputLanguageItem.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestLanguageCodeL( CStifItemParser& /*aItem*/ );    
    /**
    * TestLanguageNameL test method of CAknInputLanguageItem.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestLanguageNameL( CStifItemParser& /*aItem*/ );    
    /**
    * TestCapabilitiesL test method of CAknInputLanguageItem.
    * @since S60 5.0
    * @param aItem is not used.
    * @return Symbian OS error code.
    */
    virtual TInt TestCapabilitiesL( CStifItemParser& /*aItem*/ );    
	
private:    // Data

    /**
     * ScreenSaver Property
     */
    TInt iOldScreenSaverProperty;
    
    };

#endif      // C_TESTPUBINPUTLANGUAGEINFO_H

// End of File
