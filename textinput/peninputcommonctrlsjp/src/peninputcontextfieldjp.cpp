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
* Description:  Implementation for CFepInputContextFieldJp
*
*/


#include <peninputlayout.h>
#include <peninputcontextfieldjp.h>
#include <peninputlayoutinsertionpoint.h>
#include <peninputcmd.h>
#include <aknenv.h>
#include <peninputeventjp.h>
#include <txtetext.h>  // CEditableText::EParagraphDelimiter

const TInt KGapLeftOfEachLine = 2;
const TInt KConversionRectMarginY = 2;
const TInt KConversionLeftMarginX = 1;
const TInt KConversionRightMarginX = 3;
const TInt KConversionSelectingMargin = 1;

#ifdef _DEBUG
_LIT(KPeninputICFJp, "Peninput ICF JP");
enum TPanicPeninputICFJp
    {
    EPanicICFJpStartInlineAlready = 1,
    EPanicICFJpCursorPositionNotReady,
    EPanicICFJpCommitInlineNotReady,
    EPanicICFJpUpdateInlineNotReady,
    EPanicICFJpDoUpdateInlineNotReady,
    EPanicICFJpEnd
    };
#endif // _DEBUG

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Check that target is between TCursorSelection.
// ---------------------------------------------------------------------------
//
TInt PositionInArea( const TInt aTargetVal, const TCursorSelection aArea )
    {
    return Max ( Min( aTargetVal, aArea.HigherPos() ), aArea.LowerPos() );
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CFepInputContextFieldJp* CFepInputContextFieldJp::NewL( TRect aRect,
                                     CFepUiLayout* aUiLayout,TInt aControlId )
    {
    CFepInputContextFieldJp* self =
        new (ELeave) CFepInputContextFieldJp( aRect, aUiLayout,aControlId );

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CFepInputContextFieldJp::~CFepInputContextFieldJp()
    {
    delete iPictoInterface;
    delete iDummyControl;
    if ( iPreBuffer )
        {
        delete iPreBuffer;
        iPreBuffer = NULL;
        }
    }

// ---------------------------------------------------------------------------
// Gets the text from the current control and puts it into
// the edit area.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextFieldJp::SetTextL(
                                       const TFepInputContextFieldData& aData )
    {

    if ( iConversionOffset >= 0 )
        {
        // Nothing is done at conversion.
        return;
        }

    if ( aData.iCmd == EPeninputICFInitial )
        {
        iPreConversionData.iRect = aData.iRect;
        iPreConversionData.iCursorVisibility = aData.iCursorVisibility;
        iPreConversionData.iCursorSelVisible = aData.iCursorSelVisible;
        }
    CFepInputContextField::SetTextL( aData );
    }

// ---------------------------------------------------------------------------
// Start only
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextFieldJp::StartInlineL()
    {
    __ASSERT_DEBUG( ( iConversionOffset < 0 ),
                    User::Panic( KPeninputICFJp, EPanicICFJpStartInlineAlready ) );

    // Set Text and Cursor and Region
    const TInt cursorLowPos = iSelectedCompositionText.LowerPos();
    const TInt linehead = iRelativePos;
    iConversionOffset = cursorLowPos;
    iConversionArea.SetSelection( iConversionOffset, iConversionOffset );

    if ( iConvertingState == ETransitoryInputAreaNextSegmentWait )
        {
        // It exists next transitory input area,
        // becase of the multi segment in transitory input area
        SetDisplayData( linehead, linehead + iPreDisplayTextOffset );
        iPreDisplayTextOffset = iDisplayTextOffset;
        }

    iPreConversionData.iCurSel = iSelectedCompositionText;
    iPreConversionData.iLength = iBuffer->Des().Length();
    iPreBuffer = iBuffer->Des().AllocL();
    iPreConversionData.iText.Set( *iPreBuffer );
    iConvertingState = ETransitoryInputAreaStandby;

    const TInt newLinehead = Max( iBuffer->Left( 
                                    cursorLowPos ).LocateReverse( 
                                      CEditableText::EParagraphDelimiter ) + 1,
                                  0 );
    if ( linehead != newLinehead )
        {
        SetDisplayData( newLinehead, newLinehead );
        }
    }


// ---------------------------------------------------------------------------
// Update always replace.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextFieldJp::UpdateInlineL( TDesC& aText,
                                const TInt aDivisionPoint,
                                const TTransitoryInputAreaState aState
                                        /*=ETransitoryInputAreaConverting*/ )
    {
    __ASSERT_DEBUG( ( iConversionOffset >= 0 ),
                    User::Panic( KPeninputICFJp, EPanicICFJpUpdateInlineNotReady ) );

    const TInt replaceLen = iConversionArea.Length() 
                            + iSelectedCompositionText.Length();
    const TInt textLen = aText.Length();
    const TInt linehead = iRelativePos;
    const TInt cursorPosition = ( aDivisionPoint < 0 )
                                ? Max( iSelectedCompositionText.LowerPos() 
                                  + textLen - replaceLen, linehead )
                                : iConversionOffset + aDivisionPoint;

    // Set Region
    iConversionArea.SetSelection( iConversionOffset + textLen, 
                                  iConversionOffset );

    // Set Text
    if ( iBuffer )
        {
        // Replace
        iBuffer->Des().Replace( iConversionOffset, replaceLen, aText );
        }
    else
        {
        // Insert
        iBuffer = HBufC::NewL( Max( replaceLen, textLen ) );
        iBuffer->Des().Copy( aText );
        }

    iConvertingState = aState;
    DoUpdateInline( cursorPosition, aDivisionPoint );
    }


// ---------------------------------------------------------------------------
// Commit
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextFieldJp::CommitInlineL()
    {
    __ASSERT_DEBUG( ( iConversionOffset >= 0 ),
                    User::Panic( KPeninputICFJp, EPanicICFJpCommitInlineNotReady ) );

    // Set next conversion starting position
    const TInt nextPosition = ( iDivisionPoint < 0 )
                            ? iConversionArea.HigherPos()
                            : iConversionArea.LowerPos() + iDivisionPoint;

    const TInt nextTransitoryInputLength = ( iDivisionPoint > 0 )
                                ? iConversionArea.Length() - iDivisionPoint
                                : 0;
    const TTransitoryInputAreaState state = ( nextTransitoryInputLength > 0 )
                                          ? ETransitoryInputAreaNextSegmentWait
                                          : ETransitoryInputAreaStandby;

    // Initialize
    iPreDisplayTextOffset = iDisplayTextOffset;
    ResetConversion( state, nextPosition );
    ResetInputContextFieldDataL( EFalse );
    }


// ---------------------------------------------------------------------------
// Complete
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextFieldJp::CompleteInlineL()
    {
    __ASSERT_DEBUG( ( iConversionOffset >= 0 ),
                    User::Panic( KPeninputICFJp, EPanicICFJpCommitInlineNotReady ) );

    // Initialize
    ResetConversion( ETransitoryInputAreaStandby, iConversionOffset );
    ResetInputContextFieldDataL( EFalse );
    }


// ---------------------------------------------------------------------------
// Cancel
// For synchronization with Edwin, Call SetTextL() after this function.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextFieldJp::CancelInlineL()
    {
    if ( iConversionOffset < 0 )
        {
        return;
        }

    ResetConversion( ETransitoryInputAreaStandby, iConversionOffset );
    ResetInputContextFieldDataL( ETrue );
    }


// ---------------------------------------------------------------------------
// Set cursor position on context field.
// aPosition is the value when the head of an unconfirmed string is 0.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextFieldJp::SetCursorPosition(
                const TInt aPosition )
    {
    __ASSERT_DEBUG( ( iConversionOffset >= 0 ),
                    User::Panic( KPeninputICFJp, 
                                 EPanicICFJpCursorPositionNotReady ) );

    DoUpdateInline( PositionInArea( aPosition + iConversionOffset,
                            iConversionArea ), iDivisionPoint );
    }

// ---------------------------------------------------------------------------
// Return one character before cursor position.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TChar CFepInputContextFieldJp::CharacterOnCursor()
    {
    const TInt cursorPos = iSelectedCompositionText.iCursorPos;

    return ( ( iBuffer && cursorPos > 0 ) && ( iConversionOffset < 0 ) )
                ? (TChar)( ( iBuffer->Mid( cursorPos - 1, 1 ) )[0] )
                : TChar( KNoChar );
    }

// ---------------------------------------------------------------------------
// Return one character before cursor position moving aMovingIndex.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TChar CFepInputContextFieldJp::CharacterAroundCursor( 
                                                    const TInt aMovingIndex )
    {
    const TInt cursorPos = iSelectedCompositionText.iCursorPos + aMovingIndex;

    return ( ( iBuffer ) 
              && ( iConversionOffset < 0 )
              && ( cursorPos > 0 ) 
              && ( cursorPos <= iBuffer->Length() ) )
                ? (TChar)( ( iBuffer->Mid( cursorPos - 1, 1 ) )[0] )
                : TChar( KNoChar );
    }

// ---------------------------------------------------------------------------
// Return cursor position
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepInputContextFieldJp::CursorPosition() const
    {
    return iSelectedCompositionText.iCursorPos - iConversionOffset;
    }

// ---------------------------------------------------------------------------
// Return dividing position
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepInputContextFieldJp::DivisionPoint() const
    {
    return iDivisionPoint;
    }

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CFepInputContextFieldJp::CFepInputContextFieldJp( TRect aRect,
                                                  CFepUiLayout* aUiLayout,
                                                  TInt aControlId ):
            CFepInputContextField(aRect,aUiLayout,aControlId),
            iConversionOffset(-1),
            iDivisionPoint(-1),
            iConversionArea( 0, 0 ),
            iConvertingState(ETransitoryInputAreaStandby)
    {
    SetControlType(ECtrlInputContextArea);
    iPreConversionData.iCmd = EPeninputICFInitial;
    iPreConversionData.iStartPos = 0;
    iPreConversionData.iMidPos = -1;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::ConstructL()
    {
    BaseConstructL();

    iDummyControl = new (ELeave) CCoeControl();
    iPictoInterface = CAknPictographInterface::NewL( *iDummyControl, *this );

    TCallBack callback( StaticPictographCallBack, this );
    SetPictographCallBackL( callback );
    }

// ---------------------------------------------------------------------------
// Draws the text conversion
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::DrawConversion()
    {
    const TInt displayOffset = iRelativePos + iDisplayTextOffset;
    const TInt lowPosition = Max( iConversionArea.LowerPos() - displayOffset, 
                                  0 );
    const TInt highPosition = Min( iConversionArea.HigherPos() - displayOffset,
                                   iDisplayText.Length() );
    const TInt divisionPosition = Max( iConversionArea.LowerPos() 
                                       + iDivisionPoint 
                                       - displayOffset, 
                                       0 );

    // Draw
    if ( iDivisionPoint > 0 && highPosition > 0 )
        {
        // Draw the conversion data.
        DoDrawConversion( lowPosition, divisionPosition,
                            EFirstTargetColors );

        // Draw the next conversion data.
        DoDrawConversion( divisionPosition, highPosition,
                            ETransitoryInputAreaColors );
        }
    else
        {
        // Draw the conversion data.
        DoDrawConversion( lowPosition, highPosition,
                            ETransitoryInputAreaColors );
        }
    }

// ---------------------------------------------------------------------------
// Draws the text conversion( main frame )
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::DoDrawConversion( const TInt aStartPos,
                                const TInt aEndPos,
                                const TContextFieldDrawColorJp aConversion )
    {
    if ( !iFont || iInDragging )
        {
        // no draw
        return;
        }

    CFbsBitGc* gc = static_cast<CFbsBitGc*>( BitGc() );
    TRect converseRect = Rect();
    const TInt baseLine = iFont->AscentInPixels()/2 + converseRect.Height()/2;

    //The conversion rectangular sets
    converseRect.iTl.iX = PositionInWindow( aStartPos ).iX;
    converseRect.iBr.iX =
                    ( PositionInWindow( aEndPos ).iX > converseRect.iBr.iX )
                    ? converseRect.iBr.iX : PositionInWindow( aEndPos ).iX;
    converseRect.iTl.iY += baseLine - ( iFont->AscentInPixels()
                        + KConversionRectMarginY );
    converseRect.iBr.iY += baseLine + iFont->DescentInPixels()
                        + KConversionRectMarginY - Rect().Height();
    const TInt conversionBaseLine = baseLine + Rect().iTl.iY
                                  - converseRect.iTl.iY;

    // draw selection by inverting colors in the selected text rectancle
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetPenSize( PenSize() );

    // Draw pen color and brush color select
    switch ( aConversion )
        {
        case EFirstTargetColors:
            {
            const TRgb KTargetBgColor     = AKN_LAF_COLOR_STATIC(210);
            gc->SetPenColor( BkColor() );
            gc->SetBrushColor( KTargetBgColor );
            }
            break;
        case ETransitoryInputAreaColors:
            {
            const TRgb KTransitoryBgColor = AKN_LAF_COLOR_STATIC(244);
            gc->SetPenColor( PenColor() );
            gc->SetBrushColor( KTransitoryBgColor );
            }
            break;
        default:
            break;
        }

    // draw text
    TPtrC ptr(iDisplayText.Mid( aStartPos, aEndPos - aStartPos ));
    gc->DrawText( ptr,
                  converseRect,
                  conversionBaseLine,
                  CGraphicsContext::ELeft );

    if (iPictoInterface)
        {
        iPictoInterface->Interface()->DrawPictographsInText(
                *gc, *iFont, ptr, converseRect,
                conversionBaseLine, CGraphicsContext::ELeft, KGapLeftOfEachLine );
        }
    }

// ---------------------------------------------------------------------------
// Reculculate offset of context field when cursor position move.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::DoUpdateInline( const TInt aCursorPosition,
                                            const TInt aDivisionPoint )
    {
    __ASSERT_DEBUG( ( iConversionOffset >= 0 ),
                    User::Panic( KPeninputICFJp, 
                                 EPanicICFJpDoUpdateInlineNotReady ) );

    // Set Cursor and Region
    SetCursorVisible( EFalse, EFalse );
    iDivisionPoint = aDivisionPoint;
    iSelectedCompositionText.SetSelection( aCursorPosition, aCursorPosition );

    CalculateDisplayTransitoryText();

    iInsertionPoint->SetPosition( PositionOfInsertionPointOnWindow() );
    if ( iDivisionPoint < 0 )
        {
        SetCursorVisible( ETrue, ETrue );
        }
    Draw();
    UpdateArea(Rect(),EFalse);
    }

// ---------------------------------------------------------------------------
// Calculate display text offset
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::CalculateDisplayTransitoryText()
    {
    if( !iBuffer || !iFont || ( iConversionOffset < 0 ) )
        {
        // Nothing is done at no-buffer or no-font or non-converting state.
        return;
        }

    // Check the cursor or dividing point.
    // These valid are on Editor, and not using any offset.
    const TInt conversionOffset = iConversionOffset;
    const TInt divisionPoint = conversionOffset + iDivisionPoint;
    const TInt conversionEnd = iConversionArea.HigherPos();
    const TInt icfWidth = iCorrectedRect.Width();
    const TInt linehead = iRelativePos;
    const TInt cursorPosition = iSelectedCompositionText.iCursorPos;

    TInt displayOffset = iRelativePos + iDisplayTextOffset;
    TInt cursorPos = cursorPosition;
    TInt endPos = cursorPos;
    TBool leftMarginX = EFalse;
    TBool rightMarginX = EFalse;
	const TInt pixels = BufferWidthInPixels( displayOffset, conversionEnd );

    if ( iDivisionPoint < 0 )
        {
        // Non conversion
        if ( cursorPos == conversionEnd )
            {
            // Append to transitory input area
            leftMarginX = ETrue;
            rightMarginX = EFalse;
            }
        else
            {
            // Insert to transitory input area
            leftMarginX = ETrue;
            rightMarginX = ( pixels > icfWidth ) ? ETrue : EFalse;
            }
        }
    else
        {
        // Conversion
        switch ( iConvertingState )
            {
            case ETransitoryInputAreaConverting:
                cursorPos = conversionOffset;
                endPos = divisionPoint;
                leftMarginX = EFalse;
                rightMarginX = EFalse;
                break;
            case ETransitoryInputAreaSwitching:
                cursorPos = divisionPoint;
                endPos = cursorPos;
                leftMarginX = ETrue;
                rightMarginX = ( pixels > icfWidth ) ? ETrue : EFalse;
                break;
            default:
                break;
            }
        }

    TInt cursorPixels = BufferWidthInPixels( displayOffset, endPos );
    const TInt rightMaxPixels = ( rightMarginX )
                                ? icfWidth
                                    - iFont->MaxNormalCharWidthInPixels()
                                    * KConversionRightMarginX
                                : icfWidth;

    // Right Check by Pixels
    while ( rightMaxPixels < cursorPixels )
        {
        // Too right
        ++displayOffset;
        cursorPixels = BufferWidthInPixels( displayOffset, endPos );
        }

    const TInt leftMinPos = ( leftMarginX )
                            ? displayOffset + KConversionLeftMarginX
                            : displayOffset;

    // Left Check by Position
    if ( cursorPos < leftMinPos )
        {
        // Too left
        displayOffset = ( leftMarginX ) ? cursorPos - KConversionLeftMarginX
                                        : cursorPos;
        }

    displayOffset = Max( displayOffset, linehead );
    TInt lineEnd = iBuffer->Mid( cursorPosition ).Locate( 
                                          CEditableText::EParagraphDelimiter );
    lineEnd = lineEnd < 0? iBuffer->Des().Length() : lineEnd + cursorPosition;

    SetDisplayData( linehead, displayOffset );
    iDisplayText.Set( iBuffer->Mid( displayOffset, lineEnd - displayOffset ) );
    }

// ---------------------------------------------------------------------------
// Calculate width in pixels of selecting area in iBuffer.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CFepInputContextFieldJp::BufferWidthInPixels( TInt aStartPos, 
                                                   TInt aEndPos ) const
    {
    const TInt length = Max( aEndPos - aStartPos, 0 );

    return iFont->TextWidthInPixels( iBuffer->Mid( aStartPos, length ) );
    }

// ---------------------------------------------------------------------------
// Set member data for display.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::SetDisplayData( TInt aLineHead, 
                                                  TInt aDisplayOffset )
    {
    const TInt cursor = iSelectedCompositionText.iCursorPos - aLineHead;

    iDisplayTextOffset = Max( aDisplayOffset - aLineHead, 0 );
    iRelativePos = aLineHead;
    iDisplayTextCurSel.SetSelection( cursor, cursor );
    }

// ---------------------------------------------------------------------------
// Reset member data of parent class.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::ResetInputContextFieldDataL( TBool aDrawFlag )
    {
    
    if ( aDrawFlag )
        {
        CFepInputContextField::SetTextL( iPreConversionData );
        }
    else
        {
        iSelectedCompositionText = iPreConversionData.iCurSel;
        TPtr ptr = iBuffer->Des();
        ptr = iPreConversionData.iText;
        SetCursorVisible( iPreConversionData.iCursorVisibility, 
                          iPreConversionData.iCursorSelVisible );
        iInsertionPoint->SetPosition( PositionOfInsertionPointOnWindow() );
        }
    delete iPreBuffer;
    iPreBuffer = NULL;
    }

// ---------------------------------------------------------------------------
// Reset member data of transitory input area.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::ResetConversion( 
                                            TTransitoryInputAreaState aState, 
                                            TInt aPosition )
    {
    iConversionOffset = -1;
    iDivisionPoint = -1;
    iConvertingState = aState;
    iConversionArea.SetSelection( aPosition, aPosition );
    }


// ---------------------------------------------------------------------------
// The argument returns whether it is the right side from transitory input 
// area.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CFepInputContextFieldJp::IsRightFromConversion( TInt aX ) const
    {
    const TInt displayOffset = iRelativePos + iDisplayTextOffset;
    const TInt xPosOffset = aX - iCorrectedRect.iTl.iX;
    const TInt highPosOffset = iConversionArea.HigherPos() - displayOffset;
    const TInt highPosPixel = iFont->TextWidthInPixels( 
                                        iDisplayText.Left( highPosOffset ) );
    const TBool pointFlag = ( xPosOffset > highPosPixel ) ? ETrue : EFalse;
    const TBool curposFlag = ( CursorPosition() == iConversionArea.Length() ) 
                             ? ETrue : EFalse;

    return ( pointFlag && curposFlag && ( iDivisionPoint < 0 ) ) 
           ? ETrue 
           : EFalse;
    }


// ---------------------------------------------------------------------------
// From class CFepUiBaseCtrl
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CFepInputContextFieldJp::HandlePointerUpEventL(
                                         const TPoint& aPoint )
    {
    if ( iConversionOffset < 0 )
        {
        // Non-conversion
        // Behave the common function
        return ( !CFepInputContextField::HandlePointerUpEventL(aPoint) ) ?
            NULL : this;
        }

    // Setting of derivation origin
    CFepUiBaseCtrl::HandlePointerUpEventL(aPoint);
    iScrolling = EFalse;

    // Does Freely pointer event
    if( iPreCaptureCtrl )
        {
        // In case of not being, it makes and does freely.
        iPreCaptureCtrl->CapturePointer();
        }
    else
        {
        // In case of being, it does freely.
        CapturePointer( EFalse );
        }

    if ( iAllCompleteFlag )
        {
        iAllCompleteFlag = IsRightFromConversion( aPoint.iX );
        }

    const TInt eventType = ( iAllCompleteFlag ) ? EEventIcfCompleteAll 
                                                : EEventIcfPointerUp;
    TInt cursorPos = PositionInDisplayText( aPoint.iX );
    SetPositionOfInsertionPointInBuffer( cursorPos );

    // selection was dragged inside visible buffer -> update selection
    TCursorSelection area = iConversionArea;
    if ( iConvertingState == ETransitoryInputAreaSwitching )
        {
        // At least one character are the state that has been selected.
        area.iAnchorPos += KConversionSelectingMargin;
        }
    const TInt newPosition
                = PositionInArea( iSelectedCompositionText.iCursorPos, area );
    const TInt pos = newPosition - iConversionOffset;
    if ( iDivisionPoint < 0 )
        {
        DoUpdateInline( newPosition,
                        ( iDivisionPoint < 0 ) ? iDivisionPoint : pos );
        }

    TPtrC position((TUint16*) &pos );
    ReportEvent( eventType, position );

    return this;
    }

// ---------------------------------------------------------------------------
// From class CFepUiBaseCtrl
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CFepInputContextFieldJp::HandlePointerDownEventL(
                                         const TPoint& aPoint )
    {
    if ( iConversionOffset < 0 )
        {
        // Non-conversion
        // Behave the common function
        return ( !CFepInputContextField::HandlePointerDownEventL(aPoint) ) ?
            NULL : this;
        }

    if ( ( iDivisionPoint >= 0 )
            && ( iConvertingState == ETransitoryInputAreaConverting ) )
        {
        // Nothing is done at converting state..
        return this;
        }

    // Conversion
    CFepUiBaseCtrl::HandlePointerDownEventL( aPoint );
    iAllCompleteFlag = IsRightFromConversion( aPoint.iX );
    iScrolling = EFalse;

    // Set cursor position
    iScrolling = EFalse;
    iPreCaptureCtrl = CapturePointer();

    return this;
    }


// ---------------------------------------------------------------------------
// From class CFepUiBaseCtrl
// Handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CFepInputContextFieldJp::HandlePointerMoveEventL(
                                         const TPoint& aPoint )
    {

    if ( iConversionOffset < 0 )
        {
        // Non-conversion
        // Behave the common function
        return ( !CFepLayoutEditAreaBase::HandlePointerMoveEventL(aPoint) ) ?
            NULL : this;
        }

    if( !PointerDown() || iDimmed || ( ( iDivisionPoint >= 0 )
                && ( iConvertingState == ETransitoryInputAreaConverting ) ) )
        {
        // Nothing is done at no-pointerdown, no-dimm, converting state..
        return this;
        }

    // Updating by the drug operation.
    const TInt displayOffset = iRelativePos + iDisplayTextOffset;
    const TInt newPos = PositionInArea( PositionInDisplayText( aPoint.iX ) 
                                        + displayOffset, 
                                        iConversionArea );
    const TBool checkOfPosition =
                    ( newPos != iSelectedCompositionText.iCursorPos )
                    ? ETrue : EFalse;

    if ( iAllCompleteFlag )
        {
        iAllCompleteFlag = IsRightFromConversion( aPoint.iX );
        }

    if( checkOfPosition )
        {
        // Updating when cursor position or dividing position changes.
        // Even if the pointer moves, the cursor position is never changed.
        TCursorSelection area = iConversionArea;
        if ( iConvertingState == ETransitoryInputAreaSwitching )
            {
            // At least one character are the state that has been selected.
            area.iAnchorPos += KConversionSelectingMargin;
            }
        const TInt dividingPoint = PositionInArea( newPos, area )
                                   - iConversionOffset;
        DoUpdateInline( newPos, ( iDivisionPoint < 0 ) 
                                ? iDivisionPoint 
                                : dividingPoint );
        }

    return this;
    }

// ---------------------------------------------------------------------------
// From class CFepLayoutEditAreaBase
// Draws the text into the given graphic context
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepInputContextFieldJp::DrawContent( CBitmapContext* aGc,
                                           const TRect& aRect )
    {
    if ( iConvertingState == ETransitoryInputAreaNextSegmentWait )
        {
        return;
        }
    if ( ( iConversionOffset < 0 ) || !iBuffer || !iFont || iInDragging )
        {
        // Non-conversion, no-buffer, no-font, no-dragging
        // Behave the common function
        CFepLayoutEditAreaBase::DrawContent( aGc, aRect );

        if (iPictoInterface)
            {
            TRect editRect = Rect();
            aGc->SetBrushStyle( CGraphicsContext::ENullBrush );
            aGc->UseFont( iFont );
            aGc->SetPenColor( iTextColor );
            const TInt baseLine = iFont->AscentInPixels()/2 + editRect.Height()/2;

            iPictoInterface->Interface()->DrawPictographsInText(
                    *aGc, *iFont, iDisplayText, editRect,
                    baseLine, CGraphicsContext::ELeft, KGapLeftOfEachLine );

            aGc->DiscardFont();
            }
        return;
        }

    TRect editRect = Rect();
    if( aRect.Intersects( editRect ) )
        {
        // draw text
        // set brush invisible so the background
        // of the edit area does not get painted white
        const TInt baseLine = iFont->AscentInPixels()/2 + editRect.Height()/2;
        aGc->SetBrushStyle( CGraphicsContext::ENullBrush );
        aGc->UseFont( iFont );
        aGc->SetPenColor( iTextColor );
        aGc->DrawText( iDisplayText,
                       editRect,
                       baseLine,
                       CGraphicsContext::ELeft,
                       KGapLeftOfEachLine );
        if (iPictoInterface)
            {
            iPictoInterface->Interface()->DrawPictographsInText(
                    *aGc, *iFont, iDisplayText, editRect,
                    baseLine, CGraphicsContext::ELeft, KGapLeftOfEachLine );
            }

        if( iConversionOffset >= 0 )
            {
            // Draw text conversion
            DrawConversion();
            }

        iInsertionPoint->Draw( aGc, ETrue );

        aGc->DiscardFont();
        }
    }

void CFepInputContextFieldJp::DrawPictographArea()
    {
    if ( iPictoCallBack.iFunction )
        {
        iPictoCallBack.CallBack();
        }
    }

void CFepInputContextFieldJp::SetPictographCallBackL( TCallBack& aCallBack )
    {
    iPictoCallBack = aCallBack;
    }

// Callback for doing a redraw when animating pictographs
TInt CFepInputContextFieldJp::StaticPictographCallBack( TAny* aPtr )
    {
    CFepInputContextFieldJp* self = static_cast<CFepInputContextFieldJp*>( aPtr );
    self->PictographCallBack();
    return KErrNone;
    }

void CFepInputContextFieldJp::PictographCallBack()
    {
    Draw();
    //UpdateArea(Rect(),EFalse);
    }

//  End of File
