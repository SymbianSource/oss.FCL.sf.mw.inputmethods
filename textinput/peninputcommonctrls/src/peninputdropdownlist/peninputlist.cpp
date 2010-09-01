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
* Description:  Abstract base class for drop-down list objects
*
*/

#include <fbs.h>

#include "peninputlist.h"
#include "peninputcandidate.h"
#include "peninputpluginutils.h"
#include <AknsDrawUtils.h> 
#include "peninputdropdownlist.h"
#include "peninputlayout.h"

const TInt KInvalidCandIndex = -1;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CList 

// -----------------------------------------------------------------------------
// CList::CList
// construtor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CList::CList(CListManager* aManager, MFepCtrlDropdownListContext *aOwner) 
: iOwner(aOwner), iManager(aManager), iNextButtonDisable(ETrue),
  iPreviousButtonDisable(ETrue)
    {
    }
    
// -----------------------------------------------------------------------------
// CList::~CList
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CList::~CList()
    {     
    }

// -----------------------------------------------------------------------------
// CList::ConstructL
// 2nd construtor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::ConstructL()
    {
    // Pre calculate all areas
    CalculateAreas();
    }
    
// -----------------------------------------------------------------------------
// CList::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::HandlePointerDownL(const TPoint& aPoint)
    {
    // position where the tap occurred

    // set clicked rect as empty rect
    iClickedRect = KEmptyRect;

    // Check if content rect tapped
    if ( iContentRect.Contains(aPoint) )
        {
        CCandidate* candidate = FindCandidate(aPoint, iClickedCandidateIndex);
        
        if ( candidate )
            {
			#ifdef RD_TACTILE_FEEDBACK
	        static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
	            DoTactileFeedback(ETouchFeedbackBasic);
			#endif //RD_TACTILE_FEEDBACK
            TRect rect = candidate->GetRect();
            iClickedRect.SetRect(rect.iTl,rect.iBr);

            // Set the flag to indicate that candidate button has been pressed down
            // so that we know to draw correctly
            iIndicateButtonClicked = ETrue ;
            DrawSelection(iClickedRect,ETrue);
            DrawHighightCell(iClickedRect,EFalse);
            }
        else
            {
            iClickedRect = KDummyRect;
            // This is a dummy value so that we know
            // in HandlePointerUpL that iClickedRect is
            // is not empty but the pointer down event did
            // not occure inside any valid rect such as
            // candidate rect or any of the navigation rects
            }
        }
    } 

// -----------------------------------------------------------------------------
// CList::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::HandlePointerDragL(const TPoint& /*aPoint*/)
    {
    }
    
// -----------------------------------------------------------------------------
// CList::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::HandlePointerUpL( const TPoint& /*aPoint*/)
    {
    }

// -----------------------------------------------------------------------------
// CList::HandlePointerLeave
// handle pointer leave function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::HandlePointerLeave(const TPoint& /*aPoint*/)
    {
    DrawSelection( iClickedRect, EFalse );
    DrawHighightCell(iClickedRect, ETrue);
    }
    
// -----------------------------------------------------------------------------
// CList::Draw
// draw the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::Draw()
    {
    // Draw mask
    iOwner->DrawMask(iRect);
    
    // Active bitmap context
    iOwner->ActivateBitmapContext();

    iOwner->Gc()->SetBrushStyle(CGraphicsContext::ESolidBrush);
    iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
    iOwner->Gc()->SetPenSize(iOwner->GetPenSize());       
    iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);
    iOwner->Gc()->SetPenColor( iOwner->GetBorderColor() );          
    iOwner->Gc()->DrawRect(iRect); 
    
    }

// -----------------------------------------------------------------------------
// CList::Rect
// get the rect of list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TRect& CList::Rect()
    {
    return iRect;
    }

// -----------------------------------------------------------------------------
// CList::UpdateList
// update the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::UpdateListL(TBool /*aDrawFlag*/, TBool /*aRecalculate*/)
    {
    // Do nothing, every derived class will have its action
    }


// -----------------------------------------------------------------------------
// CList::ResetStatus
// update the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::ResetStatus()
    {
    iPenDownInCandidate = EFalse;
    iClickedRect = KEmptyRect;
    }
    
// -----------------------------------------------------------------------------
// CList::FindCandidate
// find candidate according to the assigned position
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
CCandidate* CList::FindCandidate(const TPoint& aPosition, TInt& aIndex)
    {
    CCandidate* candidate = NULL;
    
    aIndex = KErrNotFound;

    if ( iOwner->CandidateCount() > 0 )
        {
        // Loop through the candidates and find the one that contains
        // the TPoint aPosition
        for (TInt i = iOwner->GetFirstCandidateIndex(); i < iOwner->GetLastCandidateIndex(); i++)
            {
            candidate = iOwner->GetCandidate(i); 
            
            if ( !candidate )
                {
                continue;
                }
                
            TRect rect = candidate->GetRect();
            
            if ( rect.Contains(aPosition) )
                {
                // We found the candidate
                aIndex = i;
                return candidate;
                }
            }
        }
        
    return NULL;
    }

// -----------------------------------------------------------------------------
// CList::CalculatePositionsL
// calculate the position of all visible cells in the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CList::CalculatePositionsL()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CList::SetHighlightCell
// Set cell to highlight or not
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::SetHighlightCell(const TInt aCellIndex, TBool aHighlight, TBool aForceDraw)
    {
    if ( !aForceDraw && iPenDownInCandidate ) 
        {
        return;        
        }
        
    if ( ( aCellIndex<0 ) || ( aCellIndex > iOwner->CandidateCount() ) )
        {
        return;
        }

    CCandidate* candidate = iOwner->GetCandidate(aCellIndex);
    
    if ( ! candidate )
        {
        return;
        }

    TRect rect = candidate->GetRect();
    
    iOwner->DrawMask(rect);
    iOwner->ActivateBitmapContext();

	if (!iDropdownListDrawInfo.iSupportSkin)
		{
	    iOwner->Gc()->SetBrushStyle(CGraphicsContext::ENullBrush); 
	    iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
	    iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);
	    iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());

	    // Draw the bottom and right line
	    iOwner->Gc()->DrawLine( TPoint(rect.iTl.iX, rect.iBr.iY),rect.iBr);
	    iOwner->Gc()->DrawLine( TPoint(rect.iBr.iX, rect.iTl.iY),rect.iBr);
	    
	    // also draw left and top line
	    if (iOwner->GetCellHorizontalMargin() != 0)
	        {
	        iOwner->Gc()->DrawLine(rect.iTl, TPoint(rect.iTl.iX, rect.iBr.iY));
	        }
	        
	    if (iOwner->GetCellVerticalMargin() != 0)
	        {
	        iOwner->Gc()->DrawLine(rect.iTl, TPoint(rect.iBr.iX, rect.iTl.iY));
	        }

	    rect.Shrink(TSize(1,1));
	    
	    if ((iIsMultiRows) && (iOwner->GetCellVerticalMargin() == 0))
	        {        
	        if ( ( rect.iTl.iY - 1 - Rect().iTl.iY ) != ( iOwner->GetRowNum() - 1 ) * 
                                                      iOwner->GetCellHeight() )
	            {
	            //If it isn't the last row
	            rect.iBr.iY = rect.iBr.iY + 1;
	            }
	        }
		}

    const CFbsBitmap* bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapHighlight);
    if ( bmp )
        {
        if (aHighlight)
            {
            CFbsBitmap* bmpTemp = const_cast<CFbsBitmap*>(bmp);
            AknIconUtils::SetSize(bmpTemp, rect.Size(), EAspectRatioNotPreserved);
            
            if (iDropdownListDrawInfo.iSupportSkin)
                {
                CBitmapDb::TBitmapType bmpID = CBitmapDb::EBitmapHighlight;
                CBitmapDb::TBitmapType maskBmpID = CBitmapDb::EBitmapHighlightMask;             
                bmp = iOwner->GetBitmapDb()->GetBitMap(bmpID);
                bmpTemp = const_cast<CFbsBitmap*>(bmp);
                iOwner->Gc()->DrawBitmapMasked(rect, 
                                               bmp, 
                                               bmp->SizeInPixels(), 
                                               iOwner->GetBitmapDb()->GetBitMap(maskBmpID),
                                               ETrue);
                }
            else
            	{
	            iOwner->Gc()->DrawBitmapMasked(rect, 
	                                           bmp, 
	                                           bmp->SizeInPixels(), 
	                                           iOwner->GetBitmapDb()->GetBitMap(
                                                                  CBitmapDb::EBitmapHighlightMask),
	                                           ETrue);
            	}
            }
        else
            {
            // Draw bitmap
            if (iDropdownListDrawInfo.iSupportSkin)
            	{
			    TRect innerRect = rect;
			    innerRect.Shrink( 4, 0 );
			    
				AknPenInputDrawUtils::Draw3PiecesFrame(AknsUtils::SkinInstance(),
										   *iOwner->Gc(),
										   rect,
										   innerRect,
										   iDropdownListDrawInfo.iCandidateLeftImgID,
										   iDropdownListDrawInfo.iCandidateMiddleImgID,
										   iDropdownListDrawInfo.iCandidateRightImgID);
            	}
        	else
        		{
	            bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPrimary);
	            iOwner->Gc()->DrawBitmapMasked(rect, 
	                                           bmp, 
	                                           bmp->SizeInPixels(), 
	                                           iOwner->GetBitmapDb()->GetBitMap(
                                                                    CBitmapDb::EBitmapPrimaryMask),
	                                           ETrue);
        		}
            }
        }
    else
        {
        if (aHighlight)
            {
            iOwner->Gc()->SetBrushColor(iOwner->GetHighLightBgColor());
            }
        else
            {
            iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());        
            }

        iOwner->Gc()->SetPenStyle(CGraphicsContext::ENullPen);
        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );

        rect.Shrink(TSize(1,1));

        iOwner->Gc()->DrawRect(rect);

        rect.Shrink(TSize(-1,-1));
        }       
    
    // draw canditate's text
    iOwner->Gc()->UseFont(iOwner->GetFont());

    candidate->DrawText(*iOwner->Gc(),iOwner->GetAlignment(),
                        iOwner->GetTextMargin(),
                        iOwner->GetTextColor(),
                        iOwner->GetSpecialTextColor(),
                        iOwner->GetFixedText());
    
    iOwner->Gc()->DiscardFont();      
    iOwner->Invalidate(candidate->GetRect());
    }

// -----------------------------------------------------------------------------
// CList::DrawSelection
// Draw selection cell just selected
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
void CList::DrawSelection(const TRect& aRect, TBool aIndicate)
    {
    // Check whether the rect that should be redrawn is in content rect   
    TRect tmpRect = iContentRect;
    tmpRect.Intersection(aRect);
    
    if ( tmpRect == aRect )
        {
        // rect is inside content rect thus we
        // should be drawing candidate press
        DrawCandidatePress(aRect, aIndicate);
        }
    else
        {
        // rect is inside navigation rect thus we
        // should be drawing navigation press
        DrawNavigationPress(aRect, aIndicate);     
        }
    }

// -----------------------------------------------------------------------------
// CList::DrawCandidatePress
// draw candidate when pressed or un-pressed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::DrawCandidatePress(const TRect& aRect, TBool aIndicate)
    {
    // We are dealing with the candidates
    TInt index = 0;
    // Get the candidate
    CCandidate* candidate = FindCandidate(aRect.Center(), index);
    
    if (!candidate)
        {
        return;
        }
        
    iOwner->DrawMask(aRect);
    
    iOwner->ActivateBitmapContext();
    
    iPenDownInCandidate = EFalse;
    
    TBool isDrawBmp = ETrue;
    CBitmapDb::TBitmapType bmpID = CBitmapDb::EBitmapPrimary;
    CBitmapDb::TBitmapType maskBmpID = CBitmapDb::EBitmapPrimaryMask;
    const CFbsBitmap* bmp = NULL;
    // set bg color
    if ( aIndicate )
        {
        bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPenDown);
        if ( bmp )
            {
            // Draw bitmap
            bmpID = CBitmapDb::EBitmapPenDown;
            maskBmpID = CBitmapDb::EBitmapPenDownMask;
            isDrawBmp = ETrue;                       
            iPenDownInCandidate = ETrue;           
            }
        else
            {
            iOwner->Gc()->SetBrushColor(iOwner->GetButtonDownBgColor());
            }                
        }
    else
        {
        const RArray<TInt>& secondaryList = iOwner->GetSecondaryList();
        
        if ( iOwner->IsEnableSecondary() && (secondaryList.Find(index) != KErrNotFound) )
            {
            bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapSecondary);
            if ( bmp )
                {
                // Draw bitmap
                bmpID = CBitmapDb::EBitmapSecondary;
                maskBmpID = CBitmapDb::EBitmapSecondaryMask;
                isDrawBmp = ETrue;                                           
                }
            else            
                {
                bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPrimary);
                if ( bmp )
                    {
                    // Draw bitmap
                    bmpID = CBitmapDb::EBitmapPrimary;
                    maskBmpID = CBitmapDb::EBitmapPrimaryMask;
                    isDrawBmp = ETrue;                                           
                    }
                else            
                    {
                    // If enable multi color and the color has been set, then use the color
                    iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());                    
                    }
                }                            
            }
        else
            {
            // Else use background color
            bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPrimary);
            if ( bmp )
                {
                bmpID = CBitmapDb::EBitmapPrimary;
                maskBmpID = CBitmapDb::EBitmapPrimaryMask;
                isDrawBmp = ETrue;                   
                }
            else
                {
                iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
                }            
            }
        }
    
    // Get the candidate rect
    TRect rect = candidate->GetRect();
    
    if (!iDropdownListDrawInfo.iSupportSkin)
    	{
	    iOwner->Gc()->SetBrushStyle( CGraphicsContext::ENullBrush ); 
	    iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
	    iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);
	    iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());

	    // Draw the bottom line
	    iOwner->Gc()->DrawLine( TPoint(rect.iTl.iX, rect.iBr.iY),rect.iBr );
	    iOwner->Gc()->DrawLine( TPoint(rect.iBr.iX, rect.iTl.iY),rect.iBr );
	    
	    if (iOwner->GetCellHorizontalMargin() != 0)
	        {
	        iOwner->Gc()->DrawLine(rect.iTl, TPoint(rect.iTl.iX, rect.iBr.iY));
	        }
	        
	    if (iOwner->GetCellVerticalMargin() != 0)
	        {
	        iOwner->Gc()->DrawLine(rect.iTl, TPoint(rect.iBr.iX, rect.iTl.iY));
	        }

	    rect.Shrink(TSize(1,1));
	    
	    if ((iIsMultiRows) && (iOwner->GetCellVerticalMargin() == 0))
	        {        
	        if ( ( rect.iTl.iY - 1 - Rect().iTl.iY ) != ( iOwner->GetRowNum() - 1 ) * 
                                                      iOwner->GetCellHeight() )
	            {
	            //If it isn't the last row
	            rect.iBr.iY = rect.iBr.iY + 1;
	            }
	        }
    	}
        
    if ( isDrawBmp )
        {
        if (iDropdownListDrawInfo.iSupportSkin)
        	{
			if ( aIndicate )
				{
/*				
				iOwner->Gc()->SetPenStyle(CGraphicsContext::ENullPen);
		        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );    
		        iOwner->Gc()->SetBrushColor(iDropdownListDrawInfo.iCandidatesHighlightColor);
		        
		        // Draw the whole rect without border
		        iOwner->Gc()->DrawRect( rect );  	*/
		        
                /*bmpID = CBitmapDb::EBitmapHighlight;
                maskBmpID = CBitmapDb::EBitmapHighlightMask;		        
	            bmp = iOwner->GetBitmapDb()->GetBitMap(bmpID);
	            iOwner->Gc()->DrawBitmapMasked(rect, 
	                                           bmp, 
	                                           bmp->SizeInPixels(), 
	                                           iOwner->GetBitmapDb()->GetBitMap(maskBmpID),
	                                           ETrue); 		*/
                TRect innerRect = rect;
                innerRect.Shrink( 4, 0 );
                
                // ----- draw bitmaps -----
                iOwner->ActivateBitmapContext();
                iOwner->Gc()->Clear(rect);
                    
                
                
                AknPenInputDrawUtils::Draw3PiecesFrame(AknsUtils::SkinInstance(),
                                           *iOwner->Gc(),
                                           rect,
                                           innerRect,
                                           iDropdownListDrawInfo.iCandidatePressedLeftImgID,
                                           iDropdownListDrawInfo.iCandidatePressedMiddleImgID,
                                           iDropdownListDrawInfo.iCandidatePressedRightImgID);				
				}
			else
				{
			    TRect innerRect = rect;
			    innerRect.Shrink( 4, 0 );
			    
			    // ----- draw bitmaps -----
			    iOwner->ActivateBitmapContext();
			    iOwner->Gc()->Clear(rect);
			    	
				
				
				AknPenInputDrawUtils::Draw3PiecesFrame(AknsUtils::SkinInstance(),
										   *iOwner->Gc(),
										   rect,
										   innerRect,
										   iDropdownListDrawInfo.iCandidateLeftImgID,
										   iDropdownListDrawInfo.iCandidateMiddleImgID,
										   iDropdownListDrawInfo.iCandidateRightImgID);
				}
        	}
        else
        	{
	        bmp = iOwner->GetBitmapDb()->GetBitMap(bmpID);
	        iOwner->Gc()->DrawBitmapMasked(rect, 
	                                       bmp, 
	                                       bmp->SizeInPixels(), 
	                                       iOwner->GetBitmapDb()->GetBitMap(maskBmpID), 
	                                       ETrue);
        	}
        }
    else
        {
        iOwner->Gc()->SetPenStyle(CGraphicsContext::ENullPen);
        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );    
        // Draw the whole rect without border
        iOwner->Gc()->DrawRect( rect );      
        }
        
    // draw canditate's text
    iOwner->Gc()->UseFont(iOwner->GetFont());
    TRgb textColor;
    if (aIndicate)
        {
        textColor = iOwner->GetHighLightTextColor();
        }
    else
        {
        textColor = iOwner->GetTextColor();
        }
        
    candidate->DrawText(*iOwner->Gc(),iOwner->GetAlignment(),
                        iOwner->GetTextMargin(),        
                        textColor,
                        iOwner->GetSpecialTextColor(),
                        iOwner->GetFixedText());      
                        
    iOwner->Gc()->DiscardFont();
    
    // Invalidate the rect
    iOwner->Invalidate(aRect);
    }

// -----------------------------------------------------------------------------
// CList::DrawNavigationPress
// draw navigation button when pressed or un-pressed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CList::DrawNavigationPress(const TRect& /*aRect*/, TBool /*aIndicate*/)
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CList::CalculateAreas
// Calculate all kinds of rectangles
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CList::CalculateAreas()
    {
    //
    }

// -----------------------------------------------------------------------------
// CList::SizeChanged
// size changed function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::SizeChanged()
    {
    CalculateAreas();
    }

// -----------------------------------------------------------------------------
// CList::DrawCandidates
// draw all visible candidates to the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CList::DrawCandidates()
    {    
    const RArray<TInt>& secondaryList = iOwner->GetSecondaryList();
    
    TBool isDrawBmp = EFalse;
    CBitmapDb::TBitmapType bmpID = CBitmapDb::EBitmapPrimary;
    CBitmapDb::TBitmapType maskBmpID = CBitmapDb::EBitmapPrimaryMask;
    const CFbsBitmap* bmp = NULL;         
    for(TInt i = iOwner->GetFirstCandidateIndex(); i < iOwner->GetLastCandidateIndex(); i++)
        {
        CCandidate* candidate = iOwner->GetCandidate(i);
        if ( !candidate )
            {
            continue;
            }
            
        TRect rect = candidate->GetRect();
 
        if (iDropdownListDrawInfo.iSupportSkin)
        	{
        	
        	if (iOwner->GetHighlightList().Find(i) != KErrNotFound)
        		{
				/*iOwner->Gc()->SetPenStyle(CGraphicsContext::ENullPen);
		        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );    
		        iOwner->Gc()->SetBrushColor(iDropdownListDrawInfo.iCandidatesHighlightColor);
		        // Draw the whole rect without border
		        iOwner->Gc()->DrawRect( rect );*/
                bmpID = CBitmapDb::EBitmapHighlight;
                maskBmpID = CBitmapDb::EBitmapHighlightMask;		        
	            bmp = iOwner->GetBitmapDb()->GetBitMap(bmpID);
	            iOwner->Gc()->DrawBitmapMasked(rect, 
	                                           bmp, 
	                                           bmp->SizeInPixels(), 
	                                           iOwner->GetBitmapDb()->GetBitMap(maskBmpID),
	                                           ETrue);
        		}
        	else
        		{
			    TRect innerRect = rect;
			    innerRect.Shrink( 4, 0 );
			    
			    // ----- draw bitmaps -----
			 	iOwner->ActivateBitmapContext();                
			    	
				
				AknPenInputDrawUtils::Draw3PiecesFrame(AknsUtils::SkinInstance(),
													   *iOwner->Gc(),
													   rect,
													   innerRect,
													   iDropdownListDrawInfo.iCandidateLeftImgID,
													   iDropdownListDrawInfo.iCandidateMiddleImgID,
													   iDropdownListDrawInfo.iCandidateRightImgID);
        		}
        	}
        else
        	{
	        if ( iOwner->IsEnableSecondary() && ( secondaryList.Find(i) != KErrNotFound) )
	            {
	            bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapSecondary);
	            if ( bmp )
	                {
	                // Draw bitmap
	                bmpID = CBitmapDb::EBitmapSecondary;
	                maskBmpID = CBitmapDb::EBitmapSecondaryMask;
	                isDrawBmp = ETrue;                                           
	                }
	            else            
	                {
	                bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPrimary);
	                if ( bmp )
	                    {
	                    // Draw bitmap
	                    bmpID = CBitmapDb::EBitmapPrimary;
	                    maskBmpID = CBitmapDb::EBitmapPrimaryMask;
	                    isDrawBmp = ETrue;                                           
	                    }
	                else            
	                    {
	                    // If enable multi color and the color has been set, then use the color
	                    iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());                    
	                    }
	                }                            
	            }
	        else if (iOwner->GetHighlightList().Find(i) != KErrNotFound)
	            {
	            bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapHighlight);
	            if ( bmp )
	                {
	                // Draw bitmap
	                bmpID = CBitmapDb::EBitmapHighlight;
	                maskBmpID = CBitmapDb::EBitmapHighlightMask;
	                isDrawBmp = ETrue;                                           
	                }
	            else            
	                {
	                // If the cell is set to highlight, use high light color
	                iOwner->Gc()->SetBrushColor(iOwner->GetHighLightBgColor());
	                }                                        
	            }
	        else
	            {
	            // Else use background color
	            bmp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPrimary);
	            if ( bmp )
	                {
	                bmpID = CBitmapDb::EBitmapPrimary;
	                maskBmpID = CBitmapDb::EBitmapPrimaryMask;
	                isDrawBmp = ETrue;                   
	                }
	            else
	                {
	                iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
	                }            
	            }         

	        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ENullBrush ); 
	        iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
	        iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);
	        iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());
	        // Draw the bottom line
	        iOwner->Gc()->DrawLine( rect.iTl, TPoint(rect.iBr.iX, rect.iTl.iY) );
	        iOwner->Gc()->DrawLine( TPoint(rect.iTl.iX, rect.iBr.iY),rect.iBr );
	        iOwner->Gc()->DrawLine( TPoint(rect.iBr.iX, rect.iTl.iY),rect.iBr );
	        
	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            iOwner->Gc()->DrawLine(rect.iTl, TPoint(rect.iTl.iX, rect.iBr.iY));
	            }

	        rect.Shrink(TSize(1,1));

	        if ( isDrawBmp )
	            {
                bmpID = CBitmapDb::EBitmapSecondary;
                maskBmpID = CBitmapDb::EBitmapSecondaryMask;	            
	            bmp = iOwner->GetBitmapDb()->GetBitMap(bmpID);
	            iOwner->Gc()->DrawBitmapMasked(rect, 
	                                           bmp, 
	                                           bmp->SizeInPixels(), 
	                                           iOwner->GetBitmapDb()->GetBitMap(maskBmpID),
	                                           ETrue);                        
	            }
	        else
	            {            
	            iOwner->Gc()->SetPenStyle(CGraphicsContext::ENullPen);
	            iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );    
	            iOwner->Gc()->DrawRect(rect);
	            }
        	}
        
        // Draw the text of the candidate
        iOwner->Gc()->UseFont(iOwner->GetFont());
        candidate->DrawText(*iOwner->Gc(),iOwner->GetAlignment(),
                                          iOwner->GetTextMargin(),        
                                          iOwner->GetTextColor(),
                                          iOwner->GetSpecialTextColor(),
                                          iOwner->GetFixedText());
        }
    iOwner->Gc()->DiscardFont();
    }

// -----------------------------------------------------------------------------
// CList::CompareRect
// Compare two rect is equal or not
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CList::CompareRect(const TRect& aSrcRect, const TRect& aDesRect)
    {
    
    if (aSrcRect == aDesRect)
        {
        DrawSelection(iClickedRect, ETrue);
        DrawHighightCell(iClickedRect, EFalse);
        return ETrue;        
        }
    else
        {
        TBool hasDraw = EFalse;
        const RArray<TInt>& highLightList = iOwner->GetHighlightList();        
        for (TInt i=0; i < highLightList.Count(); i++ )
            {
            CCandidate* candidate = iOwner->GetCandidate(i);        
            if ( candidate )
                {
                if ( iClickedRect == candidate->GetRect() )
                    {
                    DrawHighightCell(iClickedRect, ETrue);
                    hasDraw = ETrue;
                    break;
                    }
                }
            }        
      
        if ( !hasDraw )
            {
            DrawSelection(iClickedRect, EFalse);
            DrawHighightCell(iClickedRect, ETrue);
            }

        return EFalse;        
        }
    }

// -----------------------------------------------------------------------------
// CList::Move
// when the drop down list is moved, the function will be called.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CList::Move(const TPoint& aOffsetPoint)
    {
    // Move the iRect
    iRect.Move(aOffsetPoint);
    
    // Move the content rect
    iContentRect.Move(aOffsetPoint);
    
    // Move the navigation rect
    iNavigationRect.Move(aOffsetPoint);
    
    // Move the close button rect
    iNavigationCloseRect.Move(aOffsetPoint);
    
    // Move the up button rect
    iNavigationPreviousRect.Move(aOffsetPoint);
    
    // Move the down button rect
    iNavigationNextRect.Move(aOffsetPoint);
    
    }

void CList::DrawHighightCell(const TRect& aSrcRect, const TBool aIndidate)
    {
    const RArray<TInt>& highLightList = iOwner->GetHighlightList();

    TInt candIdx;
    for (TInt i = 0; i < highLightList.Count(); i++)
        {
        candIdx = highLightList[i];

        // if highlight candidate are not in current page, then do not draw it
        if ((candIdx >= iOwner->GetFirstCandidateIndex()) &&
            (candIdx <= iOwner->GetLastCandidateIndex()))
            {
            CCandidate* candidate = iOwner->GetCandidate(candIdx);
            
            if (candidate)
                {
                if (aSrcRect == candidate->GetRect())
                    {
                    if (aIndidate)
                        {
                        SetHighlightCell(candIdx, ETrue);
                        }

                    continue;    
                    }
                }
            }
        }
    }
    
TInt CList::StartCandIdxOfLine(TInt /*aCandIndex*/)
    {
    return KInvalidCandIndex;
    }
    
void CList::SetNextPageHighlightCellAfterOnePageL(const TInt /*aCellIndex*/)
    {
    }

void CList::SetDropdownListImgID(TDropdownListDrawInfo aDropdownListDrawInfo)
	{
	iDropdownListDrawInfo = aDropdownListDrawInfo;
	}
	

// End Of File
