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


#include "peninputcrpmsg.h"
#include "peninputcrpclient.h"

#define SZ(x) (sizeof(x) / sizeof(TInt))
	
CPenInputCrpServiceClient* CPenInputCrpServiceClient::NewL()	
    {
    CPenInputCrpServiceClient* self = new(ELeave) CPenInputCrpServiceClient();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
	
void CPenInputCrpServiceClient::ConstructL()
	{
	const TUid id = {0x2000B5E7};

	BaseConstructL(id, KNullDesC8);
	}	
															 

TBool CPenInputCrpServiceClient::IsDsaApp(TInt aGrpId)
    {
	const TInt msg[] = {
				NPenInputCRPServiceMsg::EIsDsaApp,
				aGrpId
    			};
	TPckgC<TInt[SZ(msg)]> buf(msg);
	
    return  SendSynchronMessage(buf);
    }
    
TBool CPenInputCrpServiceClient::IsDsaActive()
	{
	const TInt msg[] = {
				NPenInputCRPServiceMsg::EIsDsaApp
    			};
	TPckgC<TInt[SZ(msg)]> buf(msg);
	
    return  SendSynchronMessage(buf);		
	}
    
void CPenInputCrpServiceClient::RemoveDsaApp(TInt aGrpId)
    {
	const TInt msg[] = {
				NPenInputCRPServiceMsg::ERemoveDsaApp,
				aGrpId
    			};
	TPckgC<TInt[SZ(msg)]> buf(msg);
	SendMessage(buf);
	Flush();
    
    }
    
void CPenInputCrpServiceClient::HandleMessage(const TDesC8& /*aData*/)
	{
	
	}
    
void CPenInputCrpServiceClient::OnReplace()
	{
	/*empty*/
	}
	
