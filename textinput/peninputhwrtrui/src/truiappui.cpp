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
* Description:  Implement of class CTruiAppUi
*
*/


#include <avkon.hrh>
#include <aknnavide.h>
#include <barsread.h>
#include <akntabgrp.h>
#include <trui.rsg>
#include <akntitle.h> 
#include <hlplch.h>
#include <AknsUtils.h>

#include "trui.hrh"
#include "truiappui.h"
#include "truimainview.h"
#include "truishortcutsview.h"
#include "truishortcuteditview.h"
#include "truishortcutedittextview.h"
#include "truiinfomessageview.h"
#include "truiengine.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ConstructL is called by the application framework
// ---------------------------------------------------------------------------
//
void CTruiAppUi::ConstructL()
    {
    BaseConstructL();    
    //Create HWR engine
    iEngine = CTruiEngine::NewL( iEikonEnv );

    //Create view
    iMainView = CTruiMainView::NewL();
    AddViewL( iMainView );    
 
    iShortcutsView = CTruiShortcutsView::NewL();   
    AddViewL( iShortcutsView );
    
    iShortcutEditView = CTruiShortcutEditView::NewL();   
    AddViewL( iShortcutEditView );
    
    iShortcutEditTextView = CTruiShortcutEditTextView::NewL();   
    AddViewL( iShortcutEditTextView );   
    
    iInfoMessageView = CTruiInfoMessageView::NewL();   
    AddViewL( iInfoMessageView );      

    SetDefaultViewL( *iMainView );
  
    iNaviDecorator = CreateNavigationTabL( R_NAVIGATIONPANE_TABGROUP, 0 );
    
    // Create empty navi pane
    // Get status pane from AppUi
    CEikStatusPane* statusPane = StatusPane();
    CEikStatusPaneBase::TPaneCapabilities naviPaneCapability =
        statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidNavi ) );        
    // if the navigation pane can be used
    if ( naviPaneCapability.IsPresent() && naviPaneCapability.IsAppOwned() )
        {
        CAknNavigationControlContainer* naviPane = 
            static_cast<CAknNavigationControlContainer*>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        // if the navigation pane can be used
        if ( naviPaneCapability.IsPresent() && naviPaneCapability.IsAppOwned() )
            {
            iEmptyNaviDecorator = naviPane->CreateNavigationLabelL();
            }        
        }   
    AknsUtils::SetAvkonSkinEnabledL( ETrue );    
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CTruiAppUi::CTruiAppUi()                              
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CTruiAppUi::~CTruiAppUi()
    {
    if ( iNaviDecorator )
        {
        delete iNaviDecorator;        
        }
    if ( iEmptyNaviDecorator )
        {
        delete iEmptyNaviDecorator;        
        }
    if ( iEngine )
        {
        delete iEngine;        
        }    
    }

// ---------------------------------------------------------------------------
// handle any menu commands
// ---------------------------------------------------------------------------
//
void CTruiAppUi::HandleCommandL( TInt aCommand )
    {
    switch( aCommand )
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            {
            PrepareToExit();
            Exit();
            break;
            }
        case EMainViewHelp:
        case EShortcutViewHelp:
            {
            CArrayFix<TCoeHelpContext>* buf = CCoeAppUi::AppHelpContextL();
            HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(), 
                                                 buf );
            break;
            }
        default:
            break;
        }
    }    
    
// ---------------------------------------------------------------------------
// Create navigation pane
// ---------------------------------------------------------------------------
//
CAknNavigationDecorator* CTruiAppUi::CreateNavigationTabL
                                     ( const TInt aResourceId, 
                                       const TInt aDefaultActive )
    {    
    // Get status pane from AppUi
    CEikStatusPane* statusPane = StatusPane();
    CEikStatusPaneBase::TPaneCapabilities naviPaneCapability =
        statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidNavi ) );
    
    CAknNavigationDecorator* naviDecorator = NULL;
    // if the navigation pane can be used
    if ( naviPaneCapability.IsPresent() && naviPaneCapability.IsAppOwned() )
        {
        // Get the navigation pane from status pane.
        CAknNavigationControlContainer* naviPane = 
            static_cast<CAknNavigationControlContainer*>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC( reader, aResourceId );
        
        // Create the navigation pane tab group
        naviDecorator = naviPane->CreateTabGroupL( reader, this );
        CleanupStack::PushL( naviDecorator );
        
        // Set tab text
        CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>
                                 ( naviDecorator->DecoratedControl() );    
        tabGroup->SetActiveTabByIndex( aDefaultActive );
         
        // Push and show the navigation pane
        naviPane->PushL( *naviDecorator );
        CleanupStack::Pop( naviDecorator );
        CleanupStack::PopAndDestroy(); // reader
        }
    return naviDecorator;
    }

// ---------------------------------------------------------------------------
// return pointer of hand writing engine
// ---------------------------------------------------------------------------
//
CTruiEngine* CTruiAppUi::HwrEngine() const
    {
    return iEngine;
    }

// ---------------------------------------------------------------------------
// Return a pointer to navigation pane.
// ---------------------------------------------------------------------------
//
CAknNavigationDecorator* CTruiAppUi::GetNaviDecorator() const
    {
    return iNaviDecorator;
    }

// ---------------------------------------------------------------------------
// Close the navi pane
// ---------------------------------------------------------------------------
//
void CTruiAppUi::CloseNaviPaneL()
    {
    // Get status pane from AppUi
    CEikStatusPane* statusPane = StatusPane();

    CEikStatusPaneBase::TPaneCapabilities naviPaneCapability =
        statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidNavi ) );   
    // if the navigation pane can be used
    if ( naviPaneCapability.IsPresent() && naviPaneCapability.IsAppOwned() )
        {
        // Get the navigation pane from status pane.
        CAknNavigationControlContainer* naviPane =
             static_cast<CAknNavigationControlContainer*>
             ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );        
        // Pop and hide the navigation pane
        naviPane->PushL( *iEmptyNaviDecorator );
        }
    }

// ---------------------------------------------------------------------------
// Show the navi pane
// ---------------------------------------------------------------------------
//
void CTruiAppUi::ShowNaviPaneL()
    {
    CEikStatusPane* statusPane = StatusPane();

    CEikStatusPaneBase::TPaneCapabilities naviPaneCapability =
        statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidNavi ) );  
    // if the navigation pane can be used
    if ( naviPaneCapability.IsPresent() && naviPaneCapability.IsAppOwned() )
        {
        CAknNavigationControlContainer* naviPane =
              static_cast<CAknNavigationControlContainer*>
              ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        // Push and show the navigation pane
        naviPane->PushL( *iNaviDecorator );
        }
    }

// ---------------------------------------------------------------------------
// Handle active tab changed
// ---------------------------------------------------------------------------
//
void CTruiAppUi::TabChangedL( TInt aIndex )
    {
    if ( aIndex == 0 )
        {
        // Switch to mainview
        ActivateLocalViewL( KTruiMainViewId );        
        }
    else
        {
        // Switch to shortcuts view
        ActivateLocalViewL( KTruiShortcutsViewId );
        }
    }
    
// ---------------------------------------------------------------------------
// Show the title pane
// ---------------------------------------------------------------------------
//    
void CTruiAppUi::ShowTitlePaneL( const TDesC& aTitle )
    {
    CEikStatusPane* statusPane = StatusPane();
    CEikStatusPaneBase::TPaneCapabilities titlePaneCapability =
        statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidTitle ) );
    if ( titlePaneCapability.IsPresent() && titlePaneCapability.IsAppOwned() )
        {
        CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        titlePane->SetTextL( aTitle );
        }
    }





