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
* Description:  drop-down list control
*
*/


// INCLUDE FILES


#include "peninputlistmanager.h"
#include "peninputbitmapdb.h"
#include "peninputcandidate.h"
#include "peninputpages.h"
#include "peninputlayoutbubblectrl.h"

// added by txin
//#include "peninputlistmultirowrollwithicon.h"
// end
#include "peninputlist.h"
#include <peninputlayoutrootctrl.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayout.h>
#include <peninputdropdownlist.h>
#include <coemain.h>
#include <AknUtils.h>

// constant definition
const TInt32 KInvalidResId = -1;

// added by txin
const TInt KInvalidCandIndex = -1;
// end adding

const TInt KPreviewBubbleOffset = 20;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CFepCtrlDropdownList 

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CFepCtrlDropdownList* CFepCtrlDropdownList::NewL( const TPoint& aTopLeftPoint,
                                                           CFepUiLayout* aUiLayout,
                                                           TInt aControlId,
                                                           TResourceReader& aReader,
                                                           const TInt aCellWidth,
                                                           const TInt aCellHeight,
                                                           const TInt aNaviWidth,
                                                           const TInt aColNum,
                                                           const TInt aRowNum,
                                                           const CFont* aFont,
                                                           MListEventHandler* aHandler,
                                                           const CGraphicsContext::TTextAlign aAlignment,
                                                           const TInt aTextMargin,
                                                           const TRgb aBorderColor,
                                                           const TRgb aBgColor,
                                                           const TRgb aNavigationBgColor,
                                                           const TRgb aTextColor,
                                                           const TRgb aSpecialTextColor,
                                                           const TRgb aHighLightTextColor,
                                                           const TRgb aHighLightBgColor,
                                                           const TRgb aButtonDownBgColor )
    {
    CFepCtrlDropdownList* self = NewLC(aTopLeftPoint,
                                       aUiLayout,
                                       aControlId,
                                       aReader,
                                       aCellWidth,
                                       aCellHeight,
                                       aNaviWidth,
                                       aColNum,
                                       aRowNum,
                                       aFont,
                                       aHandler,
                                       aAlignment,
                                       aTextMargin,                                       
                                       aBorderColor,
                                       aBgColor,
                                       aNavigationBgColor,
                                       aTextColor,
                                       aSpecialTextColor,
                                       aHighLightTextColor,
                                       aHighLightBgColor,
                                       aButtonDownBgColor);
    CleanupStack::Pop();
    return self;  
    } 

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::NewLC
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C CFepCtrlDropdownList* CFepCtrlDropdownList::NewLC(const TPoint& aTopLeftPoint,
                                                           CFepUiLayout* aUiLayout,
                                                           TInt aControlId,
                                                           TResourceReader& aReader,
                                                           const TInt aCellWidth,
                                                           const TInt aCellHeight,
                                                           const TInt aNaviWidth,
                                                           const TInt aColNum,
                                                           const TInt aRowNum,
                                                           const CFont* aFont,
                                                           MListEventHandler* aHandler,
                                                     const CGraphicsContext::TTextAlign aAlignment,
                                                           const TInt aTextMargin,
                                                           const TRgb aBorderColor,
                                                           const TRgb aBgColor,
                                                           const TRgb aNavigationBgColor,
                                                           const TRgb aTextColor,
                                                           const TRgb aSpecialTextColor,
                                                           const TRgb aHighLightTextColor,
                                                           const TRgb aHighLightBgColor,
                                                           const TRgb aButtonDownBgColor)
    {
    CFepCtrlDropdownList* self = new (ELeave) CFepCtrlDropdownList(aTopLeftPoint,
                                                                  aUiLayout,
                                                                  aControlId,
                                                                  aCellWidth,
                                                                  aCellHeight,
                                                                  aNaviWidth,
                                                                  aColNum,
                                                                  aRowNum,
                                                                  aFont,
                                                                  aHandler,
                                                                  aAlignment,
                                                                  aTextMargin,
                                                                  aBorderColor,
                                                                  aBgColor,
                                                                  aNavigationBgColor,
                                                                  aTextColor,
                                                                  aSpecialTextColor,
                                                                  aHighLightTextColor,
                                                                  aHighLightBgColor,
                                                                  aButtonDownBgColor);
    CleanupStack::PushL(self);
    self->ConstructL(aTopLeftPoint,aReader);
    return self;  
    } 
    
// txin
EXPORT_C CFepCtrlDropdownList* CFepCtrlDropdownList::NewL(CFepUiLayout* aUiLayout,
                                                          TInt aControlId,
                                                          TInt aResId,
                                                          TPoint aTopLeftPoint,
                                                          TSize aCellSize,
                                                          TInt aColNum,
                                                          TInt aRowNum)
    {
    CFepCtrlDropdownList* self = new (ELeave) CFepCtrlDropdownList(aUiLayout,
                                                                   aControlId,
                                                                   aTopLeftPoint,
                                                                   aCellSize,
                                                                   aColNum,
                                                                   aRowNum);
    CleanupStack::PushL(self);
    self->ConstructL(aTopLeftPoint, aResId);
    CleanupStack::Pop(self);
    return self;
    }
    
EXPORT_C CFepCtrlDropdownList* CFepCtrlDropdownList::NewL(CFepUiLayout* aUiLayout,
                                                          TInt aControlId,
                                                          TInt aResId,
                                                          TPoint aTopLeftPoint,
                                                          const CFont* aFont,
                                                          TSize aCellSize,
                                                          TInt aNaviWidth,
                                                          TInt aColNum,
                                                          TInt aRowNum,
                                                          TInt aSpinBtnHeight,
                                                          TInt aHorizontalCellMargin,
                                                          TInt aVerticalCellMargin,
                                                          THDirection aDirection,
                                                          TBool aPageShown)
    {
    CFepCtrlDropdownList* self = new (ELeave) CFepCtrlDropdownList(aUiLayout,
                                                                   aControlId,
                                                                   aResId,
                                                                   aTopLeftPoint,
                                                                   aFont,
                                                                   aCellSize,
                                                                   aNaviWidth,
                                                                   aColNum,
                                                                   aRowNum,
                                                                   aSpinBtnHeight,
                                                                   aHorizontalCellMargin,
                                                                   aVerticalCellMargin,
                                                                   aDirection,
                                                                   aPageShown);
    CleanupStack::PushL(self);
    self->ConstructL(aTopLeftPoint, aResId);
    CleanupStack::Pop(self);
    return self;
    }
    
    
// end
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::CFepCtrlDropdownList
// construtor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
CFepCtrlDropdownList::CFepCtrlDropdownList(const TPoint& aTopLeftPoint,
                                           CFepUiLayout* aUiLayout,
                                           TInt aControlId,                           
                                           const TInt aCellWidth,
                                           const TInt aCellHeight,
                                           const TInt aNaviWidth,                               
                                           const TInt aColNum,
                                           const TInt aRowNum,
                                           const CFont* aFont,
                                           MListEventHandler* aHandler,
                                           const CGraphicsContext::TTextAlign aAlignment,
                                           const TInt aTextMargin,
                                           const TRgb aBorderColor,
                                           const TRgb aBgColor,
                                           const TRgb aNavigationBgColor,
                                           const TRgb aTextColor,
                                           const TRgb aSpecialTextColor,
                                           const TRgb aHighLightTextColor,
                                           const TRgb aHighLightBgColor,
                                           const TRgb aButtonDownBgColor)
    : CFepUiBaseCtrl(TRect(), aUiLayout, aControlId),
    iEnableSecondary(EFalse),
    iFirstCandidateIndex(0),
    iLastCandidateIndex(0),
    iFont(aFont), iCellWidth(aCellWidth), iCellHeight(aCellHeight), iNaviWidth(aNaviWidth),
    iSpinBtnHeight(aCellHeight), 
    iCellHorizontalMargin(0),
    iCellVerticalMargin(0),
    iOriginalCandIdx(KInvalidCandIndex),
    iOriginalFirstCandIdx(KInvalidCandIndex),
    iUpdateListReDraw(ETrue),
    iAlignment(aAlignment), iTextMargin(aTextMargin), iColNum(aColNum), iRowNum(aRowNum), 
    iBorderColor(aBorderColor), iBgColor(aBgColor), iNavigationBgColor(aNavigationBgColor),
    iTextColor(aTextColor), iSpecialTextColor(aSpecialTextColor), 
    iHighLightTextColor(aHighLightTextColor),
    iHighLightBgColor(aHighLightBgColor), iButtonDownBgColor(aButtonDownBgColor), 
    iTopLeftPoint(aTopLeftPoint), 
    iEventIdForCandidateSelected(-1),
    iEventIdForNextPageCandidate(-1),
    iEventIdForPreviousPageCandidate(-1),
    iEventIdForCandidateExisted(-1),
    iListEventHandler(aHandler), iFirstTimeConstruct(ETrue), iListType(EListNoExpandable), 
    iFuzzyBoundry(0),
    iCandExpandType(ECandExpandDownward),
    iCandHDirection(ECandFromLToR),
    iPageCtrlShown(ETrue)
    {
    SetControlType(KDropDownListCtrlType);
    }

// txin
CFepCtrlDropdownList::CFepCtrlDropdownList(CFepUiLayout* aUiLayout,
                                           TInt aControlId,
                                           TPoint aTopLeftPoint,
                                           TSize aCellSize,
                                           TInt aColNum,
                                           TInt aRowNum)
    : CFepUiBaseCtrl(TRect(), aUiLayout, aControlId),
    iEnableSecondary(EFalse),
    iFirstCandidateIndex(0),
    iLastCandidateIndex(0),
    iCellWidth(aCellSize.iWidth), iCellHeight(aCellSize.iHeight), iNaviWidth(iCellWidth),
    iSpinBtnHeight(iCellHeight), 
    iCellHorizontalMargin(0),
    iCellVerticalMargin(0),
    iOriginalCandIdx(KInvalidCandIndex),
    iOriginalFirstCandIdx(KInvalidCandIndex),
    iUpdateListReDraw(ETrue),
    iAlignment(CGraphicsContext::ECenter), iTextMargin(3), iColNum(aColNum), iRowNum(aRowNum), 
    iBorderColor(KRgbBlack), iBgColor(TRgb(0xeeeeee)), iNavigationBgColor(TRgb(0xeeeeee)),
    iTextColor(KRgbBlack), iSpecialTextColor(KRgbBlack), iHighLightTextColor(KRgbBlack),
    iHighLightBgColor(TRgb(0xcccccc)), iButtonDownBgColor(KRgbGray), iTopLeftPoint(aTopLeftPoint), 
    iEventIdForCandidateSelected(-1),
    iEventIdForNextPageCandidate(-1),
    iEventIdForPreviousPageCandidate(-1),
    iEventIdForCandidateExisted(-1),
    iFirstTimeConstruct(ETrue), iListType(EListNoExpandable), 
    iFuzzyBoundry(0),
    iCandExpandType(ECandExpandDownward),
    iCandHDirection(ECandFromLToR),
    iPageCtrlShown(ETrue)
    {
    SetControlType(KDropDownListCtrlType);
    }

CFepCtrlDropdownList::CFepCtrlDropdownList(CFepUiLayout* aUiLayout,
                         TInt aControlId,
                         TInt /*aResId*/,
                         TPoint aTopLeftPoint,
                         const CFont* /*aFont*/,
                         TSize aCellSize,
                         TInt aNaviWidth,
                         TInt aColNum,
                         TInt aRowNum,
                         TInt aSpinBtnHeight,
                         TInt aHorizontalCellMargin,
                         TInt aVerticalCellMargin,
                                               THDirection aDirection,
                                               TBool aPageShown)
    : CFepUiBaseCtrl(TRect(), aUiLayout, aControlId),
    iEnableSecondary(EFalse),
    iFirstCandidateIndex(0),
    iLastCandidateIndex(0),
    iCellWidth(aCellSize.iWidth), iCellHeight(aCellSize.iHeight), iNaviWidth(aNaviWidth),
    iSpinBtnHeight(aSpinBtnHeight), 
    iCellHorizontalMargin(aHorizontalCellMargin),
    iCellVerticalMargin(aVerticalCellMargin),
    iOriginalCandIdx(KInvalidCandIndex),
    iOriginalFirstCandIdx(KInvalidCandIndex),
    iUpdateListReDraw(ETrue),
    iAlignment(CGraphicsContext::ECenter), iTextMargin(3), iColNum(aColNum), iRowNum(aRowNum), 
    iBorderColor(KRgbBlack), iBgColor(TRgb(0xeeeeee)), iNavigationBgColor(TRgb(0xeeeeee)),
    iTextColor(KRgbBlack), iSpecialTextColor(KRgbBlack), iHighLightTextColor(KRgbBlack),
    iHighLightBgColor(TRgb(0xcccccc)), iButtonDownBgColor(KRgbGray), iTopLeftPoint(aTopLeftPoint), 
    iEventIdForCandidateSelected(-1),
    iEventIdForNextPageCandidate(-1),
    iEventIdForPreviousPageCandidate(-1),
    iEventIdForCandidateExisted(-1),
    iFirstTimeConstruct(ETrue), iListType(EListNoExpandable), 
    iFuzzyBoundry(0),
    iCandExpandType(ECandExpandDownward),
    iCandHDirection(aDirection),
    iPageCtrlShown(aPageShown)
    {
    SetControlType(KDropDownListCtrlType);
    }

// end

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ConstructL
// 2nd construct function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFepCtrlDropdownList::ConstructL(const TPoint& aTopLeftPoint, TResourceReader& aReader)
    {
    BaseConstructL();

    iBitmapDb = CBitmapDb::NewL(aReader, 
                                TSize(iCellWidth, iCellHeight),
                                TSize(iNaviWidth, iCellHeight),
                                TSize(iNaviWidth, iSpinBtnHeight));

    SetTextColor(iBitmapDb->TextColor());
    SetBorderColor(iBitmapDb->FrameColor());
    iFirstTimeConstruct = EFalse;

    iPages = CPages::NewL();

    iListManager = CListManager::NewL(this);

    iListManager->SetActiveList(iListManager->ListOneRowWithoutIcon());

    SetRect( iListManager->ActiveList()->Rect() );

    const TSize& size = Rect().Size();
    TRect rect(aTopLeftPoint,size);
    SetRect(rect);
    }
    
// txin
void CFepCtrlDropdownList::ConstructL(TPoint aTopLeftPoint, TInt aResId)
    {
    SetResourceId(aResId);

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, aResId);
    ConstructL(aTopLeftPoint, reader);
    CleanupStack::PopAndDestroy(); // reader
    }
// end    

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::~CFepCtrlDropdownList
// destructor function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C CFepCtrlDropdownList::~CFepCtrlDropdownList()
    {
    delete iBubbleCtrl;
    
    // destroy list objects
    delete iListManager;

    // Delete bitmapDb
    delete iBitmapDb;
    
    // Delete pages
    delete iPages;
    
    // Delete all candidate
    iCandidatelist.ResetAndDestroy();
    
    // Clear and close iSecondaryList
    iSecondaryList.Close();   

    // Clear and close iHighlightList
    iHighlightList.Close();       
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SizeChanged
// size change function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CFepCtrlDropdownList::SizeChanged(const TInt aCellWidth,
                                                 const TInt aCellHeight,
                                                 const TInt aNaviWidth,
                                                 const TPoint aTopLeftPoint,
                                                 const TInt aColNum,
                                                 const TInt aRowNum)
    {
    iCellWidth = aCellWidth; 
    iCellHeight = aCellHeight; 
    iNaviWidth = aNaviWidth;
    iSpinBtnHeight = aCellHeight;
    
    iBitmapDb->SizeChanged(TSize(aNaviWidth, aCellHeight));
    
    if ( aTopLeftPoint != TPoint(0,0) )
        {
        iTopLeftPoint = aTopLeftPoint;        
        }
    
    if ( aColNum )
        iColNum = aColNum;
    if ( aRowNum )
        iRowNum = aRowNum;  
    
    iListManager->SizeChanged();
    
	switch ( iListType )
		{
		case EListNoExpandable:
	        ChangeActiveList( iListManager->ListOneRowWithoutIcon() );
			break;
		case EListNoExpandableWithBubble:
	        {
	        ChangeActiveList(iListManager->ListOneRowWithoutIconWithBubble());
	        }
		    break;
		case EListExpandable:
		case EListExpandableWithoutIcon:
            {
            ChangeActiveList(iListManager->ListOneRowWithIcon());
            }
            break;
		case EListExpandableWithBubble:
		    {
		    ChangeActiveList(iListManager->ListOneRowWithIconWithBubble());
            }
            break;
		// txin
		case EListExpandableRollWithIcon:
            {
            TInt onerowCandIdx = OneRowStartCandIdx();

            if (onerowCandIdx != KInvalidCandIndex)
                {
                ChangeActiveList(iListManager->ListOneRowRollWithIcon(), onerowCandIdx);
                return;
                }

            ChangeActiveList(iListManager->ListOneRowRollWithIcon());
            }
	        break;
		case EListExpandableRollWithIconWithBubble:
            {
            TInt onerowCandIdx = OneRowStartCandIdx();

            if (onerowCandIdx != KInvalidCandIndex)
                {
                ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble(), onerowCandIdx);
                return;
                }

            ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble());
            }
	        break;
	        
        case EListExpandableMultiRowWithoutIcon:
            {
            ChangeActiveList(iListManager->ListExpandableMultiRowWithoutIcon());
            }
            break;            
	    default:
	    	__ASSERT_DEBUG( EFalse, 
	    			User::Panic( _L("TListType Error::SizeChanged()"), 0 ) );
	    	break;
		}
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
EXPORT_C CFepUiBaseCtrl* CFepCtrlDropdownList::HandlePointerDownEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl::HandlePointerDownEventL(aPoint);
    
    iListManager->ActiveList()->HandlePointerDownL(aPoint);
    
    return this;
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerUpEventL
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl*  CFepCtrlDropdownList::HandlePointerUpEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl::HandlePointerUpEventL(aPoint);
    
    iListManager->ActiveList()->HandlePointerUpL(aPoint);
    
    return this;
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::HandlePointerMoveEventL
// Handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepCtrlDropdownList::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    
    if(!PointerDown())
        {
        return NULL;
        }
    
    iListManager->ActiveList()->HandlePointerDragL(aPoint);
    
    return this;
    }

EXPORT_C void CFepCtrlDropdownList::HandlePointerLeave(const TPoint& aPoint)
    {
    CFepUiBaseCtrl::HandlePointerLeave(aPoint);
    if(!Hiden())
        iListManager->ActiveList()->HandlePointerLeave(aPoint);
    ClearBubble();
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::AddCandidatesL
// Add candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CFepCtrlDropdownList::AddCandidatesL(const RPointerArray<HBufC>& aCandidates,
                                                   const TListType /*aListType*/)
    {
    // add new candidates
    for (TInt i = iCandidatelist.Count(); i < aCandidates.Count(); i++)
        {
        CCandidate* candidate = CCandidate::NewL(*aCandidates[i], this);

        if (iCandidatelist.Append(candidate) != KErrNone)
            {
            delete candidate;
            }
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetCandidatesL
// Set candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CFepCtrlDropdownList::SetCandidatesL(const RPointerArray<HBufC>& aCandidates, 
                                                   const TListType aListType,
                                                   const TDesC& aFixedText)
    {
    //clear last candidates which had been set    
    iCandidatelist.ResetAndDestroy();
    
	iListManager->ResetStatus();        
	
    iHighlightList.Reset();
    
    iFixedText.Zero();
    iFixedText.Append(aFixedText);
    
    iFirstCandidateIndex = 0;
    iLastCandidateIndex = 0;
    // create or add new ones
    for( TInt i=0; i < aCandidates.Count(); i++ )
        {
        // CCandidate is s.c. self-containing class
        CCandidate* candidate = CCandidate::NewL(*aCandidates[i], this);
        // referring to memory object
        iCandidatelist.Append(candidate);
        }
    
	iListType = aListType;

	switch ( aListType )
		{
		case EListNoExpandable:
		    {
		    ChangeActiveList( iListManager->ListOneRowWithoutIcon() );
		    }
			break;
		case EListNoExpandableWithBubble:
		    {
		    ChangeActiveList( iListManager->ListOneRowWithoutIconWithBubble() );
		    }
			break;
		case EListExpandable:
		case EListExpandableWithoutIcon:
		    {
		    ChangeActiveList(iListManager->ListOneRowWithIcon());
		    }
		    break;
		case EListExpandableWithBubble:
		    {
		    ChangeActiveList(iListManager->ListOneRowWithIconWithBubble());
		    }
		    break;
		    
		// txin
		case EListExpandableRollWithIcon:
		    {
            SetOneRowStartCandIdx(KInvalidCandIndex);
            SetOriginalFirstCandIdx(KInvalidCandIndex);

            ChangeActiveList(iListManager->ListOneRowRollWithIcon());
		    }
	        break;
		case EListExpandableRollWithIconWithBubble:
		    {
            SetOneRowStartCandIdx(KInvalidCandIndex);
            SetOriginalFirstCandIdx(KInvalidCandIndex);

            ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble());
		    }
	        break;
	        
		case EListExpandableMultiRowWithoutIcon:
		    {
            ChangeActiveList( iListManager->ListExpandableMultiRowWithoutIcon() );
		    }
            break;        
	    default:
	    	__ASSERT_DEBUG( EFalse, 
	    			User::Panic( _L("TListType Error::SetCandidatesL()"), 0 ) );
	    	break;
		}
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::Move
// when the control is moved, the function will be called
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::Move(const TPoint& aOffset)
    {
    ClearRect(Rect());
    Invalidate(Rect(),ETrue);    
    iListManager->Move(aOffset);
    SetRect(iListManager->ActiveList()->Rect());
    iListManager->ActiveList()->Draw();
    Invalidate(iListManager->ActiveList()->Rect());
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::OnDeActivate
// when the control is hide, the function will be called
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::OnDeActivate()
    {
	iListManager->ResetStatus();
    }

// ---------------------------------------------------------------------------
// Hide or show dropdown list.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepCtrlDropdownList::Hide( TBool aFlag )
    {
    ClearBubble();
    CFepUiBaseCtrl::Hide( aFlag );
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::FoldList
// fold the drop down list control
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::FoldList()
    {
    if ((iListManager->ActiveList() == iListManager->ListMultiRowWithIcon()) 
    	|| (iListManager->ActiveList() == iListManager->ListMultiRowWithoutIcon()))
        {
        ChangeActiveList(iListManager->ListOneRowWithIcon(), iFirstCandidateIndex);            
        }
    // added by txin    
    else if (iListManager->ActiveList() == iListManager->ListMultiRowRollWithIcon())
        {
        TInt onerowCandIdx = OneRowStartCandIdx();

        if (onerowCandIdx != KInvalidCandIndex)
            {
            ChangeActiveList(iListManager->ListOneRowRollWithIcon(), onerowCandIdx);
            return;
            }

        ChangeActiveList(iListManager->ListOneRowRollWithIcon(), iFirstCandidateIndex);
        }
    else if (iListManager->ActiveList() == iListManager->ListMultiRowRollWithIconWithBubble())
        {
        TInt onerowCandIdx = OneRowStartCandIdx();

        if (onerowCandIdx != KInvalidCandIndex)
            {
            ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble(), onerowCandIdx);
            return;
            }

        ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble(), iFirstCandidateIndex);
        }
    else if(iListManager->ActiveList() == iListManager->ListMultiRowWithIconWithBubble())
        {
        ChangeActiveList(iListManager->ListOneRowWithIconWithBubble(), iFirstCandidateIndex);
        }
    // end    
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ResetAndClear
// reset and clear and redraw the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
EXPORT_C void CFepCtrlDropdownList::ResetAndClear(const TListType aListType)
    {
    //clear last candidates which had been set
    iCandidatelist.ResetAndDestroy();
    
    iFixedText.Zero();
   
    iFirstCandidateIndex = 0;
    iLastCandidateIndex = 0;
    
	switch ( aListType )
		{
		case EListNoExpandable:
	        ChangeActiveList( iListManager->ListOneRowWithoutIcon() );
			break;
		case EListNoExpandableWithBubble:
	        ChangeActiveList( iListManager->ListOneRowWithoutIconWithBubble() );
			break;
			
		case EListExpandable:
		case EListExpandableWithoutIcon:
			{
		    ChangeActiveList(iListManager->ListOneRowWithIcon());
		    NotifyClosePage();
			}
		    break;
		case EListExpandableWithBubble:
			{
		    ChangeActiveList(iListManager->ListOneRowWithIconWithBubble());
		    NotifyClosePage();
			}
		    break;
		    
		// txin
		case EListExpandableRollWithIcon:
		    {
		    ChangeActiveList(iListManager->ListOneRowRollWithIcon());
		    NotifyClosePage();
		    }
	        break;
		// end
		case EListExpandableRollWithIconWithBubble:
		    {
		    ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble());
		    NotifyClosePage();
		    }
	        break;
		case EListExpandableMultiRowWithoutIcon:
		    {
            ChangeActiveList(iListManager->ListExpandableMultiRowWithoutIcon());
            NotifyClosePage();		    
		    }
		    break;
	    default:
	    	__ASSERT_DEBUG( EFalse, 
	    			User::Panic( _L("TListType Error::ResetAndClear()"), 0 ) );
	    	break;
		}
    }


// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::HandleEvent
// when there is out event coming, the function will be called
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::HandleControlEvent(TInt aEventType, 
                                                       CFepUiBaseCtrl* aCtrl, 
                                                       const TDesC& aEventData)
    {
    if ( iListEventHandler )
        {
        iListEventHandler->HandleListOuterEvent(aEventType, aCtrl, aEventData);        
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::Draw
// draw the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::Draw()
    {   
	if(!AbleToDraw())
        return;
    
    if (!iFirstTimeConstruct)
    	{
        // if construct is not finished, then no need to draw
///DrawOpaqueMaskBackground();
        
        //ActivateBitmapContext();
        
  	    iListManager->ActiveList()->Draw();
  	    Invalidate(iListManager->ActiveList()->Rect());
    	}
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetHighlightCell
// set or unset the high light effect for the cell
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetHighlightCell(const TInt aCellIndex, 
                                                     TBool aHighlight, 
                                                     const TBool aIsDraw)
    {
    if (aHighlight)
        {

        iHighlightList.Reset();

        iHighlightList.Append(aCellIndex);
        
        if ((iListType == EListExpandableRollWithIcon ||
            iListType == EListExpandableRollWithIconWithBubble) && 
            (aCellIndex == GetFirstCandidateIndex()))
            {
            SetOneRowStartCandIdx(GetFirstCandidateIndex());
            }
        }
    else
        {
        TInt index = iHighlightList.Find(aCellIndex);
        
        if ( index != KErrNotFound )
            {
            iHighlightList.Remove(index);            
            }
        }
        
    if( aIsDraw )
        {
        iListManager->ActiveList()->SetHighlightCell(aCellIndex, aHighlight, EFalse);
        
        Draw();
        }
    }


// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::Rect
// get the rect of the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C const TRect& CFepCtrlDropdownList::Rect()
    {
    return iListManager->ActiveList()->Rect();
    }
    

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetEventIdForCandidateSelected
// set event id when candidate is selected
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetEventIdForCandidateSelected(TInt aEventId)
    {
    iEventIdForCandidateSelected = aEventId;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetEventIdForNextPageCandidate
// set event id when need get next page candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetEventIdForNextPageCandidate(TInt aEventId)
    {
	  iEventIdForNextPageCandidate = aEventId;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetEventIdForPreviousPageCandidate
// set event id when need get previous page candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetEventIdForPreviousPageCandidate(TInt aEventId)
    {
	  iEventIdForPreviousPageCandidate = aEventId;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetEventIdForCandidateExisted
// set event id when need judge if candidate exist
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetEventIdForCandidateExisted(TInt aEventId)
    {
    iEventIdForCandidateExisted = aEventId;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetTopLeftPoint
// set top left point of the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetTopLeftPoint(const TPoint aTopLeftPoint)
    {
    SizeChanged(iCellWidth, iCellHeight, iNaviWidth, aTopLeftPoint);
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetCellWidth
// set cell width
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetCellWidth(const TInt aCellWidth)
    {
    SizeChanged(aCellWidth, iCellHeight, iNaviWidth);
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetCellHeight
// set cell height
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetCellHeight(const TInt aCellHeight)
    {
    SizeChanged(iCellWidth, aCellHeight, iNaviWidth);
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetNaviWidth
// set navigation width
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetNaviWidth(const TInt aNaviWidth)      
    {
    SizeChanged(iCellWidth, iCellHeight, aNaviWidth);    
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetColNum
// set columns number
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetColNum(const TInt aColNum)
    {
    SizeChanged(iCellWidth, iCellHeight, iNaviWidth, iTopLeftPoint, aColNum);
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetRowNum
// Set rows number
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetRowNum(const TInt aRowNum)
    {
    SizeChanged(iCellWidth, iCellHeight, iNaviWidth, iTopLeftPoint, iColNum, aRowNum);    
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetBorderColor
// Set border color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetBorderColor(TRgb aBorderColor, TBool aReDraw)
    {
    iBorderColor = aBorderColor;
    
    if ( aReDraw )
        {
        Draw();
        }
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetBgColor
// Set background color for the whole DDl (drop down list)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetBgColor(TRgb aBgColor, TBool aReDraw)
    {
    iBgColor = aBgColor;
    
    if ( aReDraw )
        {
        Draw();
        }    
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetNavigationBgColor
// set background color for navigation area
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetNavigationBgColor(TRgb aNavigationBgColor, TBool aReDraw)
    {
    iNavigationBgColor = aNavigationBgColor;
    
    if ( aReDraw )
        {
        Draw();
        }
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetTextColor
// set text color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetTextColor(TRgb aTextColor, TBool aReDraw)
    {
    iTextColor = aTextColor;
    
    if ( aReDraw )
        {
        Draw();
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetSpecialTextColor
// set special text color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetSpecialTextColor(TRgb aSpecialTextColor, TBool aReDraw)
    {
    iSpecialTextColor = aSpecialTextColor;
    
    if ( aReDraw )
        {
        Draw();
        }    
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetHighLightTextColor
// set text color for highlight
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetHighLightTextColor(TRgb aHighLightTextColor, TBool aReDraw)
    {
    iHighLightTextColor = aHighLightTextColor;
    
    if ( aReDraw )
        {
        Draw();
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetHighLightBgColor
// set background color for highlight
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetHighLightBgColor(TRgb aHighLightBgColor, TBool aReDraw)
    {
    iHighLightBgColor = aHighLightBgColor;
    
    if ( aReDraw )
        {
        Draw();
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetTextAlighment
// set text alignment
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetTextAlighment(const CGraphicsContext::TTextAlign aAlignment,
                                                     TBool aReDraw)
    {
    iAlignment = aAlignment;
    
    if ( aReDraw )
        {
        Draw();
        }
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetTextAlighment
// set text alignment
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetTextMargin(const TInt aTextMargin, TBool aReDraw)
    {
    iTextMargin = aTextMargin;
    
    if ( aReDraw )
        {
        Draw();
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetCellColor
// set cell color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetSecondaryList(RArray<TInt>& aIndexList, TBool aReDraw)
    {
    iSecondaryList.Reset();
    for (TInt i=0; i < aIndexList.Count(); i++)
        {
        iSecondaryList.Append(aIndexList[i]);
        }
    
    if ( aReDraw )
        {
        Draw();
        }    
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetFont
// set text font
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetFont(const CFont* aFont, TBool aReDraw)
    {
    iFont = aFont;
    
    if ( aReDraw )
        {
		switch ( iListType )
			{
			case EListNoExpandable:
		        ChangeActiveList( iListManager->ListOneRowWithoutIcon() );
				break;
			case EListExpandable:
			case EListExpandableWithoutIcon:
			    ChangeActiveList(iListManager->ListOneRowWithIcon());
			    break;
			// txin
			case EListExpandableRollWithIcon:
		        ChangeActiveList(iListManager->ListOneRowRollWithIcon());
		        break;
			case EListExpandableRollWithIconWithBubble:
			    ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble());
			    break;
			case EListExpandableMultiRowWithoutIcon:
			    {
                ChangeActiveList( iListManager->ListExpandableMultiRowWithoutIcon() );
			    }
                break;			    
		    default:
		    	__ASSERT_DEBUG( EFalse, 
		    			User::Panic( _L("TListType Error::SetFont()"), 0 ) );
		    	break;
			}
        }     
    } 

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetListEventHandler
// set list event handler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::SetListEventHandler(MListEventHandler* aHandler)
    {
    iListEventHandler = aHandler;
    }
                            
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::CandidateCount
// get candidate count
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C TInt CFepCtrlDropdownList::CandidateCount() const
    {
    return iCandidatelist.Count(); 
    }                              

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetCellColorList
// get multi color list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
const RArray<TInt>& CFepCtrlDropdownList::GetSecondaryList() const
    {
    return iSecondaryList; 
    }
                                              
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::EnableMultiColor
// enable multi color or not 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CFepCtrlDropdownList::EnableSecondary(TBool aEnable, TBool aReDraw)
    {
    iEnableSecondary = aEnable;
    
    if ( aReDraw )
        {
        Draw();
        }    
    }
                                             
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::IsEnableSecondary
// get multi color is enable or disable
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C TBool CFepCtrlDropdownList::IsEnableSecondary() const
    {
    return iEnableSecondary;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::DrawMask
// draw mask
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
void CFepCtrlDropdownList::DrawMask(TRect aRect)
    {
    Gc()->Activate( MaskBitmapDevice() );
    Gc()->SetBrushStyle( CGraphicsContext::ESolidBrush );
    Gc()->SetBrushColor( TRgb(KOpaqueColor) );
    Gc()->SetPenSize(PenSize());
    Gc()->SetPenColor( TRgb(KOpaqueColor) );
    Gc()->DrawRect(aRect);    
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ActivateBitmapContext
// active bitmap context
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
void CFepCtrlDropdownList::ActivateBitmapContext()
    {
    Gc()->Activate(BitmapDevice());
    } 
        
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::Gc
// return the graphics context
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CFbsBitGc* CFepCtrlDropdownList::Gc()
    {
    return static_cast<CFbsBitGc*>(BitGc());
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetFirstCandidateIndex
// get FirstCandidateIndex
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetFirstCandidateIndex()
    {
    return iFirstCandidateIndex;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetLastCandidateIndex
// get LastCandidateIndex
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetLastCandidateIndex()
    {
    return iLastCandidateIndex;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetFirstCandidateIndex
// set FirstCandidateIndex
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CFepCtrlDropdownList::SetFirstCandidateIndex(TInt aFirstIndex)
    {
    iFirstCandidateIndex = aFirstIndex;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetLastCandidateIndex
// Set LastCandidateIndex
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CFepCtrlDropdownList::SetLastCandidateIndex(TInt aLastIndex)
    {
    iLastCandidateIndex = aLastIndex;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetCandidate
// get candidate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CCandidate* CFepCtrlDropdownList::GetCandidate(TInt aIndex)
    {
    if ( ( aIndex >= 0 ) && ( aIndex < iCandidatelist.Count() ) )
        {
        return iCandidatelist[aIndex];
        }
        
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetTopLeftPoint
// get Top Left Point
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TPoint CFepCtrlDropdownList::GetTopLeftPoint()
    {
    return iTopLeftPoint;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetCellWidth
// get width
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetCellWidth()
    {
    return iCellWidth;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetCellHeight
// get cell height
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetCellHeight()
    {
    return iCellHeight;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetNaviWidth
// get navigation width
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetNaviWidth()
    {
    return iNaviWidth;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetColNum
// get columns number
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetColNum()
    {
    return iColNum;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetRowNum
// get rows number
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetRowNum()
    {
    return iRowNum;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetFont
// get text font
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
const CFont* CFepCtrlDropdownList::GetFont()
    {
    return iFont;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetAlignment
// get alignment
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
CGraphicsContext::TTextAlign CFepCtrlDropdownList::GetAlignment()
    {
    return iAlignment;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetAlignment
// get alignment
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt CFepCtrlDropdownList::GetTextMargin()
    {
    return iTextMargin;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetBgColor
// get background color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TRgb CFepCtrlDropdownList::GetBgColor()
    {
    return iBgColor;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetButtonDownBgColor
// get button down color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TRgb CFepCtrlDropdownList::GetButtonDownBgColor()
    {
    return iButtonDownBgColor;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetBorderColor
// get border color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
TRgb CFepCtrlDropdownList::GetBorderColor()
    {
    return iBorderColor;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetTextColor
// get text color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TRgb CFepCtrlDropdownList::GetTextColor()
    {
    return iTextColor;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetHighLightBgColor
// get highlight background color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TRgb CFepCtrlDropdownList::GetHighLightBgColor()
    {
    return iHighLightBgColor;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetHighLightTextColor
// get highlignt text color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TRgb CFepCtrlDropdownList::GetHighLightTextColor()
    {
    return iHighLightTextColor;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetSpecialTextColor
// get special text color
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TRgb CFepCtrlDropdownList::GetSpecialTextColor()
    {
    return iSpecialTextColor;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetFixedText
// get fixed text
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TDesC CFepCtrlDropdownList::GetFixedText()
    {
    return iFixedText;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetHighlightList
// get bitmap manager
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
const RArray<TInt>& CFepCtrlDropdownList::GetHighlightList() const
    {
    return iHighlightList;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetPages
// get page manager
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
CPages* CFepCtrlDropdownList::GetPages()
    {
    return iPages;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetBitmapDb
// get bitmap manager
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CBitmapDb* CFepCtrlDropdownList::GetBitmapDb()
    {
    return iBitmapDb;
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ReportCandidateSelectEvent
// report candidate selected event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
void CFepCtrlDropdownList::ReportCandidateSelectEventL(const TDesC& aCandidate,
                                                      const TInt aSelectedIndex)
    {
    HBufC* bufc = HBufC::NewL( aCandidate.Length() + 1 );    
    TPtr ptr = bufc->Des();
    ptr.Copy( aCandidate );
    ptr.Append( (TUint16*)&aSelectedIndex, 1 );
    ReportEvent(iEventIdForCandidateSelected, *bufc);
    delete bufc;
    bufc = NULL;
    }
        
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::Invalidate
// update the animation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
void CFepCtrlDropdownList::Invalidate(const TRect& aRect, TBool aUpdateAll)
    {
    RootControl()->UpdateArea(aRect,aUpdateAll);
    }    

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ChangeActiveList
// change active list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFepCtrlDropdownList::ChangeActiveList(CList* aList, TInt aLastIndex)
    {
    TRect rect = Rect();       
        
    CList* beforeList = iListManager->ActiveList();
    
    if( RootControl()->CtrlCapPointer() == this)
        {
        SetCapture(EFalse);
        }
    
	switch ( iListType )
		{
		case EListNoExpandable:
		case EListNoExpandableWithBubble:
	        EnableSecondary(ETrue);
			break;
		case EListExpandable:
		case EListExpandableWithBubble:
		case EListExpandableWithoutIcon:
		// txin
		case EListExpandableRollWithIcon:
		case EListExpandableRollWithIconWithBubble:
		// end
	        EnableSecondary(EFalse);
	        break;
		case EListExpandableMultiRowWithoutIcon:
		    break;
	    default:
	    	__ASSERT_DEBUG( EFalse, 
	    			User::Panic( _L("TListType Error::ChangeActiveList()"), 0 ) );
	    	break;
		}
    
    iListManager->SetActiveList(aList);
    SetRect(iListManager->ActiveList()->Rect());      
    
    if ( aLastIndex )
        {
        SetLastCandidateIndex(aLastIndex); 
        }
    else
        {
        GetPages()->Reset();
        SetFirstCandidateIndex(0);
        SetLastCandidateIndex(0);
        }
        
    if(!Hiden() && Ready())
        {
        TRAP_IGNORE(iListManager->ActiveList()->UpdateListL());
        }
    
	// If the list is shrink, then redraw
    // txin
	if ( ( ( beforeList == iListManager->ListMultiRowWithIcon() )
		&& ( aList != iListManager->ListMultiRowWithIcon() ) ) 
		|| ( ( ( beforeList == iListManager->ListMultiRowWithoutIcon() )
	     && ( aList != iListManager->ListMultiRowWithoutIcon() ) ) ) 
	    || ( ( ( beforeList == iListManager->ListMultiRowRollWithIcon())
	     && ( aList != iListManager->ListMultiRowRollWithIcon() ) ) ) 
	     || ( ( ( beforeList == iListManager->ListExpandableMultiRowWithoutIcon() )
	     && ( aList != iListManager->ListExpandableMultiRowWithoutIcon() ) ) )
	     || ( ( ( beforeList == iListManager->ListMultiRowWithIconWithBubble() ))
	     && (aList != iListManager->ListMultiRowWithIconWithBubble() ) )
	     || ( ( ( beforeList == iListManager->ListMultiRowRollWithIconWithBubble() ))
	     && (aList != iListManager->ListMultiRowRollWithIconWithBubble() ) ) )
		{
	    rect.Shrink(TSize(0,-1));
	    RootControl()->ReDrawRect(rect);
	    Invalidate(rect,ETrue);			
		}
    // end
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::GetPenSize
// get pen size
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TSize CFepCtrlDropdownList::GetPenSize()
    {
    return PenSize();
    }

void CFepCtrlDropdownList::SetCapture(TBool aFlag)
    {
    CapturePointer(aFlag);
    }
    
// ---------------------------------------------------------------------------
// CFepCtrlDropdownList::SetActive
// Set active for the button
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetActive(TBool aActiveFlag)
    {
    if(IsActive() != aActiveFlag)
        {
        CFepUiBaseCtrl::SetActive(aActiveFlag);    
        /*Draw();
        UpdateArea(Rect(),EFalse);*/
        }
    }
    
// ---------------------------------------------------------------------------
// CFepCtrlDropdownList::ConstructFromResourceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::ConstructFromResourceL()
	{
    if (iResourceId == KInvalidResId)
    	{
    	User::Leave(KErrArgument);
    	}
    
    // first time construct will not go this way
    // only useful for skin change
    if (!iFirstTimeConstruct)
    	{
        TResourceReader reader;
        CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);
	    iBitmapDb->ConstructFromResourceL(reader, 
	                                      TSize(iCellWidth, iCellHeight),
	                                      TSize(iNaviWidth, iCellHeight),
	                                      TSize(iNaviWidth, iSpinBtnHeight));
	    CleanupStack::PopAndDestroy(); // reader
        SetTextColor(iBitmapDb->TextColor());
        SetBorderColor(iBitmapDb->FrameColor());
        iListManager->ActiveList()->UpdateListL(AbleToDraw(),EFalse);
	    Draw();
    	}
    
    iFirstTimeConstruct = EFalse;	
	}
	
// ---------------------------------------------------------------------------
// CFepCtrlDropdownList::NotifyGetNextPage
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepCtrlDropdownList::NotifyGetNextPage()
    {
    ReportEvent(iEventIdForNextPageCandidate);
    }
    
// ---------------------------------------------------------------------------
// CFepCtrlDropdownList::NotifyGetPreviousPage
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CFepCtrlDropdownList::NotifyGetPreviousPage()
    {
    ReportEvent(iEventIdForPreviousPageCandidate);
    } 
    
// ---------------------------------------------------------------------------
// CFepCtrlDropdownList::SetFlagCandidateExist
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetFlagCandidateExist(TBool aFlag)
    {
    iCandidateExisted = aFlag;
    }
    
// ---------------------------------------------------------------------------
// CFepCtrlDropdownList::NextCandidateExist
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CFepCtrlDropdownList::NextCandidateExist()
    {
    ReportEvent(iEventIdForCandidateExisted);
    
    return iCandidateExisted;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::AutoChangeActiveList
// change active list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFepCtrlDropdownList::AutoChangeActiveList( TInt aLastIndex )
	{
	switch ( iListType )
		{
		case EListExpandable:
			if ( iListManager->ActiveList() 
					== iListManager->ListOneRowWithIcon() )
				{
				ChangeActiveList( iListManager->ListMultiRowWithIcon(), aLastIndex );
				BringToTop();
				}
			else
				{
				BringToBack();
				ChangeActiveList( iListManager->ListOneRowWithIcon(), aLastIndex );
				NotifyClosePage();
				}
			break;
		case EListExpandableWithoutIcon:
			if ( iListManager->ActiveList() 
					== iListManager->ListOneRowWithIcon() )
				{
				ChangeActiveList( iListManager->ListMultiRowWithoutIcon(), aLastIndex );
				BringToTop();
				}
			else
				{
				BringToBack();
				ChangeActiveList( iListManager->ListOneRowWithIcon(), aLastIndex );
				}
			break;
        // txin
		case EListExpandableRollWithIcon:
		    {
			if (iListManager->ActiveList() == iListManager->ListOneRowRollWithIcon())
				{
				ChangeActiveList(iListManager->ListMultiRowRollWithIcon(), aLastIndex);
				BringToTop();
				}
			else
				{
				BringToBack();
				ChangeActiveList(iListManager->ListOneRowRollWithIcon(), aLastIndex);
				NotifyClosePage();
				}
		    }
		    break;
		case EListExpandableRollWithIconWithBubble:
		    {
			if (iListManager->ActiveList() == iListManager->ListOneRowRollWithIconWithBubble())
				{
				ChangeActiveList(iListManager->ListMultiRowRollWithIconWithBubble(), aLastIndex);
				BringToTop();
				}
			else
				{
				BringToBack();
				ChangeActiveList(iListManager->ListOneRowRollWithIconWithBubble(), aLastIndex);
				NotifyClosePage();
				}
		    }
		    break;
		    
		case EListExpandableWithBubble:
		    {
			if ( iListManager->ActiveList() 
					== iListManager->ListOneRowWithIconWithBubble() )
				{
				ChangeActiveList( iListManager->ListMultiRowWithIconWithBubble(), aLastIndex );
				BringToTop();
				}
			else
				{
				BringToBack();
				ChangeActiveList( iListManager->ListOneRowWithIconWithBubble(), aLastIndex );
				NotifyClosePage();
				}
		    }
		    break;
        case EListExpandableMultiRowWithoutIcon:
            break;              
		case EListNoExpandable:
		case EListNoExpandableWithBubble:
			break;
	    default:
	    	__ASSERT_DEBUG( EFalse, 
	    			User::Panic( _L("TListType Error::AutoChangeActiveList()"), 0 ) );
	    	break;
		}

	}

// added by txin in 2007.06.07
EXPORT_C void CFepCtrlDropdownList::SetSpinBtnHeight(const TInt aHeight)
    {
    // make sure navi next, navi previous plus close, do not beyond row count 
    TInt minExpandHeight = 2 * aHeight + GetCellHeight() + 4 * GetCellVerticalMargin();
    TInt realExpandHeight = GetCellHeight() * GetRowNum() + 
                            (GetRowNum() + 1) * GetCellVerticalMargin();

    if ((aHeight > 0) && (minExpandHeight <= realExpandHeight))
        {
        iSpinBtnHeight = aHeight;
        SizeChanged(iCellWidth, iCellHeight, iNaviWidth);
        }
    }

EXPORT_C void CFepCtrlDropdownList::SetCellMargin(const TInt aHorizontalMargin, 
                                                  const TInt aVerticalMargin)
    {
    if (((aHorizontalMargin > 0) && (aHorizontalMargin != iCellHorizontalMargin)) || 
        ((aVerticalMargin > 0) && (aVerticalMargin != iCellVerticalMargin)))
        {
        iCellHorizontalMargin = aHorizontalMargin;
        iCellVerticalMargin = aVerticalMargin;

        SizeChanged(iCellWidth, iCellHeight, iNaviWidth); 
        }
    }
    
TInt CFepCtrlDropdownList::GetSpinBtnHeight()
    {
    return iSpinBtnHeight;
    }

TInt CFepCtrlDropdownList::GetCellHorizontalMargin()
    {
    return iCellHorizontalMargin;
    }

TInt CFepCtrlDropdownList::GetCellVerticalMargin()
    {
    return iCellVerticalMargin;
    }

TInt CFepCtrlDropdownList::ListType()
    {
    return iListType;
    }
    
TInt CFepCtrlDropdownList::OneRowStartCandIdx()
    {
    return iOriginalCandIdx;
    }
    
void CFepCtrlDropdownList::SetOneRowStartCandIdx(TInt aOriginalCandIdx)
    {
    iOriginalCandIdx = aOriginalCandIdx;
    }

TInt CFepCtrlDropdownList::GetOriginalFirstCandIdx()
    {
    return iOriginalFirstCandIdx;
    }

void CFepCtrlDropdownList::SetOriginalFirstCandIdx(TInt aOriginalFirstCandIdx)
    {
    iOriginalFirstCandIdx = aOriginalFirstCandIdx;
    }
    
EXPORT_C void CFepCtrlDropdownList::SetCandTruncateType(const TCandTruncateType aTruncateType)
    {
    iCandTruncateType = aTruncateType;
    }
    
TInt CFepCtrlDropdownList::CandTruncateType()
    {
    return iCandTruncateType;
    }
    
CList* CFepCtrlDropdownList::ActiveList()
    {
    return iListManager->ActiveList();
    }

void CFepCtrlDropdownList::SetUpdateListReDrawFlag(TBool aReDraw)
    {
    iUpdateListReDraw = aReDraw;
    }

TBool CFepCtrlDropdownList::GetUpdateListRedrawFlag()
    {
    return iUpdateListReDraw;
    }

EXPORT_C void CFepCtrlDropdownList::SetFuzzyBoundry(TInt aFuzzyBoundry)
    {
    if (aFuzzyBoundry != iFuzzyBoundry && aFuzzyBoundry >= 0)
        {
        iFuzzyBoundry = aFuzzyBoundry;
        }
    }

TInt CFepCtrlDropdownList::FuzzyBoundry()
    {
    return iFuzzyBoundry;
    }

EXPORT_C void CFepCtrlDropdownList::SetCandExpandType(const TExpandType aType)
    {
    if (aType != iCandExpandType)
        {
        iCandExpandType = aType;
        }
    }

TInt CFepCtrlDropdownList::CandExpandType()
    {
    return iCandExpandType;
    }

void CFepCtrlDropdownList::NotifyClosePage()
	{
	UiLayout()->RootControl()->Draw();
	}
	
EXPORT_C void CFepCtrlDropdownList::SetDropdownListImgID(
              TDropdownListDrawInfo aDropdownListDrawInfo)
	{
	iListManager->ListOneRowWithIcon()->SetDropdownListImgID(aDropdownListDrawInfo);
	
	iListManager->ListOneRowWithIconWithBubble()->SetDropdownListImgID(aDropdownListDrawInfo);
													 
	iListManager->ListMultiRowWithIcon()->SetDropdownListImgID(aDropdownListDrawInfo);		
	
	iListManager->ListMultiRowWithIconWithBubble()->SetDropdownListImgID(aDropdownListDrawInfo);	
													 
	iListManager->ListOneRowWithoutIcon()->SetDropdownListImgID(aDropdownListDrawInfo);		
	
    iListManager->ListOneRowWithoutIconWithBubble()->SetDropdownListImgID(aDropdownListDrawInfo);			
													 
	iListManager->ListOneRowRollWithIcon()->SetDropdownListImgID(aDropdownListDrawInfo);
			
	iListManager->ListOneRowRollWithIconWithBubble()->SetDropdownListImgID(aDropdownListDrawInfo);		
													 
	iListManager->ListMultiRowRollWithIcon()->SetDropdownListImgID(aDropdownListDrawInfo);
	
	iListManager->ListMultiRowRollWithIconWithBubble()->SetDropdownListImgID(aDropdownListDrawInfo);
	
	iListManager->ListExpandableMultiRowWithoutIcon()->SetDropdownListImgID(aDropdownListDrawInfo);
	}

CFepUiLayout* CFepCtrlDropdownList::UiLayout()
    {
    return CFepUiBaseCtrl::UiLayout();
    }

TInt CFepCtrlDropdownList::CandHDirection()
    {
    return iCandHDirection;
    }

TBool CFepCtrlDropdownList::PageCtrlShown()
    {
    return iPageCtrlShown;
    }
    
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ShowBubble
// Set show preview bubble on/off
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::ShowBubble(TBool aFlag)
    {  
    TBool flag = iBubbleCtrl ? ETrue : EFalse;
    iIsShowBubble = aFlag;
    if ( flag != aFlag )
        {
        if(!flag)
            {
            TRAP_IGNORE(iBubbleCtrl = CBubbleCtrl::NewL(TRect(0,0,0,0),UiLayout(),-0xFFFF));
            }
        }
    }    

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetBubbleBitmapParam
// Set preview bubble bitmap
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetBubbleBitmapParam(CFbsBitmap* aBmpId,
                                                     CFbsBitmap* aMaskBmpId ,
                                                     TAknsItemID aBgSkinId )
    {
    if ( iBubbleCtrl )
        {
        iBubbleCtrl->SetBitmapParam(aBmpId,aMaskBmpId,aBgSkinId);
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetBubbleTextFont
// Set preview bubble text font
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetBubbleTextFont(const CFont* aFont)
    {
    if ( iBubbleCtrl )
        {
        iBubbleFont = aFont;    
        }
    }
       
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetTextFormat
// Set preview bubble text format
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetTextFormat(TAknTextLineLayout aTextFormat)
    {
    if ( iBubbleCtrl )
        {
        iBubbleCtrl->SetTextFormat(aTextFormat);   
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetBubbleSize
// Set preview bubble size
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetFrameDiff(TInt aLeftDiff, TInt aTopDiff,
                                             TInt aRightDiff,TInt aBottomDiff)
    {
    if ( iBubbleCtrl )
        {
        iBubbleCtrl->SetFrameDiff(aLeftDiff,aTopDiff,aRightDiff,aBottomDiff);    
        }        
    }    

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::SetBubbleSize
// Set preview bubble size
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::SetBubbleSize(const TSize& aSize)
    {
    iBubbleSize = aSize;
    }


// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::DrawBubble
// Show preview bubble
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFepCtrlDropdownList::DrawBubble(const TRect aRect, const TDesC& aText)
    {
    if ( iBubbleCtrl && iIsShowBubble && !(iBubbleCtrl->IsShowing()))
        {
        if(iBubbleFont)
            {
            iBubbleSize.iWidth = iBubbleFont->TextWidthInPixels(aText) + KPreviewBubbleOffset;
            }
            
        if(iBubbleSize.iWidth < aRect.Width() + KPreviewBubbleOffset )
            {
            iBubbleSize.iWidth = aRect.Width() + KPreviewBubbleOffset;
            }
         
        
        TRect rect = iBubbleCtrl->Rect();
        TRect parentWndRect = Rect();       
        TInt x1 = aRect.iTl.iX - ( iBubbleSize.iWidth - aRect.Width() ) / 2;
        TInt x2 = aRect.iBr.iX + ( iBubbleSize.iWidth - aRect.Width() ) / 2;
        
        TRect screenRect;
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, screenRect); 
        if ( x1 < 0 )
            {
            rect.iTl.iX = screenRect.iTl.iX;     
            rect.iBr.iX = screenRect.iTl.iX + iBubbleSize.iWidth; 
            }
        else if ( x2 > screenRect.iBr.iX )
            {
            rect.iTl.iX = screenRect.iBr.iX - iBubbleSize.iWidth;     
            rect.iBr.iX = screenRect.iBr.iX;                 
            }
        else
            {
            rect.iTl.iX = x1;     
            rect.iBr.iX = x2;                 
            }
        
        rect.iTl.iY = iBubbleVerticalMargin + aRect.iTl.iY - 
                      iBubbleSize.iHeight;
        rect.iBr.iY = iBubbleVerticalMargin + aRect.iTl.iY;
		
		TRAP_IGNORE(iBubbleCtrl->SetTextL(aText));		
	
        iBubbleCtrl->Popup(rect);
        }
    }

// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ClearBubble
// Hide preview bubble
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFepCtrlDropdownList::ClearBubble()
    {
    if ( iBubbleCtrl && iBubbleCtrl->IsShowing())
        {
        iBubbleCtrl->Close();
        }
    }
       
// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ReDrawBubble
// ReDraw preview bubble
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFepCtrlDropdownList::ReDrawBubble()
    {
    if(iBubbleCtrl && iBubbleCtrl->IsShowing())
        {
        iBubbleCtrl->Draw();
        }
    }


// -----------------------------------------------------------------------------
// CFepCtrlDropdownList::ReDrawRect
// ReDraw Dropdown list rect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepCtrlDropdownList::ReDrawRect(const TRect& /*aRect*/)
    {   
	if(!AbleToDraw())
        return;
    
    if (!iFirstTimeConstruct)
    	{
  	    iListManager->ActiveList()->Draw();
    	}
    } 
	
// end adding

// End Of File
