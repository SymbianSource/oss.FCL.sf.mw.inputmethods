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

#include "PtiKeymappings_ITUT_18.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Dutch[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe4,0xe1,0xe0,0xe2,0xe3,0xe5,0xe6,0xe7,
     0x64,0x65,0x66,0x33,0xeb,0xe9,0xe8,0xea,0xf0,
     0x67,0x68,0x69,0x34,0xef,0xed,0xec,0xee,0x20,0x20,0x20,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf6,0xf3,0xf2,0xf4,0xf5,0xf8,0xf1,
     0x70,0x71,0x72,0x73,0x37,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xfc,0xfa,0xf9,0xfb,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0xfe,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc4,0xc1,0xc0,0xc2,0xc3,0xc5,0xc6,0xc7,
     0x44,0x45,0x46,0x33,0xcb,0xc9,0xc8,0xca,0xd0,
     0x47,0x48,0x49,0x34,0xcf,0xcd,0xcc,0xce,0x20,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd6,0xd3,0xd2,0xd4,0xd5,0xd8,0xd1,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xdc,0xda,0xd9,0xdb,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0xde,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Dutch = 203;


const TPtiKeyBinding MultitapKeyBindings_table_Dutch[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 12},
        {EPtiKey3, EPtiCaseLower, 42, 9},
        {EPtiKey4, EPtiCaseLower, 51, 11},
        {EPtiKey5, EPtiCaseLower, 62, 5},
        {EPtiKey6, EPtiCaseLower, 67, 11},
        {EPtiKey7, EPtiCaseLower, 78, 7},
        {EPtiKey8, EPtiCaseLower, 85, 8},
        {EPtiKey9, EPtiCaseLower, 93, 7},
        {EPtiKeyStar, EPtiCaseLower, 100, 0},
        {EPtiKey0, EPtiCaseLower, 100, 3},
        {EPtiKeyHash, EPtiCaseLower, 103, 0},
        {EPtiKey1, EPtiCaseUpper, 103, 30},
        {EPtiKey2, EPtiCaseUpper, 133, 12},
        {EPtiKey3, EPtiCaseUpper, 145, 9},
        {EPtiKey4, EPtiCaseUpper, 154, 9},
        {EPtiKey5, EPtiCaseUpper, 163, 5},
        {EPtiKey6, EPtiCaseUpper, 168, 11},
        {EPtiKey7, EPtiCaseUpper, 179, 6},
        {EPtiKey8, EPtiCaseUpper, 185, 8},
        {EPtiKey9, EPtiCaseUpper, 193, 7},
        {EPtiKeyStar, EPtiCaseUpper, 200, 0},
        {EPtiKey0, EPtiCaseUpper, 200, 3},
        {EPtiKeyHash, EPtiCaseUpper, 203, 0},
    };

const TInt KMaxMultitapIndex_Dutch = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Dutch[] =
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
     0x65,0xe9,0xe8,0xeb,0xea,
     0x72,
     0x74,0xfe,
     0x79,0xfd,
     0x75,0xfc,0xf9,0xfb,0xfa,
     0x69,0xee,0xef,0xec,0xed,
     0x6f,0xf6,0xf4,0xf2,0xf3,0xf5,0xf8,
     0x70,
     0x2d,
     0x23,
     0x61,0xe4,0xe0,0xe1,0xe2,0xe6,0xe3,0xe5,
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
     0x45,0xc9,0xc8,0xcb,0xca,
     0x52,
     0x54,0xde,
     0x59,0xdd,
     0x55,0xdc,0xd9,0xdb,0xda,
     0x49,0xce,0xcf,0xcc,0xcd,
     0x4f,0xd6,0xd4,0xd2,0xd3,0xd5,0xd8,
     0x50,
     0x5f,
     0x7e,
     0x41,0xc4,0xc0,0xc1,0xc2,0xc6,0xc3,0xc5,
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

const TInt KStandardQwertyDataSize_Dutch = 149;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Dutch[] =
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

const TInt KMaxStandardQwertyIndex_Dutch = 88;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Dutch[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00E8,0x00EB,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FC,0x00F9,0x00FB,0x00FA,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x00F6,0x00F4,0x00F2,0x00F3,0x00F5,0x00F8,
     0x0070,
     0x0061,0x00E4,0x00E0,0x00E1,0x00E2,0x00E6,0x00E3,0x00E5,
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
     0x0045,0x00C9,0x00C8,0x00CB,0x00CA,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00DC,0x00D9,0x00DB,0x00DA,
     0x0049,0x00CE,0x00CF,0x00CC,0x00CD,
     0x004F,0x00D6,0x00D4,0x00D2,0x00D3,0x00D5,0x00D8,
     0x0050,
     0x0041,0x00C4,0x00C0,0x00C1,0x00C2,0x00C6,0x00C3,0x00C5,
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

const TInt KMiniQwertyDataSize_Dutch = 156;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Dutch[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 5},
        {EPtiKeyQwerty4, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 8, 2},
        {EPtiKeyQwerty6, EPtiCaseLower, 10, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 12, 5},
        {EPtiKeyQwerty8, EPtiCaseLower, 17, 5},
        {EPtiKeyQwerty9, EPtiCaseLower, 22, 7},
        {EPtiKeyQwerty0, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 30, 8},
        {EPtiKeyQwertyS, EPtiCaseLower, 38, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 43, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 51, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 55, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 59, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 66, 5},
        {EPtiKeyQwerty4, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 72, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 74, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 76, 5},
        {EPtiKeyQwerty8, EPtiCaseUpper, 81, 5},
        {EPtiKeyQwerty9, EPtiCaseUpper, 86, 7},
        {EPtiKeyQwerty0, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 94, 8},
        {EPtiKeyQwertyS, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 103, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 106, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 114, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 118, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 149, 1},
    };

const TInt KMaxMiniQwertyIndex_Dutch = 83;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Dutch[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00EB,0x00E9,0x00E8,0x00EA,
     0x0072,
     0x0074,
     0x0079,
     0x0075,0x00FB,0x00F9,0x00FC,0x00FA,
     0x0069,0x00EF,0x00EE,0x00EC,0x00ED,
     0x006F,0x00F4,0x00F2,0x00F6,0x00F3,0x00F5,
     0x0070,
     0x0061,0x00E0,0x00E2,0x00E6,0x00E4,0x00E1,0x00E3,
     0x0073,0x00DF,
     0x0064,
     0x0066,
     0x0067,
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
     0x0045,0x00CB,0x00C9,0x00C8,0x00CA,
     0x0052,
     0x0054,
     0x0059,
     0x0055,0x00DB,0x00D9,0x00DC,0x00DA,
     0x0049,0x00CF,0x00CE,0x00CC,0x00CD,
     0x004F,0x00D4,0x00D2,0x00D6,0x00D3,0x00D5,
     0x0050,
     0x0041,0x00C0,0x00C2,0x00C6,0x00C4,0x00C1,0x00C3,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
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

const TInt KMiniQwerty3x11DataSize_Dutch = 132;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Dutch[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 7, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 8, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 9, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 10, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 15, 5},
        {EPtiKeyQwertyO, EPtiCaseLower, 20, 6},
        {EPtiKeyQwertyP, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 27, 7},
        {EPtiKeyQwertyS, EPtiCaseLower, 34, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 46, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 50, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 52, 1},
        {EStdKeySpace, EPtiCaseLower, 53, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 55, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 63, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 68, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 73, 6},
        {EPtiKeyQwertyP, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 80, 7},
        {EPtiKeyQwertyS, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 98, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 102, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 104, 1},
        {EStdKeySpace, EPtiCaseUpper, 105, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 105, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 106, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 107, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 108, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 109, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 110, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 131, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Dutch = 83;

//
// Class implementation.
//

CPtiLangDataImplDutch* CPtiLangDataImplDutch::NewL()
    {
    return new (ELeave) CPtiLangDataImplDutch();
    }


TInt CPtiLangDataImplDutch::LanguageCode() const
    {
    return ELangDutch;
    }


const TUint16* CPtiLangDataImplDutch::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Dutch;
        return MultitapData_table_Dutch;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Dutch;
        return StandardQwertyData_table_Dutch;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Dutch;
        return MiniQwertyData_table_Dutch;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Dutch;
        return MiniQwerty3x11Data_table_Dutch;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplDutch::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Dutch;
        return MultitapKeyBindings_table_Dutch;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Dutch;
        return StandardQwertyKeyBindings_table_Dutch;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Dutch;
        return MiniQwertyKeyBindings_table_Dutch;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Dutch;
        return MiniQwerty3x11KeyBindings_table_Dutch;
        }

    aNumItems = 0;
    return NULL;
    }


