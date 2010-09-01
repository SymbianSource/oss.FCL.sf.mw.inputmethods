/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateInitialLatinMultitap methods.
*
*/












#include "AknFepUiInputStateInitialLatinMultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface

#include "AknFepCaseManager.h"
#include <uikon.hrh>

#include <PtiEngine.h>

TAknFepInputStateInitialLatinMultitap::
TAknFepInputStateInitialLatinMultitap(MAknFepUIManagerStateInterface* aOwner)
:TAknFepInputStateInitialMultitapBase(aOwner)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtiTextCase caseMode = ptiengine->Case();
    ptiengine->SetCase(caseMode);
    }

// End of file
