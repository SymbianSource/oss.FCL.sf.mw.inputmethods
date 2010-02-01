/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr
*
*/


#ifndef PENINPUTHWRFSCNSTORECONSTANTS_H
#define PENINPUTHWRFSCNSTORECONSTANTS_H

// writing speed definition
enum TPeninputHwrfscnWritingSpeed
    {
    EHwrWritingSpeedVeryFast = 300*1000,
    EHwrWritingSpeedFast = 400*1000,
    EHwrWritingSpeedNormal = 550*1000,
    EHwrWritingSpeedSlow = 700*1000,
    EHwrWritingSpeedVerySlow = 1000*1000
    };
    
//the predict symbol unicode when the primary is the english
const TUint16 EnglishPredictSymCode[10] =
	{
	0x002E,
	0x002C,
	0x003A,
	0x0022,
	0x003F,
	0x0021,
	0x002D,
	0x0027,
	0xF800,
	0xF801	
	};
//the predict symbol unicode when the primary is the number
/*
const TUint16 NumberPredictSymCode[9] =
	{
	0x002E,
	0x002C,
	0x005E,
	0x0025,
	0x0040,
	0x002B,
	0x002D,
	0x002F,
	0x002A	
	};
*/
	
const TUint16 NumberPredictSymCode[10] =
	{
	0x0030,
	0x0031,
	0x0032,
	0x0033,
	0x0034,
	0x0035,
	0x0036,
	0x0037,
	0x0038,
	0x0039	
	};
	
		
// the default end mark    
const TInt KDefaultStrokeEndMarkX = -65536;
const TInt KDefaultStrokeEndMarkY = -65536;

//candiate number
const TInt KCandidateCount = 27;
const TInt KMaxHomophonicCandidateCount = 100;
const TInt KMaxPredictiveCount = 200;
const TInt KPredictiveCountPerPage = 36;
const TInt KHomophonicCountPerPage = 36;
const TInt KPremaryCandidateCount = 20;

const TInt KAuxCandidateCount = 3;
const TInt KAuxCandidateCountNumMode = 1;
const TInt KCandidateRowCount = 3;
const TInt KCandidateCountPerRow = 9;
const TInt KCandidateCountNumMode = 10;
const TInt KMaxCandidateWinRowCount = 4;
const TInt KCandidateListCount = 2;


const TInt KSpecialConvertedCharCount = 10;
//one spelling number
const TInt KMaxOneSpellingLettersNum = 6;
//4 means 4 tones
const TInt KMaxSpellingsLettersNum = 4*KMaxOneSpellingLettersNum;
//max number string
const TInt KMaxNumericString = 32;

const TInt KPlainNumSize = 11;

const TInt KNumberCount = 10;

const TUint16 KSegment = 0x0020;

//category
_LIT(KCategory, "AKNFEPHWR");
//the number string
_LIT(KNumberString,"0123456789");
_LIT(KSeparator,"\0");
_LIT(KDash,"-");
_LIT(KSolidus,"/");
_LIT(KPolyphonicSeparator, "\x0A42");
_LIT(KGestureBackspace, "\x2408");
_LIT(KDisplayBackspace, "\x2190");
_LIT(KGestureSpace, "\xF800");
_LIT(KGestureEnter, "\xF801");

_LIT(KSmileFontCode, "\x263A");
_LIT(KCryFontCode, "\x2639");
_LIT(KColon, "\x003A");
_LIT(KRightParenthesis, "\x0029");
_LIT(KLeftParenthesis, "\x0028");

#endif //end PENINPUTHWRFSCNSTORECONSTANTS_H