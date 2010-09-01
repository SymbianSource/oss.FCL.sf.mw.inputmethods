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
* Description:            Provides the TAknFepInputStateInitialIndicPhoneticMultitap definition.
*
*/











#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_INDIC_PHONETIC_MULTITAP_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_INDIC_PHONETIC_MULTITAP_H__

#include "AknFepUIInputStateInitialMultitapBase.h"
/**
*  This class  handles the multitap input for Indic languages.
*  @lib avkonfep.dll
*  @since Series 60 3.1
*/
class TAknFepInputStateInitialIndicPhoneticMultitap : public TAknFepInputStateInitialMultitapBase
    {
public:
    /**
    * 
    * Constructor for TAknFepInputStateInitialIndicPhoneticMultitap.	
    * @since 5.0
    * @return 
    */
    TAknFepInputStateInitialIndicPhoneticMultitap(MAknFepUIManagerStateInterface* aOwner, 
                                                  TInt aLanguage); 
public: //from TAknFepInputStateBase    
    /**
    * 
    * Returns the virama character depending on the input language.	
    * @since 5.0
    * @return Etrue if the Key is handled. 
    */

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
#ifdef RD_HINDI_PHONETIC_INPUT 
    /**
    * 
    * Populates iPhoneticLatinChar to aChar.	
    * @since 5.0
    * @return None. 
    */
    virtual void GetPhoneticLatinChar(TChar& aChar)const;
#endif 
    
public: //from MptiObserver
    /**
    * 
    * Gets called when PTI Engine's timer expires.	
    * @since 5.0
    * @return 
    */
	void KeyTimerExpired();
	/**
	 * Transliterate the current inputted character to language specific character
	 * of Halant.
	 * @since 5.0
	 * @param Null
	 * @return Null
	 */
private:
	void TransliterateFromLatinL();
	/**
     * Removes Halant added by Phonetic PtiENgine for North Indian Languages.
     * 
     * @since 5.0
     * @param aLanguage- Current Phonetic input language is passed to this function.
     * Engine.
     * @return Nothing
     */
	void RemoveHalantL(TLanguage aLanguage);
	
	/**
     * Checks wheather the current Phonetic Input Language follows North Indian style of editing. This is necessary for removal
     * of Halant.
     * @since 5.0
     * @param aLanguage- Current Phonetic input language is passed to this function.
     * Engine.
     * @return Returns True if the current language comes under North Indian style of writting.
     */
	TBool IsToRemoveHalantForLanguage(TInt aLanguage);
	/**
	 * Checks if the character has phonetic equivallent to be Transliterated
	 * of Halant.
	 * @since 5.0
	 * @param aChar character to be checked
	 * @return returns ETrue if the character can be transliterated
	 */
	inline TBool IsToTransliterate(TChar& aChar);
	/**
	 * Checks if the current transliterated character can be inputted as per the language rule, if not then Question mark is inputted 
	 * of Halant.
	 * @since 5.0
	 * @param aChar character to be validated for inputting
	 * @return ETrue if the character is allowed
	 */
	TBool ValidatePhoneticInputL(const TDes& aBuf, TLanguage aLanguage);
	/**
	 * Checks if the current inputted character is a fallback rendered character
	 * of Halant.
	 * @since 5.0
	 * @param aChar Character to be checked
	 * @return ETrue is the character is a fallback rendered character
	 */	
	TBool IsInvalidPhoneticCharacter( TUint aChar, TLanguage aLanguage );
	/**
	 * Handels the key timer expiry
	 * of Halant.
	 * @since 5.0
	 * @param Null
	 * @return Null
	 */
	void HandleKeyTimerExpiryL();
	};


#endif //__AKN_FEP_UI_INPUT_STATE_INIT_INDIC_MULTITAP_H__

// End of File
