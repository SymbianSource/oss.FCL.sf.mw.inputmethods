// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Tests for the Combination of key controls which 
// is used to Turn on and Off the Fep Using the CFepGenericGlobalSettings API.
// 
//

/**
 @file
 @test 
 @internalComponent - Internal Symbian test code
*/

#include <e32std.h>
#include <coeaui.h>
#include <fepbase.h>
#include <coemain.h>
#include "TFepGen.h"

// global variables
TBool gIsOn = ETrue;
TFepOnOrOffKeyData	gOnKey(0,0,0);
TFepOnOrOffKeyData	gOffKey(0,0,0);	


/*************************************************************
 **  
 **  CFepGenAppUi
 **  
 *************************************************************/

CFepGenAppUi::CFepGenAppUi(CTmsTestStep* aStep) :
  	CTestCoeAppUi(aStep)
	{
  	}

CFepGenAppUi::~CFepGenAppUi()
    {
    }

void CFepGenAppUi::ConstructL()
    {
    CTestCoeAppUi::ConstructL();
    TRect rect(TPoint(40,40),TSize(560,160));
    AutoTestManager().StartAutoTest();
    }

void CFepGenAppUi::RunTestStepL(TInt aStepNum)
	{
	switch(aStepNum)
		{	 
		case 1:
			{
			// Need to save original settings
			TRAPD(err, StoreOriginalSettingsL());
			TEST(err == KErrNone);
			}
			break;
		case 2:
			{
			SetTestStepID(_L("UIF-FEPBASE-0001"));
			TRAPD(err, CreateFepGenericL());
			TEST(err == KErrNone);
			RecordTestResultL();
			}
			break;
		case 3:
			{
			SetTestStepID(_L("UIF-FEPBASE-0002"));
			TRAPD(err, FepStoreAndBroadCastL());
			TEST(err == KErrNone);
			RecordTestResultL();
			CloseTMSGraphicsStep();
			}
			break;
		case 4:
			{
			// Need to get original settings
			TRAPD(err, RetrieveOriginalSettingsL());
			TEST(err == KErrNone);
			}
			break;
		case 5:
			AutoTestManager().FinishAllTestCases(CAutoTestManager::EPass);
			break;
		}
	}
	

void CFepGenAppUi::StoreOriginalSettingsL()
	{
	CFepGenericGlobalSettings *fep=CFepGenericGlobalSettings::NewL();
	CleanupStack::PushL(fep);

	gIsOn = fep->IsOn();
	gOnKey = fep->OnKeyData();
	gOffKey = fep->OffKeyData();

	CleanupStack::PopAndDestroy(fep);	
	}
	
	
void CFepGenAppUi::RetrieveOriginalSettingsL()
	{
	CFepGenericGlobalSettings *fep=CFepGenericGlobalSettings::NewL();
	CleanupStack::PushL(fep);

	fep->SetIsOn(gIsOn);
	fep->SetOnKeyData(gOnKey);
	fep->SetOffKeyData(gOffKey);

	fep->StoreChangesAndBroadcastL();

	CleanupStack::PopAndDestroy(fep);		
	}
	
/**
   @SYMTestCaseID UIF-FEPBASE-0001
  
   @SYMDEF    DEF083097
  
   @SYMTestCaseDesc   This Test case is used to test the functionality of the 
   CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) 
   and CFepGenericGlobalSettings::NewL () .The functionality of Set API s of the class CFepGenericGlobalSettings is also tested which is used to overide system parameters for fep.
   @SYMTestPriority High 
  
   @SYMTestStatus Implemented
  
       
   @SYMTestActions   This methods creates Objects of CFepGenericGlobalSettings using CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) and 
   CFepGenericGlobalSettings::NewL () . Checks whether the attributes of the two objects are same.It then calls the Set methods of CFepGenericGlobalSettings namely SetIsOn();SetOnkeyData(), SetOffkeyData(),with one of the objects created by   
   CFepGenericGlobalSettings using CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) method and checks that the attributes of the object created using 
   CFepGenericGlobalSettings::NewL () are not the same .

       
   @SYMTestExpectedResults   The test case fails when the attributes of the objects created using CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) and 
   CFepGenericGlobalSettings::NewL () are not the same.
   When the SET methods are called using the CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) method's object.The attributes get changed.
   And the attributes are not equal to the object created using the CFepGenericGlobalSettings::NewL ().The test case fails if they are identical.

*/
void CFepGenAppUi::CreateFepGenericL()
    {
    INFO_PRINTF1(_L("Executing Test case UIF_FEPBASE_001"));

   	const TFepOnOrOffKeyData genKeyData1(EKeySpace,EModifierFunc|EModifierLeftShift ,EModifierFunc);
	TEST(genKeyData1.ModifierMask()==(EModifierFunc|EModifierLeftShift));
	TEST(genKeyData1.ModifierValues()==EModifierFunc);

	const TFepOnOrOffKeyData genKeyData2(EKeyDelete,EModifierFunc|EModifierRightShift,EModifierFunc) ;
	TEST(genKeyData2.ModifierMask()==(EModifierFunc|EModifierRightShift));
	TEST(genKeyData2.ModifierValues()==EModifierFunc);

	CCoeEnv* env = CCoeEnv::Static();
	CFepGenericGlobalSettings *fepWithParam=CFepGenericGlobalSettings::NewL(*env,genKeyData1,genKeyData2,ETrue);
	CleanupStack::PushL(fepWithParam);

	CFepGenericGlobalSettings *fepNoParam=CFepGenericGlobalSettings::NewL();
	CleanupStack::PushL(fepNoParam);


	const TFepOnOrOffKeyData keyOnFepPar=fepWithParam->OnKeyData();
	const TFepOnOrOffKeyData keyOnFepNoPar=fepNoParam->OnKeyData();
	const TFepOnOrOffKeyData keyOffFepPar=fepWithParam->OffKeyData();
	const TFepOnOrOffKeyData keyOffFepNoPar=fepNoParam->OffKeyData();
	
	INFO_PRINTF2(_L("val of keyOnFepPar.ModifierMask() '%d' "),keyOnFepPar.ModifierMask());
	INFO_PRINTF2(_L("val of keyOnFepNoPar.ModifierMask() '%d' "),keyOnFepNoPar.ModifierMask());
	INFO_PRINTF2(_L("val of keyOnFepPar.ModifierValues() '%d' "),keyOnFepPar.ModifierValues());
	INFO_PRINTF2(_L("val of keyOnFepNoPar.ModifierValues() '%d' "),keyOnFepNoPar.ModifierValues());
	INFO_PRINTF2(_L("val of keyOnFepPar.CharacterCodeForFoldedMatch() '%d' "),keyOnFepPar.CharacterCodeForFoldedMatch());
	INFO_PRINTF2(_L("val of keyOnFepNoPar.CharacterCodeForFoldedMatch() '%d' "),keyOnFepNoPar.CharacterCodeForFoldedMatch());

	
	INFO_PRINTF2(_L("val of keyOffFepPar.ModifierMask() '%d' "),keyOffFepPar.ModifierMask());
	INFO_PRINTF2(_L("val of keyOffFepNoPar.ModifierMask() '%d' "),keyOffFepNoPar.ModifierMask());
	INFO_PRINTF2(_L("val of keyOffFepPar.ModifierValues() '%d' "),keyOffFepPar.ModifierValues());
	INFO_PRINTF2(_L("val of keyOffFepNoPar.ModifierValues() '%d' "),keyOffFepNoPar.ModifierValues());
	INFO_PRINTF2(_L("val of keyOffFepPar.CharacterCodeForFoldedMatch() '%d' "),keyOffFepPar.CharacterCodeForFoldedMatch());
	INFO_PRINTF2(_L("val of keyOffFepNoPar.CharacterCodeForFoldedMatch() '%d' "),keyOffFepNoPar.CharacterCodeForFoldedMatch());
	
	TEST(fepWithParam->IsOn()==fepNoParam->IsOn());
	
	TEST(keyOnFepPar==keyOnFepNoPar);
	TEST(genKeyData1!=keyOnFepPar);
	TEST(genKeyData1!=keyOnFepNoPar);
	
	TEST(keyOffFepPar==keyOffFepNoPar);
	TEST(genKeyData2!=keyOffFepPar);
	TEST(genKeyData2!=keyOffFepNoPar);

	if(fepWithParam->IsOn())
	{
	INFO_PRINTF2(_L("fepWithParam->IsOn() is ETRUE '%d' "),fepWithParam->IsOn());
	fepWithParam->SetIsOn(EFalse);
	TEST(!(fepWithParam->IsOn()));
	INFO_PRINTF2(_L("fepWithParam->IsOn() Was ETRUE and Now It is changed to '%d' "),fepWithParam->IsOn());
	}
	else
	{
	INFO_PRINTF2(_L("fepWithParam->IsOn() is EFalse '%d' "),fepWithParam->IsOn());
	fepWithParam->SetIsOn(ETrue);
	TEST(fepWithParam->IsOn());
	INFO_PRINTF2(_L("fepWithParam->IsOn() was EFalse and now changed to '%d' "),fepWithParam->IsOn());
	}

	fepWithParam->SetOnKeyData(genKeyData1);
	fepWithParam->SetOffKeyData(genKeyData2);
	
	
	TEST(genKeyData1==fepWithParam->OnKeyData());
	TEST(genKeyData2==fepWithParam->OffKeyData());
	TEST(fepNoParam->IsOn()!=fepWithParam->IsOn());
	TEST(genKeyData1!=fepNoParam->OnKeyData());
	TEST(genKeyData2!=fepNoParam->OffKeyData());
	
	// After the test, we should restore original values...
	fepWithParam->SetOnKeyData(genKeyData1);
	fepWithParam->SetOffKeyData(genKeyData2);
	
	
	CleanupStack::PopAndDestroy(2,fepWithParam);
	INFO_PRINTF1(_L("End of  Test case 1"));
    }
    
/**
   @SYMTestCaseID UIF-FEPBASE-0002
  
   @SYMDEF    DEF083097
  
   @SYMTestCaseDesc		This test case tests the functionality of StoreAndBroadcastL() method of CFepGenericFlobalSettings().

   @SYMTestPriority High 
  
   @SYMTestStatus Implemented  
       
   @SYMTestActions 	This methods creates Objects of CFepGenericGlobalSettings using CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) and
   CFepGenericGlobalSettings::NewL () . It then calls the Set methods of CFepGenericGlobalSettings namely SetIsOn();SetOnkeyData(), SetOffkeyData(),with one of the objects created by   
   CFepGenericGlobalSettings using CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) method and ,Then it calls the StoreAndBroadcastL() and checks that the attributes of the object created using 
   CFepGenericGlobalSettings::NewL () are the same .
   	 
   @SYMTestExpectedResults 	When the SET methods are called using the CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment,const TFepOnOrOffKeyData& aDefaultOnKeyData,const TFepOnOrOffKeyData& aDefaultOffKeyData,TBool aDefaultIsOn) method's object.The attributes get changed.
   StoreAndBroadCastL(), Stores the local changes to the systemSettings and distributes the changes to all the instances of FEP.So when a new object is created Using the NewL() method , the attributes should be same as that of the previuosly created object 's attributes.
*/
    
void CFepGenAppUi::FepStoreAndBroadCastL()
    {
    INFO_PRINTF1(_L("Executing Test case UIF_FEPBASE_0002"));
	const  TFepOnOrOffKeyData onKeyData1(EKeySpace,EModifierFunc|EModifierLeftShift ,EModifierFunc);
	TEST(onKeyData1.ModifierMask()==(EModifierFunc|EModifierLeftShift));
	TEST(onKeyData1.ModifierValues()==EModifierFunc);
	const TFepOnOrOffKeyData offKeyData1(EKeyDelete,EModifierFunc|EModifierRightShift,EModifierFunc) ;
	TEST(offKeyData1.ModifierMask()==(EModifierFunc|EModifierRightShift));
	TEST(offKeyData1.ModifierValues()==EModifierFunc);
	
	const  TFepOnOrOffKeyData testOnKeyData1(EKeyEnter,EModifierFunc|EModifierLeftShift ,EModifierFunc);
	const  TFepOnOrOffKeyData testOffKeyData1(EKeyTab,EModifierFunc|EModifierLeftShift ,EModifierFunc);

	CCoeEnv* env = CCoeEnv::Static();
	CFepGenericGlobalSettings *fepGen1=	CFepGenericGlobalSettings::NewL(*env,onKeyData1,offKeyData1,ETrue);
	CleanupStack::PushL(fepGen1);
	
	fepGen1->SetOffKeyData(testOffKeyData1);
	fepGen1->SetOnKeyData(testOnKeyData1);
	fepGen1->SetIsOn(EFalse);
	fepGen1->StoreChangesAndBroadcastL();
	CleanupStack::PopAndDestroy(fepGen1);
	
	CFepGenericGlobalSettings *fepGen2=CFepGenericGlobalSettings::NewL();
	CleanupStack::PushL(fepGen2);
	TFepOnOrOffKeyData testOnKeyData2=fepGen2->OnKeyData();
	TFepOnOrOffKeyData testOffKeyData2=fepGen2->OffKeyData();
	
	TEST(!(fepGen2->IsOn()));
	TEST(testOnKeyData2!=onKeyData1);
	TEST(testOnKeyData2==testOnKeyData1);
	TEST(testOffKeyData2!=offKeyData1);
	TEST(testOffKeyData2==testOffKeyData1);
	
	CleanupStack::PopAndDestroy(fepGen2);
	INFO_PRINTF1(_L("End of Test case 2")); 
    }


/*************************************************************
 **  
 **  CTFepGenStep
 **  
 *************************************************************/

/**
  Completes the construction of the Control Environment(CCoeEnv object).\n
  Instantiates the CFepGenAppUi class which serves as a AppUi class.\n
  Sets the CFepGenAppUi object as the application's user interface object.\n
  Invokes the second phase constructor of the application's UI.\n
*/
void CTFepGenStep::ConstructFepGenAppL(CCoeEnv* aCoe)
	{
	aCoe->ConstructL();
	CFepGenAppUi* appUi=new(ELeave) CFepGenAppUi(this);
	aCoe->SetAppUi(appUi);
	appUi->ConstructL();
	}

CTFepGenStep::CTFepGenStep()
	{
	SetTestStepName(KTFepGen);
	}	

CTFepGenStep::~CTFepGenStep()
	{
	}
	
TVerdict CTFepGenStep::doTestStepL()
	{
    INFO_PRINTF1(_L("APPFWK_FEPBASE:FepGeneric Started"));
    
    __UHEAP_MARK;
    
	CCoeEnv* coe=new(ELeave) CCoeEnv;
	TEST(coe !=NULL);
	if(coe==NULL)
		{
		INFO_PRINTF1(_L("Failed to create CCoeEnv "));
 		return TestStepResult();
		}
		
    TRAPD(err,ConstructFepGenAppL(coe));
    if (!err)
        coe->ExecuteD();
	else
		{
		INFO_PRINTF2(_L("Failed to Construct CCoeEnv ,error %d"),err);
		SetTestStepResult(EFail);
 		coe->PrepareToExit();
		coe->DestroyEnvironment();
		delete coe;
		return TestStepResult();
		}
	
	__UHEAP_MARKEND;

	INFO_PRINTF1(_L("Test Finished"));

	return TestStepResult();
	}

	
    
  
 


    





