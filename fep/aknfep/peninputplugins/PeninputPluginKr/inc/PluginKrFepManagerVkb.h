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

#ifndef PLUGINKRFEPMANAGERVKB_H
#define PLUGINKRFEPMANAGERVKB_H

#include <e32std.h>
#include <e32base.h>
#include "PluginKrFepManagerBase.h"

class CPluginKrFepManagerVkb : public CPluginKrFepManagerBase
    {
public:
    
    static CPluginKrFepManagerVkb* NewLC(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer);

    static CPluginKrFepManagerVkb* NewL(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer);

    virtual ~CPluginKrFepManagerVkb();

private:

    CPluginKrFepManagerVkb(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer);

    void ConstructL();

    };

#endif // PLUGINKRFEPMANAGERVKB_H
