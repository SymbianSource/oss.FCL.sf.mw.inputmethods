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
* Description:  Implement of class CTruiTextSettingPageContainer
*
*/


#include <aknlayoutscalable_apps.cdl.h>
#include <AknsControlContext.h>
#include <AknPreviewPopUpController.h>
#include <AknPreviewPopUpObserver.h>
#include <layoutmetadata.cdl.h>

#include "truitextsettingpagecontainer.h"
#include "truihwrbox.h"
#include "truiappui.h"
#include "truiengine.h"
#include "truishortcutsview.h"
#include "truiapplication.h"
#include "truiinfomessageview.h"
#include "truishortcuteditview.h"
#include "truishortcutedittextview.h"
#include "trui.hrh"
#include "truicontainerbase.h"

// Initialized size for hwrbox
const TSize KHwrboxSize = TSize( 100, 100 );

// The x-cordination of hwrbox pos
const TInt KHwrBoxPosPortraitX = 60;
const TInt KHwrBoxPosLandscapeX = 100;

const TInt KPopupWindowVarity = 1;

const TInt KParaDelimiterSize = 1;

_LIT( KParaDelimiterFormat, "%c" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
CTruiTextSettingPageContainer::CTruiTextSettingPageContainer
                ( TInt aResourceID, TDes& aText, TInt aTextSettingPageFlags )
             :CAknTextSettingPage( aResourceID, aText, aTextSettingPageFlags )
    {  
    }
    
// ---------------------------------------------------------------------------
// Perform the second phase construction of a CTruiMainViewContainer object.
// ---------------------------------------------------------------------------
//
void CTruiTextSettingPageContainer::ConstructL( const TRect& aRect, TBool aEnableHwrBox )
    {
    CAknTextSettingPage::ConstructL();
    TextControl()->SetObserver( this );
    // Check character model 
    CEikEdwin* edwin = static_cast<CEikEdwin*>( EditorControl() );
    HBufC* shortcutText = edwin->GetTextInHBufL();    
    if ( shortcutText )
        {        
        // Replace paragraph seperator to line break
        TPtr ptr = shortcutText->Des();
        TBuf<KParaDelimiterSize> paraDelimiter;
        paraDelimiter.Format( KParaDelimiterFormat, EKeyEnter ); 
        AknTextUtils::ReplaceCharacters( ptr, paraDelimiter, 
                                TChar( CEditableText::EParagraphDelimiter ) );
        CleanupStack::PushL( shortcutText );
        edwin->SetTextL( shortcutText );
        CleanupStack::PopAndDestroy( shortcutText );
        }    
        
    if ( aEnableHwrBox )
        {
        CTruiAppUi* appUi = static_cast<CTruiAppUi*>( iAvkonViewAppUi );
        CTruiEngine* engine = appUi->HwrEngine();
        if ( engine->CheckShortcutModel( engine->Shortcut() ) )
            {
            // Create hwrbox            
            iHwrBox = CTruiHwrBox::NewL( NULL, NULL );
            iHwrBox->SetHwrBoxFlag( CTruiHwrBox::ENoFrameHwrBox 
                                    | CTruiHwrBox::EReadOnlyHwrBox);
            
            // Create preview popup
            iPopupController = CAknPreviewPopUpController::NewL
                      ( *iHwrBox, CAknPreviewPopUpController::EPermanentMode );            
            iPopupController->AddObserverL( *this );
            // Set guide line
            iHwrBox->SetDisplayBottomGuideLine( ETrue );
            if ( engine->CurrentLanguageScript() == EMainViewSubmenuHebrew )
                {
                iHwrBox->SetDisplayTopGuideLine( ETrue );
                }
            iPopupController->SetPopUpShowDelay( 0 );            
            }                          
        }
    SetRect( aRect );
    if ( iPopupController )
        {
        iPopupController->ShowPopUp();
        }    
    ActivateL();
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CTruiTextSettingPageContainer* CTruiTextSettingPageContainer::NewL
                                                ( TInt aResourceId, 
                                                  TDes& aText,
                                                  const TRect& aRect,
                                                  TBool aEnableHwrBox,
                                                  TInt aTextSettingPageFlags )
    {
    CTruiTextSettingPageContainer* self = CTruiTextSettingPageContainer::NewLC
                                                  ( aResourceId, aText,
                                                    aRect,
                                                    aEnableHwrBox,
                                                    aTextSettingPageFlags );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CTruiTextSettingPageContainer* CTruiTextSettingPageContainer::NewLC
                                                ( TInt aResourceId, 
                                                  TDes& aText, 
                                                  const TRect& aRect,
                                                  TBool aEnableHwrBox,
                                                  TInt aTextSettingPageFlags )
    {
    CTruiTextSettingPageContainer* self = new (ELeave) 
                                          CTruiTextSettingPageContainer
                                                 ( aResourceId, aText,
                                                   aTextSettingPageFlags );
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aEnableHwrBox );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//

CTruiTextSettingPageContainer::~CTruiTextSettingPageContainer()
    {
    delete iPopupController;
    delete iHwrBox;
    }
    
// ---------------------------------------------------------------------------
// Responds to changes to the size and position of the contents of this control.
// ---------------------------------------------------------------------------
//
void CTruiTextSettingPageContainer::SizeChanged()
    {
    CAknTextSettingPage::SizeChanged();
    
    if ( iHwrBox )
        {
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
                
        TPoint labelPos = TextLabel()->PositionRelativeToScreen();
        TPoint hwrPos = TPoint( 0, 0 );
        if ( TBidiText::ScriptDirectionality( User::Language() )
                                              ==  TBidiText::ELeftToRight )
            {
            TInt offset = Layout_Meta_Data::IsLandscapeOrientation() ?
                          KHwrBoxPosLandscapeX : KHwrBoxPosPortraitX;
            hwrPos = labelPos + TPoint( offset, TextLabel()->Size().iHeight );
            }
        else
            {
            hwrPos = labelPos + TPoint( TextLabel()->Size().iWidth, 
                            		TextLabel()->Size().iHeight );
            }

        iHwrBox->SetSize( preview_popup_layout_rect.Rect().Size() );
        iPopupController->SetPosition( hwrPos );
        }
    }
    
// ---------------------------------------------------------------------------
// Processes events from the softkeys.     
// ---------------------------------------------------------------------------
//
void CTruiTextSettingPageContainer::ProcessCommandL( TInt aCommandId )
    {
    HideMenu();    
    switch ( aCommandId )
        {
        case EAknSoftkeyOk:
        case EAknSoftkeySelect:
            {
            // Get the current text in setting page
            CTruiAppUi* appUi = static_cast<CTruiAppUi*>( iAvkonViewAppUi );
            CTruiEngine* engine = appUi->HwrEngine();
            CEikEdwin* edwin = static_cast<CEikEdwin*>( EditorControl() );
            HBufC* shortcutText = edwin->GetTextInHBufL();
            CleanupStack::PushL( shortcutText );
            // Replace paragraph seperator to line break            
            TPtr ptr = shortcutText->Des();
            TBuf<KParaDelimiterSize> paraDelimiter;
            paraDelimiter.Format( KParaDelimiterFormat, CEditableText::EParagraphDelimiter );
            AknTextUtils::ReplaceCharacters( ptr, paraDelimiter, TChar( EKeyEnter ) );
            // Update new text
            engine->ChangeShortcutTextL( *shortcutText );
            // Recalculate selected index of shortcut list
            // Update shortcut list
            engine->UpdateShortcutListL();
            // Find the current shortcut's index in shortcut list
            TInt index = 0;
            CDesCArray* shortcutList = engine->ShortcutTextList();
            if ( shortcutList )
                {                
                TInt ret = 0;
                ret = shortcutList->Find( *shortcutText, index );
                if ( ret == KErrNotFound )
                    {
                    TInt oldIndex = engine->CurrentSelectedIndex();
                    index = ( oldIndex > shortcutList->Count() ) 
                                       ? shortcutList->Count() : oldIndex;
                    }
                }
            // Set current index in Engine
            engine->SetCurrentSelectedIndex( index + 1 );           
            CleanupStack::PopAndDestroy( shortcutText );            
            // Store the content of setting page
            UpdateSettingL();
            CTruiShortcutEditTextView* view = 
                                     static_cast<CTruiShortcutEditTextView*>
                              ( appUi->View( KTruiShortcutEditTextViewId ) );
            if ( view->StartViewId() == 
                        TVwsViewId( KUidtruiApp, KTruiInfoMessageViewId ) )
                {
                ProcessCommandL( EAknSoftkeyNext );
                }
            else
                {
                // Switch to shortcuts view.
                iAvkonViewAppUi->ActivateLocalViewL
                                            ( view->StartViewId().iViewUid );
                }
            break;
            }
        case EAknSoftkeyCancel:
            {
            iAvkonViewAppUi->ActivateLocalViewL( KTruiShortcutsViewId );
            break;
            }
        case EAknSoftkeyNext:
            {
            UpdateSettingL();
            // Get the current text in setting page
            CTruiAppUi* appUi = static_cast<CTruiAppUi*>( iAvkonViewAppUi );
            CTruiEngine* engine = appUi->HwrEngine();
            CEikEdwin* edwin = static_cast<CEikEdwin*>( EditorControl() );
            HBufC* shortcutText = edwin->GetTextInHBufL();
            CleanupStack::PushL( shortcutText );
            // Replace paragraph seperator to line break            
            TPtr ptr = shortcutText->Des();
            TBuf<KParaDelimiterSize> paraDelimiter;
            paraDelimiter.Format( KParaDelimiterFormat, CEditableText::EParagraphDelimiter );
            AknTextUtils::ReplaceCharacters( ptr, paraDelimiter, TChar( EKeyEnter ) );
            engine->SetShortcut( ptr );            
            CleanupStack::PopAndDestroy( shortcutText );                        
            iAvkonViewAppUi->ActivateLocalViewL( KTruiShortcutEditViewId );
            break;
            }
        case EAknSoftkeyBack:
            {
            CTruiAppUi* appUi = static_cast<CTruiAppUi*>( iAvkonViewAppUi );
            CTruiEngine* engine = appUi->HwrEngine();            
            if ( engine->DisplayWizard() )
                {
                iAvkonViewAppUi->ActivateLocalViewL( KTruiInfoMessageViewId );
                }
            else
                {
                iAvkonViewAppUi->ActivateLocalViewL( KTruiShortcutsViewId );
                }
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// From MAknPreviewPopUpObserver
// Called by the preview popup when an appropriate event takes place.
// ---------------------------------------------------------------------------
// 
void CTruiTextSettingPageContainer::HandlePreviewPopUpEventL(
      CAknPreviewPopUpController* aController, TPreviewPopUpEvent aEvent )
    {
    if ( aController == iPopupController && aEvent == EPreviewPopUpShown )
        {
        CTruiAppUi* appUi = static_cast<CTruiAppUi*>( iAvkonViewAppUi );
        CTruiEngine* engine = appUi->HwrEngine();
        CEikEdwin* edwin = TextControl();
        HBufC* shortcutText = edwin->GetTextInHBufL();
        CleanupStack::PushL( shortcutText );
        // Get character model
        TUint unicode = 0;
        TRAPD( err, engine->GetShortcutModelL( engine->Shortcut(), iHwrBox->Model(), unicode ) );
        if ( err == KErrNone )
            {
            TSize refSize = CTruiContainerBase::OriginSymbolSize();
            iHwrBox->ShowTrails( iHwrBox->Model(), ETrue, refSize );
            }    
        CleanupStack::PopAndDestroy( shortcutText );
        }
    }
