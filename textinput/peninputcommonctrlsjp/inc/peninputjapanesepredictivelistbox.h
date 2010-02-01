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
* Description:  Predictive Candidate list
*
*/


#ifndef CPENINPUTJAPANESEPREDICTIVELISTBOX_H
#define CPENINPUTJAPANESEPREDICTIVELISTBOX_H

// INCLUDES
#include <aknlists.h>   // CAknSinglePopupMenuStyleListBox

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CPeninputJapanesePredictiveListBox class.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 2.6
*/
class CPeninputJapanesePredictiveListBox
        : public CAknSinglePopupMenuStyleListBox
    {
    public:  // Constructors and destructor
        void ConstructL(const CCoeControl* aParent, TInt aFlags);

    public: // New functions
        /**
        * Selecting the index as argument
        * @since 2.6
        * @param aCandidateNo  The index of the cursor position
        * @param aDraw  ETrue is redraw.
        */
        void SetCurrentCandidateNo(TInt aCandidateNo, TBool aDraw) const;

        /**
        * Selecting the index as argument
        * @since 2.6
        * @param aLineNo        line number
        * @param aPoint         position
       * @param aDraw  ETrue is redraw.
        */
        void SetCurrentCandidateNo(TInt aLineNo, const TPoint& aPoint, TBool aDraw) const;

        /**
        * Return the candidate number selected currently
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt CurrentCandidateNo() const;

        /**
        * selecting a previous candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MovePreviousItem() const;

        /**
        * selecting a next candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MoveNextItem() const;

        /**
        * selecting a previous line candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MovePreviousLineItem() const;

        /**
        * selecting a next line candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MoveNextLineItem() const;

        /**
         * Handling of pointer event within the @c CEikListBox. 
         * Used for e.g. selecting an item from the list box.
         *
         * @param aPointerEvent Details of the pointer event that is being handled.
         */
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);

        /**
        * DrawNow,DrawScrollBarsNow
        *
        */
        void Draw() const;

        /**
        * Set linecount
        *
        */
        void SetLineCount(TInt aLineCount);

    public: // Functions from CAknSinglePopupMenuStyleListBox
        virtual void SizeChanged();

    public: // Functions from CEikFormattedCellListBox
        virtual CListBoxView* MakeViewClassInstanceL();

    /**
     * from CEikListBox
     * Used for scrolling through the items in the list box. 
     *
     * @param aScrollBar Scroll bar for the list box.
     * @param aEventType Type of the event that occured.
     */
    virtual void HandleScrollEventL(CEikScrollBar* aScrollBar, 
                                             TEikScrollEvent aEventType);

    protected: // Functions from CAknSinglePopupMenuStyleListBox
        virtual void CreateItemDrawerL();

    protected: // Functions from CEikListBox
        /**
         * Sets an item as the current item, even if it is not currently
         * visible. Redraws the list box to reflect the change. This
         * should not be called from within another Draw function.
         *
         * @param aItemIndex The index of the list box item to update.
         */
        virtual void UpdateCurrentItem(TInt aItemIndex) const;

    private:
        TInt iLineCount;
    };

#endif  // CPENINPUTJAPANESEPREDICTIVELISTBOX_H

// End of File
