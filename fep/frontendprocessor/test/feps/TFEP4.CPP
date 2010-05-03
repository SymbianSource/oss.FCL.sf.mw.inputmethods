// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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



/**  @file
     @internalComponent
*/ 

#include <e32std.h>
#include <e32base.h>
#include <e32keys.h>
#include <s32strm.h>
#include <gdi.h>
#include <txtfrmat.h>
#include <fbs.h>
#include <bitstd.h>
#include <bitdev.h>
#include <w32std.h>
#include <frmtlay.h>
#include <coemain.h>
#include <coeaui.h>
#include <coecntrl.h>
#include <coefepff.h>
#include <fepbase.h>
#include <fepitfr.h>
#include <techview/eikon.hrh>
#include <techview/eikdialg.h>
#include <techview/eikchkbx.h>
#include <techview/eikbutb.h>
#include <techview/eikchlst.h>
#include <gulutil.h>	// for FontUtils, together with egul.lib

#include "tfep4.hrh"
#include "TFEP4.H"
#include "TFEP4COM.H"

// constants

enum TPanic
	{
	EPanicBackupAlreadyExists=1,
	EPanicBadIndex1,
	EPanicBadIndex2,
	EPanicNotExpectedToBeSimulatingKeyEvent1,
	EPanicNotExpectedToBeSimulatingKeyEvent2,
	EPanicBadAttributeUid1,
	EPanicBadAttributeUid2,
	EPanicBadAttributeUid3,
	EPanicBadAttributeUid4,
	EPanicBadInputMethod1,
	EPanicBadInputMethod2,
	EPanicBadInputMethod3,
	EPanicBadInputMethod4,
	EPanicBadInputMethod5,
	EPanicBadTextLength1,
	EPanicBadTextLength2,
	EPanicBadKeyCode1,
	EPanicBadKeyCode2,
	EPanicBadKeyCode3,
	EPanicBadKeyCode4,
	EPanicBadCharacterInBuffer,
	EPanicInconsistentState,
	EPanicNegativeIndex,
	EPanicArithmeticConfusion,
	EPanicBadLengthOfTextBeforeSelection,
	EPanicSelectionExtendsPastEndOfDocument,
	EPanicBadLengthOfTextAfterSelection,
	EPanicBadLengthOfSelection,
	EPanicBadHeight,
	EPanicIsAlreadyActive,
	EPanicNoFepAwareTextEditorAlthoughCurrentlyInlineEditing,
	EPanicBadKeyResponse,
	EPanicUnexpectedButtonId,
	EPanicBadCheckBoxState,
	EPanicBadNumberOfAttributes
	};

_LIT(KLitTFEP4, "TFEP4");
#if defined(_UNICODE)
const TUint KEllipsisCharacter=0x2026;
_LIT(KLitCompositionFontTypefaceName, "publicDomainUnicode");
//_LIT(KLitCompositionFontTypefaceName, "SimHei");
//_LIT(KLitCompositionFontTypefaceName, "STFangSong");
#else
const TUint KEllipsisCharacter=0x85;
_LIT(KLitCompositionFontTypefaceName, "courier");
#endif
_LIT(KLitStatusFontTypefaceName, "SwissA");		// original "arial"
_LIT(KLitNoFocusedFepAwareTextEditor, "No focused FEP-aware text-editor");
_LIT(KLitNoFocusedFepAwareTextEditorSupportingState, "No focused FEP-aware text-editor supporting state");
_LIT(KLitNotAvailable, "[not available]");
//_LIT(KLitTooLong, "[too long]");
_LIT(KLitQuotationMark, "\"");
_LIT(KLitTextBeforeSelectionColonSpace, "Text before selection: ");
_LIT(KLitTextAfterSelectionColonSpace, "Text after selection: ");
_LIT(KLitSelectionColonSpace, "Selection: ");
_LIT(KLitInputMethodColonSpace, "Input method: ");
_LIT(KLitPlain, "plain");
_LIT(KLitHexadecimalCharacterCode, "hexadecimal character-code");
_LIT(KLitPinYin, "pin yin");
_LIT(KLitInlineEditingColonSpace, "Inline editing: ");
_LIT(KLitEnabled, "enabled");
_LIT(KLitDisabled, "disabled");
_LIT(KLitOpeningSquareBracket, "[");
_LIT(KLitCaptionColonSpace, "Caption: ");
_LIT(KLitStateColonSpace, "State: ");
_LIT(KLitInputCapabilitiesColonSpace, "Input-capabilities: ");
_LIT(KLitNone, "none");
_LIT(KLitWesternNumericIntegerPositive, "Western numeric integer positive");
_LIT(KLitWesternNumericIntegerNegative, "Western numeric integer negative");
_LIT(KLitWesternNumericReal, "Western numeric real");
_LIT(KLitWesternAlphabetic, "Western alphabetic");
_LIT(KLitJapaneseHiragana, "Japanese hiragana");
_LIT(KLitJapaneseKatakanaHalfWidth, "Japanese katakana half-width");
_LIT(KLitJapaneseKatakanaFullWidth, "Japanese katakana full-width");
_LIT(KLitDialableCharacters, "dialable characters");
_LIT(KLitSecretText, "secret text");
_LIT(KLitAllText, "all text");
_LIT(KLitNavigation, "navigation");
_LIT(KLitExtensionsColonSpace, "extensions: ");
_LIT(KLitPhoneticAlphabet, "phonetic alphabet");
_LIT(KLitCommaSpace, ", ");
_LIT(KLitClosingSquareBracket, "]");

LOCAL_D const TText* const KPhoneticAlphabet[]=
	{
	_S("Alpha"),
	_S("Bravo"),
	_S("Charlie"),
	_S("Delta"),
	_S("Echo"),
	_S("Foxtrot"),
	_S("Golf"),
	_S("Hotel"),
	_S("India"),
	_S("Juliette"),
	_S("Kilo"),
	_S("Lima"),
	_S("Mike"),
	_S("November"),
	_S("Oscar"),
	_S("Papa"),
	_S("Quebec"),
	_S("Romeo"),
	_S("Sierra"),
	_S("Tango"),
	_S("Uniform"),
	_S("Victor"),
	_S("Whiskey"),
	_S("X-Ray"),
	_S("Yankee"),
	_S("Zulu")
	};

// local and global functions
LOCAL_C inline TBool IsLowSurrogate(TText16 aInt16)
/**
@return True, if aText16 is low surrogate; false, otherwise.
*/
    {
    return (aInt16 & 0xFC00) == 0xDC00;
    }

LOCAL_C void Panic(TPanic aPanic)
	{
	User::Panic(KLitTFEP4, aPanic);
	}

GLDEF_C TInt E32Dll(
					)
	{
	return KErrNone;
	}

// TTstTextBackup

#pragma warning(disable: 4355) // "'this' : used in base member initializer list"

TTstTextBackup::TTstTextBackup(TDes& aText)
	:iCleanupItem(Cleanup, this),
	 iOriginal(aText),
	 iBackup(NULL)
	{
	}

#pragma warning(default: 4355)

void TTstTextBackup::PushOntoCleanupStackL()
	{
	__ASSERT_DEBUG(iBackup==NULL, Panic(EPanicBackupAlreadyExists));
	iBackup=iOriginal.AllocL();
	CleanupStack::PushL(iCleanupItem);
	}

void TTstTextBackup::PopOffCleanupStack()
	{
	delete iBackup;
	CleanupStack::Pop();
	}

void TTstTextBackup::Cleanup(TAny* aTextBackup)
	{
	TTstTextBackup* const textBackup=STATIC_CAST(TTstTextBackup*, aTextBackup);
	textBackup->iOriginal=*textBackup->iBackup;
	delete textBackup->iBackup;
	}

// CTstInsertionPoint

CTstInsertionPoint* CTstInsertionPoint::NewL(RWindowBase& aWindow, CCoeEnv& aConeEnvironment)
	{
	CTstInsertionPoint* const insertionPoint=new(ELeave) CTstInsertionPoint(aConeEnvironment.WsSession());
	CleanupStack::PushL(insertionPoint);
	insertionPoint->ConstructL(aWindow, aConeEnvironment.ScreenDevice()->DisplayMode());
	CleanupStack::Pop(); // insertionPoint
	return insertionPoint;
	}

CTstInsertionPoint::~CTstInsertionPoint()
	{
	iSprite.Close();
	delete iSpriteMember.iBitmap; // must be deleted after iSprite is closed as iSprite has a reference to it
	// iSpriteMember.iMaskBitmap is not deleted as it is the same as iSpriteMember.iBitmap
	}

void CTstInsertionPoint::SetPosition(const TPoint& aPosition)
	{
	iPosition=aPosition;
	if (iFlags&EFlagOn)
		{
		iSprite.SetPosition(iPosition);
		}
	}

void CTstInsertionPoint::SetOn(TBool aOn)
	{
	if (!aOn!=!(iFlags&EFlagOn)) // fold non-zero values on both sides before comparing for inequality
		{
		DoSetOn(aOn);
		}
	}

CTstInsertionPoint::CTstInsertionPoint(RWsSession& aWindowServerSession)
	:iSprite(aWindowServerSession),
	 iPosition(0, 0),
	 iFlags(0)
	{
	iSpriteMember.iBitmap=NULL;
	iSpriteMember.iMaskBitmap=NULL;
	}

void CTstInsertionPoint::ConstructL(RWindowBase& aWindow, TDisplayMode aDisplayMode)
	{
	iSpriteMember.iBitmap=CreateBitmapL(aDisplayMode);
	iSpriteMember.iMaskBitmap=iSpriteMember.iBitmap;
	iSpriteMember.iInvertMask=ETrue;
	iSpriteMember.iDrawMode=CGraphicsContext::EDrawModePEN;
	iSpriteMember.iOffset.iX=-(CTstInsertionPoint::EWidth/2);
	iSpriteMember.iOffset.iY=0;
	iSpriteMember.iInterval=0;
	User::LeaveIfError(iSprite.Construct(aWindow, iPosition, 0));
	User::LeaveIfError(iSprite.AppendMember(iSpriteMember));
	DoSetOn(EFalse);
	User::LeaveIfError(iSprite.Activate());
	}

CFbsBitmap* CTstInsertionPoint::CreateBitmapL(TDisplayMode aDisplayMode)
	{
	CFbsBitmap* const bitmap=new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(TSize(CTstInsertionPoint::EWidth, CTstInsertionPoint::EHeight), aDisplayMode));
	CFbsBitmapDevice* const bitmapDevice=CFbsBitmapDevice::NewL(bitmap);
	CleanupStack::PushL(bitmapDevice);
	CFbsBitGc* const graphicsContext=CFbsBitGc::NewL();
	CleanupStack::PushL(graphicsContext);
	graphicsContext->Activate(bitmapDevice);
	graphicsContext->SetBrushStyle(CGraphicsContext::ESolidBrush);
	graphicsContext->SetBrushColor(KRgbWhite);
	graphicsContext->SetPenStyle(CGraphicsContext::ESolidPen);
	graphicsContext->SetPenColor(KRgbBlack);
	graphicsContext->Clear();
	const TPoint bottomLeft(0, CTstInsertionPoint::EHeight-1);
	const TPoint bottomRight(CTstInsertionPoint::EWidth-1, CTstInsertionPoint::EHeight-1);
	const TPoint top(CTstInsertionPoint::EWidth/2, 0);
	graphicsContext->DrawLine(bottomLeft, bottomRight);
	graphicsContext->DrawLine(bottomRight, top);
	graphicsContext->DrawLine(top, bottomLeft);
	CleanupStack::PopAndDestroy(2); // graphicsContext and bitmapDevice
	CleanupStack::Pop(); // bitmap
	return bitmap;
	}

void CTstInsertionPoint::DoSetOn(TBool aOn)
	{
	iSprite.SetPosition(aOn? iPosition: TPoint(-(EWidth*4), -(EHeight*4)));
	if (aOn)
		{
		iFlags|=EFlagOn;
		}
	else
		{
		iFlags&=~EFlagOn;
		}
	}

// TTstArrayOfOneCtrlCharacter

TTstArrayOfOneCtrlCharacter::TTstArrayOfOneCtrlCharacter(TUint aCharacter)
	:iCharacter(aCharacter)
	{
	}

TArray<CCoeFep::MModifiedCharacter> TTstArrayOfOneCtrlCharacter::ArrayOfModifiedCharacters()
	{
	return TArray<CCoeFep::MModifiedCharacter>(NumberOfModifiedCharacters, ModifiedCharacter, (const CBase*)this);
	}

TInt TTstArrayOfOneCtrlCharacter::NumberOfModifiedCharacters(const CBase*)
	{
	return 1;
	}

const TAny* TTstArrayOfOneCtrlCharacter::ModifiedCharacter(const CBase* aThis, TInt aIndex)
	{
	__ASSERT_ALWAYS(aIndex==0, Panic(EPanicBadIndex1));
	return aThis;
	}

TUint TTstArrayOfOneCtrlCharacter::CharacterCode() const
	{
	return iCharacter;
	}

TUint TTstArrayOfOneCtrlCharacter::ModifierMask() const
	{
	return EModifierCtrl|EModifierShift|EModifierLeftShift|EModifierRightShift;
	}

TUint TTstArrayOfOneCtrlCharacter::ModifierValues() const
	{
	return EModifierCtrl;
	}

// CTstControl

CTstControl* CTstControl::NewL(CTstFep& aFep)
	{
	CTstControl* const control=new(ELeave) CTstControl(aFep);
	CleanupStack::PushL(control);
	control->ConstructL();
	CleanupStack::Pop(); // control
	return control;
	}

CTstControl::~CTstControl()
	{
	if (iFlags&EFlagInsideInlineEditingTransaction)
		{
		CancelInlineEdit(*iInputCapabilities.FepAwareTextEditor());
		}
	iCoeEnv->ReleaseScreenFont(iCompositionFont);
	iCoeEnv->ReleaseScreenFont(iStatusFont);
	STATIC_CAST(CCoeAppUi*, iCoeEnv->AppUi())->RemoveFromStack(this);
	delete iInsertionPoint;
	}

void CTstControl::CancelTransaction()
	{
	__ASSERT_DEBUG(!iFep.IsSimulatingKeyEvent() || ((~iFlags&EFlagInsideInlineEditingTransaction) && (iBuffer.Length()==0)), Panic(EPanicNotExpectedToBeSimulatingKeyEvent1));
	TBool needToDraw=EFalse;
	if (iFlags&EFlagInsideInlineEditingTransaction)
		{
		CancelInlineEdit(*iInputCapabilities.FepAwareTextEditor());
		needToDraw=ETrue;
		}
	if (iBuffer.Length()>0)
		{
		ResetBuffer();
		needToDraw=ETrue;
		}
	if (needToDraw)
		{
		DrawNow();
		}
	}

void CTstControl::IsOnHasChangedState()
	{
	ChangeSetupAndResetBufferAndDrawNow(NULL);
	}

void CTstControl::OfferPointerEventL(CCoeFep::TEventResponse& aEventResponse, const TPointerEvent& aPointerEvent, const CCoeControl* aWindowOwningControl)
	{
	// this function must correctly set aEventResponse *before* calling anything that can leave
	if (aWindowOwningControl==this)
		{
		aEventResponse=CCoeFep::EEventWasConsumed;
		HandlePointerEventL(aPointerEvent);
		}
	else
		{
		aEventResponse=CCoeFep::EEventWasNotConsumed;
		}
	}

TInt CTstControl::NumberOfAttributes()
	{
	return 2;
	}

TUid CTstControl::AttributeAtIndex(TInt aIndex)
	{
	switch (aIndex)
		{
	case 0:
		return TUid::Uid(ETstInlineEditingEnabledUid);
	case 1:
		return TUid::Uid(ETstInputMethodUid);
#if defined(_DEBUG)
	default:
		Panic(EPanicBadIndex2);
		break;
#endif
		}
	return KNullUid;
	}

void CTstControl::WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const
	{
	switch (aAttributeUid.iUid)
		{
	case ETstInlineEditingEnabledUid:
		aStream.WriteUint8L((iFlags&EFlagInlineEditingEnabled)!=0);
		break;
	case ETstInputMethodUid:
		aStream.WriteUint8L(iInputMethod);
		break;
#if defined(_DEBUG)
	default:
		Panic(EPanicBadAttributeUid1);
		break;
#endif
		}
	}

void CTstControl::ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream)
	{
	switch (aAttributeUid.iUid)
		{
	case ETstInlineEditingEnabledUid:
		ChangeSetupAndResetBufferAndDrawNow(SetFlagInlineEditingEnabled, aStream.ReadUint8L());
		break;
	case ETstInputMethodUid:
		{
		TInt inputMethod=aStream.ReadUint8L();
		if ((inputMethod!=EInputMethodPlain) && (inputMethod!=EInputMethodHexadecimalCharacterCode) && (inputMethod != EInputMethodPinYin))
			{
			User::Leave(KErrCorrupt);
			}
		ChangeSetupAndResetBufferAndDrawNow(SetInputMethod, inputMethod);
		}
		break;
#if defined(_DEBUG)
	default:
		Panic(EPanicBadAttributeUid2);
		break;
#endif
		}
	}

void CTstControl::WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream, TBool aInlineEditingEnabled, TInt aInputMethod)
	{
	switch (aAttributeUid.iUid)
		{
	case ETstInlineEditingEnabledUid:
		aStream.WriteUint8L(aInlineEditingEnabled!=EFalse);
		break;
	case ETstInputMethodUid:
		aStream.WriteUint8L(aInputMethod);
		break;
#if defined(_DEBUG)
	default:
		Panic(EPanicBadAttributeUid3);
		break;
#endif
		}
	}

void CTstControl::ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream, TBool& aInlineEditingEnabled, TInt& aInputMethod)
	{
	switch (aAttributeUid.iUid)
		{
	case ETstInlineEditingEnabledUid:
		aInlineEditingEnabled=aStream.ReadUint8L();
		break;
	case ETstInputMethodUid:
		aInputMethod=aStream.ReadUint8L();
		break;
#if defined(_DEBUG)
	default:
		Panic(EPanicBadAttributeUid4);
		break;
#endif
		}
	}

void CTstControl::HandleGainingForeground()
	{
	DrawableWindow()->MoveToGroup(iCoeEnv->WsSession().GetFocusWindowGroup()); // ignore the error returned
	ChangeSetupAndResetBufferAndDrawNow(SetForeground, ETrue);
	}

void CTstControl::HandleLosingForeground()
	{
	ChangeSetupAndResetBufferAndDrawNow(SetForeground, EFalse);
	}

void CTstControl::HandleChangeInFocus()
	{
	ChangeSetupAndResetBufferAndDrawNow(SetInputCapabilities, NULL);
	}

void CTstControl::HandleDestructionOfFocusedItem()
	{
	if (!IsBeingDestroyed())
		{
		const TCoeInputCapabilities inputCapabilities(STATIC_CAST(const CCoeAppUi*, iCoeEnv->AppUi())->InputCapabilities());
		if (inputCapabilities.FepAwareTextEditor()==NULL)
			{
			iFlags&=~EFlagInsideInlineEditingTransaction; // turn this off now so that ChangeSetupAndResetBufferAndDrawNow does not try to call CancelInlineEdit on an object that has probably been destroyed by now
			}
		ChangeSetupAndResetBufferAndDrawNow(SetInputCapabilities, REINTERPRET_CAST(TInt, &inputCapabilities));
		}
	}

CTstControl::CTstControl(CTstFep& aFep)
	:iFep(aFep),
	 iInputMethod(EInputMethodHexadecimalCharacterCode),
	 iFlags(0),
	 iBuffer(KNullDesC),
	 iSelectedCompositionText(0, 0),
	 iInputCapabilities(TCoeInputCapabilities::ENone),
	 iCompositionFont(NULL),
	 iStatusFont(NULL),
	 iInsertionPoint(NULL),
	 iPositionOnWindowBeingDragged(0, 0)
	{
	}

void CTstControl::ConstructL()
	{
	CreateWindowL();
	EnableDragEvents();
	ClaimPointerGrab();
	SetNonFocusing();
	RDrawableWindow& window=*DrawableWindow();
	window.SetOrdinalPosition(0, ECoeWinPriorityFep);
	window.SetShadowHeight(3);
	TFontSpec compositionFontSpec(KLitCompositionFontTypefaceName, 200);
	compositionFontSpec.iTypeface.SetIsProportional(EFalse);
	//compositionFontSpec.SetScriptTypeForMetrics(ELangPrcChinese);
	iCompositionFont=iCoeEnv->CreateScreenFontL(compositionFontSpec);
	//iStatusFont=iCoeEnv->CreateScreenFontL(TFontSpec(KLitStatusFontTypefaceName, 120));
	iStatusFont=iCoeEnv->CreateScreenFontL(TFontSpec(KLitStatusFontTypefaceName, FontUtils::TwipsFromPoints(8)));
	iInsertionPoint=CTstInsertionPoint::NewL(window, *iCoeEnv);
	SetPositionOfInsertionPointInBuffer(0);
	const TInt statusFontHeightInPixels=iStatusFont->HeightInPixels();
	const TSize size(320, 1 + EGapAboveCompositionLine + iCompositionFont->HeightInPixels() + EGapBelowCompositionLine
						+ EGapAboveCompositionLine + iCompositionFont->HeightInPixels() + EGapBelowCompositionLine		// character selector
						+ EGapAboveTopStatusLine
						+ (4 * (statusFontHeightInPixels + EGapBetweenEachStatusLine))
						+ statusFontHeightInPixels + EGapBelowBottomStatusLine + 1);
	const TSize screenSize(iCoeEnv->ScreenDevice()->SizeInPixels());
	SetExtent(TPoint(screenSize.iWidth-(size.iWidth+10), screenSize.iHeight-(size.iHeight+10)), size);
	STATIC_CAST(CCoeAppUi*, iCoeEnv->AppUi())->AddToStackL(this, ECoeStackPriorityFep, ECoeStackFlagRefusesFocus|ECoeStackFlagSharable);
	ChangeSetupAndResetBufferAndDrawNow(SetForeground, iCoeEnv->RootWin().Identifier()==iCoeEnv->WsSession().GetFocusWindowGroup());
	ChangeSetupAndResetBufferAndDrawNow(SetFocus, EFalse);
	ChangeSetupAndResetBufferAndDrawNow(SetFlagInlineEditingEnabled, ETrue);
	ChangeSetupAndResetBufferAndDrawNow(SetInputMethod, EInputMethodHexadecimalCharacterCode);
	ChangeSetupAndResetBufferAndDrawNow(SetInputCapabilities, NULL);
	}

void CTstControl::SetForeground(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	SetFlag(aControl, aChangeWasMade, aParameter, EFlagForeground);
	}

void CTstControl::SetFocus(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	aChangeWasMade=EFalse;
	if (!aParameter!=!aControl.IsFocused()) // fold non-zero values on both sides before comparing for inequality
		{
		CCoeAppUi& appUi=*STATIC_CAST(CCoeAppUi*, aControl.iCoeEnv->AppUi());
		appUi.UpdateStackedControlFlags(&aControl, (aParameter? 0: ECoeStackFlagRefusesFocus), ECoeStackFlagRefusesFocus);
		appUi.HandleStackChanged();
		aChangeWasMade=ETrue;
		TBool notUsed;
		SetInputCapabilities(aControl, notUsed, NULL);
		}
	}

void CTstControl::SetFlagInlineEditingEnabled(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	SetFlag(aControl, aChangeWasMade, aParameter, EFlagInlineEditingEnabled);
	}

void CTstControl::SetInputMethod(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	__ASSERT_DEBUG((aParameter==EInputMethodPlain) || (aParameter==EInputMethodHexadecimalCharacterCode) || (aParameter==EInputMethodPinYin), Panic(EPanicBadInputMethod1));
	aChangeWasMade=EFalse;
	if (aControl.iInputMethod!=aParameter)
		{
		aControl.iInputMethod=aParameter;
		aChangeWasMade=ETrue;
		}
	}

void CTstControl::SetInputCapabilities(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	aChangeWasMade=EFalse;
	TCoeInputCapabilities inputCapabilities(TCoeInputCapabilities::ENone);
	if (!aControl.IsFocused())
		{
		inputCapabilities=(aParameter!=NULL)? *REINTERPRET_CAST(const TCoeInputCapabilities*, aParameter): STATIC_CAST(const CCoeAppUi*, aControl.iCoeEnv->AppUi())->InputCapabilities();
		}
	if (aControl.iInputCapabilities!=inputCapabilities)
		{
		aControl.iInputCapabilities=inputCapabilities;
		aChangeWasMade=ETrue;
		}
	}

void CTstControl::SetFlag(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter, TUint aFlag)
	{
	aChangeWasMade=EFalse;
	if (!aParameter!=!(aControl.iFlags&aFlag)) // fold non-zero values on both sides before comparing for inequality
		{
		aControl.iFlags^=aFlag;
		aChangeWasMade=ETrue;
		}
	}

void CTstControl::ChangeSetupAndResetBufferAndDrawNow(FChangeFunction aChangeFunction, TInt aParameter)
	{
	__ASSERT_DEBUG(!iFep.IsSimulatingKeyEvent() || ((~iFlags&EFlagInsideInlineEditingTransaction) && (iBuffer.Length()==0)), Panic(EPanicNotExpectedToBeSimulatingKeyEvent2));
	TBool needToDraw=EFalse;
	if (iFlags&EFlagInsideInlineEditingTransaction)
		{
		CancelInlineEdit(*iInputCapabilities.FepAwareTextEditor());
		needToDraw=ETrue;
		}
	if (aChangeFunction!=NULL)
		{
		TBool changeWasMade;
		(*aChangeFunction)(*this, changeWasMade, aParameter);
		if (changeWasMade)
			{
			needToDraw=ETrue;
			}
		}
	if (iBuffer.Length()>0)
		{
		ResetBuffer();
		needToDraw=ETrue;
		}
	const TBool potentiallyInlineEditing=((iInputCapabilities.FepAwareTextEditor()!=NULL) && (iFlags&EFlagInlineEditingEnabled)); // iInputCapabilities.FepAwareTextEditor may return a different value here to the call at the start of the function, hence the need to call it again
	iInsertionPoint->SetOn(!IsFocused() && !potentiallyInlineEditing);
	if (!potentiallyInlineEditing!=!(iFlags&EFlagHasNoCompositionWindow)) // fold non-zero values on both sides before comparing for inequality
		{
		const TInt heightOfCompositionWindow=EGapAboveCompositionLine+iCompositionFont->HeightInPixels()+EGapBelowCompositionLine;
		TInt windowAdjustmentY;
		if (potentiallyInlineEditing)
			{
			windowAdjustmentY=heightOfCompositionWindow;
			iFlags|=EFlagHasNoCompositionWindow;
			}
		else
			{
			windowAdjustmentY=-heightOfCompositionWindow;
			iFlags&=~EFlagHasNoCompositionWindow;
			}
		TRect rectangle(Position(), Size()); // creating rectangle from Rect() will give the wrong result as this control owns a window (its top left position would come out as TPoint(0, 0) rather than what Position() returns)
		rectangle.iTl.iY+=windowAdjustmentY;
		SetRect(rectangle);
		needToDraw=ETrue;
		}
	const TBool shouldBeVisible=(iFep.IsOn() && (iFlags&EFlagForeground));
	if (!IsVisible()!=!shouldBeVisible) // fold non-zero values on both sides before comparing for inequality
		{
		MakeVisible(shouldBeVisible);
		needToDraw=EFalse;
		}
	if (needToDraw)
		{
		DrawNow();
		}
	}

void CTstControl::InsertCompositionCharacterAndDrawNowL(TUint aCharacterCode)
	{
	const TInt positionOfNewText=iSelectedCompositionText.LowerPos();
	if (positionOfNewText<EMaximumLengthOfBuffer)
		{
		TTstTextBackup textBackup=iBuffer;
		textBackup.PushOntoCleanupStackL();
		const TCoeInputCapabilities::MCoeFepSpecificExtensions* fepSpecificInputCapabilityExtensions=iInputCapabilities.FepSpecificExtensions(TUid::Uid(KTestFep4Uid));
		const TUint upperCaseCharacterCode=User::UpperCase(aCharacterCode);
		TPtrC textToInsert;
		TBuf<1> buffer;
		TInt newPositionOfInsertionPointInBuffer=positionOfNewText;
		if ((iInputMethod==EInputMethodPlain) && (fepSpecificInputCapabilityExtensions!=NULL) && fepSpecificInputCapabilityExtensions->SupportsPhoneticAlphabet() && (upperCaseCharacterCode>='A') && (upperCaseCharacterCode<='Z'))
			{
			textToInsert.Set(TPtrC(KPhoneticAlphabet[upperCaseCharacterCode-'A']));
			const TInt lengthOfRoomForTextToInsert=EMaximumLengthOfBuffer-positionOfNewText;
			if (textToInsert.Length()>lengthOfRoomForTextToInsert)
				{
				textToInsert.Set(textToInsert.Left(lengthOfRoomForTextToInsert));
				}
			newPositionOfInsertionPointInBuffer+=textToInsert.Length();
			}
		else
			{
			buffer.Append(aCharacterCode);
			textToInsert.Set(buffer);
			++newPositionOfInsertionPointInBuffer;
			}
		iBuffer.Delete(positionOfNewText, iSelectedCompositionText.Length());
		const TInt lengthToDeleteToMakeRoom=textToInsert.Length()-(EMaximumLengthOfBuffer-iBuffer.Length());
		if (lengthToDeleteToMakeRoom>0)
			{
			iBuffer.Delete(iBuffer.Length()-lengthToDeleteToMakeRoom, lengthToDeleteToMakeRoom);
			}
		iBuffer.Insert(positionOfNewText, textToInsert);
		__ASSERT_DEBUG((iInputMethod==EInputMethodPlain) || (iInputMethod==EInputMethodHexadecimalCharacterCode) || (iInputMethod==EInputMethodPinYin), Panic(EPanicBadInputMethod4));
		if (iInputMethod==EInputMethodHexadecimalCharacterCode)
			{
			if (newPositionOfInsertionPointInBuffer>=ENumberOfHexadecimalDigitsPerCharacterCode)
				{
				TUint characterCode=0;
				for (TInt i=ENumberOfHexadecimalDigitsPerCharacterCode; ; --i)
					{
					if (i<=0)
						{
						// to support surrogate
						TInt byteCount = characterCode <= 0xFFFF ? 1 : 2;
						const TInt positionToInsertCharacterCode=newPositionOfInsertionPointInBuffer-ENumberOfHexadecimalDigitsPerCharacterCode;
						iBuffer.Delete(positionToInsertCharacterCode, ENumberOfHexadecimalDigitsPerCharacterCode-byteCount);
						if (byteCount == 1)
							{
							iBuffer[positionToInsertCharacterCode]=(TText)characterCode;
							}
						else
							{
							iBuffer[positionToInsertCharacterCode] = (TText)(0xD7C0 + (characterCode >> 10));
							iBuffer[positionToInsertCharacterCode+1] = (TText)(0xDC00 | (characterCode & 0x03FF));
							}
						newPositionOfInsertionPointInBuffer-=ENumberOfHexadecimalDigitsPerCharacterCode-byteCount;
						break;
						}
					const TCharUC hexadecimalDigit=iBuffer[newPositionOfInsertionPointInBuffer-i];
					characterCode*=16;
					if ((hexadecimalDigit>='0') && (hexadecimalDigit<='9'))
						{
						characterCode+=hexadecimalDigit-'0';
						}
					else if ((hexadecimalDigit>='A') && (hexadecimalDigit<='F'))
						{
						characterCode+=(hexadecimalDigit-'A')+10;
						}
					else
						{
						break;
						}
					}
				}
			}
		else if (iInputMethod == EInputMethodPinYin)
			{
			// to support pinyin
			if (newPositionOfInsertionPointInBuffer > 0)
				{
				if (aCharacterCode == 0x20)		// last inputted characters is space
					{
					TBool found = EFalse;
					TPtrC pinyin;		// founded pinyin
					// skip over "illegal" Pinyin character, including Chinese characters
					TInt posOfFirstPinyinChar = 0;
					while (posOfFirstPinyinChar < iBuffer.Length())	//newPositionOfInsertionPointInBuffer)
						{
						if (CPinyinSelector::IsValidPinyinChar(iBuffer[posOfFirstPinyinChar]))
							break;
						posOfFirstPinyinChar++;
						}
					if (posOfFirstPinyinChar < iBuffer.Length())	//newPositionOfInsertionPointInBuffer)
						{
						TInt posOfLastPinyinChar = iBuffer.Length() - 1;	//newPositionOfInsertionPointInBuffer - 1;
						while (posOfLastPinyinChar > posOfFirstPinyinChar)
							{
							// try to find long-est, legal pinyin
							TInt length = posOfLastPinyinChar - posOfFirstPinyinChar + 1;	//+1?
							pinyin.Set(iBuffer.Ptr()+posOfFirstPinyinChar, length);
							TBool legal = iPinYinSelector.SetPinyin(&pinyin);
							if (legal)
								{
								found = ETrue;
								break;
								}
							posOfLastPinyinChar--;
							}
						if (found && pinyin.Length()>0)
							{
							TInt characterCode = iPinYinSelector.NextCharacter();
							if (characterCode != 0xFFFFFFFF)
								{
								TInt byteCount = (characterCode <= 0xFFFF) ? 1 : 2;		// actually it is 16-bit count
								TInt positionToInsertCharacterCode = posOfFirstPinyinChar;
								// assert: positionOfNewText > positionToInsertCharacterCode
								if (positionOfNewText > positionToInsertCharacterCode)
									{
									iBuffer.Delete(positionOfNewText, 1);	// space
									// if (pinyin.Length() <= byteCount)...
									iBuffer.Delete(positionToInsertCharacterCode, pinyin.Length()-byteCount);	// pinyin
									}
								else
									{
									// if (pinyin.Length() <= byteCount)...
									iBuffer.Delete(positionToInsertCharacterCode, pinyin.Length()-byteCount);	// pinyin
									iBuffer.Delete(positionOfNewText, 1);	// space
									positionToInsertCharacterCode--;
									}
								if (byteCount == 1)
									{
									iBuffer[positionToInsertCharacterCode]=(TText)characterCode;
									}
								else
									{
									iBuffer[positionToInsertCharacterCode] = (TText)(0xD7C0 + (characterCode >> 10));
									iBuffer[positionToInsertCharacterCode+1] = (TText)(0xDC00 | (characterCode & 0x03FF));
									}
								newPositionOfInsertionPointInBuffer = positionToInsertCharacterCode + byteCount;
								}
							}
						}
					}
				}
			}
		SetPositionOfInsertionPointInBuffer(newPositionOfInsertionPointInBuffer);
		MCoeFepAwareTextEditor* const fepAwareTextEditor=iInputCapabilities.FepAwareTextEditor();
		if ((fepAwareTextEditor==NULL) || !(iFlags&EFlagInlineEditingEnabled))
			{
			DrawNow();
			}
		else
			{
			if (iInputMethod == EInputMethodPinYin)
				DrawNow();	// draw candidate characters for current pinyin
			SetInlineTextL(*fepAwareTextEditor);
			}
		textBackup.PopOffCleanupStack();
		}
	} // positionOfNewText newPositionOfInsertionPointInBuffer iCursorPos

void CTstControl::SetInlineTextL(MCoeFepAwareTextEditor& aFepAwareTextEditor)
	{
	if (iFlags&EFlagInsideInlineEditingTransaction)
		{
		aFepAwareTextEditor.UpdateFepInlineTextL(iBuffer, iSelectedCompositionText.iCursorPos);
		}
	else
		{
		aFepAwareTextEditor.StartFepInlineEditL(iBuffer, iSelectedCompositionText.iCursorPos, ETrue, this, *this, *this);
		iFlags|=EFlagInsideInlineEditingTransaction;
		}
	}

void CTstControl::CancelInlineEdit(MCoeFepAwareTextEditor& aFepAwareTextEditor)
	{
	aFepAwareTextEditor.CancelFepInlineEdit();
	iFlags&=~EFlagInsideInlineEditingTransaction;
	}

void CTstControl::CommitInlineEditL(MCoeFepAwareTextEditor& aFepAwareTextEditor)
	{
	aFepAwareTextEditor.CommitFepInlineEditL(*iCoeEnv);
	iFlags&=~EFlagInsideInlineEditingTransaction;
	}

void CTstControl::ResetBuffer()
	{
	SetPositionOfInsertionPointInBuffer(0);
	iBuffer.SetLength(0);
	}

void CTstControl::SetPositionOfInsertionPointInBuffer(TInt aPositionOfInsertionPointInBuffer)
	{
	SetPositionOfInsertionPointInBuffer(aPositionOfInsertionPointInBuffer, aPositionOfInsertionPointInBuffer);
	}

void CTstControl::SetPositionOfInsertionPointInBuffer(TInt aPositionOfInsertionPointInBuffer, TInt aPositionOfAnchorInBuffer)
	{
	iSelectedCompositionText.iCursorPos=aPositionOfInsertionPointInBuffer;
	iSelectedCompositionText.iAnchorPos=aPositionOfAnchorInBuffer;
	iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
	}

TPoint CTstControl::PositionOfInsertionPointOnWindow() const
	{
	return TPoint(1+EGapLeftOfEachLine+iCompositionFont->TextWidthInPixels(iBuffer.Left(iSelectedCompositionText.iCursorPos)), 1+EGapAboveCompositionLine+iCompositionFont->HeightInPixels());
	}

TInt CTstControl::PositionInBuffer(TInt aX) const
	{
	const TInt lengthOfBuffer=iBuffer.Length();
	TInt previousX=0; // dummy initialization to prevent compiler warning "local variable 'previousX' may be used without having been initialized"
	for (TInt i=0; ; ++i)
		{
		if (i>lengthOfBuffer)
			{
			return lengthOfBuffer;
			}
		const TInt x=1+EGapLeftOfEachLine+iCompositionFont->TextWidthInPixels(iBuffer.Left(i));
		if (x>aX)
			{
			__ASSERT_DEBUG(i>=0, Panic(EPanicNegativeIndex));
			if (i<=0)
				{
				return 0;
				}
			if (aX-previousX<x-aX)
				{
				return i-1;
				}
			return i;
			}
		previousX=x;
		}
	}

TBool CTstControl::IsLegalNonDigitForRealNumber(const TCharUC& aKeyCodeInUpperCase) const
	{
	const TInt startOfSelectedText=iSelectedCompositionText.LowerPos();
	const TInt endOfSelectedText=iSelectedCompositionText.HigherPos();
	TInt positionOfExistingDecimalPoint=iBuffer.Locate('.');
	if ((positionOfExistingDecimalPoint>=startOfSelectedText) && (positionOfExistingDecimalPoint<endOfSelectedText))
		{
		positionOfExistingDecimalPoint=KErrNotFound;
		}
	TInt positionOfExistingE=iBuffer.Locate('E');
	if ((positionOfExistingE>=startOfSelectedText) && (positionOfExistingE<endOfSelectedText))
		{
		positionOfExistingE=KErrNotFound;
		}
	if (aKeyCodeInUpperCase=='.')
		{
		return (positionOfExistingDecimalPoint==KErrNotFound) && ((positionOfExistingE==KErrNotFound) || (positionOfExistingE>=endOfSelectedText));
		}
	if (aKeyCodeInUpperCase=='E')
		{
		return (positionOfExistingE==KErrNotFound) && ((positionOfExistingDecimalPoint==KErrNotFound) || (positionOfExistingDecimalPoint<startOfSelectedText));
		}
	return EFalse;
	}

void CTstControl::GetFormatAtDocumentPosition(TCharFormat& aFormat, TInt aDocumentPosition, const MCoeFepAwareTextEditor& aFepAwareTextEditor)
	{
	if (aDocumentPosition<0)
		{
		aDocumentPosition=0;
		}
	const TInt documentLength=aFepAwareTextEditor.DocumentLengthForFep();
	if (aDocumentPosition>documentLength)
		{
		aDocumentPosition=documentLength;
		}
	aFepAwareTextEditor.GetFormatForFep(aFormat, aDocumentPosition);
	}

TInt CTstControl::NumberOfCharactersInBuffer(const CBase* aControl)
	{
#if 0
	return STATIC_CAST(const CTstControl*, aControl)->iBuffer.Length();
#else
	// should not return unpaired surrogate
	CTstControl* control = STATIC_CAST(CTstControl*, CONST_CAST(CBase*, aControl));
	return control->iBuffer.Length();
#endif
	}

const TAny* CTstControl::CharacterInBuffer(const CBase* aControl, TInt aIndex)
	{
#if 0
	CTstControl* control=STATIC_CAST(CTstControl*, CONST_CAST(CBase*, aControl));
	control->iCharacterCode=control->iBuffer[aIndex];
	return &control->iCharacterCode;
#else
	// should not return unpaired surrogate
	CTstControl* control=STATIC_CAST(CTstControl*, CONST_CAST(CBase*, aControl));
	control->iCharacterCode=control->iBuffer[aIndex];
	return &control->iCharacterCode;
#endif
	}

TKeyResponse CTstControl::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode)
	{
	// CTstFep::OfferKeyEventL assumes that this will not leave if it returns EKeyWasNotConsumed
	FEP_START_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, aEventCode);
	const TBool isFocused=IsFocused();
	const TCharUC keyCodeInUpperCase=aKeyEvent.iCode;
	TInt bufferLength=iBuffer.Length();
	
	// should we reset pinyin selector?
	TBool needResetPinyinSelector = ETrue;
	if (keyCodeInUpperCase == EKeyPageUp || keyCodeInUpperCase == EKeyPageDown)
		needResetPinyinSelector = EFalse;
	if (keyCodeInUpperCase >= '0' && keyCodeInUpperCase <= '9')
		{
		if (!iPinYinSelector.IsReseted())
			needResetPinyinSelector = EFalse;
		}
	if (needResetPinyinSelector)
		iPinYinSelector.Reset();
		
	if ((aKeyEvent.iModifiers&EModifierRightShift) && !isFocused)
		{
		if (keyCodeInUpperCase=='W')
			{
 			if (iFlags&EFlagInsideInlineEditingTransaction)
				{
				TBool didIt=EFalse;
				MCoeFepAwareTextEditor* const fepAwareTextEditor=iInputCapabilities.FepAwareTextEditor();
				if (fepAwareTextEditor!=NULL)
					{
					MCoeFepAwareTextEditor_Extension1* const extension=fepAwareTextEditor->Extension1();
					if (extension!=NULL)
						{
						TTextCursor textCursor;
						textCursor.iType=TTextCursor::ETypeHollowRectangle;
						textCursor.iHeight=0; // ignored by CEikEdwin as it checks for iWidth.
						textCursor.iAscent=0; // ignored by CEikEdwin
						textCursor.iWidth=5;
						textCursor.iFlags=TTextCursor::EFlagNoFlash;
						textCursor.iColor=TRgb(255,0,0);
						extension->SetCursorType(didIt,textCursor);
						}
					}
				if (!didIt)
					{
					User::InfoPrint(_L("Couldn't set the cursor-type"));
					}
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		else if (bufferLength==0)
			{
			switch (keyCodeInUpperCase)
				{
			case 'M':
  				ChangeSetupAndResetBufferAndDrawNow(SetInputMethod, (iInputMethod+1<EInputMethodOnePastTheLast)? iInputMethod+1: 0);
  				// client app may not have the capability "WriteDeviceData"
  				//iFep.WriteAttributeDataAndBroadcastL(TUid::Uid(ETstInputMethodUid));
  				FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function

			case 'F':
				ChangeSetupAndResetBufferAndDrawNow(SetFocus, ETrue);
				FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			case 'I':
				ChangeSetupAndResetBufferAndDrawNow(SetFlagInlineEditingEnabled, !(iFlags&EFlagInlineEditingEnabled));
				// client app may not have the capability "WriteDeviceData"
				//iFep.WriteAttributeDataAndBroadcastL(TUid::Uid(ETstInlineEditingEnabledUid));
				FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			case 'X':
			case 'C':
			case 'V':
				{
				//TTstArrayOfOneCtrlCharacter arrayOfOneCtrlCharacter(keyCodeInUpperCase-('A'-1)); // the TTstArrayOfOneCtrlCharacter object cannot be an anonymous (temporary) object as its lifetime must be guaranteed to last until iFep.SimulateKeyEventsL returns
                //iFep.SimulateKeyEventsL(arrayOfOneCtrlCharacter.ArrayOfModifiedCharacters());
				const TInt numberOfCharacters = 7;
			    CArrayFix<TUint>* arrayOfCharacters=new(ELeave) CArrayFixFlat<TUint>(numberOfCharacters); // a RArray would be better than a CArrayFix, but unfortunately RArray doesn't (yet) have a TArray interface
			    CleanupStack::PushL(arrayOfCharacters);
			    arrayOfCharacters->AppendL(' ');
			    arrayOfCharacters->AppendL('!');
			    arrayOfCharacters->AppendL('\"');
			    arrayOfCharacters->AppendL('#');
			    TUint32 ch;
                ch = 0x20024;
                TUint32 hi = ((ch - 0x10000) / 0x400) + 0xD800;
                TUint32 lo = ((ch - 0x10000) % 0x400) + 0xDC00;
                arrayOfCharacters->AppendL(lo);
                arrayOfCharacters->AppendL(hi);
                arrayOfCharacters->AppendL(hi);
			    iFep.SimulateKeyEventsL(arrayOfCharacters->Array());
			    CleanupStack::PopAndDestroy();
				}
				FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			case 'T':
				{
				MCoeFepAwareTextEditor* const fepAwareTextEditor=iInputCapabilities.FepAwareTextEditor();
				if (fepAwareTextEditor==NULL)
					{
					User::InfoPrint(KLitNoFocusedFepAwareTextEditor);
					}
				else
					{
					MCoeFepAwareTextEditor_Extension1* const extension1=fepAwareTextEditor->Extension1();
					if (extension1==NULL)
						{
						User::InfoPrint(KLitNoFocusedFepAwareTextEditorSupportingState);
						}
					else
						{
						MCoeFepAwareTextEditor_Extension1::CState* const state=extension1->State(TUid::Uid(KTestFep4Uid));
						if (state!=NULL)
							{
							STATIC_CAST(CStateInformation*, state)->IncrementMeaninglessNumber(); // this cast is safe as KTestFep4Uid is used to retrieve state
							}
						else
							{
							CStateInformation* stateInformation=CStateInformation::NewLC(1);
							extension1->SetStateTransferingOwnershipL(stateInformation, TUid::Uid(KTestFep4Uid));
							CleanupStack::Pop(stateInformation);
							}
						DrawNow();
						}
					}
				}
				FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			case 'N':
				{
				const MCoeFepAwareTextEditor* const fepAwareTextEditor=iInputCapabilities.FepAwareTextEditor();
				if (fepAwareTextEditor==NULL)
					{
					User::InfoPrint(KLitNotAvailable);
					}
				else
					{
					TCursorSelection cursorSelection;
					fepAwareTextEditor->GetCursorSelectionForFep(cursorSelection);
					TPoint position;
					TInt height;
					TInt ascent;
					fepAwareTextEditor->GetScreenCoordinatesForFepL(position,height,ascent,cursorSelection.LowerPos());
					position.iY+=height-ascent;
					const TSize screenSize(iCoeEnv->ScreenDevice()->SizeInPixels());
					const TInt xMaximum=screenSize.iWidth-iSize.iWidth;
					if ((position.iX<0) || (xMaximum<0))
						{
						position.iX=0;
						}
					else if (position.iX>xMaximum)
						{
						position.iX=xMaximum;
						}
					const TInt yMaximum=screenSize.iHeight-iSize.iHeight;
					if ((position.iY<0) || (yMaximum<0))
						{
						position.iY=0;
						}
					else
						{
						const TInt yOverlapIfFepIsBelow=position.iY-yMaximum;
						if (yOverlapIfFepIsBelow>0)
							{
							const TInt yPositionIfFepIsAbove=Max(0, position.iY-(height+iSize.iHeight));
							const TInt yOverlapIfFepIsAbove=(yPositionIfFepIsAbove+iSize.iHeight)-(position.iY-height);
							if (yOverlapIfFepIsAbove<yOverlapIfFepIsBelow)
								{
								position.iY=yPositionIfFepIsAbove;
								}
							}
						}
					SetPosition(position);
					}
				}
				FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			case 'B':
			case 'A':
			case 'S':
				{
				const MCoeFepAwareTextEditor* const fepAwareTextEditor=iInputCapabilities.FepAwareTextEditor();
				TBuf<40+EMaximumLengthOfDisplayOfContextInformation> textToDisplay;
				if (fepAwareTextEditor==NULL)
					{
					textToDisplay=KLitNotAvailable;
					}
				else
					{
					TCursorSelection cursorSelection;
					fepAwareTextEditor->GetCursorSelectionForFep(cursorSelection);
					switch (keyCodeInUpperCase)
						{
					case 'B':
						{
						TInt lengthToRetrieve=EMaximumLengthOfDisplayOfContextInformation+1;
						TInt documentPositionBeforeSelection=cursorSelection.LowerPos()-lengthToRetrieve;
						if (documentPositionBeforeSelection<0)
							{
							lengthToRetrieve+=documentPositionBeforeSelection; // same as doing "lengthToRetrieve=cursorSelection.LowerPos()", hence the assert below
							__ASSERT_DEBUG(lengthToRetrieve==cursorSelection.LowerPos(), Panic(EPanicArithmeticConfusion));
							documentPositionBeforeSelection=0;
							}
						fepAwareTextEditor->GetEditorContentForFep(textToDisplay, documentPositionBeforeSelection, lengthToRetrieve);
						const TInt lengthOfTextBeforeSelection=textToDisplay.Length();
						if (lengthOfTextBeforeSelection>EMaximumLengthOfDisplayOfContextInformation)
							{
							textToDisplay.Delete(0, (lengthOfTextBeforeSelection-EMaximumLengthOfDisplayOfContextInformation)-1);
							__ASSERT_DEBUG(textToDisplay.Length()==EMaximumLengthOfDisplayOfContextInformation+1, Panic(EPanicBadLengthOfTextBeforeSelection));
							textToDisplay[0]=KEllipsisCharacter;
							}
						}
						textToDisplay.Insert(0, KLitQuotationMark);
						textToDisplay.Append(KLitQuotationMark);
						textToDisplay.Insert(0, KLitTextBeforeSelectionColonSpace);
						break;
					case 'A':
						{
						const TInt documentLength=fepAwareTextEditor->DocumentLengthForFep();
						const TInt documentPositionAfterSelection=cursorSelection.HigherPos();
						if (documentPositionAfterSelection>documentLength)
							{
							__ASSERT_DEBUG(documentPositionAfterSelection==documentLength+1, Panic(EPanicSelectionExtendsPastEndOfDocument));
							}
						else
							{
							fepAwareTextEditor->GetEditorContentForFep(textToDisplay, documentPositionAfterSelection, EMaximumLengthOfDisplayOfContextInformation+1);
							const TInt lengthOfTextAfterSelection=textToDisplay.Length();
							if (lengthOfTextAfterSelection>EMaximumLengthOfDisplayOfContextInformation)
								{
								textToDisplay.Delete(EMaximumLengthOfDisplayOfContextInformation, (lengthOfTextAfterSelection-EMaximumLengthOfDisplayOfContextInformation)-1);
								__ASSERT_DEBUG(textToDisplay.Length()==EMaximumLengthOfDisplayOfContextInformation+1, Panic(EPanicBadLengthOfTextAfterSelection));
								textToDisplay[EMaximumLengthOfDisplayOfContextInformation]=KEllipsisCharacter;
								}
							}
						}
						textToDisplay.Insert(0, KLitQuotationMark);
						textToDisplay.Append(KLitQuotationMark);
						textToDisplay.Insert(0, KLitTextAfterSelectionColonSpace);
						break;
					case 'S':
						fepAwareTextEditor->GetEditorContentForFep(textToDisplay, cursorSelection.LowerPos(), cursorSelection.Length());
						{
						const TInt lengthOfSelection=textToDisplay.Length();
						if (lengthOfSelection>EMaximumLengthOfDisplayOfContextInformation)
							{
							textToDisplay.Delete(EMaximumLengthOfDisplayOfContextInformation, (lengthOfSelection-EMaximumLengthOfDisplayOfContextInformation)-1);
							__ASSERT_DEBUG(textToDisplay.Length()==EMaximumLengthOfDisplayOfContextInformation+1, Panic(EPanicBadLengthOfSelection));
							textToDisplay[EMaximumLengthOfDisplayOfContextInformation]=KEllipsisCharacter;
							}
						}
						textToDisplay.Insert(0, KLitQuotationMark);
						textToDisplay.Append(KLitQuotationMark);
						textToDisplay.Insert(0, KLitSelectionColonSpace);
						break;
#if defined(_DEBUG)
					default:
						Panic(EPanicBadKeyCode1);
						break;
#endif
						}
					}
				User::InfoPrint(textToDisplay);
				}
				FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
				}
			}
		}
	if (iInputCapabilities.IsNone() && !isFocused)
		{
		FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasNotConsumed); // returns from this function
		}
	switch (keyCodeInUpperCase)
		{
	case EKeyEnter:
	case EKeyEscape:
		if (isFocused)
			{
			ChangeSetupAndResetBufferAndDrawNow(SetFocus, EFalse);
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		if (bufferLength>0)
			{
			TBool needToDraw=ETrue;
			switch (keyCodeInUpperCase)
				{
			case EKeyEnter:
				if (~iFlags&EFlagInsideInlineEditingTransaction)
					{
					iFep.SimulateKeyEventsL(TArray<TUint>(NumberOfCharactersInBuffer, CharacterInBuffer, this));
					}
				else
					{
					CommitInlineEditL(*iInputCapabilities.FepAwareTextEditor());
					// for pinyin, need redraw candidate characters
					if (iInputMethod != EInputMethodPinYin)
						needToDraw=EFalse;
					}
				break;
			case EKeyEscape:
				if (iFlags&EFlagInsideInlineEditingTransaction)
					{
					CancelInlineEdit(*iInputCapabilities.FepAwareTextEditor());	
					needToDraw=EFalse;
					}
				break;
#if defined(_DEBUG)
			default:
				Panic(EPanicBadKeyCode2);
				break;
#endif
				}
			ResetBuffer();
			if (needToDraw)
				{
				DrawNow();
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		break;
	case EKeyPageUp:
		// For Unicode IME: decrease last Unicode value.
		// For Pinyin IME: find previous character has the same pinyin.
	case EKeyPageDown:
		// For Unicode IME: increase last Unicode value.
		// For Pinyin IME: find next character has the same pinyin.
		if (iInputMethod==EInputMethodHexadecimalCharacterCode)
			{
			// unicode
			if (bufferLength > 0)
				{
				if (bufferLength > 1)
					{
					TText high = iBuffer[bufferLength-2];
					TText low = iBuffer[bufferLength-1];
					TInt characterCode;
					if ((high & 0xF800) == 0xD800 && (low & 0xF800) == 0xD800)
						{
						characterCode = ((high - 0xd7f7) << 10) + low;
						if (characterCode > 0x10000 & characterCode < 0x2FFFE)
							{
							if (keyCodeInUpperCase == EKeyPageUp)
								characterCode--;
							else
								characterCode++;
							iBuffer[bufferLength-2] = (TText)(0xD7C0 + (characterCode >> 10));
							iBuffer[bufferLength-1] = (TText)(0xDC00 | (characterCode & 0x03FF));
							if (iFlags&EFlagInsideInlineEditingTransaction)
								{
								SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
								}
							else
								{
								DrawNow();
								}
							FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
							}
						}
					}
				else
					{
					if ((iBuffer[bufferLength-1] & 0xF800) != 0xD800)
						{
						TText characterCode = (keyCodeInUpperCase == EKeyPageUp) ? (iBuffer[bufferLength-1]-1) : (iBuffer[bufferLength-1]+1);
						if (characterCode <= 0xFFFF)
							{
							iBuffer[bufferLength-1] = characterCode;
							if (iFlags&EFlagInsideInlineEditingTransaction)
								{
								SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
								}
							else
								{
								DrawNow();
								}
							FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
							}
						}
					}
				}
			}
		else if (iInputMethod==EInputMethodPinYin)
			{
			// pinyin
			if (bufferLength > 0)
				{
				TInt characterCode = (keyCodeInUpperCase == EKeyPageUp)
									? iPinYinSelector.PreviousPage()
									: iPinYinSelector.NextPage();
				if (characterCode != CPinyinSelector::EInvalidChar)
					{
					TInt positionOfNewText = iSelectedCompositionText.LowerPos();
					
					TInt byteCount = 1;		// byte count of current char
					if (bufferLength >= 2 && (iBuffer[positionOfNewText-1] & 0xF800) == 0xD800)
						{
						byteCount = 2;
						}
					if (characterCode > 0x10000 & characterCode < 0x2FFFE)
						{
						if (byteCount == 1)
							{
							_LIT(KOneSpace, " ");
							iBuffer.Insert(positionOfNewText-1, KOneSpace);
							positionOfNewText++;
							SetPositionOfInsertionPointInBuffer(positionOfNewText+1);
							}
						iBuffer[positionOfNewText-2] = (TText)(0xD7C0 + (characterCode >> 10));
						iBuffer[positionOfNewText-1] = (TText)(0xDC00 | (characterCode & 0x03FF));
						}
					else if (characterCode >=0 && characterCode <= 0xFFFF)
						{
						if (byteCount == 2)
							{
							iBuffer.Delete(positionOfNewText-1, 1);
							positionOfNewText--;
							SetPositionOfInsertionPointInBuffer(positionOfNewText);//-1);
							}
						iBuffer[positionOfNewText-1] = characterCode;
						}
					
					// redraw
					if (iFlags&EFlagInsideInlineEditingTransaction)
						{
						DrawNow();		// for character selector
						SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
						}
					else
						{
						DrawNow();
						}
					FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
					}
				}
			}
		break;
	case EKeyDelete:
	case EKeyBackspace:
		if (!isFocused && (bufferLength>0))
			{
			TCursorSelection portionToDelete(iSelectedCompositionText.iCursorPos, iSelectedCompositionText.iAnchorPos);
			const TBool deleteLeft=((keyCodeInUpperCase==EKeyBackspace) && !(aKeyEvent.iModifiers&EModifierShift));
			if (portionToDelete.Length()==0)
				{
				if (deleteLeft)
					{
					if (portionToDelete.iCursorPos>0)
						{
						//    If surrogate low part is deleted, then delete surrogate high part too
						if ( IsLowSurrogate( iBuffer[ portionToDelete.iCursorPos - 1 ] ) )
						    --portionToDelete.iCursorPos;
						--portionToDelete.iCursorPos;
						}
					}
				else
					{
					if (portionToDelete.iCursorPos<bufferLength)
						{
						++portionToDelete.iCursorPos;
						}
					}
				}
			if (portionToDelete.Length()>0)
				{
				TTstTextBackup textBackup=iBuffer;
				textBackup.PushOntoCleanupStackL();
				SetPositionOfInsertionPointInBuffer(portionToDelete.LowerPos());
				iBuffer.Delete(portionToDelete.LowerPos(), portionToDelete.Length());
				//  Reset buffer length because we may delete more than one byte for surrogate parts
				bufferLength = iBuffer.Length();
				if (~iFlags&EFlagInsideInlineEditingTransaction)
					{
					DrawNow();
					}
				else
					{
					if (bufferLength-1>0)
						{
						SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
						}
					else
						{
						CancelInlineEdit(*iInputCapabilities.FepAwareTextEditor());
						}
					}
				textBackup.PopOffCleanupStack();
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		break;
	case EKeySpace:
		if (!isFocused && (bufferLength>0))
			{
			__ASSERT_DEBUG((iInputMethod==EInputMethodPlain) || (iInputMethod==EInputMethodHexadecimalCharacterCode) || (iInputMethod==EInputMethodPinYin), Panic(EPanicBadInputMethod2));
			if ((iInputMethod==EInputMethodPlain) && (iSelectedCompositionText.iCursorPos<bufferLength))
				{
				const TInt increment=(aKeyEvent.iModifiers&EModifierShift)? -1: 1;
				const TCharUC limit=(aKeyEvent.iModifiers&EModifierShift)? 'A': 'Z';
				const TCharUC characterToAdjust=iBuffer[iSelectedCompositionText.iCursorPos];
				__ASSERT_DEBUG((characterToAdjust>='A') && (characterToAdjust<='Z'), Panic(EPanicBadCharacterInBuffer));
				if (characterToAdjust!=limit)
					{
					TTstTextBackup textBackup=iBuffer;
					textBackup.PushOntoCleanupStackL();
					iBuffer[iSelectedCompositionText.iCursorPos]=(TText)(iBuffer[iSelectedCompositionText.iCursorPos]+increment);
					if (iFlags&EFlagInsideInlineEditingTransaction)
						{
						SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
						}
					else
						{
						DrawNow();
						}
					textBackup.PopOffCleanupStack();
					}
				}
			else if (iInputMethod == EInputMethodPinYin)
				{
				iCharacterCode=aKeyEvent.iCode; // set iCharacterCode to aKeyEvent.iCode rather than keyCodeInUpperCase so that the original case is preserved
				iFep.MakeDeferredFunctionCall(*this); // deferred call to InsertCompositionCharacterAndDrawNowL
				if (bufferLength==0)
					{
					iCoeEnv->ForEachFepObserverCall(FepObserverHandleStartOfTransactionL); // must be called before inserting the composition character as an application's HandleStartOfTransactionL function may move focus to a control supporting inline-editing
					}
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		if (iInputMethod==EInputMethodPinYin)
			{
			if (!isFocused && (bufferLength>0))
				{
				if (!iPinYinSelector.IsReseted())
					{
					TInt characterCode = iPinYinSelector.CharacterInCurrentPage(keyCodeInUpperCase-'0'-1);
					// below code copy from EPageDown
					if (characterCode != CPinyinSelector::EInvalidChar)
						{
						TInt positionOfNewText = iSelectedCompositionText.LowerPos();
						
						TInt byteCount = 1;		// byte count of current char
						if (bufferLength >= 2 && (iBuffer[positionOfNewText-1] & 0xF800) == 0xD800)
							{
							byteCount = 2;
							}
						if (characterCode > 0x10000 & characterCode < 0x2FFFE)
							{
							if (byteCount == 1)
								{
								_LIT(KOneSpace, " ");
								iBuffer.Insert(positionOfNewText-1, KOneSpace);
								positionOfNewText++;
								SetPositionOfInsertionPointInBuffer(positionOfNewText+1);
								}
							iBuffer[positionOfNewText-2] = (TText)(0xD7C0 + (characterCode >> 10));
							iBuffer[positionOfNewText-1] = (TText)(0xDC00 | (characterCode & 0x03FF));
							}
						else if (characterCode >=0 && characterCode <= 0xFFFF)
							{
							if (byteCount == 2)
								{
								iBuffer.Delete(positionOfNewText-1, 1);
								positionOfNewText--;
								SetPositionOfInsertionPointInBuffer(positionOfNewText);//-1);
								}
							iBuffer[positionOfNewText-1] = characterCode;
							}
						
						if (positionOfNewText < iBuffer.Length())
							{
							// simulate a SPACE keypress, to process next pinyin
							iCharacterCode = 32; // set iCharacterCode to aKeyEvent.iCode rather than keyCodeInUpperCase so that the original case is preserved
							iFep.MakeDeferredFunctionCall(*this); // deferred call to InsertCompositionCharacterAndDrawNowL
							}

						// redraw
						if (iFlags&EFlagInsideInlineEditingTransaction)
							{
							DrawNow();		// for character selector
							SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
							}
						else
							{
							DrawNow();
							}
						FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
						}
					}
				}
			break;
			}
		else
			{
			// goto default case for other IME
			goto __label_default;
			}
	case EKeyHome:
	case EKeyEnd:
//	case EKeyPageUp:
//	case EKeyPageDown:
	case EKeyLeftArrow:
	case EKeyRightArrow:
	case EKeyUpArrow:
	case EKeyDownArrow:
		if (isFocused)
			{
			TPoint offset(0, 0);
			TInt magnification=10;
			switch (keyCodeInUpperCase)
				{
			case EKeyLeftArrow:
				offset.iX=-1;
				break;
			case EKeyRightArrow:
				offset.iX=1;
				break;
			case EKeyUpArrow:
				offset.iY=-1;
				break;
			case EKeyDownArrow:
				offset.iY=1;
				break;
			case EKeyHome:
				offset.iX=-iPosition.iX;
				offset.iY=-iPosition.iY;
				magnification=1;
				break;
			case EKeyEnd:
				{
				const TSize screenWidth(iCoeEnv->ScreenDevice()->SizeInPixels());
				offset.iX=(screenWidth.iWidth-iSize.iWidth)-iPosition.iX;
				offset.iY=(screenWidth.iHeight-iSize.iHeight)-iPosition.iY;
				}
				magnification=1;
				break;
			case EKeyPageUp:
				offset.iY=-iSize.iHeight;
				magnification=1;
				break;
			case EKeyPageDown:
				offset.iY=iSize.iHeight;
				magnification=1;
				break;
#if defined(_DEBUG)
			default:
				Panic(EPanicBadKeyCode3);
				break;
#endif
				}
			if (aKeyEvent.iModifiers&EModifierCtrl)
				{
				offset.iX*=magnification;
				offset.iY*=magnification;
				}
			SetPosition(TPoint(iPosition.iX+offset.iX, iPosition.iY+offset.iY));
			if (iFlags&EFlagWindowIsBeingDragged)
				{
				iPositionOnWindowBeingDragged.iX-=offset.iX;
				iPositionOnWindowBeingDragged.iY-=offset.iY;
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		if (bufferLength>0)
			{
			TInt newPositionOfInsertionPointInBuffer=iSelectedCompositionText.iCursorPos;
			switch (keyCodeInUpperCase)
				{
			case EKeyLeftArrow:
				if (newPositionOfInsertionPointInBuffer>0)
					{
					--newPositionOfInsertionPointInBuffer;
					}
				break;
			case EKeyRightArrow:
				if (newPositionOfInsertionPointInBuffer<bufferLength)
					{
					++newPositionOfInsertionPointInBuffer;
					}
				break;
			case EKeyHome:
			case EKeyPageUp:
			case EKeyUpArrow:
				newPositionOfInsertionPointInBuffer=0;
				break;
			case EKeyEnd:
			case EKeyPageDown:
			case EKeyDownArrow:
				newPositionOfInsertionPointInBuffer=bufferLength;
				break;
#if defined(_DEBUG)
			default:
				Panic(EPanicBadKeyCode4);
				break;
#endif
				}
			if (iSelectedCompositionText.iCursorPos!=newPositionOfInsertionPointInBuffer)
				{
				if ((iSelectedCompositionText.Length()>0) && !(aKeyEvent.iModifiers&EModifierShift))
					{
					if (newPositionOfInsertionPointInBuffer<iSelectedCompositionText.iCursorPos)
						{
						newPositionOfInsertionPointInBuffer=iSelectedCompositionText.LowerPos();
						}
					else
						{
						newPositionOfInsertionPointInBuffer=iSelectedCompositionText.HigherPos();
						}
					}
				}
			const TInt newPositionOfAnchorInBuffer=(aKeyEvent.iModifiers&EModifierShift)? iSelectedCompositionText.iAnchorPos: newPositionOfInsertionPointInBuffer;
			if ((iSelectedCompositionText.iCursorPos!=newPositionOfInsertionPointInBuffer) || (iSelectedCompositionText.iAnchorPos!=newPositionOfAnchorInBuffer))
				{
				const TBool needToDraw=((iSelectedCompositionText.Length()>0) || (aKeyEvent.iModifiers&EModifierShift)); // don't need to draw if there is no selected text as a result of this event, or if there previously was no selected text before this event
				SetPositionOfInsertionPointInBuffer(newPositionOfInsertionPointInBuffer, newPositionOfAnchorInBuffer);
				if (iFlags&EFlagInsideInlineEditingTransaction)
					{
					SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
					}
				else if (needToDraw)
					{
					DrawNow();
					}
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		break;
	default:
__label_default:
		if (isFocused)
			{
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		TBool isLegalCompositionCharacter=EFalse;
		switch (iInputMethod)
			{
		case EInputMethodPlain:
			if (iInputCapabilities.SupportsAllText())
				{
				isLegalCompositionCharacter=keyCodeInUpperCase.IsPrint();
				}
			else
				{
				const TBool isDigit=((keyCodeInUpperCase>='0') && (keyCodeInUpperCase<='9'));
				isLegalCompositionCharacter=(iInputCapabilities.SupportsWesternNumericIntegerPositive() && isDigit);
				if (isLegalCompositionCharacter)
					{
					break;
					}
				isLegalCompositionCharacter=(iInputCapabilities.SupportsWesternNumericIntegerNegative() && (isDigit || ((iSelectedCompositionText.LowerPos()==0) && (keyCodeInUpperCase=='-'))));
				if (isLegalCompositionCharacter)
					{
					break;
					}
				isLegalCompositionCharacter=(iInputCapabilities.SupportsWesternNumericReal() && (isDigit || IsLegalNonDigitForRealNumber(keyCodeInUpperCase)));
				if (isLegalCompositionCharacter)
					{
					break;
					}
				const TUint foldFlags=(TChar::EFoldCase|TChar::EFoldAccents);
				const TUint foldedKeyCode=User::Fold(keyCodeInUpperCase, foldFlags);
				isLegalCompositionCharacter=(iInputCapabilities.SupportsWesternAlphabetic() && (foldedKeyCode>=User::Fold('A', foldFlags)) && (foldedKeyCode<=User::Fold('Z', foldFlags)));
				if (isLegalCompositionCharacter)
					{
					break;
					}
				}
			break;
		case EInputMethodHexadecimalCharacterCode:
			isLegalCompositionCharacter=keyCodeInUpperCase.IsHexDigit();
			break;
		case EInputMethodPinYin:
			isLegalCompositionCharacter = (keyCodeInUpperCase.IsAlpha() || keyCodeInUpperCase.IsSpace());
			break;
		case EInputMethodOnePastTheLast:
#if defined(_DEBUG)
		default:
			Panic(EPanicBadInputMethod3);
#endif
			break;
			}
		if (isLegalCompositionCharacter)
			{
			iCharacterCode=aKeyEvent.iCode; // set iCharacterCode to aKeyEvent.iCode rather than keyCodeInUpperCase so that the original case is preserved
			iFep.MakeDeferredFunctionCall(*this); // deferred call to InsertCompositionCharacterAndDrawNowL
			if (bufferLength==0)
				{
				iCoeEnv->ForEachFepObserverCall(FepObserverHandleStartOfTransactionL); // must be called before inserting the composition character as an application's HandleStartOfTransactionL function may move focus to a control supporting inline-editing
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		if (bufferLength>0)
			{
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
		break;
		}
	FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasNotConsumed); // returns from this function
	}

void CTstControl::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
	switch (aPointerEvent.iType)
		{
	case TPointerEvent::EButton1Down:
		if (IsFocused() ||
			(iFlags&EFlagHasNoCompositionWindow) ||
			(aPointerEvent.iPosition.iX<1) || (aPointerEvent.iPosition.iX>=iSize.iWidth-1) ||
			(aPointerEvent.iPosition.iY<1) || (aPointerEvent.iPosition.iY>=1+EGapAboveCompositionLine+iCompositionFont->HeightInPixels()+EGapBelowCompositionLine))
			{
			iFlags|=EFlagWindowIsBeingDragged;
			iPositionOnWindowBeingDragged=aPointerEvent.iPosition;
			}
		else
			{
			if (~iFlags&EFlagInsideInlineEditingTransaction)
				{
				const TInt newPositionOfInsertionPointInBuffer=PositionInBuffer(aPointerEvent.iPosition.iX);
				const TInt newPositionOfAnchorInBuffer=(aPointerEvent.iModifiers&EModifierShift)? iSelectedCompositionText.iAnchorPos: newPositionOfInsertionPointInBuffer;
				if ((iSelectedCompositionText.iCursorPos!=newPositionOfInsertionPointInBuffer) || (iSelectedCompositionText.iAnchorPos!=newPositionOfAnchorInBuffer))
					{
					const TBool needToDraw=((iSelectedCompositionText.Length()>0) || (aPointerEvent.iModifiers&EModifierShift)); // don't need to draw if there is no selected text as a result of this event, or if there previously was no selected text before this event
					SetPositionOfInsertionPointInBuffer(newPositionOfInsertionPointInBuffer, newPositionOfAnchorInBuffer);
					if (needToDraw)
						{
						DrawNow();
						}
					}
				}
			}
		break;
	case TPointerEvent::EDrag:
		if (iFlags&EFlagWindowIsBeingDragged)
			{
			SetPosition(aPointerEvent.iParentPosition-iPositionOnWindowBeingDragged);
			}
		else
			{
			if (~iFlags&EFlagInsideInlineEditingTransaction)
				{
				const TInt newPositionOfInsertionPointInBuffer=PositionInBuffer(aPointerEvent.iPosition.iX);
				if (iSelectedCompositionText.iCursorPos!=newPositionOfInsertionPointInBuffer)
					{
					SetPositionOfInsertionPointInBuffer(newPositionOfInsertionPointInBuffer, iSelectedCompositionText.iAnchorPos);
					DrawNow();
					}
				}
			}
		break;
	case TPointerEvent::EButton1Up:
		iFlags&=~EFlagWindowIsBeingDragged;
		break;
#if defined(__GCC32__)
	default:
		break;
#endif
		}
	}

void CTstControl::Draw(const TRect&) const
	{
	CWindowGc& graphicsContext=SystemGc();
	graphicsContext.SetPenStyle(CGraphicsContext::ESolidPen);
	graphicsContext.SetPenColor(KRgbBlack);
	graphicsContext.SetBrushStyle(CGraphicsContext::ENullBrush);
	TRect rectangle(Rect());
	graphicsContext.DrawRect(rectangle);
	rectangle.Shrink(1, 1);
	graphicsContext.SetBrushStyle(CGraphicsContext::ESolidBrush);
	if (IsFocused())
		{
		__ASSERT_DEBUG(iBuffer.Length()==0, Panic(EPanicInconsistentState));
		// the pen color is still black here
		graphicsContext.SetBrushColor(KRgbBlack);
		graphicsContext.DrawRect(rectangle);
		const TPoint center=rectangle.Center();
		graphicsContext.SetPenColor(KRgbWhite);
		const TPoint leftArrowHead((rectangle.iTl.iX+center.iX)/2, center.iY);
		const TPoint rightArrowHead((center.iX+rectangle.iBr.iX)/2, center.iY);
		const TPoint topArrowHead(center.iX, (rectangle.iTl.iY+center.iY)/2);
		const TPoint bottomArrowHead(center.iX, (center.iY+rectangle.iBr.iY)/2);
		graphicsContext.DrawLine(leftArrowHead, rightArrowHead);
		graphicsContext.Plot(rightArrowHead);
		graphicsContext.DrawLine(topArrowHead, bottomArrowHead);
		graphicsContext.Plot(bottomArrowHead);
		graphicsContext.DrawLine(TPoint(leftArrowHead.iX+EArrowHeadSize, leftArrowHead.iY+EArrowHeadSize), leftArrowHead);
		graphicsContext.DrawLine(TPoint(leftArrowHead.iX+EArrowHeadSize, leftArrowHead.iY-EArrowHeadSize), leftArrowHead);
		graphicsContext.DrawLine(TPoint(rightArrowHead.iX-EArrowHeadSize, rightArrowHead.iY-EArrowHeadSize), rightArrowHead);
		graphicsContext.DrawLine(TPoint(rightArrowHead.iX-EArrowHeadSize, rightArrowHead.iY+EArrowHeadSize), rightArrowHead);
		graphicsContext.DrawLine(TPoint(topArrowHead.iX-EArrowHeadSize, topArrowHead.iY+EArrowHeadSize), topArrowHead);
		graphicsContext.DrawLine(TPoint(topArrowHead.iX+EArrowHeadSize, topArrowHead.iY+EArrowHeadSize), topArrowHead);
		graphicsContext.DrawLine(TPoint(bottomArrowHead.iX+EArrowHeadSize, bottomArrowHead.iY-EArrowHeadSize), bottomArrowHead);
		graphicsContext.DrawLine(TPoint(bottomArrowHead.iX-EArrowHeadSize, bottomArrowHead.iY-EArrowHeadSize), bottomArrowHead);
		}
	else
		{
		TRect temp(rectangle);
		temp.iBr.iY=temp.iTl.iY;
		graphicsContext.SetBrushColor(KRgbWhite);
		if (~iFlags&EFlagHasNoCompositionWindow)
			{
			graphicsContext.UseFont(iCompositionFont);
			temp.iBr.iY+=EGapAboveCompositionLine+iCompositionFont->HeightInPixels()+EGapBelowCompositionLine;
			TRect temp2=temp;
			const TInt startOfSelectedCompositionText=iSelectedCompositionText.LowerPos();
			const TPtrC textBeforeSelectedText(iBuffer.Left(startOfSelectedCompositionText));
			temp2.iBr.iX=temp2.iTl.iX;
			temp2.iBr.iX+=iCompositionFont->TextWidthInPixels(textBeforeSelectedText);
			graphicsContext.SetPenColor(KRgbRed);
			graphicsContext.DrawText(textBeforeSelectedText, temp2, EGapAboveCompositionLine+iCompositionFont->AscentInPixels(), CGraphicsContext::ELeft, EGapLeftOfEachLine);
			
			const TPtrC selectedText(iBuffer.Mid(startOfSelectedCompositionText, iSelectedCompositionText.Length()));
			temp2.iTl.iX=temp2.iBr.iX;
			temp2.iBr.iX+=iCompositionFont->TextWidthInPixels(selectedText);
			graphicsContext.SetPenColor(KRgbGreen);
			graphicsContext.DrawText(selectedText, temp2, EGapAboveCompositionLine+iCompositionFont->AscentInPixels(), CGraphicsContext::ELeft, EGapLeftOfEachLine);
			
			const TPtrC textAfterSelectedText(iBuffer.Mid(iSelectedCompositionText.HigherPos()));
			temp2.iTl.iX=temp2.iBr.iX;
			temp2.iBr.iX=temp.iBr.iX;
			graphicsContext.SetPenColor(KRgbBlue);//KRgbRed);
			graphicsContext.DrawText(textAfterSelectedText, temp2, EGapAboveCompositionLine+iCompositionFont->AscentInPixels(), CGraphicsContext::ELeft, EGapLeftOfEachLine);
			graphicsContext.DiscardFont();
			graphicsContext.SetPenColor(KRgbBlack);
			}
		
		TBuf<200> textToDisplay;
		const TInt statusFontHeightInPixels=iStatusFont->HeightInPixels();
		const TInt statusFontAscentInPixels=iStatusFont->AscentInPixels();
		
		// character selector
		graphicsContext.SetBrushColor(KRgbBlue);
		graphicsContext.SetPenColor(KRgbYellow);
		graphicsContext.UseFont(iCompositionFont);
		iPinYinSelector.GetPageDescriptor(&textToDisplay);
		temp.iTl.iY = temp.iBr.iY;
		temp.iBr.iY += EGapAboveCompositionLine + iCompositionFont->HeightInPixels() + EGapBelowCompositionLine;
		graphicsContext.DrawText(textToDisplay, temp, EGapAboveCompositionLine+iCompositionFont->AscentInPixels(), CGraphicsContext::ELeft, EGapLeftOfEachLine);
		graphicsContext.DiscardFont();
		
		graphicsContext.SetBrushColor(KRgbGray);
		graphicsContext.SetPenColor(KRgbBlack);
		graphicsContext.UseFont(iStatusFont);

		// Input method: ...
		textToDisplay=KLitInputMethodColonSpace;
		switch (iInputMethod)
			{
		case EInputMethodPlain:
			textToDisplay.Append(KLitPlain);
			break;
		case EInputMethodHexadecimalCharacterCode:
			textToDisplay.Append(KLitHexadecimalCharacterCode);
			break;
		case EInputMethodPinYin:
			textToDisplay.Append(KLitPinYin);
			break;
		default:
#if defined(_DEBUG)
			Panic(EPanicBadInputMethod5);
#endif
			break;
			}
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=EGapAboveTopStatusLine+statusFontHeightInPixels+EGapBetweenEachStatusLine;
		graphicsContext.DrawText(textToDisplay, temp, EGapAboveTopStatusLine+statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		
		// Inline editing: ...
		textToDisplay=KLitInlineEditingColonSpace;
		textToDisplay.Append((iFlags&EFlagInlineEditingEnabled)? KLitEnabled(): KLitDisabled());
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=statusFontHeightInPixels+EGapBetweenEachStatusLine;
		graphicsContext.DrawText(textToDisplay, temp, statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		
		// [Caption: "..."]
		textToDisplay=KLitOpeningSquareBracket;
		textToDisplay.Append(KLitCaptionColonSpace);
		textToDisplay.Append(KLitQuotationMark);
		const MCoeCaptionRetrieverForFep* captionRetrieverForFep=iInputCapabilities.CaptionRetrieverForFep();
		if (captionRetrieverForFep!=NULL)
			{
			TBuf<51> caption;
			captionRetrieverForFep->GetCaptionForFep(caption);
			const TInt captionLength=caption.Length();
			if (captionLength==caption.MaxLength())
				{
				caption[captionLength-1]=KEllipsisCharacter;
				}
			textToDisplay.Append(caption);
			}
		textToDisplay.Append(KLitQuotationMark);
		textToDisplay.Append(KLitClosingSquareBracket);
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=statusFontHeightInPixels+EGapBetweenEachStatusLine;
		graphicsContext.DrawText(textToDisplay, temp, statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		
		// [State: "..."]
		textToDisplay=KLitOpeningSquareBracket;
		textToDisplay.Append(KLitStateColonSpace);
		textToDisplay.Append(KLitQuotationMark);
		MCoeFepAwareTextEditor* const fepAwareTextEditor=iInputCapabilities.FepAwareTextEditor();
		if (fepAwareTextEditor!=NULL)
			{
			MCoeFepAwareTextEditor_Extension1* const extension1=fepAwareTextEditor->Extension1();
			if (extension1!=NULL)
				{
				MCoeFepAwareTextEditor_Extension1::CState* const state=extension1->State(TUid::Uid(KTestFep4Uid));
				if (state!=NULL)
					{
					textToDisplay.AppendNum(STATIC_CAST(CStateInformation*, state)->MeaninglessNumber()); // this cast is safe as KTestFep4Uid is used to retrieve state
					}
				}
			}
		textToDisplay.Append(KLitQuotationMark);
		textToDisplay.Append(KLitClosingSquareBracket);
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=statusFontHeightInPixels+EGapBetweenEachStatusLine;
		graphicsContext.DrawText(textToDisplay, temp, statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		
		// [Input-capabilities: ...]
		textToDisplay=KLitOpeningSquareBracket;
		textToDisplay.Append(KLitInputCapabilitiesColonSpace);
		if (iInputCapabilities.IsNone())
			{
			textToDisplay.Append(KLitNone);
			}
		else
			{
			TBool deleteLastSeparator=EFalse;
			if (iInputCapabilities.SupportsWesternNumericIntegerPositive())
				{
				textToDisplay.Append(KLitWesternNumericIntegerPositive);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsWesternNumericIntegerNegative())
				{
				textToDisplay.Append(KLitWesternNumericIntegerNegative);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsWesternNumericReal())
				{
				textToDisplay.Append(KLitWesternNumericReal);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsWesternAlphabetic())
				{
				textToDisplay.Append(KLitWesternAlphabetic);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsJapaneseHiragana())
				{
				textToDisplay.Append(KLitJapaneseHiragana);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsJapaneseKatakanaHalfWidth())
				{
				textToDisplay.Append(KLitJapaneseKatakanaHalfWidth);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsJapaneseKatakanaFullWidth())
				{
				textToDisplay.Append(KLitJapaneseKatakanaFullWidth);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsDialableCharacters())
				{
				textToDisplay.Append(KLitDialableCharacters);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsSecretText())
				{
				textToDisplay.Append(KLitSecretText);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsAllText())
				{
				textToDisplay.Append(KLitAllText);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			if (iInputCapabilities.SupportsNavigation())
				{
				textToDisplay.Append(KLitNavigation);
				textToDisplay.Append(KLitCommaSpace);
				deleteLastSeparator=ETrue;
				}
			const TCoeInputCapabilities::MCoeFepSpecificExtensions* const fepSpecificInputCapabilityExtensions=iInputCapabilities.FepSpecificExtensions(TUid::Uid(KTestFep4Uid));
			if (fepSpecificInputCapabilityExtensions!=NULL)
				{
				textToDisplay.Append(KLitExtensionsColonSpace);
				if (fepSpecificInputCapabilityExtensions->SupportsPhoneticAlphabet())
					{
					textToDisplay.Append(KLitPhoneticAlphabet);
					textToDisplay.Append(KLitCommaSpace);
					deleteLastSeparator=ETrue;
					}
				}
			if (deleteLastSeparator)
				{
				const TInt lengthToDelete=KLitCommaSpace().Length();
				textToDisplay.Delete(textToDisplay.Length()-lengthToDelete, lengthToDelete);
				}
			}
		textToDisplay.Append(KLitClosingSquareBracket);
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY=rectangle.iBr.iY;
		__ASSERT_DEBUG(temp.iBr.iY==temp.iTl.iY+statusFontHeightInPixels+EGapBelowBottomStatusLine, Panic(EPanicBadHeight));
		graphicsContext.DrawText(textToDisplay, temp, statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		graphicsContext.DiscardFont();
		}
	}

void CTstControl::DrawText(const TParam& aParam, const TLineInfo& aLineInfo, const TCharFormat& aFormat, const TDesC& aText, const TPoint& aTextOrigin, TInt aExtraPixels) const
	{
	MFormCustomDraw::DrawText(aParam, aLineInfo, aFormat, aText, aTextOrigin, aExtraPixels);
	// ?? draw the dotted underline whose phase starts at aParam.iTextLayoutTopLeft.iX
	}

void CTstControl::GetFormatOfFepInlineText(TCharFormat& aFormat, TInt& aNumberOfCharactersWithSameFormat, TInt aPositionOfCharacter) const
	{
	const TInt lengthOfSelection=iSelectedCompositionText.Length();
	const TInt startOfSelectedInlineText=iSelectedCompositionText.LowerPos();
	const TInt endOfSelectedInlineText=iSelectedCompositionText.HigherPos();
	const MCoeFepAwareTextEditor& fepAwareTextEditor=*iInputCapabilities.FepAwareTextEditor();
	TCursorSelection cursorSelection;
	fepAwareTextEditor.GetCursorSelectionForFep(cursorSelection);
	TCharFormat formatBeforeInlineText;
	GetFormatAtDocumentPosition(formatBeforeInlineText, cursorSelection.LowerPos()-1, fepAwareTextEditor);
	TCharFormat formatAfterInlineText;
	GetFormatAtDocumentPosition(formatAfterInlineText, cursorSelection.HigherPos()+1, fepAwareTextEditor);
	aFormat=formatBeforeInlineText;
	if ((lengthOfSelection>0) && (aPositionOfCharacter>=startOfSelectedInlineText) && (aPositionOfCharacter<endOfSelectedInlineText))
		{
		aFormat.iFontPresentation.iTextColor=KRgbGreen;
		}
	else
		{
		aFormat.iFontPresentation.iTextColor=KRgbRed;
		}
	if ((formatBeforeInlineText.iFontPresentation.iUnderline!=EUnderlineOn) && (formatAfterInlineText.iFontPresentation.iUnderline!=EUnderlineOn))
		{
		aFormat.iFontPresentation.iUnderline=EUnderlineOn;
		}
	else if ((formatBeforeInlineText.iFontPresentation.iUnderline!=EUnderlineOff) && (formatAfterInlineText.iFontPresentation.iUnderline!=EUnderlineOff))
		{
		aFormat.iFontPresentation.iUnderline=EUnderlineOff;
		}
	else
		{
		aFormat.iFontSpec.iFontStyle.SetPrintPosition(EPrintPosSuperscript);
		}
	const TInt endOfRunOfCharactersOfSameFormat=((lengthOfSelection==0) || (aPositionOfCharacter>=endOfSelectedInlineText))?
													iBuffer.Length():
												(aPositionOfCharacter>=startOfSelectedInlineText)?
													endOfSelectedInlineText:
													startOfSelectedInlineText;
	aNumberOfCharactersWithSameFormat=endOfRunOfCharactersOfSameFormat-aPositionOfCharacter;
	}

void CTstControl::HandlePointerEventInInlineTextL(TPointerEvent::TType aType, TUint, TInt aPositionInInlineText)
	{
	MCoeFepAwareTextEditor* const fepAwareTextEditor=iInputCapabilities.FepAwareTextEditor();
	__ASSERT_DEBUG(fepAwareTextEditor!=NULL, Panic(EPanicNoFepAwareTextEditorAlthoughCurrentlyInlineEditing));
	const TCursorSelection oldSelectedCompositionText=iSelectedCompositionText;
	switch (aType)
		{
	case TPointerEvent::EButton1Down:
		iSelectedCompositionText.iAnchorPos=aPositionInInlineText;
		// fall through
	case TPointerEvent::EDrag:
		iSelectedCompositionText.iCursorPos=aPositionInInlineText;
		break;
#if defined(__GCC32__)
	default:
		break;
#endif
		}
	if ((iSelectedCompositionText.iCursorPos!=oldSelectedCompositionText.iCursorPos) || (iSelectedCompositionText.iAnchorPos!=oldSelectedCompositionText.iAnchorPos))
		{
		fepAwareTextEditor->UpdateFepInlineTextL(iBuffer, iSelectedCompositionText.iCursorPos);
		}
	}

void CTstControl::ExecuteFunctionL()
	{
	InsertCompositionCharacterAndDrawNowL(iCharacterCode);
	}

// CTstControl::CStateInformation

CTstControl::CStateInformation* CTstControl::CStateInformation::NewLC(TInt aMeaninglessNumber)
	{
	CStateInformation* const stateInformation=new(ELeave) CStateInformation;
	CleanupStack::PushL(stateInformation);
	stateInformation->ConstructL(aMeaninglessNumber);
	return stateInformation;
	}

CTstControl::CStateInformation::~CStateInformation()
	{
	delete iMeaninglessNumber;
	}

CTstControl::CStateInformation::CStateInformation()
	:iMeaninglessNumber(NULL)
	{
	}

void CTstControl::CStateInformation::ConstructL(TInt aMeaninglessNumber)
	{
	BaseConstructL();
	iMeaninglessNumber=new(ELeave) TInt(aMeaninglessNumber);
	}

// CTstFep

CTstFep::CTstFep(CCoeEnv& aConeEnvironment)
	:CCoeFep(aConeEnvironment)
	{
	}

void CTstFep::ConstructL(const CCoeFepParameters& aFepParameters)
	{
	BaseConstructL(aFepParameters);
	iControl=CTstControl::NewL(*this);
	ReadAllAttributesL();
	iControl->ActivateL();
	}

CTstFep::~CTstFep()
	{
	delete iControl;
	}

void CTstFep::CancelTransaction()
	{
	iControl->CancelTransaction();
	}

void CTstFep::IsOnHasChangedState()
	{
	iControl->IsOnHasChangedState();
	}

void CTstFep::OfferKeyEventL(TEventResponse& aEventResponse, const TKeyEvent& aKeyEvent, TEventCode aEventCode)
	{
	// this function must correctly set aEventResponse *before* calling anything that can leave
	aEventResponse=EEventWasConsumed; // this assumes that CTstControl::OfferKeyEventL will not leave if it returns EKeyWasNotConsumed
	switch (iControl->OfferKeyEventL(aKeyEvent, aEventCode))
		{
	case EKeyWasNotConsumed:
		aEventResponse=EEventWasNotConsumed;
		break;
	case EKeyWasConsumed:
		aEventResponse=EEventWasConsumed;
		break;
#if defined(_DEBUG)
	default:
		Panic(EPanicBadKeyResponse);
		break;
#endif
		}
	}

void CTstFep::OfferPointerEventL(TEventResponse& aEventResponse, const TPointerEvent& aPointerEvent, const CCoeControl* aWindowOwningControl)
	{
	iControl->OfferPointerEventL(aEventResponse, aPointerEvent, aWindowOwningControl);
	}

void CTstFep::OfferPointerBufferReadyEventL(TEventResponse& aEventResponse, const CCoeControl*)
	{
	aEventResponse=EEventWasNotConsumed;
	}

TInt CTstFep::NumberOfAttributes() const
	{
	return CTstControl::NumberOfAttributes();
	}

TUid CTstFep::AttributeAtIndex(TInt aIndex) const
	{
	return CTstControl::AttributeAtIndex(aIndex);
	}

void CTstFep::WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const
	{
	iControl->WriteAttributeDataToStreamL(aAttributeUid, aStream);
	}

void CTstFep::ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream)
	{
	iControl->ReadAttributeDataFromStreamL(aAttributeUid, aStream);
	}

void CTstFep::HandleGainingForeground()
	{
	iControl->HandleGainingForeground();
	}

void CTstFep::HandleLosingForeground()
	{
	iControl->HandleLosingForeground();
	}

void CTstFep::HandleChangeInFocus()
	{
	iControl->HandleChangeInFocus();
	}

void CTstFep::HandleDestructionOfFocusedItem()
	{
	iControl->HandleDestructionOfFocusedItem();
	}

// TTstResourceFileId

#pragma warning(disable: 4355) // "'this' : used in base member initializer list"

TTstResourceFileId::TTstResourceFileId(CCoeEnv& aConeEnvironment, TInt aResourceFileId)
	:TCleanupItem(UnloadResourceFile, this),
	 iConeEnvironment(aConeEnvironment),
	 iResourceFileId(aResourceFileId)
	{
	}

#pragma warning(default: 4355)

void TTstResourceFileId::UnloadResourceFile(TAny* aThis)
	{
	TTstResourceFileId& resourceFileId=*STATIC_CAST(TTstResourceFileId*, aThis);
	resourceFileId.iConeEnvironment.DeleteResourceFile(resourceFileId.iResourceFileId);
	}

// CTstSettingsDialog

CTstSettingsDialog::CTstSettingsDialog()
	:iInlineEditingEnabled(ETrue),
	 iInputMethod(0)
	{
	}

TBool CTstSettingsDialog::OkToExitL(TInt aButtonId)
	{
	__ASSERT_ALWAYS(aButtonId==EEikBidOk, Panic(EPanicUnexpectedButtonId));
	switch (STATIC_CAST(CEikCheckBox*, Control(EControlIdInlineEditingEnabled))->State())
		{
	case CEikButtonBase::EClear:
		iInlineEditingEnabled=EFalse;
		break;
	case CEikButtonBase::ESet:
		iInlineEditingEnabled=ETrue;
		break;
	case CEikButtonBase::EIndeterminate:
#if defined(_DEBUG)
	default:
		Panic(EPanicBadCheckBoxState);
#endif
		break;
		}
	iInputMethod=STATIC_CAST(CEikChoiceListBase*, Control(EControlIdInputMethod))->CurrentItem();
	TFixedArray<TUid, 2> attributeUids;
	__ASSERT_DEBUG(NumberOfAttributes()==2, Panic(EPanicBadNumberOfAttributes));
	attributeUids[0].iUid=AttributeAtIndex(0).iUid;
	attributeUids[1].iUid=AttributeAtIndex(1).iUid;
	WriteAttributeDataAndBroadcastL(*iCoeEnv, attributeUids.Array());
	return ETrue;
	}

void CTstSettingsDialog::PreLayoutDynInitL()
	{
	ReadAllAttributesL(*iCoeEnv);
	STATIC_CAST(CEikCheckBox*, Control(EControlIdInlineEditingEnabled))->SetState(iInlineEditingEnabled? CEikButtonBase::ESet: CEikButtonBase::EClear);
	STATIC_CAST(CEikChoiceListBase*, Control(EControlIdInputMethod))->SetCurrentItem(iInputMethod);
	}

TInt CTstSettingsDialog::NumberOfAttributes() const
	{
	return CTstControl::NumberOfAttributes();
	}

TUid CTstSettingsDialog::AttributeAtIndex(TInt aIndex) const
	{
	return CTstControl::AttributeAtIndex(aIndex);
	}

void CTstSettingsDialog::WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const
	{
	CTstControl::WriteAttributeDataToStreamL(aAttributeUid, aStream, iInlineEditingEnabled, iInputMethod);
	}

void CTstSettingsDialog::ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream)
	{
	CTstControl::ReadAttributeDataFromStreamL(aAttributeUid, aStream, iInlineEditingEnabled, iInputMethod);
	}

CPinyinSelector::CPinyinSelector()
	{
	iPinyinTable = &pinyinTable;
	iPageSize = 6;
	iReset = ETrue;
	iCurrentPinyin = NULL;
	}

TBool CPinyinSelector::SetPinyin(const TDesC *aCurrentPinyin, TInt aIndexInCurrentPinyin)
	{
	TInt index;
	TBool found = EFalse;
	iReset = ETrue;
	for (index=0; index<iPinyinTable->iPinyinCharacterPairCount; index++)
		{
//		if (aCurrentPinyin->Compare(iPinyinTable->iPairArray[index].iPinYin) == 0)
		TPtrC py(iPinyinTable->iPairArray[index].iPinYin, iPinyinTable->iPairArray[index].iPinYinLength);
		if (aCurrentPinyin->Compare(py) == 0)
			{
			iCurrentPinyin = &(iPinyinTable->iPairArray[index]);
			iIndexInCurrentPinyin = aIndexInCurrentPinyin;
			found = ETrue;
			iReset = EFalse;
			return found;
			}
		}
	if (!found)
		{
#if defined(_DEBUG)
//		Panic(EPanicBadIndex1);
#endif
		return found;
		}
	return found;
	}

TInt CPinyinSelector::PreviousCharacter()
	{
	if (iReset)
		return EInvalidChar;
	if (iIndexInCurrentPinyin > 0)
		{
		iIndexInCurrentPinyin--;
		}
	if (iIndexInCurrentPinyin < 0)
		iIndexInCurrentPinyin = 0;
	if (iIndexInCurrentPinyin > iCurrentPinyin->iCharacterCount - 1)
		iIndexInCurrentPinyin = iCurrentPinyin->iCharacterCount - 1;
	return iCurrentPinyin->iCharacterArray[iIndexInCurrentPinyin];
	}

TInt CPinyinSelector::NextCharacter()
	{
	if (iReset)
		return EInvalidChar;
	if (iIndexInCurrentPinyin < iCurrentPinyin->iCharacterCount - 1)
		{
		iIndexInCurrentPinyin++;
		}
	if (iIndexInCurrentPinyin < 0)
		iIndexInCurrentPinyin = 0;
	if (iIndexInCurrentPinyin > iCurrentPinyin->iCharacterCount - 1)
		iIndexInCurrentPinyin = iCurrentPinyin->iCharacterCount - 1;
	return iCurrentPinyin->iCharacterArray[iIndexInCurrentPinyin];
	}

void CPinyinSelector::SetPageSize(TInt nPageSize)
	{
	iPageSize = nPageSize;
	if (iPageSize < 1)
		iPageSize = 1;
	if (iPageSize > 8)
		iPageSize = 8;
	}

TInt CPinyinSelector::NextPage()
	{
	if (iReset)
		return EInvalidChar;
	
	TInt firstIndexInThisPage = ((iIndexInCurrentPinyin+iPageSize-1) / iPageSize) * iPageSize;
	if (firstIndexInThisPage + iPageSize < iCurrentPinyin->iCharacterCount)
		{
		iIndexInCurrentPinyin = firstIndexInThisPage + iPageSize;
		}
	if (iIndexInCurrentPinyin < 0)
		iIndexInCurrentPinyin = 0;
	if (iIndexInCurrentPinyin > iCurrentPinyin->iCharacterCount - 1)
		iIndexInCurrentPinyin = iCurrentPinyin->iCharacterCount - 1;
	return iCurrentPinyin->iCharacterArray[iIndexInCurrentPinyin];
	}

TInt CPinyinSelector::PreviousPage()
	{
	if (iReset)
		return EInvalidChar;
	
	TInt firstIndexInThisPage = ((iIndexInCurrentPinyin+iPageSize-1) / iPageSize) * iPageSize;
	if (firstIndexInThisPage - iPageSize >= 0)
		{
		iIndexInCurrentPinyin = firstIndexInThisPage - iPageSize;
		}
	if (iIndexInCurrentPinyin < 0)
		iIndexInCurrentPinyin = 0;
	if (iIndexInCurrentPinyin > iCurrentPinyin->iCharacterCount - 1)
		iIndexInCurrentPinyin = iCurrentPinyin->iCharacterCount - 1;
	return iCurrentPinyin->iCharacterArray[iIndexInCurrentPinyin];
	}

TInt CPinyinSelector::CharacterInCurrentPage(TInt aIndex)
	{
	if (aIndex < 0)
		aIndex = 0;
	
	TInt firstIndexInThisPage = ((iIndexInCurrentPinyin+iPageSize-1) / iPageSize) * iPageSize;
	aIndex += firstIndexInThisPage;
	if (aIndex > iCurrentPinyin->iCharacterCount - 1)
		aIndex = iCurrentPinyin->iCharacterCount - 1;
	
	return iCurrentPinyin->iCharacterArray[aIndex];
	}

void CPinyinSelector::GetPageDescriptor(TBuf<200> *page) const
	{
	page->Zero();
#if 0
	// for debug
	_LIT(KTemp, "\x5DEE");
	page->Append(KTemp);
#endif
	if (iReset || iCurrentPinyin == NULL)
		return ;

	TInt firstIndexInThisPage = ((iIndexInCurrentPinyin+iPageSize-1) / iPageSize) * iPageSize;
	TInt index = firstIndexInThisPage;
	while (index < firstIndexInThisPage+iPageSize && index < iCurrentPinyin->iCharacterCount)
		{
		_LIT(KFormat, "%d%c");
		_LIT(KOneSpace, " ");
		
		if (index != firstIndexInThisPage)
			page->Append(KOneSpace);
		TInt theChar = iCurrentPinyin->iCharacterArray[index];
		TBuf<200> temp;
		if (theChar < 0x10000)
			{
			temp.Format(KFormat, index-firstIndexInThisPage+1, theChar);
			}
		else
			{
			temp.Format(KFormat, index-firstIndexInThisPage+1, L' ');
			}
		page->Append(temp);
		index++;
		}
	}

TBool CPinyinSelector::IsValidPinyinChar(TInt aChar)
	{
	TBool b = EFalse;
	if ('a' <= aChar && aChar <= 'z')
		b = ETrue;
	else if ('A' <= aChar && aChar <= 'Z')
		b = ETrue;
	return b;
	}

void CPinyinSelector::Reset()
	{
	iReset = ETrue;
	}

TBool CPinyinSelector::IsReseted()
	{
	return iReset;
	}
