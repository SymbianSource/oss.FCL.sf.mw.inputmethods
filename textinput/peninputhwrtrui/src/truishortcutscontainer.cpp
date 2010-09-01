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
* Description:  Implement of class CTruiShortcutsContainer
*
*/


#include <barsread.h>
#include <aknnavide.h>
#include <akntabgrp.h>
#include <eikspane.h>
#include <aknViewAppUi.h>
#include <aknlists.h>
#include <AknIconArray.h>
#include <eikclbd.h>
#include <trui.rsg>
#include <StringLoader.h>
#include <trui_icons.mbg>
#include <AknIconUtils.h>
#include <gulicon.h> 
#include <akntextsettingpage.h>
#include <AknQueryDialog.h>  
#include <AknPreviewPopUpController.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <akntoolbar.h>
#include <aknbutton.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <biditext.h>

#include "truicontainerbase.h"
#include "truishortcutscontainer.h"
#include "truimainview.h"
#include "truiengine.h"
#include "truiappui.h"
#include "truiengine.h"
#include "truishortcuteditview.h"
#include "trui.hrh"
#include "truiapplication.h"
#include "truihelp.h"
#include "truihwrbox.h"
#include "truishortcutedittextview.h"
#include "truiinfomessageview.h"
#include "truishortcutsview.h"

_LIT( KTRUIICONSFILENAME, "\\resource\\apps\\trui_icons.mif" );

// Initialized size for item array
const TInt KItemArraySize = 1;

// Initialized size for icon array
const TInt KIconArraySize = 3;

// listbox item format string
_LIT( KListItemWithPicFormat, "1\t%S\t\t" );
_LIT( KListItemFormat, "\t%S\t\t" );
    
// Delay time of showing tooltip
const TTimeIntervalMicroSeconds32 KTooltipShowDelay = 1000000;

// Delay time of hiding tooltip
const TTimeIntervalMicroSeconds32 KTooltipHideDelay = 3000000;

// Rect of popup hwrbox
const TRect KPopupHwrBoxRect = TRect( TPoint( 0, 0 ), TSize( 75, 75 ) );

const TPoint KPopupHwrBoxPosLandscape = TPoint( 479, 360 );
const TPoint KPopupHwrBoxPosPortrait = TPoint( 337, 500 );

const TInt KPopupWindowVarity = 0;

const TInt KParaDelimiterSize = 1;
_LIT( KParaDelimiterFormat, "%c" );
    
// ======== MEMBER FUNCTIONS ========

CTruiShortcutsContainer::CTruiShortcutsContainer()
    {  
    }
    
void CTruiShortcutsContainer::ConstructL( const TRect& aRect )
    {
    // Initialize control array
    InitComponentArrayL();
        
    // Set parent control or owning window
    CreateWindowL();   
    
    // Load shortcuts
    iEngine->UpdateShortcutListL();
    
    // Initialize controls
    InitializeControlsL();
    SetRect( aRect );
                
    // Set default selected index
    iListBox->SetCurrentItemIndex( iEngine->CurrentSelectedIndex() );
               
    HandleListBoxFocusChangedL( CurrentItemIndex() );
    
    ActivateL();    
    }
    
CTruiShortcutsContainer* CTruiShortcutsContainer::NewL( const TRect& aRect )
    {
    CTruiShortcutsContainer* self = CTruiShortcutsContainer::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }
    
CTruiShortcutsContainer* CTruiShortcutsContainer::NewLC
                                                  ( const TRect& aRect )
    {
    CTruiShortcutsContainer* self = new (ELeave) CTruiShortcutsContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }
    
CTruiShortcutsContainer::~CTruiShortcutsContainer()
    {    
    delete iPopupController;     
    delete iHwrBox;
    }
    
// ---------------------------------------------------------------------------
// Create controls to be displayed in this container.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsContainer::InitializeControlsL()
    {
    // Create toolbar
    CAknToolbar* toolbar = iAppUi->View( KTruiShortcutsViewId )->Toolbar();
    // Get Clear button from toolbar    
    CAknButton* deleteButton = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EShortcutsViewButtonIdDelete ) );

    // Create listbox
    CreateListBoxL();
    
    // Create preview popup window
    iPopupController = CreatePopupHwrBoxL();    
    }
            
// ---------------------------------------------------------------------------
// From class CCoeControl.
// Responds to changes to the size and position of the contents of this control.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsContainer::SizeChanged()
    {
    if ( iListBox )
        {
        iListBox->SetRect( Rect() );
        }    
    
    // caculate main pane
    TRect rect = Rect();    
    TAknWindowComponentLayout main_pane_layout = 
                            AknLayoutScalable_Apps::main_hwr_training_pane();
    TAknLayoutRect main_pane_layout_rect;
    main_pane_layout_rect.LayoutRect( rect, main_pane_layout );
    TRect main_pane_rect = main_pane_layout_rect.Rect();
    
    // Calculate popup_hwr_training_preview_window
    TAknWindowComponentLayout preview_popup_layout = 
               AknLayoutScalable_Apps::popup_hwr_training_preview_window
                                                     ( KPopupWindowVarity );
    TAknLayoutRect preview_popup_layout_rect;
    preview_popup_layout_rect.LayoutRect( main_pane_rect, preview_popup_layout );    
    
    iHwrBox->SetSize( preview_popup_layout_rect.Rect().Size() );
    
    TPoint itemPos = PopupWindowPosition( CurrentItemIndex() );
    iPopupController->SetPosition( itemPos );
    }       

// ---------------------------------------------------------------------------
// From class CCoeControl.
// This is called whenever the control gains or loses focus, 
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsContainer::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    if ( IsFocused() )
        {        
        if ( iDisplayPopupWindow )
            {            
            TRAPD( err, CheckAndPreviewShortcutModelL( CurrentItemIndex() ) );
            if ( err != KErrNone )
                {
                iDisplayPopupWindow = EFalse;
                }
            }
        }
    else
        {        
        // Close pupup preview window
        iPopupController->HidePopUp();
        }
    }

// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles pointer events.
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    iPenDownPoint = aPointerEvent.iPosition;
    CCoeControl::HandlePointerEventL( aPointerEvent );    
    }
    
// ---------------------------------------------------------------------------
// Popup the TextSettingPage and added the new shortcut to ListBox.
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::AddShortcutL()
    {    
    iEngine->NewShortcut( ETrue );
    iEngine->SetShortcut( KNullDesC );
    // Get always display setting from ini file
    TInt isNotDisplayWizard = 0;
    TRAPD( err, GetSettingFromIniFileL( KAlwaysDisplayWizardKey, isNotDisplayWizard ) );
    if ( err != KErrNone )
        {
        isNotDisplayWizard = 0;
        }
        
    if ( isNotDisplayWizard )
        {
        iEngine->SetDisplayWizard( EFalse );
        iAppUi->ActivateLocalViewL( KTruiShortcutEditTextViewId );
        }
    else
        {
        iEngine->SetDisplayWizard( ETrue );
        iAppUi->ActivateLocalViewL( KTruiInfoMessageViewId );
        }    
    }

// ---------------------------------------------------------------------------
// Edit shortcut
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsContainer::EditShortcutL( TBool aOnlyEditText )
    {
    CDesCArray* shortcutlist = iEngine->ShortcutTextList();
    TInt posInEngine = iListBox->CurrentItemIndex() - 1;
    iEngine->SetShortcut( shortcutlist->MdcaPoint( posInEngine ) );
    iEngine->NewShortcut( EFalse );
    // Switch to shortcut edit view 
    if ( aOnlyEditText )
        {
        iAppUi->ActivateLocalViewL( KTruiShortcutEditTextViewId );
        }
    else
        {
        iAppUi->ActivateLocalViewL( KTruiShortcutEditViewId );
        }
    }

// ---------------------------------------------------------------------------
// Check if reset introduction
// ---------------------------------------------------------------------------
// 
TBool CTruiShortcutsContainer::IsResetIntroduction()
    {
    TInt value = 0;
    TRAPD( err, GetSettingFromIniFileL( KAlwaysDisplayWizardKey, value ) );
    if ( err != KErrNone )
        {
        value = 0;
        }
    return value ? ETrue: EFalse;
    }    

// ---------------------------------------------------------------------------
// Reset introduction
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsContainer::ResetIntroductionL()
    {
    SaveSettingIntoIniFileL( KAlwaysDisplayWizardKey, 0 );
    iAppUi->ActivateLocalViewL( KTruiInfoMessageViewId );
    }
    
// ---------------------------------------------------------------------------
// From MEikListBoxObserver
// Handle event from an listbox
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::HandleListBoxEventL( CEikListBox* /*aListBox*/,
                                                  TListBoxEvent aEventType )
    {       
    switch( aEventType )
        {
        case EEventPenDownOnItem:
            {            
            iPopupController->HidePopUp();
            // Check index
            TInt index = CurrentItemIndex();
            iListBox->View()->XYPosToItemIndex( iPenDownPoint, index );
            // Popup preview window
            HandleListBoxFocusChangedL( index );
            }
            break;
        case EEventItemClicked:
            {
            break;
            }
        case EEventItemDoubleClicked:
            {            
            if ( CurrentItemIndex() > 0 )
                {
                if ( MarkCount() == 0 )
                    {
                    // Pop context menu
                    PopupContextMenuL( R_TRUI_SHORTCUTSVIEW_CONTEXT_MENUBAR );
                    }
                }
            else
               {                
               // Tap on "New shortcut"
               AddShortcutL();
               }
            // Set the current index into engine
            // Update toolbar button's status
            break;
            }
        case EEventItemDraggingActioned:
            {            
            iPopupController->HidePopUp();            
            TInt index = CurrentItemIndex();
            iListBox->View()->XYPosToItemIndex( iPenDownPoint, index );
            // Popup preview window
            HandleListBoxFocusChangedL( index );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles key event.
// ---------------------------------------------------------------------------
//      
TKeyResponse CTruiShortcutsContainer::OfferKeyEventL
                                      ( const TKeyEvent& aKeyEvent, 
                                        TEventCode aType )
    {
    if ( aType == EEventKey )
        {        
        switch ( aKeyEvent.iCode )
            {            
            case EKeyLeftArrow:
                {                
                CAknNavigationDecorator* naviDecorator = 
                                         iAppUi->GetNaviDecorator();
                CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>
                                        ( naviDecorator->DecoratedControl() );
                return tabGroup->OfferKeyEventL( aKeyEvent, aType );   
                }
            case EKeyRightArrow:
                {                
                return EKeyWasConsumed;
                }
            case EKeyUpArrow:
            case EKeyDownArrow:
                {
                if ( iListBox )
                    {                    
                    iPopupController->HidePopUp();
                    TKeyResponse keyResponse = iListBox->OfferKeyEventL
                                               ( aKeyEvent, aType );
                    // Set the CBA button
                    if ( IsMarked( 0 ) )
                        {
                        iListBox->SetCurrentItemIndex( 0 );
                        AknSelectionService::HandleMarkableListProcessCommandL
                                             ( EAknCmdUnmark, iListBox );
                        iListBox->SetCurrentItemIndexAndDraw( iEngine->CurrentSelectedIndex() );
                        }                        
                    HandleListBoxFocusChangedL( CurrentItemIndex() );
                    return keyResponse;
                    }		   
			    }		
			case EKeyEnter:
			case EKeyOK:
			    {
			    
                if ( iListBox != NULL && CurrentItemIndex() == 0 )
                    {
                    // Select "Create new shortcut" item
                    AddShortcutL();
                    }
                else
                    {
                    if ( MarkCount() == 0 )
                        {
                        // Pop context menu
                        PopupContextMenuL( R_TRUI_SHORTCUTSVIEW_CONTEXT_MENUBAR );
                        }
                    }
                return EKeyWasConsumed;                
                 }
            default:
                break;			    
            }
       }    
    return EKeyWasNotConsumed;    
    }

// ---------------------------------------------------------------------------
// Gets the control's help context. Returns a NULL context by default.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidtruiApp;
    aContext.iContext = HWRT_HLP_SHORTCUTS;
    }
        
// ---------------------------------------------------------------------------
// Delete shortcut from ListBox
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::DeleteItemsL()
    {
    CTextListBoxModel* model = iListBox->Model();
    CDesCArray* itemArray = static_cast<CDesCArray*>( model->ItemTextArray() );
    if ( MarkCount() > 0 )
        {
        // Delete all marked items, ignoring the selected item.
        RArray<TInt> selectionIndexes;
        CleanupClosePushL( selectionIndexes );
        const CArrayFix<TInt>* indexArray = iListBox->SelectionIndexes();
        TInt indexArrayLen = indexArray->Count();
        
        // Copy the selection indexes array into RArray<TInt>
        for ( TInt i = 0; i < indexArrayLen; i++ )
            {
            selectionIndexes.Append( ( *indexArray )[i] );
            }
        selectionIndexes.Sort();
        // Delete all marked items    
        TInt selectionCount = selectionIndexes.Count();                    
        for ( TInt i = selectionCount - 1; i >= 0; i-- )
            {   
            // Delete item from Engine
            TInt itemArrayIndex = selectionIndexes[i];
            CDesCArray* shortcutList = iEngine->ShortcutTextList();

            if ( shortcutList )
                {
                iEngine->DeleteShortcut( (*shortcutList)[itemArrayIndex-1] );            
                // Delete the currently selected item from array
                itemArray->Delete( itemArrayIndex );
                // Update ListBox
                AknListBoxUtils::HandleItemRemovalAndPositionHighlightL( iListBox, 
                                                                        itemArrayIndex,
                                                                        ETrue);     
                }        
            }
        CleanupStack::PopAndDestroy( &selectionIndexes ); // selectionIndexes           
        }
    else
        {
        // Delete the currently selected item.
        // Get the currently selected item
        TInt selectedIndex = CurrentItemIndex();

        // Delete item from Engine
        CDesCArray* shortcutList = iEngine->ShortcutTextList();
        if ( shortcutList )
            {
            iEngine->DeleteShortcut( (*shortcutList)[selectedIndex-1] );            
            // Delete the currently selected item from array
            itemArray->Delete( selectedIndex );
            // Update ListBox
            AknListBoxUtils::HandleItemRemovalAndPositionHighlightL( iListBox, 
                                                                    selectedIndex,
                                                                    ETrue);     
            }        
    }
    iListBox->DrawNow();                
    iEngine->UpdateShortcutListL();
    // Set the current CBA
    HandleListBoxFocusChangedL( CurrentItemIndex() );
    }
    
// ---------------------------------------------------------------------------
// Delete shortcut
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::DeleteShortcutL()
    {
    // Popup note dialog
    CAknQueryDialog* dlg = CAknQueryDialog::NewL();
    HBufC* prompt = NULL;
    TInt count = MarkCount();
    if ( count )
        {
        prompt = StringLoader::LoadLC( R_QTN_HWRT_QUERY_DELETE_SEVERAL_SHORTCUTS, count );
        }
    else
        {
        prompt = StringLoader::LoadLC( R_QTN_HWRT_QUERY_DELETE_SHORTCUT );
        }
    
    if ( dlg->ExecuteLD( R_TRUI_QUERYDIALOG_DELETE_SHORTCUT, *prompt ) )
        {
        // press yes 
        // Delete item from Listbox, also delete from Engine in it.
        iPopupController->HidePopUp();
        DeleteItemsL();        
        }
    CleanupStack::PopAndDestroy( prompt ); // prompt
    }

// ---------------------------------------------------------------------------
// Handle event when listbox change to another focused item.
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::HandleListBoxFocusChangedL( TInt aIndex )
    {
    // Set the current index into engine
    iEngine->SetCurrentSelectedIndex( aIndex );
    // Update toolbar button's status
    UpdateToolbarButtonStatus( aIndex );
    // Draw current item // add this line
    iListBox->DrawItem( aIndex ); // add this line
    // Preview shortcut model if it has model.
    CheckAndPreviewShortcutModelL( aIndex );
    }
    
// ---------------------------------------------------------------------------
// Mark shortcut
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::MarkShortcutL()
    {
    // The first item mustn't be marked
    TInt currentIndex = CurrentItemIndex();
    if ( currentIndex != 0 )
        {
        AknSelectionService::HandleMarkableListProcessCommandL( EAknCmdMark,
                                                                iListBox );       
        }
        
    CAknToolbar* toolbar = iAppUi->View( KTruiShortcutsViewId )->Toolbar();
    if ( toolbar )
        {
        // Set edit text button's status
        CAknButton* editTextButton = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EShortcutsViewButtonIdEditText ) );
        editTextButton->SetDimmed( ETrue );
        editTextButton->DrawNow();
        // Set edit model button's status
        CAknButton* editModelButton = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EShortcutsViewButtonIdEditModel ) );
        editModelButton->SetDimmed( ETrue );
        editModelButton->DrawNow();
        }    
    }
    
// ---------------------------------------------------------------------------
// Unmark shortcut
// ---------------------------------------------------------------------------
//     
void CTruiShortcutsContainer::UnmarkShortcutL()
    {
    // The first item mustn't be unmarked
    TInt currentIndex = CurrentItemIndex();
    if ( currentIndex != 0 )
        {
        AknSelectionService::HandleMarkableListProcessCommandL( EAknCmdUnmark,
                                                                iListBox );
        }
    if ( MarkCount() == 0 )
        {
        CAknToolbar* toolbar = iAppUi->View( KTruiShortcutsViewId )->Toolbar();
        if ( toolbar )
            {
            // Set edit text button's status
            CAknButton* editTextButton = static_cast<CAknButton*>
                        ( toolbar->ControlOrNull( EShortcutsViewButtonIdEditText ) );
            editTextButton->SetDimmed( EFalse );
            editTextButton->DrawNow();
            // Set edit model button's status
            CAknButton* editModelButton = static_cast<CAknButton*>
                        ( toolbar->ControlOrNull( EShortcutsViewButtonIdEditModel ) );
            editModelButton->SetDimmed( EFalse );
            editModelButton->DrawNow();
            }
        }
    }
    
// ---------------------------------------------------------------------------
// Return current selected item's index
// ---------------------------------------------------------------------------
//
TInt CTruiShortcutsContainer::CurrentItemIndex()
    {       
    return iListBox->CurrentItemIndex();
    }

// ---------------------------------------------------------------------------
// Test if the currently selected index is marked
// ---------------------------------------------------------------------------
//    
TBool CTruiShortcutsContainer::IsMarked( TInt aSelectedIndex )
    {
    const CArrayFix<TInt>* aIndexArray = iListBox->SelectionIndexes();    
    for ( TInt index = 0; index < aIndexArray->Count(); index++ )
        {
        if ( aSelectedIndex == ( *aIndexArray )[index] )
            {
            return ETrue;
            }
        }
    return EFalse;        
    }

// ---------------------------------------------------------------------------
// Return the count of being marked currently.
// ---------------------------------------------------------------------------
// 
TInt CTruiShortcutsContainer::MarkCount()
    {
    const CArrayFix<TInt>* aIndexArray = iListBox->SelectionIndexes();
    return aIndexArray->Count();
    }

// ---------------------------------------------------------------------------
// Create preview popup controller to show popup hwrbox.
// ---------------------------------------------------------------------------
// 
CAknPreviewPopUpController* CTruiShortcutsContainer::CreatePopupHwrBoxL()
    {   
    iHwrBox = CTruiHwrBox::NewL( this, NULL );   
    iHwrBox->SetHwrBoxFlag( CTruiHwrBox::ENoFrameHwrBox 
                            | CTruiHwrBox::EReadOnlyHwrBox );
    CAknPreviewPopUpController* popupController =
                      CAknPreviewPopUpController::NewL
                      ( *iHwrBox, CAknPreviewPopUpController::EPermanentMode );
    popupController->AddObserverL( *this );
    return popupController;
    }

// ---------------------------------------------------------------------------
// Show the text tooltip.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsContainer::PopupHwrBoxL( const TPoint& aPosition, 
                           const TTimeIntervalMicroSeconds32& aShowDelay,
                           const TTimeIntervalMicroSeconds32& aHideDelay )
    {    
    if ( iPopupController && iHwrBox )
        {
        iPopupController->SetPosition( aPosition );
        // Set hwrbox's rect to engine and set guiding line to hwrbox
        iHwrBox->SetDisplayBottomGuideLine( ETrue );
        if ( iEngine->CurrentLanguageScript() == EMainViewSubmenuHebrew )
            {
            iHwrBox->SetDisplayTopGuideLine( ETrue );
            }        
        iPopupController->SetPopUpShowDelay( aShowDelay );
        iPopupController->SetPopUpHideDelay( aHideDelay );
        iPopupController->ShowPopUp();
        }
    }   
    
// ---------------------------------------------------------------------------
// Preview shortcut model in popup hwrbox.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsContainer::PreviewShortcutModelL( const TDesC& aShortcutText,
                                                     TInt aIndex )
    {
    // Get shortcut model
    if ( iEngine->CheckShortcutModel( aShortcutText ) )
        {        
        TRAPD( err, iEngine->GetShortcutModelL( aShortcutText, 
                                                iHwrBox->Model(), 
                                                iShortcutUnicode ) );
        if ( err == KErrNone )
            { 
            // Popup hwrbox
            // Get position for popup window
            TPoint itemPos = PopupWindowPosition( aIndex );            
            PopupHwrBoxL( itemPos,
                          KTooltipShowDelay,
                          KTooltipHideDelay );
            }
        }    
    }
    
// ---------------------------------------------------------------------------
// Called by the preview popup when an appropriate event takes place.
// ---------------------------------------------------------------------------
//
void CTruiShortcutsContainer::HandlePreviewPopUpEventL(
      CAknPreviewPopUpController* aController, TPreviewPopUpEvent aEvent )
    {
    if ( aController == iPopupController && aEvent == EPreviewPopUpShown )
        {
        TSize refSize = CTruiContainerBase::OriginSymbolSize();       
        iHwrBox->ShowTrails( iHwrBox->Model(), ETrue, refSize );        
        iDisplayPopupWindow = EFalse;
        }
    else
        {
        iHwrBox->ClearExistModel();
        iHwrBox->DrawNow();
        iDisplayPopupWindow = ETrue;
        }
    }

// ---------------------------------------------------------------------------
// Create list box to display all shortcut
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsContainer::CreateListBoxL()
    {
    // listbox instance
    iListBox = new (ELeave) CAknSingleGraphicStyleListBox;
    Components().AppendLC( iListBox );
    CleanupStack::Pop( iListBox ); // iListBox
    
    // Construct listbox
    iListBox->ConstructL( this, EAknListBoxMarkableList ); 

    // Set container control
    iListBox->SetContainerWindowL( *this );
    
    // Set observer
    iListBox->SetListBoxObserver( this );

    // Add scrollbars to listbox
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EAuto, CEikScrollBarFrame::EAuto );    

    // Get shortcut list from engine
    CDesCArray* primalList = iEngine->ShortcutTextList();   

    LoadTextArrayL( primalList );

    // construct icon array with granularity 3
    CArrayPtr<CGulIcon>* iconList = new (ELeave) CAknIconArray( KIconArraySize );
    CleanupStack::PushL( iconList );
    CFbsBitmap* bitmap;
    CFbsBitmap* bitmapm;

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiMarkedAdd,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG16,
                                 bitmap,
                                 bitmapm,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_indi_marked_add,
                                 EMbmTrui_iconsQgn_indi_marked_add_mask,
                                 KRgbBlack
                                 ); 
    CleanupStack::PushL( bitmap );
    CleanupStack::PushL( bitmapm );
    CGulIcon* iconMark = CGulIcon::NewL( bitmap, bitmapm ); // Ownership transfered
    CleanupStack::Pop( bitmapm );
    CleanupStack::Pop( bitmap );                                   
    CleanupStack::PushL( iconMark );
    iconList->AppendL( iconMark );
    CleanupStack::Pop( iconMark );
    
    AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnPropHwrTrainingShortcut,
                            bitmap,
                            bitmapm,
                            KTRUIICONSFILENAME,
                            EMbmTrui_iconsQgn_prop_hwrtraining_shortcut_new,
                            EMbmTrui_iconsQgn_prop_hwrtraining_shortcut_new_mask
                            );
    CleanupStack::PushL( bitmap );
    CleanupStack::PushL( bitmapm );    
    CGulIcon* iconNew = CGulIcon::NewL( bitmap, bitmapm ); // Ownership transfered
    CleanupStack::Pop( bitmapm );
    CleanupStack::Pop( bitmap );
    CleanupStack::PushL( iconNew );
    iconList->AppendL( iconNew );
    CleanupStack::Pop( iconNew );

    iListBox->ItemDrawer()->ColumnData()->SetIconArray( iconList );
    
    CleanupStack::Pop( iconList ); // iconList

    iListBox->SetFocus( ETrue );
    // update listbox
    iListBox->ActivateL();    
    }

// ---------------------------------------------------------------------------
// Setup text data for listbox
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsContainer::LoadTextArrayL( const CDesCArray* aTextArray )
    {  
    // Construct listbox item array.
    // The items in the list contain an option of create new shortcut
    // and the primal shortcuts from HWR engine
    CDesCArray* itemList = new (ELeave) CDesCArrayFlat( KItemArraySize ); 
    CleanupStack::PushL(itemList);
          
    TBuf<KShortcutMaxLength> item;    
    HBufC* newShortcutText = StringLoader::LoadL( R_TRUI_SHORTCUTVIEW_NEW_SHORTCUT );
    item.Format( KListItemWithPicFormat, newShortcutText );
    delete newShortcutText;
    newShortcutText = NULL;
    itemList->AppendL(item);
    
    // if at least one shortcut exists
     if ( aTextArray )
        {
        // Item from WHR engine
        for ( TInt i=0; i < aTextArray->Count(); i++ )
            {
            TBuf<KShortcutMaxLength> itemPrimal;
            itemPrimal = aTextArray->MdcaPoint(i);
            item.Format( KListItemFormat, &itemPrimal );
            TBuf<KParaDelimiterSize> paraDelimiter;
            paraDelimiter.Format( KParaDelimiterFormat, EKeyEnter );
            AknTextUtils::ReplaceCharacters( item, paraDelimiter, TChar( EKeySpace ) );          
            itemList->AppendL( item );
            }        
        } 
    // set items and ownership
    CleanupStack::Pop( itemList );
    iListBox->Model()->SetItemTextArray( itemList );
    iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
    iListBox->HandleItemAdditionL();    
    }
    
// ---------------------------------------------------------------------------
// Popup context menu.
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsContainer::PopupContextMenuL( TInt aResourceId )
    {    
    // Hide popup preview window
    iPopupController->HidePopUp();  
    // Popup context menu  
    CEikMenuBar* parentMenuBar = iAppUi->View( KTruiShortcutsViewId )->MenuBar();
    if ( parentMenuBar )
        {
        parentMenuBar->SetMenuTitleResourceId( aResourceId );
        parentMenuBar->SetMenuType( CEikMenuBar::EMenuContext );
        parentMenuBar->TryDisplayMenuBarL();
        parentMenuBar->SetMenuTitleResourceId( R_TRUI_SHORTCUTSVIEW_MENUBAR );
        parentMenuBar->SetMenuType( CEikMenuBar::EMenuOptions );
        }    
    }

// ---------------------------------------------------------------------------
// Get position relative to screen origin for popup window.
// ---------------------------------------------------------------------------
// 
TPoint CTruiShortcutsContainer::PopupWindowPosition( TInt aIndex )
    {
    // Place popup window to left-bottom
    TPoint itemPos = iListBox->View()->ItemPos( aIndex ) 
                     + iListBox->PositionRelativeToScreen();
    TSize itemSize = iListBox->View()->ItemSize( aIndex );    
    itemPos.iY += itemSize.iHeight;
    
    // if direction is upforwards.
    if ( Rect().iBr.iY + iListBox->PositionRelativeToScreen().iY - 
         ( itemPos.iY + iPopupController->Size().iHeight ) < 0 )
        {
        itemPos.iY -= ( itemSize.iHeight + iPopupController->Size().iHeight );
        }
        
    if ( TBidiText::ScriptDirectionality( User::Language() )
         ==  TBidiText::ELeftToRight )
        { // None-arabic,hebrew, place popup window to right-bottom
        itemPos.iX += itemSize.iWidth;
        // Sub width of scroll bar
        TInt scrollbarWidth = 0;
        if ( iListBox->ScrollBarFrame() )
            {
            scrollbarWidth = iListBox->ScrollBarFrame()
                             ->ScrollBarBreadth( CEikScrollBar::EVertical );
            }
        itemPos.iX -= scrollbarWidth;
        }
    else
        {
        itemPos.iX += iPopupController->Size().iWidth;        
        }
    return itemPos;
    }
    
// ---------------------------------------------------------------------------
// Check if need to preview shortcut and perform previewing necessarily.
// ---------------------------------------------------------------------------
// 
TBool CTruiShortcutsContainer::CheckAndPreviewShortcutModelL( TInt aIndex )
    {
    TBool ret = EFalse;
    if ( aIndex > 0 )
        {
        // Get selected shortcut text
        CDesCArray* shortcutlist = iEngine->ShortcutTextList();
        TInt posInEngine = aIndex - 1;
        // Popup hwr box to preview
        PreviewShortcutModelL( shortcutlist->MdcaPoint( posInEngine ), aIndex );
        ret = ETrue;
        }
    else
        {
        iPopupController->HidePopUp();
        ret = EFalse;
        }    
    return ret;
    }
    
// ---------------------------------------------------------------------------
// Update buttons' status in toolbar
// ---------------------------------------------------------------------------
// 
void CTruiShortcutsContainer::UpdateToolbarButtonStatus( TInt aIndex )
    {
    CAknToolbar* toolbar = iAppUi->View( KTruiShortcutsViewId )->Toolbar();
    if ( toolbar )
        {
        // Set edit text button's status
        CAknButton* editTextButton = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EShortcutsViewButtonIdEditText ) );
        if ( editTextButton )
            {
            if ( aIndex > 0 )
                {
                if ( MarkCount() > 0 )
                    {
                    editTextButton->SetDimmed( ETrue );
                    }
                else
                    {
                    editTextButton->SetDimmed( EFalse );
                    }
                }
            else
                {
                editTextButton->SetDimmed( ETrue );
                }
            editTextButton->DrawNow();
            }
            
        // Set edit model button's status
        CAknButton* editModelButton = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EShortcutsViewButtonIdEditModel ) );
        if ( editModelButton )
            {
            if( aIndex > 0 )
                {
                if ( MarkCount() > 0 )
                    {
                    editModelButton->SetDimmed( ETrue );
                    }
                else
                    {
                    CDesCArray* shortcutlist = iEngine->ShortcutTextList();
                    TInt posInEngine = aIndex - 1;
                    if ( iEngine->CheckShortcutModel
                                 ( shortcutlist->MdcaPoint( posInEngine ) ) )
                        {
                        // Shortcut model exist and allow to edit model.
                        editModelButton->SetDimmed( EFalse );
                        }
                    }
                }
            else
                {
                editModelButton->SetDimmed( ETrue );
                }
            editModelButton->DrawNow();
            }
                    
        // Set delete button's status            
        CAknButton* deleteButton = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EShortcutsViewButtonIdDelete ) );
        if ( deleteButton )  
            {
            TBool isDimmed = aIndex > 0 ? EFalse : ETrue;
            deleteButton->SetDimmed( isDimmed );
            deleteButton->DrawNow(); 
            }
        }
    }
