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

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapaneseCandidateListboxView.h" 

// ============================ MEMBER FUNCTIONS ===============================

#define ITEM_EXISTS_BEGIN TInt no_of_items__ = iModel->NumberOfItems()
#define ITEM_EXISTS(x) (((x) > -1) && ((x) < no_of_items__))

#define ITEM_EXISTS_ONCE(x) (((x) > -1) && ((x) < iModel->NumberOfItems()))

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListboxView::Draw
// Drawing listbox
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateListboxView::Draw(
        const TRect* /*aClipRect*/) const
    {
    if (RedrawDisabled() || !IsVisible())
        return;

    if (iModel->NumberOfItems() > 0)
        {
        TInt firstPotentialItemIndex = iTopItemIndex;
        TInt lastPotentialItemIndex =
            iTopItemIndex + NumberOfItemsThatFitInRect(iViewRect) - 1;
        TInt i;
        ITEM_EXISTS_BEGIN;
        for (i = firstPotentialItemIndex; i <= lastPotentialItemIndex; i++)
            {
            if (ITEM_EXISTS(i))
                {
                DrawItem(i);
                }
            else
                {
                break;
                }
            }
        }
    } 

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateListboxView::DrawItem
// Drawing item of index appointed with an argument
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateListboxView::DrawItem(
        TInt aItemIndex) const
    {
    if (RedrawDisabled() || !IsVisible())
        {
        return;
        }

    if (((aItemIndex > -1) && (aItemIndex < iModel->NumberOfItems()))
        && ItemIsVisible(aItemIndex))
        {
        iItemDrawer->DrawItem(
            aItemIndex,
            ItemPos(aItemIndex),
            ItemIsSelected(aItemIndex),
            (aItemIndex == iCurrentItemIndex),
            (iFlags & EEmphasized) > 0,
            (iFlags & EDimmed) > 0);
        }
    }

// End of File
