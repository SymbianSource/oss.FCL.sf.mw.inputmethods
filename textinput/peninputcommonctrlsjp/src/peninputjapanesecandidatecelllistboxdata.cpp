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
* Description:  Candidate list CellListBoxData
*
*/

// INCLUDE FILES
#include <AknBidiTextUtils.h>  // KAknBidiExtraSpacePerLine
#include <AknUtils.h>  // KMaxColumnDataLength others

#include "peninputjapanesecandidatecelllistboxdata.h"

const TInt KLayoutTempValue_5 = 5;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateCellListBoxData::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPeninputJapaneseCandidateCellListBoxData*
    CPeninputJapaneseCandidateCellListBoxData::NewL()
    {
    CPeninputJapaneseCandidateCellListBoxData* self =
            new(ELeave) CPeninputJapaneseCandidateCellListBoxData;
    self->ConstructLD();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateCellListBoxData::Draw
// Draw items
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateCellListBoxData::Draw(
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
    DrawFormattedCandidate(aProperties, aGc, aText, aItemRect, aHighlight, aColors);

    aGc.Reset();

    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateCellListBoxData::DrawDefaultHighlight
// Draw highlight item
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateCellListBoxData::DrawDefaultHighlight(
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
            TAknLayoutRect topLeft;
            topLeft.LayoutRect(aItemRect,
                               ELayoutEmpty,
                               1, 1, ELayoutEmpty, ELayoutEmpty, KLayoutTempValue_5, KLayoutTempValue_5);

            TAknLayoutRect bottomRight;
            bottomRight.LayoutRect(aItemRect,
                                   ELayoutEmpty,
                                   ELayoutEmpty, ELayoutEmpty, 1, 0, KLayoutTempValue_5,KLayoutTempValue_5);

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

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateCellListBoxData::DrawFormattedCandidate
// Draw one item
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateCellListBoxData::DrawFormattedCandidate( TListItemProperties&/* aProperties*/,
                                          CWindowGc& aGc,
                                          const TDesC* aText,
                                          const TRect& aItemRect,
                                          TBool /*aHighlight*/,
                                          const TColors& aColors ) const
    {
    TRect textRect(aItemRect);
    textRect.iTl.iX += 3;
    
    const CFont* font=SubCellFont(0);
    if (font==NULL)
        {
        font=CEikonEnv::Static()->NormalFont();
        }
    
    TInt extraVerticalSpace=(textRect.Height()-font->HeightInPixels());
    TInt baseLineOffset=extraVerticalSpace/2+font->AscentInPixels();
    TPtrC text;
    TextUtils::ColumnText(text,0, aText);

    TBuf<KMaxColumnDataLength + KAknBidiExtraSpacePerLine> clipbuf = 
        text.Left(KMaxColumnDataLength);
                    TInt maxClipWidth = textRect.Size().iWidth;
                    
                    TBool clipped = AknBidiTextUtils::ConvertToVisualAndClip(
                        text.Left(KMaxColumnDataLength), 
                        clipbuf,
                        *font,
                        textRect.Size().iWidth, 
                        maxClipWidth );
    aGc.UseFont(font);
    aGc.SetBrushStyle( CGraphicsContext::ENullBrush );
    aGc.SetPenColor(aColors.iText);
    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.DrawText(clipbuf, textRect, baseLineOffset, CGraphicsContext::ELeft, 0);
    aGc.DiscardFont();
    return;
    }

// End of File
