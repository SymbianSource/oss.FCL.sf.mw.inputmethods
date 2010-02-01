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
#include <e32svr.h>
#include <bitstd.h>
#include <bitdev.h>
#include <fbs.h>
#include <w32adll.h>

#include "tfep2be.h"
#include "tfep2com.h"

#define DEBUGGING_MESSAGES

// numeric constants

const TInt KPanicClientFromServer=1; // must be greater than zero (CTstHandWritingRecognizer::OfferRawEvent assumes this to be true)

// literal constant text

#if defined(_DEBUG)
_LIT(KLitTFEP2BE, "TFEP2BE");
#endif

// local and global functions

#if defined(_DEBUG)

enum TPanic
	{
	EPanicUnexpectedError=1,
	EPanicBadDataInDllTls,
	EPanicUnexpectedRawEvent,
	EPanicTimerActive1,
	EPanicTimerActive2,
	EPanicNoPendingCharactersToMove,
	EPanicUnexpectedNullPointer1,
	EPanicUnexpectedNullPointer2,
	EPanicBadGranularity,
	EPanicAlreadyConstructed,
	EPanicNonAlignedDescriptorLength1,
	EPanicNonAlignedDescriptorLength2,
	EPanicNonAlignedDescriptorMaximumLength,
	EPanicBadDescriptorLength,
	EPanicBadNumberOfCharacters1,
	EPanicBadNumberOfCharacters2
	};

LOCAL_C void Panic(TPanic aPanic)
	{
	User::Panic(KLitTFEP2BE, aPanic);
	}

#endif

LOCAL_C void PanicClientFromServer()
	{
	User::Leave(KPanicClientFromServer);
	}

LOCAL_C void HandleErrorIfErrorL(MAnimGeneralFunctions& aFunctions, TInt aError)
	{
	switch (aError)
		{
	case KErrNone:
		break;
	case KPanicClientFromServer:
		aFunctions.Panic();
		break;
	default:
		User::Leave(aError);
		break;
		}
	}

LOCAL_C void HandleErrorIfError(MAnimGeneralFunctions& aFunctions, TInt aError)
	{
	switch (aError)
		{
	case KErrNone:
		break;
	case KPanicClientFromServer:
		aFunctions.Panic();
		break;
	default:
#if defined(_DEBUG)
		Panic(EPanicUnexpectedError);
#endif
		break;
		}
	}

GLDEF_C TInt E32Dll(
					)
	{
	return KErrNone;
	}

// CTstHandWritingRecognizer

CTstHandWritingRecognizer::CTstHandWritingRecognizer()
	:iFlags(0),
	 iArrayOfPolyLines(12),
	 iArrayOfCharactersPending(10),
	 iTimeOutTimer(NULL),
	 iMessage_RequestForNotificationOfStartOfTransaction(),
	 iMessage_RequestForCharacters(),
	 iMaximumLengthOfClientSideCharacterBuffer(0),
	 iMainBitmap(NULL),
	 iMaskBitmap(NULL),
	 iGraphicsContext(NULL)
	{
	}

CTstHandWritingRecognizer::~CTstHandWritingRecognizer()
	{
	iFunctions->GetRawEvents(EFalse);
	iSpriteFunctions->Activate(EFalse);
	if (iFlags&EFlagReferenceCountIncremented)
		{
		SBitmapHandlesWithReferenceCount* const bitmapHandlesWithReferenceCount=STATIC_CAST(SBitmapHandlesWithReferenceCount*, Dll::Tls());
		__ASSERT_DEBUG((bitmapHandlesWithReferenceCount!=NULL) && (bitmapHandlesWithReferenceCount->iReferenceCount>0), Panic(EPanicBadDataInDllTls));
		--bitmapHandlesWithReferenceCount->iReferenceCount;
		if (bitmapHandlesWithReferenceCount->iReferenceCount==0)
			{
			delete bitmapHandlesWithReferenceCount;
			Dll::SetTls(NULL);
			}
		}
	iArrayOfPolyLines.ResetAndDestroy();
	iArrayOfPolyLines.Close();
	iArrayOfCharactersPending.Close();
	delete iTimeOutTimer;
	delete iGraphicsContext;
	delete iMainBitmap;
	delete iMaskBitmap;
	}

void CTstHandWritingRecognizer::HandleTimeOut()
	{
	TRAPD(error, DoRecognitionL());
	CompleteRequestForCharacters(error);
	}

void CTstHandWritingRecognizer::ConstructLP(TAny* /*aParameters*/)
	{
	// this functions does not actually construct anything, but simply writes the bitmap handles stored in Dll::Tls() (if any) to the client's descriptor
	STstBitmapHandles bitmapHandles(0, 0);
	const SBitmapHandlesWithReferenceCount* const bitmapHandlesWithReferenceCount=REINTERPRET_CAST(const SBitmapHandlesWithReferenceCount*, Dll::Tls());
	if (bitmapHandlesWithReferenceCount!=NULL)
		{
		bitmapHandles=bitmapHandlesWithReferenceCount->iBitmapHandles;
		}
	iFunctions->Message()->WriteL(EIpcSlot, TPckgC<STstBitmapHandles>(bitmapHandles));
	}

TInt CTstHandWritingRecognizer::CommandReplyLP(TInt aOpcode, TAny* aParameters)
	{
	switch (aOpcode)
		{
	case EHandWritingRecognizerCommandFinishConstructionL:
		{
		__ASSERT_ALWAYS(iFlags==0, PanicClientFromServer());
		iFunctions->SetSync(MAnimGeneralFunctions::ESyncNone);
		iSpriteFunctions->SizeChangedL();
		iArrayOfCharactersPending.ConstructL();
		iTimeOutTimer=CTimeOutTimer::NewL(*this);
		const TSpriteMember* const spriteMember=iSpriteFunctions->GetSpriteMember(0);
		__ASSERT_ALWAYS((spriteMember!=NULL) && (spriteMember->iBitmap!=NULL) && (spriteMember->iMaskBitmap!=NULL), PanicClientFromServer());
		const STstBitmapHandles bitmapHandles(spriteMember->iBitmap->Handle(), spriteMember->iMaskBitmap->Handle());
		SBitmapHandlesWithReferenceCount* bitmapHandlesWithReferenceCount=STATIC_CAST(SBitmapHandlesWithReferenceCount*, Dll::Tls());
		if (bitmapHandlesWithReferenceCount!=NULL)
			{
			__ASSERT_ALWAYS((bitmapHandlesWithReferenceCount->iBitmapHandles.iMain==bitmapHandles.iMain) && (bitmapHandlesWithReferenceCount->iBitmapHandles.iMask==bitmapHandles.iMask), PanicClientFromServer());
			++bitmapHandlesWithReferenceCount->iReferenceCount;
			}
		else
			{
			bitmapHandlesWithReferenceCount=new(ELeave) SBitmapHandlesWithReferenceCount;
			CleanupStack::PushL(bitmapHandlesWithReferenceCount);
			bitmapHandlesWithReferenceCount->iBitmapHandles=bitmapHandles;
			bitmapHandlesWithReferenceCount->iReferenceCount=1;
			User::LeaveIfError(Dll::SetTls(bitmapHandlesWithReferenceCount));
			CleanupStack::Pop(); // bitmapHandlesWithReferenceCount
			}
		iFlags|=EFlagReferenceCountIncremented;
#if defined(DEBUGGING_MESSAGES)
		RDebug::Print(_L("reference-count of the bitmap handles: %d"), bitmapHandlesWithReferenceCount->iReferenceCount);
#endif
		iMainBitmap=CFbsBitmapDevice::NewL(spriteMember->iBitmap);
		iMaskBitmap=CFbsBitmapDevice::NewL(spriteMember->iMaskBitmap);
		iGraphicsContext=CFbsBitGc::NewL();
		iGraphicsContext->SetBrushStyle(CGraphicsContext::ESolidBrush);
		iGraphicsContext->SetBrushColor(KRgbWhite);
		iGraphicsContext->SetPenStyle(CGraphicsContext::ESolidPen);
		iGraphicsContext->SetPenColor(KRgbBlack);
		}
		break;
	case EHandWritingRecognizerCommandRequestNotificationOfStartOfTransaction:
		__ASSERT_ALWAYS(iMessage_RequestForNotificationOfStartOfTransaction.IsNull(), PanicClientFromServer());
		iMessage_RequestForNotificationOfStartOfTransaction=*iFunctions->Message();
		__ASSERT_ALWAYS(!iMessage_RequestForNotificationOfStartOfTransaction.IsNull(), PanicClientFromServer());
		break;
	case EHandWritingRecognizerCommandCancelRequestForNotificationOfStartOfTransaction:
		if (!iMessage_RequestForNotificationOfStartOfTransaction.IsNull())
			{
			iMessage_RequestForNotificationOfStartOfTransaction.Complete(KErrCancel);
			}
		break;
	case EHandWritingRecognizerCommandRequestCharacters:
		__ASSERT_ALWAYS(iMessage_RequestForCharacters.IsNull() && (iMaximumLengthOfClientSideCharacterBuffer==0), PanicClientFromServer());
		iMessage_RequestForCharacters=*iFunctions->Message();
		iMaximumLengthOfClientSideCharacterBuffer=iFunctions->Message()->GetDesMaxLength(EAsyncIpcSlot);
		__ASSERT_ALWAYS((!iMessage_RequestForCharacters.IsNull()) && (iMaximumLengthOfClientSideCharacterBuffer>=STATIC_CAST(TInt, sizeof(TUint))), PanicClientFromServer());
		if (iArrayOfCharactersPending.Count()>0)
			{
			TRAPD(error, MovePendingCharactersToClientBufferL());
			CompleteRequestForCharacters(error);
			}
		else
			{
			iFunctions->GetRawEvents(ETrue);
			iSpriteFunctions->Activate(ETrue);
			}
		break;
	case EHandWritingRecognizerCommandCancelRequestForCharacters:
		if (!iMessage_RequestForCharacters.IsNull())
			{
			CompleteRequestForCharacters(KErrCancel);
			}
		break;
	case EHandWritingRecognizerCommandSetUpperCase:
		{
		STstParametersForHandWritingRecognizerCommandSetUpperCase* const parameters=REINTERPRET_CAST(STstParametersForHandWritingRecognizerCommandSetUpperCase*, aParameters);
		if (parameters->iUpperCase)
			{
			iFlags|=EFlagUpperCase;
			}
		else
			{
			iFlags&=~EFlagUpperCase;
			}
		}
		break;
	default:
		PanicClientFromServer();
		break;
		}
	return KErrNone; // dummy return to prevent compiler error
	}

TBool CTstHandWritingRecognizer::OfferRawEventLP(const TRawEvent& aRawEvent)
	{
	__ASSERT_DEBUG(!iMessage_RequestForCharacters.IsNull(), Panic(EPanicUnexpectedRawEvent));
	switch (aRawEvent.Type())
		{
	case TRawEvent::EPointerMove:
		{
		if (iFlags&EFlagPointerIsDown) // this test is needed in the cases where (i) the TRawEvent::EPointerMove event occurs when the pointer is not "down" (this is possible for some pointing devices, e.g. a mouse), and (ii) there was a "leave" when handling the TRawEvent::EButton1Down event
			{
			__ASSERT_DEBUG(!iTimeOutTimer->IsActive(), Panic(EPanicTimerActive1));
			const TPoint thisPoint=aRawEvent.Pos();
			CArrayFix<TPoint>& polyLine=*iArrayOfPolyLines[iArrayOfPolyLines.Count()-1];
			polyLine.AppendL(thisPoint);
			const TPoint previousPoint=polyLine[polyLine.Count()-2];
			TRect rectangleDrawnTo;
			DrawLine(*iMainBitmap, rectangleDrawnTo, EPenWidthForMainBitmap, previousPoint, thisPoint);
			TRect temp;
			DrawLine(*iMaskBitmap, temp, EPenWidthForMaskBitmap, previousPoint, thisPoint);
			rectangleDrawnTo.BoundingRect(temp);
			Plot(*iMainBitmap, temp, EPenWidthForMainBitmap, thisPoint);
			rectangleDrawnTo.BoundingRect(temp);
			Plot(*iMaskBitmap, temp, EPenWidthForMaskBitmap, thisPoint);
			rectangleDrawnTo.BoundingRect(temp);
			iSpriteFunctions->UpdateMember(0, rectangleDrawnTo, EFalse);
			}
		}
		return ETrue;
	case TRawEvent::EButton1Down:
		{
		iTimeOutTimer->Cancel();
		const TPoint thisPoint=aRawEvent.Pos();
		CArrayFix<TPoint>* const polyLine=new(ELeave) CArrayFixSeg<TPoint>(50);
		CleanupStack::PushL(polyLine);
		polyLine->AppendL(thisPoint);
		User::LeaveIfError(iArrayOfPolyLines.Append(polyLine));
		CleanupStack::Pop(); // polyLine
		TRect rectangleDrawnTo;
		Plot(*iMainBitmap, rectangleDrawnTo, EPenWidthForMainBitmap, thisPoint);
		TRect temp;
		Plot(*iMaskBitmap, temp, EPenWidthForMaskBitmap, thisPoint);
		rectangleDrawnTo.BoundingRect(temp);
		iSpriteFunctions->UpdateMember(0, rectangleDrawnTo, EFalse);
		iFlags|=EFlagPointerIsDown;
		if ((iArrayOfPolyLines.Count()==1) && (!iMessage_RequestForNotificationOfStartOfTransaction.IsNull()))
			{
			iMessage_RequestForNotificationOfStartOfTransaction.Complete(KErrNone);
			}
		}
		return ETrue;
	case TRawEvent::EButton1Up:
		if (iFlags&EFlagPointerIsDown) // this test is needed in the case where there was a "leave" when handling the TRawEvent::EButton1Down event
			{
			__ASSERT_DEBUG(!iTimeOutTimer->IsActive(), Panic(EPanicTimerActive2));
			iTimeOutTimer->After(ETimeOutInMicroSeconds);
			iFlags&=~EFlagPointerIsDown;
			}
		return ETrue;
	default:
		return EFalse;
		}
	}

void CTstHandWritingRecognizer::DoRecognitionL()
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
	TTime homeTime;
	homeTime.HomeTime();
	TInt64 seedForRandomNumber=homeTime.Int64();
	const TUint baseCharacter=(iFlags&EFlagUpperCase)? 'A': 'a';
	for (i=0; i<numberOfCharacters; ++i)
		{
		iArrayOfCharactersPending.AppendL(baseCharacter+(Math::Rand(seedForRandomNumber)%26));
		}
	MovePendingCharactersToClientBufferL();
	}

void CTstHandWritingRecognizer::MovePendingCharactersToClientBufferL()
	{
	__ASSERT_DEBUG(iArrayOfCharactersPending.Count()>0, Panic(EPanicNoPendingCharactersToMove));
	__ASSERT_DEBUG((!iMessage_RequestForCharacters.IsNull()) && (iMaximumLengthOfClientSideCharacterBuffer>=STATIC_CAST(TInt, sizeof(TUint))), Panic(EPanicUnexpectedNullPointer1));
	const TInt numberOfCharactersToMove=Min(iArrayOfCharactersPending.Count(), iMaximumLengthOfClientSideCharacterBuffer/sizeof(TUint));
	iMessage_RequestForCharacters.WriteL(EAsyncIpcSlot, iArrayOfCharactersPending.DescriptorFromStart(numberOfCharactersToMove), 0);
	iArrayOfCharactersPending.RemoveFromStart(numberOfCharactersToMove);
	}

void CTstHandWritingRecognizer::CompleteRequestForCharacters(TInt aErrorCode)
	{
	__ASSERT_DEBUG((!iMessage_RequestForCharacters.IsNull()) && (iMaximumLengthOfClientSideCharacterBuffer>=STATIC_CAST(TInt, sizeof(TUint))), Panic(EPanicUnexpectedNullPointer2));
	iArrayOfPolyLines.ResetAndDestroy();
	iArrayOfCharactersPending.Reset();
	iTimeOutTimer->Cancel();
	iMessage_RequestForCharacters.Complete(aErrorCode);
	iMaximumLengthOfClientSideCharacterBuffer=NULL;
	iFunctions->GetRawEvents(EFalse);
	iSpriteFunctions->Activate(EFalse);
	ClearBitmap(*iMainBitmap);
	ClearBitmap(*iMaskBitmap);
	// there is no need to call iSpriteFunctions->UpdateMember as the sprite has just been de-activated (3 lines above)
	}

void CTstHandWritingRecognizer::ClearBitmap(CFbsBitmapDevice& aBitmap)
	{
	iGraphicsContext->Activate(&aBitmap);
	iGraphicsContext->Clear();
	}

void CTstHandWritingRecognizer::DrawLine(CFbsBitmapDevice& aBitmap, TRect& aRectangleDrawnTo, TInt aPenSize, const TPoint& aPoint1, const TPoint& aPoint2)
	{
	iGraphicsContext->Activate(&aBitmap);
	iGraphicsContext->SetPenSize(TSize(aPenSize, aPenSize));
	iGraphicsContext->DrawLine(aPoint1, aPoint2);
	iGraphicsContext->RectDrawnTo(aRectangleDrawnTo);
	}

void CTstHandWritingRecognizer::Plot(CFbsBitmapDevice& aBitmap, TRect& aRectangleDrawnTo, TInt aPenSize, const TPoint& aPoint)
	{
	iGraphicsContext->Activate(&aBitmap);
	iGraphicsContext->SetPenSize(TSize(aPenSize, aPenSize));
	iGraphicsContext->Plot(aPoint);
	iGraphicsContext->RectDrawnTo(aRectangleDrawnTo);
	}

void CTstHandWritingRecognizer::ConstructL(TAny* aParameters)
	{
	TRAPD(error, ConstructLP(aParameters));
	HandleErrorIfErrorL(*iFunctions, error);
	}

TInt CTstHandWritingRecognizer::CommandReplyL(TInt aOpcode, TAny* aParameters)
	{
	TInt returnVal=0; // dummy initialization to prevent compiler warning
	TRAPD(error, returnVal=CommandReplyLP(aOpcode, aParameters));
	HandleErrorIfErrorL(*iFunctions, error);
	return returnVal;
	}

void CTstHandWritingRecognizer::Command(TInt, TAny*)
	{
	iFunctions->Panic();
	}

void CTstHandWritingRecognizer::Animate(TDateTime*)
	{
	}

TBool CTstHandWritingRecognizer::OfferRawEvent(const TRawEvent& aRawEvent)
	{
	TBool returnVal=0; // dummy initialization to prevent compiler warning
	TRAPD(error, returnVal=OfferRawEventLP(aRawEvent));
	if (error>=0) // if error is a KErrXxxxx, ignore it (as OfferRawEvent cannot leave)
		{
		HandleErrorIfError(*iFunctions, error);
		}
	return returnVal;
	}

// CTstHandWritingRecognizer::RFlatArrayOfCharacters

CTstHandWritingRecognizer::RFlatArrayOfCharacters::RFlatArrayOfCharacters(TInt aGranularity)
	:iGranularity(aGranularity),
	 iDescriptor(NULL)
	{
	__ASSERT_DEBUG(aGranularity>0, Panic(EPanicBadGranularity));
	}

void CTstHandWritingRecognizer::RFlatArrayOfCharacters::ConstructL()
	{
	__ASSERT_DEBUG(iDescriptor==NULL, Panic(EPanicAlreadyConstructed));
	iDescriptor=HBufC8::NewL(iGranularity*sizeof(TUint));
	}

void CTstHandWritingRecognizer::RFlatArrayOfCharacters::Close()
	{
	delete iDescriptor;
	iDescriptor=NULL;
	}

void CTstHandWritingRecognizer::RFlatArrayOfCharacters::AppendL(TUint aCharacter)
	{
	const TInt oldDescriptorLength=iDescriptor->Length();
	__ASSERT_DEBUG(oldDescriptorLength%sizeof(TUint)==0, Panic(EPanicNonAlignedDescriptorLength1));
	const TInt oldDescriptorMaximumLength=iDescriptor->Des().MaxLength();
	__ASSERT_DEBUG(oldDescriptorMaximumLength%sizeof(TUint)==0, Panic(EPanicNonAlignedDescriptorMaximumLength));
	if (oldDescriptorLength>=oldDescriptorMaximumLength)
		{
		iDescriptor=iDescriptor->ReAllocL(oldDescriptorLength+(iGranularity*sizeof(TUint)));
		}
	TPtr8 descriptor(iDescriptor->Des());
	__ASSERT_DEBUG(oldDescriptorLength==descriptor.Length(), Panic(EPanicBadDescriptorLength));
	descriptor.SetLength(oldDescriptorLength+sizeof(TUint));
	*REINTERPRET_CAST(TUint*, CONST_CAST(TUint8*, descriptor.Ptr()+oldDescriptorLength))=aCharacter;
	}

TInt CTstHandWritingRecognizer::RFlatArrayOfCharacters::Count() const
	{
	const TInt descriptorLength=iDescriptor->Length();
	__ASSERT_DEBUG(descriptorLength%sizeof(TUint)==0, Panic(EPanicNonAlignedDescriptorLength2));
	return descriptorLength/sizeof(TUint);
	}

TPtrC8 CTstHandWritingRecognizer::RFlatArrayOfCharacters::DescriptorFromStart(TInt aNumberOfCharacters) const
	{
	__ASSERT_DEBUG((aNumberOfCharacters>0) && (aNumberOfCharacters<=Count()), Panic(EPanicBadNumberOfCharacters1));
	return iDescriptor->Left(aNumberOfCharacters*sizeof(TUint));
	}

void CTstHandWritingRecognizer::RFlatArrayOfCharacters::RemoveFromStart(TInt aNumberOfCharacters)
	{
	__ASSERT_DEBUG((aNumberOfCharacters>0) && (aNumberOfCharacters<=Count()), Panic(EPanicBadNumberOfCharacters2));
	TPtr8 descriptor(iDescriptor->Des());
	descriptor.Delete(0, aNumberOfCharacters*sizeof(TUint));
	// we could "iDescriptor=iDescriptor->ReAllocL" here to free up unused memory, but it's probably not worth it as iDescriptor will never grow very large
	}

void CTstHandWritingRecognizer::RFlatArrayOfCharacters::Reset()
	{
	TPtr8 descriptor(iDescriptor->Des());
	descriptor.SetLength(0);
	// we could "iDescriptor=iDescriptor->ReAllocL" here to free up unused memory, but it's probably not worth it as iDescriptor will never grow very large
	}

// CTstHandWritingRecognizer::CTimeOutTimer

CTstHandWritingRecognizer::CTimeOutTimer* CTstHandWritingRecognizer::CTimeOutTimer::NewL(CTstHandWritingRecognizer& aHandWritingRecognizer)
	{
	CTimeOutTimer* const timeOutTimer=new(ELeave) CTimeOutTimer(aHandWritingRecognizer);
	CleanupStack::PushL(timeOutTimer);
	CActiveScheduler::Add(timeOutTimer);
	timeOutTimer->ConstructL();
	CleanupStack::Pop(); // timeOutTimer
	return timeOutTimer;
	}

CTstHandWritingRecognizer::CTimeOutTimer::~CTimeOutTimer()
	{
	Cancel();
	}

CTstHandWritingRecognizer::CTimeOutTimer::CTimeOutTimer(CTstHandWritingRecognizer& aHandWritingRecognizer)
	:CTimer(EPriorityLow),
	 iHandWritingRecognizer(aHandWritingRecognizer)
	{
	}

void CTstHandWritingRecognizer::CTimeOutTimer::RunL()
	{
	iHandWritingRecognizer.HandleTimeOut();
	}

// CTstDll

CAnim* CTstDll::CreateInstanceL(TInt aType)
	{
	switch (aType)
		{
	case EAnimTypeHandWritingRecognizer:
		return new(ELeave) CTstHandWritingRecognizer;
	default:
		User::Leave(KErrArgument);
		return NULL; // dummy return to prevent compiler error
		}
	}

// the exported function

EXPORT_C CAnimDll* CreateCAnimDllL()
	{
	return new(ELeave) CTstDll;
	}

