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












// CAknFepInlineTextDecorator class
//
#include <aknenv.h>

#include "AknFepInlineTextDecorator.h"
#include "AknFepManager.h"

#ifdef _DEBUG
#include "AknFepPanic.h"
#endif

void CAknFepInlineTextDecorator::DecorateInlineText(
	const TDesC& aInlineText,
	TDes& aDecoratedText,
	TInt& aPositionOfInsertionPointInInlineText,
	TChar aPreviousCharacter,
	TBool aRTLParagraph,
    TBool aInputDirectionIsRTL)
	{
	DecorateNeutralCharacters(aInlineText, aDecoratedText, aPositionOfInsertionPointInInlineText, 
                              aPreviousCharacter, aRTLParagraph, aInputDirectionIsRTL);
	}

/**
* This routine performs a consolidation of directional markers in the region around text being committed.
* It returns EFalse if no change has been made to the buffer.
*/
TBool CAknFepInlineTextDecorator::ResolveDecoratedText(
	TDes& aDecoratedTextPlusSurroundings,
	TCursorSelection aDecoratedTextSpan)
	{
	TBool needsResolving(EFalse); // Return value
	
    // Note that this is a copy of a part of the buffer and that aNewTextSpan is relative to this
    // new buffer, not the whole text buffer.
    // aNewTextSpan must fit within the descriptor; that is from 0 to length-1
    // But remember that span is a span; indices refer to positions between characters. 0 is position just before 0
    // 1 is a position between 0 and character 1.  
    __ASSERT_DEBUG(aDecoratedTextSpan.LowerPos() >= 0, 
                   AknFepPanic(EAknFepPanicCharPosOutOfRange) );
    __ASSERT_DEBUG(aDecoratedTextSpan.HigherPos() <= aDecoratedTextPlusSurroundings.Length(), 
                   AknFepPanic(EAknFepPanicCharPosOutOfRange) );
    
    // Examine the decorated text for real content
    TBool cont = EFalse;
	TInt ii = 0;
	for (ii = aDecoratedTextSpan.LowerPos(); ii < aDecoratedTextSpan.HigherPos(); ii++)
		{
		if (!CharacterIsDirectionalMarker(aDecoratedTextPlusSurroundings[ii]))
			cont = ETrue;
		}
	// if all we have entered are directional markers we want
	// to keep them so do nothing and return
	if (!cont)
		return needsResolving; // Uses initialized value of return value. Please maintain.

	ii = 0;
	while (ii < aDecoratedTextPlusSurroundings.Length())
		{
		TChar prevCh = ENullChar;
		if (ii > 0)
			prevCh = aDecoratedTextPlusSurroundings[ii-1];
		TChar ch = aDecoratedTextPlusSurroundings[ii];
		TChar nextCh = ENullChar;
		if (ii <  aDecoratedTextPlusSurroundings.Length()-1)
			nextCh = aDecoratedTextPlusSurroundings[ii+1];

		if (CharacterIsDirectionalMarker(ch))
			{
			// Remove a marker if :
			//
			// 1. previous character is strong and has the same directionality
			// 2. next character is a marker of the same type (remove both)
			// 3. next character is strong and has the same directionality
			//
			if (CharactersHaveSameStrongDirection(ch, prevCh))
				{
				aDecoratedTextPlusSurroundings.Delete(ii, ESingleCharacter);
				ii--;
				needsResolving = ETrue;
				}
			else if (CharacterIsDirectionalMarker(nextCh) && CharactersHaveSameStrongDirection(ch, nextCh))
				{
				aDecoratedTextPlusSurroundings.Delete(ii, ETwoCharacters);
				ii--;
				needsResolving = ETrue;
				}
			else if (CharactersHaveSameStrongDirection(ch, nextCh))
				{
				aDecoratedTextPlusSurroundings.Delete(ii, ESingleCharacter);
				ii--;
				needsResolving = ETrue;
				}
			}
		ii++;
		}
    return needsResolving;
	}


TBool CAknFepInlineTextDecorator::CharacterIsDirectionalMarker(TChar aChar)
	{
	return (aChar == ELeftToRightMark || aChar == ERightToLeftMark);
	}

TBool CAknFepInlineTextDecorator::CharacterIsStrongLeftToRight(TChar aChar)
	{
	return (aChar.GetBdCategory() == TChar::ELeftToRight ||
			   aChar == ELeftToRightMark);
	}

TBool CAknFepInlineTextDecorator::CharacterIsStrongRightToLeft(TChar aChar)
	{
	return ((aChar.GetBdCategory() == TChar::ERightToLeft) || 
			(aChar.GetBdCategory() == TChar::ERightToLeftArabic) ||
			aChar == ERightToLeftMark);
	}

TBool CAknFepInlineTextDecorator::CharactersHaveSameStrongDirection(TChar aCh1, TChar aCh2)
	{
	return ((CharacterIsStrongLeftToRight(aCh1) && CharacterIsStrongLeftToRight(aCh2)) ||
				(CharacterIsStrongRightToLeft(aCh1) && CharacterIsStrongRightToLeft(aCh2)));
	}

TBool CAknFepInlineTextDecorator::CharacterIsWeaklyDirectional(TChar aChar)
	{
	return ((aChar.GetBdCategory() == TChar::EEuropeanNumber) ||
			(aChar.GetBdCategory() == TChar::EEuropeanNumberSeparator) ||
			(aChar.GetBdCategory() == TChar::EEuropeanNumberTerminator) ||
			(aChar.GetBdCategory() == TChar::EArabicNumber) ||
			(aChar.GetBdCategory() == TChar::ECommonNumberSeparator));
	}

TBool CAknFepInlineTextDecorator::CharacterIsSeparator(TChar aChar)
	{
	return ((aChar.GetBdCategory() == TChar::EParagraphSeparator) || 
			(aChar.GetBdCategory() == TChar::ESegmentSeparator));
	}

TBool CAknFepInlineTextDecorator::CharacterIsNeutral(TChar aChar)
	{
	return ((aChar.GetBdCategory() == TChar::EWhitespace) || 
			(aChar.GetBdCategory() == TChar::EOtherNeutral));
	}

void CAknFepInlineTextDecorator::DecorateNeutralCharacters(
	const TDesC& aInlineText,
	TDes& aDecoratedText,
	TInt& aPositionOfInsertionPointInInlineText,
	TChar aPreviousCharacter,
	TBool aRTLParagraph,
    TBool aInputDirectionIsRTL)
	{

	TChar curCh;
	TChar prevCh = aPreviousCharacter;
	TBuf<ESingleCharacter> insertBuf(ESingleCharacter);

	for (TInt ii = 0; ii < aInlineText.Length(); ii++)
		{
		curCh = aInlineText[ii];
		aDecoratedText.Append(curCh);

		if (CharacterIsNeutral(curCh))
			{
			// mark the neutral character with appropriate direction markers if :
			//
			// 1. the input mode (lang) is opposed to the paragraph direction and 
			//     previous character is not neutral
			// 2. the input mode (lang) is opposed to the previous char direction
			//     (=> charcater is entered inside existing block)
			//
			// Note that we need to increment the insertion point by 2 if the
			// markers are inserted before it
			//
			if ((aInputDirectionIsRTL && !aRTLParagraph && !CharacterIsNeutral(prevCh)) ||
				(aInputDirectionIsRTL &&  CharacterIsStrongLeftToRight(prevCh)))
				{ // mark neutral with RLM
				insertBuf[0]=ERightToLeftMark;
				aDecoratedText.Insert(aDecoratedText.Length() -1, insertBuf);
				aDecoratedText.Append(insertBuf);
				
				if (aPositionOfInsertionPointInInlineText > ii)
					aPositionOfInsertionPointInInlineText+=ETwoCharacters;
				}

			else if ((!aInputDirectionIsRTL && aRTLParagraph && !CharacterIsNeutral(prevCh)) ||
					 (!aInputDirectionIsRTL && CharacterIsStrongRightToLeft(prevCh)))
				{ // mark neutral with LRM
				insertBuf[0]=ELeftToRightMark;
				aDecoratedText.Insert(aDecoratedText.Length() -1, insertBuf);
				aDecoratedText.Append(insertBuf);
				
				if (aPositionOfInsertionPointInInlineText > ii)
					aPositionOfInsertionPointInInlineText+=ETwoCharacters;
				}
			}
		prevCh = curCh;
		}
	}

