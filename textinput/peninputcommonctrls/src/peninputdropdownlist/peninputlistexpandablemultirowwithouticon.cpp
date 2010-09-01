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
* Description:  Multi rows list without icon for drop-down list objects
*
*/



// Include 
#include <peninputdropdownlistcontext.h>
#include <peninputdropdownlist.h>

#include "peninputlist.h"
#include "peninputlistexpandablemultirowwithouticon.h"


// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListExpandableMultiRowWithoutIcon 

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::CListExpandableMultiRowWithoutIcon
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListExpandableMultiRowWithoutIcon::CListExpandableMultiRowWithoutIcon( CListManager* aManager, 
                          MFepCtrlDropdownListContext *aOwner )
						  :CList( aManager, aOwner )
    {
    iIsMultiRows = ETrue;
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListExpandableMultiRowWithoutIcon* CListExpandableMultiRowWithoutIcon::NewL( 
										CListManager* aManager, 
                          				MFepCtrlDropdownListContext *aOwner )
    {
    CListExpandableMultiRowWithoutIcon* self 
		= new (ELeave) CListExpandableMultiRowWithoutIcon( aManager, aOwner );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::~CListExpandableMultiRowWithoutIcon
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListExpandableMultiRowWithoutIcon::~CListExpandableMultiRowWithoutIcon()
    {     
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListExpandableMultiRowWithoutIcon::HandlePointerDragL( const TPoint& aPoint )
    {
    // Pass the event first to the base
    CList::HandlePointerDragL( aPoint );
    
    if ( !iClickedRect.IsEmpty() )
        {
		// clicked rect is not outside the whole DDL
        TInt index = -1;
        
        CCandidate* candidate = FindCandidate( aPoint, index );
        iMoveLatestRect = KEmptyRect;
        
        if ( candidate )
            {
            // candidate exist
            iMoveLatestRect.SetRect( candidate->GetRect().iTl, 
									 candidate->GetRect().Size() );
            }
        
        // Compare the two rect, then decide draw it or not
        CompareRect( iMoveLatestRect, iClickedRect );
        }
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListExpandableMultiRowWithoutIcon::HandlePointerUpL( const TPoint& aPoint )
    {  
	// clicked rect is outside the whole DDL
    if ( iClickedRect.IsEmpty() )
        {
        iOwner->SetCapture( EFalse );
        // Change active list object ie. close the DDL
        iOwner->AutoChangeActiveList( iOwner->GetFirstCandidateIndex() );
        }
    else
        {
        if ( ( iContentRect.Contains( aPoint ) ) 
        	&& ( iClickedRect.Contains( aPoint ) ) )
            {
            // Redraw the selected aread    
            DrawSelection( iClickedRect, EFalse );
            
            iOwner->SetCapture( EFalse );
            
            // We have selected a candidate and iClickedCandidateIndex is 
			//	the selected Index.
            // Send the candidate to others
            if (iClickedCandidateIndex != KErrNotFound)
                {
		        iOwner->AutoChangeActiveList( 
									iOwner->GetFirstCandidateIndex() );
                CCandidate* candidate = iOwner->GetCandidate( 
												iClickedCandidateIndex );
                if ( candidate )
                    {
                    iOwner->ReportCandidateSelectEventL( 
												candidate->GetCandidate(),
                                                iClickedCandidateIndex );
                    }
                }
            }

        iClickedRect = KEmptyRect;
        }

    iIndicateButtonClicked = EFalse;
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::Draw
// draw the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListExpandableMultiRowWithoutIcon::Draw()
    {
    // Call draw in base class;
    CList::Draw();
    
    // Draw all visible candidates
    DrawCandidates();
    iOwner->Invalidate(iRect);
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::CalculateAreas
// Calculate all kinds of rectangles
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListExpandableMultiRowWithoutIcon::CalculateAreas()
    {
    // Calculate the rect of the list
    iRect.iTl = iOwner->GetTopLeftPoint();
    TInt width = iOwner->GetCellWidth() * iOwner->GetColNum() + 
                 iOwner->GetNaviWidth() + 
                 (iOwner->GetColNum() + 2) * iOwner->GetCellHorizontalMargin();

    TInt height = iOwner->GetCellHeight() * iOwner->GetRowNum() + 
                  (iOwner->GetRowNum() + 1) * iOwner->GetCellVerticalMargin();

    iRect.SetSize(TSize( width,height));
    
    if (iOwner->CandExpandType() == CFepCtrlDropdownList::ECandExpandUpward)
        {
        // expand upward
        TInt onerowheight = iOwner->GetCellHeight() + 2 * iOwner->GetCellVerticalMargin();
        iRect.Move(0, -(height - onerowheight));
        }

    // Calculate the rect of the content
    iContentRect.iTl.SetXY(iRect.iTl.iX + iOwner->GetCellHorizontalMargin(),
                           iRect.iTl.iY + iOwner->GetCellVerticalMargin());

    iContentRect.SetSize(TSize (width - 2 * iOwner->GetCellHorizontalMargin(),
                                height - 2 * iOwner->GetCellVerticalMargin()));

    // Initialized the rect of the navigation area and the down and up button
    iNavigationRect = KEmptyRect;
    iNavigationNextRect = KEmptyRect;
    iNavigationPreviousRect = KEmptyRect;
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::CalculatePositionsL
// calculate the position of all visible cells in the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
TInt CListExpandableMultiRowWithoutIcon::CalculatePositionsL()
    {
    TInt lastCandidateIndex = iOwner->GetLastCandidateIndex();
	TInt count = iOwner->CandidateCount();

    if ( count > 0 )
        {
        TPoint setpoints;
        TBool candHLToR = (iOwner->CandHDirection() == CFepCtrlDropdownList::ECandFromLToR);
        candHLToR ? setpoints.SetXY(iContentRect.iTl.iX, iContentRect.iTl.iY) : 
                    setpoints.SetXY(iContentRect.iBr.iX, iContentRect.iTl.iY);

       	count = Min( count, lastCandidateIndex 
					+ ( iOwner->GetColNum() + 1 ) * iOwner->GetRowNum() );

        for ( TInt currentCellIndex = 0, 
        		   lastUsedCellCount = 0, 
        		   lastusedRow = 0; 
        	  lastCandidateIndex <= count; 
        	  lastCandidateIndex++ )
            {
            // setting a position for this candidate 
            CCandidate* candidate = iOwner->GetCandidate( lastCandidateIndex );

            if ( candidate )
                {
                // each candidate exist
                TInt leftUsedCellCount = candidate->CalculateDisplayTextL(iOwner->GetCellWidth(), iOwner->GetFont(), iOwner->GetColNum());
	            if (( currentCellIndex > iOwner->GetColNum() ) ||  
	                ((!candHLToR) && ((setpoints.iX < leftUsedCellCount*(iOwner->GetCellWidth() + (lastUsedCellCount-1)*iOwner->GetCellHorizontalMargin())))))
	                {
	                // The cell index moved the edge of right
	                lastusedRow++;
	                if ( lastusedRow >= iOwner->GetRowNum() )
	                    {
		                // The row index moved bottom
	                    break;
	                    }
	                    
	                currentCellIndex = 0;
	                
                    setpoints.iY = setpoints.iY + 
                                   iOwner->GetCellHeight() + 
                                   iOwner->GetCellVerticalMargin();

                    candHLToR ? (setpoints.iX = iContentRect.iTl.iX) : (setpoints.iX = iContentRect.iBr.iX);
	                }

	            lastUsedCellCount = candidate->SetPositionL(setpoints, 
													        iOwner->GetCellWidth(), 
	                                                        iOwner->GetCellHeight(), 
													        iOwner->GetFont(),
													        iOwner->GetCellHorizontalMargin());

	            currentCellIndex += lastUsedCellCount;
	            candHLToR? (setpoints.iX += lastUsedCellCount*(iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin())) : 
	                       (setpoints.iX -= lastUsedCellCount*(iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin()));
                } // if each candidate exist
            } // for-loop
        } // if candidate exist
         
    return lastCandidateIndex; // as lastCandidateIndex
    }

// -----------------------------------------------------------------------------
// CListExpandableMultiRowWithoutIcon::UpdateList
// update the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListExpandableMultiRowWithoutIcon::UpdateListL(TBool aDrawFlag, TBool aRecalculate )
    {
    if ( aRecalculate )
        {
        // Recalculate the position of all candidates
        iOwner->SetLastCandidateIndex( CalculatePositionsL() );
        }
    if(aDrawFlag)
        {
        Draw();
        iOwner->Invalidate(iRect);
        }
    }

// End Of File
