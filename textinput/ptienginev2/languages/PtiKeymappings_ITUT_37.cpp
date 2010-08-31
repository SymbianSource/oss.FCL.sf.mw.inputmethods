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

#include "PtiKeymappings_ITUT_37.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Arabic[] =
    {
     0x5f,0x3a,0x2f,0x40,0x29,0x28,0x2d,0x31,0x22,0x27,0x21,0x61f,0x60c,0x2e,0x61b,
     0x628,0x62a,0x629,0x62b,0x32,
     0x627,0x623,0x625,0x622,0x649,0x624,0x626,0x621,0x33,
     0x633,0x634,0x635,0x636,0x34,
     0x62f,0x630,0x631,0x632,0x35,
     0x62c,0x62d,0x62e,0x36,
     0x646,0x647,0x648,0x64a,0x37,
     0x641,0x642,0x643,0x644,0x645,0x38,
     0x637,0x638,0x639,0x63a,0x39,
     0x20,0x30,0x21b3,
     0x5f,0x3a,0x2f,0x40,0x29,0x28,0x2d,0x31,0x22,0x27,0x21,0x61f,0x60c,0x2e,0x61b,
     0x628,0x62a,0x629,0x62b,0x32,
     0x627,0x623,0x625,0x622,0x649,0x624,0x626,0x621,0x33,
     0x633,0x634,0x635,0x636,0x34,
     0x62f,0x630,0x631,0x632,0x35,
     0x62c,0x62d,0x62e,0x36,
     0x646,0x647,0x648,0x64a,0x37,
     0x641,0x642,0x643,0x644,0x645,0x38,
     0x637,0x638,0x639,0x63a,0x39,
     0x20,0x30,0x21b3,
    };

const TInt KMultitapDataSize_Arabic = 124;


const TPtiKeyBinding MultitapKeyBindings_table_Arabic[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 15},
        {EPtiKey2, EPtiCaseLower, 15, 5},
        {EPtiKey3, EPtiCaseLower, 20, 9},
        {EPtiKey4, EPtiCaseLower, 29, 5},
        {EPtiKey5, EPtiCaseLower, 34, 5},
        {EPtiKey6, EPtiCaseLower, 39, 4},
        {EPtiKey7, EPtiCaseLower, 43, 5},
        {EPtiKey8, EPtiCaseLower, 48, 6},
        {EPtiKey9, EPtiCaseLower, 54, 5},
        {EPtiKeyStar, EPtiCaseLower, 59, 0},
        {EPtiKey0, EPtiCaseLower, 59, 3},
        {EPtiKeyHash, EPtiCaseLower, 62, 0},
        {EPtiKey1, EPtiCaseUpper, 62, 15},
        {EPtiKey2, EPtiCaseUpper, 77, 5},
        {EPtiKey3, EPtiCaseUpper, 82, 9},
        {EPtiKey4, EPtiCaseUpper, 91, 5},
        {EPtiKey5, EPtiCaseUpper, 96, 5},
        {EPtiKey6, EPtiCaseUpper, 101, 4},
        {EPtiKey7, EPtiCaseUpper, 105, 5},
        {EPtiKey8, EPtiCaseUpper, 110, 6},
        {EPtiKey9, EPtiCaseUpper, 116, 5},
        {EPtiKeyStar, EPtiCaseUpper, 121, 0},
        {EPtiKey0, EPtiCaseUpper, 121, 3},
        {EPtiKeyHash, EPtiCaseUpper, 124, 0},
    };

const TInt KMaxMultitapIndex_Arabic = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Arabic[] =
    {
     0x0031,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030,
     0x002d,
     0x0636,
     0x0635,
     0x062b,
     0x0642,
     0x0641,
     0x063a,
     0x0639,
     0x0647,
     0x062e,
     0x062d,
     0x062c,
     0x002b,
     0x0634,
     0x0633,
     0x064a,
     0x0628,
     0x0644,
     0x0627,
     0x062a,
     0x0646,
     0x0645,
     0x0643,
     0x0637,
     0x0626,
     0x0621,
     0x0624,
     0x0631,
     0xfefb,
     0x0649,
     0x0629,
     0x0648,
     0x0632,
     0x0638,
     0x0021,
     0x0022,
     0x20ac,
     0x0024,
     0x0025,
     0x0026,
     0x002a,
     0x0029,
     0x0028,
     0x003d,
     0x0040,
     0x0636,
     0x0635,
     0x062b,
     0x0642,
     0xfef9,
     0x0625,
     0x0639,
     0x0647,
     0x062e,
     0x061b,
     0x062f,
     0x0023,
     0x0634,
     0x0633,
     0x064a,
     0x0628,
     0x0fef7,
     0x0623,
     0x062a,
     0x0646,
     0x0645,
     0x003a,
     0x0630,
     0x0626,
     0x0621,
     0x0624,
     0x0631,
     0xfef5,
     0x0622,
     0x0629,
     0x060c,
     0x002e,
     0x061f,
    };

const TInt KStandardQwertyDataSize_Arabic = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Arabic[] =
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
        {EPtiKeyQwertyE, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyO, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 27, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 32, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 33, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 43, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 70, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 87, 1},
    };

const TInt KMaxStandardQwertyIndex_Arabic = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Arabic[] =
	{
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{';',EPtiKeyQwertySemicolon,EPtiCaseLower},
	{',',EPtiKeyQwertyComma,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	{'?',EPtiKeyQwertySlash,EPtiCaseUpper},
	{'/',EPtiKeyQwertySlash,EPtiCaseLower},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Arabic = 7;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Arabic[] =
    {
     0x0636,
     0x0635,
     0x062B,
     0x0642,
     0x0641,
     0x063A,
     0x0639,
     0x062E,
     0x062D,
     0x062C,
     0x0634,
     0x0633,
     0x064A,
     0x0628,
     0x0644,
     0x0627,
     0x062A,
     0x0646,
     0x0645,
     0x0621,
     0x062F,
     0x0647,
     0x0631,
     0xFEFB,
     0x0649,
     0x0643,
     0x0648,
     0x0637,
     0x0040,
     0x0026,
     0x060C,
     0x061B,
     0x0636,
     0x0635,
     0x062B,
     0x0642,
     0xFEF9,
     0x0625,
     0x0639,
     0x062E,
     0x062D,
     0x062C,
     0x0634,
     0x0633,
     0x0626,
     0x0628,
     0xFEF7,
     0x0623,
     0x062A,
     0x0646,
     0x0645,
     0x0621,
     0x0630,
     0x0629,
     0x0632,
     0xFEF5,
     0x0622,
     0x0643,
     0x0624,
     0x0638,
     0x002C,
     0x002E,
     0x0021,
     0x0022,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x002B,
     0x002F,
     0x0027,
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
     0x061F,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
    };

const TInt KMiniQwertyDataSize_Arabic = 95;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Arabic[] =
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
        {EPtiKeyQwertyA, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 27, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 32, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 33, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 40, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 41, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 62, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 63, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 64, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 65, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 66, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 67, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 68, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 69, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 70, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 71, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 72, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 73, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 74, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 75, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 86, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 87, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 88, 1},
    };

const TInt KMaxMiniQwertyIndex_Arabic = 83;


//
// Numeric key data table for MiniQwerty
//
const TPtiNumericKeyBinding MiniQwertyNumericKeysData_table_Arabic[] =
	{
	{'p',EPtiKeyQwerty0,EPtiCaseLower},
	{'w',EPtiKeyQwerty2,EPtiCaseLower},
	{'E',EPtiKeyQwerty3,EPtiCaseUpper},
	};

const TInt KNumMiniQwertyNumericKeysEntries_Arabic = 3;
//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Arabic[] =
    {
     0x0636,
     0x0635,
     0x062B,
     0x0642,
     0x0641,
     0x063A,
     0x0639,
     0x0647,
     0x062D,
     0x062C,
     0x0634,
     0x0633,
     0x064A,
     0x0628,
     0x0644,
     0x0627,
     0x0646,
     0x0643,
     0x0637,
     0x060C,
     0x0621,
     0x062F,
     0x0631,
     0xFEFB,
     0x0649,
     0x0648,
     0x0632,
     0x0636,
     0x0635,
     0x062B,
     0x0642,
     0xFEF9,
     0x0625,
     0x0639,
     0x0647,
     0x062E,
     0x062C,
     0x0634,
     0x0633,
     0x064A,
     0x0628,
     0xFEF7,
     0x0623,
     0x062A,
     0x0645,
     0x0638,
     0x061B,
     0x0626,
     0x0630,
     0x0624,
     0xFEF5,
     0x0622,
     0x0629,
     0x061F,
     0x0031,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030,
     0x0040,
     0x002F,
     0x0027,
     0x0022,
     0x002A,
     0x0023,
     0x002B,
     0x002D,
     0x003D,
     0x002E,
     0x0028,
     0x0029,
     0x0026,
     0x0021,
     0x003F,
     0x003B,
     0x003A,
    };

const TInt KMiniQwerty3x11DataSize_Arabic = 81;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Arabic[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 3, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 4, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 5, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 6, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 7, 1},
        {EPtiKeyQwertyO, EPtiCaseLower, 8, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 9, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 26, 1},
        {EStdKeySpace, EPtiCaseLower, 27, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 27, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 28, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 29, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 30, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 31, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 32, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 33, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 40, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 41, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 53, 1},
        {EStdKeySpace, EPtiCaseUpper, 54, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 54, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 55, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 56, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 57, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 58, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 59, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 60, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 61, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 62, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 63, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 64, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 65, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 66, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 67, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 68, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 69, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 70, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 71, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 72, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 73, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 74, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 75, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 80, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Arabic = 83;


//
// Numeric key data table for MiniQwerty3x11
//
const TPtiNumericKeyBinding MiniQwerty3x11NumericKeysData_table_Arabic[] =
	{
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{',',EPtiKeyQwertyComma,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	{'/',EPtiKeyQwertyS,EPtiCaseFnLower},
	};

const TInt KNumMiniQwerty3x11NumericKeysEntries_Arabic = 5;
//
// Class implementation.
//

CPtiLangDataImplArabic* CPtiLangDataImplArabic::NewL()
    {
    return new (ELeave) CPtiLangDataImplArabic();
    }


TInt CPtiLangDataImplArabic::LanguageCode() const
    {
    return ELangArabic;
    }


const TUint16* CPtiLangDataImplArabic::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Arabic;
        return MultitapData_table_Arabic;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Arabic;
        return StandardQwertyData_table_Arabic;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Arabic;
        return MiniQwertyData_table_Arabic;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Arabic;
        return MiniQwerty3x11Data_table_Arabic;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplArabic::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Arabic;
        return MultitapKeyBindings_table_Arabic;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Arabic;
        return StandardQwertyKeyBindings_table_Arabic;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Arabic;
        return MiniQwertyKeyBindings_table_Arabic;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Arabic;
        return MiniQwerty3x11KeyBindings_table_Arabic;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplArabic::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Arabic;
        return StandardQwertyNumericKeysData_table_Arabic;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumEntries = KNumMiniQwertyNumericKeysEntries_Arabic;
        return MiniQwertyNumericKeysData_table_Arabic;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumEntries = KNumMiniQwerty3x11NumericKeysEntries_Arabic;
        return MiniQwerty3x11NumericKeysData_table_Arabic;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplArabic::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


