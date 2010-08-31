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

#include "PtiKeymappings_ITUT_72.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Marathi[] =
    {
     0x0901,0x0902,0x0903,0x093C,0x094D,0x0977,0x0972,0x0971,0x0031,0x002E,0x002C,0x003f,0x0021,0x0964,
     0x093E,0x093F,0x0940,0x0941,0x0942,0x0943,0x0032,
     0x0947,0x0948,0x0945,0x094B,0x094C,0x0949,0x0033,
     0x0915,0x0916,0x0917,0x0918,0x0919,0x0034,
     0x091A,0x091B,0x091C,0x091D,0x091E,0x0035,
     0x091F,0x0920,0x0921,0x0922,0x0923,0x0036,
     0x0924,0x0925,0x0926,0x0927,0x0928,0x0037,
     0x092A,0x092B,0x092C,0x092D,0x092E,0x0038,
     0x092F,0x0930,0x0932,0x0935,0x0936,0x0937,0x0938,0x0939,0x0933,0x0039,
     0x0020,0x0030,0x21B2,0x0973,0x0974,0x0975,0x0976,
     0x0901,0x0902,0x0903,0x093C,0x094D,0x0977,0x0972,0x0971,0x0031,0x002E,0x002C,0x003f,0x0021,0x0964,
     0x0905,0x0906,0x0907,0x0908,0x0909,0x090A,0x090B,0x0032,0x0978,
     0x090F,0x0910,0x090D,0x0913,0x0914,0x0911,0x0033,
     0x0915,0x0916,0x0917,0x0918,0x0919,0x0034,
     0x091A,0x091B,0x091C,0x091D,0x091E,0x0035,
     0x091F,0x0920,0x0921,0x0922,0x0923,0x0036,
     0x0924,0x0925,0x0926,0x0927,0x0928,0x0037,
     0x092A,0x092B,0x092C,0x092D,0x092E,0x0038,
     0x092F,0x0930,0x0932,0x0935,0x0936,0x0937,0x0938,0x0939,0x0933,0x0039,
     0x0020,0x0030,0x21B2,
    };

const TInt KMultitapDataSize_Marathi = 148;


const TPtiKeyBinding MultitapKeyBindings_table_Marathi[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 14},
        {EPtiKey2, EPtiCaseLower, 14, 7},
        {EPtiKey3, EPtiCaseLower, 21, 7},
        {EPtiKey4, EPtiCaseLower, 28, 6},
        {EPtiKey5, EPtiCaseLower, 34, 6},
        {EPtiKey6, EPtiCaseLower, 40, 6},
        {EPtiKey7, EPtiCaseLower, 46, 6},
        {EPtiKey8, EPtiCaseLower, 52, 6},
        {EPtiKey9, EPtiCaseLower, 58, 10},
        {EPtiKeyStar, EPtiCaseLower, 68, 0},
        {EPtiKey0, EPtiCaseLower, 68, 7},
        {EPtiKeyHash, EPtiCaseLower, 75, 0},
        {EPtiKey1, EPtiCaseUpper, 75, 14},
        {EPtiKey2, EPtiCaseUpper, 89, 9},
        {EPtiKey3, EPtiCaseUpper, 98, 7},
        {EPtiKey4, EPtiCaseUpper, 105, 6},
        {EPtiKey5, EPtiCaseUpper, 111, 6},
        {EPtiKey6, EPtiCaseUpper, 117, 6},
        {EPtiKey7, EPtiCaseUpper, 123, 6},
        {EPtiKey8, EPtiCaseUpper, 129, 6},
        {EPtiKey9, EPtiCaseUpper, 135, 10},
        {EPtiKeyStar, EPtiCaseUpper, 145, 0},
        {EPtiKey0, EPtiCaseUpper, 145, 3},
        {EPtiKeyHash, EPtiCaseUpper, 148, 0},
    };

const TInt KMaxMultitapIndex_Marathi = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Marathi[] =
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

const TInt KStandardQwertyDataSize_Marathi = 99;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Marathi[] =
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

const TInt KMaxStandardQwertyIndex_Marathi = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Marathi[] =
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

const TInt KNumStandardQwertyNumericKeysEntries_Marathi = 18;
//
// Class implementation.
//

CPtiLangDataImplMarathi* CPtiLangDataImplMarathi::NewL()
    {
    return new (ELeave) CPtiLangDataImplMarathi();
    }


TInt CPtiLangDataImplMarathi::LanguageCode() const
    {
    return ELangMarathi;
    }


const TUint16* CPtiLangDataImplMarathi::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Marathi;
        return MultitapData_table_Marathi;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Marathi;
        return StandardQwertyData_table_Marathi;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplMarathi::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Marathi;
        return MultitapKeyBindings_table_Marathi;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Marathi;
        return StandardQwertyKeyBindings_table_Marathi;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplMarathi::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Marathi;
        return StandardQwertyNumericKeysData_table_Marathi;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplMarathi::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


