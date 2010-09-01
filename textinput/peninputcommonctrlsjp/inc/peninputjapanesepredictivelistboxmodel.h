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


#ifndef CPENINPUTJAPANESEPREDICTIVELISTBOXMODEL_H
#define CPENINPUTJAPANESEPREDICTIVELISTBOXMODEL_H

// INCLUDES
#include <AknUtils.h>


// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  TPredictivePopupElement class.
*  This structure stores the information to display a candidate.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 2.6
*/
struct TPredictivePopupElement
    {
    TInt iStartXPos;
    TInt iEndXPos;
    TInt iCandidateNo;
    };

typedef CArrayFixFlat<TPredictivePopupElement> CPredictivePopupElementArray;

// CLASS DECLARATION

/**
*  CPredictivePopupLineInfo class.
*  This class stores a line information on predictive candidate list.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 2.6
*/
class CPredictivePopupLineInfo : public CBase
    {
public:
    inline CPredictivePopupLineInfo() {};
    virtual ~CPredictivePopupLineInfo();

    HBufC* iLineText;
    CPredictivePopupElementArray* iElementsOnLine;
    };

typedef CArrayPtrFlat<CPredictivePopupLineInfo> CPredictivePopupLineInfoArray;


// CLASS DECLARATION

/**
*  CPeninputJapanesePredictiveListBoxModel class.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 2.6
*/
class CPeninputJapanesePredictiveListBoxModel
        : public CAknFilteredTextListBoxModel
    {
    public: // New functions
        /**
        * This function registeres the array of line information of candidate list.
        * @since 2,6
        * @param aLineInfoArray
        */
        void SetLineInfoArray(CPredictivePopupLineInfoArray* aLineInfoArray);

        /**
        * This function returns a arrary object that the information is included each line.
        * @since 2.6
        * @return array object
        */
        CPredictivePopupLineInfoArray* LineInfoArray();

        /**
        * This function searches a item index as key in a candidate number
        * 
        * @since 2.6
        * @param aCandidateNo   candidate number
        * @return item index  -1:no data
        */
        TInt SearchItemIndexFromCandidateNo(TInt aCandidateNo) const;

        /**
        * This function is that it checks whether the candidate is included in the line.
        * If including it, The displaying information is returned.
        * @since 2.6
        * @param aLineNo        line number
        * @param aCandidateNo   candidate number
        * @param aElement       display mode
        * @return ETrue if including candidate
        */
        TBool SearchElement(TInt aLineNo,
                            TInt aCandidateNo,
                            TPredictivePopupElement& aElement);

        /**
        * This function searches a CandidateNo from linenumber and Position
        * @since 2.6
        * @param aLineNo        line number
        * @param aPoint         position
        * @eturn candidate number
        */
        TInt SearchCandidateNoFromPosition(TInt aLineNo, const TPoint& aPoint) const;

        /**
        * Set total item numbers in listbox
        * @since 2.6
        * @param aMaximumItems  total item numbers
        */
        void SetMaximumItems(TInt aMaximumItems);

        /**
        * Return total item numbers
        * @since 2.6
        * @return total item numbers
        */
        TInt MaximumItems() const;

    private:    // Data
        /** 
        * Not Own
        */
        CPredictivePopupLineInfoArray* iLineInfoArray;

        TInt iMaximumItems;
    };

#endif  // CPENINPUTJAPANESEPREDICTIVELISTBOXMODEL_H

// End of File
