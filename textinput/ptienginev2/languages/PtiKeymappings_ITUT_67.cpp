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
     0x0071,
     0x0077,
     0x0065,0x0113,0x011B,
     0x0072,0x0157,0x0159,
     0x0074,0x0165,
     0x0079,
     0x0075,0x016B,
     0x0069,0x012B,
     0x006F,
     0x0070,
     0xF001,0x00AF,
     0x0061,0x0101,
     0x0073,0x0161,
     0x0064,0x010F,
     0x0066,
     0x0067,0x0123,
     0x0068,
     0x006A,
     0x006B,0x0137,
     0x006C,0x013C,
     0xF003,0x02C7,
     0xF002,0x00B8,
     0x007A,0x017E,
     0x0078,
     0x0063,0x010D,
     0x0076,
     0x0062,
     0x006E,0x0146,0x0148,
     0x006D,
     0x002C,
     0x002E,
     0x003F,
     0x002D,
     0x0051,
     0x0057,
     0x0045,0x0112,0x011A,
     0x0052,0x0156,0x0158,
     0x0054,0x0164,
     0x0059,
     0x0055,0x016A,
     0x0049,0x012A,
     0x004F,
     0x0050,
     0xF001,0x00AF,
     0x0041,0x0100,
     0x0053,0x0160,
     0x0044,0x010E,
     0x0046,
     0x0047,0x0122,
     0x0048,
     0x004A,
     0x004B,0x0136,
     0x004C,0x013B,
     0xF003,0x02C7,
     0xF002,0x00B8,
     0x005A,0x017D,
     0x0058,
     0x0043,0x010C,
     0x0056,
     0x0042,
     0x004E,0x0145,0x0147,
     0x004D,
     0x0027,
     0x002F,
     0x0021,
     0x002B,
    };

const TInt KStandardQwertyDataSize_Latvian = 106;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Latvian[] =
    {
        {EPtiKeyQwertyAtCharacter, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty1, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 3},
        {EPtiKeyQwerty3, EPtiCaseLower, 5, 3},
        {EPtiKeyQwerty4, EPtiCaseLower, 8, 2},
        {EPtiKeyQwerty5, EPtiCaseLower, 10, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 11, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 13, 2},
        {EPtiKeyQwerty8, EPtiCaseLower, 15, 1},
        {EPtiKeyQwerty9, EPtiCaseLower, 16, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 17, 2},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 19, 2},
        {EPtiKeyQwertyQ, EPtiCaseLower, 21, 2},
        {EPtiKeyQwertyW, EPtiCaseLower, 23, 2},
        {EPtiKeyQwertyE, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 26, 2},
        {EPtiKeyQwertyT, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 30, 2},
        {EPtiKeyQwertyI, EPtiCaseLower, 32, 2},
        {EPtiKeyQwertyO, EPtiCaseLower, 34, 2},
        {EPtiKeyQwertyP, EPtiCaseLower, 36, 2},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 38, 0},
        {EPtiKeyQwertyHash, EPtiCaseLower, 38, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 41, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 45, 3},
        {EPtiKeyQwertyH, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 53, 0},
        {0x5b, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyZ, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyC, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyV, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyB, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyN, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyM, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyComma, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertySlash, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 55, 3},
        {EPtiKeyQwerty3, EPtiCaseUpper, 58, 3},
        {EPtiKeyQwerty4, EPtiCaseUpper, 61, 2},
        {EPtiKeyQwerty5, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 64, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 66, 2},
        {EPtiKeyQwerty8, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 70, 2},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 72, 2},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 74, 2},
        {EPtiKeyQwertyW, EPtiCaseUpper, 76, 2},
        {EPtiKeyQwertyE, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 79, 2},
        {EPtiKeyQwertyT, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 83, 2},
        {EPtiKeyQwertyI, EPtiCaseUpper, 85, 2},
        {EPtiKeyQwertyO, EPtiCaseUpper, 87, 2},
        {EPtiKeyQwertyP, EPtiCaseUpper, 89, 2},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 91, 0},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 91, 2},
        {EPtiKeyQwertyA, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 94, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 98, 3},
        {EPtiKeyQwertyH, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 106, 0},
        {0x5b, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyX, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyC, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyV, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyB, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyM, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 106, 0},
    };

const TInt KMaxStandardQwertyIndex_Latvian = 92;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Latvian[] =
    {
    2,  0x0113,0xf000,0x011B,0xf000,0xf000,
    3,  0xf000,0x0157,0x0159,0xf000,0xf000,
    4,  0xf000,0xf000,0x0165,0xf000,0xf000,
    6,  0x016B,0xf000,0xf000,0xf000,0xf000,
    7,  0x012B,0xf000,0xf000,0xf000,0xf000,
    11,  0x0101,0xf000,0xf000,0xf000,0xf000,
    12,  0xf000,0xf000,0x0161,0xf000,0xf000,
    13,  0xf000,0xf000,0x010F,0xf000,0xf000,
    15,  0xf000,0x0123,0xf000,0xf000,0xf000,
    18,  0xf000,0x0137,0xf000,0xf000,0xf000,
    19,  0xf000,0x013C,0xf000,0xf000,0xf000,
    22,  0xf000,0xf000,0x017E,0xf000,0xf000,
    24,  0xf000,0xf000,0x010D,0xf000,0xf000,
    27,  0xf000,0x0146,0x0148,0xf000,0xf000,
    35,  0x0112,0xf000,0x011A,0xf000,0xf000,
    36,  0xf000,0x0156,0x0158,0xf000,0xf000,
    37,  0xf000,0xf000,0x0164,0xf000,0xf000,
    39,  0x016A,0xf000,0xf000,0xf000,0xf000,
    40,  0x012A,0xf000,0xf000,0xf000,0xf000,
    44,  0x0100,0xf000,0xf000,0xf000,0xf000,
    45,  0xf000,0xf000,0x0160,0xf000,0xf000,
    46,  0xf000,0xf000,0x010E,0xf000,0xf000,
    48,  0xf000,0x0122,0xf000,0xf000,0xf000,
    51,  0xf000,0x0136,0xf000,0xf000,0xf000,
    52,  0xf000,0x013B,0xf000,0xf000,0xf000,
    55,  0xf000,0xf000,0x017D,0xf000,0xf000,
    57,  0xf000,0xf000,0x010C,0xf000,0xf000,
    60,  0xf000,0x0145,0x0147,0xf000,0xf000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Latvian = 28;

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


