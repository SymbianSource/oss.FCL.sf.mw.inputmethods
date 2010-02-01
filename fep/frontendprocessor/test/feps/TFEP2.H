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

#if !defined(__TFEP2_H__)
#define __TFEP2_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__GDI_H__)
#include <gdi.h>
#endif

#if !defined(__W32STD_H__)
#include <w32std.h>
#endif

#if !defined(__COEMAIN_H__)
#include <coemain.h>
#endif

#if !defined(__COECNTRL_H__)
#include <coecntrl.h>
#endif

#if !defined(__FEPBASE_H__)
#include <fepbase.h>
#endif

#if !defined(__EIKDIALG_H__)
#include <techview/eikdialg.h>
#endif

class CTstFep;

class CTstScribbleWindow : public CCoeControl // this needs to be a separate control as it needs to be a window with "pointer-buffers" enabled
	{
public:
	static CTstScribbleWindow* NewL(CTstFep& aFep, RWindowTreeNode& aParent);
	virtual ~CTstScribbleWindow();
	void SetPointerBufferEnabled(TBool aPointerBufferEnabled);
	void HandleTimeOutL();
	void CancelTransactionAndDrawNow();
	void CancelTransaction(TBool& aChangeWasMade);
	// from CCoeControl
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	virtual void HandlePointerBufferReadyL();
private:
	typedef CArrayFix<TPoint> CPolyLine;
	enum {ENumberOfPointsInBuffer=32};
	enum
		{
		EFlagPointerIsDown					=0x00000001,
		EFlagPointerBufferActuallyEnabled	=0x00000002
		};
	class CTimeOutTimer : public CTimer
		{
	public:
		static CTimeOutTimer* NewL(CTstScribbleWindow& aScribbleWindow);
		virtual ~CTimeOutTimer();
	private:
		CTimeOutTimer(CTstScribbleWindow& aScribbleWindow);
		virtual void RunL();
	private:
		CTstScribbleWindow& iScribbleWindow;
		};
private:
	CTstScribbleWindow(CTstFep& aFep);
	void ConstructL(RWindowTreeNode& aParent);
	// from CCoeControl
	virtual void Draw(const TRect& aRect) const;
private:
	CTstFep& iFep;
	TUint iFlags;
	RPointerArray<CPolyLine> iArrayOfPolyLines;
	CTimeOutTimer* iTimeOutTimer;
	};

struct STstBitmapHandles;
class CFbsBitmap;

class CTstWholeScreenScribbleArea : public CBase
	{
public:
	static CTstWholeScreenScribbleArea* NewL(RWsSession& aWindowServerSession, RWindowGroup& aWindowGroup, const TSize& aScreenSize, TDisplayMode aDisplayMode, const TDesC& aDllName);
	~CTstWholeScreenScribbleArea();
	inline void RequestNotificationOfStartOfTransaction(TRequestStatus& aRequestStatus) {iHandWritingRecognizer.RequestNotificationOfStartOfTransaction(aRequestStatus);}
	inline void CancelRequestForNotificationOfStartOfTransaction() {iHandWritingRecognizer.CancelRequestForNotificationOfStartOfTransaction();}
	inline void RequestCharacters(TRequestStatus& aRequestStatus, TDes8& aCharacterBuffer, TBool aUpperCase) {iHandWritingRecognizer.RequestCharacters(aRequestStatus, aCharacterBuffer, aUpperCase);}
	inline void CancelRequestForCharacters() {iHandWritingRecognizer.CancelRequestForCharacters();}
	inline void SetUpperCase(TBool aUpperCase) {iHandWritingRecognizer.SetUpperCase(aUpperCase);}
private:
	enum
		{
		EFlagMutexIsConstructed=0x00000001
		};
	class RHandWritingRecognizer : public RAnim
		{
	public:
		inline RHandWritingRecognizer() {}
		inline RHandWritingRecognizer(RAnimDll& aAnimDll) :RAnim(aAnimDll) {}
		void ConstructL(const RWsSprite& aSprite, STstBitmapHandles& aBitmapHandles);
		void FinishConstructionL();
		void RequestNotificationOfStartOfTransaction(TRequestStatus& aRequestStatus);
		void CancelRequestForNotificationOfStartOfTransaction();
		void RequestCharacters(TRequestStatus& aRequestStatus, TDes8& aCharacterBuffer, TBool aUpperCase);
		void CancelRequestForCharacters();
		void SetUpperCase(TBool aUpperCase);
		};
private:
	CTstWholeScreenScribbleArea(RWsSession& aWindowServerSession);
	void ConstructL(RWindowGroup& aWindowGroup, const TSize& aScreenSize, TDisplayMode aDisplayMode, const TDesC& aDllName);
	static CFbsBitmap* CreateBitmapL(TInt aHandleOfBitmapToUse, const TSize& aScreenSize, TDisplayMode aDisplayMode);
	static void SignalMutex(TAny* aMutex);
private:
	TUint iFlags;
	RMutex iMutex;
	TSpriteMember iSpriteMember;
	RAnimDll iDll;
	RWsSprite iSprite;
	RHandWritingRecognizer iHandWritingRecognizer;
	};

class CFont;

class CTstControl : public CCoeControl
	{
public:
	static CTstControl* NewL(CTstFep& aFep);
	virtual ~CTstControl();
	void CancelTransaction();
	void IsOnHasChangedState();
	void OfferPointerEventL(CCoeFep::TEventResponse& aEventResponse, const TPointerEvent& aPointerEvent, const CCoeControl* aWindowOwningControl);
	void OfferPointerBufferReadyEventL(CCoeFep::TEventResponse& aEventResponse, const CCoeControl* aWindowOwningControl);
	inline TBool UpperCase() const {return (iFlags&EFlagUpperCase);}
	static TInt NumberOfAttributes();
	static TUid AttributeAtIndex(TInt aIndex);
	void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const;
	void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream);
	static void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream, TBool aUpperCase, TBool aWholeScreen, TBool aPointerBufferEnabled);
	static void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream, TBool& aUpperCase, TBool& aWholeScreen, TBool& aPointerBufferEnabled);
	void HandleGainingForeground();
	void HandleLosingForeground();
	void HandleChangeInFocus();
	void HandleDestructionOfFocusedItem();
private:
	typedef void (*FChangeFunction)(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	enum {EArrowHeadSize=4};
	enum {EMaximumLengthOfDisplayOfContextInformation=15};
	enum
		{
		ETstUpperCaseUid=0x10003eab,
		ETstWholeScreenUid=0x1000550c,
		ETstPointerBufferEnabledUid=0x10004d18
		};
	enum
		{
		EFlagForeground						=0x00000001,
		EFlagWindowIsBeingDragged			=0x00000002,
		EFlagUpperCase						=0x00000004,
		EFlagWholeScreen					=0x00000008,
		EFlagPointerBufferEnabled			=0x00000010,
		EFlagActuallyWholeScreen			=0x00000020
		};
	enum
		{
		EHeightOfScribbleWindow	=60,
		EGapLeftOfEachLine		=1,
		EGapAboveTopLine		=1,
		EGapBetweenEachLine		=1,
		EGapBelowBottomLine		=1
		};
	class CHandlerForStartOfTransaction : public CActive
		{
	public:
		static CHandlerForStartOfTransaction* NewL(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CCoeEnv& aConeEnvironment);
		virtual ~CHandlerForStartOfTransaction();
	private:
		CHandlerForStartOfTransaction(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CCoeEnv& aConeEnvironment);
		void RequestNotificationOfStartOfTransaction();
		virtual void DoCancel();
		virtual void RunL();
	private:
		CTstWholeScreenScribbleArea& iWholeScreenScribbleArea;
		CCoeEnv& iConeEnvironment;
		};
	class CHandlerForCharacters : public CActive
		{
	public:
		static CHandlerForCharacters* NewL(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CTstFep& aFep, TBool aForeground);
		virtual ~CHandlerForCharacters();
		inline void SetWholeScreen(TBool aWholeScreen) {SetFlagAffectingWhetherActive(EFlagWholeScreen, aWholeScreen);}
		inline void SetIsOn(TBool aIsOn) {SetFlagAffectingWhetherActive(EFlagIsOn, aIsOn);}
		inline void SetForeground(TBool aForeground) {SetFlagAffectingWhetherActive(EFlagForeground, aForeground);}
		void SetUpperCase(TBool aUpperCase);
	private:
		enum {EMaximumLengthOfCharacterBuffer=4*sizeof(TUint)};
		enum
			{
			EFlagWholeScreen	=0x00000001,
			EFlagIsOn			=0x00000002,
			EFlagForeground		=0x00000004,
			EFlagUpperCase		=0x00000008
			};
	private:
		CHandlerForCharacters(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CTstFep& aFep, TBool aForeground);
		void SetFlagAffectingWhetherActive(TUint aFlagAffectingWhetherActive, TBool aSetting); // aFlag may be passed EFlagWholeScreen, EFlagIsOn or EFlagForeground
		void RequestCharacters();
		static TInt NumberOfCharactersInBuffer(const CBase* aCharacterBuffer); // aCharacterBuffer is really a TDesC8
		static const TAny* CharacterInBuffer(const CBase* aCharacterBuffer, TInt aIndex); // aCharacterBuffer is really a TDesC8
		virtual void DoCancel();
		virtual void RunL();
	private:
		CTstWholeScreenScribbleArea& iWholeScreenScribbleArea;
		CTstFep& iFep;
		TUint iFlags;
		TBuf8<EMaximumLengthOfCharacterBuffer> iCharacterBuffer;
		};
private:
	CTstControl(CTstFep& aFep);
	void ConstructL();
	static void SetForeground(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetFocus(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetUpperCase(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetWholeScreen(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetPointerBufferEnabled(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetInputCapabilities(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetFlag(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter, TUint aFlag);
	void DoSetWholeScreen(TBool aWholeScreen);
	void ChangeSetupAndDrawNow(FChangeFunction aChangeFunction, TInt aParameter=0);
public:
	// from CCoeControl
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode);
private:
	// from CCoeControl
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	virtual TInt CountComponentControls() const;
	virtual CCoeControl* ComponentControl(TInt aIndex) const;
	virtual void Draw(const TRect& aRect) const;
private:
	CTstFep& iFep;
	TUint iFlags;
	TCoeInputCapabilities iInputCapabilities;
	CTstScribbleWindow* iScribbleWindow;
	CTstWholeScreenScribbleArea* iWholeScreenScribbleArea;
	CHandlerForStartOfTransaction* iHandlerForStartOfTransaction;
	CHandlerForCharacters* iHandlerForCharacters;
	CFont* iStatusFont;
	TPoint iPositionOnWindowBeingDragged;
	};

class CTstFep : public CCoeFep
	{
public:
	CTstFep(CCoeEnv& aConeEnvironment);
	void ConstructL(const CCoeFepParameters& aFepParameters);
	virtual ~CTstFep();
	inline void SimulateKeyEventsL(const TArray<TUint>& aArrayOfCharacters) {CCoeFep::SimulateKeyEventsL(aArrayOfCharacters);}
	inline void WriteAttributeDataAndBroadcastL(TUid aAttributeUid) {CCoeFep::WriteAttributeDataAndBroadcastL(aAttributeUid);}
	inline TBool IsOn() const {return CCoeFep::IsOn();}
	inline TBool UpperCase() const {return iControl->UpperCase();}
private:
	// from CCoeFep
	virtual void CancelTransaction();
	virtual void IsOnHasChangedState();
	virtual void OfferKeyEventL(TEventResponse& aEventResponse, const TKeyEvent& aKeyEvent, TEventCode aEventCode);
	virtual void OfferPointerEventL(TEventResponse& aEventResponse, const TPointerEvent& aPointerEvent, const CCoeControl* aWindowOwningControl);
	virtual void OfferPointerBufferReadyEventL(TEventResponse& aEventResponse, const CCoeControl* aWindowOwningControl);
	// from MFepAttributeStorer (via CCoeFep)
	virtual TInt NumberOfAttributes() const;
	virtual TUid AttributeAtIndex(TInt aIndex) const;
	virtual void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const;
	virtual void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream);
	// from MCoeForegroundObserver (via CCoeFep)
	virtual void HandleGainingForeground();
	virtual void HandleLosingForeground();
	// from MCoeFocusObserver (via CCoeFep)
	virtual void HandleChangeInFocus();
	virtual void HandleDestructionOfFocusedItem();
private:
	CTstControl* iControl;
	};

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

class CTstSettingsDialog : public CEikDialog, private MFepAttributeStorer
	{
public:
	CTstSettingsDialog();
private:
	TUint CheckBoxStateAsFlag(TInt aControlId, TUint aFlag) const;
	void SetCheckBoxState(TInt aControlId, TUint aFlag);
	// from CEikDialog
	virtual TBool OkToExitL(TInt aButtonId);
	virtual void PreLayoutDynInitL();
	// from MFepAttributeStorer
	virtual TInt NumberOfAttributes() const;
	virtual TUid AttributeAtIndex(TInt aIndex) const;
	virtual void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const;
	virtual void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream);
private:
	enum
		{
		EFlagUpperCase				=0x00000001,
		EFlagWholeScreen			=0x00000002,
		EFlagPointerBufferEnabled	=0x00000004
		};
private:
	TUint iFlags;
	TUid iUid;
	};

#endif

