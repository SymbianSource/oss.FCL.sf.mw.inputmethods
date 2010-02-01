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
 *  Name     : CGSPredictiveSettingData from AknFepPredictiveSettingData.h
 * ============================================================================
 */

#ifndef __DUALLANGUAGE_DATA_H
#define __DUALLANGUAGE_DATA_H

#include <e32base.h>
#include <badesca.h>
#include <aknsettingitemlist.h>

/**
 * Class: 		CGSPredictiveSettingData
 *
 * Discussion:	An aggregate class containing data for all editing controls
 *				This class is used as a central respository for all data
 *				to be displayed and editted within this app.
 */
class CGSDualLanguageData : public CBase
{
public:
	/**
 	* Function: 	NewL
	*
 	* Discussion:	Factory function used to create a new instance of
 	* 				CGSPredictiveSettingData
	*
 	* Returns:		A newly created instance of CGSPredictiveSettingData with all data
 	*				items valid
	*/
	static CGSDualLanguageData* NewL();

	/**
 	* Function: 	NewLC
	*
 	* Discussion:	Factory function used to create a new instance of
 	* 				CGSPredictiveSettingData and leave a pointer to it on the cleanup
 	*				stack
	*
 	* Returns:		A newly created instance of CGSPredictiveSettingData with all data
 	*				items valid
	*/
	static CGSDualLanguageData* NewLC();

	/**
	* Function:		~CGSPredictiveSettingData
	*
	* Discussion: 	Virtual destructor for CGSPredictiveSettingData. Ensures that all
	*				allocated memory is returned to the heap before deletion
	* 				of object.
	*
	*/
	virtual ~CGSDualLanguageData();

    /**
    * Function:    LoadL
    * 
    * Discussion:  Loads itself with parameters passed.
    * 
    */
	void LoadL(CDesCArrayFlat* aItems,
	           CDesCArrayFlat* aSecondaryItems,
	           TInt aCurrentValue,
	           TInt aCurrentSecondaryValue);

    
    /**
    * Function:     SaveL
    * 
    * Discussion:   Saves itself.
    * 
    */
	void SaveL() const;
    
	/**
 	* Function: 	Items
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iItems member
	*/
	inline CDesCArrayFlat* Items() {return iItems; }
	
	/**
 	* Function: 	SecondaryItems
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iSecondaryItems member
	*/
	inline CDesCArrayFlat* SecondaryItems() { return iSecondaryItems; }

	/**
 	* Function: 	CurrentValue
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iCurrentValue member
	*/
	inline TInt& CurrentValue() {return iCurrentValue; }

	/**
 	* Function: 	CurrentSecondaryValue
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iCurrentSecondaryValue member
	*/
	inline TInt& CurrentSecondaryValue() {return iCurrentSecondaryValue; }
	
private:
	/**
	* Function:		CGSPredictiveSettingData
	*
	* Discussion: 	Hidden, first-stage non-leaving constructor.
	*
	*/
    CGSDualLanguageData();

	/**
	* Function:		ConstructL
	*
	* Discussion: 	Hidden, second-stage possibly-leaving constructor.
	*
	*/
	void ConstructL();


public:
    CDesCArrayFlat* iItems;
    CDesCArrayFlat* iSecondaryItems;
    TInt iCurrentValue;
    TInt iCurrentSecondaryValue;    
};

#endif // __DUALLANGUAGE_DATA_H

