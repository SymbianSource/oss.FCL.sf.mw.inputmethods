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

#if !defined(__TFEPGEN_H__)
#define __TFEPGEN_H__

#include <coecntrl.h>
#include <test/testexecutestepbase.h>
#include "appfwk_test_AppUi.h"


//! A CTestStep Derived Class.
class CTFepGenStep : public CTmsTestStep
	{
public:
	CTFepGenStep();
	~CTFepGenStep();
	virtual TVerdict doTestStepL();
	void ConstructFepGenAppL(CCoeEnv* aCoe);
private:
	};
	

//! A CTestCoeAppUi derived class.
class CFepGenAppUi : public CTestCoeAppUi 
    {
public:
	CFepGenAppUi(CTmsTestStep* aStep);
    ~CFepGenAppUi();
    void ConstructL();
	void DoTestsL();
protected:
	void RunTestStepL(TInt aStepNum);
private:
	void CreateFepGenericL();
	void FepStoreAndBroadCastL();
	void StoreOriginalSettingsL();
	void RetrieveOriginalSettingsL();
   	};
 
 
_LIT(KTFepGen,"TFepGen");

#endif



