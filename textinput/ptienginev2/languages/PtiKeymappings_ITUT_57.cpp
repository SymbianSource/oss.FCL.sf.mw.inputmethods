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

#include "PtiKeymappings_ITUT_57.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Hebrew[] =
    {
     0x2e,0x2c,0x3f,0x21,0x27,0x22,0x31,0x2d,0x28,0x29,0x40,0x2f,0x3a,0x5f,
     0x5d3,0x5d4,0x5d5,0x32,
     0x5d0,0x5d1,0x5d2,0x33,
     0x5de,0x5dd,0x5e0,0x5df,0x34,
     0x5d9,0x5db,0x5da,0x5dc,0x35,
     0x5d6,0x5d7,0x5d8,0x36,
     0x5e8,0x5e9,0x5ea,0x37,
     0x5e6,0x5e5,0x5e7,0x38,
     0x5e1,0x5e2,0x5e4,0x5e3,0x39,
     0x20,0x30,0x21b3,
     0x2e,0x2c,0x3f,0x21,0x27,0x22,0x31,0x2d,0x28,0x29,0x40,0x2f,0x3a,0x5f,
     0x5d3,0x5d4,0x5d5,0x32,
     0x5d0,0x5d1,0x5d2,0x33,
     0x5de,0x5dd,0x5e0,0x5df,0x34,
     0x5d9,0x5db,0x5da,0x5dc,0x35,
     0x5d6,0x5d7,0x5d8,0x36,
     0x5e8,0x5e9,0x5ea,0x37,
     0x5e6,0x5e5,0x5e7,0x38,
     0x5e1,0x5e2,0x5e4,0x5e3,0x39,
     0x20,0x30,0x21b3,
    };

const TInt KMultitapDataSize_Hebrew = 104;


const TPtiKeyBinding MultitapKeyBindings_table_Hebrew[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 14},
        {EPtiKey2, EPtiCaseLower, 14, 4},
        {EPtiKey3, EPtiCaseLower, 18, 4},
        {EPtiKey4, EPtiCaseLower, 22, 5},
        {EPtiKey5, EPtiCaseLower, 27, 5},
        {EPtiKey6, EPtiCaseLower, 32, 4},
        {EPtiKey7, EPtiCaseLower, 36, 4},
        {EPtiKey8, EPtiCaseLower, 40, 4},
        {EPtiKey9, EPtiCaseLower, 44, 5},
        {EPtiKeyStar, EPtiCaseLower, 49, 0},
        {EPtiKey0, EPtiCaseLower, 49, 3},
        {EPtiKeyHash, EPtiCaseLower, 52, 0},
        {EPtiKey1, EPtiCaseUpper, 52, 14},
        {EPtiKey2, EPtiCaseUpper, 66, 4},
        {EPtiKey3, EPtiCaseUpper, 70, 4},
        {EPtiKey4, EPtiCaseUpper, 74, 5},
        {EPtiKey5, EPtiCaseUpper, 79, 5},
        {EPtiKey6, EPtiCaseUpper, 84, 4},
        {EPtiKey7, EPtiCaseUpper, 88, 4},
        {EPtiKey8, EPtiCaseUpper, 92, 4},
        {EPtiKey9, EPtiCaseUpper, 96, 5},
        {EPtiKeyStar, EPtiCaseUpper, 101, 0},
        {EPtiKey0, EPtiCaseUpper, 101, 3},
        {EPtiKeyHash, EPtiCaseUpper, 104, 0},
    };

const TInt KMaxMultitapIndex_Hebrew = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Hebrew[] =
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
     0x2f,
     0x5c,
     0x5e7,
     0x5e8,
     0x5d0,
     0x5d8,
     0x5d5,
     0x5df,
     0x5dd,
     0x5e4,
     0x3a,
     0x2b,
     0x5e9,
     0x5d3,
     0x5d2,
     0x5db,
     0x5e2,
     0x5d9,
     0x5d7,
     0x5dc,
     0x5da,
     0x5e3,
     0x2c,
     0x5d6,
     0x5e1,
     0x5d1,
     0x5d4,
     0x5e0,
     0x5de,
     0x5e6,
     0x5ea,
     0x5e5,
     0x2e,
     0x21,
     0x40,
     0x20ac,
     0x24,
     0x25,
     0x26,
     0x2a,
     0x29,
     0x28,
     0x3d,
     0x5f,
     0x2f,
     0x5c,
     0x5e7,
     0x5e8,
     0x5d0,
     0x5d8,
     0x5d5,
     0x5df,
     0x5dd,
     0x5e4,
     0x3b,
     0x23,
     0x5e9,
     0x5d3,
     0x5d2,
     0x5db,
     0x5e2,
     0x5d9,
     0x5d7,
     0x5dc,
     0x5da,
     0x27,
     0x22,
     0x5d6,
     0x5e1,
     0x5d1,
     0x5d4,
     0x5e0,
     0x5de,
     0x5e6,
     0x3e,
     0x3c,
     0x3f,
    };

const TInt KStandardQwertyDataSize_Hebrew = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Hebrew[] =
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

const TInt KMaxStandardQwertyIndex_Hebrew = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Hebrew[] =
	{
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Hebrew = 3;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Hebrew[] =
    {
     0x05E7,
     0x05E8,
     0x05D0,
     0x05D8,
     0x05D5,
     0x05DF,
     0x05DD,
     0x05E4,
     0x05E9,
     0x05D3,
     0x05D2,
     0x05DB,
     0x05E2,
     0x05D9,
     0x05D7,
     0x05DC,
     0x05DA,
     0x05D6,
     0x05E1,
     0x05D1,
     0x05D4,
     0x05E0,
     0x05DE,
     0x05E6,
     0x05EA,
     0x05E5,
     0x0040,
     0x003F,
     0x002C,
     0x002E,
     0x05E7,
     0x05E8,
     0x05D0,
     0x05D8,
     0x05D5,
     0x05DF,
     0x05DD,
     0x05E4,
     0x05E9,
     0x05D3,
     0x05D2,
     0x05DB,
     0x05E2,
     0x05D9,
     0x05D7,
     0x05DC,
     0x05DA,
     0x05D6,
     0x05E1,
     0x05D1,
     0x05D4,
     0x05E0,
     0x05DE,
     0x05E6,
     0x05EA,
     0x05E3,
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
     0x0027,
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

const TInt KMiniQwertyDataSize_Hebrew = 91;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Hebrew[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 0},
        {EPtiKeyQwerty2, EPtiCaseLower, 0, 0},
        {EPtiKeyQwerty3, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty4, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 2, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 3, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 4, 1},
        {EPtiKeyQwerty8, EPtiCaseLower, 5, 1},
        {EPtiKeyQwerty9, EPtiCaseLower, 6, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 7, 1},
        {EPtiKeyQwertyA, EPtiCaseLower, 8, 1},
        {EPtiKeyQwertyS, EPtiCaseLower, 9, 1},
        {EPtiKeyQwertyD, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyF, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyG, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyH, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyJ, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyK, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyL, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyX, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyC, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyV, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyB, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyN, EPtiCaseLower, 22, 1},
        {EPtiKeyQwertyM, EPtiCaseLower, 23, 1},
        {EPtiKeyQwertyComma, EPtiCaseLower, 24, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 25, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 30, 0},
        {EPtiKeyQwerty2, EPtiCaseUpper, 30, 0},
        {EPtiKeyQwerty3, EPtiCaseUpper, 30, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 31, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 32, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 33, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 40, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 41, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 58, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 59, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 60, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 61, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 62, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 63, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 64, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 65, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 66, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 67, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 68, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 69, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 70, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 71, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 72, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 73, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 74, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 75, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 84, 1},
    };

const TInt KMaxMiniQwertyIndex_Hebrew = 83;


//
// Numeric key data table for MiniQwerty
//
const TPtiNumericKeyBinding MiniQwertyNumericKeysData_table_Hebrew[] =
	{
	{'p',EPtiKeyQwerty0,EPtiCaseLower},
	{'w',EPtiKeyQwerty2,EPtiCaseLower},
	{'E',EPtiKeyQwerty3,EPtiCaseUpper},
	};

const TInt KNumMiniQwertyNumericKeysEntries_Hebrew = 3;
//
// Class implementation.
//

CPtiLangDataImplHebrew* CPtiLangDataImplHebrew::NewL()
    {
    return new (ELeave) CPtiLangDataImplHebrew();
    }


TInt CPtiLangDataImplHebrew::LanguageCode() const
    {
    return ELangHebrew;
    }


const TUint16* CPtiLangDataImplHebrew::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Hebrew;
        return MultitapData_table_Hebrew;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Hebrew;
        return StandardQwertyData_table_Hebrew;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Hebrew;
        return MiniQwertyData_table_Hebrew;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplHebrew::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Hebrew;
        return MultitapKeyBindings_table_Hebrew;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Hebrew;
        return StandardQwertyKeyBindings_table_Hebrew;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Hebrew;
        return MiniQwertyKeyBindings_table_Hebrew;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplHebrew::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Hebrew;
        return StandardQwertyNumericKeysData_table_Hebrew;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumEntries = KNumMiniQwertyNumericKeysEntries_Hebrew;
        return MiniQwertyNumericKeysData_table_Hebrew;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplHebrew::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


