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

#ifndef __FEPBPRIV_H__
#define __FEPBPRIV_H__

#include <e32base.h>
#include <coemain.h>	// class CCoeEnv

const TUint KUidFepSpecificSettingsRepository=0x10009793;

class MFepAttributeStorer;
class CRepository;

/** 
@internalComponent
*/
NONSHARABLE_CLASS(CFepSettingsTracker) : public CActive
	{
public:
	static CFepSettingsTracker* NewL(CCoeEnv& aConeEnvironment, MFepAttributeStorer& aFepAttributeStorer);
	virtual ~CFepSettingsTracker();
private:
	CFepSettingsTracker(CCoeEnv& aConeEnvironment, MFepAttributeStorer& aFepAttributeStorer);
	void ConstructL();
	void Queue();
	// from CActive
	virtual void DoCancel();
	virtual void RunL();
private:
	CCoeEnv& iConeEnvironment;
	MFepAttributeStorer& iFepAttributeStorer;
	CRepository* iRepository;
	};


#endif // __FEPBPRIV_H__