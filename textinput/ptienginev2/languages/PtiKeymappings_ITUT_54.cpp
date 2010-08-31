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

#include "PtiKeymappings_ITUT_54.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Greek[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x3b1,0x3b2,0x3b3,0x3ac,0x32,0x61,0x62,0x63,
     0x3b4,0x3b5,0x3b6,0x3ad,0x33,0x64,0x65,0x66,
     0x3b7,0x3b8,0x3b9,0x3ae,0x3af,0x3ca,0x390,0x34,0x67,0x68,0x69,
     0x3ba,0x3bb,0x3bc,0x35,0x6a,0x6b,0x6c,
     0x3bd,0x3be,0x3bf,0x3cc,0x36,0x6d,0x6e,0x6f,
     0x3c0,0x3c1,0x3c3,0x3c2,0x37,0x70,0x71,0x72,0x73,
     0x3c4,0x3c5,0x3c6,0x3cd,0x3cb,0x3b0,0x38,0x74,0x75,0x76,
     0x3c7,0x3c8,0x3c9,0x3ce,0x39,0x77,0x78,0x79,0x7a,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x391,0x392,0x393,0x386,0x32,0x41,0x42,0x43,
     0x394,0x395,0x396,0x388,0x33,0x44,0x45,0x46,
     0x397,0x398,0x399,0x389,0x38a,0x3aa,0x34,0x47,0x48,0x49,
     0x39a,0x39b,0x39c,0x35,0x4a,0x4b,0x4c,
     0x39d,0x39e,0x39f,0x38c,0x36,0x4d,0x4e,0x4f,
     0x3a0,0x3a1,0x3a3,0x37,0x50,0x51,0x52,0x53,
     0x3a4,0x3a5,0x3a6,0x38e,0x3ab,0x38,0x54,0x55,0x56,
     0x3a7,0x3a8,0x3a9,0x38f,0x39,0x57,0x58,0x59,0x5a,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Greek = 203;


const TPtiKeyBinding MultitapKeyBindings_table_Greek[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 8},
        {EPtiKey3, EPtiCaseLower, 38, 8},
        {EPtiKey4, EPtiCaseLower, 46, 11},
        {EPtiKey5, EPtiCaseLower, 57, 7},
        {EPtiKey6, EPtiCaseLower, 64, 8},
        {EPtiKey7, EPtiCaseLower, 72, 9},
        {EPtiKey8, EPtiCaseLower, 81, 10},
        {EPtiKey9, EPtiCaseLower, 91, 9},
        {EPtiKeyStar, EPtiCaseLower, 100, 0},
        {EPtiKey0, EPtiCaseLower, 100, 3},
        {EPtiKeyHash, EPtiCaseLower, 103, 0},
        {EPtiKey1, EPtiCaseUpper, 103, 30},
        {EPtiKey2, EPtiCaseUpper, 133, 8},
        {EPtiKey3, EPtiCaseUpper, 141, 8},
        {EPtiKey4, EPtiCaseUpper, 149, 10},
        {EPtiKey5, EPtiCaseUpper, 159, 7},
        {EPtiKey6, EPtiCaseUpper, 166, 8},
        {EPtiKey7, EPtiCaseUpper, 174, 8},
        {EPtiKey8, EPtiCaseUpper, 182, 9},
        {EPtiKey9, EPtiCaseUpper, 191, 9},
        {EPtiKeyStar, EPtiCaseUpper, 200, 0},
        {EPtiKey0, EPtiCaseUpper, 200, 3},
        {EPtiKeyHash, EPtiCaseUpper, 203, 0},
    };

const TInt KMaxMultitapIndex_Greek = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Greek[] =
    {
     0x31,
     0x32,
     0x33,
     0x34,
     0x35,
     0x36,
     0x37,
     0x38,
     0x39,
     0x30,
     0x3b,
     0x387,
     0x3c2,
     0x3b5,0x3ad,
     0x3c1,
     0x3c4,
     0x3c5,0x3cd,0x3cb,0x3b0,
     0x3b8,
     0x3b9,0x3af,0x3ca,0x390,
     0x3bf,0x3cc,
     0x3c0,
     0x2d,
     0x2b,
     0x3b1,0x3ac,
     0x3c3,
     0x3b4,
     0x3c6,
     0x3b3,
     0x3b7,0x3ae,
     0x3be,
     0x3ba,
     0x3bb,
     0xf001,0x384,
     0xab,
     0x3b6,
     0x3c7,
     0x3c8,
     0x3c9,0x3ce,
     0x3b2,
     0x3bd,
     0x3bc,
     0x2c,
     0x2e,
     0x2f,
     0x21,
     0x40,
     0x23,
     0x24,
     0x25,
     0x20ac,
     0x26,
     0x5c,
     0x28,
     0x29,
     0x3a,
     0x387,
     0xf003,0x385,
     0x395,0x388,
     0x3a1,
     0x3a4,
     0x3a5,0x38e,0x3ab,
     0x398,
     0x399,0x38a,0x3aa,
     0x39f,0x38c,
     0x3a0,
     0x3d,
     0x2a,
     0x391,0x386,
     0x3a3,
     0x394,
     0x3a6,
     0x393,
     0x397,0x389,
     0x39e,
     0x39a,
     0x39b,
     0xf002,0xa8,
     0xbb,
     0x396,
     0x3a7,
     0x3a8,
     0x3a9,0x38f,
     0x392,
     0x39d,
     0x39c,
     0x3c,
     0x3e,
     0x3f,
    };

const TInt KStandardQwertyDataSize_Greek = 111;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Greek[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 1},
        {EPtiKeyQwerty4, EPtiCaseLower, 3, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 4, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 5, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 6, 1},
        {EPtiKeyQwerty8, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty9, EPtiCaseLower, 8, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 9, 1},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyQ, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 13, 2},
        {EPtiKeyQwertyR, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 17, 4},
        {EPtiKeyQwertyU, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 22, 4},
        {EPtiKeyQwertyO, EPtiCaseLower, 26, 2},
        {EPtiKeyQwertyP, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 31, 2},
        {EPtiKeyQwertyS, EPtiCaseLower, 33, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyJ, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 42, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 55, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 68, 2},
        {EPtiKeyQwertyE, EPtiCaseUpper, 70, 2},
        {EPtiKeyQwertyR, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 74, 3},
        {EPtiKeyQwertyU, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 78, 3},
        {EPtiKeyQwertyO, EPtiCaseUpper, 81, 2},
        {EPtiKeyQwertyP, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 86, 2},
        {EPtiKeyQwertyS, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 92, 2},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 97, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 103, 2},
        {EPtiKeyQwertyB, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 110, 1},
    };

const TInt KMaxStandardQwertyIndex_Greek = 88;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Greek[] =
    {
    13,  0x03AD,0xF000,0xF000,0xf000,0xf000,
    16,  0x03CD,0x03CB,0x03B0,0xf000,0xf000,
    18,  0x03AF,0x03CA,0x0390,0xf000,0xf000,
    19,  0x03CC,0xF000,0xF000,0xf000,0xf000,
    23,  0x03AC,0xF000,0xF000,0xf000,0xf000,
    28,  0x03AE,0xF000,0xF000,0xf000,0xf000,
    37,  0x03CE,0xF000,0xF000,0xf000,0xf000,
    57,  0x0388,0xF000,0xF000,0xf000,0xf000,
    60,  0x038E,0x03AB,0xF000,0xf000,0xf000,
    62,  0x038A,0x03AA,0xF000,0xf000,0xf000,
    63,  0x038C,0xF000,0xF000,0xf000,0xf000,
    67,  0x0386,0xF000,0xF000,0xf000,0xf000,
    72,  0x0389,0xF000,0xF000,0xf000,0xf000,
    81,  0x038F,0xF000,0xF000,0xf000,0xf000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Greek = 14;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Greek[] =
	{
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Greek = 3;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Greek[] =
    {
     0x0387,
     0x03C2,
     0x03B5,0x03AD,
     0x03C1,
     0x03C4,
     0x03C5,0x03CD,0x03CB,0x03B0,
     0x03B8,
     0x03B9,0x03AF,0x03CA,0x0390,
     0x03BF,0x03CC,
     0x03C0,
     0x03B1,0x03AC,
     0x03C3,
     0x03B4,
     0x03C6,
     0x03B3,
     0x03B7,0x03AE,
     0x03BE,
     0x03BA,
     0x03BB,
     0x03B6,
     0x03C7,
     0x03C8,
     0x03C9,0x03CE,
     0x03B2,
     0x03BD,
     0x03BC,
     0x002C,
     0x002E,
     0x0040,
     0x0026,
     0x0027,
     0xF002,0x0384,
     0x0387,
     0xF001,0x0385,
     0x0045,
     0x0050,
     0x0054,
     0x0059,
     0x0398,
     0x0399,0x038A,0x03AA,
     0x004F,
     0x03A0,
     0x0041,
     0x03A3,
     0x0394,
     0x03A6,
     0x0393,
     0x0048,
     0x039E,
     0x039A,
     0x039B,
     0x005A,
     0x0058,
     0x03A8,
     0x03A9,0x038F,
     0x0042,
     0x004E,
     0x004D,
     0x003B,
     0x003A,
     0x003F,
     0xF003,0x00A8,
     0x0021,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x002B,
     0x002F,
     0x0025,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x002D,
     0x0028,
     0x0029,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x003D,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
    };

const TInt KMiniQwertyDataSize_Greek = 111;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Greek[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 2},
        {EPtiKeyQwerty4, EPtiCaseLower, 4, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 5, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 6, 4},
        {EPtiKeyQwerty7, EPtiCaseLower, 10, 1},
        {EPtiKeyQwerty8, EPtiCaseLower, 11, 4},
        {EPtiKeyQwerty9, EPtiCaseLower, 15, 2},
        {EPtiKeyQwerty0, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 18, 2},
        {EPtiKeyQwertyS, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 24, 2},
        {EPtiKeyQwertyJ, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 27, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 32, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 38, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 45, 2},
        {EPtiKeyQwerty3, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 52, 3},
        {EPtiKeyQwerty9, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 69, 2},
        {EPtiKeyQwertyB, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 86, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 87, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 88, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 89, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 90, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 91, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 92, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 93, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 94, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 95, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 96, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 97, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 98, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 99, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 100, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 101, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 102, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 103, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 104, 1},
    };

const TInt KMaxMiniQwertyIndex_Greek = 82;


//
// Dead key data table for MiniQwerty
//
const TUint16 MiniQwertyDeadKeyData_table_Greek[] =
    {
    2,  0xF000,0xF000,0x03AD,0xF000,0xF000,
    5,  0x03B0,0x03CB,0x03CD,0xF000,0xF000,
    7,  0x0390,0x03CA,0x03AF,0xF000,0xF000,
    8,  0xF000,0xF000,0x03CC,0xF000,0xF000,
    10,  0xF000,0xF000,0x03AC,0xF000,0xF000,
    15,  0xF000,0xF000,0x03AE,0xF000,0xF000,
    22,  0xF000,0xF000,0x03CE,0xF000,0xF000,
    39,  0xF000,0x03AA,0x038A,0xF000,0xF000,
    54,  0xF000,0xF000,0x038F,0xF000,0xF000,
    };

const TInt KMiniQwertyDeadKeyTableRows_Greek = 9;


//
// Numeric key data table for MiniQwerty
//
const TPtiNumericKeyBinding MiniQwertyNumericKeysData_table_Greek[] =
	{
	{'p',EPtiKeyQwerty0,EPtiCaseLower},
	{'w',EPtiKeyQwerty2,EPtiCaseLower},
	{'E',EPtiKeyQwerty3,EPtiCaseUpper},
	};

const TInt KNumMiniQwertyNumericKeysEntries_Greek = 3;
//
// Class implementation.
//

CPtiLangDataImplGreek* CPtiLangDataImplGreek::NewL()
    {
    return new (ELeave) CPtiLangDataImplGreek();
    }


TInt CPtiLangDataImplGreek::LanguageCode() const
    {
    return ELangGreek;
    }


const TUint16* CPtiLangDataImplGreek::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Greek;
        return MultitapData_table_Greek;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Greek;
        return StandardQwertyData_table_Greek;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Greek;
        return MiniQwertyData_table_Greek;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplGreek::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Greek;
        return MultitapKeyBindings_table_Greek;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Greek;
        return StandardQwertyKeyBindings_table_Greek;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Greek;
        return MiniQwertyKeyBindings_table_Greek;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplGreek::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_Greek;
        return StandardQwertyDeadKeyData_table_Greek;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumRowsInArray = KMiniQwertyDeadKeyTableRows_Greek;
        return MiniQwertyDeadKeyData_table_Greek;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplGreek::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Greek;
        return StandardQwertyNumericKeysData_table_Greek;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumEntries = KNumMiniQwertyNumericKeysEntries_Greek;
        return MiniQwertyNumericKeysData_table_Greek;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplGreek::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


