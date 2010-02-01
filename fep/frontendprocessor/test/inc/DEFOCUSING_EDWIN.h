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

#ifndef		__DEFOCUSING_EDWIN_H
#define		__DEFOCUSING_EDWIN_H


#include "appfwk_test_AppUi.h"

_LIT(KDefocusingEdwinStep, "DefocusingEdwin");



/*************************************************************
 **  
 **  CTestTimer
 **  
 *************************************************************/

class CTestTimer : private CTimer
	{
public:
	static CTestTimer* NewL(TInt aDurationInSeconds);
	TBool	iCompleted;
private:
	CTestTimer(TInt aDurationInSeconds);
	void QueueAndInfoPrint();
	// from CTimer
	virtual void RunL();
private:
	TInt iSecondsRemaining;
	};


/*************************************************************
 **  
 **  CDefocusingEdwinUi
 **  
 *************************************************************/

class CDefocusingEdwinUi  : public CTestAppUi
	{
public:
	CDefocusingEdwinUi(CTmsTestStep* aStep);
	~CDefocusingEdwinUi();
	void ConstructL();
	void SendEventToWindowGroups(TWsEvent&	aEvent);
	void HandleApplicationSpecificEventL(TInt aType, const TWsEvent& aEvent);
protected:
	CEikEdwin* iTextEditor;
	CTestTimer*	iTestTimer;
	TTime		iTmStart;
	TInt		iCurrentSecond;
protected:
	virtual void RunTestStepL(TInt aStep);
	};



/*************************************************************
 **  
 **  CTestDefocusingEdwinStep
 **  
 *************************************************************/

class CTestDefocusingEdwinStep : public CTmsTestStep
	{
public:
	CTestDefocusingEdwinStep();
	~CTestDefocusingEdwinStep();
	//virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	void ConstructAppL(CEikonEnv* aCoe);
private:
	};

#endif



