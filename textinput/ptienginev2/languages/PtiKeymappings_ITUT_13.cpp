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
     0x0071,
     0x0077,
     0x0065,0x00EA,0x00E8,0x00E9,
     0x0072,0x0155,
     0x0074,
     0x0079,0x00FD,
     0x0075,0x00FB,0x00F9,0x00FA,
     0x0069,0x00EE,0x00EC,0x00ED,
     0x006F,0x00F4,0x00F2,0x00F5,0x00F3,
     0x0070,
     0xf004,0x00B4,
     0x0061,0x00E2,0x00E0,0x00E3,0x00E1,
     0x0073,0x015B,
     0x0064,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,0x013A,
     0x00E7,
     0xf003,0x007E,
     0x007A,0x017A,
     0x0078,
     0x0063,0x0107,
     0x0076,
     0x0062,
     0x006E,0x0144,
     0x006D,
     0x002E,
     0x003F,
     0xf002,0x0060,
     0xf001,0x005E,
     0x0051,
     0x0057,
     0x0045,0x00CA,0x00C8,0x00C9,
     0x0052,0x0154,
     0x0054,
     0x0059,0x00DD,
     0x0055,0x00DB,0x00D9,0x00DA,
     0x0049,0x00CE,0x00CC,0x00CD,
     0x004F,0x00D4,0x00D2,0x00D5,0x00D3,
     0x0050,
     0xf004,0x00B4,
     0x0041,0x00C2,0x00C0,0x00C3,0x00C1,
     0x0053,0x015A,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,0x0139,
     0x00C7,
     0xf003,0x007E,
     0x005A,0x0179,
     0x0058,
     0x0043,0x0106,
     0x0056,
     0x0042,
     0x004E,0x0143,
     0x004D,
     0x002C,
     0x0021,
     0xf002,0x0060,
     0xf001,0x005E,
    };

const TInt KStandardQwertyDataSize_Portuguese = 122;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Portuguese[] =
    {
        {EPtiKeyQwertyAtCharacter, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty1, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 4},
        {EPtiKeyQwerty3, EPtiCaseLower, 6, 2},
        {EPtiKeyQwerty4, EPtiCaseLower, 8, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 9, 2},
        {EPtiKeyQwerty6, EPtiCaseLower, 11, 4},
        {EPtiKeyQwerty7, EPtiCaseLower, 15, 4},
        {EPtiKeyQwerty8, EPtiCaseLower, 19, 5},
        {EPtiKeyQwerty9, EPtiCaseLower, 24, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 25, 2},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 27, 5},
        {EPtiKeyQwertyQ, EPtiCaseLower, 32, 2},
        {EPtiKeyQwertyW, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyO, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 43, 2},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 45, 0},
        {EPtiKeyQwertyHash, EPtiCaseLower, 45, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 47, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 50, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 51, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 52, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 54, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 55, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 56, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 57, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 59, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 61, 0},
        {0x5b, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyZ, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyC, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyV, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyB, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyN, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyM, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyComma, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertySlash, EPtiCaseLower, 61, 0},
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 63, 4},
        {EPtiKeyQwerty3, EPtiCaseUpper, 67, 2},
        {EPtiKeyQwerty4, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 70, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 72, 4},
        {EPtiKeyQwerty7, EPtiCaseUpper, 76, 4},
        {EPtiKeyQwerty8, EPtiCaseUpper, 80, 5},
        {EPtiKeyQwerty9, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 86, 2},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 88, 5},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 93, 2},
        {EPtiKeyQwertyW, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 98, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 101, 2},
        {EPtiKeyQwertyO, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 104, 2},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 106, 2},
        {EPtiKeyQwertyA, EPtiCaseUpper, 108, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 109, 2},
        {EPtiKeyQwertyD, EPtiCaseUpper, 111, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 112, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 113, 2},
        {EPtiKeyQwertyH, EPtiCaseUpper, 115, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 116, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 117, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 118, 2},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 120, 2},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 122, 0},
        {0x5b, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyX, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyC, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyV, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyB, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyM, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 122, 0},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 122, 0},
    };

const TInt KMaxStandardQwertyIndex_Portuguese = 92;


//
// Dead key data table for StandardQwerty
//
const TUint16 StandardQwertyDeadKeyData_table_Portuguese[] =
    {
    2,  0x00EA,0x00E8,0xf000,0x00E9,0xf000,
    3,  0xf000,0xf000,0xf000,0x0155,0xf000,
    5,  0xf000,0xf000,0xf000,0x00FD,0xf000,
    6,  0x00FB,0x00F9,0xf000,0x00FA,0xf000,
    7,  0x00EE,0x00EC,0xf000,0x00ED,0xf000,
    8,  0x00F4,0x00F2,0x00F5,0x00F3,0xf000,
    11,  0x00E2,0x00E0,0x00E3,0x00E1,0xf000,
    12,  0xf000,0xf000,0xf000,0x015B,0xf000,
    19,  0xf000,0xf000,0xf000,0x013A,0xf000,
    22,  0xf000,0xf000,0xf000,0x017A,0xf000,
    24,  0xf000,0xf000,0xf000,0x0107,0xf000,
    27,  0xf000,0xf000,0xf000,0x0144,0xf000,
    35,  0x00CA,0x00C8,0xf000,0x00C9,0xf000,
    36,  0xf000,0xf000,0xf000,0x0154,0xf000,
    38,  0xf000,0xf000,0xf000,0x00DD,0xf000,
    39,  0x00DB,0x00D9,0xf000,0x00DA,0xf000,
    40,  0x00CE,0x00CC,0xf000,0x00CD,0xf000,
    41,  0x00D4,0x00D2,0x00D5,0x00D3,0xf000,
    44,  0x00C2,0x00C0,0x00C3,0x00C1,0xf000,
    45,  0xf000,0xf000,0xf000,0x015A,0xf000,
    52,  0xf000,0xf000,0xf000,0x0139,0xf000,
    55,  0xf000,0xf000,0xf000,0x0179,0xf000,
    57,  0xf000,0xf000,0xf000,0x0106,0xf000,
    60,  0xf000,0xf000,0xf000,0x0143,0xf000,
    };

const TInt KStandardQwertyDeadKeyTableRows_Portuguese = 24;

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


