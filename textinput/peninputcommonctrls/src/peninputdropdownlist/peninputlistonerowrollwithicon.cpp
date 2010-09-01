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

#include "peninputlistonerowrollwithicon.h"
#include "peninputcandidate.h"
#include "peninputdropdownlist.h"
#include "peninputlayout.h"

const TInt KInvalidCandIndex = -1;

CListOneRowRollWithIcon* CListOneRowRollWithIcon::NewL(CListManager* aManager,
                                                       MFepCtrlDropdownListContext* aOwner)
    {
    CListOneRowRollWithIcon* self = new (ELeave) CListOneRowRollWithIcon(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CListOneRowRollWithIcon::~CListOneRowRollWithIcon()
    {
    }

CListOneRowRollWithIcon::CListOneRowRollWithIcon(CListManager* aManager, 
                                                 MFepCtrlDropdownListContext* aOwner):
                                                 CListOneRowWithIcon(aManager, aOwner)
    {
    }

// not needed anymore
// if candidate not enough to fill one row, 
// just dim next button, but not hide it
/*
void CListOneRowRollWithIcon::Draw()
    {
    CList::Draw();
    DrawCandidates();

    if (!iNextButtonDisable)
        {
        //Clear bg for iNavigationNextRect
        iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
        iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);

        iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());
        iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );                
        iOwner->Gc()->DrawRect( iNavigationNextRect );

        TRect rect = iNavigationNextRect;
        rect.Shrink(1, 1);

        iOwner->Gc()->DrawBitmapMasked(rect, 
                                       iBButtonDown, 
                                       iBButtonDown->SizeInPixels(), 
                                       iBButtonMaskDown, 
                                       ETrue);
        }

    iOwner->Invalidate(iRect);
    }
*/    

void CListOneRowRollWithIcon::HandlePointerDownL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDownL(aPoint);

    // If click in NextButton, and it is not disable, then do 
    if (iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable)
        {
		#ifdef RD_TACTILE_FEEDBACK
        static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
                        DoTactileFeedback(ETouchFeedbackBasic);        
		#endif //RD_TACTILE_FEEDBACK
        // If click in down button and the down button is enabled 
        iClickedRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
        iIndicateButtonClicked = ETrue;
        DrawSelection(iClickedRect, ETrue);
        }
    }

void CListOneRowRollWithIcon::HandlePointerUpL(const TPoint& aPoint)
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

            if ((iOwner->ListType() == CFepCtrlDropdownList::EListExpandableRollWithIcon) && 
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
    }

void CListOneRowRollWithIcon::HandlePointerDragL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);

    if (iIndicateButtonClicked)  // clicked rect is not outside the whole DDL
        {
        iMoveLatestRect = KEmptyRect;
        
        if (iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable)
            {
            // If click in down button and the down button is enabled	
            iMoveLatestRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
            }
        else if (iContentRect.Contains(aPoint))
            {
            TInt index = -1;  
            CCandidate* candidate = FindCandidate(aPoint, index);        
                
            if ( candidate )
                {
                // If click in candidates area  
                iMoveLatestRect.SetRect(candidate->GetRect().iTl, candidate->GetRect().Size());
                }
            else
                {
                iMoveLatestRect = KEmptyRect;
                }           
            }
            
        CompareRect(iMoveLatestRect, iClickedRect);
        }
    }
    
void CListOneRowRollWithIcon::SetHighlightCell(const TInt aCellIndex, 
                                               TBool aHighlight, 
                                               TBool aForceDraw)
    {
    if (aCellIndex < iOwner->GetLastCandidateIndex())
        {
        CList::SetHighlightCell(aCellIndex, aHighlight, aForceDraw);
        }
    else if ((iOwner->GetLastCandidateIndex() > 0) &&
             aHighlight)
        {
        TInt originalFirstCandIndex = iOwner->GetOriginalFirstCandIdx();
     
        iOwner->SetUpdateListReDrawFlag(EFalse);

        if ((iOwner->ListType() == CFepCtrlDropdownList::EListExpandableRollWithIcon) && 
            (originalFirstCandIndex != KInvalidCandIndex))
            {
            iOwner->AutoChangeActiveList(originalFirstCandIndex);
            }
        else
            {
            iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
            }    

        TRAP_IGNORE(iOwner->ActiveList()->SetNextPageHighlightCellAfterOnePageL(aCellIndex));
        
        iOwner->SetUpdateListReDrawFlag(ETrue);
        }
    }

// End Of File
