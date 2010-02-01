/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/
#ifndef __PYTABLE_H__
#define __PYTABLE_H__


#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__W32STD_H__)
#include <w32std.h>
#endif

const TInt KMaxPinyinLength = 15;

struct TPinyinCharacterPair
	{
public:
	const TUint16 *iPinYin;
	TInt iPinYinLength;
	TInt iCharacterCount;
	const TInt *iCharacterArray;
	};
struct TPinyinTable
	{
public:
	TInt iPinyinCharacterPairCount;
	const TPinyinCharacterPair *iPairArray;
	};

class CPinyinSelector
	{
public:
	enum
	{
	EInvalidChar = 0xFFFFFFFF
	};
public:
	CPinyinSelector();
	TBool SetPinyin(const TDesC *aCurrentPinyin, TInt aIndexInCurrentPinyin=-1);

	// by character
	TInt NextCharacter();
	TInt PreviousCharacter();
	
	// by page
	void SetPageSize(TInt nPageSize);
	TInt NextPage();			// return first character in next page
	TInt PreviousPage();		// return first character in previous page
	TInt CharacterInCurrentPage(TInt aIndex);	// return "aIndex"th character in current page. aIndex range from 0
	void GetPageDescriptor(TBuf<200> *page) const;
	
	void Reset();
	TBool IsReseted();

public:
	static TBool IsValidPinyinChar(TInt aChar);

private:
	// current
	const TPinyinCharacterPair *iCurrentPinyin;
	TInt iIndexInCurrentPinyin;
	TInt iPageSize;		// character count in a page
	TBool iReset;
private:
	// the table
	const TPinyinTable *iPinyinTable;
	};

GLREF_D const TPinyinTable pinyinTable;

#endif	// __PYTABLE_H__
