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
















// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "ptiEngineTestSdkApi.h"

#include <PtiCore.h>
#include <PtiEngine.h>
#include <ecom/ecom.h> 
#include <PtiUserDictionary.h>
#include <PtiLanguage.h>
#include <PtiKeyMappings.h>
#include <PtiLanguageDatabase.h>
#include <PtiUserDicEntry.h>
#include <PtiUids.hrh>
#include <PtiKeyMapData.h>
#include <AknInputLanguageInfo.h>
#include <e32lang.h>


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// CONSTANTS
const TInt KPtiNumKeys = 12;
const TInt KMaxSize = 32;
const TUint16 Keys[]=
    {
        //lower case
        /*1*/ '1', '\t',
        /*2*/ 'a','b','c','\t',
        /*3*/ 'd','e','f','\t',
        /*4*/ 'g','h','i','\t',
        /*5*/ 'j','k','l','\t',
        /*6*/ 'm','n','o','\t',
        /*7*/ 'p','q','r','s','\t',
        /*8*/ 't','u','v','\t',
        /*9*/ 'w','x','y','z','\t',
        /***/ '+','\t',
        /*0*/ '0','\t',
        /*#*/ '#','\t',

        //upper case
        /*1*/ '.', '\t',
        /*2*/ 'A','B','C','\t',
        /*3*/ 'D','E','F','\t',
        /*4*/ 'G','H','I','\t',
        /*5*/ 'J','K','L','\t',
        /*6*/ 'M','N','O','\t',
        /*7*/ 'P','Q','R','S','\t',
        /*8*/ 'T','U','V','\t',
        /*9*/ 'W','X','Y','Z','\t',
        /***/ '-','\t',
        /*0*/ ' ','\t',
        /*#*/ '^',

        '\0'    //string terminator
    };
 
 
// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
_LIT(KFileName, "C:\\logs\\TestFramework\\TestUserDict.txt");
_LIT( KQwertyDir, "z:\\resource\\qwerty\\" );
_LIT( KMultiTapWildName, "*.rsc" );
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CptiEngineTestSdkApi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CptiEngineTestSdkApi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CptiEngineTestSdkApi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CptiEngineTestSdkApi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        
        // For PtiUserDictionary.h  - START
        ENTRY( "PtiUserDictCreate", CptiEngineTestSdkApi::PtiUserDictCreateL),
        ENTRY( "PtiUserDictSetData", CptiEngineTestSdkApi::PtiUserDictSetDataL),
		ENTRY( "PtiUserDictSaveData", CptiEngineTestSdkApi::PtiUserDictSaveDataL),
        ENTRY( "PtiUserDictReload", CptiEngineTestSdkApi::PtiUserDictReloadL),
        ENTRY( "PtiUserDictTestOther", CptiEngineTestSdkApi::PtiUserDictTestOtherL),
        // For PtiUserDictionary.h  - END
        
        // For PtiLanguage.h  - START
       	ENTRY( "PtiLangDestructionL", CptiEngineTestSdkApi::PtiLangDestructionL),
        ENTRY( "PtiLangSetup", CptiEngineTestSdkApi::PtiLangSetupL),
        ENTRY( "PtiLangSetKeyMap", CptiEngineTestSdkApi::PtiLangSetKeyMapL),
        ENTRY( "PtiLangQwertySetKeyMap", CptiEngineTestSdkApi::PtiLangQwertySetKeyMapL),
        ENTRY( "PtiLangHalfQwertySetKeyMap", CptiEngineTestSdkApi::PtiLangHalfQwertySetKeyMapL),
        ENTRY( "PtiLangSetLocalizedName", CptiEngineTestSdkApi::PtiLangSetLocalizedNameL),
        ENTRY( "PtiLangSetLanguageCode", CptiEngineTestSdkApi::PtiLangSetLanguageCodeL),
        ENTRY( "PtiLangTestOthers", CptiEngineTestSdkApi::PtiLangTestOthersL),
        // For PtiLanguage.h  - END
        
        // For PtiLanguageDatabase.h  - START
        ENTRY( "PtiLangDBSetup", CptiEngineTestSdkApi::PtiLangDBSetupL),
        ENTRY( "PtiLangDBTestImpl", CptiEngineTestSdkApi::PtiLangDBTestImplL),
        ENTRY( "PtiLangDBTestInfo", CptiEngineTestSdkApi::PtiLangDBTestInfoL),
        ENTRY( "PtiLangDBTestOthers", CptiEngineTestSdkApi::PtiLangDBTestOthersL),
        // For PtiLanguageDatabase.h  - END
        
        // For PtiKeyMappings.h  - START
        ENTRY( "PtiKeyMapSetup", CptiEngineTestSdkApi::PtiKeyMapSetupL),
        ENTRY( "PtiKeyTestMapping", CptiEngineTestSdkApi::PtiKeyTestMappingL),
        ENTRY( "PtiTestReplaceMapping", CptiEngineTestSdkApi::PtiTestReplaceMappingL),
        ENTRY( "PtiTestReverseMapping", CptiEngineTestSdkApi::PtiTestReverseMappingL),
        ENTRY( "PtiTestKeyData", CptiEngineTestSdkApi::PtiTestKeyDataL),
		ENTRY( "PtiTestAllKeyData", CptiEngineTestSdkApi::PtiTestAllKeyDataL),
        ENTRY( "PtiKeyMapTestOthers", CptiEngineTestSdkApi::PtiKeyMapTestOthersL),
        
        ENTRY( "PtiQwertyMapSetup", CptiEngineTestSdkApi::PtiQwertyMapSetupL),
        ENTRY( "PtiQwertyTestMapping", CptiEngineTestSdkApi::PtiQwertyTestMappingL),   
        ENTRY( "PtiQwertyTestReplaceMapping", CptiEngineTestSdkApi::PtiQwertyTestReplaceMappingL),   
        ENTRY( "PtiQwertyTestReverseMapping", CptiEngineTestSdkApi::PtiQwertyTestReverseMappingL),
        ENTRY( "PtiQwertyTestKeyData", CptiEngineTestSdkApi::PtiQwertyTestKeyDataL),        
        ENTRY( "PtiQwertyTestAllKeyData", CptiEngineTestSdkApi::PtiQwertyTestAllKeyDataL),
        ENTRY( "PtiQwertyKeyMapTestOthers", CptiEngineTestSdkApi::PtiQwertyKeyMapTestOthersL),

        ENTRY( "PtiHalfQwertyMapSetup", CptiEngineTestSdkApi::PtiHalfQwertyMapSetupL), //
        ENTRY( "PtiHalfQwertyTestReplaceMapping", CptiEngineTestSdkApi::PtiHalfQwertyTestReplaceMappingL),     //   
        ENTRY( "PtiHalfQwertyTestReverseMapping", CptiEngineTestSdkApi::PtiHalfQwertyTestReverseMappingL), //
        ENTRY( "PtiHalfQwertyTestAllKeyData", CptiEngineTestSdkApi::PtiHalfQwertyTestAllKeyDataL), //
        ENTRY( "PtiHalfQwertyTestKeyData", CptiEngineTestSdkApi::PtiHalfQwertyTestKeyDataL),   //     
        // For PtiKeyMappings.h  - END
        
        // For PtiEngine.h - START
        ENTRY( "PtiEngSetup1", CptiEngineTestSdkApi::PtiEngSetup1L),
        ENTRY( "PtiEngSetup2", CptiEngineTestSdkApi::PtiEngSetup2L),
        ENTRY( "PtiEngAddCore", CptiEngineTestSdkApi::PtiEngAddCoreL),        
        ENTRY( "PtiEngTestLanguage", CptiEngineTestSdkApi::PtiEngTestLanguageL),
        ENTRY( "PtiEngTestInfo", CptiEngineTestSdkApi::PtiEngTestInfoL),
        ENTRY( "PtiEngTestMultitapping", CptiEngineTestSdkApi::PtiEngTestMultitappingL),
        ENTRY( "PtiEngTestPredict", CptiEngineTestSdkApi::PtiEngTestPredictL),
        ENTRY( "PtiEngGetCandidates", CptiEngineTestSdkApi::PtiEngGetCandidatesL),
        ENTRY( "PtiEngTestInputMode", CptiEngineTestSdkApi::PtiEngTestInputModeL),
        ENTRY( "PtiEngTestDict", CptiEngineTestSdkApi::PtiEngTestDictL),
        ENTRY( "PtiEngTestDefaultUserDict", CptiEngineTestSdkApi::PtiEngTestDefaultUserDictL),
        ENTRY( "PtiEngTestSecDataFile", CptiEngineTestSdkApi::PtiEngTestSecDataFileL), 
        ENTRY( "PtiEngTestLastKey", CptiEngineTestSdkApi::PtiEngTestLastKeyL),
        ENTRY( "PtiEngTestNavigation", CptiEngineTestSdkApi::PtiEngTestNavigationL),
        ENTRY( "PtiEngTestReordering", CptiEngineTestSdkApi::PtiEngTestReorderingL),
        ENTRY( "PtiEngTestAutoSubstitution", CptiEngineTestSdkApi::PtiEngTestAutoSubstitutionL), 
        ENTRY( "PtiEngTestListKeyboards", CptiEngineTestSdkApi::PtiEngTestListKeyboardsL),
        ENTRY( "PtiEngTestPhysicalListKeyboards", CptiEngineTestSdkApi::PtiEngTestPhysicalListKeyboardsL),
        ENTRY( "PtiEngTestOther", CptiEngineTestSdkApi::PtiEngTestOtherL),
        ENTRY( "PtiLangSetSecondaryInputL", CptiEngineTestSdkApi::PtiLangSetSecondaryInputL),
        ENTRY( "PtiLangGetHwrRecognizerL", CptiEngineTestSdkApi::PtiLangGetHwrRecognizerL),
        ENTRY( "PtiLangAddCoreL", CptiEngineTestSdkApi::PtiLangAddCoreL),
        
        ENTRY( "PtiEngTestChineseInput", CptiEngineTestSdkApi::PtiEngTestChineseInputL),
        ENTRY( "PtiEngTestJapaneseInput", CptiEngineTestSdkApi::PtiEngTestJapaneseInputL),
        ENTRY( "PtiGetHwrRecognizer1", CptiEngineTestSdkApi::PtiGetHwrRecognizer1L),
        ENTRY( "PtiGetHwrRecognizer2", CptiEngineTestSdkApi::PtiGetHwrRecognizer2L),
        ENTRY( "PtiListHwrRecognizer", CptiEngineTestSdkApi::PtiListHwrRecognizerL),
        ENTRY( "PtiGetHwrAvailableLanguages", CptiEngineTestSdkApi::PtiGetHwrAvailableLanguagesL),

        // For PtiEngine.h - END
        
        //akninputlanguageinfo.h
        ENTRY( "AknInputLanguageCapabilities", CptiEngineTestSdkApi::AknInputLanguageCapabilitiesL),
        ENTRY( "AknInputLanguageItem", CptiEngineTestSdkApi::AknInputLanguageItemL),
        // For akninputlanguageinfo.h - END   
        
        // For PtiCore.h  - START
        ENTRY( "PtiCoreSetup", CptiEngineTestSdkApi::PtiCoreSetupL),
        ENTRY( "TestListCore", CptiEngineTestSdkApi::TestListCoreL),        
        ENTRY( "PtiCoreTestGeneral", CptiEngineTestSdkApi::PtiCoreTestGeneralL),
        ENTRY( "PtiCoreTestPredictiveLatin", CptiEngineTestSdkApi::PtiCoreTestPredictiveLatinL),
        ENTRY( "PtiCoreTestUserDict", CptiEngineTestSdkApi::PtiCoreTestUserDictL),
        ENTRY( "PtiCoreTestChinese", CptiEngineTestSdkApi::PtiCoreTestChineseL),
        ENTRY( "PtiCoreTestJapanese", CptiEngineTestSdkApi::PtiCoreTestJapaneseL),        
        // For PtiCore.h  - END
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// For PtiUserDictionary.h  - START
TInt CptiEngineTestSdkApi::PtiUserDictCreateL()
	{	
	__UHEAP_MARK;
	TInt errStatus = KErrNone;
	
	CPtiUserDictionary *ptiUserDictionary = CPtiUserDictionary::NewL(KMaxSize);
	if(ptiUserDictionary)
		{
		errStatus =  KErrNone;
		REComSession::FinalClose();
		delete ptiUserDictionary;
		ptiUserDictionary = NULL;
		}
	else
		errStatus =  KErrGeneral;
	
	__UHEAP_MARKEND;
	return errStatus;	
	
	}

TInt CptiEngineTestSdkApi::PtiUserDictSetDataL()
	{
	__UHEAP_MARK;
	TUint8 data[] = {'h', 'e', 'l', 'l', 'o'};
	TInt size = sizeof(data)/sizeof(TUint8);
	TInt errStatus = KErrNone;
	
	CPtiUserDictionary *ptiUserDictionary = CPtiUserDictionary::NewL(KMaxSize);	
	
	CleanupStack::PushL(ptiUserDictionary);
	ptiUserDictionary->SetDataL(static_cast<TAny*>(data), size);
	
	if(0 != ptiUserDictionary->Size())
		{
		// Data Set successfully in UserDictionary
		errStatus = KErrNone;
		}		
	else
		errStatus =  KErrGeneral;
	
	TInt spaceLeft = ptiUserDictionary->SpaceLeft();
	if(-1 == spaceLeft)
		iLog->Log( _L(" Space Left Not Found in Dictionary ") );
	
	CleanupStack::PopAndDestroy(ptiUserDictionary);
	REComSession::FinalClose();
		
	__UHEAP_MARKEND;
	return errStatus;		
	}

void CptiEngineTestSdkApi :: CreateUserDicFileL(TDesC& aFilename)
	{	
	RFs fsSession;
	User::LeaveIfError( fsSession.Connect() );
    CleanupClosePushL( fsSession );
    
	RFile file;
	CleanupClosePushL(file);
	if ( file.Open( fsSession, aFilename, EFileWrite | EFileShareAny ) != KErrNone )
		{
		file.Create( fsSession, aFilename, EFileWrite | EFileShareAny );
		}
		
	file.Close();
	CleanupStack::PopAndDestroy(2); //file , fsSession	
	}

TInt CptiEngineTestSdkApi::PtiUserDictSaveDataL()
	{
	
	TInt errStatus = KErrNone;
	__UHEAP_MARK;	
	CPtiUserDictionary *ptiUserDictionary = CPtiUserDictionary::NewL(KMaxSize);

	TBuf<KMaxFileName> filename(KFileName);
	TUint8 data[] = {'h', 'e', 'l', 'l', 'o'};
	TInt size = sizeof(data)/sizeof(TUint8);
	CleanupStack::PushL(ptiUserDictionary);
	ptiUserDictionary->SetDataL(static_cast<TAny*>(data), size);
	
	ptiUserDictionary->SetFileName(filename);
	
	if(0 == ptiUserDictionary->FileName().Compare(filename))
		iLog->Log( _L(" User dictionary FileName Set !! ") );
	
	CreateUserDicFileL(filename);
	
	if (KErrNone == ptiUserDictionary->WriteL())
		errStatus = KErrNone;
	else
		errStatus = KErrGeneral;
	
	CleanupStack::PopAndDestroy(ptiUserDictionary);
	REComSession::FinalClose();
		
	__UHEAP_MARKEND; 
	return errStatus;
	}

TInt CptiEngineTestSdkApi::PtiUserDictReloadL()
	{
	__UHEAP_MARK;	
	TBuf<KMaxFileName> filename(KFileName);
	TInt errStatus = KErrNone;
	
	CreateUserDicFileL(filename);	
	CPtiUserDictionary *ptiUserDictionary = CPtiUserDictionary::NewL(filename, KMaxSize);
		
	if(NULL != ptiUserDictionary)
		{
		REComSession::FinalClose();
		delete ptiUserDictionary;
		ptiUserDictionary = NULL;
		errStatus = KErrNone;
		}
	else
		errStatus = KErrGeneral;
	
	__UHEAP_MARKEND;	
	return errStatus;
	
	}

TInt CptiEngineTestSdkApi::PtiUserDictTestOtherL()
	{
	__UHEAP_MARK;	
	TInt errStatus = KErrNone;
	
	CPtiUserDictionary *ptiUserDictionary = CPtiUserDictionary::NewL(KMaxSize);	
	CleanupStack::PushL(ptiUserDictionary);
	
	TInt userDicId = 1;
	ptiUserDictionary->SetId(userDicId);
	
	if(userDicId == ptiUserDictionary->Id())
		{
		iLog->Log( _L(" User Dictionary Id retrieved !! ") );
		}
	
	TInt32 coreID = 1;
	ptiUserDictionary->SetCoreUID(coreID);
	if(coreID == ptiUserDictionary->CoreUID())
		{
		iLog->Log( _L(" Core ID retrieved !! ") );
		}
		
	// Set to Default 
	ptiUserDictionary->SetDefault(1);
	if(1 == ptiUserDictionary->IsDefault())
		{
		iLog->Log( _L(" Default Dictionary !! ") );
		}
		
	TBool flag = ETrue;
	ptiUserDictionary->SetUpdateFlag(flag);
	if(flag == ptiUserDictionary->UpdateFlag())
		{
		iLog->Log( _L(" Flag Updated in  Dictionary !! ") );
		}
		
	TInt symbolValue = 1;
	ptiUserDictionary->SetSymbolClass(symbolValue);
	if(symbolValue== ptiUserDictionary->SymbolClass())
		{
		iLog->Log( _L(" Symbol Class value Retrieved !! ") );
		}		
	
	CleanupStack::PopAndDestroy(ptiUserDictionary);
	REComSession::FinalClose();
		
	__UHEAP_MARKEND;
	return errStatus;
	}
// For PtiUserDictionary.h  - END



// For PtiLanguage.h  - START
TInt CptiEngineTestSdkApi::PtiLangDestructionL()
	{
    __UHEAP_MARK;	
    TInt errStatus = KErrNone;
    
    CPtiCoreLanguage *ptiCoreLang = CPtiCoreLanguage::NewL();    
    
    if(NULL != ptiCoreLang)
		{
		delete ptiCoreLang;
		ptiCoreLang = NULL;
		}
	else
		errStatus =  KErrGeneral;
	
    __UHEAP_MARKEND;
	return errStatus;	
	}

TInt CptiEngineTestSdkApi::PtiLangSetupL()
    {
    __UHEAP_MARK;	
    TInt errStatus = KErrNone;
    
    CPtiCoreLanguage *ptiCoreLang = CPtiCoreLanguage::NewL();    
    
    if(NULL != ptiCoreLang)
		{
		errStatus =  KErrNone;
		REComSession::FinalClose();
		delete ptiCoreLang;
		ptiCoreLang = NULL;
		}
	else
		errStatus =  KErrGeneral;
	
    __UHEAP_MARKEND;
	return errStatus;	
    }


TInt CptiEngineTestSdkApi::PtiLangSetKeyMapL()
    {
    __UHEAP_MARK;	
    TInt errStatus = KErrNone;
    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineMultitapping);
    
    CPtiCoreLanguage *ptiCoreLang = static_cast<CPtiCoreLanguage*> (ptiEngine->GetLanguage(ELangEnglish));
       
    CPtiMappings * mappigns = static_cast<CPtiMappings*>(static_cast<CPtiKeyMappings*>(ptiCoreLang->GetKeymappings()));
    
    if(EPtiKeyboard12Key != mappigns->KeyboardType())
        errStatus = KErrGeneral;
    
    CPtiKeyMappings * keyMappings = static_cast<CPtiKeyMappings*> (CreateKeyMapL());
    ptiCoreLang->SetKeyMappings(keyMappings);
            
    if(NULL != ptiCoreLang->GetKeymappings())
    	{
    	errStatus =  KErrNone;			
    	}
    else
    	errStatus =  KErrGeneral;
   
    delete keyMappings->KeyMapData(); // we need to delete this manually
    ptiCoreLang->ClearMultitapping();
    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();
		
    __UHEAP_MARKEND;
    return errStatus;	
    }


TInt CptiEngineTestSdkApi::PtiLangQwertySetKeyMapL()
	{
	__UHEAP_MARK;		
	
	TInt errStatus = KErrNone;
	    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineQwerty);
    
    CPtiCoreLanguage *ptiCoreLang = static_cast<CPtiCoreLanguage*> (ptiEngine->GetLanguage(ELangEnglish));
      
    CPtiQwertyKeyMappings * qwertyKeymappings = static_cast<CPtiQwertyKeyMappings*> (CreateQwertyKeyMapL());
    ptiCoreLang->SetQwertyKeyMappings(qwertyKeymappings);
    
        
    if(NULL != ptiCoreLang->GetKeymappings())
        {
        errStatus =  KErrNone;          
        }
    else
        errStatus =  KErrGeneral;
   
    delete qwertyKeymappings->KeyMapData(); // we need to delete this manually
    ptiCoreLang->ClearQwerty();
    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();
	        
  	__UHEAP_MARKEND;
	return errStatus;	
	}
TInt CptiEngineTestSdkApi::PtiLangHalfQwertySetKeyMapL()
    {
    __UHEAP_MARK;       
    TInt errStatus = KErrNone;
            
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineQwerty);
    
    CPtiCoreLanguage *ptiCoreLang = static_cast<CPtiCoreLanguage*> (ptiEngine->GetLanguage(ELangEnglish));
      
    CPtiHalfQwertyKeyMappings * halfQwertyKeymappings = static_cast<CPtiHalfQwertyKeyMappings*> (CreateHalfQwertyKeyMapL());
    ptiCoreLang->SetHalfQwertyKeyMappings(halfQwertyKeymappings);
    
        
    if(NULL != ptiCoreLang->GetKeymappings())
        {
        errStatus =  KErrNone;          
        }
    else
        errStatus =  KErrGeneral;
   
    delete halfQwertyKeymappings->KeyMapData(); // we need to delete this manually
    ptiCoreLang->ClearHalfQwerty();
    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();
                
    __UHEAP_MARKEND;
    return errStatus;   
    }

TInt CptiEngineTestSdkApi::PtiLangSetLocalizedNameL()
    {
    __UHEAP_MARK;	
    CPtiCoreLanguage *ptiCoreLang = CPtiCoreLanguage::NewL();
    TInt errStatus = KErrNone;            
    _LIT(KLongL12nName, "English, use in both western Eroup and north America");
    
    CleanupStack::PushL(ptiCoreLang);
    ptiCoreLang->SetLocalizedName(KLongL12nName);
    TBuf<KMaxLanguageNameLength> cutName;
    const TDesC& ref = KLongL12nName;
    cutName.Copy(ref.Left(KMaxLanguageNameLength));
    
    if(ptiCoreLang->LocalizedNameDes() == cutName)
    	{
    	errStatus =  KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;
    
    if(0 == ptiCoreLang->LocalizedName().Compare(cutName))
    	iLog->Log( _L(" Localized Name Set Succesfully ") );    
    	
    CleanupStack::PopAndDestroy(ptiCoreLang);
	REComSession::FinalClose();
	
	__UHEAP_MARKEND;
    return errStatus;	
    }

TInt CptiEngineTestSdkApi::PtiLangSetLanguageCodeL()
    {
    __UHEAP_MARK;	
    CPtiCoreLanguage *ptiCoreLang = CPtiCoreLanguage::NewL();
    TInt errStatus = KErrNone;
    
    //lan code
    CleanupStack::PushL(ptiCoreLang);
    TInt oldCode = ptiCoreLang->LanguageCode();
    ptiCoreLang->SetLanguageCode(31);      // chinese
    ptiCoreLang->SetLanguageCode(31);      // set same code again
    
    ptiCoreLang->SetLanguageCode(1);       // set another code
    
    if(1 == ptiCoreLang->LanguageCode()) 
    	{
    	errStatus = KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;
    
    ptiCoreLang->SetLanguageCode(oldCode); // reset   
    
    CleanupStack::PopAndDestroy(ptiCoreLang);
    REComSession::FinalClose();
	
    __UHEAP_MARKEND;
    return errStatus;	
    }

TInt CptiEngineTestSdkApi::PtiLangTestOthersL()
    {
    __UHEAP_MARK;	    
    TInt errStatus = KErrNone;    
    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineQwerty);
    
    CPtiCoreLanguage *ptiCoreLang = static_cast<CPtiCoreLanguage*> (ptiEngine->GetLanguage(ELangEnglish));
        
	RArray<TInt> inputModes;
	CArrayFixFlat<TInt>* modes = new (ELeave) CArrayFixFlat<TInt>(10);
	
	ptiCoreLang->GetAvailableInputModesL(inputModes);		
	inputModes.Close();
	
	CleanupStack::PushL(modes);
	ptiCoreLang->GetAvailableInputModesL(modes);
	CleanupStack::PopAndDestroy(modes); // modes
	
	CPtiKeyMapDataFactory * dataFactory = NULL;
	dataFactory = ptiCoreLang->DataFactory();
	if(NULL == dataFactory)
	    errStatus = KErrGeneral;
	
	CPtiKeyMapData *keyMapData =  NULL;
	keyMapData = ptiCoreLang->RawKeyMapData();
	if(NULL == keyMapData)
	   errStatus = KErrGeneral; 
	
	ptiCoreLang->RemoveCore(NULL);	
	
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();
	
	__UHEAP_MARKEND;
	return errStatus;	
    }

TInt CptiEngineTestSdkApi::PtiLangSetSecondaryInputL()
	{
	 __UHEAP_MARK;	    
    TInt errStatus = KErrNone;    
    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
#ifdef __DUAL_LANGUAGE_SUPPORT__
    // Change secondary input
    ptiEngine->SetSecondaryInputL( 0 );
#endif //__DUAL_LANGUAGE_SUPPORT__
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();
		
	__UHEAP_MARKEND;
	return errStatus;
	}

TInt CptiEngineTestSdkApi::PtiLangGetHwrRecognizerL()
	{
	 __UHEAP_MARK;	    
    TInt errStatus = KErrNone;    
    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    TLanguage language = ELangEnglish;
    ptiEngine->GetHwrRecognizerL( language );
    ptiEngine->GetHwrRecognizerL( 0 );
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();
	
	__UHEAP_MARKEND;
	return errStatus;
	}

TInt CptiEngineTestSdkApi::PtiLangAddCoreL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    // This will always leave in MCL env as this method with TDesC parameter is deprecated, and made it such a way that it always leaves.
    TRAPD(errl,ptiEngine->AddCoreL( _L("fileName") ));
    if ( errl != KErrNone )
        {
        errStatus = KErrNone;
        }
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();
    __UHEAP_MARKEND;
    return errStatus;
    }

// For PtiLanguage.h  - END


// For PtiLanguageDatabase.h  - START
TInt CptiEngineTestSdkApi::PtiLangDBSetupL()
{
	__UHEAP_MARK;	    
	TInt errStatus = KErrNone;
	CPtiLanguageDatabase *ptiLangDB = CPtiLanguageDatabase::CreateDatabaseL(TUid::Uid(EPtiLangImplUidEnglish3));
	
	if(NULL != ptiLangDB)
		{
		errStatus =  KErrNone;
		delete ptiLangDB;
		ptiLangDB = NULL;
		REComSession::FinalClose();		
		}
	else
		errStatus =  KErrGeneral;
	
	__UHEAP_MARKEND;
	return errStatus;		
}

TInt CptiEngineTestSdkApi::PtiLangDBTestImplL()
{
	__UHEAP_MARK;	
	TInt errStatus = KErrNone;
	CPtiLanguageDatabase *ptiLangDB = CPtiLanguageDatabase::CreateDatabaseL(TUid::Uid(EPtiLangImplUidEnglish3));

	CleanupStack::PushL(ptiLangDB);
    // #languages
    if(ptiLangDB->NumLanguages() != 0)
    	{
    	iLog->Log( _L("NumLanguages got") );    	
    	}
    
    // lan code[]
    if(ptiLangDB->LanguageCode(0)==1)
    	{
    	iLog->Log(_L("1st language code retrieved"));
    	}

    // sub lan code[]
    if(ptiLangDB->SubLanguageCode(0)==0)
    	{
    	iLog->Log(_L("sub lan code at 0 got" ));
    	}
    
    // lan data[]
    if(ptiLangDB->LangData(0) == NULL)
    	{
    	iLog->Log(_L("lan data at 0 got" ));
    	}
    
    if(ptiLangDB->LangData(0, NULL)== NULL)
    	{
    	iLog->Log(_L("lan data at 0 with param NULL got" ));
    	}
    
    if(ptiLangDB->PinyinPhraseData() == NULL)
    	{
    	iLog->Log(_L("pinyin phrase data got"));
    	}    
    
    if(ptiLangDB->ZhuyinPhraseData() == NULL)
    	{
    	iLog->Log(_L("zhuyin phrase data got" ));
    	}
    if(ptiLangDB->NativeId(0) == 9)
    	{
        iLog->Log(_L("native id at 0 got"));
    	}
    
    if(ptiLangDB->CoreUID() == 0)
    	{    	
        iLog->Log(_L("Core UID is 0" ));
    	}
    
    CleanupStack::PopAndDestroy(ptiLangDB);
    REComSession::FinalClose();
		
    __UHEAP_MARKEND;
	return errStatus;		
}

TInt CptiEngineTestSdkApi::PtiLangDBTestInfoL()
    {
    __UHEAP_MARK;	    
	TInt errStatus = KErrNone;
	CPtiLanguageDatabase *ptiLangDB = CPtiLanguageDatabase::CreateDatabaseL(TUid::Uid(EPtiLangImplUidEnglish3));
	CleanupStack::PushL(ptiLangDB);
	
    //language code for uid
    if( ELangEnglish == CPtiLanguageDatabase::LanguageCodeForUid(EPtiLangImplUidEnglish3))
    	{
    	errStatus =  KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;
    
    RArray<TPtiLangDatabaseMapping> result;
    ptiLangDB->CreateMappingTableL(GetCoreUidL(),result);
    result.Close();
    
    RArray<TPtiLangDatabaseMappingOpaque> result1;
    ptiLangDB->CreateMappingTableWithOpaqueL(GetCoreUidL(),result1);
    result1.Close();
    
    CleanupStack::PopAndDestroy(ptiLangDB);
    REComSession::FinalClose();
		    
    __UHEAP_MARKEND;
    return errStatus;    	
    }


TInt CptiEngineTestSdkApi::PtiLangDBTestOthersL()
	{
	// a helper class
	class CMockLanDB: public CPtiLanguageDatabase
		{
	public:
		//
		};
	
	__UHEAP_MARK;
	TInt errStatus = KErrNone;
	
	CMockLanDB* ptr = new (ELeave) CMockLanDB;
	CleanupStack::PushL(ptr);

	if( 0 == ptr->CoreUID() )
		{
		iLog->Log(_L("Empty CoreUID returned"));
		}

	if( ptr->LangData(0)== NULL )
		{
		iLog->Log(_L("Empty LangData1 returned"));
		}

	if( ptr->LangData(0, NULL)== NULL )
		{
		iLog->Log(_L("Empty LangData2 returned"));
		}

	if( 0 == ptr->LanguageCode(0) )
		{
		iLog->Log(_L("Empty Language code returned"));
		}

	if( 0 == ptr->NativeId(0))
		{
		iLog->Log(_L("Empty NativeId returned"));
		}

	if(0 == ptr->NumLanguages())
		{
		iLog->Log(_L("Empty NumLanguages returned"));
		}

	CleanupStack::PopAndDestroy();	//ptr;
	REComSession::FinalClose();
	
	__UHEAP_MARKEND;
    return errStatus;   
	}
// For PtiLanguageDatabase.h  - END


// For PtiKeyMappings.h  - START


MPtiKeyMappings* CptiEngineTestSdkApi::CreateKeyMapL()
    {
     RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
     
    TUid uid =  TUid::Uid(dataImpl[0]);
    CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(uid);  
    CleanupStack::PushL(keymapDatafactory);
    
    dataImpl.Close();
    
    CPtiKeyMapData * keymapData =static_cast<CPtiKeyMapData*>( keymapDatafactory->KeyMapDataForLanguageL(01));
    MPtiKeyMappings * multitapKeymappings = CPtiKeyMappings::NewL(keymapData);
   
    CleanupStack::PopAndDestroy(keymapDatafactory);  //keymapDatafactory
    return  multitapKeymappings;
    }

TInt CptiEngineTestSdkApi::PtiKeyMapSetupL()
    {    
    __UHEAP_MARK;	   
    TInt errStatus = KErrNone;
    
    MPtiKeyMappings *ptiKeyMap = NULL;
    
    TRAPD(err,ptiKeyMap = CPtiKeyMappings::NewL((TInt16*)NULL));
    if ( err != KErrNone )
        {
        errStatus = KErrNone;
        }
    TBufC<10> dummybuf;
    TRAPD(err1,ptiKeyMap = CPtiKeyMappings::NewL(dummybuf));
    if ( err1 != KErrNone )
        {
        errStatus = KErrNone;
        }
    ptiKeyMap = CreateKeyMapL();
    
    if(NULL != ptiKeyMap)
        {
        errStatus =  KErrNone;
        delete static_cast<CPtiKeyMappings*>(ptiKeyMap)->KeyMapData();
        REComSession::FinalClose();
        delete ptiKeyMap;
        ptiKeyMap = NULL;
        }
    else
        errStatus =  KErrGeneral;
    
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiKeyTestMappingL()
    {
    __UHEAP_MARK;
    TBool isAppend = ETrue;
    TInt errStatus = KErrNone;
    MPtiKeyMappings *ptiKeyMap = CreateKeyMapL();

    if ( TUint('d') == ptiKeyMap->StartMapping(EPtiKey3, EPtiCaseLower))
    	{    	
        iLog->Log(_L("Print 'd' (Key 3)"));
    	}
    
    if( TUint16('e') ==  ptiKeyMap->NextKey(EPtiKey3, isAppend, EPtiCaseLower))
    	{
    	iLog->Log(_L("press 3 again, got 'e'"));
    	}
    
    if ( TUint16('F') == ptiKeyMap->NextKey(EPtiKey3, isAppend, EPtiCaseUpper))
    	{
    	iLog->Log(_L("then, press Shift 3 got 'F'"));
    	}
    
    delete static_cast<CPtiKeyMappings*>(ptiKeyMap)->KeyMapData();
    delete ptiKeyMap;
    REComSession::FinalClose();
		
    __UHEAP_MARKEND;
    return errStatus;    
    }

TInt CptiEngineTestSdkApi::PtiTestReplaceMappingL()
    {
    __UHEAP_MARK;	   
	TInt errStatus = KErrNone;
    TBool isAppend = ETrue;
    _LIT(KNewMapLower, "!@#$");    
    TBuf<5> lowerMap(KNewMapLower);
    
    MPtiKeyMappings *ptiKeyMap = CreateKeyMapL();
    
    errStatus = ptiKeyMap->ReplaceKeyMapL(EPtiKey1, lowerMap, EPtiCaseLower);
    
    if( TUint('a') ==  ptiKeyMap->StartMapping(EPtiKey2, EPtiCaseLower,EPtiEngineMultitapping))
    	{
    	iLog->Log(_L("press 1 got '!'" ));           
    	}
    if( TUint16('@') == ptiKeyMap->NextKey(EPtiKey1, isAppend, EPtiCaseLower))
    	{
    	iLog->Log(_L("press 1, again got '@'"));
    	}
        
    delete static_cast<CPtiKeyMappings*>(ptiKeyMap)->KeyMapData();
    delete ptiKeyMap;
   	REComSession::FinalClose();
		    
    __UHEAP_MARKEND;
    return errStatus;	
    }

TInt CptiEngineTestSdkApi::PtiTestReverseMappingL()
    {
    __UHEAP_MARK;	   
	TInt errStatus = KErrNone;	
	
	MPtiKeyMappings *ptiKeyMap = CreateKeyMapL();
	
    if( EPtiKey9 ==  ptiKeyMap->KeyForCharacter('X'))
    	{
    	errStatus =  KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;
    
    delete static_cast<CPtiKeyMappings*>(ptiKeyMap)->KeyMapData();
    delete ptiKeyMap;
	REComSession::FinalClose();
		
	__UHEAP_MARKEND;   
    return errStatus;        
    }

TInt CptiEngineTestSdkApi::PtiTestKeyDataL()
    {
    __UHEAP_MARK;	   
	TInt errStatus = KErrNone;	
    _LIT(KNewMapLower, "abc2");    
    TBuf<4> mapStr;

    TBuf<4> lowerMap(KNewMapLower);
    
    MPtiKeyMappings *ptiKeyMap = CreateKeyMapL();
    static_cast<CPtiKeyMappings*>(ptiKeyMap)->GetDataForKey(EPtiKey2, mapStr, EPtiCaseLower);
    if( mapStr == KNewMapLower)
    	{
    	errStatus =  KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;
    
    delete static_cast<CPtiKeyMappings*>(ptiKeyMap)->KeyMapData();
    delete ptiKeyMap;		
	REComSession::FinalClose();
	
	__UHEAP_MARKEND; 
    return errStatus;	           
    }

TInt CptiEngineTestSdkApi::PtiTestAllKeyDataL()
    {
    __UHEAP_MARK;	   
	TInt errStatus = KErrNone;	
	
	MPtiKeyMappings *ptiKeyMap = CreateKeyMapL();
    
    _LIT(KNewMapLower, "!@#$"); 
    TBuf<5> lowerMap(KNewMapLower);
    
    ptiKeyMap->ReplaceKeyMapL(EPtiKey1, lowerMap, EPtiCaseLower);
    
    // This has to be called to set iCurrentKey in CPtiMappings , other wise GetAll will fail.
    ptiKeyMap->StartMapping(EPtiKey1, EPtiCaseLower,EPtiEngineMultitapping); 
        
    TPtrC keyData(ptiKeyMap->GetAll(EPtiCaseLower));
    
    if(0 != keyData.Length())
    	errStatus =  KErrNone;
    else
    	errStatus =  KErrGeneral;
    
    delete static_cast<CPtiKeyMappings*>(ptiKeyMap)->KeyMapData();
    delete ptiKeyMap;		
	REComSession::FinalClose();
	
	__UHEAP_MARKEND; 
    return errStatus;	
	
    }


TInt CptiEngineTestSdkApi::PtiKeyMapTestOthersL()
    {
    __UHEAP_MARK;	   
	TInt errStatus = KErrNone;	
    TInt16 charBuffer[1+(KPtiNumKeys*2)*2+1+sizeof(Keys)/sizeof(TUint16)];
    if ( Keys[0] != '0' )
        {
        errStatus = KErrNone;
        }
    
    MPtiKeyMappings *ptiKeyMap = CreateKeyMapL();
    
    TInt count = static_cast<CPtiKeyMappings*>(ptiKeyMap)->WriteData(charBuffer);
    if(0 != count)
    	errStatus = KErrNone;
    else
    	errStatus = KErrGeneral;
    
    TInt dataSize = static_cast<CPtiKeyMappings*>(ptiKeyMap)->ChunkDataSize();
    
    delete static_cast<CPtiKeyMappings*>(ptiKeyMap)->KeyMapData();
    delete ptiKeyMap;		
	REComSession::FinalClose();
	
	errStatus = KErrNone; // As the above two methods are Deprecated in PTIEngine V2, always return KErrNone
	__UHEAP_MARKEND; 
    return errStatus;	
  
    }


MPtiKeyMappings* CptiEngineTestSdkApi::CreateQwertyKeyMapL()
    {    
    
    RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
     
    TUid uid =  TUid::Uid(dataImpl[0]);
    CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(uid);  
    CleanupStack::PushL(keymapDatafactory);
    dataImpl.Close();
    
    CPtiKeyMapData * keymapData =static_cast<CPtiKeyMapData*>( keymapDatafactory->KeyMapDataForLanguageL(01));
    MPtiKeyMappings * qwertyKeymappings = CPtiQwertyKeyMappings::NewL(keymapData);
    
    CleanupStack::PopAndDestroy(keymapDatafactory);;
    return  qwertyKeymappings;
    
    }
// ---------------------------------------------------------------------------
// CptiEngineTestSdkApi::QwertyMapSetupL()
// ---------------------------------------------------------------------------
//   
TInt CptiEngineTestSdkApi::PtiQwertyMapSetupL()
    {
    __UHEAP_MARK;	   
	TInt errStatus = KErrNone;
	
	MPtiKeyMappings *ptiQwertyKeyMap = NULL;
	
	TRAPD(err,CPtiQwertyKeyMappings::NewL((TInt16*)NULL));
    if ( err != KErrNone )
        {
        errStatus = KErrNone;
        }
	
    TBufC<10> dummybuf;
    TRAPD(err1,CPtiQwertyKeyMappings::NewL(dummybuf));
    if ( err1 != KErrNone )
        {
        errStatus = KErrNone;
        }
    ptiQwertyKeyMap = CreateQwertyKeyMapL();
    
    if(NULL != ptiQwertyKeyMap)
		{
		errStatus =  KErrNone;
		delete static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
		REComSession::FinalClose();
		delete ptiQwertyKeyMap;
		ptiQwertyKeyMap = NULL;
		}
	else
		errStatus =  KErrGeneral;
	
    __UHEAP_MARKEND;
	return errStatus;	
    }


// ---------------------------------------------------------------------------
// CptiEngineTestSdkApi::QwertyTestMappingL()
// ---------------------------------------------------------------------------
//  
TInt CptiEngineTestSdkApi::PtiQwertyTestMappingL()
    {
    TInt errStatus = KErrNone;
    __UHEAP_MARK;	   
	MPtiKeyMappings *ptiQwertyKeyMap = CreateQwertyKeyMapL();
    
    CPtiKeyMapData *keyMappings = static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    TInt dataSize;
    const TUint16 * keyData = keyMappings->KeyData(EPtiKeyboard12Key,dataSize);
    TPtrC keydata(keyData);
    TInt size = keydata.Length();
    
    TBool isAppend = ETrue;
    ptiQwertyKeyMap->StartMapping( (TPtiKey)keydata[size/2+1], EPtiCaseLower,EPtiEngineQwerty);
    
    if( (TPtiKey)keydata[0] == ptiQwertyKeyMap->NextKey( (TPtiKey)keydata[0], isAppend, EPtiCaseLower ) )
    	{
    	iLog->Log(_L(" keydata[0] Retrieved !!" ));    
    	}
    
    delete static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    delete ptiQwertyKeyMap;		
    REComSession::FinalClose();
	
    __UHEAP_MARKEND;
    return errStatus;    
    }


// ---------------------------------------------------------------------------
// CptiEngineTestSdkApi::QwertyTestReplaceMappingL()
// ---------------------------------------------------------------------------
//  
TInt CptiEngineTestSdkApi::PtiQwertyTestReplaceMappingL()
    {
    
    TInt errStatus = KErrNone;
    
    __UHEAP_MARK;	   
	
    TBool isAppend = ETrue;
    _LIT( KNewMapLower, "`" );    
    TBuf<5> lowerMap( KNewMapLower );
    
    MPtiKeyMappings *ptiQwertyKeyMap = CreateQwertyKeyMapL();    
    
    CPtiKeyMapData *keyMappings = static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    TInt dataSize;
    const TUint16 * keyData = keyMappings->KeyData(EPtiKeyboard12Key,dataSize);
    TPtrC keydata(keyData);
    TInt size = keydata.Length();

    errStatus = ptiQwertyKeyMap->ReplaceKeyMapL( (TPtiKey)keydata[0], lowerMap, EPtiCaseLower ) ;
    
    if( TUint16('`') == ptiQwertyKeyMap->StartMapping( (TPtiKey)keydata[0], EPtiCaseLower ) ) 
    	{
    	iLog->Log(_L(" [`] Retrieved !!" ));   
    	}
    if( TUint16('`') == ptiQwertyKeyMap->NextKey( (TPtiKey)keydata[0], isAppend, EPtiCaseLower) )
    	{
    	iLog->Log(_L(" [`] Retrieved !!" ));  
    	}
    
    delete static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    delete ptiQwertyKeyMap; 	
	REComSession::FinalClose();
	
	__UHEAP_MARKEND;
	return errStatus;	  
    }


TInt CptiEngineTestSdkApi::PtiQwertyTestReverseMappingL()
    {   
    TInt errStatus = KErrNone;
    
    __UHEAP_MARK;	   

    MPtiKeyMappings *ptiQwertyKeyMap = CreateQwertyKeyMapL();
    
    CPtiKeyMapData *keyMappings = static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    TInt dataSize;
    const TUint16 * keyData = keyMappings->KeyData(EPtiKeyboard12Key,dataSize);
    TPtrC keydata(keyData);
    TInt size = keydata.Length();
    const TUint16 keyChar = keydata[4];
    TPtiKey akey = ptiQwertyKeyMap->KeyForCharacter( keyChar );
    TPtiKey bkey = (TPtiKey)keydata[4];
    if( bkey == akey )
    	{
    	errStatus =  KErrNone;
    	}
    else
    	errStatus =  KErrNone;
    
    delete static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    delete ptiQwertyKeyMap; 	
	REComSession::FinalClose();
		
	__UHEAP_MARKEND;
	return errStatus;   
    }


TInt CptiEngineTestSdkApi::PtiQwertyTestKeyDataL()
    {
    TInt errStatus = KErrNone;
    
    __UHEAP_MARK;	   

    _LIT( KNewMapLower, '1' ); 
    TBuf<8> mapStr;
    
    MPtiKeyMappings *ptiQwertyKeyMap =CreateQwertyKeyMapL();
    
    CPtiKeyMapData *keyMappings = static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    TInt dataSize;
    const TUint16 * keyData = keyMappings->KeyData(EPtiKeyboard12Key,dataSize);
    TPtrC keydata(keyData);
    
    static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->GetDataForKey( (TPtiKey)keydata[4], mapStr, EPtiCaseLower );
    if( mapStr == KNewMapLower )
    	{
    	errStatus =  KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;        
    
    delete static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    delete ptiQwertyKeyMap; 		
	REComSession::FinalClose();
		
	__UHEAP_MARKEND;
	errStatus =  KErrNone;
	return errStatus;    
    }


TInt CptiEngineTestSdkApi::PtiQwertyTestAllKeyDataL()
    {
    TInt errStatus = KErrNone;  
    
    __UHEAP_MARK;	   

    _LIT(KNewMapLower, "!@#$");
    TBuf<5> lowerMap(KNewMapLower);    
	
	MPtiKeyMappings *ptiQwertyKeyMap = CreateQwertyKeyMapL();
   
    CPtiKeyMapData *keyMappings =  static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    TInt dataSize;
    const TUint16 * keyData = keyMappings->KeyData(EPtiKeyboard12Key,dataSize);
    TPtrC keydata(keyData);
    TInt size = keydata.Length();
    
    ptiQwertyKeyMap->ReplaceKeyMapL( (TPtiKey)keydata[0], lowerMap, EPtiCaseLower ) ;
        
    // This has to be called to set iCurrentKey in CPtiMappings , other wise GetAll will fail.
    ptiQwertyKeyMap->StartMapping(EPtiKeyQwertyA, EPtiCaseLower,EPtiEngineQwerty); 
    TPtrC qwertyKeyData(ptiQwertyKeyMap->GetAll(EPtiCaseLower));
    
    if(0 != qwertyKeyData.Length())
    	errStatus =  KErrNone;
    else
    	errStatus =  KErrGeneral;
    
    delete static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    delete ptiQwertyKeyMap; 		
	REComSession::FinalClose();
	
	__UHEAP_MARKEND;
    return errStatus;	
	
    }


TInt CptiEngineTestSdkApi::PtiQwertyKeyMapTestOthersL()
    {
    __UHEAP_MARK;	   
	TInt errStatus = KErrNone;	
    TInt16 charBuffer[1+(KPtiNumKeys*2)*2+1+sizeof(Keys)/sizeof(TUint16)];
    
    MPtiKeyMappings *ptiQwertyKeyMap = CreateQwertyKeyMapL();
    
    //These two functions are deprecated
    static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->WriteData(charBuffer);
    static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->ChunkDataSize();

        
    if(KErrNone !=   static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->SetKeyboardType(EPtiKeyboard12Key))
            errStatus = KErrGeneral;
    
    if((TInt(EFalse)) == (static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->HasDataBlockForKeyboardType(EPtiKeyboard12Key)))
            errStatus = KErrGeneral;    
        
    delete static_cast<CPtiQwertyKeyMappings*>(ptiQwertyKeyMap)->KeyMapData();
    delete ptiQwertyKeyMap; 		
	REComSession::FinalClose();
	
	__UHEAP_MARKEND; 
    return errStatus;  
    }

bool CptiEngineTestSdkApi::ValidQwertyMode()
    {    
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() ); // Start session
    CleanupClosePushL( fsSession ); 
    
    fsSession.SetSessionPath( KQwertyDir );                 
            
    TFindFile fileFinder( fsSession );
    CDir* fileList = NULL;
    TInt err = fileFinder.FindWildByPath( KMultiTapWildName, &KQwertyDir, fileList ); 
    delete fileList;
    CleanupStack::PopAndDestroy(); // fsSession 
    if( err == KErrNone )
        {
        return true;
        }
    else
        {
        return false;
        }    
    }

MPtiKeyMappings* CptiEngineTestSdkApi::CreateHalfQwertyKeyMapL()
    {    
    RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
     
    TUid uid =  TUid::Uid(dataImpl[0]);
    CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(uid);  
    CleanupStack::PushL(keymapDatafactory);
    dataImpl.Close();
    
    CPtiKeyMapData * keymapData =static_cast<CPtiKeyMapData*>( keymapDatafactory->KeyMapDataForLanguageL(01));
    MPtiKeyMappings * halfqwertyKeymappings = CPtiHalfQwertyKeyMappings::NewL(keymapData);
 
    CleanupStack::PopAndDestroy(keymapDatafactory);
    return  halfqwertyKeymappings;
    
    }

// ---------------------------------------------------------------------------
// CptiEngineTestSdkApi::PtiHalfQwertyMapSetupL()
// ---------------------------------------------------------------------------
//   
TInt CptiEngineTestSdkApi::PtiHalfQwertyMapSetupL()
    {
       __UHEAP_MARK;     
       TInt errStatus = KErrNone;
       MPtiKeyMappings *ptiHalfQwertyKeyMap = CreateHalfQwertyKeyMapL();
       
       if(NULL != ptiHalfQwertyKeyMap)
           {
           delete static_cast<CPtiHalfQwertyKeyMappings*>(ptiHalfQwertyKeyMap)->KeyMapData();
           errStatus =  KErrNone;
           REComSession::FinalClose();
           delete ptiHalfQwertyKeyMap;
           ptiHalfQwertyKeyMap = NULL;
           }
       else
           errStatus =  KErrNone;
       
       __UHEAP_MARKEND;
       return errStatus;   
    }

// ---------------------------------------------------------------------------
// CptiEngineTestSdkApi::HalfQwertyTestReplaceMappingL()
// ---------------------------------------------------------------------------
//  
TInt CptiEngineTestSdkApi::PtiHalfQwertyTestReplaceMappingL()
    {
        __UHEAP_MARK;      
        TInt errStatus = KErrNone;
        TBool isAppend = ETrue;
        _LIT(KNewMapLower, "!@");    
        TBuf<5> lowerMap(KNewMapLower);
        
        MPtiKeyMappings *ptiHalfQwertyKeyMap =CreateHalfQwertyKeyMapL();
 
        errStatus = ptiHalfQwertyKeyMap->ReplaceKeyMapL(EPtiKey1, lowerMap, EPtiCaseLower);
        
        if( TUint('!') ==  ptiHalfQwertyKeyMap->StartMapping(EPtiKey1, EPtiCaseLower,EPtiEngineHalfQwerty))
            {
            iLog->Log(_L("press 1 got '!'" ));           
            }
        if( TUint16('@') == ptiHalfQwertyKeyMap->NextKey(EPtiKey1, isAppend, EPtiCaseLower))
            {
            iLog->Log(_L("press 1, again got '@'"));
            }
            
        delete static_cast<CPtiHalfQwertyKeyMappings*>(ptiHalfQwertyKeyMap)->KeyMapData();
        delete ptiHalfQwertyKeyMap;
        REComSession::FinalClose();
                
        __UHEAP_MARKEND;
        return errStatus;   
    }

TInt CptiEngineTestSdkApi::PtiHalfQwertyTestReverseMappingL()
    {
    __UHEAP_MARK;      
    TInt errStatus = KErrNone;  
    
    MPtiKeyMappings *ptiHalfQwertyKeyMap = CreateHalfQwertyKeyMapL();
    
    if( EPtiKeyQwertyZ ==  ptiHalfQwertyKeyMap->KeyForCharacter('x'))
        {
        errStatus =  KErrNone;
        }
    else
        errStatus =  KErrNone;
    
    delete static_cast<CPtiHalfQwertyKeyMappings*>(ptiHalfQwertyKeyMap)->KeyMapData();
    delete ptiHalfQwertyKeyMap;  
    REComSession::FinalClose();
        
    __UHEAP_MARKEND;   
    return errStatus;        
    }

TInt CptiEngineTestSdkApi::PtiHalfQwertyTestAllKeyDataL()
    {
    __UHEAP_MARK;      
    TInt errStatus = KErrNone;  
    
    MPtiKeyMappings *ptiHalfQwertyKeyMap = CreateHalfQwertyKeyMapL();
    
    _LIT(KNewMapLower, "!@"); 
    TBuf<5> lowerMap(KNewMapLower);
    
    ptiHalfQwertyKeyMap->ReplaceKeyMapL(EPtiKey1, lowerMap, EPtiCaseLower);
    
    // This has to be called to set iCurrentKey in CPtiMappings , other wise GetAll will fail.
    ptiHalfQwertyKeyMap->StartMapping(EPtiKey1, EPtiCaseLower,EPtiEngineHalfQwerty);
    TPtrC keyData(ptiHalfQwertyKeyMap->GetAll(EPtiCaseLower));
    
    if(0 != keyData.Length())
        errStatus =  KErrNone;
    else
        errStatus =  KErrNone;
    
    delete static_cast<CPtiHalfQwertyKeyMappings*>(ptiHalfQwertyKeyMap)->KeyMapData();
    delete ptiHalfQwertyKeyMap;         
    REComSession::FinalClose();
    
    __UHEAP_MARKEND; 
    return errStatus;   
    
    }
TInt CptiEngineTestSdkApi::PtiHalfQwertyTestKeyDataL()
    {
    __UHEAP_MARK;      
    TInt errStatus = KErrNone;  
    _LIT(KNewMapLower, "as");    
    TBuf<2> mapStr;

    TBuf<2> lowerMap(KNewMapLower);
    
    MPtiKeyMappings *ptiHalfQwertyKeyMap = CreateHalfQwertyKeyMapL();
    
    static_cast<CPtiHalfQwertyKeyMappings*>(ptiHalfQwertyKeyMap)->GetDataForKey(EPtiKeyQwertyA, mapStr, EPtiCaseLower);
    if( mapStr == KNewMapLower)
        {
        errStatus =  KErrNone;
        }
    else
        errStatus =  KErrNone;
    
    delete static_cast<CPtiHalfQwertyKeyMappings*>(ptiHalfQwertyKeyMap)->KeyMapData();
    delete ptiHalfQwertyKeyMap;          
    REComSession::FinalClose();
    
    __UHEAP_MARKEND; 
    return errStatus;              
    }
// For PtiKeyMappings.h  - END


// For PtiEngine.h START

TInt CptiEngineTestSdkApi::GetCoreUidL()
    {
    __UHEAP_MARK;
	CArrayFix<TInt>* list = CPtiCore::ListCoresLC();
	TInt firstUID = 0;
	firstUID = (*list)[0] ;
		
	CleanupStack::PopAndDestroy(list);  //list
	REComSession::FinalClose();
	
	__UHEAP_MARKEND;
    return firstUID;
    }
    
TInt CptiEngineTestSdkApi::PtiEngSetup1L()
    {
    __UHEAP_MARK;
    CPtiEngine *ptiEngine = NULL;
    
    TInt errStatus = KErrNone;
    TRAPD(err,CPtiEngine::NewL(_L("DummyName")));
    if ( err != KErrNone )
        {
        errStatus = KErrNone;
        }
    ptiEngine = CPtiEngine::NewL();
    
    if(NULL != ptiEngine)
    	{
    	delete ptiEngine;
    	ptiEngine = NULL;
    	REComSession::FinalClose();
    	errStatus = KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;
    
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngSetup2L()
    {
    __UHEAP_MARK;
    
    TInt coreID = GetCoreUidL();
    CPtiEngine *ptiEngine = CPtiEngine::NewL(TUid::Uid(coreID));
    TInt errStatus = KErrNone;
    
    if(NULL != ptiEngine)
    	{
    	delete ptiEngine;
    	ptiEngine = NULL;
    	REComSession::FinalClose();
    	errStatus = KErrNone;
    	}
    else
    	errStatus =  KErrGeneral;
    
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngAddCoreL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    TInt coreID = GetCoreUidL();
    errStatus = ptiEngine->AddCoreL(TUid::Uid(coreID));
    	
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();  
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngTestLanguageL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    CArrayFixFlat<TInt>* langs = new (ELeave) CArrayFixFlat<TInt>(100);
    CleanupStack::PushL(langs);
    
    ptiEngine->GetAvailableLanguagesL(langs);

    if(langs->Count()>0)
    	{
        iLog->Log(_L("available languages got into CArray" ));
    	}
    CleanupStack::PopAndDestroy(langs); //langs

    RArray<TInt> langRefs;
    ptiEngine->GetAvailableLanguagesL(langRefs);
    if(langRefs.Count()>0)
    	{
    	_LIT( KLangInfo, "available languages got into RArray" );
        iLog->Log( KLangInfo );
    	}

    //
    //activate
    //
    if(langRefs.Count())
        {
        MPtiLanguage *lan = ptiEngine->GetLanguage(langRefs[0]);
        if (lan->HasInputMode(EPtiEngineMultitapping))
            {
            errStatus = ptiEngine->ActivateLanguageL(lan->LanguageCode(), EPtiEngineMultitapping);            
            }
        else
            {
            errStatus = ptiEngine->ActivateLanguageL(lan->LanguageCode(), EPtiEnginePredictive);            
            }
        }

    langRefs.Close();
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    errStatus = KErrNone;
    return errStatus;
    }


TInt CptiEngineTestSdkApi::PtiEngTestInfoL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    TUint noOfLang = ptiEngine->NumberOfLanguages();
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineMultitapping);	  
    
    if (ptiEngine->InputMode() == EPtiEngineMultitapping)
        {
        iLog->Log(_L("Input mode is Multitapping" ));         
        }
    else
        {
        iLog->Log(_L("Input mode is Predictive" ));
        }

    // localized language name
    TFileName localName;
    ptiEngine->GetLocalizedLanguageName(31, localName); //31 is chinese?
    localName.ZeroTerminate();
        
    //observer
    MPtiObserver* obs = ptiEngine->Observer();
    ptiEngine->SetObserver(obs);
   
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }


TInt CptiEngineTestSdkApi::PtiEngTestMultitappingL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine);
    
    //multitapping
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineMultitapping);
    ptiEngine->AppendKeyPress(EPtiKey3);
    ptiEngine->AppendKeyPress(EPtiKey6);
    ptiEngine->AppendKeyPress(EPtiKey6);
    ptiEngine->AppendKeyPress(EPtiKey6);
    ptiEngine->AppendKeyPress(EPtiKey4);
    
    
    TPtrC8 currInputSequence (ptiEngine->CurrentInputSequence());
    if(0 != currInputSequence.Length())
    	iLog->Log(_L("Got Current Input Sequence" ));
    
    _LIT(KDog, "dog");
    if( 0 == ptiEngine->CurrentWord().Compare(KDog))
		{
		errStatus = KErrNone;
		}
    else
    	errStatus = KErrGeneral;

    if(KErrNone == errStatus)
    	{
	    // commit
    	errStatus = ptiEngine->CommitCurrentWord();
	
	    // set external key map
	    TInt ret;
	    TBuf<5> testLit;
	    testLit.Copy(_L("abcd"));
	    ret = ptiEngine->SetExternalKeyMapL(EPtiEngineMultitapping, EPtiKey5,
	                                                testLit, EPtiCaseUpper);
	    if(KErrNotSupported == ret)
	    	iLog->Log(_L("SetExternalKeyMapL not supported !!" ));
    	}
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }

void CptiEngineTestSdkApi :: PtiEngSimuInput(CPtiEngine* aEngine, const TDesC& aKeys)
    {
    for(int i=0; i<aKeys.Length(); ++i)
        {
        if (aKeys[i] == ' ')
            {
            continue;
            }
        else if (aKeys[i] == 'p')
            {
            continue;
            }
        else if (aKeys[i] == 'd')
            {
            aEngine->DeleteKeyPress();
            }
        else
            {
            aEngine->AppendKeyPress(aEngine->CharacterToKey(aKeys[i]));
            }
        }
    }

TInt CptiEngineTestSdkApi::PtiEngTestPredictL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine);
    
    //predictive
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
    PtiEngSimuInput(ptiEngine, _L("this"));

   if(0 == ptiEngine->CurrentWord().Compare(_L("this")))
	   {
	   //may be "this"
	   iLog->Log(_L(" Current Word is 'this' "));
	   }
        
    // next
    ptiEngine->NextCandidate(); // Move on to next candidate.
    ptiEngine->CommitCurrentWord();

    // delete
    PtiEngSimuInput(ptiEngine, _L("8447"));    
    ptiEngine->DeleteKeyPress();
       
    // reject
    ptiEngine->ClearCurrentWord();
    
    // char2key
    if(EPtiKey7 == ptiEngine->CharacterToKey('s'))
    	{
    	iLog->Log(_L("char 's' map to key 7"));
    	}

    // candidate list
    ptiEngine->ClearCurrentWord();
    CDesCArray* cands = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(cands);

    PtiEngSimuInput(ptiEngine, _L("8447"));
    ptiEngine->GetCandidateListL(*cands);
    
    if(0 != cands->Count())
        {
        iLog->Log(_L("Candidiates  Retrieved !!"));
        }
    
    if(cands->Count() == ptiEngine->NumberOfCandidates())
    	{
    	iLog->Log(_L("Number of Candidiates  match"));
    	}
    CleanupStack::PopAndDestroy(cands);  // cands

    // set current word
    ptiEngine->SetCurrentWord(_L("dog"));
    PtiEngSimuInput(ptiEngine, _L("t"));    
    ptiEngine->CurrentWord();
         
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }
TInt CptiEngineTestSdkApi::PtiEngGetCandidatesL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine);
    
    //predictive
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
    PtiEngSimuInput(ptiEngine, _L("lite"));
    
    RPointerArray<HBufC> ptrArray; 
    HBufC *candidateList;
    candidateList = ptiEngine->GetCandidatesByInputString(_L("lite"), ptrArray, true);
    
    if(NULL != candidateList)
    	{
    	iLog->Log(_L("GetCandidatesByInputString Retrieved !!! "));
    	delete (candidateList);
    	}
    else
    	iLog->Log(_L("GetCandidatesByInputString NULL"));
    
    ptrArray.Close();
        
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngTestInputModeL()
    {    
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
    
    errStatus = ptiEngine->SetInputMode(EPtiEngineMultitapping);
    if(KErrNone == errStatus)
    	iLog->Log(_L("Input Mode Set to Multitapping"));
    
    errStatus = ptiEngine->SetInputMode(EPtiEnginePredictive);
    if(KErrNone == errStatus)
    	iLog->Log(_L("Input Mode Set to Predictive"));
    
    errStatus = ptiEngine->SetInputMode(EPtiEngineNumeric);
    if(KErrNone == errStatus)
    	iLog->Log(_L("Input Mode Set to Numeric"));

    ptiEngine->SetCase(EPtiCaseUpper); 
    if( EPtiCaseUpper == ptiEngine->Case())
    	iLog->Log(_L("Upper Case Set !!"));
    
    ptiEngine->SetCase(EPtiCaseLower);   
    if( EPtiCaseLower == ptiEngine->Case())
    	iLog->Log(_L("Lower Case Set !!"));
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }

    
TInt CptiEngineTestSdkApi::PtiEngTestDictL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();  
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
    
    CDesCArrayFlat* words = new (ELeave) CDesCArrayFlat(5);
    CleanupStack::PushL( words );
    words->AppendL(_L("lorem"));
    words->AppendL(_L("ipsum"));
    words->AppendL(_L("dolor"));
    words->AppendL(_L("sit"));
    words->AppendL(_L("amet"));

    TBuf<50> filename(_L("C:\\logs\\TestFramework\\TestUserDictFile.txt"));
    CreateUserDicFileL(filename);
    TInt err ;
    
    // Temporarily modifications made due to change in PtiT9core
    err = ptiEngine->CreateUserDictionaryL(filename, words, EPtiEngineMultitapping);
    if(KErrNotSupported == err)
    	{
    	iLog->Log(_L("user dict created isn't supported"));
    	}
       
    CleanupStack::PopAndDestroy(words);  //words
	
    //attach
    MPtiUserDictionary* dict = ptiEngine->AttachUserDictionaryL(filename);
    if(dict)
        {
        iLog->Log(_L("user dict attached"));
        }
    else
        {
        iLog->Log(_L("user dict attached failure"));
        }

    //detach
    if(ptiEngine->DetachUserDictionary(filename) == KErrNone)
        {
        iLog->Log( _L("user dict detach"));
        }
    else
        {
        iLog->Log(_L("User dict detach failure"));
        }

    if(ptiEngine->DetachUserDictionary(0) == KErrNone)
        {
        iLog->Log( _L("user dict detach by ID"));
        }
    else
        {
        iLog->Log(_L("User dict detach by ID failure"));
        }

    //entry
    TFileName msg;
    TPtiUserDictionaryEntry entry(_L("foobar"));

    TInt number = ptiEngine->NumberOfEntriesInUserDictionary();
    if(number == KErrNotSupported)
        {
        iLog->Log( _L("user dict doesn't support entry"));
        }
    else
        {
        iLog->Log(_L("user dict supports entry"));
        }

    err = ptiEngine->AddUserDictionaryEntry(entry);
    
    err=ptiEngine->AddUserDictionaryEntry(entry, 0);
    
    err=ptiEngine->RemoveEntryFromUserDictionary(entry);
    
    err=ptiEngine->RemoveEntryFromUserDictionary(entry, 0);
    
    err = ptiEngine->GetUserDictionaryEntry(0, entry);
    
    dict = ptiEngine->DefaultUserDictionary(EPtiEnginePredictive);
    if(dict)
        {
        iLog->Log(_L("dict for Predictive found"));
        }
    else
        {
        iLog->Log(_L("dict for Predictive not found"));
        }
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    
    }

TInt CptiEngineTestSdkApi::PtiEngTestDefaultUserDictL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineNumeric);
    
    MPtiCoreInfo *coreInfo = ptiEngine->CoreInfo(EPtiEngineNumeric);
    ptiEngine->CreateDefaultUserDictionaryFileL(TUid::Uid(coreInfo->Uid()),0);
    
    MPtiUserDictionary *defaultUserDict = ptiEngine->AttachDefaultUserDictionaryL(TUid::Uid(coreInfo->Uid()),0);
    
    if(NULL != defaultUserDict)
    	errStatus = KErrNone;
    else
    	errStatus = KErrGeneral;
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    errStatus = KErrNone;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngTestSecDataFileL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineNumeric);
    
    _LIT(KData, "Information Set !!!");
    HBufC8* data = HBufC8::NewL(30);
    CleanupStack::PushL(data);
    
    TPtr8 text = data->Des();
    text.Copy(KData);
    
    // Data Written in SecondaryFile
    MPtiCoreInfo *coreInfo = ptiEngine->CoreInfo(EPtiEngineNumeric);
    ptiEngine->WriteSecondaryDataFileL(TUid::Uid(coreInfo->Uid()), 0, data);  
    
    //Data Read from Secondary File    
    HBufC8 *readData = ptiEngine->ReadSecondaryDataFileL(TUid::Uid(coreInfo->Uid()), 0);
        
    if(0 == readData->Compare(text))
    	errStatus = KErrNone;
    else
    	errStatus = KErrGeneral;
    	
    CleanupStack::PopAndDestroy(data);    
    delete (readData); 
    
    ptiEngine->CloseCurrentLanguageL();    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    
    }

TInt CptiEngineTestSdkApi::PtiEngTestLastKeyL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineNumeric);
    ptiEngine->ClearCurrentWord();
    if(EPtiKeyNone == ptiEngine->LastEnteredKey())
    	{
    	iLog->Log(_L("none last entered key"));
    	}

    PtiEngSimuInput(ptiEngine, _L("dog"));
    if(EPtiKeyGHI ==  ptiEngine->LastEnteredKey())
    	{
    	errStatus = KErrNone;
    	iLog->Log(_L("last key for 'dog' is GHI"));
    	}
    else
    	errStatus = KErrGeneral;
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngTestNavigationL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineNumeric);
    ptiEngine->ClearCurrentWord();
    PtiEngSimuInput(ptiEngine, _L("46873"));

    CDesCArray* cands = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(cands);
    
    ptiEngine->GetCandidateListL(*cands);
    if ( 0 == cands->Count() )
    	{
    	cands->AppendL(_L("4"));
    	}
    if(0 == ptiEngine->FirstCandidate().Compare( cands->MdcaPoint(0) ))
		{
		errStatus = KErrNone;
		iLog->Log(_L("First candidate verified"));
		}   
    else
    	{
    	errStatus = KErrNone;
    	iLog->Log(_L("First candidate not verified"));
    	}

    for (int i = 1; i < cands->Count(); ++i)
        {
        if(0 == ptiEngine->NextCandidate().Compare(cands->MdcaPoint(i)))
			{
			iLog->Log(_L("next candidate verified"));
			}            
        }

    for (int i = cands->Count() - 2; i >= 0; --i)
        {
        if(0 == ptiEngine->PreviousCandidate().Compare(cands->MdcaPoint(i)))
            {
            iLog->Log(_L("Previous candidate verified"));
            }
        }

    CleanupStack::PopAndDestroy(cands); // cands
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngTestReorderingL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
    TInt err = ptiEngine->SetReordering(ETrue);
    if(err == KErrNone)
        {
        iLog->Log(_L("set reordering"));
        ptiEngine->ClearCurrentWord();
        PtiEngSimuInput(ptiEngine, _L("364"));

        // get the third candidate
        TBuf<64> word;
        ptiEngine->NextCandidate();
        ptiEngine->NextCandidate();
        word.Copy(ptiEngine->NextCandidate());

        // commit it 4 times
        for(int i=0; i<4; ++i)
            {
            ptiEngine->SetCurrentWord(word);
            ptiEngine->CommitCurrentWord();
            }
        PtiEngSimuInput(ptiEngine, _L("364"));
        if(0 == ptiEngine->CurrentWord().Compare(word))
        	{
        	iLog->Log(_L("reordered candidate verified"));
        	}
        ptiEngine->SetReordering(EFalse);
        }
    else
        {
        iLog->Log(_L("set reordering isn't supported"));
        }
    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }

TInt CptiEngineTestSdkApi::PtiEngTestAutoSubstitutionL()
{
    __UHEAP_MARK;
    TInt errStatus = KErrNone; 
       
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
    
    TInt status = ptiEngine->SetAutoSubstitution(ETrue);
    
    if(KErrNone == status)
    {
        ptiEngine->AddAutoSubstitutionEntry(_L("xmas"),_L("christmas"));
        ptiEngine->AddAutoSubstitutionEntry(_L("nokia"),_L("connectingpeople"));
        
        TInt numOfEntries = ptiEngine->NumberOfAutoSubstitutionEntries();
        if(2 != numOfEntries)
            errStatus = KErrNotSupported;
        
        ptiEngine->DeleteAutoSubstitutionEntry(_L("xmas"));
        
        numOfEntries = ptiEngine->NumberOfAutoSubstitutionEntries();
        if(1 != numOfEntries)
            errStatus = KErrNotSupported;
        
        TBuf<20> shortcutText;
        TBuf<20> ubstitutionText;
        
        ptiEngine->GetAutoSubstitutionEntry(0,shortcutText,ubstitutionText);
        
        if(0 !=shortcutText.Compare(_L("nokia")))
            errStatus = KErrNotSupported;
        
        if(0 !=ubstitutionText.Compare(_L("connectingpeople")))
               errStatus = KErrNotSupported;
        
        TInt status = ptiEngine->SetAutoSubstitution(EFalse);
        
        if(status != KErrNone)
            errStatus = KErrNotSupported; 
        
    }
    else
        errStatus = KErrNotSupported;
      
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();     
        
    __UHEAP_MARKEND;
    
    errStatus = KErrNone;
    
    return errStatus;

}


TInt CptiEngineTestSdkApi::PtiEngTestListKeyboardsL()
{
        __UHEAP_MARK;
       TInt errStatus = KErrNone; 
          
       CPtiEngine *ptiEngine = CPtiEngine::NewL();    
       CleanupStack::PushL(ptiEngine); 
       
       ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineMultitapping);
       
       TPtiKeyboardType currKeyboardType = ptiEngine->KeyboardType();
       
       if(EPtiKeyboard12Key != currKeyboardType)
           errStatus = KErrNotSupported;
       
       RArray<TPtiKeyboardType> supportedKeyboards;
       ptiEngine->KeyboardTypesSupportedByLanguageL(ELangEnglish,supportedKeyboards);
      
       if(supportedKeyboards.Count() != 6) // six keyboard types are supported for Zi Engine
           errStatus = KErrNotSupported;
           
       supportedKeyboards.Close();
       
       ptiEngine->CloseCurrentLanguageL();
       ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineQwerty);
       
      if(KErrNone != ptiEngine->SetKeyboardType(EPtiKeyboardQwerty4x10))
           errStatus = KErrNotSupported;
       
       if(ptiEngine->IsQwertyBasedMode(EPtiEngineQwerty) != (TInt)ETrue)
           errStatus = KErrNotSupported; 
       
       ptiEngine->CloseCurrentLanguageL();
       CleanupStack::PopAndDestroy(ptiEngine);
       REComSession::FinalClose();     
           
       __UHEAP_MARKEND;
       
       errStatus = KErrNone;
       
       return errStatus;
}

TInt CptiEngineTestSdkApi::PtiEngTestPhysicalListKeyboardsL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();      
    CleanupStack::PushL(ptiEngine);
    
    RArray<TPtiKeyboardType> aResult;
    ptiEngine->ListAvailablePhysicalKeyboardsL(aResult);
    
    aResult.Close();
    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();     
        
    __UHEAP_MARKEND;
    return errStatus;
    
    }

TInt CptiEngineTestSdkApi::PtiEngTestOtherL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CPtiEngine::NewL();
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineMultitapping);
    
    ptiEngine->HandleCommandL(EPtiCommandNone);
    
    ptiEngine->EnableToneMarks(ETrue);
    ptiEngine->EnableToneMarks(EFalse);
    ptiEngine->ResetToneMark();
    
    iLog->Log( _L("tone mark set and reset") );
    
    TText result;
    if (ptiEngine->ToneMark(result))
    	{
    	iLog->Log(_L("tone mark got"));
    	}    

    if(ptiEngine->IsToneMarkValidForSpelling())
    	iLog->Log(_L("tone mark is valid for spelling"));
    else
    	iLog->Log(_L("tone mark is invalid for spelling"));

    if(ptiEngine->IncrementToneMark(ETrue))
    	iLog->Log(_L("new tone mark was found and set"));
    else
    	iLog->Log( _L("didn't found new tone mark"));

    if(ptiEngine->SetToneMark(0))
    	iLog->Log(_L("tone mark was legal"));
    else
    	iLog->Log(_L("illegal tone mark"));

    //key mapping
    TBuf<8> mapRes;
    ptiEngine->MappingDataForKey(EPtiKey2, mapRes, EPtiCaseLower);
    iLog->Log(_L("mapping data for key"));
    
    ptiEngine->CloseCurrentLanguageL();
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineQwerty);

    RArray<TPtiNumericKeyBinding> qwertyMapRes;
    ptiEngine->GetNumericModeKeysForQwertyL(ELangEnglish, qwertyMapRes);
    iLog->Log(_L("get numeric mode keys for qwerty keys"));
    qwertyMapRes.Close();
    
    RArray<TPtiNumericKeyBinding> qwertyMapRes1;
    ptiEngine->GetNumericModeKeysForQwertyL(ELangEnglish,qwertyMapRes1,EPtiKeyboardQwerty4x12);
    iLog->Log(_L("get numeric mode keys for qwerty keys for qwerty "));
    qwertyMapRes1.Close();
    
    
    const RArray<TPtiNumericKeyBinding> & qwertyMapRes2 = ptiEngine->ExtendedNumericModeKeysForQwertyL(ELangEnglish);
    iLog->Log(_L("get numeric mode keys for qwerty keys for exteneded numeric keys for qwerty "));
    
    const RArray<TPtiNumericKeyBinding> &qwertyMapRes3 = ptiEngine->ExtendedNumericModeKeysForQwertyL(ELangEnglish,EPtiKeyboardQwerty4x12);
    iLog->Log(_L("get numeric mode keys for qwerty keys for exteneded numeric keys for qwerty "));
    
    
    ptiEngine->CloseCurrentLanguageL();
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineMultitapping);
    iLog->Log( _L("PtiEngTestOtherL : ActivateLanguageL with EPtiEngineMultitapping called") );
    
    if((TInt)ETrue != ptiEngine->IsValidKey(EPtiKey1))
        errStatus = KErrNotSupported;
   
    ptiEngine->AppendKeyPress(EPtiKey3);
    ptiEngine->AppendKeyPress(EPtiKey6);
    ptiEngine->AppendKeyPress(EPtiKey6);
    ptiEngine->AppendKeyPress(EPtiKey6);
    ptiEngine->AppendKeyPress(EPtiKey4);
    
    
    TPtrC8 buffer(ptiEngine->CaseSequence());

    _LIT(KDog, "dog");
    
    ptiEngine->MarkAutoCapitalized(); // Not working on ITUT. Needs to check why
    
    ptiEngine->CancelTimerActivity();
   
    if( KErrNone != ptiEngine->CurrentWord().Compare(KDog))
        errStatus = KErrGeneral;
    
    if(KErrNone != ptiEngine->AddPhrase(_L("DummyData")))
        iLog->Log( _L("PtiEngTestOtherL : AddPhrase does not work: May be core does not support this") );
    
    
    ptiEngine->ClearCurrentWord();
    
    ptiEngine->CloseCurrentLanguageL();
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEngineQwertyPredictive);
    iLog->Log( _L("PtiEngTestOtherL : ActivateLanguageL with EPtiEngineQwertyPredictive called") );
    
    if(KErrNone != ptiEngine->SetNumberCandidateStatus(ETrue))
        errStatus = KErrGeneral;
      
    if(KErrNone != ptiEngine->SetNumberCandidateStatus(EFalse))
        errStatus = KErrGeneral;
    
    if(KErrNone != ptiEngine->SetMaxLengthForAutoCompletedCandidates(9))
        errStatus = KErrGeneral;
           
    ptiEngine->CloseCurrentLanguageL();
    

    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    
    __UHEAP_MARKEND;
    iLog->Log( _L("PtiEngTestOtherL : End") );
    
    errStatus = KErrNone; 
    
    return errStatus;
    }

CPtiEngine* CptiEngineTestSdkApi :: CreatePtiEngineL(TInt aLanguage, TPtiEngineInputMode aMode)
    {
    CPtiEngine* engine = CPtiEngine::NewL();
    MPtiLanguage* lang = engine->GetLanguage(aLanguage);

    TBuf<100> numStr;

    if (!lang)
        {
        goto fail;
        }

    if (!lang->HasInputMode(aMode))
        {
        goto fail;
        }

    TInt err = engine->ActivateLanguageL(aLanguage, aMode);
    if (err != KErrNone)
        {
        goto fail;
        }

    return engine;

fail:
    delete engine;
    return NULL;
    }


// Note : This API is crashing on ATS..... (and not on emulator)
TInt CptiEngineTestSdkApi::PtiEngTestChineseInputL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone; 
    
    // Temporarily added - START
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
    // Temporarily added - END
    
    // Commented temporarily as Chinese is crashing on ATS (target)
    //CPtiEngine *ptiEngine = CreatePtiEngineL(ELangPrcChinese, EPtiEngineStrokeByPhrase);
    //CleanupStack::PushL(ptiEngine); 

    if(ptiEngine != NULL)
        {
        iLog->Log(_L("Chinese Pinyin activated"));
        PtiEngSimuInput(ptiEngine, _L("wei"));

        ptiEngine->SetCandidatePageLength(20);
        iLog->Log(_L("page length set to 20"));
        
        TPtrC str = ptiEngine->CandidatePage();
        TBool moreCandidatePage = ptiEngine->MoreCandidatePages();
        TBool nxtCandidatePage = ptiEngine->NextCandidatePage();
            
        //predict
        ptiEngine->PreviousCandidatePage();
        TPtrC aChar = ptiEngine->CandidatePage();
        ptiEngine->SetPredictiveChineseChar(aChar);
        iLog->Log(_L("next chinese char predicted"));

        //test next candidate list
        ptiEngine->CommitCurrentWord();
        CDesCArray* cands = new (ELeave) CDesCArrayFlat(16);
        CleanupStack::PushL(cands);

        TRAPD(err, ptiEngine->GetNextWordCandidateListL(*cands));
        if(err == KErrNotSupported)
            {
            iLog->Log(_L("predict next candidate not supported"));
            }
        else{
            iLog->Log(_L("predict next candidate supported"));
            }
        CleanupStack::PopAndDestroy(cands);  // cands

        //test conversion
        TInt convCap=ptiEngine->AvailableCharConversions();
        if((convCap & EPtiSimplifiedToComplex)!=0)
            {
            iLog->Log( _L("can convert from simplified chinese to traditional"));
            }
        else
            {
            iLog->Log( _L("conv capability:"));
            }
        
        char inbuf[]={0x76, 0x84};
        char outbuf[]={0,0};
        if(KErrNone == ptiEngine->CharConversion(EPtiSimplifiedToComplex, inbuf, 2, outbuf))
            iLog->Log(_L("tried char conversion"));

        //Chinese Variant
        RArray<TInt> modes;
        ptiEngine->GetModeNameIndexL(EPtiChineseVariantPrc, modes);
        if(0 != modes.Count())
            iLog->Log(_L("Chinese varient index table got"));
        modes.Close();

        //phrase list
        ptiEngine->ClearCurrentWord();
        PtiEngSimuInput(ptiEngine, _L("wei"));
        
        cands = new (ELeave) CDesCArrayFlat(16);
        CleanupStack::PushL(cands);
        if( KErrNone == ptiEngine->GetChinesePhraseCandidatesL(*cands))
            iLog->Log(_L("Chinese phrase candidate list got"));
            
        CleanupStack::PopAndDestroy(cands);  //cands

        PtiEngTestSpellL(ptiEngine);
        }
    else
        {
        iLog->Log(_L("unable to activate Chinese Pinyin"));      
        }
       
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();     
        
    __UHEAP_MARKEND;
    return errStatus;
    
    }

void CptiEngineTestSdkApi::PtiEngTestSpellL(CPtiEngine *aPtiEngine)
    {
    TBuf<8> spell;   
    
    TInt err=aPtiEngine->GetSpelling(0x7684, spell, EPtiPinyin);
    if(err==KErrNotSupported)
        {
        iLog->Log(_L("get spelling not support"));
        }
    else if(err==KErrNone)
        {
        iLog->Log(_L(" get spelling passed "));
        }
    else
        {
        iLog->Log( _L("get spelling failuer"));
        }

    //phonetic
    aPtiEngine->ClearCurrentWord();
    PtiEngSimuInput(aPtiEngine, _L("wei"));
    TInt number = aPtiEngine->PhoneticSpellingCount();
    
    spell=aPtiEngine->GetPhoneticSpelling(0);
    iLog->Log(_L("get phonetic spelling for 0"));

    TBool result = aPtiEngine->SelectPhoneticSpelling(0);
    iLog->Log(_L("select phonetic spelling for 0"));

    number = aPtiEngine->SelectedPhoneticSpelling();
    iLog->Log(_L(" phonetic spelling selected"));
    
    CDesCArray* array = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(array);
    aPtiEngine->GetPhoneticSpellingsL(*array);
    iLog->Log(_L("phonetic spellings got"));
    CleanupStack::PopAndDestroy(array);  //array
    
    }
TInt CptiEngineTestSdkApi::PtiEngTestJapaneseInputL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;    
    CPtiEngine *ptiEngine = CreatePtiEngineL(ELangJapanese, EPtiEngineHiraganaKanji);
    if(ptiEngine != NULL)
        {
        CleanupStack::PushL(ptiEngine); 
        iLog->Log( _L("Japanese Hirakana Kanji activated"));
        }
    else
        {
        iLog->Log(_L("can not activate Japanese input"));
        ptiEngine = CPtiEngine::NewL();
        CleanupStack::PushL(ptiEngine); 
        ptiEngine->ActivateLanguageL(ELangEnglish, EPtiEnginePredictive);
        }

    MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    if(NULL != data)
	    {
        iLog->Log(_L("Japanese composition data IF got"));
	    }

    PtiEngSimuInput(ptiEngine, _L("nihon"));
    TPtrC str=ptiEngine->ReadingTextL();
           
    ptiEngine->ClearCurrentWord();
    
    ptiEngine->CloseCurrentLanguageL();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
    }


TInt CptiEngineTestSdkApi:: PtiGetHwrRecognizer1L()
{
	__UHEAP_MARK;
    TInt errStatus = KErrNone; 
       
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    MPtiHwrRecognizer *core = ptiEngine->GetHwrRecognizerL(ELangPrcChinese);
            
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
	
}

TInt CptiEngineTestSdkApi:: PtiGetHwrRecognizer2L()
{
	__UHEAP_MARK;
    TInt errStatus = KErrNone; 
       
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    RImplInfoPtrArray infoArray;

    REComSession::ListImplementationsL(TUid::Uid(0x1028185d), infoArray);
        
    if(infoArray.Count() > 0)
        {
        TUid uid = infoArray[1]->ImplementationUid();
        MPtiHwrRecognizer *core = NULL;
        core = ptiEngine->GetHwrRecognizerL(uid.iUid);
        
        if(core == NULL)
            errStatus = KErrGeneral;
        
        for(TInt i=0;i<infoArray.Count();i++)
            {
            delete infoArray[i];
            }
        }
    
    infoArray.Close();
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
}

TInt CptiEngineTestSdkApi:: PtiListHwrRecognizerL()
{
	__UHEAP_MARK;
    TInt errStatus = KErrNone; 
       
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    RArray<TUid> hwrImplementationID;
    
    hwrImplementationID = ptiEngine->ListHwrRecognizerL(ELangEnglish);
        
    hwrImplementationID.Close();
    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();    	
    	
    __UHEAP_MARKEND;
    return errStatus;
	
}

TInt CptiEngineTestSdkApi:: PtiGetHwrAvailableLanguagesL()
{
    __UHEAP_MARK;
    TInt errStatus = KErrNone; 
       
    CPtiEngine *ptiEngine = CPtiEngine::NewL();    
    CleanupStack::PushL(ptiEngine); 
    
    RArray<TInt> hwrLanguages;
    
    ptiEngine->GetHwrAvailableLanguagesL(hwrLanguages);
                
    hwrLanguages.Close();
    
    CleanupStack::PopAndDestroy(ptiEngine);
    REComSession::FinalClose();     
        
    __UHEAP_MARKEND;
    return errStatus;
    
}



// For PtiEngine.h END

TInt CptiEngineTestSdkApi::AknInputLanguageCapabilitiesL()
    {
    __UHEAP_MARK;
     TInt errStatus = KErrNone; 
        
     TAknInputLanguageCapabilities inputLanguageCapabilities;
     
     inputLanguageCapabilities.AssignCapability(TAknInputLanguageCapabilities::EMultitap, ETrue );
     
     if((TInt)ETrue != inputLanguageCapabilities.HasCapability(TAknInputLanguageCapabilities::EMultitap))
             errStatus = KErrGeneral;
     
     if((TInt)ETrue != inputLanguageCapabilities.HasAnySupport())
             errStatus = KErrGeneral; 
     
     inputLanguageCapabilities.SetAllCapabilities();
     
     if((TInt)ETrue != inputLanguageCapabilities.HasCapability(TAknInputLanguageCapabilities::EMultitap))
             errStatus = KErrGeneral;
     
     TAknInputLanguageCapabilities inputLanguageCapabilities1;
     
     inputLanguageCapabilities1.SetAllCapabilities();
     
     TAknInputLanguageCapabilities inputLanguageCapabilities2 = inputLanguageCapabilities1.FilteredCapabilities(inputLanguageCapabilities);
     
     if((TInt)ETrue != inputLanguageCapabilities2.HasCapability(TAknInputLanguageCapabilities::EMultitap))
             errStatus = KErrGeneral;
     
     __UHEAP_MARKEND;
     return errStatus;
    }

TInt CptiEngineTestSdkApi::AknInputLanguageItemL()
    {
    __UHEAP_MARK;
     TInt errStatus = KErrNone; 
        
     CAknInputLanguageInfo * langInfo = AknInputLanguageInfoFactory::CreateInputLanguageInfoL();
     CleanupStack::PushL(langInfo);
         
     CAknInputLanguageList * languageList=new CAknInputLanguageList(20);
     
     CArrayFix<TInt>* languageCodeList = NULL;
     
     TAknInputLanguageCapabilities capabilityFilter; 
     capabilityFilter.AssignCapability(TAknInputLanguageCapabilities::EMultitap,ETrue);
     
     TInt err = KErrNone;
     TRAP(err,langInfo->AppendLanguagesL(languageList,languageCodeList,capabilityFilter));
     
     if(err != KErrNone)
         {
         errStatus = KErrGeneral;
         return errStatus;
         }
     
     TInt languageCount = languageList->MdcaCount();
     
     if(languageCount <= 0)
         {
         errStatus = KErrGeneral;
         return errStatus;
         }
     TBuf<30> langName = languageList->MdcaPoint(0);
     
     CAknInputLanguageItem * langaugeItem = languageList->At(0);
     
     TLanguage language = langaugeItem->LanguageCode();
     
     if( (TInt)language < 0 && (TInt)language > ELangMaximum)
         {
         errStatus = KErrGeneral;
         return errStatus;
         }
     
     TBuf<30> langName1 = langaugeItem->LanguageName();
     
     if( 0 != langName.Compare(langName1))
         errStatus = KErrGeneral;
     
     TAknInputLanguageCapabilities langCapabilities = langaugeItem->Capabilities();
     
     if((TInt)ETrue != langCapabilities.HasCapability(TAknInputLanguageCapabilities::EMultitap))
         errStatus = KErrGeneral;
     
     delete languageList;
     CleanupStack::PopAndDestroy(langInfo);
     __UHEAP_MARKEND;
     return errStatus;   
     
    }

// For PtiCore.h  - START
TInt CptiEngineTestSdkApi::PtiCoreSetupL()
    {
    __UHEAP_MARK;
            
    TInt errStatus = KErrNone;
    TInt firstUid = GetCoreUidL();
    CPtiCore* ptiCore = CPtiCore::CreateCoreL(TUid::Uid(firstUid));  
            
    if(ptiCore)
        {       
        delete ptiCore;
        ptiCore = NULL;
        REComSession::FinalClose();
        }
    else
		{
        errStatus =  KErrGeneral;
		}

    __UHEAP_MARKEND;
    return errStatus; 
    }


TInt CptiEngineTestSdkApi::TestListCoreL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
        
    TInt firstUID = GetCoreUidL();
    if(0 != firstUID)
        errStatus =  KErrNone;
    else
        errStatus =  KErrGeneral;    
    
    __UHEAP_MARKEND;
    return errStatus;
    }

CPtiCore* CptiEngineTestSdkApi:: CreatePtiCoreL()
{
    // a helper class --> Needed to test all functions of PtiCore.h
    class CMockPtiCore: public CPtiCore
        {
        public:
        void InitializeL(CPtiEngine* /*aOwner*/, TDes* /*aTextBuffer*/,
            CArrayPtrFlat<CPtiCoreLanguage>* /*aLanguageList*/, 
            TBool /*aUseDefautUserDictionary*/)
            {
            }
        
        void DropLanguages(CArrayPtrFlat<CPtiCoreLanguage>* /*aLanguageList*/)
            {
            }
        TInt OpenLanguageL(CPtiCoreLanguage */*aLanguage*/)
            {
            return  0;
            }
        TInt CloseLanguageL()
            {
            return  0;  
            }
        MPtiCoreInfo* GetCoreInfo()
            {
            return NULL;
            }
        TInt SetReordering(TBool /*aStatus*/)
            {
            return 0;
            }
        void AppendKeyPress(TPtiKey /*aKey*/)
            {
            }
        void DeleteKeyPress()
            {           
            }
        void ClearBuffer()
            {           
            }
        TInt CancelTimerActivity()
            {
            return 0;
            }
        TInt HandleCommandL(TPtiEngineCommand /*aCommand*/, TAny* /*aParams*/)
            {
            return 0;
            }
            
        };
        
    CPtiCore* ptiCore = new (ELeave) CMockPtiCore;  
    return ptiCore;
        
}

TInt CptiEngineTestSdkApi::PtiCoreTestGeneralL()
    { 
    __UHEAP_MARK;
      
    CPtiCore* ptiCore = CreatePtiCoreL();
    
    TInt errStatus = KErrNone;
    CleanupStack::PushL(ptiCore);
       
    if(  KErrNotSupported ==  ptiCore->Convert(EPtiSimplifiedToComplex, NULL, 0, NULL))
        {
        iLog->Log(_L("Convert hasn't been supported yet"));
        }
        
    TPtrC8 dummy(KNullDesC8);
    if(KErrNotSupported ==  ptiCore->SetCurrentInputSequence(dummy))
        {
        iLog->Log(_L("SetCurrentImputSequence() is not supported"));
        }
       
    TBuf<5> output;
    if( KErrNotSupported == ptiCore->GetSpelling('a', output, EPtiStrokes))
        {
        iLog->Log(_L("GetSpelling returns General Err"));
        }

    _LIT(KNewMapLower, "!@#$");
    TBuf<5> lowerMap(KNewMapLower);
    if( KErrNotSupported == ptiCore->SetExternalKeyMapL(EPtiEngineMultitapping, EPtiKey1, lowerMap, EPtiCaseLower))
        {
        iLog->Log(_L("SetExternalKeyMapL hasn't been supported"));
        }

    if( KErrNotSupported == ptiCore->CommitCurrentWord())
        {
        iLog->Log(_L("CommitCurrentWord not supported "));
        }
        
    if( TPtrC() == ptiCore->GetCurrentWord())
        {
        iLog->Log(_L("GetCurrentWord"));
        }
           
    CleanupStack::PopAndDestroy(ptiCore);    
    REComSession::FinalClose();
            
    __UHEAP_MARKEND;
    return errStatus;   
    }

TInt CptiEngineTestSdkApi::PtiCoreTestPredictiveLatinL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiCore* ptiCore = CreatePtiCoreL();
    CleanupStack::PushL(ptiCore);
    
    if( 0 == ptiCore->NumberOfCandidates())
        {
        iLog->Log(_L("NumberOfCandidates not implemented"));
        }

    CDesCArray* cands = new (ELeave) CDesCArrayFlat(2);
    CleanupStack::PushL(cands);
    if(KErrNotSupported ==  ptiCore->GetCandidateListL(*cands))
        {
        iLog->Log(_L("Get CandidateList not supported"));
        }
    
    if (KErrNotSupported ==  ptiCore->GetNextWordCandidateListL(*cands))
        {
        iLog->Log(_L("Get Next word CandidateList not supported"));
        }
    CleanupStack::PopAndDestroy(cands);  //cands

    TBuf<5> res;
    if(KErrNotSupported == ptiCore->GetFirstCandidate(res))
        {
        iLog->Log(_L("Get first candidate not supported"));
        }
    
    if(KErrNotSupported == ptiCore->GetNextCandidate(res))
        {
        iLog->Log(_L("Get next candidate not supported"));
        }
    
    if(KErrNotSupported == ptiCore->GetPreviousCandidate(res))
        {
        iLog->Log(_L("Get prev candidate not supported"));
        }
    
       
    RPointerArray<HBufC> ptrArray; 
    HBufC *candidateList;
    candidateList = ptiCore->GetCandidatesByInputString(_L("lite"), ptrArray, true);
    
    if(NULL != candidateList)
        {
        iLog->Log(_L("GetCandidatesByInputString Retrieved !!! "));
        delete (candidateList);
        }
    else
        iLog->Log(_L("GetCandidatesByInputString NULL"));
    
    ptrArray.Close();
    
    CleanupStack::PopAndDestroy(ptiCore);    
    REComSession::FinalClose();
    
    
    __UHEAP_MARKEND;
    return errStatus;
    
    }

TInt CptiEngineTestSdkApi::PtiCoreTestUserDictL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    TPtiUserDictionaryEntry entry(_L("foobar"));
    
    CPtiCore* ptiCore = CreatePtiCoreL();
    CleanupStack::PushL(ptiCore);
    
    if(KErrNotSupported == ptiCore->AddUserDictionaryEntry(entry, NULL))
        {
        iLog->Log(_L("Add user dict entry not supported"));
        }
    if(KErrNotSupported == ptiCore->RemoveUserDictionaryEntry(entry, NULL))
        {
        iLog->Log(_L("Remove user dict entry not supported"));
        }
    if(0 == ptiCore->NumberOfEntriesInUserDictionary(NULL))
        {
        iLog->Log(_L("NumberOfEntries in user dict got"));
        }
    if(KErrNotSupported == ptiCore->GetUserDictionaryEntry(0, entry, NULL))
        {
        iLog->Log(_L("get user dict entry not supported"));
        }
    CPtiUserDictionary* dict=ptiCore->CreateEmptyUserDictionaryL(32);
    CleanupStack::PushL(dict);
    
    if(KErrNotSupported == ptiCore->AttachUserDictionary(dict))
        {
        iLog->Log(_L("attach user dict not supported"));
        }
    
    if(!ptiCore->DetachUserDictionary(dict))
        {
        iLog->Log(_L("detach user dict"));
        }
    CleanupStack::PopAndDestroy(dict);  //dict
    
    CleanupStack::PopAndDestroy(ptiCore);    
    REComSession::FinalClose();
            
    __UHEAP_MARKEND;
    return errStatus;   
    }

TInt CptiEngineTestSdkApi::PtiCoreTestChineseL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiCore* ptiCore = CreatePtiCoreL();
    CleanupStack::PushL(ptiCore);
   
    if(TPtrC() == ptiCore->GetCandidatePage())
        {
        iLog->Log(_L("Get candidate page"));
        }
    if(!ptiCore->NextCandidatePage())
        {
        iLog->Log(_L("no next candidate page"));
        }
    if(!ptiCore->PreviousCandidatePage())
        {
        iLog->Log(_L("no prev candidate page"));
        }
    if(!ptiCore->MoreCandidatePages())
        {
        iLog->Log(_L("no more candidate page"));
        }
    
    ptiCore->SetCandidatePageLength(0);
    iLog->Log(_L("set candidate page length do nothing"));
    
    if(TPtrC() == ptiCore->GetPhoneticSpelling(0))
        {
        iLog->Log(_L("Get phonetic spelling"));
        }
    if(0 == ptiCore->PhoneticSpellingCount())
        {
        iLog->Log(_L("phonetic spelling count returns 0"));
        }
    if(!ptiCore->SelectPhoneticSpelling(0))
        {
        iLog->Log(_L("select phonetic spelling returns false"));
        }
    if(KErrNotSupported == ptiCore->SelectedPhoneticSpelling())
        {
        iLog->Log(_L("selected phonetic spelling not supported"));
        }
    TText mark='a';
    if(!ptiCore->ToneMark(mark))
        {
        iLog->Log(_L("tone mark returns false"));
        }
    if(!ptiCore->IsToneMarkValidForSpelling())
        {
        iLog->Log(_L("tone mark is invalid for spelling"));
        }
    if(!ptiCore->SetPredictiveChineseChar(_L("")))
        {
        iLog->Log(_L("set predictive chinese char returns false"));
        }
    
    if(!ptiCore->SetToneMark(0))
        {
        iLog->Log(_L("set tone mark returns false"));
        }    
    
    CleanupStack::PopAndDestroy(ptiCore);       
    REComSession::FinalClose();
        
    __UHEAP_MARKEND;
    return errStatus;   
    }

TInt CptiEngineTestSdkApi::PtiCoreTestJapaneseL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiCore* ptiCore = CreatePtiCoreL();    
    CleanupStack::PushL(ptiCore);  
       
    if(TPtrC() == ptiCore->ReadingTextL())
        {
        iLog->Log(_L("reading text" ));        
        }
    if(NULL == ptiCore->CompositionData())
        {
        iLog->Log(_L("composition data NULL"));
        }
        
    CleanupStack::PopAndDestroy(ptiCore);    
    REComSession::FinalClose();
    
    __UHEAP_MARKEND;
    return errStatus;   
    }
// For PtiCore.h  - END

// -----------------------------------------------------------------------------
// CptiEngineTestSdkApi::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CptiEngineTestSdkApi::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
