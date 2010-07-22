/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIInterface methods.
*
*/












#include "aknfepuiinterface.h"
#include "AknFepUiInterfacePlugin.h"
#include <ecom/ecom.h>

void PanicNoImpl()
	{
	_LIT(KReason, "NoFepUiImpl");
	User::Panic(KReason, 0);
	}

const TUid KAvkonPluginUid = { 0x10285839 };

void EComDtorCleanup(TAny* aUid)
	{
	TUid dtor = {(TInt)aUid};
	REComSession::DestroyedImplementation(dtor);
	}

void EComImplCleanup(TAny* aArray)
	{
	RImplInfoPtrArray* array = (RImplInfoPtrArray*) aArray;
	array->ResetAndDestroy();
	}
	
// factory function	
EXPORT_C CAknFepUIInterface* CAknFepUIInterface::NewL()
	{
	RImplInfoPtrArray impls;
	CleanupStack::PushL(TCleanupItem(EComImplCleanup, &impls));	
	REComSession::ListImplementationsL(TUid::Uid(CFepUiInterfacePlugin::ETypeId), impls);
	
	// Move Avkon plugin to the back of the list to act as default implementation
	TInt count = impls.Count();
	for (int i=0; i<count-1; i++)
		{
		CImplementationInformation* info = impls[i];
		if (info->ImplementationUid() == KAvkonPluginUid)
			{
			impls.Remove(i);
			// Append will not fail as space has been freed by Remove
			impls.Append(info);
			break;
			}
		}

	// Find the first plugin that says it should be used	
	CAknFepUIInterface* impl = NULL;
	for (int i=0; !impl && i<count; i++)
		{
		TUid dtorUid;
		CFepUiInterfacePlugin* plugin = (CFepUiInterfacePlugin*)REComSession::CreateImplementationL(impls[i]->ImplementationUid(), dtorUid);
		if (plugin->UseThisPlugin())
			{
			CleanupStack::PushL(TCleanupItem(EComDtorCleanup, (TAny*)dtorUid.iUid));
			CleanupStack::PushL(plugin);
			impl = plugin->NewImplL();
			impl->iEcomDtorUid = dtorUid;
			CleanupStack::PopAndDestroy(plugin);
			CleanupStack::Pop();
			}
		else
			{
			delete plugin;
			REComSession::DestroyedImplementation(dtorUid);
			}
		}
	
	CleanupStack::PopAndDestroy(&impls);
	
	__ASSERT_ALWAYS(impl, PanicNoImpl());
	return impl;
	}

EXPORT_C CAknFepUIInterface::~CAknFepUIInterface()
	{
	if (iEcomDtorUid.iUid)
		REComSession::DestroyedImplementation(iEcomDtorUid);
	}
