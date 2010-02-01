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

#ifndef __GSPREDICTIVESETTING_DATA_H
#define __GSPREDICTIVESETTING_DATA_H

#include <e32base.h>

/**
 * Class: 		CGSPredictiveSettingData
 *
 * Discussion:	An aggregate class containing data for all editing controls
 *				This class is used as a central respository for all data
 *				to be displayed and editted within this app.
 */
class CGSPredictiveSettingData : public CBase
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
	static CGSPredictiveSettingData* NewL();

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
	static CGSPredictiveSettingData* NewLC();

	/**
	* Function:		~CGSPredictiveSettingData
	*
	* Discussion: 	Virtual destructor for CGSPredictiveSettingData. Ensures that all
	*				allocated memory is returned to the heap before deletion
	* 				of object.
	*
	*/
	virtual ~CGSPredictiveSettingData();

    /**
    * Function:    LoadL
    * 
    * Discussion:  Loads itself with parameters passed.
    * 
    */
    void LoadL(TInt aAutoWordCompl,
			   TInt aTypingCorrection,
			   TInt aNumberCandidates,
			   TInt aPrimaryCandidate,
			   TInt aPhysicalKeyboards,
			   TInt aAWCBitmask);
    
    /**
    * Function:     SaveL
    * 
    * Discussion:   Saves itself.
    * 
    */
	void SaveL() const;
    
	/**
 	* Function: 	AutoWordCompl
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iAutoWordCompl member
	*/
	inline TInt& AutoWordCompl() {return iAutoWordCompl; }
	
	/**
 	* Function: 	TypingCorrection
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iTypingCorrection member
	*/
	inline TInt& TypingCorrection() { return iTypingCorrection; }

	
	/**
 	* Function: 	NumberCandidates
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iNumberCandidates member
	*/
	inline TInt& NumberCandidates() {return iNumberCandidates; }
	
	/**
 	* Function: 	PrimaryCandidate
	*
 	* Discussion:	Trivial accessor function
	*
 	* Returns:		A reference to the iPrimaryCandidate member
	*/
	inline TInt& PrimaryCandidate() {return iPrimaryCandidate; }
	
	/**
	* Function:     PhysicalKeyboards
	*
	* Discussion:   Trivial accessor function
	*
	* Returns:      Value of iPhysicalKeyboards member
	*/
	inline TInt PhysicalKeyboards() {return iPhysicalKeyboards; }

    /**
    * Function:     AWCBitmask
    *
    * Discussion:   Trivial accessor function
    *
    * Returns:      Value of iPhysicalKeyboards member
    */
    inline TInt& AWCBitmask() {return iAWCBitmask; }

private:
	/**
	* Function:		CGSPredictiveSettingData
	*
	* Discussion: 	Hidden, first-stage non-leaving constructor.
	*
	*/
	CGSPredictiveSettingData();

	/**
	* Function:		ConstructL
	*
	* Discussion: 	Hidden, second-stage possibly-leaving constructor.
	*
	*/
	void ConstructL();


public:
	TInt iAutoWordCompl;					/* Data for AutoWordCompl field          */
	TInt iTypingCorrection;					/* Data for TypingCorrection text field  */
	TInt iNumberCandidates;					/* Data for NumberCandidates field       */
	TInt iPrimaryCandidate;					/* Data for PrimaryCandidate field       */
    TInt iPhysicalKeyboards;                /* Data for PhysicalKeyboards field      */
    TInt iAWCBitmask;                       /* AWC data in bitmask form              */
    
};

#endif // __GSPREDICTIVESETTING_DATA_H

