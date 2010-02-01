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

#if (!defined __TAPPMORSE_H__)
#define __TAPPMORSE_H__

#include "appfwk_test_AppUi.h"
#include <fepbase.h>
#include <coecntrl.h>

//! A CTestStep derived class     

/**   Launch application and perform test.\n  */

class CTAppMorse : public CTmsTestStep
	{
public:
	CTAppMorse();
	~CTAppMorse();
	virtual TVerdict doTestStepL();
	void ConstructAppL(CEikonEnv* aEikEnv);
	};


//	CStackFaultTestFep class definition


//! A CCoeFep derived class 
/** 
  Class derived from the Abstract base class for all the FEPs.\n
  This class implements all the pure virtual functions of the base class.\n
*/
class CStackFaultTestFep : public CCoeFep
	{
public:
	static CCoeFep* NewFepL( CCoeEnv& aConeEnvironment, const CCoeFepParameters& aFepParameters );
	inline void MakeDeferredFunctionCall(MDeferredFunctionCall& aDeferredFunctionCall) {CCoeFep::MakeDeferredFunctionCall(aDeferredFunctionCall);}
	inline void SimulateKeyEventsL(const TArray<TUint>& aArrayOfCharacters) {CCoeFep::SimulateKeyEventsL(aArrayOfCharacters);}
	inline void SimulateKeyEventsL(const TArray<MModifiedCharacter>& aArrayOfModifiedCharacters) {CCoeFep::SimulateKeyEventsL(aArrayOfModifiedCharacters);}
	inline void WriteAttributeDataAndBroadcastL(TUid aAttributeUid) {CCoeFep::WriteAttributeDataAndBroadcastL(aAttributeUid);}
	inline TBool IsOn() const {return CCoeFep::IsOn();}
private:
	CStackFaultTestFep(CCoeEnv& aConeEnvironment);
	void ConstructL(const CCoeFepParameters& aFepParameters);
// None of these functions below are called for the test but must be
// defined to complete the interfaces that a fep is required to implement
	// from CCoeFep
	virtual void CancelTransaction() {};
	virtual void IsOnHasChangedState() {};
	virtual void OfferKeyEventL(TEventResponse& aEventResponse, const TKeyEvent& /*aKeyEvent*/, TEventCode /*aEventCode*/) {aEventResponse=EEventWasNotConsumed;};
	virtual void OfferPointerEventL(TEventResponse& aEventResponse, const TPointerEvent& /*aPointerEvent*/, const CCoeControl* /*aWindowOwningControl*/) {aEventResponse=EEventWasNotConsumed;};
	virtual void OfferPointerBufferReadyEventL(TEventResponse& aEventResponse, const CCoeControl* /*aWindowOwningControl*/) {aEventResponse=EEventWasNotConsumed;};
	// from MFepAttributeStorer (via CCoeFep)
	virtual TInt NumberOfAttributes() const {return 0;};
	virtual TUid AttributeAtIndex(TInt /*aIndex*/) const {return KNullUid;};
	virtual void WriteAttributeDataToStreamL(TUid /*aAttributeUid*/, RWriteStream& /*aStream*/) const {};
	virtual void ReadAttributeDataFromStreamL(TUid /*aAttributeUid*/, RReadStream& /*aStream*/) {};
	// from MCoeForegroundObserver (via CCoeFep)
	virtual void HandleGainingForeground() {};
	virtual void HandleLosingForeground() {};
	// from MCoeFocusObserver (via CCoeFep)
	virtual void HandleChangeInFocus() {};
	virtual void HandleDestructionOfFocusedItem() {};
	};


//	CTAppMorseAppView class definition



//! A CCoeControl derived class  
/**   Class derived from the control base class.\n
	  All controls are derived from this class and the handling of events are also specified.\n
*/
class CTAppMorseAppView : public CCoeControl
	{
	class CStop;
public:
	static CTAppMorseAppView* NewL( const TRect& aRect,CTmsTestStep* aStep );
	void ConstructL( const TRect& aRect );
	~CTAppMorseAppView();
	void MakeFep();
private:
	CTAppMorseAppView(CTmsTestStep* aStep);
	void Draw(const TRect& /* aRect */) const {};
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	void DoTestL();
protected:
	CTestExecuteLogger&	Logger() {return iStep->Logger();}
private:
	CStop* iStop;
	CCoeFep* iTestFep;
	TUint iNextValidScanCode;
	CTestStep* iStep;
	};
	

//	CTAppMorseAppView::CStop class definition


//! A CActive derived class for the the active scheduler 
/**
  Encapsulates both the issuing of a request to an asynchronous service provider\n
  and the handling of completed requests
*/
class CTAppMorseAppView::CStop : public CActive
	{
public:
	CStop( CEikonEnv* env );
	void DoStop();
	TInt RunError(TInt);
protected:
	void DoCancel();
	void RunL();
private:
	CEikonEnv* iEikEnv;
	};


//	CTAppMorseAppUi class definition
//

// A CTestAppUi derived Class
/**
  User interface class for the test application.\n Implements the RunTestStepL function.\n
*/
class CTAppMorseAppUi : public CTestAppUi
	{
public:
	CTAppMorseAppUi(CTmsTestStep* aStep);
	void ConstructL(CTmsTestStep* aStep);
	~CTAppMorseAppUi();
	void DoExit();
	void ConstructL() {};
private:
	void HandleCommandL(TInt aCommand);
protected: //from CTestAppUi
 	virtual void RunTestStepL(TInt aNumStep);
private:
	CCoeControl* iAppView;
	};

_LIT(KTAppMorse,"TAppMorse");
#endif




