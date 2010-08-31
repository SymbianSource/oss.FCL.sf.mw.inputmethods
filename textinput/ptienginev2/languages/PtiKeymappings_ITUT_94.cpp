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

#include "PtiKeymappings_ITUT_94.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Urdu[] =
    {
     0x2e,0x61f,0x3a,0x60c,0x21,0x6f1,
     0x628,0x67e,0x62a,0x679,0x62b,0x6c3,0x6f2,
     0x627,0x622,0x624,0x626,0x6d3,0x621,0x6f3,
     0x633,0x634,0x635,0x636,0x6f4,
     0x62f,0x688,0x630,0x631,0x691,0x632,0x698,0x6f5,
     0x62c,0x686,0x62d,0x62e,0x6f6,
     0x646,0x6ba,0x648,0x6be,0x6c1,0x6c2,0x6cc,0x6d2,0x6f7,
     0x641,0x642,0x6a9,0x6af,0x644,0x645,0x6f8,
     0x637,0x638,0x639,0x63a,0x6f9,
     0x20,0x6f0,0x21b3,
     0x2e,0x61f,0x3a,0x60c,0x21,0x6f1,
     0x628,0x67e,0x62a,0x679,0x62b,0x6c3,0x6f2,
     0x627,0x622,0x624,0x626,0x6d3,0x621,0x6f3,
     0x633,0x634,0x635,0x636,0x6f4,
     0x62f,0x688,0x630,0x631,0x691,0x632,0x698,0x6f5,
     0x62c,0x686,0x62d,0x62e,0x6f6,
     0x646,0x6ba,0x648,0x6be,0x6c1,0x6c2,0x6cc,0x6d2,0x6f7,
     0x641,0x642,0x6a9,0x6af,0x644,0x645,0x38,
     0x637,0x638,0x639,0x63a,0x6f9,
     0x20,0x30,0x21b3,
    };

const TInt KMultitapDataSize_Urdu = 124;


const TPtiKeyBinding MultitapKeyBindings_table_Urdu[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 6},
        {EPtiKey2, EPtiCaseLower, 6, 7},
        {EPtiKey3, EPtiCaseLower, 13, 7},
        {EPtiKey4, EPtiCaseLower, 20, 5},
        {EPtiKey5, EPtiCaseLower, 25, 8},
        {EPtiKey6, EPtiCaseLower, 33, 5},
        {EPtiKey7, EPtiCaseLower, 38, 9},
        {EPtiKey8, EPtiCaseLower, 47, 7},
        {EPtiKey9, EPtiCaseLower, 54, 5},
        {EPtiKeyStar, EPtiCaseLower, 59, 0},
        {EPtiKey0, EPtiCaseLower, 59, 3},
        {EPtiKeyHash, EPtiCaseLower, 62, 0},
        {EPtiKey1, EPtiCaseUpper, 62, 6},
        {EPtiKey2, EPtiCaseUpper, 68, 7},
        {EPtiKey3, EPtiCaseUpper, 75, 7},
        {EPtiKey4, EPtiCaseUpper, 82, 5},
        {EPtiKey5, EPtiCaseUpper, 87, 8},
        {EPtiKey6, EPtiCaseUpper, 95, 5},
        {EPtiKey7, EPtiCaseUpper, 100, 9},
        {EPtiKey8, EPtiCaseUpper, 109, 7},
        {EPtiKey9, EPtiCaseUpper, 116, 5},
        {EPtiKeyStar, EPtiCaseUpper, 121, 0},
        {EPtiKey0, EPtiCaseUpper, 121, 3},
        {EPtiKeyHash, EPtiCaseUpper, 124, 0},
    };

const TInt KMaxMultitapIndex_Urdu = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Urdu[] =
    {
     0x06f1,
     0x06f2,
     0x06f3,
     0x06f4,
     0x06f5,
     0x06f6,
     0x06f7,
     0x06f8,
     0x06f9,
     0x06f0,
     0x0637,
     0x0635,
     0x06be,
     0x062f,
     0x06BB,
     0x067e,
     0x062a,
     0x0628,
     0x062c,
     0x062d,
     0x002d,
     0x003d,
     0x0645,
     0x0648,
     0x0631,
     0x0646,
     0x0644,
     0x0647,
     0x0627,
     0x06a9,
     0x06cc,
     0x061b,
     0x0027,
     0x005c,
     0x0642,
     0x0641,
     0x06d2,
     0x0633,
     0x0634,
     0x063a,
     0x0639,
     0x060c,
     0x06d4,
     0x002f,
     0x0021,
     0x0040,
     0x0023,
     0x0024,
     0x066a,
     0x005e,
     0x0029,
     0x0028,
     0x005f,
     0x002b,
     0x0638,
     0x0636,
     0x0630,
     0x0688,
     0x062b,
     0x067e,
     0x06c3,
     0x0628,
     0x0686,
     0x062e,
     0x007d,
     0x007b,
     0x0698,
     0x0632,
     0x0691,
     0x06ba,
     0x06c2,
     0x0621,
     0x0622,
     0x06af,
     0x064a,
     0x003a,
     0x0022,
     0x002a,
     0x200d,
     0x200c,
     0x06d3,
     0x200e,
     0x0624,
     0x0626,
     0x200f,
     0x003e,
     0x003c,
     0x061f,
    };

const TInt KStandardQwertyDataSize_Urdu = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Urdu[] =
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

const TInt KMaxStandardQwertyIndex_Urdu = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_Urdu[] =
	{
	{'*',EPtiKeyQwerty7,EPtiCaseUpper},
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
	{';',EPtiKeyQwertySemicolon,EPtiCaseLower},
	{'.',EPtiKeyQwertyFullstop,EPtiCaseLower},
	{',',EPtiKeyQwertyComma,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	{'?',EPtiKeyQwertySlash,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_Urdu = 18;
//
// Class implementation.
//

CPtiLangDataImplUrdu* CPtiLangDataImplUrdu::NewL()
    {
    return new (ELeave) CPtiLangDataImplUrdu();
    }


TInt CPtiLangDataImplUrdu::LanguageCode() const
    {
    return ELangUrdu;
    }


const TUint16* CPtiLangDataImplUrdu::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Urdu;
        return MultitapData_table_Urdu;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Urdu;
        return StandardQwertyData_table_Urdu;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplUrdu::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Urdu;
        return MultitapKeyBindings_table_Urdu;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Urdu;
        return StandardQwertyKeyBindings_table_Urdu;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplUrdu::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_Urdu;
        return StandardQwertyNumericKeysData_table_Urdu;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplUrdu::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


