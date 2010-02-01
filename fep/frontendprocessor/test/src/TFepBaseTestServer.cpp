// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Non-secure versions will be xxxServer.Dll and require a thread to be started
// in the process of the client. The client initialises the server by calling the
// one and only ordinal.
// Creates a FEP test server to test the FEP key concepts:\n
// FEP base, control input-capabilities, FEP-aware text editors, foreground observer,\n
// and focus observer.
// 
//

/**
 @file 
 @test
 @internalComponent - Internal Symbian test code
*/

#include <basched.h>
#include <eikenv.h>
#include <eikappui.h>
#include <techview/eikon.hrh>
#include <eikapp.h>
#include <eikdoc.h>
#include <techview/eiklabel.h>
#include <eikfctry.h>
#include <coecntrl.h>
#include <barsread.h>
#include <techview/eiktbar.h>
#include <techview/eikmenub.h>
#include <eikspane.h>
#include <test/testexecutestepbase.h>
#include <test/testexecuteserverbase.h>
#include <ecom/ecom.h>
#include <test/testexecutelog.h>

#include "TFepGen.h"
#include "TAppMorse.h"
#include "TFEP1TESTTARGET.h"
#include "KEYBOARDLOGGER.H"
#include "DEFOCUSING_EDWIN.h"

_LIT(KServerName,"TFepBaseTestServer");


//!  A Test Server for FEP base tests. 
class CTFepBaseTestServer : public CTestServer
	{
public:
	static CTFepBaseTestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};
	
	
CTFepBaseTestServer* CTFepBaseTestServer::NewL()
/**
  @return - Instance of the test server
  Same code for Secure and non-secure variants
  Called inside the MainL() function to create and start the
  CTestServer derived server.
 */
	{
	CTFepBaseTestServer * server = new (ELeave) CTFepBaseTestServer();
	CleanupStack::PushL(server);
	// CServer base class call
	server->StartL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}


LOCAL_C void MainL()
/**
  Secure variant
  Much simpler, uses the new Rendezvous() call to sync with the client
*/
	{
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTFepBaseTestServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTFepBaseTestServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

GLDEF_C TInt E32Main()
/**
  @return - Standard Epoc error code on process exit
  Secure variant only
  Process entry point. Called by client using RProcess API
*/
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());
	delete cleanup;
	REComSession::FinalClose();
	__UHEAP_MARKEND;
	return KErrNone;
    }



CTestStep* CTFepBaseTestServer::CreateTestStep(const TDesC& aStepName)
/**
  @return - A CTestStep derived instance
  Secure and non-secure variants
  Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;
	// This server creates just one step but create as many as you want
	// They are created "just in time" when the worker thread is created
	if(aStepName == KTAppMorse)
		{
		testStep = new CTAppMorse();
		}
	if(aStepName == KTFepGen)
		{
		testStep = new CTFepGenStep();
		}
	if(aStepName == KFep1TestTargetStep)
		{
		testStep = new CTestFep1TargetStep();
		}
	if(aStepName == KKeyboardLoggerStep)
		{
		testStep = new CTestKeyboardLoggerStep();
		}
	if(aStepName == KDefocusingEdwinStep)
		{
		testStep = new CTestDefocusingEdwinStep();
		}
	
	return testStep;
	}
