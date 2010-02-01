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
 *  Name     : CAknFepPredictiveSettingData from AknFepPredictiveSettingData 
 * ============================================================================
 */

#include "AknFepPredictiveSettingData.h"

CAknFepPredictiveSettingData *CAknFepPredictiveSettingData::NewL()
	{
	CAknFepPredictiveSettingData *self = CAknFepPredictiveSettingData::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CAknFepPredictiveSettingData *CAknFepPredictiveSettingData::NewLC()
	{
	CAknFepPredictiveSettingData *self = new (ELeave) CAknFepPredictiveSettingData();
	CleanupStack::PushL(self);
	return self;
	}

CAknFepPredictiveSettingData::~CAknFepPredictiveSettingData()
	{
	}

CAknFepPredictiveSettingData::CAknFepPredictiveSettingData()
	{
	}



void CAknFepPredictiveSettingData::LoadL(TInt aAutoWordCompl,
										 TInt aTypingCorrection,
										 TInt aNumberCandidates,
										 TInt aPrimaryCandidate,
										 TInt aPhysicalKeyboards,
										 TInt aAWCBitmask)
    {
    iAutoWordCompl = aAutoWordCompl;
    iTypingCorrection = aTypingCorrection;
    iNumberCandidates = aNumberCandidates;
    iPrimaryCandidate = aPrimaryCandidate;
    iPhysicalKeyboards = aPhysicalKeyboards;
    iAWCBitmask = aAWCBitmask;
    }
    
void CAknFepPredictiveSettingData::SaveL() const
    {
    }
//EOF
