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

















// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "ptiEngTestDomApi.h"

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
#include <PtiKeyboardDatabase.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
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
// CptiEngTestDomApi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CptiEngTestDomApi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CptiEngTestDomApi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        
        ENTRY( "PtiTestKeyBindings", CptiEngTestDomApi::PtiTestKeyMapDataKeyBindingsL),
        ENTRY( "PtiTestLanguages", CptiEngTestDomApi::PtiTestKeyMapDataLanguagesL),
        ENTRY( "PtiTestKeys", CptiEngTestDomApi::PtiTestKeyMapDataTestkeysL),
        ENTRY( "PtiTestDeadKeys", CptiEngTestDomApi::PtiTestKeyMapDataTestDeadkeysL),
        ENTRY( "PtiTestNumericKeys", CptiEngTestDomApi::PtiTestKeyMapDataTestNumerickeysL),
        ENTRY( "PtiTestVowels", CptiEngTestDomApi::PtiTestKeyMapDataTestVowelsL),
        ENTRY( "PtiTestOthers", CptiEngTestDomApi::PtiTestKeyMapDataTestOthersL),
        ENTRY( "PtiTestDataFactory", CptiEngTestDomApi::PtiTestKeyMapDataFactoryL),
        
        ENTRY( "PtiTestKeyboardDatabase", CptiEngTestDomApi::PtiTestKeyboardDatabaseFactoryL),
                // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataKeyBindingsL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt  CptiEngTestDomApi::PtiTestKeyMapDataKeyBindingsL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiKeyMapData * keyMapData = static_cast<CPtiKeyMapData*>(GetKeyMapDataL());
    
    TPtiKey key = EPtiKey1;
    TPtiTextCase ptiCase =EPtiCaseLower;
    TInt index = keyMapData->CaseBasedIndexInBindingTable(EPtiKeyboard12Key,key,ptiCase);
    
    if(index != 0)
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : CaseBasedIndexInBindingTable for EPtiKeyboard12Key,EPtiKey1,EPtiCaseLower returned something other than 0"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : CaseBasedIndexInBindingTable for EPtiKeyboard12Key,EPtiKey1,EPtiCaseLower returned  0"));
    
    TPtiTextCase ptiCase1;
    TPtiKey key1 =keyMapData->KeyForBindingTableIndex(EPtiKeyboard12Key,index,ptiCase1);    
    if( (ptiCase != ptiCase1) || (key != key1))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : KeyForBindingTableIndex for EPtiKeyboard12Key,index 0 returned something other than EPtiKey1"));
        }
    else 
        iLog->Log(_L("CptiEngTestDomApi : KeyForBindingTableIndex for EPtiKeyboard12Key,index 0 returned EPtiKey1"));
    
    delete keyMapData;
    REComSession::FinalClose();
    iLog->Log(_L(" "));
     __UHEAP_MARKEND;
    return errStatus;
    }
// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataLanguagesL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyMapDataLanguagesL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiKeyMapData * keyMapData = static_cast<CPtiKeyMapData*>(GetKeyMapDataL());
    
    RArray<TInt> langList;
    
    //CPtiKeyMapData::ListLanguagesL(langList); No implementation but exported in .h file :(

    TInt langCode =keyMapData->LanguageCode();
    if( langCode != 01) //01 is for english
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : LanguageCode for english returned something other than 01"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : LanguageCode for english 01"));
    
    langList.Close();
    delete keyMapData;
    REComSession::FinalClose();
    iLog->Log(_L(" "));
     __UHEAP_MARKEND;
    return errStatus;
    }

// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataTestkeysL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyMapDataTestkeysL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiKeyMapData * keyMapData = static_cast<CPtiKeyMapData*>(GetKeyMapDataL());
    
    if((TInt)EFalse == keyMapData->HasKeyData(EPtiKeyboard12Key))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboard12Key returned EFlase"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboard12Key returned ETrue"));
    
    if((TInt)EFalse == keyMapData->HasKeyData(EPtiKeyboardQwerty4x12))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardQwerty4x12 returned EFlase"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardQwerty4x12 returned ETrue"));
    
    if((TInt)EFalse == keyMapData->HasKeyData(EPtiKeyboardQwerty4x10))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardQwerty4x10 returned EFlase"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardQwerty4x10 returned ETrue"));
    
    if((TInt)EFalse == keyMapData->HasKeyData(EPtiKeyboardQwerty3x11))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardQwerty3x11 returned EFlase"));
        }
    else 
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardQwerty3x11 returned ETrue"));
    
    if((TInt)EFalse == keyMapData->HasKeyData(EPtiKeyboardHalfQwerty))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardHalfQwerty returned EFlase"));
        }
    else 
        iLog->Log(_L("CptiEngTestDomApi : HasKeyData for EPtiKeyboardHalfQwerty returned ETrue"));
    
    TBuf<200> dataforkey1_0(keyMapData->DataForKey(EPtiKeyboard12Key,EPtiKey1,EPtiCaseLower)); 
    if(0 == dataforkey1_0.Length())
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : DataForKey for EPtiKeyboard12Key returned NULL"));
        }
    else 
        iLog->Log(_L("CptiEngTestDomApi : DataForKey for EPtiKeyboard12Key returned valid data"));
    
    TBuf<200> dataforkey1_1(keyMapData->DataForKey(EPtiKeyboard12Key,0)); 
    if(0 == dataforkey1_1.Length())
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : DataForKey for EPtiKeyboard12Key returned NULL"));
        }
    else 
        iLog->Log(_L("CptiEngTestDomApi : DataForKey for EPtiKeyboard12Key returned valid data"));
        
    
    TInt numberOfKeys =keyMapData->NumberOfKeys(EPtiKeyboard12Key);     
    
    if( 12 != numberOfKeys)
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : NumberOfKeys for EPtiKeyboard12Key returned something other than 12 "));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : NumberOfKeys for EPtiKeyboard12Key returned 12 "));
    
    TInt dataSize =0;
    const TUint16* keyData= keyMapData->KeyData(EPtiKeyboard12Key,dataSize);      
    if((dataSize <=0) || (keyData == NULL))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : KeyData for EPtiKeyboard12Key returned NULL"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : KeyData for EPtiKeyboard12Key returned valid pointer"));
    
    TInt numOfItems =0;
    const TPtiKeyBinding* keyBindings = keyMapData->KeyBindingTable(EPtiKeyboard12Key,numOfItems);
    if((numOfItems <=0) || (keyBindings == NULL))
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : KeyBindingTable for EPtiKeyboard12Key returned NULL"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : KeyBindingTable for EPtiKeyboard12Key returned valid pointer"));
    
    delete keyMapData;

    //This part of the code is to just to make sure that below API's are called 
    //KeyBindingTable,KeyData,LanguageCode
    CPtiKeyMapData * keyMapDataDummy = NULL;
    keyMapDataDummy = new CPtiKeyMapData();
    if(keyMapDataDummy)
        {
        TInt dummyItems =0;
        keyMapDataDummy->KeyBindingTable(EPtiKeyboard12Key,dummyItems);
        keyMapDataDummy->KeyData(EPtiKeyboard12Key,dummyItems);
        keyMapDataDummy->LanguageCode();
        }
    delete keyMapDataDummy;
    keyMapDataDummy = NULL;

    REComSession::FinalClose();
    iLog->Log(_L(" "));
     __UHEAP_MARKEND;
    return errStatus;
    }
// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataTestDeadkeysL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyMapDataTestDeadkeysL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiKeyMapData * keyMapData = static_cast<CPtiKeyMapData*>(GetKeyMapDataL());

    if( TInt(ETrue) == keyMapData->IsDeadKey(EPtiKeyboard12Key,EPtiKey1 ,EPtiCaseLower))
        {
        iLog->Log(_L("CptiEngTestDomApi : IsDeadKey for EPtiKeyboard12Key,EPtiKey1,EPtiCaseLower returned ETrue"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : IsDeadKey for EPtiKeyboard12Key,EPtiKey1,EPtiCaseLower returned EFalse"));
    
    TBuf<100> deadKeyData(keyMapData->DeadKeyDataForKey(EPtiKeyboard12Key,EPtiKey1 ,EPtiCaseLower));
    if(deadKeyData.Length() == 0)
        {
        iLog->Log(_L("CptiEngTestDomApi : DeadKeyDataForKey for EPtiKeyboard12Key,EPtiKey1,EPtiCaseLower returned emptystring"));
        }
    else
        iLog->Log(_L("CptiEngTestDomApi : DeadKeyDataForKey for EPtiKeyboard12Key,EPtiKey1,EPtiCaseLower returned valid data"));
   
    delete keyMapData;
    
    //Loop thru all the keymapings for vowel table
    RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
    
    TInt tempVal1=0;
    TInt tempVal2=0;
    
    for(TInt a =0;a<dataImpl.Count();a++)
        {
        CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(TUid::Uid(dataImpl[a]));  
        CleanupStack::PushL(keymapDatafactory);
       
        RArray<TInt> langList;
        keymapDatafactory->ListLanguagesL(langList);
       
        for(TInt b=0;b<langList.Count();b++)
            {
            CPtiKeyMapData * keyMapData =NULL;
            keyMapData = static_cast<CPtiKeyMapData*>(keymapDatafactory->KeyMapDataForLanguageL(langList[b]));
           
            if( TInt(ETrue) == keyMapData->HasDeadKeys(EPtiKeyboard12Key))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasDeadKeys for key board EPtiKeyboard12Key returned ETrue for lang"));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                }
            TInt numOfRows =0;
            const TUint16* deadKeyDataArray1= keyMapData->DeadKeyDataArray(EPtiKeyboard12Key,numOfRows);    
            if(deadKeyDataArray1 != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : DeadKeyDataArray for key board EPtiKeyboard12Key is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForDeadKeys(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if( TInt(ETrue) == keyMapData->HasDeadKeys(EPtiKeyboardQwerty4x12))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasDeadKeys for key board EPtiKeyboardQwerty4x12 returned ETrue for lang"));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                }
            const TUint16* deadKeyDataArray2 = keyMapData->DeadKeyDataArray(EPtiKeyboardQwerty4x12,numOfRows);
            if(deadKeyDataArray2 != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : DeadKeyDataArray for key board EPtiKeyboardQwerty4x12 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForDeadKeys(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if( TInt(ETrue) == keyMapData->HasDeadKeys(EPtiKeyboardQwerty4x10))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasDeadKeys for key board EPtiKeyboardQwerty4x10 returned ETrue for lang"));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                }
            const TUint16* deadKeyDataArray3 = keyMapData->DeadKeyDataArray(EPtiKeyboardQwerty4x10,numOfRows);
            if(deadKeyDataArray3 != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : DeadKeyDataArray for key board EPtiKeyboardQwerty4x10 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForDeadKeys(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if( TInt(ETrue) == keyMapData->HasDeadKeys(EPtiKeyboardQwerty3x11))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasDeadKeys for key board EPtiKeyboardQwerty3x11 returned ETrue for lang"));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                }
            const TUint16* deadKeyDataArray4 = keyMapData->DeadKeyDataArray(EPtiKeyboardQwerty3x11,numOfRows);
            if(deadKeyDataArray4 != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : DeadKeyDataArray for key board EPtiKeyboardQwerty3x11 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForDeadKeys(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if( TInt(ETrue) == keyMapData->HasDeadKeys(EPtiKeyboardHalfQwerty))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasDeadKeys for key board EPtiKeyboardHalfQwerty returned ETrue for lang"));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                }
            const TUint16* deadKeyDataArray5 = keyMapData->DeadKeyDataArray(EPtiKeyboardHalfQwerty,numOfRows);
            if(deadKeyDataArray5 != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : DeadKeyDataArray for key board EPtiKeyboardHalfQwerty is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForDeadKeys(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            delete keyMapData;
            }

        langList.Close();
        CleanupStack::PopAndDestroy(keymapDatafactory);  //keymapDatafactory
        }
   
    dataImpl.Close();
   
    REComSession::FinalClose();
    iLog->Log(_L(" "));
     __UHEAP_MARKEND;
    return errStatus;
    }
void CptiEngTestDomApi::UpdateLogForDeadKeys(TInt aVal,TInt bVal, TInt aLang,TInt &aErrStatus)
    {
    if(aVal != bVal)
        {
        aErrStatus = KErrGeneral;
        TBuf<200> data(_L("CptiEngTestDomApi : HasDeadKeys & DeadKeyDataArray conflict for lang "));
        data.AppendNum(aLang);
        iLog->Log(data);
        }
    }
// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataTestNumerickeysL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyMapDataTestNumerickeysL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;

    //Loop thru all the keymapings for Numeric Key table
    RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
   
    for(TInt a =0;a<dataImpl.Count();a++)
        {
        CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(TUid::Uid(dataImpl[a]));  
        CleanupStack::PushL(keymapDatafactory);
        
        RArray<TInt> langList;
        keymapDatafactory->ListLanguagesL(langList);
        
        for(TInt b=0;b<langList.Count();b++)
            {
            CPtiKeyMapData * keyMapData =NULL;
            keyMapData = static_cast<CPtiKeyMapData*>(keymapDatafactory->KeyMapDataForLanguageL(langList[b]));
            
            RArray<TPtiNumericKeyBinding> numericModeKeys;
            keyMapData->GetNumericModeKeysL(EPtiKeyboard12Key,numericModeKeys); 
            if(numericModeKeys.Count() >0)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : GetNumericModeKeysL for key board EPtiKeyboard12Key returned a valide Numeric Kay Bindings for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);

                for(TInt a=0;a<numericModeKeys.Count();a++)
                    { 
                    if(TInt(ETrue) == keyMapData->IsNumberModeKey(EPtiKeyboard12Key,numericModeKeys[a]))
                        {
                        TBuf<200> data(_L("CptiEngTestDomApi : IsNumberModeKey for key board EPtiKeyboard12Key with key  "));
                        data.AppendNum(numericModeKeys[a].iKey);
                        data.Append(_L(" returned ETrue for lang "));
                        data.AppendNum(langList[b]);
                        iLog->Log(data);
                        }
                    
                    }
                numericModeKeys.Reset();
                } 
            TInt numOfEntries;
            const TPtiNumericKeyBinding* numericKeyBindings = NULL;
            numericKeyBindings = keyMapData->NumericModeKeysTable(EPtiKeyboard12Key,numOfEntries);
            if(numericKeyBindings != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : NumericModeKeysTable for key board EPtiKeyboard12Key is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                }
            
            keyMapData->GetNumericModeKeysL(EPtiKeyboardQwerty4x12,numericModeKeys); 
            if(numericModeKeys.Count() >0)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : GetNumericModeKeysL for key board EPtiKeyboardQwerty4x12 returned a valide Numeric Kay Bindings for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);

                for(TInt a=0;a<numericModeKeys.Count();a++)
                    { 
                    if(TInt(ETrue) == keyMapData->IsNumberModeKey(EPtiKeyboardQwerty4x12,numericModeKeys[a]))
                        {
                        TBuf<200> data(_L("CptiEngTestDomApi : IsNumberModeKey for key board EPtiKeyboardQwerty4x12 with key  "));
                        data.AppendNum(numericModeKeys[a].iKey);
                        data.Append(_L(" returned ETrue for lang "));
                        data.AppendNum(langList[b]);
                        iLog->Log(data);
                        }
                    
                    }
                numericModeKeys.Reset();
                } 
            numericKeyBindings = keyMapData->NumericModeKeysTable(EPtiKeyboardQwerty4x12,numOfEntries);
            if(numericKeyBindings != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : NumericModeKeysTable for key board EPtiKeyboardQwerty4x12 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                }
            
            keyMapData->GetNumericModeKeysL(EPtiKeyboardQwerty4x10,numericModeKeys); 
            if(numericModeKeys.Count() >0)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : GetNumericModeKeysL for key board EPtiKeyboardQwerty4x10 returned a valide Numeric Kay Bindings for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                for(TInt a=0;a<numericModeKeys.Count();a++)
                    { 
                    if(TInt(ETrue) == keyMapData->IsNumberModeKey(EPtiKeyboardQwerty4x10,numericModeKeys[a]))
                        {
                        TBuf<200> data(_L("CptiEngTestDomApi : IsNumberModeKey for key board EPtiKeyboardQwerty4x10 with key  "));
                        data.AppendNum(numericModeKeys[a].iKey);
                        data.Append(_L(" returned ETrue for lang "));
                        data.AppendNum(langList[b]);
                        iLog->Log(data);
                        }
                    
                    }
                numericModeKeys.Reset();
                } 
            numericKeyBindings = keyMapData->NumericModeKeysTable(EPtiKeyboardQwerty4x10,numOfEntries);
            if(numericKeyBindings != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : NumericModeKeysTable for key board EPtiKeyboardQwerty4x10 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                }
            
            keyMapData->GetNumericModeKeysL(EPtiKeyboardQwerty3x11,numericModeKeys); 
            if(numericModeKeys.Count() >0)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : GetNumericModeKeysL for key board EPtiKeyboardQwerty3x11 returned a valide Numeric Kay Bindings for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                
                for(TInt a=0;a<numericModeKeys.Count();a++)
                    { 
                    if(TInt(ETrue) == keyMapData->IsNumberModeKey(EPtiKeyboardQwerty3x11,numericModeKeys[a]))
                        {
                        TBuf<200> data(_L("CptiEngTestDomApi : IsNumberModeKey for key board EPtiKeyboardQwerty3x11 with key  "));
                        data.AppendNum(numericModeKeys[a].iKey);
                        data.Append(_L(" returned ETrue for lang "));
                        data.AppendNum(langList[b]);
                        iLog->Log(data);
                        }
                    
                    }
                numericModeKeys.Reset();
                } 
            numericKeyBindings = keyMapData->NumericModeKeysTable(EPtiKeyboardQwerty3x11,numOfEntries);
            if(numericKeyBindings != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : NumericModeKeysTable for key board EPtiKeyboardQwerty3x11 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                }
            
            keyMapData->GetNumericModeKeysL(EPtiKeyboardHalfQwerty,numericModeKeys); 
            if(numericModeKeys.Count() >0)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : GetNumericModeKeysL for key board EPtiKeyboardHalfQwerty returned a valide Numeric Kay Bindings for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                
                for(TInt a=0;a<numericModeKeys.Count();a++)
                    { 
                    if(TInt(ETrue) == keyMapData->IsNumberModeKey(EPtiKeyboardHalfQwerty,numericModeKeys[a]))
                        {
                        TBuf<200> data(_L("CptiEngTestDomApi : IsNumberModeKey for key board EPtiKeyboardHalfQwerty with key  "));
                        data.AppendNum(numericModeKeys[a].iKey);
                        data.Append(_L(" returned ETrue for lang "));
                        data.AppendNum(langList[b]);
                        iLog->Log(data);
                        }
                    
                    }
                numericModeKeys.Reset();
                } 
            numericKeyBindings = keyMapData->NumericModeKeysTable(EPtiKeyboardHalfQwerty,numOfEntries);
            if(numericKeyBindings != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : NumericModeKeysTable for key board EPtiKeyboardHalfQwerty is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                }
            delete keyMapData;
            numericModeKeys.Close();
            }

        langList.Close();
        CleanupStack::PopAndDestroy(keymapDatafactory);  //keymapDatafactory
        }
    
    dataImpl.Close();
    
    REComSession::FinalClose();
    iLog->Log(_L(" "));
     __UHEAP_MARKEND;
    return errStatus;
    }

// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataTestVowelsL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyMapDataTestVowelsL()
    {    
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
   
    //Loop thru all the keymapings for vowel table
    RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
    TInt tempVal1=0;
    TInt tempVal2=0;
    
    for(TInt a =0;a<dataImpl.Count();a++)
        {
        CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(TUid::Uid(dataImpl[a]));  
        CleanupStack::PushL(keymapDatafactory);
        
        RArray<TInt> langList;
        keymapDatafactory->ListLanguagesL(langList);
        
        for(TInt b=0;b<langList.Count();b++)
            {
            CPtiKeyMapData * keyMapData =NULL;
            keyMapData = static_cast<CPtiKeyMapData*>(keymapDatafactory->KeyMapDataForLanguageL(langList[b]));
            
            if(TInt(ETrue) == keyMapData->HasVowelSequences(EPtiKeyboard12Key))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasVowelSequences for key board EPtiKeyboard12Key returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                } 
            TInt numOfEntries;
            const TVowelSequence* vowelsSequance = NULL;
            vowelsSequance = keyMapData->VowelSequenceTable(EPtiKeyboard12Key,numOfEntries);
            if(vowelsSequance != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : VowelSequenceTable for key board EPtiKeyboard12Key is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForVowels(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if(TInt(ETrue) == keyMapData->HasVowelSequences(EPtiKeyboardQwerty4x12))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasVowelSequences for key board EPtiKeyboardQwerty4x12 returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                } 
            vowelsSequance = keyMapData->VowelSequenceTable(EPtiKeyboardQwerty4x12,numOfEntries);
            if(vowelsSequance != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : VowelSequenceTable for key board EPtiKeyboardQwerty4x12 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForVowels(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if(TInt(ETrue) == keyMapData->HasVowelSequences(EPtiKeyboardQwerty4x10))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasVowelSequences for key board EPtiKeyboardQwerty4x10 returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                } 
            vowelsSequance = keyMapData->VowelSequenceTable(EPtiKeyboardQwerty4x10,numOfEntries);
            if(vowelsSequance != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : VowelSequenceTable for key board EPtiKeyboardQwerty4x10 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForVowels(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if(TInt(ETrue) == keyMapData->HasVowelSequences(EPtiKeyboardQwerty3x11))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasVowelSequences for key board EPtiKeyboardQwerty3x11 returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                } 
            vowelsSequance = keyMapData->VowelSequenceTable(EPtiKeyboardQwerty3x11,numOfEntries);
            if(vowelsSequance != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : VowelSequenceTable for key board EPtiKeyboardQwerty3x11 is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForVowels(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            if(TInt(ETrue) == keyMapData->HasVowelSequences(EPtiKeyboardHalfQwerty))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasVowelSequences for key board EPtiKeyboardHalfQwerty returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal1 =1;
                } 
            vowelsSequance = keyMapData->VowelSequenceTable(EPtiKeyboardHalfQwerty,numOfEntries);
            if(vowelsSequance != NULL)
                {
                TBuf<200> data(_L("CptiEngTestDomApi : VowelSequenceTable for key board EPtiKeyboardHalfQwerty is there for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                tempVal2 =1;
                }
            UpdateLogForVowels(tempVal1,tempVal2,langList[b],errStatus);
            tempVal1=tempVal2=0;
            
            delete keyMapData;
            }

        langList.Close();
        CleanupStack::PopAndDestroy(keymapDatafactory);  //keymapDatafactory
        }
    
    dataImpl.Close();
     
    REComSession::FinalClose();
    iLog->Log(_L(" "));
     __UHEAP_MARKEND;
    return errStatus;
    
    }
void CptiEngTestDomApi::UpdateLogForVowels(TInt aVal,TInt bVal, TInt aLang,TInt &aErrStatus)
    {
    if(aVal != bVal)
        {
        aErrStatus = KErrGeneral;
        TBuf<200> data(_L("CptiEngTestDomApi : HasVowelSequences & VowelSequenceTable conflict for lang "));
        data.AppendNum(aLang);
        iLog->Log(data);
        }
    }
// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataTestOthersL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyMapDataTestOthersL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    CPtiKeyMapData * keyMapData = static_cast<CPtiKeyMapData*>(GetKeyMapDataL());

    //for future use
    keyMapData->Reserved_1();     
    keyMapData->Reserved_2();   
    keyMapData->Reserved_3();     
    keyMapData->Reserved_4();  

    delete keyMapData;
    
    //Loop thru all the keymapings for latin modes
    RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
   
    for(TInt a =0;a<dataImpl.Count();a++)
        {
        CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(TUid::Uid(dataImpl[a]));  
        CleanupStack::PushL(keymapDatafactory);
        
        RArray<TInt> langList;
        keymapDatafactory->ListLanguagesL(langList);
        
        for(TInt b=0;b<langList.Count();b++)
            {
            CPtiKeyMapData * keyMapData =NULL;
            keyMapData = static_cast<CPtiKeyMapData*>(keymapDatafactory->KeyMapDataForLanguageL(langList[b]));
            TBool boolValue1 = keyMapData->SuitableForLatinOnlyMode();
            if((TInt)boolValue1 == EFalse )
                {
                TBuf<200> data(_L("CptiEngTestDomApi : SuitableForLatinOnlyMode returned EFalse for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                }
            else
                {
                TBuf<200> data(_L("CptiEngTestDomApi : SuitableForLatinOnlyMode returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                }
            
            if(TInt(ETrue) == keyMapData->HasFnKeyBindings(EPtiKeyboard12Key))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasFnKeyBindings for key board EPtiKeyboard12Key returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                } 
            if(TInt(ETrue) == keyMapData->HasFnKeyBindings(EPtiKeyboardQwerty4x12))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasFnKeyBindings for key board EPtiKeyboardQwerty4x12 returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                } 
            if(TInt(ETrue) == keyMapData->HasFnKeyBindings(EPtiKeyboardQwerty4x10))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasFnKeyBindings for key board EPtiKeyboardQwerty4x10 returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                } 
            if(TInt(ETrue) == keyMapData->HasFnKeyBindings(EPtiKeyboardQwerty3x11))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasFnKeyBindings for key board EPtiKeyboardQwerty3x11 returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                } 
            if(TInt(ETrue) == keyMapData->HasFnKeyBindings(EPtiKeyboardHalfQwerty))
                {
                TBuf<200> data(_L("CptiEngTestDomApi : HasFnKeyBindings for key board EPtiKeyboardHalfQwerty returned ETrue for lang "));
                data.AppendNum(langList[b]);
                iLog->Log(data);
                } 

            delete keyMapData;
            }

        langList.Close();
        CleanupStack::PopAndDestroy(keymapDatafactory);  //keymapDatafactory
        }
    
    dataImpl.Close();

    REComSession::FinalClose();
    iLog->Log(_L(" "));
     __UHEAP_MARKEND;
    return errStatus;
    }
// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyMapDataTestDataFactoryL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyMapDataFactoryL()
    {    
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    
    RArray<TInt> implList;
    
    TRAP(errStatus,CPtiKeyMapDataFactory::ListImplementationsL(implList));
    if(errStatus != KErrNone)
        {
        iLog->Log(_L("CptiEngTestDomApi : ListImplementationsL leaved"));
        }
    
    if(implList.Count() > 0)
        {
        CPtiKeyMapDataFactory* dataFactory = NULL;
        TRAP(errStatus, dataFactory = CPtiKeyMapDataFactory::CreateImplementationL(TUid::Uid(implList[0])));
        if(errStatus != KErrNone)
            {
            iLog->Log(_L("CptiEngTestDomApi : CPtiKeyMapDataFactory leaved"));
            }
        else
            delete dataFactory;
        }
    else
        {
        errStatus = KErrGeneral;
        iLog->Log(_L("CptiEngTestDomApi : ListImplementationsL returned <0"));
        }
    
    implList.Close();
    
    class tempclass :public CPtiKeyMapDataFactory
        {
    public:
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode) 
            {
            return NULL; // Just return NULL
            }
        void ListLanguagesL(RArray<TInt>& aResult)
            {
            //Empty
            }
        };
    CPtiKeyMapDataFactory * dataFactory = new tempclass();
   
    dataFactory->Reserved_1();     
    dataFactory->Reserved_2();
    
    delete dataFactory;
    
    REComSession::FinalClose();
    iLog->Log(_L(" "));

     __UHEAP_MARKEND;
    return errStatus;

    }
// -----------------------------------------------------------------------------
// CptiEngTestDomApi::PtiTestKeyboardDatabaseFactoryL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CptiEngTestDomApi::PtiTestKeyboardDatabaseFactoryL()
    {
    __UHEAP_MARK;
    TInt errStatus = KErrNone;
    iLog->Log(_L("CptiEngTestDomApi : CPtiKeyboardDatabase is very specific to XT9 engine"));
    
    CPtiKeyboardDatabaseFactory * dataFactory = NULL;
    RArray<TPtiKeyboardDatabaseMappingOpaque> result;
    CleanupClosePushL(result);   

    RArray<TInt> implResult;
    CleanupClosePushL(implResult);    
    CPtiKeyboardDatabaseFactory::ListImplementationsL(0x2000BEFF,implResult); //0x2000BEFF is interface uid for T9 keyboard plugins
    if(implResult.Count() > 0)
        {
        iLog->Log(_L("CptiEngTestDomApi : PtiTestKeyboardDatabaseFactoryL returned a valid list of imples"));
        CPtiKeyboardDatabaseFactory::CreateMappingTableWithOpaqueL(0x2000BEFF,result);
        if(result.Count() >0 )
            {
            iLog->Log(_L("CptiEngTestDomApi : CreateMappingTableWithOpaqueL returned a valid list of TPtiKeyboardDatabaseMappingOpaque"));
            }    
        }
    CleanupStack::PopAndDestroy(); //implResult
    
    for(TInt i=0;i<result.Count();i++)
        {
        dataFactory = CPtiKeyboardDatabaseFactory::CreateImplementationL(TUid::Uid(result[i].iUid));
        if (!dataFactory)
            {
            errStatus = KErrGeneral;
            break;
            }
        CleanupStack::PushL(dataFactory);
        //for future use
        dataFactory->Reserved_1();     
        dataFactory->Reserved_2();               

        TBuf<200> data(_L("CptiEngTestDomApi : CreateImplementationL returned a valide pointer to CPtiKeyboardDatabaseFactory for uid "));
        data.AppendNum(result[i].iUid);
        iLog->Log(data);

        RArray<TInt> langList;
        CleanupClosePushL(langList);
        dataFactory->ListLanguagesL(langList);
        TBool langFound = EFalse;
        for(TInt j=0;j<langList.Count();j++)
            {
            CPtiKeyboardDatabase* keyboardDatabase = NULL;
            keyboardDatabase = static_cast<CPtiKeyboardDatabase*>(dataFactory->KeyMapDataForLanguageL(langList[j]));
            if (!keyboardDatabase)
                {
                delete dataFactory;
                errStatus = KErrGeneral;
                break;
                }

            keyboardDatabase->Reserved_1();     
            keyboardDatabase->Reserved_2(); 

            if (keyboardDatabase->LanguageCode() == langList[j])
                {
                langFound = ETrue;     
                }

            if(keyboardDatabase->NativeId() <= 0)
                {
                errStatus = KErrGeneral;
                }

            // Not trying to interpreat KbdData as it would not be understable
            keyboardDatabase->KdbData(0, NULL);

            delete keyboardDatabase;
            keyboardDatabase = NULL;
            }
        
        CleanupStack::PopAndDestroy(); //langList
        if (!langFound)
            {
            errStatus = KErrGeneral;
            }
        
        CleanupStack::PopAndDestroy(); //dataFactory
        }
    CleanupStack::PopAndDestroy(); //result
    
    REComSession::FinalClose();
    iLog->Log(_L(" "));
    __UHEAP_MARKEND;
    return errStatus;
    }
// This function return CKeyMapData for english language
MPtiKeyMapData * CptiEngTestDomApi::GetKeyMapDataL()
    {
    RArray<TInt> dataImpl;
    CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
    
    TUid uid =  TUid::Uid(dataImpl[0]);
    CPtiKeyMapDataFactory * keymapDatafactory = CPtiKeyMapDataFactory::CreateImplementationL(uid);  
    CleanupStack::PushL(keymapDatafactory);
   
    dataImpl.Close();
   
    MPtiKeyMapData * keyMapData =NULL;
    keyMapData = keymapDatafactory->KeyMapDataForLanguageL(01); //English
    
    if(NULL == keyMapData)
        User::Leave(KErrGeneral); //This leave fails all the testcases that are calling this function
   
    CleanupStack::PopAndDestroy(keymapDatafactory);  //keymapDatafactory
    return  keyMapData;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
