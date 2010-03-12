/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  const definitions for finger hwrar
*
*/


#ifndef PENINPUTFINGERHWRARSTORECONSTANTS_H
#define PENINPUTFINGERHWRARSTORECONSTANTS_H

/**
 *  writing speed definition
 * 
 *  @lib peninputfingerhwrar.lib
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
 *  char range id definition
 * 
 *  @lib peninputfingerhwrar.lib
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
 *  @lib peninputfingerhwrar.lib
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

enum TPagePosition
    {
    EPagePosPageNo,
    EPagePosNextPage,
    EPagePosPrevPage,
    EPagePosLastPage,
    EPagePosFirstPage,
    };
		
// the default end mark    
const TInt KDefaultStrokeEndMarkX = -65536;
const TInt KDefaultStrokeEndMarkY = -65536;

//candiate number
const TInt KCandidateCount = 6;
const TInt KPremaryCandidateCount = 10;

const TInt KCandidateRowCount = 2;
const TInt KCandidateCountPerRow = 5;



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
_LIT(KCategory, "AKNFEPHWRAR");

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


#endif //PENINPUTFINGERHWRARSTORECONSTANTS_H
