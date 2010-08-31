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

#include "PtiKeymappings_ITUT_07.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Danish[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe6,0xe5,0xe4,0xe0,0xe1,0xe2,0xe3,0xe7,
     0x64,0x65,0x66,0x33,0xe9,0xf0,0xe8,0xeb,0xea,
     0x67,0x68,0x69,0x34,0xee,0xef,0xec,0xed,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf8,0xf6,0xf4,0xf2,0xf3,0xf5,0xf1,
     0x70,0x71,0x72,0x73,0x37,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xfc,0xf9,0xfb,0xfa,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0xfe,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc6,0xc5,0xc4,0xc0,0xc1,0xc2,0xc3,0xc7,
     0x44,0x45,0x46,0x33,0xc9,0xd0,0xc8,0xcb,0xca,
     0x47,0x48,0x49,0x34,0xce,0xcf,0xcc,0xcd,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd8,0xd6,0xd4,0xd2,0xd3,0xd5,0xd1,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xdc,0xd9,0xdb,0xda,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0xde,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Danish = 199;


const TPtiKeyBinding MultitapKeyBindings_table_Danish[] =
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

const TInt KMaxMultitapIndex_Danish = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Danish[] =
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
     0x23,
     0x71,
     0x77,
     0x65,0xe9,0xe8,0xeb,0xea,
     0x72,
     0x74,0xfe,
     0x79,0xfd,
     0x75,0xfc,0xf9,0xfb,0xfa,
     0x69,0xee,0xef,0xec,0xed,
     0x6f,0xf6,0xf4,0xf2,0xf3,0xf5,
     0x70,
     0xe5,
     0x2b,
     0x61,0xe4,0xe0,0xe1,0xe2,0xe3,
     0x73,0xdf,
     0x64,0xf0,
     0x66,
     0x67,
     0x68,
     0x6a,
     0x6b,
     0x6c,
     0xf8,0xf6,
     0xe6,0xe4,
     0x7a,
     0x78,
     0x63,0xe7,
     0x76,
     0x62,
     0x6e,0xf1,
     0x6d,
     0x2c,
     0x2e,
     0x2d,
     0x21,
     0x22,
     0x40,
     0x20ac,
     0x25,
     0x26,
     0x2f,
     0x28,
     0x29,
     0x3f,
     0x3d,
     0x51,
     0x57,
     0x45,0xc9,0xc8,0xcb,0xca,
     0x52,
     0x54,0xde,
     0x59,0xdd,
     0x55,0xdc,0xd9,0xdb,0xda,
     0x49,0xce,0xcf,0xcc,0xcd,
     0x4f,0xd6,0xd4,0xd2,0xd3,0xd5,
     0x50,
     0xc5,
     0x2a,
     0x41,0xc4,0xc0,0xc1,0xc2,0xc3,
     0x53,
     0x44,0xd0,
     0x46,
     0x47,
     0x48,
     0x4a,
     0x4b,
     0x4c,
     0xd8,0xd6,
     0xc6,0xc4,
     0x5a,
     0x58,
     0x43,0xc7,
     0x56,
     0x42,
     0x4e,0xd1,
     0x4d,
     0x3b,
     0x3a,
     0x27,
    };

const TInt KStandardQwertyDataSize_Danish = 147;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Danish[] =
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
        {EPtiKeyQwertyR, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 19, 2},
        {EPtiKeyQwertyY, EPtiCaseLower, 21, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 23, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 28, 5},
        {EPtiKeyQwertyO, EPtiCaseLower, 33, 6},
        {EPtiKeyQwertyP, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 42, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 50, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 58, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 60, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 64, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 66, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 67, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 68, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 70, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 71, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 72, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 73, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 87, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 93, 2},
        {EPtiKeyQwertyY, EPtiCaseUpper, 95, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 97, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 102, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 107, 6},
        {EPtiKeyQwertyP, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 116, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 123, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 127, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 128, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 129, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 130, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 131, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 133, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 135, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 136, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 137, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 139, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 140, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 141, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 143, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 144, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 145, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 146, 1},
    };

const TInt KMaxStandardQwertyIndex_Danish = 88;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Danish[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00E8,0x00EB,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FC,0x00F9,0x00FB,0x00FA,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x00F6,0x00F4,0x00F2,0x00F3,0x00F5,
     0x0070,
     0x0061,0x00E4,0x00E0,0x00E1,0x00E2,0x00E3,
     0x0073,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x007A,
     0x0078,
     0x0063,0x00E7,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x00E6,0x00E4,
     0x00F8,0x00F6,
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
     0x004F,0x00D6,0x00D4,0x00D2,0x00D3,0x00D5,
     0x0050,
     0x0041,0x00C4,0x00C0,0x00C1,0x00C2,0x00C3,
     0x0053,
     0x0044,0x00D0,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x005A,
     0x0058,
     0x0043,0x00C7,
     0x0056,
     0x0042,
     0x004E,0x00D1,
     0x004D,
     0x00C6,0x00C4,
     0x00D8,0x00D6,
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

const TInt KMiniQwertyDataSize_Danish = 155;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Danish[] =
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
        {EPtiKeyQwertyS, EPtiCaseLower, 35, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 47, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 51, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 54, 2},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 56, 2},
        {EPtiKeyQwerty1, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 64, 5},
        {EPtiKeyQwerty4, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 70, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 72, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 74, 5},
        {EPtiKeyQwerty8, EPtiCaseUpper, 79, 5},
        {EPtiKeyQwerty9, EPtiCaseUpper, 84, 6},
        {EPtiKeyQwerty0, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 91, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 98, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 108, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 112, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 115, 2},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 117, 2},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnUpper, 154, 1},
    };

const TInt KMaxMiniQwertyIndex_Danish = 84;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Danish[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00E8,0x00EB,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FC,0x00F9,0x00FB,0x00FA,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x00F6,0x00F4,0x00F2,0x00F3,0x00F5,
     0x0070,
     0x0061,0x00E4,0x00E0,0x00E1,0x00E2,0x00E3,
     0x0073,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x002C,
     0x005A,
     0x0058,
     0x0043,0x00C7,
     0x0056,
     0x0042,
     0x004E,0x00D1,
     0x004D,
     0x0051,
     0x0057,
     0x0045,0x00C9,0x00C8,0x00CB,0x00CA,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00DC,0x00D9,0x00DB,0x00DA,
     0x0049,0x00CE,0x00CF,0x00CC,0x00CD,
     0x004F,0x00D6,0x00D4,0x00D2,0x00D3,0x00D5,
     0x0050,
     0x0041,0x00C4,0x00C0,0x00C1,0x00C2,0x00C3,
     0x0053,
     0x0044,0x00D0,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x002E,
     0x005A,
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
     0x002B,
     0x002D,
     0x002A,
     0x0023,
     0x003D,
     0x00E6,
     0x00F8,
     0x00E5,
     0x0025,
     0x0026,
     0x0022,
     0x003F,
     0x0021,
     0x003B,
     0x003A,
     0x00C6,
     0x00D8,
     0x00C5,
    };

const TInt KMiniQwerty3x11DataSize_Danish = 139;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Danish[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 7, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 8, 2},
        {EPtiKeyQwertyY, EPtiCaseLower, 10, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 12, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 17, 5},
        {EPtiKeyQwertyO, EPtiCaseLower, 22, 6},
        {EPtiKeyQwertyP, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 29, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 35, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 52, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 54, 1},
        {EStdKeySpace, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 57, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 63, 2},
        {EPtiKeyQwertyY, EPtiCaseUpper, 65, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 67, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 72, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 77, 6},
        {EPtiKeyQwertyP, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 84, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 91, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 102, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 106, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 108, 1},
        {EStdKeySpace, EPtiCaseUpper, 109, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 109, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 110, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 136, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 137, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnUpper, 138, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Danish = 86;

//
// Class implementation.
//

CPtiLangDataImplDanish* CPtiLangDataImplDanish::NewL()
    {
    return new (ELeave) CPtiLangDataImplDanish();
    }


TInt CPtiLangDataImplDanish::LanguageCode() const
    {
    return ELangDanish;
    }


const TUint16* CPtiLangDataImplDanish::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Danish;
        return MultitapData_table_Danish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Danish;
        return StandardQwertyData_table_Danish;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Danish;
        return MiniQwertyData_table_Danish;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Danish;
        return MiniQwerty3x11Data_table_Danish;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplDanish::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Danish;
        return MultitapKeyBindings_table_Danish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Danish;
        return StandardQwertyKeyBindings_table_Danish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Danish;
        return MiniQwertyKeyBindings_table_Danish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Danish;
        return MiniQwerty3x11KeyBindings_table_Danish;
        }

    aNumItems = 0;
    return NULL;
    }


