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
*       Provides global enumerations.
*
*/











#ifndef __AKN_FEP_UI_INDIC_ENUMS_H__
#define __AKN_FEP_UI_INDIC_ENUMS_H__

enum TIndicInputResponse
	{
	/*causes the default input.  New Case will rather be applied*/
	EIndicInputResponseNone, 
	/*forces the input to the number.  This will happen in case we 
	try to add modifier without the preceeding consonant being a vowel, 
	consonant or a modified consonant*/
	EIndicInputResponseNumber,
	/*When VIRAMA is inserted in the already entered text between two consonants, 
	then to we need to insert ZWS (Zero Width Space U+200B) so as to prevent 
	the incorrect joining*/ 
	EIndicInputResponseInsertZWS,
	/*When ligature is formed while editing in middle of the word with the help of "*" key 
	we need to handle it so that it should not form a ligature and a explicit halant 
	is  entered in the text*/
	EIndicInputResponseInsertZWSandLigature,
	/*causes the user to input ligature by explicitly appending VIRAMA*/
	EIndicInputResponseZWSandCharacter,
	/*an error input.  not currently used but can be used later*/
	EIndicInputResponseIgnore,	
	/*Response for Repha insertion*/
	EIndicInputResponseInsertRepha,	
	/*Response for Rakar insertion*/
	EIndicInputResponseInsertRakar,	
	/*Response for ligature insertion*/
	EIndicInputResponseInsertDirectLigature,
	/*Response for Virama, ZWS and ligature insertion*/
	EIndicInputResponseInsertViramaZWSandDirectLigature,
	/*Response for ZWS and ligature insertion*/
	EIndicInputResponseZWSandDirectLigature,
	/*Response for invalid input*/
	EIndicInputResponseInvalid,
#ifdef RD_HINDI_PHONETIC_INPUT	
	/*causes the inputting of transliterated text returned by PtiEngine in Multitap mode*/
	EIndicInputResponsePhoneticMultitapText,
	/*causes the inputting of transliterated text returned by PtiEngine in Qwerty mode*/
	EIndicInputResponsePhoneticQwertyText,
#endif

#ifdef RD_MARATHI
	/*Response for Eye Lash Ra insertion*/
	EIndicInputResponseInsertEyeLashRa,	
	/*Response for Chandra A insertion*/
	EIndicInputResponseInsertChandraA, 
#endif // RD_MARATHI

	/* not used*/
	EindicInputResponseLast
	};
	
enum TIndicBaseCharacter
	{
	EIndicBaseVowel,
	EIndicBaseConsonant	
	};
//Changing ZWS to ZWNJ for time being, Latter the variable names need to be changed.		
#define ZERO_WIDTH_SPACE 0x200C	
#define MAX_SYLABLE_BOUNDRY 12
#define ZERO_WIDTH_JOINER 0x200D
#endif //__AKN_FEP_UI_INDIC_ENUMS_H__
// End of file
