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


#include <fbs.h>
#include <AknsUtils.h>
#include <AknLayoutDef.h>
#include <peninputdropdownlistcontext.h>

#include "peninputdropdownlist.h"
#include "peninputlayout.h"
#include "peninputlistmultirowrollwithiconwithbubble.h"


const TInt KInvalidCandIndex = -1;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListMultiRowRollWithIconWithBubble 

// -----------------------------------------------------------------------------
// CListMultiRowRollWithIconWithBubble::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowRollWithIconWithBubble* CListMultiRowRollWithIconWithBubble::NewL(CListManager* aManager, 
                                                           MFepCtrlDropdownListContext* aOwner)
    {
    CListMultiRowRollWithIconWithBubble* self = new (ELeave) CListMultiRowRollWithIconWithBubble(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListMultiRowRollWithIconWithBubble::CListMultiRowRollWithIconWithBubble
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowRollWithIconWithBubble::CListMultiRowRollWithIconWithBubble(CListManager* aManager, 
                                                     MFepCtrlDropdownListContext* aOwner):
                                                     CListMultiRowRollWithIcon(aManager, aOwner)
    { 
    }

// -----------------------------------------------------------------------------
// CListMultiRowRollWithIconWithBubble::~CListMultiRowRollWithIconWithBubble
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowRollWithIconWithBubble::~CListMultiRowRollWithIconWithBubble()
    {

    }

// -----------------------------------------------------------------------------
// CListMultiRowRollWithIconWithBubble::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowRollWithIconWithBubble::HandlePointerDownL(const TPoint& aPoint)
    {
    CList::HandlePointerDownL(aPoint);
    
    // if press outside of expanded list, close it
    // also consider fuzzy boundary
    TRect fuzzyrect = iRect;
    fuzzyrect.Grow(iOwner->FuzzyBoundry(), iOwner->FuzzyBoundry());

    if (!fuzzyrect.Contains(aPoint))
        {
        iOwner->SetCapture(EFalse);
        
        if (iOwner->OneRowStartCandIdx() != KInvalidCandIndex)
            {
            iOwner->AutoChangeActiveList(iOwner->OneRowStartCandIdx());
            }
        else
            {
            iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
            }    
        iOwner->ClearBubble();
        return;
        }

    // If click in navigation rect and NextButton is not disable, then do 
    if ( iNavigationRect.Contains(aPoint) )
        {      
        if ( iNavigationCloseRect.Contains(aPoint) && !iCloseButtonDisable )
            {
			#ifdef RD_TACTILE_FEEDBACK
            static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
                            DoTactileFeedback(ETouchFeedbackBasic);
			#endif
            iClickedRect.SetRect(iNavigationCloseRect.iTl, iNavigationCloseRect.iBr);
            iIndicateButtonClicked = ETrue;
            DrawSelection(iClickedRect, ETrue);                
            }
        else if (!iOwner->PageCtrlShown())
            {
            iClickedRect = KDummyRect;
            return;
            }
        else if ( iNavigationPreviousRect.Contains(aPoint) && !iPreviousButtonDisable )
            {
			#ifdef RD_TACTILE_FEEDBACK
            static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
                            DoTactileFeedback(ETouchFeedbackBasic); 
			#endif
            iClickedRect.SetRect(iNavigationPreviousRect.iTl, iNavigationPreviousRect.iBr);
            iIndicateButtonClicked = ETrue;
            DrawSelection(iClickedRect, ETrue);                
            }
        else if ( iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable )
            {
			#ifdef RD_TACTILE_FEEDBACK
            static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
                            DoTactileFeedback(ETouchFeedbackBasic);   
			#endif
            iClickedRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
            iIndicateButtonClicked = ETrue;
            DrawSelection(iClickedRect, ETrue);                
            }
        else
            {
            iClickedRect = KDummyRect;                                
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
// CListMultiRowRollWithIconWithBubble::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowRollWithIconWithBubble::HandlePointerDragL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);

    CCandidate* candidate = FindCandidate(aPoint, iClickedCandidateIndex);
    
    TBool isUpdateBubble = EFalse;
    if ( candidate )
        {
        if (iClickedRect != candidate->GetRect())
            {// move to the other candidate
            // set update prview bubble flag
            isUpdateBubble = ETrue;
            // remenber the previous cell rect
            iMoveLatestRect = iClickedRect;
            // get the new cell rect
            iClickedRect.SetRect(candidate->GetRect().iTl, candidate->GetRect().Size());
            }
        }
    else if ( iNavigationRect.Contains(aPoint) )
        {
        iOwner->ClearBubble();
        if ( iNavigationCloseRect.Contains(aPoint) && !iCloseButtonDisable &&
             iClickedRect !=  iNavigationCloseRect)
            {
            iMoveLatestRect = iClickedRect;
            iClickedRect.SetRect(iNavigationCloseRect.iTl, iNavigationCloseRect.iBr);
            CompareRect(iMoveLatestRect, iClickedRect);
            }
        else if (!iOwner->PageCtrlShown())
            {
            return;
            }
        else if ( iNavigationPreviousRect.Contains(aPoint) && !iPreviousButtonDisable &&
                  iClickedRect != iNavigationPreviousRect )
            {
            iMoveLatestRect = iClickedRect;
            iClickedRect.SetRect(iNavigationPreviousRect.iTl, iNavigationPreviousRect.iBr);
            CompareRect(iMoveLatestRect, iClickedRect);
            }
        else if ( iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable &&
                  iClickedRect != iNavigationNextRect )
            {
            iMoveLatestRect = iClickedRect;
            iClickedRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
            CompareRect(iMoveLatestRect, iClickedRect);
            }
        else if(!((iNavigationCloseRect.Contains(aPoint) && !iCloseButtonDisable) ||
                (iNavigationPreviousRect.Contains(aPoint) && !iPreviousButtonDisable) ||
                (iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable)))
            {
            // Move to navigation area but not valid navigation button
            TRect dummyrect = KDummyRect;
            if(iClickedRect != dummyrect)
                {
                iMoveLatestRect = iClickedRect;
                iClickedRect = KDummyRect;
                CompareRect(iMoveLatestRect, iClickedRect);
                }
            }
        }
    else
        {// outside the list
        // remember the previous cell 
        iMoveLatestRect = iClickedRect;
        // set the new cell to empty
        iClickedRect = KDummyRect;
        // redraw the cell only when drag outside at the first time
        if(iMoveLatestRect != iClickedRect)
            {
            iOwner->ClearBubble();
            DrawSelection(iMoveLatestRect, EFalse);
            DrawHighightCell(iMoveLatestRect, ETrue);            
            }
        }
    
    if(candidate && isUpdateBubble)
        {
        iOwner->ClearBubble();
        CompareRect(iMoveLatestRect, iClickedRect);
        // Show preview bubble when drag into a candidate cell
        // Make sure show the bubble after redraw clicked cell
        iOwner->DrawBubble(iClickedRect, candidate->GetCandidate());
        }
    }

// -----------------------------------------------------------------------------
// CListMultiRowRollWithIconWithBubble::HandlePointerUpL
// handle pointer up 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowRollWithIconWithBubble::HandlePointerUpL(const TPoint& aPoint)
    {
    // Reset the button clicked flag to EFalse
    iIndicateButtonClicked = EFalse;
   
    iOwner->ClearBubble();
#ifdef __WINS__
    // in emulator, double click will be treated as:
    // 1 pointer down and 2 pointer up, which will cause error
    if (iClickedRect.IsEmpty() && ((CFepUiBaseCtrl*)iOwner)->PointerDown()) 
#else
    if (iClickedRect.IsEmpty())
#endif // __WINS__
        {
        iOwner->SetCapture(EFalse);
        // Change active list object ie. close the dd-list
        
        // txin modify
        //iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
        if (iOwner->OneRowStartCandIdx() != KInvalidCandIndex)
            {
            iOwner->AutoChangeActiveList(iOwner->OneRowStartCandIdx());
            }
        else
            {
            iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
            }    
        // end
        }
    else
        {
        if ( iContentRect.Contains(aPoint) )
            {
            if ( iClickedRect.Contains(aPoint) )
                {
                // Redraw the selected aread    
                DrawSelection(iClickedRect, EFalse);
                
                iOwner->SetCapture(EFalse);
                
                // We have selected a candidate and iClickedCandidateIndex is the selected Index.
                // Send the candidate to others
                if (iClickedCandidateIndex != KErrNotFound)
                    {         
                    CCandidate* candidate = iOwner->GetCandidate(iClickedCandidateIndex);

                    if (candidate)
                        {
                        iOwner->ReportCandidateSelectEventL(candidate->GetCandidate(),
                                                           iClickedCandidateIndex);
                        }

			        TInt lastIdxOfLine = StartCandIdxOfLine(iClickedCandidateIndex);

                    TInt originalFirstCandIdx = iOwner->GetFirstCandidateIndex();
                    TInt originalLastCandIdx = iOwner->GetLastCandidateIndex();
                    
			        if (lastIdxOfLine == KInvalidCandIndex)
			            {
			            lastIdxOfLine = iOwner->GetFirstCandidateIndex();
			            }
			            
                    iOwner->SetOneRowStartCandIdx(lastIdxOfLine);
			        
                    iOwner->AutoChangeActiveList(lastIdxOfLine);
                    
                    iOwner->SetOriginalFirstCandIdx(originalFirstCandIdx);
                    }
                }
            }
        else if ( iNavigationCloseRect.Contains(aPoint) )
            {
            if ( iNavigationCloseRect == iClickedRect )
                {
                iOwner->SetCapture(EFalse);
		        
		        if (iOwner->OneRowStartCandIdx() != KInvalidCandIndex)
		            {
                    iOwner->AutoChangeActiveList(iOwner->OneRowStartCandIdx());
		            }
		        else
		            {
		            iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
		            }    
                }
            }
        else if (!iOwner->PageCtrlShown())
            {
            iClickedRect =  KEmptyRect;
            return;
            }
        else if ( iNavigationNextRect.Contains(aPoint) )
            {
            if ( iNavigationNextRect == iClickedRect )
                {
                iOwner->NotifyGetNextPage();
                UpdateListL();
                }
            }
        else if ( iNavigationPreviousRect.Contains(aPoint) )
            {
            if ( iNavigationPreviousRect == iClickedRect )
                {
                // Change page to previous
                iOwner->NotifyGetPreviousPage(); // do nothing, just to avoid clear candidate

                TInt firstIndex = 0;
                TInt lastIndex = 0;
                
                if (iOwner->GetPages()->GetPreviousIndex(iOwner->GetLastCandidateIndex(), 
                                                         firstIndex,lastIndex))    
                    {
                    iOwner->SetFirstCandidateIndex(firstIndex);
                    iOwner->SetLastCandidateIndex(lastIndex);
                    }
                    
                UpdateListL(ETrue, EFalse);
                }             
            }

       	iClickedRect =  KEmptyRect;
        }
    }

// -----------------------------------------------------------------------------
// CListMultiRowRollWithIconWithBubble::CompareRect
// Compare src and des rect 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CListMultiRowRollWithIconWithBubble::CompareRect(const TRect& aSrcRect, const TRect& aDesRect)
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
