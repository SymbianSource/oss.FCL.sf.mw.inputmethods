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
* Description:  Predictive Candidate list view
*
*/


// INCLUDE FILES
#include "peninputjapanesepredictivelistboxmodel.h"
#include "peninputjapanesepredictiveListboxitemdrawer.h"
#include "peninputjapanesepredictivelistboxview.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::CurrentCandidateNo
// Return the selected candidate number currently
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::CurrentCandidateNo() const
    {
    return iCurrentCandidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::SetCurrentCandidateNo
// Set current candidate number
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::SetCurrentCandidateNo(
        TInt aCandidateNo)
    {
    iOldCandidateNo = iCurrentCandidateNo;
    iCurrentCandidateNo = aCandidateNo;

    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iModel);

    const TInt itemIndex = model->SearchItemIndexFromCandidateNo(aCandidateNo);

    return itemIndex;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::SetCurrentCandidateNo
// Set current candidate number
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::SetCurrentCandidateNo(
        TInt aLineNo, const TPoint& aPoint)
    {
    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iModel);

    iCurrentCandidateNo = model->SearchCandidateNoFromPosition(aLineNo, aPoint);
    if (iCurrentCandidateNo < 0)  // no data
        {
        iCurrentCandidateNo = 0;
        }

    return aLineNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::PreviousItem
// Return previous candidate number
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::PreviousItem()
    {
    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iModel);
    const TInt maximumItems = model->MaximumItems();

    TInt itemindex = iCurrentCandidateNo - 1;
    if (itemindex < 0)
        {
        itemindex = maximumItems - 1;
        }

    return itemindex;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::NextItem
// Return next candidate number
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::NextItem()
    {
    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iModel);
    const TInt maximumItems = model->MaximumItems();

    TInt itemindex = iCurrentCandidateNo +1;
    if (itemindex >= maximumItems)
        {
        itemindex = 0;
        }

    return itemindex;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::PreviousLineItem
// Return candidate number of previous line
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::PreviousLineItem()
    {
    TInt currentItemIndex = CurrentItemIndex();
    TPredictivePopupElement currentElement;

    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iModel);
    CPredictivePopupLineInfoArray* lineInfoArray = model->LineInfoArray();
    TInt previousCandidateNo = -1;
    if (lineInfoArray)
        {
        const TInt lineInfoArrayCount = lineInfoArray->Count();

        TInt previousItemIndex = currentItemIndex;

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
        }

    if (previousCandidateNo == -1)
        {
        previousCandidateNo = iCurrentCandidateNo;
        }

    return previousCandidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::NextLineItem
// Return candidate number of next line
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::NextLineItem()
    {
    TInt currentItemIndex = CurrentItemIndex();
    TPredictivePopupElement currentElement;

    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iModel);

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
// CPeninputJapanesePredictiveListBoxView::OldCandidateNo
// Return the selected candidate number previously
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxView::OldCandidateNo() const
    {
    return iOldCandidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxView::DrawItem
// Drawing item
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBoxView::DrawItem(TInt aItemIndex) const
    {
    if (RedrawDisabled() || !IsVisible())
        return;

    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iModel);

    if (model->LineInfoArray())
        {
        if (((aItemIndex > -1) && (aItemIndex < model->LineInfoArray()->Count()))
            && ItemIsVisible(aItemIndex))
            {
            TInt highlightStartXPos = 0;
            TInt highlightEndXPos = 0;

            TPredictivePopupElement currentElement;

            if (model->SearchElement(aItemIndex,
                                     iCurrentCandidateNo, currentElement))
                {
                highlightStartXPos = currentElement.iStartXPos;
                highlightEndXPos   = currentElement.iEndXPos;
                }

            CPeninputJapanesePredictiveListBoxItemDrawer* itemDrawer = 
                static_cast<CPeninputJapanesePredictiveListBoxItemDrawer*>(iItemDrawer);

            itemDrawer->DrawItem(
                aItemIndex,
                ItemPos(aItemIndex),
                ItemIsSelected(aItemIndex),
                (aItemIndex == iCurrentItemIndex),
                (iFlags & EEmphasized) > 0, (iFlags & EDimmed) > 0,
                highlightStartXPos, highlightEndXPos);
            }
        }
    }

// End of File
