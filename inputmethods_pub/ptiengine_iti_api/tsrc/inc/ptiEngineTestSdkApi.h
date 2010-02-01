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
* Description:               ?Description
*
*/
















#ifndef PTIENGINETESTSDKAPI_H
#define PTIENGINETESTSDKAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <PtiEngine.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KptiEngineTestSdkApiLogPath, "\\logs\\testframework\\ptiEngineTestSdkApi\\" ); 
// Log file
_LIT( KptiEngineTestSdkApiLogFile, "ptiEngineTestSdkApi.txt" ); 
_LIT( KptiEngineTestSdkApiLogFileWithTitle, "ptiEngineTestSdkApi_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CptiEngineTestSdkApi;
class MPtiKeyMappings;
class CPtiKeyMappings;
class CPtiEngine;


// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CptiEngineTestSdkApi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CptiEngineTestSdkApi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CptiEngineTestSdkApi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CptiEngineTestSdkApi();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CptiEngineTestSdkApi( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        virtual TInt PtiUserDictCreateL();
        virtual TInt PtiUserDictSetDataL();
        virtual TInt PtiUserDictSaveDataL();
        virtual TInt PtiUserDictReloadL(); 
        virtual TInt PtiUserDictTestOtherL(); 
        void CreateUserDicFileL(TDesC& aFilename);	
                
        virtual TInt PtiLangDestructionL();
        virtual TInt PtiLangSetupL();
        virtual TInt PtiLangSetKeyMapL();
        virtual TInt PtiLangQwertySetKeyMapL();
        virtual TInt PtiLangHalfQwertySetKeyMapL();
        virtual TInt PtiLangSetLocalizedNameL();
        virtual TInt PtiLangSetLanguageCodeL();
        virtual TInt PtiLangTestOthersL();  
        virtual TInt PtiLangSetSecondaryInputL();
        virtual TInt PtiLangGetHwrRecognizerL();
        virtual TInt PtiLangAddCoreL();
         
        virtual TInt PtiLangDBSetupL();
        virtual TInt PtiLangDBTestImplL();
        virtual TInt PtiLangDBTestInfoL();
        virtual TInt PtiLangDBTestOthersL();
        
        MPtiKeyMappings* CreateKeyMapL();
        virtual TInt PtiKeyMapSetupL();
        virtual TInt PtiKeyTestMappingL();
        virtual TInt PtiTestReplaceMappingL();
        virtual TInt PtiTestReverseMappingL();
        virtual TInt PtiTestKeyDataL();
        virtual TInt PtiKeyMapTestOthersL();
        virtual TInt PtiTestAllKeyDataL();
                  
        MPtiKeyMappings* CreateQwertyKeyMapL();
        bool ValidQwertyMode();
        virtual TInt PtiQwertyTestKeyDataL();
        virtual TInt PtiQwertyTestReverseMappingL();
        virtual TInt PtiQwertyTestReplaceMappingL();
        virtual TInt PtiQwertyTestMappingL();
        virtual TInt PtiQwertyMapSetupL();
        virtual TInt PtiQwertyTestAllKeyDataL();
        virtual TInt PtiQwertyKeyMapTestOthersL();
        
        MPtiKeyMappings* CreateHalfQwertyKeyMapL();     
        virtual TInt PtiHalfQwertyMapSetupL();          
        virtual TInt PtiHalfQwertyTestReplaceMappingL();    
        virtual TInt PtiHalfQwertyTestReverseMappingL();
        virtual TInt PtiHalfQwertyTestAllKeyDataL();
        virtual TInt PtiHalfQwertyTestKeyDataL();
        
        TInt GetCoreUidL(); 
        virtual TInt PtiEngSetup1L();
        virtual TInt PtiEngSetup2L();
        virtual TInt PtiEngAddCoreL();
        virtual TInt PtiEngTestLanguageL();
        virtual TInt PtiEngTestInfoL();
        virtual TInt PtiEngTestMultitappingL();
        virtual TInt PtiEngTestPredictL();
        virtual TInt PtiEngGetCandidatesL();
        virtual TInt PtiEngTestInputModeL();
        virtual TInt PtiEngTestDictL();
        virtual TInt PtiEngTestDefaultUserDictL();
        virtual TInt PtiEngTestSecDataFileL();
        virtual TInt PtiEngTestLastKeyL();
        virtual TInt PtiEngTestNavigationL();
        virtual TInt PtiEngTestReorderingL();
        virtual TInt PtiEngTestAutoSubstitutionL();
        virtual TInt PtiEngTestListKeyboardsL();
        virtual TInt PtiEngTestPhysicalListKeyboardsL();
        virtual TInt PtiEngTestOtherL(); 

        virtual TInt PtiEngTestChineseInputL();
        virtual TInt PtiEngTestJapaneseInputL(); 
        void PtiEngTestSpellL(CPtiEngine *aPtiEngine);
        void PtiEngSimuInput(CPtiEngine* aEngine, const TDesC& aKeys);
        CPtiEngine* CreatePtiEngineL(TInt aLanguage, TPtiEngineInputMode aMode);
        virtual TInt PtiGetHwrRecognizer1L();
        virtual TInt PtiGetHwrRecognizer2L();
        virtual TInt PtiListHwrRecognizerL();
        virtual TInt PtiGetHwrAvailableLanguagesL();
        
        virtual TInt AknInputLanguageCapabilitiesL();
        virtual TInt AknInputLanguageItemL();
        
        virtual TInt TestListCoreL();    
        virtual TInt PtiCoreSetupL();        
        virtual TInt PtiCoreTestGeneralL();
        virtual TInt PtiCoreTestPredictiveLatinL();
        virtual TInt PtiCoreTestUserDictL();
        virtual TInt PtiCoreTestChineseL();
        virtual TInt PtiCoreTestJapaneseL();
        CPtiCore*  CreatePtiCoreL();
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // PTIENGINETESTSDKAPI_H

// End of File
