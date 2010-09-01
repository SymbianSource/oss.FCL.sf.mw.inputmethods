/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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

#include "PtiKeymappings_ITUT_09.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Finnish[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe4,0xe5,0xe6,0xe0,0xe1,0xe2,0xe3,0xe7,
     0x64,0x65,0x66,0x33,0xe9,0xe8,0xeb,0xea,0xf0,
     0x67,0x68,0x69,0x34,0xee,0xef,0xec,0xed,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf6,0xf8,0xf2,0xf3,0xf4,0xf5,0xf1,
     0x70,0x71,0x72,0x73,0x37,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xfc,0xf9,0xfb,0xfa,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0xfe,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc4,0xc5,0xc6,0xc0,0xc1,0xc2,0xc3,0xc7,
     0x44,0x45,0x46,0x33,0xc9,0xc8,0xcb,0xca,0xd0,
     0x47,0x48,0x49,0x34,0xce,0xcf,0xcc,0xcd,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd6,0xd8,0xd2,0xd3,0xd4,0xd5,0xd1,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xdc,0xd9,0xdb,0xda,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0xde,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Finnish = 199;


const TPtiKeyBinding MultitapKeyBindings_table_Finnish[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 12},
        {EPtiKey3, EPtiCaseLower, 42, 9},
        {EPtiKey4, EPtiCaseLower, 51, 8},
        {EPtiKey5, EPtiCaseLower, 59, 5},
        {EPtiKey6, EPtiCaseLower, 64, 11},
        {EPtiKey7, EPtiCaseLower, 75, 7},
        {EPtiKey8, EPtiCaseLower, 82, 8},
        {EPtiKey9, EPtiCaseLower, 90, 7},
        {EPtiKeyStar, EPtiCaseLower, 97, 0},
        {EPtiKey0, EPtiCaseLower, 97, 3},
        {EPtiKeyHash, EPtiCaseLower, 100, 0},
        {EPtiKey1, EPtiCaseUpper, 100, 30},
        {EPtiKey2, EPtiCaseUpper, 130, 12},
        {EPtiKey3, EPtiCaseUpper, 142, 9},
        {EPtiKey4, EPtiCaseUpper, 151, 8},
        {EPtiKey5, EPtiCaseUpper, 159, 5},
        {EPtiKey6, EPtiCaseUpper, 164, 11},
        {EPtiKey7, EPtiCaseUpper, 175, 6},
        {EPtiKey8, EPtiCaseUpper, 181, 8},
        {EPtiKey9, EPtiCaseUpper, 189, 7},
        {EPtiKeyStar, EPtiCaseUpper, 196, 0},
        {EPtiKey0, EPtiCaseUpper, 196, 3},
        {EPtiKeyHash, EPtiCaseUpper, 199, 0},
    };

const TInt KMaxMultitapIndex_Finnish = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Finnish[] =
    {
     0x0071,
     0x0077,
     0x0065,
     0x0072,
     0x0074,
     0x0079,
     0x0075,
     0x0069,
     0x006F,
     0x0070,
     0x00E5,
     0x0061,
     0x0073,
     0x0064,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x00F6,
     0x00E4,
     0x007A,
     0x0078,
     0x0063,
     0x0076,
     0x0062,
     0x006E,
     0x006D,
     0x002C,
     0x002E,
     0x003F,
     0x002D,
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
     0x00C5,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x00D6,
     0x00C4,
     0x005A,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004E,
     0x004D,
     0x0027,
     0x002F,
     0x0021,
     0x002B,
    };

const TInt KStandardQwertyDataSize_Finnish = 66;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Finnish[] =
    {
        {EPtiKeyQwertyAtCharacter, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty1, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 3, 1},
        {EPtiKeyQwerty4, EPtiCaseLower, 4, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 5, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 6, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty8, EPtiCaseLower, 8, 1},
        {EPtiKeyQwerty9, EPtiCaseLower, 9, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyQ, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyO, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 22, 0},
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
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 33, 0},
        {0x5b, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyZ, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyC, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyV, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyB, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyN, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyM, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyComma, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertySlash, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 33, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 40, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 41, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 55, 0},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 66, 0},
        {0x5b, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyX, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyC, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyV, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyB, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyM, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 66, 0},
    };

const TInt KMaxStandardQwertyIndex_Finnish = 92;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Finnish[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00E8,0x00EB,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FC,0x00F9,0x00FB,0x00FA,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x00F8,0x00F2,0x00F3,0x00F4,0x00F5,
     0x0070,
     0x0061,0x00E6,0x00E0,0x00E1,0x00E2,0x00E3,
     0x0073,0x0161,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x007A,0x017E,
     0x0078,
     0x0063,0x00E7,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x00F6,0x00F8,
     0x00E4,0x00E6,
     0x0040,
     0x003F,
     0x002C,
     0x002E,
     0x0051,
     0x0057,
     0x0045,0x00C9,0x00C8,0x00CB,0x00CA,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00DC,0x00D9,0x00DB,0x00DA,
     0x0049,0x00CE,0x00CF,0x00CC,0x00CD,
     0x004F,0x00D8,0x00D2,0x00D3,0x00D4,0x00D5,
     0x0050,
     0x0041,0x00C6,0x00C0,0x00C1,0x00C2,0x00C3,
     0x0053,0x0160,
     0x0044,0x00D0,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x005A,0x017D,
     0x0058,
     0x0043,0x00C7,
     0x0056,
     0x0042,
     0x004E,0x00D1,
     0x004D,
     0x00D6,0x00D8,
     0x00C4,0x00C6,
     0x003B,
     0x003A,
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
     0x00E5,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
     0x00C5,
    };

const TInt KMiniQwertyDataSize_Finnish = 159;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Finnish[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 5},
        {EPtiKeyQwerty4, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 8, 2},
        {EPtiKeyQwerty6, EPtiCaseLower, 10, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 12, 5},
        {EPtiKeyQwerty8, EPtiCaseLower, 17, 5},
        {EPtiKeyQwerty9, EPtiCaseLower, 22, 6},
        {EPtiKeyQwerty0, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 29, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 35, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 38, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 46, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 53, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 56, 2},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 58, 2},
        {EPtiKeyQwerty1, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 66, 5},
        {EPtiKeyQwerty4, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 72, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 74, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 76, 5},
        {EPtiKeyQwerty8, EPtiCaseUpper, 81, 5},
        {EPtiKeyQwerty9, EPtiCaseUpper, 86, 6},
        {EPtiKeyQwerty0, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 93, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 99, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 101, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 109, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 112, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 116, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 119, 2},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 121, 2},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 149, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 150, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 151, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnUpper, 158, 1},
    };

const TInt KMaxMiniQwertyIndex_Finnish = 84;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Finnish[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00E8,0x00EB,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,
     0x0075,0x00FC,0x00F9,0x00FB,0x00FA,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x00F6,0x00F8,0x00F2,0x00F3,0x00F4,0x00F5,
     0x0070,
     0x0061,0x00E4,0x00E5,0x00E6,0x00E0,0x00E1,0x00E2,0x00E3,
     0x0073,0x0161,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x002E,
     0x007A,0x017E,
     0x0078,
     0x0063,0x00E7,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x0051,
     0x0057,
     0x0045,0x00C9,0x00C8,0x00CB,0x00CA,
     0x0052,
     0x0054,0x00DE,
     0x0059,
     0x0055,0x00DC,0x00D9,0x00DB,0x00DA,
     0x0049,0x00CE,0x00CF,0x00CC,0x00CD,
     0x004F,0x00D6,0x00D8,0x00D2,0x00D3,0x00D4,0x00D5,
     0x0050,
     0x0041,0x00C4,0x00C5,0x00C6,0x00C0,0x00C1,0x00C2,0x00C3,
     0x0053,0x0160,
     0x0044,0x00D0,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x002C,
     0x005A,0x017D,
     0x0058,
     0x0043,0x00C7,
     0x0056,
     0x0042,
     0x004E,0x00D1,
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
     0x0030,
     0x0040,
     0x002F,
     0x002D,
     0x002B,
     0x003D,
     0x0023,
     0x002A,
     0x00F6,
     0x00E4,
     0x00E5,
     0x0025,
     0x0026,
     0x0022,
     0x003F,
     0x0021,
     0x003B,
     0x003A,
     0x00D6,
     0x00C4,
     0x00C5,
    };

const TInt KMiniQwerty3x11DataSize_Finnish = 147;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Finnish[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 7, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 8, 2},
        {EPtiKeyQwertyY, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 11, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 16, 5},
        {EPtiKeyQwertyO, EPtiCaseLower, 21, 7},
        {EPtiKeyQwertyP, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 29, 8},
        {EPtiKeyQwertyS, EPtiCaseLower, 37, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 52, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 56, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 58, 1},
        {EStdKeySpace, EPtiCaseLower, 59, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 61, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 67, 2},
        {EPtiKeyQwertyY, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 70, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 75, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 80, 7},
        {EPtiKeyQwertyP, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 88, 8},
        {EPtiKeyQwertyS, EPtiCaseUpper, 96, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 98, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 107, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 110, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 114, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 116, 1},
        {EStdKeySpace, EPtiCaseUpper, 117, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 144, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 145, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnUpper, 146, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Finnish = 86;

//
// Class implementation.
//

CPtiLangDataImplFinnish* CPtiLangDataImplFinnish::NewL()
    {
    return new (ELeave) CPtiLangDataImplFinnish();
    }


TInt CPtiLangDataImplFinnish::LanguageCode() const
    {
    return ELangFinnish;
    }


const TUint16* CPtiLangDataImplFinnish::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Finnish;
        return MultitapData_table_Finnish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Finnish;
        return StandardQwertyData_table_Finnish;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Finnish;
        return MiniQwertyData_table_Finnish;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Finnish;
        return MiniQwerty3x11Data_table_Finnish;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplFinnish::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Finnish;
        return MultitapKeyBindings_table_Finnish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Finnish;
        return StandardQwertyKeyBindings_table_Finnish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Finnish;
        return MiniQwertyKeyBindings_table_Finnish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Finnish;
        return MiniQwerty3x11KeyBindings_table_Finnish;
        }

    aNumItems = 0;
    return NULL;
    }


