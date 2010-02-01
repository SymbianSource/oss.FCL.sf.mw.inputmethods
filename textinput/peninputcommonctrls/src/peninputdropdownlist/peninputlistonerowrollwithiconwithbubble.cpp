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


#include <peninputdropdownlistcontext.h>

#include "peninputlistonerowrollwithiconwithbubble.h"
#include "peninputcandidate.h"
#include "peninputdropdownlist.h"
#include "peninputlayout.h"

const TInt KInvalidCandIndex = -1;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListOneRowRollWithIconWithBubble 

// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowRollWithIconWithBubble* CListOneRowRollWithIconWithBubble::NewL(CListManager* aManager,
                                                       MFepCtrlDropdownListContext* aOwner)
    {
    CListOneRowRollWithIconWithBubble* self = new (ELeave) CListOneRowRollWithIconWithBubble(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::~CListOneRowRollWithIconWithBubble
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowRollWithIconWithBubble::~CListOneRowRollWithIconWithBubble()
    {
    }

// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::CListOneRowRollWithIconWithBubble
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowRollWithIconWithBubble::CListOneRowRollWithIconWithBubble(CListManager* aManager, 
                                                 MFepCtrlDropdownListContext* aOwner):
                                                 CListOneRowRollWithIcon(aManager, aOwner)
    {
    }

// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::HandlePointerDownL
// handle pointer down function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowRollWithIconWithBubble::HandlePointerDownL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDownL(aPoint);

    // If click in NextButton, and it is not disable, then do 
    if (iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable)
        {
		#ifdef RD_TACTILE_FEEDBACK
        static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
                        DoTactileFeedback(ETouchFeedbackBasic);  
		#endif
        
        // If click in down button and the down button is enabled 
        iClickedRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
        iIndicateButtonClicked = ETrue;
        DrawSelection(iClickedRect, ETrue);
        }
    else
        {
        TInt index = -1;
        CCandidate* candidate = FindCandidate(aPoint, index);
        if(candidate)
            {
            iOwner->DrawBubble(iClickedRect, candidate->GetCandidate());
            }
        }
    }

// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowRollWithIconWithBubble::HandlePointerUpL(const TPoint& aPoint)
    {
    iIndicateButtonClicked = EFalse;

    if (!iClickedRect.IsEmpty())
        {
        // Find out which rectangle was clicked and save it for 
        // later usage (HandlePointerUpL)
        if (iContentRect.Contains(aPoint))
            {
            // The point is inside the content rect
            // Check whether the point is inside the same last clicked
            // candidate rectangle
            if (iClickedRect.Contains(aPoint))
                {
                if (iClickedCandidateIndex != KErrNotFound)
                    {
                    // Redraw the selected aread  
                    DrawSelection(iClickedRect, EFalse);

                    // We have selected a candidate and iClickedCandidateIndex is the selected Index.
                    // Then send message to plugin
                    CCandidate* candidate = iOwner->GetCandidate(iClickedCandidateIndex);
                    if ( candidate )
                        {
                        iOwner->ReportCandidateSelectEventL(candidate->GetCandidate(),
                                                           iClickedCandidateIndex);
                        }
                    }
                }
            }
        else if ((iNavigationNextRect == iClickedRect) && !iNextButtonDisable)
            {
            TInt originalFirstCandIndex = iOwner->GetOriginalFirstCandIdx();

            if ((iOwner->ListType() == CFepCtrlDropdownList::EListExpandableRollWithIconWithBubble) && 
                (originalFirstCandIndex != KInvalidCandIndex))
                {
                iOwner->AutoChangeActiveList(originalFirstCandIndex);
                }
            else
                {
                iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
                }    

            iOwner->SetCapture(ETrue);    
            }

        iClickedRect =  KEmptyRect;   
        }
    iOwner->ClearBubble();
    }

// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowRollWithIconWithBubble::HandlePointerDragL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);

    if (iIndicateButtonClicked)  // clicked rect is not outside the whole DDL
        {
        if ( iNavigationRect.Contains(aPoint) )
            {
            iOwner->ClearBubble();
            
            if ( iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable &&
                 iNavigationNextRect != iClickedRect )
                {
                // If click in down button and the down button is enabled
                iMoveLatestRect = iClickedRect;                
                iClickedRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
                }  
            }
        else if (iContentRect.Contains(aPoint))
            { 
            CCandidate* candidate = FindCandidate(aPoint, iClickedCandidateIndex);        
                
            if ( candidate )
                {
                // If click in candidates area
                if(iClickedRect != candidate->GetRect())
                    {
                    // Reset clicked rect when move to other candidate.
                    iMoveLatestRect = iClickedRect;
                    iClickedRect.SetRect(candidate->GetRect().iTl, candidate->GetRect().Size());
                    
                    iOwner->ClearBubble();
                    iOwner->DrawBubble(iClickedRect, candidate->GetCandidate());                    
                    }
                }
            else
                {
                iOwner->ClearBubble();
                iMoveLatestRect = iClickedRect;
                
                iClickedRect = KEmptyRect;
                }           
            }
        else
            {
            CompareRect(iClickedRect, iMoveLatestRect);
            iOwner->ClearBubble();
            iClickedRect = KEmptyRect;
            iMoveLatestRect = iClickedRect;
            }
            
        CompareRect(iMoveLatestRect, iClickedRect);
        }
    }
 
// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::HandlePointerLeave
// handle pointer leave function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowRollWithIconWithBubble::HandlePointerLeave(const TPoint& /*aPoint*/)
    {
    iMoveLatestRect = iClickedRect;
    iClickedRect = KEmptyRect;
    DrawSelection( iMoveLatestRect, EFalse );
    DrawHighightCell(iMoveLatestRect, ETrue);
    }

// -----------------------------------------------------------------------------
// CListOneRowRollWithIconWithBubble::CompareRect
// Compare src and des rect 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TBool CListOneRowRollWithIconWithBubble::CompareRect(const TRect& aSrcRect, const TRect& aDesRect)
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
