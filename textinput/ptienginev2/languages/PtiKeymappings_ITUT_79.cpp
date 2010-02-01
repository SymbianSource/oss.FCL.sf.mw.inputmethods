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

#include "PtiKeymappings_ITUT_79.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Serbian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x62,0x63,0x10d,0x107,0x32,0xe4,0xe0,0xe1,0xe2,0xe7,
     0x64,0x65,0x66,0x111,0x33,0xe8,0xe9,0xeb,0xea,
     0x67,0x68,0x69,0x34,0xee,0xef,0xec,0xed,0x131,0x11f,
     0x6a,0x6b,0x6c,0x35,0xa3,
     0x6d,0x6e,0x6f,0x36,0xf6,0xf4,0xf2,0xf3,0xf8,0xf1,
     0x70,0x71,0x72,0x73,0x161,0x37,0x24,0xdf,
     0x74,0x75,0x76,0x38,0xfc,0xf9,0xfb,0xfa,
     0x77,0x78,0x79,0x7a,0x17e,0x39,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x42,0x43,0x10c,0x106,0x32,0xc4,0xc0,0xc1,0xc2,0xc7,
     0x44,0x45,0x46,0x110,0x33,0xc8,0xc9,0xcb,0xca,
     0x47,0x48,0x49,0x34,0xce,0xcf,0xcc,0xcd,0x130,0x11e,
     0x4a,0x4b,0x4c,0x35,0xa3,
     0x4d,0x4e,0x4f,0x36,0xd6,0xd4,0xd2,0xd3,0xd8,0xd1,
     0x50,0x51,0x52,0x53,0x160,0x37,0x24,
     0x54,0x55,0x56,0x38,0xdc,0xd9,0xdb,0xda,
     0x57,0x58,0x59,0x5a,0x17d,0x39,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Serbian = 199;


const TPtiKeyBinding MultitapKeyBindings_table_Serbian[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 11},
        {EPtiKey3, EPtiCaseLower, 41, 9},
        {EPtiKey4, EPtiCaseLower, 50, 10},
        {EPtiKey5, EPtiCaseLower, 60, 5},
        {EPtiKey6, EPtiCaseLower, 65, 10},
        {EPtiKey7, EPtiCaseLower, 75, 8},
        {EPtiKey8, EPtiCaseLower, 83, 8},
        {EPtiKey9, EPtiCaseLower, 91, 6},
        {EPtiKeyStar, EPtiCaseLower, 97, 0},
        {EPtiKey0, EPtiCaseLower, 97, 3},
        {EPtiKeyHash, EPtiCaseLower, 100, 0},
        {EPtiKey1, EPtiCaseUpper, 100, 30},
        {EPtiKey2, EPtiCaseUpper, 130, 11},
        {EPtiKey3, EPtiCaseUpper, 141, 9},
        {EPtiKey4, EPtiCaseUpper, 150, 10},
        {EPtiKey5, EPtiCaseUpper, 160, 5},
        {EPtiKey6, EPtiCaseUpper, 165, 10},
        {EPtiKey7, EPtiCaseUpper, 175, 7},
        {EPtiKey8, EPtiCaseUpper, 182, 8},
        {EPtiKey9, EPtiCaseUpper, 190, 6},
        {EPtiKeyStar, EPtiCaseUpper, 196, 0},
        {EPtiKey0, EPtiCaseUpper, 196, 3},
        {EPtiKeyHash, EPtiCaseUpper, 199, 0},
    };

const TInt KMaxMultitapIndex_Serbian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Serbian[] =
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
     0x65,0xe8,0xe9,0xeb,0xea,
     0x72,
     0x74,
     0x7a,
     0x75,0xfc,0xf9,0xfb,0xfa,
     0x69,0xee,0xef,0xec,0xed,0x131,
     0x6f,0xf6,0xf4,0xf2,0xf3,0xf8,
     0x70,
     0x161,0x111,
     0x2b,
     0x61,0xe4,0xe0,0xe1,0xe2,
     0x73,0xdf,
     0x64,0x111,
     0x66,
     0x67,0x11f,
     0x68,
     0x6a,
     0x6b,
     0x6c,0x142,
     0x10d,
     0x107,
     0x79,
     0x78,
     0x63,0xe7,
     0x76,
     0x62,
     0x6e,0xf1,
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
     0x45,0xc8,0xc9,0xcb,0xca,
     0x52,
     0x54,
     0x5a,
     0x55,0xdc,0xd9,0xdb,0xda,
     0x49,0xce,0xcf,0xcc,0xcd,0x130,
     0x4f,0xd6,0xd4,0xd2,0xd3,0xd8,
     0x50,
     0x160,0x110,
     0x2a,
     0x41,0xc4,0xc0,0xc1,0xc2,
     0x53,
     0x44,0x110,
     0x46,
     0x47,0x11e,
     0x48,
     0x4a,
     0x4b,
     0x4c,0x141,
     0x10c,
     0x106,
     0x59,
     0x58,
     0x43,0xc7,
     0x56,
     0x42,
     0x4e,0xd1,
     0x4d,
     0x3b,
     0x3a,
     0x17d,
    };

const TInt KStandardQwertyDataSize_Serbian = 145;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Serbian[] =
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
        {EPtiKeyQwertyT, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 21, 5},
        {EPtiKeyQwertyI, EPtiCaseLower, 26, 6},
        {EPtiKeyQwertyO, EPtiCaseLower, 32, 6},
        {EPtiKeyQwertyP, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 39, 2},
        {EPtiKeyQwertyHash, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 42, 5},
        {EPtiKeyQwertyS, EPtiCaseLower, 47, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 49, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 52, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 57, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 59, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 60, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 61, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 62, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 63, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 65, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 66, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 67, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 69, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 70, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 71, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 72, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 86, 5},
        {EPtiKeyQwertyR, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 94, 5},
        {EPtiKeyQwertyI, EPtiCaseUpper, 99, 6},
        {EPtiKeyQwertyO, EPtiCaseUpper, 105, 6},
        {EPtiKeyQwertyP, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 112, 2},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 114, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 115, 5},
        {EPtiKeyQwertyS, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 121, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 123, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 124, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 127, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 128, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 129, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 131, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 132, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 133, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 134, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 135, 2},
        {EPtiKeyQwertyV, EPtiCaseUpper, 137, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 138, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 139, 2},
        {EPtiKeyQwertyM, EPtiCaseUpper, 141, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 142, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 143, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 144, 1},
    };

const TInt KMaxStandardQwertyIndex_Serbian = 88;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Serbian[] =
    {
     0x0071,
     0x0077,
     0x0065,0x00E8,0x00E9,0x00EB,0x00EA,
     0x0072,
     0x0074,
     0x007A,0x017E,
     0x0075,0x00FC,0x00F9,0x00FB,0x00FA,
     0x0069,0x00EE,0x00EF,0x00EC,0x00ED,0x0131,
     0x006F,0x00F6,0x00F4,0x00F2,0x00F3,0x00F8,
     0x0070,
     0x0061,0x00E4,0x00E0,0x00E1,0x00E2,
     0x0073,0x0161,0x00DF,
     0x0064,0x0111,
     0x0066,
     0x0067,0x011F,
     0x0068,
     0x006A,
     0x006B,
     0x006C,0x0142,
     0x0079,
     0x0078,
     0x0063,0x00E7,0x010D,0x0107,
     0x0076,
     0x0062,
     0x006E,0x00F1,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x0021,
     0x0027,
     0x003F,
     0x0051,
     0x0057,
     0x0045,0x00C8,0x00C9,0x00CB,0x00CA,
     0x0052,
     0x0054,
     0x005A,0x017D,
     0x0055,0x00DC,0x00D9,0x00DB,0x00DA,
     0x0049,0x00CE,0x00CF,0x00CC,0x00CD,0x0130,
     0x004F,0x00D6,0x00D4,0x00D2,0x00D3,0x00D8,
     0x0050,
     0x0041,0x00C4,0x00C0,0x00C1,0x00C2,
     0x0053,0x0160,
     0x0044,0x0110,
     0x0046,
     0x0047,0x011E,
     0x0048,
     0x004A,
     0x004B,
     0x004C,0x0141,
     0x0059,
     0x0058,
     0x0043,0x00C7,0x010C,0x0106,
     0x0056,
     0x0042,
     0x004E,0x00D1,
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

const TInt KMiniQwertyDataSize_Serbian = 161;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Serbian[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 5},
        {EPtiKeyQwerty4, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 8, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 9, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 11, 5},
        {EPtiKeyQwerty8, EPtiCaseLower, 16, 6},
        {EPtiKeyQwerty9, EPtiCaseLower, 22, 6},
        {EPtiKeyQwerty0, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 29, 5},
        {EPtiKeyQwertyS, EPtiCaseLower, 34, 3},
        {EPtiKeyQwertyD, EPtiCaseLower, 37, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 44, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 45, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 48, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 49, 4},
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
        {EPtiKeyQwerty5, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 73, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 75, 5},
        {EPtiKeyQwerty8, EPtiCaseUpper, 80, 6},
        {EPtiKeyQwerty9, EPtiCaseUpper, 86, 6},
        {EPtiKeyQwerty0, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 93, 5},
        {EPtiKeyQwertyS, EPtiCaseUpper, 98, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 100, 2},
        {EPtiKeyQwertyF, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 103, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 105, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 106, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 107, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 108, 2},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 110, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 112, 4},
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
        {EPtiKeyQwerty1, EPtiCaseFnUpper, 156, 1},
        {EPtiKeyQwerty2, EPtiCaseFnUpper, 157, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 158, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 159, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 160, 1},
    };

const TInt KMaxMiniQwertyIndex_Serbian = 88;

//
// Class implementation.
//

CPtiLangDataImplSerbian* CPtiLangDataImplSerbian::NewL()
    {
    return new (ELeave) CPtiLangDataImplSerbian();
    }


TInt CPtiLangDataImplSerbian::LanguageCode() const
    {
    return ELangSerbian;
    }


const TUint16* CPtiLangDataImplSerbian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Serbian;
        return MultitapData_table_Serbian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Serbian;
        return StandardQwertyData_table_Serbian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Serbian;
        return MiniQwertyData_table_Serbian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplSerbian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Serbian;
        return MultitapKeyBindings_table_Serbian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Serbian;
        return StandardQwertyKeyBindings_table_Serbian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Serbian;
        return MiniQwertyKeyBindings_table_Serbian;
        }

    aNumItems = 0;
    return NULL;
    }


