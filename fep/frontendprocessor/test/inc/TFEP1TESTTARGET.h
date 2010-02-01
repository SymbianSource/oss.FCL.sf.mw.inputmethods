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

#ifndef		__TFEP1TESTTARGET_H
#define		__TFEP1TESTTARGET_H

#include <eikenv.h>
#include <e32std.h>
#include <e32base.h>
#include <coemain.h>
#include <bautils.h>
#include <techview/eikdialg.h>
#include "TFEP1COM.H"

#include "appfwk_test_AppUi.h"

_LIT(KFep1TestTargetStep, "Fep1TestTarget");



/*************************************************************
 **  
 **  TTstResourceFileId - TCleanupItem derived class
 **  
 *************************************************************/
/*
	Encapsulates the clean up operations. The class unloads the resource file from the list maintained by CCoeEnv 
*/
class TTstResourceFileId : public TCleanupItem
	{
public:
	TTstResourceFileId(CCoeEnv& aConeEnvironment, TInt aResourceFileId);
private:
	static void UnloadResourceFile(TAny* aThis);
private:
	CCoeEnv& iConeEnvironment;
	TInt iResourceFileId;
	};


/*************************************************************
 **  
 **  CTstTestTargetDialog - CEikDialog derived class
 **  
 *************************************************************/
/*
	The class to create the target dialog, specifying the FEP specific extensions supported by the controls.\n 
*/

class CTstTestTargetDialog : public CEikDialog, private TCoeInputCapabilities::MCoeFepSpecificExtensions
	{
private:
	// from CEikDialog
	virtual void PreLayoutDynInitL();
	virtual TBool OkToExitL(TInt aKeyCode);
	// from TCoeInputCapabilities::MCoeFepSpecificExtensions
	virtual TBool SupportsPhoneticAlphabet() const;
	};



/*************************************************************
 **  
 **  CFep1TestTargetUi
 **  
 *************************************************************/

class CFep1TestTargetUi  : public CTestAppUi
	{
public:
	CFep1TestTargetUi(CTmsTestStep* aStep);
	~CFep1TestTargetUi();
	void ConstructL();
protected:
	virtual void HandleApplicationSpecificEventL(TInt aType,const TWsEvent& aEvent);
protected:
	virtual void RunTestStepL(TInt aStep);
private:
	void SendEventToWindowGroups(TWsEvent&	aEvent);
	void SendKey(TUint aCode, TUint aModifiers = 0,  TInt aRepeats = 0, TInt aScanCode = 0);
	};


/*************************************************************
 **  
 **  CTestFep1TargetStep
 **  
 *************************************************************/
/**
	Performs the FEP tests, with a Target Dialog application.
*/
class CTestFep1TargetStep : public CTmsTestStep
	{
public:
	CTestFep1TargetStep();
	~CTestFep1TargetStep();
	virtual TVerdict doTestStepL();

	void ConstructAppL(CEikonEnv* aCoe);
	};



#endif


