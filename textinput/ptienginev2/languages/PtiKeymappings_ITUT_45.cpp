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

#include "PtiKeymappings_ITUT_45.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Croatian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x10d,0x107,0x32,0xe1,0xe2,0xe4,0x103,0x105,
     0x64,0x111,0x65,0x66,0x33,0x10f,0xe9,0x119,0x11b,
     0x67,0x68,0x69,0x34,0xed,0xee,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0x144,0x148,0xf3,0xf4,0xf6,0x151,
     0x70,0x71,0x72,0x73,0x161,0x37,0x155,0x159,0x15b,0x15f,0xdf,
     0x74,0x75,0x76,0x38,0x163,0x165,0xfa,0xfc,0x16f,0x171,
     0x77,0x78,0x79,0x7a,0x17e,0x39,0xfd,0x17a,0x17c,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x10c,0x106,0x32,0xc1,0xc2,0xc4,0x102,0x104,
     0x44,0x110,0x45,0x46,0x33,0x10e,0xc9,0x118,0x11a,
     0x47,0x48,0x49,0x34,0xcd,0xce,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0x143,0x147,0xd3,0xd4,0xd6,0x150,
     0x50,0x51,0x52,0x53,0x37,0x160,0x24,
     0x54,0x55,0x56,0x38,0x162,0x164,0xda,0xdc,0x16e,0x170,
     0x57,0x58,0x59,0x5a,0x17d,0x39,0xdd,0x179,0x17b,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Croatian = 204;


const TPtiKeyBinding MultitapKeyBindings_table_Croatian[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 11},
        {EPtiKey3, EPtiCaseLower, 41, 9},
        {EPtiKey4, EPtiCaseLower, 50, 6},
        {EPtiKey5, EPtiCaseLower, 56, 5},
        {EPtiKey6, EPtiCaseLower, 61, 10},
        {EPtiKey7, EPtiCaseLower, 71, 11},
        {EPtiKey8, EPtiCaseLower, 82, 10},
        {EPtiKey9, EPtiCaseLower, 92, 9},
        {EPtiKeyStar, EPtiCaseLower, 101, 0},
        {EPtiKey0, EPtiCaseLower, 101, 3},
        {EPtiKeyHash, EPtiCaseLower, 104, 0},
        {EPtiKey1, EPtiCaseUpper, 104, 30},
        {EPtiKey2, EPtiCaseUpper, 134, 11},
        {EPtiKey3, EPtiCaseUpper, 145, 9},
        {EPtiKey4, EPtiCaseUpper, 154, 6},
        {EPtiKey5, EPtiCaseUpper, 160, 5},
        {EPtiKey6, EPtiCaseUpper, 165, 10},
        {EPtiKey7, EPtiCaseUpper, 175, 7},
        {EPtiKey8, EPtiCaseUpper, 182, 10},
        {EPtiKey9, EPtiCaseUpper, 192, 9},
        {EPtiKeyStar, EPtiCaseUpper, 201, 0},
        {EPtiKey0, EPtiCaseUpper, 201, 3},
        {EPtiKeyHash, EPtiCaseUpper, 204, 0},
    };

const TInt KMaxMultitapIndex_Croatian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Croatian[] =
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
     0x2d,
     0x71,
     0x77,
     0x65,0xe9,0x119,0x11b,
     0x72,0x155,0x159,
     0x74,0x163,0x165,
     0x7a,0x17a,0x17c,
     0x75,0xfa,0xfc,0x16f,0x171,
     0x69,0xed,0xee,
     0x6f,0xf3,0xf4,0xf6,0x151,
     0x70,
     0x161,0x111,
     0x2b,
     0x61,0xe1,0xe2,0xe4,0x103,0x105,
     0x73,0xdf,0x15b,0x15f,
     0x64,0x111,0x10f,
     0x66,
     0x67,
     0x68,
     0x6a,
     0x6b,
     0x6c,0x142,
     0x10d,
     0x107,
     0x79,0xfd,
     0x78,
     0x63,
     0x76,
     0x62,
     0x6e,0x144,0x148,
     0x6d,
     0x2c,
     0x2e,
     0x17e,
     0x21,
     0x22,
     0x23,
     0x24,
     0x40,
     0x27,
     0x2f,
     0x28,
     0x29,
     0x3f,
     0x3d,
     0x51,
     0x57,
     0x45,0xc9,0x118,0x11a,
     0x52,0x154,0x158,
     0x54,0x162,0x164,
     0x5a,0x179,0x17b,
     0x55,0xda,0xdc,0x16e,0x170,
     0x49,0xcd,0xce,
     0x4f,0xd3,0xd4,0xd6,0x150,
     0x50,
     0x160,0x110,
     0x2a,
     0x41,0xc1,0xc2,0xc4,0x102,0x104,
     0x53,0x15a,0x15e,
     0x44,0x110,0x10e,
     0x46,
     0x47,
     0x48,
     0x4a,
     0x4b,
     0x4c,0x141,
     0x10c,
     0x106,
     0x59,0xdd,
     0x58,
     0x43,
     0x56,
     0x42,
     0x4e,0x143,0x147,
     0x4d,
     0x3b,
     0x3a,
     0x17d,
    };

const TInt KStandardQwertyDataSize_Croatian = 155;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Croatian[] =
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
        {EPtiKeyQwertyY, EPtiCaseLower, 23, 3},
        {EPtiKeyQwertyU, EPtiCaseLower, 26, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 31, 3},
        {EPtiKeyQwertyO, EPtiCaseLower, 34, 5},
        {EPtiKeyQwertyP, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyHash, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 43, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 49, 4},
        {EPtiKeyQwertyD, EPtiCaseLower, 53, 3},
        {EPtiKeyQwertyF, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 61, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 64, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 65, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 67, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 68, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 69, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 70, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 71, 3},
        {EPtiKeyQwertyM, EPtiCaseLower, 74, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 75, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 76, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 77, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 91, 4},
        {EPtiKeyQwertyR, EPtiCaseUpper, 95, 3},
        {EPtiKeyQwertyT, EPtiCaseUpper, 98, 3},
        {EPtiKeyQwertyY, EPtiCaseUpper, 101, 3},
        {EPtiKeyQwertyU, EPtiCaseUpper, 104, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 109, 3},
        {EPtiKeyQwertyO, EPtiCaseUpper, 112, 5},
        {EPtiKeyQwertyP, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 118, 2},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 121, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 127, 3},
        {EPtiKeyQwertyD, EPtiCaseUpper, 130, 3},
        {EPtiKeyQwertyF, EPtiCaseUpper, 133, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 134, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 135, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 136, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 137, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 138, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 140, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 141, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 142, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 144, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 145, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 146, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 147, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 148, 3},
        {EPtiKeyQwertyM, EPtiCaseUpper, 151, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 152, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 153, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 154, 1},
    };

const TInt KMaxStandardQwertyIndex_Croatian = 88;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Croatian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E9,0x0119,0x011B,
     0x0072,0x0155,0x0159,
     0x0074,0x0163,0x0165,
     0x007A,0x017E,0x017A,0x017C,
     0x0075,0x00FA,0x00FC,0x016F,0x0171,
     0x0069,0x00ED,0x00EE,
     0x006F,0x00F3,0x00F4,0x00F6,0x0151,
     0x0070,
     0x0061,0x00E1,0x00E2,0x00E4,0x0103,0x0105,
     0x0073,0x0161,0x00DF,0x015B,0x015F,
     0x0064,0x0111,0x010F,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,0x0142,
     0x0079,0x00FD,
     0x0078,
     0x0063,0x010D,0x0107,
     0x0076,
     0x0062,
     0x006E,0x0144,0x0148,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x0021,
     0x0027,
     0x003F,
     0x0051,
     0x0057,
     0x0045,0x00C9,0x0118,0x011A,
     0x0052,0x0154,0x0158,
     0x0054,0x0162,0x0164,
     0x005A,0x017D,0x0179,0x017B,
     0x0055,0x00DA,0x00DC,0x016E,0x0170,
     0x0049,0x00CD,0x00CE,
     0x004F,0x00D3,0x00D4,0x00D6,0x0150,
     0x0050,
     0x0041,0x00C1,0x00C2,0x00C4,0x0102,0x0104,
     0x0053,0x0160,0x015A,0x015E,
     0x0044,0x0110,0x010E,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,0x0141,
     0x0059,0x00DD,
     0x0058,
     0x0043,0x010C,0x0106,
     0x0056,
     0x0042,
     0x004E,0x0143,0x0147,
     0x004D,
     0x003B,
     0x003A,
     0x0161,
     0x0111,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x002B,
     0x0109,
     0x0107,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x002D,
     0x017E,
     0x002F,
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
     0x0160,
     0x0110,
     0x0108,
     0x0106,
     0x017D,
    };

const TInt KMiniQwertyDataSize_Croatian = 171;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Croatian[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 4},
        {EPtiKeyQwerty4, EPtiCaseLower, 6, 3},
        {EPtiKeyQwerty5, EPtiCaseLower, 9, 3},
        {EPtiKeyQwerty6, EPtiCaseLower, 12, 4},
        {EPtiKeyQwerty7, EPtiCaseLower, 16, 5},
        {EPtiKeyQwerty8, EPtiCaseLower, 21, 3},
        {EPtiKeyQwerty9, EPtiCaseLower, 24, 5},
        {EPtiKeyQwerty0, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 30, 6},
        {EPtiKeyQwertyS, EPtiCaseLower, 36, 5},
        {EPtiKeyQwertyD, EPtiCaseLower, 41, 3},
        {EPtiKeyQwertyF, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 45, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 46, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 51, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 53, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 54, 3},
        {EPtiKeyQwertyV, EPtiCaseLower, 57, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 58, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 59, 3},
        {EPtiKeyQwertyM, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 63, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 64, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 70, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 71, 4},
        {EPtiKeyQwerty4, EPtiCaseUpper, 75, 3},
        {EPtiKeyQwerty5, EPtiCaseUpper, 78, 3},
        {EPtiKeyQwerty6, EPtiCaseUpper, 81, 4},
        {EPtiKeyQwerty7, EPtiCaseUpper, 85, 5},
        {EPtiKeyQwerty8, EPtiCaseUpper, 90, 3},
        {EPtiKeyQwerty9, EPtiCaseUpper, 93, 5},
        {EPtiKeyQwerty0, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 99, 6},
        {EPtiKeyQwertyS, EPtiCaseUpper, 105, 4},
        {EPtiKeyQwertyD, EPtiCaseUpper, 109, 3},
        {EPtiKeyQwertyF, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 113, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 117, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 119, 2},
        {EPtiKeyQwertyX, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 122, 3},
        {EPtiKeyQwertyV, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 127, 3},
        {EPtiKeyQwertyM, EPtiCaseUpper, 130, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 131, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 132, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 133, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 134, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 135, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 136, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 137, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 138, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 139, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 140, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 141, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 142, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 143, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 144, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 145, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 146, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 147, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 148, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 149, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 150, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 151, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 152, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 153, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 154, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 155, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 156, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 157, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 158, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 159, 1},
        {EPtiKeyQwerty1, EPtiCaseFnUpper, 166, 1},
        {EPtiKeyQwerty2, EPtiCaseFnUpper, 167, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 168, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 169, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 170, 1},
    };

const TInt KMaxMiniQwertyIndex_Croatian = 88;

//
// Class implementation.
//

CPtiLangDataImplCroatian* CPtiLangDataImplCroatian::NewL()
    {
    return new (ELeave) CPtiLangDataImplCroatian();
    }


TInt CPtiLangDataImplCroatian::LanguageCode() const
    {
    return ELangCroatian;
    }


const TUint16* CPtiLangDataImplCroatian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Croatian;
        return MultitapData_table_Croatian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Croatian;
        return StandardQwertyData_table_Croatian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Croatian;
        return MiniQwertyData_table_Croatian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplCroatian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Croatian;
        return MultitapKeyBindings_table_Croatian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Croatian;
        return StandardQwertyKeyBindings_table_Croatian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Croatian;
        return MiniQwertyKeyBindings_table_Croatian;
        }

    aNumItems = 0;
    return NULL;
    }


