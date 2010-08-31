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
     0x65,0xe8,0xe9,0xea,0xeb,
     0x72,
     0x74,0xfe,
     0x79,0xfd,
     0x75,0xf9,0xfa,0xfb,0xfc,
     0x69,0xec,0xed,0xee,0xef,
     0x6f,0xf6,0xf8,0xf2,0xf3,0xf4,0xf5,
     0x70,
     0x2d,
     0x23,
     0x61,0xe4,0xe6,0xe5,0xe0,0xe1,0xe2,0xe3,
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
     0x45,0xc8,0xc9,0xca,0xcb,
     0x52,
     0x54,0xde,
     0x59,0xdd,
     0x55,0xd9,0xda,0xdb,0xdc,
     0x49,0xcc,0xcd,0xce,0xcf,
     0x4f,0xd6,0xd8,0xd2,0xd3,0xd4,0xd5,
     0x50,
     0x5f,
     0x7e,
     0x41,0xc4,0xc6,0xc5,0xc0,0xc1,0xc2,0xc3,
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

const TInt KStandardQwertyDataSize_Indonesian = 149;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Indonesian[] =
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

const TInt KMaxStandardQwertyIndex_Indonesian = 88;

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


