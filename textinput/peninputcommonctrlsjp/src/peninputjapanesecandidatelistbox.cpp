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
* Description:  Candidate List ListBox
*
*/

#include <aknlayoutscalable_avkon.cdl.h>
#include "peninputeventjp.h"
#include "peninputjapanesecandidatelistbox.h"
#include "peninputjapanesecandidatecelllistboxdata.h"

// const TInt KLayoutTempValue_2 = 2;   // temporary


// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateListbox::SetMaximumColumns
// Set maximum columns in listbox
// -----------------------------------------------------------------------------
void CPeninputJapaneseCandidateListBox::SetMaximumColumns(
        const TInt aMaximumColumns)
    {
    iMaximumColumns = aMaximumColumns;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateListBox::SetMaximumRows
// Set maximum visible rows in listbox
// -----------------------------------------------------------------------------
void CPeninputJapaneseCandidateListBox::SetMaximumRows(
        const TInt aMaximumRows)
    {
    iMaximumRows = aMaximumRows;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateListBox::HandlePointerEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateListBox::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    const TInt old = CurrentItemIndex();
    CAknSinglePopupMenuStyleListBox::HandlePointerEventL(aPointerEvent);
    switch (aPointerEvent.iType)
        {
        case TPointerEvent::EButton1Up:
            break;
        case TPointerEvent::EButton1Down:
            {
            if (CurrentItemIndex() != old)
                {
                ReportListBoxEventL(MEikListBoxObserver::EEventItemDraggingActioned);// need draw.(not leave)
                }
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateListbox::MinimumSize
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// temporary
TSize CPeninputJapaneseCandidateListBox::MinimumSize()
    {
    TAknTextLineLayout textLineLayout =
        AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();
    const CFont* font = AknLayoutUtils::FontFromId(textLineLayout.iFont, NULL);
    TInt width = CEikListBox::CalcWidthBasedOnRequiredItemWidth(iMaximumColumns * font->AscentInPixels()+KPENINPUT_LAYOUT_VALUE_6);
    return TSize(width, CEikListBox::CalcHeightBasedOnNumOfItems(iMaximumRows));
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateListbox::SizeChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CPeninputJapaneseCandidateListBox::SizeChanged()
    {
    CEikFormattedCellListBox& listBox = *this;
    CFormattedCellListBoxItemDrawer* itemDrawer = listBox.ItemDrawer();

    TAknWindowLineLayout textWindowLayout =
        AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine();

    TAknTextLineLayout textLineLayout =
        AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();
    const CFont* font = AknLayoutUtils::FontFromId(textLineLayout.iFont, NULL);
   textWindowLayout.iH = font->AscentInPixels()+KPENINPUT_LAYOUT_VALUE_2;  // control itemheight


    AknListBoxLayouts::SetupStandardListBox(listBox);  // SetBorder(TGulBorder::ENone)
    AknListBoxLayouts::SetupStandardFormListbox(itemDrawer); // Set Standard Colors

    // Scroll bar : before AknListBoxLayouts::SetupListboxPos()
    TAknWindowLineLayout scrollLineLayout = AknLayoutScalable_Avkon::scroll_pane_cp30().LayoutLine();
    scrollLineLayout.il = ELayoutEmpty;
    if (Model()->NumberOfItems() <= iMaximumRows)
        {
        scrollLineLayout.iW = 0;
        }
    TRect scrollBarParent = TRect(TPoint(listBox.Rect().iTl),
                                  TSize(listBox.Rect().Width() + scrollLineLayout.iW + scrollLineLayout.ir,
                                        listBox.Rect().Height()));
    AknLayoutUtils::LayoutVerticalScrollBar(iSBFrame, scrollBarParent, scrollLineLayout);

    AknListBoxLayouts::SetupListboxPos(listBox, textWindowLayout);  // ItemDrawer()->SetItemCellSize()
                                                                    // Draw Scrollbar

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

    // setting text box size in listbox
    TInt baseline = itemSize.iHeight/2 + font->AscentInPixels() / 2;
    textLineLayout.iB = ELayoutP - baseline;  // control itemheight
    AknListBoxLayouts::SetupFormAntiFlickerTextCell(
            listBox, itemDrawer, 0, textLineLayout,
            TPoint(0, 0),
            TPoint(itemSize.iWidth, itemSize.iHeight)); // CheckColor.

    CEikFormattedCellListBox::SizeChanged();
    UpdateScrollBarThumbs();
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateListBox::CreateItemDrawerL
// Create ItemDrawer
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateListBox::CreateItemDrawerL()
    {
    CPeninputJapaneseCandidateCellListBoxData* cellData =
        CPeninputJapaneseCandidateCellListBoxData::NewL();
    CleanupStack::PushL(cellData);

    iItemDrawer =
        new(ELeave) CFormattedCellListBoxItemDrawer(Model(), NULL, cellData);
    CleanupStack::Pop(cellData);
    }

void CPeninputJapaneseCandidateListBox::HandleScrollEventL(CEikScrollBar* aScrollBar,TEikScrollEvent aEventType)
    {
    TInt oldThumbPos = (aEventType & KEikScrollEventBarMask) ? iView->HScrollOffset() : iView->TopItemIndex();
    TInt newThumbPos = aScrollBar->ThumbPosition();
    TInt maxThumbPos = static_cast<CTextListBoxModel*>(Model())->NumberOfItems() - iMaximumRows;
    TBool update = ETrue; // for the case EEikScrollThumbRelease so that after it there is now update.
    TInt newThumbPosBeforeCorrecting = newThumbPos;

    switch (aEventType & KEikScrollEventBarMask)
        {
    case KEikScrollEventFromVBar:
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
        break;
    default:
        break;
        }
    }
// End Of File
