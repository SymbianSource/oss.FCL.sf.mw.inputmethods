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
* Description:  Implement of class CTruiInfoMessageContainer
*
*/


#include <eikenv.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <trui.rsg>
#include <eiklabel.h>
#include <eikon.hrh>
#include <AknUtils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <eikedwin.h>

#include "truiinfomessagecontainer.h"
#include "truiappradiobutton.h"
#include "truiappui.h"
#include "truiengine.h"
#include "truicontainerbase.h"

// Varity of instruct pane both for landscape and portrait
const TInt KVarityInstructPane = 4;

// Varity of list pane both for landscape and portrait
const TInt KVarityListPane = 2;

// Varity of symbol option pane both for landscape and portrait
const TInt KVaritySymbolOptionPane = 2;

// Varity of list symbol option pane both for landscape and portrait
const TInt KVarityListSymbolOptionPane = 1;


// ======== MEMBER FUNCTIONS ========

CTruiInfoMessageContainer::CTruiInfoMessageContainer()
    {  
    }
    
void CTruiInfoMessageContainer::ConstructL( const TRect& aRect )
    {    
    // Initialize control array
    InitComponentArrayL();
        
    // Set parent control or owning window
    CreateWindowL();       

    // Initialize controls
    InitializeControlsL();

    iLabelText = iEikonEnv->AllocReadResourceL( R_TRUI_INFOMAESSAGEVIEW_MESSAGE );
    iEdwin->SetTextL( iLabelText );    

    SetRect( aRect );
    ActivateL();   
    
    iBackGround = CAknsBasicBackgroundControlContext::NewL
                  ( KAknsIIDQsnBgAreaMain, Rect(), EFalse);
    iEdwin->SetSkinBackgroundControlContextL( iBackGround );
    }        
    
CTruiInfoMessageContainer* CTruiInfoMessageContainer::NewL( const TRect& aRect )
    {
    CTruiInfoMessageContainer* self = CTruiInfoMessageContainer::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }
    
CTruiInfoMessageContainer* CTruiInfoMessageContainer::NewLC( const TRect& aRect )
    {
    CTruiInfoMessageContainer* self = new (ELeave) CTruiInfoMessageContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }
    
CTruiInfoMessageContainer::~CTruiInfoMessageContainer()
    {
    delete iBackGround;        
    delete iLabelText;
    }

// ---------------------------------------------------------------------------
// Do something before exit.
// ---------------------------------------------------------------------------
// 
void CTruiInfoMessageContainer::PrepareToExitL()
    {
    // Save checkbox setting
    const RArray<CTruiRadioButton::TRadioButtonStatus>& statusArray = 
                                          iRadioButtonContainer->StatusArray();
    TBool isDisplayed = statusArray.Count() && statusArray[0] ? EFalse : ETrue;
    iEngine->SetDisplayWizard( isDisplayed );
    if ( statusArray.Count() )
        {
        SaveSettingIntoIniFileL( KAlwaysDisplayWizardKey, statusArray[0] );
        }    
    }
    
// ---------------------------------------------------------------------------
// Create controls to be displayed in this container.
// ---------------------------------------------------------------------------
//
void CTruiInfoMessageContainer::InitializeControlsL()
    {
    iRadioButtonContainer = CTruiRadioButton::NewL( this, 
                                               CTruiRadioButton:: ECheckBox );
    Components().AppendLC( iRadioButtonContainer );
    CleanupStack::Pop( iRadioButtonContainer );
    
    HBufC* radioText1 = iEikonEnv->AllocReadResourceLC
                             ( R_TRUI_INFOMAESSAGEVIEW_CHECKBOX_TEXT );
    iRadioButtonContainer->AddItem( radioText1, CTruiRadioButton::ENonSelected );
    CleanupStack::Pop( radioText1 ); // transfered ownership
    iRadioButtonContainer->HandleItemAdditionL();
    
    iEdwin = new (ELeave) CEikEdwin;
    iEdwin->ConstructL();
    iEdwin->SetReadOnly( ETrue );     
    Components().AppendLC( iEdwin );
    CleanupStack::Pop( iEdwin );
    
    }

// ---------------------------------------------------------------------------
// Create label and display it in the container.
// ---------------------------------------------------------------------------
//
CEikLabel* CTruiInfoMessageContainer::CreateLabelL( const CCoeControl* aParent )
    {
    CEikLabel* aknLabel = new (ELeave) CEikLabel();
    CleanupStack::PushL( aknLabel );
    aknLabel->SetContainerWindowL( *aParent );
    aknLabel->ActivateL();
    CleanupStack::Pop( aknLabel ); 
    return aknLabel;
    }


// ---------------------------------------------------------------------------
// From class CCoeControl.
// Responds to changes to the size and position of the contents of this control.
// ---------------------------------------------------------------------------
//
void CTruiInfoMessageContainer::SizeChanged()
    {
    TRect rect = Rect();

    TAknWindowComponentLayout main_pane_layout = 
                            AknLayoutScalable_Apps::main_hwr_training_pane();
    TAknLayoutRect main_pane_layout_rect;
    main_pane_layout_rect.LayoutRect( rect, main_pane_layout );
    TRect main_pane_rect = main_pane_layout_rect.Rect();

    // hwr_training_instruct_pane    
    TAknWindowComponentLayout instruct_pane_layout = 
               AknLayoutScalable_Apps::hwr_training_instruct_pane( KVarityInstructPane );    
    TAknLayoutRect instruct_pane_layout_rect;
    instruct_pane_layout_rect.LayoutRect( main_pane_rect, instruct_pane_layout );
    iInstructPaneRect = instruct_pane_layout_rect.Rect();
    
    // count of text rows    
    TInt count = AknLayoutScalable_Apps::
               list_single_hwr_training_instruct_pane_ParamLimits( KVarityListPane )
               .LastRow() + 1;
    
    // array contains all text rows in help screen
    RArray<TAknTextComponentLayout> textLayoutArray;
    for( TInt i = 0; i < count; i++ )
        {       
        // pane area for each row
        TAknWindowComponentLayout each_row_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_instruct_pane
                                    ( KVarityListPane, 0, i );
        TAknLayoutRect each_row_layout_rect;
        each_row_layout_rect.LayoutRect( iInstructPaneRect, 
                                         each_row_layout );
        // smaller pane for text itself
        TAknTextComponentLayout each_row_text_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_instruct_pane_t1();
    
        // composition of two latter componets
        TAknTextComponentLayout textCompCompositionWiz = 
            TAknWindowComponentLayout::ComposeText(
                each_row_layout, each_row_text_layout );
 
        TRAPD( error, textLayoutArray.AppendL( textCompCompositionWiz ) );
        // if error in append function, then just exit the loop      
        if ( error != KErrNone )
            {
            break;
            }
        }       
    iMultilineLayout = TAknTextComponentLayout::Multiline( textLayoutArray );
    textLayoutArray.Close();

    // Do layout for edwin
    AknLayoutUtils::LayoutEdwin( iEdwin, 
                                 iInstructPaneRect, 
                                 iMultilineLayout,
                                 EAknsCIQsnTextColorsCG6 ); 

	// Do layout for radio button
    // hwr_training_instruct_pane
    TAknWindowComponentLayout symbol_option_pane = 
               AknLayoutScalable_Apps::main_hwr_training_symbol_option_pane
                                      ( KVaritySymbolOptionPane );
    TAknLayoutRect symbol_option_pane_layout_rect;
    symbol_option_pane_layout_rect.LayoutRect( main_pane_rect, symbol_option_pane );
    
	TAknWindowComponentLayout list_option_pane = 
               AknLayoutScalable_Apps::list_single_hwr_training_symbol_option_pane
                                      ( KVarityListSymbolOptionPane );
    TAknLayoutRect list_option_pane_layout_rect;
    list_option_pane_layout_rect.LayoutRect( symbol_option_pane_layout_rect.Rect(), 
                                             list_option_pane );
    // Create multiline text for radion button
    // count of text rows    
    TInt radionLines = AknLayoutScalable_Apps::
               list_single_hwr_training_symbol_option_pane_ParamLimits
               ( KVarityListSymbolOptionPane )
               .LastRow() + 1;
    
    // array contains all text rows in help screen
    RArray<TAknTextComponentLayout> radionTextLayoutArray;
    for( TInt i = 0; i < radionLines; i++ )
        {       
        // pane area for each row
        TAknWindowComponentLayout each_row_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_symbol_option_pane
                                    ( KVarityListSymbolOptionPane, 0, i );
        TAknLayoutRect each_row_layout_rect;
        each_row_layout_rect.LayoutRect( symbol_option_pane_layout_rect.Rect(), 
                                         each_row_layout );
        // smaller pane for text itself
        TAknTextComponentLayout each_row_text_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_symbol_option_pane_t1();
    
        // composition of two latter componets
        TAknTextComponentLayout textCompCompositionWiz = 
            TAknWindowComponentLayout::ComposeText(
                each_row_layout, each_row_text_layout );
 
        TRAPD( error, radionTextLayoutArray.AppendL( textCompCompositionWiz ) );
        // if error in append function, then just exit the loop      
        if ( error != KErrNone )
            {
            break;
            }
        }       
    TAknMultiLineTextLayout radioMultilineLayout 
                            = TAknTextComponentLayout::Multiline( radionTextLayoutArray );
    radionTextLayoutArray.Close();  
    iRadioButtonContainer->SetMultilineLayout( radioMultilineLayout );
    iRadioButtonContainer->SetRect( symbol_option_pane_layout_rect.Rect() );
    
    if ( iBackGround )
        {
        iBackGround->SetRect( Rect() );
        }
    }
    
// ---------------------------------------------------------------------------
// Draw.
// ---------------------------------------------------------------------------
//    
void CTruiInfoMessageContainer::Draw( const TRect& /*aRect*/ ) const
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
// From class CCoeControl.
// Handles a change to the control's resources.
// ---------------------------------------------------------------------------
//
void CTruiInfoMessageContainer::HandleResourceChange( TInt aType )
    {    
    CTruiContainerBase::HandleResourceChange( aType );
    if ( aType == KAknsMessageSkinChange )
        {
        if ( iEdwin )
            {
            // Do layout for edwin
            AknLayoutUtils::LayoutEdwin( iEdwin, 
                                         iInstructPaneRect, 
                                         iMultilineLayout,
                                         EAknsCIQsnTextColorsCG6 );
            }
        }
    }
