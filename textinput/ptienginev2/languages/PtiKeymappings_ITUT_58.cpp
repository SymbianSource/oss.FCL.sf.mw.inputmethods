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

#include "PtiKeymappings_ITUT_58.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Hindi[] =
    {
     0x901,0x902,0x903,0x94d,0x31,
     0x93e,0x93f,0x940,0x941,0x942,0x943,0x32,
     0x947,0x948,0x945,0x94b,0x94c,0x949,0x33,
     0x915,0x916,0x917,0x918,0x919,0x34,0x958,0x959,0x95a,
     0x91a,0x91b,0x91c,0x91d,0x91e,0x35,0x95b,
     0x91f,0x920,0x921,0x922,0x923,0x36,0x95c,0x95d,
     0x924,0x925,0x926,0x927,0x928,0x37,0x929,
     0x92a,0x92b,0x92c,0x92d,0x92e,0x38,0x95e,
     0x92f,0x930,0x932,0x933,0x935,0x936,0x937,0x938,0x939,0x39,0x931,0x934,0x95f,
     0x20,0x30,0x21b2,
     0x901,0x902,0x903,0x31,
     0x905,0x906,0x907,0x908,0x909,0x90a,0x90b,0x32,
     0x90f,0x910,0x90d,0x913,0x914,0x911,0x33,
     0x915,0x916,0x917,0x918,0x919,0x34,0x958,0x959,0x95a,
     0x91a,0x91b,0x91c,0x91d,0x91e,0x35,0x95b,
     0x91f,0x920,0x921,0x922,0x923,0x36,0x95c,0x95d,
     0x924,0x925,0x926,0x927,0x928,0x37,0x929,
     0x92a,0x92b,0x92c,0x92d,0x92e,0x38,0x95e,
     0x92f,0x930,0x932,0x933,0x935,0x936,0x937,0x938,0x939,0x39,0x931,0x934,0x95f,
     0x20,0x30,0x21b2,
    };

const TInt KMultitapDataSize_Hindi = 146;


const TPtiKeyBinding MultitapKeyBindings_table_Hindi[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 5},
        {EPtiKey2, EPtiCaseLower, 5, 7},
        {EPtiKey3, EPtiCaseLower, 12, 7},
        {EPtiKey4, EPtiCaseLower, 19, 9},
        {EPtiKey5, EPtiCaseLower, 28, 7},
        {EPtiKey6, EPtiCaseLower, 35, 8},
        {EPtiKey7, EPtiCaseLower, 43, 7},
        {EPtiKey8, EPtiCaseLower, 50, 7},
        {EPtiKey9, EPtiCaseLower, 57, 13},
        {EPtiKeyStar, EPtiCaseLower, 70, 0},
        {EPtiKey0, EPtiCaseLower, 70, 3},
        {EPtiKeyHash, EPtiCaseLower, 73, 0},
        {EPtiKey1, EPtiCaseUpper, 73, 4},
        {EPtiKey2, EPtiCaseUpper, 77, 8},
        {EPtiKey3, EPtiCaseUpper, 85, 7},
        {EPtiKey4, EPtiCaseUpper, 92, 9},
        {EPtiKey5, EPtiCaseUpper, 101, 7},
        {EPtiKey6, EPtiCaseUpper, 108, 8},
        {EPtiKey7, EPtiCaseUpper, 116, 7},
        {EPtiKey8, EPtiCaseUpper, 123, 7},
        {EPtiKey9, EPtiCaseUpper, 130, 13},
        {EPtiKeyStar, EPtiCaseUpper, 143, 0},
        {EPtiKey0, EPtiCaseUpper, 143, 3},
        {EPtiKeyHash, EPtiCaseUpper, 146, 0},
    };

const TInt KMaxMultitapIndex_Hindi = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Hindi[] =
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
     0x94c,
     0x948,
     0x93e,
     0x940,
     0x942,
     0x92c,
     0x939,
     0x917,
     0x926,
     0x91c,
     0x921,
     0x93c,
     0x94b,
     0x947,
     0x94d,
     0x93f,
     0x941,
     0x92a,
     0x930,
     0x915,
     0x924,
     0x91a,
     0x91f,
     0x943,
     0x902,
     0x92e,
     0x928,
     0x935,
     0x932,
     0x938,
     0x2c,
     0x2e,
     0x92f,
     0x90d,
     0x945,
     0x903,
     0x2464,0x930,0x94d,
     0x2464,0x91c,0x94d,0x91e,
     0x2464,0x936,0x94d,0x930,
     0x2464,0x915,0x94d,0x937,
     0x2a,
     0x949,
     0x911,
     0x2d,
     0x914,
     0x910,
     0x906,
     0x908,
     0x90a,
     0x92d,
     0x919,
     0x918,
     0x927,
     0x91d,
     0x922,
     0x91e,
     0x913,
     0x90f,
     0x905,
     0x907,
     0x909,
     0x92b,
     0x931,
     0x916,
     0x925,
     0x91b,
     0x920,
     0x90b,
     0x901,
     0x923,
     0x929,
     0x934,
     0x933,
     0x936,
     0x937,
     0x2f,
     0x95f,
    };

const TInt KStandardQwertyDataSize_Hindi = 99;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Hindi[] =
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
        {EPtiKeyQwerty4, EPtiCaseUpper, 47, 3},
        {EPtiKeyQwerty5, EPtiCaseUpper, 50, 4},
        {EPtiKeyQwerty6, EPtiCaseUpper, 54, 4},
        {EPtiKeyQwerty7, EPtiCaseUpper, 58, 4},
        {EPtiKeyQwerty8, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 66, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 67, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 68, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 70, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 71, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 91, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 92, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 93, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 94, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 95, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 96, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 97, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 98, 1},
    };

const TInt KMaxStandardQwertyIndex_Hindi = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Hindi[] =
	{
	{'+',EPtiKeyQwertyPlus,EPtiCaseLower},
	{'*',EPtiKeyQwerty8,EPtiCaseUpper},
	{'#',EPtiKeyQwertyHash,EPtiCaseLower},
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{'1',EPtiKeyQwerty1,EPtiCaseLower},
	{'2',EPtiKeyQwerty2,EPtiCaseLower},
	{'3',EPtiKeyQwerty3,EPtiCaseLower},
	{'4',EPtiKeyQwerty4,EPtiCaseLower},
	{'5',EPtiKeyQwerty5,EPtiCaseLower},
	{'6',EPtiKeyQwerty6,EPtiCaseLower},
	{'7',EPtiKeyQwerty7,EPtiCaseLower},
	{'8',EPtiKeyQwerty8,EPtiCaseLower},
	{'9',EPtiKeyQwerty9,EPtiCaseLower},
	{'0',EPtiKeyQwerty0,EPtiCaseLower},
	{'-',EPtiKeyQwertyPlus,EPtiCaseUpper},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	{'?',EPtiKeyQwertySlash,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Hindi = 18;
//
// Class implementation.
//

CPtiLangDataImplHindi* CPtiLangDataImplHindi::NewL()
    {
    return new (ELeave) CPtiLangDataImplHindi();
    }


TInt CPtiLangDataImplHindi::LanguageCode() const
    {
    return ELangHindi;
    }


const TUint16* CPtiLangDataImplHindi::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Hindi;
        return MultitapData_table_Hindi;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Hindi;
        return StandardQwertyData_table_Hindi;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplHindi::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Hindi;
        return MultitapKeyBindings_table_Hindi;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Hindi;
        return StandardQwertyKeyBindings_table_Hindi;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplHindi::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Hindi;
        return StandardQwertyNumericKeysData_table_Hindi;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplHindi::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


