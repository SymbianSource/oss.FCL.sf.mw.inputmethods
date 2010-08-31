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
* Description: Keymapping data implementation for 
*
*/












#include <PtiKeyMapData.h>

#include <languages.hrh>

#include "PtiKeymappings_ITUT_29.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_TaiwanChinese[] =
    {
     0x3105,0x3106,0x3107,0x3108 ,
     0x3109,0x310A,0x310B,0x310C,
     0x310D,0x310E,0x310F,
     0x3110,0x3111,0x3112,
     0x3113,0x3114,0x3115,0x3116,
     0x3117,0x3118,0x3119,
     0x311A,0x311B,0x311C,0x311D,
     0x311E,0x311F,0x3120,0x3121,
     0x3122,0x3123,0x3124,0x3125,0x3126 ,
     0x3127,0x3128,0x3129,
     0xFF0C,
     0x3002,
     0xFF1F,
     0x0020,0x0030,0x21B2,
    };

const TInt KMultitapDataSize_TaiwanChinese = 43;


const TPtiKeyBinding MultitapKeyBindings_table_TaiwanChinese[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 4},
        {EPtiKey2, EPtiCaseLower, 4, 4},
        {EPtiKey3, EPtiCaseLower, 8, 3},
        {EPtiKey4, EPtiCaseLower, 11, 3},
        {EPtiKey5, EPtiCaseLower, 14, 4},
        {EPtiKey6, EPtiCaseLower, 18, 3},
        {EPtiKey7, EPtiCaseLower, 21, 4},
        {EPtiKey8, EPtiCaseLower, 25, 4},
        {EPtiKey9, EPtiCaseLower, 29, 5},
        {EPtiKeyStar, EPtiCaseLower, 34, 0},
        {EPtiKey0, EPtiCaseLower, 34, 3},
        {EPtiKeyHash, EPtiCaseLower, 37, 0},
        {EPtiKey1, EPtiCaseUpper, 37, 0},
        {EPtiKey2, EPtiCaseUpper, 37, 0},
        {EPtiKey3, EPtiCaseUpper, 37, 0},
        {EPtiKey4, EPtiCaseUpper, 37, 0},
        {EPtiKey5, EPtiCaseUpper, 37, 0},
        {EPtiKey6, EPtiCaseUpper, 37, 0},
        {EPtiKey7, EPtiCaseUpper, 37, 1},
        {EPtiKey8, EPtiCaseUpper, 38, 1},
        {EPtiKey9, EPtiCaseUpper, 39, 1},
        {EPtiKeyStar, EPtiCaseUpper, 40, 0},
        {EPtiKey0, EPtiCaseUpper, 40, 3},
        {EPtiKeyHash, EPtiCaseUpper, 43, 0},
    };

const TInt KMaxMultitapIndex_TaiwanChinese = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_TaiwanChinese[] =
    {
     0x2462,0x3105,0x2462,0xff01,0x2461,0x0031  ,
     0x2462,0x3109,0x2462,0xff02,0x2461,0x0032 ,
     0x2462,0x02c7,0x2462,0x0024,0x2461,0x0033   ,
     0x2462,0x02cb,0x2462,0x20ac,0x2461,0x0034   ,
     0x2462,0x3113,0x2462,0x0025,0x2461,0x0035,
     0x2462,0x02ca,0x2462,0x0026,0x2461,0x0036,
     0x2462,0x02d9,0x2462,0x002a,0x2461,0x0037,
     0x2462,0x311a,0x2462,0xff08,0x2461,0x0038,
     0x2462,0x311e,0x2462,0xff09,0x2461,0x0039,
     0x2462,0x3122,0x2461,0x0030,
     0x2462,0x002b,0x2462,0x002b,0x2461,0x002b,0x2461,0x002b ,
     0x2462,0x3106,0x2461,0x4e00,
     0x2462,0x310a,0x2461,0x4e28,
     0x2462,0x310d,0x2461,0x4e3f,
     0x2462,0x3110  ,
     0x2462,0x3114 ,
     0x2462,0x3117 ,
     0x2462,0x3127,
     0x2462,0x311b,0x2461,0x4e00 ,
     0x2462,0x311f,0x2461,0x4e28,
     0x2462,0x3123,0x2461,0x4e3f,
     0x2462,0xff0d,0x2462,0x002d,0x2461,0xff0d,0x2461,0x002d,
     0x2462,0x0023,0x2462,0x0023,0x2461,0x0023,0x2461,0x0023 ,
     0x2462,0x3107,0x2461,0x4e36,
     0x2462,0x310b,0x2461,0x4e5b  ,
     0x2462,0x310e,0x2461,0x003f,
     0x2462,0x3111,
     0x2462,0x3115,
     0x2462,0x3118,
     0x2462,0x3128,0x2461,0x003,
     0x2462,0x311c,0x2461,0x4e36,
     0x2462,0x3120,0x2461,0x4e5b,
     0x2462,0x3124,0x2462,0xff1b,0x2461,0xff1a,0x2461,0x003a,
     0x2462,0x3126,0x2462,0x0040,0x2461,0xff07,0x2461,0x0027,
     0x2462,0x3108 ,
     0x2462,0x310c ,
     0x2462,0x310f ,
     0x2462,0x3112  ,
     0x2462,0x3116,
     0x2462,0x3119,
     0x2462,0x3129,
     0x2462,0x311d,0x2462,0x003c,0x2461,0xff0c,0x2461,0x002c  ,
     0x2462,0x3121,0x2462,0x003e,0x2461,0x3002,0x2461,0x002e,
     0x2462,0x3125,0x2462,0xff1f,0x2461,0x002f,0x2461,0x002f ,
     0x2462,0x0031,0x2461,0xff01,
     0x2462,0x0032,0x2461,0xff02,
     0x2462,0x0033,0x2461,0x0024,
     0x2462,0x0034,0x2461,0x20ac,
     0x2462,0x0035,0x2461,0x0025,
     0x2462,0x0036,0x2461,0x0026,
     0x2462,0x0037,0x2461,0x002a,
     0x2462,0x0038,0x2461,0xff08,
     0x2462,0x0039,0x2461,0xff09,
     0x2462,0x0030,
     0x003d,
     0x0051,
     0x0057,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004f,
     0x0050,
     0x3001,
     0x007e,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004a,
     0x004b,
     0x004c,
     0x2462,0xff1a,0x2461,0xff1b,
     0x2462,0xff07,0x2461,0x0040,
     0x005a,
     0x0058,
     0x0043 ,
     0x0056,
     0x0042,
     0x004e,
     0x004d,
     0x2462,0xff0c,0x2461,0x003c,
     0x2462,0x3002,0x2461,0x003e,
     0x2462,0x002f,0x2461,0xff1f,
    };

const TInt KStandardQwertyDataSize_TaiwanChinese = 285;


const TPtiKeyBinding StandardQwertyKeyBindings_table_TaiwanChinese[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 6},
        {EPtiKeyQwerty2, EPtiCaseLower, 6, 6},
        {EPtiKeyQwerty3, EPtiCaseLower, 12, 6},
        {EPtiKeyQwerty4, EPtiCaseLower, 18, 6},
        {EPtiKeyQwerty5, EPtiCaseLower, 24, 6},
        {EPtiKeyQwerty6, EPtiCaseLower, 30, 6},
        {EPtiKeyQwerty7, EPtiCaseLower, 36, 6},
        {EPtiKeyQwerty8, EPtiCaseLower, 42, 6},
        {EPtiKeyQwerty9, EPtiCaseLower, 48, 6},
        {EPtiKeyQwerty0, EPtiCaseLower, 54, 4},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 58, 8},
        {EPtiKeyQwertyQ, EPtiCaseLower, 66, 4},
        {EPtiKeyQwertyW, EPtiCaseLower, 70, 4},
        {EPtiKeyQwertyE, EPtiCaseLower, 74, 4},
        {EPtiKeyQwertyR, EPtiCaseLower, 78, 2},
        {EPtiKeyQwertyT, EPtiCaseLower, 80, 2},
        {EPtiKeyQwertyY, EPtiCaseLower, 82, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 84, 2},
        {EPtiKeyQwertyI, EPtiCaseLower, 86, 4},
        {EPtiKeyQwertyO, EPtiCaseLower, 90, 4},
        {EPtiKeyQwertyP, EPtiCaseLower, 94, 4},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 98, 8},
        {EPtiKeyQwertyHash, EPtiCaseLower, 106, 8},
        {EPtiKeyQwertyA, EPtiCaseLower, 114, 4},
        {EPtiKeyQwertyS, EPtiCaseLower, 118, 4},
        {EPtiKeyQwertyD, EPtiCaseLower, 122, 4},
        {EPtiKeyQwertyF, EPtiCaseLower, 126, 2},
        {EPtiKeyQwertyG, EPtiCaseLower, 128, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 130, 2},
        {EPtiKeyQwertyJ, EPtiCaseLower, 132, 4},
        {EPtiKeyQwertyK, EPtiCaseLower, 136, 4},
        {EPtiKeyQwertyL, EPtiCaseLower, 140, 4},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 144, 8},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 152, 8},
        {EPtiKeyQwertyZ, EPtiCaseLower, 160, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 162, 2},
        {EPtiKeyQwertyC, EPtiCaseLower, 164, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 166, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 168, 2},
        {EPtiKeyQwertyN, EPtiCaseLower, 170, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 172, 2},
        {EPtiKeyQwertyComma, EPtiCaseLower, 174, 8},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 182, 8},
        {EPtiKeyQwertySlash, EPtiCaseLower, 190, 8},
        {EPtiKeyQwerty1, EPtiCaseUpper, 198, 4},
        {EPtiKeyQwerty2, EPtiCaseUpper, 202, 4},
        {EPtiKeyQwerty3, EPtiCaseUpper, 206, 4},
        {EPtiKeyQwerty4, EPtiCaseUpper, 210, 4},
        {EPtiKeyQwerty5, EPtiCaseUpper, 214, 4},
        {EPtiKeyQwerty6, EPtiCaseUpper, 218, 4},
        {EPtiKeyQwerty7, EPtiCaseUpper, 222, 4},
        {EPtiKeyQwerty8, EPtiCaseUpper, 226, 4},
        {EPtiKeyQwerty9, EPtiCaseUpper, 230, 4},
        {EPtiKeyQwerty0, EPtiCaseUpper, 234, 2},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 236, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 237, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 238, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 239, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 240, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 241, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 242, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 243, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 244, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 245, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 246, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 247, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 248, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 249, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 250, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 251, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 252, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 253, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 254, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 255, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 256, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 257, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 258, 4},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 262, 4},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 266, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 267, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 268, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 269, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 270, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 271, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 272, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 273, 4},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 277, 4},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 281, 4},
    };

const TInt KMaxStandardQwertyIndex_TaiwanChinese = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_TaiwanChinese[] =
	{
	{'*',EPtiKeyQwerty6,EPtiCaseLower},
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	{'?',EPtiKeyQwertySlash,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_TaiwanChinese = 5;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_TaiwanChinese[] =
    {
     0x2462,0x3105,
     0x2462,0x3109,
     0x2462,0x02C7,0x2461,0x4e00,
     0x2462,0x02CB,0x2461,0x4e00,
     0x2462,0x3113,0x2461,0x4e28,
     0x2462,0x02CA,0x2461,0x4e28,
     0x2462,0x02D9,0x2461,0x4e3f,
     0x2462,0x311A,0x2462,0x311B,0x2461,0x4e3f,
     0x2462,0x311E,0x2462,0x311F,
     0x2462,0x3122,0x2462,0x3123,
     0x2462,0x3106,0x2462,0x3107,
     0x2462,0x310A,0x2462,0x310B,
     0x2462,0x310D,0x2462,0x310E,0x2461,0x4e36,
     0x2462,0x3110,0x2462,0x3111,0x2461,0x4e36,
     0x2462,0x3114,0x2462,0x3115,0x2461,0x4e5b,
     0x2462,0x3117,0x2462,0x3118,0x2461,0x4e5b,
     0x2462,0x3127,0x2462,0x3128,0x2461,0x003f,
     0x2462,0x311C,0x2462,0x311D,0x2461,0x003f,
     0x2462,0x3120,0x2462,0x3121,
     0x2462,0x3108,
     0x2462,0x310C,
     0x2462,0x310F,
     0x2462,0x3112,
     0x2462,0x3116,
     0x2462,0x3119,
     0x2462,0x3129,0x2462,0x3126,
     0x2462,0x3124,0x2462,0x3125,0x2461,0xFF0C,0x2461,0x002c,
     0x2462,0x3002,0x2462,0x002e,0x2461,0x3002,0x2461,0x002e,
     0x2462,0x002F,0x2462,0x002F,0x2461,0x002F,0x2461,0x002F,
     0x2462,0x0040,0x2462,0x0040,0x2461,0x0040,0x2461,0x0040,
     0x2462,0x0020,0x2461,0x2022,
     0x2462,0x003F,0x2462,0x003F,0x2461,0x003f,0x2461,0x003f,
     0x0051,
     0x0057,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004F,
     0x0050,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x005A,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004E,
     0x004D,
     0x2462,0xFF1B,0x2462,0x003b,0x2461,0xff1b,0x2461,0x003b,
     0x2462,0xFF1A,0x2462,0x003a,0x2461,0xff1a,0x2461,0x003a,
     0x2462,0xFF07,0x2462,0x0027,0x2461,0xFF07,0x2461,0x0027,
     0x2462,0xff02,0x2462,0x0022,0x2461,0xff02,0x2461,0x0022,
     0x2462,0x005e,0x2462,0x005E,0x2461,0x005e,0x2461,0x005e,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x002A,
     0x002A,
     0x0030,
     0x0023,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x002A,
     0x002A,
     0x0030,
     0x0023,
    };

const TInt KMiniQwertyDataSize_TaiwanChinese = 256;


const TPtiKeyBinding MiniQwertyKeyBindings_table_TaiwanChinese[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 2},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 2},
        {EPtiKeyQwerty3, EPtiCaseLower, 4, 4},
        {EPtiKeyQwerty4, EPtiCaseLower, 8, 4},
        {EPtiKeyQwerty5, EPtiCaseLower, 12, 4},
        {EPtiKeyQwerty6, EPtiCaseLower, 16, 4},
        {EPtiKeyQwerty7, EPtiCaseLower, 20, 4},
        {EPtiKeyQwerty8, EPtiCaseLower, 24, 6},
        {EPtiKeyQwerty9, EPtiCaseLower, 30, 4},
        {EPtiKeyQwerty0, EPtiCaseLower, 34, 4},
        {EPtiKeyQwertyA, EPtiCaseLower, 38, 4},
        {EPtiKeyQwertyS, EPtiCaseLower, 42, 4},
        {EPtiKeyQwertyD, EPtiCaseLower, 46, 6},
        {EPtiKeyQwertyF, EPtiCaseLower, 52, 6},
        {EPtiKeyQwertyG, EPtiCaseLower, 58, 6},
        {EPtiKeyQwertyH, EPtiCaseLower, 64, 6},
        {EPtiKeyQwertyJ, EPtiCaseLower, 70, 6},
        {EPtiKeyQwertyK, EPtiCaseLower, 76, 6},
        {EPtiKeyQwertyL, EPtiCaseLower, 82, 4},
        {EPtiKeyQwertyZ, EPtiCaseLower, 86, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 88, 2},
        {EPtiKeyQwertyC, EPtiCaseLower, 90, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 92, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 94, 2},
        {EPtiKeyQwertyN, EPtiCaseLower, 96, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 98, 4},
        {EPtiKeyQwertyComma, EPtiCaseLower, 102, 8},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 110, 8},
        {EPtiKeyQwerty1, EPtiCaseUpper, 146, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 147, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 148, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 149, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 150, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 151, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 152, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 153, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 154, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 155, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 156, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 157, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 158, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 159, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 160, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 161, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 162, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 163, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 164, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 165, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 166, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 167, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 168, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 169, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 170, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 171, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 172, 8},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 180, 8},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 212, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 213, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 214, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 215, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 216, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 217, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 218, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 219, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 220, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 221, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 222, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 223, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 224, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 225, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 226, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 227, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 228, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 229, 1},
        {EPtiKeyQwerty3, EPtiCaseFnUpper, 234, 1},
        {EPtiKeyQwerty4, EPtiCaseFnUpper, 235, 1},
        {EPtiKeyQwerty5, EPtiCaseFnUpper, 236, 1},
        {EPtiKeyQwerty6, EPtiCaseFnUpper, 237, 1},
        {EPtiKeyQwerty7, EPtiCaseFnUpper, 238, 1},
        {EPtiKeyQwerty8, EPtiCaseFnUpper, 239, 1},
        {EPtiKeyQwertyD, EPtiCaseFnUpper, 240, 1},
        {EPtiKeyQwertyF, EPtiCaseFnUpper, 241, 1},
        {EPtiKeyQwertyG, EPtiCaseFnUpper, 242, 1},
        {EPtiKeyQwertyH, EPtiCaseFnUpper, 243, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnUpper, 244, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 245, 1},
        {EPtiKeyQwertyC, EPtiCaseFnUpper, 246, 1},
        {EPtiKeyQwertyV, EPtiCaseFnUpper, 247, 1},
        {EPtiKeyQwertyB, EPtiCaseFnUpper, 248, 1},
        {EPtiKeyQwertyN, EPtiCaseFnUpper, 249, 1},
        {EPtiKeyQwertyM, EPtiCaseFnUpper, 250, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnUpper, 251, 1},
    };

const TInt KMaxMiniQwertyIndex_TaiwanChinese = 92;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_TaiwanChinese[] =
    {
     0x2462,0x3105,0x2462,0x3106,0x2461,0x4e00,
     0x2462,0x3109,0x2462,0x310A,0x2461,0x4e28,
     0x2462,0x02C7,0x2461,0x4e3f,
     0x2462,0x02CB,0x2461,0x4e36,
     0x2462,0x3113,0x2462,0x3114,0x2461,0x4e5b,
     0x2462,0x02CA,0x2461,0x003f,
     0x2462,0x02D9,
     0x2462,0x311A,0x2462,0x311B,
     0x2462,0x311E,0x2462,0x311F,
     0x2462,0x3122,0x2462,0x3123,
     0x2462,0x3107,0x2462,0x3108,
     0x2462,0x310B,
     0x2462,0x310D,0x2462,0x310E,
     0x2462,0x3110,0x2462,0x3111,
     0x2462,0x3115,
     0x2462,0x3117,0x2462,0x3118,
     0x2462,0x3127,0x2462,0x3128,
     0x2462,0x311C,0x2462,0x311D,
     0x2462,0x3120,0x2462,0x3121,
     0xFF0C,0x002c,
     0x2462,0x310C,
     0x2462,0x310F,
     0x2462,0x3112,
     0x2462,0x3116,
     0x2462,0x3119,
     0x2462,0x3126,0x2462,0x3129,
     0x2462,0x3124,0x2462,0x3125,
     0x2462,0x0020,0x2461,0x2022,
     0x0051,
     0x0057,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004F,
     0x0050,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x3002,0x002e,
     0x005A,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004E,
     0x004D,
     0x0031,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030 ,
     0x0040,
     0x002f,
     0x002b,
     0x002d,
     0x003d,
     0x0023,
     0x002a,
     0x003b,
     0x003a,
     0x0027 ,
     0x0028,
     0x0029,
     0x0025,
     0x0026,
     0x0022,
     0x003f,
     0x0021,
     0x0031,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030  ,
     0x0040,
     0x002f,
     0x002b,
     0x002d,
     0x003d,
     0x0023,
     0x002a,
     0x003b,
     0x003a,
     0x0027  ,
     0x0028,
     0x0029,
     0x0025,
     0x0026,
     0x0022,
     0x003f,
     0x0021,
    };

const TInt KMiniQwerty3x11DataSize_TaiwanChinese = 182;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_TaiwanChinese[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 6},
        {EPtiKeyQwertyW, EPtiCaseLower, 6, 6},
        {EPtiKeyQwertyE, EPtiCaseLower, 12, 4},
        {EPtiKeyQwertyR, EPtiCaseLower, 16, 4},
        {EPtiKeyQwertyT, EPtiCaseLower, 20, 6},
        {EPtiKeyQwertyY, EPtiCaseLower, 26, 4},
        {EPtiKeyQwertyU, EPtiCaseLower, 30, 2},
        {EPtiKeyQwertyI, EPtiCaseLower, 32, 4},
        {EPtiKeyQwertyO, EPtiCaseLower, 36, 4},
        {EPtiKeyQwertyP, EPtiCaseLower, 40, 4},
        {EPtiKeyQwertyA, EPtiCaseLower, 44, 4},
        {EPtiKeyQwertyS, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 50, 4},
        {EPtiKeyQwertyF, EPtiCaseLower, 54, 4},
        {EPtiKeyQwertyG, EPtiCaseLower, 58, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 60, 4},
        {EPtiKeyQwertyJ, EPtiCaseLower, 64, 4},
        {EPtiKeyQwertyK, EPtiCaseLower, 68, 4},
        {EPtiKeyQwertyL, EPtiCaseLower, 72, 4},
        {EPtiKeyQwertyComma, EPtiCaseLower, 76, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 78, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 80, 2},
        {EPtiKeyQwertyC, EPtiCaseLower, 82, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 84, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 86, 2},
        {EPtiKeyQwertyN, EPtiCaseLower, 88, 4},
        {EPtiKeyQwertyM, EPtiCaseLower, 92, 4},
        {EStdKeySpace, EPtiCaseLower, 96, 4},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 119, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 123, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 124, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 127, 1},
        {EStdKeySpace, EPtiCaseUpper, 128, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 149, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 150, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 151, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 152, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 153, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 154, 1},
        {EPtiKeyQwertyQ, EPtiCaseFnUpper, 155, 1},
        {EPtiKeyQwertyW, EPtiCaseFnUpper, 156, 1},
        {EPtiKeyQwertyE, EPtiCaseFnUpper, 157, 1},
        {EPtiKeyQwertyR, EPtiCaseFnUpper, 158, 1},
        {EPtiKeyQwertyT, EPtiCaseFnUpper, 159, 1},
        {EPtiKeyQwertyY, EPtiCaseFnUpper, 160, 1},
        {EPtiKeyQwertyU, EPtiCaseFnUpper, 161, 1},
        {EPtiKeyQwertyI, EPtiCaseFnUpper, 162, 1},
        {EPtiKeyQwertyO, EPtiCaseFnUpper, 163, 1},
        {EPtiKeyQwertyP, EPtiCaseFnUpper, 164, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 165, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 166, 1},
        {EPtiKeyQwertyD, EPtiCaseFnUpper, 167, 1},
        {EPtiKeyQwertyF, EPtiCaseFnUpper, 168, 1},
        {EPtiKeyQwertyG, EPtiCaseFnUpper, 169, 1},
        {EPtiKeyQwertyH, EPtiCaseFnUpper, 170, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnUpper, 171, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 172, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 173, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnUpper, 174, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 175, 1},
        {EPtiKeyQwertyX, EPtiCaseFnUpper, 176, 1},
        {EPtiKeyQwertyC, EPtiCaseFnUpper, 177, 1},
        {EPtiKeyQwertyV, EPtiCaseFnUpper, 178, 1},
        {EPtiKeyQwertyB, EPtiCaseFnUpper, 179, 1},
        {EPtiKeyQwertyN, EPtiCaseFnUpper, 180, 1},
        {EPtiKeyQwertyM, EPtiCaseFnUpper, 181, 1},
    };

const TInt KMaxMiniQwerty3x11Index_TaiwanChinese = 110;

//
// HalfQwerty keymap data table
//
const TUint16 HalfQwertyData_table_TaiwanChinese[] =
    {
     0x0071,0x0077,
     0x0065,0x0072,0x2461,0x4e00,
     0x0074,0x0079,0x2461,0x4e28,
     0x0075,0x0069,0x2461,0x4e3f,
     0x006f,0x0070,
     0x0061,0x0073,
     0x0064,0x0066,0x2461,0x4e36,
     0x0067,0x0068,0x2461,0x4e5b,
     0x006a,0x006b,0x2461,0x003f,
     0x006c,
     0x007a,0x0078,
     0x0063,0x0076,
     0x0062,0x006e,
     0x006d,
     0x3002,0xFF0C,0xFF1A,0xFF1B,
     0x0020,
     0x005e,
     0x0051,0x0057,
     0x0045,0x0052,
     0x0054,0x0059,
     0x0055,0x0049,
     0x004f,0x0050,
     0x0041,0x0053,
     0x0044,0x0046,
     0x0047,0x0048,
     0x004a,0x004b,
     0x004c,
     0x005a,0x0058,
     0x0043,0x0056,
     0x0042,0x004e,
     0x004d,
     0x3002,0xFF0C,0xFF1A,0xFF1B,
     0x0020,
     0x005e,
     0xFF01,
     0x0031,
     0x0032,
     0x0033,
     0x002d,
     0xFF1F,
     0x0034,
     0x0035,
     0x0036,
     0x0027,
     0x0040,
     0x0037,
     0x0038,
     0x0039,
     0xFF1A, 0xFF1B, 0x3002, 0xFF0C,
     0x002a, 0x002b,
     0x0030,
     0x0023, 0x002f,
     0xFF01,
     0x0031,
     0x0032,
     0x0033,
     0x002d,
     0xFF1F,
     0x0034,
     0x0035,
     0x0036,
     0x0027,
     0x0040,
     0x0037,
     0x0038,
     0x0039,
     0xFF1A, 0xFF1B, 0x3002, 0xFF0C,
     0x002a, 0x002b,
     0x0030,
     0x0023, 0x002f,
    };

const TInt KHalfQwertyDataSize_TaiwanChinese = 122;


const TPtiKeyBinding HalfQwertyKeyBindings_table_TaiwanChinese[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 2},
        {EPtiKeyQwerty1, EPtiCaseLower, 2, 4},
        {EPtiKeyQwerty2, EPtiCaseLower, 6, 4},
        {EPtiKeyQwerty3, EPtiCaseLower, 10, 4},
        {EPtiKeyQwertyO, EPtiCaseLower, 14, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 16, 2},
        {EPtiKeyQwerty4, EPtiCaseLower, 18, 4},
        {EPtiKeyQwerty5, EPtiCaseLower, 22, 4},
        {EPtiKeyQwerty6, EPtiCaseLower, 26, 4},
        {EPtiKeyQwertyL, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 31, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 33, 2},
        {EPtiKeyQwerty8, EPtiCaseLower, 35, 2},
        {EPtiKeyQwerty9, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 38, 4},
        {EPtiKeyQwertyChr, EPtiCaseLower, 42, 0},
        {EPtiKeyQwertySpace, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 44, 2},
        {EPtiKeyQwerty1, EPtiCaseUpper, 46, 2},
        {EPtiKeyQwerty2, EPtiCaseUpper, 48, 2},
        {EPtiKeyQwerty3, EPtiCaseUpper, 50, 2},
        {EPtiKeyQwertyO, EPtiCaseUpper, 52, 2},
        {EPtiKeyQwertyA, EPtiCaseUpper, 54, 2},
        {EPtiKeyQwerty4, EPtiCaseUpper, 56, 2},
        {EPtiKeyQwerty5, EPtiCaseUpper, 58, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 60, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 63, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 65, 2},
        {EPtiKeyQwerty8, EPtiCaseUpper, 67, 2},
        {EPtiKeyQwerty9, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 70, 4},
        {EPtiKeyQwertyChr, EPtiCaseUpper, 74, 0},
        {EPtiKeyQwertySpace, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 86, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 87, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 88, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 89, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 90, 4},
        {EPtiKeyQwertyChr, EPtiCaseFnLower, 94, 2},
        {EPtiKeyQwertySpace, EPtiCaseFnLower, 96, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseFnLower, 97, 2},
        {EPtiKeyQwertyQ, EPtiCaseFnUpper, 99, 1},
        {EPtiKeyQwerty1, EPtiCaseFnUpper, 100, 1},
        {EPtiKeyQwerty2, EPtiCaseFnUpper, 101, 1},
        {EPtiKeyQwerty3, EPtiCaseFnUpper, 102, 1},
        {EPtiKeyQwertyO, EPtiCaseFnUpper, 103, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 104, 1},
        {EPtiKeyQwerty4, EPtiCaseFnUpper, 105, 1},
        {EPtiKeyQwerty5, EPtiCaseFnUpper, 106, 1},
        {EPtiKeyQwerty6, EPtiCaseFnUpper, 107, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 108, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 109, 1},
        {EPtiKeyQwerty7, EPtiCaseFnUpper, 110, 1},
        {EPtiKeyQwerty8, EPtiCaseFnUpper, 111, 1},
        {EPtiKeyQwerty9, EPtiCaseFnUpper, 112, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnUpper, 113, 4},
        {EPtiKeyQwertyChr, EPtiCaseFnUpper, 117, 2},
        {EPtiKeyQwertySpace, EPtiCaseFnUpper, 119, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseFnUpper, 120, 2},
    };

const TInt KMaxHalfQwertyIndex_TaiwanChinese = 72;

//
// CustomQwerty keymap data table
//
const TUint16 CustomQwertyData_table_TaiwanChinese[] =
    {
     0x0061,
     0x0062 ,
     0x0063,
     0x0064,
     0x0065,
     0x0066,
     0x0067,
     0x0068,
     0x0069,
     0x006A,
     0x006B,
     0x006C,
     0x0031 ,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030,
     0x003f,
     0x003e,
     0x0041,
     0x0042,
     0x0043,
     0x0044,
     0x0045,
     0x0046,
     0x0047,
     0x0048,
     0x0049,
     0x004A,
     0x004B,
     0x004C,
     0x0021 ,
     0x0022,
     0x0023,
     0x0024,
     0x0025,
     0x0026,
     0x0027,
     0x0028,
     0x0029,
     0x002A,
     0x002B,
     0x002C,
    };

const TInt KCustomQwertyDataSize_TaiwanChinese = 48;


const TPtiKeyBinding CustomQwertyKeyBindings_table_TaiwanChinese[] =
    {
        { EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        { EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        { EPtiKeyQwerty3, EPtiCaseLower, 2, 1},
        { EPtiKeyQwerty4, EPtiCaseLower, 3, 1},
        { EPtiKeyQwerty5, EPtiCaseLower, 4, 1},
        { EPtiKeyQwerty6, EPtiCaseLower, 5, 1},
        { EPtiKeyQwerty7, EPtiCaseLower, 6, 1},
        { EPtiKeyQwerty8, EPtiCaseLower, 7, 1},
        { EPtiKeyQwerty9, EPtiCaseLower, 8, 1},
        { EPtiKeyQwerty0, EPtiCaseLower, 9, 1},
        { EPtiKeyQwerty1, EPtiCaseUpper, 24, 1},
        { EPtiKeyQwerty2, EPtiCaseUpper, 25, 1},
        { EPtiKeyQwerty3, EPtiCaseUpper, 26, 1},
        { EPtiKeyQwerty4, EPtiCaseUpper, 27, 1},
        { EPtiKeyQwerty5, EPtiCaseUpper, 28, 1},
        { EPtiKeyQwerty6, EPtiCaseUpper, 29, 1},
        { EPtiKeyQwerty7, EPtiCaseUpper, 30, 1},
        { EPtiKeyQwerty8, EPtiCaseUpper, 31, 1},
        { EPtiKeyQwerty9, EPtiCaseUpper, 32, 1},
        { EPtiKeyQwerty0, EPtiCaseUpper, 33, 1},
    };

const TInt KMaxCustomQwertyIndex_TaiwanChinese = 20;

//
// Class implementation.
//

CPtiLangDataImplTaiwanChinese* CPtiLangDataImplTaiwanChinese::NewL()
    {
    return new (ELeave) CPtiLangDataImplTaiwanChinese();
    }


TInt CPtiLangDataImplTaiwanChinese::LanguageCode() const
    {
    return ELangTaiwanChinese;
    }


const TUint16* CPtiLangDataImplTaiwanChinese::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_TaiwanChinese;
        return MultitapData_table_TaiwanChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_TaiwanChinese;
        return StandardQwertyData_table_TaiwanChinese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_TaiwanChinese;
        return MiniQwertyData_table_TaiwanChinese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_TaiwanChinese;
        return MiniQwerty3x11Data_table_TaiwanChinese;
        }

    if (aKeyboardType == EPtiKeyboardHalfQwerty)
        {
        aDataSize = KHalfQwertyDataSize_TaiwanChinese;
        return HalfQwertyData_table_TaiwanChinese;
        }

    if (aKeyboardType == EPtiKeyboardCustomQwerty)
        {
        aDataSize = KCustomQwertyDataSize_TaiwanChinese;
        return CustomQwertyData_table_TaiwanChinese;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplTaiwanChinese::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_TaiwanChinese;
        return MultitapKeyBindings_table_TaiwanChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_TaiwanChinese;
        return StandardQwertyKeyBindings_table_TaiwanChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_TaiwanChinese;
        return MiniQwertyKeyBindings_table_TaiwanChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_TaiwanChinese;
        return MiniQwerty3x11KeyBindings_table_TaiwanChinese;
        }
    if (aKeyboardType == EPtiKeyboardHalfQwerty)
        {
        aNumItems = KMaxHalfQwertyIndex_TaiwanChinese;
        return HalfQwertyKeyBindings_table_TaiwanChinese;
        }
    if (aKeyboardType == EPtiKeyboardCustomQwerty)
        {
        aNumItems = KMaxCustomQwertyIndex_TaiwanChinese;
        return CustomQwertyKeyBindings_table_TaiwanChinese;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplTaiwanChinese::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_TaiwanChinese;
        return StandardQwertyNumericKeysData_table_TaiwanChinese;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplTaiwanChinese::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


