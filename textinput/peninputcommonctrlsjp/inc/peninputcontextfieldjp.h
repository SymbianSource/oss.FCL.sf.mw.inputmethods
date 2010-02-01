/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of input context field for japanese.
*
*/


#ifndef C_CFEPINPUTCONTEXTFIELDJP_H
#define C_CFEPINPUTCONTEXTFIELDJP_H


#include <peninputinputcontextfield.h>
#include <AknPictographDrawerInterface.h>
#include <AknPictographInterface.h>
#include <coecntrl.h>

/**
 *  Class for Edwin-like editing area
 *  CFepInputContextFieldJp
 *
 *  @lib ?library
 *  @lib peninputlayoutcontrol.lib
 *  @since S60 v3.2
 */
class CFepInputContextFieldJp : public CFepInputContextField,
                                public MAknPictographAnimatorCallBack
    {

public:

    /**  The conversion state in input context field */
    enum TTransitoryInputAreaState
        {
        ETransitoryInputAreaStandby,
        ETransitoryInputAreaNextSegmentWait,
        ETransitoryInputAreaConverting,
        ETransitoryInputAreaSwitching
        };

    /**  The drawing colors */
    enum TContextFieldDrawColorJp
        {
        EFirstTargetColors,
        ETransitoryInputAreaColors
        };

    /**  The input context field error codes */
    enum TInputContextFieldJpErrorCode
        {
        EStartInlineAlready = -3000,
        EUpdateInlineNotReady,
        ECommitInlineNotReady,
        ESetCursorPositionNotReady,
        EDoUpdateInlineNotReady
        };

    /**  In this class, the three offsets beside the above-mentioned is used.
     *   The top left position of app editor becomes a standard
     *   ( E ).
     *   The line head position on app editor is iRelativePos
     *   ( RO ).
     *   The iDisplayText's top position on iRelativePos is iDisplayTextOffset
     *   ( DTO ).
     *   The iConversionArea's top position on app editor is iConversionOffset
     *   ( CO ).
     */

public:  // Methods

    /**
     * Static constructor.
     *
     * @since S60 v3.2
     * @param aRect Control area
     * @param aUiLayout The layout which contains this control
     * @param aControlId The control Id
     * @return An instance of CFepInputContextFieldJp
     */
    IMPORT_C static CFepInputContextFieldJp* NewL( TRect aRect,
                                                   CFepUiLayout* aUiLayout,
                                                   TInt aControlId );

    // Constructors and destructor
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    IMPORT_C virtual ~CFepInputContextFieldJp();

    /**
     * Sets the text to the editor
     *
     * @since S60 v3.2
     * @param aData The text data
     */
    IMPORT_C void SetTextL( const TFepInputContextFieldData& aData );

    /**
     * Starting of transitory input
     *
     * @since S60 v3.2
     */
    IMPORT_C void StartInlineL();

    /**
     * Updating of transitory input area, and Inserting of one
     *
     * @since S60 v3.2
     * @param aText          The conversion text data
     * @param aDivisionPoint The division point in conversion text
     *                       or the cursor position in conversion text
     *                       (offset = iConversionOffset)
     * @param aState         The state of TTransitoryInputAreaState
     */
    // Replace
    IMPORT_C void UpdateInlineL( TDesC& aText, 
                                 const TInt aDivisionPoint,
                                 const TTransitoryInputAreaState aState
                                    = ETransitoryInputAreaConverting );

    /**
     * Commiting of first target of transitory input area
     *
     * @since S60 v3.2
     */
    IMPORT_C void CommitInlineL();

    /**
     * commiting of all characters of transitory input area
     *
     * @since S60 v3.2
     */
    IMPORT_C void CompleteInlineL();

    /**
     * Forced termination or cancel of transitory input
     *
     * @since S60 v3.2
     */
    IMPORT_C void CancelInlineL();

    /**
     * Set cursor position only transitory input state
     *
     * @since S60 v3.2
     * @param aPosition The position ( CO )
     */
    IMPORT_C void SetCursorPosition( const TInt aPosition );

    /**
     * Return one character before cursor position
     * only non transitory input state
     *
     * @since S60 v3.2
     * @return The character on cursor
     */
    IMPORT_C TChar CharacterOnCursor();

    /**
     * Return one character before cursor position
     * only non transitory input state
     *
     * @since S60 v3.2
     * @return The character on cursor
     */
    IMPORT_C TChar CharacterAroundCursor( const TInt aMovingIndex = 0 );

    /**
     * Return the cursor position
     *
     * @since S60 v3.2
     * @return The cursor position ( CO )
     */
    IMPORT_C TInt CursorPosition() const;

    /**
     * Return the dividing position
     *
     * @since S60 v3.2
     * @return The dividing position ( CO )
     */
    IMPORT_C TInt DivisionPoint() const;

private:  // Methods

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aRect The control area
     * @param aUiLayout The layout which contains this control
     * @param aControlId control Id
     */
    CFepInputContextFieldJp( TRect aRect,
                             CFepUiLayout* aUiLayout,
                             TInt aControlId );

    /**
     * 2nd phase constructor.
     *
     * @since S60 v3.2
     */
    void ConstructL();

    /**
     * Draw selected text
     *
     * @since S60 v3.2
     */
    void DrawConversion();

    /**
     * Draw selected text
     *
     * @since S60 v3.2
     * @param aStartPos   The start position for drawing area ( DTO )
     * @param aEndPos     The end position for drawing area ( DTO )
     * @param aConversion drawing state of TContextFieldDrawColorJp
     */
    void DoDrawConversion( const TInt aStartPos,
                           const TInt aEndPos,
                           const TContextFieldDrawColorJp aConversion );

    /**
     * Update transitory input area in japanese common
     *
     * @since S60 v3.2
     * @param aCursorPosition The cursor position ( E )
     * @param aDivisionPoint  The division point in conversion text ( CO )
     */
    void DoUpdateInline( const TInt aCursorPosition,
                         const TInt aDivisionPoint );

    /**
     * Reculculate offset of input context field for japanese,
     * when cursor position move.
     *
     * @since S60 v3.2
     */
    void CalculateDisplayTransitoryText();

    /**
     * Calculate width in pixels of selecting area in iBuffer.
     *
     * @since S60 v3.2
     * @param aStartPos The position of selecting start in iBuffer.
     * @param aEndPos   The position of selecting end in iBuffer.
     * @return The width in pixels of selecting area in iBuffer.
     */
    TInt BufferWidthInPixels( TInt aStartPos, TInt aEndPos ) const;

    /**
     * Set parent member data.
     *
     * @since S60 v3.2
     * @param aLineHead      The position of line head.
     * @param aDisplayOffset The position of display offset in Editor.
     */
    void SetDisplayData( TInt aLineHead, TInt aDisplayOffset );

    /**
     * Reset parent member data.
     *
     * @since S60 v3.2
     * @param aDrawFlag  The flag whether draw or not.
     */
    void ResetInputContextFieldDataL( TBool aDrawFlag );

    /**
     * Reset member data.
     *
     * @since S60 v3.2
     * @param aState    The state of transitory input area.
     * @param aPosition The position of display offset in Editor.
     */
    void ResetConversion( TTransitoryInputAreaState aState, TInt aPosition );

    /**
     * The argument returns whether it is the right side from transitory 
     * input area.
     *
     * @since S60 v3.2
     * @param aX The x coordinates position of pointer.
     * @return The argument returns whether it is the right side from 
     * transitory input area.
     */
    TBool IsRightFromConversion( TInt aX ) const;

// from base class CFepUiBaseCtrl

    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 v3.2
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */
    CFepUiBaseCtrl* HandlePointerUpEventL( const TPoint& aPoint );

    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 v3.2
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */
    CFepUiBaseCtrl* HandlePointerDownEventL( const TPoint& aPoint );

    /**
     * From CFepUiBaseCtrl
     * Handle pointer move event
     *
     * @since S60 v3.2
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */
    CFepUiBaseCtrl* HandlePointerMoveEventL( const TPoint& aPoint );

// from base class CFepLayoutEditAreaBase

    /**
     * From CFepLayoutEditAreaBase
     * Draws the text buffer into the given graphic context
     *
     * @since S60 v3.2
     * @param aGc The graphic context
     * @param aRect Invalid rectangle. Edit area is redrawn only if
     * the invalid rectangle intersects with the rectangle of the edit area
     */
    void DrawContent( CBitmapContext* aGc, const TRect& aRect );

// from base class MAknPictographAnimatorCallBack
    void DrawPictographArea();

// for pictograph
    static TInt StaticPictographCallBack( TAny* aPtr );
    void PictographCallBack();
    void SetPictographCallBackL( TCallBack& aCallBack );

private: // data

    /**
     * The offset of conversion text of contextfield ( E )
     */
    TInt iConversionOffset;

    /**
     * The dividing point ( CO )
     */
    TInt iDivisionPoint;

    /**
     * The iDisplayTextOffset before conversing ( RO )
     */
    TInt iPreDisplayTextOffset;

    /**
     * Conversing area ( E )
     */
    TCursorSelection iConversionArea;

    /**
     * The state of conversion
     */
    TTransitoryInputAreaState iConvertingState;

    /**
     * The input context field data before conversing
     */
	TFepInputContextFieldData iPreConversionData;

    /**
     * The flag for all complete of transitory input area.
     */
    TBool iAllCompleteFlag;

    /**
     * The iBuffer before conversing
     * Own.
     */
	HBufC* iPreBuffer;

    /**
     * for Pictograph
     */
    CAknPictographInterface*    iPictoInterface;
    TCallBack                   iPictoCallBack;
    CCoeControl*                iDummyControl;
    };

#endif // C_CFEPINPUTCONTEXTFIELDJP_H

// End of File