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
 *  Name     : CAknFepPredictiveSettingList from SettingExampleSettingList.h
 * ============================================================================
 */

#ifndef __AKN_FEP_PREDICTIVESETTING_LIST_H
#define __AKN_FEP_PREDICTIVESETTING_LIST_H


#include <coecntrl.h>
#include <aknsettingitemlist.h> 
#include <akncheckboxsettingpage.h>

//class CAknFepPredictiveSettingData;
#include "AknFepPredictiveSettingData.h"

class CAknFepPredictiveSettingDialog;
/**
 * Class: CAknFepMultiSelectionSettingItem
 *
 * Discussion:  An implementation of CAknBigSettingItemBase, providing a 
 *              big setting item that opens a multiselection (checkbox)
 *              setting page. Specialized implementation to show
 *              word autocompletion settings.
 * 
 */
class CAknFepMultiSelectionSettingItem : public CAknBigSettingItemBase
    {
public:
    CAknFepMultiSelectionSettingItem( TInt aResourceId, CAknFepPredictiveSettingData &aData,
            CCoeEnv* aCoeEnv );
    virtual ~CAknFepMultiSelectionSettingItem();
    virtual void EditItemL( TBool aCalledFromMenu );
    virtual void StoreL();
    virtual void LoadL();

private:
    CSelectionItemList* CreateSelectionItemListL();
    
    CAknFepPredictiveSettingData &iData; // not owned
    CSelectionItemList* iSelection;
    CCoeEnv* iCoeEnv;
    TInt iItutIndex;
    TInt iHalfQwertyIndex;
    TInt iQwertyIndex;
    };

/**
 * Class: CAknFepPredictiveSettingList
 *
 * Discussion: 	A specialised subclass of CAknSettingItemList. This class is
 *				the control used to both display the current values of the
 *				settings and to launch the relevant editor for those items.
 *
 */
class CAknFepPredictiveSettingList : public CAknSettingItemList
	{
public:
	/**
 	* Function: 	NewL
	*
 	* Discussion:	Factory function used to create a new instance of
 	* 				CAknFepPredictiveSettingList
	*
	* Params:		aData - reference to the data to be displayed and edited.
	*
 	* Returns:		A newly created instance of CAknFepPredictiveSettingList
	*/
	static CAknFepPredictiveSettingList* NewL(CAknFepPredictiveSettingData &aData, CAknFepPredictiveSettingDialog &aDialog);

	/**
 	* Function: 	NewLC
	*
 	* Discussion:	Factory function used to create a new instance of
 	* 				CAknFepPredictiveSettingList and leave a pointer to it on the
 	*				cleanup	stack.
	*
	* Params:		aData - reference to the data to be displayed and edited.
	*
 	* Returns:		A newly created instance of CAknFepPredictiveSettingList
	*/
	static CAknFepPredictiveSettingList* NewLC(CAknFepPredictiveSettingData &aData, CAknFepPredictiveSettingDialog &aDialog);

	/**
	* Function:		~CAknFepPredictiveSettingList
	*
	* Discussion: 	Virtual destructor for CAknFepPredictiveSettingList. Ensures that
	*				all	allocated memory is returned to the heap before
	*				deletion of object.
	*
	*/
	virtual ~CAknFepPredictiveSettingList();


	/**
	* Function:		SizeChanged
	*
	* Discussion: 	Inherited from CCoeControl. Calculates size based on
	*				contained control required sizes.
	*
	*/
	void SizeChanged();

	/**
	 * Function:	EditCurrentItemL
     * Params:		aCalledFromMenu - ETrue if called from menu, EFalse otherwise.
	 *
 	 *
	 * Discussion:	Starts the setting page for the currently selected item 
	 *				in the list.
	 */
	void EditCurrentItemL(TBool aCalledFromMenu);
	
    TBool DeviceHasMultipleKeyboards(); 
	
	
private:
	/**
	* Function:		CAknFepPredictiveSettingList
	*
	* Discussion: 	Hidden, first-stage non-leaving constructor.
	*
	* Params:		aData - reference to the data to be displayed and edited.
	*
	*/
	CAknFepPredictiveSettingList(CAknFepPredictiveSettingData &aData, CAknFepPredictiveSettingDialog &aDialog);

	/**
	* Function:		CreateSettingItemL
	*
	* Discussion: 	Factory function for creating CAknSettingItem derived
	*				SettingItem classes used t display current values
	*
	* Params:		aSettingId - the id of the control which should be created
	*
	*/
	CAknSettingItem* CreateSettingItemL (TInt aSettingId);
	
	
	void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

	void HandleResourceChange(TInt aType);

private:
	CAknFepPredictiveSettingData &iSettingsData;		/* Reference to data owned by Dialog */
	CAknFepPredictiveSettingDialog &iSettingsDialog;
	};

#endif // __AKN_FEP_PREDICTIVESETTING_LIST_H

