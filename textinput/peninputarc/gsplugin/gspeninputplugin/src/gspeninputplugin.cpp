/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GSPenInputPlugin implementation.
*
*/


#include <gspeninputpluginrsc.rsg>
#include <gslistbox.h>
#include <gsprivatepluginproviderids.h>
#include <aknViewAppUi.h>
#include <StringLoader.h>
#include <hlplch.h> // HlpLauncher
#include <peninputpluginutils.h>
#include <eikmenup.h>
#include <akntitle.h> 
#include <eikapp.h>
#include <gspeninputplugin.mbg>
#include <AknsUtils.h>

#include "gspeninputplugin.h"
#include "gspeninputcontainer.h"
#include "gspeninputmodel.h"

_LIT( KPeninputIconFileName, "gspeninputplugin_aif.mif" );

const TUid KUidPenInputSettingApp = { 0x2001959B };

const TUid KUidCalibrationApp = { 0x102828BC };


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputPlugin::CGSPenInputPlugin()
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSPenInputPlugin::~CGSPenInputPlugin()
    {
    REComSession::DestroyedImplementation( iDestrouctorIDKey );
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CGSPenInputPlugin::ConstructL()
    {
    OpenLocalizedResourceFileL(KGSInputPenPluginResourceFileName,
                               iResourceLoader);
    BaseConstructL(R_GS_INPUT_VIEW);
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputPlugin* CGSPenInputPlugin::NewL(TAny* /*aInitParams*/)
    {
    CGSPenInputPlugin* self = new (ELeave) CGSPenInputPlugin();

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------
// Returns views id
// ---------------------------------------------------------
//
TUid CGSPenInputPlugin::Id() const
    {
    return KGSInputPenPluginUID;
    }

// ---------------------------------------------------------
// Handle client area change
// ---------------------------------------------------------
//
void CGSPenInputPlugin::HandleClientRectChange()
    {
    if (iContainer)
        {
        iContainer->SetRect(ClientRect());
        }
    }

// ---------------------------------------------------------
// Activate GsPenInputPlugin view
// ---------------------------------------------------------
//
void CGSPenInputPlugin::DoActivateL(const TVwsViewId& aPrevViewId,
                                       TUid aCustomMessageId,
                                       const TDesC8& aCustomMessage)
    {
    CGSBaseView::DoActivateL(aPrevViewId, aCustomMessageId, aCustomMessage);
    CEikStatusPane* statusPane = StatusPane();
    CEikStatusPaneBase::TPaneCapabilities titlePaneCapability =
        statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidTitle ) );
        
    TUid appUid = AppUi()->Application()->AppDllUid();   
    if ( appUid == KUidPenInputSettingApp )
        {
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC( reader, R_GS_INPUTPEN_PLUGIN_TITLE );
        if ( titlePaneCapability.IsPresent() && titlePaneCapability.IsAppOwned() )
            {
            CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
                ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
            titlePane->SetFromResourceL( reader );
            }
        CleanupStack::PopAndDestroy(); // reader    
        }
    }

// ---------------------------------------------------------
// Deactivation of GsPenInputPlugin view
// ---------------------------------------------------------
//
void CGSPenInputPlugin::DoDeactivate()
    {
    CGSBaseView::DoDeactivate();
    }

// ---------------------------------------------------------
// Command handling function called by application frame work
// ---------------------------------------------------------
//
void CGSPenInputPlugin::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EGSCmdAppChange:
        case EGSMSKCmdAppChange:
            {
            HandleListBoxSelectionL();
            }
            break;
        case EAknSoftkeyBack:
            {
            TUid appUid = AppUi()->Application()->AppDllUid();   
            
            if( appUid == KUidPenInputSettingApp )
                {                
                AppUi()->HandleCommandL( EEikCmdExit );
                }                
            else
                {
                AppUi()->ActivateLocalViewL( KGSGenPluginUid );
                }                
            }
            break;
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), AppUi()->AppHelpContextL() );            
            }
            break;
        default:
            {
            AppUi()->HandleCommandL(aCommand);
            }
            break;
        }
    }

// ---------------------------------------------------------
// Method for getting caption of this plugin
// ---------------------------------------------------------
//
void CGSPenInputPlugin::GetCaptionL(TDes& aCaption) const
    {
    HBufC* result = StringLoader::LoadL(R_CP_INPUTPEN_PLUGIN_CAPTION);

    CleanupStack::PushL(result);
    
    aCaption.Copy(*result);
    
    CleanupStack::PopAndDestroy(result);
    }

// ---------------------------------------------------------
// Read the ID of the plugin provider category
// ---------------------------------------------------------
//
TInt CGSPenInputPlugin::PluginProviderCategory() const
    {
    //This plugin is created by 3rd party.
    return KGSPluginProviderInternal;
    }

// ---------------------------------------------------------
// Functionality for creating a container
// ---------------------------------------------------------
//
void CGSPenInputPlugin::NewContainerL()
    {
    iContainer = new (ELeave) CGSPenInputContainer;
    }

// ---------------------------------------------------------
// Handles listbox selection
// ---------------------------------------------------------
//
void CGSPenInputPlugin::HandleListBoxSelectionL()
    {
    const TInt currentFeatureId = Container()->CurrentSelectedId();
    
    switch (currentFeatureId)
        {
        case EGSInputpenIdHwrTraining:
            {
            _LIT(KPeninputHwrTrainingUI, "z:\\sys\\bin\\trui.exe" );
            AknPenInputUtils::StartAppL( KPeninputHwrTrainingUI );            
            }
            break;
        case EGSInputpenIdInputLanguage:
            {
            Container()->ShowInputLanguagePageL();
            }
            break; 
        case EGSDefaultChineseOnscreenKeyboard:
            {
            Container()->ShowInputModePageL();
            }
            break;
        case EGSInputpenIdWritingSpeed:
            {
            Container()->ShowWritingSpeedPageL();
            }
            break;
        case EGSInputpenIdGuidLine:
            {
            Container()->ShowGuideLinePageL();
            }
            break;
        case EGSInputpenIdRecognitionWithDictionary:
            {
            Container()->ShowRecognitionWithDictionaryL();
            }
            break;             
        case EGSInputpenIdPenTrailWidth:
            {
            Container()->ShowTrailWidthPageL();    
            }
            break;
        case EGSInputpenIdPenTrailColour:
            {
            Container()->ShowTrailColourPageL(); 
            }
            break;
        case EGSInputpenIdTouchScrCalib:
            {            
            Container()->LaunchAppL( KUidCalibrationApp );
            }
            break;
        case EGSInputpenIdInputMethodForFind:
            {
            Container()->ShowInputMethodForFindPageL(); 
            }
            break;
        case EGSInputpenIdChineseFindMethod:
            {
            Container()->ShowChineseFindMethodPageL(); 
            }
            break;
       default:
            break;
        }
     }

// ---------------------------------------------------------
// Returns the container this view has
// ---------------------------------------------------------
//
CGSPenInputContainer* CGSPenInputPlugin::Container() const
    {
    CGSPenInputContainer* container = static_cast<CGSPenInputContainer*>(iContainer);

    return container;
    }
    
void CGSPenInputPlugin::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane)
    {
    if ( aResourceId != R_GS_INPUT_ITEM_CHANGE )
    	{
    	return;
    	}
    	
    const TInt currentId = Container()->CurrentSelectedId();
    
    if ( currentId == EGSInputpenIdHwrTraining || currentId == EGSInputpenIdTouchScrCalib )
    	{
    	aMenuPane->SetItemTextL( EGSCmdAppChange, R_GS_INPUT_ITEM_TXT_OPEN );
    	}
    else
    	{
        aMenuPane->SetItemTextL( EGSCmdAppChange, R_GS_INPUT_ITEM_TXT_CHANGE );
    	}
    }
// ---------------------------------------------------------
// Creates a new icon of desired type.
// ---------------------------------------------------------
//
CGulIcon* CGSPenInputPlugin::CreateIconL( const TUid aIconType )
    {    
    TParse iconFilePar;    
    iconFilePar.Set( KPeninputIconFileName, &KDC_BITMAP_DIR, 
                        NULL );
    TFileName fileName = iconFilePar.FullName();
    //delete iconFilePar;
    //iconFilePar = NULL;
    CGulIcon* icon = NULL;    
    if ( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), 
            KAknsIIDQgnPropCpDevPen,
            fileName,
            EMbmGspeninputpluginQgn_prop_cp_dev_pen,
            EMbmGspeninputpluginQgn_prop_cp_dev_pen_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }
    return icon;
    }

// End Of File
