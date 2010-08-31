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

#include "PtiKeymappings_ITUT_42.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Bulgarian[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x430,0x431,0x432,0x433,0x32,0x61,0x62,0x63,
     0x434,0x435,0x436,0x437,0x33,0x64,0x65,0x66,
     0x438,0x439,0x43a,0x43b,0x34,0x67,0x68,0x69,
     0x43c,0x43d,0x43e,0x43f,0x35,0x6a,0x6b,0x6c,
     0x440,0x441,0x442,0x443,0x36,0x6d,0x6e,0x6f,
     0x444,0x445,0x446,0x447,0x37,0x70,0x71,0x72,0x73,
     0x448,0x449,0x44a,0x38,0x74,0x75,0x76,
     0x44c,0x44e,0x44f,0x39,0x44b,0x44d,0x77,0x78,0x79,0x7a,
     0x20,0x30,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x410,0x411,0x412,0x413,0x32,0x41,0x42,0x43,
     0x414,0x415,0x416,0x417,0x33,0x44,0x45,0x46,
     0x418,0x419,0x41a,0x41b,0x34,0x47,0x48,0x49,
     0x41c,0x41d,0x41e,0x41f,0x35,0x4a,0x4b,0x4c,
     0x420,0x421,0x422,0x423,0x36,0x4d,0x4e,0x4f,
     0x424,0x425,0x426,0x427,0x37,0x50,0x51,0x52,0x53,
     0x428,0x429,0x42a,0x38,0x54,0x55,0x56,
     0x42c,0x42e,0x42f,0x39,0x42b,0x42d,0x57,0x58,0x59,0x5a,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Bulgarian = 198;


const TPtiKeyBinding MultitapKeyBindings_table_Bulgarian[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 8},
        {EPtiKey3, EPtiCaseLower, 38, 8},
        {EPtiKey4, EPtiCaseLower, 46, 8},
        {EPtiKey5, EPtiCaseLower, 54, 8},
        {EPtiKey6, EPtiCaseLower, 62, 8},
        {EPtiKey7, EPtiCaseLower, 70, 9},
        {EPtiKey8, EPtiCaseLower, 79, 7},
        {EPtiKey9, EPtiCaseLower, 86, 10},
        {EPtiKeyStar, EPtiCaseLower, 96, 0},
        {EPtiKey0, EPtiCaseLower, 96, 3},
        {EPtiKeyHash, EPtiCaseLower, 99, 0},
        {EPtiKey1, EPtiCaseUpper, 99, 30},
        {EPtiKey2, EPtiCaseUpper, 129, 8},
        {EPtiKey3, EPtiCaseUpper, 137, 8},
        {EPtiKey4, EPtiCaseUpper, 145, 8},
        {EPtiKey5, EPtiCaseUpper, 153, 8},
        {EPtiKey6, EPtiCaseUpper, 161, 8},
        {EPtiKey7, EPtiCaseUpper, 169, 9},
        {EPtiKey8, EPtiCaseUpper, 178, 7},
        {EPtiKey9, EPtiCaseUpper, 185, 10},
        {EPtiKeyStar, EPtiCaseUpper, 195, 0},
        {EPtiKey0, EPtiCaseUpper, 195, 3},
        {EPtiKeyHash, EPtiCaseUpper, 198, 0},
    };

const TInt KMaxMultitapIndex_Bulgarian = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Bulgarian[] =
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
     0x2c,
     0x443,
     0x435,
     0x438,
     0x448,
     0x449,
     0x43a,
     0x441,
     0x434,
     0x437,
     0x446,
     0x2e,
     0x44c,
     0x44f,
     0x430,
     0x43e,
     0x436,
     0x433,
     0x442,
     0x43d,
     0x432,
     0x43c,
     0x447,
     0x28,
     0x44e,
     0x439,
     0x44a,
     0x44d,
     0x444,
     0x445,
     0x43f,
     0x440,
     0x43b,
     0x431,
     0x21,
     0x3f,
     0x2b,
     0x22,
     0x25,
     0x3d,
     0x3a,
     0x2f,
     0x5f,
     0x3b,
     0x44b,
     0x423,
     0x415,
     0x418,
     0x428,
     0x429,
     0x41a,
     0x421,
     0x414,
     0x417,
     0x426,
     0x2d,
     0x42c,
     0x42f,
     0x410,
     0x41e,
     0x416,
     0x413,
     0x422,
     0x41d,
     0x412,
     0x41c,
     0x427,
     0x29,
     0x42e,
     0x419,
     0x42a,
     0x42d,
     0x424,
     0x425,
     0x41f,
     0x420,
     0x41b,
     0x411,
    };

const TInt KStandardQwertyDataSize_Bulgarian = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Bulgarian[] =
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

const TInt KMaxStandardQwertyIndex_Bulgarian = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Bulgarian[] =
	{
	{'*',EPtiKeyQwerty8,EPtiCaseUpper},
	{'#',EPtiKeyQwertyHash,EPtiCaseLower},
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Bulgarian = 5;
//
// Class implementation.
//

CPtiLangDataImplBulgarian* CPtiLangDataImplBulgarian::NewL()
    {
    return new (ELeave) CPtiLangDataImplBulgarian();
    }


TInt CPtiLangDataImplBulgarian::LanguageCode() const
    {
    return ELangBulgarian;
    }


const TUint16* CPtiLangDataImplBulgarian::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Bulgarian;
        return MultitapData_table_Bulgarian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Bulgarian;
        return StandardQwertyData_table_Bulgarian;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplBulgarian::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Bulgarian;
        return MultitapKeyBindings_table_Bulgarian;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Bulgarian;
        return StandardQwertyKeyBindings_table_Bulgarian;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplBulgarian::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Bulgarian;
        return StandardQwertyNumericKeysData_table_Bulgarian;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplBulgarian::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


