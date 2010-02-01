/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninput hwr data manager
*
*/

#include "peninputpentracedecorator.h"

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputPenTraceDecorator* CPeninputPenTraceDecorator::NewL()
    {
    CPeninputPenTraceDecorator* self = CPeninputPenTraceDecorator::NewLC();
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputPenTraceDecorator* CPeninputPenTraceDecorator::NewLC()
    {
    CPeninputPenTraceDecorator* self = new(ELeave) CPeninputPenTraceDecorator();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputPenTraceDecorator::~CPeninputPenTraceDecorator()
    {
    ReleaseBuffer();   
    }


// ---------------------------------------------------------------------------
// draw a single point. 
// ---------------------------------------------------------------------------
//

TRect CPeninputPenTraceDecorator::DrawPoint( const TPoint& aPoint )
    {
    if ( !iBitsBitmap || !iMaskBitmap || !iXSquareArray )
        {
        __ASSERT_DEBUG( EFalse, User::Panic( _L("PenTrace Decorator: invalid status"), 0 ) );        
        return TRect();
        }
    
    TSize cvsize = iBitsBitmap->SizeInPixels();
    if ( cvsize.iWidth < 1 || cvsize.iHeight < 1 )
        {
        __ASSERT_DEBUG( EFalse, User::Panic( _L("PenTrace Decorator: invalid status"), 0 ) );        
        return TRect();
        }
    
    TBitmapUtil bits( iBitsBitmap );
    TBitmapUtil mask( iMaskBitmap );
    
    TDrawLineParams params;
    
    bits.Begin( TPoint( 0, 0 ) );
    mask.Begin( TPoint( 0, 0 ) );
    
    if ( ETrue )
        {        
        TDisplayMode mode = iDisplayMode;
        TRgb rgb = iPenTraceColor;
        TInt32 clr = (TInt32) Rgb2DeviceColor( mode, rgb );
        
        params.rtAffected = TRect();
        
        params.bits = &bits;
        params.mask = &mask;
        
        params.SqX = iXSquareArray;   // squares of X
        
        params.nW = cvsize.iWidth;   // width of destination 
        params.nH = cvsize.iHeight;  // height of destination
        
        params.color = clr;
        params.bR = rgb.Red();       // value of red channel 
        params.bG = rgb.Green();     // value of green channel
        params.bB = rgb.Blue();      // value of blue channel 
        
        params.x1 = aPoint.iX << 4 ; // (x of start point) << 4
        params.y1 = aPoint.iY << 4 ; // (y of start point) << 4
        params.d1 = 1;               // width of start point
        
        params.x2 = aPoint.iX << 4;  // (x of end point) << 4
        params.y2 = aPoint.iY << 4;  // (y of end point) << 4
        params.d2 = 1;               // width of end point
        
        DrawLongSegment( &params );
        }
    
    mask.End();
    bits.End();
    
    iModified = ETrue;
    
    return params.rtAffected;    
    }

// ---------------------------------------------------------------------------
// draw a single line. 
// ---------------------------------------------------------------------------
//
TRect CPeninputPenTraceDecorator::DrawLine( 
    const TPoint& aPt1, const TInt aWidth1, const TRgb& /*aColor1*/, TAny* /*aData1*/,  
    const TPoint& aPt2, const TInt aWidth2, const TRgb& /*aColor2*/, TAny* /*aData2*/ )
    {
    if ( !iBitsBitmap || !iMaskBitmap || !iXSquareArray )
        {
        __ASSERT_DEBUG( EFalse, User::Panic( _L("PenTrace Decorator: invalid status"), 0 ) );        
        return TRect();
        }
    
    TSize cvsize = iBitsBitmap->SizeInPixels();
    if ( cvsize.iWidth < 1 || cvsize.iHeight < 1 )
        {
        __ASSERT_DEBUG( EFalse, User::Panic( _L("PenTrace Decorator: invalid status"), 0 ) );        
        return TRect();
        }
    
    TBitmapUtil bits( iBitsBitmap );
    TBitmapUtil mask( iMaskBitmap );
    
    TDrawLineParams params;
    
    bits.Begin( TPoint( 0, 0 ) );
    mask.Begin( TPoint( 0, 0 ) );
    
    if (  ETrue  )
        {        
        TDisplayMode mode = iDisplayMode;
        TRgb rgb = iPenTraceColor;
        TInt32 clr = (TInt32) Rgb2DeviceColor( mode, rgb );
        
        params.rtAffected = TRect();
        
        params.bits = &bits;
        params.mask = &mask;
        
        params.SqX = iXSquareArray;   // squares of X
        
        params.nW = cvsize.iWidth;   // width of destination 
        params.nH = cvsize.iHeight;  // height of destination
        
        params.color = clr;
        params.bR = rgb.Red();       // value of red channel 
        params.bG = rgb.Green();     // value of green channel
        params.bB = rgb.Blue();      // value of blue channel 
        
        params.x1 = aPt1.iX << 4 ;   // (x of start point) << 4
        params.y1 = aPt1.iY << 4 ;   // (y of start point) << 4
        params.d1 = aWidth1;         // (width of start point) << 3
        
        params.x2 = aPt2.iX << 4;    // (x of end point) << 4
        params.y2 = aPt2.iY << 4;    // (y of end point) << 4
        params.d2 = aWidth2;         // (width of end point) << 3
        
        DrawLongSegment( &params );
        }
    
    mask.End();
    bits.End();
    
    iModified = ETrue;
    iLineCount++;
    
    return params.rtAffected;
    }

// ---------------------------------------------------------------------------
// Continue to draw a line in a series of lines. 
// ---------------------------------------------------------------------------
//
TRect CPeninputPenTraceDecorator::DrawNextLine( const TPoint& aPt1, const TPoint& aPt2 )
    {
    if ( !iBitsBitmap || !iMaskBitmap || !iXSquareArray )
        {
        __ASSERT_DEBUG( EFalse, User::Panic( _L("Pen Trace Decorator: invalid status"), 0 ) );        
        return TRect();
        }
    
    TSize cvsize = iBitsBitmap->SizeInPixels();
    if ( cvsize.iWidth < 1 || cvsize.iHeight < 1 )
        {
        __ASSERT_DEBUG( EFalse, User::Panic( _L("Pen Trace Decorator: invalid status"), 0 ) );        
        return TRect();
        }
    
    TInt width_start = 0 ;
    TInt width_end = 0;
    
    TInt penWidthDefault = 16;        //default width
    TInt penWidthMin = 8;             // min width
    TInt penWidthAdapt = 4;           // adaption factor
    TInt penPressureThreshold = 1200; // threshold of pressure
    
    TPoint pt1 = aPt1;
    TPoint pt2 = aPt2;

    width_start = iLastPenWidth;
    TInt pressure = ( pt1.iX - pt2.iX ) * ( pt1.iX - pt2.iX ) + ( pt1.iY - pt2.iY ) * ( pt1.iY - pt2.iY );
    if ( pressure >= penPressureThreshold )
        {
        width_end = penWidthMin;
        }
    else
        {
        width_end = (TUint8)( ( penWidthDefault * penWidthAdapt * ( penPressureThreshold - pressure ) + penPressureThreshold / 2 ) / penPressureThreshold + penWidthMin );
        }
    
    
    width_end = (TUint8)( ( width_start + width_end + 1 ) / 2 );
    iLastPenWidth = width_end;
    
    TBitmapUtil bits( iBitsBitmap );
    TBitmapUtil mask( iMaskBitmap );
    
    TDrawLineParams params;
    
    bits.Begin( TPoint( 0, 0 ) );
    mask.Begin( TPoint( 0, 0 ) );
    
    if (  ETrue  )
        {        
        TDisplayMode mode = iDisplayMode;
        TRgb color = iPenTraceColor;
        TInt32 clrdev = (TInt32) Rgb2DeviceColor( mode, color );
        
        params.rtAffected = TRect();
        
        params.bits = &bits;
        params.mask = &mask;
        
        params.SqX = iXSquareArray;   // squares of X
        
        params.nW = cvsize.iWidth;   // width of destination 
        params.nH = cvsize.iHeight;  // height of destination
        
        params.color = clrdev;
        params.bR = color.Red();     // value of red channel 
        params.bG = color.Green();   // value of green channel
        params.bB = color.Blue();    // value of blue channel 
        
        params.x1 = pt1.iX << 4 ;    // (x of start point) << 4
        params.y1 = pt1.iY << 4 ;    // (y of start point) << 4
        params.d1 = width_start;     // (width of start point) << 3
        
        params.x2 = pt2.iX << 4;     // (x of end point) << 4
        params.y2 = pt2.iY << 4;     // (y of end point) << 4
        params.d2 = width_end;       // (width of end point) << 3
        
        DrawLongSegment( &params );
        }
    
    mask.End();
    bits.End();
    
    iModified = ETrue;
    iLineCount++;
    
    return params.rtAffected;
    }

// ---------------------------------------------------------------------------
// Clear decoration buffer. 
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::Clear( TBool aAlwaysErase )
    {
    iLastPenWidth = 8;
    iLineCount = 0;
    
    if ( !( aAlwaysErase || iModified ) )
        {
        return;
        }
    
    TSize size = iBitsBitmap->SizeInPixels();
    TBitmapUtil bmpbits( iBitsBitmap );
    bmpbits.Begin( TPoint( 0, 0 ) );
    for ( TInt i = 0; i < size.iHeight; i++ )
        {
        bmpbits.SetPos( TPoint( 0, i ) );
        
        for ( TInt j = 0; j < size.iWidth; j++ )
            {
            bmpbits.SetPixel( 0xFFFFFF );
            bmpbits.IncXPos();
            }
        }
    bmpbits.End();

    size = iMaskBitmap->SizeInPixels();
    TBitmapUtil maskbits( iMaskBitmap ); 
    maskbits.Begin( TPoint( 0, 0 ) );
    for ( TInt i = 0; i < size.iHeight; i++ )
        {
        maskbits.SetPos( TPoint( 0, i ) );
        
        for ( TInt j = 0; j < size.iWidth; j++ )
            {
            maskbits.SetPixel( 0x000000 );
            maskbits.IncXPos();
            }
        }
    maskbits.End();
    
    iModified = EFalse;
    }

// ---------------------------------------------------------------------------
// Resize decoration buffer. 
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::ResizeL( const TSize& aSize, TBool aAlwaysResize )
    {
    if ( aSize.iWidth < 1 && aSize.iHeight < 1 )
        {
        return;
        }
    
    if ( iBufferSize == aSize && !aAlwaysResize )
        {
        return;
        }

    TDisplayMode mode = iDisplayMode;
    
    if ( iBitsBitmap ) 
        {
        delete iBitsBitmap;
        iBitsBitmap = NULL;
        }
    iBufferSize = aSize;
    iBitsBitmap = new(ELeave) CFbsBitmap();
    iBitsBitmap->Create( aSize, mode );
    
    
    if ( iMaskBitmap )
        {
        delete iMaskBitmap;
        iMaskBitmap = NULL;
        }
    iMaskBitmap = new(ELeave) CFbsBitmap();
    iMaskBitmap->Create( aSize, EGray256 );
    
    Clear( ETrue );
    
    if ( iXSquareArray )
        {
        delete iXSquareArray;
        }
    iXSquareArray = new TInt32[ aSize.iWidth + 1 ];    
    }

// ---------------------------------------------------------------------------
// Set display mode of decoration buffer. 
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::SetDisplayModeL( TDisplayMode aDisplayMode, TBool aUpdateBuffer )
    {
    if ( iDisplayMode == aDisplayMode )
        {
        return;
        }
    
    iDisplayMode = aDisplayMode;
    if ( aUpdateBuffer )
        {
        ResizeL( iBufferSize, ETrue );
        }
    }

// ---------------------------------------------------------------------------
// Enable decorator. 
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::Enable()
    {
    if ( !iEnabled && ( !iBitsBitmap || !iMaskBitmap || !iXSquareArray ) )
        {
        TRAPD( err, ResizeL( iBufferSize, ETrue ) );
        if ( err )
            {
            iEnabled = EFalse;
            return;
            }
        }
    iEnabled = ETrue;
    }

// ---------------------------------------------------------------------------
// Disable decorator. 
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::Disable( TBool aReleaseBuffer )
    {
    iEnabled = EFalse;
    if (  aReleaseBuffer )
        {
        ReleaseBuffer();
        }    
    }

// ---------------------------------------------------------------------------
// Get Enabled/Disabled state of decorator. 
// ---------------------------------------------------------------------------
//
TBool CPeninputPenTraceDecorator::Enabled()
    {
    return iEnabled;
    }

// ---------------------------------------------------------------------------
// Set pen trace color. 
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::SetPenTraceColor( const TRgb& aColor )
    {
    iPenTraceColor = aColor;
    }

// ---------------------------------------------------------------------------
// Get image channel of decoration buffer. 
// ---------------------------------------------------------------------------
//
CFbsBitmap* CPeninputPenTraceDecorator::BitsBitmap()
    {
    return iBitsBitmap;
    }

// ---------------------------------------------------------------------------
// Get mask channel of decoration buffer. 
// ---------------------------------------------------------------------------
//
CFbsBitmap* CPeninputPenTraceDecorator::MaskBitmap()
    {
    return iMaskBitmap;
    }

// ---------------------------------------------------------------------------
// Convert device-independent color to device-dependent color. 
// ---------------------------------------------------------------------------
//
TInt CPeninputPenTraceDecorator::Rgb2DeviceColor( TDisplayMode aDisplayMode, const TRgb& aColor )
    {
    switch( aDisplayMode )
        {
        case EGray2:
            {
            return aColor.Gray2();
            }
        case EGray4:
            {
            return aColor.Gray4();
            }
        case EGray16:
            {
            return aColor.Gray16();
            }
        case EGray256:
            {
            return aColor.Gray256();
            }
        case EColor16:
            {
            return aColor.Color16();
            }
        case EColor256:
            {
            return aColor.Color256();
            }
        case EColor64K:
            {
            return aColor.Color64K();
            }
        case EColor16M:
            {
            return aColor.Color16M();
            }
        case EColor4K:
            {
            return aColor.Color4K();
            }
        case EColor16MU:
            {
            return aColor.Color16MU();
            }
        case EColor16MA:
            {
            return aColor.Color16MA();
            }
        case EColor16MAP:
            {
            return aColor.Color16MAP();
            }
        default: 
            return aColor.Internal();
        }
    }

// ---------------------------------------------------------------------------
// draw short line pixel by pixel. 
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::DrawShortSegment( TDrawLineParams* aParams )
    {
    TInt32 RPF2;   // squares of outer circle radius 
    TInt32 RMF2;   // squares of inner circle radius 
    TInt32 LX;     // left border of circle 
    TInt32 RX;     // right border of circle
    TInt32 LY;     // top border of circle
    TInt32 RY;     // bottom border of circle
    TInt32 SqY;    // squares of Y
    TInt32 SqDist; // squared distance from center for one pixel 
    TInt32 RPMF2;  // diameter of circle 
    TInt32 nFact;

    TInt32 x, y;
    TInt32 d = aParams->currd;
    
    TInt32 color = aParams->color;
    
    d = ( d < 12 ) ? 12 : d;
    RPF2 = ( d + 8 );
    RMF2 = ( d - 8 );
    RMF2 = ( RMF2 < 8 ) ? 8 : RMF2;

    // Determine bounds 
    LX = (TInt32)( (aParams->currx - RPF2) >> 4 );
    RX = (TInt32)( (aParams->currx + RPF2) >> 4 );
    LY = (TInt32)( (aParams->curry - RPF2) >> 4 );
    RY = (TInt32)( (aParams->curry + RPF2) >> 4 );
    
    LX = ( LX < 0 ) ? 0 : LX;
    RX = ( RX > aParams->nW - 1 ) ? aParams->nW - 1 : RX;    
    LY = ( LY < 0 ) ? 0 : LY;
    RY = ( RY > aParams->nH - 1 ) ? aParams->nH - 1 : RY;
    
    if ( RX < LX )
        {
        return;
        }
    
    TRect rtDraw( LX, LY, RX, RY );
    if ( aParams->rtAffected.IsEmpty() )
        {
        aParams->rtAffected = rtDraw;
        }
    else
        {
        aParams->rtAffected.BoundingRect( rtDraw );
        }    
    
    RPF2 *= RPF2;
    RMF2 *= RMF2;
    
    RPMF2 = d * 2;

    // Optimization run: find squares of X first 
    for ( x = LX; x <= RX; x++ )
        {
        y = (x << 4) - aParams->currx;
        aParams->SqX[x] = y * y;
        }
    
    // Loop through Y values 
    for ( y = LY; y <= RY; y++ )
        {
        aParams->bits->SetPos( TPoint( LX, y ) );
        aParams->mask->SetPos( TPoint( LX, y ) );
        
        SqY = (y << 4) - aParams->curry;
        SqY *= SqY;
        // Loop through X values 
        for ( x = LX; x <= RX; x++ )
            {
            // determine squared distance from center for this pixel 
            SqDist = SqY + aParams->SqX[x];
            // inside inner circle? Most often.. 
            if ( SqDist < RMF2 )
                {
                if ( !aParams->mask->GetPixel() )
                    {
                    aParams->bits->SetPixel( color );
                    }
                // inside the inner circle.. give the scanline the new color
                TUint8 a = (TUint8)( aParams->mask->GetPixel() & 0xFF );
                a = (TUint8)( 128 + ( a >> 1 ) );
                aParams->mask->SetPixel( a ); //mask bitmap must be EGray256
                }
            else
                {
                // inside outer circle? 
                if ( SqDist < RPF2 )
                    {
                    // We are inbetween the inner and outer bound, now mix the color
                    nFact = (18 - (SqDist - RMF2) / RPMF2) / 3;
                    if ( nFact > 0 )
                        {
                        if ( !aParams->mask->GetPixel() )
                            {
                            aParams->bits->SetPixel( color );
                            }
                        TUint8 a = (TUint8)(aParams->mask->GetPixel() & 0xFF);
                        a = (TUint8)( 16 * nFact + ((a * (16 - nFact)) >> 4) );
                        aParams->mask->SetPixel( a ); //mask bitmap must be EGray256
                        }
                    }
                }
            
            aParams->bits->IncXPos();
            aParams->mask->IncXPos();
            }
        }
    }

// ---------------------------------------------------------------------------
// draw line step by step.
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::DrawLongSegment(TDrawLineParams * aParams)
    {
    TInt32 dx = aParams->x2 - aParams->x1;
    TInt32 dy = aParams->y2 - aParams->y1;
    TInt32 step  = ( dx > 0 ) ? dx : -dx;
    TInt32 stepy = ( dy > 0 ) ? dy : -dy;
    TInt32 dmin  = aParams->d1;
    TInt32 dstep, currd, xystep, currxy;
    // the line length < 1 pixel? 
    if ( step < 12 && stepy < 12 )
        {
        aParams->currx = aParams->x2;
        aParams->curry = aParams->y2;
        aParams->currd = aParams->d2;
        DrawShortSegment(aParams);
        return;
        }
    // find minimum d 
    if ( aParams->d1 > aParams->d2 )
        {
        dmin = aParams->d2;
        }
    if ( dmin < 12 )
        {
        dmin = 12;
        }
    
    // x distances >= y distances? 
    if ( step >= stepy )
        {
        if ( ( step * 4 ) > dmin )
            {
            step = dmin / 4;
            step = ( step < 16 ) ? 16 : step & 0xFFFFFFF0;
            step = ( dx < 0 ) ? -step : step;
            
            // start x 
            aParams->currx = aParams->x1;
            currd = aParams->d1 << 4;
            dstep = ( ( aParams->d2 - aParams->d1 ) << 4 ) * step / dx;
            currxy = aParams->y1 << 4;
            xystep = ( ( aParams->y2 - aParams->y1 ) << 4 ) * step / dx;
            
            // x is growing 
            if ( dx > 0 )
                {
                while ( aParams->currx < aParams->x2 )
                    {
                    aParams->curry = currxy >> 4;
                    aParams->currd = currd >> 4;
                    DrawShortSegment( aParams );
                    currxy += xystep;
                    currd += dstep;
                    aParams->currx += step;
                    }
                }
            else
                {
                // x is decreasing 
                while ( aParams->currx > aParams->x2 )
                    {
                    aParams->curry = currxy >> 4;
                    aParams->currd = currd >> 4;
                    DrawShortSegment( aParams );
                    currxy += xystep;
                    currd += dstep;
                    aParams->currx += step;
                    }
                }
            return;
            }
        }
    else
        {
        if ( ( stepy * 4 ) > dmin )
            {
            step = dmin / 4;
            step = ( step < 16 ) ? 16 : step & 0xFFFFFFF0;
            step = ( dy < 0 ) ? -step : step;

            // start y 
            aParams->curry = aParams->y1;
            currd = aParams->d1 << 4;
            dstep = ( ( aParams->d2 - aParams->d1 ) << 4 ) * step / dy;
            currxy = aParams->x1 << 4;
            xystep = ( ( aParams->x2 - aParams->x1 ) << 4 ) * step / dy;
            
            // y is growing 
            if ( dy > 0 )
                {
                while ( aParams->curry < aParams->y2 )
                    {
                    aParams->currx = currxy >> 4;
                    aParams->currd = currd >> 4;
                    DrawShortSegment( aParams );
                    currxy += xystep;
                    currd += dstep;
                    aParams->curry += step;
                    }
                }
            else
                {
                // y is decreasing 
                while ( aParams->curry > aParams->y2 )
                    {
                    aParams->currx = currxy >> 4;
                    aParams->currd = currd >> 4;
                    DrawShortSegment( aParams );
                    currxy += xystep;
                    currd += dstep;
                    aParams->curry += step;
                    }
                }
            return;
            }
        }
    // Draw end point
    aParams->currx = aParams->x2;
    aParams->curry = aParams->y2;
    aParams->currd = aParams->d2;
    
    DrawShortSegment( aParams );
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputPenTraceDecorator::CPeninputPenTraceDecorator()
    : iPenTraceColor( 0, 0, 0 ), iBufferSize( 0, 0 ) 
    , iModified( EFalse ), iEnabled( EFalse )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::ConstructL()
    {

    }

// ---------------------------------------------------------------------------
// Release decoration buffer.
// ---------------------------------------------------------------------------
//
void CPeninputPenTraceDecorator::ReleaseBuffer()
    {
    if ( iBitsBitmap ) 
        {
        delete iBitsBitmap;
        iBitsBitmap = NULL;
        }
    
    if ( iMaskBitmap )
        {
        delete iMaskBitmap;
        iMaskBitmap = NULL;
        }
    if ( iXSquareArray )
        {
        delete iXSquareArray;
        iXSquareArray = NULL;
        }    
    }

// End Of File

