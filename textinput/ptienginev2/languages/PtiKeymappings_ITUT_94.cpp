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
     0x641,0x642,0x6a9,0x6af,0x644,0x645,0x6f8,
     0x637,0x638,0x639,0x63a,0x6f9,
     0x20,0x6f0,0x21b3,
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
     0x0637,
     0x0635,
     0x06BE,
     0x062F,
     0x0679,
     0x067E,
     0x062A,
     0x0628,
     0x062C,
     0x062D,
     0x0021,
     0x0645,
     0x0648,
     0x0631,
     0x0646,
     0x0644,
     0x06C1,
     0x0627,
     0x06A9,
     0x06CC,
     0x061B,
     0x005C,
     0x0642,
     0x0641,
     0x06D2,
     0x0633,
     0x0634,
     0x063A,
     0x0639,
     0x060C,
     0x002E,
     0x002F,
     0x061F,
     0x0638,
     0x0636,
     0x0630,
     0x0688,
     0x062B,
     0x06C3,
     0x0628,
     0x0686,
     0x062E,
     0x0029,
     0x0028,
     0x0698,
     0x0632,
     0x0691,
     0x06BA,
     0x06C2,
     0x0621,
     0x0622,
     0x06AF,
     0x064A,
     0x003A,
     0x0022,
     0x200C,
     0x200D,
     0x06D3,
     0x200E,
     0x0624,
     0x0626,
     0x200F,
     0x003E,
     0x003C,
     0x005F,
     0x0640,
    };

const TInt KStandardQwertyDataSize_Urdu = 66;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Urdu[] =
    {
        {EPtiKeyQwertyAtCharacter, EPtiCaseLower, 0, 1},
        {EPtiKeyQwerty1, EPtiCaseLower, 1, 1},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 1},
        {EPtiKeyQwerty3, EPtiCaseLower, 3, 1},
        {EPtiKeyQwerty4, EPtiCaseLower, 4, 1},
        {EPtiKeyQwerty5, EPtiCaseLower, 5, 1},
        {EPtiKeyQwerty6, EPtiCaseLower, 6, 1},
        {EPtiKeyQwerty7, EPtiCaseLower, 7, 1},
        {EPtiKeyQwerty8, EPtiCaseLower, 8, 1},
        {EPtiKeyQwerty9, EPtiCaseLower, 9, 1},
        {EPtiKeyQwerty0, EPtiCaseLower, 10, 1},
        {EPtiKeyQwertyPlus, EPtiCaseLower, 11, 1},
        {EPtiKeyQwertyQ, EPtiCaseLower, 12, 1},
        {EPtiKeyQwertyW, EPtiCaseLower, 13, 1},
        {EPtiKeyQwertyE, EPtiCaseLower, 14, 1},
        {EPtiKeyQwertyR, EPtiCaseLower, 15, 1},
        {EPtiKeyQwertyT, EPtiCaseLower, 16, 1},
        {EPtiKeyQwertyY, EPtiCaseLower, 17, 1},
        {EPtiKeyQwertyU, EPtiCaseLower, 18, 1},
        {EPtiKeyQwertyI, EPtiCaseLower, 19, 1},
        {EPtiKeyQwertyO, EPtiCaseLower, 20, 1},
        {EPtiKeyQwertyP, EPtiCaseLower, 21, 1},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 22, 0},
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
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 33, 0},
        {0x5b, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyZ, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyC, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyV, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyB, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyN, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyM, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyComma, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertySlash, EPtiCaseLower, 33, 0},
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 33, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 34, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 35, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 36, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 37, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 38, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 39, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 40, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 41, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 42, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 43, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 55, 0},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 66, 0},
        {0x5b, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyX, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyC, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyV, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyB, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyN, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyM, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 66, 0},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 66, 0},
    };

const TInt KMaxStandardQwertyIndex_Urdu = 92;

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


