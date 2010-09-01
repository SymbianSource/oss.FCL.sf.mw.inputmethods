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
* Description:  Implement of class CTruiMainView
*
*/


#include <aknViewAppUi.h>
#include <eikmenup.h>
#include <trui.rsg>
#include <akntoolbar.h>
#include <aknnavide.h>
#include <akntabgrp.h>

#include "truimainview.h"
#include "truimainviewcontainer.h"
#include "trui.hrh"
#include "truiappui.h"
#include "truiengine.h"
#include "truishortcutsview.h"

// ======== MEMBER FUNCTIONS ========

CTruiMainView::CTruiMainView()
    {
    }

void CTruiMainView::ConstructL()
    {
    BaseConstructL( R_TRUI_MAINVIEW );
    }

CTruiMainView* CTruiMainView::NewL()
    {
    CTruiMainView* self = CTruiMainView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CTruiMainView* CTruiMainView::NewLC()
    {
    CTruiMainView* self = new( ELeave ) CTruiMainView;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CTruiMainView::~CTruiMainView()
    {
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Returns views id.
// ---------------------------------------------------------------------------
//
TUid CTruiMainView::Id() const
    {
    return KTruiMainViewId;    
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Command handling function.
// ---------------------------------------------------------------------------
//
void CTruiMainView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EMainViewCharGrid:
            {
            // Handle menu item command "Character Grid"
            iContainer->HandleChooseCharEventL();             
            }
            break;
        case EMainViewSubmenuWestern:
        case EMainViewSubmenuCyrillic:
        case EMainViewSubmenuGreek:
        case EMainViewSubmenuHebrew:
            {
            iContainer->HandleLanguageScriptChangedL( aCommand );
            }
            break;
        case EMainViewSubmenuLatinLowerLetter:
        case EMainViewSubmenuLatinUpperLetter:
        case EMainViewSubmenuLatinAccent:
        case EMainViewSubmenuDigital:
        case EMainViewSubmenuSymbols:
        case EMainViewSubmenuCyrillicLowerLetters:
        case EMainViewSubmenuCyrillicUpperLetters:
        case EMainViewSubmenuCyrillicAccent:        
        case EMainViewSubmenuGreekLowerLetters:
        case EMainViewSubmenuGreekUpperLetters:       
        case EMainViewSubmenuHebrewLetters:        
            {
            // Handle character range changed
            // Get range index in menu pane by range Id
            CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
            CTruiEngine* engine = appui->HwrEngine();
            const RArray<TInt>& rangeIdArray = 
                                engine->GetCurrentSupportedRangesId();
            TInt indexOfMenuItems = rangeIdArray.Find( aCommand );
            if ( indexOfMenuItems != KErrNotFound )
                {
                // Set current character range by index in Engine
                engine->SetCharacterRangeByIndexL( indexOfMenuItems );

                // Call iContainer->HandleCharRangeChanged to handle it.
                iContainer->HandleCharRangeChangedL( indexOfMenuItems, 
                                                     aCommand );
                }	        
            }
	        break;
        case EMainViewDeleteCurrentModel:
            {
            iContainer->DeleteModelL();
            }
            break;
        case EMainViewDeleteAllModel:
            {
            iContainer->DeleteAllModelL();
            break;
            }
        case EMainViewPreviewModel:
            {
            iContainer->PreviewCharacterModel();
            break;
            }
        case EMainViewToShortcutsView:
            {
            CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
            CAknNavigationDecorator* naviDecorator = appui->GetNaviDecorator();
            CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>
                                    ( naviDecorator->DecoratedControl() );
            tabGroup->SetActiveTabById( ENavigationPaneShortcutsViewTab );
            // Switch to local view.
            appui->ActivateLocalViewL( KTruiShortcutsViewId );
            break;
            }
        case EAknSoftkeyExit:
            {
            iContainer->PrepareToExitL();
            AppUi()->HandleCommandL( aCommand );
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
void CTruiMainView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                  TUid /*aCustomMessageId*/,
                  const TDesC8& /*aCustomMessage*/ )
    {
    // Set title pane
    CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
    HBufC* titleText = 
           iEikonEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_TITLEPANE_TEXT );
    appui->ShowTitlePaneL( *titleText );
    CleanupStack::PopAndDestroy( titleText );

    iContainer = CTruiMainViewContainer::NewL( ClientRect() );
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
void CTruiMainView::DoDeactivate()
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
void CTruiMainView::OfferToolbarEventL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EMainViewButtonIdGrid:
            {
            // Open SCT
            iContainer->HandleChooseCharEventL();
            break;
            }
        case EMainViewButtonIdPreview:
            {
            // Preview character animation
            iContainer->PreviewCharacterModel();
            break;
            }
        case EMainViewButtonIdDelete:
            {
            // Clear HWR box and delete character model
            iContainer->DeleteModelL();
            break;
            }
        }
    }
    
// ---------------------------------------------------------------------------
// Dynamically initialises a menu pane.
// ---------------------------------------------------------------------------
//
void CTruiMainView::DynInitMenuPaneL( TInt aResourceId, 
                                      CEikMenuPane* aMenuPane )
    {
    // Get engine
    CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
    CTruiEngine* engine = appui->HwrEngine();
    
    if ( aResourceId == R_TRUI_MAINVIEW_MENUPANE )
        {
        // Get language script from engine
        // Determine whether "Language script" will be displayed.
        aMenuPane->SetItemDimmed( EMainViewLanguageScript, 
                            ( engine->SupportedLanguageScript().Count() <= 1 ) );
        
        // Determine whether "Delete model" will be displayed
        iDimAllDelete = iContainer->IsAllDelete();
                       
        TBool isDimmed = ( iContainer->IsButtonDimmed() && iDimAllDelete ) 
                         ? ETrue: EFalse;
        aMenuPane->SetItemDimmed( EMainViewDeleteModel, isDimmed );
        
        // Determine whether "Preview model" will be displayed.
        TBool isPreviewDimmed = iContainer->IsButtonDimmed() ? ETrue: EFalse;
        aMenuPane->SetItemDimmed( EMainViewPreviewModel, isPreviewDimmed );
        }    
    if ( aResourceId == R_TRUI_MAINVIEW_MENU_LANGUAGESCRIPT_CASCADE )
        {        
        // Set which items will be displayed by supported language script
        const RArray<TInt>& languageScriptId = 
                                           engine->SupportedLanguageScript();
        aMenuPane->SetItemDimmed( EMainViewSubmenuWestern, ETrue );
        aMenuPane->SetItemDimmed( EMainViewSubmenuCyrillic, ETrue );
        aMenuPane->SetItemDimmed( EMainViewSubmenuGreek, ETrue );
        aMenuPane->SetItemDimmed( EMainViewSubmenuHebrew, ETrue );
        for ( TInt i = 0; i < languageScriptId.Count(); i++ )
            {
            aMenuPane->SetItemDimmed( languageScriptId[i], EFalse );
            }
        
        if ( IsMenuItemDimmed( aMenuPane, EMainViewSubmenuWestern ) )
            {
            aMenuPane->DeleteMenuItem( EMainViewSubmenuWestern );
            }
        if ( IsMenuItemDimmed( aMenuPane, EMainViewSubmenuCyrillic ) )
            {
            aMenuPane->DeleteMenuItem( EMainViewSubmenuCyrillic );
            }
        if ( IsMenuItemDimmed( aMenuPane, EMainViewSubmenuGreek ) )
            {
            aMenuPane->DeleteMenuItem( EMainViewSubmenuGreek );
            }
        if ( IsMenuItemDimmed( aMenuPane, EMainViewSubmenuHebrew ) )
            {
            aMenuPane->DeleteMenuItem( EMainViewSubmenuHebrew );
            }
        
        // Set which item will be indicated by current language script         
        aMenuPane->SetItemButtonState( engine->CurrentLanguageScript(), 
                                       EEikMenuItemSymbolOn );
        
        // Set current selected item
        TInt pos;
        aMenuPane->ItemAndPos( engine->CurrentLanguageScript(), pos );
        aMenuPane->SetSelectedItem( pos );
        }
    if ( aResourceId == R_TRUI_MAINVIEW_MENU_CHARRANGE_CASCADE )
        {
        // Get language script from model      
        // Init character range submenu                     
        InitCharRangeSubMenuL( engine->CurrentLanguageScript(), aMenuPane );
        
        if ( engine->CurrentLanguageScript() == EMainViewSubmenuCyrillic )
            {
            // Dim cyrillic accent range
            aMenuPane->SetItemDimmed( EMainViewSubmenuCyrillicAccent, ETrue );
            }

        // Get the current item's Id
        TInt rangeId = engine->GetCurrentCharacterRange();
        
        // Set which item to be indicated       
        aMenuPane->SetItemButtonState( rangeId, EEikMenuItemSymbolOn );
        
        // Set which item to be highlighted
        const RArray<TInt>& rangeIdArray = 
                                      engine->GetCurrentSupportedRangesId();
        TInt indexOfMenuItems = rangeIdArray.Find( rangeId );
        if ( indexOfMenuItems != KErrNotFound )
            {
            aMenuPane->SetSelectedItem( indexOfMenuItems );
            }
        }
    if ( aResourceId == R_TRUI_MAINVIEW_MENU_DELETE_CASCADE )
        {
        aMenuPane->SetItemDimmed( EMainViewDeleteCurrentModel, 
                                  iContainer->IsButtonDimmed() );
        aMenuPane->SetItemDimmed( EMainViewDeleteAllModel, iDimAllDelete );
        }
    }

// ---------------------------------------------------------------------------
// Dynamically initialises the character range menu pane.
// ---------------------------------------------------------------------------
//
void CTruiMainView::InitCharRangeSubMenuL( TInt aScriptId,
                                           CEikMenuPane* aMenuPane)
    {
    // Get the corrent reource ID of MENU_ITEMS by 
    TInt reourceId = MenuItemsReourceId( aScriptId );
    // Add these menu items into menu pane
    aMenuPane->AddMenuItemsL( reourceId );    
    }
    
// ---------------------------------------------------------------------------
// Get reource ID of menu items for initializing menu pane.
// ---------------------------------------------------------------------------
//
TInt CTruiMainView::MenuItemsReourceId( TInt aScriptId )
    {
    TInt reourceId;
    switch ( aScriptId )
        {
        case EMainViewSubmenuWestern:
            {
            reourceId = R_TRUI_MAINVIEW_SUBMENU_CHARRANGE_LATIN;
            }
            break;            
        case EMainViewSubmenuCyrillic:
            {
            reourceId = R_TRUI_MAINVIEW_SUBMENU_CHARRANGE_CYRILLIC;
            }
            break;                    
        case EMainViewSubmenuGreek:
            {
            reourceId = R_TRUI_MAINVIEW_SUBMENU_CHARRANGE_GREEK;
            }
            break;        
        case EMainViewSubmenuHebrew:
            {
            reourceId = R_TRUI_MAINVIEW_SUBMENU_CHARRANGE_HEBREW;
            }        
            break;
        default:
            reourceId = 0;
            break;            
        }
    return reourceId;            
    }   

// ---------------------------------------------------------------------------
// Check menu item dimmed by menu item id.
// ---------------------------------------------------------------------------
//
TBool CTruiMainView::IsMenuItemDimmed( CEikMenuPane* aMenuPane, TInt aMenuItemId )
    {
    CEikMenuPaneItem::SData& itemdata = aMenuPane->ItemData( aMenuItemId );    
    return ( itemdata.iFlags & EEikMenuItemDimmed == EEikMenuItemDimmed );    
    }
