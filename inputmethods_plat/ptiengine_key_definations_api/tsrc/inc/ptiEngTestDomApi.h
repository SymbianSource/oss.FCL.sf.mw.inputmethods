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
* Description:                ?Description
*
*/

















#ifndef PTIENGTESTDOMAPI_H
#define PTIENGTESTDOMAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KptiEngTestDomApiLogPath, "\\logs\\testframework\\ptiEngTestDomApi\\" ); 
// Log file
_LIT( KptiEngTestDomApiLogFile, "ptiEngTestDomApi.txt" ); 
_LIT( KptiEngTestDomApiLogFileWithTitle, "ptiEngTestDomApi_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CptiEngTestDomApi;
class MPtiKeyMapData;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CptiEngTestDomApi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CptiEngTestDomApi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CptiEngTestDomApi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CptiEngTestDomApi();

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
        CptiEngTestDomApi( CTestModuleIf& aTestModuleIf );

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
        //PtiKeyMapData.h start
         MPtiKeyMapData * GetKeyMapDataL();
        
        virtual TInt  PtiTestKeyMapDataKeyBindingsL();
        virtual TInt  PtiTestKeyMapDataLanguagesL();
        virtual TInt  PtiTestKeyMapDataTestkeysL();
        virtual TInt  PtiTestKeyMapDataTestDeadkeysL();
        virtual TInt  PtiTestKeyMapDataTestNumerickeysL();
        virtual TInt  PtiTestKeyMapDataTestVowelsL();
        virtual TInt  PtiTestKeyMapDataTestOthersL();
        virtual TInt  PtiTestKeyMapDataFactoryL();

        void UpdateLogForVowels(TInt aVal,TInt bVal, TInt aLang,TInt &aErrStatus);    
        void UpdateLogForDeadKeys(TInt aVal,TInt bVal, TInt aLang,TInt &aErrStatus);   
        //PtiKeyMapData.h end
        
        //PtiKeyboardDatabase.h start
        virtual TInt  PtiTestKeyboardDatabaseFactoryL();
        //PtiKeyboardDatabase.h end
         
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

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

#endif      // PTIENGTESTDOMAPI_H

// End of File
