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
#include <fbs.h>
#include <AknsUtils.h>
#include <AknLayoutDef.h>
#include <AknsDrawUtils.h> 

#include <peninputdropdownlistcontext.h>

#include "peninputlistonerowwithicon.h"
#include "peninputcandidate.h"
#include "peninputdropdownlist.h"
#include "peninputlayout.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListOneRowWithIcon 

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::CListOneRowWithIcon
// C++ default constructor can NOT contain any code, that
// might leave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithIcon::CListOneRowWithIcon(CListManager* aManager, 
                                         MFepCtrlDropdownListContext *aOwner)
: CList(aManager, aOwner)
    {
    iIsMultiRows = EFalse ;
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithIcon* CListOneRowWithIcon::NewL(CListManager* aManager, 
                                               MFepCtrlDropdownListContext *aOwner)
    {
    CListOneRowWithIcon* self = new (ELeave) CListOneRowWithIcon(aManager, aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::~CListOneRowWithIcon
// desstrutor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListOneRowWithIcon::~CListOneRowWithIcon()
    {     
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::HandlePointerDownL
// handle pointer down 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIcon::HandlePointerDownL(const TPoint& aPoint)
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
			#endif //RD_TACTILE_FEEDBACK 
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
    }


// -----------------------------------------------------------------------------
// CListOneRowWithIcon::HandlePointerDragL
// handle pointer drag function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIcon::HandlePointerDragL(const TPoint& aPoint)
    {
    // Pass the event first to the base
    CList::HandlePointerDragL(aPoint);

    if ( iIndicateButtonClicked )  // clicked rect is not outside the whole DDL
        {
        iMoveLatestRect = KEmptyRect;
        
        if ( iNavigationRect.Contains(aPoint) )
            {
            if ( iNavigationNextRect.Contains(aPoint) && !iNextButtonDisable )
                {
                // If click in down button and the down button is enabled                   
                iMoveLatestRect.SetRect(iNavigationNextRect.iTl, iNavigationNextRect.iBr);
                } 
            else if ( iNavigationPreviousRect.Contains(aPoint) && !iPreviousButtonDisable )
                {
                // If click in up button and the up button is enabled                     
                iMoveLatestRect.SetRect(iNavigationPreviousRect.iTl, iNavigationPreviousRect.iBr);
                }
            }
        else if ( iContentRect.Contains(aPoint) )
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

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::HandlePointerUpL
// handle pointer up function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIcon::HandlePointerUpL(const TPoint& aPoint)
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
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::Draw
// draw the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIcon::Draw()
    {
    CList::Draw();
    
    DrawCandidates();
	
	DrawSelection(iClickedRect, ETrue);
	
	if (iDropdownListDrawInfo.iSupportSkin)
		{
	    //mask bitmaps
	    TRect rect = iNavigationNextRect;        
	    TRect innerRect = rect;
	    innerRect.Shrink( 4, 4 );
	    
	    // ----- draw bitmaps -----
		
		
		TAknsItemID expandBtnImgID;
		CBitmapDb::TBitmapType bitmapType;
		
		if ( (iOwner->GetLastCandidateIndex() >= iOwner->CandidateCount()) 
        	 && ( iOwner->GetPages()->PageCount() < 1 ) )
			{
			expandBtnImgID = iDropdownListDrawInfo.iInactiveBtnImgID;
			bitmapType = CBitmapDb::EBitmapNextDisableMask;
			}
		else
			{
			expandBtnImgID = iDropdownListDrawInfo.iNormalBtnImgID;
			bitmapType = CBitmapDb::EBitmapNextMask;
			}
			
		// Draw background    
		AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                     *iOwner->Gc(), 
	                     rect, 
	                     innerRect,
	                     expandBtnImgID,
	                     KAknsIIDDefault );
	                     
	    iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(bitmapType);   
		TRect srcRect( TPoint( 0, 0 ), iBButtonDown->SizeInPixels() );
		iOwner->Gc()->BitBltMasked( iNavigationNextRect.iTl, 
								  iBButtonDown, 
								  srcRect,
								  iBButtonMaskDown,
								  EFalse);

		}
	else
		{
	    //Clear bg for iNavigationNextRect
	    iOwner->Gc()->SetPenSize(iOwner->GetPenSize());
	    iOwner->Gc()->SetPenStyle(CGraphicsContext::ESolidPen);

	    iOwner->Gc()->SetPenColor(iOwner->GetBorderColor());
	    iOwner->Gc()->SetBrushColor(iOwner->GetBgColor());
	    iOwner->Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );                
	    iOwner->Gc()->DrawRect( iNavigationNextRect );
	            
	    //Draw navigation bitmap
	    TRect rect = iNavigationNextRect;
	    rect.Shrink(1, 1);
	    iOwner->Gc()->DrawBitmapMasked(rect, iBButtonDown, 
	                                   iBButtonDown->SizeInPixels(), iBButtonMaskDown, ETrue);
		}

    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::UpdateList
// update the liset
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIcon::UpdateListL(TBool aDrawFlag, TBool aRecalculate)
    {
    if (aRecalculate)
        {
        iOwner->SetLastCandidateIndex(CalculatePositionsL());
        }
    
    if ( (iOwner->GetLastCandidateIndex() >= iOwner->CandidateCount()) 
        && ( iOwner->GetPages()->PageCount() < 1 ) )
        {
        iNextButtonDisable = ETrue;
        
        //Set disable bitmap for button down and buttondown mask
        iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextDisable);
        iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextDisableMask);
        }
    else
        {
        iNextButtonDisable = EFalse;  
        
        //Set enable bitmap for button down and buttondown mask
        iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapExpand);
        iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapExpandMask);
        
        if (!iBButtonDown)
            {
            iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNext);
            iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextMask);
            }
        }
    if(aDrawFlag)    
        {
        Draw();
        iOwner->Invalidate(iRect);
        }
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::CalculateAreas
// Calculate all kinds of rectangles
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListOneRowWithIcon::CalculateAreas()
    {
    // Calculate the rect of the list     
    iRect.iTl = iOwner->GetTopLeftPoint();
    TInt width = iOwner->GetCellWidth() * iOwner->GetColNum() + iOwner->GetNaviWidth() + 
                 (iOwner->GetColNum() + 2) * iOwner->GetCellHorizontalMargin();
    
    TInt height = iOwner->GetCellHeight() + 2 * iOwner->GetCellVerticalMargin();
    
    iRect.SetSize(TSize(width, height));

    TBool candHLToR = (iOwner->CandHDirection() == CFepCtrlDropdownList::ECandFromLToR);
    TInt posX = iRect.iTl.iX + iOwner->GetCellHorizontalMargin();
    TInt posY = iRect.iTl.iY + iOwner->GetCellVerticalMargin();
    // width of content rect
    TInt widthC = iOwner->GetCellWidth() * iOwner->GetColNum() + (iOwner->GetColNum() - 1) * iOwner->GetCellHorizontalMargin();
    height = iOwner->GetCellHeight();
    // width of navi rect
    TInt widthN = iOwner->GetNaviWidth();

    if (candHLToR)
        {
        iContentRect.iTl.SetXY(posX, posY);
        iNavigationRect.iTl.SetXY(posX + widthC + iOwner->GetCellHorizontalMargin(),
                                  posY);
        }
    else
        {
        iNavigationRect.iTl.SetXY(posX, posY);
        iContentRect.iTl.SetXY(posX + widthN + iOwner->GetCellHorizontalMargin(), 
                               posY);
        }

    // Calculate the rect of the content
    iContentRect.SetSize(TSize(widthC, height));

    // Calculate the rect of the navigation area
    iNavigationRect.SetSize(TSize(widthN, height));
    // Calculate the rect of the down button    
    iNavigationNextRect = iNavigationRect; 
    // Calculate the rect of the up button       
    iNavigationPreviousRect = TRect(0,0,0,0);
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::CalculatePositionsL
// calculate the position of all visible cells in the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt CListOneRowWithIcon::CalculatePositionsL()
    {
    TInt t = 0;
    TPoint setpoints;

    //RArray<CCandidate>& candidates = iOwner->GetCandidatelist;
    TInt count = iOwner->CandidateCount();
    
    iOwner->SetFirstCandidateIndex(iOwner->GetLastCandidateIndex());    
        
    if ( count > 0 )
        {
        TBool candHLToR = (iOwner->CandHDirection() == CFepCtrlDropdownList::ECandFromLToR);
        candHLToR ? setpoints.SetXY(iContentRect.iTl.iX, iContentRect.iTl.iY) : 
                    setpoints.SetXY(iContentRect.iBr.iX, iContentRect.iTl.iY);  

        TInt lastUsedCellCount = 0;
        TInt currentCellIndex = 0;
        count = Min(count,iOwner->GetLastCandidateIndex() + iOwner->GetColNum());    
              
        for(t = iOwner->GetLastCandidateIndex(); t < count ; t++)
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
        
            if (currentCellIndex > iOwner->GetColNum())
                {
                break;
                }

            candHLToR ? (setpoints.iX += lastUsedCellCount * (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin())) : 
                        (setpoints.iX -= lastUsedCellCount * (iOwner->GetCellWidth() + iOwner->GetCellHorizontalMargin())) ;
            } // for-loop
        } // if candidate exist 
     
    return t; // as lastCandidateIndex
    }

// -----------------------------------------------------------------------------
// CListOneRowWithIcon::DrawNavigationPress
// draw navigation button when pressed or un-pressed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CListOneRowWithIcon::DrawNavigationPress(const TRect& aRect,TBool aIndicate)
    {
    TBool retval = EFalse;
    
    if ( aRect == iNavigationNextRect )
        {
        retval = ETrue;
        if (iDropdownListDrawInfo.iSupportSkin)
        	{
		    TRect rect = iNavigationNextRect;        
		    TRect innerRect = rect;
		    innerRect.Shrink( 4, 4 );
		    	
			
			TAknsItemID expandBtnImgID;
	        if ( aIndicate )
	            {
	            expandBtnImgID = iDropdownListDrawInfo.iPressedBtnImgID;
	            }
	        else
	            {
	            expandBtnImgID = iDropdownListDrawInfo.iNormalBtnImgID;
	            }
			AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                 *iOwner->Gc(), 
                 rect, 
                 innerRect,
                 expandBtnImgID,
                 KAknsIIDDefault ); 
	            
	            
			iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapExpand);
			iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapExpandMask);

			if (!iBButtonDown)
			    {
			    iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNext);
			    iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextMask);
			    }
	            
			TRect srcRect( TPoint( 0, 0 ), iBButtonDown->SizeInPixels() );
			       
			iOwner->Gc()->BitBltMasked( iNavigationNextRect.iTl, 
							  iBButtonDown, 
							  srcRect,
							  iBButtonMaskDown,
							  EFalse);	
        	}
        else
        	{
	        if ( aIndicate )
	            {
	            // NEXT PRESSED DOWN
	            iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapExpandPress);
	            iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(
                                                                CBitmapDb::EBitmapExpandPressMask);
	            
	            if (!iBButtonDown)
	                {
	                iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapNextPress);
	                iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(
                                                                CBitmapDb::EBitmapNextPressMask);
	                }
	            }
	        else
	            {
	            // NEXT ORIGINAL
	            iBButtonDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapExpand);
	            iBButtonMaskDown = iOwner->GetBitmapDb()->GetBitMap(CBitmapDb::EBitmapExpandMask);
	            
	            if (!iBButtonDown)
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
