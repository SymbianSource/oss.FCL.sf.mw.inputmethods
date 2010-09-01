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
* Description:  Predictive Candidate list drawer
*
*/


// INCLUDE FILES
#include "peninputjapanesepredictivecelllistboxdata.h"
#include "peninputjapanesepredictiveListboxitemdrawer.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxItemDrawer::
//                              CPeninputJapanesePredictiveListBoxItemDrawer
// Constractor
// -----------------------------------------------------------------------------
CPeninputJapanesePredictiveListBoxItemDrawer::CPeninputJapanesePredictiveListBoxItemDrawer(
        MTextListBoxModel* aTextListBoxModel,
        const CFont* aFont,
        CFormattedCellListBoxData* aFormattedCellData)
            : CFormattedCellListBoxItemDrawer(aTextListBoxModel,
                                              aFont,
                                              aFormattedCellData)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxItemDrawer::DrawItem
// Drawing a item
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBoxItemDrawer::DrawItem(
        TInt aItemIndex,
        const TPoint& aItemRectPos,
        TBool aItemIsSelected,
        TBool aItemIsCurrent,
        TBool aViewIsEmphasized,
        TBool aViewIsDimmed,
        TInt aHighlightStartXPos,
        TInt aHighlightEndXPos) const
    {
    // [HWR] at DrawNow() for pictograph blinking, gc is Activated for MaskBitmap();
    if (iBitmapDevice)
       {
        CFbsBitGc* gc = reinterpret_cast<CFbsBitGc*>(Gc());
        gc->Activate(iBitmapDevice);
        }

    TRect actualItemRect(aItemRectPos, iItemCellSize);

    CPeninputJapanesePredictiveCellListBoxData* data =
        static_cast<CPeninputJapanesePredictiveCellListBoxData*>(iData);
    data->SetHighlightArea(aHighlightStartXPos, aHighlightEndXPos);

    DrawActualItem(
        aItemIndex,
        actualItemRect, 
        aItemIsCurrent, aViewIsEmphasized, aViewIsDimmed, aItemIsSelected);
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxItemDrawer::SetBitmapDevice
// Set Bitmap device for sprite
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBoxItemDrawer::SetBitmapDevice(CFbsBitmapDevice* aBitmapDevice)
    {
    iBitmapDevice = aBitmapDevice;
    }

// End of File
