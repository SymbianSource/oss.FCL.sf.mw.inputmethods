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
* Description:   candidate stands for every cell in drop down list
*
*/


#include <gdi.h>
#include <e32base.h>
#include <w32std.h>
#include <biditext.h> 

#include "peninputcandidate.h"
#include "peninputdropdownlist.h"

_LIT(KEllipsis, "\x2026");

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CCandidate 

CCandidate* CCandidate::NewL(const TDesC& aString,
                             MFepCtrlDropdownListContext* aOwner)
    {
    CCandidate* self = new (ELeave) CCandidate(aString, aOwner);  // Q: is here will call ctor of base class?
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;
    }
    
void CCandidate::ConstructL()
    {
    iDisplayText = iCandidate.AllocL();
    }

// -----------------------------------------------------------------------------
// CCandidate::CCandidate
// construct function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCandidate::CCandidate(const TDesC& aString, MFepCtrlDropdownListContext* aOwner)
: iCandidate(aString),iBaselineOffset(0), iOwner(aOwner)
    {
    }

// -----------------------------------------------------------------------------
// CCandidate::~CCandidate
// destructor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCandidate::~CCandidate() 
    {
    delete iDisplayText;
    }

// -----------------------------------------------------------------------------
// CCandidate::GetCandidate
// get candidate string
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CCandidate::GetCandidate()
    {
    return iCandidate;
    }

// -----------------------------------------------------------------------------
// CCandidate::GetRect
// get rect of the candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TRect& CCandidate::GetRect() const
    {
    return iRect;
    }

// -----------------------------------------------------------------------------
// CCandidate::Move
// move rect of the candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCandidate::Move(const TPoint& aOffsetPoint)
    {
    iRect.Move(aOffsetPoint);
    }
    
// -----------------------------------------------------------------------------
// CCandidate::SetPosition
// set position for the candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCandidate::SetPositionL(const TPoint& aLeftTopPosition, 
                              TInt aWidthForOneUnit, 
                              TInt aHeight, 
                              const CFont* aTextFont,
                              TInt aCellHorizontalMargin)
    {
    iTextFont = aTextFont;
    
    // Calculate base line offset
    iBaselineOffset = aHeight/2 + aTextFont->AscentInPixels()/2 ; 
    
    // Calculate the cell for the candidate
    //const TInt cellCount = CalculateCells(aWidthForOneUnit, aTextFont);
    const TInt cellCount = CalculateDisplayTextL(aWidthForOneUnit, aTextFont, iOwner->GetColNum());

    // modified by txin
    // original:
    /*
    iRect.iBr = aLeftTopPosition;
    
    iRect.SetRect(aLeftTopPosition, 
                  TSize(cellCount * aWidthForOneUnit + (cellCount - 1) * aCellHorizontalMargin,
                        aHeight));
    */
    // if owner's horizontal direction is RToL, then aLeftTopPosition actually mean RightTop pos
    TInt candHDir = iOwner->CandHDirection();
    TSize candSize = TSize(cellCount * aWidthForOneUnit + (cellCount - 1) * aCellHorizontalMargin,
                           aHeight);
    (candHDir == CFepCtrlDropdownList::ECandFromLToR) ?
        iRect.SetRect(aLeftTopPosition, candSize) : 
        iRect.SetRect(TPoint(aLeftTopPosition.iX - candSize.iWidth, aLeftTopPosition.iY), candSize);
    // end modify

    return cellCount;
    }

// -----------------------------------------------------------------------------
// CCandidate::DrawText
// draw text of the candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCandidate::DrawText(CFbsBitGc& aGc, const CGraphicsContext::TTextAlign aAlignment, 
                          const TInt aTextMargin, TRgb aTextColor, 
                          TRgb aSpecialTextColor, const TDesC& aFixedText)
    {
    
    aGc.SetBrushStyle(CGraphicsContext::ENullBrush);
    if ( aFixedText.Length() == 0 )    
        {
        // If fixed text has no any data
        aGc.SetPenColor(aTextColor);
        if(TBidiText::TextDirectionality( *iDisplayText ) == TBidiText:: ERightToLeft)
            {
            // If find the fixed text in the candidate and the index is equal to 0
            TInt posX = 0;
            if(!iTextFont)
                {
                iTextFont = iOwner->GetFont();
                }
            if ( aAlignment == CGraphicsContext::ECenter )
                {
                posX = iRect.iTl.iX + 
                       (iRect.Width() - iTextFont->TextWidthInPixels(*iDisplayText)) / 2;
                }
            else  if ( aAlignment == CGraphicsContext::ELeft )
                {
                posX = iRect.iTl.iX + aTextMargin;
                }
            else if ( aAlignment == CGraphicsContext::ERight )
                {
                posX = iRect.iTl.iX + iRect.Width() - 
                       iTextFont->TextWidthInPixels(*iDisplayText) - aTextMargin;
                }
            
            CGraphicsContext::TDrawTextExtendedParam param;
            param.iParRightToLeft = ETrue;
            aGc.DrawTextExtended(*iDisplayText, TPoint(posX,iRect.iTl.iY + iBaselineOffset), param);            
            }
        else
            {
            aGc.DrawText( *iDisplayText, iRect, iBaselineOffset, aAlignment);
            }
        }
    else
        {
        if ( iCandidate.Find(aFixedText) == 0 )
            {
            // If find the fixed text in the candidate and the index is equal to 0
            TInt posX = 0;
            if(!iTextFont)
                {
                iTextFont = iOwner->GetFont();
                }            
            if ( aAlignment == CGraphicsContext::ECenter )
                {
                posX = iRect.iTl.iX + 
                       (iRect.Width() - iTextFont->TextWidthInPixels(iCandidate)) / 2;
                }
            else  if ( aAlignment == CGraphicsContext::ELeft )
                {
                posX = iRect.iTl.iX + aTextMargin;
                }
            else if ( aAlignment == CGraphicsContext::ERight )
                {
                posX = iRect.iTl.iX + iRect.Width() - 
                       iTextFont->TextWidthInPixels(iCandidate) - aTextMargin;
                }
            
            
            // Draw Fixed text
            aGc.SetPenColor(aTextColor);
            aGc.DrawText(aFixedText, TPoint(posX,iRect.iTl.iY + iBaselineOffset));
            
            // Snatch the special text
            TPtrC buf = iCandidate.Mid(aFixedText.Length());      
            
            // Set X position for the special text
            posX = posX + iTextFont->TextWidthInPixels(aFixedText);
            
            // Draw special text
            aGc.SetPenColor(aSpecialTextColor);
            aGc.DrawText(buf, TPoint(posX, iRect.iTl.iY + iBaselineOffset));
            }
        else
            {
            aGc.SetPenColor(aTextColor);
            if(TBidiText::TextDirectionality( iCandidate ) == TBidiText:: ERightToLeft)
                {
                // If find the fixed text in the candidate and the index is equal to 0
                TInt posX = 0;
                if(!iTextFont)
                    {
                    iTextFont = iOwner->GetFont();
                    }                
                if ( aAlignment == CGraphicsContext::ECenter )
                    {
                    posX = iRect.iTl.iX + 
                           (iRect.Width() - iTextFont->TextWidthInPixels(iCandidate)) / 2;
                    }
                else  if ( aAlignment == CGraphicsContext::ELeft )
                    {
                    posX = iRect.iTl.iX + aTextMargin;
                    }
                else if ( aAlignment == CGraphicsContext::ERight )
                    {
                    posX = iRect.iTl.iX + iRect.Width() - 
                           iTextFont->TextWidthInPixels(iCandidate) - aTextMargin;
                    }
                
                CGraphicsContext::TDrawTextExtendedParam param;
                param.iParRightToLeft = ETrue;
                aGc.DrawTextExtended(iCandidate, TPoint(posX,iRect.iTl.iY + iBaselineOffset), param);            
                }
            else
                {
                aGc.DrawText( iCandidate, iRect, iBaselineOffset, CGraphicsContext::ECenter);
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CCandidate::CalculateCells
// calculate the cell for the candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCandidate::CalculateCells(const TInt aWidthForOneUnit, 
                                const CFont* aTextFont,
                                const TDesC& aText)
    {
    const TInt width = aTextFont->TextWidthInPixels(aText) + 2 * iOwner->GetTextMargin();

    if (width <= aWidthForOneUnit)
        {
        return 1;
        }
    
    return (width + aWidthForOneUnit + 
     iOwner->GetCellHorizontalMargin() - 1)/(aWidthForOneUnit + iOwner->GetCellHorizontalMargin());
    }
    
TInt CCandidate::CalculateDisplayTextL(const TInt aWidthForOneUnit, 
                                       const CFont* aTextFont,
                                       TInt aColNum)
    {
    TInt cellcnt = CalculateCells(aWidthForOneUnit, aTextFont, *iDisplayText);
    
    if (cellcnt <= aColNum)
        {
        return cellcnt;
        }

    TPtr ptr = iDisplayText->Des();

    if (iOwner->CandTruncateType() == CFepCtrlDropdownList::ECandTruncateFromBeginning)
        {
        while (cellcnt > aColNum && ptr.Length() > 0)
            {
            ptr.Delete(0, 1);
            cellcnt = CalculateCells(aWidthForOneUnit, aTextFont, ptr);
            }
        }
    else if (iOwner->CandTruncateType() == CFepCtrlDropdownList::ECandTruncateAsEllipsis)
        {
        while (cellcnt > aColNum && ptr.Length() > 1)
            {
            ptr.Delete(ptr.Length()-2, 2);
            ptr.Append(KEllipsis);
            cellcnt = CalculateCells(aWidthForOneUnit, aTextFont, ptr);
            }
        }
    else if (iOwner->CandTruncateType() == CFepCtrlDropdownList::ECandTruncateFromEnd)
        {
        while (cellcnt > aColNum && ptr.Length() > 0)
            {
            ptr.Delete(ptr.Length()-1, 1);
            cellcnt = CalculateCells(aWidthForOneUnit, aTextFont, ptr);
            }
        }
        
    return cellcnt;    
    }

// End Of File
