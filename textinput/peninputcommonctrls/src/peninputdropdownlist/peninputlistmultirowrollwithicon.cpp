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
#include "peninputlistmultirowrollwithicon.h"

// added by txin
const TInt KInvalidCandIndex = -1;
// end adding

CListMultiRowRollWithIcon* CListMultiRowRollWithIcon::NewL(CListManager* aManager, 
                                                           MFepCtrlDropdownListContext* aOwner)
    {
    CListMultiRowRollWithIcon* self = new (ELeave) CListMultiRowRollWithIcon(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
CListMultiRowRollWithIcon::~CListMultiRowRollWithIcon()
    {
    iLines.Reset();
    iLines.Close();
    }
    
void CListMultiRowRollWithIcon::HandlePointerDownL(const TPoint& aPoint)
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
			#endif //RD_TACTILE_FEEDBACK 
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
			#endif //RD_TACTILE_FEEDBACK
            iClickedRect.SetRect(iNavigationPreviousRect.iTl, iNavigationPreviousRect.iBr);
            iIndicateButtonClicked = ETrue;
            DrawSelection(iClickedRect, ETrue);                
            }
        else if ( iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable )
            {
			#ifdef RD_TACTILE_FEEDBACK
            static_cast<CFepCtrlDropdownList*>(iOwner)->UiLayout()->
                            DoTactileFeedback(ETouchFeedbackBasic);
			#endif //RD_TACTILE_FEEDBACK
            iClickedRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
            iIndicateButtonClicked = ETrue;
            DrawSelection(iClickedRect, ETrue);                
            }
        else
            {
            iClickedRect = KDummyRect;                                
            }
        }
    }

void CListMultiRowRollWithIcon::HandlePointerUpL(const TPoint& aPoint)
    {
    // Reset the button clicked flag to EFalse
    iIndicateButtonClicked = EFalse;
   
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
    
void CListMultiRowRollWithIcon::SetNextPageHighlightCellAfterOnePageL(const TInt aCellIndex)
    {
    while (aCellIndex >= iOwner->GetLastCandidateIndex())
        {
        UpdateListL();
        }
    
    TInt lastIdxOfLine = StartCandIdxOfLine(aCellIndex);

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

CListMultiRowRollWithIcon::CListMultiRowRollWithIcon(CListManager* aManager, 
                                                     MFepCtrlDropdownListContext* aOwner):
                                                     CListMultiRowWithIcon(aManager, aOwner)
    {
    iIsMultiRows = ETrue; 
    }

TInt CListMultiRowRollWithIcon::StartCandIdxOfLine(TInt aCandIndex)
    {
    TInt lineCnt = iLines.Count();

    for (TInt i = 0; i < lineCnt; i++)
        {
        if ((aCandIndex >= iLines[i].iStartIdx) && (aCandIndex <= iLines[i].iEndIdx))
            {
            return iLines[i].iStartIdx;
            }
        }
    
    return KInvalidCandIndex;
    }
    
void CListMultiRowRollWithIcon::ResetStatus()
    {
    CList::ResetStatus();
    iLines.Reset();
    }

TInt CListMultiRowRollWithIcon::CalculatePositionsL()
    {
    TLineCandidate lineCand;

    TInt t = 0;
    TPoint setpoints;
    TInt count = iOwner->CandidateCount();
    CCandidate* candidate = NULL;
    
    if ( count > 0 )
        {     
        // modified by txin
        // original
        /*
        setpoints.iX = iContentRect.iTl.iX;
        setpoints.iY = iContentRect.iTl.iY;
        */

        TBool candHLToR = (iOwner->CandHDirection() == CFepCtrlDropdownList::ECandFromLToR); // horizontal direction
        candHLToR ? setpoints.SetXY(iContentRect.iTl.iX, iContentRect.iTl.iY) : 
            setpoints.SetXY(iContentRect.iBr.iX, iContentRect.iTl.iY);
        // end modify

        TInt currentCellIndex = 0;
        TInt lastUsedCellCount = 0;
        TInt lastusedRow = 0;
        iOwner->SetFirstCandidateIndex (iOwner->GetLastCandidateIndex());         
        count = Min(count, iOwner->GetLastCandidateIndex() + 
                           iOwner->GetColNum() * iOwner->GetRowNum() );   

        lineCand.iStartIdx = iOwner->GetLastCandidateIndex();
                
        for(t = iOwner->GetLastCandidateIndex(); t < count ; t++)
            {   
            candidate = iOwner->GetCandidate(t);

            if (!candidate)
                {
                continue;
                }
                            
            lastUsedCellCount = candidate->SetPositionL(setpoints,
                                                        iOwner->GetCellWidth(),
                                                        iOwner->GetCellHeight(),
                                                        iOwner->GetFont(),
                                                        iOwner->GetCellHorizontalMargin());
            currentCellIndex += lastUsedCellCount;
            if ( currentCellIndex > iOwner->GetColNum() )
                {
                lastusedRow++;                
                if ( lastusedRow >= iOwner->GetRowNum() )
                    {
                    break;
                    }
                    
                currentCellIndex = 0;
                setpoints.iY = setpoints.iY + 
                               iOwner->GetCellHeight() + 
                               iOwner->GetCellVerticalMargin();
                
                candHLToR ? (setpoints.iX = iContentRect.iTl.iX) : (setpoints.iX = iContentRect.iBr.iX);

                lastUsedCellCount = candidate->SetPositionL(setpoints,
                                                            iOwner->GetCellWidth(),
                                                            iOwner->GetCellHeight(),
                                                            iOwner->GetFont(),
                                                            iOwner->GetCellHorizontalMargin()); 
                currentCellIndex += lastUsedCellCount; 
                
                lineCand.iEndIdx = t - 1;
                iLines.Append(lineCand);
                
                lineCand.iStartIdx = t;

                candHLToR ? (setpoints.iX += lastUsedCellCount * (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin())) : 
                            (setpoints.iX -= lastUsedCellCount * (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin()));
                }
               else
                {
                candHLToR ? (setpoints.iX += lastUsedCellCount * (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin())) : 
                            (setpoints.iX -= lastUsedCellCount * (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin()));
                }                
            } // for-loop
        } // if candidate exist

    lineCand.iEndIdx = t-1;
    iLines.Append(lineCand);        

    return t; // as lastCandidateIndex
    }
    
void CListMultiRowRollWithIcon::UpdateListL(TBool aDrawFlag, TBool aRecalculate)
    {
    RefreshListL(aRecalculate);
    
    if (iOwner->GetUpdateListRedrawFlag() && aDrawFlag )
        {
        Draw();
		iOwner->Invalidate(iRect);
        }
    }

// End Of File
