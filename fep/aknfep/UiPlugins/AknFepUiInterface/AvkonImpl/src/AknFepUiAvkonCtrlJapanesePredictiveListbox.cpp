/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Predictive candidate list
*
*/












// INCLUDE FILES
#include <eikdef.h>
#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <aknPopup.h>
#include <aknlists.h>
#include <AknLayout.lag>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknFontId.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListboxView.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListboxItemDrawer.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveCellListBoxData.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListBoxModel.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListbox.h"
#include "AknFepUIAvkonCtrlJapanesePredictivePane.h"

// CONSTANTS
const TInt KListboxMarge = 4;
const TInt KScrollRightMarge = 5;
const TInt KListBoxVisibleRow = 3;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListbox::ConstructL(
        const CCoeControl* aParent,
        TInt aFlags)
    {
    CAknFilteredTextListBoxModel* model =
            new(ELeave) CAknFepUICtrlJapanesePredictiveListBoxModel;
    iModel=model;
    model->ConstructL();
    CreateItemDrawerL();

    EnableExtendedDrawingL();

    iItemDrawer->SetDrawMark(EFalse);
    CEikListBox::ConstructL(aParent,aFlags);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::SetCurrentCandidateNo
// Selecting a candidate number appointed with an argument
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListbox::SetCurrentCandidateNo(
        TInt aCandidateNo, TBool aDraw)
    {
    CAknFepUICtrlJapanesePredictiveListboxView* view =
        static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());
    TInt itemIndex = view->SetCurrentCandidateNo(aCandidateNo);

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
// CAknFepUICtrlJapanesePredictiveListbox::CurrentCandidateNo
// Returning a selected number
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListbox::CurrentCandidateNo()
    {
    CAknFepUICtrlJapanesePredictiveListboxView* view =
        static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());
    return view->CurrentCandidateNo();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::MovePreviousItem
// selecting a previous candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListbox::MovePreviousItem()
    {
    CAknFepUICtrlJapanesePredictiveListboxView* view =
        static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());

    TInt candidateNo = view->PreviousItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::MoveNextItem
// selecting a next candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListbox::MoveNextItem()
    {
    CAknFepUICtrlJapanesePredictiveListboxView* view =
        static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());

    TInt candidateNo = view->NextItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::MovePreviousLineItem
// selecting a previous line candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListbox::MovePreviousLineItem()
    {
    CAknFepUICtrlJapanesePredictiveListboxView* view =
        static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());
    TInt candidateNo = view->PreviousLineItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::MoveNextLineItem
// selecting a next line candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListbox::MoveNextLineItem()
    {
    CAknFepUICtrlJapanesePredictiveListboxView* view =
        static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());
    TInt candidateNo = view->NextLineItem();
    SetCurrentCandidateNo(candidateNo, ETrue);

    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::SizeChanged
// This function is called in case changing size
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListbox::SizeChanged()
    {
    CEikFormattedCellListBox& listBox = *this;
    CFormattedCellListBoxItemDrawer* itemDrawer = listBox.ItemDrawer();

    TAknWindowLineLayout textWindowLayout =
        AKN_LAYOUT_WINDOW_popup_fep_japan_predictive_window(0,0);
    // setting textbox size in listbox
    TAknTextLineLayout textLineLayout =
        AKN_LAYOUT_TEXT_Predictive_candidate_selection_list_texts_Line_1(0,0);

    TAknTextComponentLayout scal = AknLayoutScalable_Avkon::popup_fep_japan_predictive_window_t1(0);
    TAknWindowLineLayout scrollLineLayout = AknLayoutScalable_Avkon::scroll_pane_cp29().LayoutLine();

    TAknLayoutText text;
    text.LayoutText(textWindowLayout.Rect(), textLineLayout);
    textWindowLayout.iW = text.TextRect().Width();
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(Model());
    TInt maxRows = model->LineInfoArray()->Count();
    TInt maxVisibleRows = Min(KJapPredictiveVisibleListRowsMax, maxRows);
    maxVisibleRows = Max(KJapPredictiveVisibleListRowsMin, maxVisibleRows);
    textWindowLayout.iH = listBox.Size().iHeight / maxVisibleRows;

    AknListBoxLayouts::SetupStandardListBox(listBox);
    AknListBoxLayouts::SetupStandardFormListbox(itemDrawer);
    AknListBoxLayouts::SetupListboxPos(listBox, textWindowLayout);
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

    textLineLayout.il -= KListboxMarge;
    textLineLayout.ir = 0;
    // Baseline must be set to textLineLayout.iB.
    const CFont* font = AknLayoutUtils::FontFromId(textLineLayout.iFont);
    TInt baseLineOffset = (textWindowLayout.iH - font->HeightInPixels())/2 + font->AscentInPixels();

    if( TAknFontId::IsEncodedFont(textLineLayout.FontId()) )
        {
        textLineLayout.iB = textWindowLayout.iH - font->DescentInPixels() -1 - baseLineOffset;
        }
    else
        {
        textLineLayout.iB = baseLineOffset;
        }
    TSize itemSize = View()->ItemSize();

    AknListBoxLayouts::SetupFormAntiFlickerTextCell(listBox, itemDrawer, 0,
                            textLineLayout,
                            TPoint(0, 0),
                            TPoint(itemSize.iWidth, itemSize.iHeight));

    CEikFormattedCellListBox::SizeChanged();

    // Scroll bar
    scrollLineLayout.il = ELayoutEmpty;
    scrollLineLayout.ir = KScrollRightMarge;
    scrollLineLayout.it = 0;
    scrollLineLayout.ib = 0;
    if (maxRows <= KJapPredictiveVisibleListRowsMax)
        {
        scrollLineLayout.iW = 0;
        }
    TAknWindowLineLayout laf =
        AKN_LAYOUT_WINDOW_popup_fep_japan_predictive_window(0,0);
    TRect scrollBarParent = TRect(TPoint(0, listBox.Rect().iTl.iY),
                                  TPoint(laf.iW, listBox.Rect().iBr.iY));
    AknLayoutUtils::LayoutVerticalScrollBar(iSBFrame, scrollBarParent, scrollLineLayout);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::MakeViewClassInstanceL
// Creating a view
// -----------------------------------------------------------------------------
//
CListBoxView* CAknFepUICtrlJapanesePredictiveListbox::MakeViewClassInstanceL()
    {
    return new(ELeave) CAknFepUICtrlJapanesePredictiveListboxView;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::CreateItemDrawerL
// Creating a ItemDrawer
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListbox::CreateItemDrawerL()
    {
    CAknFepUICtrlJapanesePredictiveCellListBoxData*
        cellData = CAknFepUICtrlJapanesePredictiveCellListBoxData::NewL();
    CleanupStack::PushL(cellData);

    // getting font object
    TAknTextLineLayout textLineLayout =
        AKN_LAYOUT_TEXT_Predictive_candidate_selection_list_texts_Line_1(
            0, 0);
    const CFont* font = AknLayoutUtils::FontFromId(textLineLayout.iFont);

    iItemDrawer =
        new(ELeave) CAknFepUICtrlJapanesePredictiveListboxItemDrawer(
                        Model(), font, cellData);

    CleanupStack::Pop(cellData);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::UpdateCurrentItem
// Update the current index
// Notes: The argument of aItemIndex means the current line. (not current candidate index)
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListbox::UpdateCurrentItem(TInt aItemIndex) const
    {
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(Model());
    CAknFepUICtrlJapanesePredictiveListboxView* view =
        static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());

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

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::HandlePointerEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAknFepUICtrlJapanesePredictiveListbox::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    const TInt old = CurrentItemIndex();
    CAknSinglePopupMenuStyleListBox::HandlePointerEventL(aPointerEvent);
    switch (aPointerEvent.iType)
        {
        case TPointerEvent::EButton1Up:
            break;
        case TPointerEvent::EButton1Down:
            {
            CAknFepUICtrlJapanesePredictiveListboxView* view =
                static_cast<CAknFepUICtrlJapanesePredictiveListboxView*>(View());
            TInt oldCandidateNo = view->CurrentCandidateNo();
            TPoint pos = aPointerEvent.iPosition - Rect().iTl;
            TInt itemIndex = view->SetCurrentCandidateNo(CurrentItemIndex(), pos);
            SetCurrentItemIndex(itemIndex);
            TInt newCandidateNo = view->CurrentCandidateNo();

            if (oldCandidateNo != newCandidateNo)
                {
                ReportListBoxEventL(MEikListBoxObserver::EEventItemDraggingActioned);// need draw.
                }
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListbox::HandleScrollEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAknFepUICtrlJapanesePredictiveListbox::HandleScrollEventL(
                                                    CEikScrollBar* aScrollBar,
                                                    TEikScrollEvent aEventType)
    {
    TInt oldThumbPos = (aEventType & KEikScrollEventBarMask) ?
                                iView->HScrollOffset() : iView->TopItemIndex();
    TInt newThumbPos = aScrollBar->ThumbPosition();
    TInt maxThumbPos = static_cast<CTextListBoxModel*>(Model())->NumberOfItems()
                                - KListBoxVisibleRow;
    TBool update = ETrue;
    TInt newThumbPosBeforeCorrecting = newThumbPos;

    if ((aEventType & KEikScrollEventBarMask) == KEikScrollEventFromVBar)
        {
        switch (aEventType)
            {
            case EEikScrollUp:
                if ( oldThumbPos == 0  && (iListBoxFlags & ELoopScrolling))
                    {
                    newThumbPos = maxThumbPos;
                    }
                break;
            case EEikScrollDown:
                if ( oldThumbPos == maxThumbPos && (iListBoxFlags & ELoopScrolling) )
                    {
                    newThumbPos = 0;
                    }
                break;
            case EEikScrollPageUp:
            case EEikScrollPageDown:
            case EEikScrollThumbDragVert:
                break;
            case EEikScrollThumbReleaseVert:
                // in the case of drag events, the scrollbar automatically updates its thumb pos...
                if(AknLayoutUtils::PenEnabled())
                    {
                    update = EFalse;
                    }
                break;

            default:
                break;
            }
        if ( (!AknLayoutUtils::PenEnabled()) || update )
            {
            iView->VScrollTo(newThumbPos);
            if (aEventType != EEikScrollThumbDragVert)
                {
                aScrollBar->SetModelThumbPosition(iView->TopItemIndex());
                }
            }

        // if event has changed thumb position, then update scroll bar
        if ( AknLayoutUtils::PenEnabled() && newThumbPos != newThumbPosBeforeCorrecting )
            {
            UpdateScrollBarThumbs();
            }
        }
    }

// End of File
