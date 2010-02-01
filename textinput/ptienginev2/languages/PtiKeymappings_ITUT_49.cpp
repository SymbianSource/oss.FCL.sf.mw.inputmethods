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

#include "PtiKeymappings_ITUT_49.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Estonian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe4,0x101,0x105,0xe5,0xe6,0x10d,
     0x64,0x65,0x66,0x33,0xe9,0x113,0x117,0x119,
     0x67,0x68,0x69,0x34,0x123,0x12b,0x12f,
     0x6a,0x6b,0x6c,0x35,0x137,0x13c,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf5,0xf6,0xf8,0x146,
     0x70,0x71,0x72,0x73,0x37,0x161,0x157,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xfc,0x16b,0x173,
     0x77,0x78,0x79,0x7a,0x39,0x17e,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc4,0x100,0x104,0xc5,0xc6,0x10c,
     0x44,0x45,0x46,0x33,0xc9,0x112,0x116,0x118,
     0x47,0x48,0x49,0x34,0x122,0x12a,0x12e,
     0x4a,0x4b,0x4c,0x35,0x136,0x13b,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd5,0xd6,0xd8,0x145,
     0x50,0x51,0x52,0x53,0x37,0x160,0x156,0x24,
     0x54,0x55,0x56,0x38,0xdc,0x16a,0x172,
     0x57,0x58,0x59,0x5a,0x39,0x17d,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Estonian = 189;


const TPtiKeyBinding MultitapKeyBindings_table_Estonian[] =
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

const TInt KMaxMultitapIndex_Estonian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Estonian[] =
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
     0x40,
     0x71,
     0x77,
     0x65,0xe9,0x113,0x117,0x119,
     0x72,0x157,
     0x74,
     0x7a,0x17e,
     0x75,0x16b,0x173,
     0x69,0x12b,0x12f,
     0x6f,0xf5,0xf8,
     0x70,
     0xfc,
     0x2b,
     0x61,0x101,0x105,0xe5,0xe6,
     0x73,0x161,0xdf,
     0x64,
     0x66,
     0x67,0x123,
     0x68,
     0x6a,
     0x6b,0x137,
     0x6c,0x13c,
     0xf6,
     0xe4,
     0x79,
     0x78,
     0x63,0x10d,
     0x76,
     0x62,
     0x6e,0x146,
     0x6d,
     0x2c,
     0x2e,
     0x2d,
     0x21,
     0x22,
     0x20ac,
     0x2a,
     0x2f,
     0x28,
     0x29,
     0x3d,
     0x3f,
     0xdf,
     0xf001,0x7e,
     0x51,
     0x57,
     0x45,0xc9,0x112,0x116,0x118,
     0x52,0x156,
     0x54,
     0x5a,0x17d,
     0x55,0x16a,0x172,
     0x49,0x12a,0x12e,
     0x4f,0xd5,0xd8,
     0x50,
     0xdc,
     0x23,
     0x41,0x100,0x104,0xc5,0xc6,
     0x53,0x160,
     0x44,
     0x46,
     0x47,0x122,
     0x48,
     0x4a,
     0x4b,0x136,
     0x4c,0x13b,
     0xd6,
     0xc4,
     0x59,
     0x58,
     0x43,0x10c,
     0x56,
     0x42,
     0x4e,0x145,
     0x4d,
     0x3b,
     0x3a,
     0x27,
    };

const TInt KStandardQwertyDataSize_Estonian = 134;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Estonian[] =
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
        {EPtiKeyQwertyY, EPtiCaseLower, 21, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 23, 3},
        {EPtiKeyQwertyI, EPtiCaseLower, 26, 3},
        {EPtiKeyQwertyO, EPtiCaseLower, 29, 3},
        {EPtiKeyQwertyP, EPtiCaseLower, 32, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 33, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 35, 5},
        {EPtiKeyQwertyS, EPtiCaseLower, 40, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 45, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyL, EPtiCaseLower, 51, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 57, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 61, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 64, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 65, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 66, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 70, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 77, 2},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 81, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 86, 2},
        {EPtiKeyQwertyT, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 89, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 91, 3},
        {EPtiKeyQwertyI, EPtiCaseUpper, 94, 3},
        {EPtiKeyQwertyO, EPtiCaseUpper, 97, 3},
        {EPtiKeyQwertyP, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 103, 5},
        {EPtiKeyQwertyS, EPtiCaseUpper, 108, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 112, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 116, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 118, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 123, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 124, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 127, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 128, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 130, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 131, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 132, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 133, 1},
    };

const TInt KMaxStandardQwertyIndex_Estonian = 88;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Estonian[] =
    {
    19,  0x00f5,0xf000,0xf000,0xf000,0xf000,
    63,  0x00d5,0xf000,0xf000,0xf000,0xf000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Estonian = 2;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Estonian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x0113,0x0117,0x0119,
     0x0072,0x0157,
     0x0074,
     0x007A,0x017E,
     0x0075,0x00FC,0x016B,0x0173,
     0x0069,0x012B,0x012F,
     0x006F,0x00F5,0x00F8,
     0x0070,
     0x0061,0x0101,0x0105,0x00E5,0x00E6,
     0x0073,0x0161,0x00DF,
     0x0064,
     0x0066,
     0x0067,0x0123,
     0x0068,
     0x006A,
     0x006B,0x0137,
     0x006C,0x013C,
     0x0079,
     0x0078,
     0x0063,0x010D,
     0x0076,
     0x0062,
     0x006E,0x0146,
     0x006D,
     0x00F6,
     0x00E4,
     0x0040,
     0x0026,
     0x002C,
     0x002E,
     0x0051,
     0x0057,
     0x0045,0x00C9,0x0112,0x0116,0x0118,
     0x0052,0x0156,
     0x0054,
     0x005A,0x017D,
     0x0055,0x00DC,0x016A,0x0172,
     0x0049,0x012A,0x012E,
     0x004F,0x00D5,0x00D8,
     0x0050,
     0x0041,0x0100,0x0104,0x00C5,0x00C6,
     0x0053,0x0160,
     0x0044,
     0x0046,
     0x0047,0x0122,
     0x0048,
     0x004A,
     0x004B,0x0136,
     0x004C,0x013B,
     0x0059,
     0x0058,
     0x0043,0x010C,
     0x0056,
     0x0042,
     0x004E,0x0145,
     0x004D,
     0x00D6,
     0x00C4,
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
     0x00DF,
     0x003F,
     0x0027,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x00FC,
     0x002F,
     0x002D,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x002B,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
     0x00DF,
     0x00DC,
    };

const TInt KMiniQwertyDataSize_Estonian = 144;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Estonian[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 5},
        {EPtiKeyQwerty4, EPtiCaseLower, 7, 2},
        {EPtiKeyQwerty5, EPtiCaseLower, 9, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 10, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 12, 4},
        {EPtiKeyQwerty8, EPtiCaseLower, 16, 3},
        {EPtiKeyQwerty9, EPtiCaseLower, 19, 3},
        {EPtiKeyQwerty0, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 23, 5},
        {EPtiKeyQwertyS, EPtiCaseLower, 28, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 32, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 33, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyL, EPtiCaseLower, 39, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 43, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 47, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 51, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 58, 5},
        {EPtiKeyQwerty4, EPtiCaseUpper, 63, 2},
        {EPtiKeyQwerty5, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 66, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 68, 4},
        {EPtiKeyQwerty8, EPtiCaseUpper, 72, 3},
        {EPtiKeyQwerty9, EPtiCaseUpper, 75, 3},
        {EPtiKeyQwerty0, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 79, 5},
        {EPtiKeyQwertyS, EPtiCaseUpper, 84, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 88, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 92, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 94, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 98, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 102, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 109, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 110, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwerty9, EPtiCaseFnUpper, 142, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 143, 1},
    };

const TInt KMaxMiniQwertyIndex_Estonian = 85;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Estonian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x0113,0x0117,0x0119,
     0x0072,0x0157,
     0x0074,
     0x007A,0x017E,
     0x0075,0x00FC,0x016B,0x0173,
     0x0069,0x012B,0x012F,
     0x006F,0x00F5,0x00F8,
     0x0070,
     0x0061,0x0101,0x0105,0x00E5,0x00E6,
     0x0073,0x0161,0x00DF,
     0x0064,
     0x0066,
     0x0067,0x0123,
     0x0068,
     0x006A,
     0x006B,0x0137,
     0x006C,0x013C,
     0x00FC,
     0x0079,
     0x0078,
     0x0063,0x010D,
     0x0076,
     0x0062,
     0x006E,0x0146,
     0x006D,
     0x0051,
     0x0057,
     0x0045,0x00C9,0x0112,0x0116,0x0118,
     0x0052,0x0156,
     0x0054,
     0x005A,0x017D,
     0x0055,0x00DC,0x016A,0x0172,
     0x0049,0x012A,0x012E,
     0x004F,0x00D5,0x00D8,
     0x0050,
     0x0041,0x0100,0x0104,0x00C5,0x00C6,
     0x0053,0x0160,
     0x0044,
     0x0046,
     0x0047,0x0122,
     0x0048,
     0x004A,
     0x004B,0x0136,
     0x0059,
     0x0058,
     0x0043,0x010C,
     0x0056,
     0x0042,
     0x004E,0x0145,
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
     0x002B,
     0x002D,
     0x002A,
     0x0023,
     0x003D,
     0x00F6,
     0x00E4,
     0x00DF,
     0x0027,
     0x0021,
     0x003F,
     0x002C,
     0x002E,
     0x003B,
     0x003A,
     0x00D6,
     0x00C4,
     0x00DF,
    };

const TInt KMiniQwerty3x11DataSize_Estonian = 128;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Estonian[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 7, 2},
        {EPtiKeyQwertyT, EPtiCaseLower, 9, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 10, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 12, 4},
        {EPtiKeyQwertyI, EPtiCaseLower, 16, 3},
        {EPtiKeyQwertyO, EPtiCaseLower, 19, 3},
        {EPtiKeyQwertyP, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 23, 5},
        {EPtiKeyQwertyS, EPtiCaseLower, 28, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 32, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 33, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyL, EPtiCaseLower, 39, 2},
        {EPtiKeyQwertyComma, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 44, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 50, 1},
        {EStdKeySpace, EPtiCaseLower, 51, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 53, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 58, 2},
        {EPtiKeyQwertyT, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 61, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 63, 4},
        {EPtiKeyQwertyI, EPtiCaseUpper, 67, 3},
        {EPtiKeyQwertyO, EPtiCaseUpper, 70, 3},
        {EPtiKeyQwertyP, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 74, 5},
        {EPtiKeyQwertyS, EPtiCaseUpper, 79, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 83, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 87, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 89, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 89, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 91, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 95, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 97, 1},
        {EStdKeySpace, EPtiCaseUpper, 98, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 98, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 99, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 100, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 101, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 102, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 103, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 104, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 105, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 106, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 107, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 108, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 109, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 110, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 125, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 126, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnUpper, 127, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Estonian = 86;

//
// Class implementation.
//

CPtiLangDataImplEstonian* CPtiLangDataImplEstonian::NewL()
    {
    return new (ELeave) CPtiLangDataImplEstonian();
    }


TInt CPtiLangDataImplEstonian::LanguageCode() const
    {
    return ELangEstonian;
    }


const TUint16* CPtiLangDataImplEstonian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Estonian;
        return MultitapData_table_Estonian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Estonian;
        return StandardQwertyData_table_Estonian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Estonian;
        return MiniQwertyData_table_Estonian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Estonian;
        return MiniQwerty3x11Data_table_Estonian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplEstonian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Estonian;
        return MultitapKeyBindings_table_Estonian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Estonian;
        return StandardQwertyKeyBindings_table_Estonian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Estonian;
        return MiniQwertyKeyBindings_table_Estonian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Estonian;
        return MiniQwerty3x11KeyBindings_table_Estonian;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplEstonian::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_Estonian;
        return StandardQwertyDeadKeyData_table_Estonian;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


