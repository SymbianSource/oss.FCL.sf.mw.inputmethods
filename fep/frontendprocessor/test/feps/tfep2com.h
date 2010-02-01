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

#if !defined(__TFEP2COM_H__)
#define __TFEP2COM_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

enum {ETimeOutInMicroSeconds=500000};

enum
	{
	EIpcSlot=1, // must be at least 1 as the first slot of the TIpcArgs passed to RAnim::CommandReply is reserved
	EAsyncIpcSlot=2, // must be at least 2 as the first two slots of the TIpcArgs passed to RAnim::AsyncCommandReply are reserved
	};

enum
	{
	EAnimTypeHandWritingRecognizer=100
	};

enum
	{
	EHandWritingRecognizerCommandFinishConstructionL=200,
	EHandWritingRecognizerCommandRequestNotificationOfStartOfTransaction,
	EHandWritingRecognizerCommandCancelRequestForNotificationOfStartOfTransaction,
	EHandWritingRecognizerCommandRequestCharacters,
	EHandWritingRecognizerCommandCancelRequestForCharacters,
	EHandWritingRecognizerCommandSetUpperCase
	};

struct STstBitmapHandles
	{
	inline STstBitmapHandles() :iMain(0), iMask(0) {}
	inline STstBitmapHandles(TInt aMain, TInt aMask) :iMain(aMain), iMask(aMask) {}
	TInt iMain;
	TInt iMask;
	};

struct STstParametersForHandWritingRecognizerCommandSetUpperCase
	{
	TBool iUpperCase;
	};

#endif

