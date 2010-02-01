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
* Description:  Implement of class CTruiShortcutEditTextView
*
*/


#include <avkon.hrh>
#include <trui.rsg>
#include <eikcolib.h>
#include <akntoolbar.h>

#include "truiinfomessageview.h"
#include "truiappui.h"
#include "truiinfomessagecontainer.h"
#include "truishortcutedittextview.h"
#include "truishortcutsview.h"
#include "truiengine.h"

// ======== MEMBER FUNCTIONS ========

CTruiInfoMessageView::CTruiInfoMessageView()
    {
    }
    
void CTruiInfoMessageView::ConstructL()
    {
    BaseConstructL( R_TRUI_INFOMESSAGEVIEW );
    }    
    
CTruiInfoMessageView* CTruiInfoMessageView::NewL()
    {
    CTruiInfoMessageView* self = CTruiInfoMessageView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }
    
CTruiInfoMessageView* CTruiInfoMessageView::NewLC()
    {
    CTruiInfoMessageView* self = new( ELeave ) CTruiInfoMessageView;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
CTruiInfoMessageView::~CTruiInfoMessageView()
    {
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Returns views id.
// ---------------------------------------------------------------------------
//
TUid CTruiInfoMessageView::Id() const
    {
    return KTruiInfoMessageViewId;    
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Command handling function.
// ---------------------------------------------------------------------------
//
void CTruiInfoMessageView::HandleCommandL( TInt aCommand )
    {
    CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
    switch ( aCommand )
        {
        case EAknSoftkeyNext:
            {
            appui->HwrEngine()->SetShortcut( KNullDesC );
            iContainer->PrepareToExitL();
            appui->ActivateLocalViewL( KTruiShortcutEditTextViewId );
            break;
            }            
        case EAknSoftkeyCancel:
            {
            appui->ActivateLocalViewL( KTruiShortcutsViewId );
            break;            
            }
        }
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Carried out when View is activated
// ---------------------------------------------------------------------------
//
void CTruiInfoMessageView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                  TUid /*aCustomMessageId*/,
                  const TDesC8& /*aCustomMessage*/ )
	{
    CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
    // Set title pane
    HBufC* titleText = iEikonEnv->AllocReadResourceLC
                                ( R_TRUI_INFOMESSAGEVIEW_TITLEPANE_TEXT );
    appui->ShowTitlePaneL( *titleText );
    CleanupStack::PopAndDestroy( titleText );
    
    // Close navi pane
    appui->CloseNaviPaneL();
	
	// Create container        
    iContainer = CTruiInfoMessageContainer::NewL( ClientRect() );
    iContainer->SetMopParent( this );
    iContainer->SetView( this );
    AppUi()->AddToStackL( *this, iContainer );
	}

// ---------------------------------------------------------------------------
// From class CAknView.
// Carried out when View is deactivated
// ---------------------------------------------------------------------------
//
void CTruiInfoMessageView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }
