/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface to CRP
*
*/


#ifndef R_RPENINPUCRP_H
#define R_RPENINPUCRP_H

#include <w32std.h>

class CPenInputCrpServiceClient : public CWsGraphic
	{
	public:
		//CPenInputCrpServiceClient();
		//~CPenInputCrpServiceClient();
        static CPenInputCrpServiceClient* NewL();		
        TBool IsDsaApp(TInt aGrpId);
        TBool IsDsaActive();
        void RemoveDsaApp(TInt aGrpId);
    private:
        void ConstructL();        
        void HandleMessage(const TDesC8& /*aData*/);
        void OnReplace();
	};
	
#endif //R_RPENINPUCRP_H