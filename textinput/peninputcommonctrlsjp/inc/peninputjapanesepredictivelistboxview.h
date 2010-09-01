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
* Description:  Predictive Candidate list view
*
*/


#ifndef CPENINPUTJAPANESEPREDICTIVELISTBOXVIEW_H
#define CPENINPUTJAPANESEPREDICTIVELISTBOXVIEW_H

// INCLUDES
#include <eiklbv.h>  // CListBoxView

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CPeninputJapanesePredictiveListBoxView draws all the list items and manages
*  the area of the screen allocated for list itself. In particular,
*  this class does not handle margins of the whole list.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 2.6
*/
class CPeninputJapanesePredictiveListBoxView : public CListBoxView
    {
    public: // New functions
        /**
        * Return the selected candidate number currently
        * @since 2.6
        * @return candidate number
        */
        TInt CurrentCandidateNo() const;

        /**
        * Set current candidate number
        * @since 2.6
        * @param    aCandidateNo    candidate number
        * @return   item index including candidate number
        */
        TInt SetCurrentCandidateNo(TInt aCandidateNo);

        /**
        * This function searches a CandidateNo from linenumber and Position
        * @since 2.6
        * @param aLineNo        line number
        * @param aPoint         position
        * @return   item index including candidate number
        */
        TInt SetCurrentCandidateNo(TInt aLineNo, const TPoint& aPoint);

        /**
        * Return previous candidate number
        * @since 2.6
        * @return candidate number
        */
        TInt PreviousItem();

        /**
        * Return next candidate number
        * @since 2.6
        * @return candidate number
        */
        TInt NextItem();

        /**
        * Return candidate number of previous line
        * @since 2.6
        * @return candidate number
        */
        TInt PreviousLineItem();

        /**
        * Return candidate number of next line
        * @since 2.6
        * @return candidate number
        */
        TInt NextLineItem();

        /**
        * Return the selected candidate number currently
        * @since 3.2
        * @return candidate number
        */
        TInt OldCandidateNo() const;

    public: // from CListBoxView
        virtual void DrawItem(TInt aItemIndex) const;

    private:    // Data
        TInt iCurrentCandidateNo;
        TInt iOldCandidateNo;
    };

#endif  // CPENINPUTJAPANESEPREDICTIVELISTBOXVIEW_H

// End of File
