/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for avkon based choice list control
*
*/


#include "peninputlayoutaknchoicelist.h"
#include "peninputlayout.h"

#include <avkon.hrh> // AknButton flags
#include <AknsConstants.h>
//#include <avkon.mbg>

#include <eikclbd.h>
#include <eiklbx.h>
#include <eikscrlb.h>

#include <AknLayout2ScalableDef.h>
#include <aknlayoutscalable_avkon.cdl.h>

#include <AknsFrameBackgroundControlContext.h> 



const TInt KMaxTxtLen = 256;

NONSHARABLE_CLASS(CPenUiScrolbarCtrl) : public CFepUiBaseCtrl
    {
    public:
        CPenUiScrolbarCtrl(CCoeControl* aCtrl, CFepUiLayout* aLayout);
        void ConstructL();         
    };

CPenUiScrolbarCtrl::CPenUiScrolbarCtrl(CCoeControl* aCtrl, CFepUiLayout* aLayout)
                : CFepUiBaseCtrl(aCtrl,aLayout,EFalse)
    {
    }

void CPenUiScrolbarCtrl::ConstructL()
    {
    BaseConstructL();
    }
    
// implementation for class     CFepLayoutAknChoiceList
EXPORT_C  CFepLayoutAknChoiceList* CFepLayoutAknChoiceList::NewL(CFepUiLayout* aUiLayout,
                    TInt aControlId,CCoeControl* aParent)
    {
    CFepLayoutAknChoiceList* self = new (ELeave) CFepLayoutAknChoiceList(aUiLayout,
                                                                         aControlId,aParent);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;        
    }

EXPORT_C CFepLayoutAknChoiceList::CFepLayoutAknChoiceList(CFepUiLayout* aUiLayout, 
                                  TInt aControlId,CCoeControl* aParent)
                                  : CFepLayoutChoiceList(aUiLayout,aControlId)
    {
    iParent = aParent;
    }

EXPORT_C CFepLayoutAknChoiceList::~CFepLayoutAknChoiceList()
    {
    delete iScrollbarCtrl;
    delete iTxtTransferBuf;
    delete iBgContext;
    }
    
EXPORT_C void CFepLayoutAknChoiceList::ConstructL()
    {    
    CFepLayoutChoiceList::ConstructL();

    iListBox = new(ELeave) CAknSinglePopupMenuStyleListBox ;//CAknSingleStyleListBox;
    
    iListBox->ConstructL(iParent,EAknListBoxMenuList); 
    iListBox->MakeVisible(EFalse);

   	      
    CWindowGc* gc = RootControl()->GetRWindowGcL();//(static_cast<CGenericItutUiLayout*>(UiLayout()))->WindowGc();
    iListBox->SetGc(gc);
    
    iListBox->SetGc(gc);
    iListBox->ItemDrawer()->SetGc(gc);    

    iListBox->CreateScrollBarFrameL(ETrue,EFalse,EFalse);    
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, 
                                                        CEikScrollBarFrame::EAuto);
    
    CEikScrollBar* scrbar = iListBox->ScrollBarFrame()->VerticalScrollBar();
    iScrollbarCtrl = new(ELeave) CPenUiScrolbarCtrl(scrbar, UiLayout());
    static_cast<CPenUiScrolbarCtrl*>(iScrollbarCtrl)->ConstructL();
    UpdateTextColors();
    
    CFormattedCellListBoxData* d = iListBox->ItemDrawer()->FormattedCellData();
    TBool b = d->SubCellIsTransparent(0);
    TMargins m = d->SubCellMargins(0);
    //UseSubCellColors
    
    
    iWndControl = iListBox;
    
    iTxtTransferBuf = HBufC::NewL(KMaxTxtLen);    
    
    TRect nullRect( 0, 0, 0, 0 );

    iBgContext = CAknsFrameBackgroundControlContext::NewL(
        KAknsIIDQsnFrPopupSub, nullRect, nullRect, EFalse );        
        
    
    }
    
EXPORT_C void CFepLayoutAknChoiceList::SetItemsL(const RPointerArray<CFepLayoutChoiceList::SItem>& 
                                                 aItemList)
    {
    TRect r = iRect;
    CFepLayoutChoiceList::SetItemsL(aItemList);
    //
    CDesCArray* itemArray = static_cast<CDesCArray*>(FormatedCellListBox()->Model()->ItemTextArray());
      //= (CDesCArray*) itemList;

    for(int ii = 0; ii < aItemList.Count(); ++ii)
        {
        itemArray->AppendL(aItemList[ii]->iText);
        }    
    iListBox->HandleItemAdditionL();        
    SetRect(r);
    }


EXPORT_C void CFepLayoutAknChoiceList::AddItemL(const CFepLayoutChoiceList::SItem& aItem)
    {
    TRect r = iRect;
    CFepLayoutChoiceList::AddItemL(aItem);
    CDesCArray* itemArray = static_cast<CDesCArray*>(FormatedCellListBox()->Model()->ItemTextArray());    
    itemArray->AppendL(AdjustColumnText(aItem.iText));
    iListBox->HandleItemAdditionL();    
    SetRect(r);
    }
    
    
EXPORT_C void CFepLayoutAknChoiceList::InsertItemL(TInt aPostion, 
                                                   const CFepLayoutChoiceList::SItem& aItem)
    {
    TRect r = iRect;
    CFepLayoutChoiceList::InsertItemL(aPostion,aItem);
    CDesCArray* itemArray = static_cast<CDesCArray*>(FormatedCellListBox()->Model()->ItemTextArray());     
    itemArray->InsertL(aPostion,AdjustColumnText(aItem.iText));   
    iListBox->HandleItemAdditionL();    
    SetRect(r);
    }

EXPORT_C void CFepLayoutAknChoiceList::RemoveItemByCommand(TInt aCommand)
    {
    TInt idx = FindCommand(aCommand);
    if(idx > 0)
        {        
        RemoveItemByIndex(idx);
        }
    }
    
void CFepLayoutAknChoiceList::RemoveItemByIndexL(TInt aIndex)
    {
    CDesCArray* itemArray = static_cast<CDesCArray*>(FormatedCellListBox()->Model()->ItemTextArray());    
    __ASSERT_DEBUG(aIndex >= 0 && aIndex < itemArray->Count(),User::Leave(EUiGeneralIndexError));
    CFepLayoutChoiceList::RemoveItemByIndex(aIndex);
    itemArray->Delete(aIndex);
    TRAP_IGNORE(iListBox->HandleItemRemovalL());
    }

EXPORT_C void CFepLayoutAknChoiceList::RemoveItemByIndex(TInt aIndex)
    {
    TRAP_IGNORE(RemoveItemByIndexL(aIndex));
    }
    
EXPORT_C void CFepLayoutAknChoiceList::ClearItemsL()
    {
    CFepLayoutChoiceList::ClearItemsL();    
    CDesCArray* itemArray = static_cast<CDesCArray*>(FormatedCellListBox()->Model()->ItemTextArray());    
    itemArray->Reset();
    }
    
EXPORT_C TInt CFepLayoutAknChoiceList::ItemsCount()
    {
    CDesCArray* itemArray = static_cast<CDesCArray*>(FormatedCellListBox()->Model()->ItemTextArray());    
    return itemArray->Count();
    }
    
EXPORT_C void CFepLayoutAknChoiceList::Draw()
    {
    
    if(iWndControl && AbleToDraw())
        {
  		DrawOpaqueMaskBackground();              
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
        gc->Activate( BitmapDevice() );

        const TRect& r = Rect();
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRAP_IGNORE(CWindowGc& sysgc = *(RootControl()->GetRWindowGcL()));
        //if ( !AknsDrawUtils::Background( skin, iBgContext, sysgc, r ) )
            {
            gc->Clear( Rect());
            }
        
        iWndControl->DrawNow();
        UpdateArea(iRect, EFalse);        
        }

    }
    
EXPORT_C void CFepLayoutAknChoiceList::SetItemRect(const TRect& , const TRect& )
    {
    //do nothing
    }

// CFepLayoutChoiceList::OnDisplay
// Prepare display.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutAknChoiceList::OnDisplay(const TPoint& /*aOffset*/)
    {
    //set window position
    //iListBox->ActivateL();	
    //TBool b = iListBox->IsVisible();    
    TPoint layoutPos = UiLayout()->LayoutOwner()->LayoutRect().iTl;
    layoutPos = TPoint(0,0);
    //CFepLayoutChoiceList::OnDisplay(aOffset);
 //   iListBox->SetExtent(layoutPos + aOffset,Rect().Size());    
    
    CListBoxView* view = iListBox->View();
    TRect r = view->ViewRect();
    r.iBr.iY -= 50; 
    view->SetViewRect(r);

    TSize is = view->ItemSize(0);    
    TInt w = MinItemWidthInPixels();
    TSize s = iListBox->ItemDrawer()->MinimumCellSize();
    TSize ls = iListBox->ItemDrawer()->LafItemSize ();
    
    UpdateControlContext();
    UpdateTextColors();
    iListBox->MakeVisible(ETrue);
    TRAP_IGNORE(iListBox->ActivateL());	    
//    iListBox->ItemDrawer()->FormattedCellData()->SetSkinEnabledL( EFalse );
    // Marquee can be requested.. ;) 
  //  iListBox->ItemDrawer()->FormattedCellData()->EnableMarqueeL( EFalse );
    
    }



EXPORT_C CEikColumnListBox* CFepLayoutAknChoiceList::ListBox()
    {
    return 0;//iListBox;
    }
EXPORT_C CEikFormattedCellListBox* CFepLayoutAknChoiceList::FormatedCellListBox()
    {
    return iListBox;
    }

void CFepLayoutAknChoiceList::UpdateTextColors()
    {
    TRgb color( KRgbBlack );  // sane default for nonskinned case
    if ( AknsUtils::AvkonSkinEnabled() )
        {
        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                               color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG8 );
        }

    iListBox->ItemDrawer()->SetTextColor( color );
    iListBox->ItemDrawer()->SetBackColor( AKN_LAF_COLOR( 0 ) );

    if ( AknsUtils::AvkonSkinEnabled() )
        {
        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                               color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG8 );
        }

    CFormattedCellListBoxData* d = iListBox->ItemDrawer()->FormattedCellData();

    iListBox->ItemDrawer()->SetHighlightedTextColor( color );
    iListBox->ItemDrawer()->SetHighlightedBackColor( AKN_LAF_COLOR( 0 ) );    
    }
    
void CFepLayoutAknChoiceList::UpdateControlContext()
    {
    
        CFormattedCellListBoxItemDrawer* itemDrawer = iListBox->ItemDrawer();
        CFormattedCellListBoxData* d = itemDrawer->FormattedCellData();        

        TAknLayoutRect cellRect;
        cellRect.LayoutRect( Rect(), 
            AknLayoutScalable_Avkon::list_single_choice_list_pane(0) );
        
        TRect cell( cellRect.Rect() );

        TRAP_IGNORE(iListBox->SetItemHeightL( cell.Height() ));

        //d->SetTextSubCellL( 0, AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(0) );    
    {
    
    const TAknTextLineLayout& aTextLayout =  
                                       AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(0);
            
    TAknLayoutText t;
    TInt subCell = 0;
    t.LayoutText( iListBox->View()->ItemDrawer()->ItemCellSize(), aTextLayout );
    CFormattedCellListBoxData* d = iListBox->ItemDrawer()->FormattedCellData();
    TRAP_IGNORE(d->SetSubCellSizeL( subCell, t.TextRect().Size() ));
    TRAP_IGNORE(d->SetSubCellPositionL( subCell, t.TextRect().iTl ));
    TRAP_IGNORE(d->SetSubCellFontL( subCell, t.Font() ));
    TRAP_IGNORE(d->SetSubCellAlignmentL( subCell, 
                                         AknLayoutUtils::TextAlignFromId( aTextLayout.iJ ) ));
    TInt B = 22;//AknLayoutUtils::CorrectBaseline( iListBox->View()->ItemDrawer()->ItemCellSize().iHeight, aTextLayout.iB, aTextLayout.FontId() );
    TRAP_IGNORE(d->SetSubCellBaselinePosL( subCell, B ));
    }
    
    
    {
        TAknLayoutRect cellRect;
        cellRect.LayoutRect( Rect(), 
            AknLayoutScalable_Avkon::list_single_choice_list_pane(0) );
            
        iListBox->View()->ItemDrawer()->SetItemCellSize( cellRect.Rect().Size() );
            
        TRAP_IGNORE(iListBox->SetItemHeightL( cell.Height()*2 ));            
    
    }
/*    TRect rectParent( Rect() );
                      
    TAknLayoutRect layoutRect;
    TInt variety = 0;

    // check if scrollbar-layout should be selected
    if ( ScrollbarVisible() )
        {
        variety = 1;

        iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOn);            
        
        AknLayoutUtils::LayoutVerticalScrollBar( iListBox->ScrollBarFrame(), 
            rectParent, 
            AknLayoutScalable_Avkon::scroll_pane_cp23().LayoutLine()
        );                    
        }
    else
        {
        iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOff);
        }
        
    layoutRect.LayoutRect( rectParent, 
        AknLayoutScalable_Avkon::list_choice_list_pane( variety ).LayoutLine() );        
    iListBox->SetRect( layoutRect.Rect() );
            
    TAknLayoutRect innerRect;
    innerRect.LayoutRect( 
        rectParent, 
        AknLayoutScalable_Avkon::list_choice_list_pane(0).LayoutLine() 
        );
    
    iBgContext->SetFrameRects(rectParent, innerRect.Rect());        */
    }    
// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::SetRect
// Set the pop up window's rect
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepLayoutAknChoiceList::SetRect(const TRect& aRect)
    {    
    
    //Ensure that the top left of pop up window is always (0,0)
    //iRect.Move(-aRect.iTl.iX,-aRect.iTl.iY);    
    if(iRect == aRect)
        return;
    
    iRect = aRect;  
    /*      
    if(iWndControl)
        iWndControl->SetRect(iRect); 
        */       
    iListBox->SetRect( aRect);        
    UpdateControlContext();        
    if(iScrollbarCtrl)
        {        
        //set the rect of the scroll bar    
        TRect rect = iScrollbarCtrl->WndControl()->Rect();
        iScrollbarCtrl->SetRect(rect);
        }
    
    }

EXPORT_C TBool CFepLayoutAknChoiceList::ScrollbarVisible()
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CFepLayoutAknChoiceList::HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
EXPORT_C CFepUiBaseCtrl* CFepLayoutAknChoiceList::HandlePointerDownEventL(const TPoint& aPoint)
    {
    if(ScrollbarVisible())
        {
        if(iScrollbarCtrl->Rect().Contains(aPoint))
            {
            iScrollbarCaptured = ETrue;
            return iScrollbarCtrl->HandlePointerDownEventL(aPoint);
            }
        }
    CFepUiBaseCtrl::HandlePointerDownEventL(aPoint);
    //CFepLayoutChoiceList::HandlePointerDownEventL(aPoint);
    return this;
    }

// ---------------------------------------------------------------------------
// CFepLayoutAknChoiceList::HandlePointerUpEventL
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CFepLayoutAknChoiceList::HandlePointerUpEventL(
                                                         const TPoint& aPoint)
    {
    if(iScrollbarCaptured)
        {
        iScrollbarCaptured = EFalse;
        return iScrollbarCtrl->HandlePointerUpEventL(aPoint);
        }
    //CFepUiBaseCtrl::HandlePointerUpEventL(aPoint);
    CFepLayoutChoiceList::HandlePointerUpEventL(aPoint);
    return this;
    }

// ---------------------------------------------------------------------------
// CFepLayoutAknChoiceList::HandlePointerMoveEventL
// Handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepLayoutAknChoiceList::HandlePointerMoveEventL(
                                                        const TPoint& aPoint)
    {
    if(iScrollbarCaptured)
        {
        iScrollbarCtrl->HandlePointerMoveEventL(aPoint);
        }
    if(PointerDown()) //only handle move event after pointer down
        CFepUiBaseCtrl::HandlePointerMoveEventL(aPoint);
    //CFepLayoutChoiceList::HandlePointerMoveEventL(aPoint);
    return this;

    }

// ---------------------------------------------------------------------------
// CFepLayoutAknChoiceList::AdjustColumnText
// Adjust the txt which is added to column list.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
const TDesC& CFepLayoutAknChoiceList::AdjustColumnText(const TDesC& aTxt)
    {
    return aTxt;
    /*_LIT(KLitTabulate,"\t");
    if(aTxt.Find(KLitTabulate) >=0)
        return aTxt;
    if(KMaxTxtLen < aTxt.Length() + KLitTabulate().Length())
        {
        delete iTxtTransferBuf;
        iTxtTransferBuf = 0;
        iTxtTransferBuf = HBufC::NewL(aTxt.Length() + KLitTabulate().Length());        
        }
    TPtr ptr = iTxtTransferBuf->Des();    
    ptr.Zero();
    ptr.Append(KLitTabulate);
    ptr.Append(aTxt);
    return *iTxtTransferBuf;*/
    }

// CFepLayoutAknChoiceList::ReCalcLayout
// Calculates the displaying rect.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutAknChoiceList::ReCalcLayout()
    {
    //do nothing
    }
    								       
EXPORT_C TInt CFepLayoutAknChoiceList::MinItemWidthInPixels()
    {
    
    TInt minWidth = 0;
    for(TInt i = 0; i < ItemsCount(); ++i)
        {
        TInt itemWidth = iListBox->ItemDrawer()->ItemWidthInPixels (i);
        minWidth = itemWidth > minWidth ? itemWidth : minWidth;
        }
        
      
    CFormattedCellListBoxData* d = iListBox->ItemDrawer()->FormattedCellData();
    TSize s1 = d->SubCellSize(0);
    TSize s2 = d->SubCellSize(1);
    return minWidth;	      
    }

//END OF FILE

