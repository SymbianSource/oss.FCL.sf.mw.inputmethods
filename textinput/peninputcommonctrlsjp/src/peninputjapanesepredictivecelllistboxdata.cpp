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
* Description:  Predictive Candidate list cellist data box
*
*/


// INCLUDE FILES
#include <aknPopup.h>
#include <AknsDrawUtils.h>
#include <AknLayout.lag>
#include <aknlayoutscalable_avkon.cdl.h>
#include <skinlayout.cdl.h>

#include "peninputjapanesepredictivewnd.h"
#include "peninputjapanesepredictivecelllistboxdata.h"
using namespace SkinLayout;

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveCellListBoxData::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
CPeninputJapanesePredictiveCellListBoxData*
    CPeninputJapanesePredictiveCellListBoxData::NewL()
    {
    CPeninputJapanesePredictiveCellListBoxData* self =
        new(ELeave) CPeninputJapanesePredictiveCellListBoxData;
    self->ConstructLD();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveCellListBoxData::SetHighlightArea
// Setting rang of x-point to highlight
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveCellListBoxData::SetHighlightArea(
        TInt aHighlightStartXPos, TInt aHighlightEndXPos)
    {
    iHighlightStartXPos = aHighlightStartXPos;
    iHighlightEndXPos   = aHighlightEndXPos;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveCellListBoxData::Draw
// Drawing a item in listbox
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveCellListBoxData::Draw(
        TListItemProperties aProperties,
        CWindowGc& aGc,
        const TDesC* aText,
        const TRect& aRect,
        TBool aHighlight,
        const TColors& aColors) const
    {
    const TRect& aItemRect = aRect;

    aHighlight = (iHighlightStartXPos != 0 || iHighlightEndXPos != 0);
    TRect highlightItemRect = aRect;

    TAknTextLineLayout textLineLayout =
        AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();  // color
    TAknWindowLineLayout laf =
        AKN_LAYOUT_WINDOW_Predictive_candidate_selection_popup_window_graphics_Line_4(aItemRect);

    highlightItemRect.iBr.iX = highlightItemRect.iTl.iX + iHighlightEndXPos;
    highlightItemRect.iTl.iX = highlightItemRect.iTl.iX + iHighlightStartXPos;
    DrawDefaultHighlight(aGc, highlightItemRect, aHighlight);

    // Draw the actual items.
    TColors col = aColors;
    col.iBack = AKN_LAF_COLOR_STATIC(laf.iC);
    // drawing current line
    col.iHighlightedText = AKN_LAF_COLOR_STATIC(textLineLayout.iC);
    DrawFormatted(aProperties, aGc, aText, aItemRect, EFalse, col);
    iWnd->ReDraw();

    aGc.Reset();
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveCellListBoxData::DrawDefaultHighlight
// Highlighting a item
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveCellListBoxData::DrawDefaultHighlight(
        CWindowGc& aGc,
        const TRect& aItemRect,
        TBool aHighlight) const
    {
    if (aHighlight)
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TBool highlightDrawn = EFalse;
        MAknsControlContext* cc = AknsDrawUtils::ControlContext(Control());

        if (cc)
            {
            TAknWindowLineLayout layCorner = List_highlight_skin_placing__general__Line_2();
            TAknLayoutRect topLeft;
            topLeft.LayoutRect(
                aItemRect,
                ELayoutEmpty,
                0, 0, ELayoutEmpty, ELayoutEmpty, layCorner.iW, layCorner.iH);

            TAknLayoutRect bottomRight;
            bottomRight.LayoutRect(
                aItemRect,
                ELayoutEmpty,
                ELayoutEmpty, ELayoutEmpty, -layCorner.iW, 0, layCorner.iW, layCorner.iH);

            TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
            TRect innerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
            aGc.SetPenStyle(CGraphicsContext::ENullPen);

            const TAknsItemID* skinHighlightFrameId = &KAknsIIDQsnFrList;
            const TAknsItemID* skinHighlightFrameCenterId = &KAknsIIDQsnFrListCenter;
            highlightDrawn =
                AknsDrawUtils::DrawFrame(
                    skin, aGc, outerRect, innerRect,
                    *skinHighlightFrameId, *skinHighlightFrameCenterId);
            }

        if (!highlightDrawn)
            {
            TAknWindowLineLayout hl = AKN_LAYOUT_WINDOW_Predictive_candidate_selection_highlight_Line_1;
            hl.iW = ELayoutEmpty;
            hl.il += 1;
            hl.it += 1;
            hl.ir = -1;
            TAknLayoutRect highlight;
            highlight.LayoutRect(aItemRect, hl);
            highlight.DrawRect(aGc);
            }
        }
    }


// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveCellListBoxData::SetWndt
// Set Container (Popup Wnd)
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveCellListBoxData::SetWnd(CPeninputJapanesePredictiveWnd* aWnd)
    {
    iWnd = aWnd;
    }
// End of File
