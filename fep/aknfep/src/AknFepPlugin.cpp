/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       CAknFepPlugin implementation.
*
*/











#include "AknFepManager.h"
#include "AknFepPlugin.h"
#include <ecom/implementationproxy.h>

CAknFepPlugin* CAknFepPlugin::NewL()
	{
	return new (ELeave) CAknFepPlugin();
	}


CCoeFep* CAknFepPlugin::NewFepL(CCoeEnv& aConeEnvironment, const CCoeFepParameters& aFepParameters)
	{
    CAknFepManager* const aknFep = new(ELeave) CAknFepManager(aConeEnvironment);
    CleanupStack::PushL(aknFep);
    aknFep->ConstructL(aFepParameters);
    CleanupStack::Pop(); // aknFep
    return aknFep;
	}


void CAknFepPlugin::SynchronouslyExecuteSettingsDialogL(CCoeEnv& /*aConeEnvironment*/)
	{
	}


const TImplementationProxy ImplementationTable[] =
 	{
#ifdef __EABI__ 
	IMPLEMENTATION_PROXY_ENTRY(0x101FD65A, CAknFepPlugin::NewL) 
#else
	{{0x101FD65A}, CAknFepPlugin::NewL}		
#endif
 	};	
  
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
 	{
 	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
 	return ImplementationTable;
 	}

// End of file

