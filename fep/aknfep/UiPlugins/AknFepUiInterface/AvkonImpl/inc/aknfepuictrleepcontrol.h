/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
 *      Provides the CAknFepUICtrlEEPControl definition 
 *
*/












#ifndef C_AKNFEPUICTRLEEPCONTROL_H
#define C_AKNFEPUICTRLEEPCONTROL_H

#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

#include "insertionpoint.h"

class CAknFepUiLayoutDataMgr;
/**
 * The editable entry pane control class.
 * 
 * @lib aknfepuiavkonplugin.dll
 * @since S60 v5.0
 */
const TInt KLROffset = 5;
const TInt KCursorResetOffset = 20;
const TInt KCursorResetStartOffset = 85;
const TInt KDenominator = 100;

class CAknFepUICtrlEEPControl : public CCoeControl, public MRefreshObserver
    {
public:
    /**
     * The display style for the text.
     */
    enum TDspStyle
        {
        ENomal,
        EUnderline,
        EHighlight,
        EUnderlineHighlight
        };
    
    class TSegment
        {
    public:
        /**
         * C++ constructor.
         * 
         * @since S60 v5.0
         * @param None.
         * @return self object.
         */
        TSegment( TInt aBeg, TInt aEnd, TDspStyle aStyle );
        
        /**
         * Compare the Index
         * 
         * @since S60 v5.0
         * 
         * @param s1 the TSegment object.
         * @param s2 the TSegment object.
         * @return index 
         */
        static TInt
            CompareByBegIndex( const TSegment& s1, const TSegment& s2 );
        TInt iBegin;
        TInt iEnd;
        TDspStyle iStyle;
        };
    
    /**
     * NewL function.
     * 
     * @since S60 v5.0
     * @param aParent. the RWindow object.
     * @return self object.
     */
    static CAknFepUICtrlEEPControl* NewL( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * NewLC function.
     * 
     * @since S60 v5.0
     * @param aParent. the RWindow object.
     * @return self object.
     */
    static CAknFepUICtrlEEPControl* NewLC( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr );


    /**
     * Cancel and destroy.
     * 
     * @since S60 v5.0
     * 
     * @return none.
     */
    virtual ~CAknFepUICtrlEEPControl( );

    /**
     * From CCoeControl
     * Draw the control.
     * 
     * @since S60 v5.0
     * @param aRect need to draw rect.
     * @return None.
     */
    virtual void Draw( const TRect& aRect ) const;

    /**
     * From CCoeControl
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    virtual void SizeChanged( );

    /**
     * From MRefreshObserver
     * Called by the cursor object.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void Refresh();

    /**
     * Set the visual text and store it into member data buffer.
     * 
     * @since S60 v5.0
     * @param aDes. The buffer you want to display.
     * @return None.
     */
    void SetText( const TDesC& aDes );

    /**
     * Set the cursor position in the visual text rect.
     * 
     * @since S60 v5.0
     * @param aIndex. it represent the cursor should appear in front of aIndex character.
     * @return. error code. 0 for success.
     */
    TInt SetCursorPosition( TInt aIndex );

    /**
     * Get the cursor position in visual text.
     * 
     * @since S60 v5.0
     * @param None.
     * @return. The cursor position.
     */
    TInt GetCursorPosition( );

    /**
     * Set the underlined text beginning index and ending index.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index for underlined text.
     * @param aEnd. the ending index for underlined text.
     * @return. error code, 0 for success.
     */
    TInt SetUnderline( TInt aBeg, TInt aEnd );

    /**
     * Set the highlighted text beginning index and ending index.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index for highlighted text.
     * @param aEnd. the ending index for highlighted text.
     * @param aColor. the color of highlighted text, KRgbRed by default.
     * @return. error code, 0 for success.
     */
    TInt SetHighlight( TInt aBeg, TInt aEnd );
    
    /**
     * Add a TSegment into iArray to identify the segment with a specified style.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index.
     * @param aEnd. the ending index.
     * @param aType. the specified style.
     */
    void SetSegmentL ( TInt aBeg, TInt aEnd, TDspStyle aStyle );
    
    /**
     * Add a TSegment into iArray to identify the segment with a specified style.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index.
     * @param aEnd. the ending index.
     * @param aType. the specified style.
     */
    void SetSegment ( TInt aBeg, TInt aEnd, TDspStyle aStyle );

    /**
     * Active the cursor,the cursor will be appeared with a glint.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void ActiveCursor( );

    /**
     * Deactive the cursor, the cursor will be gray and with no glint.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void DeactiveCursor( );
    
    /**
     * Enable the cursor.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void EnableCursor( );

    /**
     * Disable the cursor.
     *  
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void DisableCursor( );

    /**
     * Move cursor to the left by a specified stride.
     * 
     * @since S60 v5.0
     * @param aStride. specify the stride.
     * @return. error code, 0 for success.
     */
    TInt MoveCursorLeft( TInt aStride );

    /**
     * Move cursor to the right by a specified stride.
     * 
     * @since S60 v5.0
     * @param aStride. specify the stride.
     * @return. error code, 0 for success.
     */
    TInt MoveCursorRight( TInt aStride );

    /**
     * Reset all its member data to initialized state.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void ResetAll( );

private:

    /**
     * C++ constructor.
     * 
     * @since S60 v5.0
     * @param None.
     */
    CAknFepUICtrlEEPControl(CAknFepUiLayoutDataMgr* aLafDataMgr);

    /**
     * Two-phrase constructor.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void ConstructL( RWindow& aParent );

    /**
     * Calculate the frame rect.
     * 
     * @since S60 v5.0
     * @param aOuterRect. a output param for storing the outer rect.
     * @param aInnerRect. a output param for storing the inner rect.
     * @return None.
     */
    void CalculateFrameRects( TRect& aOuterRect, TRect& aInnerRect ) const;

private:
    //Data.

    /**
     * Laf data manager.
     */
	CAknFepUiLayoutDataMgr* iLafDataMgr;
	
    /**
     * Store visual text.
     * Own
     */
    HBufC* iBuffer;
    
    /**
     * Whether cursor should be drawn on the screen.
     */
    TBool isCursorDraw;
    
    /**
     * The cursor pointer.
     * Own
     */
    CInsertionPoint* iInsertionPoint;
    
    /**
     * The cursor position in visual text.
     */
    TInt iCursorPosition;
    
    /**
     * the color for highlighted text.
     */
    TRgb iHighlightColor;
    
    /**
     * The color for non-highlighted text.
     */
    TRgb iDefaultTextColor;
    
    /**
     * Distinguish different segment with a different format to its context.
     */
    RPointerArray<TSegment> iArray;
    };

#include "aknfepuictrleepcontrol.inl"

#endif /* C_AKNFEPUICTRLEEPCONTROL_H */

// End of file
