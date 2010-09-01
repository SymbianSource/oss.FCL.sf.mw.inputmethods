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
#include <fbs.h>
#include <AknsUtils.h>
#include <AknLayoutDef.h>
#include <AknsDrawUtils.h> 

#include <peninputdropdownlistcontext.h>

#include "peninputlistonerowwithiconwithbubble.h"
#include "peninputcandidate.h"
#include "peninputdropdownlist.h"
#include "peninputlayout.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListOneRowWithIconWithBubble 

// -----------------------------------------------------------------------------
// CListOneRowWithIconWithBubble::CListOneRowWithIconWithBubble
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithIconWithBubble::CListOneRowWithIconWithBubble(CListManager* aManager, 
                                         MFepCtrlDropdownListContext *aOwner)
: CListOneRowWithIcon(aManager, aOwner)
    {
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIconWithBubble::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithIconWithBubble* CListOneRowWithIconWithBubble::NewL(CListManager* aManager, 
                                               MFepCtrlDropdownListContext *aOwner)
    {
    CListOneRowWithIconWithBubble* self = new (ELeave) CListOneRowWithIconWithBubble(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIconWithBubble::~CListOneRowWithIconWithBubble
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithIconWithBubble::~CListOneRowWithIconWithBubble()
    {     
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIconWithBubble::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIconWithBubble::HandlePointerDownL(const TPoint& aPoint)
    {
    // Pass the event first to the base     
    CList::HandlePointerDownL(aPoint);

    // If click in navigation rect and NextButton is not disable, then do 
    if ( iNavigationRect.Contains(aPoint) )
        {      
        if ( (iOwner->GetLastCandidateIndex() < iOwner->CandidateCount()) 
             || ( iOwner->GetPages()->PageCount() > 1 ) )
            {
			#ifdef RD_TACTILE_FEEDBACK
            static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
                DoTactileFeedback(ETouchFeedbackBasic);
			#endif
            }        
        
        if ( iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable )
            {
            // If click in down button and the down button is enabled 
            iClickedRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
            iIndicateButtonClicked = ETrue;
            DrawSelection(iClickedRect, ETrue);                
            } 
        else if ( iNavigationPreviousRect.Contains(aPoint) && !iPreviousButtonDisable )
            {
            // If click in up button and the up button is enabled                 
            iClickedRect.SetRect(iNavigationPreviousRect.iTl, iNavigationPreviousRect.iBr);
            iIndicateButtonClicked = ETrue;
            DrawSelection(iClickedRect, ETrue);                
            }
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
// CListOneRowWithIconWithBubble::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIconWithBubble::HandlePointerDragL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);

    if ( iIndicateButtonClicked )  // clicked rect is not outside the whole DDL
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
            else if ( iNavigationPreviousRect.Contains(aPoint) && !iPreviousButtonDisable &&
                      iNavigationPreviousRect != iClickedRect )
                {
                // If click in up button and the up button is enabled
                iMoveLatestRect = iClickedRect;                    
                iClickedRect.SetRect(iNavigationPreviousRect.iTl, iNavigationPreviousRect.iBr);
                }  
            CompareRect(iMoveLatestRect, iClickedRect);
            }
        else if ( iContentRect.Contains(aPoint) )
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
                CompareRect(iMoveLatestRect, iClickedRect);
                }            
            }
        else
            {
            CompareRect(iClickedRect, iMoveLatestRect);
            iOwner->ClearBubble();
            iClickedRect = KEmptyRect;
            iMoveLatestRect = iClickedRect;
            
            CompareRect(iMoveLatestRect, iClickedRect);
            }
        }
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIconWithBubble::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIconWithBubble::HandlePointerUpL(const TPoint& aPoint)
    {
    // Reset the button clicked flag to EFalse
    iIndicateButtonClicked = EFalse;

    if ( !iClickedRect.IsEmpty() )
        {
        // Find out which rectangle was clicked and save it for 
        // later usage (HandlePointerUpL)
        if ( iContentRect.Contains(aPoint) )
            {
            // The point is inside the content rect
            // Check whether the point is inside the same last clicked
            // candidate rectangle
             
            if ( iClickedRect.Contains(aPoint) )
                {
                if ( iClickedCandidateIndex != KErrNotFound )
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
        else if ( iNavigationRect.Contains(aPoint) )
            {
            if ( iNavigationRect == iClickedRect ) 
                {
                iOwner->AutoChangeActiveList( iOwner->GetFirstCandidateIndex() );
                iOwner->SetCapture(ETrue);
                }
            }

        iClickedRect =  KEmptyRect;   
        }
    iOwner->ClearBubble();    
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIconWithBubble::HandlePointerLeave
// handle pointer leave function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIconWithBubble::HandlePointerLeave(const TPoint& /*aPoint*/)
    {
    iMoveLatestRect = iClickedRect;
    iClickedRect = KEmptyRect;
    DrawSelection( iMoveLatestRect, EFalse );
    DrawHighightCell(iMoveLatestRect, ETrue);
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIconWithBubble::CompareRect
// Compare src and des rect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CListOneRowWithIconWithBubble::CompareRect(const TRect& aSrcRect, const TRect& aDesRect)
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
