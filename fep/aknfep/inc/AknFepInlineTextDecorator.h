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












#ifndef _AKNFEP_INLINE_TEXT_DECORATOR_H__
#define _AKNFEP_INLINE_TEXT_DECORATOR_H__

#include <e32base.h>

class CAknFepInlineTextDecorator : public CBase
	{
public:
	enum
		{
		ELeftToRightMark = 0x200E,		// LRM
		ERightToLeftMark = 0x200F		// RLM
		};
private:
	enum
		{
		ENullChar = 0,
		ESingleCharacter = 1,
		ETwoCharacters = 2
		};

public:
	/**
	* Applies required markup to a piece of bidirectional text to ensure correct
	* rendering for Averell
	* 
	* @param aInlineText original inline text
	* @param aDecoratedText descriptor which will hold the original text plus
	*				any decoration
	* @param aPositionOfInsertionPointInInlineText position of insertion point 
	*				(cursor) in the original inline text
	* @param aPreviousCharacter the character immediately prior to the inline
	*				text
	* @param aRTLParagraph set to true if the inline text is being inserted into
	*				a paragraph with RTL directionality in the CParaFormat
	* @param aInputDirectionIsRtl is to be set to ETrue if the current input direction is RTL
	*/
	void DecorateInlineText(
		const TDesC& aInlineText, 
		TDes& aDecoratedText,
		TInt& aPositionOfInsertionPointInInlineText,
		TChar aPreviousCharacter,
		TBool aRTLParagraph,
        TBool aInputDirectionIsRTL);
	
	/**
	* Resolves any decorated text + any surrounding text that has been added to the beginning
    * or end to remove unnecessary directional markers.
	*
	* @param aDecoratedTextPlusSurroundings descriptor to hold a piece of text to be parsed
	*				and resolved; new text, optionally with accreted surrounding text
	* @param aDecoratedTextSpan span of text originally inspected and decorated (must be subset of descriptor)
	* 
	* @return   EFalse if and only if no change was made to aDecoratedTextPlusSurroundings 
	*/
	TBool ResolveDecoratedText(
		TDes& aDecoratedTextPlusSurroundings,
		TCursorSelection aDecoratedTextSpan);

	/**
	* Determines if a character is an LRM (0x200e) or an RLM (0x200f)
	* 
	* @param aChar the character to be inspected
	* @return ETrue if the character has the value 0x200e or 0x200f
	*/
	TBool CharacterIsDirectionalMarker(TChar aChar);

	/**
	* Determines if a character has strong LTR directionality
	* 
	* @param	aChar the character to be inspected
	* @return	ETrue if the character has bidi category of TChar::ELeftToRight
	*				or is an LRM
	*/
	TBool CharacterIsStrongLeftToRight(TChar aChar);

	/**
	* Determines if a character has strong RTL directionality
	* 
	* @param	aChar the character to be inspected
	* @return	ETrue if the character has bidi category of TChar::ERightToLeft
	*				or is an RLM
	*/
	TBool CharacterIsStrongRightToLeft(TChar aChar);

	/**
	* Determines if two characters share the same strong directionality
	* 
	* @param aCh1 the first character to be compared
	* @param aCh2 the second character to be compared
	* @return ETrue if aCh1 and aCh2 share the same strong directionality
	*/
	TBool CharactersHaveSameStrongDirection(TChar aCh1, TChar aCh2);

	/**
	* Determines if a character has weak directionality
	* 
	* @param	aChar the character to be inspected
	* @return	ETrue if the character has any of the following bidi categories
	*					TChar::EEuropeanNumber
	*					TChar::EEuropeanNumberSeparator
	*					TChar::EEuropeanNumberTerminator
	*					TChar::EArabicNumber
	*					TChar::ECommonNumberSeparator
	*/
	TBool CharacterIsWeaklyDirectional(TChar aChar);

	/**
	* Determines if a character is a separator character
	* 
	* @param	aChar the character to be inspected
	* @return	ETrue if the character has any of the following bidi categories
	*					TChar::EParagraphSeparator
	*					TChar::ESegmentSeparator
	*/
	TBool CharacterIsSeparator(TChar aChar);

	/**
	* Determines if a character has neutral directionlaity
	* 
	* @param	aChar the character to be inspected
	* @return	ETrue if the character has any of the following bidi categories
	*					TChar::EWhitespace
	*					TChar::EOtherNeutral
	*/
	TBool CharacterIsNeutral(TChar aChar);

private:
	void DecorateNeutralCharacters(
		const TDesC& aInlineText, 
		TDes& aDecoratedText,
		TInt& aPositionOfInsertionPointInInlineText,
		TChar aPreviousCharacter,
		TBool aRTLParagraph,
        TBool aInputDirectionIsRTL);
	};

#endif
