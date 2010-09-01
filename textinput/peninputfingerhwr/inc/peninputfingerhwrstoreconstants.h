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
* Description:  const definitions for finger hwr
*
*/

#ifndef PENINPUTFINGERHWRSTORECONSTANTS_H
#define PENINPUTFINGERHWRSTORECONSTANTS_H

/**
 *  writing speed definition
 * 
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
enum TPeninputHwrfscnWritingSpeed
    {
    EHwrWritingSpeedVeryFast = 300*1000,
    EHwrWritingSpeedFast = 400*1000,
    EHwrWritingSpeedNormal = 550*1000,
    EHwrWritingSpeedSlow = 700*1000,
    EHwrWritingSpeedVerySlow = 1000*1000
    };
    
/**
 *  
 * 
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
enum TPeninputHwrfscnStateMgrCmd
    {
    ERecgnizeCmd,
    EPredictiveCmd,
    ESetTriggerStringCmd,
    EAppendTriggerStringCmd
    };

/**
 *  char range id definition
 * 
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
enum TFingerHwrRange
    {
    EFingerHwrNativeRange,
    EFingerHwrEnglishRange,
    EFingerHwrNumberRange,
    EFingerHwrSymbolRange
    };    

/**
 *  pop directions definition
 * 
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
enum TPeninputPopListDirection
    {
    EPopDirAuto = 0,
    EPopDirLeft,
    EPopDirRight,
    EPopDirTop,
    EPopDirDown
    };

/**
 *  row index definition
 * 
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
enum TRowCount
    {
    ERowOne,
    ERowTwo
    };
    
// the predict symbol unicode when the primary is the english
const TUint16 EnglishSymbolCodeOneRow[6] =
	{
	0x0027,     // '
	0x002C,     // ,
	0x002E,     // .
	0x003F,     // ?
	0x0021,     // !	
#ifdef RD_INTELLIGENT_TEXT_INPUT
    0x002D	    // -
#else
    0xF800	    // space
#endif
	};


const TUint16 EnglishSymbolCodeTwoRow[6] =
	{	
	0x0040,     // @
	0x003A,     // :
	0x003B,     // ;
    0x0029,     // )
	0x0044,     // D
	0x0028	    // (
	};

const TUint16 ColonSymbol = 0x003A;


const TUint16 ChineseSymbolCode[6] =
	{
	0xFF0C,
	0x3002,
	0xFF1F,
	0xFF01,
	0x3001,
	0xFF1A	
	};
	

const TUint16 ChinesePredictCode[100] =
	{
	0x6211, 0x4F60, 0x7684, 0x4E86, 0xFF0C, 0x4E0D, 0x597D, 0x5728, 0x662F, 0x6709, 
	0x3002, 0x4E00, 0x4E48,	0x53BB,	0x5427,	0x5929, 0x4E0A, 0xFF1F, 0x6765, 0x6CA1,
	0x554A, 0x5230, 0x5417,	0xFF01, 0x4EEC, 0x5C31, 0x56DE, 0x8FD8, 0x90A3, 0x8981, 
    0x4E0B, 0x5BB6, 0x4E2A, 0x65F6, 0x70B9, 0x8FD9, 0x4E5F, 0x8FC7, 0x7ED9, 0x770B, 
    0x4EC0, 0x4F1A, 0x4EBA, 0x8BF4, 0x53EF, 0x5927, 0x4ED6, 0x4E8B, 0x591A, 0x5B66, 
    0x60F3, 0xFF1A, 0x80FD, 0x4EE5, 0x73B0, 0x90FD, 0x9053, 0x77E5, 0x660E, 0x5F97, 
    0x8BDD, 0x5403, 0x5B50, 0x53D1, 0x91CC, 0x5462, 0x548C, 0x51FA, 0x6253, 0x4ECA, 
    0x884C, 0x5C0F, 0x7535, 0x7B49, 0x6837, 0x5FC3, 0x8D77, 0x7740, 0x540E, 0x600E, 
    0x8001, 0x5F88, 0x518D, 0x628A, 0x51E0, 0x7528, 0x5F00, 0x4E2D, 0x751F, 0x95EE, 
    0x95F4, 0x5979, 0x3001, 0x513F, 0x5730, 0x5BF9, 0x81EA, 0x89C1, 0x4E3A, 0x5DF2
	};		
	
	
	
		
// the default end mark    
const TInt KDefaultStrokeEndMarkX = -65536;
const TInt KDefaultStrokeEndMarkY = -65536;

//candiate number
const TInt KCandidateCount = 6;
const TInt KMaxPredictiveCount = 100;
const TInt KPredictiveCountPerPage = 20;
const TInt KPremaryCandidateCount = 10;

const TInt KCandidateRowCount = 1;
const TInt KPredictiveRowCount = 3;
const TInt KCandidateCountPerRow = 5;
const TInt KPredictiveCountPerRow = 5;


const TInt KSpecialConvertedCharCount = 2;

//max number string
const TInt KMaxNumericString = 32;


const TInt KNumberCount = 10;

const TUint16 KSegment = 0x0020;

const TInt32 KInvalidResId = -1;
const TInt KInvalidImg = -1 ;
const TInt KNotSupportSkin = -1;
const TInt KInvalidIndex = -1;

//category
_LIT(KCategory, "AKNFEPHWR");

//the number string
_LIT(KNumberString,"0123456789");

//special chars
_LIT(KSeparator,"\0");
_LIT(KDash,"-");
_LIT(KSolidus,"/");
_LIT(KPolyphonicSeparator, "\x0A42");
_LIT(KGestureBackspace, "\x2408");
_LIT(KDisplayBackspace, "\x2190");

_LIT(KGestureSpace, "\xF800");
_LIT(KDisplaySpace, "\x0020");
_LIT(KGestureEnter, "\xF801");

_LIT(KColon, "\x003A");


#endif //PENINPUTFINGERHWRSTORECONSTANTS_H