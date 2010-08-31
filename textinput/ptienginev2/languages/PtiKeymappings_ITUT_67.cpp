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

#include "PtiKeymappings_ITUT_67.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Latvian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x101,0x62,0x63,0x10d,0x32,0xe4,0x105,0xe5,0xe6,
     0x64,0x65,0x113,0x66,0x33,0xe9,0x117,0x119,
     0x67,0x123,0x68,0x69,0x12b,0x34,0x12f,
     0x6a,0x6b,0x137,0x6c,0x13c,0x35,0xa3,
     0x6d,0x6e,0x146,0x6f,0x36,0xf5,0xf6,0xf8,
     0x70,0x71,0x72,0x157,0x73,0x161,0x37,0xdf,0x24,
     0x74,0x75,0x16b,0x76,0x38,0xfc,0x173,
     0x77,0x78,0x79,0x7a,0x17e,0x39,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x100,0x42,0x43,0x10c,0x32,0xc4,0x104,0xc5,0xc6,
     0x44,0x45,0x112,0x46,0x33,0xc9,0x116,0x118,
     0x47,0x122,0x48,0x49,0x12a,0x34,0x12e,
     0x4a,0x4b,0x136,0x4c,0x13b,0x35,0xa3,
     0x4d,0x4e,0x145,0x4f,0x36,0xd5,0xd6,0xd8,
     0x50,0x51,0x52,0x156,0x53,0x160,0x37,0x24,
     0x54,0x55,0x16a,0x56,0x38,0xdc,0x172,
     0x57,0x58,0x59,0x5a,0x17d,0x39,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Latvian = 189;


const TPtiKeyBinding MultitapKeyBindings_table_Latvian[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 10},
        {EPtiKey3, EPtiCaseLower, 40, 8},
        {EPtiKey4, EPtiCaseLower, 48, 7},
        {EPtiKey5, EPtiCaseLower, 55, 7},
        {EPtiKey6, EPtiCaseLower, 62, 8},
        {EPtiKey7, EPtiCaseLower, 70, 9},
        {EPtiKey8, EPtiCaseLower, 79, 7},
        {EPtiKey9, EPtiCaseLower, 86, 6},
        {EPtiKeyStar, EPtiCaseLower, 92, 0},
        {EPtiKey0, EPtiCaseLower, 92, 3},
        {EPtiKeyHash, EPtiCaseLower, 95, 0},
        {EPtiKey1, EPtiCaseUpper, 95, 30},
        {EPtiKey2, EPtiCaseUpper, 125, 10},
        {EPtiKey3, EPtiCaseUpper, 135, 8},
        {EPtiKey4, EPtiCaseUpper, 143, 7},
        {EPtiKey5, EPtiCaseUpper, 150, 7},
        {EPtiKey6, EPtiCaseUpper, 157, 8},
        {EPtiKey7, EPtiCaseUpper, 165, 8},
        {EPtiKey8, EPtiCaseUpper, 173, 7},
        {EPtiKey9, EPtiCaseUpper, 180, 6},
        {EPtiKeyStar, EPtiCaseUpper, 186, 0},
        {EPtiKey0, EPtiCaseUpper, 186, 3},
        {EPtiKeyHash, EPtiCaseUpper, 189, 0},
    };

const TInt KMaxMultitapIndex_Latvian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Latvian[] =
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
     0x2d,
     0x71,
     0x77,
     0x65,0x113,0x119,0x117,0xe9,
     0x72,0x157,
     0x74,
     0x79,
     0x75,0x16b,0x173,0xfc,
     0x69,0x12b,0x12f,
     0x6f,0xf5,0xf6,0xf8,
     0x70,
     0xf005,0x2d9,
     0x2b,
     0x61,0x101,0x105,0xe4,0xe5,0xe6,
     0x73,0x161,0xdf,
     0x64,
     0x66,
     0x67,0x123,
     0x68,
     0x6a,
     0x6b,0x137,
     0x6c,0x13c,
     0xf002,0x2c7,
     0xf003,0x2db,
     0x7a,0x17e,
     0x78,
     0x63,0x10d,
     0x76,
     0x62,
     0x6e,0x146,
     0x6d,
     0xf004,0xb8,
     0xf001,0x2c9,
     0x2e,
     0x21,
     0x22,
     0x40,
     0x3b,
     0x3d,
     0x3a,
     0x3f,
     0x2a,
     0x28,
     0x29,
     0x2f,
     0x51,
     0x57,
     0x45,0x112,0x118,0x116,0xc9,
     0x52,0x156,
     0x54,
     0x59,
     0x55,0x16a,0x172,0xdc,
     0x49,0x12a,0x12e,
     0x4f,0xd5,0xd6,0xd8,
     0x50,
     0xf005,0x2d9,
     0x23,
     0x41,0x100,0x104,0xc4,0xc5,0xc6,
     0x53,0x160,
     0x44,
     0x46,
     0x47,0x122,
     0x48,
     0x4a,
     0x4b,0x136,
     0x4c,0x13b,
     0xf002,0x2c7,
     0xf003,0x2db,
     0x5a,0x17d,
     0x58,
     0x43,0x10c,
     0x56,
     0x42,
     0x4e,0x145,
     0x4d,
     0xf004,0xb8,
     0xf001,0x2c9,
     0x2c,
    };

const TInt KStandardQwertyDataSize_Latvian = 149;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Latvian[] =
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
        {EPtiKeyQwertyE, EPtiCaseLower, 13, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 18, 2},
        {EPtiKeyQwertyT, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 22, 4},
        {EPtiKeyQwertyI, EPtiCaseLower, 26, 3},
        {EPtiKeyQwertyO, EPtiCaseLower, 29, 4},
        {EPtiKeyQwertyP, EPtiCaseLower, 33, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 34, 2},
        {EPtiKeyQwertyHash, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 37, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 43, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 52, 2},
        {EPtiKeyQwertyL, EPtiCaseLower, 54, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 56, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 58, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 60, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 63, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 65, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 66, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 67, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 69, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 70, 2},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 72, 2},
        {EPtiKeyQwertySlash, EPtiCaseLower, 74, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 88, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 93, 2},
        {EPtiKeyQwertyT, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 97, 4},
        {EPtiKeyQwertyI, EPtiCaseUpper, 101, 3},
        {EPtiKeyQwertyO, EPtiCaseUpper, 104, 4},
        {EPtiKeyQwertyP, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 109, 2},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 112, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 118, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 122, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 124, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 126, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 128, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 130, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 132, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 134, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 136, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 137, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 139, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 140, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 141, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 143, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 144, 2},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 146, 2},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 148, 1},
    };

const TInt KMaxStandardQwertyIndex_Latvian = 88;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Latvian[] =
    {
    13,  0x0113,0xF000,0x0119,0xF000,0x0117,
    14,  0xF000,0xF000,0xF000,0x0157,0xF000,
    17,  0x016B,0xF000,0x0173,0xF000,0xF000,
    18,  0x012B,0xF000,0x012F,0xF000,0xF000,
    23,  0x0101,0xF000,0x0105,0xF000,0xF000,
    24,  0xF000,0x0161,0xF000,0xF000,0xF000,
    27,  0xF000,0xF000,0xF000,0x0123,0xF000,
    30,  0xF000,0xF000,0xF000,0x0137,0xF000,
    31,  0xF000,0xF000,0xF000,0x013C,0xF000,
    34,  0xF000,0x017E,0xF000,0xF000,0xF000,
    36,  0xF000,0x010D,0xF000,0xF000,0xF000,
    39,  0xF000,0xF000,0xF000,0x0146,0xF000,
    57,  0x0112,0xF000,0x0118,0xF000,0x0116,
    58,  0xF000,0xF000,0xF000,0x0156,0xF000,
    61,  0x016A,0xF000,0x0172,0xF000,0xF000,
    62,  0x012A,0xF000,0x012E,0xF000,0xF000,
    67,  0x0100,0xF000,0x0104,0xF000,0xF000,
    68,  0xF000,0x0160,0xF000,0xF000,0xF000,
    71,  0xF000,0xF000,0xF000,0x0122,0xF000,
    74,  0xF000,0xF000,0xF000,0x0136,0xF000,
    75,  0xF000,0xF000,0xF000,0x013B,0xF000,
    78,  0xF000,0x017D,0xF000,0xF000,0xF000,
    80,  0xF000,0x010C,0xF000,0xF000,0xF000,
    83,  0xF000,0xF000,0xF000,0x0145,0xF000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Latvian = 24;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Latvian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x0113,0x0119,0x0117,0x00E9,
     0x0072,0x0157,
     0x0074,
     0x0079,
     0x0075,0x016B,0x0173,0x00FC,
     0x0069,0x012B,0x012F,
     0x006F,0x00F5,0x00F6,0x00F8,
     0x0070,
     0x0061,0x0101,0x0105,0x00E4,0x00E5,0x00E6,
     0x0073,0x0161,0x00DF,
     0x0064,
     0x0066,
     0x0067,0x0123,
     0x0068,
     0x006A,
     0x006B,0x0137,
     0x006C,0x013C,
     0x007A,0x017E,
     0x0078,
     0x0063,0x010D,
     0x0076,
     0x0062,
     0x006E,0x0146,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x0026,
     0x0027,
     0x003F,
     0x0051,
     0x0057,
     0x0045,0x0112,0x0118,0x0116,0x00C9,
     0x0052,0x0156,
     0x0054,
     0x0059,
     0x0055,0x016A,0x0172,0x00DC,
     0x0049,0x012A,0x012E,
     0x004F,0x00D5,0x00D6,0x00D8,
     0x0050,
     0x0041,0x0100,0x0104,0x00C4,0x00C5,0x00C6,
     0x0053,0x0160,
     0x0044,
     0x0046,
     0x0047,0x0122,
     0x0048,
     0x004A,
     0x004B,0x0136,
     0x004C,0x013B,
     0x005A,0x017D,
     0x0058,
     0x0043,0x010C,
     0x0056,
     0x0042,
     0x004E,0x0145,
     0x004D,
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

const TInt KMiniQwertyDataSize_Latvian = 144;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Latvian[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 5},
        {EPtiKeyQwerty4, EPtiCaseLower, 7, 2},
        {EPtiKeyQwerty5, EPtiCaseLower, 9, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 10, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 11, 4},
        {EPtiKeyQwerty8, EPtiCaseLower, 15, 3},
        {EPtiKeyQwerty9, EPtiCaseLower, 18, 4},
        {EPtiKeyQwerty0, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 23, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 29, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 32, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 33, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 34, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 38, 2},
        {EPtiKeyQwertyL, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 42, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 45, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 53, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 60, 5},
        {EPtiKeyQwerty4, EPtiCaseUpper, 65, 2},
        {EPtiKeyQwerty5, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 69, 4},
        {EPtiKeyQwerty8, EPtiCaseUpper, 73, 3},
        {EPtiKeyQwerty9, EPtiCaseUpper, 76, 4},
        {EPtiKeyQwerty0, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 81, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 87, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 91, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 95, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 97, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 99, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 102, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 106, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 137, 1},
    };

const TInt KMaxMiniQwertyIndex_Latvian = 83;

//
// Class implementation.
//

CPtiLangDataImplLatvian* CPtiLangDataImplLatvian::NewL()
    {
    return new (ELeave) CPtiLangDataImplLatvian();
    }


TInt CPtiLangDataImplLatvian::LanguageCode() const
    {
    return ELangLatvian;
    }


const TUint16* CPtiLangDataImplLatvian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Latvian;
        return MultitapData_table_Latvian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Latvian;
        return StandardQwertyData_table_Latvian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Latvian;
        return MiniQwertyData_table_Latvian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplLatvian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Latvian;
        return MultitapKeyBindings_table_Latvian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Latvian;
        return StandardQwertyKeyBindings_table_Latvian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Latvian;
        return MiniQwertyKeyBindings_table_Latvian;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplLatvian::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_Latvian;
        return StandardQwertyDeadKeyData_table_Latvian;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


