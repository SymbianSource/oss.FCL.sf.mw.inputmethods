/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Predictive candidate list
*
*/


// INCLUDE FILES
#include <aknlayoutscalable_avkon.cdl.h>

#include "peninputeventjp.h"
#include "peninputjapanesepredictivelistboxview.h"
#include "peninputjapanesepredictiveListboxitemdrawer.h"
#include "peninputjapanesepredictivecelllistboxdata.h"
#include "peninputjapanesepredictivelistboxmodel.h"
#include "peninputjapanesepredictivelistbox.h"
#include "peninputjapanesepredictivewnd.h"

// CONSTANTS
const TInt KScrollRightMarge = 5;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::ConstructL(
        const CCoeControl* aParent,
        TInt aFlags)
    {
    CAknFilteredTextListBoxModel* model =
            new(ELeave) CPeninputJapanesePredictiveListBoxModel;
    iModel=model;
    model->ConstructL();
    CreateItemDrawerL();

    EnableExtendedDrawingL();

    iItemDrawer->SetDrawMark(EFalse);
    CEikListBox::ConstructL(aParent,aFlags);
    iLineCount = KPENINPUT_LAYOUT_PREDICTIONPANE_ROW_COUNT;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::SetCurrentCandidateNo
// Selecting a candidate number appointed with an argument
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::SetCurrentCandidateNo(
        TInt aCandidateNo, TBool aDraw) const
    {
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());
    TInt itemIndex = view->SetCurrentCandidateNo(aCandidateNo);
    if (itemIndex >= 0)
        {
        if (aDraw)
            {
            SetCurrentItemIndexAndDraw(itemIndex);
            }
        else
            {
            SetCurrentItemIndex(itemIndex);
            }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::SetCurrentCandidateNo
// Selecting a candidate number appointed with an argument
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::SetCurrentCandidateNo(
        TInt aLineNo, const TPoint& aPoint, TBool aDraw) const
    {
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());
    TInt itemIndex = view->SetCurrentCandidateNo(aLineNo, aPoint);

    if (aDraw)
        {
        SetCurrentItemIndexAndDraw(itemIndex);
        }
    else
        {
        SetCurrentItemIndex(itemIndex);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::CurrentCandidateNo
// Returning a selected number
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBox::CurrentCandidateNo() const
    {
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());
    return view->CurrentCandidateNo();
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::MovePreviousItem
// selecting a previous candidate
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBox::MovePreviousItem() const
    {
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());

    TInt candidateNo = view->PreviousItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::MoveNextItem
// selecting a next candidate
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBox::MoveNextItem() const
    {
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());

    TInt candidateNo = view->NextItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::MovePreviousLineItem
// selecting a previous line candidate
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBox::MovePreviousLineItem() const
    {
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());
    TInt candidateNo = view->PreviousLineItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::MoveNextLineItem
// selecting a next line candidate
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBox::MoveNextLineItem() const
    {
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());
    TInt candidateNo = view->NextLineItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::HandlePointerEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    CAknSinglePopupMenuStyleListBox::HandlePointerEventL(aPointerEvent);
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::Draw
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::Draw() const
    {
    CWindowGc* gcSave = CCoeEnv::Static()->SwapSystemGc(ItemDrawer()->Gc());
    DrawNow();
    CCoeEnv::Static()->SwapSystemGc(gcSave);
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::SetLineCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::SetLineCount(TInt aLineCount)
    {
    iLineCount = aLineCount;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::SizeChanged
// This function is called in case changing size
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::SizeChanged()
    {
    CEikFormattedCellListBox& listBox = *this;
    CFormattedCellListBoxItemDrawer* itemDrawer = listBox.ItemDrawer();

    TAknWindowLineLayout textWindowLayout =
        AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine();

    TAknTextLineLayout textLineLayout =
        AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();
    const CFont* font = AknLayoutUtils::FontFromId(textLineLayout.iFont, NULL);
    textWindowLayout.iH = Rect().Size().iHeight/2;  // control itemheight

    AknListBoxLayouts::SetupStandardListBox(listBox);  // SetBorder(TGulBorder::ENone)
    AknListBoxLayouts::SetupStandardFormListbox(itemDrawer); // Set Standard Colors
    AknListBoxLayouts::SetupListboxPos(listBox, textWindowLayout);  // ItemDrawer()->SetItemCellSize()
    {
    TRgb color( KRgbBlack );
    // this does not modify color unless it gets a correct one
    // no real need to check errors
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                              color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG10 );
    itemDrawer->SetHighlightedTextColor( color );
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                               color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG7 );
    iItemDrawer->SetTextColor( color );
    }

    TSize itemSize = View()->ItemSize();

    // setting textbox size in listbox
    TInt baseline = itemSize.iHeight/2 + font->AscentInPixels() / 2;
    textLineLayout.iB = ELayoutP - baseline;  // control itemheight

    AknListBoxLayouts::SetupFormAntiFlickerTextCell(listBox, itemDrawer, 0,
                            textLineLayout,
                            TPoint(0, 0),
                            TPoint(itemSize.iWidth, itemSize.iHeight));

    CEikFormattedCellListBox::SizeChanged();

    // Scroll bar
    TAknWindowLineLayout scrollLineLayout = AknLayoutScalable_Avkon::scroll_pane_cp30().LayoutLine();
    scrollLineLayout.il = ELayoutEmpty;
    scrollLineLayout.ir = KScrollRightMarge;
    scrollLineLayout.it = 0;
    scrollLineLayout.ib = 0;
    TRect scrollBarParent = TRect(TPoint(listBox.Rect().iTl),
                                  TSize(listBox.Rect().Width() + scrollLineLayout.iW + scrollLineLayout.ir,
                                        listBox.Rect().Height()));
    AknLayoutUtils::LayoutVerticalScrollBar(iSBFrame, scrollBarParent, scrollLineLayout);
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::MakeViewClassInstanceL
// Creating a view
// -----------------------------------------------------------------------------
CListBoxView* CPeninputJapanesePredictiveListBox::MakeViewClassInstanceL()
    {
    return new(ELeave) CPeninputJapanesePredictiveListBoxView;
    }

void CPeninputJapanesePredictiveListBox::HandleScrollEventL(CEikScrollBar* aScrollBar,TEikScrollEvent aEventType)
    {
    TInt oldThumbPos = (aEventType & KEikScrollEventBarMask) ? iView->HScrollOffset() : iView->TopItemIndex();
    TInt newThumbPos = oldThumbPos;
    TInt maxThumbPos = static_cast<CTextListBoxModel*>(Model())->NumberOfItems() - iLineCount;

    switch (aEventType & KEikScrollEventBarMask)
        {
    case KEikScrollEventFromVBar:
        switch (aEventType)
            {
        case EEikScrollUp:
            if (newThumbPos > 0)
                {
                -- newThumbPos;
                }
            if ( oldThumbPos == 0  && (iListBoxFlags & ELoopScrolling))
                {
                newThumbPos = maxThumbPos;              
                }
            break;
        case EEikScrollDown:
            if (newThumbPos < maxThumbPos)
                {
                ++ newThumbPos;
                }
            if ( oldThumbPos == maxThumbPos && (iListBoxFlags & ELoopScrolling) )
                {
                newThumbPos = 0;                
                }
            break;

        default:
            break;
            }

        if ( newThumbPos != oldThumbPos )
            {
            iView->VScrollTo(newThumbPos);
            aScrollBar->SetModelThumbPosition(iView->TopItemIndex());
            UpdateScrollBarThumbs();
            }
        break;
    default:
        break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::CreateItemDrawerL
// Creating a ItemDrawer
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::CreateItemDrawerL()
    {
    CPeninputJapanesePredictiveCellListBoxData*
        cellData = CPeninputJapanesePredictiveCellListBoxData::NewL();
    CleanupStack::PushL(cellData);

    iItemDrawer =
        new(ELeave) CPeninputJapanesePredictiveListBoxItemDrawer(
                        Model(), NULL, cellData);

    CleanupStack::Pop(cellData);    // cellData
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBox::UpdateCurrentItem
// Update the current index
// Notes: The argument of aItemIndex means the current line. (not current candidate index)
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBox::UpdateCurrentItem(TInt aItemIndex) const
    {
    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(Model());
    CPeninputJapanesePredictiveListBoxView* view =
        static_cast<CPeninputJapanesePredictiveListBoxView*>(View());

    if (!model->LineInfoArray())
        {
        return;
        }
    TPredictivePopupElement element;
    TInt curCandi = view->CurrentCandidateNo();
    TInt oldCandi = view->OldCandidateNo();
    TInt maxLine = model->LineInfoArray()->Count();

    TInt oldCurrentItemIndex = view->CurrentItemIndex();
    if (oldCurrentItemIndex != aItemIndex)
        {
        // Set the current line
        view->SetCurrentItemIndex(aItemIndex);

        if ( IsReadyToDraw() )
            {
            // Draw the old line
            view->DrawItem(oldCurrentItemIndex);
            }

        if (oldCurrentItemIndex+1 < maxLine
         && oldCurrentItemIndex+1 != aItemIndex)
            {
            if (model->SearchElement(oldCurrentItemIndex+1,
                                     oldCandi, element))
                {
                view->DrawItem(oldCurrentItemIndex+1);
                }
            }
        // Set the top position
        if (!(view->ItemIsVisible(aItemIndex)))
            {
            SetTopItemIndex(view->CalcNewTopItemIndexSoItemIsVisible(aItemIndex));
            DrawNow();
            }
        }
    // Draw the current line
    DrawItem(aItemIndex);
    // Check next line including the same candidate
    TInt nextLine = aItemIndex+1;
    if (nextLine < maxLine
     && model->SearchElement(nextLine, curCandi, element))
        {
        if (!(view->ItemIsVisible(nextLine)))
            {
            SetTopItemIndex(view->CalcNewTopItemIndexSoItemIsVisible(nextLine));
            DrawNow();
            }
        DrawItem(nextLine);
        }
    UpdateScrollBarThumbs();
    }

// End of File
