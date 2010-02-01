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
#include "AknFepUIAvkonCtrlJapanesePredictiveListBoxModel.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListboxItemDrawer.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListboxView.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::CurrentCandidateNo
// Return the selected candidate number currently
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListboxView::CurrentCandidateNo() const
    {
    return iCurrentCandidateNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::SetCurrentCandidateNo
// Set current candidate number
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListboxView::SetCurrentCandidateNo(
        TInt aCandidateNo)
    {
    iOldCandidateNo = iCurrentCandidateNo;
    iCurrentCandidateNo = aCandidateNo;

    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(iModel);

    const TInt itemIndex = model->SearchItemIndexFromCandidateNo(aCandidateNo);

    return itemIndex;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::SetCurrentCandidateNo
// Set current candidate number
// -----------------------------------------------------------------------------
TInt CAknFepUICtrlJapanesePredictiveListboxView::SetCurrentCandidateNo(
        TInt aLineNo, const TPoint& aPoint)
    {
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(iModel);

    iCurrentCandidateNo = model->SearchCandidateNoFromPosition(aLineNo, aPoint);

    return aLineNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::PreviousItem
// Return previous candidate number
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListboxView::PreviousItem()
    {
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(iModel);
    const TInt maximumItems = model->MaximumItems();

    TInt itemindex = iCurrentCandidateNo - 1;
    if (itemindex < 0)
        {
        itemindex = maximumItems - 1;
        }

    return itemindex;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::NextItem
// Return next candidate number
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListboxView::NextItem()
    {
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(iModel);
    const TInt maximumItems = model->MaximumItems();

    TInt itemindex = iCurrentCandidateNo +1;
    if (itemindex >= maximumItems)
        {
        itemindex = 0;
        }

    return itemindex;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::PreviousLineItem
// Return candidate number of previous line
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListboxView::PreviousLineItem()
    {
    TInt currentItemIndex = CurrentItemIndex();
    TPredictivePopupElement currentElement;

    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(iModel);
    CPredictivePopupLineInfoArray* lineInfoArray = model->LineInfoArray();
    const TInt lineInfoArrayCount = lineInfoArray->Count();

    TInt previousItemIndex = currentItemIndex;
    TInt previousCandidateNo = -1;

    if (model->SearchElement(
            currentItemIndex, iCurrentCandidateNo, currentElement))
        {
        do
            {
            previousItemIndex--;
            if (previousItemIndex < 0)
                {
                previousItemIndex = lineInfoArrayCount - 1;
                }

            if (previousItemIndex == currentItemIndex)
                {
                break;      // in case one loop
                }

            CPredictivePopupLineInfo* lineInfo =
                lineInfoArray->At(previousItemIndex);

            CPredictivePopupElementArray* elementArray =
                lineInfo->iElementsOnLine;

            const TInt numberOfElement = elementArray->Count();
            TInt lastCandidateNo = -1;
            TInt i;
            for (i = 0; i < numberOfElement; i++)
                {
                TPredictivePopupElement element = elementArray->At(i);
                if (currentElement.iStartXPos >= element.iStartXPos
                 && currentElement.iStartXPos < element.iEndXPos)
                    {
                    previousCandidateNo = element.iCandidateNo;
                    break;
                    }
                else if (currentElement.iStartXPos < element.iStartXPos)
                    {
                    previousCandidateNo = element.iCandidateNo;
                    break;
                    }

                lastCandidateNo = element.iCandidateNo;
                }

            if (previousCandidateNo == -1)
                {
                previousCandidateNo = lastCandidateNo;
                }
            }
        while (previousCandidateNo == iCurrentCandidateNo);
        }

    if (previousCandidateNo == -1)
        {
        previousCandidateNo = iCurrentCandidateNo;
        }

    return previousCandidateNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::NextLineItem
// Return candidate number of next line
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListboxView::NextLineItem()
    {
    TInt currentItemIndex = CurrentItemIndex();
    TPredictivePopupElement currentElement;

    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(iModel);

    CPredictivePopupLineInfoArray* lineInfoArray = model->LineInfoArray();
    TInt nextCandidateNo = -1;

    if (lineInfoArray)
        {
        const TInt lineInfoArrayCount = lineInfoArray->Count();

        TInt nextItemIndex = currentItemIndex;

        if (model->SearchElement(
                    currentItemIndex, iCurrentCandidateNo, currentElement))
            {
            do
                {
                nextItemIndex++;
                if (nextItemIndex >= lineInfoArrayCount)
                    {
                    nextItemIndex = 0;
                    }

                if (nextItemIndex == currentItemIndex)
                    {
                    break;      // in case one loop
                    }

                CPredictivePopupLineInfo* lineInfo =
                    lineInfoArray->At(nextItemIndex);

                CPredictivePopupElementArray* elementArray =
                    lineInfo->iElementsOnLine;

                const TInt numberOfElement = elementArray->Count();
                TInt lastCandidateNo = -1;
                TInt i;
                for (i = 0; i < numberOfElement; i++)
                    {
                    TPredictivePopupElement element = elementArray->At(i);
                    if (currentElement.iStartXPos >= element.iStartXPos
                     && currentElement.iStartXPos < element.iEndXPos)
                        {
                        nextCandidateNo = element.iCandidateNo;
                        break;
                        }
                    else if (currentElement.iStartXPos < element.iStartXPos)
                        {
                        nextCandidateNo = element.iCandidateNo;
                        break;
                        }

                    lastCandidateNo = element.iCandidateNo;
                    }

                if (nextCandidateNo == -1)
                    {
                    nextCandidateNo = lastCandidateNo;
                    }
                }
            while (nextCandidateNo == iCurrentCandidateNo);
            }
        }
    if (nextCandidateNo == -1)
        {
        nextCandidateNo = iCurrentCandidateNo;
        }
    return nextCandidateNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::DrawItem
// Drawing item
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListboxView::DrawItem(TInt aItemIndex) const
    {
    if (RedrawDisabled() || !IsVisible())
        return;

    if (((aItemIndex > -1) && (aItemIndex < iModel->NumberOfItems()))
        && ItemIsVisible(aItemIndex))
        {
        TInt highlightStartXPos = 0;
        TInt highlightEndXPos = 0;

        TPredictivePopupElement currentElement;
        CAknFepUICtrlJapanesePredictiveListBoxModel* model =
            static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(iModel);

        if (model->SearchElement(aItemIndex,
                                 iCurrentCandidateNo, currentElement))
            {
            highlightStartXPos = currentElement.iStartXPos;
            highlightEndXPos   = currentElement.iEndXPos;
            }

        CAknFepUICtrlJapanesePredictiveListboxItemDrawer* itemDrawer =
            static_cast<CAknFepUICtrlJapanesePredictiveListboxItemDrawer*>(iItemDrawer);

        itemDrawer->DrawItem(
            aItemIndex,
            ItemPos(aItemIndex),
            ItemIsSelected(aItemIndex),
            (aItemIndex == iCurrentItemIndex),
            (iFlags & EEmphasized) > 0, (iFlags & EDimmed) > 0,
            highlightStartXPos, highlightEndXPos);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListboxView::OldCandidateNo
// Return the selected candidate number previously
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListboxView::OldCandidateNo() const
    {
    return iOldCandidateNo;
    }

// End of File
