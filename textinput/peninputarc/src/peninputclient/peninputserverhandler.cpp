/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <peninputserverhandler.h>

EXPORT_C CPenInputServerHandler* CPenInputServerHandler::NewL()
    {
    CPenInputServerHandler* self = new (ELeave) CPenInputServerHandler(); 
    CleanupStack::PushL( self );
    self->ConstructL();       
    CleanupStack::Pop(self);
    return self;
    }

CPenInputServerHandler::CPenInputServerHandler()
    :iPenInputSvrConnected(EFalse)
    {
    }

void CPenInputServerHandler::ConstructL()
    {
    }

EXPORT_C CPenInputServerHandler::~CPenInputServerHandler()
    {
    if (iPenInputSvrConnected)
        {
        DisconnectServer();
        }
    }
    
EXPORT_C TInt CPenInputServerHandler::ConnectServer()
    {
    if (!iPenInputSvrConnected)
        {
        if (iPenInputServer.Connect() != KErrNone)
            {
            return KErrCouldNotConnect;
            }
     
        iPenInputSvrConnected = ETrue;
        iPenInputServer.SetForeground(EFalse);    
        }
    
    return KErrNone;    
    }

EXPORT_C void CPenInputServerHandler::SetUiLayout(TInt aLayoutId)
    {
    if (iPenInputSvrConnected)
        {
        TUid layoutId;
        layoutId.iUid = aLayoutId;
        iPenInputServer.SetUiLayoutId(layoutId);
        iPenInputServer.ActivateLayout(ETrue);
        }
    }

EXPORT_C void CPenInputServerHandler::DisconnectServer()
    {
    if (iPenInputSvrConnected)
        {
        iPenInputServer.ActivateLayout(EFalse);
        iPenInputServer.Close();

        iPenInputSvrConnected = EFalse;
        }
    }

EXPORT_C void CPenInputServerHandler::ActivateUiLayout(TBool aActivate)
    {
    if (iPenInputSvrConnected)
        {
        iPenInputServer.ActivateLayout(aActivate);
        }
    }

EXPORT_C void CPenInputServerHandler::SendCommandToServer(TInt aCmdId)
    {
    if (iPenInputSvrConnected)
        {
        iPenInputServer.HandleCommand(aCmdId);
        }	
    }

EXPORT_C void CPenInputServerHandler::SendCommandToServer(TInt aCmdId, TInt aParam)
    {
    if (iPenInputSvrConnected)
        {
        TPckgC<TInt> msgdata(aParam);
        iPenInputServer.HandleCommand(aCmdId, msgdata);
        }
    }

EXPORT_C void CPenInputServerHandler::SendCommandToServer(TInt aCmdId, const TDesC& aParam)
    {
    if (iPenInputSvrConnected)
        {
        TPtrC ptr(aParam);
        TPckgC<TPtrC> msgdata(aParam);
        iPenInputServer.HandleCommand(aCmdId, msgdata);
        }
    }

EXPORT_C void CPenInputServerHandler::SendNewKeyToServer(TInt aCmdId, const TItutKey& aKeyInfo)
    {
    if (iPenInputSvrConnected)
        {
        TPckgC<TItutKey> msgdata(aKeyInfo);
        iPenInputServer.HandleCommand(aCmdId, msgdata);
        }
    }
    
EXPORT_C void CPenInputServerHandler::SetIcfText(const TFepInputContextFieldData& aData)
    {
    if (iPenInputSvrConnected)
        {
        TPckgC<TFepInputContextFieldData> msgdata(aData);
        iPenInputServer.HandleCommand(EItutExtCmdSetIcfData, msgdata);
        }
    }

EXPORT_C void CPenInputServerHandler::SendPromptTextToServer(TInt aCmdId, 
                                                             const TFepPromptText& aPromptText)
    {
    if (iPenInputSvrConnected)
        {
        TPckgC<TFepPromptText> msgdata(aPromptText);
        iPenInputServer.HandleCommand(aCmdId, msgdata);
        }
    }

// End Of File
