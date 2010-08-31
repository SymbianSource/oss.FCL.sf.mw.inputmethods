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
* Description:  Implementation for CFepLayoutEditAreaBase
*
*/

#include <peninputcmd.h>
#include <AknBidiTextUtils.h>
#include <AknsDrawUtils.h>
#include <avkon.hrh>

#include "peninputlayouteditareabase.h"
#include "peninputlayoutinsertionpoint.h"
#include "peninputlayout.h"


// CONSTANTS
const TInt KGapLeftOfEachLine = 2;//1;
const TInt KSelectionRectMarginY = 2;
const TInt KDefaultHeight = 10;
const TInt KCursorCorrectionX = -1 * ( KCursorWidth + KCursorPosCorrectionX );
const TInt KDefaultRightMargin = 3 * KCursorWidth;
const TInt KAddBufLen = 100;
const TInt KSecretUpdateTimer = 1000000; // 1s
const TInt KSecretInstantShowTimer = 100000; // 100ms
const TInt KParagraphSeperator = 0x2029;


// ============================ MEMBER FUNCTIONS ===============================


// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::CFepLayoutEditAreaBase
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
EXPORT_C CFepLayoutEditAreaBase::CFepLayoutEditAreaBase(TRect aRect,
                                      CFepUiLayout* aUiLayout,TInt aControlId):
    CFepUiBaseCtrl(aRect,aUiLayout,aControlId),
    iCorrectedRect(aRect),iFontOwnership(EFalse)
    {    
    SetControlType(ECtrlEditAreaBase);
    iCursorVisible = ETrue;
    iCursorSelVisible = ETrue;
    
#ifdef RD_TACTILE_FEEDBACK     
    //register the area for tactile feedback
    if(aUiLayout)
    	{
    	//Advanced Tactile feedback REQ417-47932
    	if(IsKindOfControl(ECtrlTactileFeedback))
    		{
    		SetTactileFeedbackType(ETouchFeedbackSensitiveKeypad);
    		aUiLayout->RegisterFeedbackArea(reinterpret_cast<TInt>(this),aRect,ETouchFeedbackSensitiveKeypad);
    		}
    	}
            
	
#endif //RD_TACTILE_FEEDBACK   
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
EXPORT_C void CFepLayoutEditAreaBase::BaseConstructL()
    {
    CFepUiBaseCtrl::BaseConstructL();
    
    // Initialize text color
    iTextColor = KRgbBlack;
    
    // Create the caret
    iInsertionPoint = CInsertionPoint::NewL(this,BitmapDevice(),BitGc(),KDefaultHeight);
    iSecretTextTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::~CFepLayoutEditAreaBase
// Destructor
// ---------------------------------------------------------------------------

EXPORT_C CFepLayoutEditAreaBase::~CFepLayoutEditAreaBase()
    {
    delete iInsertionPoint;
    delete iBuffer;    
    delete iVisualBuffer;

    iSecretTextTimer->Cancel();
    delete iSecretTextTimer;
        
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        }    
    
    iOverlappedCtrlList.Close();
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetRect
// Sets the rectangle
// (other items were commented in a header).
// ---------------------------------------------------------------------------    
//
EXPORT_C void CFepLayoutEditAreaBase::SetRect(TRect aRect)
    {
    CFepUiBaseCtrl::SetRect(aRect);
    iCorrectedRect = aRect;
    iCorrectedRect.iBr.iX += KCursorCorrectionX;
	
	// resize the background bitmap
    if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )
	    {
	    AknIconUtils::SetSize( BackgroundBmp(), 
	    					   Rect().Size(), 
	    					   EAspectRatioNotPreserved );	    
	    }
	if( BkMaskBmp() && BkMaskBmp()->SizeInPixels() != Rect().Size() )
		{
	    AknIconUtils::SetSize( BkMaskBmp(), 
	    					   Rect().Size(), 
	    					   EAspectRatioNotPreserved );			
		}
    if (iBuffer && iFont)
        {
        TRAP_IGNORE(CalculateDisplayTextL());
        iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
        Draw();
        UpdateArea(Rect(),EFalse);
        }
    }

// ---------------------------------------------------------------------------
// Handle pointer down event
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepLayoutEditAreaBase::HandlePointerDownEventL(const TPoint& aPt)
    {
    if (iDimmed || !(iCursorVisible||iCursorSelVisible))
        {
        return NULL;
        }
        
    CFepUiBaseCtrl::HandlePointerDownEventL(aPt);
    
    // get the index of the character in render buffer where down event occurred
    TInt caretPos = PositionInVisualText(aPt.iX);

    //cancel any selection
    if(iDisplayTextCurSel.Length() && (iCursorVisible||iCursorSelVisible))
        {
        TRect rect = DrawSelection();
        UpdateArea(rect,EFalse);
        iDisplayTextCurSel.SetSelection(0,0);   
        iSelectedCompositionText.SetSelection(0,0);             
        }
    
    // set the new cursor position
    TInt newPosition = PositionInDisplayText(aPt.iX);
    SetPositionOfInsertionPointInBuffer(newPosition);   
    
    iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow()); 
    SetFocus();
    
    iScrolling = EFalse;
    
    CFepUiBaseCtrl* temp = CapturePointer();
    if (temp != this)
        {
        iPreCaptureCtrl = temp;
        }

    return this;
            
    }

TInt CFepLayoutEditAreaBase::CursorDistance(TInt aX) const
    {
    TInt cursorDistance;
    if ( iTextDirection == TBidiText::ELeftToRight )
        {
        cursorDistance = aX - iCorrectedRect.iTl.iX - KGapLeftOfEachLine;
        }
    else
        {
        cursorDistance = iCorrectedRect.iBr.iX - aX - KGapLeftOfEachLine;
        }
    
    TInt maxWidth = iCorrectedRect.Width() - KGapLeftOfEachLine;
    if(cursorDistance < 0)
        {
        cursorDistance = 0;
        }
    else 
        {
        cursorDistance = cursorDistance > maxWidth ? maxWidth : cursorDistance;
        }
    return cursorDistance;
    }

TBidiText::TDirectionality CFepLayoutEditAreaBase::CharDirection( TChar aChar ) const
    {
    TBuf<1> buf;
    buf.Zero();
    buf.Append( aChar );
    
    return TBidiText::TextDirectionality( buf );
    }

void CFepLayoutEditAreaBase::AdjustPostionInVisualText( const TInt /*cursorDistance*/, TInt& /*retVal*/ )
    {
    switch ( iTextDirection )
        {
        case TBidiText::ERightToLeft:
            break;
        case TBidiText::ELeftToRight:
        default:
            break;
        }
    
    }
    
TInt CFepLayoutEditAreaBase::PositionInVisualText(TInt aX)
    {
    TInt retVal;
        
    if(!iBuffer || !iFont)
        {
        // Edit area was empty, position is zero
        retVal = 0;
        return retVal;
        }
        
    // get the distance of the cursor from the beginning of iCorrectedRect
    TInt cursorDistance = CursorDistance(aX);
    
    // get the index of the cursor
    retVal = iFont->TextCount(iVisualText, cursorDistance);
    AdjustPostionInVisualText( cursorDistance, retVal ); 
 
    return retVal;
    }
    
TPoint CFepLayoutEditAreaBase::PositionInVisualWindow(TInt aCharacterIndex) const //visual text position in ICF
    {
    TPoint position;
   
    if(iFont)
        position.iY = iCorrectedRect.iTl.iY + (iCorrectedRect.Height() - 
                                    iFont->HeightInPixels())/2;
    else
        position.iY = iCorrectedRect.iTl.iY + (iCorrectedRect.Height()/4);

    if ( iTextDirection ==TBidiText::ELeftToRight )
        {
        if(aCharacterIndex < 0)
            {
            // index too small, set the point to the beginning of iCorrectedRect
            position.iX = iCorrectedRect.iTl.iX;
            }
        else if (aCharacterIndex > iVisualText.Length())
            {
            // index too big, set the point to the end of iCorrectedRect
            position.iX = iCorrectedRect.iBr.iX;
            }
        else
            {
            // calculate the position of the character relative to parent window
            position.iX = KGapLeftOfEachLine + iCorrectedRect.iTl.iX;    
            if(iFont)
                {
                position.iX += iFont->TextWidthInPixels(iVisualText.Left(aCharacterIndex));
                }
            }
        }
    else
        {
        if(aCharacterIndex < 0)
            {
            position.iX = iCorrectedRect.iBr.iX;
            }
        else if (aCharacterIndex > iVisualText.Length())
            {
            position.iX = iCorrectedRect.iTl.iX;
            }
        else
            {
            position.iX = iCorrectedRect.iBr.iX - KGapLeftOfEachLine;    
            if(iFont)
                {
                position.iX -= iFont->TextWidthInPixels(iVisualText.Left(aCharacterIndex));
                }
            }
        }

    return position;
    }
// ---------------------------------------------------------------------------
// Handle pointer up event
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepLayoutEditAreaBase::HandlePointerUpEventL(const TPoint& aPt)
    {
    if (iDimmed || !(iCursorVisible||iCursorSelVisible))
        {
        return NULL;
        }
    
    CFepUiBaseCtrl::HandlePointerUpEventL(aPt);
    
    iScrolling = EFalse;
    
    if(iPreCaptureCtrl)
        {
        iPreCaptureCtrl->CapturePointer();
        }
    else
        {
        CapturePointer(EFalse);   
        }

    CalculateDisplayTextL();
    iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
    Draw();
    UpdateArea(Rect(),EFalse);

    return this;    
    }
    
// ---------------------------------------------------------------------------
// Handle pointer move event
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepLayoutEditAreaBase::HandlePointerMoveEventL(const TPoint& aPt)
    {
    if( !PointerDown() || iDimmed || !(iCursorVisible||iCursorSelVisible) ) 
        return this;

	TInt newPosition = PositionInDisplayText(aPt.iX);

    TInt anchorPos = iDisplayTextCurSel.iAnchorPos - iDisplayTextOffset;

    TInt cursorPos = iDisplayTextCurSel.iCursorPos - iDisplayTextOffset;

    

    if(Rect().Contains(aPt) && newPosition == cursorPos)

        {

        return this;

        }


    iInsertionPoint->DelayCursorWhileDraging();    
    if(aPt.iX < iCorrectedRect.iTl.iX)
        {
        // selection was dragged beyond the left edge of iCorrectedRect -> scroll left
        ScrollLeft();
        } 
    else if(aPt.iX > iCorrectedRect.iBr.iX)
        {
        // selection was dragged beyond the right edge of iCorrectedRect -> scroll right
        ScrollRight();
        }
    else
        {
        // selection was dragged inside visible buffer -> update selection      
        SetPositionOfInsertionPointInBuffer(newPosition, anchorPos);

        iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
        if(iScrolling)
            {
            // text was scrolled just before user dragged the pen back inside iCorrectedRect
            // invalidate entire rect to show the new selection correctly
            Draw();
            UpdateArea(Rect(),EFalse);
            iScrolling = EFalse;
            }
        else
            {         
            if(newPosition != cursorPos )
                {
                TPoint prevPos = PositionInWindow(cursorPos);
                TPoint newPos = PositionInWindow(newPosition);

                // invalidate only changed selection, to minimize flickering
                TRect invalidRect(Rect());
                invalidRect.iTl.iX = Min(prevPos.iX, newPos.iX);
                invalidRect.iBr.iX = Max(prevPos.iX, newPos.iX) + KCursorSelectionGap;
                invalidRect.iBr.iX = invalidRect.iBr.iX > Rect().iBr.iX ? Rect().iBr.iX : invalidRect.iBr.iX;
              
                Draw();
                UpdateArea(invalidRect,EFalse);
                }
            }
        }
    return this;
    }


// ---------------------------------------------------------------------------
// Drawa edit area
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutEditAreaBase::Draw()
    {
    if(!AbleToDraw())
        return;
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    TRect rect = Rect();
	DrawOpaqueMaskBackground();
    // ----- draw bitmaps -----
	DrawBackground();
    DrawContent(gc,rect);
    }
    

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::DrawSelection()
// Draws the text selection
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TRect CFepLayoutEditAreaBase::DrawSelection()
    {
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    gc->SetClippingRegion(&ValidClipRegion());
    TRect rect = Rect();

    if (!iFont || iInDragging)
        {
        return TRect(rect.iTl, TSize(0,0));
        }
    TInt baseLine = iFont->AscentInPixels() + (rect.Height() - 
                                                iFont->HeightInPixels())/2;    
    
    TRect selRect(rect);
    TInt startIndex = iDisplayTextCurSel.LowerPos() - iDisplayTextOffset;
    TInt endIndex = iDisplayTextCurSel.HigherPos() - iDisplayTextOffset;
    if(startIndex < 0)
        {
        // set startIndex to the beginning of the buffer.
        startIndex = 0;
        }
    if(endIndex> iDisplayText.Length())
        {
        // set endIndex to the end of the buffer
        endIndex = iDisplayText.Length();
        }
    TInt pos1 = PositionInWindow(startIndex).iX;
    TInt pos2 = PositionInWindow(endIndex).iX;
    selRect.iTl.iX = Min( pos1, pos2 );
    selRect.iBr.iX = Max( pos1, pos2 );
    if(selRect.iBr.iX > rect.iBr.iX)
        {
        selRect.iBr.iX = rect.iBr.iX;
        }
    selRect.iTl.iY += ((baseLine - iFont->AscentInPixels()) - 
                                                    KSelectionRectMarginY);
    selRect.iBr.iY -= ((rect.Height()-(baseLine + iFont->DescentInPixels()))
                                             - KSelectionRectMarginY);
    selRect.Intersection( rect );
    
    // draw selection by inverting colors in the selected text rectancle
    gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc->SetBrushColor(KRgbBlack);
    gc->SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);
    gc->SetPenSize(PenSize());
    gc->SetPenColor(PenColor());
    gc->DrawRect(selRect);

    // restore normal draw mode
    gc->SetDrawMode(CGraphicsContext::EDrawModePEN);
    gc->SetBrushStyle(CGraphicsContext::ENullBrush);                
    gc->CancelClipping();        
    return selRect;
    }

TInt CFepLayoutEditAreaBase::UpdateSecretText( TAny* aEditArea )
    {
    CFepLayoutEditAreaBase* editArea = static_cast<CFepLayoutEditAreaBase*>(aEditArea);
    
    if ( !editArea->iTextIsSecret )
        {
        editArea->iSecretTextTimer->Cancel();
        return KErrNone;
        }
    TPtr ptr = editArea->iBuffer->Des();
    TInt startPos = ptr.LocateReverse( '*' ) + 1;
    TBuf<1> buf;
    buf.Append( '*' );
 
    if ( startPos < ptr.Length() )
        {
        ptr.Replace( startPos, 1, buf );
        
        if ( startPos == ptr.Length() -1 )
            {
            editArea->iSecretTextTimer->Cancel();
            }
        }
    TRAP_IGNORE(editArea->CalculateDisplayTextL());
    editArea->Draw();
    editArea->UpdateArea(editArea->Rect(),EFalse);
    return KErrNone;
    }
 
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::DrawContent
// Draws the text into the given graphic context
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutEditAreaBase::DrawContent(CBitmapContext* aGc, const TRect& aRect)
    {    

    if(!iBuffer || !iFont || iInDragging)
        {
        return;
        }
    aGc->SetClippingRegion(ValidClipRegion());
    
    TRect editRect = Rect();
    if(aRect.Intersects(editRect))
        {
        // set brush invisible so the background of the edit area does not get painted white
        aGc->SetBrushStyle(CGraphicsContext::ENullBrush);
        aGc->UseFont(iFont);
    
        // draw text
/*        TInt baseLine = iFont->AscentInPixels() + (editRect.Height()-iFont->
                                                    HeightInPixels())/2;*/
        TInt baseLine = iFont->AscentInPixels()/2 + editRect.Height()/2;
	    TRgb color( KRgbBlack );  // sane default for nonskinned case
	    if(iTextColor != KRgbRed)//temperarily to fix the vkb warning color defect.
    	    {
    	    

    	    if ( AknsUtils::AvkonSkinEnabled() )
    	        {
    	        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
    	                 color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG60 );
    	        }
            }
        else
            {
            color = iTextColor; //when color group is ready for chinese, pls delete this judgement.
            }
        aGc->SetPenColor(color);
        
        if( isCenter )
            {
            aGc->DrawText(iVisualText, editRect, baseLine,
                 CGraphicsContext::ECenter,KGapLeftOfEachLine);
            }
        else
            {
            if ( iTextDirection ==TBidiText::ELeftToRight )
                {
                aGc->DrawText(iVisualText, editRect, baseLine,
                     CGraphicsContext::ELeft,KGapLeftOfEachLine);
                }
            else
                {
                aGc->DrawText(iVisualText, editRect, baseLine,
                     CGraphicsContext::ERight,KGapLeftOfEachLine);
                }
            }
        // draw text selection (if any)
        TInt selectionLength = iDisplayTextCurSel.Length(); 
        if( selectionLength > 0 && (iCursorVisible||iCursorSelVisible) )
            {
            // calculate the bounding rectancle of the selected text
            DrawSelection();
            }
        iInsertionPoint->Draw(aGc, ETrue);

        aGc->DiscardFont();
        }
    aGc->CancelClippingRegion();
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetPositionOfInsertionPointInBuffer
// Sets the position of the cursor within the buffer, i.e.
// what is the ordinal of the character where the cursor is.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutEditAreaBase::SetPositionOfInsertionPointInBuffer(
                                        TInt& aPositionOfInsertionPointInBuffer)
    {
    SetPositionOfInsertionPointInBuffer(aPositionOfInsertionPointInBuffer, 
                                            aPositionOfInsertionPointInBuffer);
    //make sure anchor pos is ajusted
    iDisplayTextCurSel.iAnchorPos = iDisplayTextCurSel.iCursorPos;
    iSelectedCompositionText.iAnchorPos = iSelectedCompositionText.iCursorPos;
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetPositionOfInsertionPointInBuffer
// See previous. In addition adjusts the anchor position also
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutEditAreaBase::SetPositionOfInsertionPointInBuffer(
        TInt& aPositionOfInsertionPointInBuffer, TInt& aPositionOfAnchorInBuffer)
    {
    TInt lastCurPos = iDisplayTextCurSel.iCursorPos;
    
    iDisplayTextCurSel.iCursorPos = aPositionOfInsertionPointInBuffer + iDisplayTextOffset;
    iDisplayTextCurSel.iAnchorPos = aPositionOfAnchorInBuffer + iDisplayTextOffset;
    AdjustSelectedCompositionText(iDisplayTextCurSel.iCursorPos);
    
    iSelectedCompositionText.iCursorPos = iDisplayTextCurSel.iCursorPos + iRelativePos;
    iSelectedCompositionText.iAnchorPos = iDisplayTextCurSel.iAnchorPos + iRelativePos;
    
    if (lastCurPos != iDisplayTextCurSel.iCursorPos)
        {
        ReportEvent(EEventSetAppCursorSelection);
        }
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::PositionOfInsertionPointOnWindow
// Calculates coordinates where the cursor should be drawn
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TPoint CFepLayoutEditAreaBase::PositionOfInsertionPointOnWindow()
    {
    // default position, this is returned if there is no text in the buffer
    TPoint position =iCorrectedRect.iTl; 

    if(iBuffer)
        {
        TInt pos = iDisplayTextCurSel.iCursorPos - iDisplayTextOffset;
        if(pos < 0)
            {
            pos = 0;
            }
        position = PositionInWindow(pos);
        }
        
    return position;
    }


// ---------------------------------------------------------------------------
// CVkbEditArea::PositionInWindow
// Calculates the position of a character in iDisplayText
// as pixels relative to the parent window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TPoint CFepLayoutEditAreaBase::PositionInWindow(TInt aCharacterIndex)
    {
    TPoint position;
    if(iFont)
        position.iY = iCorrectedRect.iTl.iY + (iCorrectedRect.Height() - 
                                    iFont->HeightInPixels())/2;
    else
        position.iY = iCorrectedRect.iTl.iY + (iCorrectedRect.Height()/4);
    
    TInt offsetCenter = 0; 
     
    if(isCenter)   
        {
        offsetCenter = (iCorrectedRect.Width() - iFont->TextWidthInPixels(iDisplayText))/2;
        }
       
    if ( iTextDirection ==TBidiText::ELeftToRight )
        {
        if(aCharacterIndex < 0)
            {
            // index too small, set the point to the beginning of iCorrectedRect
            position.iX = iCorrectedRect.iTl.iX + offsetCenter;
            }
        else if (aCharacterIndex > iDisplayText.Length())
            {
            // index too big, set the point to the end of iCorrectedRect
            position.iX = iCorrectedRect.iBr.iX - offsetCenter;
            }
        else
            {
            // calculate the position of the character relative to parent window
            position.iX = KGapLeftOfEachLine + iCorrectedRect.iTl.iX;    
            if(iFont)
                {
                position.iX += TextWidth( 0, aCharacterIndex ) + offsetCenter;
                }
            }
        }
    else
        {
        if(aCharacterIndex < 0)
            {
            position.iX = iCorrectedRect.iBr.iX - offsetCenter;
            }
        else if (aCharacterIndex > iDisplayText.Length())
            {
            position.iX = iCorrectedRect.iTl.iX + offsetCenter;
            }
        else
            {
            position.iX = iCorrectedRect.iBr.iX - KGapLeftOfEachLine;    
            if( iFont && ( aCharacterIndex > 0) )
                {
                /*TInt count = 0;
                for ( TInt i = aCharacterIndex-1; i > 0; i-- )
                    {
                    if ( CharDirection( iDisplayText[i] ) == iTextDirection )
                        {
                        break;
                        }
                    else
                        {
                        count++;
                        }
                    }
                 position.iX -= TextWidth( 0, aCharacterIndex-count );*/
                position.iX -= TextWidth( 0, aCharacterIndex ) + offsetCenter;
                }
            }
        }

    return position;
    }

// ---------------------------------------------------------------------------
// CVkbEditArea::PositionInDisplayText
// Gets the character offset in the text buffer 
// of the given screen coordinate
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepLayoutEditAreaBase::PositionInDisplayText(TInt aX) const
    {
    TInt retVal;
        
    if(!iBuffer || !iFont)
        {
        // Edit area was empty, position is zero
        retVal = 0;
        }
    else
        {
        TInt offsetCenter = 0;
        if(isCenter)
            {
            offsetCenter = (iCorrectedRect.Width() - iFont->TextWidthInPixels(iDisplayText))/2;
            }
        
        // get the distance of the cursor from the beginning of iCorrectedRect
        TInt cursorDistance;
        
        if ( iTextDirection == TBidiText::ELeftToRight)
            {
            cursorDistance = aX - iCorrectedRect.iTl.iX - KGapLeftOfEachLine - offsetCenter;
            }
        else
            {
            cursorDistance = iCorrectedRect.iBr.iX - aX - KGapLeftOfEachLine - offsetCenter;
            }
        
        TInt maxWidth = iCorrectedRect.Width() - KGapLeftOfEachLine;
        if(cursorDistance < 0)
            {
            cursorDistance = 0;
            }
        else 
            {
            cursorDistance = cursorDistance > maxWidth ? maxWidth : cursorDistance;
            }
        // get the index of the cursor
        retVal = iFont->TextCount(iDisplayText, cursorDistance);

        /*for ( TInt i = retVal; i < iDisplayText.Length(); i++ )
            {
            if ( CharDirection( iDisplayText[i] ) == iTextDirection )
                {
                break;
                }
            else
                {
                retVal++;
                }
            }*/
        
        if ( retVal < iDisplayText.Length() )
            {
            TInt textWidth = iFont->TextWidthInPixels( iDisplayText.Mid( retVal,1 ) );
            TInt offsetWidth = cursorDistance - iFont->TextWidthInPixels(iDisplayText.Left( retVal ));
            if ( offsetWidth > textWidth/2 )
                {
                retVal++;
                }
            }
        }

    return retVal;
    }


// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetTextColor
// Sets the color of the text
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutEditAreaBase::SetTextColor(const TRgb aColor)
    {
    iTextColor = aColor;
    }

// ---------------------------------------------------------------------------
// CVkbEditArea::ScrollLeft
// Scrolls selection to the left exposing more text
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutEditAreaBase::ScrollLeft()
    {
    
    iScrolling = ETrue;
    if(iBuffer)
        {
        TInt cursorPos = iSelectedCompositionText.iCursorPos;
        TInt anchorPos = iSelectedCompositionText.iAnchorPos;
        // decrease the cursor position, the anchor remains unchanged
       /* TInt index = iDisplayTextCurSel.iCursorPos-1;
        index = ( index < 0 )? 0:index;
        index = ( index > iDisplayText.Length()-1 )? iDisplayText.Length()-1:index; 
        if ( CharDirection(iDisplayText[index]) == TBidiText::ELeftToRight )*/
        if ( iTextDirection == TBidiText::ELeftToRight )
            {
            cursorPos--;
            }
        else
            {
            cursorPos++;
            }

        UpdateContent(TCursorSelection(cursorPos, anchorPos) );

        if( (anchorPos == iSelectedCompositionText.iAnchorPos) && 
            (cursorPos == iSelectedCompositionText.iCursorPos) )
            {
            return;
            }
        else
            {
            ReportEvent(EEventSetAppCursorSelection);
            }
        }
    }
    

// ---------------------------------------------------------------------------
// CVkbEditArea::ScrollRight
// Scrolls selection to the right exposing more text
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutEditAreaBase::ScrollRight()
    {
    
    iScrolling = ETrue;
    if(iBuffer)
        {
        // increase the cursor position, the anchor remains unchanged
        TInt cursorPos = iSelectedCompositionText.iCursorPos;
        TInt anchorPos = iSelectedCompositionText.iAnchorPos;
        
        /*TInt index = iDisplayTextCurSel.iCursorPos-1;
        index = ( index < 0 )? 0:index;
        index = ( index > iDisplayText.Length()-1 )? iDisplayText.Length()-1:index; 
        if ( CharDirection(iDisplayText[index]) == TBidiText::ELeftToRight )*/
        if ( iTextDirection == TBidiText::ELeftToRight )
            {
            cursorPos++;
            }
        else
            {
            cursorPos--;
            }

        UpdateContent(TCursorSelection(cursorPos, anchorPos) );

        if( (anchorPos == iSelectedCompositionText.iAnchorPos) && 
            (cursorPos == iSelectedCompositionText.iCursorPos) )
            {
            return;
            }
        else
            {
            ReportEvent(EEventSetAppCursorSelection);
            }
        }
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::PrepareForFocusLoss
// Sets the color of the text
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutEditAreaBase::PrepareForFocusLoss()
    {
    iInsertionPoint->SetOn(EFalse);
    iHasFocus = EFalse;

    ReportEvent(EEventControlFocusLost);
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::PrepareForFocusGain
// Sets the color of the text
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepLayoutEditAreaBase::PrepareForFocusGain()
    {
     if (CursorVisible())
        {
        iInsertionPoint->SetOn(ETrue);
        }
    iHasFocus = ETrue;
    ReportEvent(EEventControlFocusGained);

    return ETrue;
    }

void CFepLayoutEditAreaBase::IncreaseBufferL(TInt aLength)
    {
	if ( !iBuffer )
        {
        iBuffer = HBufC::NewL( aLength + KAddBufLen );
        }
    else if ( iBuffer->Length() + aLength > iBuffer->Des().MaxLength() )
        {
        //delete iBuffer;
        //iBuffer = NULL;
        iBuffer = iBuffer->ReAllocL( iBuffer->Length() + aLength + KAddBufLen);
        }
    }
    
EXPORT_C void CFepLayoutEditAreaBase::SetTextAlignmentL(TInt aAlignment)
    {
    switch (aAlignment)
        {
        case EAknEditorAlignCenter:
            isCenter = ETrue;
            break;
        default:
            isCenter = EFalse;
            break;
        }
    
    }
    

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetTextL
// Sets the text to be displayed
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutEditAreaBase::SetTextL( const TFepInputContextFieldData& aData )
   {
    iSelectedCompositionText.iCursorPos = aData.iCurSel.iCursorPos;
    iSelectedCompositionText.iAnchorPos = aData.iCurSel.iAnchorPos;
    
    IncreaseBufferL( aData.iText.Length() );
    
    TPtr ptr = iBuffer->Des();    
    
    switch ( aData.iCmd )
        {
        case EPeninputICFInitial:
            {
            if ( aData.iFlag & EFepICFDataDirectionMFNE )
                {
                iMfneEditor = ETrue;
                }
            else
                {
                iMfneEditor = EFalse;
                }
            ptr = aData.iText;
            if ( aData.iFlag == EFepICFDataDirectionRTL )
                {
                iTextDirection = TBidiText::ERightToLeft;
                }
             else
                {
                iTextDirection = TBidiText::ELeftToRight;
                }
            }
            break;
        case EPeninputICFReplace:
            {
            if(aData.iStartPos > ptr.Length() || 
               aData.iStartPos + aData.iLength > ptr.Length())
                {
                return;
                }
            ptr.Replace( aData.iStartPos, aData.iLength, aData.iText );
                    
            if ( aData.iMidPos >= 0 )
                {
                //inline
                } 
           
            }
            break;
        case EPeninputICFDelete:
            {
            if(aData.iStartPos > ptr.Length())
                {
                return;
                }
            ptr.Delete( aData.iStartPos, aData.iLength );
            }
            break;
        case EPeninputICFSetCurSel:
            {
            break;
            }
        default:
            {
            return;
            }
        }
    
    UpdateText( aData );
    
    CalculateDisplayTextL();
    
    SetCursorVisible( aData.iCursorVisibility, aData.iCursorSelVisible );
    
    iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
    Draw();
    UpdateArea(Rect(),EFalse);
   }

void CFepLayoutEditAreaBase::UpdateText( const TFepInputContextFieldData& aData )
    {
    if ( !iTextIsSecret )
        {
        return;
        }
    
    TPtr ptr = iBuffer->Des();  
    
    switch( aData.iCmd )
        {
        case EPeninputICFInitial:
            {
            ptr.Fill( '*', ptr.Length() );
            }
            break;
        case EPeninputICFReplace:
            {                    
            if ( iSecretTextTimer->IsActive() )
                {
                iSecretTextTimer->Cancel();
                TBuf<1> buf;
                buf.Append( '*' );
                
                if (aData.iStartPos != 0)
                    {
                    ptr.Replace( aData.iStartPos - 1, 1, buf );
                    }
                }

            iSecretTextTimer->Start(
                aData.iText.Length()>1?KSecretInstantShowTimer:KSecretUpdateTimer,
                KSecretUpdateTimer, TCallBack( UpdateSecretText, this ) );
            }
            break;
        case EPeninputICFSetCurSel:
        default:
            {
            break;
            }
        }
    }
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::UpdateCursorSelection
// update cursor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutEditAreaBase::UpdateCursorSelection( const TCursorSelection& aCursorSel)
    {
    if( (aCursorSel.iAnchorPos == iSelectedCompositionText.iAnchorPos) && 
        (aCursorSel.iCursorPos == iSelectedCompositionText.iCursorPos) )
        {
        return;
        }
       
    iSelectedCompositionText = aCursorSel;
    
    TRAP_IGNORE(CalculateDisplayTextL());
        
    iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
    Draw();
    UpdateArea(Rect(),EFalse);
    }
       
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::CalculateDisplayTextL
// calcuate display text according to text and cursor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepLayoutEditAreaBase::CalculateDisplayTextL()   
    {
    if(!iBuffer || !iFont)
        {
        return;
        }
    
    TInt cursorPos = iSelectedCompositionText.iCursorPos;
    
    // finding a paragraph include current cursor selection 
    TInt beginPos = iBuffer->Left( cursorPos ).LocateReverse(KParagraphSeperator); 
    beginPos = beginPos < 0? 0 : beginPos + 1; 
    
    if( cursorPos > iBuffer->Length() )
        {
        return;
        }
        
    TInt endPos = iBuffer->Mid( cursorPos ).Locate(KParagraphSeperator); 
    endPos = endPos < 0? iBuffer->Des().Length() - 1 : endPos + cursorPos - 1; 

    cursorPos -= beginPos;
    iDisplayTextCurSel.iCursorPos = iSelectedCompositionText.iCursorPos - beginPos;
    iDisplayTextCurSel.iAnchorPos = iSelectedCompositionText.iAnchorPos - beginPos;
    
    iDisplayText.Set( iBuffer->Mid( beginPos, endPos - beginPos +1 ) );
    iRelativePos = beginPos;
    
    iDisplayTextOffset = 0;
    
    TInt width = iFont->TextWidthInPixels(iDisplayText);//*iBuffer
    
    // get visual text and direction
    delete iVisualBuffer;
    iVisualBuffer = NULL;
    iVisualBuffer = HBufC::NewL(iDisplayText.Length() + TBidiLogicalToVisual::KMinCharAvailable);
    //CleanupStack::PushL( iVisualBuffer );
    if ( iMfneEditor )
        {
        TPtr text = iVisualBuffer->Des();
        AknBidiTextUtils::ConvertToVisualAndClip(iDisplayText, text, *iFont, width, width);
        iVisualText.Set( text );
        }
    else
        {
        iVisualText.Set( iDisplayText );
        }
    //CleanupStack::Pop();
    
    TInt rectWidth = iCorrectedRect.Width();

    TInt partRectWidth = iCorrectedRect.Width() - 
                     Max(KDefaultRightMargin, iFont->MaxNormalCharWidthInPixels());

    TPtrC left = iDisplayText.Left(cursorPos);
    TInt widthToCursor = iFont->TextWidthInPixels(left);          
    
    TPtrC right = iDisplayText.Mid(cursorPos);
    TInt cursorToEnd = iFont->TextWidthInPixels(right);   

    if (widthToCursor > rectWidth/2 && cursorToEnd > rectWidth/2)
        {
        TInt newPos = widthToCursor - (rectWidth/2);
        
        TInt from = iFont->TextCount(iDisplayText, newPos);
        iDisplayTextOffset = from;
        iDisplayText.Set(iDisplayText.Mid(iDisplayTextOffset));
        }
    else if (widthToCursor > rectWidth/2 && cursorToEnd <= rectWidth/2)
        {
        TInt newPos = width - partRectWidth;
        if(newPos < 0)
            {
            newPos = 0;                
            }

        // find the index of the first character
        TInt from = iFont->TextCount(iDisplayText, newPos);//*iBuffer
        TInt usedWidth = iFont->TextWidthInPixels(iDisplayText.Left(from));//iBuffer
    
        //if there are half char not displayable (usedWidth < newPos)
        //or if usedWidth == newPos but there are chars before, increase "from"
        //also because of gap before chars
        if (usedWidth <= newPos && newPos != 0)
            {
            from ++;
            }

        iDisplayTextOffset = from;
        iDisplayText.Set(iDisplayText.Mid(iDisplayTextOffset));
        }
    
    if ( iTextDirection == TBidiText::ELeftToRight )
        {
        iVisualText.Set(iVisualText.Mid(iDisplayTextOffset));
        }
    else
        {
        iVisualText.Set(iVisualText.Left(iVisualText.Length()-iDisplayTextOffset));
        }

    // clip the end of text if it exceeds editor rectancle
    TInt maxCount = iFont->TextCount(iDisplayText,rectWidth);
    if(maxCount < iDisplayText.Length())
        {
        iDisplayText.Set(iDisplayText.Left(maxCount));
        if ( iTextDirection == TBidiText::ELeftToRight )
            {
            iVisualText.Set(iVisualText.Left(maxCount));
            }
        else
            {
            iVisualText.Set(iVisualText.Right(maxCount));
            }
        }
   }
    
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::IsFocused
// 
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepLayoutEditAreaBase::IsFocused()
    {
    return iHasFocus;
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetFont
// Set font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C void CFepLayoutEditAreaBase::SetFont(const TFontSpec& aFontSpec)
    {
    iFontSpec = aFontSpec;
    if(BitmapDevice())
        {
        if(iFontOwnership && iFont)
            {
            BitmapDevice()->ReleaseFont(iFont);
            iFont = NULL;    
            }    
        
        if (KErrNone != BitmapDevice()->GetNearestFontInPixels(iFont,iFontSpec))
            iFont = NULL;
        }
    iFontOwnership = ETrue;
    if(iFont && iInsertionPoint)
        {
        iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow()); 
        iInsertionPoint->SetHeight(iFont->HeightInPixels());
        }
    }        

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetFont
// Set font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C void CFepLayoutEditAreaBase::SetFont(const CFont* aFont)
    {
    
    if(BitmapDevice())
        {
        if(iFontOwnership && iFont)
            {
            BitmapDevice()->ReleaseFont(iFont);
            }    
        
        iFont = const_cast<CFont*> (aFont);
        if(iFont)
            iFontSpec = iFont->FontSpecInTwips();
        }
    iFontOwnership = EFalse;
        
    if(iFont && iInsertionPoint)
        {
        iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow()); 
        iInsertionPoint->SetHeight(iFont->HeightInPixels());
        }
    }        

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::UpdateArea
// Update layout area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
/*void CFepLayoutEditAreaBase::UpdateArea(const TRect& aRect,TBool aUpdateFlag)
    {
    CFepUiBaseCtrl::UpdateArea(aRect,aUpdateFlag);
    }
  */  
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::Move
// Move the window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CFepLayoutEditAreaBase::Move(const TPoint& aOffset)
    {
    CFepUiBaseCtrl::Move(aOffset);
    //move text rect
    iCorrectedRect.Move(aOffset);
    //move cursor
    if(iInsertionPoint)
        {        
        TPoint curPos = iInsertionPoint->Position();
        curPos += aOffset;
        iInsertionPoint->SetPosition(PositionOfInsertionPointOnWindow());
        }
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::CancelPointerDownL
// Move the window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                
EXPORT_C void CFepLayoutEditAreaBase::CancelPointerDownL()
    {
    if (PointerDown())
        {
        SetPointerDown(EFalse);
        if(iDisplayTextCurSel.Length() && (iCursorVisible||iCursorSelVisible))
            {
            TRect rect = DrawSelection();
            UpdateArea(rect,EFalse);
            }
        }
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepLayoutEditAreaBase::HandlePointerLeave(const TPoint& aPt)
    {
    /*if(iDisplayTextCurSel.Length())
        {
        TRect rect = DrawSelection();
        UpdateArea(rect,ETrue);
        iDisplayTextCurSel.SetSelection(0,0);                
        iInsertionPoint->SetOn(ETrue);
        }    */
    if (iDimmed || !(iCursorVisible||iCursorSelVisible))
        {
        return;
        }

    CFepUiBaseCtrl::HandlePointerLeave(aPt);
    }


// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetFocus
// Set editor focus
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepLayoutEditAreaBase::SetFocus(TBool aFlag)
    {
    if(aFlag == iHasFocus)
        return;// do nothing is no status changed.
    if(aFlag) //gain focus
        {
        PrepareForFocusGain();
        }
    else
        {
        PrepareForFocusLoss();
        }
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::AdjustSelectedCompositionText
// Adjust cursor
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepLayoutEditAreaBase::AdjustSelectedCompositionText(
        TInt& aPositionOfInsertionPointInBuffer)
    {
    if(!iBuffer)
        {
        aPositionOfInsertionPointInBuffer = 0;
        return;
        } 
    
    if(aPositionOfInsertionPointInBuffer < 0)
        {
        aPositionOfInsertionPointInBuffer = 0;
        } 
    
    if(aPositionOfInsertionPointInBuffer > iBuffer->Length())
        {
        aPositionOfInsertionPointInBuffer = iBuffer->Length();
        }

    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::OnDeActivate
// Response to layout de activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepLayoutEditAreaBase::OnDeActivate()
    {    
    CFepUiBaseCtrl::OnDeActivate();
    //cancel the insertion pointer timer
    if(iHasFocus)
        iInsertionPoint->SetOn(EFalse);
    
    DisableRegionUpdating(); //temp fix for font problem when layout is destroyed.
    
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::OnActivate
// Response to layout activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepLayoutEditAreaBase::OnActivate()
    {
    CFepUiBaseCtrl::OnActivate();
    EnableRegionUpdating();
    //set cursor if needed    
    //cancel the insertion pointer timer
    iOverlappedCtrlList.Reset();
    TBool on = iCursorVisible && iHasFocus;
    //TBool on = iCursorVisible && iHasFocus && (iOverlappedCtrlList.Count() <= 0) ;
    //iInsertionPoint->SetOn(iCursorVisible && iHasFocus);    
    iInsertionPoint->SetOn(on);
    
    }
        
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SetCursorVisible
// Set Cursor Visible
// ---------------------------------------------------------------------------
//           
EXPORT_C void CFepLayoutEditAreaBase::SetCursorVisible(TBool aCursorVisibleFlag,
                                                       TBool aCursorSelVisibleFlag)
    {
    if ( (iCursorVisible == aCursorVisibleFlag) && 
         (iCursorSelVisible == aCursorSelVisibleFlag) )
        {
        return;
        }
        
    iCursorVisible = aCursorVisibleFlag;
    iCursorSelVisible = aCursorSelVisibleFlag;
    iInsertionPoint->SetOn(iCursorVisible && iHasFocus);
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::CursorVisible
// Get Cursor Visible
// ---------------------------------------------------------------------------
//           
EXPORT_C TBool CFepLayoutEditAreaBase::CursorVisible() const
    {
    return iCursorVisible;
    }

EXPORT_C void CFepLayoutEditAreaBase::SetDimmed(TBool aDimFlag)
    {
    iDimmed = aDimFlag;
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::OnLayoutDraggingStart
// Response to layout dragging start event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepLayoutEditAreaBase::OnLayoutDraggingStart()
    {
    iInDragging = ETrue;
    }
    
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::OnLayoutDraggingEnd
// Response to layout dragging end event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepLayoutEditAreaBase::OnLayoutDraggingEnd()
    {
    iInDragging = EFalse;
    Draw();
    UpdateArea(Rect(),EFalse);
    }    

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::GetEditorFieldMaxLen
// Get the maximum length of the editor field.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepLayoutEditAreaBase::GetEditorFieldMaxLen()
    {
    TFontSpec curFont;
    if( iFont )
        {
        _LIT(KMinChars, "..''");
        return iCorrectedRect.Width() * KMinChars().Length()/ iFont->TextWidthInPixels(KMinChars); 
        }
    return iCorrectedRect.Width() / 2;
    }
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::UpdateValidRegion
// Update valid region
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepLayoutEditAreaBase::UpdateValidRegion(CFepUiBaseCtrl* /*aCtrl*/,
                                                            TBool /*aRemoveFlag*/)
    {
    return;
    /*
    //check whether overlaps with others, so as we can show/hide cursor
    if(!iUpdateFlagEnabled || !aCtrl || aCtrl == this)
        return; // do nothing if not overlapped with ICF cursor
    TPoint curPt = PositionOfInsertionPointOnWindow();
    if(aRemoveFlag) //a control is going to be hiden
        {
        RestoreCursorState(aCtrl);
        }
    else //a shown control changes rect
        {
        TInt idx = FindOverlappedCtrl(aCtrl);
        if(idx != KErrNotFound)
            {
            if(aCtrl->Rect().Contains(curPt))
                return;
            //restore
            RestoreCursorState(idx);
            }
        else  //new control
            {
            if(!aCtrl->Rect().Contains(curPt))
                return;
        
            SaveCursorState(aCtrl);
            //disable cursor
            iInsertionPoint->SetOn(EFalse);            
            }
        }*/
    }

// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::SaveCursorState
// Save current cursor state
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CFepLayoutEditAreaBase::SaveCursorState(CFepUiBaseCtrl* aCtrl)    
    {
    for(TInt i = 0 ; i < iOverlappedCtrlList.Count(); ++i)
        {        
        if(aCtrl == iOverlappedCtrlList[i].iCtrl)
            return;
        }
    TOverlappedInfo ctrlInfo={aCtrl,iInsertionPoint->IsOn()};
    
    iOverlappedCtrlList.Append(ctrlInfo);
    }


// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::RestoreCursorState
// Restore cusror state
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CFepLayoutEditAreaBase::RestoreCursorState(TInt aIdx)    
    {
    //no need to check index. as it's checked before called.							   
    iInsertionPoint->SetOn(iOverlappedCtrlList[aIdx].iCursorStateBeforeOverlapped);
    iOverlappedCtrlList.Remove(aIdx);
    }

    
// ---------------------------------------------------------------------------
// CFepLayoutEditAreaBase::RestoreCursorState
// Restore cusror state
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CFepLayoutEditAreaBase::RestoreCursorState(CFepUiBaseCtrl* aCtrl)    
    {
    //need check whether that control previously overlapped with ICF    
    TInt idx = FindOverlappedCtrl(aCtrl);
    if(idx != KErrNotFound)
        RestoreCursorState(idx);
    }
    
    
TInt CFepLayoutEditAreaBase::FindOverlappedCtrl(CFepUiBaseCtrl* aCtrl) 
    {
    for(TInt i = 0 ; i < iOverlappedCtrlList.Count(); ++i)
        {        
        if(aCtrl == iOverlappedCtrlList[i].iCtrl)
            return i;
        }
    return KErrNotFound;
    }


TInt CFepLayoutEditAreaBase::TextWidth( TInt aStart, TInt aEnd )
    {
    CFont::TMeasureTextInput input;
    input.iStartInputChar = aStart;
    input.iEndInputChar = aEnd;
    input.iFlags = 0;
    CFbsFont::TMeasureTextOutput output;
    TInt width = iFont->MeasureText(iDisplayText, &input, &output);
    return width;
    }

//  End of File 
