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
* Description:            Provides the TAknFepUiIndicInputManager definition.
*
*/











#ifndef __AKN_FEP_UI_INDIC_INPUT_MANAGER_H__
#define __AKN_FEP_UI_INDIC_INPUT_MANAGER_H__

#include "AknFepGlobalEnums.h"
#include "AknFepUiIndicEnums.h"

#include <e32const.h>
#include <PtiEngine.h>
#ifdef RD_HINDI_PHONETIC_INPUT	
#include <PtiIndicDefs.h>
#endif
/**
*  This class provides Helper methods for Indic Multitap class.
*  @lib avkonfep.dll
*  @since Series 60 3.1
*/

class TAknFepUiIndicInputManager 
	{
	public:
		
		/**
	    * This method validates the input key and returns a correct response code.
	    * Depending on the code, the character is added. This method applies the rules of 
	    * Language to the user Input. 
	    *
	    * @since 3.1M
	    * @return TIndicInputResponse
	    */
		static	TIndicInputResponse ValidateInput(TInt  aKey, 
												  TUint aPrevChar,
												  TUint aCurrChar,
												  TUint aNextChar, 
												  TBool& aIsHalantAllowed,	
												  TBool aIsStarKeyPressed, 
												  TBool aIsKeyTimerExpired, 
												  TPtiTextCase& aNewCase, 
												  TKeyPressLength aLength,
												  TLanguage aLanguage,
												  TBool aIsRakarPresent = EFalse,
												  TBool aIsZWSPresent = EFalse
#ifdef RD_MARATHI
												  , TBool aIsChandraAPresent = EFalse
#endif // RD_MARATHI
												  );
		/**
	    * Checks wheather Character is Empty. 
	    * @since 3.1M
	    * @return ETrue if the  character is not present else EFalse
	    */										  
		static TBool IsCharEmpty(TUint aChar, TLanguage aLanguage);	
		/**
	    * Checks wheather Character is Other. Space is not supposed to be Other character.
	    * @since 3.1M
	    * @return ETrue if the  character is any other character that doesn’t belong to Hindi (like digits) based on the given language else EFalse
	    */
		static TBool IsCharOther(TUint aChar, TLanguage aLanguage);
		/**
	    * Checks wheather Character is Base consonant.
	    * @since 3.1M
	    * @return ETrue if the  character is a base consonant based on 
	    * the given language else Efalse
	    */
		static TBool IsCharBaseConsonant(TUint aChar, TLanguage aLanguage);
		/**
	    * 
	    * Checks wheather Character is Nukta.
	    * @since 3.1M
	    * @return ETrue if the  character is a Nukta based on the 
	    * given language else Efalse
	    */
		static TBool IsCharNukta(TUint aChar, TLanguage aLanguage);
		/**
	    * 
	    * Checks wheather Character is Matra.
	    * @since 3.1M
	    * @return ETrue if the  character is a matra based on the 
	    * given language else EFalse
	    */
		static TBool IsCharMatra(TUint aChar,  TLanguage aLanguage);
		/**
	    * 
	    * Checks wheather Character is Vowel.
	    * @since 3.1M
	    * @return ETrue if the  character is an vowel based on the 
	    * given language else EFalse
	    */
		static TBool IsCharAnVowel(TUint aChar, TLanguage aLanguage);
		/**
	    * 
	    * Checks wheather Character is Modifier.
	    * @since 3.1M
	    * @return ETrue if the  character is an modifier based on the 
	    * given language else EFalse
	    */
		static TBool IsCharModifier(TUint aChar, TLanguage aLanguage);
		/**
	    * 
	    * Checks wheather Character is Virama.
	    * @since 3.1M
	    * @return ETrue if Character is virama.
	    */
		static TBool IsCharVirama(TUint aChar, TLanguage aLanguage);
		/**
	    * 
	    * Checks wheather Character is Space.
	    * @since 3.1M
	    * @return ETrue if Character is Space.
	    */
		static TBool IsCharSpace(TUint aChar);
		/**
	    * 
	    * Checks wheather it is possilble to enter Virama (Halant) as per the language rule.
	    * @since 3.1M
	    * @return
	    */
		static void SetHalantOrMatraAllowed(TUint aPrevChar,TBool& aViramaAllowed,
                                        TBool aIsKeyTimerExpired,TLanguage aLanguage);
		/**
	    * 
	    * Returns the virama character depending on the input language.	
	    * @since 3.1M
	    * @return Virama character for the langiage aLanguage
	    */
		static TUint Virama(TLanguage aLanguage);
		/**
	    * 
	    * Returns the True if the language is an Indic Language.	
	    * @since 3.1M
	    * @param aInputLanguage current input language
	    *  param aUiLanguage current UI language
	    * @return True for Indic language.
	    */
		static TBool IsIndicLangauge(TLanguage aInputLanguage);
		/**
	    * 
	    * Returns the True if the language supports Indic Digits.	
	    * @since 3.1M
	    * @param aInputLanguage current input language
	    *  param aUiLanguage current UI language
	    *
	    * @return True for digit support.
	    */
		static TBool IsIndicDigitsSupported(TLanguage aInputLanguage);

	    /**
		 * Handles the logic of Indic Repha input. This function first checks
		 * the validity of the inputted Repha and then allows it to be entered
		 *
		 * @since 3.2
		 * @param aChar The characer to be checked.
		 * @param aLanguage The input language
		 * @return ETrue if the character is allowed as governed by the rules 
		 * of the language else EFalse.
		 */
		static TBool IsRephaAllowed( TUint aChar, TLanguage aLanguage );
		
		/**
		 * This function checks whether the character is Repha (U0971).
		 *
		 * @since 3.2
		 * @param aChar The character to be checked.
		 * @param aLanguage The input language
		 * @return ETrue if the  character is repha else returns EFalse
		 */			
		static TBool IsCharRepha( TUint aChar, TLanguage aLanguage );		
		
		/**
		 * This function returns the Ra character for the language specified.
		 *
		 * @since 3.2
		 * @param aLanguage The input language
		 * @return TUint The ra character.
		 */
		static TUint RaCharacter( TLanguage aLanguage );
		
		/**
		 * This function populates the Repha character into aRepha parameter.
		 *
		 * @since 3.2
		 * @param aRepha A descriptor in which Repha is stored.
		 * @param aLanguage The input language
		 * @return void
		 */		
		static void GetRepha( TDes& aRepha, TLanguage aLanguage );
		
		/**
		 * This function checks whether the character is Rakar (U0972).
		 *
		 * @since 3.2
		 * @param aChar The character to be checked.
		 * @param aLanguage The input language
		 * @return ETrue if the  character is Rakar else returns EFalse
		 */			
		static TBool IsCharRakar( TUint aChar, TLanguage aLanguage );
		
		/**
		 * This function populates the Rakar character into aRakar parameter.
		 *
		 * @since 3.2
		 * @param aRakar A descriptor in which Rakar is stored.
		 * @param aLanguage The input language
		 * @return void
		 */
		static void GetRakar( TDes& aRakar, TLanguage aLanguage );

		/**
		 * This function populates the ligature into the buffer 
		 * depending on the current input character.
		 *
		 * @since 3.2
		 * @param aLigature A descriptor in which the ligature is stored.
		 * @param aCurrChar The current character from the key map.
		 * @param aLanguage The input language
		 * @return void
		 */
		static void GetLigature( TDes& aLigature, 
								TUint aCurrChar, 
								TLanguage aLanguage );

		/**
		 * This function checks whether the character is a special ligature.
		 *
		 * @since 3.2
		 * @param aChar The character to be checked.
		 * @param aLanguage The input language
		 * @return ETrue if the  character is a special ligature else EFalse.
		 */
		static TBool IsCharLigature( TUint aChar, TLanguage aLanguage );

		/**
		 * This function checks if entry of modifier is allowed.
		 *
		 * @since 3.2
		 * @param aPrevChar The previous character.
		 * @param aIsKeyTimerExpired. Indicates if the multitap timer is alive
		 * @param aLanguage. The input language.
		 * @return void
		 */
		 static TBool IsModifierAllowed( TUint aPrevChar, 
		 								TLanguage aLanguage );

	   /**
	    * 
	    * Checks if the character is the first ligature.
	    * @since 3.2M
	    * @return ETrue if Character is the first ligature.
	    */
		static TBool IsCharAtStartOfLigature(TUint aChar, 
											TLanguage aLanguage);
#ifdef RD_HINDI_PHONETIC_INPUT	
		/**
	    * 
	    * Returns the True if the language is Phonetic Language.	
	    * @since 3.1M
	    * @param aInputLanguage current input language
	    *  param aUiLanguage current UI language
	    *
	    * @return True if language is Phonetic.
	    */
		static TBool IsIndicPhoneticLanguage(TLanguage aInputLanguage);
	    /** @since 3.1M
	    * @param aInputLanguage current input language
	    *  param aChar Indic Character
	    *
	    * @returns Key associated with char.
	    */
		static TInt MapCharacterToKeys(TUint aChar, TLanguage aLanguage);
#endif

		/**
		 * This function checks whether the script is Devanagari
		 *
		 * @since 5.0
		 * @param aLanguage The input language
		 * @return ETrue if the  script is Devanagari,
		 * 		   else returns EFalse
		 */
		static TBool IsScriptDevanagari( TLanguage aInputLanguage );

        /**
		 * This function checks the response code. AknFepManager calls
		 * this method with the response code to know if the CommitInlineEditL
		 * needs to be invoked.
		 *
		 * @since 5.0
		 * @param aResponse The Current response code.
		 * @return ETrue if the inline text is to be committed
		 * 		   else returns EFalse
		 */
		static TBool IsToCommitCharSeq( TIndicInputResponse aResponse );

		/**
		 * This function checks whether the character is a Nukta Consonant.
		 *
		 * @since 5.0
		 * @param aChar The character to be checked.
		 * @param aLanguage The input language
		 * @return ETrue if the  character is a Nukta Consonant,
		 * 		   else returns EFalse
		 */
		static TBool IsCharNuktaConsonant( TUint aChar, TLanguage aLanguage );
    
#ifdef RD_MARATHI
		/**
		 * This function checks whether the character is Eye Lash Ra (U0977).
		 *
		 * @since 5.0
		 * @param aChar The character to be checked.
		 * @param aLanguage The input language
		 * @return ETrue if the  character is Eye Lash Ra, else returns EFalse
		 */
		static TBool IsCharEyeLashRa( TUint aChar, TLanguage aLanguage );

		/**
		 * This function populates the Eye Lash Ra character into 
		 * aEyeLashRa parameter. (0x0931+0x094D+0x200D)
		 *
		 * @since 5.0
		 * @param aEyeLashRa A descriptor in which Eye Lash Ra is stored.
		 * @param aLanguage The input language
		 * @return void
		 */
		static void GetEyeLashRa( TDes& aEyeLashRa, 
									TLanguage aLanguage );

		/**
		 * This function checks whether the character is a Chandra A (0x0978).
		 *
		 * @since 5.0
		 * @param aChar The character to be checked.
		 * @return ETrue if the  character is a Chandra A,
		 * 		   else returns EFalse
		 */
		static TBool IsCharChandraA( TUint aChar );

		/**
		 * This function populates the Chandra A character into 
		 * aChandraA parameter. (0x0905+0x0945)
		 *
		 * @since 5.0
		 * @param aChandraA A descriptor in which Chandra A is stored.
		 * @param aIsToGetSingleUnicodeValue. If ETrue, return 0x0978
		 * @return void
		 */
		static void GetChandraA( TDes& aChandraA, 
								TBool aIsToGetSingleUnicodeValue = EFalse );
#endif // RD_MARATHI
#ifndef RD_INTELLIGENT_TEXT_INPUT
		/**
	    * 
	    * Returns True if aChar is a mathra( Dependant vowel), Modifier or Virama.
	    * @since 5.0
	    */		
		static TBool IsIndicDependantCharacter(TUint aChar, TLanguage aLanguage);
#endif	// RD_INTELLIGENT_TEXT_INPUT

	}; //__AKN_FEP_UI_INDIC_INPUT_MANAGER_H__
	
#endif	

//End of File
