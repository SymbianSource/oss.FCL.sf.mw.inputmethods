/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Multi rows list with icon with bubble for drop-down list objects
*
*/

// Include 
#include <fbs.h>
#include <AknsUtils.h>
#include <AknLayoutDef.h>
#include <peninputdropdownlistcontext.h>
#include <peninputdropdownlist.h>
#include <aknsdrawutils.h>
#include "peninputlistmultirowwithouticonwithbubble.h"
#include "peninputlayout.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListMultiRowWithoutIconWithBubble 

// -----------------------------------------------------------------------------
// CListMultiRowWithoutIconWithBubble::CListMultiRowWithoutIconWithBubble
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowWithoutIconWithBubble::CListMultiRowWithoutIconWithBubble(CListManager* aManager, 
                                             MFepCtrlDropdownListContext *aOwner)
: CListExpandableMultiRowWithoutIcon(aManager, aOwner)
    {
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithoutIconWithBubble::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowWithoutIconWithBubble* CListMultiRowWithoutIconWithBubble::NewL(CListManager* aManager, 
                                                   MFepCtrlDropdownListContext *aOwner)
    {
    CListMultiRowWithoutIconWithBubble* self = new (ELeave) CListMultiRowWithoutIconWithBubble(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithoutIconWithBubble::~CListMultiRowWithoutIconWithBubble
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowWithoutIconWithBubble::~CListMultiRowWithoutIconWithBubble()
    {     
    }

    
// -----------------------------------------------------------------------------
// CListMultiRowWithoutIconWithBubble::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithoutIconWithBubble::HandlePointerDownL(const TPoint& aPoint)
    {		
    CList::HandlePointerDownL(aPoint);

    // also consider fuzzy boundry of drop down list
    TRect fuzzyrect = iRect;
    fuzzyrect.Grow(iOwner->FuzzyBoundry(), iOwner->FuzzyBoundry());

    if (!fuzzyrect.Contains(aPoint))
        {
        iOwner->SetCapture(EFalse);
        iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
        iOwner->ClearBubble();
        return;
        }
        
    if(iContentRect.Contains(aPoint))
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
// CListMultiRowWithoutIconWithBubble::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithoutIconWithBubble::HandlePointerDragL(const TPoint& aPoint)
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
    else 
        {// move to the aperture
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
// CListMultiRowWithoutIconWithBubble::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithoutIconWithBubble::HandlePointerUpL(const TPoint& aPoint)
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
        iOwner->AutoChangeActiveList( iOwner->GetFirstCandidateIndex() );
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
			        iOwner->AutoChangeActiveList( iOwner->GetFirstCandidateIndex() );
                    CCandidate* candidate = iOwner->GetCandidate(iClickedCandidateIndex);
                    if ( candidate )
                        {
                        iOwner->ReportCandidateSelectEventL(candidate->GetCandidate(),
                                                           iClickedCandidateIndex);
                        }
                    }
                }
            }            
       	iClickedRect =  KEmptyRect;
        }
    }
    

TBool CListMultiRowWithoutIconWithBubble::CompareRect(const TRect& aSrcRect, const TRect& aDesRect)
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
