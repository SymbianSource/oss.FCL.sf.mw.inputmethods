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

#include "PtiKeymappings_ITUT_96.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Vietnamese[] =
    {
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x61,0x103,0xe2,0x62,0x63,0x32, 0x301,
     0x64,0x111,0x65,0xea,0x66,0x33, 0x300,
     0x67,0x68,0x69,0x34, 0x309,
     0x6a,0x6b,0x6c,0x35, 0x303,
     0x6d,0x6e,0x6f,0xf4,0x1a1,0x36,0x323,
     0x70,0x71,0x72,0x73,0x37,
     0x74,0x75,0x1b0,0x76,0x38,
     0x77,0x78,0x79,0x7a,0x39,
     0x0020,0x0030,0x21b2,
     0x2e,0x2c,0x3f,0x21,0x31,0x40,0x27,0x2d,0x5f,0x28,0x29,0x3a,0x3b,0x26,0x2f,0x25,0x2a,0x23,0x2b,0x3c,0x3d,0x3e,0x22,0x24,0xa3,0xa7,0xa5,0xa4,0xa1,0xbf,
     0x41,0x102,0xc2,0x42,0x43,0x32, 0x301,
     0x44,0x110,0x45,0xca,0x46,0x33, 0x300,
     0x47,0x48,0x49,0x34, 0x309,
     0x4a,0x4b,0x4c,0x35, 0x303,
     0x4d,0x4e,0x4f,0xd4,0x1a0,0x36,0x323,
     0x50,0x51,0x52,0x53,0x37,
     0x54,0x55,0x1af,0x56,0x38,
     0x57,0x58,0x59,0x5a,0x39,
     0x0020,0x0030,0x21b2,
    };

const TInt KMultitapDataSize_Vietnamese = 158;


const TPtiKeyBinding MultitapKeyBindings_table_Vietnamese[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 30},
        {EPtiKey2, EPtiCaseLower, 30, 7},
        {EPtiKey3, EPtiCaseLower, 37, 7},
        {EPtiKey4, EPtiCaseLower, 44, 5},
        {EPtiKey5, EPtiCaseLower, 49, 5},
        {EPtiKey6, EPtiCaseLower, 54, 7},
        {EPtiKey7, EPtiCaseLower, 61, 5},
        {EPtiKey8, EPtiCaseLower, 66, 5},
        {EPtiKey9, EPtiCaseLower, 71, 5},
        {EPtiKeyStar, EPtiCaseLower, 76, 0},
        {EPtiKey0, EPtiCaseLower, 76, 3},
        {EPtiKeyHash, EPtiCaseLower, 79, 0},
        {EPtiKey1, EPtiCaseUpper, 79, 30},
        {EPtiKey2, EPtiCaseUpper, 109, 7},
        {EPtiKey3, EPtiCaseUpper, 116, 7},
        {EPtiKey4, EPtiCaseUpper, 123, 5},
        {EPtiKey5, EPtiCaseUpper, 128, 5},
        {EPtiKey6, EPtiCaseUpper, 133, 7},
        {EPtiKey7, EPtiCaseUpper, 140, 5},
        {EPtiKey8, EPtiCaseUpper, 145, 5},
        {EPtiKey9, EPtiCaseUpper, 150, 5},
        {EPtiKeyStar, EPtiCaseUpper, 155, 0},
        {EPtiKey0, EPtiCaseUpper, 155, 3},
        {EPtiKeyHash, EPtiCaseUpper, 158, 0},
    };

const TInt KMaxMultitapIndex_Vietnamese = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_Vietnamese[] =
    {
     0x0301,
     0x0300,
     0x0309,
     0x0303,
     0x0323,
     0x0103,
     0x00E2,
     0x00EA,
     0x00F4,
     0x01A1,
     0x01B0,
     0x0071,
     0x0077,
     0x0065,
     0x0072,
     0x0074,
     0x0079,
     0x0075,
     0x0069,
     0x006F,
     0x0070,
     0x0111,
     0x0061,
     0x0073,
     0x0064,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x0027,
     0x002D,
     0x007A,
     0x0078,
     0x0063,
     0x0076,
     0x0062,
     0x006E,
     0x006D,
     0x002C,
     0x002E,
     0x003F,
     0x0021,
     0x0301,
     0x0300,
     0x0309,
     0x0303,
     0x0323,
     0x0102,
     0x00C2,
     0x00CA,
     0x00D4,
     0x01A0,
     0x01AF,
     0x0051,
     0x0057,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004F,
     0x0050,
     0x0110,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x0022,
     0x002B,
     0x005A,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004E,
     0x004D,
     0x0026,
     0x0023,
     0x002F,
     0x005C,
    };

const TInt KStandardQwertyDataSize_Vietnamese = 88;


const TPtiKeyBinding StandardQwertyKeyBindings_table_Vietnamese[] =
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
        {0x5b, EPtiCaseLower, 33, 1},
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
        {EPtiKeyQwertyAtCharacter, EPtiCaseUpper, 44, 1},
        {EPtiKeyQwerty1, EPtiCaseUpper, 45, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 46, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 47, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 48, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 49, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 50, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 51, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 52, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 53, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 54, 1},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 55, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 56, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 57, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 58, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 59, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 60, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 61, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 63, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 64, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 65, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 66, 0},
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
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 77, 0},
        {0x5b, EPtiCaseUpper, 77, 1},
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

const TInt KMaxStandardQwertyIndex_Vietnamese = 92;


const TVowelSequence StandardQwertyVowelSeq_table_Vietnamese[] =
	{
	{TPtiKey(0x0041), TPtiKey(0x01AF), {0x0103, 0x0102}},
	{TPtiKey(0x0041), TPtiKey(0x0041), {0x00e2, 0x00c2}},
	{TPtiKey(0x0045), TPtiKey(0x0045), {0x00ea, 0x00ca}},
	{TPtiKey(0x004F), TPtiKey(0x01AF), {0x01a1, 0x01a0}},
	{TPtiKey(0x004F), TPtiKey(0x004F), {0x00f4, 0x00d4}},
	{TPtiKey(0x0055), TPtiKey(0x01AF), {0x01b0, 0x01af}},
	{TPtiKey(0x0044), TPtiKey(0x0044), {0x0111, 0x0110}},
	{TPtiKey(0x01AF), TPtiKey(0x01AF), {0x0077, 0x0057}}
	};

const TInt KNumStandardQwertyVowelSeqEntries_Vietnamese = 8;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_Vietnamese[] =
    {
     0x0071,
     0x01B0,
     0x0065,
     0x0072,
     0x0074,
     0x0079,
     0x0075,
     0x0069,
     0x006F,
     0x0070,
     0x0061,
     0x0073,
     0x0064,
     0x0066,
     0x0067,
     0x0068,
     0x006A,
     0x006B,
     0x006C,
     0x007A,
     0x0078,
     0x0063,
     0x0076,
     0x0062,
     0x006E,
     0x006D,
     0x002C,
     0x002E,
     0x0040,
     0x0026,
     0x0027,
     0x003F,
     0x0051,
     0x01AF,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004F,
     0x0050,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004A,
     0x004B,
     0x004C,
     0x005A,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004E,
     0x004D,
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
     0x0025,
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

const TInt KMiniQwertyDataSize_Vietnamese = 93;


const TPtiKeyBinding MiniQwertyKeyBindings_table_Vietnamese[] =
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
        {EPtiKeyQwerty1, EPtiCaseFnLower, 60, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 61, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 62, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 63, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 64, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 65, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 66, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 67, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 68, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 69, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 70, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 71, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 72, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 73, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 74, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 75, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 86, 1},
    };

const TInt KMaxMiniQwertyIndex_Vietnamese = 83;


//
// Numeric key data table for MiniQwerty
//
const TPtiNumericKeyBinding MiniQwertyNumericKeysData_table_Vietnamese[] =
	{
	{'w',EPtiKeyQwertyD,EPtiCaseLower},
	};

const TInt KNumMiniQwertyNumericKeysEntries_Vietnamese = 1;

const TVowelSequence MiniQwertyVowelSeq_table_Vietnamese[] =
	{
	{TPtiKey(0x0041), TPtiKey(0x01AF), {0x0103, 0x0102}},
	{TPtiKey(0x0041), TPtiKey(0x0041), {0x00e2, 0x00c2}},
	{TPtiKey(0x0045), TPtiKey(0x0045), {0x00ea, 0x00ca}},
	{TPtiKey(0x004F), TPtiKey(0x01AF), {0x01a1, 0x01a0}},
	{TPtiKey(0x004F), TPtiKey(0x004F), {0x00f4, 0x00d4}},
	{TPtiKey(0x0055), TPtiKey(0x01AF), {0x01b0, 0x01af}},
	{TPtiKey(0x0044), TPtiKey(0x0044), {0x0111, 0x0110}},
	{TPtiKey(0x01AF), TPtiKey(0x01AF), {0x0077, 0x0057}}
	};

const TInt KNumMiniQwertyVowelSeqEntries_Vietnamese = 8;
//
// Class implementation.
//

CPtiLangDataImplVietnamese* CPtiLangDataImplVietnamese::NewL()
    {
    return new (ELeave) CPtiLangDataImplVietnamese();
    }


TInt CPtiLangDataImplVietnamese::LanguageCode() const
    {
    return ELangVietnamese;
    }


const TUint16* CPtiLangDataImplVietnamese::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Vietnamese;
        return MultitapData_table_Vietnamese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_Vietnamese;
        return StandardQwertyData_table_Vietnamese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_Vietnamese;
        return MiniQwertyData_table_Vietnamese;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplVietnamese::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Vietnamese;
        return MultitapKeyBindings_table_Vietnamese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_Vietnamese;
        return StandardQwertyKeyBindings_table_Vietnamese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_Vietnamese;
        return MiniQwertyKeyBindings_table_Vietnamese;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplVietnamese::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumEntries = KNumMiniQwertyNumericKeysEntries_Vietnamese;
        return MiniQwertyNumericKeysData_table_Vietnamese;
        }

    aNumEntries = 0;
    return NULL;
    }


const TVowelSequence* CPtiLangDataImplVietnamese::VowelSequenceTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyVowelSeqEntries_Vietnamese;
        return StandardQwertyVowelSeq_table_Vietnamese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumEntries = KNumMiniQwertyVowelSeqEntries_Vietnamese;
        return MiniQwertyVowelSeq_table_Vietnamese;
        }

    aNumEntries = 0;
    return NULL;
    }


