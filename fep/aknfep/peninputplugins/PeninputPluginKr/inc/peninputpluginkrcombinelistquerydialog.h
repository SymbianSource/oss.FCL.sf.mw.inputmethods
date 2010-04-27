/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           Korean Combining Word List Query Dialg 
*
*/


#ifndef C_PENINPUTPLUGINKRCOMBINELISTQUERYDIALOG_H
#define C_PENINPUTPLUGINKRCOMBINELISTQUERYDIALOG_H
#include <aknlistquerydialog.h>

class CPeninputPluginKrCombineListQueryDialog : public CAknListQueryDialog 
    {
public:
    /**
     * C++ default constructor.
     *
     * @since S60 v5.0
     * @param aIndex After the query is dismissed, the index will 
     *        hold the value of selected item.
     */
    CPeninputPluginKrCombineListQueryDialog(TInt* aIndex);

    /**
     * C++ default constructor.
     *
     * @since S60 v5.0
     * @param aSelectionIndexArray Will hold the values of selected
     *        items (in multiselection list).
     */
    CPeninputPluginKrCombineListQueryDialog(
              CListBoxView::CSelectionIndexArray* aSelectionIndexArray);
protected:
    /**
     * from CCoeControl
     *
     * Handles a change to the control's resources. 
     * @since S60 v5.0
     * @param aType aType A message UID valuey
     */
    void HandleResourceChange( TInt aType );
    };

#endif //C_PENINPUTPLUGINKRCOMBINELISTQUERYDIALOG_H

// End of File
