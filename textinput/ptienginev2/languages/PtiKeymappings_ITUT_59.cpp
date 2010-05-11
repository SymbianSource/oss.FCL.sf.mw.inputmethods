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

#include "PtiKeymappings_ITUT_59.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Indonesian[] =
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

const TInt KMultitapDataSize_Indonesian = 199;


const TPtiKeyBinding MultitapKeyBindings_table_Indonesian[] =
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

const TInt KMaxMultitapIndex_Indonesian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Indonesian[] =
    {
     0x0071,
     0x0077,
     0x0065,
     0x0072,
     0x0074,
     0x0079,
     0x0075,
     0x0069,
     0x006F,
     0x0070,
     0x0061,
     0x0073,
     0x0064,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x0027,
     0x007A,
     0x0078,
     0x0063,
     0x0076,
     0x0062,
     0x006E,
     0x006D,
     0x002C,
     0x002E,
     0x003F,
     0x0051,
     0x0057,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004F,
     0x0050,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x002F,
     0x005A,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004E,
     0x004D,
     0x002B,
     0x002D,
     0x0021,
    };

const TInt KStandardQwertyDataSize_Indonesian = 60;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Indonesian[] =
    {
        {EPtiKeyQwertyAtCharacter, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty1, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 3, 1},
        {EPtiKeyQwerty4, EPtiCaseLower, 4, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 5, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 6, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty8, EPtiCaseLower, 8, 1},
        {EPtiKeyQwerty9, EPtiCaseLower, 9, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 10, 0},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyQ, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyO, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 20, 0},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 20, 0},
        {EPtiKeyQwertyHash, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 27, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 30, 0},
        {0x5b, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyZ, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyC, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyV, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyB, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyN, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyM, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyComma, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertySlash, EPtiCaseLower, 30, 0},
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 30, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 31, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 32, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 33, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 40, 0},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 40, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 41, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 50, 0},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 50, 0},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 60, 0},
        {0x5b, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyX, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyC, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyV, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyB, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyM, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 60, 0},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 60, 0},
    };

const TInt KMaxStandardQwertyIndex_Indonesian = 92;

//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Indonesian[] =
    {
     0x0071,
     0x0077,
     0x0065,
     0x0072,
     0x0074,
     0x0079,
     0x0075,
     0x0069,
     0x006F,
     0x0070,
     0x0061,
     0x0073,
     0x0064,
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
     0x006E,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x0026,
     0x0027,
     0x003F,
     0x0051,
     0x0057,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004F,
     0x0050,
     0x0041,
     0x0053,
     0x0044,
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
     0x004E,
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

const TInt KMiniQwertyDataSize_Indonesian = 93;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Indonesian[] =
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
        {EPtiKeyQwertyA, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 27, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 32, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 33, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 40, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 41, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 60, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 61, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 62, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 63, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 64, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 65, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 66, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 67, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 68, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 69, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 70, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 71, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 72, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 73, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 74, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 75, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 86, 1},
    };

const TInt KMaxMiniQwertyIndex_Indonesian = 83;

//
// Class implementation.
//

CPtiLangDataImplIndonesian* CPtiLangDataImplIndonesian::NewL()
    {
    return new (ELeave) CPtiLangDataImplIndonesian();
    }


TInt CPtiLangDataImplIndonesian::LanguageCode() const
    {
    return ELangIndonesian;
    }


const TUint16* CPtiLangDataImplIndonesian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Indonesian;
        return MultitapData_table_Indonesian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Indonesian;
        return StandardQwertyData_table_Indonesian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Indonesian;
        return MiniQwertyData_table_Indonesian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplIndonesian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Indonesian;
        return MultitapKeyBindings_table_Indonesian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Indonesian;
        return StandardQwertyKeyBindings_table_Indonesian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Indonesian;
        return MiniQwertyKeyBindings_table_Indonesian;
        }

    aNumItems = 0;
    return NULL;
    }


