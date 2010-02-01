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
* Description:  Implement of class CTruiShortcutEditContainer
*
*/


#include <eikedwin.h>
#include <aknbutton.h>
#include <eiklabel.h>
#include <barsread.h>
#include <aknnavide.h>
#include <aknnotewrappers.h> 
#include <trui.rsg>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <avkon.rsg>
#include <layoutmetadata.cdl.h>
#include <akntoolbar.h>
#include <aknSctDialog.h>

#include "truicontainerbase.h"
#include "truishortcuteditcontainer.h"
#include "truihwrbox.h"
#include "truiengine.h"
#include "truiappui.h"
#include "truishortcutsview.h"
#include "truiapplication.h"
#include "truiappradiobutton.h"
#include "truishortcutsview.h"
#include "truiapplication.h"
#include "truishortcutedittextview.h"
#include "trui.hrh"
#include "truishortcuteditview.h"

const TInt KSimilarMsgLength = 160;

// The maximum number of lines for tips
const TInt KIndicatorBufSize = 2;

// The index of each item in radio button groups
const TInt KOwnIndex = 0;
const TInt KPresetIndex = 1;

// Layout for portrait
const TPoint KLabelTipPosPortrait = TPoint( 0, 0 );
const TSize KLabelTipSizePortrait = TSize( 360, 60 );
const TPoint KRadioPosPortrait = TPoint( 5, 73 );
const TSize KRadioSizePortrait = TSize( 196, 99 );
const TPoint KLeftBtnPosPotrait = TPoint( 115, 160 );
const TPoint KIndicatorPosPotrait = TPoint( 165, 160 );
const TPoint KRightBtnPosPotrait = TPoint( 215, 160 );
const TPoint KHwrBoxPosPortrait = TPoint( 90, 215 );

const TSize KButtonSize = TSize( 50, 50 );

// Layout for landscape
const TPoint KRadioPosLandscape = TPoint( 5, 18 );
const TPoint KLabelTipPosLandscape = TPoint( 5, 120 );
const TSize KLabelTipSizeLandscape = TSize( 250, 120 );

const TPoint KLeftBtnPos = TPoint( 300, 15 );

const TPoint KRightBtnPos = TPoint( 400, 15 );
const TPoint KIndicatorPosLandscape = TPoint( 350, 15 );
const TSize KHwrBoxSizeLandscape = TSize( 200, 200 );
const TPoint KHwrBoxPosLandscape = TPoint( 275, 80 );

const TInt KTooltipPopupTimeOut = 0;
const TInt KTooltipStayTimeOut = 3000;

const TInt KSctBufferSize = 32;
const TInt KHwrTooltipY = 20;

_LIT( KActiveCharFormat, "%c" );

// Varity of landscape for common components
const TInt KVarityLandscape = 3;

// Varity of portrait for common components
const TInt KVarityPortrait = 2;

const TInt KVarityListPane = 1;

// ======== MEMBER FUNCTIONS ========

CTruiShortcutEditContainer::CTruiShortcutEditContainer()
    {  
    iActiveCharPos = KErrNotFound;
    }
    
void CTruiShortcutEditContainer::ConstructL( const TRect& aRect )
    {    
    // Initialize control array
    InitComponentArrayL();
        
    // Set parent control or owning window
    CreateWindowL();

    iBackGround = CAknsBasicBackgroundControlContext::NewL
                  ( KAknsIIDQsnBgAreaMain, Rect(), EFalse);
    
    // Initialize controls
    InitializeControlsL();       
    
#ifdef TRAININGUI_FOR_PRESET
    // Get all presets from Engine
    HBufC* presetHBuf = iEikonEnv->AllocReadResourceLC( R_TRUI_PRESET_SYMBOLS );
    for ( TInt i = 0; i < presetHBuf->Length(); i++ )
        {
        iPresets.Append( ( *presetHBuf )[i] );
        }
    CleanupStack::PopAndDestroy( presetHBuf );        
#endif
      
    // Load setting data
    CTruiShortcutEditView* view = static_cast<CTruiShortcutEditView*>
                           ( iAppUi->View( KTruiShortcutEditViewId ) );
    
    if ( view->PreviousViewId() == TVwsViewId( KUidtruiApp, KTruiShortcutsViewId ) )
        {
        // Edit old shortcut
        iEngine->GetShortcutModelL( iEngine->Shortcut(), 
                                    iHwrBox->Model(), 
                                    iUnicode );
        // Backup shortcut model
        BackupShortcutModel( iHwrBox->Model(), iUnicode );
        // Load the current active char whether is own or preset.
        iShortcutType = iUnicode ? KPresetIndex : KOwnIndex;
        iIsEmptyHwrBox = iUnicode ? ETrue : EFalse;
#ifdef TRAININGUI_FOR_PRESET
        if ( iUnicode )
            {
            // Edit preset shortcut
            iActiveCharPos = iPresets.Find( iUnicode );
            }
        else
            {
            // Edit own shortcut, get the first available preset shortcut
            if ( FindFirstAvailableCharL( 0, iPresets.Count() - 1, 1, iUnicode ) )
                {
                iActiveCharPos = iPresets.Find( iUnicode );
                }
            else
                {        
                iActiveCharPos = KErrNotFound;
                }            
            }
#endif
        }
    else
        {
        // Create new shortcut        
        // Load the current active char whether is own or preset.
        iShortcutType = KOwnIndex;
        iIsEmptyHwrBox = ETrue;
#ifdef TRAININGUI_FOR_PRESET
        if ( FindFirstAvailableCharL( 0, iPresets.Count() - 1, 1, iUnicode ) )
            {
            iActiveCharPos = iPresets.Find( iUnicode );
            }
        else
            {        
            iActiveCharPos = KErrNotFound;
            }
#endif
        }

    // Set radio button
    iRadioButtonContainer->HandlePointerRadioButtonL( iShortcutType, 
                                                      EEventRequestFocus );
    SetRect( aRect );
    
    ActivateL();
    }
    
CTruiShortcutEditContainer* CTruiShortcutEditContainer::NewL( const TRect& aRect )
    {
    CTruiShortcutEditContainer* self = CTruiShortcutEditContainer::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }
    
CTruiShortcutEditContainer* CTruiShortcutEditContainer::NewLC( const TRect& aRect )
    {
    CTruiShortcutEditContainer* self = new (ELeave) CTruiShortcutEditContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }
    
CTruiShortcutEditContainer::~CTruiShortcutEditContainer()
    {
    delete iBackGround;
    delete iLabeltext;
    delete iWrappedLabelText;
    iModelBak.Close();
    iPresets.Close();
    iLineWidthArray.Close();
    }
    
// ---------------------------------------------------------------------------
// Create controls to be displayed in this container.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::InitializeControlsL()
    {
    // Create fixed toolbar
    CAknToolbar* toolbar = iAppUi->View( KTruiShortcutEditViewId )->Toolbar();
    // Get Clear button from toolbar    
    CAknButton* clearButton = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EShortcutEditViewDelete ) );

    CreateRadioButtonL();         

    CcnstructButtonL();
    
    CreateLabelL();
    
    CreateHwrBoxL();
    }
    
// ---------------------------------------------------------------------------
// From class CCoeControl.
// Responds to changes to the size and position of the contents of this control.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::SizeChanged()
    {
    TInt varity = Layout_Meta_Data::IsLandscapeOrientation() ? KVarityLandscape
                                                             : KVarityPortrait;
    DoSizeChanged( varity );
    // Set hwrbox's rect to engine and set guiding line to hwrbox
    iHwrBox->SetDisplayBottomGuideLine( ETrue );
    TBool isDisplayTopGuideLine = ( iEngine->CurrentLanguageScript() 
                             == EMainViewSubmenuHebrew ) ? ETrue : EFalse;
    
    iHwrBox->SetDisplayTopGuideLine( isDisplayTopGuideLine );
        
    if ( varity == KVarityLandscape )
        {
        DoLayoutLandscape();
        }
    else if ( varity == KVarityPortrait )  
        {
        DoLayoutPortait();
        }
    if ( iBackGround )
        {
        iBackGround->SetRect( Rect() );
        }    
    }

// ---------------------------------------------------------------------------
// Layout under landscape condition.
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::DoLayoutLandscape()
    {    
#ifdef TRAININGUI_FOR_PRESET  
    iRadioButtonContainer->SetRect( TRect( KRadioPosLandscape, 
                                           KRadioSizePortrait ) );
    iLeftBtn->SetRect( TRect( KLeftBtnPos, KButtonSize ) );
    iRightBtn->SetRect( TRect( KRightBtnPos, KButtonSize ) );
    iIndicator->SetRect( TRect( KIndicatorPosLandscape, KButtonSize ) );
#endif
    }
    
// ---------------------------------------------------------------------------
// Layout under portait condition.
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::DoLayoutPortait()
    {
#ifdef TRAININGUI_FOR_PRESET
    iRadioButtonContainer->SetRect( TRect( KRadioPosPortrait, 
                                           KRadioSizePortrait ) );    
    iLeftBtn->SetRect( TRect( KLeftBtnPosPotrait, KButtonSize ) );
    iRightBtn->SetRect( TRect( KRightBtnPosPotrait, KButtonSize ) );
    iIndicator->SetRect( TRect( KIndicatorPosPotrait, KButtonSize ) );
#endif
    }    

// ---------------------------------------------------------------------------
// Save own shortcut.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::SaveOwnShortcutL( const TDesC& aShortcut, 
                                                   RArray<TPoint>& aModel,
                                                   TDes& aErrMsg )
    {
    // Ensure the shortcut doesn't exist.
    if ( iEngine->CheckShortcutModel( aShortcut ) )
        {
        iEngine->DeleteShortcut( aShortcut );
        }
    if ( aModel.Count() )
        {
        iEngine->SaveShortcutL( aShortcut, aModel, aErrMsg );
        }
    }    
    
// ---------------------------------------------------------------------------
// Save preset shortcut.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::SavePresetShortcutL( const TDesC& aShortcut, 
                                                      TUint aUnicode )
    {
    // Ensure the shortcut doesn't exist.
    if ( iEngine->CheckShortcutModel( aShortcut ) )
        {
        iEngine->DeleteShortcut( aShortcut );
        }
    // Check if aUnicode is in use.
    RArray<TPoint> presetModel;
    CleanupClosePushL( presetModel );
    TBuf<KSimilarMsgLength> msg;
    iEngine->GetPresetSymbolByUnicodeL( aUnicode, presetModel, msg );
    if ( msg.Length() > 0 )
        {
        // In use, being assigned to some shortcut.
        User::Leave( KErrInUse );
        }
    CleanupStack::PopAndDestroy( &presetModel );
    iEngine->SavePresetShortcutL( aUnicode, aShortcut );
    }

// ---------------------------------------------------------------------------
// Backup shortcut model. It is probably to be restored when press Back to exit.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::BackupShortcutModel
                                 ( const RArray<TPoint>& aOriginalModel,
                                   TUint aOriginalUnicode )
    {
    iUnicodeBak = aOriginalUnicode;
    if ( !iUnicodeBak )
        {
        // If old shortcut is own, it is necessary to store trails.
        // If it is preset, backup is unecessary
        for ( TInt i = 0; i < aOriginalModel.Count(); i++ )
            {
            iModelBak.Append( aOriginalModel[i] );
            }        
        }    
    }

// ---------------------------------------------------------------------------
// Set active char.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::SetActiveCharL( TUint aUnicode )
    {
    // Set active indicator
    TBuf<KIndicatorBufSize> preset;

    // Preset symbol
    preset.Format( KActiveCharFormat, aUnicode );
    iIndicator->SetTextL( preset );        
    iIndicator->DrawNow();    
    // Clear hwrbox and stop playing animation    
    iHwrBox->ClearExistModel();
    // Get model
    TBuf<KSimilarMsgLength> similarMsg;
    iEngine->GetPresetSymbolByUnicodeL( aUnicode, iHwrBox->Model(),
                                        similarMsg );
    // Play animation in HwrBox 
    iHwrBox->ModelIsReady( ETrue );
    if ( iShortcutType == KPresetIndex )
        {
        // As for preset, need to scale trails size firstly
        iHwrBox->PlayAnimation( ETrue, CTruiContainerBase::OriginSymbolSize() );
        }
    else
        {
        iHwrBox->PlayAnimation();
        }    
    }

// ---------------------------------------------------------------------------
// Find the first available preset symbol.
// ---------------------------------------------------------------------------
//
TBool CTruiShortcutEditContainer::FindFirstAvailableCharL( TInt aStartPos, 
                                                           TInt aEndPos,
                                                           TInt aDirection, 
                                                           TUint& aUnicode )
    {
    while ( aStartPos != aEndPos && aStartPos < iPresets.Count() )
        {
        if ( !CheckPresetSymbolInUseL( iPresets[aStartPos] ) )
            {            
            aUnicode = iPresets[aStartPos];
            return ETrue;
            }
        else
            {
            // In use
            if ( iUnicodeBak == iPresets[aStartPos] )
                {
                // But the preset symbol is the old preset 
                // which users are editting, 
                // Still display the symbol in indicator                
                aUnicode = iPresets[aStartPos];
                return ETrue;
                }
            }
        if ( aDirection )
            {
            // Go right           
            aStartPos = ( ++aStartPos + iPresets.Count() ) % iPresets.Count();
            }
        else
            {
            // Go left
            aStartPos = ( --aStartPos + iPresets.Count() ) % iPresets.Count();
            }                    
        }
    if ( aEndPos < iPresets.Count() && !CheckPresetSymbolInUseL( iPresets[aEndPos] ) )
        {
        aUnicode = iPresets[aEndPos];
        return ETrue;
        }      
    return EFalse;           
    }
    
// ---------------------------------------------------------------------------
// Return if the preset is in use.
// ---------------------------------------------------------------------------
//
TBool CTruiShortcutEditContainer::CheckPresetSymbolInUseL( TUint aUnicode )
    {
    // Check if iUnicode is in use
    RArray<TPoint> presetModels;
    CleanupClosePushL( presetModels );
    TBuf<KSimilarMsgLength> similarMsg;      
    iEngine->GetPresetSymbolByUnicodeL( aUnicode,
                                        presetModels,
                                        similarMsg );
    CleanupStack::PopAndDestroy( &presetModels );
    if ( similarMsg.Length() )
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// Do something to response to size changed.
// ---------------------------------------------------------------------------
// 
void CTruiShortcutEditContainer::DoSizeChanged( TInt aVarity )
    {
    TRect rect = Rect();
    
    TAknWindowComponentLayout main_pane_layout = 
                            AknLayoutScalable_Apps::main_hwr_training_pane();
    TAknLayoutRect main_pane_layout_rect;
    main_pane_layout_rect.LayoutRect( rect, main_pane_layout );
    TRect main_pane_rect = main_pane_layout_rect.Rect();
        
    // hwr_training_write_pane
    TAknWindowComponentLayout write_pane_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane( aVarity );
    TAknLayoutRect write_pane_layout_rect;
    write_pane_layout_rect.LayoutRect( main_pane_rect, write_pane_layout );
    iHwrBox->SetRect( write_pane_layout_rect.Rect() );
    
    TPoint hwrPos = iHwrBox->PositionRelativeToScreen();
    iHwrBox->SetTooltipPosition( hwrPos + TPoint( iHwrBox->Size().iWidth, KHwrTooltipY ) );

    // hwr_training_instruct_pane
    TAknWindowComponentLayout instruct_pane_layout = 
               AknLayoutScalable_Apps::hwr_training_instruct_pane( aVarity );
    TAknLayoutRect instruct_pane_layout_rect;
    instruct_pane_layout_rect.LayoutRect( main_pane_rect, instruct_pane_layout );    
    // count of text rows
    TInt count = AknLayoutScalable_Apps::
                 list_single_hwr_training_instruct_pane_ParamLimits( KVarityListPane )
                 .LastRow() + 1;

    // array contains all text rows in help screen
    RArray<TAknTextComponentLayout> textLayoutArray;
    iLineWidthArray.Reset();
    for( TInt i = 0; i < count; i++ )
        {
        // pane area for each row
        TAknWindowComponentLayout each_row_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_instruct_pane
            ( KVarityListPane, 0, i );
        TAknLayoutRect each_row_layout_rect;
        each_row_layout_rect.LayoutRect( instruct_pane_layout_rect.Rect(), 
                                         each_row_layout );
        iLineWidthArray.Append( each_row_layout_rect.Rect().Width() );
        // smaller pane for text itself
        TAknTextComponentLayout each_row_text_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_instruct_pane_t1();
    
        // composition of two latter componets
        TAknTextComponentLayout textCompCompositionWiz = 
            TAknWindowComponentLayout::ComposeText(
                each_row_layout, each_row_text_layout );
 
        TRAPD(error, textLayoutArray.AppendL( textCompCompositionWiz ) );
        // if error in append function, then just exit the loop      
        if ( error != KErrNone )
            {
            break;
            }
        }       
    iMultilineLayout = TAknTextComponentLayout::Multiline( textLayoutArray ); 
    if ( iLabeltext && iWrappedLabelText )
        {
        // Be ready to show label
        TAknLayoutText multilineTextLayout;
        multilineTextLayout.LayoutText( instruct_pane_layout_rect.Rect(), iMultilineLayout );    
        const CFont* font = AknLayoutUtils::FontFromId( iMultilineLayout.FontId() );
        TRAPD( err, WrapLabelTextL( font, *iLabeltext, iLineWidthArray, iWrappedLabelText ) );
        if ( err == KErrNone )
            {
            TRAP_IGNORE( iLabelTip->SetTextL( *iWrappedLabelText ) );
            }
        else
            {
            TRAP_IGNORE( iLabelTip->SetTextL( *iLabeltext ) );
            }        
        AknLayoutUtils::LayoutLabel( iLabelTip, 
                                     instruct_pane_layout_rect.Rect(), 
                                     iMultilineLayout );
        }
    textLayoutArray.Close();

    TRgb labelColor = KRgbBlack;
    TInt error = GetCachedLabelTextColor( labelColor );
    if ( error == KErrNone )
        {
        TRAP_IGNORE( iLabelTip->OverrideColorL( EColorLabelText, labelColor ) );
        TRAP_IGNORE( iIndicator->OverrideColorL( EColorLabelText, labelColor ) );
        }
    }
        
// ---------------------------------------------------------------------------
// Create radio button.
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::CreateRadioButtonL()
    {
    iRadioButtonContainer = CTruiRadioButton::NewL( this,
                                             CTruiRadioButton::ERadioButton,
                                             this );
    Components().AppendLC( iRadioButtonContainer );
    CleanupStack::Pop( iRadioButtonContainer );
    
    HBufC* radioTextOwn = iEikonEnv->AllocReadResourceLC
                             ( R_TRUI_SHORTCUTEDITMODELVIEW_RADIOBUTTON_OWN );
    iRadioButtonContainer->AddItem( radioTextOwn, CTruiRadioButton::ENonSelected );
    CleanupStack::Pop( radioTextOwn ); // transfered ownership
    HBufC* radioTextPreset = iEikonEnv->AllocReadResourceLC
                             ( R_TRUI_SHORTCUTEDITMODELVIEW_RADIOBUTTON_PRESET );
    iRadioButtonContainer->AddItem( radioTextPreset, CTruiRadioButton::ENonSelected );
    CleanupStack::Pop( radioTextPreset ); // transfered ownership
    iRadioButtonContainer->HandleItemAdditionL();
    }
   
// ---------------------------------------------------------------------------
// Create HWR box to write and play model
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::CreateHwrBoxL()
    {
    CTruiHwrBox* hwrBox = CTruiHwrBox::NewL( this, this, iBackGround );    
    hwrBox->SetHwrBoxFlag( CTruiHwrBox::EFrameHwrBox );
    Components().AppendLC( hwrBox );
    hwrBox->ActivateL();    
    CleanupStack::Pop( hwrBox );
    iHwrBox = hwrBox;
    }

// ---------------------------------------------------------------------------
// Create buttons.
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::CcnstructButtonL()
    {
    // Left button for last character
    CAknButton* preButton = CreateButtonL( R_MAINVIEW_BUTTON_LEFT,
                                           this,
                                           this );
    Components().AppendLC( preButton );
    CleanupStack::Pop( preButton );    
    iLeftBtn = preButton;

    // Right button for next character
    CAknButton* nextButton = CreateButtonL( R_MAINVIEW_BUTTON_RIGHT,
                                           this,
                                           this );
    Components().AppendLC( nextButton );
    CleanupStack::Pop( nextButton );    
    iRightBtn = nextButton;    
    }

// ---------------------------------------------------------------------------
// Create buttons.
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::CreateLabelL()
    {
    //Create label tip
    CEikLabel* tipLabel = new (ELeave) CEikLabel();
    CleanupStack::PushL( tipLabel );    
    tipLabel->SetContainerWindowL( *this );
    tipLabel->SetTextL( KNullDesC );
    tipLabel->ActivateL();
    CleanupStack::Pop( tipLabel );
    Components().AppendLC( tipLabel );
    CleanupStack::Pop( tipLabel );
    iLabelTip = tipLabel;    

    // Create active character indicator
    CEikLabel* aknLabel = new (ELeave) CEikLabel();
    CleanupStack::PushL( aknLabel );
    aknLabel->SetContainerWindowL( *this );
    aknLabel->SetObserver( this );
    aknLabel->SetAlignment( 
         TGulAlignmentValue ( EEikLabelAlignHCenter|EEikLabelAlignVCenter ) );
    aknLabel->SetTextL( KNullDesC );
    aknLabel->ActivateL();
    CleanupStack::Pop( aknLabel );
    Components().AppendLC( aknLabel );    
    CleanupStack::Pop( aknLabel );     
    iIndicator = aknLabel;
    iIndicator->SetObserver( this );    
    }
    
// ---------------------------------------------------------------------------
// Draw.
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    // Gets the control's extent
    TRect rect( Rect());
    // Clears the screen
    gc.Clear( rect );

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();  
    if ( AknsDrawUtils::HasBitmapBackground( skin, iBackGround ) ) 
        {
        AknsDrawUtils::Background( skin, iBackGround, gc, rect );
        }   
    }

// ---------------------------------------------------------------------------
// Switch to shortcuts view.
// ---------------------------------------------------------------------------
//  
void CTruiShortcutEditContainer::SwitchToShortcutViewL()
    {    
    // Update shortcut list
    iEngine->UpdateShortcutListL();
    // Find the current shortcut's index in shortcut list
    CDesCArray* shortcutList = iEngine->ShortcutTextList();
    TInt index = DefaultSelectedIndex( shortcutList, iEngine->Shortcut() );
    if ( index == KErrNotFound )
        {
        TInt oldIndex = iEngine->CurrentSelectedIndex();
        if ( shortcutList )
            {
            index = ( oldIndex > shortcutList->Count() ) 
                               ? shortcutList->Count() : oldIndex;
            }
        else
            {
            index = 0;
            }
        }
       
    // Set current index in Engine
    iEngine->SetCurrentSelectedIndex( index );

    // Close tooltip
    iHwrBox->HideInfoPopupNote();

    iAppUi->ActivateLocalViewL( KTruiShortcutsViewId );
    // Show NaviDecorator
    iAppUi->ShowNaviPaneL();
    }

// ---------------------------------------------------------------------------
// Go back to previous view
// ---------------------------------------------------------------------------
//  
void CTruiShortcutEditContainer::BackToPreviousViewL()
    {
    if ( iIsRestored )
        {
        // Check if restore the old shortcut
        if ( iUnicodeBak )
            {
            // Old shortcut is preset, restore it.
            SavePresetShortcutL( iEngine->Shortcut(), iUnicodeBak );
            }
        else
            {
            // Old shortcut is own, restore it.
            // Or create new shortcut
            TBuf<KSimilarMsgLength> similarMsg;
            SaveOwnShortcutL( iEngine->Shortcut(), iModelBak, similarMsg );
            }        
        }  
        
    // Close tooltip
    iHwrBox->HideInfoPopupNote();
    
    // Show NaviDecorator      
    CTruiShortcutEditView* view = static_cast<CTruiShortcutEditView*>
                           ( iAppUi->View( KTruiShortcutEditViewId ) );
    iAppUi->ActivateLocalViewL( view->PreviousViewId().iViewUid );
    
    if ( view->PreviousViewId() == TVwsViewId( KUidtruiApp, KTruiShortcutsViewId ) )
        {
        iAppUi->ShowNaviPaneL();        
        }    
    }
    
// ---------------------------------------------------------------------------
// Handles an event from an observed control.
// ---------------------------------------------------------------------------
//     
void CTruiShortcutEditContainer::HandleControlEventL( CCoeControl* aControl,
                                                      TCoeEvent aEventType )
    {
    if ( aControl == iLeftBtn && aEventType == EEventStateChanged )
        {        
        if ( iActiveCharPos != KErrNotFound )
            {
            // Edit an old preset
            // Find first available preset, otherwise, show error note all in use
            // Create a new preset shortcut but at least one is available
            // Find first available preset, otherwise, nothing to do
            TInt startPos = ( iActiveCharPos - 1 + iPresets.Count() ) 
                            % iPresets.Count();
            TInt endPos = ( iActiveCharPos + 1 + iPresets.Count() ) 
                            % iPresets.Count();       
            if ( FindFirstAvailableCharL( startPos, endPos, 0, iUnicode ) )
                {
                iActiveCharPos = iPresets.Find( iUnicode );
                SetActiveCharL( iUnicode ); 
                }
            else if ( !iEngine->NewShortcut() )
                {
                // if Create a new preset shortcut but at least one is available
                // nothing to do
                // show error note : all in use
                PopupTooltipL( R_TRUI_SHORTCUTEDITVIEW_ERROR_NOTE_ALL_IN_USE );
                }
            }
        else
            {
            // Create a new preset, but all in use
            // Show error note
            PopupTooltipL( R_TRUI_SHORTCUTEDITVIEW_ERROR_NOTE_ALL_IN_USE );
            }
        }
    else if ( aControl == iRightBtn && aEventType == EEventStateChanged )
        {
        if ( iActiveCharPos != KErrNotFound )
            {
            // Edit an old preset
            // Find first available preset, otherwise, show error note all in use
            // Create a new preset shortcut but at least one is available
            // Find first available preset, otherwise, nothing to do
            TInt startPos = ( iActiveCharPos + 1 + iPresets.Count() ) 
                            % iPresets.Count();
            TInt endPos = ( iActiveCharPos - 1 + iPresets.Count() ) 
                            % iPresets.Count();       
            if ( FindFirstAvailableCharL( startPos, endPos, 1, iUnicode ) )
                {
                iActiveCharPos = iPresets.Find( iUnicode );
                SetActiveCharL( iUnicode ); 
                }
            else if ( !iEngine->NewShortcut() )
                {
                // show error note : all in use
                PopupTooltipL( R_TRUI_SHORTCUTEDITVIEW_ERROR_NOTE_ALL_IN_USE );
                }
            }
        else
            {
            // Create a new preset, but all in use
            // Show error note
            PopupTooltipL( R_TRUI_SHORTCUTEDITVIEW_ERROR_NOTE_ALL_IN_USE );
            }
        }
    else if ( iShortcutType == KPresetIndex && aControl == iIndicator 
              && aEventType == EEventRequestFocus )
        {
        PreviewShortcutModel();
        }
    }

// ---------------------------------------------------------------------------
// From class MTruiHWRBoxObserver.
// Handle the save model event
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::SaveNewModelL()
    {
    // Carry out saving new model
    if ( iShortcutType == KOwnIndex )
        {
        iIsRestored = ETrue;
        
        // If shortcut is own, it will save new model
        TBuf<KSimilarMsgLength> similarMsg;
        TRAPD( err, SaveOwnShortcutL( iEngine->Shortcut(), 
                                      iHwrBox->Model(), 
                                      similarMsg ) );
        switch ( err )
            {
            case KErrNone:
                {
                iIsEmptyHwrBox = EFalse;
                CAknToolbar* toolbar = iAppUi->View( KTruiShortcutEditViewId )
                                             ->Toolbar();
                CAknButton* clearBtn = static_cast<CAknButton*>
                          ( toolbar->ControlOrNull( EShortcutEditViewDelete ) );
                clearBtn->SetDimmed( EFalse );
                clearBtn->DrawNow(); 
                CAknButton* previewBtn = static_cast<CAknButton*>
                          ( toolbar->ControlOrNull( EShortcutEditViewPreviewModel ) );
                previewBtn->SetDimmed( EFalse );
                previewBtn->DrawNow(); 

                TPoint hwrPos = iHwrBox->PositionRelativeToScreen();
                HBufC* tooltipText = 
                        iEikonEnv->AllocReadResourceLC( R_TRUI_TOOLTIP_SAVED );
                iHwrBox->ShowTooltipL( *tooltipText, 
                                  hwrPos + TPoint( iHwrBox->Size().iWidth, KHwrTooltipY ),
                                       KTooltipPopupTimeOut, 
                                       KTooltipStayTimeOut );
                CleanupStack::PopAndDestroy( tooltipText );
                break;
                }
            case KErrAlreadyExists:
                {
                //Clear invalid model
                ClearCurrentModelL();
                
                //Popup err message
                PopupTooltipL( R_TRUI_SHORTCUTEDITVIEW_ERROR_NOTE_IN_USE );
                break;
                }
                
            }
        }       
    }

// ---------------------------------------------------------------------------
// From class MTruiHWRBoxObserver.
// Handle the message event from the HWRBox
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::HandleEventL( TMessageType aMessage )
    {
    if ( aMessage == EModelExist )
        {
        PopupTooltipL( R_TRUI_NOTE_CLEAR_BOX );
        }
    }

// ---------------------------------------------------------------------------
// From class MTruiHWRBoxObserver.
// Notify that which radio button is selected.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::SelectChangedL( TInt aIndex )
    {
    // Change the text below radio buttons
    TInt tipResource = aIndex ? R_TRUI_SHORTCUTEDITVIEW_PRESET_SYMBOL
                              : R_TRUI_SHORTCUTEDITVIEW_OWN_SYMBOL;    
    delete iLabeltext;
    iLabeltext = NULL;
    iLabeltext = iEikonEnv->AllocReadResourceL( tipResource );
    delete iWrappedLabelText;
    iWrappedLabelText = NULL;
    // count of text rows
    TInt lines = AknLayoutScalable_Apps::
                 list_single_hwr_training_instruct_pane_ParamLimits( KVarityListPane )
                 .LastRow() + 1;         
    iWrappedLabelText = HBufC::NewL( iLabeltext->Length() + lines
                                     + KExtraSize );
    // Be ready to show label
    const CFont* font = AknLayoutUtils::FontFromId( iMultilineLayout.FontId() );
    TRAPD( err, WrapLabelTextL( font, *iLabeltext, iLineWidthArray, iWrappedLabelText ) );
    if ( err == KErrNone )
        {
        iLabelTip->SetTextL( *iWrappedLabelText );
        }
    else
        {
        iLabelTip->SetTextL( *iLabeltext );
        }
    // Get Clear button from toolbar
    CAknToolbar* toolbar = iAppUi->View( KTruiShortcutEditViewId )->Toolbar();
    CAknButton* clearBtn = static_cast<CAknButton*>
                ( toolbar->ControlOrNull( EShortcutEditViewDelete ) );
    CAknButton* gridBtn = static_cast<CAknButton*>
                ( toolbar->ControlOrNull( EShortcutEditViewGrid ) );
    // Dim left and right buttons when selecting "own symbol"
    TBool btnDimmed = aIndex ? EFalse : ETrue;
    iLeftBtn->SetDimmed( btnDimmed );
    iLeftBtn->DrawNow();
    iRightBtn->SetDimmed( btnDimmed );
    iRightBtn->DrawNow();

    // Set grid button status
    gridBtn->SetDimmed( btnDimmed );
    gridBtn->DrawNow();

    // Set active indicator
    TBuf<KIndicatorBufSize> preset;
    if ( aIndex && iUnicode )
        {
        // Preset symbol
        preset.Format( KActiveCharFormat, iUnicode );
        }
    else
        {
        // Own
        preset.Copy( KNullDesC );
        }
    iIndicator->SetTextL( preset );        
    iIndicator->DrawNow();    

    // Update current shortcut type
    iShortcutType = aIndex;   

    // Clear hwrbox and stop playing animation    
    iHwrBox->ClearExistModel();
    
    if ( iShortcutType == KPresetIndex )
        {
        // If preset shortcut, get trail points.
        TBuf<KSimilarMsgLength> similarMsg;
        iEngine->GetPresetSymbolByUnicodeL( iUnicode, 
                                            iHwrBox->Model(), similarMsg );
        iHwrBox->ModelIsReady( ETrue );
        iHwrBox->PlayAnimation( ETrue, CTruiContainerBase::OriginSymbolSize() );
        }
    else if ( !iIsEmptyHwrBox )
        {
        // If own shortcut, get trail points
        TUint unicode = 0;
        iEngine->GetShortcutModelL( iEngine->Shortcut(), iHwrBox->Model(),
                                    unicode );
        iHwrBox->ModelIsReady( ETrue );
        iHwrBox->PlayAnimation();
        }

    // Set hwrbox model
    if ( iShortcutType == KPresetIndex )
        {        
        iHwrBox->SetHwrBoxReadOnly( ETrue );        
        }
    else
        {
        iHwrBox->SetHwrBoxReadOnly( EFalse );
        }
    
    // Set clear button's status
    clearBtn->SetDimmed( !( iHwrBox->Model().Count() ) | aIndex );
    clearBtn->DrawNow();

    // Set preview button status
    CAknButton* previewBtn = static_cast<CAknButton*>
              ( toolbar->ControlOrNull( EShortcutEditViewPreviewModel ) );

    previewBtn->SetDimmed( !( iHwrBox->Model().Count() | aIndex ) );
    previewBtn->DrawNow();
    }
    
// ---------------------------------------------------------------------------
// Preview shortcut model
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::PreviewShortcutModel()
    {
    //even hwrbox know if current character has existing model.
    //ask for engine to confirm this
    iHwrBox->PlayAnimation(); 
    }

// ---------------------------------------------------------------------------
// Check if model exist
// ---------------------------------------------------------------------------
//
TBool CTruiShortcutEditContainer::HasShortcutModel()
    {    
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// Prepare to exit
// ---------------------------------------------------------------------------
//
TBool CTruiShortcutEditContainer::PrepareToExitL()
    {
    iHwrBox->HideInfoPopupNote();
    //If previous char's model is not saved, save it before exit.
    if ( iHwrBox->BoxState() == CTruiHwrBox::ENewModelDraw )
        {
        //Call save function from hwrbox,
        //in order to stop periodics in the hwrbox
        iHwrBox->SaveModelByManuallyL();
        }
        
    //After saving model, if shortcut model is empty,
    //notice user that shortcut will be delete.
    if ( iHwrBox->Model().Count() == 0 )
        {
        // Not assign model to shortcut
        CAknQueryDialog* dlg = CAknQueryDialog::NewL();
        if ( dlg->ExecuteLD( R_TRUI_QUERYDIALOG_SHORTCUT_NOT_ASSIGNED ) )
            {
            // yes/ok pressed
            iEngine->DeleteShortcut( iEngine->Shortcut() );
            return ETrue;
            }
        else
            {
            return EFalse;
            }
        }
    else
        {
        if ( iShortcutType == KPresetIndex )
            {
            // Save preset
            // No need to save own preset again, because it has been saved before.
            TRAPD( err, SavePresetShortcutL( iEngine->Shortcut(), iUnicode ) );
            if ( err == KErrInUse )
                {
                // Pop information note
                HBufC* errMsg = iEikonEnv->AllocReadResourceLC
                              ( R_TRUI_SHORTCUTEDITVIEW_ERROR_NOTE_IN_USE );
                CAknConfirmationNote* note = new (ELeave) CAknConfirmationNote 
                                                                    ( ETrue );
                note->ExecuteLD( *errMsg );                
                CleanupStack::PopAndDestroy( errMsg );
                return EFalse;
                }                
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// Clear current model
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::ClearCurrentModelL()
    {
    // Get Clear button from toolbar
    CAknToolbar* toolbar = iAppUi->View( KTruiShortcutEditViewId )->Toolbar();    
    CAknButton* clearBtn = static_cast<CAknButton*>
                ( toolbar->ControlOrNull( EShortcutEditViewDelete ) );    
    clearBtn->SetDimmed( ETrue );
    clearBtn->DrawNow();
    
    // Set preview button status
    CAknButton* previewBtn = static_cast<CAknButton*>
              ( toolbar->ControlOrNull( EShortcutEditViewPreviewModel ) );

    previewBtn->SetDimmed( ETrue );
    previewBtn->DrawNow();

    iHwrBox->ClearExistModel();
    HBufC* tipText = iEikonEnv->AllocReadResourceLC( R_TRUI_TOOLTIP_DELETED );
    TPoint hwrPos = iHwrBox->PositionRelativeToScreen();
    iHwrBox->ShowTooltipL( *tipText, 
                           hwrPos + TPoint( iHwrBox->Size().iWidth, KHwrTooltipY ),
                           KTooltipPopupTimeOut, KTooltipStayTimeOut );
    CleanupStack::PopAndDestroy( tipText ); 
    iIsEmptyHwrBox = ETrue;
    }
    
// ---------------------------------------------------------------------------
// Popup tooltip
// ---------------------------------------------------------------------------
//    
void CTruiShortcutEditContainer::PopupTooltipL( TInt aResourceID )
    {
    HBufC* tooltiptext =iEikonEnv->AllocReadResourceLC( aResourceID );
    CAknInformationNote* note = new (ELeave) CAknInformationNote( ETrue );
    note->ExecuteLD( *tooltiptext );
    CleanupStack::PopAndDestroy( tooltiptext );
    }

// ---------------------------------------------------------------------------
// This is called whenever the control gains or loses focus, 
// ---------------------------------------------------------------------------
// 
void CTruiShortcutEditContainer::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    if ( IsFocused() )
        {
        if ( iHwrBox->BoxState() == CTruiHwrBox::EExistModelStop )
            {
            iHwrBox->ResumePlaying();
            }
        }
    else
        {
        if ( iHwrBox->BoxState() == CTruiHwrBox::EExistModelDraw )
            {
            iHwrBox->PausePlaying();
            }
        }
    iHwrBox->SetFocus( IsFocused() );
    }

// ---------------------------------------------------------------------------
// Return the currently selected index, which will be set in Shortcuts view.
// ---------------------------------------------------------------------------
//
TInt CTruiShortcutEditContainer::DefaultSelectedIndex( const CDesCArray* aItemList,
                                                       const TDesC& aText )
    {    
    if ( aItemList )
        {
        for ( TInt i = 0; i < aItemList->Count(); i++ )
            {        
            if ( (*aItemList)[i].Compare( aText ) == 0 )
                {
                return i + 1;
                }
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// Popup SCT dialog.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::PopupSctDialogL()
    {
    TBuf<KSctBufferSize> selectedStr;
    CAknCharMapDialog* sct = new (ELeave) CAknCharMapDialog( EAknSCTLowerCase, 
                                           selectedStr, 
                                           R_TRUI_SCT_CHARSET_PRESET );    
    TInt keyboardMode = CurrentKeyBoardModeL(); 
    if ( keyboardMode )
        {
        // QWERTY: Lock numeric keys.
        sct->LockNumericKeys( ETrue );
        }
    else
        {
        // ITU-T: Unlock numeric keys.
        sct->LockNumericKeys( EFalse );
        }
    sct->DisableRecentCharsRow();
    if ( sct->ExecuteLD( R_TRUI_SCT_DIALOG ) )
        {        
        TInt selectedUnicode = selectedStr.Length() ? selectedStr[0] : NULL;
        TBool isInUse = CheckPresetSymbolInUseL( selectedUnicode );
        if ( isInUse && selectedUnicode != iUnicodeBak )
            {
            // Preset is in use
            //Popup err message
            PopupTooltipL( R_TRUI_SHORTCUTEDITVIEW_ERROR_NOTE_IN_USE );
            return;
            }
        else
            {
            TInt activePos = iPresets.Find( selectedUnicode );
            
            // Set active char pos
            iActiveCharPos = ( activePos != KErrNotFound ) ? activePos : 0;
            iUnicode = iPresets[iActiveCharPos];
            SetActiveCharL( selectedUnicode );            
            }        
        }
    }

// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles a change to the control's resources.
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditContainer::HandleResourceChange( TInt aType )
    {
    CTruiContainerBase::HandleResourceChange( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch && iHwrBox )
        {
        if ( iHwrBox->BoxState() == CTruiHwrBox::ENewModelDraw )
            {
            TRAP_IGNORE( iHwrBox->SaveModelByManuallyL() );
            }        
        }    

    if ( aType == KAknsMessageSkinChange )
        {
        TRgb labelColor = KRgbBlack;
        TInt error = GetCachedLabelTextColor( labelColor );
        if ( error == KErrNone )
            {
            TRAPD( errLable, iLabelTip->OverrideColorL( EColorLabelText, 
                                                        labelColor ) );
            if ( errLable != KErrNone )
                {
                return;
                }
            TRAP_IGNORE( iIndicator->OverrideColorL( EColorLabelText, 
                                                     labelColor ) );
            }        
        }         
    }
