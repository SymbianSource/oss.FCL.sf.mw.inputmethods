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

#include "PtiKeymappings_ITUT_17.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Hungarian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0xe1,0x62,0x63,0x32,0xe2,0xe4,0x103,0x105,0x107,0x10d,
     0x64,0x65,0xe9,0x66,0x33,0x10f,0x111,0x119,0x11b,
     0x67,0x68,0x69,0xed,0x34,0xee,0xef,0xec,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0xf3,0xf6,0x151,0x36,0x144,0x148,0xf4,
     0x70,0x71,0x72,0x73,0x37,0x155,0x159,0x15b,0x15f,0x161,0xdf,0x24,
     0x74,0x75,0xfa,0xfc,0x171,0x76,0x38,0x163,0x165,0x16f,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0x17a,0x17c,0x17e,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0xc1,0x42,0x43,0x32,0xc2,0xc4,0x102,0x104,0x106,0x10c,
     0x44,0x45,0xc9,0x46,0x33,0x10e,0x110,0x118,0x11a,
     0x47,0x48,0x49,0xcd,0x34,0xce,0xcf,0xcc,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0xd3,0xd6,0x150,0x36,0x143,0x147,0xd4,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0xda,0xdc,0x170,0x56,0x38,0x162,0x164,0x16e,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0x179,0x17b,0x17d,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Hungarian = 208;


const TPtiKeyBinding MultitapKeyBindings_table_Hungarian[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 11},
        {EPtiKey3, EPtiCaseLower, 41, 9},
        {EPtiKey4, EPtiCaseLower, 50, 8},
        {EPtiKey5, EPtiCaseLower, 58, 5},
        {EPtiKey6, EPtiCaseLower, 63, 10},
        {EPtiKey7, EPtiCaseLower, 73, 12},
        {EPtiKey8, EPtiCaseLower, 85, 10},
        {EPtiKey9, EPtiCaseLower, 95, 9},
        {EPtiKeyStar, EPtiCaseLower, 104, 0},
        {EPtiKey0, EPtiCaseLower, 104, 3},
        {EPtiKeyHash, EPtiCaseLower, 107, 0},
        {EPtiKey1, EPtiCaseUpper, 107, 30},
        {EPtiKey2, EPtiCaseUpper, 137, 11},
        {EPtiKey3, EPtiCaseUpper, 148, 9},
        {EPtiKey4, EPtiCaseUpper, 157, 8},
        {EPtiKey5, EPtiCaseUpper, 165, 5},
        {EPtiKey6, EPtiCaseUpper, 170, 10},
        {EPtiKey7, EPtiCaseUpper, 180, 6},
        {EPtiKey8, EPtiCaseUpper, 186, 10},
        {EPtiKey9, EPtiCaseUpper, 196, 9},
        {EPtiKeyStar, EPtiCaseUpper, 205, 0},
        {EPtiKey0, EPtiCaseUpper, 205, 3},
        {EPtiKeyHash, EPtiCaseUpper, 208, 0},
    };

const TInt KMaxMultitapIndex_Hungarian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Hungarian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00EB,0x00E9,
     0x0072,0x0155,
     0x0074,
     0x0079,0x00FF,0x00FD,
     0x0075,0x00FC,0x00FA,0x0171,
     0x0069,0x00EF,0x00ED,
     0x006F,0x00F6,0x00F3,0x0151,
     0x0070,
     0xF002,0x00B4,
     0x0061,0x00E1,
     0x0073,0x015B,
     0x0064,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,0x013A,
     0xF001,0x00A8,
     0xF003,0x02DD,
     0x007A,0x017A,
     0x0078,
     0x0063,0x0107,
     0x0076,
     0x0062,
     0x006E,0x0144,
     0x006D,
     0x002C,
     0x002E,
     0x003F,
     0x002D,
     0x0051,
     0x0057,
     0x0045,0x00CB,0x00C9,
     0x0052,0x0154,
     0x0054,
     0x0059,0x0178,0x00DD,
     0x0055,0x00DC,0x00DA,0x0170,
     0x0049,0x00CF,0x00CD,
     0x004F,0x00D6,0x00D3,0x0150,
     0x0050,
     0xF002,0x00B4,
     0x0041,0x00C1,
     0x0053,0x015A,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,0x0139,
     0xF001,0x00A8,
     0xF003,0x02DD,
     0x005A,0x0179,
     0x0058,
     0x0043,0x0106,
     0x0056,
     0x0042,
     0x004E,0x0143,
     0x004D,
     0x0027,
     0x002F,
     0x0021,
     0x002B,
    };

const TInt KStandardQwertyDataSize_Hungarian = 110;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Hungarian[] =
    {
        {EPtiKeyQwertyAtCharacter, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty1, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 3},
        {EPtiKeyQwerty3, EPtiCaseLower, 5, 2},
        {EPtiKeyQwerty4, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 8, 3},
        {EPtiKeyQwerty6, EPtiCaseLower, 11, 4},
        {EPtiKeyQwerty7, EPtiCaseLower, 15, 3},
        {EPtiKeyQwerty8, EPtiCaseLower, 18, 4},
        {EPtiKeyQwerty9, EPtiCaseLower, 22, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 23, 2},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 25, 2},
        {EPtiKeyQwertyQ, EPtiCaseLower, 27, 2},
        {EPtiKeyQwertyW, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 32, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 33, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 35, 2},
        {EPtiKeyQwertyO, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyP, EPtiCaseLower, 39, 2},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 41, 0},
        {EPtiKeyQwertyHash, EPtiCaseLower, 41, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 44, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 52, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 55, 0},
        {0x5b, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyZ, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyC, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyV, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyB, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyN, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyM, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyComma, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertySlash, EPtiCaseLower, 55, 0},
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 57, 3},
        {EPtiKeyQwerty3, EPtiCaseUpper, 60, 2},
        {EPtiKeyQwerty4, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 63, 3},
        {EPtiKeyQwerty6, EPtiCaseUpper, 66, 4},
        {EPtiKeyQwerty7, EPtiCaseUpper, 70, 3},
        {EPtiKeyQwerty8, EPtiCaseUpper, 73, 4},
        {EPtiKeyQwerty9, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 78, 2},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 80, 2},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 82, 2},
        {EPtiKeyQwertyW, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 90, 2},
        {EPtiKeyQwertyO, EPtiCaseUpper, 92, 2},
        {EPtiKeyQwertyP, EPtiCaseUpper, 94, 2},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 96, 0},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 96, 2},
        {EPtiKeyQwertyA, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 99, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 103, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 110, 0},
        {0x5b, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyX, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyC, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyV, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyB, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyM, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 110, 0},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 110, 0},
    };

const TInt KMaxStandardQwertyIndex_Hungarian = 92;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Hungarian[] =
    {
    2,  0x00EB,0x00E9,0xf000,0xf000,0xf000,
    3,  0xF000,0xF000,0x0155,0xf000,0xf000,
    5,  0x00FF,0x00FD,0xf000,0xf000,0xf000,
    6,  0x00FC,0x00FA,0x0171,0xf000,0xf000,
    7,  0x00EF,0x00ED,0xf000,0xf000,0xf000,
    8,  0x00F6,0x00F3,0x0151,0xf000,0xf000,
    11,  0xF000,0x00E1,0xf000,0xf000,0xf000,
    12,  0xF000,0xF000,0x015B,0xf000,0xf000,
    19,  0xF000,0x013A,0xf000,0xf000,0xf000,
    22,  0xF000,0x017A,0xf000,0xf000,0xf000,
    24,  0xF000,0x0107,0xf000,0xf000,0xf000,
    27,  0xF000,0x0144,0xf000,0xf000,0xf000,
    35,  0x00CB,0x00C9,0xf000,0xf000,0xf000,
    36,  0xF000,0xF000,0x0154,0xf000,0xf000,
    38,  0x0178,0x00DD,0xf000,0xf000,0xf000,
    39,  0x00DC,0x00DA,0x0170,0xf000,0xf000,
    40,  0x00CF,0x00CD,0xf000,0xf000,0xf000,
    41,  0x00D6,0x00D3,0x0150,0xf000,0xf000,
    44,  0xF000,0x00C1,0xf000,0xf000,0xf000,
    45,  0xF000,0xF000,0x015A,0xf000,0xf000,
    52,  0xF000,0x0139,0xf000,0xf000,0xf000,
    55,  0xF000,0x0179,0xf000,0xf000,0xf000,
    57,  0xF000,0x0106,0xf000,0xf000,0xf000,
    60,  0xF000,0x0143,0xf000,0xf000,0xf000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Hungarian = 24;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Hungarian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x0119,0x011B,0x00E9,
     0x0072,0x0155,0x0159,
     0x0074,0x0163,0x0165,
     0x0079,0x00FD,
     0x0075,0x00FA,0x016F,0x0171,
     0x0069,0x00ED,0x00EE,
     0x006F,0x00F3,0x00F4,0x0151,
     0x0070,
     0x0061,0x00E4,0x00E2,0x0103,0x0105,0x00E1,
     0x0073,0x00DF,0x015B,0x015F,0x0161,
     0x0064,0x0111,0x010F,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x007A,0x017A,0x017C,0x017E,
     0x0078,
     0x0063,0x0107,0x010D,
     0x0076,
     0x0062,
     0x006E,0x0144,0x0148,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x0026,
     0x0027,
     0x003F,
     0x0051,
     0x0057,
     0x0045,0x0118,0x011A,0x00C9,
     0x0052,0x0154,0x0158,
     0x0054,0x0162,0x0164,
     0x0059,0x00DD,
     0x0055,0x00DA,0x016E,0x0170,
     0x0049,0x00CD,0x00CE,
     0x004F,0x00D3,0x00D4,0x0150,
     0x0050,
     0x0041,0x00C4,0x00C2,0x0102,0x0104,0x00C1,
     0x0053,0x015A,0x015E,0x0160,
     0x0044,0x0110,0x010E,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x005A,0x0179,0x017B,0x017D,
     0x0058,
     0x0043,0x0106,0x010C,
     0x0056,
     0x0042,
     0x004E,0x0143,0x0147,
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

const TInt KMiniQwertyDataSize_Hungarian = 160;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Hungarian[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 4},
        {EPtiKeyQwerty4, EPtiCaseLower, 6, 3},
        {EPtiKeyQwerty5, EPtiCaseLower, 9, 3},
        {EPtiKeyQwerty6, EPtiCaseLower, 12, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 14, 4},
        {EPtiKeyQwerty8, EPtiCaseLower, 18, 3},
        {EPtiKeyQwerty9, EPtiCaseLower, 21, 4},
        {EPtiKeyQwerty0, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 26, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 32, 5},
        {EPtiKeyQwertyD, EPtiCaseLower, 37, 3},
        {EPtiKeyQwertyF, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 46, 4},
        {EPtiKeyQwertyX, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 51, 3},
        {EPtiKeyQwertyV, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 56, 3},
        {EPtiKeyQwertyM, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 61, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 68, 4},
        {EPtiKeyQwerty4, EPtiCaseUpper, 72, 3},
        {EPtiKeyQwerty5, EPtiCaseUpper, 75, 3},
        {EPtiKeyQwerty6, EPtiCaseUpper, 78, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 80, 4},
        {EPtiKeyQwerty8, EPtiCaseUpper, 84, 3},
        {EPtiKeyQwerty9, EPtiCaseUpper, 87, 4},
        {EPtiKeyQwerty0, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 92, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 98, 4},
        {EPtiKeyQwertyD, EPtiCaseUpper, 102, 3},
        {EPtiKeyQwertyF, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 109, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 111, 4},
        {EPtiKeyQwertyX, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 116, 3},
        {EPtiKeyQwertyV, EPtiCaseUpper, 119, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 121, 3},
        {EPtiKeyQwertyM, EPtiCaseUpper, 124, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 126, 1},
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
    };

const TInt KMaxMiniQwertyIndex_Hungarian = 83;

//
// Class implementation.
//

CPtiLangDataImplHungarian* CPtiLangDataImplHungarian::NewL()
    {
    return new (ELeave) CPtiLangDataImplHungarian();
    }


TInt CPtiLangDataImplHungarian::LanguageCode() const
    {
    return ELangHungarian;
    }


const TUint16* CPtiLangDataImplHungarian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Hungarian;
        return MultitapData_table_Hungarian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Hungarian;
        return StandardQwertyData_table_Hungarian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Hungarian;
        return MiniQwertyData_table_Hungarian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplHungarian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Hungarian;
        return MultitapKeyBindings_table_Hungarian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Hungarian;
        return StandardQwertyKeyBindings_table_Hungarian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Hungarian;
        return MiniQwertyKeyBindings_table_Hungarian;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplHungarian::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_Hungarian;
        return StandardQwertyDeadKeyData_table_Hungarian;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


