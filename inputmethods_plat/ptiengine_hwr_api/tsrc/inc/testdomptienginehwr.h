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
* Description:               test PtiHwrRecognizer.h
*
*/


#ifndef C_TESTDOMPTIENGINEHWR_H
#define C_TESTDOMPTIENGINEHWR_H

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
_LIT( KtestdomptienginehwrLogPath, "\\logs\\testframework\\testdomptienginehwr\\" ); 
// Log file
_LIT( KtestdomptienginehwrLogFile, "testdomptienginehwr.txt" ); 
_LIT( KtestdomptienginehwrLogFileWithTitle, "testdomptienginehwr_[%S].txt" );

/**
*  CTestDOMPtiengineHWR test class for STIF Test Framework TestScripter.
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CTestDOMPtiengineHWR ) : public CScriptBase
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CTestDOMPtiengineHWR* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CTestDOMPtiengineHWR();

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
    CTestDOMPtiengineHWR( CTestModuleIf& aTestModuleIf );

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

private: // Test PtiHwrRecognizer.h
/*                              class CHwrRecognizer                          */
        /**
     * TestHRSetRangeL test function for testing the 
     *     NewL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRNewL( CStifItemParser& aItem );
    
    /**
     * TestHRGetRangeL test function for testing the 
     *     static functions function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRStaticFuncL( CStifItemParser& aItem );
    
    /**
     * TestHRSetRangeL test function for testing the 
     *     SetRange function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSetRangeL( CStifItemParser& aItem );
    
    /**
     * TestHRGetRangeL test function for testing the 
     *     GetRange function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetRangeL( CStifItemParser& aItem );
    
    /**
     * TestHRAddAuxiliaryRangeL test function for testing the 
     *     AddAuxiliaryRange function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRAddAuxiliaryRangeL( CStifItemParser& aItem );
    
    /**
     * TestHRRemoveAllAuxRangeL test function for testing the 
     *     RemoveAllAuxRange function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRRemoveAllAuxRangeL( CStifItemParser& aItem );
    
    /**
     * TestHRRemoveAuxiliaryRangeL test function for testing the 
     *     RemoveAuxiliaryRange function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRRemoveAuxiliaryRangeL( CStifItemParser& aItem );
    
    /**
     * TestHRSetCandidateNumL test function for testing the 
     *     SetCandidateNum function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSetCandidateNumL( CStifItemParser& aItem );
    
    /**
     * TestHRGetCandidateNumL test function for testing the 
     *     GetCandidateNum function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetCandidateNumL( CStifItemParser& aItem );
    
    /**
     * TestHRStrokeEndMarkL test function for testing the 
     *     StrokeEndMark function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRStrokeEndMarkL( CStifItemParser& aItem );
    
    /**
     * TestHRRecognizeL test function for testing the 
     *     Recognize function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRRecognizeL( CStifItemParser& aItem );
    
    /**
     * TestHRSetInputAreaSizeL test function for testing the 
     *     SetInputAreaSize and SetScreenSize function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSetInputAreaSizeL( CStifItemParser& aItem );
    
    /**
     * TestHRGetEngineVersionL test function for testing the 
     *     GetEngineVersion function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetEngineVersionL( CStifItemParser& aItem );
    
    /**
     * TestHRGetDictVersionL test function for testing the 
     *     GetDictVersion function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetDictVersionL( CStifItemParser& aItem );
    
    /**
     * TestHREnableWordRecognitionL test function for testing the 
     *     EnableWordRecognition and EnableRecognitionDictionary function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHREnableWordRecognitionL( CStifItemParser& aItem );
    
    /**
     * TestHRSetFirstLetterOrderL test function for testing the 
     *     SetFirstLetterOrder function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSetFirstLetterOrderL( CStifItemParser& aItem );
    
    /**
     * TestHRSetNumberModeL test function for testing the 
     *     SetNumberMode function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSetNumberModeL( CStifItemParser& aItem );
    
    /**
     * TestHRGetTopGuideLinePosL test function for testing the 
     *     GetTopGuideLinePos function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetTopGuideLinePosL( CStifItemParser& aItem );
    
    /**
     * TestHRGetBottomGuideLinePosL test function for testing the 
     *     GetBottomGuideLinePos function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetBottomGuideLinePosL( CStifItemParser& aItem );
    
    /**
     * TestHRSetPermittedSymbolSetL test function for testing the 
     *     SetPermittedSymbolSet function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSetPermittedSymbolSetL( CStifItemParser& aItem );
    
    /**
     * TestHRRecognizeWithCharSetL test function for testing the 
     *     RecognizeWithCharSet function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRRecognizeWithCharSetL( CStifItemParser& aItem );
    
    /**
     * TestHRSetAuxCandidateNumL test function for testing the 
     *     SetAuxCandidateNum function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSetAuxCandidateNumL( CStifItemParser& aItem );
    
    /**
     * TestHRGetAuxCandidateNumL test function for testing the 
     *     GetAuxCandidateNum function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetAuxCandidateNumL( CStifItemParser& aItem );
    
    /**
     * TestHRQueryUdmInterfaceL test function for testing the 
     *     QueryUdmInterfaceL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRQueryUdmInterfaceL( CStifItemParser& aItem );
    
    /**
     * TestHRMcrBeginSessionL test function for testing the 
     *     McrBeginSessionL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRMcrBeginSessionL( CStifItemParser& aItem );
    
    /**
     * TestHRMcrAddStrokesL test function for testing the 
     *     McrAddStrokesL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRMcrAddStrokesL( CStifItemParser& aItem );
    
    /**
     * TestHRMcrEndSessionL test function for testing the 
     *     McrEndSessionL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRMcrEndSessionL( CStifItemParser& aItem );
    
    /**
     * TestHRSymbolModelValidL test function for testing the 
     *     SymbolModelValid function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRSymbolModelValidL( CStifItemParser& aItem );
    
    /**
     * TestHRIsSupportUdmL test function for testing the 
     *     IsSupportUdm function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRIsSupportUdmL( CStifItemParser& aItem );
    
    /**
     * TestHRLoadUdmL test function for testing the 
     *     LoadUdmL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRLoadUdmL( CStifItemParser& aItem );
    
    /**
     * TestHRUnLoadUdmL test function for testing the 
     *     UnLoadUdmL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRUnLoadUdmL( CStifItemParser& aItem );
    
    /**
     * TestHRGetModelIndexListL test function for testing the 
     *     GetModelIndexListL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetModelIndexListL( CStifItemParser& aItem );
    
    /**
     * TestHRGetSymbolModelL test function for testing the 
     *     GetSymbolModelL function
     * @since S60 5.0
     * @param aItem never used
     * @return Symbian OS error code.
     */
    virtual TInt TestHRGetSymbolModelL( CStifItemParser& aItem );

private:    // Data

    /**
     * ScreenSaver Property
     */
    TInt iOldScreenSaverProperty;


    };


#endif      // C_TESTDOMPTIENGINEHWR_H

// End of File


