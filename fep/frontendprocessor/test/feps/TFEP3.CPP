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

/**
 @file
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
#include <bautils.h>
#include <techview/eikon.hrh>
#include <techview/eikdialg.h>
#include <techview/eikchkbx.h>
#include <techview/eikbutb.h>
#include <techview/eikchlst.h>

#include "tfep3.hrh"
#include "TFEP3.H"

#define DEBUGGING_MESSAGES
#if defined(DEBUGGING_MESSAGES)
#include <e32svr.h>
#endif

// constants

enum TPanic
	{
	EPanicBackupAlreadyExists=1,
	EPanicNotExpectedToBeSimulatingKeyEvent1,
	EPanicNotExpectedToBeSimulatingKeyEvent2,
	EPanicBadIndex,
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

LOCAL_D const TUint Alpha[10][4] = {{0,0,0,0},{0,0,0,0},{65,66,67,0},{68,69,70,0},{71,72,73,0},{74,75,76,0},{77,78,79,0},{80,81,82,83},{84,85,86,0},{87,88,89,90}} ;

_LIT(KLitTFEP3, "TFEP3");
#if defined(_UNICODE)
const TUint KEllipsisCharacter=0x2026;
_LIT(KLitCompositionFontTypefaceName, "publicDomainUnicode");
#else
const TUint KEllipsisCharacter=0x85;
_LIT(KLitCompositionFontTypefaceName, "courier");
#endif
_LIT(KLitStatusFontTypefaceName, "arial");
_LIT(KLitNotAvailable, "[not available]");
//_LIT(KLitTooLong, "[too long]");
_LIT(KLitQuotationMark, "\"");
_LIT(KLitTextBeforeSelectionColonSpace, "Text before selection: ");
_LIT(KLitTextAfterSelectionColonSpace, "Text after selection: ");
_LIT(KLitSelectionColonSpace, "Selection: ");
_LIT(KLitInputMethodColonSpace, "Input method: ");
_LIT(KLitPlain, "plain");
_LIT(KLitHexadecimalCharacterCode, "hexadecimal character-code");
_LIT(KLitNumberKeyPad, "number keypad");
_LIT(KLitInlineEditingColonSpace, "Inline editing: ");
_LIT(KLitEnabled, "enabled");
_LIT(KLitDisabled, "disabled");
_LIT(KLitOpeningSquareBracket, "[");
_LIT(KLitCaptionColonSpace, "Caption: ");
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
_LIT(KLitCommaSpace, ", ");
_LIT(KLitClosingSquareBracket, "]");

// local and global functions

LOCAL_C void Panic(TPanic aPanic)
	{
	User::Panic(KLitTFEP3, aPanic);
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
	delete iMoveCursorTimer;
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
	// InlineEditingEnabled & InputMethod...change this if want to add any more attributes...
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
		Panic(EPanicBadIndex);
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
		if ((inputMethod!=EInputMethodPlain) && (inputMethod!=EInputMethodHexadecimalCharacterCode) && (inputMethod!=EInputMethodNumberKeyPad))
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
	 iInputMethod(EInputMethodPlain),
	 iFlags(0),
	 iBuffer(KNullDesC),
	 iSelectedCompositionText(0, 0),
	 iInputCapabilities(TCoeInputCapabilities::ENone),
	 iCompositionFont(NULL),
	 iStatusFont(NULL),
	 iInsertionPoint(NULL),
	 iPositionOnWindowBeingDragged(0, 0),
	 iCountKeyPressed(0),
	 iLastKeyPressed(0)
	{
	}

void CTstControl::ConstructL()
	{
	iMoveCursorTimer = CTstControl::CMoveCursorTimer::NewL(*this);
	CreateWindowL();
	EnableDragEvents();
	ClaimPointerGrab();
	SetNonFocusing();
	RDrawableWindow& window=*DrawableWindow();
	window.SetOrdinalPosition(0, ECoeWinPriorityFep);
	window.SetShadowHeight(3);
	TFontSpec compositionFontSpec(KLitCompositionFontTypefaceName, 200);
	compositionFontSpec.iTypeface.SetIsProportional(EFalse);
	iCompositionFont=iCoeEnv->CreateScreenFontL(compositionFontSpec);
	iStatusFont=iCoeEnv->CreateScreenFontL(TFontSpec(KLitStatusFontTypefaceName, 120));
	iInsertionPoint=CTstInsertionPoint::NewL(window, *iCoeEnv);
	SetPositionOfInsertionPointInBuffer(0);
	const TInt statusFontHeightInPixels=iStatusFont->HeightInPixels();
	const TSize size(320, 1+EGapAboveCompositionLine+iCompositionFont->HeightInPixels()+EGapBelowCompositionLine+EGapAboveTopStatusLine+(3*(statusFontHeightInPixels+EGapBetweenEachStatusLine))+statusFontHeightInPixels+EGapBelowBottomStatusLine+1);
	const TSize screenSize=iCoeEnv->ScreenDevice()->SizeInPixels();
	SetExtent(TPoint(screenSize.iWidth-(size.iWidth+10), screenSize.iHeight-(size.iHeight+10)), size);
	STATIC_CAST(CCoeAppUi*, iCoeEnv->AppUi())->AddToStackL(this, ECoeStackPriorityFep, ECoeStackFlagRefusesFocus|ECoeStackFlagSharable);
	ChangeSetupAndResetBufferAndDrawNow(SetForeground, iCoeEnv->RootWin().Identifier()==iCoeEnv->WsSession().GetFocusWindowGroup());
	ChangeSetupAndResetBufferAndDrawNow(SetFocus, EFalse);
	ChangeSetupAndResetBufferAndDrawNow(SetFlagInlineEditingEnabled, ETrue);
	ChangeSetupAndResetBufferAndDrawNow(SetInputMethod, EInputMethodPlain);
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
		}
	}

void CTstControl::SetFlagInlineEditingEnabled(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	SetFlag(aControl, aChangeWasMade, aParameter, EFlagInlineEditingEnabled);
	}

void CTstControl::SetInputMethod(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	__ASSERT_DEBUG((aParameter==EInputMethodPlain) || (aParameter==EInputMethodHexadecimalCharacterCode) || (aParameter ==EInputMethodNumberKeyPad), Panic(EPanicBadInputMethod1));
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
	if (!aControl.IsFocused())
		{
		const TCoeInputCapabilities inputCapabilities((aParameter!=NULL)? *REINTERPRET_CAST(const TCoeInputCapabilities*, aParameter): STATIC_CAST(const CCoeAppUi*, aControl.iCoeEnv->AppUi())->InputCapabilities());
		if (aControl.iInputCapabilities!=inputCapabilities)
			{
			aControl.iInputCapabilities=inputCapabilities;
			aChangeWasMade=ETrue;
			}
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

// The CMoveCursorTimer 

CTstControl::CMoveCursorTimer::~CMoveCursorTimer()
{
	// Cancel and delete anything
	Cancel();
}


CTstControl::CMoveCursorTimer::CMoveCursorTimer(CTstControl& aControl):CTimer(EPriorityStandard),iControl(aControl)
{
		CActiveScheduler::Add(this);
}

CTstControl::CMoveCursorTimer* CTstControl::CMoveCursorTimer::NewL(CTstControl& aControl)
{
	CMoveCursorTimer* cursorTimer = new (ELeave) CMoveCursorTimer(aControl);
	CleanupStack::PushL(cursorTimer);
	cursorTimer->ConstructL();
	CleanupStack::Pop();
	return cursorTimer;
}


void CTstControl::CMoveCursorTimer::ConstructL()
{
	CTimer::ConstructL();
}

void CTstControl::CMoveCursorTimer::RunL()
{

	TUint moveCursor = iControl.iSelectedCompositionText.iCursorPos+1;
	iControl.iCountKeyPressed = 0;
	iControl.SetPositionOfInsertionPointInBuffer(moveCursor);
	iControl.DrawNow();

}


void CTstControl::CMoveCursorTimer::StartTimer()
{
	Cancel();
	CTimer::After(600000);
}


// End of the CMoveCursorTimer 


void CTstControl::InsertCompositionCharacterAndDrawNowL(TUint aCharacterCode)
	{
	if (iBuffer.Length() < EMaximumLengthOfBuffer-1)
		{
		TTstTextBackup textBackup=iBuffer;
		textBackup.PushOntoCleanupStackL();
		TBuf<1> buffer=KNullDesC();
		buffer.Append(aCharacterCode);
		iBuffer.Insert(iSelectedCompositionText.iCursorPos, buffer);
		TInt newPositionOfInsertionPointInBuffer=iSelectedCompositionText.iCursorPos+1;
		__ASSERT_DEBUG((iInputMethod==EInputMethodPlain) || (iInputMethod==EInputMethodHexadecimalCharacterCode) || (iInputMethod==EInputMethodNumberKeyPad), Panic(EPanicBadInputMethod4));
		if (iInputMethod==EInputMethodHexadecimalCharacterCode)
			{
			if (newPositionOfInsertionPointInBuffer>=ENumberOfHexadecimalDigitsPerCharacterCode)
				{
				TUint characterCode=0;
				for (TInt i=ENumberOfHexadecimalDigitsPerCharacterCode; ; --i)
					{
					if (i<=0)
						{
						const TInt positionToInsertCharacterCode=newPositionOfInsertionPointInBuffer-ENumberOfHexadecimalDigitsPerCharacterCode;
						iBuffer.Delete(positionToInsertCharacterCode, ENumberOfHexadecimalDigitsPerCharacterCode-1);
						iBuffer[positionToInsertCharacterCode]=(TText)characterCode;
						newPositionOfInsertionPointInBuffer-=ENumberOfHexadecimalDigitsPerCharacterCode-1;
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
		else if (iInputMethod==EInputMethodNumberKeyPad)
			{	
			const TInt positionToDelete = newPositionOfInsertionPointInBuffer-1;
			iBuffer.Delete(positionToDelete,1);
			newPositionOfInsertionPointInBuffer = positionToDelete;

			if (char(aCharacterCode)=='1')
				{
				// Does nothing as 1 has Nothing associated with it... 
				}
			else
				{
				 if((iCountKeyPressed==0)||(iLastKeyPressed==aCharacterCode))
					{
					 
					const TBool first=(iCountKeyPressed==0); 
					iCountKeyPressed++;
					iLastKeyPressed=aCharacterCode;
					TInt code = iLastKeyPressed - '0';
					TInt limit = ((code == 7)||(code == 9)) ? 4 : 3;
					if (iCountKeyPressed > limit)
						{
						 iCountKeyPressed = short(iCountKeyPressed % limit);
						 if (iCountKeyPressed == 0)
							 iCountKeyPressed = limit;
						}
					TUint character = Alpha [code] [iCountKeyPressed-1];
					if (!first)
						{
						iBuffer.Delete(newPositionOfInsertionPointInBuffer,1);
						}
					buffer.Delete(0,1);
					buffer.Append(character);
					iBuffer.Insert(newPositionOfInsertionPointInBuffer,buffer);
					}
				 else
					{
					 if (aCharacterCode == '0') 
						{
						 iCountKeyPressed = 0;
						 newPositionOfInsertionPointInBuffer++;
						}
					 else
						{
						iLastKeyPressed = aCharacterCode;
						iCountKeyPressed = 1;

						TInt code = iLastKeyPressed - '0';
						TUint character = Alpha [code] [iCountKeyPressed -1];
						buffer.Delete(0,1);
						buffer.Append(character);
						newPositionOfInsertionPointInBuffer++;
						iBuffer.Insert(newPositionOfInsertionPointInBuffer,buffer);
						}
					}
				 iMoveCursorTimer->StartTimer();
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
			SetInlineTextL(*fepAwareTextEditor);
			}
		
		textBackup.PopOffCleanupStack();
		}
	}


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
	iSelectedCompositionText.iCursorPos=aPositionOfInsertionPointInBuffer;
	iSelectedCompositionText.iAnchorPos=aPositionOfInsertionPointInBuffer;
	iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
	}

TPoint CTstControl::PositionOfInsertionPointOnWindow() const
	{
	return TPoint(1+EGapLeftOfEachLine+iCompositionFont->TextWidthInPixels(iBuffer.Left(iSelectedCompositionText.iCursorPos)), 1+EGapAboveCompositionLine+iCompositionFont->HeightInPixels());
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
	return STATIC_CAST(const CTstControl*, aControl)->iBuffer.Length();
	}

const TAny* CTstControl::CharacterInBuffer(const CBase* aControl, TInt aIndex)
	{
	CTstControl* control=STATIC_CAST(CTstControl*, CONST_CAST(CBase*, aControl));
	control->iCharacterCode=control->iBuffer[aIndex];
	return &control->iCharacterCode;
	}

TKeyResponse CTstControl::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode)
	{
	// CTstFep::OfferKeyEventL assumes that this will not leave if it returns EKeyWasNotConsumed
	FEP_START_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, aEventCode);
	const TBool isFocused=IsFocused();
	const TCharUC keyCodeInUpperCase=aKeyEvent.iCode;
	TInt bufferLength=iBuffer.Length();
	if ((aKeyEvent.iModifiers&EModifierRightShift) && !isFocused && (bufferLength==0))
		{
		switch (keyCodeInUpperCase)
			{
		case 'F':
			ChangeSetupAndResetBufferAndDrawNow(SetFocus, ETrue);
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
		case 'I':
			ChangeSetupAndResetBufferAndDrawNow(SetFlagInlineEditingEnabled, !(iFlags&EFlagInlineEditingEnabled));
			iFep.WriteAttributeDataAndBroadcastL(TUid::Uid(ETstInlineEditingEnabledUid));
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
		case 'M':
			ChangeSetupAndResetBufferAndDrawNow(SetInputMethod, (iInputMethod+1<EInputMethodOnePastTheLast)? iInputMethod+1: 0);
			iFep.WriteAttributeDataAndBroadcastL(TUid::Uid(ETstInputMethodUid));
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
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
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
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
					const TInt documentPositionAfterSelection=cursorSelection.HigherPos()+1;
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
					fepAwareTextEditor->GetEditorContentForFep(textToDisplay, cursorSelection.LowerPos(), EMaximumLengthOfDisplayOfContextInformation+1);
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
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
			}
		}
	switch (keyCodeInUpperCase)
		{
	case EKeyEnter:
	case EKeyEscape:
		if (isFocused)
			{
			ChangeSetupAndResetBufferAndDrawNow(SetFocus, EFalse);
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
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
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
		break;
	case EKeyDelete:
	case EKeyBackspace:
		if (!isFocused && (bufferLength>0))
			{
			const TBool deletePreviousCharacter=((keyCodeInUpperCase==EKeyBackspace) && !(aKeyEvent.iModifiers&EModifierShift));
			if (deletePreviousCharacter? (iSelectedCompositionText.iCursorPos>0): (iSelectedCompositionText.iCursorPos<bufferLength))
				{
				TTstTextBackup textBackup=iBuffer;
				textBackup.PushOntoCleanupStackL();
				if (deletePreviousCharacter)
					{
					SetPositionOfInsertionPointInBuffer(iSelectedCompositionText.iCursorPos-1);
					}
				iBuffer.Delete(iSelectedCompositionText.iCursorPos, 1);
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
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
		break;
	case EKeySpace:
		if (!isFocused && (bufferLength>0))
			{
			__ASSERT_DEBUG((iInputMethod==EInputMethodPlain) || (iInputMethod==EInputMethodHexadecimalCharacterCode) || (iInputMethod==EInputMethodNumberKeyPad), Panic(EPanicBadInputMethod2));
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
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
		break;
	case EKeyHome:
	case EKeyEnd:
	case EKeyPageUp:
	case EKeyPageDown:
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
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
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
				SetPositionOfInsertionPointInBuffer(newPositionOfInsertionPointInBuffer);
				}
			if (iFlags&EFlagInsideInlineEditingTransaction)
				{
				SetInlineTextL(*iInputCapabilities.FepAwareTextEditor());
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
		break;
	default:
		if (isFocused)
			{
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
		TBool isLegalCompositionCharacter=EFalse;
		switch (iInputMethod)
			{
		case EInputMethodPlain:
			isLegalCompositionCharacter=keyCodeInUpperCase.IsAlpha();
			break;
		case EInputMethodHexadecimalCharacterCode:
			isLegalCompositionCharacter=keyCodeInUpperCase.IsHexDigit();
			break;
		case EInputMethodNumberKeyPad:
			isLegalCompositionCharacter=keyCodeInUpperCase.IsDigit();
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
			iCharacterCode=aKeyEvent.iCode;// set iCharacterCode to aKeyEvent.iCode rather than keyCodeInUpperCase so that the original case is preserved
			iFep.MakeDeferredFunctionCall(*this); // deferred call to InsertCompositionCharacterAndDrawNowL
			if (bufferLength==0)
				{
				iCoeEnv->ForEachFepObserverCall(FepObserverHandleStartOfTransactionL); // must be called before inserting the composition character as an application's HandleStartOfTransactionL function may move focus to a control supporting inline-editing
				}
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
		if (bufferLength>0)
			{
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed);
			}
		break;
		}
	FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasNotConsumed);
	}

void CTstControl::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
	switch (aPointerEvent.iType)
		{
	case TPointerEvent::EButton1Down:
	case TPointerEvent::EDrag:
		if (iFlags&EFlagWindowIsBeingDragged)
			{
			SetPosition(aPointerEvent.iParentPosition-iPositionOnWindowBeingDragged);
			}
		else
			{
			iFlags|=EFlagWindowIsBeingDragged;
			iPositionOnWindowBeingDragged=aPointerEvent.iPosition;
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
			graphicsContext.DrawText(iBuffer, temp, EGapAboveCompositionLine+iCompositionFont->AscentInPixels(), CGraphicsContext::ELeft, EGapLeftOfEachLine);
			graphicsContext.DiscardFont();
			}
		TBuf<200> textToDisplay;
		textToDisplay=KLitInputMethodColonSpace;
		switch (iInputMethod)
			{
		case EInputMethodPlain:
			textToDisplay.Append(KLitPlain);
			break;
		case EInputMethodHexadecimalCharacterCode:
			textToDisplay.Append(KLitHexadecimalCharacterCode);
			break;
		case EInputMethodNumberKeyPad:
			textToDisplay.Append(KLitNumberKeyPad);
			break;
		default:
#if defined(_DEBUG)
			Panic(EPanicBadInputMethod5);
#endif
			break;
			}
		const TInt statusFontHeightInPixels=iStatusFont->HeightInPixels();
		const TInt statusFontAscentInPixels=iStatusFont->AscentInPixels();
		graphicsContext.SetBrushColor(KRgbGray);
		graphicsContext.UseFont(iStatusFont);
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=EGapAboveTopStatusLine+statusFontHeightInPixels+EGapBetweenEachStatusLine;
		graphicsContext.DrawText(textToDisplay, temp, EGapAboveTopStatusLine+statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		textToDisplay=KLitInlineEditingColonSpace;
		textToDisplay.Append((iFlags&EFlagInlineEditingEnabled)? KLitEnabled(): KLitDisabled());
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=statusFontHeightInPixels+EGapBetweenEachStatusLine;
		graphicsContext.DrawText(textToDisplay, temp, statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
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

/*	if (aFormat.iFontPresentation.iHighlightStyle == TFontPresentation::EFontHighlightFepInlineText)
		{
		//draw the dotted underline
		TInt width = aParam.iDrawRect.Width();
		TPoint second (aTextOrigin.iX + width, aTextOrigin.iY+1);
		aParam.iGc.SetPenStyle(aParam.iGc.EDottedPen);
		aParam.iGc.SetUnderlineStyle(EUnderlineOff);
		aParam.iGc.DrawLine(TPoint(aTextOrigin.iX,aTextOrigin.iY+1),second);
		}
*/	MFormCustomDraw::DrawText(aParam,aLineInfo,aFormat,aText,aTextOrigin,aExtraPixels);
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
	aFormat.iFontPresentation.iTextColor.SetBlue(0);
	//aFormat.iFontPresentation.iHighlightStyle=TFontPresentation::EFontHighlightFepInlineText;
	if ((lengthOfSelection>0) && (aPositionOfCharacter>=startOfSelectedInlineText) && (aPositionOfCharacter<endOfSelectedInlineText))
		{ 
		aFormat.iFontPresentation.iTextColor.SetRed(0);
		aFormat.iFontPresentation.iTextColor.SetGreen(255);
		}
	else
		{
		aFormat.iFontPresentation.iTextColor.SetRed(255);
		aFormat.iFontPresentation.iTextColor.SetGreen(0);
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
	aEventResponse=CCoeFep::EEventWasConsumed; // this assumes that CTstControl::OfferKeyEventL will not leave if it returns EKeyWasNotConsumed
	
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

