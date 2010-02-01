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

#include "peninputcrp.h"
#include "peninputcrpmsg.h"
#include <ecom/implementationproxy.h>
#include <s32mem.h> //RDesReadStream

//const TInt KDefaultScreen(0);
const TInt KImpId(0x2000B5E7);

#define INVERTLANDSCAPE

const TInt KMonitoredEvents = TWservCrEvent::EDsaDrawingBegin|TWservCrEvent::EDsaDrawingEnd ;

	
class CAknWsService : public CBase, public MWsEventHandler
	{
	public:
		static void CreateL(MWsGraphicDrawerEnvironment& aEnv, CAknWsServiceCRP* aSrv);
		static void Delete(CAknWsServiceCRP* aSrv);
		static inline CAknWsService* Service();
		~CAknWsService();
		
	private:
		void DoHandleEvent(const TWservCrEvent& aEvent);
		void ConstructL();
		CAknWsService(MWsGraphicDrawerEnvironment& aEnv);
			    
	private:
		MWsGraphicDrawerEnvironment* iEnv;
		CAknWsServiceCRP* iService;		
	};


inline CAknWsService* CAknWsService::Service()
	{
	return static_cast<CAknWsService*> (Dll::Tls());
	}

CAknWsService::CAknWsService(MWsGraphicDrawerEnvironment& aEnv) :
iEnv(&aEnv)
	{
	}

void CAknWsService::ConstructL()	
	{
	}

void CAknWsService::CreateL(MWsGraphicDrawerEnvironment& aEnv, CAknWsServiceCRP* aSrv)
	{
	CAknWsService* srv = Service();
	if(srv == NULL)
		{
		srv = new (ELeave) CAknWsService(aEnv);
		CleanupStack::PushL(srv);
		srv->ConstructL();
		CleanupStack::Pop();
		Dll::SetTls(srv);
		}
	else
		{
		CAknWsServiceCRP* s = srv->iService;
		if(s != NULL)
			{
			while(s->iNext != NULL)
				s = s->iNext;
			s->iNext = aSrv;
			return;
			}
		}
	srv->iService = aSrv;
	aEnv.RegisterEventHandler(aSrv, srv, KMonitoredEvents);	
	}
	
void CAknWsService::Delete(CAknWsServiceCRP* aSrv) 
	{
	CAknWsService* srv = Service();
	if(srv != NULL)
		{
		if(aSrv == srv->iService)
			{
			srv->iEnv->UnregisterEventHandler(aSrv);
			if(aSrv->iNext != NULL)
				{
				srv->iEnv->RegisterEventHandler(aSrv->iNext, srv, KMonitoredEvents);
				}
			else  
				{
				srv->iService = NULL;	
				delete srv;
				Dll::SetTls(NULL);
				return;
				}
			}
			
		CAknWsServiceCRP* s = srv->iService;
		while(s->iNext != aSrv)
			{
			s = s->iNext;
			ASSERT(s != NULL);
			}
		s->iNext = aSrv->iNext;	
		}
	}
	
CAknWsService::~CAknWsService()
	{
	RDebug::Print(_L("QQQ destructor"));
	
	}
	
		
void CAknWsService::DoHandleEvent(const TWservCrEvent& aEvent)
	{
	
    //TInt data = 0;
    if(aEvent.Type() == TWservCrEvent::EDsaDrawingBegin)
        {
        // DSA is active 
       	iService->SetDsaActive(ETrue);
        }
        
    if(aEvent.Type() == TWservCrEvent::EDsaDrawingEnd)
        {
        // DSA is inactive
        iService->SetDsaActive(EFalse);
        }
	
	}
		

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


CWsGraphicDrawer* CAknWsServiceCRP::CreateL()
	{
	CAknWsServiceCRP* crp = new (ELeave) CAknWsServiceCRP();
	return crp;
	}
	

	
CAknWsServiceCRP::~CAknWsServiceCRP()	
	{
	CAknWsService::Delete(this);
    iDsaGrpList.Close();
	}

void CAknWsServiceCRP::DoDraw(MWsGc& /*aGc*/, const TRect& /*aRect*/, const TDesC8& /*aData*/) const
	{
	//draws nothing
	}
	
void CAknWsServiceCRP::HandleMessage(const TDesC8& aData)
	{
	TRAP_IGNORE(DoHandleMessageL(aData));
	}
	
TBool CAknWsServiceCRP::IsDsaApp(TInt aId)
    {
    return iDsaGrpList.Find(aId) != KErrNotFound;
    }

void CAknWsServiceCRP::RemoveDsaApp(TInt aId)
    {
    TInt idx = iDsaGrpList.Find(aId);
    if(KErrNotFound != idx)
        iDsaGrpList.Remove(idx);
    }
TInt CAknWsServiceCRP::HandleSynchronMessage(const TDesC8& aData)
	{
	RDesReadStream in(aData);
	TInt cmd = 0;
	TRAP_IGNORE( cmd = in.ReadInt32L()); 
//	RDebug::Printf("QQQ HandleSynchronMessage %d", cmd);
	switch(cmd)
		{
	    case NPenInputCRPServiceMsg::EIsDsaApp:
	        {
            return iIsDsaActive;	        
	        }
	    default:
	        break;
		}
	return 0;
	}
	
void CAknWsServiceCRP::DoHandleMessageL(const TDesC8& aData)
	{
	RDesReadStream in(aData);
	const TInt cmd = in.ReadInt32L(); 
//	RDebug::Printf("QQQ DoHandleMessageL %d", cmd);
	switch(cmd)
		{
	    case NPenInputCRPServiceMsg::ERemoveDsaApp:
            {
            TInt id = in.ReadInt32L(); 
            RemoveDsaApp(id);
            }
	        break;
        default:		        
			break;
		} 	
	}
	
	
void CAknWsServiceCRP::ConstructL(MWsGraphicDrawerEnvironment& aEnv,
 const TGraphicDrawerId& aId, MWsClient& aOwner, const TDesC8& /*aData*/)
	{
	BaseConstructL(aEnv, aId, aOwner);
	CAknWsService::CreateL(aEnv, this);
	}
	
	

void CAknWsServiceCRP::AddDsaGrp(TInt aId)
    {
    if(KErrNotFound == iDsaGrpList.Find(aId))
        {
        iDsaGrpList.Append(aId);
        }
    }
    
void CAknWsServiceCRP::SetDsaActive( TBool aIsActive )
	{
	iIsDsaActive = aIsActive;
	}
    
TAny* CAknWsServiceCRP::ResolveObjectInterface(TUint aTypeId)
	{
	if(aTypeId == KWsGraphicHandleSynchronMessageId)
		{
		TAny* interface = NULL;
		interface = static_cast<MWsGraphicHandleSynchronMessage*>(this);
		return interface;
		}
	return CWsGraphicDrawer::ResolveObjectInterface(aTypeId);
	}	
    
    
LOCAL_C const TImplementationProxy KImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KImpId, CAknWsServiceCRP::CreateL)
	};
	
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = (sizeof(KImplementationTable) / sizeof(TImplementationProxy));
	return KImplementationTable;
	}

