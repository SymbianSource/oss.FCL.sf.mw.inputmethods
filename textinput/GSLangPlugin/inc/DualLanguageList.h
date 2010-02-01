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
* Description:
*
*/
/*
 * ============================================================================
 *  Name     : CGSPredictiveSettingList from SettingExampleSettingList.h
 * ============================================================================
 */

#ifndef __DUALLANGUAGE_LIST_H
#define __DUALLANGUAGE_LIST_H


#include <coecntrl.h>
#include <aknsettingitemlist.h> 
#include <akncheckboxsettingpage.h>
#include "GSLangModel.h"
#include "DualLanguageData.h"

class CGSDualLanguageDialog;

class CGSLanguageSettingItem : public CAknEnumeratedTextPopupSettingItem
    {
public:
    CGSLanguageSettingItem( TInt aResourceId, TBool aPrimary, CGSDualLanguageData* aData );
    virtual ~CGSLanguageSettingItem();
protected:
    virtual void CompleteConstructionL();
private:
    CGSDualLanguageData* iData;
    TBool iPrimary;
    };

/**
 * Class: CGSDualLanguageList
 *
 * Discussion:  A specialised subclass of CAknSettingItemList. This class is
 *              the control used to both display the current values of the
 *              settings and to launch the relevant editor for those items.
 *
 */
class CGSDualLanguageList : public CAknSettingItemList
    {
public:
    /**
    * Function:     NewL
    *
    * Discussion:   Factory function used to create a new instance of
    *               CGSPredictiveSettingList
    *
    * Params:       aData - reference to the data to be displayed and edited.
    *
    * Returns:      A newly created instance of CGSPredictiveSettingList
    */
    static CGSDualLanguageList* NewL(CGSDualLanguageData* aData, CGSDualLanguageDialog* aDialog);

    /**
    * Function:     NewLC
    *
    * Discussion:   Factory function used to create a new instance of
    *               CGSPredictiveSettingList and leave a pointer to it on the
    *               cleanup stack.
    *
    * Params:       aData - reference to the data to be displayed and edited.
    *
    * Returns:      A newly created instance of CGSPredictiveSettingList
    */
    static CGSDualLanguageList* NewLC(CGSDualLanguageData* aData, CGSDualLanguageDialog* aDialog);

    /**
    * Function:     ~CGSPredictiveSettingList
    *
    * Discussion:   Virtual destructor for CGSPredictiveSettingList. Ensures that
    *               all allocated memory is returned to the heap before
    *               deletion of object.
    *
    */
    virtual ~CGSDualLanguageList();


    /**
    * Function:     SizeChanged
    *
    * Discussion:   Inherited from CCoeControl. Calculates size based on
    *               contained control required sizes.
    *
    */
    void SizeChanged();

    /**
     * Function:    EditCurrentItemL
     * Params:      aCalledFromMenu - ETrue if called from menu, EFalse otherwise.
     *
     *
     * Discussion:  Starts the setting page for the currently selected item 
     *              in the list.
     */
    void EditCurrentItemL(TBool aCalledFromMenu);
    
    // Toggle whether secondary input item is visible
    void HideSecondaryInputL(TBool aHidden);
    
private:
    /**
    * Function:     CGSPredictiveSettingList
    *
    * Discussion:   Hidden, first-stage non-leaving constructor.
    *
    * Params:       aData - reference to the data to be displayed and edited.
    *
    */
    CGSDualLanguageList(CGSDualLanguageData* aData, CGSDualLanguageDialog* aDialog);

    /**
    * Function:     CreateSettingItemL
    *
    * Discussion:   Factory function for creating CAknSettingItem derived
    *               SettingItem classes used t display current values
    *
    * Params:       aSettingId - the id of the control which should be created
    *
    */
    CAknSettingItem* CreateSettingItemL (TInt aSettingId);
    
    void HandleListBoxEventL(CEikListBox* /*aListBox*/, TListBoxEvent aEventType);

private:
    CGSDualLanguageData* iData;
    CGSDualLanguageDialog* iDialog;
    TInt iPrimaryValue;
    TInt iSecondaryValue;
    };

#endif // __DUALLANGUAGE_LIST_H

