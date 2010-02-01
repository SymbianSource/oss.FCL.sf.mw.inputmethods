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

#if !defined(__TFEP4_H__)
#define __TFEP4_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__W32STD_H__)
#include <w32std.h>
#endif

#if !defined(__FRMTLAY_H__)
#include <frmtlay.h>
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

#if !defined(__FEPITFR_H__)
#include <fepitfr.h>
#endif

#if !defined(__EIKDIALG_H__)
#include <techview/eikdialg.h>
#endif

#include "pytable.h"

class TTstTextBackup
	{
public:
	TTstTextBackup(TDes& aText);
	void PushOntoCleanupStackL();
	void PopOffCleanupStack();
private:
	static void Cleanup(TAny* aTextBackup);
private:
	TCleanupItem iCleanupItem;
	TDes& iOriginal;
	HBufC* iBackup;
	};

class CTstInsertionPoint : public CBase
	{
public:
	enum
		{
		EWidth=5,
		EHeight=3
		};
public:
	static CTstInsertionPoint* NewL(RWindowBase& aWindow, CCoeEnv& aCoeEnv);
	virtual ~CTstInsertionPoint();
	void SetPosition(const TPoint& aPosition);
	void SetOn(TBool aOn);
private:
	enum
		{
		EFlagOn=0x00000001
		};
private:
	CTstInsertionPoint(RWsSession& aWindowServerSession);
	void ConstructL(RWindowBase& aWindow, TDisplayMode aDisplayMode);
	static CFbsBitmap* CreateBitmapL(TDisplayMode aDisplayMode);
	void DoSetOn(TBool aOn);
private:
	TSpriteMember iSpriteMember;
	RWsSprite iSprite;
	TPoint iPosition;
	TUint iFlags;
	};

class TTstArrayOfOneCtrlCharacter : private CCoeFep::MModifiedCharacter
	{
public:
	TTstArrayOfOneCtrlCharacter(TUint aCharacter);
	TArray<CCoeFep::MModifiedCharacter> ArrayOfModifiedCharacters(); // this function is deliberately non-const so that its client is not tempted to create an anonymous (temporary) TTstArrayOfOneCtrlCharacter object whose lifetime may turn out to be too short
private:
	static TInt NumberOfModifiedCharacters(const CBase* aThis); // aThis is really a TTstArrayOfOneCtrlCharacter
	static const TAny* ModifiedCharacter(const CBase* aThis, TInt aIndex); // aThis is really a TTstArrayOfOneCtrlCharacter
	// from CCoeFep::MModifiedCharacter
	virtual TUint CharacterCode() const;
	virtual TUint ModifierMask() const;
	virtual TUint ModifierValues() const;
private:
	TUint iCharacter;
	};

class CFont;
class TCharFormat;
class CTstFep;

class CTstControl : public CCoeControl, private MFormCustomDraw, private MFepInlineTextFormatRetriever, private MFepPointerEventHandlerDuringInlineEdit, private CCoeFep::MDeferredFunctionCall
	{
public:
	static CTstControl* NewL(CTstFep& aFep);
	virtual ~CTstControl();
	void CancelTransaction();
	void IsOnHasChangedState();
	void OfferPointerEventL(CCoeFep::TEventResponse& aEventResponse, const TPointerEvent& aPointerEvent, const CCoeControl* aWindowOwningControl);
	static TInt NumberOfAttributes();
	static TUid AttributeAtIndex(TInt aIndex);
	void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const;
	void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream);
	static void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream, TBool aInlineEditingEnabled, TInt aInputMethod);
	static void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream, TBool& aInlineEditingEnabled, TInt& aInputMethod);
	void HandleGainingForeground();
	void HandleLosingForeground();
	void HandleChangeInFocus();
	void HandleDestructionOfFocusedItem();
private:
	typedef void (*FChangeFunction)(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	enum {EArrowHeadSize=4};
//	enum {ENumberOfHexadecimalDigitsPerCharacterCode=2*sizeof(TText)};
	enum {ENumberOfHexadecimalDigitsPerCharacterCode=5};
	enum
		{
		EMaximumLengthOfBuffer=25,
		EMaximumLengthOfDisplayOfContextInformation=15
		};
	enum
		{
		ETstInlineEditingEnabledUid=0x100014a2,
		ETstInputMethodUid=0x100014a1
		};
	enum
		{
		EInputMethodPlain,
		EInputMethodHexadecimalCharacterCode,
		EInputMethodPinYin,
		// the next one must always be the last value in the enum
		EInputMethodOnePastTheLast
		};
	enum
		{
		EFlagForeground						=0x00000001,
		EFlagWindowIsBeingDragged			=0x00000002,
		EFlagInlineEditingEnabled			=0x00000004,
		EFlagInsideInlineEditingTransaction	=0x00000008,
		EFlagHasNoCompositionWindow			=0x00000010
		};
	enum
		{
		EGapLeftOfEachLine			=1,
		EGapAboveCompositionLine	=1,
		EGapBelowCompositionLine	=CTstInsertionPoint::EHeight+1,
		EGapAboveTopStatusLine		=1,
		EGapBetweenEachStatusLine	=1,
		EGapBelowBottomStatusLine	=1
		};
	class CStateInformation : public MCoeFepAwareTextEditor_Extension1::CState
		{
	public:
		static CStateInformation* NewLC(TInt aMeaninglessNumber);
		virtual ~CStateInformation();
		inline void IncrementMeaninglessNumber() {++*iMeaninglessNumber;}
		inline TInt MeaninglessNumber() const {return *iMeaninglessNumber;}
	private:
		CStateInformation();
		void ConstructL(TInt aMeaninglessNumber);
	private:
		TInt* iMeaninglessNumber; // this TInt is not "in-line" as we want to test that CStateInformation's destructor gets called
		};
private:
	CTstControl(CTstFep& aFep);
	void ConstructL();
	static void SetForeground(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetFocus(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetFlagInlineEditingEnabled(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetInputMethod(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetInputCapabilities(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter);
	static void SetFlag(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter, TUint aFlag);
	void ChangeSetupAndResetBufferAndDrawNow(FChangeFunction aChangeFunction, TInt aParameter=0);
	void InsertCompositionCharacterAndDrawNowL(TUint aCharacterCode);
	void SetInlineTextL(MCoeFepAwareTextEditor& aFepAwareTextEditor);
	void CancelInlineEdit(MCoeFepAwareTextEditor& aFepAwareTextEditor);
	void CommitInlineEditL(MCoeFepAwareTextEditor& aFepAwareTextEditor);
	void ResetBuffer();
	void SetPositionOfInsertionPointInBuffer(TInt aPositionOfInsertionPointInBuffer);
	void SetPositionOfInsertionPointInBuffer(TInt aPositionOfInsertionPointInBuffer, TInt aPositionOfAnchorInBuffer);
	TPoint PositionOfInsertionPointOnWindow() const;
	TInt PositionInBuffer(TInt aX) const;
	TBool IsLegalNonDigitForRealNumber(const TCharUC& aKeyCodeInUpperCase) const;
	static void GetFormatAtDocumentPosition(TCharFormat& aFormat, TInt aDocumentPosition, const MCoeFepAwareTextEditor& aFepAwareTextEditor);
	static TInt NumberOfCharactersInBuffer(const CBase* aControl); // aControl is really a CTstControl
	static const TAny* CharacterInBuffer(const CBase* aControl, TInt aIndex); // aControl is really a CTstControl
public:
	// from CCoeControl
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode);
private:
	// from CCoeControl
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	virtual void Draw(const TRect& aRect) const;
	// from MFormCustomDraw
	virtual void DrawText(const TParam& aParam, const TLineInfo& aLineInfo, const TCharFormat& aFormat, const TDesC& aText, const TPoint& aTextOrigin, TInt aExtraPixels) const;
	// from MFepInlineTextFormatRetriever
	virtual void GetFormatOfFepInlineText(TCharFormat& aFormat, TInt& aNumberOfCharactersWithSameFormat, TInt aPositionOfCharacter) const;
	// from MFepPointerEventHandlerDuringInlineEdit
	virtual void HandlePointerEventInInlineTextL(TPointerEvent::TType aType, TUint aModifiers, TInt aPositionInInlineText);
	// from CCoeFep::MDeferredFunctionCall
	virtual void ExecuteFunctionL();
private:
	CTstFep& iFep;
	TInt iInputMethod;
	TUint iFlags;
	TUint iCharacterCode; // used by CCoeFep::MDeferredFunctionCall::ExecuteFunctionL, and also independently by CharacterInBuffer
	TBuf<EMaximumLengthOfBuffer> iBuffer;
	TCursorSelection iSelectedCompositionText;
	TCoeInputCapabilities iInputCapabilities;
	CFont* iCompositionFont;
	CFont* iStatusFont;
	CTstInsertionPoint* iInsertionPoint;
	TPoint iPositionOnWindowBeingDragged;
private:
	CPinyinSelector iPinYinSelector;
	};

class CTstFep : public CCoeFep
	{
public:
	CTstFep(CCoeEnv& aConeEnvironment);
	void ConstructL(const CCoeFepParameters& aFepParameters);
	virtual ~CTstFep();
	inline void MakeDeferredFunctionCall(MDeferredFunctionCall& aDeferredFunctionCall) {CCoeFep::MakeDeferredFunctionCall(aDeferredFunctionCall);}
	inline void SimulateKeyEventsL(const TArray<TUint>& aArrayOfCharacters) {CCoeFep::SimulateKeyEventsL(aArrayOfCharacters);}
	inline void SimulateKeyEventsL(const TArray<MModifiedCharacter>& aArrayOfModifiedCharacters) {CCoeFep::SimulateKeyEventsL(aArrayOfModifiedCharacters);}
	inline void WriteAttributeDataAndBroadcastL(TUid aAttributeUid) {CCoeFep::WriteAttributeDataAndBroadcastL(aAttributeUid);}
	inline TBool IsOn() const {return CCoeFep::IsOn();}
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
	// from CEikDialog
	virtual TBool OkToExitL(TInt aButtonId);
	virtual void PreLayoutDynInitL();
	// from MFepAttributeStorer
	virtual TInt NumberOfAttributes() const;
	virtual TUid AttributeAtIndex(TInt aIndex) const;
	virtual void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const;
	virtual void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream);
private:
	TBool iInlineEditingEnabled;
	TInt iInputMethod;
	TUid iUid;
	};
	


#endif

