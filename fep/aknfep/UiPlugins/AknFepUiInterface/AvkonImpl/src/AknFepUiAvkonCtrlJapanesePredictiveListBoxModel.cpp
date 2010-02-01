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
#include "AknFepUIAvkonCtrlJapanesePredictivePane.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CPredictivePopupLineInfo::~CPredictivePopupLineInfo
// Destructor
// -----------------------------------------------------------------------------
//
CPredictivePopupLineInfo::~CPredictivePopupLineInfo()
    {
    delete iLineText;
    delete iElementsOnLine;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListBoxModel::SetLineInfoArray
// Registering a array with candidate line information
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListBoxModel::SetLineInfoArray(
        CPredictivePopupLineInfoArray* aLineInfoArray)
    {
    iLineInfoArray = aLineInfoArray;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListBoxModel::LineInfoArray
// Returning a arrary with candidate line information
// -----------------------------------------------------------------------------
//
CPredictivePopupLineInfoArray*
    CAknFepUICtrlJapanesePredictiveListBoxModel::LineInfoArray() const
    {
    return iLineInfoArray;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListBoxModel::SearchItemIndexFromCandidateNo
// This function searches a item index as key in a candidate number
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListBoxModel::SearchItemIndexFromCandidateNo(
        TInt aCandidateNo) const
    {
    TInt lineNo = -1;
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
    return lineNo;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListBoxModel::SearchElement
// This function is that it checks whether the candidate is included in the line.
// If including it, The displaying information is returned.
// -----------------------------------------------------------------------------
//
TBool CAknFepUICtrlJapanesePredictiveListBoxModel::SearchElement(
        TInt aLineNo, TInt aCandidateNo, TPredictivePopupElement& aElement)
    {
    TBool foundFlag = EFalse;

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
    return foundFlag;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListBoxModel::SetMaximumItems
// Set total item numbers in listbox
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictiveListBoxModel::SetMaximumItems(
        TInt aMaximumItems)
    {
    iMaximumItems = aMaximumItems;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListBoxModel::MaximumItems
// Return total item numbers
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictiveListBoxModel::MaximumItems()
    {
    return iMaximumItems;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictiveListBoxModel::SearchCandidateNoFromPosition
// This function searches a CandidateNo from linenumber and Position
// -----------------------------------------------------------------------------
TInt CAknFepUICtrlJapanesePredictiveListBoxModel::SearchCandidateNoFromPosition(
        TInt aLineNo, const TPoint& aPoint) const
    {
    TInt candidateNo = -1;
    if ( 0 <= aLineNo && iLineInfoArray->Count() > aLineNo )
        {
        CPredictivePopupLineInfo* lineInfo = iLineInfoArray->At(aLineNo);
        CPredictivePopupElementArray* elementArray = lineInfo->iElementsOnLine;
        const TInt numberOfElement = elementArray->Count();
        candidateNo = elementArray->At(numberOfElement - 1).iCandidateNo;
        for (TInt ii = 0; ii < numberOfElement; ii++)
            {
            TPredictivePopupElement element = elementArray->At(ii);
            //if (aPoint.iX /*- margin*/ <= element.iEndXPos)
            if (aPoint.iX <= element.iEndXPos)
                {
                candidateNo = element.iCandidateNo;
                break;
                }
            }
        }
    return candidateNo;
    }
// End of File
