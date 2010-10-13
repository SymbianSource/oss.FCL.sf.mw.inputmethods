/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateBase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUTSTATE_BASE_H__
#define __AKN_FEP_UI_INPUTSTATE_BASE_H__

// INCLUDES
#include <e32std.h>
#include <uikon.hrh>

#include "AknFepGlobalEnums.h"
#include "AknFepManager.h"

#include <PtiObserver.h> // MPtiObserver

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATION

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;

/**
 *  TAknFepInputStateBase class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepInputStateBase : public MPtiObserver
    {
public: // Constructors and destructor
    /**
     * C++ default constructor
     *
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterfa
     */
    TAknFepInputStateBase(MAknFepUIManagerStateInterface* aOwner);

public: // Functions from base classes
    /**
     * From MPtiObserver interface
     * Call this function when Multitap timer of PtiEngine was time-out
     *
     * @since 2.6
     */
    virtual void KeyTimerExpired();

    /**
     * From MPtiObserver interface
     * Call this function when getting last word in selection list
     *
     * @since 2.6
     */
    virtual void LastWordInSelectionList();

    /**
     * From MPtiObserver interface
     * Call this function when getting first word in selection list
     *
     * @since 2.6
     */
    virtual void FirstWordInSelectionList();

public: // New functions
    /**
     * Hnadling Keypress events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETrue If aKey was handled
     */
    virtual TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aCommandId Command ID value
     */
    virtual void HandleCommandL(TInt aCommandId);
    virtual void HandleCommandL(TInt aCommandId, TInt aParam);
    /**
     * return own input state.
     *
     * @since 2.6
     * @return own input sate
     */
    virtual TUIState State() const;

    /**
     * Processing of Closeing UI
     * @since 2.6
     */
    virtual void CloseUI();

    /**
     * Initialize state
     * @since 2.6
     */
    virtual void InitializeStateL();

    /**
     * A patch to force the vtable to be copied during an assignment.
     * @since 2.6
     * @param aState TAknFepInputStateBase object
     */
    virtual void operator=(const TAknFepInputStateBase& aState);

	/**
	 * Set the Star key flag
	 *
     * @param aSet TBool.
     * @return None
	 */
	virtual void SetStarKeyFlag( TBool aSet );
#ifdef RD_HINDI_PHONETIC_INPUT 
	/**
	 * Gives the iPhoneticLatinChar
	 *
     * @param aChar TChar iPhoneticLatinChar value.
     * @since 5.0
     * @return None
	 */
	virtual void GetPhoneticLatinChar(TChar& aChar)const;
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
	/**
	 * Gets the length of the character of the short key press
     * @return Lenght of the character
	 */
	virtual TInt GetShortKeyPressTxtLength();
	/**
	 * Set the text for short key press
	 *
     * @param aText Teh text entered for short key press.
     * @return None
	 */
	virtual void SetShortKeyPressText(const TDesC& aText);
#endif // RD_INTELLIGENT_TEXT_INPUT	

protected: // Data
		TBool iFakeDelete;  	                                        
		TBool iCharKeyException;	                                    
protected: // Data
    MAknFepUIManagerStateInterface* iOwner;
    TUIState iState;
    TWordMatchState iMatchState; 
    TInt iData; // data store for sub-classes...
    TInt iCbaResourceID;
protected:
#ifdef RD_HINDI_PHONETIC_INPUT        
	TChar iPhoneticLatinChar; // Used for current Latin character to be transliterated on key timer expiry or on other key press 
    TBuf<CAknFepManager::EMaximumFepWordLength > iIndicPhoneticBuffer; // Used to hold the uncommited word to append with latin character    
#endif
	//Used for Hindi
    TBool iIsStarKeyPressed;
	TBool iIsKeyTimerExpired;
	TLanguage iIndicLanguage;
	TBool iIsHalantAllowed;  
	TBool iZWSPresent;
	TBool iKey1Pressed;
	TBool iRephaPresent;
	TBool iRakarPresent;
	TBool iResponseInsertZWS;
	TBool iLigaturePresent;
        TBool iIsLastWord;
	/** Stores the character which was present in the editor before
	  * multitapping started. Used for validation of current character.
	  */
	TUint iPreviousCommittedChar;

#ifdef RD_MARATHI
	TBool iEyeLashRaPresent;
	TBool iChandraAPresent;
#endif // RD_MARATHI
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TBuf<CAknFepManager::EMaximumFepWordLength > iShortKeyPressText;
#endif
    };

#endif

// End of file
