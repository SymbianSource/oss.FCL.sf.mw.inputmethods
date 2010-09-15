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












#include <eikappui.h>
#include <eikenv.h> 
#include <AknUtils.h> 
#include <AknBidiTextUtils.h> //AknBidiTextUtils 
#include <bidivisual.h> //TBidiLogicalToVisual
#include <AknsSkinInstance.h> 
#include <AknsDrawUtils.h> 
#include <skinlayout.cdl.h>
#include "aknlayoutscalable_avkon.cdl.h"

#include "aknfepuictrleepcontrol.h"
#include "insertionpoint.h"
#include "aknfepuilayoutdatamgr.h"
#include "aknfepchineseuidataconv.h"

const TInt KMaxDispNum = 100;

// ---------------------------------------------------------
// NewL function.
// ---------------------------------------------------------
//
CAknFepUICtrlEEPControl* CAknFepUICtrlEEPControl::NewL( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CAknFepUICtrlEEPControl* self= CAknFepUICtrlEEPControl::NewLC( aParent, aLafDataMgr );
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------
// NewLC function.
// ---------------------------------------------------------
//
CAknFepUICtrlEEPControl* CAknFepUICtrlEEPControl::NewLC( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CAknFepUICtrlEEPControl* self = new ( ELeave ) CAknFepUICtrlEEPControl(aLafDataMgr);
    CleanupStack::PushL( self ) ;
    self->ConstructL( aParent );
    return self;
    }

// ---------------------------------------------------------
// Cancel and destroy.
// ---------------------------------------------------------
//
CAknFepUICtrlEEPControl::~CAknFepUICtrlEEPControl( )
    {
    if ( iBuffer )
        {
        delete iBuffer;
        iBuffer = NULL;
        }
    if ( iInsertionPoint )
        {
        delete iInsertionPoint;
        iInsertionPoint = NULL;
        }
    iArray.ResetAndDestroy( );
    }

// ---------------------------------------------------------
// Draw the control.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::Draw( const TRect& /*aRect*/ ) const
    {
    CWindowGc& gc = SystemGc( );
    TAknTextLineLayout textPaneTextLayout;
    gc.Clear( Rect( ) ) ;
    gc.SetPenStyle( CGraphicsContext::ESolidPen ) ;
    TInt fontWidth = 0;
    TRect editRectTmp;
    MAknsSkinInstance* skin = AknsUtils::SkinInstance( );
    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects( outerRect, innerRect ) ;

    AknsDrawUtils::DrawFrame( skin, gc, outerRect, innerRect,
        KAknsIIDQsnFrPopupSub, KAknsIIDQsnFrPopupCenterSubmenu ) ;

    TRect textrect;
    textrect.iTl = Rect().iTl + TPoint( Rect().Width( )*KLROffset/KDenominator, 3 ) ;
    textrect.SetHeight( Rect().Height( ) ) ;
    textrect.SetWidth( Rect().Width( )- Rect().Width( )*KLROffset/KDenominator * 2 ) ;

        TAknLayoutText layoutText;
        TAknTextLineLayout textLayout = 
                    CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
        layoutText.LayoutText( textrect, textLayout ) ;
        const CFont* font = layoutText.Font( );
        gc.UseFont( font ) ;
    TInt baseLine = font->AscentInPixels( )/2 + Rect().Height( )/2 - 2;

    TInt arrayCount = iArray.Count( );
    TInt beg = 0;
    TInt end = iBuffer->Length( ) - 1;
    
    for ( TInt i = 0; i < arrayCount; i++ )
        {
        TSegment* tmp = iArray[i];
        fontWidth = font->TextWidthInPixels( iBuffer->Left( beg ) ) ;
        editRectTmp = textrect;
        editRectTmp.iTl.iX = textrect.iTl.iX + fontWidth;
        gc.SetPenColor( iDefaultTextColor ) ;
        gc.SetUnderlineStyle( EUnderlineOff ) ;
        gc.DrawText( iBuffer->Mid( beg, tmp->iBegin - beg ) , editRectTmp,
            baseLine ) ;
        fontWidth = font->TextWidthInPixels( iBuffer->Left( tmp->iBegin ) ) ;
        editRectTmp = textrect;
        editRectTmp.iTl.iX = textrect.iTl.iX + fontWidth;
        if ( tmp->iStyle == EHighlight )
            {
            gc.SetPenColor( iHighlightColor ) ;
            }
        else if ( tmp->iStyle == EUnderline )
            {
            gc.SetUnderlineStyle( EUnderlineOn ) ;
            }
        else if ( tmp->iStyle == EUnderlineHighlight )
            {
            gc.SetPenColor( iHighlightColor ) ;
            gc.SetUnderlineStyle( EUnderlineOn ) ;
            }
        gc.DrawText(
            iBuffer->Mid( tmp->iBegin, tmp->iEnd - tmp->iBegin + 1 ) ,
            editRectTmp, baseLine ) ;
        beg = tmp->iEnd + 1;
        }
    gc.SetPenColor( iDefaultTextColor ) ;
    gc.SetUnderlineStyle( EUnderlineOff ) ;
    fontWidth = font->TextWidthInPixels( iBuffer->Left( beg ) ) ;
    editRectTmp = textrect;
    editRectTmp.iTl.iX = textrect.iTl.iX + fontWidth;
    gc.DrawText( iBuffer->Right( end - beg + 1 ) , editRectTmp, baseLine ) ;
    gc.DiscardFont( );
    
    if ( isCursorDraw )
        {
        iInsertionPoint->SetTextRect( textrect ) ;
        //iInsertionPoint->SetHeight( font->HeightInPixels( ) + 4 ) ;
        iInsertionPoint->SetHeight();
        iInsertionPoint->Draw( gc ) ;
        }
    }

// ---------------------------------------------------------
// From CCoeControl.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::SizeChanged( )
    {
    }

// ---------------------------------------------------------
// Set the visual text and store it into member data buffer.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::SetText( const TDesC& aDes )
    {
    ResetAll( );
    * (this->iBuffer ) = aDes;
    }

// ---------------------------------------------------------
// Set the cursor position in the visual text rect.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlEEPControl::SetCursorPosition( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex> iBuffer->Length() + 1 )
        {
        return -1;
        }
    else
        {
        iCursorPosition = aIndex;
        TInt offSetInPixel = 0;
        TAknTextLineLayout textPaneTextLayout = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
        const CFont* font = AknLayoutUtils::FontFromId ( textPaneTextLayout.iFont,
            NULL );
        offSetInPixel = font->TextWidthInPixels ( iBuffer->Left ( iCursorPosition ) );
        this->iInsertionPoint->SetPosition ( offSetInPixel );
        return 0;
        }
    }

// ---------------------------------------------------------
// Get the cursor position in visual text.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlEEPControl::GetCursorPosition( )
    {
    return iCursorPosition;
    }

// ---------------------------------------------------------
// Set the underlined text beginning index and ending index.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlEEPControl::SetUnderline( TInt aBeg, TInt aEnd )
    {
    if ( aBeg> aEnd 
    		|| ( aBeg < 0 || aBeg> iBuffer->Length() )
    		|| ( aEnd < 0 || aEnd> iBuffer->Length() ))
        {
        return -1;
        }
    else
        {
        SetSegment ( aBeg, aEnd, EUnderline );
        return 0;
        }
    }

// ---------------------------------------------------------
// Set the highlighted text beginning index and ending index.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlEEPControl::SetHighlight( TInt aBeg, TInt aEnd )
    {
    if ( aBeg> aEnd 
    		|| ( aBeg < 0 || aBeg> iBuffer->Length() )
    		|| ( aEnd < 0 || aEnd> iBuffer->Length() ))
        {
        return -1;
        }
    else
        {
        SetSegment ( aBeg, aEnd, EHighlight );
        return 0;
        }
    }

// ---------------------------------------------------------
// Add a TSegment into iArray to identify the segment with a specified style.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::SetSegmentL ( TInt aBeg, TInt aEnd, TDspStyle aStyle )
    {
    TInt left = aBeg;
    TInt right = aEnd;
    TSegment* tmp;
    TSegment* newSeg;
    RPointerArray<TSegment> newArray;
    TBool consumed = EFalse;
    
    if ( iArray.Count() == 0 )
        {
        tmp = new ( ELeave )TSegment ( aBeg, aEnd, aStyle );
        TLinearOrder<TSegment> orderByBegIndex(TSegment::CompareByBegIndex);
        iArray.InsertInOrder ( tmp, orderByBegIndex );
        consumed = ETrue;
        return;
        }
    
    for ( TInt i = 0; i < iArray.Count(); i++ )
        {
        tmp = iArray[i];
        if ( left > tmp->iBegin && left < tmp->iEnd 
            && right > tmp->iEnd )
            {
            if ( aStyle == tmp->iStyle )
                {
                left = tmp->iEnd + 1;
                }
            else
                {
                TInt tmpIndex = tmp->iEnd;
                tmp->iEnd = left - 1;
                newSeg = new ( ELeave ) TSegment ( left, tmpIndex, EUnderlineHighlight );
                CleanupStack::PushL( newSeg );
                newArray.AppendL( newSeg );
                CleanupStack::Pop( newSeg );
                left = tmpIndex + 1;
                }
            }
        else if ( left < tmp->iBegin
            && right > tmp->iBegin && right < tmp->iEnd )
            {
            if ( aStyle == tmp->iStyle )
                {
                tmp->iBegin = left;
                consumed = ETrue;
                break;
                }
            else
                {
                newSeg = new ( ELeave ) TSegment ( left, tmp->iBegin - 1, aStyle );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                newSeg = new ( ELeave ) TSegment ( tmp->iBegin, right, EUnderlineHighlight );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                tmp->iBegin = right + 1;
                consumed = ETrue;
                break;
                }
            }
        else if ( left > tmp->iBegin && left < tmp->iEnd 
            && right > tmp->iBegin && right < tmp->iEnd )
            {
            if ( aStyle == tmp->iStyle )
                {
                consumed = ETrue;
                break;
                }
            else
                {
                newSeg = new ( ELeave ) TSegment ( tmp->iBegin, left - 1, tmp->iStyle );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                newSeg = new ( ELeave ) TSegment ( left, right, EUnderlineHighlight );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                tmp->iBegin = right + 1;
                consumed = ETrue;
                break;
                }
            }
        else if ( left < tmp->iBegin && right > tmp->iEnd )
            {
            if ( aStyle == tmp->iStyle )
                {
                tmp->iBegin = left;
                left = tmp->iEnd + 1;
                }
            else
                {
                newSeg = new ( ELeave ) TSegment ( left, tmp->iBegin - 1, aStyle );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                tmp->iStyle = EUnderlineHighlight;
                left = tmp->iEnd + 1;
                }
            }
        else if ( left < tmp->iBegin && right < tmp->iBegin )
            {
            newSeg = new ( ELeave ) TSegment ( left, right, aStyle );
            CleanupStack::PushL(newSeg);
            newArray.AppendL( newSeg );
            CleanupStack::Pop(newSeg);
            consumed = ETrue;
            break;
            }
        else if ( left > tmp->iEnd && right > tmp->iEnd )
            {
            if ( i == ( iArray.Count() - 1 )
                || right < iArray[i+1]->iBegin )
                {
                newSeg = new ( ELeave ) TSegment ( left, right, aStyle );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                consumed = ETrue;
                break;
                }
            }
        else if (left == tmp->iBegin && right < tmp->iEnd )
        	{
        	if ( aStyle == tmp->iStyle )
        		{
        		consumed = ETrue;
        		break;
        		}
        	else
        		{
        		newSeg = new ( ELeave ) TSegment ( left, right, EUnderlineHighlight );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
        		tmp->iBegin = right + 1;
        		consumed = ETrue;
        		break;
        		}
        	}
        else if (left > tmp->iBegin && right == tmp->iEnd )
        	{
        	if ( aStyle == tmp->iStyle )
        		{
        		consumed = ETrue;
        		break;
        		}
        	else
        		{
        		newSeg = new ( ELeave ) TSegment ( left, right, EUnderlineHighlight );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
        		tmp->iEnd = left - 1;
        		consumed = ETrue;
        		break;
        		}
        	}
        else if (left == tmp->iBegin && right == tmp->iEnd )
        	{
        	if ( aStyle == tmp->iStyle )
        		{
        		consumed = ETrue;
        		break;
        		}
        	else
        		{
        		tmp->iStyle = EUnderlineHighlight;
        		consumed = ETrue;
        		break;
        		}
        	}
        else if (left == tmp->iBegin && right > tmp->iEnd )
        	{
        	if ( aStyle == tmp->iStyle )
        		{
        		left = tmp->iEnd + 1;
        		}
        	else
        		{
        		tmp->iStyle = EUnderlineHighlight;
        		left = tmp->iEnd + 1;
        		}
        	}
        else if (left < tmp->iBegin && right == tmp->iEnd )
        	{
        	if ( aStyle == tmp->iStyle )
        		{
        		tmp->iBegin = left;
        		consumed = ETrue;
        		break;
        		}
        	else
        		{
        		newSeg = new ( ELeave ) TSegment ( left, tmp->iBegin - 1, aStyle );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
        		tmp->iStyle = EUnderlineHighlight;
        		consumed = ETrue;
        		break;
        		}
        	}
        else if (left == tmp->iEnd && tmp->iBegin < left )
        	{
        	if ( aStyle == tmp->iStyle )
        		{
        		left = tmp->iEnd + 1;
        		}
        	else
        		{
        		tmp->iEnd = tmp->iEnd - 1;
                newSeg = new ( ELeave ) TSegment ( left, left, EUnderlineHighlight );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                left = left + 1;
        		}
        	}
        else if (right == tmp->iBegin && tmp->iEnd > right )
        	{
        	if ( aStyle == tmp->iStyle )
        		{
        		tmp->iBegin = left;
        		consumed = ETrue;
        		break;
        		}
        	else
        		{
        		tmp->iBegin = tmp->iBegin + 1;
                newSeg = new ( ELeave ) TSegment ( right, right, EUnderlineHighlight );
                CleanupStack::PushL(newSeg);
                newArray.AppendL( newSeg );
                CleanupStack::Pop(newSeg);
                right = right - 1;
        		}
        	}
        }
    
    if ( consumed == EFalse )
    	{
    	newSeg = new ( ELeave ) TSegment ( left, right, aStyle );
        CleanupStack::PushL(newSeg);
        newArray.AppendL( newSeg );
        CleanupStack::Pop(newSeg);
    	consumed = ETrue;
    	}
    
    for ( TInt ii = 0; ii < newArray.Count();  ii++ )
        {
        TLinearOrder<TSegment> orderByBegIndex(TSegment::CompareByBegIndex);
        TSegment* tmp1 = newArray[ii];
        TInt err = 0;
        err = iArray.InsertInOrder ( newArray[ii], orderByBegIndex );
        if ( err == KErrAlreadyExists )
        	{
        	delete tmp1;
        	}
        }
    newArray.Reset();
    
    for ( TInt ii = 0; ii < iArray.Count() - 1; )
    	{
    	if ( iArray[ii]->iStyle == iArray[ii + 1]->iStyle 
    		&& iArray[ii]->iEnd + 1 == iArray[ii + 1]->iBegin )
    		{
    		iArray[ii]->iEnd = iArray[ii + 1]->iEnd;
    		TSegment* tmp1 = iArray[ii + 1];
    		iArray.Remove(ii + 1);
    		delete tmp1;
    		}
    	else
    		{
    		ii ++;
    		}
    	}
    return;
    }

// ---------------------------------------------------------
// Add a TSegment into iArray to identify the segment with a specified style.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::SetSegment( TInt aBeg, TInt aEnd, TDspStyle aStyle )
	{
	TRAP_IGNORE( SetSegmentL( aBeg,aEnd,aStyle ) );
	}

// ---------------------------------------------------------
// Active the cursor,the cursor will be appeared with a glint.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::ActiveCursor( )
    {
    iInsertionPoint->Active( );
    }

// ---------------------------------------------------------
// Deactive the cursor, the cursor will be gray and with no glint.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::DeactiveCursor( )
    {
    iInsertionPoint->Deactive( );
    }

// ---------------------------------------------------------
// Enable the cursor.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::EnableCursor()
    {
    isCursorDraw = ETrue;
    DrawDeferred();
    }

// ---------------------------------------------------------
// Disable the cursor.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::DisableCursor()
    {
    isCursorDraw = EFalse;
    DrawDeferred();
    }

// ---------------------------------------------------------
// Move cursor to the left by a specified stride.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlEEPControl::MoveCursorLeft( TInt aStride )
    {
    TRect panerect = Rect( );
    TAknTextLineLayout textPaneTextLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
    const CFont* font = AknLayoutUtils::FontFromId(
        textPaneTextLayout.iFont, NULL ) ;
    if ( iCursorPosition - aStride >= 0 
    	&& font->TextWidthInPixels( iBuffer->Left( iCursorPosition - aStride ) ) >= panerect.Width() * KLROffset / KDenominator)
        {
        iCursorPosition = iCursorPosition - aStride;
        TInt offSetInPixel = 0;
        offSetInPixel
            = font->TextWidthInPixels( iBuffer->Left( iCursorPosition ) ) ;
        this->iInsertionPoint->SetPosition( offSetInPixel ) ;
        return 0;
        }
    else
        {
        return -1;
        }
    }

// ---------------------------------------------------------
// Move cursor to the right by a specified stride.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlEEPControl::MoveCursorRight( TInt aStride )
    {
    TRect panerect = Rect( );
    TAknTextLineLayout textPaneTextLayout = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
    const CFont* font = AknLayoutUtils::FontFromId(
        textPaneTextLayout.iFont, NULL ) ;
    TInt lenBegToCursor = font->TextWidthInPixels( iBuffer->Left( iCursorPosition + aStride ) );
    if ( iBuffer->Length( ) >= iCursorPosition + aStride 
    	&& lenBegToCursor < panerect.Width() * KCursorResetStartOffset / KDenominator
    	)
        {
        iCursorPosition = iCursorPosition + aStride;
        TInt offSetInPixel = 0;
        offSetInPixel
            = font->TextWidthInPixels( iBuffer->Left( iCursorPosition ) );
        this->iInsertionPoint->SetPosition( offSetInPixel ) ;
        return 0;
        }
    else
        {
        return -1;
        }
    }

// ---------------------------------------------------------
// Called by the cursor object.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::Refresh( )
    {
    this->DrawDeferred( );
    }

// ---------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------
//
CAknFepUICtrlEEPControl::CAknFepUICtrlEEPControl( CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    iBuffer = NULL;
    iInsertionPoint = NULL;
    iCursorPosition = 0;
    isCursorDraw = ETrue;
    iLafDataMgr = aLafDataMgr;
    }

// ---------------------------------------------------------
// Two-phrase constructor.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::ConstructL( RWindow& aParent )
    {
    SetContainerWindowL( aParent );
    this->iBuffer = HBufC::New( KMaxDispNum ) ;
    this->iInsertionPoint = CInsertionPoint::NewL( iLafDataMgr );
    this->iInsertionPoint->SetRefreshObserver( this ) ;
    iHighlightColor = KRgbRed;
    iDefaultTextColor = KRgbBlack;
    }

// ---------------------------------------------------------
// Reset all its member data to initialized state.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::ResetAll( )
    {
    iBuffer->Des().Zero( );
    iArray.ResetAndDestroy( );
    iCursorPosition = 0;
    isCursorDraw = ETrue;
    this->iInsertionPoint->SetPosition( 0 );
    }

// ---------------------------------------------------------
// Calculate the frame rect.
// ---------------------------------------------------------
//
void CAknFepUICtrlEEPControl::CalculateFrameRects( TRect& aOuterRect,
    TRect& aInnerRect ) const
    {
	aOuterRect = Rect();
	aInnerRect = aOuterRect;
	aInnerRect.Shrink( 3, 3 );
    }

// End of file
