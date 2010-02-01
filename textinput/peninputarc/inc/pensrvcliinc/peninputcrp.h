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
#ifndef AKNWSSERVICECRP_H
#define AKNWSSERVICECRP_H

#include<w32std.h>
#include<graphics/wsgraphicdrawerinterface.h>
#include<graphics/wsgraphicdrawer.h>


NONSHARABLE_CLASS(CAknWsServiceCRP) : public CWsGraphicDrawer, public MWsGraphicHandleSynchronMessage
	{
public:
	static CWsGraphicDrawer* CreateL();
	~CAknWsServiceCRP();
	CAknWsServiceCRP* iNext; //public access!
	//void SendHandle();
	void AddDsaGrp(TInt aId);
	void SetDsaActive( TBool aIsActive );
private:
	TAny* ResolveObjectInterface(TUint aTypeId);
	TInt HandleSynchronMessage(const TDesC8& aData);
	void DoDraw(MWsGc& aGc, const TRect& aRect, const TDesC8& aData) const;
	void HandleMessage(const TDesC8& aData);
	void ConstructL(MWsGraphicDrawerEnvironment& aEnv, const TGraphicDrawerId& aId, MWsClient& aOwner, const TDesC8& aData);
private:
	void ConstructL();
	void DoHandleMessageL(const TDesC8& aDes);
	//void Snapshot();

    TBool IsDsaApp(TInt aId);
    
    void RemoveDsaApp(TInt aId);
	
private:
    RArray<TInt> iDsaGrpList;
    TBool iIsDsaActive;
	};

#endif
