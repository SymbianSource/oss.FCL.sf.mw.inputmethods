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
* Description:  Implement of class CTruiShortcutEditView
*
*/


#include <aknViewAppUi.h>
#include <trui.rsg>
#include <eikmenup.h>
#include <eikbtgpc.h>
#include <avkon.rsg>
#include <akntoolbar.h>

#include "truishortcuteditview.h"
#include "truishortcuteditcontainer.h"
#include "truiappui.h"
#include "trui.hrh"
#include "truishortcutsview.h"
#include "truiapplication.h"
#include "truiengine.h"
#include "truishortcutedittextview.h"

_LIT( KSeperatorBetweenTitle, " " );

// ======== MEMBER FUNCTIONS ========

CTruiShortcutEditView::CTruiShortcutEditView()
    {
    }

void CTruiShortcutEditView::ConstructL()
    {
    BaseConstructL( R_TRUI_SHORTCUTEDITVIEW );
    }

CTruiShortcutEditView* CTruiShortcutEditView::NewL()
    {
    CTruiShortcutEditView* self = CTruiShortcutEditView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CTruiShortcutEditView* CTruiShortcutEditView::NewLC()
    {
    CTruiShortcutEditView* self = new( ELeave ) CTruiShortcutEditView;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CTruiShortcutEditView::~CTruiShortcutEditView()
    {
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Returns views id.
// ---------------------------------------------------------------------------
//
TUid CTruiShortcutEditView::Id() const
    {
    return KTruiShortcutEditViewId;    
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Command handling function.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyDone:
            {
            if ( iContainer->PrepareToExitL() )
                { 
                iContainer->SwitchToShortcutViewL();
                }            
            break;
            }
        case EAknSoftkeyBack:
            {
            iContainer->BackToPreviousViewL();   
            break;
            }
        }
    }

    
// ---------------------------------------------------------------------------
// From class CAknView.
// Carried out when View is activated
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditView::DoActivateL( const TVwsViewId& aPrevViewId,
                  TUid /*aCustomMessageId*/,
                  const TDesC8& /*aCustomMessage*/ )
    {
    iPreViewId = aPrevViewId;
    CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
    CEikButtonGroupContainer* cba = Cba();
    // Set title pane and SK1's text
    if ( aPrevViewId == TVwsViewId( KUidtruiApp, KTruiShortcutsViewId ) )
        {
        appui->ShowTitlePaneL( appui->HwrEngine()->Shortcut() );
        cba->SetCommandSetL( R_AVKON_SOFTKEYS_DONE_BACK );
        }
    else if ( aPrevViewId == TVwsViewId( KUidtruiApp, KTruiShortcutEditTextViewId ) )
        {
        HBufC* numberText = iEikonEnv->AllocReadResourceLC
                           ( R_TRUI_SHORTCUTEDITTEXTVIEW_TITLEPANE_LAST_NUMBER );
        HBufC* title = HBufC::NewLC( appui->HwrEngine()->Shortcut().Length() 
                                     + numberText->Length() + KExtraSize );
        TPtr titlePtr = title->Des();
        titlePtr.Copy( *numberText );        
        titlePtr.Append( KSeperatorBetweenTitle );
        titlePtr.Append( appui->HwrEngine()->Shortcut() );
        appui->ShowTitlePaneL( *title );    
        CleanupStack::PopAndDestroy( title );
        CleanupStack::PopAndDestroy( numberText );
        cba->SetCommandSetL( R_TRUI_SHORTCUTEDITVIEW_CREATE_CBA );
        }
                
    // Close navi pane
    appui->CloseNaviPaneL();
    
    iContainer = CTruiShortcutEditContainer::NewL( ClientRect() );
    iContainer->SetMopParent( this );
    AppUi()->AddToStackL( *this, iContainer );
    iContainer->SetView( this );    

    // Set toolbar
    CAknToolbar* toolbar = Toolbar();
    if ( toolbar )
        {
        toolbar->SetToolbarObserver( this );
        toolbar->SetToolbarVisibility( ETrue, EFalse );
        }
    }

// ---------------------------------------------------------------------------
// From class CAknView.
// Carried out when View is deactivated
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditView::DoDeactivate()
    {	
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// ---------------------------------------------------------------------------
// From class MAknToolbarObserver.
// Handles toolbar events for a certain toolbar item.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditView::OfferToolbarEventL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EShortcutEditViewDelete:
            {
            iContainer->ClearCurrentModelL();
            break;
            }
        case EShortcutEditViewPreviewModel:
            {
            iContainer->PreviewShortcutModel();
            break;
            }
        case EShortcutEditViewGrid:
            {
            iContainer->PopupSctDialogL();
            break;
            }
        }
    }
    
