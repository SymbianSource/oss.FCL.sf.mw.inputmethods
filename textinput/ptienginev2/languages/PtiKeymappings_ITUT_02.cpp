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

#include "PtiKeymappings_ITUT_02.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_French[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe0,0xe2,0xe6,0xe7,0xe4,0xe1,0xe3,
     0x64,0x65,0x66,0x33,0xe9,0xe8,0xea,0xeb,
     0x67,0x68,0x69,0x34,0xef,0xee,0xec,0xed,0x11f,0x131,
     0x6a,0x6b,0x6c,0x35,
     0x6d,0x6e,0x6f,0x36,0xf4,0x153,0xf6,0xf1,0xf2,0xf3,0xf8,
     0x70,0x71,0x72,0x73,0x37,0x24,0xdf,
     0x74,0x75,0x76,0x38,0xf9,0xfb,0xfc,0xfa,
     0x77,0x78,0x79,0x7a,0x39,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc0,0xc2,0xc6,0xc7,0xc4,0xc1,0xc3,
     0x44,0x45,0x46,0x33,0xc9,0xc8,0xca,0xcb,
     0x47,0x48,0x49,0x34,0xcf,0xce,0xcc,0xcd,0x11e,0x130,
     0x4a,0x4b,0x4c,0x35,
     0x4d,0x4e,0x4f,0x36,0xd4,0x152,0xd6,0xd1,0xd2,0xd3,0xd8,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xd9,0xdb,0xdc,0xda,
     0x57,0x58,0x59,0x5a,0x39,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_French = 193;


const TPtiKeyBinding MultitapKeyBindings_table_French[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 11},
        {EPtiKey3, EPtiCaseLower, 41, 8},
        {EPtiKey4, EPtiCaseLower, 49, 10},
        {EPtiKey5, EPtiCaseLower, 59, 4},
        {EPtiKey6, EPtiCaseLower, 63, 11},
        {EPtiKey7, EPtiCaseLower, 74, 7},
        {EPtiKey8, EPtiCaseLower, 81, 8},
        {EPtiKey9, EPtiCaseLower, 89, 5},
        {EPtiKeyStar, EPtiCaseLower, 94, 0},
        {EPtiKey0, EPtiCaseLower, 94, 3},
        {EPtiKeyHash, EPtiCaseLower, 97, 0},
        {EPtiKey1, EPtiCaseUpper, 97, 30},
        {EPtiKey2, EPtiCaseUpper, 127, 11},
        {EPtiKey3, EPtiCaseUpper, 138, 8},
        {EPtiKey4, EPtiCaseUpper, 146, 10},
        {EPtiKey5, EPtiCaseUpper, 156, 4},
        {EPtiKey6, EPtiCaseUpper, 160, 11},
        {EPtiKey7, EPtiCaseUpper, 171, 6},
        {EPtiKey8, EPtiCaseUpper, 177, 8},
        {EPtiKey9, EPtiCaseUpper, 185, 5},
        {EPtiKeyStar, EPtiCaseUpper, 190, 0},
        {EPtiKey0, EPtiCaseUpper, 190, 3},
        {EPtiKeyHash, EPtiCaseUpper, 193, 0},
    };

const TInt KMaxMultitapIndex_French = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_French[] =
    {
     0x26,
     0xe9,
     0x22,
     0x27,
     0x28,
     0x2d,
     0xe8,
     0x29,
     0xe7,
     0xe0,
     0x40,
     0x61,0xe6,0xe2,0xe0,0xe1,0xe3,0xe4,
     0x7a,
     0x65,0xeb,0xea,0xe8,0xe9,
     0x72,
     0x74,
     0x79,
     0x75,0xf9,0xfb,0xfa,0xfc,
     0x69,0xef,0xee,0xec,0xed,0x131,
     0x6f,0x153,0xf6,0xf4,0xf2,0xf3,0xf8,
     0x70,
     0xf001,0x5e,
     0x2b,
     0x71,
     0x73,0xdf,
     0x64,
     0x66,
     0x67,0x11f,
     0x68,
     0x6a,
     0x6b,
     0x6c,
     0x6d,
     0xf9,
     0x77,
     0x78,
     0x63,0xe7,
     0x76,
     0x62,
     0x6e,0xf1,
     0x2c,
     0x3b,
     0x3a,
     0x21,
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
     0x20ac,
     0x41,0xc6,0xc2,0xc0,0xc1,0xc3,0xc4,
     0x5a,
     0x45,0xcb,0xca,0xc8,0xc9,
     0x52,
     0x54,
     0x59,
     0x55,0xd9,0xdb,0xda,0xdc,
     0x49,0xcf,0xce,0xcc,0xcd,0x130,
     0x4f,0x152,0xd6,0xd4,0xd2,0xd3,0xd8,
     0x50,
     0xf002,0xa8,
     0x23,
     0x51,
     0x53,
     0x44,
     0x46,
     0x47,0x11e,
     0x48,
     0x4a,
     0x4b,
     0x4c,
     0x4d,
     0x2a,
     0x57,
     0x58,
     0x43,0xc7,
     0x56,
     0x42,
     0x4e,0xd1,
     0x3f,
     0x2e,
     0x2f,
     0x25,
    };

const TInt KStandardQwertyDataSize_French = 147;


const TPtiKeyBinding StandardQwertyKeyBindings_table_French[] =
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
        {EPtiKeyQwertyQ, EPtiCaseLower, 11, 7},
        {EPtiKeyQwertyW, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 19, 5},
        {EPtiKeyQwertyR, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 27, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 32, 6},
        {EPtiKeyQwertyO, EPtiCaseLower, 38, 7},
        {EPtiKeyQwertyP, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 46, 2},
        {EPtiKeyQwertyHash, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 50, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 54, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 61, 1},
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
        {EPtiKeyQwertyQ, EPtiCaseUpper, 85, 7},
        {EPtiKeyQwertyW, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 93, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 101, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 106, 6},
        {EPtiKeyQwertyO, EPtiCaseUpper, 112, 7},
        {EPtiKeyQwertyP, EPtiCaseUpper, 119, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 120, 2},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 123, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 124, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 127, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 129, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 130, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 131, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 132, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 133, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 134, 1},
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

const TInt KMaxStandardQwertyIndex_French = 88;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_French[] =
    {
    11,  0x00e2,0x00e4,0xf000,0xf000,0xf000,
    13,  0x00ea,0x00eb,0xf000,0xf000,0xf000,
    17,  0x00fb,0x00fc,0xf000,0xf000,0xf000,
    18,  0x00ee,0x00ef,0xf000,0xf000,0xf000,
    19,  0x00f4,0x00f6,0xf000,0xf000,0xf000,
    55,  0x00c2,0x00c4,0xf000,0xf000,0xf000,
    57,  0x00ca,0x00cb,0xf000,0xf000,0xf000,
    61,  0x00db,0x00dc,0xf000,0xf000,0xf000,
    62,  0x00ce,0x00cf,0xf000,0xf000,0xf000,
    63,  0x00d4,0x00d6,0x0000,0x0000,0x0000,
    };

const TInt KStandardQwertyDeadKeyTableRows_French = 10;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_French[] =
    {
     0x0061,0x00E6,0x00E2,0x00E0,0x00E1,0x00E3,0x00E4,
     0x007A,
     0x0065,0x00EB,0x00EA,0x00E8,0x00E9,
     0x0072,
     0x0074,
     0x0079,
     0x0075,0x00F9,0x00FB,0x00FA,0x00FC,
     0x0069,0x00EF,0x00EE,0x00EC,0x00ED,0x0131,
     0x006F,0x0153,0x00F6,0x00F4,0x00F2,0x00F3,0x00F8,
     0x0070,
     0x0071,
     0x0073,0x00DF,
     0x0064,
     0x0066,
     0x0067,0x011F,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x0077,
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
     0x0041,0x00C6,0x00C2,0x00C0,0x00C1,0x00C3,0x00C4,
     0x005A,
     0x0045,0x00CB,0x00CA,0x00C8,0x00C9,
     0x0052,
     0x0054,
     0x0059,
     0x0055,0x00D9,0x00DB,0x00DA,0x00DC,
     0x0049,0x00CF,0x00CE,0x00CC,0x00CD,0x0130,
     0x004F,0x0152,0x00D6,0x00D4,0x00D2,0x00D3,0x00D8,
     0x0050,
     0x0051,
     0x0053,
     0x0044,
     0x0046,
     0x0047,0x011E,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x0057,
     0x0058,
     0x0043,0x00C7,
     0x0056,
     0x0042,
     0x004E,0x00D1,
     0x004D,
     0x003B,
     0x003A,
     0x00E9,
     0x00EA,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x00E8,
     0x00E7,
     0x00E0,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x002B,
     0x0021,
     0x002F,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x002D,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
     0x00C9,
     0x00CA,
     0x00C8,
     0x00C7,
     0x00C0,
    };

const TInt KMiniQwertyDataSize_French = 155;


const TPtiKeyBinding MiniQwertyKeyBindings_table_French[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 7},
        {EPtiKeyQwerty2, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 8, 5},
        {EPtiKeyQwerty4, EPtiCaseLower, 13, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 14, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 15, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 16, 5},
        {EPtiKeyQwerty8, EPtiCaseLower, 21, 6},
        {EPtiKeyQwerty9, EPtiCaseLower, 27, 7},
        {EPtiKeyQwerty0, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 36, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 52, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 56, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 61, 7},
        {EPtiKeyQwerty2, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 69, 5},
        {EPtiKeyQwerty4, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 77, 5},
        {EPtiKeyQwerty8, EPtiCaseUpper, 82, 6},
        {EPtiKeyQwerty9, EPtiCaseUpper, 88, 7},
        {EPtiKeyQwerty0, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 100, 2},
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
        {EPtiKeyQwertyComma, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwerty1, EPtiCaseFnUpper, 150, 1},
        {EPtiKeyQwerty2, EPtiCaseFnUpper, 151, 1},
        {EPtiKeyQwerty9, EPtiCaseFnUpper, 152, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 153, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 154, 1},
    };

const TInt KMaxMiniQwertyIndex_French = 88;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_French[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00E8,0x00EB,0x00EA,
     0x0072,
     0x0074,
     0x0079,
     0x0075,0x00FB,0x00F9,0x00FA,0x00FC,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,
     0x006F,0x00F4,0x0153,0x00F2,0x00F3,0x00F5,
     0x0070,
     0x0061,0x00E0,0x00E2,0x00E6,0x00E1,0x00E3,0x00E4,
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
     0x0045,0x00C9,0x00C8,0x00CB,0x00CA,
     0x0052,
     0x0054,
     0x0059,
     0x0055,0x00DB,0x00D9,0x00DA,0x00DC,
     0x0049,0x00CE,0x00CF,0x00CC,0x00CD,
     0x004F,0x00D4,0x0152,0x00D2,0x00D3,0x00D5,
     0x0050,
     0x0041,0x00C0,0x00C2,0x00C6,0x00C1,0x00C3,0x00C4,
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

const TInt KMiniQwerty3x11DataSize_French = 132;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_French[] =
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

const TInt KMaxMiniQwerty3x11Index_French = 83;

//
// Class implementation.
//

CPtiLangDataImplFrench* CPtiLangDataImplFrench::NewL()
    {
    return new (ELeave) CPtiLangDataImplFrench();
    }


TInt CPtiLangDataImplFrench::LanguageCode() const
    {
    return ELangFrench;
    }


const TUint16* CPtiLangDataImplFrench::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_French;
        return MultitapData_table_French;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_French;
        return StandardQwertyData_table_French;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_French;
        return MiniQwertyData_table_French;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_French;
        return MiniQwerty3x11Data_table_French;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplFrench::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_French;
        return MultitapKeyBindings_table_French;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_French;
        return StandardQwertyKeyBindings_table_French;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_French;
        return MiniQwertyKeyBindings_table_French;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_French;
        return MiniQwerty3x11KeyBindings_table_French;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplFrench::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_French;
        return StandardQwertyDeadKeyData_table_French;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


