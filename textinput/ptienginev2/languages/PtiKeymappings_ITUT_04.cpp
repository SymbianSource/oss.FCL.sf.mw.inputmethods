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

#include "PtiKeymappings_ITUT_04.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Spanish[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x32,0xe1,0xe0,0xaa,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
     0x64,0x65,0x66,0x33,0xe9,0xe8,0xeb,0xea,0xf0,
     0x67,0x68,0x69,0x34,0xed,0xee,0xef,0xec,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf1,0xf3,0xba,0xf6,0xf4,0xf2,0xf5,0xf8,
     0x70,0x71,0x72,0x73,0x37,0xdf,0x24,
     0x74,0x75,0x76,0x38,0xfa,0xfc,0xf9,0xfb,
     0x77,0x78,0x79,0x7a,0x39,0xfd,0xfe,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x32,0xc1,0xc0,0xaa,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
     0x44,0x45,0x46,0x33,0xc9,0xc8,0xcb,0xca,0xd0,
     0x47,0x48,0x49,0x34,0xcd,0xce,0xcf,0xcc,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd1,0xd3,0xba,0xd6,0xd4,0xd2,0xd5,0xd8,
     0x50,0x51,0x52,0x53,0x37,0x24,
     0x54,0x55,0x56,0x38,0xda,0xdc,0xd9,0xdb,
     0x57,0x58,0x59,0x5a,0x39,0xdd,0xde,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Spanish = 203;


const TPtiKeyBinding MultitapKeyBindings_table_Spanish[] =
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

const TInt KMaxMultitapIndex_Spanish = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Spanish[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,
     0x0072,0x0155,
     0x0074,
     0x0079,0x00FD,
     0x0075,0x00FA,
     0x0069,0x00ED,
     0x006F,0x00F3,
     0x0070,
     0xF001,0x00B4,
     0x0061,0x00E1,
     0x0073,0x015B,
     0x0064,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,0x013A,
     0x00F1,
     0x00E7,
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
     0x0021,
     0x0051,
     0x0057,
     0x0045,0x00C9,
     0x0052,0x0154,
     0x0054,
     0x0059,0x00DD,
     0x0055,0x00DA,
     0x0049,0x00CD,
     0x004F,0x00D3,
     0x0050,
     0xF001,0x00B4,
     0x0041,0x00C1,
     0x0053,0x015A,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,0x0139,
     0x00D1,
     0x00C7,
     0x005A,0x0179,
     0x0058,
     0x0043,0x0106,
     0x0056,
     0x0042,
     0x004E,0x0143,
     0x004D,
     0x0027,
     0x002D,
     0x00BF,
     0x00A1,
    };

const TInt KStandardQwertyDataSize_Spanish = 92;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Spanish[] =
    {
        {EPtiKeyQwertyAtCharacter, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty1, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 2},
        {EPtiKeyQwerty3, EPtiCaseLower, 4, 2},
        {EPtiKeyQwerty4, EPtiCaseLower, 6, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 7, 2},
        {EPtiKeyQwerty6, EPtiCaseLower, 9, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 11, 2},
        {EPtiKeyQwerty8, EPtiCaseLower, 13, 2},
        {EPtiKeyQwerty9, EPtiCaseLower, 15, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 16, 2},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 18, 2},
        {EPtiKeyQwertyQ, EPtiCaseLower, 20, 2},
        {EPtiKeyQwertyW, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 27, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 28, 2},
        {EPtiKeyQwertyO, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 32, 0},
        {EPtiKeyQwertyHash, EPtiCaseLower, 32, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 35, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 39, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 46, 0},
        {0x5b, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyZ, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyC, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyV, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyB, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyN, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyM, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyComma, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertySlash, EPtiCaseLower, 46, 0},
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 48, 2},
        {EPtiKeyQwerty3, EPtiCaseUpper, 50, 2},
        {EPtiKeyQwerty4, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 53, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 55, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 57, 2},
        {EPtiKeyQwerty8, EPtiCaseUpper, 59, 2},
        {EPtiKeyQwerty9, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 62, 2},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 64, 2},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 66, 2},
        {EPtiKeyQwertyW, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 70, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 74, 2},
        {EPtiKeyQwertyO, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 78, 0},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 78, 2},
        {EPtiKeyQwertyA, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 81, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 85, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 92, 0},
        {0x5b, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyX, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyC, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyV, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyB, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyM, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 92, 0},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 92, 0},
    };

const TInt KMaxStandardQwertyIndex_Spanish = 92;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Spanish[] =
    {
    2,  0x00E9,0xf000,0xf000,0xf000,0xf000,
    3,  0x0155,0xf000,0xf000,0xf000,0xf000,
    5,  0x00FD,0xf000,0xf000,0xf000,0xf000,
    6,  0x00FA,0xf000,0xf000,0xf000,0xf000,
    7,  0x00ED,0xf000,0xf000,0xf000,0xf000,
    8,  0x00F3,0xf000,0xf000,0xf000,0xf000,
    11,  0x00E1,0xf000,0xf000,0xf000,0xf000,
    12,  0x015B,0xf000,0xf000,0xf000,0xf000,
    19,  0x013A,0xf000,0xf000,0xf000,0xf000,
    22,  0x017A,0xf000,0xf000,0xf000,0xf000,
    24,  0x0107,0xf000,0xf000,0xf000,0xf000,
    27,  0x0144,0xf000,0xf000,0xf000,0xf000,
    35,  0x00C9,0xf000,0xf000,0xf000,0xf000,
    36,  0x0154,0xf000,0xf000,0xf000,0xf000,
    38,  0x00DD,0xf000,0xf000,0xf000,0xf000,
    39,  0x00DA,0xf000,0xf000,0xf000,0xf000,
    40,  0x00CD,0xf000,0xf000,0xf000,0xf000,
    41,  0x00D3,0xf000,0xf000,0xf000,0xf000,
    44,  0x00C1,0xf000,0xf000,0xf000,0xf000,
    45,  0x015A,0xf000,0xf000,0xf000,0xf000,
    52,  0x0139,0xf000,0xf000,0xf000,0xf000,
    55,  0x0179,0xf000,0xf000,0xf000,0xf000,
    57,  0x0106,0xf000,0xf000,0xf000,0xf000,
    60,  0x0143,0xf000,0xf000,0xf000,0xf000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Spanish = 24;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Spanish[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00EB,0x00E8,0x00EA,
     0x0072,
     0x0074,0x00FE,
     0x0079,0x00FD,
     0x0075,0x00FC,0x00FA,0x00F9,0x00FB,
     0x0069,0x00ED,0x00EF,0x00EC,0x00EE,
     0x006F,0x00F3,0x00F6,0x00F2,0x00F4,0x00F5,0x00BA,0x00F8,
     0x0070,
     0x0061,0x00E1,0x00AA,0x00E4,0x00E0,0x00E2,0x00E3,0x00E5,0x00E6,
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
     0x00F1,
     0x00E7,
     0x0040,
     0x0027,
     0x002C,
     0x002E,
     0x0051,
     0x0057,
     0x0045,0x00C9,0x00CB,0x00C8,0x00CA,
     0x0052,
     0x0054,0x00DE,
     0x0059,0x00DD,
     0x0055,0x00DC,0x00DA,0x00D9,0x00DB,
     0x0049,0x00CD,0x00CF,0x00CC,0x00CE,
     0x004F,0x00D3,0x00D6,0x00D2,0x00D4,0x00D5,0x00BA,0x00D8,
     0x0050,
     0x0041,0x00C1,0x00AA,0x00C4,0x00C0,0x00C2,0x00C3,0x00C5,0x00C6,
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
     0x00D1,
     0x00C7,
     0x003B,
     0x003A,
     0x0021,
     0x003F,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x002B,
     0x00A1,
     0x00BF,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x002D,
     0x002F,
     0x0022,
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

const TInt KMiniQwertyDataSize_Spanish = 160;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Spanish[] =
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
    };

const TInt KMaxMiniQwertyIndex_Spanish = 83;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_Spanish[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x00E8,0x00EA,0x00EB,
     0x0072,
     0x0074,
     0x0079,
     0x0075,0x00FA,0x00FC,0x00F9,0x00FB,
     0x0069,0x00ED,0x00EF,0x00EC,0x00EE,
     0x006F,0x00F3,0x00BA,0x00F2,0x00F4,0x00F5,
     0x0070,
     0x0061,0x00E1,0x00AA,0x00E4,0x00E0,0x00E2,0x00E3,0x00E6,
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
     0x0045,0x00C9,0x00C8,0x00CA,0x00CB,
     0x0052,
     0x0054,
     0x0059,
     0x0055,0x00DA,0x00DC,0x00D9,0x00DB,
     0x0049,0x00CD,0x00CF,0x00CC,0x00CE,
     0x004F,0x00D3,0x00BA,0x00D2,0x00D4,0x00D5,
     0x0050,
     0x0041,0x00C1,0x00AA,0x00C4,0x00C0,0x00C2,0x00C3,0x00C6,
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

const TInt KMiniQwerty3x11DataSize_Spanish = 134;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_Spanish[] =
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
        {EPtiKeyQwertyA, EPtiCaseLower, 27, 8},
        {EPtiKeyQwertyS, EPtiCaseLower, 35, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 47, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 49, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 51, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 53, 1},
        {EStdKeySpace, EPtiCaseLower, 54, 0},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 56, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 64, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 69, 5},
        {EPtiKeyQwertyO, EPtiCaseUpper, 74, 6},
        {EPtiKeyQwertyP, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 81, 8},
        {EPtiKeyQwertyS, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 100, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 104, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 106, 1},
        {EStdKeySpace, EPtiCaseUpper, 107, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 107, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 108, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 109, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 110, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 133, 1},
    };

const TInt KMaxMiniQwerty3x11Index_Spanish = 83;

//
// Class implementation.
//

CPtiLangDataImplSpanish* CPtiLangDataImplSpanish::NewL()
    {
    return new (ELeave) CPtiLangDataImplSpanish();
    }


TInt CPtiLangDataImplSpanish::LanguageCode() const
    {
    return ELangSpanish;
    }


const TUint16* CPtiLangDataImplSpanish::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Spanish;
        return MultitapData_table_Spanish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Spanish;
        return StandardQwertyData_table_Spanish;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Spanish;
        return MiniQwertyData_table_Spanish;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_Spanish;
        return MiniQwerty3x11Data_table_Spanish;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplSpanish::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Spanish;
        return MultitapKeyBindings_table_Spanish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Spanish;
        return StandardQwertyKeyBindings_table_Spanish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Spanish;
        return MiniQwertyKeyBindings_table_Spanish;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_Spanish;
        return MiniQwerty3x11KeyBindings_table_Spanish;
        }

    aNumItems = 0;
    return NULL;
    }


const TUint16* CPtiLangDataImplSpanish::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_Spanish;
        return StandardQwertyDeadKeyData_table_Spanish;
        }

    aNumRowsInArray = 0;
    return NULL;
    }


