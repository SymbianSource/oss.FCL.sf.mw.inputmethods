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
* Description:  Implement of class CTruiContainerBase
*
*/


#include <AknDef.h>
#include <aknbutton.h>
#include <barsread.h>
#include <akntoolbar.h>
#include <e32property.h>
#include <AvkonInternalCRKeys.h>
#include <eikapp.h>
#include <AknsUtils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <gulicon.h>
#include <aknview.h>
#include <s32stor.h>

#include "truicontainerbase.h"
#include "truiappui.h"

_LIT( KChangeLine, "\n" );

const TSize KHwrBoxSizeLandscape = TSize( 200, 200 );

// ======== MEMBER FUNCTIONS ========

CTruiContainerBase::CTruiContainerBase()
    {
    iAppUi = static_cast<CTruiAppUi*>( iAvkonViewAppUi );
    iEngine = iAppUi->HwrEngine();
    }

CTruiContainerBase::~CTruiContainerBase()
    {
    }

// ---------------------------------------------------------------------------
// Get the origin size in which input the model for preset symbols. 
// ---------------------------------------------------------------------------
//
TSize CTruiContainerBase::OriginSymbolSize()
    {   
    TRect rect = iAvkonViewAppUi->ClientRect();
    
    TAknWindowComponentLayout main_pane_layout = 
                            AknLayoutScalable_Apps::main_hwr_training_pane();
    TAknLayoutRect main_pane_layout_rect;
    main_pane_layout_rect.LayoutRect( rect, main_pane_layout );
    TRect main_pane_rect = main_pane_layout_rect.Rect();
    
    // hwr_training_write_pane
    TAknWindowComponentLayout write_pane_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane( 2 );
    TAknLayoutRect write_pane_layout_rect;
    write_pane_layout_rect.LayoutRect( main_pane_rect, write_pane_layout );
    
    TAknWindowComponentLayout drawable_rect_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g7( 0 );
    TAknLayoutRect drawable_rect_layout_rect;
    drawable_rect_layout_rect.LayoutRect( write_pane_layout_rect.Rect(), 
                                          drawable_rect_layout );
    return drawable_rect_layout_rect.Rect().Size();
    }

// ---------------------------------------------------------------------------
// Wrap the text to fit the minisize of Label.
// ---------------------------------------------------------------------------
//
TInt CTruiContainerBase::WrapLabelText( const CEikLabel* aLabel, 
                                            const TDesC& aText,
                                            TInt aMaxLines, 
                                            HBufC* aWrappedText )
    {
    return DoWrapLabelText( aLabel->Font(), aLabel->Size().iWidth, aText, 
                            aMaxLines, aWrappedText );
    }

// ---------------------------------------------------------------------------
// Wrap the text to fit the minisize of Label.
// ---------------------------------------------------------------------------
//
TInt CTruiContainerBase::WrapLabelText(  const TAknLayoutText& aLayoutText,
                                         const TDesC& aText,
                                         TInt aMaxLines, 
                                         HBufC* aWrappedText )
    {
    const CFont* font = aLayoutText.Font();
    TInt width = aLayoutText.TextRect().Size().iWidth;
    return DoWrapLabelText( font, width, aText, aMaxLines, aWrappedText );
    }    

// ---------------------------------------------------------------------------
// Wrap the text to fit the minisize of Label.
// ---------------------------------------------------------------------------
//
void CTruiContainerBase::WrapLabelTextL(  const CFont* aFont, const TDesC& aText,
    const RArray<TInt>& aLineWidthArray, 
    HBufC* aWrappedText )
    {    
    if ( aWrappedText && aLineWidthArray.Count() > 0 )
        {
        CArrayFixFlat<TInt>* lineWidthArray = new (ELeave) 
                                CArrayFixFlat<TInt>( aLineWidthArray.Count() );
        CleanupStack::PushL( lineWidthArray );
        // Create line array        
        for ( TInt i = 0; i < aLineWidthArray.Count(); i++ )
            {
            lineWidthArray->AppendL( aLineWidthArray[i] );
            }
        // Do wrap and clip
        TPtr wrappedTextPtr = aWrappedText->Des();
        AknTextUtils::WrapToStringAndClipL( aText, *lineWidthArray, *aFont, 
            wrappedTextPtr );        
        CleanupStack::PopAndDestroy( lineWidthArray );
        }    
    }    

// ---------------------------------------------------------------------------
// Create skin specified button from resource.
// ---------------------------------------------------------------------------
//
CAknButton* CTruiContainerBase::CreateButtonL( TInt aResourceId, 
                                               const CCoeControl* aParent, 
                                               MCoeControlObserver* aObserver )
    {
    TResourceReader reader;
    iEikonEnv->CreateResourceReaderLC( reader, aResourceId );
    CAknButton* button = ConstructButtonFromResourceL( reader );
    if ( aParent )
        {
        button->SetContainerWindowL( *aParent );
        }   
    if ( aObserver )
        {
        button->SetObserver( aObserver );    
        }    
    CleanupStack::PopAndDestroy(); // reader    
    return button;
    }

// ---------------------------------------------------------------------------
// Add skin specified buttons into toolbar.
// ---------------------------------------------------------------------------
//
void CTruiContainerBase::AddButtonsToToolbarL( CAknToolbar* aToolbar, TInt aResourceId )
    {
    TResourceReader reader;
    iEikonEnv->CreateResourceReaderLC( reader, aResourceId );
    const TInt buttonCount = reader.ReadInt16();
    for ( TInt i = 0; i < buttonCount; i++ )
        {
        // Create each toolbar item
        TInt controlType = reader.ReadInt16();
        TInt commandId = reader.ReadInt16();
        reader.ReadInt16(); // flags
        reader.ReadInt16(); // length        
        CAknButton* button = ConstructButtonFromResourceL( reader );
        aToolbar->AddItemL( button, controlType, commandId, 0, i );       
        }
    CleanupStack::PopAndDestroy(); // reader    
    }

// ---------------------------------------------------------------------------
// Return current keyboard mode.
// ---------------------------------------------------------------------------
// 
TInt CTruiContainerBase::CurrentKeyBoardModeL()
    {
    RProperty keyboardProperty;
    CleanupClosePushL( keyboardProperty );
    User::LeaveIfError( keyboardProperty.Attach( KCRUidAvkon, 
                                                 KAknQwertyInputModeActive ) );
    TInt keyboardMode = 0;
    keyboardProperty.Get( keyboardMode );    
    CleanupStack::PopAndDestroy( &keyboardProperty ); 
    return keyboardMode;
    }

// ---------------------------------------------------------------------------
// Read setting from ini file to decide if display wizard view.
// ---------------------------------------------------------------------------
// 
void CTruiContainerBase::GetSettingFromIniFileL( TUint32 aKey, TInt& aValue )
    {
    CDictionaryStore* iniFile = iAppUi->Application()->OpenIniFileLC( iEikonEnv->FsSession() );
    RDictionaryReadStream readSteam;
    readSteam.OpenLC( *iniFile, TUid::Uid( aKey ) );
    TInt16 value;
    readSteam >> value;
    aValue = value;
    CleanupStack::PopAndDestroy( &readSteam );
    CleanupStack::PopAndDestroy( iniFile );    
    }

// ---------------------------------------------------------------------------
// Save settings into file.
// ---------------------------------------------------------------------------
// 
void CTruiContainerBase::SaveSettingIntoIniFileL( TUint32 aKey, TInt aValue )
    {
    CDictionaryStore* iniFile = 
                 iAppUi->Application()->OpenIniFileLC( iEikonEnv->FsSession() );
    RDictionaryWriteStream stream;
    stream.AssignLC( *iniFile, TUid::Uid( aKey ) );
    stream << (TInt16)aValue;
    stream.CommitL();
    iniFile->CommitL();
    CleanupStack::PopAndDestroy( &stream );
    CleanupStack::PopAndDestroy( iniFile );
    }

// ---------------------------------------------------------------------------
// Get cached color of skin for label control
// ---------------------------------------------------------------------------
// 
TInt CTruiContainerBase::GetCachedLabelTextColor( TRgb& aColor )
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    // Set pen color to the color of test in main area
    return AknsUtils::GetCachedColor( skin,
                                      aColor,
                                      KAknsIIDQsnTextColors,
                                      EAknsCIQsnTextColorsCG6 );
    }

// ---------------------------------------------------------------------------
// Get cached color of skin for icon
// ---------------------------------------------------------------------------
// 
TInt CTruiContainerBase::GetCachedIcontColor( TRgb& aColor, 
                                              const TAknsItemID& aColorId, 
                                              TInt aColorIndex )
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    // Set pen color to the color of test in main area
    return AknsUtils::GetCachedColor( skin,
                                      aColor,
                                      aColorId,
                                      aColorIndex );
    }

// ---------------------------------------------------------------------------
// Create skin specified button from resource.
// ---------------------------------------------------------------------------
//
CAknButton* CTruiContainerBase::ConstructButtonFromResourceL( TResourceReader& aReader )
    {
    TInt flags = aReader.ReadInt16();
    TPtrC helpText = aReader.ReadTPtrC();
    TPtrC fileName = aReader.ReadTPtrC();
    TInt bmpId = aReader.ReadInt16();    
    TInt bmpMaskId = aReader.ReadInt16();
    TInt pressId = aReader.ReadInt16();
    TInt pressMaskId = aReader.ReadInt16();
    TInt mainItemId = aReader.ReadInt32();
    TInt minorItemId = aReader.ReadInt32();
    TInt mainItemPressId = aReader.ReadInt32();
    TInt minorItemPressId = aReader.ReadInt32();
    
    CGulIcon* icon = NULL;
    TInt length = fileName.Length();
    if ( fileName.Length() )
        {
        TAknsItemID aknItemId = { mainItemId, minorItemId };        
        
        // Create icon
        CFbsBitmap* bitmap;
        CFbsBitmap* bitmapm;
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        AknsUtils::CreateColorIconL( skin,
                                     aknItemId,
                                     KAknsIIDQsnIconColors,
                                     EAknsCIQsnIconColorsCG30,
                                     bitmap,
                                     bitmapm,
                                     fileName,
                                     bmpId,
                                     bmpMaskId,
                                     KRgbBlack );
        CleanupStack::PushL( bitmap );
        CleanupStack::PushL( bitmapm );
        icon = CGulIcon::NewL( bitmap, bitmapm ); // Ownership transfered
        CleanupStack::Pop( bitmapm );
        CleanupStack::Pop( bitmap );        
        }
    CleanupStack::PushL( icon );
    CAknButton* button =  CAknButton::NewL ( icon, 
                                             NULL,
                                             NULL,
                                             NULL,
                                             KNullDesC, 
                                             helpText, 
                                             flags,
                                             NULL );
    CleanupStack::Pop( icon );
    return button;    
    }

// ---------------------------------------------------------------------------
// Construct icons from resource.
// ---------------------------------------------------------------------------
//
CGulIcon* CTruiContainerBase::ConstructIconFromResourceL( TInt aResourceId )
    {
    TResourceReader reader;
    iEikonEnv->CreateResourceReaderLC( reader, aResourceId );
    TInt flags = reader.ReadInt16();
    TPtrC helpText = reader.ReadTPtrC();
    TPtrC fileName = reader.ReadTPtrC();
    TInt bmpId = reader.ReadInt16();    
    TInt bmpMaskId = reader.ReadInt16();
    TInt pressId = reader.ReadInt16();
    TInt pressMaskId = reader.ReadInt16();
    TInt mainItemId = reader.ReadInt32();
    TInt minorItemId = reader.ReadInt32();
    TInt mainItemPressId = reader.ReadInt32();
    TInt minorItemPressId = reader.ReadInt32();
    
    TAknsItemID aknItemId = { mainItemId, minorItemId };    
    
    // Create icon
    CFbsBitmap* bitmap;
    CFbsBitmap* bitmapm;
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    AknsUtils::CreateColorIconL( skin,
                                 aknItemId,
                                 KAknsIIDQsnIconColors,                                 
                                 EAknsCIQsnIconColorsCG30,
                                 bitmap,
                                 bitmapm,
                                 fileName,
                                 bmpId,
                                 bmpMaskId,
                                 KRgbBlack );
    CleanupStack::PushL( bitmap );
    CleanupStack::PushL( bitmapm );
    CGulIcon* icon = CGulIcon::NewL( bitmap, bitmapm ); // Ownership transfered
    CleanupStack::Pop( bitmapm );
    CleanupStack::Pop( bitmap );    
    CleanupStack::PopAndDestroy(); // reader  
    return icon;      
    }

// ---------------------------------------------------------------------------
// Wrap the text to fit the minisize of Label.
// ---------------------------------------------------------------------------
//
TInt CTruiContainerBase::DoWrapLabelText(  const CFont* aFont, TInt aWidth,
                                           const TDesC& aText, TInt aMaxLines, 
                                           HBufC* aWrappedText )
    {        
    TInt lineCount;
    TInt startPos = 0;
    TInt lines = 1;
    if ( aWidth <= 0 || !aWrappedText )
        {
        return 0;
        }    
    TPtrC subText = aText.Mid( startPos, aText.Length() - startPos );    
    TPtr newTextPtr = aWrappedText->Des();
    newTextPtr.Copy( KNullDesC ); 
    while ( ( lineCount = aFont->TextCount( subText, aWidth ) ) < subText.Length() )
        {
        // Need to wrap
        if ( ++lines > aMaxLines )
            {
            // Check if it is up to maximum line
            break;
            }
        // get the last char of every line
        TChar nextCharLine = aText[ startPos + lineCount ];
        
        // Decide at which position insert the \n
        if ( nextCharLine == TChar( ' ' ) )
            {
            // Replace nextCharLine with \n
            newTextPtr.Append( subText.Mid( 0, lineCount ) );
            newTextPtr.Append( KChangeLine );
            // And move the startPos for the next loop.
            startPos += lineCount + 1;
            }
        else
            {
            // Go forward to find " " and replace it with \n
            // And move the startPos for the next loop.
            TPtrC subTextPtr = subText.Mid( 0, lineCount );
            TInt pos = subTextPtr.LocateReverse( TChar( ' ' ) );
            if ( pos != KErrNotFound )
                {
                newTextPtr.Append( subTextPtr.Left( pos ) );
                newTextPtr.Append( KChangeLine );
                startPos += pos + 1;
                }
            else
                {
                newTextPtr.Append( subTextPtr );
                newTextPtr.Append( KChangeLine );
                startPos += lineCount;
                }
            }
        subText.Set( aText.Mid( startPos, aText.Length() - startPos ) );
        }
    newTextPtr.Append( subText );
    return lines;
    }
        
// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles a change to the control's resources.
// ---------------------------------------------------------------------------
//
void CTruiContainerBase::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType==KEikDynamicLayoutVariantSwitch && iView )
        {
        SetRect( iView->ClientRect() );            
        }     
    }
