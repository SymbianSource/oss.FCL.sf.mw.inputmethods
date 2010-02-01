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

#include "PtiKeymappings_ITUT_05.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Italian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe0,0xe1,0xe4,0xe2,0xe3,0xe5,0xe6,0xe7,
     0x64,0x65,0x66,0x33,0xe8,0xe9,0xeb,0xea,0xf0,
     0x67,0x68,0x69,0x34,0xec,0xed,0xee,0xef,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf2,0xba,0xf3,0xf6,0xf4,0xf5,0xf8,0xf1,
     0x70,0x71,0x72,0x73,0x37,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xf9,0xfa,0xfc,0xfb,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0xfe,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc0,0xc1,0xc4,0xc2,0xc3,0xc5,0xc6,0xc7,
     0x44,0x45,0x46,0x33,0xc8,0xc9,0xcb,0xca,0xd0,
     0x47,0x48,0x49,0x34,0xcc,0xcd,0xce,0xcf,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd2,0xba,0xd3,0xd6,0xd4,0xd5,0xd8,0xd1,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xd9,0xda,0xdc,0xdb,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0xde,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Italian = 201;


const TPtiKeyBinding MultitapKeyBindings_table_Italian[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 12},
        {EPtiKey3, EPtiCaseLower, 42, 9},
        {EPtiKey4, EPtiCaseLower, 51, 8},
        {EPtiKey5, EPtiCaseLower, 59, 5},
        {EPtiKey6, EPtiCaseLower, 64, 12},
        {EPtiKey7, EPtiCaseLower, 76, 7},
        {EPtiKey8, EPtiCaseLower, 83, 8},
        {EPtiKey9, EPtiCaseLower, 91, 7},
        {EPtiKeyStar, EPtiCaseLower, 98, 0},
        {EPtiKey0, EPtiCaseLower, 98, 3},
        {EPtiKeyHash, EPtiCaseLower, 101, 0},
        {EPtiKey1, EPtiCaseUpper, 101, 30},
        {EPtiKey2, EPtiCaseUpper, 131, 12},
        {EPtiKey3, EPtiCaseUpper, 143, 9},
        {EPtiKey4, EPtiCaseUpper, 152, 8},
        {EPtiKey5, EPtiCaseUpper, 160, 5},
        {EPtiKey6, EPtiCaseUpper, 165, 12},
        {EPtiKey7, EPtiCaseUpper, 177, 6},
        {EPtiKey8, EPtiCaseUpper, 183, 8},
        {EPtiKey9, EPtiCaseUpper, 191, 7},
        {EPtiKeyStar, EPtiCaseUpper, 198, 0},
        {EPtiKey0, EPtiCaseUpper, 198, 3},
        {EPtiKeyHash, EPtiCaseUpper, 201, 0},
    };

const TInt KMaxMultitapIndex_Italian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Italian[] =
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
     0x3f,
     0x71,
     0x77,
     0x65,0xe8,0xe9,0xeb,0xea,
     0x72,
     0x74,0xfe,
     0x79,0xfd,
     0x75,0xf9,0xfc,0xfb,0xfa,
     0x69,0xec,0xee,0xef,0xed,
     0x6f,0xf3,0xf2,0xba,0xf6,0xf4,0xf5,0xf8,
     0x70,
     0xe8,
     0x2b,
     0x61,0xe0,0xaa,0xe4,0xe1,0xe2,0xe3,0xe5,0xe6,
     0x73,0xdf,
     0x64,0xf0,
     0x66,
     0x67,
     0x68,
     0x6a,
     0x6b,
     0x6c,
     0xf2,
     0xe0,
     0x7a,
     0x78,
     0x63,
     0x76,
     0x62,
     0x6e,
     0x6d,
     0x2c,
     0x2e,
     0x2d,
     0x21,
     0x22,
     0x23,
     0x20ac,
     0x25,
     0x26,
     0x2f,
     0x28,
     0x29,
     0x27,
     0x40,
     0x51,
     0x57,
     0x45,0xc8,0xc9,0xcb,0xca,
     0x52,
     0x54,0xde,
     0x59,0xdd,
     0x55,0xd9,0xdc,0xdb,0xda,
     0x49,0xcc,0xce,0xcf,0xcd,
     0x4f,0xd3,0xd2,0xba,0xd6,0xd4,0xd5,0xd8,
     0x50,
     0xe9,
     0x2a,
     0x41,0xc0,0xaa,0xc4,0xc1,0xc2,0xc3,0xc5,0xc6,
     0x53,
     0x44,0xd0,
     0x46,
     0x47,
     0x48,
     0x4a,
     0x4b,
     0x4c,
     0xec,
     0xf9,
     0x5a,
     0x58,
     0x43,
     0x56,
     0x42,
     0x4e,0xd1,
     0x4d,
     0x3b,
     0x3a,
     0x3d,
    };

const TInt KStandardQwertyDataSize_Italian = 150;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Italian[] =
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
        {EPtiKeyQwertyO, EPtiCaseLower, 33, 8},
        {EPtiKeyQwertyP, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 44, 9},
        {EPtiKeyQwertyS, EPtiCaseLower, 53, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 55, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 61, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 64, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 65, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 66, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 67, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 68, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 69, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 70, 1},
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
        {EPtiKeyQwertyO, EPtiCaseUpper, 108, 8},
        {EPtiKeyQwertyP, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 119, 9},
        {EPtiKeyQwertyS, EPtiCaseUpper, 128, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 129, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 131, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 132, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 133, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 134, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 135, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 136, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 137, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 138, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 139, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 140, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 141, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 142, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 143, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 144, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 146, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 147, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 148, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 149, 1},
    };

const TInt KMaxStandardQwertyIndex_Italian = 88;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Italian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E8,0x00E9,0x00EB,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00F9,0x00FC,0x00FB,0x00FA,
     0x0069,0x00EC,0x00EE,0x00EF,0x00ED,
     0x006F,0x00F3,0x00F2,0x00BA,0x00F6,0x00F4,0x00F5,0x00F8,
     0x0070,
     0x0061,0x00E0,0x00AA,0x00E4,0x00E1,0x00E2,0x00E3,0x00E5,0x00E6,
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
     0x0063,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x003F,
     0x0027,
     0x0021,
     0x0051,
     0x0057,
     0x0045,0x00C8,0x00C9,0x00CB,0x00CA,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00D9,0x00DC,0x00DB,0x00DA,
     0x0049,0x00CC,0x00CE,0x00CF,0x00CD,
     0x004F,0x00D3,0x00D2,0x00BA,0x00D6,0x00D4,0x00D5,0x00D8,
     0x0050,
     0x0041,0x00C0,0x00AA,0x00C4,0x00C1,0x00C2,0x00C3,0x00C5,0x00C6,
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
     0x0043,
     0x0056,
     0x0042,
     0x004E,0x00D1,
     0x004D,
     0x003B,
     0x003A,
     0x00EC,
     0x00E8,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x002B,
     0x00E9,
     0x00F9,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x002D,
     0x00F2,
     0x00E0,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x002F,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
     0x00CC,
     0x00C8,
     0x00C9,
     0x00D9,
     0x00D2,
     0x00C0,
    };

const TInt KMiniQwertyDataSize_Italian = 161;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Italian[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 5},
        {EPtiKeyQwerty4, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 8, 2},
        {EPtiKeyQwerty6, EPtiCaseLower, 10, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 12, 5},
        {EPtiKeyQwerty8, EPtiCaseLower, 17, 5},
        {EPtiKeyQwerty9, EPtiCaseLower, 22, 8},
        {EPtiKeyQwerty0, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 31, 9},
        {EPtiKeyQwertyS, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 42, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 52, 1},
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
        {EPtiKeyQwerty9, EPtiCaseUpper, 86, 8},
        {EPtiKeyQwerty0, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 95, 9},
        {EPtiKeyQwertyS, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 105, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 115, 1},
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
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwerty1, EPtiCaseFnUpper, 155, 1},
        {EPtiKeyQwerty2, EPtiCaseFnUpper, 156, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 157, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 158, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 159, 1},
        {EPtiKeyQwertyX, EPtiCaseFnUpper, 160, 1},
    };

const TInt KMaxMiniQwertyIndex_Italian = 88;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Italian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E8,0x00E9,0x00EB,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00F9,0x00FC,0x00FB,0x00FA,
     0x0069,0x00EC,0x00EE,0x00EF,0x00ED,
     0x006F,0x00F3,0x00F2,0x00BA,0x00F6,0x00F4,0x00F5,0x00F8,
     0x0070,
     0x0061,0x00E0,0x00AA,0x00E4,0x00E1,0x00E2,0x00E3,0x00E5,0x00E6,
     0x0073,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x002C,
     0x007A,
     0x0078,
     0x0063,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x0051,
     0x0057,
     0x0045,0x00C8,0x00C9,0x00CB,0x00CA,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00D9,0x00DC,0x00DB,0x00DA,
     0x0049,0x00CC,0x00CE,0x00CF,0x00CD,
     0x004F,0x00D3,0x00D2,0x00BA,0x00D6,0x00D4,0x00D5,0x00D8,
     0x0050,
     0x0041,0x00C0,0x00AA,0x00C4,0x00C1,0x00C2,0x00C3,0x00C5,0x00C6,
     0x0053,
     0x0044,0x00D0,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x002E,
     0x005A,
     0x0058,
     0x0043,
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
     0x0021,
     0x003F,
     0x003B,
     0x003A,
     0x00EC,
     0x00E8,
     0x00E9,
     0x00F9,
     0x00F2,
     0x00E0,
     0x0027,
     0x00CC,
     0x00C8,
     0x00C9,
     0x00D9,
     0x00D2,
     0x00C0,
    };

const TInt KMiniQwerty3x11DataSize_Italian = 149;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Italian[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 7, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 8, 2},
        {EPtiKeyQwertyY, EPtiCaseLower, 10, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 12, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 17, 5},
        {EPtiKeyQwertyO, EPtiCaseLower, 22, 8},
        {EPtiKeyQwertyP, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 31, 9},
        {EPtiKeyQwertyS, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 42, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 53, 1},
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
        {EPtiKeyQwertyY, EPtiCaseUpper, 69, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 71, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 76, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 81, 8},
        {EPtiKeyQwertyP, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 90, 9},
        {EPtiKeyQwertyS, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 100, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 107, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 113, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 115, 1},
        {EStdKeySpace, EPtiCaseUpper, 116, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 143, 1},
        {EPtiKeyQwertyX, EPtiCaseFnUpper, 144, 1},
        {EPtiKeyQwertyC, EPtiCaseFnUpper, 145, 1},
        {EPtiKeyQwertyV, EPtiCaseFnUpper, 146, 1},
        {EPtiKeyQwertyB, EPtiCaseFnUpper, 147, 1},
        {EPtiKeyQwertyN, EPtiCaseFnUpper, 148, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Italian = 89;

//
// Class implementation.
//

CPtiLangDataImplItalian* CPtiLangDataImplItalian::NewL()
    {
    return new (ELeave) CPtiLangDataImplItalian();
    }


TInt CPtiLangDataImplItalian::LanguageCode() const
    {
    return ELangItalian;
    }


const TUint16* CPtiLangDataImplItalian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Italian;
        return MultitapData_table_Italian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Italian;
        return StandardQwertyData_table_Italian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Italian;
        return MiniQwertyData_table_Italian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Italian;
        return MiniQwerty3x11Data_table_Italian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplItalian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Italian;
        return MultitapKeyBindings_table_Italian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Italian;
        return StandardQwertyKeyBindings_table_Italian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Italian;
        return MiniQwertyKeyBindings_table_Italian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Italian;
        return MiniQwerty3x11KeyBindings_table_Italian;
        }

    aNumItems = 0;
    return NULL;
    }


