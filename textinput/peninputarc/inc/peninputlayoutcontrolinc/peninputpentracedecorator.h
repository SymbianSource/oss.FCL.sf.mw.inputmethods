/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of virtual keyboard control
*
*/


#ifndef C_PENINPUTPENTRACEDECORATOR_H
#define C_PENINPUTPENTRACEDECORATOR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gdi.h>
#include <fbs.h>


// FORWARD DECLARATIONS

/**
 * params of draw line.
 */
typedef struct
{
    TBitmapUtil* bits;
    TBitmapUtil* mask;
    TRect rtAffected;

    TInt32* SqX;     // squares of X 
    TInt32  nW;      // width of destination 
    TInt32  nH;      // height of destination 
    TInt32  x1;      // (x of start point) << 4
    TInt32  y1;      // (y of start point) << 4
    TInt32  d1;      // (width of start point) << 3
    TInt32  x2;      // (x of end point) << 4
    TInt32  y2;      // (y of end point) << 4
    TInt32  d2;      // (width of end point) << 3
    TInt32  currx;   // (x of current point) << 4
    TInt32  curry;   // (y of current point) << 4
    TInt32  currd;   // (width of current point) << 3
    TUint8  bR;      // value of red channel 
    TUint8  bG;      // value of green channel 
    TUint8  bB;      // value of blue channel 
    TInt32  color;   // device dependent color 
    
}TDrawLineParams;


// CLASS DECLARATION

/**
 *  class CPeninputPenTraceDecorator.
 *
 *  pen trace decorator for hwr. 
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 v5.0
 */
class CPeninputPenTraceDecorator : public CBase
    {
public: 
    
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object.
     */  
    static CPeninputPenTraceDecorator* NewL();

    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object.
     */ 
    static CPeninputPenTraceDecorator* NewLC();

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */ 
    ~CPeninputPenTraceDecorator();

public:
    /**
     * draw a single point. 
     * 
     * @since S60 v5.0
     * @param aPoint coordinate of point
     * @return rect of affected area, in local coordinate
     */
    TRect DrawPoint( const TPoint& aPoint );
    
    /**
     * Draw a single line.
     * 
     * @since S60 v5.0
     * @param aPt1 start point of line, in local coordinate
     * @param aWidth1 pen width of start point
     * @param aColor1 color of start point
     * @param aData1 additional param of start point
     * @param aPt2 end point of line, in local coordinate 
     * @param aWidth2 pen width of end point
     * @param aColor2 color of end point
     * @param aData1 additional param of end point
     * @return rect of affected area, in local coordinate    
     */
    TRect DrawLine( const TPoint& aPt1, const TInt aWidth1, const TRgb& aColor1, TAny* aData1,  
                    const TPoint& aPt2, const TInt aWidth2, const TRgb& aColor2, TAny* aData2 );
    
    
    /**
     * Continue to draw a line in a series of lines. 
     * The behavior of this function may be affected by previous call.
     * 
     * @since S60 v5.0
     * @param aPt1 start point of line, in local coordinate
     * @param aPt2 end point of line, in local coordinate 
     * @return rect of affected area, in local coordinate
     */    
    TRect DrawNextLine( const TPoint& aPt1, const TPoint& aPt2  );
    
    /**
     * Clear decoration buffer.
     * 
     * @since S60 v5.0
     * @param aAlwaysErase Specifies whether to clear buffer anyway. 
     *        if EFalse, do clear only when buffer is not empty.
     * @return None
     */
    void Clear( TBool aAlwaysErase = EFalse );
    
    /**
     * Resize decoration buffer.
     * 
     * @since S60 v5.0
     * @param aSize Specifies new size of decoration buffer, in pixels
     * @param aAlwaysResize Specifies whether to resize buffer anyway.
     *        if EFalse, do resize only when new size is different from old one.  
     * @return None
     */
    void ResizeL( const TSize& aSize, TBool aAlwaysResize = EFalse );
    

    /**
     * Set display mode of decoration buffer.
     * 
     * @since S60 v5.0
     * @param aDisplayMode Specifies new display mode
     * @param aUpdateBuffer Specifies whether to update decoration buffer
     * @return None
     */    
    void SetDisplayModeL( TDisplayMode aDisplayMode, TBool aUpdateBuffer = ETrue );
    
    /**
     * Enable decorator.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void Enable();
    
    /**
     * Disable decorator.
     * 
     * @since S60 v5.0
     * @param aReleaseBuffer Specifies whether to release decoration buffer
     * @return None
     */    
    void Disable( TBool aReleaseBuffer = EFalse );
    
    /**
     * Get Enabled/Disabled state of decorator.
     * 
     * @since S60 v5.0
     * @return ETrue if decorator is enabled
     */    
    TBool Enabled();

    /**
     * Set pen trace color.
     * 
     * @since S60 v5.0
     * @param aColor Specifies the color of pen trace
     * @return None
     */    
    void SetPenTraceColor(const TRgb& aColor);
    
    /**
     * Get image channel of decoration buffer.
     * 
     * @since S60 v5.0
     * @return buffered CFbsBitmap object. Do not delete it outside! 
     */    
    CFbsBitmap* BitsBitmap();
    
    /**
     * Get mask channel of decoration buffer.
     * 
     * @since S60 v5.0
     * @return buffered CFbsBitmap object. Do not delete it outside! 
     */    
    CFbsBitmap* MaskBitmap();
    
protected: //static
    /**
     * Convert device-independent color to device-dependent color.
     * 
     * @since S60 v5.0
     * @param aDisplayMode
     * @param aColor device-independent color
     * @return device-dependent color
     */
    static TInt Rgb2DeviceColor( TDisplayMode aDisplayMode, const TRgb& aColor );
    
    /**
     * draw short line pixel by pixel.
     * 
     * @since S60 v5.0
     * @param aParams
     * @return None
     */    
    static void DrawShortSegment(TDrawLineParams* aParams);
    
    /**
     * draw line step by step.
     * 
     * @since S60 v5.0
     * @param aParams
     * @return None
     */    
    static void DrawLongSegment(TDrawLineParams* aParams);
    
private:

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */ 
    CPeninputPenTraceDecorator();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */ 
    void ConstructL();

    /**
     * Release decoration buffer.
     * 
     * @since S60 v5.0
     * @return None 
     */ 
    void ReleaseBuffer();
    
private:
    /**
     * decoration buffer, image channel.
     * Own.
     */
    CFbsBitmap* iBitsBitmap;

    /**
     * decoration buffer, mask channel.
     * Own.
     */    
    CFbsBitmap* iMaskBitmap;

    /**
     * decoration buffer, misc.
     * Own.
     */     
    TInt32* iXSquareArray;
    
    /**
     * display mode.
     */
    TDisplayMode iDisplayMode;
    
    /**
     * pen trace color.
     */
    TRgb iPenTraceColor;
    
    /**
     * viewport rect.
     */
    TRect iViewportRect;
    
    /**
     * size of decoration buffer, in pixels 
     */
    TSize iBufferSize;
    
    /**
     * last pen width. 
     */
    TInt iLastPenWidth;
    
    /**
     * count of trace in buffer.
     */
    TInt iLineCount;
    
    /**
     * flag, specifies whether the buffer has been modified.
     */
    TBool iModified;
    
    /**
     * flag, specifies whether this decorator is Enabled.
     */
    TBool iEnabled;
    };

#endif // C_PENINPUTPENTRACEDECORATOR_H
