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
#include <e32math.h>
#include <e32keys.h>
#include <s32strm.h>
#include <gdi.h>
#include <fbs.h>
#include <w32std.h>
#include <coemain.h>
#include <coeaui.h>
#include <coecntrl.h>
#include <coefepff.h>
#include <fepbase.h>
#include <bautils.h>
#include <techview/eikon.hrh>
#include <techview/eikdialg.h>
#include <techview/eikchkbx.h>
#include <techview/eikbutb.h>

#include "tfep2.hrh"
#include "TFEP2.H"
#include "tfep2com.h"

#define DEBUGGING_MESSAGES
#if defined(DEBUGGING_MESSAGES)
#include <e32svr.h>
#endif

// constants

enum TPanic
	{
	EPanicTimerActive1=1,
	EPanicTimerActive2,
	EPanicUnexpectedError1,
	EPanicUnexpectedError2,
	EPanicUnexpectedError3,
	EPanicUnexpectedError4,
	EPanicUnexpectedError5,
	EPanicUnexpectedError6,
	EPanicBadIndex1,
	EPanicBadIndex2,
	EPanicBadAttributeUid1,
	EPanicBadAttributeUid2,
	EPanicBadAttributeUid3,
	EPanicBadAttributeUid4,
	EPanicLowLevelFlagsDoNotReflectStateOfRemoteObjects,
	EPanicArithmeticConfusion,
	EPanicBadLengthOfTextBeforeSelection,
	EPanicSelectionExtendsPastEndOfDocument,
	EPanicBadLengthOfTextAfterSelection,
	EPanicBadLengthOfSelection,
	EPanicBadKeyCode1,
	EPanicBadKeyCode2,
	EPanicBadHeight,
	EPanicIsAlreadyActive,
	EPanicInconsistentUpperCaseSetting,
	EPanicBadFlagAffectingWhetherActive,
	EPanicBadKeyResponse,
	EPanicBadCheckBoxState,
	EPanicUnexpectedButtonId,
	EPanicBadNumberOfAttributes
	};

#if defined(_UNICODE)
const TUint KEllipsisCharacter=0x2026;
#else
const TUint KEllipsisCharacter=0x85;
#endif
_LIT(KLitTFEP2, "TFEP2");
_LIT(KLitBitmapHandleSynchronizationMutexName, "0x10003eaa-Mutex"); // using the same UID here as in the MMP file
_LIT(KLitDllNameOfWindowServerPlugIn, "\\system\\fep\\TFEP2BE.ANI");
_LIT(KLitStatusFontTypefaceName, "arial");
_LIT(KLitNotAvailable, "[not available]");
//_LIT(KLitTooLong, "[too long]");
_LIT(KLitQuotationMark, "\"");
_LIT(KLitTextBeforeSelectionColonSpace, "Text before selection: ");
_LIT(KLitTextAfterSelectionColonSpace, "Text after selection: ");
_LIT(KLitSelectionColonSpace, "Selection: ");
_LIT(KLitUpperCaseColonSpace, "Upper case: ");
_LIT(KLitOn, "on");
_LIT(KLitOff, "off");
_LIT(KLitScribbleAreaColonSpace, "Scribble area: ");
_LIT(KLitWholeScreen, "whole screen");
_LIT(KLitWindow, "window");
_LIT(KLitPointerBufferColonSpace, "Pointer buffer: ");
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
	User::Panic(KLitTFEP2, aPanic);
	}

GLDEF_C TInt E32Dll(
					)
	{
	return KErrNone;
	}

// CTstScribbleWindow

CTstScribbleWindow* CTstScribbleWindow::NewL(CTstFep& aFep, RWindowTreeNode& aParent)
	{
	CTstScribbleWindow* const scribbleWindow=new(ELeave) CTstScribbleWindow(aFep);
	CleanupStack::PushL(scribbleWindow);
	scribbleWindow->ConstructL(aParent);
	CleanupStack::Pop(); // scribbleWindow
	return scribbleWindow;
	}

CTstScribbleWindow::~CTstScribbleWindow()
	{
	// there is no need here to call FreePointerMoveBuffer on the window, as the buffer gets freed when the window gets destroyed
	iArrayOfPolyLines.ResetAndDestroy();
	iArrayOfPolyLines.Close();
	delete iTimeOutTimer;
	}

void CTstScribbleWindow::SetPointerBufferEnabled(TBool aPointerBufferEnabled)
	{
	RDrawableWindow& window=*DrawableWindow();
	if (!aPointerBufferEnabled!=!(iFlags&EFlagPointerBufferActuallyEnabled)) // fold non-zero values on both sides before comparing for inequality
		{
		if (aPointerBufferEnabled)
			{
			window.EnablePointerMoveBuffer();
			iFlags|=EFlagPointerBufferActuallyEnabled;
			}
		else
			{
			window.DisablePointerMoveBuffer();
			iFlags&=~EFlagPointerBufferActuallyEnabled;
			}
		}
	}

void CTstScribbleWindow::HandleTimeOutL()
	{
	TInt i;
	TInt minimumX=KMaxTInt;
	TInt maximumX=KMinTInt;
	for (i=iArrayOfPolyLines.Count()-1; i>=0; --i)
		{
		const CArrayFix<TPoint>& polyLine=*iArrayOfPolyLines[i];
		for (TInt j=polyLine.Count()-1; j>=0; --j)
			{
			const TInt x=polyLine[j].iX;
			if (minimumX>x)
				{
				minimumX=x;
				}
			if (maximumX<x)
				{
				maximumX=x;
				}
			}
		}
	const TInt numberOfCharacters=((maximumX-minimumX)/80)+1;
	CArrayFix<TUint>* arrayOfCharacters=new(ELeave) CArrayFixFlat<TUint>(numberOfCharacters); // a RArray would be better than a CArrayFix, but unfortunately RArray doesn't (yet) have a TArray interface
	CleanupStack::PushL(arrayOfCharacters);
	const TUint baseCharacter=iFep.UpperCase()? 'A': 'a';
	TTime homeTime;
	homeTime.HomeTime();
	TInt64 seedForRandomNumber=homeTime.Int64();
	for (i=0; i<numberOfCharacters; ++i)
		{
		arrayOfCharacters->AppendL(baseCharacter+(Math::Rand(seedForRandomNumber)%26));
		}
	iFep.SimulateKeyEventsL(arrayOfCharacters->Array());
	CancelTransactionAndDrawNow();
	CleanupStack::PopAndDestroy(); // arrayOfCharacters
	}

void CTstScribbleWindow::CancelTransactionAndDrawNow()
	{
	TBool changeWasMade;
	CancelTransaction(changeWasMade);
	if (changeWasMade)
		{
		DrawNow();
		}
	}

void CTstScribbleWindow::CancelTransaction(TBool& aChangeWasMade)
	{
	aChangeWasMade=EFalse;
	iTimeOutTimer->Cancel();
	if (iArrayOfPolyLines.Count()>0)
		{
		iArrayOfPolyLines.ResetAndDestroy();
		aChangeWasMade=ETrue;
		}
	}

void CTstScribbleWindow::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
	switch (aPointerEvent.iType)
		{
	case TPointerEvent::EDrag:
		{
		if (iFlags&EFlagPointerIsDown) // this test is needed in the case where there was a "leave" when handling the TRawEvent::EButton1Down event
			{
			__ASSERT_DEBUG(!iTimeOutTimer->IsActive(), Panic(EPanicTimerActive1));
			CArrayFix<TPoint>& polyLine=*iArrayOfPolyLines[iArrayOfPolyLines.Count()-1];
			polyLine.AppendL(aPointerEvent.iPosition);
			ActivateGc();
			CWindowGc& graphicsContext=SystemGc();
			graphicsContext.SetPenStyle(CGraphicsContext::ESolidPen);
			graphicsContext.SetPenColor(KRgbBlack);
			graphicsContext.DrawLine(polyLine[polyLine.Count()-2], aPointerEvent.iPosition);
			graphicsContext.Plot(aPointerEvent.iPosition);
			DeactivateGc();
			}
		}
		break;
	case TPointerEvent::EButton1Down:
		{
		iTimeOutTimer->Cancel();
		CArrayFix<TPoint>* const polyLine=new(ELeave) CArrayFixSeg<TPoint>(50);
		CleanupStack::PushL(polyLine);
		polyLine->AppendL(aPointerEvent.iPosition);
		User::LeaveIfError(iArrayOfPolyLines.Append(polyLine));
		CleanupStack::Pop(); // polyLine
		ActivateGc();
		CWindowGc& graphicsContext=SystemGc();
		graphicsContext.SetPenStyle(CGraphicsContext::ESolidPen);
		graphicsContext.SetPenColor(KRgbBlack);
		graphicsContext.Plot(aPointerEvent.iPosition);
		DeactivateGc();
		iFlags|=EFlagPointerIsDown;
		if (iArrayOfPolyLines.Count()==1)
			{
			iCoeEnv->ForEachFepObserverCall(FepObserverHandleStartOfTransactionL); // called at the end of handling this event as it may launch a waiting dialog
			}
		}
		break;
	case TPointerEvent::EButton1Up:
		if (iFlags&EFlagPointerIsDown) // this test is needed in the cases where (i) there was a "leave" when handling the TRawEvent::EButton1Down event, and (ii) the window server sends a TPointerEvent::EButton1Up event if ClaimPointerGrab is called when the pointer is not down (this behaviour is the window-server's spec, it is not a bug)
			{
			__ASSERT_DEBUG(!iTimeOutTimer->IsActive(), Panic(EPanicTimerActive2));
			iTimeOutTimer->After(ETimeOutInMicroSeconds);
			iFlags&=~EFlagPointerIsDown;
			}
		break;
#if defined(__GCC32__)
	default:
		break;
#endif
		}
	}

void CTstScribbleWindow::HandlePointerBufferReadyL()
	{
	TPoint arrayOfPoints[ENumberOfPointsInBuffer];
	TPtr8 bufferOfPoints(REINTERPRET_CAST(TUint8*, arrayOfPoints), 0, ENumberOfPointsInBuffer*sizeof(TPoint));
	User::LeaveIfError(DrawableWindow()->RetrievePointerMoveBuffer(bufferOfPoints));
	if (iFlags&EFlagPointerIsDown) // this test is needed in the case where there was a "leave" when handling the TRawEvent::EButton1Down event
		{
		const TInt numberOfPointsInBuffer=bufferOfPoints.Length()/sizeof(TPoint);
		if (numberOfPointsInBuffer>0)
			{
			CArrayFix<TPoint>& polyLine=*iArrayOfPolyLines[iArrayOfPolyLines.Count()-1];
			for (TInt i=0; i<numberOfPointsInBuffer; ++i)
				{
				polyLine.AppendL(arrayOfPoints[i]);
				}
			ActivateGc();
			CWindowGc& graphicsContext=SystemGc();
			graphicsContext.SetPenStyle(CGraphicsContext::ESolidPen);
			graphicsContext.SetPenColor(KRgbBlack);
			graphicsContext.DrawLine(polyLine[polyLine.Count()-(numberOfPointsInBuffer+1)], arrayOfPoints[0]);
			graphicsContext.DrawPolyLine(arrayOfPoints, numberOfPointsInBuffer); // draws the end-point of the last line segment of the poly-line
			DeactivateGc();
			}
		}
	}

CTstScribbleWindow::CTstScribbleWindow(CTstFep& aFep)
	:iFep(aFep),
	 iFlags(0),
	 iArrayOfPolyLines(12),
	 iTimeOutTimer(NULL)
	{
	}

void CTstScribbleWindow::ConstructL(RWindowTreeNode& aParent)
	{
	CreateWindowL(aParent);
	EnableDragEvents();
	ClaimPointerGrab();
	SetNonFocusing();
	RDrawableWindow& window=*DrawableWindow();
	User::LeaveIfError(window.AllocPointerMoveBuffer(ENumberOfPointsInBuffer, 0));
	iFlags|=EFlagPointerBufferActuallyEnabled;
	iTimeOutTimer=CTimeOutTimer::NewL(*this);
	}

void CTstScribbleWindow::Draw(const TRect&) const
	{
	CWindowGc& graphicsContext=SystemGc();
	graphicsContext.SetBrushStyle(CGraphicsContext::ESolidBrush);
	graphicsContext.SetBrushColor(KRgbWhite);
	graphicsContext.SetPenStyle(CGraphicsContext::ESolidPen);
	graphicsContext.SetPenColor(KRgbWhite);
	graphicsContext.DrawRect(Rect());
	graphicsContext.SetPenColor(KRgbBlack);
	for (TInt i=iArrayOfPolyLines.Count()-1; i>=0; --i)
		{
		graphicsContext.DrawPolyLine(iArrayOfPolyLines[i]); // draws the end-point of the last line segment of the poly-line
		}
	}

// CTstScribbleWindow::CTimeOutTimer

CTstScribbleWindow::CTimeOutTimer* CTstScribbleWindow::CTimeOutTimer::NewL(CTstScribbleWindow& aScribbleWindow)
	{
	CTimeOutTimer* const timeOutTimer=new(ELeave) CTimeOutTimer(aScribbleWindow);
	CleanupStack::PushL(timeOutTimer);
	CActiveScheduler::Add(timeOutTimer);
	timeOutTimer->ConstructL();
	CleanupStack::Pop(); // timeOutTimer
	return timeOutTimer;
	}

CTstScribbleWindow::CTimeOutTimer::~CTimeOutTimer()
	{
	Cancel();
	}

CTstScribbleWindow::CTimeOutTimer::CTimeOutTimer(CTstScribbleWindow& aScribbleWindow)
	:CTimer(EPriorityLow),
	 iScribbleWindow(aScribbleWindow)
	{
	}

void CTstScribbleWindow::CTimeOutTimer::RunL()
	{
	iScribbleWindow.HandleTimeOutL();
	}

// CTstWholeScreenScribbleArea

CTstWholeScreenScribbleArea* CTstWholeScreenScribbleArea::NewL(RWsSession& aWindowServerSession, RWindowGroup& aWindowGroup, const TSize& aScreenSize, TDisplayMode aDisplayMode, const TDesC& aDllName)
	{
	CTstWholeScreenScribbleArea* const wholeScreenScribbleArea=new(ELeave) CTstWholeScreenScribbleArea(aWindowServerSession);
	CleanupStack::PushL(wholeScreenScribbleArea);
	wholeScreenScribbleArea->ConstructL(aWindowGroup, aScreenSize, aDisplayMode, aDllName);
	CleanupStack::Pop(); // wholeScreenScribbleArea
	return wholeScreenScribbleArea;
	}

CTstWholeScreenScribbleArea::~CTstWholeScreenScribbleArea()
	{
	if (iFlags&EFlagMutexIsConstructed)
		{
		iMutex.Wait();
		iHandWritingRecognizer.Close();
		iSprite.Close();
		iDll.Close();
		delete iSpriteMember.iBitmap;
		delete iSpriteMember.iMaskBitmap;
		iMutex.Signal();
		}
	}

CTstWholeScreenScribbleArea::CTstWholeScreenScribbleArea(RWsSession& aWindowServerSession)
	:iFlags(0),
	 iDll(aWindowServerSession),
	 iSprite(aWindowServerSession)
	{
	iSpriteMember.iBitmap=NULL;
	iSpriteMember.iMaskBitmap=NULL;
	}

void CTstWholeScreenScribbleArea::ConstructL(RWindowGroup& aWindowGroup, const TSize& aScreenSize, TDisplayMode aDisplayMode, const TDesC& aDllName)
	{
	// the mutex *must* be created first as it is used in the construction and destruction routines of this class
	TInt error=iMutex.CreateGlobal(KLitBitmapHandleSynchronizationMutexName);
	if (error==KErrAlreadyExists)
		{
		error=iMutex.OpenGlobal(KLitBitmapHandleSynchronizationMutexName);
		}
	User::LeaveIfError(error);
	iFlags|=EFlagMutexIsConstructed;
	iMutex.Wait();
	CleanupStack::PushL(TCleanupItem(SignalMutex, &iMutex));
	User::LeaveIfError(iDll.Load(aDllName));
	User::LeaveIfError(iSprite.Construct(aWindowGroup, TPoint(0, 0), ESpriteNoChildClip|ESpriteNoShadows));
	STstBitmapHandles bitmapHandles;
	iHandWritingRecognizer=RHandWritingRecognizer(iDll);
	iHandWritingRecognizer.ConstructL(iSprite, bitmapHandles);
	iSpriteMember.iBitmap=CreateBitmapL(bitmapHandles.iMain, aScreenSize, aDisplayMode);
	iSpriteMember.iMaskBitmap=CreateBitmapL(bitmapHandles.iMask, aScreenSize, aDisplayMode);
	iSpriteMember.iInvertMask=ETrue;
	iSpriteMember.iDrawMode=CGraphicsContext::EDrawModePEN;
	iSpriteMember.iOffset.iX=0;
	iSpriteMember.iOffset.iY=0;
	iSpriteMember.iInterval=0;
	User::LeaveIfError(iSprite.AppendMember(iSpriteMember));
	iHandWritingRecognizer.FinishConstructionL();
	CleanupStack::PopAndDestroy(); // TCleanupItem(SignalMutex, &iMutex)
	}

CFbsBitmap* CTstWholeScreenScribbleArea::CreateBitmapL(TInt aHandleOfBitmapToUse, const TSize& aScreenSize, TDisplayMode aDisplayMode)
	{
	CFbsBitmap* const bitmap=new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	if (aHandleOfBitmapToUse!=0)
		{
		User::LeaveIfError(bitmap->Duplicate(aHandleOfBitmapToUse));
		}
	else
		{
		User::LeaveIfError(bitmap->Create(aScreenSize, aDisplayMode)); // bitmaps are automatically cleared to white when first created
		}
	CleanupStack::Pop(); // bitmap
	return bitmap;
	}

void CTstWholeScreenScribbleArea::SignalMutex(TAny* aMutex)
	{
	STATIC_CAST(RMutex*, aMutex)->Signal();
	}

// CTstWholeScreenScribbleArea::RHandWritingRecognizer

void CTstWholeScreenScribbleArea::RHandWritingRecognizer::ConstructL(const RWsSprite& aSprite, STstBitmapHandles& aBitmapHandles)
	{
	TPckg<STstBitmapHandles> bitmapHandles(aBitmapHandles);
	TIpcArgs ipcArgs;
	ipcArgs.Set(EIpcSlot, &bitmapHandles);
	User::LeaveIfError(RAnim::Construct(aSprite, EAnimTypeHandWritingRecognizer, KNullDesC8, ipcArgs));
	}

void CTstWholeScreenScribbleArea::RHandWritingRecognizer::FinishConstructionL()
	{
	const TInt error=CommandReply(EHandWritingRecognizerCommandFinishConstructionL, KNullDesC8());
	__ASSERT_ALWAYS(error==KErrNone, Panic(EPanicUnexpectedError1));
	}

void CTstWholeScreenScribbleArea::RHandWritingRecognizer::RequestNotificationOfStartOfTransaction(TRequestStatus& aRequestStatus)
	{
	AsyncCommandReply(aRequestStatus, EHandWritingRecognizerCommandRequestNotificationOfStartOfTransaction, TIpcArgs());
	}

void CTstWholeScreenScribbleArea::RHandWritingRecognizer::CancelRequestForNotificationOfStartOfTransaction()
	{
	const TInt error=CommandReply(EHandWritingRecognizerCommandCancelRequestForNotificationOfStartOfTransaction);
	__ASSERT_ALWAYS(error==KErrNone, Panic(EPanicUnexpectedError3));
	}

void CTstWholeScreenScribbleArea::RHandWritingRecognizer::RequestCharacters(TRequestStatus& aRequestStatus, TDes8& aCharacterBuffer, TBool /*aUpperCase*/)
	{
	TIpcArgs ipcArgs;
	ipcArgs.Set(EAsyncIpcSlot, &aCharacterBuffer);
	AsyncCommandReply(aRequestStatus, EHandWritingRecognizerCommandRequestCharacters, ipcArgs);
	}

void CTstWholeScreenScribbleArea::RHandWritingRecognizer::CancelRequestForCharacters()
	{
	const TInt error=CommandReply(EHandWritingRecognizerCommandCancelRequestForCharacters);
	__ASSERT_ALWAYS(error==KErrNone, Panic(EPanicUnexpectedError5));
	}

void CTstWholeScreenScribbleArea::RHandWritingRecognizer::SetUpperCase(TBool aUpperCase)
	{
	TPckgBuf<STstParametersForHandWritingRecognizerCommandSetUpperCase> parameters;
	parameters().iUpperCase=aUpperCase;
	const TInt error=CommandReply(EHandWritingRecognizerCommandSetUpperCase, parameters);
	__ASSERT_ALWAYS(error==KErrNone, Panic(EPanicUnexpectedError6));
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
	delete iScribbleWindow;
	delete iHandlerForStartOfTransaction;
	delete iHandlerForCharacters;
	delete iWholeScreenScribbleArea; // must be deleted after iHandlerForStartOfTransaction and iHandlerForCharacters as they both have a reference to it
	iCoeEnv->ReleaseScreenFont(iStatusFont);
	STATIC_CAST(CCoeAppUi*, iCoeEnv->AppUi())->RemoveFromStack(this);
	}

void CTstControl::CancelTransaction()
	{
	iScribbleWindow->CancelTransactionAndDrawNow();
	}

void CTstControl::IsOnHasChangedState()
	{
	ChangeSetupAndDrawNow(NULL);
	}

void CTstControl::OfferPointerEventL(CCoeFep::TEventResponse& aEventResponse, const TPointerEvent& aPointerEvent, const CCoeControl* aWindowOwningControl)
	{
	// this function must correctly set aEventResponse *before* calling anything that can leave
	if (aWindowOwningControl==this)
		{
		aEventResponse=CCoeFep::EEventWasConsumed;
		HandlePointerEventL(aPointerEvent);
		}
	else if (aWindowOwningControl==iScribbleWindow)
		{
		aEventResponse=CCoeFep::EEventWasConsumed;
		iScribbleWindow->HandlePointerEventL(aPointerEvent);
		}
	else
		{
		aEventResponse=CCoeFep::EEventWasNotConsumed;
		}
	}

void CTstControl::OfferPointerBufferReadyEventL(CCoeFep::TEventResponse& aEventResponse, const CCoeControl* aWindowOwningControl)
	{
	// this function must correctly set aEventResponse *before* calling anything that can leave
	if (aWindowOwningControl==iScribbleWindow)
		{
		aEventResponse=CCoeFep::EEventWasConsumed;
		iScribbleWindow->HandlePointerBufferReadyL();
		}
	else
		{
		aEventResponse=CCoeFep::EEventWasNotConsumed;
		}
	}

TInt CTstControl::NumberOfAttributes()
	{
	return 3;
	}

TUid CTstControl::AttributeAtIndex(TInt aIndex)
	{
	switch (aIndex)
		{
	case 0:
		return TUid::Uid(ETstUpperCaseUid);
	case 1:
		return TUid::Uid(ETstWholeScreenUid);
	case 2:
		return TUid::Uid(ETstPointerBufferEnabledUid);
#if defined(_DEBUG)
	default:
		Panic(EPanicBadIndex1);
		break;
#endif
		}
	return KNullUid;
	}

void CTstControl::WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const
	{
	switch (aAttributeUid.iUid)
		{
	case ETstUpperCaseUid:
		aStream.WriteUint8L((iFlags&EFlagUpperCase)!=0);
		break;
	case ETstWholeScreenUid:
		aStream.WriteUint8L((iFlags&EFlagWholeScreen)!=0);
		break;
	case ETstPointerBufferEnabledUid:
		aStream.WriteUint8L((iFlags&EFlagPointerBufferEnabled)!=0);
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
	case ETstUpperCaseUid:
		ChangeSetupAndDrawNow(SetUpperCase, aStream.ReadUint8L());
		break;
	case ETstWholeScreenUid:
		ChangeSetupAndDrawNow(SetWholeScreen, aStream.ReadUint8L());
		break;
	case ETstPointerBufferEnabledUid:
		ChangeSetupAndDrawNow(SetPointerBufferEnabled, aStream.ReadUint8L());
		break;
#if defined(_DEBUG)
	default:
		Panic(EPanicBadAttributeUid2);
		break;
#endif
		}
	}

void CTstControl::WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream, TBool aUpperCase, TBool aWholeScreen, TBool aPointerBufferEnabled)
	{
	switch (aAttributeUid.iUid)
		{
	case ETstUpperCaseUid:
		aStream.WriteUint8L(aUpperCase!=EFalse);
		break;
	case ETstWholeScreenUid:
		aStream.WriteUint8L(aWholeScreen!=EFalse);
		break;
	case ETstPointerBufferEnabledUid:
		aStream.WriteUint8L(aPointerBufferEnabled!=EFalse);
		break;
#if defined(_DEBUG)
	default:
		Panic(EPanicBadAttributeUid3);
		break;
#endif
		}
	}

void CTstControl::ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream, TBool& aUpperCase, TBool& aWholeScreen, TBool& aPointerBufferEnabled)
	{
	switch (aAttributeUid.iUid)
		{
	case ETstUpperCaseUid:
		aUpperCase=aStream.ReadUint8L();
		break;
	case ETstWholeScreenUid:
		aWholeScreen=aStream.ReadUint8L();
		break;
	case ETstPointerBufferEnabledUid:
		aPointerBufferEnabled=aStream.ReadUint8L();
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
	ChangeSetupAndDrawNow(SetForeground, ETrue);
	}

void CTstControl::HandleLosingForeground()
	{
	ChangeSetupAndDrawNow(SetForeground, EFalse);
	}

void CTstControl::HandleChangeInFocus()
	{
	ChangeSetupAndDrawNow(SetInputCapabilities);
	}

void CTstControl::HandleDestructionOfFocusedItem()
	{
	if (!IsBeingDestroyed())
		{
		ChangeSetupAndDrawNow(SetInputCapabilities);
		}
	}

CTstControl::CTstControl(CTstFep& aFep)
	:iFep(aFep),
	 iFlags(0),
	 iInputCapabilities(TCoeInputCapabilities::ENone),
	 iScribbleWindow(NULL),
	 iWholeScreenScribbleArea(NULL),
	 iHandlerForStartOfTransaction(NULL),
	 iHandlerForCharacters(NULL),
	 iStatusFont(NULL),
	 iPositionOnWindowBeingDragged(0, 0)
	{
	}

void CTstControl::ConstructL()
	{
	RWindowGroup& windowGroup=iCoeEnv->RootWin();
	CreateWindowL();
	EnableDragEvents();
	ClaimPointerGrab();
	SetNonFocusing();
	RDrawableWindow& window=*DrawableWindow();
	window.SetOrdinalPosition(0, ECoeWinPriorityFep);
	window.SetShadowHeight(3);
	iScribbleWindow=CTstScribbleWindow::NewL(iFep, window);
	RWsSession& windowServerSession=iCoeEnv->WsSession();
	CWsScreenDevice& screenDevice=*iCoeEnv->ScreenDevice();
	const TSize screenSize=screenDevice.SizeInPixels();
	iWholeScreenScribbleArea=CTstWholeScreenScribbleArea::NewL(windowServerSession, windowGroup, screenSize, screenDevice.DisplayMode(), KLitDllNameOfWindowServerPlugIn);
	iHandlerForStartOfTransaction=CHandlerForStartOfTransaction::NewL(*iWholeScreenScribbleArea, *iCoeEnv);
	const TBool foreground=(windowGroup.Identifier()==windowServerSession.GetFocusWindowGroup());
	iHandlerForCharacters=CHandlerForCharacters::NewL(*iWholeScreenScribbleArea, iFep, foreground);
	iStatusFont=iCoeEnv->CreateScreenFontL(TFontSpec(KLitStatusFontTypefaceName, 120));
	const TInt statusFontHeightInPixels=iStatusFont->HeightInPixels();
	const TSize size(320, 1+EHeightOfScribbleWindow+EGapAboveTopLine+statusFontHeightInPixels+(4*(EGapBetweenEachLine+statusFontHeightInPixels))+EGapBelowBottomLine+1);
	SetExtent(TPoint(screenSize.iWidth-(size.iWidth+10), screenSize.iHeight-(size.iHeight+10)), size);
	iScribbleWindow->SetExtent(TPoint(1, 1), TSize(size.iWidth-2, EHeightOfScribbleWindow));
	STATIC_CAST(CCoeAppUi*, iCoeEnv->AppUi())->AddToStackL(this, ECoeStackPriorityFep, ECoeStackFlagRefusesFocus|ECoeStackFlagSharable);
	__ASSERT_DEBUG((iFlags&EFlagActuallyWholeScreen)==0, Panic(EPanicLowLevelFlagsDoNotReflectStateOfRemoteObjects));
	ChangeSetupAndDrawNow(SetForeground, foreground);
	ChangeSetupAndDrawNow(SetFocus, EFalse);
	ChangeSetupAndDrawNow(SetUpperCase, EFalse);
	ChangeSetupAndDrawNow(SetWholeScreen, ETrue);
	ChangeSetupAndDrawNow(SetPointerBufferEnabled, ETrue);
	ChangeSetupAndDrawNow(SetInputCapabilities);
	}

void CTstControl::SetForeground(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	SetFlag(aControl, aChangeWasMade, aParameter, EFlagForeground);
	if (aChangeWasMade)
		{
		aControl.iHandlerForCharacters->SetForeground(aParameter); // background TFEP2 instances must not have a request for characters outstanding, as the corresponding server-side object may intercept raw events from the rightful TFEP2 instance in the foreground, hence the need for this function
		if (aParameter)
			{
			aControl.iHandlerForCharacters->SetUpperCase(aControl.iFlags&EFlagUpperCase);
			aControl.DoSetWholeScreen(aControl.iFlags&EFlagWholeScreen);
			aControl.iScribbleWindow->SetPointerBufferEnabled(aControl.iFlags&EFlagPointerBufferEnabled);
			}
		}
	}

void CTstControl::SetFocus(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	aChangeWasMade=EFalse;
	if (!aParameter!=!aControl.IsFocused()) // fold non-zero values on both sides before comparing for inequality
		{
		CCoeAppUi& appUi=*STATIC_CAST(CCoeAppUi*, aControl.iCoeEnv->AppUi());
		appUi.UpdateStackedControlFlags(&aControl, (aParameter? 0: ECoeStackFlagRefusesFocus), ECoeStackFlagRefusesFocus);
		appUi.HandleStackChanged();
		aControl.iScribbleWindow->MakeVisible(!aParameter);
		aChangeWasMade=ETrue;
		}
	}

void CTstControl::SetUpperCase(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	SetFlag(aControl, aChangeWasMade, aParameter, EFlagUpperCase);
	if (aChangeWasMade && (aControl.iFlags&EFlagForeground))
		{
		aControl.iHandlerForCharacters->SetUpperCase(aParameter);
		}
	}

void CTstControl::SetWholeScreen(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	SetFlag(aControl, aChangeWasMade, aParameter, EFlagWholeScreen);
	if (aChangeWasMade && (aControl.iFlags&EFlagForeground))
		{
		aControl.DoSetWholeScreen(aParameter);
		}
	}

void CTstControl::SetPointerBufferEnabled(CTstControl& aControl, TBool& aChangeWasMade, TInt aParameter)
	{
	SetFlag(aControl, aChangeWasMade, aParameter, EFlagPointerBufferEnabled);
	if (aChangeWasMade && (aControl.iFlags&EFlagForeground))
		{
		aControl.iScribbleWindow->SetPointerBufferEnabled(aParameter);
		}
	}

void CTstControl::SetInputCapabilities(CTstControl& aControl, TBool& aChangeWasMade, TInt)
	{
	aChangeWasMade=EFalse;
	if (!aControl.IsFocused())
		{
		const TCoeInputCapabilities inputCapabilities(STATIC_CAST(const CCoeAppUi*, aControl.iCoeEnv->AppUi())->InputCapabilities());
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

void CTstControl::DoSetWholeScreen(TBool aWholeScreen)
	{
	if (!aWholeScreen!=!(iFlags&EFlagActuallyWholeScreen)) // fold non-zero values on both sides before comparing for inequality
		{
		TInt windowAdjustmentY;
		TInt scribbleWindowPositionY;
		if (aWholeScreen)
			{
			windowAdjustmentY=EHeightOfScribbleWindow;
			scribbleWindowPositionY=-EHeightOfScribbleWindow;
			iFlags|=EFlagActuallyWholeScreen;
			}
		else 
			{
			windowAdjustmentY=-EHeightOfScribbleWindow;
			scribbleWindowPositionY=1;
			iFlags&=~EFlagActuallyWholeScreen;
			}
		iHandlerForCharacters->SetWholeScreen(aWholeScreen);
		iScribbleWindow->SetPosition(TPoint(1, scribbleWindowPositionY));
		TRect rectangle(Position(), Size()); // creating rectangle from Rect() will give the wrong result as this control owns a window (its top left position would come out as TPoint(0, 0) rather than what Position() returns)
		rectangle.iTl.iY+=windowAdjustmentY;
		SetRect(rectangle);
		}
	}

void CTstControl::ChangeSetupAndDrawNow(FChangeFunction aChangeFunction, TInt aParameter)
	{
	TBool needToDraw=EFalse;
	{
	TBool changeWasMade;
	iScribbleWindow->CancelTransaction(changeWasMade);
	if (changeWasMade)
		{
		needToDraw=ETrue;
		}
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
	const TBool isOn=iFep.IsOn();
	iHandlerForCharacters->SetIsOn(isOn);
	const TBool shouldBeVisible=(isOn && (iFlags&EFlagForeground));
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

TKeyResponse CTstControl::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode)
	{
	// CTstFep::OfferKeyEventL assumes that this will not leave if it returns EKeyWasNotConsumed
	FEP_START_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, aEventCode);
	const TBool isFocused=IsFocused();
	const TCharUC keyCodeInUpperCase=aKeyEvent.iCode;
	if ((aKeyEvent.iModifiers&EModifierRightShift) && !isFocused)
		{
		switch (keyCodeInUpperCase)
			{
		case 'F':
			ChangeSetupAndDrawNow(SetFocus, ETrue);
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
		case 'C':
			ChangeSetupAndDrawNow(SetUpperCase, !(iFlags&EFlagUpperCase));
			iFep.WriteAttributeDataAndBroadcastL(TUid::Uid(ETstUpperCaseUid));
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
		case 'W':
			ChangeSetupAndDrawNow(SetWholeScreen, !(iFlags&EFlagWholeScreen));
			iFep.WriteAttributeDataAndBroadcastL(TUid::Uid(ETstWholeScreenUid));
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
		case 'P':
			ChangeSetupAndDrawNow(SetPointerBufferEnabled, !(iFlags&EFlagPointerBufferEnabled));
			iFep.WriteAttributeDataAndBroadcastL(TUid::Uid(ETstPointerBufferEnabledUid));
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
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
			}
			}
		}
	switch (keyCodeInUpperCase)
		{
	case EKeyEnter:
	case EKeyEscape:
		if (isFocused)
			{
			ChangeSetupAndDrawNow(SetFocus, EFalse);
			FEP_END_KEY_EVENT_HANDLER_L(iFep, aKeyEvent, EKeyWasConsumed); // returns from this function
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
				Panic(EPanicBadKeyCode2);
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
		break;
	default:
		if (isFocused)
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

TInt CTstControl::CountComponentControls() const
	{
	return 1;
	}

CCoeControl* CTstControl::ComponentControl(TInt aIndex) const
	{
	switch (aIndex)
		{
	case 0:
		return iScribbleWindow;
	default:
#if defined(_DEBUG)
		Panic(EPanicBadIndex2);
#endif
		return NULL;
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
		if (~iFlags&EFlagWholeScreen)
			{
			temp.iTl.iY+=EHeightOfScribbleWindow;
			}
		temp.iBr.iY=temp.iTl.iY;
		graphicsContext.SetBrushColor(KRgbGray);
		graphicsContext.UseFont(iStatusFont);
		TBuf<200> textToDisplay;
		const TInt statusFontHeightInPixels=iStatusFont->HeightInPixels();
		const TInt statusFontAscentInPixels=iStatusFont->AscentInPixels();
		textToDisplay=KLitUpperCaseColonSpace;
		textToDisplay.Append((iFlags&EFlagUpperCase)? KLitOn(): KLitOff());
		temp.iBr.iY+=EGapAboveTopLine+statusFontHeightInPixels+EGapBetweenEachLine;
		graphicsContext.DrawText(textToDisplay, temp, EGapAboveTopLine+statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		textToDisplay=KLitScribbleAreaColonSpace;
		textToDisplay.Append((iFlags&EFlagWholeScreen)? KLitWholeScreen(): KLitWindow());
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=statusFontHeightInPixels+EGapBetweenEachLine;
		graphicsContext.DrawText(textToDisplay, temp, statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		textToDisplay=KLitPointerBufferColonSpace;
		textToDisplay.Append((iFlags&EFlagPointerBufferEnabled)? KLitEnabled(): KLitDisabled());
		temp.iTl.iY=temp.iBr.iY;
		temp.iBr.iY+=statusFontHeightInPixels+EGapBetweenEachLine;
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
		temp.iBr.iY+=statusFontHeightInPixels+EGapBetweenEachLine;
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
		__ASSERT_DEBUG(temp.iBr.iY==temp.iTl.iY+statusFontHeightInPixels+EGapBelowBottomLine, Panic(EPanicBadHeight));
		graphicsContext.DrawText(textToDisplay, temp, statusFontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
		graphicsContext.DiscardFont();
		}
	}

// CTstControl::CHandlerForStartOfTransaction

CTstControl::CHandlerForStartOfTransaction* CTstControl::CHandlerForStartOfTransaction::NewL(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CCoeEnv& aConeEnvironment)
	{
	return new(ELeave) CHandlerForStartOfTransaction(aWholeScreenScribbleArea, aConeEnvironment);
	}

CTstControl::CHandlerForStartOfTransaction::~CHandlerForStartOfTransaction()
	{
	Cancel();
	}

CTstControl::CHandlerForStartOfTransaction::CHandlerForStartOfTransaction(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CCoeEnv& aConeEnvironment)
	:CActive(EActivePriorityWsEvents),
	 iWholeScreenScribbleArea(aWholeScreenScribbleArea),
	 iConeEnvironment(aConeEnvironment)
	{
	CActiveScheduler::Add(this);
	RequestNotificationOfStartOfTransaction();
	}

void CTstControl::CHandlerForStartOfTransaction::RequestNotificationOfStartOfTransaction()
	{
	iWholeScreenScribbleArea.RequestNotificationOfStartOfTransaction(iStatus);
	SetActive();
	}

void CTstControl::CHandlerForStartOfTransaction::DoCancel()
	{
	iWholeScreenScribbleArea.CancelRequestForNotificationOfStartOfTransaction();
	}

void CTstControl::CHandlerForStartOfTransaction::RunL()
	{
	RequestNotificationOfStartOfTransaction();
	iConeEnvironment.ForEachFepObserverCall(FepObserverHandleStartOfTransactionL); // called at the end of handling this event as it may launch a waiting dialog
	}

// CTstControl::CHandlerForCharacters

CTstControl::CHandlerForCharacters* CTstControl::CHandlerForCharacters::NewL(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CTstFep& aFep, TBool aForeground)
	{
	return new(ELeave) CHandlerForCharacters(aWholeScreenScribbleArea, aFep, aForeground);
	}

CTstControl::CHandlerForCharacters::~CHandlerForCharacters()
	{
	Cancel();
	}

void CTstControl::CHandlerForCharacters::SetUpperCase(TBool aUpperCase)
	{
	if (!aUpperCase!=!(iFlags&EFlagUpperCase)) // fold non-zero values on both sides before comparing for inequality
		{
		iFlags^=EFlagUpperCase;
		if (IsActive())
			{
			iWholeScreenScribbleArea.SetUpperCase(aUpperCase);
			}
		}
	__ASSERT_DEBUG(!(iFlags&EFlagUpperCase)==!aUpperCase, Panic(EPanicInconsistentUpperCaseSetting)); // fold non-zero values on both sides before comparing for equality
	}

CTstControl::CHandlerForCharacters::CHandlerForCharacters(CTstWholeScreenScribbleArea& aWholeScreenScribbleArea, CTstFep& aFep, TBool aForeground)
	:CActive(EActivePriorityWsEvents),
	 iWholeScreenScribbleArea(aWholeScreenScribbleArea),
	 iFep(aFep),
	 iFlags(aForeground? EFlagForeground: 0)
	{
	CActiveScheduler::Add(this);
	}

void CTstControl::CHandlerForCharacters::SetFlagAffectingWhetherActive(TUint aFlagAffectingWhetherActive, TBool aSetting)
	{
	const TUint flagsAffectingWhetherActive=(EFlagWholeScreen|EFlagIsOn|EFlagForeground);
	__ASSERT_DEBUG(aFlagAffectingWhetherActive&flagsAffectingWhetherActive, Panic(EPanicBadFlagAffectingWhetherActive));
	const TUint oldFlags=iFlags;
	if (aSetting)
		{
		iFlags|=aFlagAffectingWhetherActive;
		}
	else
		{
		iFlags&=~aFlagAffectingWhetherActive;
		}
	if ((iFlags^oldFlags)&flagsAffectingWhetherActive)
		{
		if ((iFlags&flagsAffectingWhetherActive)==flagsAffectingWhetherActive)
			{
			RequestCharacters();
			}
		else if ((oldFlags&flagsAffectingWhetherActive)==flagsAffectingWhetherActive)
			{
			Cancel();
			}
		}
	}

void CTstControl::CHandlerForCharacters::RequestCharacters()
	{
	iWholeScreenScribbleArea.RequestCharacters(iStatus, iCharacterBuffer, iFlags&EFlagUpperCase);
	SetActive();
	}

TInt CTstControl::CHandlerForCharacters::NumberOfCharactersInBuffer(const CBase* aCharacterBuffer)
	{
	const TDesC8& characterBuffer=*REINTERPRET_CAST(const TDesC8*, aCharacterBuffer);
	return characterBuffer.Length()/sizeof(TUint);
	}

const TAny* CTstControl::CHandlerForCharacters::CharacterInBuffer(const CBase* aCharacterBuffer, TInt aIndex)
	{
	const TDesC8& characterBuffer=*REINTERPRET_CAST(const TDesC8*, aCharacterBuffer);
	return characterBuffer.Ptr()+(aIndex*sizeof(TUint));
	}

void CTstControl::CHandlerForCharacters::DoCancel()
	{
	iWholeScreenScribbleArea.CancelRequestForCharacters();
	}

void CTstControl::CHandlerForCharacters::RunL()
	{
	TBuf8<EMaximumLengthOfCharacterBuffer> characterBuffer=iCharacterBuffer;
	RequestCharacters(); // calling RequestCharacters may complete immediately (thus trashing iCharacterBuffer), hence why iCharacterBuffer was copied to a temporary buffer first
	iFep.SimulateKeyEventsL(TArray<TUint>(NumberOfCharactersInBuffer, CharacterInBuffer, REINTERPRET_CAST(const CBase*, &characterBuffer)));
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

void CTstFep::OfferPointerBufferReadyEventL(TEventResponse& aEventResponse, const CCoeControl* aWindowOwningControl)
	{
	iControl->OfferPointerBufferReadyEventL(aEventResponse, aWindowOwningControl);
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
	:iFlags(0)
	{
	}

TUint CTstSettingsDialog::CheckBoxStateAsFlag(TInt aControlId, TUint aFlag) const
	{
	switch (STATIC_CAST(CEikCheckBox*, Control(aControlId))->State())
		{
	case CEikButtonBase::EClear:
		return 0;
	case CEikButtonBase::ESet:
		return aFlag;
	case CEikButtonBase::EIndeterminate:
	default:
#if defined(_DEBUG)
		Panic(EPanicBadCheckBoxState);
#endif
		return 0;
		}
	}

void CTstSettingsDialog::SetCheckBoxState(TInt aControlId, TUint aFlag)
	{
	STATIC_CAST(CEikCheckBox*, Control(aControlId))->SetState(iFlags&aFlag? CEikButtonBase::ESet: CEikButtonBase::EClear);
	}

TBool CTstSettingsDialog::OkToExitL(TInt aButtonId)
	{
	__ASSERT_ALWAYS(aButtonId==EEikBidOk, Panic(EPanicUnexpectedButtonId));
	iFlags=CheckBoxStateAsFlag(EControlIdUpperCase, EFlagUpperCase)|
		   CheckBoxStateAsFlag(EControlIdWholeScreen, EFlagWholeScreen)|
		   CheckBoxStateAsFlag(EControlIdPointerBufferEnabled, EFlagPointerBufferEnabled);
	TFixedArray<TUid, 3> attributeUids;
	__ASSERT_DEBUG(NumberOfAttributes()==3, Panic(EPanicBadNumberOfAttributes));
	attributeUids[0].iUid=AttributeAtIndex(0).iUid;
	attributeUids[1].iUid=AttributeAtIndex(1).iUid;
	attributeUids[2].iUid=AttributeAtIndex(2).iUid;
	WriteAttributeDataAndBroadcastL(*iCoeEnv, attributeUids.Array());
	return ETrue;
	}

void CTstSettingsDialog::PreLayoutDynInitL()
	{
	ReadAllAttributesL(*iCoeEnv);
	SetCheckBoxState(EControlIdUpperCase, EFlagUpperCase);
	SetCheckBoxState(EControlIdWholeScreen, EFlagWholeScreen);
	SetCheckBoxState(EControlIdPointerBufferEnabled, EFlagPointerBufferEnabled);
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
	CTstControl::WriteAttributeDataToStreamL(aAttributeUid, aStream, iFlags&EFlagUpperCase, iFlags&EFlagWholeScreen, iFlags&EFlagPointerBufferEnabled);
	}

void CTstSettingsDialog::ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream)
	{
	TBool upperCase;
	TBool wholeScreen;
	TBool pointerBufferEnabled;
	CTstControl::ReadAttributeDataFromStreamL(aAttributeUid, aStream, upperCase, wholeScreen, pointerBufferEnabled);
	iFlags=0;
	if (upperCase)
		{
		iFlags|=EFlagUpperCase;
		}
	if (wholeScreen)
		{
		iFlags|=EFlagWholeScreen;
		}
	if (pointerBufferEnabled)
		{
		iFlags|=EFlagPointerBufferEnabled;
		}
	}

