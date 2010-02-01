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

#if !defined(__TFEP2BE_H__)
#define __TFEP2BE_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__W32ADLL_H__)
#include <w32adll.h>
#endif

#if !defined(__TFEP2COM_H__)
#include "tfep2com.h"
#endif

class TRawEvent;
class CFbsBitmapDevice;
class CFbsBitGc;

class CTstHandWritingRecognizer : public CSpriteAnim
	{
public:
	CTstHandWritingRecognizer();
	~CTstHandWritingRecognizer();
	void HandleTimeOut();
private:
	typedef CArrayFix<TPoint> CPolyLine;
	enum
		{
		EPenWidthForMainBitmap=4,
		EPenWidthForMaskBitmap=6
		};
	enum
		{
		EFlagReferenceCountIncremented	=0x00000001,
		EFlagPointerIsDown				=0x00000002,
		EFlagUpperCase					=0x00000004
		};
	struct SBitmapHandlesWithReferenceCount
		{
		STstBitmapHandles iBitmapHandles;
		TInt iReferenceCount;
		};
	class RFlatArrayOfCharacters
		{
	public:
		RFlatArrayOfCharacters(TInt aGranularity);
		void ConstructL();
		void Close();
		void AppendL(TUint aCharacter);
		TInt Count() const;
		TPtrC8 DescriptorFromStart(TInt aNumberOfCharacters) const;
		void RemoveFromStart(TInt aNumberOfCharacters);
		void Reset();
	private:
		const TInt iGranularity;
		HBufC8* iDescriptor;
		};
	class CTimeOutTimer : public CTimer
		{
	public:
		static CTimeOutTimer* NewL(CTstHandWritingRecognizer& aHandWritingRecognizer);
		virtual ~CTimeOutTimer();
	private:
		CTimeOutTimer(CTstHandWritingRecognizer& aHandWritingRecognizer);
		virtual void RunL();
	private:
		CTstHandWritingRecognizer& iHandWritingRecognizer;
		};
private:
	void ConstructLP(TAny* aParameters);
	TInt CommandReplyLP(TInt aOpcode, TAny* aParameters);
	TBool OfferRawEventLP(const TRawEvent& aRawEvent);
	void DoRecognitionL();
	void MovePendingCharactersToClientBufferL();
	void CompleteRequestForCharacters(TInt aErrorCode);
	void ClearBitmap(CFbsBitmapDevice& aBitmap);
	void DrawLine(CFbsBitmapDevice& aBitmap, TRect& aRectangleDrawnTo, TInt aPenSize, const TPoint& aPoint1, const TPoint& aPoint2);
	void Plot(CFbsBitmapDevice& aBitmap, TRect& aRectangleDrawnTo, TInt aPenSize, const TPoint& aPoint);
	// from CSpriteAnim
	virtual void ConstructL(TAny* aParameters);
	// from CAnim
	virtual TInt CommandReplyL(TInt aOpcode, TAny* aParameters);
	virtual void Command(TInt aOpcode, TAny* aParameters);
	virtual void Animate(TDateTime* aDateTime);
	// from MEventHandler (indirectly, via CAnim)
	virtual TBool OfferRawEvent(const TRawEvent& aRawEvent);
private:
	TUint iFlags;
	RPointerArray<CPolyLine> iArrayOfPolyLines;
	RFlatArrayOfCharacters iArrayOfCharactersPending;
	CTimeOutTimer* iTimeOutTimer;
	RMessagePtr2 iMessage_RequestForNotificationOfStartOfTransaction;
	RMessagePtr2 iMessage_RequestForCharacters;
	TInt iMaximumLengthOfClientSideCharacterBuffer; // effectively the MaxLength of the descriptor in the client-side address-space
	CFbsBitmapDevice* iMainBitmap;
	CFbsBitmapDevice* iMaskBitmap;
	CFbsBitGc* iGraphicsContext;
	};

class CTstDll : public CAnimDll
	{
public:
	CAnim* CreateInstanceL(TInt aType);
	};

#endif

