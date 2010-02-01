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
* Description:               Pen input related enums
*
*/














#ifndef C_CPENINPUTSERVERHANDLER_H
#define C_CPENINPUTSERVERHANDLER_H

#include <e32base.h>
#include <peninputgenericitutcustomization.h>
#include <peninputclient.h>

class CPenInputServerHandler : public CBase
    {
public:
    IMPORT_C static CPenInputServerHandler* NewL();
    IMPORT_C virtual ~CPenInputServerHandler();
    IMPORT_C TInt ConnectServer();
    IMPORT_C void SetUiLayout(TInt aLayoutId);
    IMPORT_C void DisconnectServer();
    IMPORT_C void ActivateUiLayout(TBool aActivate);
    IMPORT_C void SendCommandToServer(TInt aCmdId);
    IMPORT_C void SendCommandToServer(TInt aCmdId, TInt aParam);
    IMPORT_C void SendCommandToServer(TInt aCmdId, const TDesC& aParam);
    IMPORT_C void SendNewKeyToServer(TInt aCmdId, const TItutKey& aKeyInfo);
    IMPORT_C void SetIcfText(const TFepInputContextFieldData& aData);
    IMPORT_C void SendPromptTextToServer(TInt aCmdId, const TFepPromptText& aPromptText);
private:
    CPenInputServerHandler();
    void ConstructL();
private:
    RPeninputServer iPenInputServer;
    TBool iPenInputSvrConnected;    
    };

#endif // C_CPENINPUTSERVERHANDLER_H
