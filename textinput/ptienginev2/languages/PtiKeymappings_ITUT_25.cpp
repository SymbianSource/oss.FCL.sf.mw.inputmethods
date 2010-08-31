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

#include "PtiKeymappings_ITUT_25.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Czech[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe1,0x10d,0xe2,0xe4,0x103,0x105,0x107,
     0x64,0x65,0x66,0x33,0x10f,0xe9,0x11b,0x111,0x119,
     0x67,0x68,0x69,0x34,0xed,0xee,
     0x6a,0x6b,0x6c,0x35,0x13a,0x13e,0x142,0xa3,
     0x6d,0x6e,0x6f,0x36,0x148,0xf3,0x144,0xf4,0xf6,0x151,
     0x70,0x71,0x72,0x73,0x37,0x159,0x161,0x155,0x15b,0x15f,0xdf,0x24,
     0x74,0x75,0x76,0x38,0x165,0xfa,0x16f,0x163,0xfc,0x171,
     0x77,0x78,0x79,0x7a,0x39,0x17e,0xfd,0x17a,0x17c,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc1,0x10c,0xc2,0xc4,0x102,0x104,0x106,
     0x44,0x45,0x46,0x33,0x10e,0xc9,0x11a,0x110,0x118,
     0x47,0x48,0x49,0x34,0xcd,0xce,
     0x4a,0x4b,0x4c,0x35,0x139,0x13d,0x141,0xa3,
     0x4d,0x4e,0x4f,0x36,0x147,0xd3,0x143,0xd4,0xd6,0x150,
     0x50,0x51,0x52,0x53,0x37,0x158,0x160,0x154,0x15a,0x15e,0x24,
     0x54,0x55,0x56,0x38,0x164,0xda,0x16e,0x162,0xdc,0x170,
     0x57,0x58,0x59,0x5a,0x39,0x17d,0xdd,0x179,0x17b,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Czech = 215;


const TPtiKeyBinding MultitapKeyBindings_table_Czech[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 11},
        {EPtiKey3, EPtiCaseLower, 41, 9},
        {EPtiKey4, EPtiCaseLower, 50, 6},
        {EPtiKey5, EPtiCaseLower, 56, 8},
        {EPtiKey6, EPtiCaseLower, 64, 10},
        {EPtiKey7, EPtiCaseLower, 74, 12},
        {EPtiKey8, EPtiCaseLower, 86, 10},
        {EPtiKey9, EPtiCaseLower, 96, 9},
        {EPtiKeyStar, EPtiCaseLower, 105, 0},
        {EPtiKey0, EPtiCaseLower, 105, 3},
        {EPtiKeyHash, EPtiCaseLower, 108, 0},
        {EPtiKey1, EPtiCaseUpper, 108, 30},
        {EPtiKey2, EPtiCaseUpper, 138, 11},
        {EPtiKey3, EPtiCaseUpper, 149, 9},
        {EPtiKey4, EPtiCaseUpper, 158, 6},
        {EPtiKey5, EPtiCaseUpper, 164, 8},
        {EPtiKey6, EPtiCaseUpper, 172, 10},
        {EPtiKey7, EPtiCaseUpper, 182, 11},
        {EPtiKey8, EPtiCaseUpper, 193, 10},
        {EPtiKey9, EPtiCaseUpper, 203, 9},
        {EPtiKeyStar, EPtiCaseUpper, 212, 0},
        {EPtiKey0, EPtiCaseUpper, 212, 3},
        {EPtiKeyHash, EPtiCaseUpper, 215, 0},
    };

const TInt KMaxMultitapIndex_Czech = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Czech[] =
    {
     0x23,
     0x161,
     0x10d,
     0x40,
     0x17e,
     0xfd,
     0xe1,
     0xed,
     0xe9,
     0x22,
     0x2b,
     0x71,
     0x77,
     0x65,0x11b,0xe9,0x119,
     0x72,0x159,0x155,
     0x74,0x165,0x163,
     0x79,0xfd,
     0x75,0x16f,0xfa,0xfc,0x171,0xf9,0xfb,
     0x69,0xed,0xee,
     0x6f,0xf3,0xf6,0x151,0xf4,
     0x70,
     0xe4,
     0x28,
     0x61,0xe1,0x103,0xe2,0x105,0xe4,
     0x73,0x161,0xdf,0x15b,0x15f,
     0x64,0x10f,0x111,
     0x66,
     0x67,
     0x68,
     0x6a,
     0x6b,
     0x6c,0x13e,0x13a,0x142,
     0xf001,0xb4,
     0xf002,0x2c7,
     0x7a,0x17e,0x17a,0x17c,
     0x78,
     0x63,0x10d,0x107,
     0x76,
     0x62,
     0x6e,0x148,0x144,
     0x6d,
     0x2c,
     0x2e,
     0x2d,
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
     0x2a,
     0x51,
     0x57,
     0x45,0x11a,0xc9,0x118,
     0x52,0x158,0x154,
     0x54,0x164,0x162,
     0x59,0xdd,
     0x55,0x16e,0xda,0xdc,0x170,0xd9,0xdb,
     0x49,0xcd,0xce,
     0x4f,0xd3,0xd6,0x150,0xd4,
     0x50,
     0x2f,
     0x29,
     0x41,0xc1,0x102,0xc2,0x104,0xc4,
     0x53,0x160,0x15a,0x15e,
     0x44,0x10e,0x110,
     0x46,
     0x47,
     0x48,
     0x4a,
     0x4b,
     0x4c,0x13d,0x139,0x141,
     0xf003,0xb0,
     0xf004,0x5e,
     0x5a,0x17d,0x179,0x17b,
     0x58,
     0x43,0x10c,0x106,
     0x56,
     0x42,
     0x4e,0x147,0x143,
     0x4d,
     0x3f,
     0x3a,
     0x21,
    };

const TInt KStandardQwertyDataSize_Czech = 173;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Czech[] =
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
        {EPtiKeyQwertyE, EPtiCaseLower, 13, 4},
        {EPtiKeyQwertyR, EPtiCaseLower, 17, 3},
        {EPtiKeyQwertyT, EPtiCaseLower, 20, 3},
        {EPtiKeyQwertyY, EPtiCaseLower, 23, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 25, 7},
        {EPtiKeyQwertyI, EPtiCaseLower, 32, 3},
        {EPtiKeyQwertyO, EPtiCaseLower, 35, 5},
        {EPtiKeyQwertyP, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 43, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 49, 5},
        {EPtiKeyQwertyD, EPtiCaseLower, 54, 3},
        {EPtiKeyQwertyF, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 61, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 62, 4},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 66, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 68, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 70, 4},
        {EPtiKeyQwertyX, EPtiCaseLower, 74, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 75, 3},
        {EPtiKeyQwertyV, EPtiCaseLower, 78, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 79, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 80, 3},
        {EPtiKeyQwertyM, EPtiCaseLower, 83, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 84, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 85, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 86, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 100, 4},
        {EPtiKeyQwertyR, EPtiCaseUpper, 104, 3},
        {EPtiKeyQwertyT, EPtiCaseUpper, 107, 3},
        {EPtiKeyQwertyY, EPtiCaseUpper, 110, 2},
        {EPtiKeyQwertyU, EPtiCaseUpper, 112, 7},
        {EPtiKeyQwertyI, EPtiCaseUpper, 119, 3},
        {EPtiKeyQwertyO, EPtiCaseUpper, 122, 5},
        {EPtiKeyQwertyP, EPtiCaseUpper, 127, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 128, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 129, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 130, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 136, 4},
        {EPtiKeyQwertyD, EPtiCaseUpper, 140, 3},
        {EPtiKeyQwertyF, EPtiCaseUpper, 143, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 144, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 145, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 146, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 147, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 148, 4},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 152, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 154, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 156, 4},
        {EPtiKeyQwertyX, EPtiCaseUpper, 160, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 161, 3},
        {EPtiKeyQwertyV, EPtiCaseUpper, 164, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 165, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 166, 3},
        {EPtiKeyQwertyM, EPtiCaseUpper, 169, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 170, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 171, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 172, 1},
    };

const TInt KMaxStandardQwertyIndex_Czech = 88;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Czech[] =
    {
    13,  0x00E9,0x011B,0xF000,0x00EA,0xf000,
    14,  0x0155,0x0159,0xF000,0xF000,0xf000,
    15,  0xF000,0x0165,0xF000,0xF000,0xf000,
    16,  0x00FD,0xF000,0xF000,0xF000,0xf000,
    17,  0x00FA,0xF000,0x016F,0xF000,0xf000,
    18,  0x00ED,0xF000,0xF000,0x00EE,0xf000,
    19,  0x00F3,0xF000,0xF000,0x00F4,0xf000,
    23,  0x00E1,0xF000,0x00E5,0x00E2,0xf000,
    24,  0x015B,0x0161,0xF000,0xF000,0xf000,
    25,  0xF000,0x010F,0xF000,0xF000,0xf000,
    31,  0x013A,0x013E,0xF000,0xF000,0xf000,
    34,  0x017A,0x017E,0xF000,0xF000,0xf000,
    36,  0x0107,0x010D,0xF000,0xF000,0xf000,
    39,  0x0144,0x0148,0xF000,0xF000,0xf000,
    57,  0x00C9,0x011A,0xF000,0x00CA,0xf000,
    58,  0x0154,0x0158,0xF000,0xF000,0xf000,
    59,  0xF000,0x0164,0xF000,0xF000,0xf000,
    60,  0x00DD,0xF000,0xF000,0xF000,0xf000,
    61,  0x00DA,0xF000,0x016E,0xF000,0xf000,
    62,  0x00CD,0xF000,0xF000,0x00CE,0xf000,
    63,  0x00D3,0xF000,0xF000,0x00D4,0xf000,
    67,  0x00C1,0xF000,0x00C5,0x00C2,0xf000,
    68,  0x015A,0x0160,0xF000,0xF000,0xf000,
    69,  0xF000,0x010E,0xF000,0xF000,0xf000,
    75,  0x0139,0x013D,0xF000,0xF000,0xf000,
    78,  0x0179,0x017D,0xF000,0xF000,0xf000,
    80,  0x0106,0x010C,0xF000,0xF000,0xf000,
    83,  0x0143,0x0147,0xF000,0xF000,0xf000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Czech = 28;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Czech[] =
	{
	{';',EPtiKeyQwertyFullstop,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Czech = 1;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Czech[] =
    {
     0x0071,
     0x0077,
     0x0065,0x011B,0x00E9,0x0119,
     0x0072,0x0159,0x0155,
     0x0074,0x0165,0x0163,
     0x007A,0x017E,0x017A,0x017C,
     0x0075,0x016F,0x00FA,0x00FC,0x0171,0x00F9,0x00FB,
     0x0069,0x00ED,0x00EE,
     0x006F,0x00F3,0x00F6,0x0151,0x00F4,
     0x0070,
     0x0061,0x00E1,0x0103,0x00E2,0x0105,0x00E4,
     0x0073,0x0161,0x00DF,0x015B,0x015F,
     0x0064,0x010F,0x0111,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,0x013E,0x013A,0x0142,
     0x0079,0x00FD,
     0x0078,
     0x0063,0x010D,0x0107,
     0x0076,
     0x0062,
     0x006E,0x0148,0x0144,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x003F,
     0x00B4,
     0x02C7,
     0x0051,
     0x0057,
     0x0045,0x011A,0x00C9,0x0118,
     0x0052,0x0158,0x0154,
     0x0054,0x0164,0x0162,
     0x005A,0x017D,0x0179,0x017B,
     0x0055,0x016E,0x00DA,0x00DC,0x0170,0x00D9,0x00DB,
     0x0049,0x00CD,0x00CE,
     0x004F,0x00D3,0x00D6,0x0150,0x00D4,
     0x0050,
     0x0041,0x00C1,0x0102,0x00C2,0x0104,0x00C4,
     0x0053,0x0160,0x015A,0x015E,
     0x0044,0x010E,0x0110,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,0x013D,0x0139,0x0141,
     0x0059,0x00DD,
     0x0058,
     0x0043,0x010C,0x0106,
     0x0056,
     0x0042,
     0x004E,0x0147,0x0143,
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
     0x016F,
     0x002F,
     0x0027,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x002B,
     0x0028,
     0x0029,
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
     0x016E,
    };

const TInt KMiniQwertyDataSize_Czech = 175;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Czech[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 4},
        {EPtiKeyQwerty4, EPtiCaseLower, 6, 3},
        {EPtiKeyQwerty5, EPtiCaseLower, 9, 3},
        {EPtiKeyQwerty6, EPtiCaseLower, 12, 4},
        {EPtiKeyQwerty7, EPtiCaseLower, 16, 7},
        {EPtiKeyQwerty8, EPtiCaseLower, 23, 3},
        {EPtiKeyQwerty9, EPtiCaseLower, 26, 5},
        {EPtiKeyQwerty0, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 32, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 38, 5},
        {EPtiKeyQwertyD, EPtiCaseLower, 43, 3},
        {EPtiKeyQwertyF, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 51, 4},
        {EPtiKeyQwertyZ, EPtiCaseLower, 55, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 58, 3},
        {EPtiKeyQwertyV, EPtiCaseLower, 61, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 63, 3},
        {EPtiKeyQwertyM, EPtiCaseLower, 66, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 67, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 68, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 75, 4},
        {EPtiKeyQwerty4, EPtiCaseUpper, 79, 3},
        {EPtiKeyQwerty5, EPtiCaseUpper, 82, 3},
        {EPtiKeyQwerty6, EPtiCaseUpper, 85, 4},
        {EPtiKeyQwerty7, EPtiCaseUpper, 89, 7},
        {EPtiKeyQwerty8, EPtiCaseUpper, 96, 3},
        {EPtiKeyQwerty9, EPtiCaseUpper, 99, 5},
        {EPtiKeyQwerty0, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 105, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 111, 4},
        {EPtiKeyQwertyD, EPtiCaseUpper, 115, 3},
        {EPtiKeyQwertyF, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 119, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 123, 4},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 127, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 129, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 130, 3},
        {EPtiKeyQwertyV, EPtiCaseUpper, 133, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 134, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 135, 3},
        {EPtiKeyQwertyM, EPtiCaseUpper, 138, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 139, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 140, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 149, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 150, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 151, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 152, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 153, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 154, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 155, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 156, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 157, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 158, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 159, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 160, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 161, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 162, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 163, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 164, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 165, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 166, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 167, 1},
        {EPtiKeyQwerty9, EPtiCaseFnUpper, 174, 1},
    };

const TInt KMaxMiniQwertyIndex_Czech = 84;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Czech[] =
    {
     0x0071,
     0x0077,
     0x0065,0x011B,0x00E9,0x0119,
     0x0072,0x0159,0x0155,
     0x0074,0x0165,0x0163,
     0x007A,0x017E,0x017A,0x017C,
     0x0075,0x016F,0x00FA,0x00FC,0x0171,0x00F9,0x00FB,
     0x0069,0x00ED,0x00EE,
     0x006F,0x00F3,0x00F6,0x0151,0x00F4,
     0x0070,
     0x0061,0x00E1,0x0103,0x00E2,0x0105,0x00E4,
     0x0073,0x0161,0x00DF,0x015B,0x015F,
     0x0064,0x010F,0x0111,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,0x013E,0x013A,0x0142,
     0x002C,
     0x0079,0x00FD,
     0x0078,
     0x0063,0x010D,0x0107,
     0x0076,
     0x0062,
     0x006E,0x0148,0x0144,
     0x006D,
     0x0051,
     0x0057,
     0x0045,0x011A,0x00C9,0x0118,
     0x0052,0x0158,0x0154,
     0x0054,0x0164,0x0162,
     0x005A,0x017D,0x0179,0x017B,
     0x0055,0x016E,0x00DA,0x00DC,0x0170,0x00D9,0x00DB,
     0x0049,0x00CD,0x00CE,
     0x004F,0x00D3,0x00D6,0x0150,0x00D4,
     0x0050,
     0x0041,0x00C1,0x0102,0x00C2,0x0104,0x00C4,
     0x0053,0x0160,0x015A,0x015E,
     0x0044,0x010E,0x0110,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x002E,
     0x0059,0x00DD,
     0x0058,
     0x0043,0x010C,0x0106,
     0x0056,
     0x0042,
     0x004E,0x0147,0x0143,
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
     0x011B,
     0x0161,
     0x010D,
     0x002D,
     0x002A,
     0x0023,
     0x002B,
     0x00ED,
     0x00E9,
     0x002F,
     0x0159,
     0x017E,
     0x00FD,
     0x00E1,
     0x00FA,
     0x016F,
     0x0040,
     0x0114,
     0x0160,
     0x010C,
     0x00CD,
     0x00C9,
     0x0158,
     0x017D,
     0x00DD,
     0x00C0,
     0x00DA,
     0x016E,
    };

const TInt KMiniQwerty3x11DataSize_Czech = 169;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Czech[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 1, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 2, 4},
        {EPtiKeyQwertyR, EPtiCaseLower, 6, 3},
        {EPtiKeyQwertyT, EPtiCaseLower, 9, 3},
        {EPtiKeyQwertyY, EPtiCaseLower, 12, 4},
        {EPtiKeyQwertyU, EPtiCaseLower, 16, 7},
        {EPtiKeyQwertyI, EPtiCaseLower, 23, 3},
        {EPtiKeyQwertyO, EPtiCaseLower, 26, 5},
        {EPtiKeyQwertyP, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 32, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 38, 5},
        {EPtiKeyQwertyD, EPtiCaseLower, 43, 3},
        {EPtiKeyQwertyF, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 51, 4},
        {EPtiKeyQwertyComma, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 56, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 59, 3},
        {EPtiKeyQwertyV, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 64, 3},
        {EPtiKeyQwertyM, EPtiCaseLower, 67, 1},
        {EStdKeySpace, EPtiCaseLower, 68, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 70, 4},
        {EPtiKeyQwertyR, EPtiCaseUpper, 74, 3},
        {EPtiKeyQwertyT, EPtiCaseUpper, 77, 3},
        {EPtiKeyQwertyY, EPtiCaseUpper, 80, 4},
        {EPtiKeyQwertyU, EPtiCaseUpper, 84, 7},
        {EPtiKeyQwertyI, EPtiCaseUpper, 91, 3},
        {EPtiKeyQwertyO, EPtiCaseUpper, 94, 5},
        {EPtiKeyQwertyP, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 100, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 106, 4},
        {EPtiKeyQwertyD, EPtiCaseUpper, 110, 3},
        {EPtiKeyQwertyF, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 118, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 119, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 122, 3},
        {EPtiKeyQwertyV, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 127, 3},
        {EPtiKeyQwertyM, EPtiCaseUpper, 130, 1},
        {EStdKeySpace, EPtiCaseUpper, 131, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 149, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 150, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 151, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 152, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 153, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 154, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 155, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 156, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 157, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 158, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 159, 1},
        {EPtiKeyQwertyD, EPtiCaseFnUpper, 160, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 161, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 162, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 163, 1},
        {EPtiKeyQwertyX, EPtiCaseFnUpper, 164, 1},
        {EPtiKeyQwertyC, EPtiCaseFnUpper, 165, 1},
        {EPtiKeyQwertyV, EPtiCaseFnUpper, 166, 1},
        {EPtiKeyQwertyB, EPtiCaseFnUpper, 167, 1},
        {EPtiKeyQwertyN, EPtiCaseFnUpper, 168, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Czech = 94;

//
// Class implementation.
//

CPtiLangDataImplCzech* CPtiLangDataImplCzech::NewL()
    {
    return new (ELeave) CPtiLangDataImplCzech();
    }


TInt CPtiLangDataImplCzech::LanguageCode() const
    {
    return ELangCzech;
    }


const TUint16* CPtiLangDataImplCzech::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Czech;
        return MultitapData_table_Czech;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Czech;
        return StandardQwertyData_table_Czech;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Czech;
        return MiniQwertyData_table_Czech;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Czech;
        return MiniQwerty3x11Data_table_Czech;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplCzech::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Czech;
        return MultitapKeyBindings_table_Czech;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Czech;
        return StandardQwertyKeyBindings_table_Czech;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Czech;
        return MiniQwertyKeyBindings_table_Czech;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Czech;
        return MiniQwerty3x11KeyBindings_table_Czech;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplCzech::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_Czech;
        return StandardQwertyDeadKeyData_table_Czech;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplCzech::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Czech;
        return StandardQwertyNumericKeysData_table_Czech;
        }

    aNumEntries = 0;
    return NULL;
    }


