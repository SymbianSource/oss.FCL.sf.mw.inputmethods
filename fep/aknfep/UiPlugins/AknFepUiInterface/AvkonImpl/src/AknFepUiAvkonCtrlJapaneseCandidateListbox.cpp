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
* Description:            Candidate list
*
*/












// INCLUDE FILES
#include <eikdef.h>

#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <aknPopup.h>
#include <aknlists.h>
#include <AknsDrawUtils.h>
#include <AknLayout.lag>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknLayout2ScalableDef.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapaneseCandidateListbox.h" 
#include "AknFepUIAvkonCtrlJapaneseCandidateListboxView.h" 
#include "AknFepUIAvkonCtrlJapaneseCandidateCellListBoxData.h" 

// CONSTANTS
const TInt KLAFBaseListColumns = 3;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListbox::SetMaximumColumns
// Set maximum columns in listbox
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateListbox::SetMaximumColumns(
        const TInt aMaximumColumns)
    {
    iMaximumColumns = aMaximumColumns;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListbox::SizeChanged
// This function is called when changing window size.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateListbox::SizeChanged() 
    {
    CEikFormattedCellListBox& listBox = *this;
    CFormattedCellListBoxItemDrawer* itemDrawer = listBox.ItemDrawer();

    TAknWindowLineLayout textWindowLayout = 
        AKN_LAYOUT_WINDOW_list_single_popup_jap_candidate_pane(
            0, iMaximumColumns - KLAFBaseListColumns);
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

    // setting text box size in listbox
    TAknTextLineLayout textLineLayout =
        AKN_LAYOUT_TEXT_List_pane_text__single_japan_fep__Line_1(
            iMaximumColumns - KLAFBaseListColumns);
    TSize itemSize = View()->ItemSize();

    AknListBoxLayouts::SetupFormAntiFlickerTextCell(
            listBox, itemDrawer, 0, textLineLayout,
            TPoint(0, 0),
            TPoint(itemSize.iWidth, itemSize.iHeight)); // CheckColor.

    CEikFormattedCellListBox::SizeChanged();

    // Scroll bar
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
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListbox::MakeViewClassInstanceL
// Create view class
// -----------------------------------------------------------------------------
//
CListBoxView* CAknFepUICtrlJapaneseCandidateListbox::MakeViewClassInstanceL()
    {
    return new(ELeave) CAknFepUICtrlJapaneseCandidateListboxView;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListbox::CreateItemDrawerL
// Create ItemDrawer
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateListbox::CreateItemDrawerL()
    {
    CAknFepUICtrlJapaneseCandidateCellListBoxData* cellData =
        CAknFepUICtrlJapaneseCandidateCellListBoxData::NewL();
    CleanupStack::PushL( cellData );

    // getting font object
    TAknTextLineLayout textLineLayout =
        AKN_LAYOUT_TEXT_List_pane_text__single_japan_fep__Line_1(0);
    const CFont* font = AknLayoutUtils::FontFromId(textLineLayout.iFont);

    iItemDrawer =
        new(ELeave) CFormattedCellListBoxItemDrawer(Model(), font, cellData);
    CleanupStack::Pop(cellData);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListbox::SetMaximumRows
// Set maximum visible rows in listbox
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateListbox::SetMaximumRows(
        const TInt aMaximumRows)
    {
    iMaximumRows = aMaximumRows;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListbox::HandlePointerEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateListbox::HandlePointerEventL(const TPointerEvent& aPointerEvent)
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
                ReportListBoxEventL(MEikListBoxObserver::EEventItemDraggingActioned);// need draw.
                }
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListbox::HandleScrollEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAknFepUICtrlJapaneseCandidateListbox::HandleScrollEventL(
                                                    CEikScrollBar* aScrollBar,
                                                    TEikScrollEvent aEventType)
    {
    TInt oldThumbPos = (aEventType & KEikScrollEventBarMask) ?
                                iView->HScrollOffset() : iView->TopItemIndex();
    TInt newThumbPos = aScrollBar->ThumbPosition();
    TInt maxThumbPos = static_cast<CTextListBoxModel*>(Model())->NumberOfItems()
                                - MaximumRows();
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
