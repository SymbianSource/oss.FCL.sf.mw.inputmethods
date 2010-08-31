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

#include <e32std.h>
#include <e32base.h>
#include <centralrepository.h>
#include <w32std.h>
#include <coemain.h>
#include <coedef.h>
#include "CoeDataStorage.h"
#include <fepbase.h>
#include <fepitfr.h>
#include <fepbpanic.h>
#include "FEPBPRIV.H"
#include <graphics/cone/coedefkeys.h>

inline TBool IsSurrogate(TText a) { return 0xD800 == (a & 0xF800); }
inline TBool IsHighSurrogate(TText a) { return 0xD800 == (a & 0xFC00); }
inline TBool IsLowSurrogate(TText a) { return 0xDC00 == (a & 0xFC00); }
inline TChar JoinSurrogates(TText aHigh, TText aLow)
	{
	return ((aHigh - 0xd7f7) << 10) + aLow;
	}

LOCAL_C void Panic(TFepBasePanic aPanic)
	{
	User::Panic(KLitFepBasePanicText, aPanic);
	}

enum { EUndefinedEventResponse=100 };

enum TFlags
	{
	EFlagOnState			=0x00000001,
	EFlagIsHandlingKeyEvent	=0x00000002,
	EFlagNoDownUpFilter		=0x00000004
	};

// internal class definitions

struct CCoeFep::SKeyEvent
	{
	TEventCode iEventCode;
	TKeyEvent iKeyEvent;
	TKeyResponse iKeyResponse;
	};

class CCoeFep::CHighPriorityActive : public CActive
	{
public:
	static CHighPriorityActive* NewL(CCoeEnv& aConeEnvironment);
	virtual ~CHighPriorityActive();
	void MakeDeferredFunctionCall(MDeferredFunctionCall& aDeferredFunctionCall);
	void SimulateKeyEventL(const TKeyEvent& aKeyEvent);
	inline TBool IsSimulatingKeyEvent() const {return (iFlags&EFlagIsSimulatingKeyEvent);}
	void SendKeyEventToApplicationNowL(TEventCode aEventCode, const TKeyEvent& aKeyEvent);
	TBool MustBeAndNowWillBeDestroyedLater();
private:
	enum
		{
		EFlagIsSimulatingKeyEvent		=0x00000001,
		EFlagOwnsSelf					=0x00000002,
		EFlagRecursionLevelIncremented	=0x00000004
		};
	struct SStackedFlags
		{
		inline SStackedFlags(TUint& aFlags) :iFlags(aFlags), iOldFlags(aFlags) {}
		TUint& iFlags;
		const TUint iOldFlags;
		};
	class CFlagClearer : public CActive
		{
	public:
		static CFlagClearer* NewL();
		virtual ~CFlagClearer();
		void TurnOffFlagIsSimulatingKeyEventDeferred(SStackedFlags& aStackedFlags);
	private:
		CFlagClearer();
		// from CActive
		virtual void DoCancel();
		virtual void RunL();
	private:
		SStackedFlags* iStackedFlags;
		};
private:
	CHighPriorityActive(CCoeEnv& aConeEnvironment);
	void ConstructL();
	void MakeReadyToRun();
	void DoSendKeyEventToApplicationNowL(TEventCode aEventCode, const TKeyEvent& aKeyEvent);
	static void DecrementRecursionLevelAndDestroySelfIfZeroAndOwnsSelf(TAny* aThis);
	static void CancelFlagClearer(TAny* aFlagClearer);
	static void TurnOffFlagIsSimulatingKeyEvent(TAny* aStackedFlags);
	static void DoTurnOffFlagIsSimulatingKeyEvent(SStackedFlags& aStackedFlags);
	// from CActive
	virtual void DoCancel();
	virtual void RunL();
	virtual TInt RunError(TInt aError);
private:
	CCoeEnv& iConeEnvironment;
	TUint iFlags;
	MDeferredFunctionCall* iDeferredFunctionCall;
	RArray<TKeyEvent> iArrayOfKeyEvents;
	CFlagClearer* iFlagClearer;
	TInt iRecursionLevel;
private:
	friend class CFlagClearer;
	};

class CCoeFep::CLowPriorityActive : public CActive
	{
public:
	static CLowPriorityActive* NewL(CHighPriorityActive& aHighPriorityActive);
	virtual ~CLowPriorityActive();
	void SendKeyEventToApplicationDeferred(TEventCode aEventCode, const TKeyEvent& aKeyEvent);
private:
	CLowPriorityActive(CHighPriorityActive& aHighPriorityActive);
	// from CActive
	virtual void DoCancel();
	virtual void RunL();
private:
	CHighPriorityActive& iHighPriorityActive;
	TEventCode iEventCode;
	TKeyEvent iKeyEvent;
	};


NONSHARABLE_CLASS(COnStateTracker) : public CActive
	{
public:
	static COnStateTracker* NewL(CCoeFep& aFep);
	virtual ~COnStateTracker();
private:
	COnStateTracker(CCoeFep& aFep);
	void ConstructL();
	void Queue();
	void SetOnState();
	// from CActive
	virtual void DoCancel();
	virtual void RunL();
private:
	CCoeFep& iFep;
	CRepository* iRepository;
	};

// non-class-member functions

EXPORT_C void FepObserverHandleStartOfTransactionL(MCoeFepObserver& aFepObserver)
	{
	aFepObserver.HandleStartOfTransactionL();
	}

LOCAL_C void FepObserverHandleCompletionOfTransactionL(MCoeFepObserver& aFepObserver)
	{
	aFepObserver.HandleCompletionOfTransactionL();
	}

// CCoeFep::CLowPriorityActive

CCoeFep::CLowPriorityActive* CCoeFep::CLowPriorityActive::NewL(CHighPriorityActive& aHighPriorityActive)
	{
	return new(ELeave) CLowPriorityActive(aHighPriorityActive);
	}

CCoeFep::CLowPriorityActive::~CLowPriorityActive()
	{
	Cancel();
	}

void CCoeFep::CLowPriorityActive::SendKeyEventToApplicationDeferred(TEventCode aEventCode, const TKeyEvent& aKeyEvent)
	{
	Cancel();
	iEventCode=aEventCode;
	iKeyEvent=aKeyEvent;
	TRequestStatus* requestStatus=&iStatus;
	User::RequestComplete(requestStatus, KErrNone);
	SetActive();
	}

CCoeFep::CLowPriorityActive::CLowPriorityActive(CHighPriorityActive& aHighPriorityActive)
	:CActive(EActivePriorityWsEvents-1), // the priority of CLowPriorityActive objects just needs to be less than the priority with which key-events are handled
	 iHighPriorityActive(aHighPriorityActive)
	{
	CActiveScheduler::Add(this);
	}

void CCoeFep::CLowPriorityActive::DoCancel()
	{
	}

void CCoeFep::CLowPriorityActive::RunL()
	{
	iHighPriorityActive.SendKeyEventToApplicationNowL(iEventCode, iKeyEvent);
	}



// COnStateTracker

COnStateTracker* COnStateTracker::NewL(CCoeFep& aFep)
	{ // static
	COnStateTracker* const self=new(ELeave) COnStateTracker(aFep);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

COnStateTracker::~COnStateTracker()
	{
	Cancel();
	delete iRepository;
	}

COnStateTracker::COnStateTracker(CCoeFep& aFep)
	:CActive(EActivePriorityWsEvents+30),
	 iFep(aFep)
	{
	CActiveScheduler::Add(this);
	}

void COnStateTracker::ConstructL()
	{
	iRepository=CRepository::NewL(TUid::Uid(KUidFepFrameworkRepository));

	// We'd like to call "Queue" and "SetOnState" here, but we can't call SetOnState from here as it involves the CCoeFep-derived object's implementation of IsOnHasChangedState being called inside the CCoeFep::BaseConstructL, which probably happens at the start of its contruction routine, before it's ready for its implementation of IsOnHasChangedState to be called. So instead we just make ourselves ready to run so that our RunL gets called as soon as it can
	TRequestStatus* requestStatus=&iStatus;
	User::RequestComplete(requestStatus, KErrNone);
	SetActive();
	}

void COnStateTracker::Queue()
	{
#if defined(_DEBUG)
	const TInt error=
#endif
	iRepository->NotifyRequest(ERepositoryKeyMask_OnState, ERepositoryKeyMask_OnState, iStatus);
	__ASSERT_DEBUG(error==KErrNone, Panic(EPanicUnexpectedError2));
	SetActive();
	}

void COnStateTracker::SetOnState()
	{
	TBool onState=EFalse;
	CFepGenericGlobalSettings::ReadOnState(*iRepository, onState);
	iFep.SetOnState(onState);
	}

void COnStateTracker::DoCancel()
	{
	iRepository->NotifyCancel(ERepositoryKeyMask_OnState, ERepositoryKeyMask_OnState);
	}

void COnStateTracker::RunL()
	{
	const TInt error=iStatus.Int();
	Queue();
	User::LeaveIfError(error);
	SetOnState();
	}


// MCoeFepObserver

/** Handles the start of a FEP transaction. 

The implementation of this function might move focus to an existing edit window, 
or might launch a new dialog (if the dialog should appear as soon 
as the user starts composing their text in the FEP rather than when the text 
contained in the FEP is committed to it).

This function should be called by the FEP when it receives the first event 
which begins a FEP transaction. It should be called after using CoeFep::MakeDeferredFunctionCall() 
to handle the event.

The FEP calls this function indirectly by calling CCoeEnv::ForEachFepObserverCall() 
passing in FepObserverHandleStartOfTransactionL. Internally, this calls HandleStartOfTransactionL() 
for each MCoeFepObserver object that has been registered with the control 
environment by calling CCoeEnv::AddFepObserverL().

@see CCoeFep::MakeDeferredFunctionCall() */
EXPORT_C void MCoeFepObserver::HandleStartOfTransactionL()
	{
	}

/** If an application needs to get more information about a FEP transaction than 
is provided by the FEP framework, (for example, a Japanese FEP may want the 
pronunciation of the transaction's text as well as the text itself), then 
it should override this function to retrieve the information it wants from the 
FEP.

This assumes that the application knows which concrete FEP is above it so 
that it can call the specialised functions in that FEP. An application can 
safely do this by calling CCoeEnv::Fep() to get the address of the current 
FEP (this is NULL if there is no FEP above it) and downcasting this address 
to the specific derived class of the FEP it is expecting, on condition that 
the CCoeEnv::FepUid() function returns the UID of the FEP it is expecting.

This function is called by the FEP framework. If the FEP is doing inline editing, 
HandleCompletionOfTransactionL() is called inside MCoeFepAwareTextEditor's 
CommitFepInlineEditL(). If it isn't doing inline editing, HandleCompletionOfTransactionL() 
is called (inside an active object) after the key events generated by one 
or more calls to CCoeFep's SimulateKeyEventsL() have been sent to the application. */
EXPORT_C void MCoeFepObserver::HandleCompletionOfTransactionL()
	{
	}

EXPORT_C void MCoeFepObserver::MCoeFepObserver_Reserved_1()
	{
	}

EXPORT_C void MCoeFepObserver::MCoeFepObserver_Reserved_2()
	{
	}

// TArrayTransformer

class TArrayTransformer : private CCoeFep::MModifiedCharacter
	{
public:
	TArrayTransformer(const TArray<TUint>& aArrayOfCharacters);
	TArray<CCoeFep::MModifiedCharacter> ArrayOfModifiedCharacters(); // this function is deliberately non-const so that its client is not tempted to create an anonymous (temporary) TArrayTransformer object whose lifetime may turn out to be too short
private:
	static TInt NumberOfModifiedCharacters(const CBase* aThis); // aThis is really a TArrayTransformer
	static const TAny* ModifiedCharacter(const CBase* aThis, TInt aIndex); // aThis is really a TArrayTransformer
	// from CCoeFep::MModifiedCharacter
	virtual TUint CharacterCode() const;
	virtual TUint ModifierMask() const;
	virtual TUint ModifierValues() const;
private:
	const TArray<TUint>& iArrayOfCharacters;
	TInt iCurrentIndex;
	};

TArrayTransformer::TArrayTransformer(const TArray<TUint>& aArrayOfCharacters)
	:iArrayOfCharacters(aArrayOfCharacters),
	 iCurrentIndex(-1)
	{
	}

TArray<CCoeFep::MModifiedCharacter> TArrayTransformer::ArrayOfModifiedCharacters()
	{
	return TArray<CCoeFep::MModifiedCharacter>(NumberOfModifiedCharacters, ModifiedCharacter, REINTERPRET_CAST(const CBase*, this));
	}

TInt TArrayTransformer::NumberOfModifiedCharacters(const CBase* aThis)
	{
	return REINTERPRET_CAST(TArrayTransformer*, CONST_CAST(CBase*, aThis))->iArrayOfCharacters.Count();
	}

const TAny* TArrayTransformer::ModifiedCharacter(const CBase* aThis, TInt aIndex)
	{
	TArrayTransformer& thisObject=*REINTERPRET_CAST(TArrayTransformer*, CONST_CAST(CBase*, aThis));
	__ASSERT_ALWAYS(aIndex>=0, Panic(EPanicBadIndex1));
	__ASSERT_ALWAYS(aIndex<thisObject.iArrayOfCharacters.Count(), Panic(EPanicBadIndex2));
	thisObject.iCurrentIndex=aIndex;
	return aThis;
	}

TUint TArrayTransformer::CharacterCode() const
	{
	__ASSERT_ALWAYS(iCurrentIndex>=0, Panic(EPanicBadIndex3));
	__ASSERT_ALWAYS(iCurrentIndex<iArrayOfCharacters.Count(), Panic(EPanicBadIndex4));
	return iArrayOfCharacters[iCurrentIndex];
	}

/**
Returns a <code>TUint</code> which indicates which modifiers to
override, rather than using the current state of the keyboard's
modifiers.

@return   "TUint"
            The modifiers to override in the key combination.
*/
TUint TArrayTransformer::ModifierMask() const
	{
	return 0;
	}

/**
Returns a <code>TUint</code> which indicates which of the modifiers
specified in the mask (returned by <code>ModifierMask()</code>) must
be on and which must be off.

@return   "TUint"
            Indicates which of the modifiers specified in the mask
            (returned by <code>ModifierMask()</code>) must be on
            and which must be off.
*/            
TUint TArrayTransformer::ModifierValues() const
	{
	return 0;
	}


// CCoeFep::CCoeFepExtra

NONSHARABLE_CLASS(CCoeFep)::CCoeFepExtra : public CBase
	{
public:
	CCoeFepExtra();
	virtual ~CCoeFepExtra();
public:
	CCoeFep::CLowPriorityActive* iLowPriorityActive;
	CFepSettingsTracker* iSettingsTracker;
	COnStateTracker* iOnStateTracker;
	};


CCoeFep::CCoeFepExtra::CCoeFepExtra()
	{
	}

CCoeFep::CCoeFepExtra::~CCoeFepExtra()
	{
	delete iLowPriorityActive;
	delete iSettingsTracker;
	delete iOnStateTracker;
	}


// CCoeFep

/** Destructor.

Removes the FEP from the control environment's message observer list prior 
to its destruction. */
EXPORT_C CCoeFep::~CCoeFep()
	{
	iConeEnvironment.RemoveForegroundObserver(*this);
	iConeEnvironment.RemoveFocusObserver(*this);
	iConeEnvironment.RemoveMessageObserver(*this);
	if (iHighPriorityActive!=NULL)
		{
		if (!iHighPriorityActive->MustBeAndNowWillBeDestroyedLater()) // if this return "true", the iHighPriorityActive object now "owns itself", i.e. it will be destroyed just before the outermost CHighPriorityActive::SendKeyEventToApplicationNowL call on the call-stack returns
			{
			delete iHighPriorityActive;
			}
		}
	delete iExtra;
	delete iLastKeyEvent;
	}

/** Tests whether or not the FEP is simulating key events.

This function should be called early in the FEP's implementation of OfferKeyEventL(). 
If it returns ETrue, OfferKeyEventL() should immediately return EKeyWasNotConsumed 
so that the application gets the key event as intended. 

It is called by the FEP_START_KEY_EVENT_HANDLER_L and FEP_START_KEY_EVENT_HANDLER_NO_DOWN_UP_FILTER_L 
macros.

@return ETrue if the FEP is simulating key events, EFalse if not. */
EXPORT_C TBool CCoeFep::IsSimulatingKeyEvent() const
	{
	return iHighPriorityActive->IsSimulatingKeyEvent();
	}

/** Since the advent of Platform security in Symbian OS, the turning on/off of FEPs is handled
via a different means (which is secure). Hence this function now only ever returns EFalse. 

@deprecated
@param aKeyEvent No longer used.
@return Always returns EFalse.
*/
EXPORT_C TBool CCoeFep::IsTurnedOnByL(const TKeyEvent& /*aKeyEvent*/) const
	{
	return EFalse;
	}

/** Since the advent of Platform security in Symbian OS, the turning on/off of FEPs is handled
via a different means (which is secure). Hence this function now only ever returns EFalse. 

@deprecated
@param aKeyEvent No longer used.
@return Always returns EFalse.
*/
EXPORT_C TBool CCoeFep::IsTurnedOffByL(const TKeyEvent& /*aKeyEvent*/) const
	{
	return EFalse;
	}

/**
@internalComponent

Note that we should maintain BC for this function as it is used by 
the published-All macro FEP_START_KEY_EVENT_HANDLER_L
*/
EXPORT_C void CCoeFep::OnStartingHandlingKeyEvent_WithDownUpFilterLC()
	{
	DoOnStartingHandlingKeyEventLC(0);
	}


/**
@internalComponent

Note that we should maintain BC for this function as it is used by 
the published-All macro FEP_START_KEY_EVENT_HANDLER_NO_DOWN_UP_FILTER_L
*/
EXPORT_C void CCoeFep::OnStartingHandlingKeyEvent_NoDownUpFilterLC()
	{
	DoOnStartingHandlingKeyEventLC(EFlagNoDownUpFilter);
	}


/**
@internalComponent

Note that we should maintain BC for this function as it is used by 
the published-All macros FEP_START_KEY_EVENT_HANDLER_L and FEP_END_KEY_EVENT_HANDLER_L
*/
EXPORT_C TKeyResponse CCoeFep::OnFinishingHandlingKeyEvent_WithDownUpFilterL(TEventCode aEventCode, const TKeyEvent& aKeyEvent, TKeyResponse aKeyResponse)
	{
	__ASSERT_ALWAYS((iFlags&EFlagNoDownUpFilter)==0, Panic(EPanicNoDownUpFilter));
	return DoOnFinishingHandlingKeyEventL(aEventCode, aKeyEvent, aKeyResponse);
	}

/**
@internalComponent

Note that we should maintain BC for this function as it is used by 
the published-All macros FEP_START_KEY_EVENT_HANDLER_NO_DOWN_UP_FILTER_L and 
FEP_END_KEY_EVENT_HANDLER_NO_DOWN_UP_FILTER_L
*/
EXPORT_C TKeyResponse CCoeFep::OnFinishingHandlingKeyEvent_NoDownUpFilterL(TEventCode aEventCode, const TKeyEvent& aKeyEvent, TKeyResponse aKeyResponse)
	{
	__ASSERT_ALWAYS(iFlags&EFlagNoDownUpFilter, Panic(EPanicWithDownUpFilter));
	return DoOnFinishingHandlingKeyEventL(aEventCode, aKeyEvent, aKeyResponse);
	}

/** Protected C++ constructor. 

This function should be called from within the FEP's constructor.

@param aConeEnvironment Control environment. Provides an interface to the 
window server, so that the FEP control can receive pointer and key events. */
EXPORT_C CCoeFep::CCoeFep(CCoeEnv& aConeEnvironment)
	:iConeEnvironment(aConeEnvironment),
	 iFlags(0),
	 iHighPriorityActive(NULL)
	{
	}

/** Initialises the FEP's generic settings (whether the FEP is on or off and what 
key events should turn it on or off). 

During construction, all FEPs must call this member function. It also initialises the 
FEP as an observer so that it receives notification of changes from the control 
environment. Examples of such changes are the target control gaining or losing focus 
or changes to the generic FEP settings made by another running instance of the same FEP.

@param aFepParameters No longer used. */
EXPORT_C void CCoeFep::BaseConstructL(const CCoeFepParameters& /*aFepParameters*/)
	{
	__ASSERT_ALWAYS(iHighPriorityActive==NULL, Panic(EPanicAlreadyCalledBaseConstructL));
	iHighPriorityActive=CHighPriorityActive::NewL(iConeEnvironment);
	iExtra = new(ELeave) CCoeFepExtra;
	iExtra->iLowPriorityActive = CLowPriorityActive::NewL(*iHighPriorityActive);
	iExtra->iSettingsTracker = CFepSettingsTracker::NewL(iConeEnvironment, *this);
	iExtra->iOnStateTracker = COnStateTracker::NewL(*this);
	iLastKeyEvent=new(ELeave) SKeyEvent;
	iConeEnvironment.AddForegroundObserverL(*this);
	iConeEnvironment.AddFocusObserverL(*this);
	iConeEnvironment.AddMessageObserverL(*this);
	}

/** Sets this FEP's attributes with values from the global settings.

Calls the FEP's implementation of MFepAttributeStorer::ReadAttributeDataFromStreamL() 
for each FEP attribute which needs to be synchronised. */
EXPORT_C void CCoeFep::ReadAllAttributesL()
	{
	MFepAttributeStorer::ReadAllAttributesL(iConeEnvironment);
	}

/** Uses a high-priority active object to call the specified object's ExecuteFunctionL().

This function must be used to handle an event if that event starts a transaction 
and if handling that event is dependent on the target control's input 
capabilities. In such a case, MakeDeferredFunctionCall() should be called 
by the FEP, (passing in the relevant MDeferredFunctionCall-derived object) 
before calling HandleStartOfTransactionL().

Note:

The reason why key event handling must be done inside a high priority active 
object is as follows:

In some UI situations, pressing a key should move focus to a previously non-focused 
or non-existent text editor control, and should insert the character corresponding 
to that key event into the newly focused control.

When text entry is via a FEP, for this focus-shift to occur at the right time, 
when the FEP receives its first key event, it should call MCoeFepObserver::HandleStartOfTransactionL(). 
The implementation of this function should do whatever focus shift is required 
(e.g. launch the dialog).

However, if it is a waiting dialog, text cannot now be entered into the FEP 
until the dialog's RunLD() or ExecuteLD() returns (this occurs when the 
dialog is either cancelled or committed). Therefore, HandleStartOfTransactionL() 
will block. The solution is for the FEP to handle the key event from within 
a high priority active object.

@param aDeferredFunctionCall Implements key event handling in its ExecuteFunctionL() 
function. This is deferred in that it is executed when the active object's 
RunL() is called. */
EXPORT_C void CCoeFep::MakeDeferredFunctionCall(MDeferredFunctionCall& aDeferredFunctionCall)
	{
	iHighPriorityActive->MakeDeferredFunctionCall(aDeferredFunctionCall);
	}

/** 
Simulates a key event for each of the character codes in the array passed to 
it.

This function should be called in order to send key events to the application 
underneath the FEP (unless inline editing is taking place, in which case a 
different mechanism is used). FEPs should not use CCoeEnv::SimulateKeyEventL() 
to simulate key events.

@param aArrayOfCharacters An array of characters. 
*/
EXPORT_C void CCoeFep::SimulateKeyEventsL(const TArray<TUint>& aArrayOfCharacters)
	{
	TArrayTransformer arrayTransformer(aArrayOfCharacters); // the TArrayTransformer object cannot be an anonymous (temporary) object as its lifetime must be guaranteed to last until SimulateKeyEventsL returns
	SimulateKeyEventsL(arrayTransformer.ArrayOfModifiedCharacters());
	}

EXPORT_C void CCoeFep::SimulateKeyEventsL(const TArray<MModifiedCharacter>& aArrayOfModifiedCharacters)
	{
	const TUint baseModifiers=iConeEnvironment.WsSession().GetModifierState()|EModifierSpecial;
	TKeyEvent keyEvent;
	keyEvent.iScanCode=0;
	keyEvent.iRepeats=0;

	const TInt numberOfCharacters=aArrayOfModifiedCharacters.Count();
	for (TInt i=0; i<numberOfCharacters; ++i)
		{
		const MModifiedCharacter& modifiedCharacter=aArrayOfModifiedCharacters[i];
		const TUint modifierMask=modifiedCharacter.ModifierMask();
		const TUint modifierValues=modifiedCharacter.ModifierValues();

		__ASSERT_ALWAYS(!(modifierValues&~modifierMask), Panic(EPanicInconsistentModifierMaskAndValues));
		keyEvent.iCode=modifiedCharacter.CharacterCode();
		
		//	Combine surrogate pair as one keyEvent.iCode
		//	skip single surrogate part
		if ( IsSurrogate( keyEvent.iCode ) )
			{
			// Combine surrogate pair
			if (i >= numberOfCharacters - 1)
				{
				//	skip it
				continue;
				}

			// If not surrogate high part
			if ( !IsHighSurrogate( keyEvent.iCode ) )
				{
				continue;
				}
			const MModifiedCharacter &nextModifiedCharacter = aArrayOfModifiedCharacters[i+1];
			TUint nextCode = nextModifiedCharacter.CharacterCode();
			//	If not surrogate low part
			if ( !IsLowSurrogate( nextCode ) )
				{
				continue;
				}
			// Combine surrogate pair
			keyEvent.iCode = JoinSurrogates( keyEvent.iCode, nextCode );
			i++;
			}

		keyEvent.iModifiers=baseModifiers;
		keyEvent.iModifiers&=~(modifierMask&~modifierValues);
		keyEvent.iModifiers|=(modifierMask&modifierValues);

		iHighPriorityActive->SimulateKeyEventL(keyEvent);
		}
	}

/** After changing the value of a single FEP attribute that needs to be synchronised, 
call this function to synchronise other running instances of the FEP.

It first calls MFepAttributeStorer::WriteAttributeDataToStreamL() which writes 
the attribute's new value to the stream and then it causes all other running 
instances of the FEP to be notified of the change.

@param aAttributeUid The UID of the attribute that has changed.
@capability WriteDeviceData */
EXPORT_C void CCoeFep::WriteAttributeDataAndBroadcastL(TUid aAttributeUid)
	{
	MFepAttributeStorer::WriteAttributeDataAndBroadcastL(iConeEnvironment, aAttributeUid);
	}

/** After changing the value of multiple FEP attributes that need to be synchronised, 
call this function to synchronise other running instances of the FEP.

It first calls MFepAttributeStorer::WriteAttributeDataToStreamL() which writes 
the attributes' new values to the stream and then it causes all other running 
instances of the FEP to be notified of the changes.

@param aAttributeUids Array of UIDs for the attribute that have changed.
@capability WriteDeviceData */
EXPORT_C void CCoeFep::WriteAttributeDataAndBroadcastL(const TArray<TUid>& aAttributeUids)
	{
	MFepAttributeStorer::WriteAttributeDataAndBroadcastL(iConeEnvironment, aAttributeUids);
	}

/** Tests whether the FEP is on or off.

@return Non-zero if the FEP is on, EFalse if the FEP is off. */
EXPORT_C TBool CCoeFep::IsOn() const
	{
	return iFlags&EFlagOnState;
	}

/** @internalComponent */
void CCoeFep::SetOnState(TBool aOnState)
	{
	const TUint oldFlags=iFlags;
	if (aOnState)
		iFlags|=EFlagOnState;
	else
		iFlags&=~EFlagOnState;

	if (oldFlags!=iFlags)
		IsOnHasChangedState();
	}

void CCoeFep::DoOnStartingHandlingKeyEventLC(TUint aFlagNoDownUpFilter)
	{
	__ASSERT_DEBUG((aFlagNoDownUpFilter&~EFlagNoDownUpFilter)==0, Panic(EPanicBadFlag));
	__ASSERT_ALWAYS((iFlags&EFlagIsHandlingKeyEvent)==0, Panic(EPanicAlreadyHandlingKeyEvent));
	CleanupStack::PushL(TCleanupItem(TurnOffKeyEventHandlingFlags, &iFlags));
	iFlags|=(EFlagIsHandlingKeyEvent|aFlagNoDownUpFilter);
	}

TKeyResponse CCoeFep::DoOnFinishingHandlingKeyEventL(TEventCode aEventCode, const TKeyEvent& aKeyEvent, TKeyResponse aKeyResponse)
	{
	__ASSERT_ALWAYS(iFlags&EFlagIsHandlingKeyEvent, Panic(EPanicNotHandlingKeyEvent));
	if (~iFlags&EFlagNoDownUpFilter)
		{
		switch (aEventCode)
			{
		case EEventKeyDown:
			__ASSERT_DEBUG(aKeyResponse==EKeyWasConsumed, Panic(EPanicBadKeyResponse1));
			iExtra->iLowPriorityActive->SendKeyEventToApplicationDeferred(aEventCode, aKeyEvent); // this is so that applications get EEventKeyDown events for keys such as shift, ctrl, etc, (i.e. keys for which no EEventKey events occur when they are pressed)
			break;
		case EEventKey:
			iExtra->iLowPriorityActive->Cancel();
			if ((aKeyResponse==EKeyWasNotConsumed) &&
				(iLastKeyEvent->iEventCode==EEventKeyDown) &&
				(iLastKeyEvent->iKeyEvent.iScanCode==aKeyEvent.iScanCode))
				{
				__ASSERT_DEBUG(iLastKeyEvent->iKeyResponse==EKeyWasConsumed, Panic(EPanicBadKeyResponse2));
				iHighPriorityActive->SendKeyEventToApplicationNowL(iLastKeyEvent->iEventCode, iLastKeyEvent->iKeyEvent);
				}
			break;
		case EEventKeyUp:
			if ((iLastKeyEvent->iEventCode==EEventKey) &&
				(iLastKeyEvent->iKeyEvent.iScanCode==aKeyEvent.iScanCode) &&
				(iLastKeyEvent->iKeyResponse==EKeyWasNotConsumed))
				{
				__ASSERT_DEBUG(aKeyResponse==EKeyWasConsumed, Panic(EPanicBadKeyResponse3));
				aKeyResponse=EKeyWasNotConsumed;
				}
			if ((iLastKeyEvent->iEventCode==EEventKeyDown) &&
				(iLastKeyEvent->iKeyEvent.iScanCode==aKeyEvent.iScanCode))
				{
				__ASSERT_DEBUG(iLastKeyEvent->iKeyResponse==EKeyWasConsumed, Panic(EPanicBadKeyResponse4));
				__ASSERT_DEBUG(aKeyResponse==EKeyWasConsumed, Panic(EPanicBadKeyResponse5));
				aKeyResponse=EKeyWasNotConsumed;
				}
			break;
		default:
			break;
			}
		}

	iLastKeyEvent->iEventCode=aEventCode;
	iLastKeyEvent->iKeyEvent=aKeyEvent;
	iLastKeyEvent->iKeyResponse=aKeyResponse;
	CleanupStack::PopAndDestroy(&iFlags); // turns the EFlagIsHandlingKeyEvent (and EFlagNoDownUpFilter if it's on) flags off
	return aKeyResponse;
	}

void CCoeFep::TurnOffKeyEventHandlingFlags(TAny* aFlags)
	{
	*STATIC_CAST(TUint*, aFlags)&=~(EFlagIsHandlingKeyEvent|EFlagNoDownUpFilter);
	}

EXPORT_C void CCoeFep::MFepAttributeStorer_Reserved_1()
	{
	}

EXPORT_C void CCoeFep::MFepAttributeStorer_Reserved_2()
	{
	}

EXPORT_C void CCoeFep::MCoeForegroundObserver_Reserved_1()
	{
	}

EXPORT_C void CCoeFep::MCoeForegroundObserver_Reserved_2()
	{
	}

EXPORT_C void CCoeFep::MCoeFocusObserver_Reserved_1()
	{
	}

EXPORT_C void CCoeFep::MCoeFocusObserver_Reserved_2()
	{
	}


/**
@publishedAll 
@released 
*/
EXPORT_C MCoeMessageObserver::TMessageResponse CCoeFep::HandleMessageL(TUint32, TUid, const TDesC8&)
	{
	return EMessageNotHandled;
	}

EXPORT_C void CCoeFep::MCoeMessageObserver_Reserved_1()
	{
	}

EXPORT_C void CCoeFep::MCoeMessageObserver_Reserved_2()
	{
	}

EXPORT_C void CCoeFep::CCoeFep_Reserved_1()
	{
	}

EXPORT_C void CCoeFep::CCoeFep_Reserved_2()
	{
	}

// CCoeFep::MDeferredFunctionCall

EXPORT_C void CCoeFep::MDeferredFunctionCall::MDeferredFunctionCall_Reserved_1()
	{
	}

EXPORT_C void CCoeFep::MDeferredFunctionCall::MDeferredFunctionCall_Reserved_2()
	{
	}

// CCoeFep::MModifiedCharacter

EXPORT_C void CCoeFep::MModifiedCharacter::MModifiedCharacter_Reserved_1()
	{
	}

EXPORT_C void CCoeFep::MModifiedCharacter::MModifiedCharacter_Reserved_2()
	{
	}

// CCoeFep::CHighPriorityActive

CCoeFep::CHighPriorityActive* CCoeFep::CHighPriorityActive::NewL(CCoeEnv& aConeEnvironment)
	{
	CHighPriorityActive* const highPriorityActive=new(ELeave) CHighPriorityActive(aConeEnvironment);
	CleanupStack::PushL(highPriorityActive);
	highPriorityActive->ConstructL();
	CleanupStack::Pop(highPriorityActive);
	return highPriorityActive;
	}

CCoeFep::CHighPriorityActive::~CHighPriorityActive()
	{
	Cancel();
	iArrayOfKeyEvents.Close();
	delete iFlagClearer;
	}

void CCoeFep::CHighPriorityActive::MakeDeferredFunctionCall(MDeferredFunctionCall& aDeferredFunctionCall)
	{
	__ASSERT_ALWAYS(iDeferredFunctionCall==NULL, Panic(EPanicDeferredFunctionCallOutstanding));
	iDeferredFunctionCall=&aDeferredFunctionCall;

	if (!IsActive())
		MakeReadyToRun();
	}
	
/**
The function appends a key event to the array of keyevents

@param const TKeyEvent& aKeyEvent The key event to be appended
*/
void CCoeFep::CHighPriorityActive::SimulateKeyEventL(const TKeyEvent& aKeyEvent)
	{
	User::LeaveIfError(iArrayOfKeyEvents.Append(aKeyEvent));
	if (!IsActive())
		MakeReadyToRun();
	}

void CCoeFep::CHighPriorityActive::SendKeyEventToApplicationNowL(TEventCode aEventCode, const TKeyEvent& aKeyEvent)
	{
	++iRecursionLevel; // must be done *before* pushing DecrementRecursionLevelAndDestroySelfIfZeroAndOwnsSelf on the cleanup-stack in case that PushL leaves and decrements iRecursionLevel without us actually having incremented it first
	CleanupStack::PushL(TCleanupItem(DecrementRecursionLevelAndDestroySelfIfZeroAndOwnsSelf, this));
	DoSendKeyEventToApplicationNowL(aEventCode, aKeyEvent);
	CleanupStack::PopAndDestroy(this); // custom cleanup-item DecrementRecursionLevelAndDestroySelfIfZeroAndOwnsSelf
	}
	
void CCoeFep::CHighPriorityActive::DoSendKeyEventToApplicationNowL(TEventCode aEventCode, const TKeyEvent& aKeyEvent)
	{
	SStackedFlags stackedFlags(iFlags);
	iFlagClearer->TurnOffFlagIsSimulatingKeyEventDeferred(stackedFlags);
	CleanupStack::PushL(TCleanupItem(CancelFlagClearer, iFlagClearer));
	CleanupStack::PushL(TCleanupItem(TurnOffFlagIsSimulatingKeyEvent, &stackedFlags));
	iFlags|=EFlagIsSimulatingKeyEvent;
	iConeEnvironment.SimulateKeyEventL(aKeyEvent, aEventCode); // this may launch a *waiting* dialog, in which case we want to restore the EFlagIsSimulatingKeyEvent flag in iFlags to its original setting as soon as possible, hence the need for the iFlagClearer active-object
	CleanupStack::PopAndDestroy(2, iFlagClearer);
	}

TBool CCoeFep::CHighPriorityActive::MustBeAndNowWillBeDestroyedLater()
	{
	if (iRecursionLevel>0)
		{
		iFlags|=EFlagOwnsSelf;
		return ETrue;
		}

	return EFalse;
	}
	
void CCoeFep::CHighPriorityActive::DecrementRecursionLevelAndDestroySelfIfZeroAndOwnsSelf(TAny* aThis)
	{ // static
	CHighPriorityActive* const self=STATIC_CAST(CHighPriorityActive*, aThis);
	--self->iRecursionLevel;
	__ASSERT_DEBUG(self->iRecursionLevel>=0, Panic(EPanicBadRecursionLevel));

	if ((self->iRecursionLevel<=0) && (self->iFlags&EFlagOwnsSelf))
		delete self;
	}
		
CCoeFep::CHighPriorityActive::CHighPriorityActive(CCoeEnv& aConeEnvironment)
	:CActive(EActivePriorityWsEvents+20), // this priority must be less than CCoeEnvExtra::CHighPriorityActive's priority (a CONE class) so that FEPs receive notification of any change in focus (resulting from them calling HandleStartOfTransactionL) *before* any deferred function (i.e. one overriding MDeferredFunctionCall::ExecuteFunctionL) is called - this is necessary as the deferred function may depend on the deferred capabilities of the newly focused control (e.g. for inline editing)
	 iConeEnvironment(aConeEnvironment),
	 iFlags(0),
	 iDeferredFunctionCall(NULL),
	 iArrayOfKeyEvents(10),
	 iRecursionLevel(0)
	{
	CActiveScheduler::Add(this);
	}

void CCoeFep::CHighPriorityActive::ConstructL()
	{
	iFlagClearer=CFlagClearer::NewL();
	}

 void CCoeFep::CHighPriorityActive::MakeReadyToRun()
	{
	TRequestStatus* requestStatus=&iStatus;
	User::RequestComplete(requestStatus, KErrNone);
	SetActive();
	}

void CCoeFep::CHighPriorityActive::CancelFlagClearer(TAny* aFlagClearer)
	{
	CFlagClearer& flagClearer=*STATIC_CAST(CFlagClearer*, aFlagClearer);
	flagClearer.Cancel();
	}

void CCoeFep::CHighPriorityActive::TurnOffFlagIsSimulatingKeyEvent(TAny* aStackedFlags)
	{
	DoTurnOffFlagIsSimulatingKeyEvent(*STATIC_CAST(SStackedFlags*, aStackedFlags));
	}

void CCoeFep::CHighPriorityActive::DoTurnOffFlagIsSimulatingKeyEvent(SStackedFlags& aStackedFlags)
	{
	aStackedFlags.iFlags&=~EFlagIsSimulatingKeyEvent;
	aStackedFlags.iFlags|=(aStackedFlags.iOldFlags&EFlagIsSimulatingKeyEvent);
	}

void CCoeFep::CHighPriorityActive::DoCancel()
	{
	}

void CCoeFep::CHighPriorityActive::RunL()
	{
	if (iDeferredFunctionCall!=NULL)
		{
		if (iArrayOfKeyEvents.Count()>0)
			MakeReadyToRun();

		MDeferredFunctionCall* deferredFunctionCall=iDeferredFunctionCall;
		iDeferredFunctionCall=NULL;
		deferredFunctionCall->ExecuteFunctionL();
		}
	else
		{
		const TKeyEvent keyEvent=iArrayOfKeyEvents[0];
		iArrayOfKeyEvents.Remove(0);
		if (iArrayOfKeyEvents.Count()>0)
			MakeReadyToRun();

		++iRecursionLevel;
		iFlags|=EFlagRecursionLevelIncremented;
		DoSendKeyEventToApplicationNowL(EEventKey, keyEvent);

		const TBool handleCompletionOfTransaction=(iArrayOfKeyEvents.Count()==0);
		CCoeEnv& coneEnvironment=iConeEnvironment;
		iFlags&=~EFlagRecursionLevelIncremented;
		DecrementRecursionLevelAndDestroySelfIfZeroAndOwnsSelf(this);

		// the "this" object may have been deleted from this point onwards, so we can't access any member objects or call any other functions that will access any member objects
		if (handleCompletionOfTransaction)
			coneEnvironment.ForEachFepObserverCall(FepObserverHandleCompletionOfTransactionL);
		}
	}

TInt CCoeFep::CHighPriorityActive::RunError(TInt aError)
	{
	if (iFlags&EFlagRecursionLevelIncremented)
		{
		iFlags&=~EFlagRecursionLevelIncremented;
		DecrementRecursionLevelAndDestroySelfIfZeroAndOwnsSelf(this);
		}

	return aError;
	}
	
// CCoeFep::CHighPriorityActive::CFlagClearer

CCoeFep::CHighPriorityActive::CFlagClearer* CCoeFep::CHighPriorityActive::CFlagClearer::NewL()
	{
	return new(ELeave) CFlagClearer;
	}

CCoeFep::CHighPriorityActive::CFlagClearer::~CFlagClearer()
	{
	Cancel();
	}

void CCoeFep::CHighPriorityActive::CFlagClearer::TurnOffFlagIsSimulatingKeyEventDeferred(SStackedFlags& aStackedFlags)
	{
	Cancel();
	TRequestStatus* requestStatus=&iStatus;
	User::RequestComplete(requestStatus, KErrNone);
	SetActive();
	iStackedFlags=&aStackedFlags;
	}

CCoeFep::CHighPriorityActive::CFlagClearer::CFlagClearer()
	:CActive(EActivePriorityWsEvents-1), // the priority of CHighPriorityActive objects just needs to be less than the priority with which key-events are handled
	 iStackedFlags(NULL)
	{
	CActiveScheduler::Add(this);
	}

void CCoeFep::CHighPriorityActive::CFlagClearer::DoCancel()
	{
	iStackedFlags=NULL;
	}

void CCoeFep::CHighPriorityActive::CFlagClearer::RunL()
	{
	DoTurnOffFlagIsSimulatingKeyEvent(*iStackedFlags);
	iStackedFlags=NULL;
	}


// MFepPointerEventHandlerDuringInlineEdit

EXPORT_C void MFepPointerEventHandlerDuringInlineEdit::MFepPointerEventHandlerDuringInlineEdit_Reserved_1()
	{
	}

EXPORT_C void MFepPointerEventHandlerDuringInlineEdit::MFepPointerEventHandlerDuringInlineEdit_Reserved_2()
	{
	}

// MFepInlineTextFormatRetriever

EXPORT_C void MFepInlineTextFormatRetriever::MFepInlineTextFormatRetriever_Reserved_1()
	{
	}

EXPORT_C void MFepInlineTextFormatRetriever::MFepInlineTextFormatRetriever_Reserved_2()
	{
	}

// MCoeFepAwareTextEditor_Extension1

/**
 Starts a FEP inline editing transaction.
 
 Before starting the transaction, sets the range of characters in the
 text editor which should be selected using <code>aCursorSelection</code>,
 without visually highlighting the selected characters.
 
 This method can be used to avoid flicker when selecting a range of characters
 followed by starting a FEP inline editing transaction.
 
 Inserts a descriptor containing the initial inline text into the text
 editor. The inline text should normally replace any selected text.
 
 @param     "TBool& aSetToTrue"
            The implementor of this method must set
            <code>aSetToTrue</code> to <code>ETrue</code>
 @param     "const TCursorSelection& aCursorSelection"
            Contains the cursor and anchor positions for the
            selection.
 @param     "const TDesC& aInitialInlineText"
            The inline text to insert into the text editor.
 @param     "TInt aPositionOfInsertionPointInInlineText"
            An insertion position within the inline text. This is
            an offset from the start of the inline text.
 @param     "TBool aCursorVisibility"
           <code>ETrue</code> for visible text cursor,
            <code>EFalse</code> for invisible text cursor in the
            text editor.
 @param     "const MFormCustomDraw* aCustomDraw"
            Pointer to a custom drawing object. May be used to do
            advanced formatting of the inline text. This parameter
            is optional - a <code>NULL</code> pointer may be
            specified.
 @param     "MFepInlineTextFormatRetriever&
			aInlineTextFormatRetriever"
             Defines a single member function,
             <code>GetFormatOfFepInlineText()</code> which is used
             by the text editor to find out the formatting to apply
             to the inline text. It is also possible to apply
             different formatting to different parts of the inline
             text.
 @param     "MFepPointerEventHandlerDuringInlineEdit&
			aPointerEventHandlerDuringInlineEdit"
             Defines a single function,
            <code>HandlePointerEventInInlineTextL</code>() which
             is called when a pointer event is received within the
             inline text. This function might update the cursor
             position within the inline text and do text selection.
 */
EXPORT_C void MCoeFepAwareTextEditor_Extension1::StartFepInlineEditL(TBool& /*aSetToTrue*/, const TCursorSelection&, const TDesC&, TInt, TBool, const MFormCustomDraw*, MFepInlineTextFormatRetriever&, MFepPointerEventHandlerDuringInlineEdit&)
// StartFepInlineEditL - By not setting aSetToTrue to ETrue, we're explicitly saying that this method is not implemented here
	{
	}

/** 
By not setting aSetToTrue to ETrue, we're explicitly saying that this method is not implemented here

@param aSetToTrue  set to ETrue if method implemented.
*/
EXPORT_C void MCoeFepAwareTextEditor_Extension1::SetCursorType(TBool& /*aSetToTrue*/, const TTextCursor&)
	{
	}

/**
By not setting aSetToTrue to ETrue, we're explicitly saying that this method is not implemented here

@param  aSetToTrue is set to Etrue if the method implemented.
*/
EXPORT_C MCoeFepLayDocExtension* MCoeFepAwareTextEditor_Extension1::GetFepLayDocExtension(TBool& /*aSetToTrue*/)
	{
	return NULL;
	}


EXPORT_C void MCoeFepAwareTextEditor_Extension1::MCoeFepAwareTextEditor_Extension1_Reserved_4()
	{
	}

EXPORT_C void MCoeFepLayDocExtension::MCoeFepLayDocExtension_Reserved_1()
	{
	}

EXPORT_C void MCoeFepLayDocExtension::MCoeFepLayDocExtension_Reserved_2()
	{
	}


// MCoeFepAwareTextEditor_Extension1::CState

/** Empty default constructor. */
EXPORT_C MCoeFepAwareTextEditor_Extension1::CState::CState()
	{
	}

/** Empty second phase base class constructor. 

This function should be called from derived classes at the beginning of their 
ConstructL() even though it is currently empty. This is because this class may 
be extended in future to own resources, which will be allocated in BaseConstructL(). */
EXPORT_C void MCoeFepAwareTextEditor_Extension1::CState::BaseConstructL()
	{
	}

/** Empty virtual destructor. 
	
This is present because the class may be extended in the future to own resources. */
EXPORT_C MCoeFepAwareTextEditor_Extension1::CState::~CState()
	{
	}

EXPORT_C void MCoeFepAwareTextEditor_Extension1::CState::CState_Reserved_1()
	{
	}

EXPORT_C void MCoeFepAwareTextEditor_Extension1::CState::CState_Reserved_2()
	{
	}

EXPORT_C void MCoeFepAwareTextEditor_Extension1::CState::CState_Reserved_3()
	{
	}

EXPORT_C void MCoeFepAwareTextEditor_Extension1::CState::CState_Reserved_4()
	{
	}

// MCoeFepAwareTextEditor

/** Commits the inline text to the document. 

This function's implementation calls the text editor's implementation of 
DoCommitFepInlineEditL() then calls HandleCompletionOfTransactionL() 
for each FEP observer which has been added to the control environment's FEP 
observer list (see CCoeEnv::AddFepObserverL()).

@param aConeEnvironment The control's environment. */
EXPORT_C void MCoeFepAwareTextEditor::CommitFepInlineEditL(CCoeEnv& aConeEnvironment)
	{
	DoCommitFepInlineEditL();
	aConeEnvironment.ForEachFepObserverCall(FepObserverHandleCompletionOfTransactionL);
	}

/** Returns a pointer to an instance of the interface class MCoeFepAwareTextEditor_Extension1, 
or NULL, if the interface is not supported. 

Calls the private virtual function of the same name.

@return A pointer to an instance of the interface class MCoeFepAwareTextEditor_Extension1, 
or NULL, if the interface is not supported. */
EXPORT_C MCoeFepAwareTextEditor_Extension1* MCoeFepAwareTextEditor::Extension1()
	{
	TBool setToTrue=EFalse;
	MCoeFepAwareTextEditor_Extension1* const extension1=Extension1(setToTrue);
	return setToTrue? extension1: NULL;
	}


/** This private function should be overridden by text editors which support the 
MCoeFepAwareTextEditor_Extension1 interface. 

The implementation of this function should simply return a pointer to itself 
(this), and set aSetToTrue to ETrue. If not overridden, the function returns 
NULL to indicate that the interface is not supported. Called by the public 
overload of MCoeFepAwareTextEditor::Extension1(). 

@param aSetToTrue This should always be set to ETrue.
@return A pointer to the object for which the function is invoked (this). 
@publishedAll
@released */
EXPORT_C MCoeFepAwareTextEditor_Extension1* MCoeFepAwareTextEditor::Extension1(TBool& aSetToTrue)
	{
	aSetToTrue=ETrue;
	return NULL;
	}

EXPORT_C void MCoeFepAwareTextEditor::MCoeFepAwareTextEditor_Reserved_2()
	{
	}

// MCoeCaptionRetrieverForFep

EXPORT_C void MCoeCaptionRetrieverForFep::MCoeCaptionRetrieverForFep_Reserved_1()
	{
	}

EXPORT_C void MCoeCaptionRetrieverForFep::MCoeCaptionRetrieverForFep_Reserved_2()
	{
	}

