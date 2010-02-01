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
#include <aknViewAppUi.h>
#include <avkon.hrh>

#include <aknPopup.h>
#include <AknsDrawUtils.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapaneseCandidateCellListBoxData.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateCellListBoxData::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapaneseCandidateCellListBoxData*
    CAknFepUICtrlJapaneseCandidateCellListBoxData::NewL()
    {
    CAknFepUICtrlJapaneseCandidateCellListBoxData* self =
            new(ELeave) CAknFepUICtrlJapaneseCandidateCellListBoxData;
    self->ConstructLD();
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateCellListBoxData::Draw
// Draw items
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateCellListBoxData::Draw(
        TListItemProperties aProperties,
        CWindowGc& aGc,
        const TDesC* aText,
        const TRect& aRect,
        TBool aHighlight, 
        const TColors& aColors) const
    {
    const TRect &aItemRect = aRect;

    DrawDefaultHighlight(aGc, aItemRect, aHighlight);

    // Draw the actual items.
    DrawFormatted(aProperties, aGc, aText, aItemRect, aHighlight, aColors);

    aGc.Reset();

    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateCellListBoxData::DrawDefaultHighlight
// Draw highlight item
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateCellListBoxData::DrawDefaultHighlight(
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
            TAknLayoutRect topLeft;
            topLeft.LayoutRect(aItemRect,
                               ELayoutEmpty,
                               1, 1, ELayoutEmpty, ELayoutEmpty, 5, 5);

            TAknLayoutRect bottomRight;
            bottomRight.LayoutRect(aItemRect,
                                   ELayoutEmpty,
                                   ELayoutEmpty, ELayoutEmpty, 1, 0, 5,5);

            TRect outerRect =
                        TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
            TRect innerRect =
                        TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
            aGc.SetPenStyle(CGraphicsContext::ENullPen);

            const TAknsItemID* skinHighlightFrameId =
                                                &KAknsIIDQsnFrList;
            const TAknsItemID* skinHighlightFrameCenterId =
                                                &KAknsIIDQsnFrListCenter;
            highlightDrawn = AknsDrawUtils::DrawFrame(
                                                skin,
                                                aGc,
                                                outerRect, innerRect,
                                                *skinHighlightFrameId,
                                                *skinHighlightFrameCenterId);
            }
        
        if (!highlightDrawn)
            {
            TAknLayoutRect highlightshadow;
            TAknLayoutRect highlight;
            highlightshadow.LayoutRect(
                    aItemRect,
                    AKN_LAYOUT_WINDOW_List_pane_highlight_graphics__various__Line_1(aItemRect));
            highlight.LayoutRect(
                    aItemRect,
                    AKN_LAYOUT_WINDOW_List_pane_highlight_graphics__various__Line_2(aItemRect));

            highlightshadow.DrawRect(aGc);
            highlight.DrawRect(aGc);
            }
        }
    }

// End of File
