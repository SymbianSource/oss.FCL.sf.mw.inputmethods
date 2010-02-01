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
// @file
// @test
// @internalComponent - Internal Symbian test code
// This is a FEP crash test.\n
// Launch the application and load the Morse FEP (TAPPMORSE.FEP).\n
// Create the stream code using character array.\n Offer these simulated events to the application.\n
// FEP intercepts these events. On success the application exits without a crash.\n  
// ___Install_Fep	// Define this if you want to use an extern fep
// 
//

      
#include <eikdef.h>
#include <eikenv.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <eikappui.h>
#include <uikon.hrh>
#include <w32std.h>
#include <fepbase.h>

#include "TAppMorse.h"

//
//	Forward class declarations
//
class CTAppMorseAppView;
class CTAppMorseAppUi;
class CTAppMorseDocument;
class CTAppMorseApplication;

//
//	Constant definitions
//

//const TUid KUidTAppMorse				= { 0x01000001 };

//Character codes to create the stream 
const TUint KFirstTestStreamCode	= 'A';
const TUint KListTestStreamCode		= 'Z';
const TUint KSizeTestStream			= KListTestStreamCode - KFirstTestStreamCode;
_LIT(KTAPPMORSEResourceFilePath,""/*"z:\\system\\data\\tappmorse.rsc"*/);

#ifdef ___Install_Fep 	// Define this if you want to use an extern fep
_LIT(KLitFepMorseDllName, "\\system\\fep\\TAPPMORSE.FEP");
const TUid KUidTAppMorseFep = { 0x11111111 };
#else					// Not defined, to use the internal fep
	
	
//
//	CStackFaultTestFep members definitions


/**
	Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
	Return a fully constructed object of CCoeFep\n

*/
CCoeFep* CStackFaultTestFep::NewFepL( CCoeEnv& aConeEnvironment, const CCoeFepParameters& aFepParameters )
	{
	CStackFaultTestFep* const fep=new(ELeave) CStackFaultTestFep(aConeEnvironment);
	CleanupStack::PushL(fep);
	fep->ConstructL(aFepParameters);
	CleanupStack::Pop(); // fep
	return fep;
	}
/**
  
  Constructor
  
*/
CStackFaultTestFep::CStackFaultTestFep(CCoeEnv& aConeEnvironment)
	:CCoeFep(aConeEnvironment)
	{
	}
/**
  Auxiliary Function for  Test Case ID TAppMorse-doTestStepL.\n
  Initialise the FEP's generic settings\n
  Sets this FEP's attributes with values from the global settings\n
  Create a stream (character array)\n
  Simulates key event for each of the character codes in the array passed to it\n
  
*/
void CStackFaultTestFep::ConstructL(const CCoeFepParameters& aFepParameters)
	{
	BaseConstructL(aFepParameters);
	ReadAllAttributesL();
	
	//Set stream to owner.  Stream will be rubbish unless Morse test app had focus
	RArray<TUint> aChars;
	for( TUint i=0; i<=KSizeTestStream; i++ )
		aChars.Insert( i+KFirstTestStreamCode, i );
	SimulateKeyEventsL( aChars.Array() );
	aChars.Close();
	}
#endif


//
//	CTAppMorseAppView members definition


/**
 Auxiliary Function for  Test Case ID TAppMorse-doTestStepL.\n
 Initialise a pointer to CTAppMorseAppView and return it\n

*/
CTAppMorseAppView* CTAppMorseAppView::NewL(const TRect& aRect, CTmsTestStep* aStep )
	{

	CTAppMorseAppView* self = new(ELeave) CTAppMorseAppView(aStep);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	};
/**
   Constructor \n
  
 */	
CTAppMorseAppView::CTAppMorseAppView(CTmsTestStep* aStep)
	: iStop( NULL )
	, iTestFep( NULL )
	, iNextValidScanCode( KFirstTestStreamCode )
	, iStep(aStep)
	{INFO_PRINTF1(_L("AppMorse App View Default Constructor"));
	}
/**
  Auxiliary Function for  Test Case ID TAppMorse-doTestStepL.\n
  Creates a control's window \n
  Set the control's extent and as ready to be drawn \n
  Initialize the active object \n
  Install the FEP, if an external fep is to be used,else use internal FEP \n
  Create a fully constructed object of CStackFaultTestFep class \n
  
*/	
void CTAppMorseAppView::ConstructL( const TRect& aRect )
	{
	INFO_PRINTF1(_L("App Mosre App View Construct L"));
	CreateWindowL();
	SetRect(aRect);
	ActivateL();
	iStop = new(ELeave) CTAppMorseAppView::CStop( iEikonEnv );
#ifdef ___Install_Fep	// Define this if you what to use an extern fep
	iCoeEnv->InstallFepL(KUidTAppMorseFep);
#else					// Not defined to use the internal fep
	CCoeFepParameters* pFepParams = NULL; // this is passed into a reference parameter - this pointer should never be dereferenced
	iTestFep = CStackFaultTestFep::NewFepL(*ControlEnv(), *pFepParams );
#endif	
	};
/**
  
   Destructor \n
   delete the FEP object and the active object\n
  
 */	
CTAppMorseAppView::~CTAppMorseAppView()
	{
	delete iStop;
	delete iTestFep;
	}
/**
   Auxiliary Function for  Test Case ID TAppMorse-doTestStepL.\n
   Send the key events to CCoeControl::OfferKeyEventL() to handle \n
  
 */	
TKeyResponse CTAppMorseAppView::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	INFO_PRINTF3(_L("OfferKeyeventL, EventCode %d , EventCode %d"), aKeyEvent.iCode, aType);
	if( aKeyEvent.iCode != iNextValidScanCode++ )
		{
		iNextValidScanCode = KFirstTestStreamCode;
		}
	else
		{
		if( iNextValidScanCode > KListTestStreamCode )
			{
			DoTestL();
			};
		};
	return CCoeControl::OfferKeyEventL(aKeyEvent,aType);
	};
/**
  Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
  Install the external FEP, if to be used.\n
  Call DoStop() of CStop \n
   
*/
void CTAppMorseAppView::DoTestL()
	{
	INFO_PRINTF1(_L("Executing Test"));
#ifdef ___Install_Fep	// Define this if you what to use an extern fep
	iCoeEnv->InstallFepL(KNullUid);
	iStop->DoStop();
	TRAPD(exitCondition,CActiveScheduler::Start());	
	if (exitCondition!=KLeaveExit)
		User::Leave(exitCondition);
#else					// Not defined to use the internal fep
	delete iTestFep;
	iTestFep = NULL;
#endif
	iStop->DoStop();
	}

//	CTAppMorseAppView::CStop members definition


/**
  
  Constructor \n
  Adds the active object to the current active scheduler \n
 
*/
CTAppMorseAppView::CStop::CStop( CEikonEnv* env ) 
	: CActive(EPriorityLow)
	, iEikEnv( env )
	{
	CActiveScheduler::Add(this);
	}
/**
  Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
  Indicate the completion status of a request made to a service provider \n
 		
  Signal the current thread that the asynchronous request associated \n
  with the specified request status object is complete (RequestComplete) \n
 
  Indicates that the active object has issued a request \n
  and that it is now outstanding(Set Active) \n
  
*/			
void CTAppMorseAppView::CStop::DoStop()
	{
	TRequestStatus* requestStatus=&iStatus;
	User::RequestComplete(requestStatus, KErrNone);
	SetActive();
	}
/**
 Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
 Handles any leave occurring in the request completion event(RunL) \n
 
*/			
TInt CTAppMorseAppView::CStop::RunError(TInt)
	{
	return KLeaveExit;
	}
/**
 Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
 Do nothing \n
 
*/		
void CTAppMorseAppView::CStop::DoCancel()
	{
	};
/**
  Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
  Handle active object's request completion event \n
 
*/			
void CTAppMorseAppView::CStop::RunL()
	{
	//Uncomment the following line if tester wants active confirmation success
	//iEikEnv->AlertWin( _L( "No Defect. Press Continue to exit" ) );
	static_cast< CTAppMorseAppUi* >( iEikEnv->AppUi() )->DoExit();
	};

//	CTAppMorseAppUi members definition

/**

 Constructor \n

*/
CTAppMorseAppUi::CTAppMorseAppUi(CTmsTestStep* aStep)
	: CTestAppUi(aStep,KTAPPMORSEResourceFilePath), iAppView( NULL )
	{INFO_PRINTF1(_L("App Morse default constructor"));
	}
/**
   Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
   Inserts a control into the app UI's control stack (AddToStackL) \n
   Call StartAutoTest() of AutoTestManager() \n
    
 */	
void CTAppMorseAppUi::ConstructL(CTmsTestStep* aStep)
	{
	INFO_PRINTF1(_L("AppMoresAppUI ContructL"));
	CTestAppUi::ConstructL();
	iAppView = CTAppMorseAppView::NewL(ClientRect(),aStep);
	AddToStackL( iAppView );
	AutoTestManager().StartAutoTest();
	};
/**
   
   Destructor \n
   Removes the control from the app UI's control stack \n
    
 */
CTAppMorseAppUi::~CTAppMorseAppUi()
	{
	RemoveFromStack( iAppView );
	delete iAppView;
	};
/**
  
  Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
  Do nothing for step number value 1\n
 
*/	
void CTAppMorseAppUi::RunTestStepL(TInt aStepNum)
	{
	switch (aStepNum)
		{
		case 1:
			{
			}
		}
	}
/**
  
  Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
  Handle the Application exit command by calling the DoExit() \n
 
*/
void CTAppMorseAppUi::HandleCommandL(TInt aCommand)
	{
	switch(aCommand)
		{
		case EEikCmdExit:
			DoExit();
			break;
		}
	};
/**
  
  Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
  Close the app UI \n
 
*/	
void CTAppMorseAppUi::DoExit()
	{
	Exit();
	};
/**
  
  Auxiliary Function for Test Case ID TAppMorse-doTestStepL.\n
  Call ConstructL() of CEikonEnv and initialize a pointer to CTAppMorseAppUi \n
  Call ConstructL() of CTAppMorseAppUi and set the application's user interface object \n
 
*/
void CTAppMorse::ConstructAppL(CEikonEnv* aEikEnv)
	{
	INFO_PRINTF1(_L("CTAppMorse::ConstructAppL"));
	aEikEnv->ConstructL();
 	CTAppMorseAppUi* appUi=new(ELeave) CTAppMorseAppUi(this);
 	appUi->ConstructL(this);
	CleanupStack::PushL(appUi);
 	aEikEnv->SetAppUi(appUi);
	CleanupStack::Pop();
	}
	

CTAppMorse::~CTAppMorse()
/**
  Destructor
 */
	{
	}

CTAppMorse::CTAppMorse()
/**
  Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTAppMorse);
	}
/**
   @SYMTestCaseID UIF-TAppMorse-doTestStepL
  
   @SYMPREQ
  
   @SYMTestCaseDesc  Simply launch the application and offer some key events.\n
   The test is dependend on the TFepMorse FEP being installed. \n
   
   @SYMTestPriority High 
  
   @SYMTestStatus Implemented
   
   @SYMTestActions Provide an active environment for creating controls and utility functions \n
   Launches the application using ExecuteD(). Simulate key events and offer them to the 
   application
  
   @SYMTestExpectedResults The test case fails if the application crashed with an exception, due to FEP not being able to intercept the event codes. \n 
   and the test passes if the application exits cleanly. 
    
 */
TVerdict CTAppMorse::doTestStepL()
	{
	SetTestStepID(_L("UIF-TAppMorse-doTestStepL"));
	INFO_PRINTF1(_L("Test Started"));
	
	CEikonEnv* EikEnv=new CEikonEnv;
	if (EikEnv==NULL)
		{	
			SetTestStepResult(EFail);
			return TestStepResult();
		}
	TRAPD(error, ConstructAppL(EikEnv));
	if (error!=KErrNone)
		{
		delete EikEnv;
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	EikEnv->ExecuteD();
	
	INFO_PRINTF1(_L("Test Finished"));
	RecordTestResultL();
	CloseTMSGraphicsStep();
	return TestStepResult();
	}
	

//	End of file TAppMorse.cpp

