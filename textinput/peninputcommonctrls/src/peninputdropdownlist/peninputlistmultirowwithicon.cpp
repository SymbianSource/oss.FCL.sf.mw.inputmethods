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
* Description:  Multi rows list with icon for drop-down list objects
*
*/


// Include 
#include <fbs.h>
#include <AknsUtils.h>
#include <AknLayoutDef.h>
#include <peninputdropdownlistcontext.h>
#include <peninputdropdownlist.h>
#include <AknsDrawUtils.h> 
#include "peninputlistmultirowwithicon.h"
#include "peninputlayout.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListMultiRowWithIcon 

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::CListMultiRowWithIcon
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowWithIcon::CListMultiRowWithIcon(CListManager* aManager, 
                                             MFepCtrlDropdownListContext *aOwner)
: CList(aManager, aOwner)
    {
    iIsMultiRows = ETrue; 
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowWithIcon* CListMultiRowWithIcon::NewL(CListManager* aManager, 
                                                   MFepCtrlDropdownListContext *aOwner)
    {
    CListMultiRowWithIcon* self = new (ELeave) CListMultiRowWithIcon(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::~CListMultiRowWithIcon
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListMultiRowWithIcon::~CListMultiRowWithIcon()
    {     
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithIcon::HandlePointerDownL(const TPoint& aPoint)
    {
    CList::HandlePointerDownL(aPoint);

    // also consider fuzzy boundry of drop down list
    TRect fuzzyrect = iRect;
    fuzzyrect.Grow(iOwner->FuzzyBoundry(), iOwner->FuzzyBoundry());

    if (!fuzzyrect.Contains(aPoint))
        {
        iOwner->SetCapture(EFalse);
        iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
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

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithIcon::HandlePointerDragL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);

    if ( !iClickedRect.IsEmpty() )  // clicked rect is not outside the whole DDL
        {        
        TInt index = -1;
        CCandidate* candidate = FindCandidate(aPoint, index);
        
        iMoveLatestRect = KEmptyRect;
        
        if ( candidate )
            {
            iMoveLatestRect.SetRect(candidate->GetRect().iTl, candidate->GetRect().Size());
            }
        else if ( iNavigationRect.Contains(aPoint) )
            {
            if ( iNavigationCloseRect.Contains(aPoint) && !iCloseButtonDisable )
                {
                iMoveLatestRect.SetRect(iNavigationCloseRect.iTl, iNavigationCloseRect.iBr);
                }
            else if (!iOwner->PageCtrlShown())
                {
                return;
                }
            else if ( iNavigationPreviousRect.Contains(aPoint) && !iPreviousButtonDisable )
                {
                iMoveLatestRect.SetRect(iNavigationPreviousRect.iTl, iNavigationPreviousRect.iBr);
                }
            else if ( iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable )
                {
                iMoveLatestRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
                }
            }
            
        CompareRect(iMoveLatestRect, iClickedRect);                                    
        }
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithIcon::HandlePointerUpL(const TPoint& aPoint)
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
        else if ( iNavigationCloseRect.Contains(aPoint) )
            {
            if ( iNavigationCloseRect == iClickedRect )
                {
                iOwner->SetCapture(EFalse);
		        iOwner->AutoChangeActiveList(iOwner->GetFirstCandidateIndex());
                }
            }
        else if (!iOwner->PageCtrlShown())
            {
            iClickedRect = KEmptyRect;
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
// CListMultiRowWithIcon::Draw
// draw the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithIcon::Draw()
    {
    // Call draw in base class;
    CList::Draw();

    DrawCandidates();

	const CFbsBitmap* bitmapBg = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapListBg);
	const CFbsBitmap* bitmapBgMask = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapListBgMask);
    
    if (!bitmapBg || !bitmapBgMask)
    	{
	    iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
	    if ((iOwner->GetCellHorizontalMargin() == 0) && (iOwner->GetCellVerticalMargin() == 0))
	        {
	        iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);
	        }
	    else
	        {
	        iOwner->Gc()->SetPenStyle(CGraphicsContext::ENullPen);
	        }

	    iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());
	    iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
	    iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );                
	    
	    // Draw the rect for navi
	    iOwner->Gc()->DrawRect(iNavigationRect);
    	}
    else
    	{
        iOwner->Gc()->DrawBitmapMasked(iNavigationRect, bitmapBg, 
                                       bitmapBg->SizeInPixels(), bitmapBgMask, ETrue);        
    		
    	}

    iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);

    // First draw down button
    if ( iBButtonClose )
        {
	    //mask bitmaps
	    TRect rect = iNavigationCloseRect;   
	    
	    if (iDropdownListDrawInfo.iSupportSkin)
	    	{
		    TRect innerRect = rect;
		    innerRect.Shrink( 4, 4 );
		    
		    // ----- draw bitmaps -----
			
			
			AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
				                      *iOwner->Gc(), 
				                      rect, 
				                      innerRect,
				                      iDropdownListDrawInfo.iNormalBtnImgID,
				                      KAknsIIDDefault ); 

		       
		    iBButtonMaskClose = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapCloseMask);   
		       
			TRect srcRect( TPoint( 0, 0 ), iBButtonClose->SizeInPixels() );
			       
			iOwner->Gc()->BitBltMasked( iNavigationCloseRect.iTl, 
							            iBButtonClose, 
									    srcRect,
									    iBButtonMaskClose,
									    EFalse);	       
	    	}
        else 
        	{
	        // Draw the rect for navi next
	        iOwner->Gc()->DrawLine(TPoint(iNavigationCloseRect.iTl.iX, iNavigationCloseRect.iBr.iY),
	                               iNavigationCloseRect.iBr);

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            iOwner->Gc()->DrawLine(iNavigationCloseRect.iTl, 
	                                   TPoint(iNavigationCloseRect.iTl.iX, 
	                                          iNavigationCloseRect.iBr.iY));
	            iOwner->Gc()->DrawLine(TPoint(iNavigationCloseRect.iBr.iX, 
	                                          iNavigationCloseRect.iTl.iY),
	                                   iNavigationCloseRect.iBr);                       
	            }
	            
	        if (iOwner->GetCellVerticalMargin() != 0)
	            {
	            iOwner->Gc()->DrawLine(iNavigationCloseRect.iTl, 
	                                   TPoint(iNavigationCloseRect.iBr.iX, 
	                                          iNavigationCloseRect.iTl.iY));
	            }
        	
        	
	        rect.Shrink(TSize(1,1));
	        rect.iBr.iY = rect.iBr.iY + 1;

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            rect.iBr.iX = rect.iBr.iX + 1;
	            }

	        iOwner->Gc()->DrawBitmapMasked(rect, iBButtonClose, 
	                                       iBButtonClose->SizeInPixels(), iBButtonMaskClose, ETrue);
        	}
        }

    if (!iOwner->PageCtrlShown())
        {
        iOwner->Invalidate(iRect);
        return;
        }

    // First draw down button
    if ( iBButtonDown )
        {
	    //mask bitmaps
	    TRect rect = iNavigationNextRect;
	    
	    if (iDropdownListDrawInfo.iSupportSkin)
	    	{
		    TRect innerRect = rect;
		    innerRect.Shrink( 4, 4 );
		    
		    // ----- draw bitmaps -----
			
			
			AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
		                     *iOwner->Gc(), 
		                     rect, 
		                     innerRect,
		                     iDropdownListDrawInfo.iNormalBtnImgID,
		                     KAknsIIDDefault ); 

		       
		    iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextMask);   
		       
			TRect srcRect( TPoint( 0, 0 ), iBButtonDown->SizeInPixels() );
			       
			iOwner->Gc()->BitBltMasked( iNavigationNextRect.iTl, 
									    iBButtonDown, 
									    srcRect,
									    iBButtonMaskDown,
									    EFalse);	    
	    	}
		else
			{
	        // Draw the rect for navi next
	        iOwner->Gc()->DrawLine(iNavigationNextRect.iTl,
	                               TPoint(iNavigationNextRect.iBr.iX, iNavigationNextRect.iTl.iY));

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            iOwner->Gc()->DrawLine(iNavigationNextRect.iTl, 
	                                  TPoint(iNavigationNextRect.iTl.iX, iNavigationNextRect.iBr.iY));
	            iOwner->Gc()->DrawLine(TPoint(iNavigationNextRect.iBr.iX, iNavigationNextRect.iTl.iY),
	                                   iNavigationNextRect.iBr);
	            }
	            
	        if (iOwner->GetCellVerticalMargin() != 0)
	            {
	            iOwner->Gc()->DrawLine(TPoint(iNavigationNextRect.iTl.iX, iNavigationNextRect.iBr.iY),
	                                   iNavigationNextRect.iBr);
	            }
			
	        rect.Shrink(TSize(1,1));               

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            rect.iBr.iX = rect.iBr.iX + 1;
	            }
	            
	        if (iOwner->GetCellVerticalMargin() != 0)
	            {
	            rect.iBr.iY = rect.iBr.iY + 1;
	            }

	        iOwner->Gc()->DrawBitmapMasked(rect, iBButtonDown, 
	                                       iBButtonDown->SizeInPixels(), iBButtonMaskDown, ETrue);				
			}
        }
        
    // Second draw up button
    if ( iBButtonUp )
        {
	    //mask bitmaps
	    TRect rect = iNavigationPreviousRect;
	    
	    if (iDropdownListDrawInfo.iSupportSkin)
	    	{
		    TRect innerRect = rect;
		    innerRect.Shrink( 4, 4 );
		    
		    // ----- draw bitmaps -----
			
			
			AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
		                     *iOwner->Gc(), 
		                     rect, 
		                     innerRect,
		                     iDropdownListDrawInfo.iNormalBtnImgID,
		                     KAknsIIDDefault ); 

		       
		    iBButtonMaskUp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPreviousMask);   
		       
			TRect srcRect( TPoint( 0, 0 ), iBButtonMaskUp->SizeInPixels() );
			       
			iOwner->Gc()->BitBltMasked( iNavigationPreviousRect.iTl, 
									    iBButtonUp, 
									    srcRect,
									    iBButtonMaskUp,
									    EFalse);	    
	    	}

		else
			{
	        // Draw the rect for navi next
	        iOwner->Gc()->DrawLine(TPoint(iNavigationPreviousRect.iTl.iX, 
	                                      iNavigationPreviousRect.iBr.iY),
	                               iNavigationPreviousRect.iBr );

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            iOwner->Gc()->DrawLine(iNavigationPreviousRect.iTl, 
	                         TPoint(iNavigationPreviousRect.iTl.iX, iNavigationPreviousRect.iBr.iY));
	            iOwner->Gc()->DrawLine(
	                         TPoint(iNavigationPreviousRect.iBr.iX, iNavigationPreviousRect.iTl.iY),
	                                iNavigationPreviousRect.iBr);
	            }
	            
	        if (iOwner->GetCellVerticalMargin() != 0)
	            {
	            iOwner->Gc()->DrawLine(iNavigationPreviousRect.iTl, 
	                         TPoint(iNavigationPreviousRect.iBr.iX, iNavigationPreviousRect.iTl.iY));
	            }
			
	        rect.Shrink(TSize(1,1));                

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            rect.iBr.iX = rect.iBr.iX + 1;
	            }
	            
	        if (iOwner->GetCellVerticalMargin() != 0)
	            {
	            rect.iBr.iY = rect.iBr.iY + 1;
	            }

	        iOwner->Gc()->DrawBitmapMasked(rect, iBButtonUp, 
	                                       iBButtonUp->SizeInPixels(), iBButtonMaskUp, ETrue);
			}
        }
        
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::UpdateList
// update the liset
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithIcon::UpdateListL(TBool aDrawFlag,TBool aRecalculate)
    {
    RefreshListL(aRecalculate);
    if(aDrawFlag)
        {
        Draw(); 
        iOwner->Invalidate(iRect); 
        }
    }

void CListMultiRowWithIcon::RefreshListL(TBool aRecalculate)
    {
    // Recalculate the pos for the candidates
    if ( aRecalculate )
        {
        iOwner->SetLastCandidateIndex(CalculatePositionsL());
        }        
    
    iOwner->GetPages()->AddPage(iOwner->GetFirstCandidateIndex(), iOwner->GetLastCandidateIndex());

    iCloseButtonDisable = EFalse;
    iBButtonClose = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapClose);
    iBButtonMaskClose = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapCloseMask); 
    
    if (iOwner->GetFirstCandidateIndex() == 0)  // First page, previous button can't be seen
        {
        iPreviousButtonDisable = ETrue;  
        iBButtonUp = NULL;
        iBButtonMaskUp = NULL; 
        }
    else
        {
        iPreviousButtonDisable = EFalse;  
        iBButtonUp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPrevious);
        iBButtonMaskUp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPreviousMask);
        }

    if ((iOwner->GetLastCandidateIndex() < iOwner->CandidateCount()) || 
         iOwner->NextCandidateExist())
        {
        iNextButtonDisable = EFalse;
        iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNext);
        iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextMask);
        }
    else
        {
        iNextButtonDisable = ETrue;
        iBButtonDown = NULL;
        iBButtonMaskDown = NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::CalculateAreas
// Calculate all kinds of rectangles
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListMultiRowWithIcon::CalculateAreas()
    {
    iRect.iTl = iOwner->GetTopLeftPoint();
    TInt width = iOwner->GetCellWidth() * iOwner->GetColNum() + 
                 iOwner->GetNaviWidth() + 
                 (iOwner->GetColNum() + 2) * iOwner->GetCellHorizontalMargin();
    
    TInt height = iOwner->GetCellHeight() * iOwner->GetRowNum() + 
                  (iOwner->GetRowNum() + 1) * iOwner->GetCellVerticalMargin();

    iRect.SetSize(TSize(width, height));
    
    if (iOwner->CandExpandType() == CFepCtrlDropdownList::ECandExpandUpward)
        {
        TInt onerowheight = iOwner->GetCellHeight() + 2 * iOwner->GetCellVerticalMargin();
        iRect.Move(0, -(height - onerowheight));
        }

    TBool candHLToR = (iOwner->CandHDirection() == CFepCtrlDropdownList::ECandFromLToR);
    TInt xPos = iRect.iTl.iX + iOwner->GetCellHorizontalMargin();
    TInt yPos = iRect.iTl.iY + iOwner->GetCellVerticalMargin();

    // width of content rect
    TInt widthC = iOwner->GetCellWidth() * iOwner->GetColNum() + 
                  (iOwner->GetColNum() - 1) * iOwner->GetCellHorizontalMargin();
    height = iOwner->GetCellHeight() * iOwner->GetRowNum() + 
             (iOwner->GetRowNum() - 1) * iOwner->GetCellVerticalMargin(); 
    // width of navi rect
    TInt widthN = iOwner->GetNaviWidth();

    if (candHLToR)
        {
        iContentRect.iTl.SetXY(xPos, yPos);
        iNavigationRect.iTl.SetXY(xPos + widthC + iOwner->GetCellHorizontalMargin(), 
                                  yPos);
        }
    else
        {
        iNavigationRect.iTl.SetXY(xPos, yPos);
        iContentRect.iTl.SetXY(xPos + widthN + iOwner->GetCellHorizontalMargin(), 
                               yPos);
        }

    iContentRect.SetSize(TSize(widthC, height));    
    iNavigationRect.SetSize(TSize(widthN, height));        

    iNavigationCloseRect.iTl = iNavigationRect.iTl;
    iNavigationCloseRect.SetSize(TSize(iOwner->GetNaviWidth(), iOwner->GetCellHeight()));

    if (!iOwner->PageCtrlShown())
        {
        iNavigationPreviousRect = TRect(0, 0, 0, 0);
        iNavigationNextRect = TRect(0, 0, 0, 0);
        return; 
        }
    else
        {
        iNavigationPreviousRect.iTl.SetXY(iNavigationCloseRect.iTl.iX, 
                                          iNavigationCloseRect.iBr.iY + iOwner->GetCellVerticalMargin());
        iNavigationPreviousRect.SetSize(TSize(iOwner->GetNaviWidth(), iOwner->GetSpinBtnHeight()));
        iNavigationNextRect.iTl.SetXY(iNavigationRect.iTl.iX, 
                                      iNavigationRect.iBr.iY - iOwner->GetSpinBtnHeight());
        iNavigationNextRect.SetSize(TSize(iOwner->GetNaviWidth(), iOwner->GetSpinBtnHeight()));
        }
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::CalculatePositionsL
// calculate the position of all visible cells in the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
TInt CListMultiRowWithIcon::CalculatePositionsL()
    {
    TInt t = 0;
    TPoint setpoints;
    TInt count = iOwner->CandidateCount();
    CCandidate* candidate = NULL;
    if ( count > 0 )
        {
        TBool candHLToR = (iOwner->CandHDirection() == CFepCtrlDropdownList::ECandFromLToR);
        candHLToR ? setpoints.SetXY(iContentRect.iTl.iX, iContentRect.iTl.iY) : 
                    setpoints.SetXY(iContentRect.iBr.iX, iContentRect.iTl.iY);

        TInt currentCellIndex = 0;
        TInt lastUsedCellCount = 0;
        TInt lastusedRow = 0;
        iOwner->SetFirstCandidateIndex (iOwner->GetLastCandidateIndex());         
        count = Min(count, iOwner->GetLastCandidateIndex() + iOwner->GetColNum() * iOwner->GetRowNum() );   

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

                candHLToR ? (setpoints.iX += lastUsedCellCount*(iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin())) : 
                            (setpoints.iX -= lastUsedCellCount*(iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin()));
                }
               else
                {
                candHLToR ? (setpoints.iX += lastUsedCellCount*(iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin())) : 
                            (setpoints.iX -= lastUsedCellCount*(iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin()));
                }                
            } // for-loop
        } // if candidate exist
        
    return t; // as lastCandidateIndex
    }

// -----------------------------------------------------------------------------
// CListMultiRowWithIcon::DrawNavigationPress
// draw navigation button when pressed or un-pressed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CListMultiRowWithIcon::DrawNavigationPress(const TRect& aRect,TBool aIndicate)
    {
    TBool retval = EFalse;

    if ( aRect == iNavigationCloseRect )
        {
        retval = ETrue;  
        
        // Activeate bitmap context
        iOwner->ActivateBitmapContext();        
        
        if (iDropdownListDrawInfo.iSupportSkin)
        	{
		    TRect rect = iNavigationCloseRect;        
		    TRect innerRect = rect;
		    innerRect.Shrink( 4, 4 );

	        if ( aIndicate )
	            {
				AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                 *iOwner->Gc(), 
	                 rect, 
	                 innerRect,
	                 iDropdownListDrawInfo.iPressedBtnImgID,
	                 KAknsIIDDefault ); 
	            }
	        else
	            {
	            
				AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                 *iOwner->Gc(), 
	                 rect, 
	                 innerRect,
	                 iDropdownListDrawInfo.iNormalBtnImgID,
	                 KAknsIIDDefault ); 
	            
	            }
	            
			iBButtonMaskClose = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapCloseMask);   
	            
			TRect srcRect( TPoint( 0, 0 ), iBButtonClose->SizeInPixels() );
			       
			iOwner->Gc()->BitBltMasked( iNavigationCloseRect.iTl, 
							  			iBButtonClose, 
							  			srcRect,
							  			iBButtonMaskClose,
							  			EFalse);	
        	}
		else
			{
	        if ( aIndicate )  // Pressed the button
	            {
	            iBButtonClose = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapClosePress);
	            iBButtonMaskClose = iOwner->GetBitmapDb()->GetBitMap(
                                                                 CBitmapDb::EBitmapClosePressMask);
	            }
	        else // Release the button
	            {
	            iBButtonClose = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapClose);
	            iBButtonMaskClose = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapCloseMask);
	            }
	        
	        // Begin draw navigation next button
	        iOwner->DrawMask(iNavigationCloseRect);
	        // Activeate bitmap context
	        iOwner->ActivateBitmapContext();
	        // Draw bitmap
	        TRect rect = iNavigationCloseRect;        
	        rect.Shrink(TSize(1,1));
	        rect.iBr.iY += 1;        

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            rect.iBr.iX = rect.iBr.iX + 1;
	            }

	        iOwner->Gc()->DrawBitmapMasked(rect, iBButtonClose, 
	                                       iBButtonClose->SizeInPixels(), iBButtonMaskClose, ETrue);
			}
       	iOwner->Invalidate(aRect);
        }
    else if (!iOwner->PageCtrlShown())
        {
        return retval;
        }
    else if ( aRect == iNavigationPreviousRect )
        {
        retval = ETrue;  
        
        // Activeate bitmap context
        iOwner->ActivateBitmapContext();
        
		if (iDropdownListDrawInfo.iSupportSkin)
			{
		    TRect rect = iNavigationPreviousRect;        
		    TRect innerRect = rect;
		    innerRect.Shrink( 4, 4 );

	        if ( aIndicate )
	            {
				AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                 *iOwner->Gc(), 
	                 rect, 
	                 innerRect,
	                 iDropdownListDrawInfo.iPressedBtnImgID,
	                 KAknsIIDDefault ); 
	            }
	        else
	            {
				AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                 *iOwner->Gc(), 
	                 rect, 
	                 innerRect,
	                 iDropdownListDrawInfo.iNormalBtnImgID,
	                 KAknsIIDDefault ); 
	            }
	            
			iBButtonMaskUp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPreviousMask);   
	            
			TRect srcRect( TPoint( 0, 0 ), iBButtonUp->SizeInPixels() );
			       
			iOwner->Gc()->BitBltMasked( iNavigationPreviousRect.iTl, 
							  			iBButtonUp, 
										srcRect,
										iBButtonMaskUp,
										EFalse);	
			}
		else
			{
	        if ( aIndicate )  // Pressed the button
	            {
	            if ( iBButtonUp )
	                {
	                iBButtonUp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPreviousPress);
	                iBButtonMaskUp = iOwner->GetBitmapDb()->GetBitMap(
                                                              CBitmapDb::EBitmapPreviousPressMask);
	                }          
	            }
	        else // Release the button
	            {
	            if ( iBButtonUp )
	                {
	                iBButtonUp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPrevious);
	                iBButtonMaskUp = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapPreviousMask);
	                }          
	            }
	        
	        // Begin draw navigation next button
	        iOwner->DrawMask(iNavigationPreviousRect);
	        
	        // Activeate bitmap context
	        iOwner->ActivateBitmapContext();
	        
	        // Clear rect
	        iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
	        iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);
	        iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());
	        iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
	        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );                
	        iOwner->Gc()->DrawRect( iNavigationPreviousRect );
	        
	        // Draw bitmap
	        TRect rect = iNavigationPreviousRect;        
	        rect.Shrink(TSize(1,1));    

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            rect.iBr.iX = rect.iBr.iX + 1;
	            }
	            
	        if (iOwner->GetCellVerticalMargin() != 0)
	            {
	            rect.iBr.iY = rect.iBr.iY + 1;
	            }

	        iOwner->Gc()->DrawBitmapMasked(rect, iBButtonUp, 
	                                       iBButtonUp->SizeInPixels(), iBButtonMaskUp, ETrue);
			}
			
        iOwner->Invalidate(aRect);
        
        }        
    else if ( aRect == iNavigationNextRect )
        {
        retval = ETrue;   

        // Activeate bitmap context
        iOwner->ActivateBitmapContext();
        

		if (iDropdownListDrawInfo.iSupportSkin)
			{
		    TRect rect = iNavigationNextRect;        
		    TRect innerRect = rect;
		    innerRect.Shrink( 4, 4 );

	        if ( aIndicate )
	            {
				AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                 *iOwner->Gc(), 
	                 rect, 
	                 innerRect,
	                 iDropdownListDrawInfo.iPressedBtnImgID,
	                 KAknsIIDDefault ); 
	            }
	        else
	            {
				AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                 *iOwner->Gc(), 
	                 rect, 
	                 innerRect,
	                 iDropdownListDrawInfo.iNormalBtnImgID,
	                 KAknsIIDDefault ); 
	            }
	            
			iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextMask);   
	            
			TRect srcRect( TPoint( 0, 0 ), iBButtonDown->SizeInPixels() );
			       
			iOwner->Gc()->BitBltMasked( iNavigationNextRect.iTl, 
										iBButtonDown, 
										srcRect,
										iBButtonMaskDown,
										EFalse);	
			}
		else
			{
	        if ( aIndicate )  // Pressed the button
	            {
	            if ( iBButtonDown )
	                {
	                iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextPress);
	                iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(
                                                                  CBitmapDb::EBitmapNextPressMask);
	                }         
	            }
	        else // Release the button
	            {
	            if ( iBButtonDown )
	                {
	                iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNext);
	                iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextMask);
	                }            
	            }
	        
	        // Begin draw navigation next button
	        iOwner->DrawMask(iNavigationNextRect);
	        
	        // Activeate bitmap context
	        iOwner->ActivateBitmapContext();
	        
	        // Clear rect
	        iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
	        iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);
	        iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());
	        iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
	        iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );       
	        iOwner->Gc()->DrawRect( iNavigationNextRect );
	        
	        // Draw bitmap
	        TRect rect = iNavigationNextRect;        
	        rect.Shrink(TSize(1,1));         

	        if (iOwner->GetCellHorizontalMargin() != 0)
	            {
	            rect.iBr.iX = rect.iBr.iX + 1;
	            }
	            
	        if (iOwner->GetCellVerticalMargin() != 0)
	            {
	            rect.iBr.iY = rect.iBr.iY + 1;
	            }

	        iOwner->Gc()->DrawBitmapMasked(rect, iBButtonDown, 
	                                       iBButtonDown->SizeInPixels(), iBButtonMaskDown, ETrue);
			}

        iOwner->Invalidate(aRect);
        } 
        
    return retval;
    }

// End Of File
