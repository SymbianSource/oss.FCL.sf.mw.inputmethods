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

#include "PtiKeymappings_ITUT_33.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Thai[] =
    {
     0xe01,0xe02,0xe03,0xe04,0xe05,0x31,
     0xe06,0xe07,0xe08,0xe09,0x32,
     0xe0a,0xe0b,0xe0c,0xe0d,0x33,
     0xe0e,0xe0f,0xe10,0xe11,0xe12,0xe13,0x34,
     0xe14,0xe15,0xe16,0xe17,0xe18,0x35,
     0xe19,0xe1a,0xe1b,0xe1c,0xe1d,0x36,
     0xe1e,0xe1f,0xe20,0xe21,0xe22,0x37,
     0xe23,0xe24,0xe25,0xe26,0xe27,0xe28,0xe29,0x38,
     0xe2a,0xe2b,0xe2c,0xe2d,0xe2e,0x39,
     0x0E2F,0x0E3F,0x0E31,0x0E34,0x0E35,0x0E36,0x0E37,0x0E38,0x0E39,0x0E46,0x0E47,0x0E48,0x0E49,0x0E4A,0x0E4B,0x0E4C,
     0xE30,0xE32,0xE33,0xE40,0xE41,0xE42,0xE43,0xE44,0xE45,
     0xe01,0xe02,0xe03,0xe04,0xe05,0x31,
     0xe06,0xe07,0xe08,0xe09,0x32,
     0xe0a,0xe0b,0xe0c,0xe0d,0x33,
     0xe0e,0xe0f,0xe10,0xe11,0xe12,0xe13,0x34,
     0xe14,0xe15,0xe16,0xe17,0xe18,0x35,
     0xe19,0xe1a,0xe1b,0xe1c,0xe1d,0x36,
     0xe1e,0xe1f,0xe20,0xe21,0xe22,0x37,
     0xe23,0xe24,0xe25,0xe26,0xe27,0xe28,0xe29,0x38,
     0xe2a,0xe2b,0xe2c,0xe2d,0xe2e,0x39,
     0x0E2F,0x0E3F,0x0E31,0x0E34,0x0E35,0x0E36,0x0E37,0x0E38,0x0E39,0x0E46,0x0E47,0x0E48,0x0E49,0x0E4A,0x0E4B,0x0E4C,
     0xE30,0xE32,0xE33,0xE40,0xE41,0xE42,0xE43,0xE44,0xE45,
    };

const TInt KMultitapDataSize_Thai = 160;


const TPtiKeyBinding MultitapKeyBindings_table_Thai[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 6},
        {EPtiKey2, EPtiCaseLower, 6, 5},
        {EPtiKey3, EPtiCaseLower, 11, 5},
        {EPtiKey4, EPtiCaseLower, 16, 7},
        {EPtiKey5, EPtiCaseLower, 23, 6},
        {EPtiKey6, EPtiCaseLower, 29, 6},
        {EPtiKey7, EPtiCaseLower, 35, 6},
        {EPtiKey8, EPtiCaseLower, 41, 8},
        {EPtiKey9, EPtiCaseLower, 49, 6},
        {EPtiKeyStar, EPtiCaseLower, 55, 16},
        {EPtiKey0, EPtiCaseLower, 71, 9},
        {EPtiKeyHash, EPtiCaseLower, 80, 0},
        {EPtiKey1, EPtiCaseUpper, 80, 6},
        {EPtiKey2, EPtiCaseUpper, 86, 5},
        {EPtiKey3, EPtiCaseUpper, 91, 5},
        {EPtiKey4, EPtiCaseUpper, 96, 7},
        {EPtiKey5, EPtiCaseUpper, 103, 6},
        {EPtiKey6, EPtiCaseUpper, 109, 6},
        {EPtiKey7, EPtiCaseUpper, 115, 6},
        {EPtiKey8, EPtiCaseUpper, 121, 8},
        {EPtiKey9, EPtiCaseUpper, 129, 6},
        {EPtiKeyStar, EPtiCaseUpper, 135, 16},
        {EPtiKey0, EPtiCaseUpper, 151, 9},
        {EPtiKeyHash, EPtiCaseUpper, 160, 0},
    };

const TInt KMaxMultitapIndex_Thai = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Thai[] =
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
     0x3f,
     0xe46,
     0xe44,
     0xe33,
     0xe1e,
     0xe30,
     0xe31,
     0xe35,
     0xe23,
     0xe19,
     0xe22,
     0xe1a,
     0x2c,
     0xe1f,
     0xe2b,
     0xe01,
     0xe14,
     0xe40,
     0xe49,
     0xe48,
     0xe32,
     0xe2a,
     0xe27,
     0xe07,
     0xe1c,
     0xe1b,
     0xe41,
     0xe2d,
     0xe34,
     0xe37,
     0xe17,
     0xe21,
     0xe43,
     0xe1d,
     0xe45,
     0x40,
     0xe20,
     0xe16,
     0xe39,
     0xe38,
     0xe36,
     0xe04,
     0xe15,
     0xe08,
     0x21,
     0xe02,
     0xe0a,
     0xe0e,
     0xe11,
     0xe18,
     0xe4d,
     0xe4a,
     0xe13,
     0xe2f,
     0xe0d,
     0xe10,
     0x2e,
     0xe25,
     0xe06,
     0xe0f,
     0xe42,
     0xe0c,
     0xe47,
     0xe4b,
     0xe29,
     0xe28,
     0xe0b,
     0x22,
     0xe1c,
     0xe1b,
     0xe09,
     0xe2e,
     0xe34,
     0xe4c,
     0xe17,
     0xe12,
     0xe2c,
     0x2f,
    };

const TInt KStandardQwertyDataSize_Thai = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Thai[] =
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

const TInt KMaxStandardQwertyIndex_Thai = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Thai[] =
	{
	{'+',EPtiKeyQwertyPlus,EPtiCaseLower},
	{'*',EPtiKeyQwerty8,EPtiCaseUpper},
	{'#',EPtiKeyQwertyHash,EPtiCaseLower},
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{';',EPtiKeyQwertySemicolon,EPtiCaseLower},
	{'-',EPtiKeyQwertyMinus,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Thai = 8;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Thai[] =
    {
     0x0E46,
     0x0E44,
     0x0E33,
     0x0E1E,
     0x0E30,
     0x0E31,
     0x0E35,
     0x0E23,
     0x0E19,
     0x0E22,
     0x0E1F,
     0x0E2B,
     0x0E01,
     0x0E14,
     0x0E40,
     0x0E49,
     0x0E48,
     0x0E32,
     0x0E2A,
     0x0E1C,
     0x0E1B,
     0x0E41,
     0x0E2D,
     0x0E34,
     0x0E37,
     0x0E17,
     0x0E21,
     0x0E43,
     0x0E03,
     0x0040,
     0x0E07,
     0x0E36,
     0x0E45,
     0x0E16,
     0x0E0E,
     0x0E11,
     0x0E18,
     0x0E4D,
     0x0E4A,
     0x0E13,
     0x0E2F,
     0x0E0D,
     0x0E24,
     0x0E06,
     0x0E0F,
     0x0E42,
     0x0E0C,
     0x0E47,
     0x0E4B,
     0x0E29,
     0x0E28,
     0x0E27,
     0x0E0B,
     0x0E09,
     0x0E2E,
     0x0E4C,
     0x0E15,
     0x0E12,
     0x0E2C,
     0x0E10,
     0x0E39,
     0x0E25,
     0x0E20,
     0x0E38,
     0x0031,
     0x0032,
     0x0033,
     0x0E04,
     0x0E02,
     0x0E1A,
     0x0034,
     0x0035,
     0x0036,
     0x0E08,
     0x0E26,
     0x0E0A,
     0x0037,
     0x0038,
     0x0039,
     0x0E1D,
     0x002A,
     0x0030,
     0x0023,
    };

const TInt KMiniQwertyDataSize_Thai = 83;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Thai[] =
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
        {EPtiKeyQwertyB, EPtiCaseUpper, 55, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwerty1, EPtiCaseFnUpper, 62, 1},
        {EPtiKeyQwerty2, EPtiCaseFnUpper, 63, 1},
        {EPtiKeyQwerty3, EPtiCaseFnUpper, 64, 1},
        {EPtiKeyQwerty5, EPtiCaseFnUpper, 65, 1},
        {EPtiKeyQwerty7, EPtiCaseFnUpper, 66, 1},
        {EPtiKeyQwerty9, EPtiCaseFnUpper, 67, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 68, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 69, 1},
        {EPtiKeyQwertyD, EPtiCaseFnUpper, 70, 1},
        {EPtiKeyQwertyG, EPtiCaseFnUpper, 71, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnUpper, 72, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 73, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 74, 1},
        {EPtiKeyQwertyX, EPtiCaseFnUpper, 75, 1},
        {EPtiKeyQwertyC, EPtiCaseFnUpper, 76, 1},
        {EPtiKeyQwertyB, EPtiCaseFnUpper, 77, 1},
        {EPtiKeyQwertyM, EPtiCaseFnUpper, 78, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnUpper, 79, 1},
    };

const TInt KMaxMiniQwertyIndex_Thai = 74;

//
// Class implementation.
//

CPtiLangDataImplThai* CPtiLangDataImplThai::NewL()
    {
    return new (ELeave) CPtiLangDataImplThai();
    }


TInt CPtiLangDataImplThai::LanguageCode() const
    {
    return ELangThai;
    }


const TUint16* CPtiLangDataImplThai::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Thai;
        return MultitapData_table_Thai;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Thai;
        return StandardQwertyData_table_Thai;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Thai;
        return MiniQwertyData_table_Thai;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplThai::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Thai;
        return MultitapKeyBindings_table_Thai;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Thai;
        return StandardQwertyKeyBindings_table_Thai;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Thai;
        return MiniQwertyKeyBindings_table_Thai;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplThai::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Thai;
        return StandardQwertyNumericKeysData_table_Thai;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplThai::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


