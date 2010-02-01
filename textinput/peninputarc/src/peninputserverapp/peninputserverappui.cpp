/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for peninput server application UI
*
*/



// INCLUDE FILES
#include "peninputserverappui.h"
#include <peninputserver.rsg>
#include <AknDef.h>
#include <avkon.hrh>
#include <apgwgnam.h>
#include "peninputserver.h"
#include "peninputclientserver.h"
#include "peninputserverapp.h"
#include <eiktbar.h>
#include <AknSgcc.h>
#include <AknsUtils.h>
#include <peninputcmd.h>

const TInt KPenInputServerReady = ESignalServerReady;
//global function
//Hide window gropu
void HideWindowGroupL(const RWsSession& aWs,const RWindowGroup& aGroup)
	{    
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC(aWs,
                                                 	aGroup.Identifier());
    wg->SetHidden(ETrue);
    wg->SetSystem(ETrue);
    wg->SetWindowGroupName(const_cast<RWindowGroup&>(aGroup));
    CleanupStack::PopAndDestroy(wg);    	
	}
// ================= MEMBER FUNCTIONS =======================
//
// ---------------------------------------------------------------------------
// CPeninputServerAppUi::ConstructL()
// ---------------------------------------------------------------------------
//
void CPeninputServerAppUi::ConstructL()
    {
    SetFullScreenApp( EFalse );
    //BaseConstructL(/*ENoAppResourceFile|ENoScreenFurniture*/EAknEnableSkin);
    //StatusPane()->MakeVisible(EFalse);
    BaseConstructL(ENoAppResourceFile|ENoScreenFurniture);
    //RAknUiServer* client = CAknSgcClient::AknSrv();
    //client->HideApplicationFromFsw(ETrue,KUidPeninputserver.iUid);
    CEikonEnv::Static()->SetSystem(ETrue);
    HideApplicationFromFSW( ETrue ); 
    //new pen input server
    iPeninputServer = CPeninputServer::NewL();
    
    // naming the server thread after the server helps to debug panics
    User::LeaveIfError( RThread().RenameMe( KPeninputServerName ) );   
    iIdle = CIdle::NewL(CActive::EPriorityIdle );
    iIdle ->Start(TCallBack(BackgroundTaskL,this));
    //RProcess::Rendezvous(KErrNone);    
    RProcess::Rendezvous(KPenInputServerReady);    
    RProcess proc;
    proc.SetPriority(EPriorityHigh);   
    RWindowGroup& rootWin = CCoeEnv::Static()->RootWin();
    rootWin.SetOrdinalPosition(-1,-1);
    rootWin.EnableReceiptOfFocus(EFalse);    

	// to make sure that wserv does not change our priority even we lose foreground
	iEikonEnv->WsSession().ComputeMode(RWsSession::EPriorityControlDisabled);
	AknsUtils::SetAvkonSkinEnabledL(ETrue);
    }

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::~CPeninputServerAppUi()
// Destructor
// Frees reserved resources
// ---------------------------------------------------------------------------
//
CPeninputServerAppUi::~CPeninputServerAppUi()
    { 
    delete iPeninputServer;
    delete iIdle;
   }

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------
//
TInt CPeninputServerAppUi::BackgroundTaskL(TAny* aPtr)
    {
    CPeninputServerAppUi* self = static_cast<CPeninputServerAppUi*>(aPtr);
    self->DoBackgroundTaskL();
    return EFalse; // only run once
    }

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::DoBackgroundTaskL
// Hide window group.
// ---------------------------------------------------------------------------
//
void CPeninputServerAppUi::DoBackgroundTaskL()
    {
    //hide application from task list.
    //HideWindowGroupL(CCoeEnv::Static()->WsSession(),CCoeEnv::Static()->RootWin());
    //let pen input server do the idle consruction
    iPeninputServer->DoIdleConstructL();
    }

// ---------------------------------------------------------------------------
// CPeninputServerAppUi::HandleResourceChangeL
// Handle system resource change
// ---------------------------------------------------------------------------
//
void CPeninputServerAppUi::HandleResourceChangeL(TInt aType)
    {
    CAknAppUi::HandleResourceChangeL(aType);    
    if(iPeninputServer)
        iPeninputServer->HandleResourceChange(aType);
    }
    
void CPeninputServerAppUi::HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination)
    {
    if(iPeninputServer)
        {
        iPeninputServer->HandleWsEventL(aEvent,aDestination);
        }
	CAknAppUi::HandleWsEventL(aEvent, aDestination);
    }

void CPeninputServerAppUi::HandleCommandL(TInt aCommand) 
    { 
    if(EEikCmdExit == aCommand) 
        { 
        iPeninputServer->PrepareExit();
        Exit();					           
        } 
    else 
        CAknAppUi::HandleCommandL(aCommand); 
    }    

// End of File
