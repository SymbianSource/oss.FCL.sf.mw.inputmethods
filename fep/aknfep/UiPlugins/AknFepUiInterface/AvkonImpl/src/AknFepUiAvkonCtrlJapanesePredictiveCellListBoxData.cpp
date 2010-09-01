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
#include <AknsDrawUtils.h>
#include <AknLayout.lag>
#include <aknlayoutscalable_avkon.cdl.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveCellListBoxData.h"

#include <skinlayout.cdl.h>
using namespace SkinLayout;

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveCellListBoxData::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapanesePredictiveCellListBoxData*
    CAknFepUICtrlJapanesePredictiveCellListBoxData::NewL()
    {
    CAknFepUICtrlJapanesePredictiveCellListBoxData* self =
        new(ELeave) CAknFepUICtrlJapanesePredictiveCellListBoxData;
    self->ConstructLD();
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveCellListBoxData::SetHighlightArea
// Setting rang of x-point to highlight
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveCellListBoxData::SetHighlightArea(
        TInt aHighlightStartXPos, TInt aHighlightEndXPos)
    {
    iHighlightStartXPos = aHighlightStartXPos;
    iHighlightEndXPos   = aHighlightEndXPos;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveCellListBoxData::Draw
// Drawing a item in listbox
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveCellListBoxData::Draw(
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
        AKN_LAYOUT_TEXT_Predictive_candidate_selection_list_texts_Line_1(0,0);
    TAknTextLineLayout highlighttextLineLayout =
        AKN_LAYOUT_TEXT_Predictive_candidate_selection_list_texts_Line_1(1,0);
    TAknWindowLineLayout laf =
        AKN_LAYOUT_WINDOW_Predictive_candidate_selection_popup_window_graphics_Line_4(aItemRect);
    TAknLayoutText textlay;
    textlay.LayoutText(aItemRect, textLineLayout);
    TAknTextComponentLayout scal = AknLayoutScalable_Avkon::popup_fep_japan_predictive_window_t1(0);

    highlightItemRect.iBr.iX = highlightItemRect.iTl.iX + iHighlightEndXPos;
    highlightItemRect.iTl.iX = highlightItemRect.iTl.iX + iHighlightStartXPos;
    DrawDefaultHighlight(aGc, highlightItemRect, aHighlight);

    // Draw the actual items.
    DrawFormatted(aProperties, aGc, aText, aItemRect, EFalse, aColors);

    aGc.Reset();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveCellListBoxData::DrawDefaultHighlight
// Highlighting a item
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveCellListBoxData::DrawDefaultHighlight(
        CWindowGc& aGc,
        const TRect& aItemRect,
        TBool aHighlight) const
    {
    if (aHighlight)
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TBool highlightDrawn = EFalse;
        MAknsControlContext* cc = AknsDrawUtils::ControlContext(Control());
        if (!cc)
            {
            cc = SkinBackgroundContext();
            }

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

// End of File
