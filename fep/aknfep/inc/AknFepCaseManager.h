/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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












#ifndef _AKNFEP_CASE_MANAGER_H__
#define _AKNFEP_CASE_MANAGER_H__

#include "AknFepManager.h"

#include <e32base.h>

class CAknEdwinState;
class CAknPictographInterface;


class CAknFepCaseManager : public CBase
    {
private:
    enum AknFepCaseManagerConsts
        {
        ETextCaseCharactersBufferMaxSize            =16
        };

public:
    /**
    * Two phase constructor for this class
    */
    static CAknFepCaseManager* NewL(CAknFepManager& aFep);

    /**
    * C++ Destructor
    */
    ~CAknFepCaseManager();

public:
    /**
    * Configure the current case state of the system based on the current
    * editor state
    */
    void ConfigureCaseStateFromEditorState();

    /**
    * Start an automatic case update - determines the case of the next
    * character based on the rules governing auto case update and applies
    * this change accordingly
    *
    * @param aNaviEvent the type of event which prompted the case update
    *                   (affects the final cursor position)
    */
    void UpdateCase(TWesternNaviEvent aNaviEvent);

    /**
    * Starts a manual case update
    */
    void ManualCaseUpdate();

    /**
    * Reverts the case of text input to the previous one in order to
    * back out unneccasary case changes
    */
    void RevertCaseChange();

    /**
    * Set the case of text input
    *
    * @param aCase the case of the next character
    * @param aOverrideNumericTest Isn't used in most cases, but if set, handles
    *                             numeric mode as normal mode. Needed for selection
    *                             style hash key loop implementation.
    */
    void SetCurrentCase(TInt aCase, TBool aOverrideNumericTest = EFalse);

    /**
    * Return the current case of text input
    *
    * @return integer representing an enumerated value of the current case
    */
    TInt CurrentCase() const;

    /**
    * check whether case can be changed
    *
    * @return Eture if case can be changed
    */
    TBool IsAbleChangeCase() const;
    
    TInt PluginPermittedCases();
    
	TBool CapsLockRequired(TInt aCurrentCase);
	/* Teleca change begin, 18.08.2009 ssal */
	/**
    * Determines whether a character is one of Korean characters
    * @param aChar the charcter to be checked
    * @return ETrue if aChar is Korean character
    */
	TBool IsKoreanCharacter( TChar aChar ) const;   
    /* Teleca change end, 18.08.2009 ssal */
private:
    /**
    * C++ Constructor
    */
    CAknFepCaseManager(CAknFepManager& aFep);

    /**
    * Second phase constructor for this class
    */
    void ConstructL();

    /**
    * Determines the position the cursor will be in when a navigation event is
    * complete. This is used to determine the required case at that position.
    *
    * @param aNaviEvent the type of event after which the case will be updated
    * @return The position in the document of the cursor when aNaviEvent is
    *           complete
    */
    TInt FindCursorPositionAtCaseCheck(TWesternNaviEvent aNaviEvent);

    /**
    * Determines, based on the case rules, whether text case needs to be
    * applied for text input i.e. whether the next character needs
    * to be entered in upper case and subsequent characters in lower case
    *
    * @return ETrue if text case needs to be applied for the next 
    *           character
    */
    TBool TextCaseRequired(TInt aCursorPos);

    /**
    * Determines, based on the case rules, whether or not caps lock input style
    * needs to be applied i.e. if the current case is upper and we should stay
    * in upper following the entry of the next character
    *
    * @return ETrue if caps lock input style is to be applied
    */
    TBool CapsLockRequired(TInt aCursorPos, TInt aCurCase);

    /**
    * Determines the case of text input based on the rules governing
    * case updates atarted by the user and applies this change accordingly
    */
    void DoManualCaseUpdate();

    /**
    * Applies the case change in the ptiengine (via the engine interface) and
    * updates the edit state indicators accordingly
    */
    void ApplyCase();

    /**
    * Sets the system to be in fixed case.  No case updates are permitted
    * in this state
    *
    * @param aFixedCase if ETrue then no case updates are subsequently
    *                   permitted
    */
    void SetFixedCase(TBool aFixedCase);

    /**
    * Determines if the current editor has the fixed case flag set
    *
    * @return ETrue if the fixed case flag is set on the current editor
    */
    TBool CurrentEditorIsFixedCase() const;

    /**
    * Determines if a case is one which is allowed in the current editor
    * based on the editor's configuration
    *
    * @param aCase the case to be checked 
    * @return ETrue if aCase is permitted by the current editor
    */
    TBool IsCasePermitted(TInt aCase) const;

    /**
    * 
    * 
    *
    * @param aChar the charcter to be checke
    * @return ETrue 
    */
    TBool IsTextCaseCharacter(TChar aChar) const;

    /**
    * 
    * 
    *
    * @param aChar the charcter to be checked
    * @return ETrue if aChar is full or half alphabet
    */
    TBool IsAlpha(TChar aChar) const;
    
    /**
    * Determines whether a character is one of Japanese characters
    * Japanese character is included as below:
    *   - Hiragana/Kanji
    *   - Katakana (Both half and full)
    *   - Symbol (Full-width only)
    *      (including Full-width Full-Stop)
    *
    * @since S60 v3.1
    * @param aChar the charcter to be checked
    * @return ETrue if aChar is Japanese character
    */
    TBool IsJapaneseCharacter(TChar aChar) const;

    /* Teleca change begin, 18.08.2009 ssal */
    /**
    * Determines whether a character is one of Korean hangul syllable
    * @param aChar the charcter to be checked
    * @return ETrue if aChar is korean character
    */
    TBool IsHangulSyllable(TChar aChar) const;

    /**
    * Determines whether a character is one of Korean hangul compatibility Jamo Syllable
    * @param aChar the charcter to be checked
    * @return ETrue if aChar is Korean character
    */
	TBool IsHangulCompatibilityJamo(TChar aChar) const;
	/* Teleca change begin, 18.08.2009 ssal */

public:    
    /**
    * Calls to ManulaCaseUpdate() in lower case situation will switch to 
    * upper case even if text case would be normally selected.
    */
    inline void SetForceUpperCase();
    
    /** 
    * Clear force upper case flag (see SetForceUpperCase()). 
	*/ 
	inline void ClearForceUpperCase();

	inline TBool IsInverseCase() const;
	inline void SetInverseCase( TBool aVal );

private:
    CAknFepManager& iFep; //not owned
    CAknPictographInterface* iPictoInterface; //owned
    
    TInt iCurrentCase;
    TInt iPreviousCase;
    TBool iFixedCase;
    TBool iForceUpperCase;
    TBool iInverseCase;
    TBufC<ETextCaseCharactersBufferMaxSize> iTextCaseCharacters;
    };

inline void CAknFepCaseManager::SetForceUpperCase()
	{
	iForceUpperCase = ETrue;
	}
    
 
inline void CAknFepCaseManager::ClearForceUpperCase()
	{
	iForceUpperCase = EFalse;
	}
	
inline TBool CAknFepCaseManager::IsInverseCase() const
    {
    return iInverseCase;
    }

inline void CAknFepCaseManager::SetInverseCase( TBool aVal )
    {
    iInverseCase = aVal;
	}

#endif //_AKNFEP_CASE_MANAGER_H__
