/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implement of class CPenInputSettingAppUi
*
*/



#include <avkon.hrh>
#include <aknnotewrappers.h> 
#include <ecom.h>
#include <AknsUtils.h>
#include <e32property.h>
#include <gspeninputplugin.h>
#include <akntitle.h> 
#include <AvkonInternalCRKeys.h>    // KAknQwertyInputModeActive

#include "peninputsettingAppui.h"
#include "peninputsettingApplication.h"

const TUid KUidPeninputGsPlugin = { 0x1027510C };

#define KMaxCmdLen 16

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ConstructL is called by the application framework
// ---------------------------------------------------------------------------
//
void CPenInputSettingAppUi::ConstructL()
    {
    BaseConstructL();   

    CEikStatusPane* statusPane = StatusPane();
    CEikStatusPaneBase::TPaneCapabilities titlePaneCapability =
        statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidTitle ) );

	CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
		( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
	titlePane->SetTextL( KNullDesC );
        
    HideApplicationFromFSW( ETrue );
    TAny* interface = REComSession::CreateImplementationL( KUidPeninputGsPlugin, 
        _FOFF(CGSPenInputPlugin,iDestrouctorIDKey));
    iAppView = reinterpret_cast<CGSPenInputPlugin*>( interface );    
    AddViewL( iAppView );    
    SetDefaultViewL( *iAppView );

    AknsUtils::SetAvkonSkinEnabledL( ETrue );    
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPenInputSettingAppUi::CPenInputSettingAppUi()                              
    {
	// no implementation required
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPenInputSettingAppUi::~CPenInputSettingAppUi()
    {    
    }

// ---------------------------------------------------------------------------
// handle any menu commands
// ---------------------------------------------------------------------------
//
void CPenInputSettingAppUi::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            Exit();
            break;
        }
    }
TBool CPenInputSettingAppUi::ProcessCommandParametersL( CApaCommandLine& aCommandLine )
    {     
    iParentWgId = aCommandLine.ParentWindowGroupID();    
    return ETrue;
    }
void CPenInputSettingAppUi::HandleWsEventL(const TWsEvent& aEvent,CCoeControl* aDestination)
    {
    if ( aEvent.Type() == KAknUidValueEndKeyCloseEvent )
        {
        iEikonEnv->WsSession().SendEventToWindowGroup( iParentWgId, aEvent );
        }
    else
        {
        CAknAppUi::HandleWsEventL( aEvent, aDestination );
        }    
    }
