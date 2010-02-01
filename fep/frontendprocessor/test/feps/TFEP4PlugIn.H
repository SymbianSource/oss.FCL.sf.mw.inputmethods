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

#ifndef __TFEP4PLUGIN_H__
#define __TFEP4PLUGIN_H__

#include <fepplugin.h>



class CTstFepPlugIn : public CCoeFepPlugIn
	{
public:
	static CTstFepPlugIn* NewL();
	virtual CCoeFep* NewFepL(CCoeEnv& aConeEnvironment, const CCoeFepParameters& aFepParameters);
	virtual void SynchronouslyExecuteSettingsDialogL(CCoeEnv& aConeEnvironment);
	};
	

#endif
