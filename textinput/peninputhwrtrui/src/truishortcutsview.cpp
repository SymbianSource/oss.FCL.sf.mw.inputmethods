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
* Description:  Implement of class CTruiShortcutsView
*
*/


#include <aknViewAppUi.h>
#include <trui.rsg>
#include <eikmenup.h>
#include <akntoolbar.h>
#include <aknnavide.h>
#include <akntabgrp.h>

#include "truishortcutsview.h"
#include "truishortcutscontainer.h"
#include "trui.hrh"
#include "truiappui.h"
#include "truishortcutedittextview.h"
#include "truimainview.h"

// ======== MEMBER FUNCTIONS ========

CTruiShortcutsView::CTruiShortcutsView()
    {
    }

void CTruiShortcutsView::ConstructL()
    {
    BaseConstructL( R_TRUI_SHORTCUTSVIEW );
    }

CTruiShortcutsView* CTruiShortcutsView::NewL()
    {
    CTruiShortcutsView* self = CTruiShortcutsView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CTruiShortcutsView* CTruiShortcutsView::NewLC()
    {
    CTruiShortcutsView* self = new( ELeave ) CTruiShortcutsView;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CTruiShortcutsView::~CTruiShortcutsView()
    {
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Returns views id.
// ---------------------------------------------------------------------------
//
TUid CTruiShortcutsView::Id() const
    {
    return KTruiShortcutsViewId;    
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Command handling function.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EShortcutViewCreate:
            {
            iContainer->AddShortcutL();
            }
            break;
        case EShortcutViewEditText:
            {
            iContainer->EditShortcutL( ETrue );
            break;
            }
        case EShortcutViewEditModel:
            {
            iContainer->EditShortcutL( EFalse );            
            }
            break;
        case EShortcutViewLinkToHandwritting:
            {
            // Set main view tab active
            CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );            
            CAknNavigationDecorator* naviDecorator = appui->GetNaviDecorator();
            CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>
                                    ( naviDecorator->DecoratedControl() );
            tabGroup->SetActiveTabById( ENavigationPaneMainViewTab );
            // Switch to local view.
            appui->ActivateLocalViewL( KTruiMainViewId );
            }
            break;
        case EShortcutViewMark:
            {
            iContainer->MarkShortcutL();
            }
            break;
        case EShortcutViewUnmark:
            {
            iContainer->UnmarkShortcutL();
            }
            break;
        case EShortcutViewResetInfoMessage:
            {
            iContainer->ResetIntroductionL();
            }
            break;
        case EShortcutViewDelete:
            {
            iContainer->DeleteShortcutL();
            }
            break;
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            }
        }
    }
	
// ---------------------------------------------------------------------------
// From class CAknView.
// Carried out when View is activated
// ---------------------------------------------------------------------------
//
void CTruiShortcutsView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                  TUid /*aCustomMessageId*/,
                  const TDesC8& /*aCustomMessage*/ )
	{
    // Set title pane
    CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
    HBufC* titleText = iEikonEnv->AllocReadResourceLC
                       ( R_TRUI_SHORTCUTSVIEW_TITLEPANE_TEXT );
    appui->ShowTitlePaneL( *titleText );
    CleanupStack::PopAndDestroy( titleText );
	
	// Show navi pane
    appui->ShowNaviPaneL();
    
    iContainer = CTruiShortcutsContainer::NewL( ClientRect() );
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
void CTruiShortcutsView::DoDeactivate()
    {
    if ( iContainer )
        {        
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }        
    }


// ---------------------------------------------------------------------------
// From MEikMenuObserver
// Dynamically initialises a menu pane.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsView::DynInitMenuPaneL( TInt aResourceId, 
                                           CEikMenuPane* aMenuPane )
    {
    // Prepare to display popup window when focus return to container
    iContainer->SetDisplayPopupWindow( ETrue );
    
    if ( aResourceId == R_TRUI_SHORTCUTSVIEW_MENUPANE )
        {       
        // Check if "Edit" will be displayed when first item highlighted.
        // Check if "Delete" will be displayed when first item highlighted.
        // Check if "Mark" will be displayed when first item highlighted.
        // Check if "Unmark" will be displayed when first item highlighted.        
        TInt currentIndex = iContainer->CurrentItemIndex();
        if ( currentIndex != 0 )
            {
            if ( iContainer->MarkCount() > 0 )
                {
                // At least one marked, "Edit" won't be displayed
                aMenuPane->SetItemDimmed( EShortcutViewEditText, ETrue );
                aMenuPane->SetItemDimmed( EShortcutViewEditModel, ETrue );                
                }
            else
                {
                // All unmark, "Edit" will be displayed.
                aMenuPane->SetItemDimmed( EShortcutViewEditText, EFalse );
                aMenuPane->SetItemDimmed( EShortcutViewEditModel, EFalse );                
                }            
            aMenuPane->SetItemDimmed( EShortcutViewDelete, EFalse );
            aMenuPane->SetItemDimmed( EShortcutViewMark, EFalse );
            aMenuPane->SetItemDimmed( EShortcutViewUnmark, EFalse );
            if ( iContainer->IsMarked( currentIndex ) )
                { 
                // The currently selected item mark, display "Unmark"
                aMenuPane->SetItemDimmed( EShortcutViewMark, ETrue );
                }
            else
                {
                // The currently selected item unmark, display "Mark"
                aMenuPane->SetItemDimmed( EShortcutViewUnmark, ETrue );
                }
            
            // Hide Reset introduction
            aMenuPane->SetItemDimmed( EShortcutViewResetInfoMessage, ETrue );
            }
        else
            {
            aMenuPane->SetItemDimmed( EShortcutViewEditText, ETrue );
            aMenuPane->SetItemDimmed( EShortcutViewEditModel, ETrue );
            aMenuPane->SetItemDimmed( EShortcutViewDelete, ETrue );
            aMenuPane->SetItemDimmed( EShortcutViewMark, ETrue );
            aMenuPane->SetItemDimmed( EShortcutViewUnmark, ETrue );
            
            // Check value of setting of introduction
            TBool isDimReset = iContainer->IsResetIntroduction() ? EFalse : ETrue;
            aMenuPane->SetItemDimmed( EShortcutViewResetInfoMessage, isDimReset );
            }
        }
    }
 
// ---------------------------------------------------------------------------
// From class MEikMenuObserver.
// Dynamically initialises a menu bar.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsView::DynInitMenuBarL( TInt aResourceId, 
                                          CEikMenuBar* /*aMenuBar*/ )
    {
    if ( aResourceId == R_TRUI_SHORTCUTSVIEW_CONTEXT_MENUBAR )
        {        
        iContainer->SetDisplayPopupWindow( ETrue );
        }
    }

// ---------------------------------------------------------------------------
// From class MAknToolbarObserver.
// Handles toolbar events for a certain toolbar item.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsView::OfferToolbarEventL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EShortcutsViewButtonIdEditText:
            {
            // Open the text setting view
            iContainer->EditShortcutL( ETrue );
            break;
            }
        case EShortcutsViewButtonIdEditModel:
            {
            // Open the shortcut edit view
            iContainer->EditShortcutL( EFalse );
            break;
            }
        case EShortcutsViewButtonIdDelete:
            {
            iContainer->DeleteShortcutL();
            break;
            }
        }
    }
// ---------------------------------------------------------------------------
// From CAknView. This function processes user commands.
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsView::ProcessCommandL( TInt aCommand )
    {
    if ( !iContainer )
        {
        return;
        }
    CAknView::ProcessCommandL( aCommand );
    }
