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
     0x002C,
     0x0443,
     0x0435,
     0x0438,
     0x0448,
     0x0449,
     0x043A,
     0x0441,
     0x0434,
     0x0437,
     0x0446,
     0x044C,
     0x044F,
     0x0430,
     0x043E,
     0x0436,
     0x0433,
     0x0442,
     0x043D,
     0x0432,
     0x043C,
     0x0447,
     0x044E,
     0x0439,
     0x044A,
     0x044D,
     0x0444,
     0x0445,
     0x043F,
     0x0440,
     0x043B,
     0x0431,
     0x002E,
     0x044B,
     0x0423,
     0x0415,
     0x0418,
     0x0428,
     0x0429,
     0x041A,
     0x0421,
     0x0414,
     0x0417,
     0x0426,
     0x042C,
     0x042F,
     0x0410,
     0x041E,
     0x0416,
     0x0413,
     0x0422,
     0x041D,
     0x0412,
     0x041C,
     0x0427,
     0x042E,
     0x0419,
     0x042A,
     0x042D,
     0x0424,
     0x0425,
     0x041F,
     0x0420,
     0x041B,
     0x0411,
     0x003F,
    };

const TInt KStandardQwertyDataSize_Bulgarian = 66;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Bulgarian[] =
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

const TInt KMaxStandardQwertyIndex_Bulgarian = 92;

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


