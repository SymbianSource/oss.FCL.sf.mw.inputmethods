/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/


// Include 
#include "peninputlistonerowwithouticonwithbubble.h"
#include "peninputcandidate.h"

#include <peninputdropdownlistcontext.h>
#include <peninputdropdownlist.h>

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListOneRowWithoutIconWithBubble 

// -----------------------------------------------------------------------------
// CListOneRowWithoutIconWithBubble::CListOneRowWithoutIconWithBubble
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithoutIconWithBubble::CListOneRowWithoutIconWithBubble(CListManager* aManager, 
                                               MFepCtrlDropdownListContext* aOwner)
:CListOneRowWithoutIcon(aManager, aOwner)
    {
    iIsMultiRows = EFalse;
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIconWithBubble::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithoutIconWithBubble* CListOneRowWithoutIconWithBubble::NewL(CListManager* aManager,
                                                     MFepCtrlDropdownListContext* aOwner)
    {
    CListOneRowWithoutIconWithBubble* self = new (ELeave) CListOneRowWithoutIconWithBubble(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIconWithBubble::~CListOneRowWithoutIconWithBubble
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithoutIconWithBubble::~CListOneRowWithoutIconWithBubble()
    {   
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIconWithBubble::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIconWithBubble::HandlePointerDownL(const TPoint& aPoint)
    {    
    // Pass the event first to the base     
    CList::HandlePointerDownL(aPoint);
    
    TInt index = -1;
    CCandidate* candidate = FindCandidate(aPoint, index);
    if(candidate)
        iOwner->DrawBubble(iClickedRect, candidate->GetCandidate());
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIconWithBubble::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIconWithBubble::HandlePointerDragL(const TPoint& aPoint)
    { 
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);
    
    // If the mouse have click in the area of DDL
    if ( iIndicateButtonClicked )  
        {
        CCandidate* candidate = FindCandidate(aPoint, iClickedCandidateIndex);
        
        if ( candidate )
            {
            if(iClickedRect != candidate->GetRect())
                {
                iMoveLatestRect = iClickedRect;
                iClickedRect.SetRect(candidate->GetRect().iTl, candidate->GetRect().Size());
                
                // Compare the two rect, then decide draw it or not
                CompareRect(iMoveLatestRect, iClickedRect);               
                
                iOwner->ClearBubble();
                iOwner->DrawBubble(iClickedRect, candidate->GetCandidate());
                }
            }
        else
            {
            iOwner->ClearBubble();
            
            iMoveLatestRect = iClickedRect;
            iClickedRect = KEmptyRect;
            
            // Compare the two rect, then decide draw it or not
            CompareRect(iMoveLatestRect, iClickedRect);
            }
        }
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIconWithBubble::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIconWithBubble::HandlePointerUpL(const TPoint& aPoint)
    {
    if ( !iClickedRect.IsEmpty() )
        {
        iOwner->ClearBubble();
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
// CListOneRowWithoutIconWithBubble::HandlePointerLeave
// handle pointer leave function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithoutIconWithBubble::HandlePointerLeave(const TPoint& /*aPoint*/)
    {
    iMoveLatestRect = iClickedRect;
    iClickedRect = KEmptyRect;
    DrawSelection( iMoveLatestRect, EFalse );
    DrawHighightCell(iMoveLatestRect, ETrue);
    }

// -----------------------------------------------------------------------------
// CListOneRowWithoutIconWithBubble::CompareRect
// Compare src and des rect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CListOneRowWithoutIconWithBubble::CompareRect(const TRect& aSrcRect, const TRect& aDesRect)
    {
    if(aSrcRect != aDesRect)  
        {
        DrawSelection(aDesRect, ETrue);
        DrawSelection(aSrcRect, EFalse);
        DrawHighightCell(aSrcRect, ETrue);
        }
          
    return EFalse;
    }

// End Of File
