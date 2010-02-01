/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*
*/


// INCLUDE FILES
#include <e32def.h>
#include <bamdesca.h>
#include <AknInputLanguageInfo.h>
#include "ptiInputLanguageInfo.h"

CAknInputLanguageItem* CAknInputLanguageItem::NewL( 
	TLanguage aLanguageCode, 
	const TDesC& aName, 
	TAknInputLanguageCapabilities aCapabilities )
	{
    CAknInputLanguageItem* item = new(ELeave)CAknInputLanguageItem(aLanguageCode);
    CleanupStack::PushL(item);
    item->ConstructL( aName, aCapabilities);
	CleanupStack::Pop(); // item
    return item;
	}

EXPORT_C CAknInputLanguageItem::~CAknInputLanguageItem()
	{
	delete iLanguageName;
	}

CAknInputLanguageItem::CAknInputLanguageItem( TLanguage aLanguageCode ): 
	iLanguageCode ( aLanguageCode )
	{
	}

void CAknInputLanguageItem::ConstructL( 
	const TDesC& aName, 
	TAknInputLanguageCapabilities aCapabilities) 
	{
	iLanguageName = aName.AllocL();
	iCapabilities = aCapabilities; 
	}

EXPORT_C TLanguage CAknInputLanguageItem::LanguageCode() const
	{
	return iLanguageCode;
	}

EXPORT_C TPtrC CAknInputLanguageItem::LanguageName() const
	{
	return iLanguageName->Des();
	}

EXPORT_C TAknInputLanguageCapabilities CAknInputLanguageItem::Capabilities() const
	{
	return iCapabilities;
	}


EXPORT_C CAknInputLanguageList::CAknInputLanguageList(TInt aGranularity): 
	CArrayPtrFlat<CAknInputLanguageItem>( aGranularity )
	{
	}

EXPORT_C CAknInputLanguageList::~CAknInputLanguageList()
	{
	ResetAndDestroy();
	}

EXPORT_C TInt CAknInputLanguageList::MdcaCount() const
	{
	return Count();
	}

EXPORT_C TPtrC CAknInputLanguageList::MdcaPoint(TInt aIndex) const
	{
	return At( aIndex )->LanguageName();
	}

// End of File
