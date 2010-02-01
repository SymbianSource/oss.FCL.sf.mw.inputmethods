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










#include "AknFepUIAvkonCtrlDualLanguageSettingData.h"

CAknFepDualLanguageSettingData *CAknFepDualLanguageSettingData::NewL()
	{
	CAknFepDualLanguageSettingData *self = CAknFepDualLanguageSettingData::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CAknFepDualLanguageSettingData *CAknFepDualLanguageSettingData::NewLC()
	{
	CAknFepDualLanguageSettingData *self = new (ELeave) CAknFepDualLanguageSettingData();
	CleanupStack::PushL(self);
	return self;
	}

CAknFepDualLanguageSettingData::~CAknFepDualLanguageSettingData()
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

CAknFepDualLanguageSettingData::CAknFepDualLanguageSettingData()
	{
	}

void CAknFepDualLanguageSettingData::LoadL(CDesCArrayFlat* aItems,
                                CDesCArrayFlat* aSecondaryItems,
                                TInt aCurrentValue,
                                TInt aCurrentSecondaryValue)
    {
    iItems = aItems;
    iSecondaryItems = aSecondaryItems;
    iCurrentValue = aCurrentValue;
    iCurrentSecondaryValue = aCurrentSecondaryValue;
    }
    
void CAknFepDualLanguageSettingData::SaveL() const
    {
    }
//EOF
