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

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveCellListBoxData.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListboxItemDrawer.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxItemDrawer::
//                              CAknFepUICtrlJapanesePredictiveListboxItemDrawer
// Constractor
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapanesePredictiveListboxItemDrawer::CAknFepUICtrlJapanesePredictiveListboxItemDrawer(
        MTextListBoxModel* aTextListBoxModel,
        const CFont* aFont,
        CFormattedCellListBoxData* aFormattedCellData)
            : CFormattedCellListBoxItemDrawer(aTextListBoxModel,
                                              aFont,
                                              aFormattedCellData)
    {
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxItemDrawer::DrawItem
// Drawing a item
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListboxItemDrawer::DrawItem(
        TInt aItemIndex,
        TPoint aItemRectPos,
        TBool aItemIsSelected,
        TBool aItemIsCurrent,
        TBool aViewIsEmphasized,
        TBool aViewIsDimmed,
        TInt aHighlightStartXPos,
        TInt aHighlightEndXPos) const
    {
    TRect actualItemRect(aItemRectPos, iItemCellSize);

    CAknFepUICtrlJapanesePredictiveCellListBoxData* data =
        static_cast<CAknFepUICtrlJapanesePredictiveCellListBoxData*>(iData);
    data->SetHighlightArea(aHighlightStartXPos, aHighlightEndXPos);

    DrawActualItem(
        aItemIndex,
        actualItemRect, 
        aItemIsCurrent, aViewIsEmphasized, aViewIsDimmed, aItemIsSelected);
    }

// End of File
