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

#include "PredictiveSettingData.h"

CGSPredictiveSettingData *CGSPredictiveSettingData::NewL()
	{
	CGSPredictiveSettingData *self = CGSPredictiveSettingData::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CGSPredictiveSettingData *CGSPredictiveSettingData::NewLC()
	{
	CGSPredictiveSettingData *self = new (ELeave) CGSPredictiveSettingData();
	CleanupStack::PushL(self);
	return self;
	}

CGSPredictiveSettingData::~CGSPredictiveSettingData()
	{
	}

CGSPredictiveSettingData::CGSPredictiveSettingData()
	{
	}



void CGSPredictiveSettingData::LoadL(TInt aAutoWordCompl,
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
    
void CGSPredictiveSettingData::SaveL() const
    {
    }
//EOF
