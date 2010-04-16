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
* Description:  Implementation for peninput server
*
*/


//  INCLUDE FILES
#include <e32svr.h>
#include <w32std.h>
#include <coedef.h>
#include <apgwgnam.h>
#include <coemain.h>
#include <AknFepInternalCRKeys.h>
#include <akniconconfig.h>
#include <AknSgcc.h> 
#include <sensorplugindomaincrkeys.h> // KCRUidSensorSettings

#ifdef RD_TACTILE_FEEDBACK 
#include <featmgr.h>
#endif //RD_TACTILE_FEEDBACK  
#include "peninputclientserver.h"
#include "peninputserver.h"
#include "peninputserversession.h"
#include <PtiEngine.h>
#include <s32mem.h>
#include <gulgcmap.h>
#include "peninputanimclientobj.h"
#include "peninputuilayoutowner.h"
//#include "aknfeppeninputenums.h"
#include <UikonInternalPSKeys.h>
#include "penuiwndctrl.h"
#include "keyrotator.h"
#include <AknDef.h>
#include <aknfeppeninputenums.h>
#include <aknappui.h> 

#include "peninputcrpclient.h"
//#define __WND_TEST_

// CONSTANTS
const TSize KInitialPeninputSize= TSize( 10, 10 );
const TInt KMsgQueueLen = 1000;
const TInt KMsgResponseQueueLen = 10;

const TInt KWsSessionFlushPerioid = 50000;//50ms
const TInt KInvalidValue = -1;

enum TActivationFlag
	{
	ECmdDeactivate = EFalse,
	ECmdActivate = ETrue,
	};

enum TUpdateFlag
	{
	EPartialUpdate = EFalse,
	EFullUpdate = ETrue
	};

#ifdef __LOG_WNDGROU__	

#ifdef __WINS__
_LIT(KLogFile,"c:\\penwndgroup.txt");
#else
_LIT(KLogFile,"\\penwndgroup.txt");
#endif

#endif //__LOG_WNDGROU__

TUid GetAppUidByWndGroupIdL( RWsSession &aWs, TInt aWndGroupId )
    {
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC(aWs,aWndGroupId);
    TUid id = wg->AppUid();
    CleanupStack::PopAndDestroy(wg);    	
    return id;
    }
    
TUid GetFocusAppUid()
    {
    RWsSession &ws = CCoeEnv::Static()->WsSession();
    TInt wgId =ws.GetFocusWindowGroup();
    TUid id = TUid::Null();
    TRAP_IGNORE( id = GetAppUidByWndGroupIdL( ws, wgId ) );
    return id;
    }

#ifdef  __LOG_WNDGROU__
void Des2Des8(const TDesC& aSrc, TDes8& aDest)
    {
    aDest.Zero();
    const TUint16* ptr = aSrc.Ptr();
    for(TInt i = 0; i < aSrc.Length(); ++i)
        {
        aDest.Append(*((TUint8*)ptr));
        ptr++;
        }
    }

#endif 

 
#ifdef __LOG_WNDGROU__    
void WriteDeactivateLogL( RFile& aFile )
    {
    TInt ret = aFile.Open(CCoeEnv::Static()->FsSession(),KLogFile,EFileShareAny|EFileWrite);
    if(ret == KErrNone)
        {
        _LIT8 (KNotesID, "Notes id: %d  \r\n");
        TBuf8<30> buf;
        buf.Zero();
         
        TInt off = 0;
        aFile.Seek(ESeekEnd,off);
        
        RWsSession &ws = CCoeEnv::Static()->WsSession();
        TInt wgId =ws.GetFocusWindowGroup();
        CApaWindowGroupName* wgname = CApaWindowGroupName::NewLC(ws,wgId);
        
        TInt focusuid = GetFocusAppUid().iUid;
        buf.Format(KNotesID, focusuid);
        

        HBufC8* name = HBufC8::NewLC(wgname->Caption().Length());
        TPtr8 nameStr = name->Des();
        Des2Des8(wgname->Caption(),nameStr);
        aFile.Write(_L8(" ..."));
        aFile.Write(nameStr);            
        aFile.Write(buf);            
        aFile.Write(_L8(" Close UI. \r\n"));
        aFile.Close();
        CleanupStack::PopAndDestroy(name);
        CleanupStack::PopAndDestroy(wgname);
        }
    }
#endif
    
TBool IsGlobalNotesApp(TUid& aUid)
    {
    const TInt KAknCapServerUid = 0x10207218;
    const TInt KAknNotifySrvUid = 0x10281EF2;      
    if(aUid.iUid == KAknCapServerUid || aUid.iUid == KAknNotifySrvUid)
        return ETrue;
    return EFalse;
    }
    
   
// ======== MEMBER FUNCTIONS ========
 
// ---------------------------------------------------------------------------
// CPeninputServer::CPeninputServer
// ---------------------------------------------------------------------------
//
CPeninputServer::CPeninputServer ( TInt aPriority) :  CServer2 ( aPriority,ESharableSessions ),
                            iActive(EFalse),iDispMode(ENone),
                            iDispModeForMask(ENone),
                            iFullUpdate(EFalse),
                            iPrevActive(EFalse),iUILayoutReady(EFalse),
                            iSupportFeedback(EFalse)
    {
    AknIconConfig::TPreferredDisplayMode displayMode;

    AknIconConfig::PreferredDisplayMode(displayMode, AknIconConfig::EImageTypeIcon);
    
    iDispMode = displayMode.iBitmapMode;
    iDispModeForMask = displayMode.iMaskMode;
    
    iSpriteMember.iBitmap = NULL;
    iSpriteMember.iMaskBitmap = NULL;   
    iLayoutId.iUid = 0;
    iPrevLayoutId = iLayoutId;

    iSpriteSize = KInitialPeninputSize;
    iSpritePosition = TPoint(0,0);    
    //iDispMode = iDispModeForMask= CCoeEnv::Static()->WsSession().GetDefModeMaxNumColors(col,grey);    
    iPreNonGloebalNotesWndGrpId = -1;
    iNoNeedClean = EFalse;
    }


// ---------------------------------------------------------------------------
// CPeninputServer::NewL
// ---------------------------------------------------------------------------
//
CPeninputServer* CPeninputServer::NewL()
    {
    CPeninputServer* pS = new (ELeave) CPeninputServer(
                                                  CActive::EPriorityStandard);
    CleanupStack::PushL( pS );
    pS->ConstructL( );
    CleanupStack::Pop(pS);
    return pS;
    }


// ---------------------------------------------------------------------------
// CPeninputServer::ConstructL
// ---------------------------------------------------------------------------
//
void CPeninputServer::ConstructL( )
    {
#ifdef RD_TACTILE_FEEDBACK     
	//KS: QUICK FIX FOR EGSN-7BCBWS 
	FeatureManager::InitializeLibL();
	//KS: QUICK FIX FOR EGSN-7BCBWS 
   	iSupportFeedback = FeatureManager::FeatureSupported( KFeatureIdTactileFeedback );

#endif //RD_TACTILE_FEEDBACK   
    
    // start server, leaves if server already running
    StartL( KPeninputServerName );            
    
    User::LeaveIfError( iKeyEventSemaphore.CreateGlobal(
                                    KAnimKeyEventSemaphore,0) );
    
    iEventBufferQueue = CEventQueue::NewL(this,KMsgEventBufferQueue);
    
    iKeyMsgResponseQueue.CreateGlobal(KMsgEventResponseQueue,KMsgResponseQueueLen);
    
    iFlushTimer = CPeriodic::NewL(CActive::EPriorityHigh);        
    
    iHardwareLayoutChangeWatcher = CAknLayoutChangeWatcher::NewL(this,
                                                            KUikLayoutState);
    iSoftwareLayoutChangeWatcher = CAknLayoutChangeWatcher::NewL(this,
                                                   KUikPreferredOrientation);
    iHardwareLayoutChangeWatcher->StartWatching();
    iSoftwareLayoutChangeWatcher->StartWatching();

	RWindowGroup& rootWin = CCoeEnv::Static()->RootWin();
	rootWin.EnableFocusChangeEvents (); // For cover UI/status pane refresh problems
	rootWin.EnableGroupListChangeEvents();
	rootWin.EnableReceiptOfFocus(EFalse);
	rootWin.AutoForeground(EFalse);

    ConstructSpriteL();
    
    //create animation object
	iAnimObj = CPeninputAnimObj::NewL(iSpriteMember);
	
	iLayoutOwner = CPeninputUiLayoutOwner::NewL( *this );
	iPenUiCtrl = new(ELeave) CPenUiWndCtrl(CCoeEnv::Static()->RootWin(),iSpriteMember.iBitmap);
	iInternalBackgroundCtrl = new(ELeave) CInternalBkCtrl(CCoeEnv::Static()->RootWin());
    iInternalBackgroundCtrl->ConstructL();	
	iPenUiCtrl->ConstructL();
	
	iCurScreenFocusedWndGrpId = GetFocusAppUid().iUid;
#ifdef __LOG_WNDGROU__    
    iLogFile.Replace(CCoeEnv::Static()->FsSession(),KLogFile,EFileShareAny|EFileWrite);
    iLogFile.Close();
    
#endif
    
    iCurScrMode = CCoeEnv::Static()->ScreenDevice()->CurrentScreenMode();
    iCrpService = CPenInputCrpServiceClient::NewL();
    iSensorRepository = CRepository::NewL(KCRUidSensorSettings);
    }

void CPeninputServer::CleanAll()
    {
    if(iNoNeedClean)
       return;    

    delete iHardwareLayoutChangeWatcher;
    delete iSoftwareLayoutChangeWatcher;
    if(iAppGc)
        CCoeEnv::Static()->SwapSystemGc(iAppGc);
    
    delete iPtiEngine;

    iKeyEventSemaphore.Close();
    iUiHandlerArray.Close();
    // iSprite will be closed by iWsSession.Close()
    // but the sprite member bitmaps must be deleted
    // explicitly
    /*
    // iPrevUiLayout is not used any more
    if(iPrevUiLayout && iPrevUiLayout!=iUiLayout)
        {
        iPrevUiLayout->Destroy();        
        iPrevUiLayout = iUiLayout;
        }
        */
    if(iUiLayout)
        {
        iUILayoutReady = EFalse;
        iDestroyingLayout = ETrue;
        iUiLayout->Destroy();
        iUiLayout = NULL;       
        }
    delete iBitmapDevice;
    delete iMaskBitmapDevice;
    delete iSpriteGc;
    delete iSpriteMember.iBitmap;
    delete iSpriteMember.iMaskBitmap;

    delete iWindowGc;            
    delete iLayoutEng;    
    
    delete iLayoutOwner;
    
    delete iEventBufferQueue;
    iKeyMsgResponseQueue.Close();
    if(iFlushTimer)
        {
        iFlushTimer->Cancel();
        delete iFlushTimer;
        }
        

#ifdef RD_TACTILE_FEEDBACK
	//KS: QUICK FIX FOR EGSN-7BCBWS 
	FeatureManager::UnInitializeLib();
	//KS: QUICK FIX FOR EGSN-7BCBWS 
    iFeedbackAreaArray.Close();
#endif // RD_TACTILE_FEEDBACK    
    delete iPenUiCtrl;
    delete iInternalBackgroundCtrl;

    iClientProcess.Close();
    iClientLayouts.Close();
#ifdef __LOG_WNDGROU__    
    iLogFile.Close();    
#endif
    delete iAnimObj;
    iAnimObj = NULL;
    
    iNoNeedClean = ETrue;
    
    delete iCrpService;
    iCrpService = NULL;
    }
// ---------------------------------------------------------------------------
// CPeninputServer::~CPeninputServer
// Destructor
// ---------------------------------------------------------------------------
//
CPeninputServer::~CPeninputServer()
    {
    CleanAll();
#ifdef __WINS__
    iCloseServer = ETrue;
#endif        
    delete iSensorRepository;
    }


// ---------------------------------------------------------------------------
// CPeninputServer::IncreaseSessionCount
// ---------------------------------------------------------------------------
//
void CPeninputServer::IncreaseSessionCount()
    {
    ++iSessionCount;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::DecreaseSessionCount
// ---------------------------------------------------------------------------
//
void CPeninputServer::DecreaseSessionCount(CPeninputServerSession* aSession)
    {
    if( aSession == iForegroundSession)
        {
        iForegroundSession = NULL;
        Hide();
        }
    else
        {
        if( aSession == iForegroundUiHandler )
            {
            iForegroundUiHandler = NULL;
            }
        }
    
    //remove the session from list
    for(TInt i = 0; i < iUiHandlerArray.Count(); i++)
        {
        if( aSession == iUiHandlerArray[i] )
            iUiHandlerArray.Remove(i);
        }
    
    --iSessionCount;

#ifdef __WINS__
        if ( iSessionCount == 0 && !iCloseServer)
            {
            DeactivateSprite();
            PrepareExit();
            CActiveScheduler::Stop();
            }
#endif
    }


// ---------------------------------------------------------------------------
// CPeninputServer::ActivateSpriteInGlobalNotesL()
// Activate sprite
// ---------------------------------------------------------------------------
//
void CPeninputServer::ActivateSpriteInGlobalNotesL()
    {
    if(iActive)
        {
	    if(iUseWindowCtrl)
	        {
/*            RWsSession &ws = CCoeEnv::Static()->WsSession();
            TInt wgId =ws.GetFocusWindowGroup();
            //TInt wgHandle = ws.GetWindowGroupHandle(wgId);

      
    		TInt priority;
    		//TInt pos;
            //wg.Construct(wgHandle);
            //pos = wg.FullOrdinalPosition();
            priority = ws.GetWindowGroupOrdinalPriority(wgId);*/
            //wg.Close();           
            //iPenUiCtrl->ShowPenUi();
            ActivatePenUiLayout(EFalse);
            iDimmed = EFalse;
            iPenUiCtrl->ShowPenUiL(iDimmed);            
	        }
	    return;
        }

        TBool bNeedWait = AnimOpNeedWaiting(ESignalPenUiActivated);
        //add animation command
        
        /*
        if(iAnimObj->AddActivationCmd(ECmdActivate,bNeedWait))
        	{
        	if(bNeedWait)
        		iForegroundUiHandler->AddDelayedCmd();
	        //iUiLayout->OnActivate();                  
            ActivatePenUiLayout();
	        
	        DrawSprite();        
	        
	        iEventBufferQueue->GetEvent();
	        
	        iActive = ETrue;                
	        
	        if(iUseWindowCtrl)
                iPenUiCtrl->ShowPenUi();
            }
            */
        if(ActivatePenUiLayout(bNeedWait))
            {
        	if(bNeedWait)
        		iForegroundUiHandler->AddDelayedCmd();
	        
	        DrawSprite();        	        
	        
	        iActive = ETrue;                
	        
	        if(iUseWindowCtrl)
	            {
                /*RWsSession &ws = CCoeEnv::Static()->WsSession();
                TInt wgId =ws.GetFocusWindowGroup();
                //TInt wgHandle = ws.GetWindowGroupHandle(wgId);

          
        		TInt priority;
        		//TInt pos;
                //wg.Construct(wgHandle);
                //pos = wg.FullOrdinalPosition();
                priority = ws.GetWindowGroupOrdinalPriority(wgId);*/
                //wg.Close();           
                iPenUiCtrl->ShowPenUiL(iDimmed);            	        
	            }
            }
        }
// ---------------------------------------------------------------------------
// CPeninputServer::ActivateSprite
// Activate sprite
// ---------------------------------------------------------------------------
//
void CPeninputServer::ActivateSprite(TInt aRequiredScrMode)
    {
    if(!iUiLayout)
        return;

    if(iCurScrMode !=  aRequiredScrMode)
        {
        iUiLayout->OnResourceChange(KEikDynamicLayoutVariantSwitch);
        iCurScrMode = aRequiredScrMode; 
        }
    ActivateSprite();
    }
// ---------------------------------------------------------------------------
// CPeninputServer::ActivateSprite
// Activate sprite
// ---------------------------------------------------------------------------
//
void CPeninputServer::ActivateSprite()
    {
    //do nothing if we don't have ui layout or already activated.
    TUid focusApp = GetFocusAppUid();
    if(!IsGlobalNotesApp(focusApp))
        iPreNonGloebalNotesWndGrpId = focusApp.iUid;         

    if(!iUiLayout)
        return;
    
    // move it out from if(!iActive) to make sure that msg bubble can be 
    // shown if fast swap from other application to msg application.
    TBool bNeedWait = AnimOpNeedWaiting(ESignalPenUiActivated);
 
    if(!iActive)
        {       
        /*
        TInt scrMode = CCoeEnv::Static()->ScreenDevice()->CurrentScreenMode();
        TBool bScrRotated = scrMode != iCurScrMode;
        
        if(bScrRotated)
            {
            iCurScrMode = scrMode;
            iUiLayout->OnResourceChange(KEikDynamicLayoutVariantSwitch);
            }
            */
        //TBool bNeedWait = AnimOpNeedWaiting(ESignalPenUiActivated);
        //add animation command
        
        /*
        if(iAnimObj->AddActivationCmd(ECmdActivate,bNeedWait))
        	{
        	if(bNeedWait)
        		iForegroundUiHandler->AddDelayedCmd();
	        //iUiLayout->OnActivate();                  
            ActivatePenUiLayout();
	        
	        DrawSprite();        
	        
	        iEventBufferQueue->GetEvent();
	        
	        iActive = ETrue;                
	        
	        if(iUseWindowCtrl)
                iPenUiCtrl->ShowPenUi();
            }
            */
            
        if(ActivatePenUiLayout(bNeedWait, ETrue))
            {
        	if(bNeedWait)
        		iForegroundUiHandler->AddDelayedCmd();
	        
	        DrawSprite();        	        
	        
	        iActive = ETrue;                
	        
	        if(iUseWindowCtrl)
	            {
                TRAP_IGNORE( iPenUiCtrl->ShowPenUiL(iDimmed) );            	        
                           	        
		        if (iPreNonGloebalNotesWndGrpId != focusApp.iUid &&
		            IsGlobalNotesApp(focusApp) && !iInGlobalEditorState )
		            {
	                iDimmed = ETrue;
	                DimPenUi();                
		            }
	            }
            }
        }
    else
        {
        //actived but dimmed by global notes
        if(iUseWindowCtrl)
            {
            ActivatePenUiLayout(EFalse);

            if (iDimmed && (iInGlobalNotesApp || iInGlobalEditorState))
                {
                DeactivatePenUiLayout(EFalse);                
                return;
                }             
            
            TRAP_IGNORE( iPenUiCtrl->ShowPenUiL(iDimmed) );
            iDimmed = EFalse;
            }
        }				  
    }

// ---------------------------------------------------------------------------
// CPeninputServer::ActivatePenUiLayout
// ---------------------------------------------------------------------------
//
TBool CPeninputServer::ActivatePenUiLayout(TBool aWaitFlag, TBool aSkipDraw)
    {    
    RecoverSimulatedKeyEventState();
    RecoverButtonEventState();
        
    if (iAnimObj && iAnimObj->AddActivationCmd(ECmdActivate,aWaitFlag))
        {
        if (iUiLayout)
            {
            iUiLayout->OnActivate();
            if( !aSkipDraw )              
            	iUiLayout->Draw();
            }						                    

        iEventBufferQueue->GetEvent(); 

        if (iUseWindowCtrl)
    	    {
    	    CPenUiWndCtrl::EditorType type;
    	    
    	    if (iInGlobalEditorState)
    	        {
    	        type = CPenUiWndCtrl::EGlobleNotesWithEditor;    
    	        }
    	    else
    	        {
                TUid focusApp = GetFocusAppUid();
                if(IsGlobalNotesApp(focusApp))
                    {
                    type = CPenUiWndCtrl::EGlobleNotes;      
                    }
                else 
                    {
                    type = CPenUiWndCtrl::ENoremalEditor;     
                    }
    	        }
    	        	    
            iPenUiCtrl->OnActivate(type); 
            //iPenUiCtrl->ShowPenUi();
    	    }
        return ETrue;
        }        
    return EFalse;    
    }


void CPeninputServer::DimPenUiWnd(TBool aFlag)
    {
    RecoverSimulatedKeyEventState();    
    if(aFlag)
        {
        iEventBufferQueue->Cancel();                   
        iUiLayout->OnDeActivate(); 
        if(iUseWindowCtrl)
            iPenUiCtrl->OnDeactivate(); //disable non fading                     
        }
    else
        {
        iEventBufferQueue->GetEvent();                   
        iUiLayout->OnActivate(); 
        if(iUseWindowCtrl)
            iPenUiCtrl->OnActivate(CPenUiWndCtrl::ENoremalEditor); //disable non fading                             
        }
    
    }
// ---------------------------------------------------------------------------
// CPeninputServer::DeactivatePenUiLayout
// ---------------------------------------------------------------------------
//
TBool CPeninputServer::DeactivatePenUiLayout(TBool aWaitFlag)
    {
    //iEventBufferQueue->Cancel();           

    if (iAnimObj && iAnimObj->AddActivationCmd(ECmdDeactivate,aWaitFlag))
        {
        if (iUiLayout)
            {
            RecoverSimulatedKeyEventState();
            RecoverButtonEventState();
            iUiLayout->OnDeActivate(); 
            if(iUseWindowCtrl)
                iPenUiCtrl->OnDeactivate(); //disable non fading             
            }								    	

        return ETrue;
        }
    return EFalse;              
    }

// ---------------------------------------------------------------------------
// CPeninputServer::DeactivatePenUiLayout
// ---------------------------------------------------------------------------
//
TBool CPeninputServer::DeactivateDimPenUiLayout(TBool aWaitFlag)
    {
    //iEventBufferQueue->Cancel();           

    if (iAnimObj && iAnimObj->AddActivationCmd(ECmdDeactivate,aWaitFlag))
        {
        if (iUiLayout)
            {
            RecoverSimulatedKeyEventState();
            iUiLayout->OnDeActivate(); 
            }								    	

        return ETrue;
        }
    return EFalse;              
    }

// CPeninputServer::DeactivateSprite
// ---------------------------------------------------------------------------
//
void CPeninputServer::DeactivateSprite(TBool aImmediateHideFlag, TBool aRotation)
    {
    if (!iActive || !iUiLayout)
        return;
#ifdef __LOG_WNDGROU__  
  //  TRAP_IGNORE( WriteDeactivateLogL(iLogFile) )  ;
#endif
        
    aRotation = aRotation || iResourceChange;  
        
    iActive = EFalse;                            
    if (!aRotation)
        {
        iDimmed = EFalse;
        iInGlobalNotesState = EFalse;
        }
    iFlushTimer->Cancel();    
    TBool bNeedWait = !aImmediateHideFlag && 
    							AnimOpNeedWaiting(ESignalPenUiDeActivated);     
  	      

    if (DeactivatePenUiLayout(bNeedWait) && bNeedWait)
        iForegroundUiHandler->AddDelayedCmd();

    if (iUseWindowCtrl)
        iPenUiCtrl->ClosePenUi(aRotation);
    
    //save current screen mode
    iCurScrMode = CCoeEnv::Static()->ScreenDevice()->CurrentScreenMode();    
    }

// ---------------------------------------------------------------------------
// CPeninputServer::HandleMessageL
// Handle client message
// ---------------------------------------------------------------------------
//
TInt CPeninputServer::HandleMessageL(const RMessage2& aMessage)
    {
    //It's traped in server session
    TInt err = KErrNone;
    if(!iPtiEngine || !iLayoutEng)
        DoIdleConstructL();
    
    TBool isForeground = IsForegroundSession(
                    static_cast<CPeninputServerSession*>(aMessage.Session()));
    //message requests UI attribute is always handled.
    switch(aMessage.Function())
        {
        
        case EPeninputRequestUiIsVisible:
            {                      
            TPckg<TBool> msg(iActive);
            
            aMessage.WriteL(0,msg);            
            }
        
            break;
         case EPeninputRequestUiGetUiPos:
             {
            TPckgC<TPoint> msg(iSpritePosition);
            TPckgC<TSize> msgSize(iSpriteSize);
            
            aMessage.WriteL(0,msg);
            aMessage.WriteL(1,msgSize);
             }
             break;
            
        case EPenInputRequestGetPenSupportLanguages:
            GetPenSupportLanguagesL(aMessage);
            break;
        case EPenInputRequestGetImePluginIdList:
            GetImePluginIdListL(aMessage);
            break;
        case EPeninputRequestRelinquishForeground:
            {
            if(isForeground)
                iForegroundSession = NULL;
            }
            break;
        case EPeninputRequestSetForeground:
            {
            TInt msgGroupId;
            TPckg<TInt> msgIdData(msgGroupId);
            aMessage.ReadL(KMsgSlot0,msgIdData);

            //TInt curFocusId = GetFocusAppUid().iUid;                
            TBool mustOpenFlag;
            TPckg<TBool> msgFlagData(mustOpenFlag);                        
            aMessage.ReadL(KMsgSlot1,msgFlagData);
            
            //ignore the command if it's from another focused group.
            /*******************************************************************/
            //This is a fix to web browse case. When swithing to web, device changeded
            // to landscape mode automatically, but previous app with pen UI, e.g. 
            // Notepad, will gain the foregound, thus open the UI.
            /*******************************************************************/
            if(!mustOpenFlag && msgGroupId != iCurScreenFocusedWndGrpId) //screen has changed, but foreground is wrong.
                {
                //only check this if it's not openned by pen click
                err = KErrNotWrongFocusedWindowGroup;
                //iForegroundSession = NULL;                    
                }
            else
                {
                CPeninputServerSession* session = 
                       static_cast<CPeninputServerSession*>(aMessage.Session());
                /*if(!IsSameProcess(iForegroundSession, session))
                    {
                    //
                    }*/
                iForegroundSession = session;                
                                                            
                //check whether there is ui notification handler from the same process
                if(iUiHandlerArray.Count())
                    SetForeGroundUiHandlerL();
                }       

            TPckgC<TBool> msg(iForegroundSession != NULL);            
            aMessage.WriteL(KMsgSlot2,msg);            
            }
            break;
            
        case EPeninputRequestIsForeground:
            {
            TPckgC<TBool> msg(isForeground);            
            aMessage.WriteL(0,msg);            
            }
            break;
    
        case EPeninputRequestIsLayoutDimmed:
            {
            TPckgC<TBool> msg(iDimmed);            
            aMessage.WriteL(0,msg);            
            }
            break;
    
        //other message is only handled when the request session is foreground session.   
        
         case EPeninputRequestUiSetUiPos:
            if(isForeground)
                {
                TPoint pt;
                TPckg<TPoint> msg(pt);            
                aMessage.ReadL(KMsgSlot0,msg);
                
                SetPosition(pt);              
                }
            else
                {                
                err = KErrNotForegroundSession;
                }
             break;
        case EPeninputServerRequestSetDisplayMode:
            if(isForeground)
                {
                TDisplayMode mode;
                TDisplayMode maskMode;
                TPckg<TDisplayMode> data1(mode);
                TPckg<TDisplayMode> data2(maskMode);
                
                aMessage.ReadL(KMsgSlot0,data1);
                aMessage.ReadL(KMsgSlot1,data2);
                
                ChangeDisplayModeL(mode,maskMode);
                }
            else
                {                
                err = KErrNotForegroundSession;
                }
                
            break;
        case EPeninputRequestActivateLayout:
            if(isForeground)                
                {
                iInGlobalEditorState = EFalse;
                TBool bFlag ;            

                TPckg<TBool> msg(bFlag );
                aMessage.ReadL(KMsgSlot0,msg);            
                if(bFlag)
                    {
                    TInt requiredMode;
                    TPckg<TInt> scrMsg(requiredMode);
                    aMessage.ReadL(KMsgSlot1,scrMsg); 
        
                    ActivateSprite(requiredMode);
                    static_cast<CPeninputServerSession*>(
                        aMessage.Session())->DoUpdateAppInfo();
                    }                
                else
                    {
                    DeactivateSprite();  
                    }
                }
            else
                {
                err = KErrNotForegroundSession;
                }            
            break;
        case EPeninputRequestActivateLayoutInGlobalNotes:
            //if(isForeground)                
                {                
                iCurGlobalNotesId = GetFocusAppUid().iUid;                
                iInGlobalEditorState = ETrue;             
                ActivateSpriteInGlobalNotesL();
                }
            /*else
                {
                err = KErrNotForegroundSession;
                }*/
            break;
            
        case EPeninputRequestInternalDimLayout:
            {
            if(iUseWindowCtrl && iActive)
                {  
                TBool dim;  
                TPckg<TBool> msg(dim);
                aMessage.ReadL(KMsgSlot0,msg);            
                TInt priority;  
                TPckg<TInt> priorityMsg(priority);
                aMessage.ReadL(KMsgSlot1,priorityMsg);

                TInt bmpHandle = iSpriteMember.iBitmap->Handle();
                TPckg<TInt> handleMsg(bmpHandle); 
                aMessage.WriteL(KMsgSlot2,handleMsg);

                TRect pos = iPenUiCtrl->Rect();
                pos.Move(iPenUiCtrl->Position());
                TPckg<TRect> extMsg(pos); 
                aMessage.WriteL(KMsgSlot3,extMsg);   
                iDimmed = dim;
                DimPenInternalUi(iDimmed,priority);
                }
                }
            break;
        case EPeninputRequestDimLayout:
            if(iUseWindowCtrl && iActive)
                {  
                TBool dim;  
                TPckg<TBool> msg(dim);
                aMessage.ReadL(KMsgSlot0,msg);            
                TInt priority;  
                TPckg<TInt> priorityMsg(priority);
                aMessage.ReadL(KMsgSlot1,priorityMsg);
                

                TInt bmpHandle = iSpriteMember.iBitmap->Handle();
                TPckg<TInt> handleMsg(bmpHandle); 
                aMessage.WriteL(KMsgSlot2,handleMsg);

                TRect pos = iPenUiCtrl->Rect();
                pos.Move(iPenUiCtrl->Position());
                TPckg<TRect> extMsg(pos); 
                aMessage.WriteL(KMsgSlot3,extMsg);   
                if(dim != iDimmed)
                    {
                    iDimmed = dim;
                    DimPenUi();
                    /*if(iDimmed)                                                      
                        //DeactivatePenUiLayout(EFalse);
                        DeactivateSprite();
                    else
                        ActivatePenUiLayout(EFalse);
                      */
                    }
                }
            break;
        case EPeninputRequestSetUiLayoutId:
        case EPeninputRequestSetUiLayoutIdWithData:
            if(isForeground || iDimmed)                        
                {                
                err = CreateLayoutL(aMessage);
                }
            else
                {                
                err = KErrNotForegroundSession;
                }
            
            break;  
        case EPeninputRequestResourceChanged:
            if(isForeground)                                
                { 
                if(!iUiLayout)
                    return KErrNoLayout; 
                
                if(iPrevActive)
                    {              
                    ActivateSprite();
                    iPrevActive = EFalse;                    
                    }
                }
            else
                {                
                err = KErrNotForegroundSession;
                }
                
            break;
        case EPeninputRequestAddUiObserver:
            {
            TInt type ;            

            TPckg<TInt> msg(type);
            aMessage.ReadL(KMsgSlot0,msg);            
            static_cast<CPeninputServerSession*>(aMessage.Session())->
                                                      RegisterPenUiType(type);

            iUiHandlerArray.Append(static_cast<CPeninputServerSession*>(aMessage.Session()));
            if(iForegroundSession)
                SetForeGroundUiHandlerL();            
            }
            break;

        case EPeninputRequestRemoveUiObserver:
            {            
            CPeninputServerSession* s =static_cast<CPeninputServerSession*>
                                                         (aMessage.Session());
            if( s == iForegroundUiHandler)
                {
                s->ProcessPendingEventBeforeExit();
                iForegroundUiHandler = NULL;                
                }
                
            for(TInt i = 0; i < iUiHandlerArray.Count();i++)
                {
                if(s == iUiHandlerArray[i])
                    {                    
                    iUiHandlerArray.Remove(i);
                    break;
                    }
                }
            }
            break;
        /*case EPeninputRequestGetDisableLayout:
            {
            TInt layoyts = 0;
            CPeninputServerSession* pSession = static_cast<CPeninputServerSession*>(aMessage.Session());
            GetProcessLayouts( pSession->ClientProcessId(), layoyts );

            TPckg<TInt> msg(layoyts);           

            aMessage.WriteL(0,msg);            
            }
            break;            
        case EPeninputRequestDisableLayout:
            {
            TInt layouts  = aMessage.Int0(); 
            CPeninputServerSession* pSession = static_cast<CPeninputServerSession*>(aMessage.Session());
            SetProcessLayouts( pSession->ClientProcessId(), layouts );
            }
            break;*/
        case EPeninputRequestServerThreadId:            
            {
            RThread thread;
            TThreadId id = thread.Id();
            TPckg<TThreadId> msg(id);
            
            aMessage.WriteL(0,msg);            
            thread.Close();
            }
            break;
        case EPeninputBackgroudDefaultOri:
            {
            TInt flag = 0;
            TPckg<TInt> msg(flag);
            
            aMessage.ReadL(0,msg);            
            BackgroudDefaultOri( flag );
            }
            break;
       case EPeninputRequestDimResChangeLayout:
            {
            TInt flag = 0;
            TPckg<TInt> msg(flag);
            
            aMessage.ReadL(0,msg); 
            
            iResourceChange = flag; 
            
            if(iUseWindowCtrl)
                iPenUiCtrl->SetResourceChange(iResourceChange);                           
            } 
            break;
    	case EPeninputRequestSupportInputMode:
    	    {
            TInt supportMode = GetSupportModeL();

            TPckg<TInt> msg(supportMode);
            
            aMessage.WriteL(0,msg);
    	    }
    	    break;            
        default: //Let user pluging handling the user command
            break;
        }
    return err;
    }
// ---------------------------------------------------------------------------
// CPeninputServer::HandleCommandL
// Handle client command
// ---------------------------------------------------------------------------
//
TInt CPeninputServer::HandleCommandL(const RMessage2& aMessage)
    {
    //let UI layout handles the command
    TInt err = KErrNone;    
    
    //command is only handled when the request session is foreground session.
    /*CPeninputServerSession* s = 
                    static_cast<CPeninputServerSession*>(aMessage.Session());
    if( iForegroundSession == static_cast<CPeninputServerSession*>(
                                                    aMessage.Session()) )*/
    if(IsForegroundSession(static_cast<CPeninputServerSession*>(aMessage.Session())))
        {
        //get cmd id    
        TInt cmdId;
        TPckg<TInt> cmdMsg(cmdId); 
        aMessage.ReadL(KMsgSlot0,cmdMsg);    
      
        TInt len = aMessage.GetDesLength(KMsgSlot1);    
        HBufC8* dataBuf = HBufC8::NewLC(len);
        TPtr8 msgData = dataBuf->Des();
        aMessage.ReadL(KMsgSlot1,msgData);                
      
        if(iUiLayout)
            {
            err = iUiLayout->HandleCommand(cmdId,(unsigned char*)msgData.Ptr());  
            TPckg<TInt> msg(err); 
            aMessage.WriteL(KMsgSlot2,msg);
            }
        
        CleanupStack::PopAndDestroy(dataBuf);
        }
    else
        {        
        err = KErrNotForegroundSession;
        }
    return err;
    
    }

TInt CPeninputServer::CreateLayoutL(const RMessage2& aMessage )
{   
    //check initialization status
    //if(!iAppGc)
      // iAppGc = CCoeEnv::Static()->SwapSystemGc(iWindowGc);
        
    TInt id;
    TPckg<TInt> msg(id);    
    aMessage.Read(KMsgSlot0,msg);
    TUid uid;
    uid.iUid = id;
    
    
    if(iLayoutId == uid && iUiLayout) 
        return KErrNone;
     
    iLayoutId = uid;

    //clear sprite before delete ui layout
    iUILayoutReady = EFalse;    
    if(iUiLayout)
        {
		ClearSpriteGc();
        DeactivateSprite();
        iDestroyingLayout = ETrue;
        iUiLayout->Destroy();
        iUiLayout = NULL;    
        iDestroyingLayout = EFalse;
        if(iAnimObj)
            {
            iAnimObj->AddFlushSessionCmd();
            }   
		#ifdef RD_TACTILE_FEEDBACK        
        //remove all registered feedback area
        const TInt KRemoveAllId = -1;
        TTactileFeedbackArea area = {KRemoveAllId,TRect(0,0,0,0)};      
        RemoveFeedbackArea(area);
		#endif
        }
    if(aMessage.Function() == EPeninputRequestSetUiLayoutId)        
        {
        TPeninputLayoutData layoutData = 
            {
            iPtiEngine,
            //iPenUiCtrl
            iInternalBackgroundCtrl,
            };
        //iUiLayout = iLayoutEng->GetUiLayout(iLayoutId,iLayoutOwner,iPtiEngine);
        iUiLayout = iLayoutEng->GetUiLayout(iLayoutId,iLayoutOwner,&layoutData);
        }
    else
        {
        TInt len = aMessage.GetDesLength(KMsgSlot1);    
        HBufC8* dataBuf = HBufC8::NewLC(len);
        TPtr8 msgData = dataBuf->Des();
        aMessage.ReadL(KMsgSlot1,msgData);                
        
        iUiLayout=iLayoutEng->GetUiLayout(iLayoutId,iLayoutOwner,msgData.Ptr());        
        CleanupStack::PopAndDestroy(dataBuf);
        }
    
    if(!iUiLayout)
        return KErrNoLayout;
    
    iSpriteMember.iInvertMask = EFalse;    
    TRect layoutRect;    
    //do we need transparency?
    TBool bTransparency = iUiLayout->SemiTransparencyRequired();
    if(bTransparency)
        {
#ifdef __ALPHA_TRANCEPARENCY__
        ChangeDisplayModeL(iDispMode,EGray256);    
#else
        ChangeDisplayModeL(iDispMode,EColor16MU);
#endif
        }
    else
        {
        ChangeDisplayModeL(iDispMode,EGray2); //use black-white mask
        }   

    layoutRect = iUiLayout->Init();
    
    if( layoutRect.IsEmpty() )
        {//this is a temp solution for VKB, the layout size can not 
         //be determined at construct time
        TPixelsTwipsAndRotation ptSize;
        CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation( ptSize );              
        layoutRect = TRect( TPoint(), ptSize.iPixelSize );
        }
    iUILayoutReady = ETrue;		  
    // register feedback area
    /*if(iSupportFeedback && iFeedbackAreaArray.Count())
        {        
        iAnimObj->AddRegisterFeedbackAreaCmd(iFeedbackAreaArray);        
        iFeedbackAreaArray.Reset();        }*/
        
    //flush the command
    if(iAnimObj)
        {
        iAnimObj->AddFlushSessionCmd();
        }
        
    TSize layoutSize = layoutRect.Size();

    iSpriteSize = layoutSize;
/*    iBitmapDevice->Resize( layoutSize);
    iMaskBitmapDevice->Resize( layoutSize);    

    //gc must be adjusted
    iSpriteGc->Activate(iBitmapDevice);
    iSpriteGc->Resized();
  */
    ResizeDrawingDevice(layoutSize);
    if(iAnimObj)
        {
        iAnimObj->AddChangeSizeCmd(layoutSize);
        }
	
    SetPosition(layoutRect.iTl);
    if(iUiLayout->PenInputType() == EPluginInputModeItut ||
       iUiLayout->PenInputType() == EPluginInputModeFSQ ||
       iUiLayout->PenInputType() == EPluginInputModeFingerHwr )
        {        
        iOpaqueUi = ETrue;
        iUseWindowCtrl = ETrue;

        if(iUseWindowCtrl)
            {            
        iPenUiCtrl->SetExtent(layoutRect.iTl,layoutSize);
            if(iAnimObj)
                {
                iAnimObj->AddEnalbeSpriteCmd(EFalse);
                }
        //must swap back when using window background
            if(iAppGc)
                {
                CCoeEnv::Static()->SwapSystemGc(iAppGc);
                }
            }
        else
            {
            if(iAnimObj)
                {
                iAnimObj->AddEnalbeSpriteCmd(ETrue);
                }

            iAppGc = CCoeEnv::Static()->SwapSystemGc(iWindowGc);
            
            }
        }
    else
        {
        iOpaqueUi = EFalse;
        if(iUseWindowCtrl && iAnimObj) //previous window control used
            {
            iUseWindowCtrl = EFalse;
            iAnimObj->AddEnalbeSpriteCmd(ETrue);
            }
        }
    
    TInt type = iUiLayout->PenInputType();
    TPckg<TBool> typeMsg(type);
            
    aMessage.WriteL(KMsgSlot2,typeMsg);            
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::SignalObserverL
// Signal oberver
// ---------------------------------------------------------------------------
//
void CPeninputServer::SignalObserverL(TInt aEventId,const TDesC& aEventData)
    {
    if(iForegroundSession)    
        iForegroundSession->SignalClientL(aEventId,aEventData);
    }    

// ---------------------------------------------------------------------------
// CPeninputServer::NewSessionL
// ---------------------------------------------------------------------------
//
CSession2* CPeninputServer::NewSessionL(const TVersion& aVersion, 
                                            const RMessage2& aMessage) const
    {
    // check we're the right version
    TVersion v( KPeninputServerMajorVersionNumber,
                KPeninputServerMinorVersionNumber,
                KPeninputServerBuildVersionNumber );

    if ( !User::QueryVersionSupported(v,aVersion) )
        {
        User::Leave( KErrNotSupported );
        }

    // create new session
    RThread client;
    CleanupClosePushL(client);
    CSession2* ret = 0;
    
    if(KErrNone == aMessage.Client(client))
        {
        //create new one
        ret = CPeninputServerSession::NewL(client,
        						const_cast<CPeninputServer&>(*this));\
        }
    CleanupStack::PopAndDestroy(&client);
    return ret;
    }


// ---------------------------------------------------------------------------
// CPeninputServer::RunError
// Handle an error from CPeninputServerSession::ServiceL()
// Reports the error to the client and completes the message
// ---------------------------------------------------------------------------
//
TInt CPeninputServer::RunError( TInt aError )
    {
    Message().Complete( aError );

    // The leave will result in an early return from CServer::RunL(), skipping
    // the call to request another message. So do that now in order to keep the
    // server running.
    ReStart();
    return KErrNone;// handled the error fully
    }


// ---------------------------------------------------------------------------
// CPeninputServer::ConstructSpriteAnimL
// ---------------------------------------------------------------------------
//
void CPeninputServer::ConstructSpriteL()
    {
    // create bitmaps for sprite
    iSpriteMember.iBitmap = CreateBitmapL( iSpriteSize, iDispMode );
#ifdef __ALPHA_TRANCEPARENCY__
    iSpriteMember.iMaskBitmap = CreateBitmapL( iSpriteSize, EGray256);    
    iDispModeForMask = EGray256;
    
#else
    iSpriteMember.iMaskBitmap = CreateBitmapL( iSpriteSize, iDispModeForMask);    
    iSpriteMember.iInvertMask = EFalse;    
#endif

    // set spritemember attributes
    iSpriteMember.iOffset.iX = 0;
    iSpriteMember.iOffset.iY = 0;
    iSpriteMember.iInterval = 0;

    iBitmapDevice = CFbsBitmapDevice::NewL(iSpriteMember.iBitmap);
    iMaskBitmapDevice = CFbsBitmapDevice::NewL( 
                                        iSpriteMember.iMaskBitmap );
    iSpriteGc = CFbsBitGc::NewL();
    iSpriteGc->Reset();
    
    CWsScreenDevice *screenDevice = CCoeEnv::Static()->ScreenDevice();
    iWindowGc = CWindowToBitmapMappingGc::NewL(*screenDevice,*iSpriteGc);   
    
    /*iBitmapDevice->Resize( iSpriteSize);
    iMaskBitmapDevice->Resize( iSpriteSize);    */
    ResizeDrawingDevice(iSpriteSize);    
    }


// ---------------------------------------------------------------------------
// CPeninputServer::CreateBitmapL
// ---------------------------------------------------------------------------
//
CFbsBitmap* CPeninputServer::CreateBitmapL( const TSize& aBitmapSize, 
                                       TDisplayMode aDisplayMode ) const
    {
    CFbsBitmap* const bitmap = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( bitmap );
    // bitmaps are automatically cleared to white when first created
    User::LeaveIfError( bitmap->Create( aBitmapSize, aDisplayMode ) ); 
    CleanupStack::Pop(bitmap);
    return bitmap;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::ChangeDisplayMode
// ---------------------------------------------------------------------------
//
void CPeninputServer::ChangeDisplayModeL(TDisplayMode aMode,
                                                    TDisplayMode aMaskMode)
    {
    if(aMode == iDispMode && aMaskMode == iDispModeForMask)
        return;        
   
    if(iDispMode != aMode)
        {        
        //use initial size to create small one in the first
        CFbsBitmap* bmp = CreateBitmapL( iSpriteSize, aMode );
        delete iSpriteMember.iBitmap;
        iSpriteMember.iBitmap = bmp;
        iDispMode = aMode;
        delete iBitmapDevice;
        iBitmapDevice = NULL;
        iBitmapDevice = CFbsBitmapDevice::NewL(iSpriteMember.iBitmap);
        }

    if(iDispModeForMask != aMaskMode)
        {
        CFbsBitmap* maskbmp = CreateBitmapL( iSpriteSize, aMaskMode);        
        delete iSpriteMember.iMaskBitmap;
        iSpriteMember.iMaskBitmap = maskbmp;
        delete iMaskBitmapDevice;
        iMaskBitmapDevice = NULL;
        iMaskBitmapDevice = CFbsBitmapDevice::NewL( 
                                        iSpriteMember.iMaskBitmap );
        iDispModeForMask = aMaskMode;
        }
    if(iAnimObj)
        {
        iAnimObj->AddChangeDispModeCmd(iSpriteMember);
        }
    }
// ---------------------------------------------------------------------------
// CPeninputServer::ClearSpriteGc
// Clears the bitmap - and mask bitmap devices.
// ---------------------------------------------------------------------------
//
void CPeninputServer::ClearSpriteGc()
    {
    //bitmap device
    TPoint pt(0,0);
    TRect rect(pt,iSpriteSize);
    iSpriteGc->Activate( iBitmapDevice );
    iSpriteGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iSpriteGc->SetBrushColor( KRgbWhite );
    iSpriteGc->Clear(rect);

    //mask bitmap device
    iSpriteGc->Activate( iMaskBitmapDevice );
    iSpriteGc->SetBrushColor( KRgbBlack ); //transparent
    iSpriteGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iSpriteGc->Clear(rect);    
    }


// ---------------------------------------------------------------------------
// CPeninputServer::DrawSprite
// Draw the sprite
// ---------------------------------------------------------------------------
//
void CPeninputServer::DrawSprite()
    {

    if(!iUiLayout)
        return;
    ClearSpriteGc();
    
    iUiLayout->Draw();

    TRect r(TPoint(0,0),iSpriteSize);
    
    if(iUseWindowCtrl)
        {        
        iPenUiCtrl->Invalidate(r,EFullUpdate);
        return;
        } 
    else
        {
        if(iAnimObj)
            {
            iAnimObj->AddUpdateUiCmd(r,EFullUpdate);
            }
        
        //The best place for the timer should also in animation object. Can be refactored 
        //later
        if(!iFlushTimer->IsActive())
            {        
            iFlushTimer->Start(KWsSessionFlushPerioid, KWsSessionFlushPerioid,
                                          TCallBack(FlushTimerCallBack,this));
            }
        iCurRectNumInBuf = 0;
   	    } 
    return;
    }

//MLayoutOwner
// ---------------------------------------------------------------------------
// CPeninputServer::UpdateArea
// Update sprite rect
// ---------------------------------------------------------------------------
//
void CPeninputServer::UpdateArea(const TRect& aRect,TBool /*aFullUpdate*/,
                                                            TBool aImmedFlag)
    {
    if (!iActive) //only update when sprite is active
        return;
    
    if(iUseWindowCtrl)
        {        
        iPenUiCtrl->Invalidate(aRect,EFalse);
        return;
        } 
	
	        TRect rect;
        if(aRect.IsEmpty())
            {
            iSpriteGc->RectDrawnTo(rect);
            }
        else
            rect = aRect;

 //   if (iOpaqueUi)
 //       aFullUpdate = EFalse; //never does full update to screen.  

    TUpdateArea area = 
        {
        aRect,EFalse
        };
    AddFlushArea(area);
    if(aImmedFlag)
        {
        FlushTimerCallBack(this);
        }
    else
        {
        if(!iFlushTimer->IsActive())
            {        
            iFlushTimer->Start(KWsSessionFlushPerioid, KWsSessionFlushPerioid,
                                          TCallBack(FlushTimerCallBack,this));
            }
        }
    }

//from MLayuoutOwner

// ---------------------------------------------------------------------------
// CPeninputServer::BitmapContext
// Get bitmap context
// ---------------------------------------------------------------------------
//
CBitmapContext* CPeninputServer::BitmapContext()
    {
    return iSpriteGc;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::BitmapDevice
// Get bitmap device
// ---------------------------------------------------------------------------
//
CFbsBitmapDevice* CPeninputServer::BitmapDevice()
    {
    return iBitmapDevice;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::MaskBmpDevice
// Get mask bitmap device
// ---------------------------------------------------------------------------
//
CFbsBitmapDevice* CPeninputServer::MaskBmpDevice()
    {
    return iMaskBitmapDevice;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::Hide
// Hide/show sprite
// ---------------------------------------------------------------------------
//
void CPeninputServer::Hide(TBool aHideFlag)
    {
    if(aHideFlag)
        {
        DeactivateSprite();
        }
    else
        {
        ActivateSprite();
        }
    }

// ---------------------------------------------------------------------------
// CPeninputServer::LayoutSizeChanged
// Handle layout size change
// ---------------------------------------------------------------------------
//
TInt CPeninputServer::LayoutSizeChanged(const TSize& aSize, TBool aNeedUpdate)
	{
	TInt ret = KErrNone;
	if(iUiLayout)
	    {
        //This is the tricky place. We need to deactivate and activate the sprite,
        //otherwise blank area appears.
        
        TSize layoutSize = aSize;
        TRect layoutRect = iUiLayout->Init();
        layoutSize = layoutRect.Size();

    	layoutSize.iWidth = layoutSize.iWidth > aSize.iWidth ? 
    	                    layoutSize.iWidth : aSize.iWidth;
    	                    
    	layoutSize.iHeight = layoutSize.iHeight > aSize.iHeight ? 
    	                    layoutSize.iHeight : aSize.iHeight;

        if(iSpriteSize != layoutSize)
            {
            //deactive sprite first 
            TBool bRestoreActive = iActive;                      
            //DeactivateSprite();	            
            iSpriteSize = layoutSize;        
            //iBitmapDevice->Resize( layoutSize);
            //iMaskBitmapDevice->Resize( layoutSize);  
            ResizeDrawingDevice(layoutSize);    
		    if (aNeedUpdate)
		    	{
		    	FlushWsSession();
		    	}
            if(iAnimObj)
                {
                iAnimObj->AddChangeSizeCmd(layoutSize);
                }

            if(bRestoreActive )
                {
                //ActivateSprite();
                }
            }

        if (iUseWindowCtrl)
            {            
            iPenUiCtrl->SetExtent(LayoutRect().iTl,layoutSize);
            //DrawSprite();
            }
	    }
	return ret;	    
	} 

void CPeninputServer::DimPenUi()
    {
    if(iDimmed)                        
        {
        RecoverButtonEventState();         
        DeactivateDimPenUiLayout(EFalse);
        if(iUseWindowCtrl)
            {
            iPenUiCtrl->DimPenUiForMenu();
            }
        }
    else
        {
        ActivatePenUiLayout(EFalse);
        }
    }
    
void CPeninputServer::DimPenInternalUi(TBool /*aFlag*/,TInt aPriority)
    {
    if(iDimmed)                        
        {
        RecoverButtonEventState();        
        DeactivateDimPenUiLayout(EFalse);
        
        if(iUseWindowCtrl)
            {
            iPenUiCtrl->DimInternalPenUiForMenu(aPriority);
            
            }        
        }
    else
        {
        ActivatePenUiLayout(EFalse);
        }
    }    

void CPeninputServer::DimPenUiForMenu(TInt aPriority)
    {
    if(iUseWindowCtrl)
        {
        iPenUiCtrl->DimPenUiForMenu(aPriority);
        }
    }
// ---------------------------------------------------------------------------
// CPeninputServer::SignalOwner
// Signal owner
// ---------------------------------------------------------------------------
//
void CPeninputServer::SignalOwner(TInt aEventType, const TDesC& aEventData)
    {    
    //send fep related event to observer ( application)
    if(ESignalFepEventBase <= aEventType) 
        {
        TRAP_IGNORE(SignalObserverL(aEventType,aEventData));        
        }
    else    
        {
        switch(aEventType)
        	{
        	case ESignalCapturePointer: //request capture pointer
	            {	 
	            TUint16* buf =   const_cast<TUint16* >( aEventData.Ptr() );
	            TBool flag = *(TBool*)aEventData.Ptr();
	            TInt captureID = *(TInt* )( buf + sizeof(TUint16));
	            if(iAnimObj)
	                {
	                iAnimObj->AddCapturePointerCmd(flag, captureID);
	                }
	            }        	
        		break;
        		
        	case ESignalSimulateEvent: //request simulate event
                {
                //TBool* flag = (TBool*)aEventData.Ptr();
                const TRawEvent* event = reinterpret_cast<const TRawEvent*>(aEventData.Ptr());
                TRawEvent* modifyEvent = const_cast<TRawEvent*>( event );
                modifyEvent->SetTip( ETrue );
	            if(iAnimObj)
	                {
	                iAnimObj->AddSimulateEventCmd(*event);
	                }
                if( event->Type() == TRawEvent::EKeyDown )
                    {
    			    static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->
    			         DisableNextKeySound( event->ScanCode() ); 
                    }
                iLastSimulatedKeyEvent = *event;
                }
                break;
        	case ESignalImmeSimulateEvent:
        		{
                const TRawEvent* event = reinterpret_cast<const TRawEvent*>( aEventData.Ptr() );
                TRawEvent* modifyEvent = const_cast<TRawEvent*>( event );
                modifyEvent->SetTip( ETrue );                
	            if(iAnimObj)
	                {
	                iAnimObj->ExecuteSimulateEventCmd(*event);
	                }          
                
                if( event->Type() == TRawEvent::EKeyDown )
                    {
    			    static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->
    			         DisableNextKeySound( event->ScanCode() ); 
                    }
                iLastSimulatedKeyEvent = *event;        		
        		}
        		break;
#ifdef RD_TACTILE_FEEDBACK                 
      	    case ESignalRegisterFeedbackArea:
      	        {
      	        if(!iSupportFeedback)
      	            return;
                AddFeedbackArea(FeedbackData(aEventData));
      	        }
      	        break;
       	    case ESignalDeRegisterFeedbackArea:
       	        {
      	        if(!iSupportFeedback)
      	            return;
  
                RemoveFeedbackArea(FeedbackData(aEventData));       	        
       	        }
       	        break;
       	    case ESignalChangeFeedbackArea:
    	        {
      	        if(!iSupportFeedback)
      	            return;    	        
  
                ChangeFeedbackArea(FeedbackData(aEventData));      	        
       	        }
       	        break;
       	    case  ESignalChangeFeedbackType:
       	    	{
      	        if(!iSupportFeedback)
      	            return;    	        
  
                ChangeFeedbackType(FeedbackData(aEventData));        	    	
       	    	}
				break;
       	      
#endif // RD_TACTILE_FEEDBACK         		
        	    case ESignalGetAllowedInputMode:
        	        {
       	        TInt disabled = 0;
       	         if ( iForegroundSession )
        	            {
        	            //GetProcessLayouts( iForegroundSession->ClientProcessId(),disabled );
        	            disabled = iForegroundSession->DisabledUiType() | DisabledByOrientation();
        	            
        	            if( iForegroundSession->DsaEverHappened() )
        	            	{
        	            	disabled |= DisabledByDSAFromAnim();	
        	            	}
        	            }
                 TInt allowed = SetDisabledLayouts( disabled  );  
                 TUint16* buf =   const_cast<TUint16* >( aEventData.Ptr() );    
                 TInt* retVal = reinterpret_cast< TInt* > ( buf );
                  *retVal = allowed;
        	        }
        	    break;
        	
        	case ESignalPopupWnd:
        	    {
                if(iUseWindowCtrl)
                    {
                    TUint16* buf = const_cast<TUint16*>( aEventData.Ptr() );
                    TRect* rects = reinterpret_cast<TRect*>( buf );
            	    TRect rectInLayout = *rects;
            	    TRect rectTrimLayout = *(rects+1);
            	    TRect rectInScreen = rectInLayout;
            	    rectInScreen.Move( iSpritePosition );
            	    
            	    iPenUiCtrl->ShowPopup( rectInScreen, rectInLayout, rectTrimLayout );
            	    }
        	    }
        	    break;

        	case ESignalPopupWndClosed:
        	    {        	    
        	    if(iUseWindowCtrl)
        	        {
        	        iPenUiCtrl->ClosePopup();
        	        }
        	    }
        	    break;
        	default:
        		break;
        	}
        }

    }
    
// ---------------------------------------------------------------------------
// CPeninputServer::LayoutRect
// Get sprite size and position
// ---------------------------------------------------------------------------
//
TRect CPeninputServer::LayoutRect()
    {
    return TRect(iSpritePosition,iSpriteSize);
    }

// ---------------------------------------------------------------------------
// CPeninputServer::SetPosition
// called when sprite has been moved
// ---------------------------------------------------------------------------
//

TPoint CPeninputServer::SetPosition(const TPoint& aNewPos)
    {
    if(iSpritePosition != aNewPos)
        {        
        iSpritePosition = aNewPos;

        if(iUseWindowCtrl)
            iPenUiCtrl->SetPosition(aNewPos);
        
        if(iAnimObj)
            {
            iAnimObj->AddSetPosCmd(iSpritePosition);
            }
        }
    return iSpritePosition;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::FlushTimerCallBack
// CPeriodic callback function: Flush window session
// ---------------------------------------------------------------------------
//
TInt CPeninputServer::FlushTimerCallBack(TAny *aPtr)
    {
    CPeninputServer* self = static_cast<CPeninputServer*>(aPtr); 
           
    self->FlushWsSession();
        
	return EFalse;        
    }

// ---------------------------------------------------------------------------
// CPeninputServer::FlushWsSession
// Flush window session
// ---------------------------------------------------------------------------
//    
void CPeninputServer::FlushWsSession()
    {
    if (iCurRectNumInBuf)
        {
        if(iAnimObj)
            {
            iAnimObj->AddUpdateUiCmd(iFlushArea,iCurRectNumInBuf);
            }

        iCurRectNumInBuf = 0;
            {        	
	        iFullUpdate = EFalse;
	          //cancel the timer
	          iFlushTimer->Cancel();        
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputServer::DoIdleConstructL
// Do idle construction.
// ---------------------------------------------------------------------------
//    
void CPeninputServer::DoIdleConstructL()
    {   
    if(!iPtiEngine)
        {
        iPtiEngine = CPtiEngine::NewL(ETrue);
        }
        
    if(!iLayoutEng)
        {
        iLayoutEng = CFepLayoutEngine::NewL(iPtiEngine);    
        }
        
    //this will only do once
    }

// ---------------------------------------------------------------------------
// CPeninputServer::HandleResourceChange
// Handle application resource change
// ---------------------------------------------------------------------------
//    
void CPeninputServer::HandleResourceChange(TInt aType)
    {   
    if(iUiLayout && !(iUiLayout->PenInputType() & DisabledByOrientation()) )
        {
        //hide the layout if it's already shown
        
        iUiLayout->OnResourceChange(aType);
        //show the layout if it's active
        }
    }

// ---------------------------------------------------------------------------
// CPeninputServer::GetImePluginIdListL
// Get IME plugin implementation id list
// ---------------------------------------------------------------------------
//    
void CPeninputServer::GetImePluginIdListL(const RMessage2& aMessage)
    {
    TInt language = aMessage.Int0();
    TInt mode = aMessage.Int1();
    TInt maxlen = aMessage.Int2();
    
    const RArray<TInt>& implList = iLayoutEng->ImePluginListL(language, mode);
    // +1 means there is extra position for count
    TInt count = implList.Count() > (maxlen/sizeof(TInt) - 1)? 
                                    (maxlen/sizeof(TInt) - 1): implList.Count();
    
	HBufC8* buf = HBufC8::NewLC((count + 1)*sizeof(TInt));
	TPtr8 bufPtr = buf->Des();
	RDesWriteStream writeStream;
	writeStream.Open(bufPtr);
	CleanupClosePushL(writeStream);
	writeStream.WriteInt32L(count);
    for(TInt ii = 0; ii < count; ++ii)
        {
        writeStream.WriteInt32L(implList[ii]);
        }
    writeStream.CommitL();
    
    aMessage.WriteL(KMsgSlot3, *buf);
    
    CleanupStack::PopAndDestroy(&writeStream);
    CleanupStack::PopAndDestroy(buf);
    }

// ---------------------------------------------------------------------------
// CPeninputServer::GetPenSupportLanguagesL
// Get touch input support language lists
// ---------------------------------------------------------------------------
//    
void CPeninputServer::GetPenSupportLanguagesL(const RMessage2& aMessage)
    {
    TInt maxlen = aMessage.Int0();
    
    const RArray<TInt>& implList = iLayoutEng->TouchSupportLanguagesL();
    // +1 means there is extra position for count
    TInt count = implList.Count() > (maxlen/sizeof(TInt) - 1)? 
                                    (maxlen/sizeof(TInt) - 1): implList.Count();
    
	HBufC8* buf = HBufC8::NewLC((count + 1)*sizeof(TInt));
	TPtr8 bufPtr = buf->Des();
	RDesWriteStream writeStream;
	writeStream.Open(bufPtr);
	CleanupClosePushL(writeStream);
	writeStream.WriteInt32L(count);
    for(TInt ii = 0; ii < count; ++ii)
        {
        writeStream.WriteInt32L(implList[ii]);
        }
    
    writeStream.CommitL();
    aMessage.WriteL(KMsgSlot1, *buf);
    CleanupStack::PopAndDestroy(&writeStream);
    CleanupStack::PopAndDestroy(buf);
    }

// ---------------------------------------------------------------------------
// CPeninputServer::PropertyChangeCallBack
// Handle proproty change
// ---------------------------------------------------------------------------
//    
TInt CPeninputServer::PropertyChangeCallBack(TAny* aData)
    {
    TRAP_IGNORE(static_cast<CPeninputServer*>(aData)->HideLayoutTemporaryL());
    return 0;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::HideLayoutTemporary
// Temporarily hide the layout
// ---------------------------------------------------------------------------
//    
void CPeninputServer::HideLayoutTemporaryL()
    {
/*    if (iUseWindowCtrl)
        {
        if (iActive && iUiLayout && 
            iUiLayout->PenInputType() == EPluginInputModeItut)
            {
            iUiLayout->HandleCommand(ECmdPenInputRotation, NULL);
            ClearSpriteGc();
            iPenUiCtrl->ClosePenUi(ETrue);
            iActive = EFalse;
            }
        return;    
        }
*/    if(iActive && !iPrevActive && 
       iUiLayout->PenInputType() != EPluginInputModeFSQ && 
       iBackgroudDefaultOri == CAknAppUiBase::EAppUiOrientationUnspecified )
        {
        
        iPrevActive = ETrue;
        ClearSpriteGc();
	    //Close the UI immediately, without notify UI handler 
	    DeactivateSprite(ETrue, ETrue);
        }    
    }
    
// ---------------------------------------------------------------------------
// CPeninputServer::SetDisabledLayouts
// Temporarily hide the layout
// ---------------------------------------------------------------------------
//    
TInt CPeninputServer::SetDisabledLayouts( TInt aDisabledLayouts )
    {
    // No spcified logic now, so just store the disabled layout in the CFepUiLayoutBase class.
    TInt allowedPlugin = EPluginInputModeNone;// EPluginInputModeHwr | EPluginInputModeVkb | EPluginInputModeItut | EPluginInputModeFSc;
    
    CRepository* repository = NULL;
    TRAPD(ret, repository = CRepository::NewL(KCRUidAknFep));
    if (ret != KErrNone)
        {
        return EPluginInputModeAll;
        }
    TInt curLanguage ;
    repository->Get(KAknFepInputTxtLang, curLanguage);
    delete repository;
    
    if (curLanguage == 401) curLanguage = 102;
    if (curLanguage == 402) curLanguage = 103;

    
    TInt testMode = EPluginInputModeHwr;
    while ( testMode < EPluginInputModeAll )
        {
        if ( iLayoutEng->IsSupportPluginMode( ( TLanguage )curLanguage, 
                                              ( TPluginInputMode )testMode )
             &&  !( aDisabledLayouts & testMode ) )
            {
            allowedPlugin |= testMode;
            }
        testMode<<=1;
        }
    return  allowedPlugin;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::GetProcessLayouts
// ---------------------------------------------------------------------------
//    
TInt CPeninputServer::GetProcessLayouts( const TProcessId& aProcessId, TInt& aLayouts )
    {
    for ( int i = 0 ; i < iClientProcess.Count() ; i ++ )
        {
        if ( aProcessId == iClientProcess[i] )
            {
            aLayouts = iClientLayouts[i];
            return KErrNone;
            }
        }
    return KErrNotFound;    
    }
    

// ---------------------------------------------------------------------------
// CPeninputServer::SetProcessLayouts
// ---------------------------------------------------------------------------
//        
TInt CPeninputServer::SetProcessLayouts( const TProcessId& aProcessId, TInt aLayouts)
    {
    
    for ( int i = 0 ; i < iClientProcess.Count() ; i ++ )
    {
         if ( aProcessId == iClientProcess[i] )
            {
            iClientLayouts[i] = aLayouts;
            return KErrNone;
            }
    }
    // not found, Insert 
    iClientProcess.Append( aProcessId );
    iClientLayouts.Append( aLayouts );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CPeninputServer::DelProcessLayouts
// ---------------------------------------------------------------------------
//        
void CPeninputServer::DelProcessLayouts( const TProcessId& /*aProcessId*/ )
    {
    }
        
void CPeninputServer::ExecutePendingAnimCmd()
    {
    if( iAnimObj )
        {
        iAnimObj->ExeOnePendingAnimCmd();
        }
    }

void CPeninputServer::SetForeGroundUiHandlerL()
    {
    if(iForegroundUiHandler)
        {
        iForegroundUiHandler->ProcessPendingEventBeforeExit();        
        iForegroundUiHandler = NULL;
        }
        
    TBool bContinue = ETrue;
    for(TInt i = iUiHandlerArray.Count()-1 ; i >= 0 && bContinue; i--)
        {
        if( (iForegroundSession == iUiHandlerArray[i]) // same session
            || (iForegroundSession->ClientId() == iUiHandlerArray[i]->ClientId())) //same thread
            {
            iForegroundUiHandler = iUiHandlerArray[i];
            bContinue = EFalse; //found, break the loop
            }
        else
            {            
            //check whether it's from same process
            bContinue = !IsSameProcess(iForegroundSession,iUiHandlerArray[i]);
            }        
        }
    }    
   
    
// ---------------------------------------------------------------------------
// CPeninputServer::HandleRawEventL
// Handle event from event queue, i.e, from animation dll
// ---------------------------------------------------------------------------
//
void CPeninputServer::HandleRawEventL(const TRawEvent& aEvent)
    {
    if(iActive)
        { 
        if( aEvent.Type() == TRawEvent::ENone && 
        	aEvent.IsTip() &&
        	iCrpService->IsDsaActive() )
            {
            SignalOwner( ESignalLayoutClosed, KNullDesC );    
            }
                   
        TBool handled = iUiLayout->HandleEventL(ERawEvent,&aEvent);

        if (TRawEvent::EButton1Down == aEvent.Type())
            {
            iLastRawEvent = aEvent;     
            }
        else if (TRawEvent::EPointerMove != aEvent.Type())
            {
            iLastRawEvent.Set(TRawEvent::EButton1Up);    
            }
          
        if(aEvent.Type() == TRawEvent::EKeyDown || 
                            aEvent.Type() == TRawEvent::EKeyUp)
            {
            //write response to response queue
            iKeyMsgResponseQueue.Send(handled);
            //signal the semaphore
            iKeyEventSemaphore.Signal();
            
            }
        }
    }
    

// ---------------------------------------------------------------------------
// CPeninputServer::HandleRawEventL
// Handle event from window server
// ---------------------------------------------------------------------------
//    
void CPeninputServer::HandleWsEventL(const TWsEvent &aEvent, 
                                                CCoeControl* /*aDestination*/)
    {    
    switch(aEvent.Type())
        {					             
        case EEventWindowGroupListChanged:
                {
    	        iCurScreenFocusedWndGrpId = GetFocusAppUid().iUid;               
                }
                break;
	    case EEventFocusGroupChanged:
    	    {
            TUid focusApp = GetFocusAppUid();
            if(IsGlobalNotesApp(focusApp))
                {
                iInGlobalNotesApp = ETrue;   
                }
            else
                {
                iInGlobalNotesApp = EFalse;    
                }
            if(!(iUiLayout && iActive ))
                return;
            
            if(iUseWindowCtrl)
                {
/*                //Switch to akncapserver or leave 
                if(iUiLayout->PenInputType() == EPluginInputModeItut ||
                   iUiLayout->PenInputType() == EPluginInputModeFSQ )
                    {
                    DeactivatePenUiLayout(EFalse);
                    iPenUiCtrl->DimPenUiForMenu();
                    break;
                    }
                */
                if(IsGlobalNotesApp(focusApp))
                        {
                        iInGlobalNotesState = ETrue; 	             
                        break;            
                        }
                    
/*                else if (focusApp.iUid == 0x102750f0)
                    {
                    DeactivateSprite();    
                    }
*/                else
                    {
                        if(iInGlobalNotesState)
                            {			  
                            //fix for fast swap case
                        iInGlobalNotesState = EFalse;                        
                        if(iPreNonGloebalNotesWndGrpId != focusApp.iUid)
                            {                            
                            iPreNonGloebalNotesWndGrpId = focusApp.iUid;    
             
                            DeactivateSprite(ETrue);//hide pen ui immediately if switched to another application
                            iForegroundSession = NULL;
                            }
                        }
                    else
                        {
                        DeactivateSprite(ETrue);
                        }
                    }
                        break;                            
                }   
            DeactivateSprite(ETrue);//hide pen ui immediately
    	    }
        break;		    
        default:
            break;
	    }
    
    }

#ifdef RD_TACTILE_FEEDBACK
// ---------------------------------------------------------------------------
// CPeninputServer::AddFeedbackArea
// add the feedback area
// ---------------------------------------------------------------------------
//
void CPeninputServer::AddFeedbackArea(const TTactileFeedbackArea& aAreaData)
    {    
    if( iDestroyingLayout || !iAnimObj )  
        {
        return;
        }
    iAnimObj->AddRegisterFeedbackAreaCmd(aAreaData,iUILayoutReady);    
    }

// ---------------------------------------------------------------------------
// CPeninputServer::RemoveFeedbackArea
// remove the feedback area
// ---------------------------------------------------------------------------
//    
void CPeninputServer::RemoveFeedbackArea(const TTactileFeedbackArea& aAreaData)
    {    
    if( iDestroyingLayout || !iAnimObj )  
        {
        return;
        }
    iAnimObj->AddDeRegisterFeedbackAreaCmd(aAreaData,iUILayoutReady);
    }    

// ---------------------------------------------------------------------------
// CPeninputServer::ChangeFeedbackArea
// change the feedback area
// ---------------------------------------------------------------------------
//
void CPeninputServer::ChangeFeedbackArea(const TTactileFeedbackArea& aAreaData)
    {   
    if( iDestroyingLayout || !iAnimObj )  
        {
        return;
        }
    iAnimObj->AddChangeFeedbackAreaCmd(aAreaData,iUILayoutReady); 
    }    

// ---------------------------------------------------------------------------
// CPeninputServer::ChangeFeedbackType
// change the feedback type
// ---------------------------------------------------------------------------
//
void CPeninputServer::ChangeFeedbackType(const TTactileFeedbackArea& aAreaData)
    {   
    if( iDestroyingLayout || !iAnimObj)  
        {
        return;
        }
    iAnimObj->AddChangeFeedbackTypeCmd(aAreaData,iUILayoutReady); 
    } 

// ---------------------------------------------------------------------------
// CPeninputServer::FindFeedbackArea
// Find the feedback area
// ---------------------------------------------------------------------------
//
TInt CPeninputServer::FindFeedbackArea(const TTactileFeedbackArea& aArea)
    {
    for(TInt i = 0; i < iFeedbackAreaArray.Count(); ++i)
        {
        TTactileFeedbackArea temp = iFeedbackAreaArray[i];
        if(iFeedbackAreaArray[i].iId == aArea.iId)
            return i;
        }
    return KErrNotFound;
    }

#endif // RD_TACTILE_FEEDBACK
    
// ---------------------------------------------------------------------------
// CPeninputServer::AnimOpNeedWaiting
// Check command for this type of event whether needs waiting.
// ---------------------------------------------------------------------------
//
TBool CPeninputServer::AnimOpNeedWaiting(TInt aSignalCode)
	{
    TBool bNeedWait = EFalse;
    if(ESignalPenUiDeActivated == aSignalCode)
        {
        return EFalse;
        }
    if(iForegroundUiHandler)
        {
        TInt uiType = iUiLayout->PenInputType();
        bNeedWait = iForegroundUiHandler->SignalUiActivationObserver(
                                            			aSignalCode,uiType);
        }	
    return bNeedWait;
	}

// ---------------------------------------------------------------------------
// CPeninputServer::RecoverSimulatedKeyEventState
// ---------------------------------------------------------------------------
//    
void CPeninputServer::RecoverSimulatedKeyEventState()
    {
    if(!iAnimObj)
        {
        return;
        }
    
    if(iLastSimulatedKeyEvent.Type() == TRawEvent::EKeyDown)
        {
        //key up event is missing, we must manually simulate the up event.
        iLastSimulatedKeyEvent.Set(TRawEvent::EKeyUp,iLastSimulatedKeyEvent.ScanCode());
        iAnimObj->AddSimulateEventCmd(iLastSimulatedKeyEvent);        
        }
    }

void CPeninputServer::RecoverButtonEventState()
    {
        if (TRawEvent::EButton1Down == iLastRawEvent.Type())
            {
            iLastRawEvent.Set(TRawEvent::EButton1Up);//,iLastSimulatedKeyEvent.ScanCode()
            TRAP_IGNORE( iUiLayout->HandleEventL(ERawEvent,&iLastRawEvent) ); 
            }
    }
    
void CPeninputServer::EnsureUpdateBuffer()
    {
    if(!iAnimObj)
        {
        return;
        }
    
    if(iCurRectNumInBuf >= KMaxRectNumInBuf-1)
        {
        //flush previous buffer        
        iAnimObj->AddUpdateUiCmd(iFlushArea,iCurRectNumInBuf);
        iCurRectNumInBuf = 0;        
        }
    }

inline TBool ContainsEachOther(const TRect aRt1, const TRect aRt2 )
    {
    TRect rt(aRt1);
    rt.BoundingRect( aRt2 );
    return rt == aRt1 || rt == aRt2;
    }
    
void CPeninputServer::PackFlushArea()
    {
    for( TInt i = iCurRectNumInBuf - 1; i > 0; i-- )
        {
        for( TInt j = i -1; j >=0; j-- )
            {
            if( ContainsEachOther(iFlushArea[i].iRect, iFlushArea[j].iRect) ) 
                {
                iFlushArea[j].iRect.BoundingRect(iFlushArea[i].iRect);
                iCurRectNumInBuf--;
                }
            }
        }
    }
    
void CPeninputServer::AddFlushArea(const TUpdateArea& aArea)
    {       
    for(TInt i = 0 ; i < iCurRectNumInBuf; ++i)
        {
        if(iFlushArea[i].iRect == aArea.iRect)
            {
            iFlushArea[i].iFullUpdateFlag |= aArea.iFullUpdateFlag;
            return;
            }
        else if( iFlushArea[i].iFullUpdateFlag == aArea.iFullUpdateFlag )
            {
            TRect rt(aArea.iRect);
            rt.BoundingRect(iFlushArea[i].iRect);
            if( rt == iFlushArea[i].iRect || rt == aArea.iRect )
                {
                iFlushArea[i].iRect = rt;
                //PackFlushArea();
                return;
                }
            }
        }
    //Ensure buffer can hold new data
    EnsureUpdateBuffer();    
    iFlushArea[iCurRectNumInBuf++] = aArea;
    }
void CPeninputServer::PrepareExit()
    {
    CleanAll();
    }

// ---------------------------------------------------------------------------
// CPeninputServer::MonitorWsGroupEvent
// Request to enable or disable window group change event    
// ---------------------------------------------------------------------------
//    
void CPeninputServer::MonitorWsGroupEvent(TBool aMonitorFlag)
    {
    RWindowGroup& rootWin = CCoeEnv::Static()->RootWin();    
    if(aMonitorFlag)
        {
    	rootWin.EnableFocusChangeEvents (); // For cover UI/status pane refresh problems
    	//rootWin.EnableGroupListChangeEvents();
        }
    else
        {
    	rootWin.DisableFocusChangeEvents (); // For cover UI/status pane refresh problems
    	//rootWin.DisableGroupListChangeEvents();
        }
    }

void CPeninputServer::CheckSessionValidL(CPeninputServerSession* aSession1,
                                     CPeninputServerSession* aSession2) const
    {
    __ASSERT_DEBUG(aSession1 && aSession2, User::Leave(ENullSession));    
    }

// ---------------------------------------------------------------------------
// CPeninputServer::IsSameProcess
// Tests whether two sessions from same process    
// ---------------------------------------------------------------------------
//            
inline TBool CPeninputServer::IsSameProcess(CPeninputServerSession* aSession1,
                                        CPeninputServerSession* aSession2) const
    {
    TRAP_IGNORE(CheckSessionValidL(aSession1, aSession2));
    return aSession1->ClientProcessId().Id() == aSession2->ClientProcessId().Id();
    }
// ---------------------------------------------------------------------------
// CPeninputServer::IsForegroundSession
// Tests whether current session is the foreground session.
// ---------------------------------------------------------------------------
//        			  
inline TBool CPeninputServer::IsForegroundSession(CPeninputServerSession* aSession) const
    {
    if(!iForegroundSession)
        return EFalse;
    
    if(iForegroundSession == aSession)
        return ETrue;
    
    //same thread?
    if(iForegroundSession->ClientId() == aSession->ClientId())
        return ETrue;
    
    //check whether the session is from the same process of foreground session
    return IsSameProcess(iForegroundSession,aSession);
    }

// ---------------------------------------------------------------------------
// CPeninputServer::ResizeDrawingDevice
// Resize the drawing device
// ---------------------------------------------------------------------------
//        			      
void CPeninputServer::ResizeDrawingDevice(const TSize& aSize)
    {
    iSpriteMember.iBitmap->Resize(aSize);
    iSpriteMember.iMaskBitmap->Resize(aSize);   

    iBitmapDevice->Resize( aSize);
    iMaskBitmapDevice->Resize( aSize);    

    //gc must be adjusted
    iSpriteGc->Activate(iBitmapDevice);
    iSpriteGc->Resized();
    }
    
TInt CPeninputServer::HandleAppInfoChange(CPeninputServerSession* aSession,
                           const TDesC& aAppInfo, TPeninputAppInfo aType) const
    {
    if(!iUiLayout)
        return KErrNoLayout;            
    
    TInt err = KErrNone;
    if(IsForegroundSession(aSession))
        {                
        iUiLayout->HandleAppInfoChange(aAppInfo,aType);                            
        }
    else        
        {
        err = KErrNotForegroundSession;
        }
    return err;
    }

void CPeninputServer::BackgroudDefaultOri( TInt aOri)
    {
    iBackgroudDefaultOri = aOri;
    }    


TInt CPeninputServer::DisabledByDSA()
    {
    TInt disabled = 0;
    TBool dsaOn = iCrpService->IsDsaActive();
    //iAnimObj->GetDSAState(dsaOn);
    if(dsaOn)
        {
        disabled = EPluginInputModeVkb | EPluginInputModeHwr | EPluginInputModeFSc;
        }
    return disabled;
    }
    
TInt CPeninputServer::DisabledByDSAFromAnim()
    {
    TInt disabled = 0;
    TBool dsaOn = EFalse;
    if(!iAnimObj)
        {
        return disabled;
        }
    
    iAnimObj->GetDSAState(dsaOn);
    if(dsaOn)
        {
        disabled = EPluginInputModeVkb | EPluginInputModeHwr | EPluginInputModeFSc;
        }
    return disabled;
    }
TInt CPeninputServer::DisabledByOrientation()
    {
    TInt disabled = 0;
    TBool sensorOn = EFalse;
    if ( iSensorRepository )
        {
        iSensorRepository->Get( KSenSettingsSensorsOn, sensorOn );
        if ( sensorOn )
            {
            TInt turnCtrl = 0;
            iSensorRepository->Get( KSenSettingsTurnCtrl, turnCtrl );
            turnCtrl = turnCtrl >> 2;
            
            if ( turnCtrl ) 
                {
                TPixelsTwipsAndRotation size; 
                CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
            
                if( size.iPixelSize.iWidth < size.iPixelSize.iHeight )
                    {
                    return disabled |= EPluginInputModeFSQ;
                    }
                if( size.iPixelSize.iWidth > size.iPixelSize.iHeight )
                    {
                    return disabled |= EPluginInputModeItut;
                    }
                }
            }
        }
    return disabled;
    }

TInt CPeninputServer::GetSupportModeL()
    {
    CRepository* repository = NULL;
    TRAPD(ret, repository = CRepository::NewL(KCRUidAknFep));
    if (ret != KErrNone)
        {
        return ret;
        }
    TInt curLanguage ;
    repository->Get(KAknFepInputTxtLang, curLanguage);
    delete repository;
    repository = NULL;
    
    if (curLanguage == 401) curLanguage = 102;
    if (curLanguage == 402) curLanguage = 103;    	    
    
    
    TInt supportMode = EPluginInputModeNone;
    TInt tempMode = EPluginInputModeHwr;
    
    iLayoutEng->InitializeL();
    
    while ( tempMode < EPluginInputModeAll )
        {
        if ( iLayoutEng->IsSupportPluginMode( ( TLanguage )curLanguage, 
                                              ( TPluginInputMode )tempMode ) )
            {
            supportMode |= tempMode;
            }
        tempMode<<=1;
        }    
    return supportMode;
    }
    
// ======== class CEventQueue========
//
// ---------------------------------------------------------------------------
// CEventQueue::CEventQueue
// Constructor. Adds the active object to the active scheduler
// and issues a request for server notification.
// ---------------------------------------------------------------------------
//

CEventQueue::CEventQueue(MRawEventHandler* aHandler):CActive( EPriorityHigh ),
                                                     iHandler(aHandler)
    {
    // issue asynchronous request and set this object active
    CActiveScheduler::Add( this );        
    }

// ---------------------------------------------------------------------------
// CEventQueue::NewL
// factory constructor. 
// ---------------------------------------------------------------------------
//
CEventQueue* CEventQueue::NewL(MRawEventHandler* aHandler,const TDesC& aName)
    {
    CEventQueue* queue = new(ELeave)CEventQueue(aHandler);
    CleanupStack::PushL(queue);
    queue->ConstructL(aName);
    CleanupStack::Pop(queue);
    return queue;
    }

// ---------------------------------------------------------------------------
// CEventQueue::ConstructL
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CEventQueue::ConstructL(const TDesC& aName)
    {
    //create message queue
    User::LeaveIfError(iMsgBufQueue.CreateGlobal(aName,KMsgQueueLen));
    }
// ---------------------------------------------------------------------------
// CEventQueue::CEventQueue
// Destructor. Cancels any outstanding requests
// ---------------------------------------------------------------------------
//
CEventQueue::~CEventQueue()
    {
    Cancel();
    iMsgBufQueue.Close();
    }

// ---------------------------------------------------------------------------
// CEventQueue::DoCancel
// Cancels the notification requests
// ---------------------------------------------------------------------------
//
void CEventQueue::DoCancel()
    {
    iMsgBufQueue.CancelDataAvailable();
    }


// ---------------------------------------------------------------------------
// CEventQueue::RunL
// ---------------------------------------------------------------------------
//
void CEventQueue::RunL()
    {    
    //iHandler is always there. No need to check it
    TRawEventBuffer events;
    iMsgBufQueue.Receive(events);
    for(TInt i = 0; i < events.iNum; i++)
        {
        iHandler->HandleRawEventL(events.iEvent[i]);
        }
    // re-issue request
    GetEvent();
    }

// ---------------------------------------------------------------------------
// CEventQueue::RunError
// ---------------------------------------------------------------------------
//
TInt CEventQueue::RunError(TInt /*aError*/)
    {    
    // re-issue request
    GetEvent();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CEventQueue::GetEvent
// Reissue request for event
// ---------------------------------------------------------------------------
//
void CEventQueue::GetEvent()
    {
    if(IsActive())
        return;
    iMsgBufQueue.NotifyDataAvailable(iStatus);
    SetActive();
    }


// ======== class CAknLayoutChangeWatcher========
//

// ---------------------------------------------------------------------------
// CAknLayoutChangeWatcher::NewL
// Factory constructor. 
// ---------------------------------------------------------------------------
//
CAknLayoutChangeWatcher* CAknLayoutChangeWatcher::NewL(
                                        CPeninputServer* aHandler,TUint aKey)
    {
    CAknLayoutChangeWatcher* self = new(ELeave)CAknLayoutChangeWatcher(
                                                            aHandler,aKey);
    CActiveScheduler::Add(self);    
    return self;
    }

// ---------------------------------------------------------------------------
// CAknLayoutChangeWatcher::~CAknLayoutChangeWatcher
// Destructor
// ---------------------------------------------------------------------------
//    
CAknLayoutChangeWatcher::~CAknLayoutChangeWatcher()    
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// CAknLayoutChangeWatcher::CAknLayoutChangeWatcher
// Default constructor
// ---------------------------------------------------------------------------
//
CAknLayoutChangeWatcher::CAknLayoutChangeWatcher(CPeninputServer* aHandler,
                         TUint aKey)
                         : CActive( EPriorityHigh ),iHandler(aHandler)
    {
    iProperty.Attach(KPSUidUikon,aKey);
	iLayoutState = KInvalidValue;
    iOrientationState = KInvalidValue;  
    }

// ---------------------------------------------------------------------------
// CAknLayoutChangeWatcher::StartWatching
// Start to monitor
// ---------------------------------------------------------------------------
//    
void CAknLayoutChangeWatcher::StartWatching()
    {
    iProperty.Subscribe(iStatus);
    SetActive();
    }

// ---------------------------------------------------------------------------
// CAknLayoutChangeWatcher::DoCancel
// Cancels the notification requests
// ---------------------------------------------------------------------------
//
void CAknLayoutChangeWatcher::DoCancel()
    {
    iProperty.Cancel();
    }


// ---------------------------------------------------------------------------
// CAknLayoutChangeWatcher::RunL
// ---------------------------------------------------------------------------
//
void CAknLayoutChangeWatcher::RunL()
    {    
    TInt layoutstate = KInvalidValue;
    TInt orientstate = KInvalidValue;
    iProperty.Get(KPSUidUikon, KUikLayoutState, layoutstate);
    iProperty.Get(KPSUidUikon, KUikPreferredOrientation, orientstate);   
    if(layoutstate != iLayoutState || orientstate != iOrientationState)
            {
            // save the previous state
            iLayoutState = layoutstate;
            iOrientationState = orientstate;            

            // hide layout when monitered object change state
            iHandler->HideLayoutTemporaryL();
            }
    StartWatching();
    }

// ---------------------------------------------------------------------------
// CEventQueue::RunError
// ---------------------------------------------------------------------------
//
TInt CAknLayoutChangeWatcher::RunError(TInt /*aError*/)
    {    
    // re-issue request
    StartWatching();
    return KErrNone;
    }

// ======== GLOBAL FUNCTIONS ========
// ---------------------------------------------------------
// PanicServer
// ---------------------------------------------------------
//
GLDEF_C void PanicServer(TPeninputServerPanic aPanic)
    {
    User::Panic( KPeninputServerName, aPanic );
    } 
// End of File

