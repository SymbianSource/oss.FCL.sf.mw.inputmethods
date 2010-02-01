// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__TFEP1COM_H__)
#define __TFEP1COM_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

const TUint KTfep1Uid=0x100013f8;

class TCoeInputCapabilities::MCoeFepSpecificExtensions // forward declared in COEINPUT.H
	{
public:
	virtual TBool SupportsPhoneticAlphabet() const=0;
	};

#endif

