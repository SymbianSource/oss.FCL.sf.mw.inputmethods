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
*       Provides the TAknFepUIIndicInputManager methods.
*
*/











#include "AknFepUiIndicInputManager.h"
// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::ValidateInput

// This function validates the input text given by PtiEngine. This function
// returns the resopnse code depending on the previous character, current input 
// character and the next character. Depending on the response code apropriate 
// action is taken for inputting the current inputed character. This function 
// incorporates the logic for Devanagari alphabate input.  
// -----------------------------------------------------------------------------


TIndicInputResponse TAknFepUiIndicInputManager::ValidateInput( TInt  aKey, 
										  TUint aPrevChar,
										  TUint aCurrChar,
										  TUint /*aNextChar*/, 
										  TBool&  aIsHalantOrMatraAllowed,	
										  TBool aIsStarKeyPressed, 
										  TBool aIsKeyTimerExpired,
										  TPtiTextCase& aNewCase, 
										  TKeyPressLength /*aLength*/,
										  TLanguage aLanguage,
										  TBool aIsRakarPresent /* = EFalse*/,
										  TBool aIsZWSPresent /* = EFalse */ 
#ifdef RD_MARATHI
										  , TBool aIsChandraAPresent /* = EFalse */
#endif // RD_MARATHI
										  )

	{
	TIndicInputResponse keyResponse =  EIndicInputResponseNone;

	TPtiTextCase setcase  = EPtiCaseLower;
	
	TAknFepUiIndicInputManager::SetHalantOrMatraAllowed( aPrevChar,
														aIsHalantOrMatraAllowed,
														aIsKeyTimerExpired,
														aLanguage );
	
	switch( aKey )
		{
		case EPtiKey1:
			{
			if( IsCharNukta( aCurrChar, aLanguage ) && 
				( IsCharNuktaConsonant( aPrevChar, aLanguage ) ||
					!IsCharBaseConsonant( aPrevChar, aLanguage ) ) )
				{
				keyResponse = EIndicInputResponseInvalid;
				}
#ifdef RD_MARATHI
			else if( IsCharEyeLashRa( aCurrChar, aLanguage ) )
				{
				keyResponse = EIndicInputResponseInsertEyeLashRa;
				}
#endif // RD_MARATHI

			else if ( IsCharEmpty( aPrevChar, aLanguage ) )
				{
				// The special characters present in key-1 have to be
				// allowed. So the response is not set to digit directly
				// But at the same time, skip the fallback modifiers.
				if( IsCharModifier( aCurrChar, aLanguage ) )
					{
					keyResponse = EIndicInputResponseInvalid;
					}
				}
			else if( IsCharRakar( aCurrChar, aLanguage ) )
				{
				if( aIsHalantOrMatraAllowed )
					{
					keyResponse = EIndicInputResponseInsertRakar;
					}
				else
					{
					keyResponse = EIndicInputResponseInvalid;
					}
				}
			else if( IsCharRepha( aCurrChar, aLanguage ) )
				{
				if( IsRephaAllowed( aPrevChar, aLanguage ) ||
					( aIsRakarPresent && !aIsKeyTimerExpired ) )
					{
					keyResponse = EIndicInputResponseInsertRepha;
					}
				else
					{
					keyResponse = EIndicInputResponseInvalid;
					}
				}
			else if ( IsCharOther( aPrevChar, aLanguage )
				     || IsCharVirama( aPrevChar, aLanguage ) )	
				{
				// Since the response  cannot be set to digit
				// directly here, validate the modifiers first.
				if( IsCharModifier( aCurrChar, aLanguage ) )
					{
					if( !IsModifierAllowed( aPrevChar, aLanguage ) )
						{
						keyResponse = EIndicInputResponseInvalid;
						}
					}
				}
			else if ( IsCharModifier( aPrevChar, aLanguage ) )
				{
				// Unlike in earlier code, the response  cannot be set to
				// Digit directly here. The special characters present in 
				// key-1 have to be allowed. As a result, the fallback 
				// modifiers need to be skipped.
				if( IsCharModifier( aCurrChar, aLanguage ) )
					{
					if( !IsModifierAllowed( aPrevChar, aLanguage ) )
						{
						keyResponse = EIndicInputResponseInvalid;
						}
					}
				else
					{
					if (  ! aIsHalantOrMatraAllowed  )
						{
						setcase = EPtiCaseUpper;	
						}
					else if ( IsCharVirama( aCurrChar, aLanguage ) )
						{
						keyResponse = EIndicInputResponseInsertZWS;
						}	
					}	
				}
#ifdef RD_MARATHI
			else if( IsCharVirama( aCurrChar, aLanguage ) &&
				aIsHalantOrMatraAllowed )
				{
				keyResponse = EIndicInputResponseInsertZWS;
				}
#endif // RD_MARATHI

			// In cases where the response cannot be set to digit directly,
			// each character in key-1 has to be validated. Example in an 
			// Here we validate the Virama, Rakara and Repha
			if( IsCharVirama( aCurrChar, aLanguage ) )
				{
				if( keyResponse != EIndicInputResponseInsertZWS )
					{
					keyResponse = EIndicInputResponseInvalid;
					}
				}
			else if( IsCharRakar( aCurrChar, aLanguage ) )
				{
				if( keyResponse != EIndicInputResponseInsertRakar )
					{
					keyResponse = EIndicInputResponseInvalid;
					}
				}
			else if( IsCharRepha( aCurrChar, aLanguage ) )
				{
				// In case of empty editor, all the modifiers, virama
				// and rakar will be skipped. Even the Repha is to be skipped
				if( keyResponse != EIndicInputResponseInsertRepha )
					{
					keyResponse = EIndicInputResponseInvalid;
					}
				}
			}
			break;
	    
	    case EPtiKey2:
	    case EPtiKey3:
	    	{
    		if ( IsCharEmpty( aPrevChar, aLanguage )
	    		|| IsCharAnVowel( aPrevChar, aLanguage )
        		|| IsCharModifier( aPrevChar, aLanguage ) 
#ifdef RD_MARATHI
				|| ( aIsChandraAPresent && !aIsKeyTimerExpired )
#endif // RD_MARATHI
	    		)
    			{
	    		setcase = EPtiCaseUpper;
	    		}
	    	else if( IsCharVirama( aPrevChar, aLanguage ) )
	    		{
	    		setcase = EPtiCaseUpper;
	    		
	    		// Response code changed to EIndicInputResponseNone to avoid
	    		// insertion of ZWS character for Vowels.
	    		keyResponse = EIndicInputResponseNone;
	    		}
	    	else if ( IsCharOther( aPrevChar, aLanguage ) )
	    		{
	    		if ( aIsKeyTimerExpired || IsCharSpace( aPrevChar ) || ( ! aIsHalantOrMatraAllowed ) )
	    			{
	    			setcase = EPtiCaseUpper;	
	    			}
	    		}
	    	else if ( IsCharMatra( aPrevChar, aLanguage ) )
		    	{
	    		if ( aIsKeyTimerExpired )
	    			{
	    			setcase = EPtiCaseUpper;	
	    			}		    	
		    	}		  
	    	else if ( IsCharBaseConsonant( aPrevChar, aLanguage )
		    		|| IsCharNukta( aPrevChar, aLanguage ) )
			    {
			    if ( aIsStarKeyPressed )
			    	{
					setcase = EPtiCaseUpper;	
			    	}					
				}
#ifdef RD_MARATHI
            if( IsCharChandraA( aCurrChar ) )
				{
				keyResponse = EIndicInputResponseInsertChandraA;
				}
#endif // RD_MARATHI
	    	}
	    	break;
	    
	    case EPtiKey4:
	    case EPtiKey5:
	    case EPtiKey6:
	    case EPtiKey7:
	    case EPtiKey8:
	    case EPtiKey9:
	    	{
			if ( aIsStarKeyPressed )
				{
				if( IsCharBaseConsonant( aCurrChar, aLanguage )
			       && ( IsCharBaseConsonant( aPrevChar, aLanguage ) || 
			            IsCharNukta( aPrevChar, aLanguage ) ) )
					{
					keyResponse = EIndicInputResponseInsertZWSandLigature;
					}
				else if( IsCharBaseConsonant( aCurrChar, aLanguage )
			       && IsCharVirama( aPrevChar, aLanguage ) )
					{
					keyResponse = EIndicInputResponseZWSandCharacter;
					}
				}
			else if( IsCharVirama( aPrevChar, aLanguage ) )
				{
				keyResponse = EIndicInputResponseZWSandCharacter;	
				}
	    	}
	    	break;
	    
		case EPtiKey0:
			{
			// Special ligatures added in key-0.
			if( IsCharLigature( aCurrChar, aLanguage ) )
				{
				if ( aIsStarKeyPressed && 
						IsCharBaseConsonant( aPrevChar, aLanguage ) )
					{
					keyResponse = EIndicInputResponseInsertViramaZWSandDirectLigature;
					}
				else if( IsCharVirama( aPrevChar, aLanguage ) && 
						IsCharAtStartOfLigature( aCurrChar, aLanguage ) && 
						!aIsZWSPresent )
					{
					keyResponse = EIndicInputResponseZWSandDirectLigature;
					}
				else
					{
					keyResponse = EIndicInputResponseInsertDirectLigature;
					}
				}
			}
	    break;

	    default:
		  	{
			keyResponse = EIndicInputResponseIgnore;
	      	}
	    	break;
			
		}
	aNewCase = setcase;
	return keyResponse;
	}



// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharOther

// This function checks for wheather the character is other character. The other 
// character includes numbers, character of other languages and spaces.
// -----------------------------------------------------------------------------

TBool TAknFepUiIndicInputManager::IsCharOther( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;	
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		
		case KLangHindiPhonetic:
#endif		
			{
			if ( !( IsCharBaseConsonant( aChar, aLanguage )
					||	IsCharNukta( aChar, aLanguage )
					||	IsCharMatra( aChar, aLanguage )
					||	IsCharAnVowel( aChar, aLanguage )
					||	IsCharModifier( aChar, aLanguage )
					||	IsCharVirama( aChar, aLanguage )
					||  IsCharRakar( aChar, aLanguage )
					||  IsCharRepha( aChar, aLanguage ) 
					||	IsCharLigature( aChar, aLanguage ) 
#ifdef RD_MARATHI
					||	IsCharEyeLashRa( aChar, aLanguage ) 
					||  IsCharChandraA( aChar )
#endif
					) )
				{
				result = ETrue;	
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	return( result );	
	}	

// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharEmpty

// This function checks wheather the character is null
// -----------------------------------------------------------------------------

TBool TAknFepUiIndicInputManager::IsCharEmpty( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;	
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		case KLangHindiPhonetic:
#endif		
			{
			if ( aChar == 0 )
				{
				result = ETrue;	
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	return( result );	
	}	


// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharBaseConsonant

// This function checks wheather the character is base consonant
// -----------------------------------------------------------------------------

TBool TAknFepUiIndicInputManager::IsCharBaseConsonant( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;	
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		case KLangHindiPhonetic:
#endif		
			{
			if ( ( aChar >= 0x0915 && aChar <= 0x0939 )
					||( aChar >= 0x0958 && aChar <= 0x095F ) )
				{
				result = ETrue;							
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	return( result );	
	}


// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharNukta

// This function checks wheather the character is Nukta.
// -----------------------------------------------------------------------------

 TBool TAknFepUiIndicInputManager::IsCharNukta( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;	
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		case KLangHindiPhonetic:
#endif		
			{
			if ( aChar == 0x093C )
				{
				result = ETrue;	
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	
	return( result );	
	}

// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharMatra

// This function checks wheather the character is Nukta.
// -----------------------------------------------------------------------------

TBool TAknFepUiIndicInputManager::IsCharMatra( TUint aChar,  TLanguage aLanguage )
	{
	TBool result = EFalse;	
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		case KLangHindiPhonetic:
#endif		
			{
			if ( ( aChar >= 0x093E && aChar <= 0x094C ) 
					||( aChar == 0x0962 || aChar == 0x0963 ) )
				{
				result = ETrue;							
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	return( result );
	}

// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharAnVowel

// This function checks for wheather the character is Nukta.
// -----------------------------------------------------------------------------

 TBool TAknFepUiIndicInputManager::IsCharAnVowel( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		case KLangHindiPhonetic:
#endif
			{
			if ( ( aChar>=0x0904 && aChar<=0x0914 )
					||( aChar==0x0960 || aChar==0x0961 ) )
				{
				result = ETrue;							
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	return( result );
	}

// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharModifier

// This function checks wheather the character is Modifier.
// -----------------------------------------------------------------------------

 TBool TAknFepUiIndicInputManager::IsCharModifier( TUint aChar, TLanguage aLanguage )
	{	
	TBool result = EFalse;
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		case KLangHindiPhonetic:
#endif		
			{
			if ( aChar>= 0x0901 && aChar <= 0x0903 )
				{
				result = ETrue;	
				}				
			}
			break;
		default: /*Other Language*/
			break;
		}
	
	return( result );
	}

// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharVirama

// This function checks wheather the character is Halant.
// -----------------------------------------------------------------------------


 TBool TAknFepUiIndicInputManager::IsCharVirama( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		
		case KLangHindiPhonetic:
#endif
			{
			if ( aChar == 0x094D )
				{
				result = ETrue;	
				}
			}
		break;
		default: /*Other Language*/
		break;
		}
		
	return( result );
	}		
	
// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::IsCharSpace

// This function checks wheather the character is Space.
// -----------------------------------------------------------------------------

TBool TAknFepUiIndicInputManager::IsCharSpace( TUint aChar )
	{
	TBool result = EFalse;
	
	if ( aChar == ( 0x0020 ) )
		{
			result = ETrue;	
		}
	return( result );	
	}
	
// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::SetHalantAllowed

// This function checks wheather the Halant can be inputted at the current curssor
// posiotion.
// -----------------------------------------------------------------------------
	
void TAknFepUiIndicInputManager::SetHalantOrMatraAllowed( TUint aPrevChar, 
													   	TBool& aViramaAllowed,
													   	TBool aIsKeyTimerExpired,
													   	TLanguage aLanguage )
	{
	if ( IsCharAnVowel( aPrevChar, aLanguage ) )
		{
		aViramaAllowed = EFalse;
		}
	else if ( IsCharBaseConsonant( aPrevChar, aLanguage ) || 
		IsCharNukta( aPrevChar, aLanguage ) )
		{
		aViramaAllowed = ETrue;
		}
	if ( ( IsCharMatra( aPrevChar, aLanguage ) || 
		IsCharOther( aPrevChar, aLanguage )  || 
		IsCharVirama( aPrevChar, aLanguage ) ||
		IsCharModifier( aPrevChar, aLanguage ) ) && aIsKeyTimerExpired )
		{
		// The new conditions are required as we are not making the response 
		// to digit as before. We iterate through all the characters in key-1
		// and skip the ones which are not required.
		aViramaAllowed = EFalse;	
		}
	}

// -----------------------------------------------------------------------------
// TAknFepUiIndicInputManager::Virama

// This function returns back the unicode value for Halant.
// -----------------------------------------------------------------------------
		
TUint TAknFepUiIndicInputManager::Virama( TLanguage aLanguage )
	{
	TUint ret = 0;
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		case KLangHindiPhonetic:
#endif		
			{
			ret = 0x094D;	
			}
		break;
		default: /*Other Language*/
		break;
		}
		
	return( ret );	
	}

TBool TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage aInputLanguage)
	{	
	TBool ret = EFalse;
	switch(aInputLanguage)
		{
		case ELangHindi:
		case ELangMarathi:
			ret = ETrue;
		break;

		default: /*Other Language*/
		break;
		}
	return ret;
	}

TBool TAknFepUiIndicInputManager :: IsIndicDigitsSupported(TLanguage aInputLanguage)
	{	
	TBool ret = EFalse;
	switch(aInputLanguage)
		{
#ifdef RD_HINDI_PHONETIC_INPUT	
		
		case KLangHindiPhonetic:
			ret = ETrue;
		break;
#endif
		default:
			TLanguage uiLanguage = User::Language();
			switch(uiLanguage)
				{
				case ELangHindi:
				case ELangMarathi:
					ret = ETrue;
				}
		}
	return ret;
	}

TBool TAknFepUiIndicInputManager::IsRephaAllowed( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;

	switch( aLanguage )
		{
		case ELangMarathi:
		case ELangHindi:
			{
			if ( IsCharBaseConsonant( aChar, aLanguage ) ||
				 IsCharMatra( aChar, aLanguage ) )
				{
				result = ETrue;
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	return( result );
	}

TBool TAknFepUiIndicInputManager::IsCharRepha( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;
	
	switch( aLanguage )
		{
		case ELangMarathi:
		case ELangHindi:
			{
			if ( aChar == 0x0971 )
				{
				result = ETrue;
				}
			}
		break;
		default: /*Other Language*/
		break;
		}
		
	return( result );
	}

TUint TAknFepUiIndicInputManager::RaCharacter( TLanguage aLanguage )
	{
	TUint ret = 0;
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
			{
			ret = 0x0930;
			}
		break;
		
		// Other Language
		default:
		break;
		}
		
	return( ret );
	}

void TAknFepUiIndicInputManager::GetRepha( TDes& aRepha, TLanguage aLanguage )
	{
	aRepha.Zero();
	
	switch( aLanguage )
		{
		case ELangMarathi:
		case ELangHindi:
			{
			TChar raChar( TAknFepUiIndicInputManager::RaCharacter( aLanguage ) );
			TChar virama( TAknFepUiIndicInputManager::Virama( aLanguage ) );
			aRepha.Append( raChar );
			aRepha.Append( virama );
			}
			break;
			
		// Other Language
		default:
		break;
		}
	}

TBool TAknFepUiIndicInputManager::IsCharRakar( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
			{
			if ( aChar == 0x0972 )
				{
				result = ETrue;
				}
			}
		break;
		default: /*Other Language*/
		break;
		}
		
	return( result );
	}

void TAknFepUiIndicInputManager::GetRakar( TDes& aRakar, TLanguage aLanguage )
	{
	aRakar.Zero();
	
	switch( aLanguage )
		{
		case ELangMarathi:
		case ELangHindi:
			{
			TChar virama( TAknFepUiIndicInputManager::Virama( aLanguage ) );
			TChar raChar( TAknFepUiIndicInputManager::RaCharacter( aLanguage ) );
			aRakar.Append( virama );
			aRakar.Append( raChar );
			}
			break;
			
		// Other Language
		default:
		break;
		}
	}
#ifdef RD_HINDI_PHONETIC_INPUT	
TBool TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage(TLanguage aInputLanguage)
	{
	TBool ret = EFalse;
	switch(aInputLanguage)
		{
		case KLangHindiPhonetic:
			ret = ETrue;
		break;
		default:
		break;
		}
	return ret;	
	}
#endif


TBool TAknFepUiIndicInputManager::IsModifierAllowed( TUint aPrevChar, 
		 											TLanguage aLanguage )
	{
	if( IsCharVirama( aPrevChar, aLanguage ) 		||
			IsCharModifier( aPrevChar, aLanguage ) 	|| 
			IsCharOther( aPrevChar, aLanguage ) )
			{
			return EFalse;
			}

	return ETrue;
	}

void TAknFepUiIndicInputManager::GetLigature( TDes& aLigature, 
									TUint aCurrChar, TLanguage aLanguage )
	{
	aLigature.Zero();
	
	switch( aLanguage )
		{
		case ELangMarathi:
		case ELangHindi:
			{
			switch( aCurrChar )
				{
				case 0x0973:
					{
					// Ksha
					aLigature.Append( 0x0915 );
					aLigature.Append( Virama( aLanguage ) );
					aLigature.Append( 0x0937 );
					}
					break;

				case 0x0974:
					{
					// Gya(n)
					aLigature.Append( 0x091C );
					aLigature.Append( Virama( aLanguage ) );
					aLigature.Append( 0x091E );
					}
					break;

				case 0x0975:
					{
					// Shra
					aLigature.Append( 0x0936 );
					aLigature.Append( Virama( aLanguage ) );
					aLigature.Append( 0x0930 );
					}
					break;

				case 0x0976:
					{
					// Tra
					aLigature.Append( 0x0924 );
					aLigature.Append( Virama( aLanguage ) );
					aLigature.Append( RaCharacter( aLanguage ) );
					}
					break;

				default:
				break;
				}
			}
			
		// Other Language
		default:
		break;
		}
	}

TBool TAknFepUiIndicInputManager::IsCharLigature( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
			{
			if ( aChar >= 0x0973 && aChar <= 0x0976 )
				{
				result = ETrue;	
				}
			}
		break;
		default: /*Other Language*/
		break;
		}

	return( result );
	}

TBool TAknFepUiIndicInputManager::IsCharAtStartOfLigature( TUint aChar, 
													TLanguage aLanguage)
	{
	TBool result = EFalse;
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
			{
			if( aChar == 0x0973 )
				{
				// Ksha is the first ligature in the key-map.
				result = ETrue;
				}
				break;
			default: /*Other Language*/
			break;
			}
		}

	return( result );
	}

#ifdef RD_HINDI_PHONETIC_INPUT		
TInt TAknFepUiIndicInputManager ::MapCharacterToKeys(TUint aChar, TLanguage aLanguage)
	{
	TPtiKey ret = EPtiKeyNone;
	if(  TAknFepUiIndicInputManager::IsCharOther( aChar, aLanguage ) 
	  || TAknFepUiIndicInputManager::IsCharEmpty( aChar, aLanguage ))
		{
		ret = EPtiKey0;	
		}
	else if(TAknFepUiIndicInputManager::IsCharBaseConsonant( aChar, aLanguage ))
		{
		ret = EPtiKey4;	
		}
	else if(   TAknFepUiIndicInputManager::IsCharMatra( aChar,  aLanguage )
			|| TAknFepUiIndicInputManager::IsCharAnVowel( aChar, aLanguage ))
		{
		ret = EPtiKey2;	
		}
	else if(   TAknFepUiIndicInputManager::IsCharNukta( aChar, aLanguage )
			|| TAknFepUiIndicInputManager::IsCharModifier( aChar, aLanguage )
			|| TAknFepUiIndicInputManager::IsCharVirama( aChar, aLanguage ))
		{
		ret = EPtiKey1;	
		}
	return ret;
	}
#endif

TBool TAknFepUiIndicInputManager :: IsScriptDevanagari( TLanguage aInputLanguage )
	{	
	TBool ret = EFalse;
	switch(aInputLanguage)
		{
		case ELangHindi:
		case ELangMarathi:
			ret = ETrue;
		break;

		default:
		break;
		}
	return ret;
	}

TBool TAknFepUiIndicInputManager::IsToCommitCharSeq( TIndicInputResponse aResponse )
    {
    TBool result = EFalse;

    switch( aResponse )
        {
        case EIndicInputResponseInsertZWSandLigature:
        case EIndicInputResponseInsertViramaZWSandDirectLigature:
            {
            result = ETrue;
            }
            break;
        default:
            break;
        }
    return result;
    }

TBool TAknFepUiIndicInputManager::IsCharNuktaConsonant( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;	
	
	switch( aLanguage )
		{	
		case ELangMarathi:
		case ELangHindi:
#ifdef RD_HINDI_PHONETIC_INPUT	
		
		case KLangHindiPhonetic:
#endif		
			{
			if ( aChar == 0x0929 || 
				aChar == 0x0931 || aChar == 0x0934 || 
				( aChar >= 0x0958 && aChar <= 0x095F ) )
				{
				result = ETrue;
				}
			}
			break;
		default: /*Other Language*/
			break;
		}
	return( result );
	}

#ifdef RD_MARATHI

TBool TAknFepUiIndicInputManager::IsCharEyeLashRa( TUint aChar, TLanguage aLanguage )
	{
	TBool result = EFalse;
	
	switch( aLanguage )
		{	
		case ELangMarathi:
			{
			if ( aChar == 0x0977 )
				{
				result = ETrue;
				}
			}
		break;
		default: /*Other Language*/
		break;
		}
		
	return( result );
	}

void TAknFepUiIndicInputManager::GetEyeLashRa( TDes& aEyeLashRa, 
											TLanguage aLanguage )
	{
	aEyeLashRa.Zero();
	
	switch( aLanguage )
		{
		case ELangMarathi:
			{
			TChar virama( TAknFepUiIndicInputManager::Virama( aLanguage ) );
			aEyeLashRa.Append( 0x0931 );
			aEyeLashRa.Append( virama );
			aEyeLashRa.Append( ZERO_WIDTH_JOINER );
			}
		break;
		// Other Language
		default:
		break;
		}
	}

TBool TAknFepUiIndicInputManager::IsCharChandraA( TUint aChar )
	{
	// Since Chandra-A is specific to Marathi, we need not
	// use the Language id check.
	if ( aChar == 0x0978 )
    	{
    	return ETrue;
    	}

	return EFalse;
	}

void TAknFepUiIndicInputManager::GetChandraA( TDes& aChandraA, 
											TBool aIsToGetSingleUnicodeValue /* = EFalse */ )
	{
	aChandraA.Zero();
	
	if( aIsToGetSingleUnicodeValue )
		{
		aChandraA.Append( 0x0978 );
		}
	else
		{
		aChandraA.Append( 0x0905 );
		aChandraA.Append( 0x0945 );	
		}
	}

#endif //RD_MARATHI

#ifndef RD_INTELLIGENT_TEXT_INPUT
TBool TAknFepUiIndicInputManager::IsIndicDependantCharacter(TUint aChar, TLanguage aLanguage)
	{
	
	return (TAknFepUiIndicInputManager::IsCharModifier(aChar, aLanguage)
	 				|| TAknFepUiIndicInputManager::IsCharMatra(aChar, aLanguage)
	  				|| TAknFepUiIndicInputManager::IsCharVirama(aChar, aLanguage)
	  				|| TAknFepUiIndicInputManager::IsCharNukta(aChar, aLanguage));
	  			
	
	}
#endif //RD_INTELLIGENT_TEXT_INPUT
//End of File
