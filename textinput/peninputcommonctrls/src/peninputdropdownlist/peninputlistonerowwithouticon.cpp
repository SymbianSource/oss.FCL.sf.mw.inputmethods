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
* Description:  One rows list with icon for drop-down list objects
*
*/


// Include 
#include "peninputlistonerowwithouticon.h"
#include "peninputcandidate.h"

#include <peninputdropdownlistcontext.h>
#include <peninputdropdownlist.h>

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListOneRowWithoutIcon 

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::CListOneRowWithoutIcon
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithoutIcon::CListOneRowWithoutIcon(CListManager* aManager, 
                                               MFepCtrlDropdownListContext* aOwner)
:CList(aManager, aOwner)
    {
    iIsMultiRows = EFalse;
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithoutIcon* CListOneRowWithoutIcon::NewL(CListManager* aManager,
                                                     MFepCtrlDropdownListContext* aOwner)
    {
    CListOneRowWithoutIcon* self = new (ELeave) CListOneRowWithoutIcon(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::~CListOneRowWithoutIcon
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithoutIcon::~CListOneRowWithoutIcon()
    {   
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIcon::HandlePointerDownL(const TPoint& aPoint)
    {    
    // Pass the event first to the base     
    CList::HandlePointerDownL(aPoint);
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIcon::HandlePointerDragL(const TPoint& aPoint)
    { 
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);
    
    // If the mouse have click in the area of DDL
    if ( iIndicateButtonClicked )  
        {
        TInt index = -1;
        
        CCandidate* candidate = FindCandidate(aPoint, index);
        iMoveLatestRect = KEmptyRect;
        
        if ( candidate )
            {
            iMoveLatestRect.SetRect(candidate->GetRect().iTl, candidate->GetRect().Size());
            }          
        
        // Compare the two rect, then decide draw it or not
        CompareRect(iMoveLatestRect, iClickedRect);
        }
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIcon::HandlePointerUpL(const TPoint& aPoint)
    {
    if ( !iClickedRect.IsEmpty() )
        {
        // Find out which rectangle was clicked and save it for 
        // later usage (HandlePointerUpL)
        if ( iContentRect.Contains( aPoint ) )
            {
            if ( iClickedRect.Contains( aPoint ) )
                {
                if ( iClickedCandidateIndex != KErrNotFound )
                    {
                    // Redraw the selection rect  
                    DrawSelection( iClickedRect, EFalse );
                    
                    // We have selected a candidate
                    // Send message to plugin         
                    CCandidate* candidate = iOwner->GetCandidate(iClickedCandidateIndex);
                    if ( candidate )
                        {
                        iOwner->ReportCandidateSelectEventL(candidate->GetCandidate(),
                                                           iClickedCandidateIndex); 
                        }
                           
                    }                
               }
                
            iIndicateButtonClicked = EFalse;
            }
            
        iClickedRect = KEmptyRect;               
        }
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::Draw
// draw the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIcon::Draw()
    {
    CList::Draw();
    
    // Draw all visible candidates
    DrawCandidates();
    DrawSelection(iClickedRect, ETrue);
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::CalculateAreas
// Calculate all kinds of rectangles
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIcon::CalculateAreas()
    {
    // Calculate the rect of the list
    iRect.iTl = iOwner->GetTopLeftPoint();
    TInt width = iOwner->GetCellWidth() * iOwner->GetColNum() + iOwner->GetNaviWidth() + 
                 (iOwner->GetColNum() + 2) * iOwner->GetCellHorizontalMargin();
    TInt height = iOwner->GetCellHeight() + 2 * iOwner->GetCellVerticalMargin();                 
    
    iRect.SetSize(TSize(width,height));

    // Calculate the rect of the content
    iContentRect.iTl = TPoint(iRect.iTl.iX + iOwner->GetCellHorizontalMargin(),
                              iRect.iTl.iY + iOwner->GetCellVerticalMargin());
    iContentRect.SetSize(TSize(width - 2 * iOwner->GetCellHorizontalMargin(),
                               iOwner->GetCellHeight()));

    // Calculate the rect of the navigation area
    iNavigationRect = TRect(0,0,0,0);

    // Calculate the rect of the down button
    iNavigationNextRect = TRect(0,0,0,0);

    // Calculate the rect of the up button
    iNavigationPreviousRect = TRect(0,0,0,0);
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::CalculatePositionsL
// calculate the position of all visible cells in the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CListOneRowWithoutIcon::CalculatePositionsL()
    {
    TInt t = 0;
    TPoint setpoints;

    const TInt count = iOwner->CandidateCount();
    
    // First store the last index
    iOwner->SetFirstCandidateIndex ( iOwner->GetLastCandidateIndex() );        
    
    if ( count > 0 )
        {
        // Initial the position to iContentRect
        /*
        setpoints.iX = iContentRect.iTl.iX;
        setpoints.iY = iContentRect.iTl.iY;
        */
        TBool candHLToR = (iOwner->CandHDirection() == CFepCtrlDropdownList::ECandFromLToR);
        candHLToR ? setpoints.SetXY(iContentRect.iTl.iX, iContentRect.iTl.iY) :
                    setpoints.SetXY(iContentRect.iBr.iX, iContentRect.iTl.iY);

        TInt cellNumberInOneRow = iOwner->GetColNum() + 1;
        TInt lastUsedCellCount = 0;
        TInt currentCellIndex = 0;
        TInt newCount = 0;
        
        newCount = Min(count,iOwner->GetLastCandidateIndex() + cellNumberInOneRow);   
                
        for( t = iOwner->GetLastCandidateIndex(); t < newCount ; t++ )
            {
            // setting a position for this candidate 
            CCandidate* candidate = iOwner->GetCandidate(t);

            if ( !candidate )
                {
                continue;
                }
                
            lastUsedCellCount = candidate->SetPositionL(setpoints,
                                                        iOwner->GetCellWidth(),
                                                        iOwner->GetCellHeight(),
                                                        iOwner->GetFont(),
                                                        iOwner->GetCellHorizontalMargin()); 
            currentCellIndex += lastUsedCellCount;
            
            if ( currentCellIndex > ( iOwner->GetColNum() + 1 ) )
                {
                // If no space to contain other cells 
                break;
                }

            // Recalcuate the x position
            candHLToR? (setpoints.iX += (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin()) * lastUsedCellCount) : 
                       (setpoints.iX -= (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin()) * lastUsedCellCount);
            } // for-loop
        } // if candidate exist 
    
    return t; // as lastCandidateIndex
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIcon::UpdateList
// update the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIcon::UpdateListL(TBool aDrawFlag, TBool aRecalculate)
    {
    if ( aRecalculate )
        {
        // Recalculate the position of all candidates
        iOwner->SetLastCandidateIndex(CalculatePositionsL());    
        }
    if(aDrawFlag)    
        {
        Draw();
        iOwner->Invalidate(iRect);
        }
    }

// End Of File
