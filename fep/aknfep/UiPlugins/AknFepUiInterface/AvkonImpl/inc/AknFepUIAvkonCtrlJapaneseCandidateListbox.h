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
* Description:            Candidate list
*
*/












#ifndef CAKNFEPUICTRLJAPANESECANDIDATELISTBOX_H
#define CAKNFEPUICTRLJAPANESECANDIDATELISTBOX_H

// INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CAknFepUICtrlJapaneseCandidateListbox class.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapaneseCandidateListbox)
        : public CAknSinglePopupMenuStyleListBox
    {
    public: // New functions
        /**
        * Set max digit number for listbox
        * @since 2.6
        * @param aMaximumColumns    max digit number
        */
        void SetMaximumColumns(const TInt aMaximumColumns);

        /**
        * Set max visible rows for listbox
        * @since 3.0
        * @param aMaximumRows    max visible rows
        */
        void SetMaximumRows(const TInt aMaximumRows);

        /**
        * Return max visible rows for listbox
        * @since 3.0
        */
        inline TInt MaximumRows();

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

    protected:  // Functions from CAknSinglePopupMenuStyleListBox
        virtual void CreateItemDrawerL();

    protected: // Functions from CEikListBox
        /**
         * from CEikListBox
         * Used for scrolling through the items in the list box.
         *
         * @param aScrollBar Scroll bar for the list box.
         * @param aEventType Type of the event that occured.
         */
        virtual void HandleScrollEventL(CEikScrollBar* aScrollBar,
                                                 TEikScrollEvent aEventType);

    private:    // Data
        TInt iMaximumColumns;
        TInt iMaximumRows;
    };

TInt CAknFepUICtrlJapaneseCandidateListbox::MaximumRows()
    {
    return iMaximumRows;
    }

#endif  // CAKNFEPUICTRLJAPANESECANDIDATELISTBOX_H

// End of File
