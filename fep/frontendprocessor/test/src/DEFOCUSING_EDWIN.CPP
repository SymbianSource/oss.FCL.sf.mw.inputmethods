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
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/
#include <e32std.h>
#include <e32base.h>
#include <coemain.h>
#include <coeaui.h>
#include <gulbordr.h>
#include <techview/eikedwin.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <eikenv.h>
#include <ecom/ecom.h>

#include "DEFOCUSING_EDWIN.h"

_LIT(KLitInitialText, "Enter some inline text (with TFEP1) here and leave it uncommitted when the timer expires: ");

const TInt	KNumberOfSeconds = 5;


/*************************************************************
 **  
 **  CTestTimer
 **  
 *************************************************************/

/**
  Application exists as long as the timer .\n 
*/

CTestTimer* CTestTimer::NewL(TInt aDurationInSeconds)
	{
	CTestTimer* timer=new(ELeave) CTestTimer(aDurationInSeconds);
	CleanupStack::PushL(timer);
	timer->ConstructL();
	timer->QueueAndInfoPrint();
	
	//CActiveScheduler::Start();
	CleanupStack::Pop(timer);
	return timer;
	}


CTestTimer::CTestTimer(TInt aDurationInSeconds)
	:CTimer(EActivePriorityWsEvents+1),
	 iSecondsRemaining(aDurationInSeconds)
	{
	CActiveScheduler::Add(this);
	}

/**  
   Format the timer message to be send to the appliation window.
*/

void CTestTimer::QueueAndInfoPrint()
	{
	After(iSecondsRemaining*1000000);
	TBuf<100> message;
	message.Format(_L("%d seconds remaining"), iSecondsRemaining);
	User::InfoPrint(message);
	}

/**
  Inform with a message ,the timer seconds remaining till its up.\n
  Send event to windows group when timer time is up .\n
*/
void CTestTimer::RunL()
	{
	--iSecondsRemaining;
	if (iSecondsRemaining>0)
		{
		QueueAndInfoPrint();
		}
	else
		{
		User::InfoPrint(_L("Time's up"));
		iCompleted = ETrue;
		
		CCoeEnv* env = CCoeEnv::Static();
		TWsEvent theEvent;
		theEvent.SetType(EEventUser + 1);

		RWsSession	ws;
		TInt theRes = ws.Connect();

		if(theRes == KErrNone)
			{
			RWindowGroup& winGroup = env->RootWin();
			theEvent.SetType(EEventUser + 1);
			theEvent.SetTimeNow();
			TInt theId = winGroup.Identifier();
			ws.SendEventToWindowGroup(theId, theEvent);
			}
		ws.Close();	
		}
	}



/*************************************************************
 **  
 **  CDefocusingEdwinUi
 **  
 *************************************************************/

CDefocusingEdwinUi::CDefocusingEdwinUi(CTmsTestStep* aStep) :
	  CTestAppUi(aStep, KNullDesC)
	  {
	  }

CDefocusingEdwinUi::~CDefocusingEdwinUi()
	{
	RemoveFromStack(iTextEditor);
	delete iTextEditor;
	delete iTestTimer;
	}

/**
  Construct a new Editor window and set the containing window \n
  Set cursor position in the document and the word wrap \n
  Set the pointer capture and make the control ready to draw\n
  Install the FEP and call the StartAutoTest()\n
*/
void CDefocusingEdwinUi::ConstructL()
	{
	CTestAppUi::ConstructL();
	iTestTimer = CTestTimer::NewL(5);

	iTextEditor=new(ELeave) CEikEdwin(TGulBorder(TGulBorder::EDeepSunkenWithOutline));
	iTextEditor->ConstructL(CEikEdwin::EOwnsWindow, 20, 0, 3);
	iTextEditor->SetContainerWindowL();
	iTextEditor->SetExtent(TPoint(20, 20), iTextEditor->Size());
	iTextEditor->SetTextL(&KLitInitialText);
	iTextEditor->SetCursorPosL(0, EFalse); // a necessary function call as it creates the CTextView in the CEikEdwin - also it needs to be passed zero to work around a bug where if it's passed textEditor->TextLength() (to put the cursor at the end of the text) then all the text gets scrolled to the left a bit (i.e. the left centimeter or so is off-screen)
	iTextEditor->SetWordWrapL(ETrue);
	iTextEditor->SetPointerCapture();
	AddToStackL(iTextEditor, ECoeStackPriorityDialog, ECoeStackFlagSharable);
	iTextEditor->ActivateL();
	iTextEditor->SetCursorPosL(iTextEditor->TextLength(), EFalse); // the cursor can now be put at the end of the text as the text has been laid out
	
	iTmStart.HomeTime();
	
	const TUid KUidTFep1 = { 0x102024D0 };
	iCoeEnv->InstallFepL(KUidTFep1);

	AutoTestManager().StartAutoTest();
	}

/**
   @SYMTestCaseID 		UIF-FEPTEST-0001
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Launch the application and offer events.
   @SYMTestPriority 	High 
   @SYMTestStatus 		Implemented
   @SYMTestActions 		Launch an application with the editor window. The application is made to exit, when a timer expires.   
   						Load the FEP (TFEP1). Create character codes for text events. Offer the texts to the applciation for 
   						the TFEP1 to intercept.  
   						A succesful implementation ensures that the application exits without a crash.
  						Here the layout of the FEP UI is such that the Composition Window is integrated into the Target Control.  
   @SYMTestExpectedResults The test case fails if the app crashed with an exception and passes if the app exits cleanly
 */
void CDefocusingEdwinUi::RunTestStepL(TInt aStep)
	{
	User::After(TTimeIntervalMicroSeconds32(1));

	TTimeIntervalSeconds theInterval;

	TTime tm;
	tm.HomeTime();
	tm.SecondsFrom(iTmStart,theInterval);

	TInt theInt = theInterval.Int();

	if(iCurrentSecond < theInt)
		{
		if(KNumberOfSeconds-iCurrentSecond < 0)
			{
			iCoeEnv->InstallFepL(KNullUid);
			AutoTestManager().FinishAllTestCases(CAutoTestManager::EPass);
			return;
			}
		else
			{
			TBuf<100> message;
			message.Format(_L("%d seconds remaining"), KNumberOfSeconds-iCurrentSecond);
			User::InfoPrint(message);
		
			iCurrentSecond = theInt;
			}
		}
	TWsEvent theEvent;
	TKeyEvent *theKey = theEvent.Key();
	theKey->iScanCode = 0;
	theKey->iModifiers= 0;
	theKey->iRepeats=0;
	theKey->iCode = 'A';
	
	TInt nRes = aStep % 7;

	if(nRes == 6)
		{
		theKey->iCode = EKeyEnter;
		}
	else
		{
		theKey->iCode += nRes;
		}

	INFO_PRINTF2(_L("Simulate Key Event with code %d"), theKey->iCode);
	SendEventToWindowGroups(theEvent);
	}

/**
  Handle the user defined events.\n
  Handle the System events
  Call made to the static implementation CEikAppUi::HandleSystemEventL().\n
*/
void CDefocusingEdwinUi::HandleApplicationSpecificEventL(TInt , const TWsEvent& aEvent)
	{
	if(aEvent.Type() == (EEventUser + 1))
		{
		Exit();
		}
	else
		{
		CEikAppUi::HandleSystemEventL(aEvent);
		}
	}
	
/**
  Handle the window events.\n
  Create a window server session and connect the client\n
  Set the event type and call the SendEventToWindowGroup().\n
  Close the session on completion \n
*/
void CDefocusingEdwinUi::SendEventToWindowGroups(TWsEvent&	aEvent)
	{
	RWsSession	ws;
	TInt theRes = ws.Connect();
	TEST(theRes == KErrNone);

	RWindowGroup& winGroup = iCoeEnv->RootWin();
	TInt theId = winGroup.Identifier();
	
	aEvent.SetType(EEventKeyDown);
	aEvent.SetTimeNow();
	ws.SendEventToWindowGroup(theId, aEvent);
		
	aEvent.SetType(EEventKey);
	aEvent.SetTimeNow();
	ws.SendEventToWindowGroup(theId, aEvent);

	aEvent.SetType(EEventKeyUp);
	aEvent.SetTimeNow();
	ws.SendEventToWindowGroup(theId, aEvent);
	
	ws.Close();
	}



/************************************************************* 
 **  
 **  CTestDefocusingEdwinStep
 **  
 *************************************************************/

CTestDefocusingEdwinStep::CTestDefocusingEdwinStep()
	{
	SetTestStepName(KDefocusingEdwinStep);
	}

CTestDefocusingEdwinStep::~CTestDefocusingEdwinStep()
	{
	}

/**
   Call ConstructL() of CEikonEnv and initialize a pointer to CDefocusingEdwinUi.
   Set the application's user interface object.
   Call ConstructL() of CDefocusingEdwinUi
 */
void CTestDefocusingEdwinStep::ConstructAppL(CEikonEnv* aCoe)
    { // runs inside a TRAP harness
	aCoe->ConstructL();
	CTestAppUi* appUi= new (ELeave) CDefocusingEdwinUi(this);
    aCoe->SetAppUi(appUi);
    appUi->ConstructL();
	}

/**
   Launches the application using ExecuteD(). \n
 */
TVerdict CTestDefocusingEdwinStep::doTestStepL()
	{
	PreallocateHALBuffer();

	INFO_PRINTF1(_L("TestDefocusingEdwin started..."));

	__UHEAP_MARK;

	SetTestStepID(_L("UIF-FEPTEST-0001"));

	CEikonEnv* coe=new(ELeave) CEikonEnv;
	TRAPD(err,ConstructAppL(coe));

	if (!err)
		coe->ExecuteD();
	else
		{
		delete coe;
		SetTestStepResult(EFail);
		}

	RecordTestResultL();
	CloseTMSGraphicsStep();

	__UHEAP_MARKEND;

	INFO_PRINTF1(_L("...TestDefocusingEdwin finished!"));

	return TestStepResult();
	}

