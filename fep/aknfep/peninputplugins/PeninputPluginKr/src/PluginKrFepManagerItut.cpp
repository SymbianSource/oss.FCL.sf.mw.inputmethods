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

#include "PeninputImePluginKr.h"
#include "eikon.hrh"

#include <aknedsts.h>               //CAknEdwinState
#include <aknfeppeninputenums.h>


#include "PluginKrFepManagerItut.h"

CPluginKrFepManagerItut* CPluginKrFepManagerItut::NewLC(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    {
    CPluginKrFepManagerItut* self = 
        new (ELeave) CPluginKrFepManagerItut(aOwner,aPenInputServer);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    return self;
    }

CPluginKrFepManagerItut* CPluginKrFepManagerItut::NewL(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    {
    CPluginKrFepManagerItut* self = 
        CPluginKrFepManagerItut::NewLC(aOwner,aPenInputServer);
    CleanupStack::Pop(); // self;
    return self;
    }

TBool CPluginKrFepManagerItut::HandleKeyL( TInt aKey, TKeyPressLength aLength,
                                           TEventCode /*aEventCode*/)
    {
    TInt ret;
    TPckgC<TInt> msgdata(aKey);
    
    if (aLength == ELongKeyPress)
        {
        iPenInputServer.HandleCommand(ECmdPenInputFingerLongKeyPress, msgdata, ret);
        }
    else
        {
        iPenInputServer.HandleCommand(ECmdPenInputFingerKeyPress, msgdata, ret);
        }
    
    return ret == KErrNone ? ETrue:EFalse;
    }

CPluginKrFepManagerItut::CPluginKrFepManagerItut(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    : CPluginKrFepManagerBase(aOwner,aPenInputServer)
    {
    }

void CPluginKrFepManagerItut::ConstructL()
    {
    }

CPluginKrFepManagerItut::~CPluginKrFepManagerItut()
    {
    }
