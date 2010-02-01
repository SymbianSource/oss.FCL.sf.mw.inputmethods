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

#ifndef		__KEYBOARDLOGGER_H
#define		__KEYBOARDLOGGER_H


#include "appfwk_test_AppUi.h"

//String literal to represent test step name.\n
_LIT(KKeyboardLoggerStep, "KeyboardLogger");


class CTstControl;


/*************************************************************
 **  
 **  TTstOverflowIgnorer
 **  
 *************************************************************/
/*
	An overflow handler for a 16-bit descriptor.
*/  
class TTstOverflowIgnorer : public TDes16Overflow
	{
public:
	virtual void Overflow(TDes16& aDescriptor);
	};


/*************************************************************
 **  
 **  CTstControl
 **  
 *************************************************************/
/*!
	The class is used to instantiate a control to receive pointer drag and move events.\n 
	The ability of Fep to handle these events is tested using the control class.\n
*/
class CTstControl : public CCoeControl
	{
public:
	CTstControl();
	static CTstControl* NewL();
	virtual ~CTstControl();
	void ConstructL();
private:
	class CClearer : public CTimer
		{
	public:
		static CClearer* NewL(CTstControl& aControl);
	private:
		CClearer(CTstControl& aControl);
		// from CActive (via CTimer)
		virtual void RunL();
	private:
		CTstControl& iControl;
		};
private:
	// from CCoeControl
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode);
	virtual TCoeInputCapabilities InputCapabilities() const;
	virtual void Draw(const TRect& aRect) const;
private:
	CFont* iFont;
	RArray<TKeyEvent> iArrayOfKeyDownEvents;
	RArray<TKeyEvent> iArrayOfKeyEvents;
	RArray<TKeyEvent> iArrayOfKeyUpEvents;
	CClearer* iClearer;
private:
	friend class CClearer;
	};


/*************************************************************
 **  
 **  CTestKeyBoardLoggerUi 
 **  
 *************************************************************/

class CTestKeyBoardLoggerUi  : public CTestAppUi
	{
public:
	CTestKeyBoardLoggerUi(CTmsTestStep* aStep) :
	  CTestAppUi(aStep, KNullDesC)
	  {
	  }
	~CTestKeyBoardLoggerUi();
	void ConstructL();
	void SendEventToWindowGroups(TWsEvent&	aEvent);
protected:
	CTstControl* iControl;
protected:
	virtual void RunTestStepL(TInt aStep);
	};


/*************************************************************
 **  
 **  CTestKeyboardLoggerStep
 **  
 *************************************************************/

class CTestKeyboardLoggerStep : public CTmsTestStep
	{
public:
	CTestKeyboardLoggerStep();
	~CTestKeyboardLoggerStep();
	virtual TVerdict doTestStepL();
	void ConstructAppL(CEikonEnv* aCoe);
	};

#endif


