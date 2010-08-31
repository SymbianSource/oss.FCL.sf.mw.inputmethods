/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for peninput server session class
*
*/


//INCLUDE FILES
#include <e32svr.h>

#include "peninputcmd.h"
#include "peninputclientserver.h"
#include "peninputanimcommand.h"
#include "peninputserver.h"
#include "peninputserversession.h"

const TInt KAllEventIndex = -1;
const TInt KFirstEventIndex = 0;
const TInt KMaxEventBuf = 50;
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputServerSession::CPeninputServerSession
// ---------------------------------------------------------------------------
//
CPeninputServerSession::CPeninputServerSession( RThread &aClient, 
                            CPeninputServer& aServer)
	                        : iPenInputSvr( aServer ),
    	                      iObserverActive(EFalse),
    	                      iCombinedMsgNum(0),
    	                      iCombinedEventId(ESignalKeyEvent),
    	                      iAppInfoType(EAppIndicatorMsg),
    	                      iDisabledUiType(0)
    {
    iPenInputSvr.IncreaseSessionCount();
    iClientId = aClient.Id();
    RProcess process;
    if ( KErrNone == aClient.Process(  process ) )
        {
        iClientProcess = process.Id();   
        process.Close();
        }
    }
// ---------------------------------------------------------------------------
// CPeninputServerSession::ConstructL
// ---------------------------------------------------------------------------
//
void CPeninputServerSession::ConstructL()
    {
    iAppInfo = KNullDesC().AllocL();
    iCombinedEventBuf.CreateMaxL(KMaxEventBuf);
    iCombinedEventBuf.Zero();    
    }

// ---------------------------------------------------------------------------
// CPeninputServerSession::NewL
// ---------------------------------------------------------------------------
//
CPeninputServerSession* CPeninputServerSession::NewL( RThread &aClient, 
                            						CPeninputServer& aServer)
    {
    CPeninputServerSession* session = new(ELeave) CPeninputServerSession(
                                           aClient, aServer);
    CleanupStack::PushL(session);
    session->ConstructL();
    CleanupStack::Pop(session);
    return session;
    }


// ---------------------------------------------------------------------------
// CPeninputServerSession::ServiceL
// Possible leave is handled by the CPeninputServer::RunError
// ---------------------------------------------------------------------------
//
void CPeninputServerSession::ServiceL( const RMessage2& aMessage )
    {
    
    switch ( aMessage.Function() )
        {

        case EPeninputServerRequestMessageNotification:
            // asynchnorous observation request issued, store it
            // a client can have only one observation request at a time
            if(!iObserverActive)
                {
                iPendingRequest = aMessage;
                iObserverActive = ETrue;
                //if already some events buffered,send the event
                if(iDataBuf.Count())
                    {
                    iPendingRequest.Complete(iSignalCodeArray[0]);                
                    iObserverActive = EFalse;
                    }
                }
            else
                {
                // cancel this request because notify request has already been issued
                aMessage.Complete(KErrCancel);
                }
            
            break;

        case EPeninputServerCancelMessageNotification:
            {
            aMessage.Complete(KErrNone);  
            if(iObserverActive)
                {
                iPendingRequest.Complete(KErrCancel);
                iObserverActive = EFalse;
                }
            ProcessPendingEventBeforeExit();
            //iSemaphore.Signal();            
            }
            break;
            
        default:
            {
            
            //TRAPD(err, DoServiceL(aMessage));
            //if(KErrNone != err)
            //    aMessage.Complete(err);
            TInt ret = DoServiceL(aMessage);
            //aMessage.Complete(ret);
            
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputServerSession::GetDisabledLayouts
// ---------------------------------------------------------------------------
//
const TProcessId& CPeninputServerSession::ClientProcessId( )
    {
    return iClientProcess ;
    }
    
TInt CPeninputServerSession::BuildEventData()
    {
    if(iCombinedMsgNum > 0)     //the previous combined msg was not sent yet. No need to do again
        return iCombinedMsgNum;    
    TInt n = 0;
    //check how many signal can be combined
    while( n < iSignalCodeArray.Count() && iSignalCodeArray[n] == iCombinedEventId)
         ++n;
    if(n <= 0)
        return 0;
    iCombinedMsgNum = 0;
    for(; iCombinedMsgNum < n ; ++iCombinedMsgNum)
        {
        //check memory fist
        if(iCombinedEventBuf.MaxLength() < iCombinedEventBuf.Length() + iDataBuf[iCombinedMsgNum]->Length()) 
            {
            TInt err = iCombinedEventBuf.ReAlloc( KMaxEventBuf + 
                                      iCombinedEventBuf.Length() + 
                                      iDataBuf[iCombinedMsgNum]->Length());
            if(err != KErrNone)
                {
                break;
                }
            }
        //copy data
        iCombinedEventBuf.Append(*(iDataBuf[iCombinedMsgNum]));        
        }
    return iCombinedMsgNum;
    }
    
void CPeninputServerSession::RemoveEventDatas(TInt aNum)
    {
    aNum = aNum > 0 ? aNum : 1;
    for(TInt i = 0 ; i < aNum; ++i)
        RemoveEventDataByIdx(KFirstEventIndex); 
    iCombinedEventBuf.Zero();
    iCombinedMsgNum = 0;
    }
    
TInt CPeninputServerSession::SendEventData(const RMessage2& aMessage, const TDesC& aData)
    {
    TInt ret = KErrNone;
    TInt maxBufLen = aMessage.Int0();    
    if(maxBufLen >= aData.Length())
        {
        TRAP_IGNORE(aMessage.WriteL(1,aData));
        
        //remove the data
        RemoveEventDatas(iCombinedMsgNum > 0 ? iCombinedMsgNum : 1);
        }
    else
        {
        ret = aData.Length();
        }
    
    return ret;    
    }
// ---------------------------------------------------------------------------
// CPeninputServerSession::DoServiceL
// Possible leave is handled by the CPeninputServer::RunError
// ---------------------------------------------------------------------------
//
TInt CPeninputServerSession::DoServiceL( const RMessage2& aMessage )
    {
    
    //TInt err = KErrNone;
    TInt ret = KErrNone;
    switch ( aMessage.Function() )
        {            
        case EPeninputRequestUiNotificationCompleted:
            {
            RemoveEventDataByIdx(KFirstEventIndex);
            iPenInputSvr.ExecutePendingAnimCmd();
            }
            break;
            
        case EPeninputServerClearServerEvent:
            {
            RemoveEventDataByIdx(KAllEventIndex);
            }
            break;
            
        case EPeninputRequestGetServerEventData:
            {
            if(iDataBuf.Count())
                {                
                TInt combinedEventNum = BuildEventData();
                if(combinedEventNum)
                    ret = SendEventData(aMessage,iCombinedEventBuf);
                else
                    ret = SendEventData(aMessage,*iDataBuf[KFirstEventIndex]);
                
//                if(ret == KErrNone)
  //                  RemoveEventDatas(combinedEventNum);
                }
            else
                {
                aMessage.WriteL(1,KNullDesC);
                }
            }
            break;        
            
        case EPeninputRequestGetServerEventDataNumber:
            {
            TInt num = iDataBuf.Count();
            TPckgC<TInt> msg(num);
            
            aMessage.WriteL(0,msg);
            }
            break;
          
        case EPeninputRequestGetDisableLayout:
            {
            //TInt layoyts = 0;
            //CPeninputServerSession* pSession = static_cast<CPeninputServerSession*>(aMessage.Session());
            //GetProcessLayouts( pSession->ClientProcessId(), layoyts );
            TInt dsaDisableUIType = iPenInputSvr.DisabledByDSA();
            TInt orientationDisableType = iPenInputSvr.DisabledByOrientation();
            if( dsaDisableUIType )
            	{
            	iDsaEverHappened = ETrue;
            	}
            TPckg<TInt> msg(iDisabledUiType | dsaDisableUIType | orientationDisableType);

            aMessage.WriteL(0,msg);            
            }
            break;            
        case EPeninputRequestDisableLayout:
            {
            TInt layouts  = aMessage.Int0(); 
            //CPeninputServerSession* pSession = static_cast<CPeninputServerSession*>(aMessage.Session());
            //SetProcessLayouts( pSession->ClientProcessId(), layouts );
            if(layouts < 0 )
                {
                iDisabledUiType = 0; //reset.
                }
            else
                {
                iDisabledUiType |= layouts;
                }
            }
            break;
            
        case EPeninputRequestUiIsVisible:
        case EPeninputServerRequestSetDisplayMode:
        case EPeninputRequestUiEditorMaxLength:
        case EPeninputRequestUiSetUiPos:        
        case EPeninputRequestUiGetUiPos:        
        case EPeninputRequestUiSendAppEditorText:        
        case EPeninputRequestActivateLayout:  
        case EPeninputRequestActivateLayoutInGlobalNotes:
        case EPeninputRequestDimLayout:
        case EPeninputRequestSetUiLayoutId:
        case EPeninputRequestSetUiLayoutIdWithData:
        case EPeninputRequestResourceChanged:
        case EPenInputRequestGetImePluginIdList:
        case EPenInputRequestGetPenSupportLanguages:
        case EPeninputRequestIsForeground:
        case EPeninputRequestSetForeground:
        case EPeninputRequestRelinquishForeground:
        case EPeninputRequestAddUiObserver:
        case EPeninputRequestIsLayoutDimmed:
        //case EPeninputRequestChangeUiObserverType:
        case EPeninputRequestRemoveUiObserver:            
        //case EPeninputRequestDisableLayout:   
        case EPeninputRequestServerThreadId:
        //case EPeninputRequestGetDisableLayout:
        case EPeninputBackgroudDefaultOri:
        case EPeninputRequestInternalDimLayout:
        case EPeninputRequestDimResChangeLayout:
        case EPeninputRequestSupportInputMode:
            ret = iPenInputSvr.HandleMessageL(aMessage);            
            break;
        case EPeninputRequestUpdateAppInfo:            
            {
            ret = HandleAppInfoChange(aMessage);
            if (ret == EBadRequest)
                {
                PanicClient(aMessage,EBadRequest );
                return EBadRequest;                
                }
            }
            break;
        case EPeninputRequestHandleClientCommand:
            {              
            ret = iPenInputSvr.HandleCommandL(aMessage);
            }
            break;        
            
        // requests we don't understand at all are a different thing,
        // so panic the client here, this function also completes the message
        default:
            {
            PanicClient(aMessage,EBadRequest );
            return EBadRequest;
            }
        }

    aMessage.Complete(ret);
    return ret;
    }


// ---------------------------------------------------------------------------
// CPeninputServerSession::SignalClientL
// ---------------------------------------------------------------------------
//
TBool CPeninputServerSession::CompletePendingRequestL(TInt aSignalCode,
                                        const TDesC& aEventData, int aPos)
    {
    TBool ok = EFalse;
    HBufC* data = aEventData.AllocLC();
//    iDataBuf.AppendL(data);
    if(aPos < 0) //append to the end
        {        
        iSignalCodeArray.Append(aSignalCode);
        iDataBuf.AppendL(data);
        }
    else
        {
        iSignalCodeArray.Insert(aSignalCode,aPos);
        iDataBuf.InsertL(data,aPos);        
        }    
    CleanupStack::Pop();//data
    
    if(iObserverActive)
        {             
        iPendingRequest.Complete(aSignalCode);
        iObserverActive = EFalse;
        ok = ETrue;
        }
    return ok;
    }
// ---------------------------------------------------------------------------
// CPeninputServerSession::SignalClientL
// ---------------------------------------------------------------------------
//
TBool CPeninputServerSession::SignalClientL(TInt aSignalCode,
                                                const TDesC& aEventData)
    {
    return CompletePendingRequestL(aSignalCode,aEventData);    
    }


// ---------------------------------------------------------------------------
// CPeninputServerSession::SignalUiActivationObserver
// ---------------------------------------------------------------------------
//
TBool CPeninputServerSession::SignalUiActivationObserver(TInt aSignalCode, 
                                                         TInt aType)
    {    
    //check registerd UI type
    if((aType & iRegistedPenUiType) || (iRegistedPenUiType == EPluginInputModeAll))
        {        
        //insert the notification event before other event as anim object is waiting 
        // for client process.
        TInt i = iSignalCodeArray.Count() - 1;
        for( ; i >=0 ; i--)
            {
            if(ESignalPenUiActivated == iSignalCodeArray[i] ||
                        ESignalPenUiDeActivated == iSignalCodeArray[i])
                        break;
            }
        TRAP_IGNORE( CompletePendingRequestL(aSignalCode,KNullDesC,++i) );
        return ETrue;
        }    
        
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputServerSession::ProcessPendingEventBeforeExit
// ---------------------------------------------------------------------------
//
void CPeninputServerSession::ProcessPendingEventBeforeExit()
    {
    //only process pen ui notification event. Other events are discarded
    
    //for(TInt i = 0; i < iDelayedCmd.Count(); i++)
    //for(TInt i = 0; i < iDelayedCmd; i++)
        {
        iPenInputSvr.ExecutePendingAnimCmd();       
      //  iDelayedCmd --;
        }
    //remove any command
    for(TInt i = 0; i < iDataBuf.Count(); i++)
        {
        delete iDataBuf[i];
        }
    iDataBuf.Reset();    
    iSignalCodeArray.Reset();        
    //iDelayedCmd.Reset();
    iDelayedCmd = 0;
    }
// ---------------------------------------------------------------------------
// CPeninputServerSession::PanicClient
// ---------------------------------------------------------------------------
//
void CPeninputServerSession::PanicClient(const RMessage2& aMessage,TInt aPanic)
    {
    aMessage.Panic(KPeninputServerName, aPanic);
    }

// ---------------------------------------------------------------------------
// CPeninputServerSession::~CPeninputServerSession
// ---------------------------------------------------------------------------
//
CPeninputServerSession::~CPeninputServerSession()
    {
    ProcessPendingEventBeforeExit();
    iPenInputSvr.DecreaseSessionCount(this);
    iPenInputSvr.DelProcessLayouts( iClientProcess );
    iDataBuf.Close();
    iSignalCodeArray.Close();
    iCombinedEventBuf.Close();
    //iDelayedCmd.Close();
    delete iAppInfo;
    }

// ---------------------------------------------------------------------------
// CPeninputServerSession::ClientId
// ---------------------------------------------------------------------------
//
TInt CPeninputServerSession::ClientId()
    {
    return iClientId;
    }
    
void CPeninputServerSession::RemoveEventDataByIdx(TInt aIndex)
    {
    //remove all if aIndex < 0
    if( aIndex == KAllEventIndex )
        {
        for(TInt i = iDataBuf.Count() - 1; i >= 0; i--)
            {        
            HBufC* usedData = iDataBuf[i];
            iDataBuf.Remove(i);
            delete usedData;
            iSignalCodeArray.Remove(i);
            }
        }
    else
        {
        if( aIndex >= 0 && aIndex < iDataBuf.Count() ) 
            {
            HBufC* usedData = iDataBuf[aIndex];
            iDataBuf.Remove(aIndex);
            delete usedData;
            iSignalCodeArray.Remove(aIndex);
            }
        }        
    }

//void CPeninputServerSession::AddDelayedCmd(TAnimationCommand aCmd)
void CPeninputServerSession::AddDelayedCmd()
    {
    //iDelayedCmd.Append(aCmd);
    //iDelayedCmd++;
    }
    
void CPeninputServerSession::RegisterPenUiType(TInt aType)
    {
    iRegistedPenUiType = aType;
    }
    
TInt CPeninputServerSession::HandleAppInfoChange(const RMessage2& aMessage)
    {
    TInt ret = 0;
    TRAP_IGNORE(ret = HandleAppInfoChangeL(aMessage));	
    return ret;
    }

TInt CPeninputServerSession::HandleAppInfoChangeL(const RMessage2& aMessage)
    {
    //TPeninputAppInfo type;
    if (aMessage.Ptr0() == NULL || aMessage.Ptr1() == NULL)
        {
        return EBadRequest;
        }

    TPckg<TPeninputAppInfo> typeData(iAppInfoType);            
    TInt rst = aMessage.Read(KMsgSlot0,typeData);
    if (rst != KErrNone)
        {
        return EBadRequest;
        }
    
    delete iAppInfo;
    iAppInfo = NULL;
    TInt len = aMessage.GetDesLength(KMsgSlot1);
    if (len < 0)
        {
        return EBadRequest;
        }
    iAppInfo = HBufC::NewL(len);
    TPtr appInfo = iAppInfo->Des();
    rst = aMessage.Read(KMsgSlot1,appInfo);
    if (rst != KErrNone)
        {
        return EBadRequest;
        }    
    return DoUpdateAppInfo();
    }
    
TInt CPeninputServerSession::DoUpdateAppInfo()
    {
    return iPenInputSvr.HandleAppInfoChange(this,*iAppInfo,iAppInfoType);  
    }
    
    
TInt CPeninputServerSession::DisabledUiType()
    {
    return iDisabledUiType;
    }
    
//end of implementation of Class CPeninputServerSession

// End of File
