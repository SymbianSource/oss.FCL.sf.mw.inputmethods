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

#include "PtiKeymappings_ITUT_13.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Portuguese[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe3,0xe1,0xe0,0xe2,0xaa,0xe4,0xe5,0xe6,0xe7,
     0x64,0x65,0x66,0x33,0xe9,0xea,0xe8,0xeb,0xf0,
     0x67,0x68,0x69,0x34,0xed,0xec,0xee,0xef,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf5,0xf3,0xf4,0xba,0xf2,0xf6,0xf8,0xf1,
     0x70,0x71,0x72,0x73,0x37,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xfa,0xfc,0xf9,0xfb,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0xfe,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc3,0xc1,0xc0,0xc2,0xaa,0xc4,0xc5,0xc6,0xc7,
     0x44,0x45,0x46,0x33,0xc9,0xca,0xc8,0xcb,0xd0,
     0x47,0x48,0x49,0x34,0xcd,0xcc,0xce,0xcf,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd5,0xd3,0xd4,0xba,0xd2,0xd6,0xd8,0xd1,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xda,0xdc,0xd9,0xdb,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0xde,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Portuguese = 203;


const TPtiKeyBinding MultitapKeyBindings_table_Portuguese[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 13},
        {EPtiKey3, EPtiCaseLower, 43, 9},
        {EPtiKey4, EPtiCaseLower, 52, 8},
        {EPtiKey5, EPtiCaseLower, 60, 5},
        {EPtiKey6, EPtiCaseLower, 65, 12},
        {EPtiKey7, EPtiCaseLower, 77, 7},
        {EPtiKey8, EPtiCaseLower, 84, 8},
        {EPtiKey9, EPtiCaseLower, 92, 7},
        {EPtiKeyStar, EPtiCaseLower, 99, 0},
        {EPtiKey0, EPtiCaseLower, 99, 3},
        {EPtiKeyHash, EPtiCaseLower, 102, 0},
        {EPtiKey1, EPtiCaseUpper, 102, 30},
        {EPtiKey2, EPtiCaseUpper, 132, 13},
        {EPtiKey3, EPtiCaseUpper, 145, 9},
        {EPtiKey4, EPtiCaseUpper, 154, 8},
        {EPtiKey5, EPtiCaseUpper, 162, 5},
        {EPtiKey6, EPtiCaseUpper, 167, 12},
        {EPtiKey7, EPtiCaseUpper, 179, 6},
        {EPtiKey8, EPtiCaseUpper, 185, 8},
        {EPtiKey9, EPtiCaseUpper, 193, 7},
        {EPtiKeyStar, EPtiCaseUpper, 200, 0},
        {EPtiKey0, EPtiCaseUpper, 200, 3},
        {EPtiKeyHash, EPtiCaseUpper, 203, 0},
    };

const TInt KMaxMultitapIndex_Portuguese = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Portuguese[] =
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
     0x003f,
     0x0071,
     0x0077,
     0x0065,0x00ea,0x00e9,0x00e8,0x00eb,
     0x0072,
     0x0074,0x00fe,
     0x0079,0x00fd,
     0x0075,0x00fa,0x00fb,0x00f9,0x00fc,
     0x0069,0x00ed,0x00ee,0x00ec,0x00ef,
     0x006f,0x00f5,0x00f4,0x00f3,0x00f2,0x00f6,0x00ba,0x00f8,
     0x0070,
     0xf001,0x00b4,
     0x00bf,
     0x0061,0x00e2,0x00e1,0x00e0,0x00e3,0x00aa,0x00e4,0x00e5,0x00e6,
     0x0073,0x00df,
     0x0064,0x00f0,
     0x0066,
     0x0067,
     0x0068,
     0x006a,
     0x006b,
     0x006c,
     0x00e7,0x00f1,
     0xf005,0x007e,
     0x007a,
     0x0078,
     0x0063,0x00e7,
     0x0076,
     0x0062,
     0x006e,0x00f1,
     0x006d,
     0x002c,
     0x002e,
     0x002d,
     0x0040,
     0x0022,
     0x0023,
     0x002b,
     0x003d,
     0x002a,
     0x002f,
     0x0028,
     0x0029,
     0x0027,
     0x0021,
     0x0051,
     0x0057,
     0x0045,0x00ca,0x00c9,0x00c8,0x00cb,
     0x0052,
     0x0054,0x00de,
     0x0059,0x00dd,
     0x0055,0x00da,0x00db,0x00d9,0x00dc,
     0x0049,0x00cd,0x00ce,0x00cc,0x00cf,
     0x004f,0x00d5,0x00d4,0x00d3,0x00d2,0x00d6,0x00ba,0x00d8,
     0x0050,
     0xf002,0x00a8,
     0x00a1,
     0x0041,0x00c2,0x00c1,0x00c0,0x00c3,0x00aa,0x00c4,0x00c5,0x00c6,
     0x0053,
     0x0044,0x00d0,
     0x0046,
     0x0047,
     0x0048,
     0x004a,
     0x004b,
     0x004c,
     0x00c7,0x00d1,
     0xf004,0x005e,
     0x005a,
     0x0058,
     0x0043,0x00c7,
     0x0056,
     0x0042,
     0x004e,0x00d1,
     0x004d,
     0x003b,
     0x003a,
     0xf003,0x0060,
    };

const TInt KStandardQwertyDataSize_Portuguese = 160;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Portuguese[] =
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
        {EPtiKeyQwertyMinus, EPtiCaseLower, 42, 2},
        {EPtiKeyQwertyHash, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 45, 9},
        {EPtiKeyQwertyS, EPtiCaseLower, 54, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 56, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 61, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 64, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 66, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 68, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 69, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 70, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 72, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 73, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 74, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 76, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 77, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 78, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 79, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 93, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 99, 2},
        {EPtiKeyQwertyY, EPtiCaseUpper, 101, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 103, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 108, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 113, 8},
        {EPtiKeyQwertyP, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 122, 2},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 124, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 125, 9},
        {EPtiKeyQwertyS, EPtiCaseUpper, 134, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 135, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 137, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 138, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 139, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 140, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 141, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 142, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 143, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 145, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 147, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 148, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 149, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 151, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 152, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 153, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 155, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 156, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 157, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 158, 2},
    };

const TInt KMaxStandardQwertyIndex_Portuguese = 88;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Portuguese[] =
    {
    13,  0x00E9, 0x00EB, 0x00E8, 0x00EA, 0xF000,
    16,  0x00FD, 0xF000, 0xF000, 0xF000, 0xF000,
    17,  0x00FA, 0x00FC, 0x00F9, 0x00FB, 0xF000,
    18,  0x00ED, 0x00EF, 0x00EC, 0x00EE, 0xF000,
    19,  0x00F3, 0x00F6, 0x00F2, 0x00F4, 0x00F5,
    23,  0x00E1, 0x00E4, 0x00E0, 0x00E2, 0x00E3,
    39,  0xF000, 0xF000, 0xF000, 0xF000, 0x00F1,
    57,  0x00C9, 0x00CB, 0x00C8, 0x00CA, 0xF000,
    60,  0x00DD, 0xF000, 0xF000, 0xF000, 0xF000,
    61,  0x00DA, 0x00DC, 0x00D9, 0x00DB, 0xF000,
    62,  0x00CD, 0x00CF, 0x00CC, 0x00CE, 0xF000,
    63,  0x00D3, 0x00D6, 0x00D2, 0x00D4, 0x00D5,
    67,  0x00C1, 0x00C4, 0x00C0, 0x00C2, 0x00C3,
    83,  0xF000, 0xF000, 0xF000, 0xF000, 0x00D1,
    };

const TInt KStandardQwertyDeadKeyTableRows_Portuguese = 14;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Portuguese[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00EA,0x00E9,0x00E8,0x00EB,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FA,0x00FB,0x00F9,0x00FC,
     0x0069,0x00ED,0x00EE,0x00EC,0x00EF,
     0x006F,0x00F5,0x00F4,0x00F3,0x00F2,0x00F6,0x00BA,0x00F8,
     0x0070,
     0x0061,0x00E2,0x00E1,0x00E0,0x00E3,0x00AA,0x00E4,0x00E5,0x00E6,
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
     0x002C,
     0x002E,
     0x0040,
     0x003F,
     0x00B4,
     0x007E,
     0x0051,
     0x0057,
     0x0045,0x00CA,0x00C9,0x00C8,0x00CB,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00DA,0x00DB,0x00D9,0x00DC,
     0x0049,0x00CD,0x00CE,0x00CC,0x00CF,
     0x004F,0x00D5,0x00D4,0x00D3,0x00D2,0x00D6,0x00BA,0x00D8,
     0x0050,
     0x0041,0x00C2,0x00C1,0x00C0,0x00C3,0x00AA,0x00C4,0x00C5,0x00C6,
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
     0x003B,
     0x003A,
     0x0060,
     0x005E,
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
     0x00E7,
     0x002D,
     0x003D,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x0027,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
     0x00C7,
    };

const TInt KMiniQwertyDataSize_Portuguese = 161;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Portuguese[] =
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
        {EPtiKeyQwertyC, EPtiCaseLower, 52, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 56, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 60, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 67, 5},
        {EPtiKeyQwerty4, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 73, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 75, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 77, 5},
        {EPtiKeyQwerty8, EPtiCaseUpper, 82, 5},
        {EPtiKeyQwerty9, EPtiCaseUpper, 87, 8},
        {EPtiKeyQwerty0, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 96, 9},
        {EPtiKeyQwertyS, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 106, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 116, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 119, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 120, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 123, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 124, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 149, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 150, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 151, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 152, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 153, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 160, 1},
    };

const TInt KMaxMiniQwertyIndex_Portuguese = 84;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Portuguese[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00EA,0x00E9,0x00E8,0x00EB,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FA,0x00FB,0x00F9,0x00FC,
     0x0069,0x00ED,0x00EE,0x00EC,0x00EF,
     0x006F,0x00F5,0x00F4,0x00F3,0x00F2,0x00F6,0x00BA,0x00F8,
     0x0070,
     0x0061,0x00E2,0x00E1,0x00E0,0x00E3,0x00AA,0x00E4,0x00E5,0x00E6,
     0x0073,0x00DF,
     0x0064,0x00F0,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x00E7,
     0x007A,
     0x0078,
     0x0063,0x00E7,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x0051,
     0x0057,
     0x0045,0x00CA,0x00C9,0x00C8,0x00CB,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00DA,0x00DB,0x00D9,0x00DC,
     0x0049,0x00CD,0x00CE,0x00CC,0x00CF,
     0x004F,0x00D5,0x00D4,0x00D3,0x00D2,0x00D6,0x00BA,0x00D8,
     0x0050,
     0x0041,0x00C2,0x00C1,0x00C0,0x00C3,0x00AA,0x00C4,0x00C5,0x00C6,
     0x0053,
     0x0044,0x00D0,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x00C7,
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
     0x003F,
     0x0021,
     0x0027,
     0x0028,
     0x0029,
     0x0025,
     0x002C,
     0x002E,
     0x003B,
     0x003A,
    };

const TInt KMiniQwerty3x11DataSize_Portuguese = 146;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Portuguese[] =
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
        {EPtiKeyQwertyC, EPtiCaseLower, 53, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 57, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 59, 1},
        {EStdKeySpace, EPtiCaseLower, 60, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 62, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 68, 2},
        {EPtiKeyQwertyY, EPtiCaseUpper, 70, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 72, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 77, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 82, 8},
        {EPtiKeyQwertyP, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 91, 9},
        {EPtiKeyQwertyS, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 101, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 112, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 116, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 118, 1},
        {EStdKeySpace, EPtiCaseUpper, 119, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 145, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Portuguese = 83;

//
// Class implementation.
//

CPtiLangDataImplPortuguese* CPtiLangDataImplPortuguese::NewL()
    {
    return new (ELeave) CPtiLangDataImplPortuguese();
    }


TInt CPtiLangDataImplPortuguese::LanguageCode() const
    {
    return ELangPortuguese;
    }


const TUint16* CPtiLangDataImplPortuguese::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Portuguese;
        return MultitapData_table_Portuguese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Portuguese;
        return StandardQwertyData_table_Portuguese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Portuguese;
        return MiniQwertyData_table_Portuguese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Portuguese;
        return MiniQwerty3x11Data_table_Portuguese;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplPortuguese::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Portuguese;
        return MultitapKeyBindings_table_Portuguese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Portuguese;
        return StandardQwertyKeyBindings_table_Portuguese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Portuguese;
        return MiniQwertyKeyBindings_table_Portuguese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Portuguese;
        return MiniQwerty3x11KeyBindings_table_Portuguese;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplPortuguese::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_Portuguese;
        return StandardQwertyDeadKeyData_table_Portuguese;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


