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
* Description:  Predictive Candidate list model
*
*/


// INCLUDE FILES
#include <aknlayoutscalable_avkon.cdl.h> // AknLayoutScalable_Avkon
#include "peninputjapanesepredictivelistboxmodel.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CPredictivePopupLineInfo::~CPredictivePopupLineInfo
// Destructor
// -----------------------------------------------------------------------------
CPredictivePopupLineInfo::~CPredictivePopupLineInfo()
    {
    delete iLineText;
    delete iElementsOnLine;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxModel::SetLineInfoArray
// Registering a array with candidate line information
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBoxModel::SetLineInfoArray(
        CPredictivePopupLineInfoArray* aLineInfoArray)
    {
    iLineInfoArray = aLineInfoArray;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxModel::LineInfoArray
// Returning a arrary with candidate line information
// -----------------------------------------------------------------------------
CPredictivePopupLineInfoArray*
    CPeninputJapanesePredictiveListBoxModel::LineInfoArray()
    {
    return iLineInfoArray;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxModel::SearchItemIndexFromCandidateNo
// This function searches a item index as key in a candidate number
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxModel::SearchItemIndexFromCandidateNo(
        TInt aCandidateNo) const
    {
    TInt lineNo = -1;
    if (iLineInfoArray)
        {
        const TInt numberOfLine = iLineInfoArray->Count();
        TInt i;
        for (i = 0; i < numberOfLine && lineNo == -1; i++)
            {
            CPredictivePopupLineInfo* lineInfo = iLineInfoArray->At(i);
            CPredictivePopupElementArray* elementArray =lineInfo->iElementsOnLine;

            const TInt numberOfElement = elementArray->Count();
            TInt j;
            for (j = 0; j < numberOfElement; j++)
                {
                TPredictivePopupElement element = elementArray->At(j);
                if (aCandidateNo == element.iCandidateNo)
                    {
                    lineNo = i;
                    break;
                    }
                }
            }
        }
    return lineNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxModel::SearchElement
// This function is that it checks whether the candidate is included in the line.
// If including it, The displaying information is returned.
// -----------------------------------------------------------------------------
TBool CPeninputJapanesePredictiveListBoxModel::SearchElement(
        TInt aLineNo, TInt aCandidateNo, TPredictivePopupElement& aElement)
    {
    TBool foundFlag = EFalse;

    if ( 0 <= aLineNo && iLineInfoArray && iLineInfoArray->Count() > aLineNo )
        {
        CPredictivePopupLineInfo* lineInfo = iLineInfoArray->At(aLineNo);
        CPredictivePopupElementArray* elementArray = lineInfo->iElementsOnLine;

        const TInt numberOfElement = elementArray->Count();
        TInt i;
        for (i = 0; i < numberOfElement; i++)
            {
            TPredictivePopupElement element = elementArray->At(i);
            if (aCandidateNo < element.iCandidateNo)
                {
                break;
                }
            else if (aCandidateNo == element.iCandidateNo)
                {
                aElement.iStartXPos   = element.iStartXPos;
                aElement.iEndXPos     = element.iEndXPos;
                aElement.iCandidateNo = element.iCandidateNo;
                foundFlag = ETrue;
                break;
                }
            }
        }
    return foundFlag;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxModel::SearchCandidateNoFromPosition
// This function searches a CandidateNo from linenumber and Position
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxModel::SearchCandidateNoFromPosition(
        TInt aLineNo, const TPoint& aPoint) const
    {
    TInt atom = AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine().iH;
    TInt margin = atom*2/5;
    TInt candidateNo = -1;
    if ( 0 <= aLineNo && iLineInfoArray && iLineInfoArray->Count() > aLineNo )
        {
        CPredictivePopupLineInfo* lineInfo = iLineInfoArray->At(aLineNo);
        CPredictivePopupElementArray* elementArray = lineInfo->iElementsOnLine;
        const TInt numberOfElement = elementArray->Count();
        candidateNo = elementArray->At(numberOfElement - 1).iCandidateNo;
        for (TInt ii = 0; ii < numberOfElement; ii++)
            {
            TPredictivePopupElement element = elementArray->At(ii);
            if (aPoint.iX - margin <= element.iEndXPos)
                {
                candidateNo = element.iCandidateNo;
                break;
                }
            }
        }
    return candidateNo;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxModel::SetMaximumItems
// Set total item numbers in listbox
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveListBoxModel::SetMaximumItems(
        TInt aMaximumItems)
    {
    iMaximumItems = aMaximumItems;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveListBoxModel::MaximumItems
// Return total item numbers
// -----------------------------------------------------------------------------
TInt CPeninputJapanesePredictiveListBoxModel::MaximumItems() const
    {
    return iMaximumItems;
    }

// End of File
