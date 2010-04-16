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


#include "peninputclient.h"
#include "peninputclientimpl.h"


// ---------------------------------------------------------------------------
// RPeninputServer::RPeninputServer
// ---------------------------------------------------------------------------
//
EXPORT_C RPeninputServer::RPeninputServer()
    {
    }

// ---------------------------------------------------------------------------
// RPeninputServer::Connect
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::Connect()
    { 
    TInt err;
    TRAP(err, iSingletonServer = RPeninputServerImpl::NewL());    
    if( !iSingletonServer )   
        {
        return KErrGeneral;
        }
    return err;
    /************************************************************************/
    // There might be several connect coming when server is not launched.    
    /************************************************************************/
    /*TInt error = KErrNone;
    CCoeStatic * obj = CCoeEnv::Static()->FindStatic(KSingletonClientId);
    CPenInputSingletonClient* client = 
                                static_cast<CPenInputSingletonClient*>(obj);
    if(client && client->IsValid())
        {
        RPeninputServerImpl* iSingletonServer = client->GetSingletonServer();
        if(iSingletonServer == 0 ) // second or later connecting from same client
            {
            error = KErrLaunchingServer;
            }
        }
    else
        {
        TRAP(error, iSingletonServer = RPeninputServerImpl::NewL(KSingletonClientId));
        }
    return error;    */
    }


// ---------------------------------------------------------------------------
// RPeninputServer::Close
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::Close()
    {
    if(iSingletonServer)
        iSingletonServer->Close();
    }

// ---------------------------------------------------------------------------
// RPeninputServer::Version
// ---------------------------------------------------------------------------
//
EXPORT_C TVersion RPeninputServer::Version(void) const
    {
    return iSingletonServer->Version() ;
    }



EXPORT_C void RPeninputServer::DimUiLayout(TBool aFlag)
    {
    iSingletonServer->DimUiLayout(aFlag);
    }
// ---------------------------------------------------------------------------
// RPeninputServer::ActivateLayout
// Activate/Deactivate a ui layout
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::ActivateLayout(TBool aFlag)
    {
    iSingletonServer->ActivateLayout(aFlag);
    }

EXPORT_C void RPeninputServer::ActivatePeninputInNotesL()
    {
    iSingletonServer->ActivatePeninputInNotesL();    
    }
// ---------------------------------------------------------------------------
// RPeninputServer::GetServerEventData
// Retrieve server event data
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::GetServerEventData(TDes& aBuf)
    {
    return iSingletonServer->GetServerEventData(aBuf);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::ServerEventDfataNum
// Get the number of server event
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::ServerEventDataNum()
    {  
    return iSingletonServer->ServerEventDataNum();    
    }

// ---------------------------------------------------------------------------
// RPeninputServer::ResourceChanged
// Tell server the client resource changed
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::ResourceChanged(TInt aType)
    {
    iSingletonServer->ResourceChanged(aType);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SetUiLayoutPos
// Set ui layout position
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::SetUiLayoutPos(const TPoint& aPos)
    {
    return iSingletonServer->SetUiLayoutPos(aPos); 
    }

// ---------------------------------------------------------------------------
// RPeninputServer::UiLayoutPos
// get ui layout position
// ---------------------------------------------------------------------------
//
EXPORT_C TPoint RPeninputServer::UiLayoutPos()
    {
    return iSingletonServer->UiLayoutPos();    
    }

// ---------------------------------------------------------------------------
// RPeninputServer::UiLayoutSize
// get ui layout size
// ---------------------------------------------------------------------------
//
EXPORT_C TSize RPeninputServer::UiLayoutSize()
    {
    return iSingletonServer->UiLayoutSize();    
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SetUiLayoutId
// Create a ui layout
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::SetUiLayoutId(TUid aId)
    {
    return iSingletonServer->SetUiLayoutId(aId); 
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SetUiLayoutId
// Create a ui layout
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::SetUiLayoutId(TUid aId,const TDesC8& aData)
    {
    return iSingletonServer->SetUiLayoutId(aId,aData);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::HandleCommand
// Ask ui layout handle command
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::HandleCommand(TInt aCmd,const TDesC8& aBuf)
    {
    return iSingletonServer->HandleCommand(aCmd,aBuf);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::HandleCommand
// Ask ui layout handle command
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::HandleCommand(TInt aCmd)
    {
    return iSingletonServer->HandleCommand(aCmd);       
    }

// ---------------------------------------------------------------------------
// RPeninputServer::HandleCommand
// Ask ui layout handle command
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::HandleCommand(TInt aCmd,const TDesC8& aBuf, TInt &aResult)
    {
    return iSingletonServer->HandleCommand(aCmd, aBuf, aResult);
    }
    
// ---------------------------------------------------------------------------
// RPeninputServer::IsVisible
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RPeninputServer::IsVisible()
    {
    return iSingletonServer->IsVisible();
    }
    
// ---------------------------------------------------------------------------
// RPeninputServer::IsVisible
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RPeninputServer::IsDimmed()
    {
    return iSingletonServer->IsDimmed();
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SupportInputMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::SupportInputMode()
    {
    return iSingletonServer->SupportInputMode();
    }

// ---------------------------------------------------------------------------
// RPeninputServer::AddPeninputServerObserverL
// Add observer
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::AddPeninputServerObserverL(
                                       MPeninputServerEventHandler* aHandler)
    {
    iSingletonServer->AddPeninputServerObserverL(aHandler);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::RemovePeninputServerObserver
// remove all observer
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::RemovePeninputServerObserver()
    {
    iSingletonServer->RemovePeninputServerObserver();
    }


// ---------------------------------------------------------------------------
// RPeninputServer::RemovePeninputServerObserver
// Remove an observer
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::RemovePeninputServerObserver(
                                         MPeninputServerEventHandler* aObserver)
    {
    iSingletonServer->RemovePeninputServerObserver(aObserver);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SetDisplayMode
// Set the display mode
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::SetDisplayMode(TDisplayMode aDisplayMode,
                                                TDisplayMode aMaskDisplayMode)
    {
    iSingletonServer->SetDisplayMode( aDisplayMode, aMaskDisplayMode); 
    }

 
// ---------------------------------------------------------------------------
// RPeninputServer::ClearServerEvent
// Set the display mode
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::ClearServerEvent()
    {        
    iSingletonServer->ClearServerEvent();
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SetDisplayMode
// To disable specified layouts at a time.
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::SetDisabledLayout( TInt aLayouts )
    {
    iSingletonServer->SetDisabledLayout( aLayouts);
    }
    
// ---------------------------------------------------------------------------
// RPeninputServer::DisabledLayout
// To disable specified layouts at a time.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::DisabledLayout()
    {
    return iSingletonServer->DisabledLayout();
    }    
    
// ---------------------------------------------------------------------------
// RPeninputServer::GetImePluginIdListL
// Get IME plugin list for a language
// ---------------------------------------------------------------------------
//
EXPORT_C void RPeninputServer::GetImePluginIdListL(TInt aLanguage, 
                                                  TInt aPluginMode, 
                                                  RArray<TInt>& aImplmentationIds)
    {
    iSingletonServer->GetImePluginIdListL(aLanguage,aPluginMode,aImplmentationIds);
    }
  
// ---------------------------------------------------------------------------
// RPeninputServer::PenSupportLanguagesL
// Get pen supported langauge
// ---------------------------------------------------------------------------
//                                 
EXPORT_C void RPeninputServer::PenSupportLanguagesL(RArray<TInt>& aLanguageLists)
    {
    iSingletonServer->PenSupportLanguagesL(aLanguageLists);
    }


// ---------------------------------------------------------------------------
// RPeninputServer::SetForeground
// Set current session to be foreground application session
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RPeninputServer::SetForeground(TBool aMustConnectFlag)
    {
    return iSingletonServer->SetForeground(aMustConnectFlag);
    }
    
// ---------------------------------------------------------------------------
// RPeninputServer::LoseForeground
// Indicates current client lost foreground
// ---------------------------------------------------------------------------
//    
EXPORT_C void RPeninputServer::LoseForeground()
    {
    iSingletonServer->LoseForeground();
    }
// ---------------------------------------------------------------------------
// RPeninputServer::IsForeground
// Test whether this session is the forground application session
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RPeninputServer::IsForeground()
    {
    return iSingletonServer->IsForeground();
    }

// ---------------------------------------------------------------------------
// RPeninputServer::AddPenUiActivationHandler
// Add an UI activate/deactivation handler
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RPeninputServer::AddPenUiActivationHandler(
                      MPenUiActivationHandler* aHandler,TInt aType)
    {   
    return iSingletonServer->AddPenUiActivationHandler(aHandler,aType);
    }
    
// ---------------------------------------------------------------------------
// RPeninputServer::RemovePenUiActivationHandler
// Remove all UI activate/deactivation handler
// ---------------------------------------------------------------------------
//    
EXPORT_C void RPeninputServer::RemovePenUiActivationHandler()
    {
    iSingletonServer->RemovePenUiActivationHandler();
    }

// ---------------------------------------------------------------------------
// RPeninputServer::RemovePenUiActivationHandler
// Remove an UI activate/deactivation handler
// Deprecated API
// ---------------------------------------------------------------------------
//    
EXPORT_C void RPeninputServer::RemovePenUiActivationHandler(MPenUiActivationHandler 
                                                                    *aHandler)
    {
    iSingletonServer->RemovePenUiActivationHandler(aHandler);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::UpdateAppInfo
// Update current application information
// ---------------------------------------------------------------------------
//   
EXPORT_C void RPeninputServer::UpdateAppInfo(const TDesC& aInfo, TPeninputAppInfo aType)
    {
    iSingletonServer->UpdateAppInfo(aInfo, aType);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SetAutoOpen
// Enable/disable the auto-open feature
// ---------------------------------------------------------------------------
//   
EXPORT_C void RPeninputServer::SetAutoOpen(TBool aFlag)
    {
	iSingletonServer->SetAutoOpen(aFlag);
    }

// ---------------------------------------------------------------------------
// RPeninputServer::AutoOpen
// get the the auto-open feature flag
// ---------------------------------------------------------------------------
//     
EXPORT_C TBool RPeninputServer::AutoOpen()    
    {
	return iSingletonServer->AutoOpen();	
    }

EXPORT_C void RPeninputServer::BackgroudDefaultOri( TInt aOri )
    {
    iSingletonServer->BackgroudDefaultOri(aOri);
    }
    
EXPORT_C void RPeninputServer::SetPreferredUiMode(TPluginInputMode aMode)
    {
    iSingletonServer->SetPreferredUiMode(aMode);
    }

EXPORT_C TPluginInputMode RPeninputServer::PreferredUiMode()
    {
    return iSingletonServer->PreferredUiMode();
    }

// ---------------------------------------------------------------------------
// RPeninputServer::SetGlobalNotes
// Enable/disable the auto-open feature
// ---------------------------------------------------------------------------
//    
EXPORT_C void RPeninputServer::SetGlobalNotes(TBool aFlag)
    {
	iSingletonServer->SetGlobalNotes(aFlag);
    } 
    
// ---------------------------------------------------------------------------
// RPeninputServer::SetAutoOpen
// Enable/disable the auto-open feature
// ---------------------------------------------------------------------------
//   
EXPORT_C void RPeninputServer::SetInternalPopUp(TBool aFlag)
    {
	iSingletonServer->SetInternalPopUp(aFlag);
    }  
    
// ---------------------------------------------------------------------------
// RPeninputServer::SetAutoOpen
// Enable/disable the auto-open feature
// ---------------------------------------------------------------------------
//   
EXPORT_C void RPeninputServer::SetEditorPriority(TInt aFlag)
    {
	iSingletonServer->SetEditorPriority(aFlag);
    }       
    
EXPORT_C void RPeninputServer::ClearTouchUI()  
    {
    iSingletonServer->ClearTouchUI();   
    }
    
EXPORT_C void RPeninputServer::SetResourceChange(TBool aFlag)  
    {
    iSingletonServer->SetResourceChange(aFlag);   
    }    
//end of class RPeninputServer
// End of File
