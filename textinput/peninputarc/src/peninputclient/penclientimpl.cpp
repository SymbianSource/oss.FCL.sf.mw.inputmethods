/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for peninput client
*
*/



#include "peninputclientserver.h"
#include "peninputclientimpl.h"
#include "peninputcmd.h"
#include "peninputsrvobserver.h"
#include <apgtask.h>
#include <w32std.h>
#include <apacmdln.h>
#include <apgcli.h>
#include <s32mem.h>
#include <coemain.h>
#include <apgwgnam.h>
#include "peninputclient.h"
const TUint KDefaultMessageSlots = 4;

const TInt KMaxSupportLanguages = 100;
const TInt KMaxImeImplementations = 10;
const TUid KSingletonClientId = 
    {
    0x10281854
    };


TUid GetAppUidByWndGroupIdL( RWsSession &aWs, TInt aWndGroupId )
    {
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC(aWs,aWndGroupId);
    TUid id = wg->AppUid();
    CleanupStack::PopAndDestroy(wg);    	
    return id;
    }
TUid AppUidFromWndGroupIdL(TInt aWndGrpId)
    {
    RWsSession &ws = CCoeEnv::Static()->WsSession();
    //TInt wgId =ws.GetFocusWindowGroup();
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC(ws,aWndGrpId);

    TUid id = wg->AppUid();
    CleanupStack::PopAndDestroy(wg);    	
    return id;
    
    }
TUid AppUidFromWndGroupId(TInt aWndGrpId)
    {
    TUid id = {0x00000000};
    TRAP_IGNORE(id = AppUidFromWndGroupIdL(aWndGrpId));
    return id;
    }

inline TUid GetFocusAppUid()
    { 
    return  AppUidFromWndGroupId(CCoeEnv::Static()->WsSession().GetFocusWindowGroup());
    }
TUid GetCurAppUid()
    {
    return  AppUidFromWndGroupId(CCoeEnv::Static()->RootWin().Identifier());
    }
    
    
   
CPeninputServerWaiter* CPeninputServerWaiter::NewL()
    {
    CPeninputServerWaiter* self = new(ELeave)CPeninputServerWaiter;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
void CPeninputServerWaiter::ConstructL()    
    {
    iWaitScheduler = new(ELeave) CActiveSchedulerWait;
    }
    
CPeninputServerWaiter::~CPeninputServerWaiter()
    {
    delete iWaitScheduler;
    }
    
void CPeninputServerWaiter::Start()
    {
    iWaitScheduler->Start();
    }
    
void CPeninputServerWaiter::Stop(TInt aFlag)
    {
    //if(aFlag)
    iError = aFlag ? KErrNone : aFlag;
    
    iWaitScheduler->AsyncStop();
    }
    
    
    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// RPeninputServerImpl::NewL
// ---------------------------------------------------------------------------
//
RPeninputServerImpl* RPeninputServerImpl::NewL()                                  
    {
    //check for singleton existence    
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(KSingletonClientId);
    CPenInputSingletonClient* client = 
                                static_cast<CPenInputSingletonClient*>(obj);
    if(client && client->IsValid())
        {
        RPeninputServerImpl* server = client->GetSingletonServer();
        return server;
/*        if(iSingletonServer == 0 ) // second or later connecting from same client
            {
            error = KErrLaunchingServer;
            }*/
        }
        
    RPeninputServerImpl* self = new(ELeave) RPeninputServerImpl();
    CleanupStack::PushL(self);
    self->ConstructL(KSingletonClientId);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::RPeninputServerImpl
// ---------------------------------------------------------------------------
//
RPeninputServerImpl::RPeninputServerImpl() : iPosition(0,0),iSize(0,0)
    {
    iObserver = 0;
    iIsForegroundSession = EFalse;
    iServerExit = EFalse;
    iLaunchServer = EFalse;
    iWaitScheduler = 0;
    iCurPenUiType = -1; 
    iWaitScheduler = 0; 
    iAppPrefferedUiMode = EPluginInputModeNone;
    iAutoOpenFlag = ETrue;
    iBackgroundCtrl = 0;
    iResourceChange = EFalse;  
    }

void RPeninputServerImpl::ConstructL(const TUid& aUid)
    {
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(aUid);
    CPenInputSingletonClient* client;
    //__ASSERT_ALWAYS(obj == NULL, User::Leave(KErrAlreadyExists));
    if(obj)
        {
        client = static_cast<CPenInputSingletonClient*>(obj);
        client->ReplaceServer(this);                                
        }
    else
        client = new (ELeave)CPenInputSingletonClient(aUid,this);       
    //Note: The client is destroyed by the control environment automatically    
    //User::LeaveIfError(DoConnectL());
    TRAPD(err, DoConnectL());
    if(KErrNone != err)
        {
        client->SetInvalid();
        User::LeaveIfError(err);
        }
    }
    

// ---------------------------------------------------------------------------
// RPeninputServerImpl::DoConnectL
// ---------------------------------------------------------------------------
//
/*TInt RPeninputServerImpl::Connect()
    {
    //do nothing
    return KErrNone;
    }
  */  
// ---------------------------------------------------------------------------
// RPeninputServerImpl::Close
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::Close()
    {
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(KSingletonClientId);
    CPenInputSingletonClient* client = 
                                static_cast<CPenInputSingletonClient*>(obj);               
    if(client)
        {
        TInt ref = client->DecreaseRef();
        if (ref <= 0)
            {
            // delete the CCoeStatic object of CPenInputSingletonClient when 
            // Peninput server was closed.
            // Basiccaly, all CCoeStatic objects are deleted in CCoeEnv destructor.
            // However, it is necessary to delete the CCoeStatic object when 
            // changing from avkon fep to other fep.
            delete client;
            }
        }
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::DoConnectL
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::DoConnectL()
    { 
    TInt error = KErrNone;
    /*************** Note ************************************************/
    // Multi-thread case is not considered here!
    /*************** Note ************************************************/        
    
     // try to connect to the server
    error = CreateSession( KPeninputServerName,
                           Version(),
                           KDefaultMessageSlots,EIpcSession_Sharable);

    if( error == KErrNotFound || error == KErrServerTerminated )
        {
        // server does not yet exist or it has terminated
        // try to create the server
        error = StartThreadL();

        if ( error == KErrNone || error == KErrAlreadyExists )
            {
            // creation of server successfull, or some other client thread 
            // created it first -> try to connect again
            error = CreateSession( KPeninputServerName,
                           Version(),
                           KDefaultMessageSlots );
            }
        else
            {
            return error;
            }
        }

    TThreadId srvThreadId;
    if(KErrNone == error)
        {
        TIpcArgs arg;        
    
        TPckg<TThreadId> msg(srvThreadId);
        arg.Set(KMsgSlot0,&msg);    
        SendReceive(EPeninputRequestServerThreadId,arg);
        }
    
    iServerExit = EFalse;
	//create default observer
    /*CPeninputServerObserver* observer = CPeninputServerObserver::NewL(this,
                                                       srvThreadId,iObserver);*/
    if(iObserver)
        {
        iObserver->ReConstructL(srvThreadId);
        }
    else
        {
        iObserver = CPeninputServerObserver::NewL(this,srvThreadId);
        }

    
    TFindThread findPeninputServer( _L("*peninputserver*") );
    TFullName name;
    if (findPeninputServer.Next( name ) == KErrNone)
        {
        // if server is already running return immediately
        //return KErrAlreadyExists;
        RThread p;
        TInt r = p.Open(name);
        p.Close();
        } 
    
    return error;
    }


// ---------------------------------------------------------------------------
// RPeninputServerImpl::Version
// ---------------------------------------------------------------------------
//
TVersion RPeninputServerImpl::Version(void) const
    {
    return( TVersion( KPeninputServerMajorVersionNumber,
                      KPeninputServerMinorVersionNumber,
                      KPeninputServerBuildVersionNumber ) );
    }


// ---------------------------------------------------------------------------
// RPeninputServerImpl::RequestMessageNotification
// Register to require message notification
// ---------------------------------------------------------------------------
//

void RPeninputServerImpl::RequestMessageNotification( TRequestStatus& aStatus )
    {
    TInt data=0;
    SendReceive( EPeninputServerRequestMessageNotification, TIpcArgs(data), aStatus ); 
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::CancelMessageNotification
// Cancel message notification
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::CancelMessageNotification()
    {
    SendReceive( EPeninputServerCancelMessageNotification);   
    }


void RPeninputServerImpl::DimUiLayout(TBool aFlag)
    {
    TRAP_IGNORE(DimUiLayoutL(aFlag));
    }

void RPeninputServerImpl::DimUiLayoutL(TBool aFlag)
    {
    TInt priority = iPriority;

    TPckgC<TBool> msg(aFlag);
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,&msg);

    TPckgC<TInt> prioriryMsg(priority);//p
    arg.Set(KMsgSlot1,&prioriryMsg);
    
    TInt bmpHandle = -1;
    TPckg<TInt> handleMsg(bmpHandle);
    arg.Set(KMsgSlot2,&handleMsg);
    
    TRect extend;
    TPckg<TRect> posMsg(extend);  
    arg.Set(KMsgSlot3,&posMsg);
    
    if (iInternalPopup)
        {
        SendReceive(EPeninputRequestInternalDimLayout,arg);    
        }
    else
        {
        SendReceive(EPeninputRequestDimLayout,arg);    
        }
   
    if(aFlag)
        {
        //show the background control
        if(!iBackgroundCtrl)
            {
            iBackgroundCtrl = CPenUiBackgroundWnd::NewL(CCoeEnv::Static()->RootWin(),bmpHandle);
            }
        if(iBackgroundCtrl)
            iBackgroundCtrl->Show(extend, iGlobalNotes, 
                iInternalPopup,priority, iResourceChange);    
        }
    else //undim the window
        {
        if(iBackgroundCtrl)
            iBackgroundCtrl->Hide();    
        }
    }
// ---------------------------------------------------------------------------
// RPeninputServerImpl::ActivateLayout
// Activate/Deactivate a ui layout
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::ActivateLayout(TBool aFlag)
    {
    if(iBackgroundCtrl && iBackgroundCtrl->IsVisible())
        iBackgroundCtrl->Hide();    
    
    //inform UI activation handler
    for(TInt i = 0 ; i < iPenUiNotificationHandler.Count(); ++i)
        {
        if(iPenUiNotificationHandler[i].iType & iCurPenUiType)
            {
            if(aFlag)
                iPenUiNotificationHandler[i].iHandler->OnPeninputUiActivated();
            else
                iPenUiNotificationHandler[i].iHandler->OnPeninputUiDeactivated();
            }
        }
    
    if ( aFlag || ( !aFlag && IsVisible() ) )        
        {
        TPckgC<TBool> msg(aFlag);
        TIpcArgs arg;    
        arg.Set(KMsgSlot0,&msg);     
        
        TInt scrMode = CCoeEnv::Static()->ScreenDevice()->CurrentScreenMode();
        TPckgC<TInt> scrMsg(scrMode);         
        arg.Set(KMsgSlot1,&scrMsg);
        SendReceive(EPeninputRequestActivateLayout,arg);
        }           
    }

void RPeninputServerImpl::ActivatePeninputInNotesL()
    {
    SendReceive(EPeninputRequestActivateLayoutInGlobalNotes);    
    }
// ---------------------------------------------------------------------------
// RPeninputServerImpl::GetServerEventData
// Retrieve server event data
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::GetServerEventData(TDes& aBuf)
    {
    if(iServerExit)  // ???
      return -1;
    
    TIpcArgs arg;
    aBuf.Zero();    
    arg.Set(KMsgSlot0,aBuf.MaxLength());
    arg.Set(KMsgSlot1,&aBuf);
    return SendReceive(EPeninputRequestGetServerEventData,arg);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::ServerEventDfataNum
// Get the number of server event
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::ServerEventDataNum()
    {  
    TInt num = -1;
    TPckg<TInt> msg(num);
    TIpcArgs arg;
    arg.Set(KMsgSlot0,&msg);
    TInt err = SendReceive(EPeninputRequestGetServerEventDataNumber,arg);
    if(err != KErrNone)
        num = err;//set the error code
    return num;
    
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::ResourceChanged
// Tell server the client resource changed
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::ResourceChanged(TInt aType)
    {
    TIpcArgs arg;    
    TPckgC<TInt> msg(aType);
    arg.Set(KMsgSlot0,&msg);
    
    SendReceive(EPeninputRequestResourceChanged,arg);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetUiLayoutPos
// Set ui layout position
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::SetUiLayoutPos(const TPoint& aPos)
    {
    TPckgC<TPoint> msg(aPos);
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,&msg);
    return SendReceive(EPeninputRequestUiSetUiPos,arg);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::UiLayoutPos
// get ui layout position
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::GetUiLayoutPosAndSize()
    {
    TPckg<TPoint> msgPos(iPosition);
    TPckg<TSize> msgSize(iSize);
    
    TIpcArgs arg;
    arg.Set(KMsgSlot0,&msgPos);
    arg.Set(KMsgSlot1,&msgSize);
    SendReceive(EPeninputRequestUiGetUiPos,arg);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::UiLayoutPos
// get ui layout position
// ---------------------------------------------------------------------------
//
TPoint RPeninputServerImpl::UiLayoutPos()
    {
    GetUiLayoutPosAndSize();
    return iPosition;    
    }

// ---------------------------------------------------------------------------
// RPeninputServer::UiLayoutSize
// get ui layout size
// ---------------------------------------------------------------------------
//
TSize RPeninputServerImpl::UiLayoutSize()
    {
    GetUiLayoutPosAndSize();
    return iSize;    
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetUiLayoutId
// Create a ui layout
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::SetUiLayoutId(TUid aId)
    {
    TInt uid = aId.iUid;    

    TPckgC<TInt> msg(uid);
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,&msg);
    
    TPckg<TInt> type(iCurPenUiType);
    arg.Set(KMsgSlot2,&type);
    return SendReceive(EPeninputRequestSetUiLayoutId,arg);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetUiLayoutId
// Create a ui layout
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::SetUiLayoutId(TUid aId,const TDesC8& aData)
    {
    TInt uid = aId.iUid;

    TPckgC<TInt> idMsg(uid);
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,&idMsg);    
    arg.Set(KMsgSlot1,&aData);
    
    TPckg<TInt> type(iCurPenUiType);
    arg.Set(KMsgSlot2,&type);
    
    return SendReceive(EPeninputRequestSetUiLayoutIdWithData,arg);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::HandleCommand
// Ask ui layout handle command
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::HandleCommand(TInt aCmd,const TDesC8& aBuf)
    {
    TInt ret;
    return HandleCommand(aCmd,aBuf,ret);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::HandleCommand
// Ask ui layout handle command
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::HandleCommand(TInt aCmd)
    {
    TInt ret;
    return HandleCommand(aCmd,KNullDesC8,ret);       
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::HandleCommand
// Ask ui layout handle command
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::HandleCommand(TInt aCmd,const TDesC8& aBuf, TInt &aResult)
    {
    //if(iSingletonServer != this)
      //  return iSingletonServer->HandleCommand(aCmd,aBuf, aResult);
    
    if(!IsForegroundSession()) // not handle non-foregound session command
        return EFalse;
    
    TIpcArgs arg;    
    TPckgC<TInt> cmdMsg(aCmd);    
    arg.Set(KMsgSlot0,&cmdMsg);   
    
    arg.Set(KMsgSlot1,&aBuf);
    
    TPckg<TInt> retMsg(aResult);
    arg.Set(KMsgSlot2,&retMsg);
    
    return SendReceive(EPeninputRequestHandleClientCommand,arg);
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::IsVisible
// ---------------------------------------------------------------------------
//
TBool RPeninputServerImpl::IsVisible()
    {
    TBool bVisible;
    TIpcArgs arg;        
    
    TPckg<TBool> msg(bVisible);
    arg.Set(KMsgSlot0,&msg);    
    SendReceive(EPeninputRequestUiIsVisible,arg);

    return bVisible;
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::IsVisible
// ---------------------------------------------------------------------------
//
TBool RPeninputServerImpl::IsDimmed()
    {
    TBool bDimmed;
    TIpcArgs arg;        
    
    TPckg<TBool> msg(bDimmed);
    arg.Set(KMsgSlot0,&msg);    
    SendReceive(EPeninputRequestIsLayoutDimmed,arg);

    return bDimmed;
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::IsDimmed
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::SupportInputMode()
    {
    TInt supportMode = 0;
    TIpcArgs arg;        
    
    TPckg<TInt> msg(supportMode);
    arg.Set(KMsgSlot0,&msg);    
    SendReceive(EPeninputRequestSupportInputMode,arg);

    return supportMode;
    }    
    
void RPeninputServerImpl::BackgroudDefaultOri( TInt aOri )   
    {
    TIpcArgs arg;    
    TPckgC<TInt> msg(aOri);
    arg.Set(KMsgSlot0,&msg);
    
    SendReceive(EPeninputBackgroudDefaultOri,arg);
    }
        
// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetAppBackgroundL
// Set server running in background
// ---------------------------------------------------------------------------
//
/*void RPeninputServerImpl::SetAppBackgroundL(RProcess& aServer)
    {
    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetDocumentNameL(KNullDesC);
    commandLine->SetExecutableNameL(KPeninputServerExe);
    //if (aType == EBackgroundApplicationType )
    commandLine->SetCommandL( EApaCommandBackground );

    commandLine->SetProcessEnvironmentL(aServer);
    CleanupStack::PopAndDestroy(commandLine);
    
    }
  */  
// ---------------------------------------------------------------------------
// RPeninputServerImpl::StartThread
// Creates the server thread on WINS and server process on MARM.
// Access to the thread/process creation is controlled with 
// a global mutex which allows only one client thread to do
// the actual server creation in WINS. In MARM the creation of
// new server exits with KErrAlreadyExits if another thread
// already created it.
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::StartThreadL()
    {
    if(iWaitScheduler)
        return KErrLaunchingServer;
    TInt ret = KErrNone;

    // check if server already exists
    TFindServer findPeninputServer( KPeninputServerName );
    TFullName name;
    if (findPeninputServer.Next( name ) == KErrNone)
        {
        // if server is already running return immediately
        return KErrAlreadyExists;
        } 
    // request status which gets signaled when server has been initialized

    RProcess server;
    User::LeaveIfError(server.Create(KPeninputServerExe,KNullDesC()));
    TRequestStatus status;            
    //User::WaitForRequest(status);
    
    
    iObserver = CPeninputServerObserver::NewL(this,
                                               0);
    server.Rendezvous(iObserver->RequestStatus());
    server.Resume();
    server.Close();
                                               
    iWaitScheduler = CPeninputServerWaiter::NewL(); 
    iWaitScheduler->Start();
    ret = iWaitScheduler->Error();
    
    return ret;
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::AddPeninputServerObserverL
// Add observer
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::AddPeninputServerObserverL(
                                       MPeninputServerEventHandler* aHandler)
    {
    //can only set once
    iObserver->AddEventHandler(aHandler);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::RemovePeninputServerObserver
// Remove observer
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::RemovePeninputServerObserver()
    {
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(KSingletonClientId);
    if ( !obj )    
        {
        return;
        }    
    delete iObserver;
    iObserver = NULL;
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::RemovePeninputServerObserver
// Remove observer
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::RemovePeninputServerObserver(
                                       MPeninputServerEventHandler* /*aHandler*/)
    {
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(KSingletonClientId);
    if ( !obj )    
        {
        return;
        }    
    delete iObserver;
    iObserver = NULL;
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetDisplayMode
// Set the display mode
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::SetDisplayMode(TDisplayMode aDisplayMode,
                                                TDisplayMode aMaskDisplayMode)
    {
    TPckgC<TDisplayMode> msg(aDisplayMode);
    TPckgC<TDisplayMode> msg2(aMaskDisplayMode);
    
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,&msg);
    arg.Set(KMsgSlot1,&msg2);
    
    SendReceive( EPeninputServerRequestSetDisplayMode, arg); 
    }

 
// ---------------------------------------------------------------------------
// RPeninputServerImpl::ClearServerEvent
// Set the display mode
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::ClearServerEvent()
    {         
    
    if(iObserver->IsActive())
        {
        TInt data=0;
        SendReceive(EPeninputServerClearServerEvent, TIpcArgs(data) );
        }    
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::AfterUiNotification
// Tell server Ui notification has been processed.
// ---------------------------------------------------------------------------
//
/*void RPeninputServerImpl::Continue()
    {
    iSingletonServer->SendReceive(EPeninputRequestUiNotificationCompleted);
    }
  */
// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetDisplayMode
// To disable specified layouts at a time.
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::SetDisabledLayout( TInt aLayouts )
    {
    
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,aLayouts);
    
    SendReceive( EPeninputRequestDisableLayout, arg );
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetDisplayMode
// To disable specified layouts at a time.
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::DisabledLayout()
    {
    TInt layouts = 0;
    TIpcArgs arg;        
    
    TPckg<TInt> msg(layouts);
    arg.Set(KMsgSlot0,&msg); 
    
    SendReceive( EPeninputRequestGetDisableLayout, arg );
    return layouts;
    }    
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::GetImePluginIdListL
// Get IME plugin list for a language
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::GetImePluginIdListL(TInt aLanguage, 
                                                  TInt aPluginMode, 
                                                  RArray<TInt>& aImplmentationIds)
    {
    TInt size = sizeof(TInt) * (KMaxImeImplementations + 1);
	HBufC8* buf = HBufC8::NewLC(size);
	TPtr8 bufPtr = buf->Des();

    TIpcArgs args;
    args.Set(KMsgSlot0, aLanguage);
    args.Set(KMsgSlot1, aPluginMode);
    args.Set(KMsgSlot2, size);
    args.Set(KMsgSlot3, &bufPtr);    
    aImplmentationIds.Reset();
    TInt err = SendReceive(EPenInputRequestGetImePluginIdList,args);
    if( err == KErrNone )
        {
        ReadIntArrayFromBufL(*buf, aImplmentationIds);
        }
    
    CleanupStack::PopAndDestroy(buf);    
    }
  
// ---------------------------------------------------------------------------
// RPeninputServerImpl::PenSupportLanguagesL
// Get pen supported langauge
// ---------------------------------------------------------------------------
//                                 
void RPeninputServerImpl::PenSupportLanguagesL(RArray<TInt>& aLanguageLists)
    {
    TInt size = sizeof(TInt) * (KMaxSupportLanguages + 1);
	HBufC8* buf = HBufC8::NewLC(size);
	TPtr8 bufPtr = buf->Des();

    TIpcArgs args;
    args.Set(KMsgSlot0, size);
    args.Set(KMsgSlot1, &bufPtr);
    aLanguageLists.Reset();
    TInt err = SendReceive(EPenInputRequestGetPenSupportLanguages,args);
    if( err == KErrNone )
        {
        TRAP(err, ReadIntArrayFromBufL(*buf, aLanguageLists));
        }
    
    CleanupStack::PopAndDestroy(buf);    
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::ReadIntArrayFromBufL
// Read buffer
// ---------------------------------------------------------------------------
//
void RPeninputServerImpl::ReadIntArrayFromBufL(const TDesC8& aBuf, RArray<TInt>& aResult)
    {
	RDesReadStream readStream;
	readStream.Open(aBuf);
	CleanupClosePushL(readStream);
	const TInt entryCount = readStream.ReadInt32L();

	for(TInt i = 0; i < entryCount; ++i)
		{
		aResult.AppendL(readStream.ReadInt32L());
		}
		
    CleanupStack::PopAndDestroy(&readStream);
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetForeground
// Set current session to be foreground application session
// ---------------------------------------------------------------------------
//
TBool RPeninputServerImpl::SetForeground(TBool aMustConnectFlag)
    {
 //   if(iIsForegroundSession)
 //       return ETrue;
    
    TInt curAppId;
    TRAP_IGNORE( curAppId = GetAppUidByWndGroupIdL(CCoeEnv::Static()->WsSession(),
                        CCoeEnv::Static()->RootWin().Identifier()).iUid);
    TIpcArgs arg;        
    TPckgC<TInt> idData(curAppId);
    arg.Set(KMsgSlot0,&idData); 

    TPckgC<TBool> flagMsg(aMustConnectFlag);    
    arg.Set(KMsgSlot1,&flagMsg); 
    
    TPckg<TInt> retMsg(iIsForegroundSession);
    arg.Set(KMsgSlot2,&retMsg);
    
    SendReceive(EPeninputRequestSetForeground,arg);
    return iIsForegroundSession;
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::LoseForeground
// Indicates current client lost foreground
// ---------------------------------------------------------------------------
//    
void RPeninputServerImpl::LoseForeground()
    {
    iIsForegroundSession = EFalse;    
    SendReceive(EPeninputRequestRelinquishForeground);    
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::IsForeground
// Test whether this session is the forground application session
// ---------------------------------------------------------------------------
//
TBool RPeninputServerImpl::IsForeground()
    {
    TBool bIsForeground;
    TIpcArgs arg;        
    
    TPckg<TBool> msg(bIsForeground);
    arg.Set(KMsgSlot0,&msg);    
    SendReceive(EPeninputRequestIsForeground,arg);
    //__ASSERT_DEBUG(bIsForeground == iIsForegroundSession,User::Leave(-1));    
    return bIsForeground;
    
    }

/*void RPeninputServerImpl::DoAddPenUiActivationHandler()
    {
    TPckgC<TInt> msg(iPenUiNotificationHandler->RegisteredType());
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,&msg);         
    
    iSingletonServer->SendReceive(EPeninputRequestAddUiObserver,arg);        
    
    }
  */  
void RPeninputServerImpl::OnServerReady( TBool aFlag)    
    {
    //iLaunchServer = EFalse;
    
    iWaitScheduler->Stop(aFlag);//AsyncStop();
    //if(iPenUiNotificationHandler)
      // DoAddPenUiActivationHandler(); 
    }
// ---------------------------------------------------------------------------
// RPeninputServerImpl::AddPenUiActivationHandler
// Add an UI activate/deactivation handler
// ---------------------------------------------------------------------------
//
TInt RPeninputServerImpl::AddPenUiActivationHandler(
                      MPenUiActivationHandler* aHandler,TInt aType)
    {
    iPenUiNotificationHandler.Append(TUiNotificationHandler(aHandler,aType));
    return KErrNone;            
    }
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::RemovePenUiActivationHandler
// Remove all UI activate/deactivation handler
// ---------------------------------------------------------------------------
//    
void RPeninputServerImpl::RemovePenUiActivationHandler()
    {
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(KSingletonClientId);
    if ( !obj )    
        {
        return;
        }
    iPenUiNotificationHandler.Reset();        
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::RemovePenUiActivationHandler
// Remove an UI activate/deactivation handler
// Deprecated API
// ---------------------------------------------------------------------------
//    
void RPeninputServerImpl::RemovePenUiActivationHandler(MPenUiActivationHandler 
                                                                    *aHandler)
    {
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(KSingletonClientId);
    if ( !obj )    
        {
        return;
        }
    for(TInt i = iPenUiNotificationHandler.Count() - 1; i >= 0 ; --i)
        {
        if(aHandler == iPenUiNotificationHandler[i].iHandler)
            {
            iPenUiNotificationHandler.Remove(i);
            }
        }
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::UpdateAppInfo
// Update current application information
// ---------------------------------------------------------------------------
//   
void RPeninputServerImpl::UpdateAppInfo(const TDesC& aInfo, TPeninputAppInfo aType)
    {
    TPckgC<TPeninputAppInfo> typeMsg(aType);
    TIpcArgs arg;    
    arg.Set(KMsgSlot0,&typeMsg);    
    arg.Set(KMsgSlot1,&aInfo);
    SendReceive(EPeninputRequestUpdateAppInfo,arg);    
    }
    
void RPeninputServerImpl::HandleServerExit()
    {
    iServerExit = ETrue;
    }
    
TBool RPeninputServerImpl::IsForegroundSession()
    {
    return iIsForegroundSession;
    }

void RPeninputServerImpl::FinalClose()
    {
    delete iBackgroundCtrl;  
      
    delete iWaitScheduler;
    iWaitScheduler = 0;
    iPenUiNotificationHandler.Close();
    delete iObserver;
    iObserver = NULL;
    
    RSessionBase::Close();    
    }
    
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetAutoOpen
// Enable/disable the auto-open feature
// ---------------------------------------------------------------------------
//   
void RPeninputServerImpl::SetAutoOpen(TBool aFlag)
	{
	iAutoOpenFlag = aFlag;
	}

// ---------------------------------------------------------------------------
// RPeninputServerImpl::AutoOpen
// get the the auto-open feature flag
// ---------------------------------------------------------------------------
//     
TBool RPeninputServerImpl::AutoOpen()    
	{
	return iAutoOpenFlag;
	}
    
    
// ---------------------------------------------------------------------------
// RPeninputServerImpl::SetPrefferredUiMode
// Set the app preferred pen ui mode.
// ---------------------------------------------------------------------------
//     
void RPeninputServerImpl::SetPreferredUiMode(TPluginInputMode aMode)    
    {
	iAppPrefferedUiMode = aMode;
    }

// ---------------------------------------------------------------------------
// RPeninputServerImpl::PrefferredUiMode
// Get the app preferred pen ui mode.
// ---------------------------------------------------------------------------
// 
TPluginInputMode RPeninputServerImpl::PreferredUiMode()
    {
	return iAppPrefferedUiMode;
    }
    
void RPeninputServerImpl::SetGlobalNotes(TBool aFlag)
    {
    iGlobalNotes = aFlag;    
    }

void RPeninputServerImpl::SetInternalPopUp(TBool aFlag)
    {
	iInternalPopup = aFlag;
    } 
    
void RPeninputServerImpl::SetEditorPriority(TInt aFlag)
    {
	iPriority = aFlag;
    }     

 void  RPeninputServerImpl::ClearTouchUI()
    {
    if(iBackgroundCtrl)
        {
        iBackgroundCtrl->Hide();
        iBackgroundCtrl->MakeVisible(EFalse);    
        }
    }
    
void RPeninputServerImpl::SetResourceChange(TBool aFlag)
    {
    iResourceChange = aFlag; 
    
    
    TBool bChange = aFlag;
    TIpcArgs arg;        
    
    TPckg<TBool> msg(bChange);
    arg.Set(KMsgSlot0,&msg);    
    SendReceive(EPeninputRequestDimResChangeLayout,arg);       
    }
    
//end of class RPeninputServerImpl


// Class CPeninputServerObserver

CPeninputServerObserver* CPeninputServerObserver::NewL(
                                        RPeninputServerImpl* aServer,
										TThreadId aSrvThreadId)
    {
    CPeninputServerObserver* self = new(ELeave) CPeninputServerObserver(aServer,
                                                                aSrvThreadId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;                                                                
    }
// ---------------------------------------------------------------------------
// CPeninputServerObserver::CPeninputServerObserver
// Constructor. Adds the active object to the active scheduler
// and issues a request for server notification.
// ---------------------------------------------------------------------------
//
CPeninputServerObserver::CPeninputServerObserver(
                                        RPeninputServerImpl* aPeninputServer,
										TThreadId aSrvThreadId)
                                        :CActive( EPriorityHigh ),
                                         iPeninputServer( aPeninputServer ),
                                         iSrvThreadId(aSrvThreadId),
                                         iServerExit(EFalse)                
    {
    __ASSERT_ALWAYS( !IsActive(), User::Panic( KPeninputServerName, 
                                                KErrObserverAlreadyActive ) );
    //__ASSERT_DEBUG(aHandler, User::Panic( KPeninputServerName, 
      //                                              KErrObserverNoHandler ));

    AddEventHandler(this); //set own as the dummy handler
    /*if(aPrevObserver)
        {
        iHandler =  aPrevObserver->iHandler;
        iUiActivationHandler =  aPrevObserver->iUiActivationHandler;
        }*/
    // issue asynchronous request and set this object active
    CActiveScheduler::Add( this );
    }

void CPeninputServerObserver::ConstructL()
    {
    if(iPeninputServer->Handle() > 0) //valid one
        iPeninputServer->RequestMessageNotification(iStatus);
    SetActive();
    }

void CPeninputServerObserver::ReConstructL(TThreadId aId)
    {
    iSrvThreadId = aId;
    if(IsActive())
        {
        Cancel(); 
        }
    ConstructL();
    }
// ---------------------------------------------------------------------------
// CPeninputServerObserver::~CPeninputServerObserver
// Destructor. Cancels any outstanding requests
// ---------------------------------------------------------------------------
//
CPeninputServerObserver::~CPeninputServerObserver()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// CPeninputServerObserver::DoCancel
// Cancels the notification requests
// ---------------------------------------------------------------------------
//
void CPeninputServerObserver::DoCancel()
    {
    if(!iServerExit)
        iPeninputServer->CancelMessageNotification();
    }


// ---------------------------------------------------------------------------
// CPeninputServerObserver::RunL
// ---------------------------------------------------------------------------
//
void CPeninputServerObserver::RunL()
    {
    
    //iHandler is always there. No need to check it
    TInt ret = EFalse;
    
    if(iStatus.Int() < 0)
        {
        //check server status
        RThread srvThread;
        TInt err = srvThread.Open(iSrvThreadId);
        TInt exitReason = 0;
        if(err == KErrNone) //thread is just closed
            {
            exitReason = srvThread.ExitReason();        
            srvThread.Close();
            iPeninputServer->OnServerReady(-1000);
            }
        if(err != KErrNone || exitReason != 0) //server has exited
            {
            iServerExit  = ETrue;
            iPeninputServer->HandleServerExit();             
            iHandler->HandleServerEventL(ESignalServerExit);
            return;
            }
        }

    if(iStatus.Int() == ESignalServerReady) //server has started
        {
        iPeninputServer->OnServerReady();
        return;
        }
    //if there is also iUiActivationHandler, handle it first
    if(iUiActivationHandler)
    	ret = iUiActivationHandler->HandleServerEventL(iStatus.Int());
    
    if(!ret)
    	ret = iHandler->HandleServerEventL(iStatus.Int());
    // re-issue request if request
    if(ret)
        {        
        iPeninputServer->RequestMessageNotification(iStatus);
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CPeninputServerObserver::RunError
// ---------------------------------------------------------------------------
//
TInt CPeninputServerObserver::RunError(TInt /*aError*/)
    {    
    // re-issue request
    iPeninputServer->RequestMessageNotification(iStatus);
    SetActive();

    return KErrNone;
    }

void CPeninputServerObserver::AddEventHandler(MPeninputServerEventHandler* aHandler)
	{
//	if(iHandler)
//		return;
	iHandler = aHandler;
	}

void CPeninputServerObserver::AddUiActivationHandler(MPeninputServerEventHandler* aHandler)
	{
	iUiActivationHandler = aHandler;
	}

TBool CPeninputServerObserver::HandleServerEventL(TInt /*aEventId*/)
    {
    //consume all events
    return ETrue;
    }
    

TUiNotificationHandler::TUiNotificationHandler(
                                MPenUiActivationHandler* aHandler,TInt aType)
                                :iHandler(aHandler),iType(aType)
    {
    }

//class CPenInputSingletonClient
CPenInputSingletonClient::CPenInputSingletonClient(const TUid& aUid,
                                            RPeninputServerImpl* aServer)
                        : CCoeStatic(aUid,EApp), 
                          iServer(aServer),iReferrence(1),iIsValid(ETrue)
    {
    }
    
RPeninputServerImpl* CPenInputSingletonClient::GetSingletonServer() 
    {
    RPeninputServerImpl* ret = 0;
    if(iIsValid && iServer->Handle())
        {
        ++iReferrence;
        ret = iServer;
        }
    return ret;
    }
    
TInt CPenInputSingletonClient::DecreaseRef() 
    {                
    --iReferrence;
    if( 0 == iReferrence && iIsValid)
        {
        iServer->FinalClose();      
        delete iServer;
        iServer = 0;          
        iIsValid = EFalse;
        }
    return iReferrence;
    }
    
TBool CPenInputSingletonClient::IsValid()
    {
    return iIsValid;
    }

void CPenInputSingletonClient::SetInvalid()
    {
    iIsValid = EFalse;
    }
    
void CPenInputSingletonClient::ReplaceServer(RPeninputServerImpl* aServer)
    {
    iServer = aServer;
    iIsValid = ETrue;
    iReferrence = 1;
    }
    
CPenInputSingletonClient::~CPenInputSingletonClient()
    {
    if ( iServer )
        {
        iServer->FinalClose();
        delete iServer;
        }    
    }

//for class background wnd  

TBool IsGlobalNotesApp(TUid& aUid)		  
    {
    const TInt KAknCapServerUid = 0x10207218;
    const TInt KAknNotifySrvUid = 0x10281EF2;      
         
    if(aUid.iUid == KAknCapServerUid || aUid.iUid == KAknNotifySrvUid)
        return ETrue;
    return EFalse;
    }


CPenUiBackgroundWnd* CPenUiBackgroundWnd::NewL(RWindowGroup& aWndGroup,TInt aBmpHandle)
    {
    CPenUiBackgroundWnd* self = new(ELeave) CPenUiBackgroundWnd(aWndGroup);
    CleanupStack::PushL(self);
    self->ConstructL(aBmpHandle);
    CleanupStack::Pop(self);
    return self;
    }
    
CPenUiBackgroundWnd::CPenUiBackgroundWnd(RWindowGroup& aWndGroup)
    :iWndGroup(aWndGroup)
    {
    }

CPenUiBackgroundWnd::~CPenUiBackgroundWnd()
    {
    delete iBitmap;

 //   if (iBitmapCpoy)
        {
 //       delete iBitmapCpoy;
 //       iBitmapCpoy = NULL;    
        }    
    }

void CPenUiBackgroundWnd::CreateBitmapL(TInt aBmpHandle)
    {
    iBitmap = new (ELeave) CFbsBitmap;
    TInt ret = iBitmap->Duplicate(aBmpHandle);    

    if(KErrNone != ret)
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    }
    
void CPenUiBackgroundWnd::ConstructL(TInt aBmpHandle)
    {
    CreateWindowL(iWndGroup);
    SetComponentsToInheritVisibility();

    Window().SetRequiredDisplayMode( EColor16MA );
    MakeVisible( EFalse );
    CreateBitmapL(aBmpHandle); 
    TBool b = IsNonFocusing();
    SetFocusing(EFalse);
    }

void CPenUiBackgroundWnd::Show(const TRect& aExtend, TBool aGlobalNotes, 
    TBool aInternal, TInt aPriority, TBool aResource)
    {
    //Show the window will cause a focus group change in global notes showing case.
    if (!iBitmap)
        {
        return;    
        }
    TUid curApp = GetCurAppUid();
    TUid focusApp = GetFocusAppUid();
    //dim effect due to global notes will not done by background control
    if(!aInternal || aGlobalNotes)
        return;    
      
//     if (iBitmapCpoy)
        {
//        delete iBitmapCpoy;
//        iBitmapCpoy = NULL;    
        }
    SetRect(aExtend);
    TRAP_IGNORE(ActivateL());
    MakeVisible(ETrue);
    if (!aResource)
        {
        RWsSession &ws = CCoeEnv::Static()->WsSession();
        TInt wgId =ws.GetFocusWindowGroup();    
        TInt priority = ws.GetWindowGroupOrdinalPriority(wgId);
        
        iWndGroup.SetOrdinalPosition( 0, aPriority); 

        Window().SetOrdinalPosition(0,aPriority);      
     
        Window().SetFaded(ETrue,RWindowTreeNode::EFadeWindowOnly);
        }
     else
        {
        Window().SetFaded(ETrue,RWindowTreeNode::EFadeWindowOnly);    
        }
 
    Window().Invalidate();
    DrawNow();
    }

void CPenUiBackgroundWnd::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
        
 //   if (iBitmapCpoy)
 //   gc.BitBlt(aRect.iTl,iBitmapCpoy,aRect);
    if (iBitmap)
    gc.BitBlt(aRect.iTl,iBitmap,aRect);
        
    }

void CPenUiBackgroundWnd::Hide()
    {
    MakeVisible( EFalse );
    }

// End of File

