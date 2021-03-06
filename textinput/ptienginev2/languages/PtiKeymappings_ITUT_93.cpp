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

#include "PtiKeymappings_ITUT_93.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Ukrainian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x430,0x431,0x432,0x433,0x491,0x32,0x61,0x62,0x63,
     0x434,0x435,0x454,0x436,0x437,0x33,0x64,0x65,0x66,
     0x456,0x457,0x438,0x439,0x43a,0x43b,0x34,0x67,0x68,0x69,
     0x43c,0x43d,0x43e,0x43f,0x35,0x6a,0x6b,0x6c,
     0x440,0x441,0x442,0x443,0x36,0x6d,0x6e,0x6f,
     0x444,0x445,0x446,0x447,0x37,0x70,0x71,0x72,0x73,
     0x448,0x449,0x38,0x74,0x75,0x76,
     0x44c,0x44e,0x44f,0x39,0x77,0x78,0x79,0x7a,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x410,0x411,0x412,0x413,0x490,0x32,0x41,0x42,0x43,
     0x414,0x415,0x404,0x416,0x417,0x33,0x44,0x45,0x46,
     0x406,0x407,0x418,0x419,0x41a,0x41b,0x34,0x47,0x48,0x49,
     0x41c,0x41d,0x41e,0x41f,0x35,0x4a,0x4b,0x4c,
     0x420,0x421,0x422,0x423,0x36,0x4d,0x4e,0x4f,
     0x424,0x425,0x426,0x427,0x37,0x50,0x51,0x52,0x53,
     0x428,0x429,0x38,0x54,0x55,0x56,
     0x42c,0x42e,0x42f,0x39,0x57,0x58,0x59,0x5a,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Ukrainian = 200;


const TPtiKeyBinding MultitapKeyBindings_table_Ukrainian[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 9},
        {EPtiKey3, EPtiCaseLower, 39, 9},
        {EPtiKey4, EPtiCaseLower, 48, 10},
        {EPtiKey5, EPtiCaseLower, 58, 8},
        {EPtiKey6, EPtiCaseLower, 66, 8},
        {EPtiKey7, EPtiCaseLower, 74, 9},
        {EPtiKey8, EPtiCaseLower, 83, 6},
        {EPtiKey9, EPtiCaseLower, 89, 8},
        {EPtiKeyStar, EPtiCaseLower, 97, 0},
        {EPtiKey0, EPtiCaseLower, 97, 3},
        {EPtiKeyHash, EPtiCaseLower, 100, 0},
        {EPtiKey1, EPtiCaseUpper, 100, 30},
        {EPtiKey2, EPtiCaseUpper, 130, 9},
        {EPtiKey3, EPtiCaseUpper, 139, 9},
        {EPtiKey4, EPtiCaseUpper, 148, 10},
        {EPtiKey5, EPtiCaseUpper, 158, 8},
        {EPtiKey6, EPtiCaseUpper, 166, 8},
        {EPtiKey7, EPtiCaseUpper, 174, 9},
        {EPtiKey8, EPtiCaseUpper, 183, 6},
        {EPtiKey9, EPtiCaseUpper, 189, 8},
        {EPtiKeyStar, EPtiCaseUpper, 197, 0},
        {EPtiKey0, EPtiCaseUpper, 197, 3},
        {EPtiKeyHash, EPtiCaseUpper, 200, 0},
    };

const TInt KMaxMultitapIndex_Ukrainian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Ukrainian[] =
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
     0x439,
     0x446,
     0x443,
     0x43a,
     0x435,
     0x43d,
     0x433,
     0x448,
     0x449,
     0x437,
     0x445,
     0x2b,
     0x444,
     0x44b,
     0x432,
     0x430,
     0x43f,
     0x440,
     0x43e,
     0x43b,
     0x434,
     0x436,
     0x44d,
     0x44f,
     0x447,
     0x441,
     0x43c,
     0x438,
     0x442,
     0x44c,
     0x431,
     0x44e,
     0x2e,
     0x21,
     0x22,
     0x40,
     0x3b,
     0x3d,
     0x3a,
     0x3f,
     0x2a,
     0x28,
     0x29,
     0x2f,
     0x419,
     0x426,
     0x423,
     0x41a,
     0x415,
     0x41d,
     0x413,
     0x428,
     0x429,
     0x417,
     0x425,
     0x23,
     0x424,
     0x42b,
     0x412,
     0x410,
     0x41f,
     0x420,
     0x41e,
     0x41b,
     0x414,
     0x416,
     0x42d,
     0x42f,
     0x427,
     0x421,
     0x41c,
     0x418,
     0x422,
     0x42c,
     0x411,
     0x42e,
     0x2c,
    };

const TInt KStandardQwertyDataSize_Ukrainian = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Ukrainian[] =
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
        {EPtiKeyQwertyE, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyO, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyHash, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 25, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 27, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 29, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 31, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 32, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 33, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 34, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 35, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 36, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 38, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 39, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 40, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 41, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertySlash, EPtiCaseLower, 43, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 70, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 87, 1},
    };

const TInt KMaxStandardQwertyIndex_Ukrainian = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Ukrainian[] =
	{
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Ukrainian = 3;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Ukrainian[] =
    {
     0x0439,
     0x0446,
     0x0443,
     0x043A,
     0x0435,
     0x043D,
     0x0433,0x0491,
     0x0448,
     0x0449,
     0x0437,
     0x0444,
     0x044B,
     0x0432,
     0x0430,
     0x043F,
     0x0440,
     0x043E,
     0x043B,
     0x0434,
     0x044F,
     0x0447,
     0x0441,
     0x043C,
     0x0438,0x0456,
     0x0442,
     0x044C,
     0x0431,
     0x044E,
     0x0040,
     0x003F,
     0x002C,
     0x002E,
     0x0419,
     0x0426,
     0x0423,
     0x041A,
     0x0415,
     0x041D,
     0x0413,0x0490,
     0x0428,
     0x0429,
     0x0417,
     0x0424,
     0x042B,
     0x0412,
     0x0410,
     0x041F,
     0x0420,
     0x041E,
     0x041B,
     0x0414,
     0x042F,
     0x0427,
     0x0421,
     0x041C,
     0x0418,0x0406,
     0x0422,
     0x042C,
     0x0411,
     0x042E,
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
     0x0445,
     0x002F,
     0x0027,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x0436,
     0x002B,
     0x002D,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x044D,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
     0x0425,
     0x0416,
     0x042D,
    };

const TInt KMiniQwertyDataSize_Ukrainian = 102;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Ukrainian[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 2, 1},
        {EPtiKeyQwerty4, EPtiCaseLower, 3, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 4, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 5, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 6, 2},
        {EPtiKeyQwerty8, EPtiCaseLower, 8, 1},
        {EPtiKeyQwerty9, EPtiCaseLower, 9, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 24, 2},
        {EPtiKeyQwertyN, EPtiCaseLower, 26, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 27, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 28, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 29, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 40, 2},
        {EPtiKeyQwerty8, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 58, 2},
        {EPtiKeyQwertyN, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 66, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 67, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 68, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 69, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 70, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 71, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 72, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 73, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 74, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 75, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 86, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 87, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 88, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 89, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 90, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 91, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 92, 1},
        {EPtiKeyQwerty9, EPtiCaseFnUpper, 99, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 100, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnUpper, 101, 1},
    };

const TInt KMaxMiniQwertyIndex_Ukrainian = 86;


//
// Numeric key data table for MiniQwerty
//
const TPtiNumericKeyBinding MiniQwertyNumericKeysData_table_Ukrainian[] =
	{
	{'p',EPtiKeyQwerty0,EPtiCaseLower},
	{'w',EPtiKeyQwerty2,EPtiCaseLower},
	{'E',EPtiKeyQwerty3,EPtiCaseUpper},
	};

const TInt KNumMiniQwertyNumericKeysEntries_Ukrainian = 3;
//
// Class implementation.
//

CPtiLangDataImplUkrainian* CPtiLangDataImplUkrainian::NewL()
    {
    return new (ELeave) CPtiLangDataImplUkrainian();
    }


TInt CPtiLangDataImplUkrainian::LanguageCode() const
    {
    return ELangUkrainian;
    }


const TUint16* CPtiLangDataImplUkrainian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Ukrainian;
        return MultitapData_table_Ukrainian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Ukrainian;
        return StandardQwertyData_table_Ukrainian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Ukrainian;
        return MiniQwertyData_table_Ukrainian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplUkrainian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Ukrainian;
        return MultitapKeyBindings_table_Ukrainian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Ukrainian;
        return StandardQwertyKeyBindings_table_Ukrainian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Ukrainian;
        return MiniQwertyKeyBindings_table_Ukrainian;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplUkrainian::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Ukrainian;
        return StandardQwertyNumericKeysData_table_Ukrainian;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumEntries = KNumMiniQwertyNumericKeysEntries_Ukrainian;
        return MiniQwertyNumericKeysData_table_Ukrainian;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplUkrainian::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


