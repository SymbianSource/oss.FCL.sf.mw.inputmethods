/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*/
/*
 * ============================================================================
 *  Name     : CGSPredictiveSettingData from AknFepPredictiveSettingData 
 * ============================================================================
 */

#include "DualLanguageData.h"

CGSDualLanguageData *CGSDualLanguageData::NewL()
	{
	CGSDualLanguageData *self = CGSDualLanguageData::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CGSDualLanguageData *CGSDualLanguageData::NewLC()
	{
	CGSDualLanguageData *self = new (ELeave) CGSDualLanguageData();
	CleanupStack::PushL(self);
	return self;
	}

CGSDualLanguageData::~CGSDualLanguageData()
	{
	if(iItems != NULL)
	    {
	    delete iItems;
	    }
	if(iSecondaryItems != NULL)
	    {
	    delete iSecondaryItems;
	    }
	}

CGSDualLanguageData::CGSDualLanguageData()
	{
	}

void CGSDualLanguageData::LoadL(CDesCArrayFlat* aItems,
                                CDesCArrayFlat* aSecondaryItems,
                                TInt aCurrentValue,
                                TInt aCurrentSecondaryValue)
    {
    iItems = aItems;
    iSecondaryItems = aSecondaryItems;
    iCurrentValue = aCurrentValue;
    iCurrentSecondaryValue = aCurrentSecondaryValue;
    }
    
void CGSDualLanguageData::SaveL() const
    {
    }
//EOF
