/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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

#include "PluginKrFepManagerHwr.h"

CPluginKrFepManagerHwr* CPluginKrFepManagerHwr::NewLC(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    {
    CPluginKrFepManagerHwr* self = 
        new (ELeave) CPluginKrFepManagerHwr(aOwner,aPenInputServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPluginKrFepManagerHwr* CPluginKrFepManagerHwr::NewL(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    {
    CPluginKrFepManagerHwr* self = 
        CPluginKrFepManagerHwr::NewLC(aOwner,aPenInputServer);
    CleanupStack::Pop(); // self;
    return self;
    }

CPluginKrFepManagerHwr::CPluginKrFepManagerHwr(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    : CPluginKrFepManagerBase(aOwner,aPenInputServer)
    {
    }

void CPluginKrFepManagerHwr::ConstructL()
    {
    }

CPluginKrFepManagerHwr::~CPluginKrFepManagerHwr()
    {
    }
