// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "FEPBPRIV.H"
#include <fepbconfig.h>
#include <fepbpanic.h>
#include <centralrepository.h>


#ifdef _DEBUG
LOCAL_C void Panic(TFepBasePanic aPanic)
	{
	User::Panic(KLitFepBasePanicText, aPanic);
	}
#endif

//
// CFepSettingsTracker
//

CFepSettingsTracker* CFepSettingsTracker::NewL(CCoeEnv& aConeEnvironment, MFepAttributeStorer& aFepAttributeStorer)
	{ // static
	CFepSettingsTracker* const self=new(ELeave) CFepSettingsTracker(aConeEnvironment, aFepAttributeStorer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CFepSettingsTracker::~CFepSettingsTracker()
	{
	Cancel();
	delete iRepository;
	}

CFepSettingsTracker::CFepSettingsTracker(CCoeEnv& aConeEnvironment, MFepAttributeStorer& aFepAttributeStorer)
	:CActive(EActivePriorityWsEvents+25),
	 iConeEnvironment(aConeEnvironment),
	 iFepAttributeStorer(aFepAttributeStorer)
	{
	CActiveScheduler::Add(this);
	}

void CFepSettingsTracker::ConstructL()
	{
	iRepository=CRepository::NewL(TUid::Uid(KUidFepSpecificSettingsRepository));
	Queue();
	// we don't call ReadAllAttributesL here - the CCoeFep-derived class has the responsibility to call this when it has been fully constructed; this obligation is documented
	}

void CFepSettingsTracker::Queue()
	{
#if defined(_DEBUG)
	const TInt error=
#endif
	iRepository->NotifyRequest(0, 0, iStatus);
	__ASSERT_DEBUG(error==KErrNone, Panic(EPanicUnexpectedError1));
	SetActive();
	}

void CFepSettingsTracker::DoCancel()
	{
	iRepository->NotifyCancel(0, 0);
	}

void CFepSettingsTracker::RunL()
	{
	const TInt error=iStatus.Int();
	Queue();
	User::LeaveIfError(error);
	iFepAttributeStorer.ReadAllAttributesL(iConeEnvironment);
	}





