/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Predictive Candidate list
*
*/












#ifndef CAKNFEPUICTRLJAPANESEPREDICTIVELISTBOX_H
#define CAKNFEPUICTRLJAPANESEPREDICTIVELISTBOX_H

// INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CAknFepUICtrlJapanesePredictiveListbox class.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapanesePredictiveListbox)
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
        void SetCurrentCandidateNo(TInt aCandidateNo, TBool aDraw);

        /**
        * Return the candidate number selected currently
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt CurrentCandidateNo();

        /**
        * selecting a previous candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MovePreviousItem();

        /**
        * selecting a next candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MoveNextItem();

        /**
        * selecting a previous line candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MovePreviousLineItem();

        /**
        * selecting a next line candidate
        * @since 2.6
        * @return the candidate number selected currently
        */
        TInt MoveNextLineItem();

        /**
         * Handling of pointer event within the @c CEikListBox.
         * Used for e.g. selecting an item from the list box.
         *
         * @since 3.2
         * @param aPointerEvent Details of the pointer event that is being handled.
         */
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);

    public: // Functions from CAknSinglePopupMenuStyleListBox
        virtual void SizeChanged();

    public: // Functions from CEikFormattedCellListBox
        virtual CListBoxView* MakeViewClassInstanceL();

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

        /**
         * from CEikListBox
         * Used for scrolling through the items in the list box.
         *
         * @param aScrollBar Scroll bar for the list box.
         * @param aEventType Type of the event that occured.
         */
        virtual void HandleScrollEventL(CEikScrollBar* aScrollBar,
                                                 TEikScrollEvent aEventType);
    };

#endif  // CAKNFEPUICTRLJAPANESEPREDICTIVELISTBOX_H

// End of File
