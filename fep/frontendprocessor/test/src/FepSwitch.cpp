// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <e32keys.h>
#include <centralrepository.h>
#include <w32std.h>
#include <coedef.h>
#include <fepbase.h>
#include <apgwgnam.h>
#include <graphics/cone/coedefkeys.h>

class CFepKeyDataListener;

const TUid KFepSwitchAppUid = {0xfabbabba}; // fake uid

// class definitions

class CFepSwitcher : public CActive
	{
public:
	static CFepSwitcher* NewLC();
	virtual ~CFepSwitcher();
	void SetOnOrOffKeyDataL(TInt aError, const TFepOnOrOffKeyData& aNewKeyData, TInt32 CFepSwitcher::* aCaptureKeyHandleOrError, TFepOnOrOffKeyData CFepSwitcher::* aKeyDataToAssignTo);
	void RetryFailedKeyCaptures(TBool& aNeedFurtherRetry);
private:
	class CRetryOnError : public CTimer
		{
	public:
		static CRetryOnError* NewL(CFepSwitcher& aFepSwitcher);
		virtual ~CRetryOnError();
		void Activate();
	private:
		CRetryOnError(CFepSwitcher& aFepSwitcher);
		void DoActivate();
		// from CActive (via CTimer)
		virtual void RunL();
	private:
		CFepSwitcher& iFepSwitcher;
		TInt iRetryCount;
		};
private:
	CFepSwitcher();
	void ConstructL();
	void Queue();
	static TBool MatchesKeyData(const TKeyEvent& aKeyEvent, const TFepOnOrOffKeyData& aKeyData);
	// from CActive
	virtual void DoCancel();
	virtual void RunL();
private:
	TUint iFlags;
	TInt32 iOnKey_CaptureKeyHandleOrError;
	TInt32 iOffKey_CaptureKeyHandleOrError; // a capture-key handle
	RWsSession iWsSession;
	RWindowGroup iWindowGroup;
	CFepGenericGlobalSettings* iFepGenericGlobalSettings;
	CRetryOnError* iRetryOnError;
	TFepOnOrOffKeyData iOnKeyData;
	TFepOnOrOffKeyData iOffKeyData;
	CFepKeyDataListener* iFepKeyDataListener_OnKey;
	CFepKeyDataListener* iFepKeyDataListener_OffKey;
	};

class CFepKeyDataListener : public CActive
	{
public:
	static CFepKeyDataListener* NewLC(CFepSwitcher& aFepSwitcher, TUint32 aRepositoryKeyMask_OnOrOff, TInt32 CFepSwitcher::* aCaptureKeyHandleOrError, TFepOnOrOffKeyData CFepSwitcher::* aKeyDataToAssignTo);
	virtual ~CFepKeyDataListener();
private:
	CFepKeyDataListener(CFepSwitcher& aFepSwitcher, TUint32 aRepositoryKeyMask_OnOrOff, TInt32 CFepSwitcher::* aCaptureKeyHandleOrError, TFepOnOrOffKeyData CFepSwitcher::* aKeyDataToAssignTo);
	void ConstructL();
	void SetFepSwitcherL();
	void Queue();
	// from CActive
	virtual void DoCancel();
	virtual void RunL();
private:
	CFepSwitcher& iFepSwitcher;
	const TUint32 iRepositoryKeyMask_OnOrOff;
	CRepository* iRepository;
	TInt32 CFepSwitcher::* const iCaptureKeyHandleOrError;
	TFepOnOrOffKeyData CFepSwitcher::* const iKeyDataToAssignTo;
	};

// CFepSwitcher

CFepSwitcher* CFepSwitcher::NewLC()
	{
	CFepSwitcher* const self=new(ELeave) CFepSwitcher;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CFepSwitcher::~CFepSwitcher()
	{
	Cancel();
	if (iWindowGroup.WsHandle()!=0)
		{
		if (iOnKey_CaptureKeyHandleOrError>0)
			{
			iWindowGroup.CancelCaptureKey(iOnKey_CaptureKeyHandleOrError);
			}
		if (iOffKey_CaptureKeyHandleOrError>0)
			{
			iWindowGroup.CancelCaptureKey(iOffKey_CaptureKeyHandleOrError);
			}
		iWindowGroup.Close();
		}
	iWsSession.Close();
	delete iFepGenericGlobalSettings;
	delete iRetryOnError;
	delete iFepKeyDataListener_OnKey;
	delete iFepKeyDataListener_OffKey;
	}

void CFepSwitcher::SetOnOrOffKeyDataL(TInt aError, const TFepOnOrOffKeyData& aNewKeyData, TInt32 CFepSwitcher::* aCaptureKeyHandleOrError, TFepOnOrOffKeyData CFepSwitcher::* aKeyDataToAssignTo)
	{
	// change the state of this object
	TInt32& captureKeyHandleOrError=this->*aCaptureKeyHandleOrError;
	TInt32 captureKeyToCancel=0;
	if (aError!=KErrNone)
		{
		captureKeyToCancel=captureKeyHandleOrError;
		captureKeyHandleOrError=0;
		}
	else if (this->*aKeyDataToAssignTo!=aNewKeyData)
		{
		this->*aKeyDataToAssignTo=aNewKeyData;
		captureKeyToCancel=captureKeyHandleOrError;
		captureKeyHandleOrError=iWindowGroup.CaptureKey(aNewKeyData.CharacterCodeForFoldedMatch(), aNewKeyData.ModifierMask(), aNewKeyData.ModifierValues());
		}
	if (captureKeyToCancel>0)
		{
		iWindowGroup.CancelCaptureKey(captureKeyToCancel);
		}

	// kick-off or stop iRetryOnError, if need be
	if ((iOnKey_CaptureKeyHandleOrError>=0) && (iOffKey_CaptureKeyHandleOrError>=0))
		{
		iRetryOnError->Cancel();
		}
	else if (!iRetryOnError->IsActive())
		{
		iRetryOnError->Activate();
		}
	}

void CFepSwitcher::RetryFailedKeyCaptures(TBool& aNeedFurtherRetry)
	{
	__ASSERT_DEBUG((iOnKey_CaptureKeyHandleOrError<0) || (iOffKey_CaptureKeyHandleOrError<0), User::Invariant());
	if (iOnKey_CaptureKeyHandleOrError<0)
		{
		iOnKey_CaptureKeyHandleOrError=iWindowGroup.CaptureKey(iOnKeyData.CharacterCodeForFoldedMatch(), iOnKeyData.ModifierMask(), iOnKeyData.ModifierValues());
		}
	if (iOffKey_CaptureKeyHandleOrError<0)
		{
		iOffKey_CaptureKeyHandleOrError=iWindowGroup.CaptureKey(iOffKeyData.CharacterCodeForFoldedMatch(), iOffKeyData.ModifierMask(), iOffKeyData.ModifierValues());
		}
	aNeedFurtherRetry=((iOnKey_CaptureKeyHandleOrError<0) || (iOffKey_CaptureKeyHandleOrError<0));
	}

CFepSwitcher::CFepSwitcher()
	:CActive(EPriorityStandard),
	 iFlags(0),
	 iOnKey_CaptureKeyHandleOrError(0),
	 iOffKey_CaptureKeyHandleOrError(0),
	 iFepGenericGlobalSettings(NULL),
	 iRetryOnError(NULL),
	 iOnKeyData(0, 0, 0),
	 iOffKeyData(0, 0, 0),
	 iFepKeyDataListener_OnKey(NULL),
	 iFepKeyDataListener_OffKey(NULL)
	{
	CActiveScheduler::Add(this);
	}

void CFepSwitcher::ConstructL()
	{
	User::LeaveIfError(iWsSession.Connect());
	iWindowGroup=RWindowGroup(iWsSession);
	User::LeaveIfError(iWindowGroup.Construct(REINTERPRET_CAST(TUint32, this), EFalse));
	CApaWindowGroupName* wgName=CApaWindowGroupName::NewLC(iWsSession,iWindowGroup.Identifier());
 	wgName->SetHidden(ETrue);
 	wgName->SetAppUid(KFepSwitchAppUid);
	wgName->SetWindowGroupName(iWindowGroup);
 	CleanupStack::PopAndDestroy(wgName);
	iFepGenericGlobalSettings=CFepGenericGlobalSettings::NewL();
	iRetryOnError=CRetryOnError::NewL(*this);
	iFepKeyDataListener_OnKey=CFepKeyDataListener::NewLC(*this, ERepositoryKeyMask_OnKeyData, &CFepSwitcher::iOnKey_CaptureKeyHandleOrError, &CFepSwitcher::iOnKeyData);
	iFepKeyDataListener_OffKey=CFepKeyDataListener::NewLC(*this, ERepositoryKeyMask_OffKeyData, &CFepSwitcher::iOffKey_CaptureKeyHandleOrError, &CFepSwitcher::iOffKeyData);
	Queue();
	}

void CFepSwitcher::Queue()
	{
	iWsSession.EventReady(&iStatus);
	SetActive();
	}

TBool CFepSwitcher::MatchesKeyData(const TKeyEvent& aKeyEvent, const TFepOnOrOffKeyData& aKeyData)
	{ // static
	return ((User::Fold(aKeyEvent.iCode)==User::Fold(aKeyData.CharacterCodeForFoldedMatch())) &&
			((aKeyEvent.iModifiers&aKeyData.ModifierMask())==aKeyData.ModifierValues()));
	}

void CFepSwitcher::DoCancel()
	{
	iWsSession.EventReadyCancel();
	}

void CFepSwitcher::RunL()
	{
	const TInt error=iStatus.Int();
	if (error<KErrNone)
		{
		Queue();
		User::Leave(error);
		}
	TWsEvent event;
	iWsSession.GetEvent(event); // need to call this *before* Queue (which calls iWsSession.EventReady), otherwise Wserv panics us
	Queue();
	if (event.Type()==EEventKey)
		{
		const TBool isOn=iFepGenericGlobalSettings->IsOn();
		const TKeyEvent& keyEvent=*event.Key();
		if ((!isOn) && MatchesKeyData(keyEvent, iOnKeyData))
			{
			iFepGenericGlobalSettings->SetIsOn(ETrue);
			iFepGenericGlobalSettings->StoreChangesAndBroadcastL();
			}
		else if (isOn && MatchesKeyData(keyEvent, iOffKeyData))
			{
			iFepGenericGlobalSettings->SetIsOn(EFalse);
			iFepGenericGlobalSettings->StoreChangesAndBroadcastL();
			}
		}
	}

// CFepSwitcher::CRetryOnError

CFepSwitcher::CRetryOnError* CFepSwitcher::CRetryOnError::NewL(CFepSwitcher& aFepSwitcher)
	{ // static
	CRetryOnError* const self=new(ELeave) CRetryOnError(aFepSwitcher);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CFepSwitcher::CRetryOnError::~CRetryOnError()
	{
	Cancel();
	}

void CFepSwitcher::CRetryOnError::Activate()
	{
	iRetryCount=0;
	DoActivate();
	}

CFepSwitcher::CRetryOnError::CRetryOnError(CFepSwitcher& aFepSwitcher)
	:CTimer(EPriorityStandard+1),
	 iFepSwitcher(aFepSwitcher)
	{
	CActiveScheduler::Add(this);
	}

void CFepSwitcher::CRetryOnError::DoActivate()
	{
	// retry every second for a minute, followed by every minute for an hour, followed by every hour
	TInt timeInterval=1000000;
	if (iRetryCount>=120)
		{
		timeInterval*=(60*60); // up it to every hour
		}
	else if (iRetryCount>=60)
		{
		timeInterval*=60; // up it to every minute
		}
	After(TTimeIntervalMicroSeconds32(timeInterval));
	}

void CFepSwitcher::CRetryOnError::RunL()
	{
	const TInt error=iStatus.Int();
	TBool needFurtherRetry=ETrue;
	iFepSwitcher.RetryFailedKeyCaptures(needFurtherRetry);
	if (needFurtherRetry)
		{
		++iRetryCount;
		DoActivate();
		}
	User::LeaveIfError(error);
	}

// CFepKeyDataListener

CFepKeyDataListener* CFepKeyDataListener::NewLC(CFepSwitcher& aFepSwitcher, TUint32 aRepositoryKeyMask_OnOrOff, TInt32 CFepSwitcher::* aCaptureKeyHandleOrError, TFepOnOrOffKeyData CFepSwitcher::* aKeyDataToAssignTo)
	{
	CFepKeyDataListener* const self=new(ELeave) CFepKeyDataListener(aFepSwitcher, aRepositoryKeyMask_OnOrOff, aCaptureKeyHandleOrError, aKeyDataToAssignTo);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CFepKeyDataListener::~CFepKeyDataListener()
	{
	Cancel();
	delete iRepository;
	}

CFepKeyDataListener::CFepKeyDataListener(CFepSwitcher& aFepSwitcher, TUint32 aRepositoryKeyMask_OnOrOff, TInt32 CFepSwitcher::* aCaptureKeyHandleOrError, TFepOnOrOffKeyData CFepSwitcher::* aKeyDataToAssignTo)
	:CActive(EPriorityStandard-1),
	 iFepSwitcher(aFepSwitcher),
	 iRepositoryKeyMask_OnOrOff(aRepositoryKeyMask_OnOrOff),
	 iRepository(NULL),
	 iCaptureKeyHandleOrError(aCaptureKeyHandleOrError),
	 iKeyDataToAssignTo(aKeyDataToAssignTo)
	{
	CActiveScheduler::Add(this);
	}

void CFepKeyDataListener::ConstructL()
	{
	iRepository=CRepository::NewL(TUid::Uid(KUidFepFrameworkRepository));
	SetFepSwitcherL();
	Queue();
	}

void CFepKeyDataListener::SetFepSwitcherL()
	{
	TFepOnOrOffKeyData onOrOffKeyData(0, 0, 0);
	TInt error=KErrNone;
	CFepGenericGlobalSettings::ReadOnOrOffKeyData(*iRepository, onOrOffKeyData, iRepositoryKeyMask_OnOrOff, &error);
	iFepSwitcher.SetOnOrOffKeyDataL(error, onOrOffKeyData, iCaptureKeyHandleOrError, iKeyDataToAssignTo);
	}

void CFepKeyDataListener::Queue()
	{
#if defined(_DEBUG)
	const TInt error=
#endif
	iRepository->NotifyRequest(iRepositoryKeyMask_OnOrOff, ERepositoryKeyMask_OnKeyData|ERepositoryKeyMask_OffKeyData, iStatus);
	__ASSERT_DEBUG(error==KErrNone, User::Invariant());
	SetActive();
	}

void CFepKeyDataListener::DoCancel()
	{
	iRepository->NotifyCancel(iRepositoryKeyMask_OnOrOff, ERepositoryKeyMask_OnKeyData|ERepositoryKeyMask_OffKeyData);
	}

void CFepKeyDataListener::RunL()
	{
	const TInt error=iStatus.Int();
	Queue();
	User::LeaveIfError(error);
	SetFepSwitcherL();
	}

// CNonPanickingScheduler

class CNonPanickingScheduler : public CActiveScheduler
	{
public:
	static CNonPanickingScheduler* NewLC();
private:
	// from CActiveScheduler
	virtual void Error(TInt aError) const;
	};

CNonPanickingScheduler* CNonPanickingScheduler::NewLC()
	{ // static
	CNonPanickingScheduler* const scheduler=new(ELeave) CNonPanickingScheduler;
	CleanupStack::PushL(scheduler);
	return scheduler;
	}

void CNonPanickingScheduler::Error(TInt) const
	{
	// don't panic here, like CActiveScheduler::Error does
	}

// top-level functions

LOCAL_C void MainL()
	{
	CActiveScheduler* const activeScheduler=CNonPanickingScheduler::NewLC();
	CActiveScheduler::Install(activeScheduler);
	CFepSwitcher* const fepSwitcher=CFepSwitcher::NewLC();
	RProcess::Rendezvous(KErrNone);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(2, activeScheduler);
	}

GLDEF_C TInt E32Main()
	{
	CTrapCleanup* const cleanup=CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(error, MainL());
	delete cleanup;
	return error;
	}

