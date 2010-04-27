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

#include <e32property.h>
#include <AvkonInternalCRKeys.h>
#include <AknFepInternalPSKeys.h>
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

TPtiKeyboardType CAknFepPredictiveSettingData::ActiveKeyboardType() const
	{    
	TInt keyboardType = EPtiKeyboardNone;
#ifdef RD_SCALABLE_UI_V2
	// Get physical keyboard type
	RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardType );

	//1. When active keyboard is virtual keyboard, 
	// need to get the active virtual keyboard type 
	// and set keyboardType again.	
	//2. When the default physical keyboard is 0, 
	// it means that there is no physical keyboard,
	// also need to get the active virtual keyboard type.
	TInt isVirtualInputActive = 0;
	RProperty::Get( KPSUidAknFep, KAknFepTouchInputActive, 
					isVirtualInputActive );    
	if ( isVirtualInputActive > 0 || keyboardType == 0 )
		{
		// Active keyboard is virtual keyboard          
		RProperty::Get( KPSUidAknFep, KAknFepVirtualKeyboardType, 
						keyboardType );      
		}
#else
	// Get physical keyboard type
	RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardType );	    
#endif

	if ( keyboardType <= 0 || keyboardType >= EPtiKeyboardMaxLayout )
		{
	    keyboardType = EPtiKeyboardNone;
		}        
	return (TPtiKeyboardType)keyboardType;
    }

TPtiKeyboardType CAknFepPredictiveSettingData::OwnerKeyboardType() const
	{
	TInt keyboardType = EPtiKeyboardNone;
#ifdef RD_SCALABLE_UI_V2	
	// When dialog state is 2, it means that the setting dialog is opened
	// for virtual qwerty keyboard, therefore, 
	// returning value of virtual qwerty keyboard type is necessary 
	TInt dialogState = 0;
	RProperty::Get( KPSUidAknFep, KAknFepSettingDialogState, dialogState );    	
	if ( dialogState == 2 )
		{
		// The setting dialog is opened by virtual qwerty keyboard,
	    // then need to get current virtual keyboard type 
	    keyboardType = EPtiKeyboardQwerty4x12;
		}
	else
		{
	    // Get physical keyboard type
	    RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardType );	    
		}
#else
    // Get physical keyboard type
    RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardType );		
#endif // RD_SCALABLE_UI_V2

	if ( keyboardType <= 0 || keyboardType >= EPtiKeyboardMaxLayout )
		{
	    keyboardType = EPtiKeyboardNone;
		}		
	return (TPtiKeyboardType)keyboardType;	
	}

//EOF
