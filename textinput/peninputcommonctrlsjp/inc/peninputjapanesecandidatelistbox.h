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
* Description:  Candidate List ListBox
*
*/

#ifndef C_CPENINPUTJAPANESECANDIDATELISTBOX_H
#define C_CPENINPUTJAPANESECANDIDATELISTBOX_H

#include <aknlists.h>   // CAknSinglePopupMenuStyleListBox

class CPeninputJapaneseCandidateListBox: public CAknSinglePopupMenuStyleListBox
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
     * Handling of pointer event within the @c CEikListBox. 
     * Used for e.g. selecting an item from the list box.
     *
     * @param aPointerEvent Details of the pointer event that is being handled.
     */
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);

protected:
    /**
     * from CEikListBox
     * Used for scrolling through the items in the list box. 
     *
     * @param aScrollBar Scroll bar for the list box.
     * @param aEventType Type of the event that occured.
     */
    virtual void HandleScrollEventL(CEikScrollBar* aScrollBar, 
                                             TEikScrollEvent aEventType);

public: // Functions from CAknSinglePopupMenuStyleListBox
    TSize MinimumSize();
    virtual void SizeChanged();

protected:  // Functions from CAknSinglePopupMenuStyleListBox
    virtual void CreateItemDrawerL();

private:    // Data
    TInt iMaximumColumns;
    TInt iMaximumRows;
    };

#endif //C_CPENINPUTJAPANESECANDIDATELISTBOX_H

// End Of File
