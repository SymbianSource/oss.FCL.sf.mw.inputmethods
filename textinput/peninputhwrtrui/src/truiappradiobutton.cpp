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
* Description:  Implement of class CTruiRadioButton
*
*/


#include <eiklabel.h>
#include <barsread.h>
#include <eikenv.h>
#include <avkon.rsg>
#include <AknIconUtils.h>
#include <AknsSkinInstance.h>
#include <AknsDrawUtils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknUtils.h>
#include <AknBidiTextUtils.h> 
#include <bidivisual.h>
#include <featmgr.h>

#ifdef RD_TACTILE_FEEDBACK 
#include <touchfeedback.h>  
#endif //RD_TACTILE_FEEDBACK

#include "truiappradiobutton.h"
#include "truiradiobuttonobserver.h"
#include "truicontainerbase.h"

// The amount of icons of bitmap files for ratio button
const TInt KRadioButtonIconCounts = 2;

const TInt KSpaceBetweenItems = 10;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CTruiRadioButton* CTruiRadioButton::NewL( CCoeControl* aParent, 
                              TSelectionType aSelectionType,
                              MTruiRadioButtonObserver* aObserver )
    {
    CTruiRadioButton* self = CTruiRadioButton::NewLC( aParent, aSelectionType, 
                                                      aObserver );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CTruiRadioButton* CTruiRadioButton::NewLC( CCoeControl* aParent, 
                              TSelectionType aSelectionType,
                              MTruiRadioButtonObserver* aObserver )
    {
    CTruiRadioButton* self = new ( ELeave ) CTruiRadioButton( aObserver, 
                                                              aSelectionType );
    CleanupStack::PushL( self );
    self->ConstructL( aParent );
    return self;
    }

// -----------------------------------------------------------------------------
// CTruiHwrBox::CTruiHwrBox()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTruiRadioButton::CTruiRadioButton( MTruiRadioButtonObserver* aObserver,
                                    TSelectionType aSelectionType )
                 :iObserver( aObserver ), iSelectionType( aSelectionType )
    {
    // No implementation required
    }

    
// -----------------------------------------------------------------------------
// CTruiRadioButton::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::ConstructL( CCoeControl* aParent )
    {
    SetContainerWindowL( *aParent );
    CreateBitmapForRadioButtonL();    
    #ifdef RD_TACTILE_FEEDBACK  
        iTactileSupported = FeatureManager::FeatureSupported( KFeatureIdTactileFeedback ); 
    #endif // RD_TACTILE_FEEDBACK    
    }
    
// -----------------------------------------------------------------------------
// Convert texts into Label and update them in radio buttons.
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::HandleItemAdditionL()
    {
    // Re-init control array
    delete iControlArray;
    iControlArray = NULL;
    if ( iTextArray.Count() > 0 )
        {        
        iControlArray = new (ELeave) CArrayPtrFlat<CCoeControl>
                                                    ( iTextArray.Count() );
        }
    // text layout
    // Add items for radio button
    for ( TInt i = 0; i < iTextArray.Count(); i++ )
        {
        // Create label
        CEikLabel* label = new (ELeave) CEikLabel();
        CleanupStack::PushL( label );
        label->SetContainerWindowL( *this );
        label->SetObserver( this );
        label->SetTextL( *iTextArray[i] );

        // Add label to control array
        iControlArray->AppendL( label );        
        CleanupStack::Pop( label );
        }        
    }
    
// -----------------------------------------------------------------------------
// Add one item to radio button.
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::AddItem( const HBufC* aText, TRadioButtonStatus aStatus )
    {
    iTextArray.Append( aText );
    iStatusArray.Append( aStatus );
    }

// -----------------------------------------------------------------------------
// CTruiRadioButton::~CTruiRadioButton()
// Destructor.
// -----------------------------------------------------------------------------
//
CTruiRadioButton::~CTruiRadioButton()
    {
    iStatusArray.Close();
    iTextArray.ResetAndDestroy();
    if ( iControlArray )
        {
        iControlArray->ResetAndDestroy();
        delete iControlArray;
        }        
    delete iRadioButtonSelectedBmp;
    delete iRadioButtonSelectedBmpm;
    delete iRadioButtonNonSelectedBmp;
    delete iRadioButtonNonSelectedBmpm;
    iIconLayout.Close();
    }
    
// ---------------------------------------------------------------------------
// Handles an event from an observed control.
// ---------------------------------------------------------------------------
//
void CTruiRadioButton::HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType )
    {
    TInt index;
    for ( index = 0; index < iControlArray->Count(); index++ )
        {
        if ( aControl == iControlArray->At( index ) )
            {
            break;
            }                    
        }
    if ( iSelectionType == ERadioButton )
        {
        HandlePointerRadioButtonL( index, aEventType );
        }
    else
        {
        HandlePointerCheckBox( index, aEventType );
        }    
    DrawNow();
    }

// ---------------------------------------------------------------------------
// Handle pointer on radio button.
// ---------------------------------------------------------------------------
//
void CTruiRadioButton::HandlePointerRadioButtonL( TInt aIndex, TCoeEvent aEventType )
    {
    if ( aIndex < iStatusArray.Count() && aEventType == EEventRequestFocus )
        {
        for ( TInt i = 0; i < iStatusArray.Count(); i++ )
            {
            if ( i == aIndex )
                {
                if ( iStatusArray[aIndex] == ENonSelected )
                    {
                    iStatusArray[aIndex] = ESelected;
                    if ( iObserver )
                        {
                        iObserver->SelectChangedL( aIndex );
                        }
                    }
                }
            else
                {
                iStatusArray[i] = ENonSelected;
                }            
            } 
        }
    }

// ---------------------------------------------------------------------------
// Handle pointer on checkbox.
// ---------------------------------------------------------------------------
//
void CTruiRadioButton::HandlePointerCheckBox( TInt aIndex, TCoeEvent aEventType )
    {
    if ( aIndex < iControlArray->Count() && aEventType == EEventRequestFocus )
        {
        iStatusArray[aIndex] = ( iStatusArray[aIndex] == ENonSelected ) 
                                            ? ESelected : ENonSelected;        
        }
    }
        
// ---------------------------------------------------------------------------
// Handles an event from an observed control.
// ---------------------------------------------------------------------------
//
void CTruiRadioButton::SizeChanged()
    {        
    LoadIconsLayout( 0 );    
    for ( TInt i = 0; i < iIconLayout.Count(); i++ )
        {
        TSize iconSize = iIconLayout[i].Rect().Size();
        AknIconUtils::SetSize( iRadioButtonSelectedBmp, iconSize );
        AknIconUtils::SetSize( iRadioButtonSelectedBmpm, iconSize );
        AknIconUtils::SetSize( iRadioButtonNonSelectedBmpm, iconSize );
        AknIconUtils::SetSize( iRadioButtonNonSelectedBmpm, iconSize );
        }
    // list_single_hwr_training_symbol_option_pane_t1  
    if ( iControlArray && iTextArray.Count() > 0 )
        {
        for ( TInt i = 0; i < iTextArray.Count(); i++ )
            {
            TAknTextComponentLayout option_pane_t1_layout = 
                       AknLayoutScalable_Apps::                       
                       list_single_hwr_training_symbol_option_pane_t1( 0 );                       
            CEikLabel* label = static_cast<CEikLabel*>( iControlArray->At( i ) );
            AknLayoutUtils::LayoutLabel( label, 
                                         Rect(), 
                                         iMultilineLayout );
            // Set color for label
            TRgb labelColor = KRgbBlack;
            TInt error = GetCachedLabelTextColor( labelColor );
            if ( error == KErrNone )
                {
                TRAP_IGNORE( label->OverrideColorL( EColorLabelText, labelColor ) );
                }
            TRAP_IGNORE( ClipToFitLabelL( label, i ) );
            // Set font for label
            if ( TBidiText::ScriptDirectionality( User::Language() )
                 ==  TBidiText::ERightToLeft )
                { // arabic,hebrew, set text RTL
                label->SetLabelAlignment( ELayoutAlignBidi );                
                }
            }            
        }
    }
    
// -----------------------------------------------------------------------------
// Draws control to given area
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::Draw( const TRect& /*aRect*/ ) const
    {
    CWindowGc& gc = SystemGc();
    // Gets the control's extent
    TRect rect( Rect() );
    // Clears the screen       
    Window().SetBackgroundColor( TRgb( ~0 ) );
    
    TSize pixSize = iRadioButtonSelectedBmp->SizeInPixels();
    for ( TInt i = 0; i < iStatusArray.Count(); i++ )
        {
        TPoint tl( rect.iTl.iX, rect.iTl.iY + pixSize.iWidth * i 
                   +  KSpaceBetweenItems * i );
        if ( iStatusArray[i] == ESelected )
            {
            iIconLayout[0].DrawImage( gc, iRadioButtonSelectedBmp, iRadioButtonSelectedBmpm );
            }
        else if ( iStatusArray[i] == ENonSelected )
            {
            iIconLayout[0].DrawImage( gc, iRadioButtonNonSelectedBmp, 
                                          iRadioButtonNonSelectedBmpm );
            }        
        }
    }

// -----------------------------------------------------------------------------
// Gets the number of controls contained in a compound control.
// -----------------------------------------------------------------------------
//
TInt CTruiRadioButton::CountComponentControls() const
    {
    if ( iControlArray )
        {
        return iControlArray->Count();
        }
    return 0;
    }

// -----------------------------------------------------------------------------
// Gets an indexed component of a compound control.
// -----------------------------------------------------------------------------
//
CCoeControl* CTruiRadioButton::ComponentControl( TInt aIndex ) const
    {
    if ( iControlArray && aIndex < iControlArray->Count() )
        {
        return iControlArray->At( aIndex );
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles a change to the control's resources.
// ---------------------------------------------------------------------------
//
void CTruiRadioButton::HandleResourceChange( TInt aType )
    {   
    CCoeControl::HandleResourceChange( aType );    
    if ( aType == KAknsMessageSkinChange && iControlArray )
        {        
        TRgb labelColor = KRgbBlack;
        TInt error = GetCachedLabelTextColor( labelColor );
        if ( error == KErrNone )
            {
            for ( TInt i = 0; i < iControlArray->Count(); i++ )
                {
                CCoeControl* label = iControlArray->At( i );
                TRAP_IGNORE( label->OverrideColorL( EColorLabelText, labelColor ) );
                }
            }  
        TRAP_IGNORE( CreateBitmapForRadioButtonL() ); 
        SizeChanged();       
        }    
    }

// -----------------------------------------------------------------------------
// Gets an indexed component of a compound control.
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
        // Click the area of icon
        TSize pixSize = iRadioButtonSelectedBmp->SizeInPixels();
        TRect rect( Rect() );
        for ( TInt i = 0; i < iControlArray->Count(); i++ )
            {
            if ( iIconLayout[i].Rect().Contains( aPointerEvent.iPosition ) )            
                {
                // Handle the event to point on Icon
                HandleControlEventL(  iControlArray->At( i ),
                                      EEventRequestFocus );
                }        
            }
        #ifdef RD_TACTILE_FEEDBACK 
        if ( iTactileSupported )
            {
            MTouchFeedback::Instance()->InstantFeedback( ETouchFeedbackBasic );
            }        
        #endif // RD_TACTILE_FEEDBACK
        }
    CCoeControl::HandlePointerEventL( aPointerEvent );
    }
    
// -----------------------------------------------------------------------------
// Return the control's minisize.
// -----------------------------------------------------------------------------
//
TSize CTruiRadioButton::MinimumSize()
    {
    TInt miniWidth = 0;
    TInt miniHeight = 0;
    for ( TInt i = 0; i < iControlArray->Count(); i++ )
        {
        TSize itemMiniSize = iControlArray->At( i )->Size();
        miniHeight += itemMiniSize.iHeight + KSpaceBetweenItems * i;
        miniWidth = itemMiniSize.iWidth 
                    + iRadioButtonSelectedBmp->SizeInPixels().iWidth;
        }
    return TSize( miniWidth, miniHeight );
    }
            
// -----------------------------------------------------------------------------
// Create radio button's bitmap.
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::CreateBitmapForRadioButtonL()
    {
    delete iRadioButtonSelectedBmp;
    delete iRadioButtonSelectedBmpm;
    delete iRadioButtonNonSelectedBmp;
    delete iRadioButtonNonSelectedBmpm;
    // Read resource file
    TResourceReader reader;
    TInt btnResource;
    if ( iSelectionType == ERadioButton )
        {
        btnResource = R_AVKON_SETTING_PAGE_RADIOBUTTON_ICONS;
        }
    else
        {
        btnResource= R_AVKON_SETTING_PAGE_CHECKBOX_ICONS;
        }
    iEikonEnv->CreateResourceReaderLC( reader, btnResource );
    reader.ReadInt16(); //not needed, simple or complex
    HBufC* bmpFile = reader.ReadHBufCL();
    CleanupStack::PushL( bmpFile );
    TInt count = reader.ReadInt16(); // count    
    if( count < KRadioButtonIconCounts )
        {
        User::Leave(KErrCorrupt);
        }    
    TInt32 bmpSelected = reader.ReadInt32();
    TInt32 bmpSelectedM = reader.ReadInt32();
    TInt32 bmpNonSelected = reader.ReadInt32();
    TInt32 bmpNonSelectedM = reader.ReadInt32();
    CleanupStack::Pop( bmpFile );
    CleanupStack::PopAndDestroy(); // reader
    
    CleanupStack::PushL( bmpFile );
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiRadiobuttOn,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 iRadioButtonSelectedBmp,
                                 iRadioButtonSelectedBmpm,
                                 *bmpFile,
                                 bmpSelected,
                                 bmpSelectedM,
                                 KRgbBlack
                                 );
    
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiRadiobuttOff,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 iRadioButtonNonSelectedBmp,
                                 iRadioButtonNonSelectedBmpm,                                  
                                 *bmpFile,
                                 bmpNonSelected,
                                 bmpNonSelectedM,
                                 KRgbBlack
                                 );
    CleanupStack::PopAndDestroy( bmpFile );   
    }

// ---------------------------------------------------------------------------
// Get cached color of skin for label control
// ---------------------------------------------------------------------------
// 
TInt CTruiRadioButton::GetCachedLabelTextColor( TRgb& aColor )
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    // Set pen color to the color of test in main area
    return AknsUtils::GetCachedColor( skin,
                                      aColor,
                                      KAknsIIDQsnTextColors,
                                      EAknsCIQsnTextColorsCG6 );
    }

// ---------------------------------------------------------------------------
// Load layouts for icons from LAF.
// ---------------------------------------------------------------------------
// 
void CTruiRadioButton::LoadIconsLayout( TInt aVarity )
    {
    TRect rect = Rect();
    iIconLayout.Reset();
    // Caculate list_single_hwr_training_symbol_option_pane_g1 for line1
    TAknWindowComponentLayout icon_checked_layout = 
         AknLayoutScalable_Apps::list_single_hwr_training_symbol_option_pane_g1         
                                        ( aVarity );
    TAknLayoutRect icon_checked_layout_rect;
    icon_checked_layout_rect.LayoutRect( rect, icon_checked_layout );    
    iIconLayout.Append( icon_checked_layout_rect );
    }
// -----------------------------------------------------------------------------
// Convert logical name to visual name for label when needed.
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::ConvertToVisualForEdwinL( CEikLabel* aLabel, 
                                                 const TDesC& aLogicalTxt, 
                                                 const CFont& aFont, 
                                                 TInt aLineWidth )
    {
    HBufC* visualText = HBufC::NewLC( aLogicalTxt.Length() 
                                      + TBidiLogicalToVisual::KMinCharAvailable
                                      + KExtraSize );
    TPtr visualTextPtr = visualText->Des();
    AknBidiTextUtils::ConvertToVisualAndClip( aLogicalTxt, visualTextPtr, 
                                              aFont, aLineWidth, aLineWidth );
    aLabel->SetTextL( *visualText ); 
    aLabel->SetLabelAlignment( ELayoutAlignBidi );
    CleanupStack::PopAndDestroy( visualText );   
    }

// -----------------------------------------------------------------------------
// Clip text of editor to fit editor width.
// -----------------------------------------------------------------------------
//
void CTruiRadioButton::ClipToFitLabelL( CEikLabel* aLabel, TInt aIndex )
    {    
    if ( aLabel && aLabel->Size().iWidth > 0 )
        {                
        // Get font fo editor from LAF
        const CFont* edwinFont = AknLayoutUtils::FontFromId
                                 ( iMultilineLayout.FontId() );
        // Prepare text to set into editor
        HBufC* textInArray = iTextArray[aIndex];
        // Create a new HBufC to store text for editor
        HBufC* edwinText = HBufC::NewLC( textInArray->Length()
                                         + KExtraSize );
        TPtr edwinTextPtr = edwinText->Des();        
        edwinTextPtr.Copy( *textInArray );
        // Clip text for editor
        AknTextUtils::ClipToFit( edwinTextPtr, *edwinFont, 
            aLabel->Size().iWidth );        
        aLabel->SetTextL( *edwinText );    
        CleanupStack::PopAndDestroy( edwinText );        
        }    
    }

