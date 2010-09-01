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

#include "PluginKrFepManagerVkb.h"

CPluginKrFepManagerVkb* CPluginKrFepManagerVkb::NewLC(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    {
    CPluginKrFepManagerVkb* self = 
        new (ELeave) CPluginKrFepManagerVkb(aOwner,aPenInputServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPluginKrFepManagerVkb* CPluginKrFepManagerVkb::NewL(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    {
    CPluginKrFepManagerVkb* self = 
        CPluginKrFepManagerVkb::NewLC(aOwner,aPenInputServer);
    CleanupStack::Pop(); // self;
    return self;
    }

CPluginKrFepManagerVkb::CPluginKrFepManagerVkb(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    : CPluginKrFepManagerBase(aOwner,aPenInputServer)
    {
    }

void CPluginKrFepManagerVkb::ConstructL()
    {
    }

CPluginKrFepManagerVkb::~CPluginKrFepManagerVkb()
    {
    }
