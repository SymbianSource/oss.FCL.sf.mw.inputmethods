/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Predective text input engine user dictionary 
*               class implementation.
*
*/


#include <s32file.h>
#include "PtiUserDictionary.h"

//                                 Core UID          Symbol class      CRC checksum
#define UDB_HEADER_SIZE (TInt32)(sizeof(iCoreUID) + sizeof(TUint32) + sizeof(TUint32))

// ---------------------------------------------------------------------------
// CPtiUserDictionary::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiUserDictionary* CPtiUserDictionary::NewL(TInt aMaxSize)
	{
	CPtiUserDictionary* ud = new (ELeave) CPtiUserDictionary();
	CleanupStack::PushL( ud );
	ud->ConstructL(aMaxSize);
	CleanupStack::Pop(); // ud
	return ud;
	}
	
// ---------------------------------------------------------------------------
// CPtiUserDictionary::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiUserDictionary* CPtiUserDictionary::NewL(TDesC& aFileName, TInt aMaxSize)
	{
	CPtiUserDictionary* ud = new (ELeave) CPtiUserDictionary();
	CleanupStack::PushL( ud );
	ud->ConstructL(aFileName, aMaxSize);
	CleanupStack::Pop(); // ud
	return ud;
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::CPtiUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiUserDictionary::CPtiUserDictionary()
	{
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::~CPtiUserDictionary
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C CPtiUserDictionary::~CPtiUserDictionary()
	{
	delete iData;
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CPtiUserDictionary::ConstructL(TInt aMaxSize)
	{
	iData = HBufC8::NewL(aMaxSize);
	TPtr8 bufP8( iData->Des() );
	bufP8.SetLength(aMaxSize);
	bufP8.FillZ();
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CPtiUserDictionary::ConstructL(TDesC& aFileName, TInt aMaxSize)
	{
	User::LeaveIfError(OpenL(aFileName, aMaxSize));
	iFileName.Copy(aFileName);
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::OpenL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiUserDictionary::OpenL(TDesC& aFileName, TInt aMaxSize)
	{
	iFileName.Copy(aFileName);

	RFs fsSession;
	User::LeaveIfError(fsSession.Connect()); // Start session
	CleanupClosePushL(fsSession);

	TEntry entry;
	User::LeaveIfError(fsSession.Entry(aFileName, entry));

	RFileReadStream inStream;
	inStream.PushL();
	User::LeaveIfError(inStream.Open(fsSession, aFileName, EFileRead|EFileShareAny));

	if (entry.iSize > aMaxSize + UDB_HEADER_SIZE)
		{
		User::Leave(KErrCorrupt);
		}
		
	delete iData;
	iData = NULL;
	iData = HBufC8::NewL(aMaxSize);
	TPtr8 dataDes = iData->Des();
	dataDes.SetLength(aMaxSize);
	dataDes.FillZ();		
	
	// First read header data.
	iCoreUID = inStream.ReadInt32L();
	iSymbolClass = inStream.ReadInt32L();
	
	// Then read and check actual data if it is there.
	if (entry.iSize > sizeof(TInt32) + sizeof(TInt32))	
		{	
		TUint32 crc = inStream.ReadInt32L();
		inStream.ReadL(dataDes, entry.iSize - UDB_HEADER_SIZE);		
		
		TUint32 dataCrc = 0;
		Mem::Crc32(dataCrc, Data().Ptr(), Data().Length());
		if (dataCrc != crc)
			{
			// Crc check failed, something wierd going on, initialize data and leave.
			dataDes.FillZ();
			User::Leave(KErrCorrupt);	
			}		
		iChecksum = crc;			
		}
		
	CleanupStack::PopAndDestroy(2); // fsSession, inStream
	
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::WriteL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiUserDictionary::WriteL(TDesC& aFileName)
	{
	RFs fsSession;
	User::LeaveIfError(fsSession.Connect()); // Start session
	CleanupClosePushL(fsSession);

	RFileWriteStream outStream;
	outStream.PushL();
	User::LeaveIfError(outStream.Open(fsSession, aFileName, EFileWrite|EFileShareAny));
	
	// First write header data.	
	outStream.WriteInt32L(iCoreUID);
	outStream.WriteInt32L(iSymbolClass);
	TUint32 c = 0;		
	Mem::Crc32(c, Data().Ptr(), Data().Length());
	outStream.WriteInt32L(c);
	
	// Then write actual data.
	outStream.WriteL(Data().Ptr(), Data().Length());	
		
	outStream.CommitL();	
	CleanupStack::PopAndDestroy(2); // fsSession, outStream  	
		
	iChecksum = c;

	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::WriteL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiUserDictionary::WriteL()
	{
	if (iFileName.Length() == 0)
		{
		return KErrCorrupt;
		}
	
	return WriteL( iFileName );
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::SpaceLeft
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiUserDictionary::SpaceLeft() const	
	{
	return -1;
	}

// ---------------------------------------------------------------------------
// CPtiUserDictionary::SetDataL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiUserDictionary::SetDataL(TAny* aData, TInt aLength)
	{
	// Keep possible existing maxlength value, because it is coming from 
	// host.
	TInt maxLength = (iData ? iData->Des().MaxLength() : aLength);

	delete iData;
	iData = NULL;
	iData = HBufC8::NewL(maxLength);
	TPtr8 dataP((TUint8*)aData, maxLength);
	dataP.Set((TUint8*)aData, aLength, maxLength);
	(*iData) = dataP;
	}

// End of file

