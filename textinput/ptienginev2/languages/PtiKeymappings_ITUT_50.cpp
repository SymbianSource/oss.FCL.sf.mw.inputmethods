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

#include "PtiKeymappings_ITUT_50.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Farsi[] =
    {
     0x2e,0x61f,0x3a,0x60c,0x21,0x31,
     0x628,0x67e,0x62a,0x629,0x62b,0x32,
     0x627,0x623,0x625,0x622,0x624,0x626,0x621,0x33,
     0x633,0x634,0x635,0x636,0x34,
     0x62f,0x630,0x631,0x632,0x698,0x35,
     0x62c,0x686,0x62d,0x62e,0x36,
     0x646,0x648,0x647,0x6cc,0x37,
     0x641,0x642,0x6a9,0x6af,0x644,0x645,0x38,
     0x637,0x638,0x639,0x63a,0x39,
     0x20,0x30,0x21b3,
     0x2e,0x61f,0x3a,0x60c,0x21,0x31,
     0x628,0x67e,0x62a,0x629,0x62b,0x32,
     0x627,0x623,0x625,0x622,0x624,0x626,0x621,0x33,
     0x633,0x634,0x635,0x636,0x34,
     0x62f,0x630,0x631,0x632,0x698,0x35,
     0x62c,0x686,0x62d,0x62e,0x36,
     0x646,0x648,0x647,0x6cc,0x37,
     0x641,0x642,0x6a9,0x6af,0x644,0x645,0x38,
     0x637,0x638,0x639,0x63a,0x39,
     0x20,0x30,0x21b3,
    };

const TInt KMultitapDataSize_Farsi = 112;


const TPtiKeyBinding MultitapKeyBindings_table_Farsi[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 6},
        {EPtiKey2, EPtiCaseLower, 6, 6},
        {EPtiKey3, EPtiCaseLower, 12, 8},
        {EPtiKey4, EPtiCaseLower, 20, 5},
        {EPtiKey5, EPtiCaseLower, 25, 6},
        {EPtiKey6, EPtiCaseLower, 31, 5},
        {EPtiKey7, EPtiCaseLower, 36, 5},
        {EPtiKey8, EPtiCaseLower, 41, 7},
        {EPtiKey9, EPtiCaseLower, 48, 5},
        {EPtiKeyStar, EPtiCaseLower, 53, 0},
        {EPtiKey0, EPtiCaseLower, 53, 3},
        {EPtiKeyHash, EPtiCaseLower, 56, 0},
        {EPtiKey1, EPtiCaseUpper, 56, 6},
        {EPtiKey2, EPtiCaseUpper, 62, 6},
        {EPtiKey3, EPtiCaseUpper, 68, 8},
        {EPtiKey4, EPtiCaseUpper, 76, 5},
        {EPtiKey5, EPtiCaseUpper, 81, 6},
        {EPtiKey6, EPtiCaseUpper, 87, 5},
        {EPtiKey7, EPtiCaseUpper, 92, 5},
        {EPtiKey8, EPtiCaseUpper, 97, 7},
        {EPtiKey9, EPtiCaseUpper, 104, 5},
        {EPtiKeyStar, EPtiCaseUpper, 109, 0},
        {EPtiKey0, EPtiCaseUpper, 109, 3},
        {EPtiKeyHash, EPtiCaseUpper, 112, 0},
    };

const TInt KMaxMultitapIndex_Farsi = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Farsi[] =
    {
     0x6f1,
     0x6f2,
     0x6f3,
     0x6f4,
     0x6f5,
     0x6f6,
     0x6f7,
     0x6f8,
     0x6f9,
     0x6f0,
     0x636,
     0x635,
     0x62b,
     0x642,
     0x641,
     0x63a,
     0x639,
     0x647,
     0x62e,
     0x62d,
     0x62c,
     0x686,
     0x634,
     0x633,
     0x6cc,
     0x628,
     0x644,
     0x627,
     0x62a,
     0x646,
     0x645,
     0x6a9,
     0x6af,
     0x40,
     0x638,
     0x637,
     0x632,
     0x631,
     0x630,
     0x62f,
     0x67e,
     0x648,
     0x2e,
     0x2f,
     0x66c,
     0x66b,
     0xfdfc,
     0x66a,
     0x3d,
     0x21,
     0x2a,
     0x2d,
     0x2b,
     0x640,
     0x636,
     0x635,
     0x62b,
     0x642,
     0x641,
     0x63a,
     0x28,
     0x28,
     0x7d,
     0x7b,
     0x5c,
     0x7c,
     0x624,
     0x626,
     0x64a,
     0x625,
     0x623,
     0x622,
     0x629,
     0xbb,
     0xab,
     0x3a,
     0x61b,
     0x60c,
     0x643,
     0x637,
     0x698,
     0x631,
     0x200c,
     0x200d,
     0x621,
     0x3e,
     0x3c,
     0x61f,
    };

const TInt KStandardQwertyDataSize_Farsi = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Farsi[] =
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

const TInt KMaxStandardQwertyIndex_Farsi = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Farsi[] =
	{
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
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Farsi = 14;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Farsi[] =
    {
     0x0636,
     0x0635,
     0x062B,
     0x0642,
     0x0641,
     0x0639,
     0x0647,
     0x062D,
     0x0686,
     0x067E,
     0x0634,
     0x0633,
     0x06CC,
     0x0628,
     0x0644,
     0x0627,
     0x062A,
     0x0646,
     0x0645,
     0x0638,
     0x0637,
     0x0632,
     0x0631,
     0x0630,
     0x062F,
     0x0626,
     0x0648,
     0x06AF,
     0x0040,
     0x0026,
     0x060C,
     0x002E,
     0x0636,
     0x0635,
     0x062B,
     0x0642,
     0x0641,
     0x063A,
     0x0647,
     0x062E,
     0x062C,
     0x067E,
     0x0634,
     0x0633,
     0x06CC,
     0x0628,
     0x06C0,
     0x0622,
     0x0640,
     0x00AB,
     0x00BB,
     0x0629,
     0x064A,
     0x0698,
     0x0624,
     0x0625,
     0x0623,
     0x0621,
     0x0648,
     0x06A9,
     0x061B,
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
     0x061F,
     0x002A,
     0x002A,
     0x0030,
     0x0030,
     0x0023,
     0x0023,
    };

const TInt KMiniQwertyDataSize_Farsi = 95;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Farsi[] =
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
        {EPtiKeyQwerty1, EPtiCaseFnLower, 62, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 63, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 64, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 65, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 66, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 67, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 68, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 69, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 70, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 71, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 72, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 73, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 74, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 75, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 86, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 87, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 88, 1},
    };

const TInt KMaxMiniQwertyIndex_Farsi = 83;


//
// Numeric key data table for MiniQwerty
//
const TPtiNumericKeyBinding MiniQwertyNumericKeysData_table_Farsi[] =
	{
	{'p',EPtiKeyQwerty0,EPtiCaseLower},
	{'w',EPtiKeyQwerty2,EPtiCaseLower},
	{'E',EPtiKeyQwerty3,EPtiCaseUpper},
	};

const TInt KNumMiniQwertyNumericKeysEntries_Farsi = 3;
//
// Class implementation.
//

CPtiLangDataImplFarsi* CPtiLangDataImplFarsi::NewL()
    {
    return new (ELeave) CPtiLangDataImplFarsi();
    }


TInt CPtiLangDataImplFarsi::LanguageCode() const
    {
    return ELangFarsi;
    }


const TUint16* CPtiLangDataImplFarsi::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Farsi;
        return MultitapData_table_Farsi;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Farsi;
        return StandardQwertyData_table_Farsi;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Farsi;
        return MiniQwertyData_table_Farsi;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplFarsi::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Farsi;
        return MultitapKeyBindings_table_Farsi;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Farsi;
        return StandardQwertyKeyBindings_table_Farsi;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Farsi;
        return MiniQwertyKeyBindings_table_Farsi;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplFarsi::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Farsi;
        return StandardQwertyNumericKeysData_table_Farsi;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumEntries = KNumMiniQwertyNumericKeysEntries_Farsi;
        return MiniQwertyNumericKeysData_table_Farsi;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplFarsi::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


