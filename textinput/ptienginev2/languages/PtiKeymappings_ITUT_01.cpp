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

#include "PtiKeymappings_ITUT_01.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_English[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe4,0xe6,0xe5,0xe0,0xe1,0xe2,0xe3,0xe7,
     0x64,0x65,0x66,0x33,0xe8,0xe9,0xea,0xeb,0xf0,
     0x67,0x68,0x69,0x34,0xec,0xed,0xee,0xef,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf6,0xf8,0xf2,0xf3,0xf4,0xf5,0xf1,
     0x70,0x71,0x72,0x73,0x37,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xf9,0xfa,0xfb,0xfc,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0xfe,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc4,0xc6,0xc5,0xc0,0xc1,0xc2,0xc3,0xc7,
     0x44,0x45,0x46,0x33,0xc8,0xc9,0xca,0xcb,0xd0,
     0x47,0x48,0x49,0x34,0xcc,0xcd,0xce,0xcf,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd6,0xd8,0xd2,0xd3,0xd4,0xd5,0xd1,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xd9,0xda,0xdb,0xdc,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0xde,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_English = 199;


const TPtiKeyBinding MultitapKeyBindings_table_English[] =
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

const TInt KMaxMultitapIndex_English = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_English[] =
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
     0x2b,
     0x71,
     0x77,
     0x65,0xe8,0xe9,0xea,0xeb,
     0x72,
     0x74,0xfe,
     0x79,0xfd,
     0x75,0xf9,0xfa,0xfb,0xfc,
     0x69,0xec,0xed,0xee,0xef,
     0x6f,0xf6,0xf8,0xf2,0xf3,0xf4,0xf5,
     0x70,
     0x2d,
     0x23,
     0x61,0xe4,0xe6,0xe5,0xe0,0xe1,0xe2,0xe3,
     0x73,0xdf,
     0x64,0xf0,
     0x66,
     0x67,
     0x68,
     0x6a,
     0x6b,
     0x6c,
     0x3b,
     0x27,
     0x7a,
     0x78,
     0x63,0xe7,
     0x76,
     0x62,
     0x6e,0xf1,
     0x6d,
     0x2c,
     0x2e,
     0x2f,
     0x21,
     0x22,
     0xa3,
     0x24,
     0x20ac,
     0x25,
     0x26,
     0x2a,
     0x28,
     0x29,
     0x3d,
     0x51,
     0x57,
     0x45,0xc8,0xc9,0xca,0xcb,
     0x52,
     0x54,0xde,
     0x59,0xdd,
     0x55,0xd9,0xda,0xdb,0xdc,
     0x49,0xcc,0xcd,0xce,0xcf,
     0x4f,0xd6,0xd8,0xd2,0xd3,0xd4,0xd5,
     0x50,
     0x5f,
     0x7e,
     0x41,0xc4,0xc6,0xc5,0xc0,0xc1,0xc2,0xc3,
     0x53,
     0x44,0xd0,
     0x46,
     0x47,
     0x48,
     0x4a,
     0x4b,
     0x4c,
     0x3a,
     0x40,
     0x5a,
     0x58,
     0x43,0xc7,
     0x56,
     0x42,
     0x4e,0xd1,
     0x4d,
     0x3c,
     0x3e,
     0x3f,
    };

const TInt KStandardQwertyDataSize_English = 149;


const TPtiKeyBinding StandardQwertyKeyBindings_table_English[] =
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
        {EPtiKeyQwertyO, EPtiCaseLower, 33, 7},
        {EPtiKeyQwertyP, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 43, 8},
        {EPtiKeyQwertyS, EPtiCaseLower, 51, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 53, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 61, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 64, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 65, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 67, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 68, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 69, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 71, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 72, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 73, 1},
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
        {EPtiKeyQwertyR, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 94, 2},
        {EPtiKeyQwertyY, EPtiCaseUpper, 96, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 98, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 103, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 108, 7},
        {EPtiKeyQwertyP, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 118, 8},
        {EPtiKeyQwertyS, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 127, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 129, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 130, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 131, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 132, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 133, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 134, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 135, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 136, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 137, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 138, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 139, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 141, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 142, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 143, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 145, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 146, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 147, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 148, 1},
    };

const TInt KMaxStandardQwertyIndex_English = 88;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_English[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E8,0x00E9,0x00EA,0x00EB,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00F9,0x00FA,0x00FB,0x00FC,
     0x0069,0x00EC,0x00ED,0x00EE,0x00EF,
     0x006F,0x00F4,0x00F6,0x00F2,0x00F3,0x00F8,
     0x0070,
     0x0061,0x00E2,0x00E4,0x00E0,0x00E1,0x00E3,0x00E6,
     0x0073,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,0x011F,
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
     0x002C,
     0x002E,
     0x0040,
     0x0026,
     0x0027,
     0x003F,
     0x0051,
     0x0057,
     0x0045,0x00C8,0x00C9,0x00CA,0x00CB,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00D9,0x00DA,0x00DB,0x00DC,
     0x0049,0x00CC,0x00CD,0x00CE,0x00CF,
     0x004F,0x00D4,0x00D6,0x00D2,0x00D3,0x00D8,
     0x0050,
     0x0041,0x00C2,0x00C4,0x00C0,0x00C1,0x00C3,0x00C6,
     0x0053,
     0x0044,0x00D0,
     0x0046,
     0x0047,0x011E,
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

const TInt KMiniQwertyDataSize_English = 152;


const TPtiKeyBinding MiniQwertyKeyBindings_table_English[] =
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
        {EPtiKeyQwertyA, EPtiCaseLower, 29, 7},
        {EPtiKeyQwertyS, EPtiCaseLower, 36, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 38, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 41, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 53, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 57, 1},
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
        {EPtiKeyQwertyA, EPtiCaseUpper, 91, 7},
        {EPtiKeyQwertyS, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 99, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 102, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 110, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 114, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 145, 1},
    };

const TInt KMaxMiniQwertyIndex_English = 83;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_English[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E8,0x00E9,0x00EA,0x00EB,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FB,0x00F9,0x00FA,0x00FC,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x00F6,0x00F4,0x00F2,0x00F3,
     0x0070,
     0x0061,0x00E4,0x00E0,0x00E2,0x00E1,0x00E3,0x00E5,
     0x0073,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,0x011F,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x002E,
     0x007A,
     0x0078,
     0x0063,0x00E7,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x0051,
     0x0057,
     0x0045,0x00C8,0x00C9,0x00CA,0x00CB,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00DB,0x00D9,0x00DA,0x00DC,
     0x0049,0x00CE,0x00CF,0x00CC,0x00CD,
     0x004F,0x00D6,0x00D4,0x00D2,0x00D3,
     0x0050,
     0x0041,0x00C4,0x00C0,0x00C2,0x00C1,0x00C3,0x00C5,
     0x0053,
     0x0044,0x00D0,
     0x0046,
     0x0047,0x011E,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x002C,
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
     0x002D,
     0x002B,
     0x003D,
     0x0023,
     0x002A,
     0x003B,
     0x003A,
     0x0027,
     0x0028,
     0x0029,
     0x0025,
     0x0026,
     0x0022,
     0x003F,
     0x0021,
    };

const TInt KMiniQwerty3x11DataSize_English = 138;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_English[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 7, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 8, 2},
        {EPtiKeyQwertyY, EPtiCaseLower, 10, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 12, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 17, 5},
        {EPtiKeyQwertyO, EPtiCaseLower, 22, 5},
        {EPtiKeyQwertyP, EPtiCaseLower, 27, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 28, 7},
        {EPtiKeyQwertyS, EPtiCaseLower, 35, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 53, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 55, 1},
        {EStdKeySpace, EPtiCaseLower, 56, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 58, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 64, 2},
        {EPtiKeyQwertyY, EPtiCaseUpper, 66, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 68, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 73, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 78, 5},
        {EPtiKeyQwertyP, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 84, 7},
        {EPtiKeyQwertyS, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 92, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 95, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 104, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 108, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 110, 1},
        {EStdKeySpace, EPtiCaseUpper, 111, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 137, 1},
    };

const TInt KMaxMiniQwerty3x11Index_English = 83;

//
// HalfQwerty keymap data table
//
const TUint16 HalfQwertyData_table_English[] =
    {
     0x0071,0x0077,
     0x0065,0x0072,0x00E8,0x00E9,0x00EA,0x00EB,
     0x0074,0x0079,0x00FE,0x00FD,
     0x0075,0x0069,0x00FB,0x00F9,0x00FA,0x00FC,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x0070,0x00F6,0x00F4,0x00F2,0x00F3,
     0x0061,0x0073,0x00E4,0x00E0,0x00E2,0x00E1,0x00E3,0x00E5,0x00DF,
     0x0064,0x0066,0x00F0,
     0x0067,0x0068,0x011F,
     0x006A,0x006B,
     0x006C,
     0x007A,0x0078,
     0x0063,0x0076,0x00E7,
     0x0062,0x006E,0x00F1,
     0x006D,
     0x002E,0x002C,0x003A,0x003B,
     0x0020,
     0x0051,0x0057,
     0x0045,0x0052,0x00C8,0x00C9,0x00CA,0x00CB,
     0x0054,0x0059,0x00DE,0x00DD,
     0x0055,0x0049,0x00DB,0x00D9,0x00DA,0x00DC,0x00CE,0x00CF,0x00CC,0x00CD,
     0x004F,0x0050,0x00D6,0x00D4,0x00D2,0x00D3,
     0x0041,0x0053,0x00C4,0x00C0,0x00C2,0x00C1,0x00C3,0x00C5,
     0x0044,0x0046,0x00D0,
     0x0047,0x0048,0x011E,
     0x004A,0x004B,
     0x004C,
     0x005A,0x0058,
     0x0043,0x0056,0x00C7,
     0x0042,0x004E,0x00D1,
     0x004D,
     0x003A,0x003B,0x002E,0x002C,
     0x0020,
     0x00E8,0x00E9,0x00EA,0x00EB,
     0x00FE,0x00FD,
     0x00FB,0x00F9,0x00FA,0x00FC,0x00EE,0x00EF,0x00EC,0x00ED,
     0x00F6,0x00F4,0x00F2,0x00F3,
     0x00E4,0x00E0,0x00E2,0x00E1,0x00E3,0x00E5,0x00DF,
     0x00F0,
     0x011F,
     0x00E7,
     0x00F1,
     0x00C8,0x00C9,0x00CA,0x00CB,
     0x00DE,0x00DD,
     0x00DB,0x00D9,0x00DA,0x00DC,0x00CE,0x00CF,0x00CC,0x00CD,
     0x00D6,0x00D4,0x00D2,0x00D3,
     0x00C4,0x00C0,0x00C2,0x00C1,0x00C3,0x00C5,
     0x00D0,
     0x011E,
     0x00C7,
     0x00D1,
     0x0021,
     0x0031,
     0x0032,
     0x0033,
     0x002D,
     0x003F,
     0x0034,
     0x0035,
     0x0036,
     0x0027,
     0x0040,
     0x0037,
     0x0038,
     0x0039,
     0x003B,
     0x002A,0x002B,
     0x0030,
     0x0023,
    };

const TInt KHalfQwertyDataSize_English = 195;


const TPtiKeyBinding HalfQwertyKeyBindings_table_English[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 2},
        {EPtiKeyQwerty1, EPtiCaseLower, 2, 6},
        {EPtiKeyQwerty2, EPtiCaseLower, 8, 4},
        {EPtiKeyQwerty3, EPtiCaseLower, 12, 10},
        {EPtiKeyQwertyO, EPtiCaseLower, 22, 6},
        {EPtiKeyQwertyA, EPtiCaseLower, 28, 9},
        {EPtiKeyQwerty4, EPtiCaseLower, 37, 3},
        {EPtiKeyQwerty5, EPtiCaseLower, 40, 3},
        {EPtiKeyQwerty6, EPtiCaseLower, 43, 2},
        {EPtiKeyQwertyL, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 46, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 48, 3},
        {EPtiKeyQwerty8, EPtiCaseLower, 51, 3},
        {EPtiKeyQwerty9, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 55, 4},
        {EPtiKeyQwertyChr, EPtiCaseLower, 59, 0},
        {EPtiKeyQwertySpace, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseLower, 60, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 60, 2},
        {EPtiKeyQwerty1, EPtiCaseUpper, 62, 6},
        {EPtiKeyQwerty2, EPtiCaseUpper, 68, 4},
        {EPtiKeyQwerty3, EPtiCaseUpper, 72, 10},
        {EPtiKeyQwertyO, EPtiCaseUpper, 82, 6},
        {EPtiKeyQwertyA, EPtiCaseUpper, 88, 8},
        {EPtiKeyQwerty4, EPtiCaseUpper, 96, 3},
        {EPtiKeyQwerty5, EPtiCaseUpper, 99, 3},
        {EPtiKeyQwerty6, EPtiCaseUpper, 102, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 105, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 107, 3},
        {EPtiKeyQwerty8, EPtiCaseUpper, 110, 3},
        {EPtiKeyQwerty9, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 114, 4},
        {EPtiKeyQwertyChr, EPtiCaseUpper, 118, 0},
        {EPtiKeyQwertySpace, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseUpper, 119, 0},
        {EPtiKeyQwerty1, EPtiCaseChrLower, 119, 4},
        {EPtiKeyQwerty2, EPtiCaseChrLower, 123, 2},
        {EPtiKeyQwerty3, EPtiCaseChrLower, 125, 8},
        {EPtiKeyQwertyO, EPtiCaseChrLower, 133, 4},
        {EPtiKeyQwertyA, EPtiCaseChrLower, 137, 7},
        {EPtiKeyQwerty4, EPtiCaseChrLower, 144, 1},
        {EPtiKeyQwerty5, EPtiCaseChrLower, 145, 1},
        {EPtiKeyQwerty7, EPtiCaseChrLower, 146, 1},
        {EPtiKeyQwerty8, EPtiCaseChrLower, 147, 1},
        {EPtiKeyQwerty1, EPtiCaseChrUpper, 148, 4},
        {EPtiKeyQwerty2, EPtiCaseChrUpper, 152, 2},
        {EPtiKeyQwerty3, EPtiCaseChrUpper, 154, 8},
        {EPtiKeyQwertyO, EPtiCaseChrUpper, 162, 4},
        {EPtiKeyQwertyA, EPtiCaseChrUpper, 166, 6},
        {EPtiKeyQwerty4, EPtiCaseChrUpper, 172, 1},
        {EPtiKeyQwerty5, EPtiCaseChrUpper, 173, 1},
        {EPtiKeyQwerty7, EPtiCaseChrUpper, 174, 1},
        {EPtiKeyQwerty8, EPtiCaseChrUpper, 175, 1},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 176, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 177, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 178, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 179, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 180, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 181, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 182, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 183, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 184, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 185, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 186, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 187, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 188, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 189, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 190, 1},
        {EPtiKeyQwertyChr, EPtiCaseFnLower, 191, 2},
        {EPtiKeyQwertySpace, EPtiCaseFnLower, 193, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseFnLower, 194, 1},
    };

const TInt KMaxHalfQwertyIndex_English = 72;

//
// CustomQwerty keymap data table
//
const TUint16 CustomQwertyData_table_English[] =
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
     0x002b,
     0x0023,
     0x002a,
     0x003f,
     0x002e,
     0x002c,
     0x0070,
     0x0077,
    };

const TInt KCustomQwertyDataSize_English = 56;


const TPtiKeyBinding CustomQwertyKeyBindings_table_English[] =
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
        { EPtiKeyQwertyA, EPtiCaseLower, 12, 1},
        { EPtiKeyQwertyB, EPtiCaseLower, 13, 1},
        { EPtiKeyQwertyC, EPtiCaseLower, 14, 1},
        { EPtiKeyQwertyD, EPtiCaseLower, 15, 1},
        { EPtiKeyQwertyE, EPtiCaseLower, 16, 1},
        { EPtiKeyQwertyF, EPtiCaseLower, 17, 1},
        { EPtiKeyQwertyG, EPtiCaseLower, 18, 1},
        { EPtiKeyQwertyH, EPtiCaseLower, 19, 1},
        { EPtiKeyQwertyI, EPtiCaseLower, 20, 1},
        { EPtiKeyQwertyJ, EPtiCaseLower, 21, 1},
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
        { EPtiKeyQwertyA, EPtiCaseUpper, 36, 1},
        { EPtiKeyQwertyB, EPtiCaseUpper, 37, 1},
        { EPtiKeyQwertyC, EPtiCaseUpper, 38, 1},
        { EPtiKeyQwertyD, EPtiCaseUpper, 39, 1},
        { EPtiKeyQwertyE, EPtiCaseUpper, 40, 1},
        { EPtiKeyQwertyF, EPtiCaseUpper, 41, 1},
        { EPtiKeyQwertyG, EPtiCaseUpper, 42, 1},
        { EPtiKeyQwertyH, EPtiCaseUpper, 43, 1},
        { EPtiKeyQwertyI, EPtiCaseUpper, 44, 1},
        { EPtiKeyQwertyJ, EPtiCaseUpper, 45, 1},
        { EPtiKeyQwerty1, EPtiCaseFnLower, 48, 1},
        { EPtiKeyQwerty2, EPtiCaseFnLower, 49, 1},
        { EPtiKeyQwerty3, EPtiCaseFnLower, 50, 1},
        { EPtiKeyQwerty4, EPtiCaseFnLower, 51, 1},
        { EPtiKeyQwerty5, EPtiCaseFnLower, 52, 1},
        { EPtiKeyQwerty6, EPtiCaseFnLower, 53, 1},
        { EPtiKeyQwerty7, EPtiCaseFnLower, 54, 1},
        { EPtiKeyQwerty8, EPtiCaseFnLower, 55, 1},
    };

const TInt KMaxCustomQwertyIndex_English = 48;

//
// Class implementation.
//

CPtiLangDataImplEnglish* CPtiLangDataImplEnglish::NewL()
    {
    return new (ELeave) CPtiLangDataImplEnglish();
    }


TInt CPtiLangDataImplEnglish::LanguageCode() const
    {
    return ELangEnglish;
    }


const TUint16* CPtiLangDataImplEnglish::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_English;
        return MultitapData_table_English;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_English;
        return StandardQwertyData_table_English;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_English;
        return MiniQwertyData_table_English;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_English;
        return MiniQwerty3x11Data_table_English;
        }

    if (aKeyboardType == EPtiKeyboardHalfQwerty)
        {
        aDataSize = KHalfQwertyDataSize_English;
        return HalfQwertyData_table_English;
        }

    if (aKeyboardType == EPtiKeyboardCustomQwerty)
        {
        aDataSize = KCustomQwertyDataSize_English;
        return CustomQwertyData_table_English;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplEnglish::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_English;
        return MultitapKeyBindings_table_English;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_English;
        return StandardQwertyKeyBindings_table_English;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_English;
        return MiniQwertyKeyBindings_table_English;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_English;
        return MiniQwerty3x11KeyBindings_table_English;
        }
    if (aKeyboardType == EPtiKeyboardHalfQwerty)
        {
        aNumItems = KMaxHalfQwertyIndex_English;
        return HalfQwertyKeyBindings_table_English;
        }
    if (aKeyboardType == EPtiKeyboardCustomQwerty)
        {
        aNumItems = KMaxCustomQwertyIndex_English;
        return CustomQwertyKeyBindings_table_English;
        }

    aNumItems = 0;
    return NULL;
    }


