/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for Floating bubble
*
*/


#include <AknsDrawUtils.h> 
#include <AknUtils.h>

#include "peninputfloatbubble.h"

const TInt KDefaultMarginSize = 10;

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFloatingBubble* CPeninputFloatingBubble::NewL()
    {
    CPeninputFloatingBubble* self = CPeninputFloatingBubble::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFloatingBubble* CPeninputFloatingBubble::NewLC()
    {
    CPeninputFloatingBubble* self = new ( ELeave ) CPeninputFloatingBubble();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFloatingBubble::~CPeninputFloatingBubble()
    {
    if ( iText ) 
        {
        delete iText;
        }
    
    iText = NULL;    
    iIconImage = NULL;
    iIconMask = NULL;
    }

// ---------------------------------------------------------------------------
// Draw this control
// ---------------------------------------------------------------------------
//
void CPeninputFloatingBubble::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();
    TRect rect = Rect();
    gc.Clear( rect ); 
    
    //draw background
    if ( iBkgndSkinId.iMajor )
        {
        TRect border = rect;
        TRect inner = rect;
        inner.Shrink( KDefaultMarginSize, KDefaultMarginSize );
        AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                                  gc,
                                  border,
                                  inner,
                                  iBkgndSkinId,
                                  KAknsIIDDefault );
        }
    else
        {
        gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc.SetBrushColor( KRgbGray );
        gc.SetPenStyle( CGraphicsContext::ESolidPen );
        gc.SetPenColor( KRgbBlack );
        gc.DrawRoundRect( rect, TSize( 2, 2 ) );        
        }
    
    //draw icon
    if ( iIconImage )
        {
        TSize size = iIconImage->SizeInPixels();
        TInt x = rect.iTl.iX + ( rect.Width() - size.iWidth ) / 2;
        TInt y = rect.iTl.iY + ( rect.Height() - size.iHeight ) / 2;
        if ( iIconMask )
            {
            gc.BitBltMasked( TPoint( x, y ), iIconImage, 
                             TRect( TPoint( 0, 0 ), size), iIconMask, 
                             EFalse );
            }
        else
            {
            gc.BitBlt( TPoint( x, y ), iIconImage );
            }
        }
    
    //draw text
    if ( iText )
        {
        gc.SetBrushStyle( CGraphicsContext::ENullBrush );
        TAknLayoutText textLayout;
        textLayout.LayoutText( rect, iTextFormat );
        TRgb color( KRgbBlack );
        if ( AknsUtils::AvkonSkinEnabled() )
            {
            AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                                       color, 
                                       KAknsIIDQsnTextColors, 
                                       EAknsCIQsnTextColorsCG60 );
            }
         textLayout.DrawText( gc, *iText, EFalse, color );
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CPeninputFloatingBubble::SetBackgroundImage( TAknsItemID aBkgndImage )
    {
    iBkgndSkinId = aBkgndImage;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CPeninputFloatingBubble::SetText( const TDesC& aText )
    {
    if ( iText )
        {
        delete iText;
        }
    iText = NULL;
    
    iText = aText.Alloc();  
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CPeninputFloatingBubble::SetTextFormat( TAknTextLineLayout aTextFormat )
    {
    iTextFormat = aTextFormat;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CPeninputFloatingBubble::SetIcon( CFbsBitmap* aIconImage, CFbsBitmap* aIconMask )
    {
    iIconImage = aIconImage;
    iIconMask = aIconMask;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFloatingBubble::CPeninputFloatingBubble()
    : iBkgndSkinId( KAknsIIDNone )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputFloatingBubble::ConstructL()
    {
    BaseConstructL();
    }
