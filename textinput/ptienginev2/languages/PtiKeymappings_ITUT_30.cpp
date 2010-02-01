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

#include "PtiKeymappings_ITUT_30.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_HongKongChinese[] =
    {
     0x3002,0xFF0C,0xFF1F,0xFF01,0x3001,0xFF1A,0x0031,0x0040,0x002D,0x005F,0x0028,0x0029,0x300A,0x300B,0x002F,0x0025,0x00A5,
     0x0020,0x0030,0x21B2,
     0xFF0C,
     0x3002,
     0xFF1F,
     0x0020,0x0030,0x21B2,
    };

const TInt KMultitapDataSize_HongKongChinese = 26;


const TPtiKeyBinding MultitapKeyBindings_table_HongKongChinese[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 17},
        {EPtiKey2, EPtiCaseLower, 17, 0},
        {EPtiKey3, EPtiCaseLower, 17, 0},
        {EPtiKey4, EPtiCaseLower, 17, 0},
        {EPtiKey5, EPtiCaseLower, 17, 0},
        {EPtiKey6, EPtiCaseLower, 17, 0},
        {EPtiKey7, EPtiCaseLower, 17, 0},
        {EPtiKey8, EPtiCaseLower, 17, 0},
        {EPtiKey9, EPtiCaseLower, 17, 0},
        {EPtiKeyStar, EPtiCaseLower, 17, 0},
        {EPtiKey0, EPtiCaseLower, 17, 3},
        {EPtiKeyHash, EPtiCaseLower, 20, 0},
        {EPtiKey1, EPtiCaseUpper, 20, 0},
        {EPtiKey2, EPtiCaseUpper, 20, 0},
        {EPtiKey3, EPtiCaseUpper, 20, 0},
        {EPtiKey4, EPtiCaseUpper, 20, 0},
        {EPtiKey5, EPtiCaseUpper, 20, 0},
        {EPtiKey6, EPtiCaseUpper, 20, 0},
        {EPtiKey7, EPtiCaseUpper, 20, 1},
        {EPtiKey8, EPtiCaseUpper, 21, 1},
        {EPtiKey9, EPtiCaseUpper, 22, 1},
        {EPtiKeyStar, EPtiCaseUpper, 23, 0},
        {EPtiKey0, EPtiCaseUpper, 23, 3},
        {EPtiKeyHash, EPtiCaseUpper, 26, 0},
    };

const TInt KMaxMultitapIndex_HongKongChinese = 24;

//
// StandardQwerty keymap data table
//
const TUint16 StandardQwertyData_table_HongKongChinese[] =
    {
     0x0031,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030,
     0x2463,0x002b,0x2463,0x002b,0x2461,0x002b,0x2461,0x002b,
     0x2463,0x624b,0x2461,0x4e00,
     0x2463,0x7530,0x2461,0x4e28 ,
     0x2463,0x6c34,0x2461,0x4e3f,
     0x2463,0x53e3 ,
     0x2463,0x5eff ,
     0x2463,0x535c,
     0x2463,0x5c71  ,
     0x2463,0x6208,0x2461,0x4e00,
     0x2463,0x4eba,0x2461,0x4e28,
     0x2463,0x5fc3,0x2461,0x4e3f,
     0x2463,0xff0d,0x2463,0x002d,0x2461,0xff0d,0x2461,0x002d,
     0x2463,0x0023,0x2463,0x0023,0x2461,0x0023,0x2461,0x0023,
     0x2463,0x65e5,0x2461,0x4e36,
     0x2463,0x5c38,0x2461,0x4e5b ,
     0x2463,0x6728,0x2461,0x003f,
     0x2463,0x706b,
     0x2463,0x571f,
     0x2463,0x7af9,
     0x2463,0x5341,0x2461,0x003f ,
     0x2463,0x5927,0x2461,0x4e36  ,
     0x2463,0x4e2d,0x2461,0x4e5b,
     0x2463,0xff1a,0x2463,0x003a,0x2461,0xff1a,0x2461,0x003a,
     0x2463,0xff07,0x2463,0x0027,0x2461,0xff07,0x2461,0x0027 ,
     0x2463,0x003f,
     0x2463,0x96e3,
     0x2463,0x91d1,
     0x2463,0x5973,
     0x2463,0x6708,
     0x2463,0x5f13,
     0x2463,0x4e00,
     0x2463,0xff0c,0x2463,0x002c,0x2461,0xff0c,0x2461,0x002c,
     0x2463,0x3002,0x2463,0x002e,0x2461,0x3002,0x2461,0x002e,
     0x2463,0x002f,0x2463,0x002f,0x2461,0x002f,0x2461,0x002f,
     0xff01,
     0xff02,
     0x0024 ,
     0x20ac,
     0x0025,
     0x0026,
     0x002a,
     0xff08,
     0xff09,
     0x003d,
     0x0051,
     0x0057,
     0x0045,
     0x0052,
     0x0054,
     0x0059,
     0x0055,
     0x0049,
     0x004f,
     0x0050,
     0x3001,
     0x007e,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004a,
     0x004b,
     0x004c,
     0xff1b,
     0x0040,
     0x005a,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004e,
     0x004d,
     0x003c,
     0x003e,
     0xff1f,
    };

const TInt KStandardQwertyDataSize_HongKongChinese = 193;


const TPtiKeyBinding StandardQwertyKeyBindings_table_HongKongChinese[] =
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
        {EPtiKeyQwertyPlus, EPtiCaseLower, 10, 8},
        {EPtiKeyQwertyQ, EPtiCaseLower, 18, 4},
        {EPtiKeyQwertyW, EPtiCaseLower, 22, 4},
        {EPtiKeyQwertyE, EPtiCaseLower, 26, 4},
        {EPtiKeyQwertyR, EPtiCaseLower, 30, 2},
        {EPtiKeyQwertyT, EPtiCaseLower, 32, 2},
        {EPtiKeyQwertyY, EPtiCaseLower, 34, 2},
        {EPtiKeyQwertyU, EPtiCaseLower, 36, 2},
        {EPtiKeyQwertyI, EPtiCaseLower, 38, 4},
        {EPtiKeyQwertyO, EPtiCaseLower, 42, 4},
        {EPtiKeyQwertyP, EPtiCaseLower, 46, 4},
        {EPtiKeyQwertyMinus, EPtiCaseLower, 50, 8},
        {EPtiKeyQwertyHash, EPtiCaseLower, 58, 8},
        {EPtiKeyQwertyA, EPtiCaseLower, 66, 4},
        {EPtiKeyQwertyS, EPtiCaseLower, 70, 4},
        {EPtiKeyQwertyD, EPtiCaseLower, 74, 4},
        {EPtiKeyQwertyF, EPtiCaseLower, 78, 2},
        {EPtiKeyQwertyG, EPtiCaseLower, 80, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 82, 2},
        {EPtiKeyQwertyJ, EPtiCaseLower, 84, 4},
        {EPtiKeyQwertyK, EPtiCaseLower, 88, 4},
        {EPtiKeyQwertyL, EPtiCaseLower, 92, 4},
        {EPtiKeyQwertySemicolon, EPtiCaseLower, 96, 8},
        {EPtiKeyQwertyApostrophe, EPtiCaseLower, 104, 8},
        {EPtiKeyQwertyZ, EPtiCaseLower, 112, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 114, 2},
        {EPtiKeyQwertyC, EPtiCaseLower, 116, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 118, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 120, 2},
        {EPtiKeyQwertyN, EPtiCaseLower, 122, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 124, 2},
        {EPtiKeyQwertyComma, EPtiCaseLower, 126, 8},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 134, 8},
        {EPtiKeyQwertySlash, EPtiCaseLower, 142, 8},
        {EPtiKeyQwerty1, EPtiCaseUpper, 150, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 151, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 152, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 153, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 154, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 155, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 156, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 157, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 158, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 159, 0},
        {EPtiKeyQwertyPlus, EPtiCaseUpper, 159, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 160, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 161, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 162, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 163, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 164, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 165, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 166, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 167, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 168, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 169, 1},
        {EPtiKeyQwertyMinus, EPtiCaseUpper, 170, 1},
        {EPtiKeyQwertyHash, EPtiCaseUpper, 171, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 172, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 173, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 174, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 175, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 176, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 177, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 178, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 179, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 180, 1},
        {EPtiKeyQwertySemicolon, EPtiCaseUpper, 181, 1},
        {EPtiKeyQwertyApostrophe, EPtiCaseUpper, 182, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 183, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 184, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 185, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 186, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 187, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 188, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 189, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 190, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 191, 1},
        {EPtiKeyQwertySlash, EPtiCaseUpper, 192, 1},
    };

const TInt KMaxStandardQwertyIndex_HongKongChinese = 88;


//
// Numeric key data table for StandardQwerty
//
const TPtiNumericKeyBinding StandardQwertyNumericKeysData_table_HongKongChinese[] =
	{
	{'*',EPtiKeyQwerty6,EPtiCaseUpper},
	{'p',EPtiKeyQwertyP,EPtiCaseLower},
	{'w',EPtiKeyQwertyW,EPtiCaseLower},
	{'E',EPtiKeyQwertyE,EPtiCaseUpper},
	{'?',EPtiKeyQwertySlash,EPtiCaseUpper},
	};

const TInt KNumStandardQwertyNumericKeysEntries_HongKongChinese = 5;
//
// MiniQwerty keymap data table
//
const TUint16 MiniQwertyData_table_HongKongChinese[] =
    {
     0x2463,0x624b,
     0x2463,0x7530,
     0x2463,0x6c34,0x2461,0x4e00,
     0x2463,0x53e3,0x2461,0x4e00,
     0x2463,0x5eff,0x2461,0x4e28,
     0x2463,0x535c,0x2461,0x4e28,
     0x2463,0x5c71,0x2461,0x4e3f,
     0x2463,0x6208,0x2461,0x4e3f,
     0x2463,0x4eba,
     0x2463,0x5fc3,
     0x2463,0x65e5,
     0x2463,0x5c38,
     0x2463,0x6728,0x2461,0x4e36,
     0x2463,0x706b,0x2461,0x4e36,
     0x2463,0x571f,0x2461,0x4e5b,
     0x2463,0x7af9,0x2461,0x4e5b,
     0x2463,0x5341,0x2461,0x003f,
     0x2463,0x5927,0x2461,0x003f,
     0x2463,0x4e2d,
     0x2463,0x003f,
     0x2463,0x96e3,
     0x2463,0x91d1,
     0x2463,0x5973,
     0x2463,0x6708,
     0x2463,0x5f13,
     0x2463,0x4e00,
     0x2463,0xFF0C,0x2463,0x002c,0x2461,0xFF0C,0x2461,0x002c,
     0x2463,0x3002,0x2463,0x002e,0x2461,0x3002,0x2461,0x002e,
     0x2463,0x002F,0x2463,0x002F,0x2461,0x002F,0x2461,0x002F,
     0x2463,0x0040,0x2463,0x0040,0x2461,0x0040,0x2461,0x0040,
     0x2461,0x2022,
     0x2463,0x003F,0x2463,0x003F,0x2461,0x003f,0x2461,0x003f,
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
     0x2463,0xFF1B,0x2463,0x003b,0x2461,0xff1b,0x2461,0x003b,
     0x2463,0xFF1A,0x2463,0x003a,0x2461,0xff1a,0x2461,0x003a,
     0x2463,0xFF07,0x2463,0x0027,0x2461,0xFF07,0x2461,0x0027,
     0x2463,0xff02,0x2463,0x0022,0x2461,0xff02,0x2461,0x0022,
     0x2463,0x005e,0x2463,0x005E,0x2461,0x005e,0x2461,0x005e,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x002A,
     0x002A,
     0x0030,
     0x0023,
     0x0031,
     0x0031,
     0x0032,
     0x0032,
     0x0033,
     0x0033,
     0x0034,
     0x0034,
     0x0035,
     0x0035,
     0x0036,
     0x0036,
     0x0037,
     0x0037,
     0x0038,
     0x0038,
     0x0039,
     0x0039,
     0x002A,
     0x002A,
     0x0030,
     0x0023,
    };

const TInt KMiniQwertyDataSize_HongKongChinese = 228;


const TPtiKeyBinding MiniQwertyKeyBindings_table_HongKongChinese[] =
    {
        {EPtiKeyQwerty1, EPtiCaseLower, 0, 2},
        {EPtiKeyQwerty2, EPtiCaseLower, 2, 2},
        {EPtiKeyQwerty3, EPtiCaseLower, 4, 4},
        {EPtiKeyQwerty4, EPtiCaseLower, 8, 4},
        {EPtiKeyQwerty5, EPtiCaseLower, 12, 4},
        {EPtiKeyQwerty6, EPtiCaseLower, 16, 4},
        {EPtiKeyQwerty7, EPtiCaseLower, 20, 4},
        {EPtiKeyQwerty8, EPtiCaseLower, 24, 4},
        {EPtiKeyQwerty9, EPtiCaseLower, 28, 2},
        {EPtiKeyQwerty0, EPtiCaseLower, 30, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 32, 2},
        {EPtiKeyQwertyS, EPtiCaseLower, 34, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 36, 4},
        {EPtiKeyQwertyF, EPtiCaseLower, 40, 4},
        {EPtiKeyQwertyG, EPtiCaseLower, 44, 4},
        {EPtiKeyQwertyH, EPtiCaseLower, 48, 4},
        {EPtiKeyQwertyJ, EPtiCaseLower, 52, 4},
        {EPtiKeyQwertyK, EPtiCaseLower, 56, 4},
        {EPtiKeyQwertyL, EPtiCaseLower, 60, 2},
        {EPtiKeyQwertyZ, EPtiCaseLower, 62, 2},
        {EPtiKeyQwertyX, EPtiCaseLower, 64, 2},
        {EPtiKeyQwertyC, EPtiCaseLower, 66, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 68, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 70, 2},
        {EPtiKeyQwertyN, EPtiCaseLower, 72, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 74, 2},
        {EPtiKeyQwertyComma, EPtiCaseLower, 76, 8},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 84, 8},
        {EPtiKeyQwerty1, EPtiCaseUpper, 118, 1},
        {EPtiKeyQwerty2, EPtiCaseUpper, 119, 1},
        {EPtiKeyQwerty3, EPtiCaseUpper, 120, 1},
        {EPtiKeyQwerty4, EPtiCaseUpper, 121, 1},
        {EPtiKeyQwerty5, EPtiCaseUpper, 122, 1},
        {EPtiKeyQwerty6, EPtiCaseUpper, 123, 1},
        {EPtiKeyQwerty7, EPtiCaseUpper, 124, 1},
        {EPtiKeyQwerty8, EPtiCaseUpper, 125, 1},
        {EPtiKeyQwerty9, EPtiCaseUpper, 126, 1},
        {EPtiKeyQwerty0, EPtiCaseUpper, 127, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 128, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 129, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 130, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 131, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 132, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 133, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 134, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 135, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 136, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 137, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 138, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 139, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 140, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 141, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 142, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 143, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 144, 8},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 152, 8},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 184, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 185, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 186, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 187, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 188, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 189, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 190, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 191, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 192, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 193, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 194, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 195, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 196, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 197, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 198, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 199, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 200, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 201, 1},
        {EPtiKeyQwerty3, EPtiCaseFnUpper, 206, 1},
        {EPtiKeyQwerty4, EPtiCaseFnUpper, 207, 1},
        {EPtiKeyQwerty5, EPtiCaseFnUpper, 208, 1},
        {EPtiKeyQwerty6, EPtiCaseFnUpper, 209, 1},
        {EPtiKeyQwerty7, EPtiCaseFnUpper, 210, 1},
        {EPtiKeyQwerty8, EPtiCaseFnUpper, 211, 1},
        {EPtiKeyQwertyD, EPtiCaseFnUpper, 212, 1},
        {EPtiKeyQwertyF, EPtiCaseFnUpper, 213, 1},
        {EPtiKeyQwertyG, EPtiCaseFnUpper, 214, 1},
        {EPtiKeyQwertyH, EPtiCaseFnUpper, 215, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnUpper, 216, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 217, 1},
        {EPtiKeyQwertyC, EPtiCaseFnUpper, 218, 1},
        {EPtiKeyQwertyV, EPtiCaseFnUpper, 219, 1},
        {EPtiKeyQwertyB, EPtiCaseFnUpper, 220, 1},
        {EPtiKeyQwertyN, EPtiCaseFnUpper, 221, 1},
        {EPtiKeyQwertyM, EPtiCaseFnUpper, 222, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnUpper, 223, 1},
    };

const TInt KMaxMiniQwertyIndex_HongKongChinese = 92;

//
// MiniQwerty3x11 keymap data table
//
const TUint16 MiniQwerty3x11Data_table_HongKongChinese[] =
    {
     0x2463,0x624b,0x2461,0x4e00,
     0x2463,0x7530,0x2461,0x4e28,
     0x2463,0x6c34,0x2461,0x4e3f,
     0x2463,0x53e3,0x2461,0x4e36,
     0x2463,0x5eff,0x2461,0x4e5b,
     0x2463,0x535c,0x2461,0x003f,
     0x2463,0x5c71,
     0x2463,0x6208,
     0x2463,0x4eba,
     0x2463,0x5fc3,
     0x2463,0x65e5,
     0x2463,0x5c38,
     0x2463,0x6728,
     0x2463,0x706b,
     0x2463,0x571f,
     0x2463,0x7af9,
     0x2463,0x5341,
     0x2463,0x5927,
     0x2463,0x4e2d,
     0x2463,0xFF0C,0x2463,0x002c,0x2461,0xFF0C,0x2461,0x002c,
     0x2463,0x96e3,
     0x2463,0x91d1,
     0x2463,0x5973,
     0x2463,0x6708,
     0x2463,0x5f13,
     0x2463,0x4e00,
     0x2461,0x2022,
     0x0051 ,
     0x0057,
     0x0045,
     0x0052,
     0x0054 ,
     0x0059,
     0x0055,
     0x0049,
     0x004f,
     0x0050,
     0x0041,
     0x0053,
     0x0044,
     0x0046,
     0x0047,
     0x0048,
     0x004a,
     0x004b,
     0x004c,
     0x2463,0x3002,0x2463,0x002e,0x2461,0x3002,0x2461,0x002e,
     0x005a,
     0x0058,
     0x0043,
     0x0056,
     0x0042,
     0x004e,
     0x004d,
     0x0031,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030 ,
     0x0040,
     0x002f,
     0x002b,
     0x002d,
     0x003d,
     0x0023,
     0x002a,
     0x003b,
     0x003a,
     0x0027 ,
     0x0028,
     0x0029,
     0x0025,
     0x0026,
     0x0022,
     0x003f,
     0x0021,
     0x0031,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030  ,
     0x0040,
     0x002f,
     0x002b,
     0x002d,
     0x003d,
     0x0023,
     0x002a,
     0x003b,
     0x003a,
     0x0027  ,
     0x0028,
     0x0029,
     0x0025,
     0x0026,
     0x0022,
     0x003f,
     0x0021,
    };

const TInt KMiniQwerty3x11DataSize_HongKongChinese = 160;


const TPtiKeyBinding MiniQwerty3x11KeyBindings_table_HongKongChinese[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 4},
        {EPtiKeyQwertyW, EPtiCaseLower, 4, 4},
        {EPtiKeyQwertyE, EPtiCaseLower, 8, 4},
        {EPtiKeyQwertyR, EPtiCaseLower, 12, 4},
        {EPtiKeyQwertyT, EPtiCaseLower, 16, 4},
        {EPtiKeyQwertyY, EPtiCaseLower, 20, 4},
        {EPtiKeyQwertyU, EPtiCaseLower, 24, 2},
        {EPtiKeyQwertyI, EPtiCaseLower, 26, 2},
        {EPtiKeyQwertyO, EPtiCaseLower, 28, 2},
        {EPtiKeyQwertyP, EPtiCaseLower, 30, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 32, 2},
        {EPtiKeyQwertyS, EPtiCaseLower, 34, 2},
        {EPtiKeyQwertyD, EPtiCaseLower, 36, 2},
        {EPtiKeyQwertyF, EPtiCaseLower, 38, 2},
        {EPtiKeyQwertyG, EPtiCaseLower, 40, 2},
        {EPtiKeyQwertyH, EPtiCaseLower, 42, 2},
        {EPtiKeyQwertyJ, EPtiCaseLower, 44, 2},
        {EPtiKeyQwertyK, EPtiCaseLower, 46, 2},
        {EPtiKeyQwertyL, EPtiCaseLower, 48, 2},
        {EPtiKeyQwertyComma, EPtiCaseLower, 50, 8},
        {EPtiKeyQwertyZ, EPtiCaseLower, 58, 0},
        {EPtiKeyQwertyX, EPtiCaseLower, 58, 2},
        {EPtiKeyQwertyC, EPtiCaseLower, 60, 2},
        {EPtiKeyQwertyV, EPtiCaseLower, 62, 2},
        {EPtiKeyQwertyB, EPtiCaseLower, 64, 2},
        {EPtiKeyQwertyN, EPtiCaseLower, 66, 2},
        {EPtiKeyQwertyM, EPtiCaseLower, 68, 2},
        {EStdKeySpace, EPtiCaseLower, 70, 2},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 72, 1},
        {EPtiKeyQwertyW, EPtiCaseUpper, 73, 1},
        {EPtiKeyQwertyE, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwertyR, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwertyT, EPtiCaseUpper, 76, 1},
        {EPtiKeyQwertyY, EPtiCaseUpper, 77, 1},
        {EPtiKeyQwertyU, EPtiCaseUpper, 78, 1},
        {EPtiKeyQwertyI, EPtiCaseUpper, 79, 1},
        {EPtiKeyQwertyO, EPtiCaseUpper, 80, 1},
        {EPtiKeyQwertyP, EPtiCaseUpper, 81, 1},
        {EPtiKeyQwertyA, EPtiCaseUpper, 82, 1},
        {EPtiKeyQwertyS, EPtiCaseUpper, 83, 1},
        {EPtiKeyQwertyD, EPtiCaseUpper, 84, 1},
        {EPtiKeyQwertyF, EPtiCaseUpper, 85, 1},
        {EPtiKeyQwertyG, EPtiCaseUpper, 86, 1},
        {EPtiKeyQwertyH, EPtiCaseUpper, 87, 1},
        {EPtiKeyQwertyJ, EPtiCaseUpper, 88, 1},
        {EPtiKeyQwertyK, EPtiCaseUpper, 89, 1},
        {EPtiKeyQwertyL, EPtiCaseUpper, 90, 1},
        {EPtiKeyQwertyComma, EPtiCaseUpper, 91, 8},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 99, 1},
        {EPtiKeyQwertyX, EPtiCaseUpper, 100, 1},
        {EPtiKeyQwertyC, EPtiCaseUpper, 101, 1},
        {EPtiKeyQwertyV, EPtiCaseUpper, 102, 1},
        {EPtiKeyQwertyB, EPtiCaseUpper, 103, 1},
        {EPtiKeyQwertyN, EPtiCaseUpper, 104, 1},
        {EPtiKeyQwertyM, EPtiCaseUpper, 105, 1},
        {EStdKeySpace, EPtiCaseUpper, 106, 0},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 106, 1},
        {EPtiKeyQwertyW, EPtiCaseFnLower, 107, 1},
        {EPtiKeyQwertyE, EPtiCaseFnLower, 108, 1},
        {EPtiKeyQwertyR, EPtiCaseFnLower, 109, 1},
        {EPtiKeyQwertyT, EPtiCaseFnLower, 110, 1},
        {EPtiKeyQwertyY, EPtiCaseFnLower, 111, 1},
        {EPtiKeyQwertyU, EPtiCaseFnLower, 112, 1},
        {EPtiKeyQwertyI, EPtiCaseFnLower, 113, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 114, 1},
        {EPtiKeyQwertyP, EPtiCaseFnLower, 115, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 116, 1},
        {EPtiKeyQwertyS, EPtiCaseFnLower, 117, 1},
        {EPtiKeyQwertyD, EPtiCaseFnLower, 118, 1},
        {EPtiKeyQwertyF, EPtiCaseFnLower, 119, 1},
        {EPtiKeyQwertyG, EPtiCaseFnLower, 120, 1},
        {EPtiKeyQwertyH, EPtiCaseFnLower, 121, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnLower, 122, 1},
        {EPtiKeyQwertyK, EPtiCaseFnLower, 123, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 124, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnLower, 125, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 126, 1},
        {EPtiKeyQwertyX, EPtiCaseFnLower, 127, 1},
        {EPtiKeyQwertyC, EPtiCaseFnLower, 128, 1},
        {EPtiKeyQwertyV, EPtiCaseFnLower, 129, 1},
        {EPtiKeyQwertyB, EPtiCaseFnLower, 130, 1},
        {EPtiKeyQwertyN, EPtiCaseFnLower, 131, 1},
        {EPtiKeyQwertyM, EPtiCaseFnLower, 132, 1},
        {EPtiKeyQwertyQ, EPtiCaseFnUpper, 133, 1},
        {EPtiKeyQwertyW, EPtiCaseFnUpper, 134, 1},
        {EPtiKeyQwertyE, EPtiCaseFnUpper, 135, 1},
        {EPtiKeyQwertyR, EPtiCaseFnUpper, 136, 1},
        {EPtiKeyQwertyT, EPtiCaseFnUpper, 137, 1},
        {EPtiKeyQwertyY, EPtiCaseFnUpper, 138, 1},
        {EPtiKeyQwertyU, EPtiCaseFnUpper, 139, 1},
        {EPtiKeyQwertyI, EPtiCaseFnUpper, 140, 1},
        {EPtiKeyQwertyO, EPtiCaseFnUpper, 141, 1},
        {EPtiKeyQwertyP, EPtiCaseFnUpper, 142, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 143, 1},
        {EPtiKeyQwertyS, EPtiCaseFnUpper, 144, 1},
        {EPtiKeyQwertyD, EPtiCaseFnUpper, 145, 1},
        {EPtiKeyQwertyF, EPtiCaseFnUpper, 146, 1},
        {EPtiKeyQwertyG, EPtiCaseFnUpper, 147, 1},
        {EPtiKeyQwertyH, EPtiCaseFnUpper, 148, 1},
        {EPtiKeyQwertyJ, EPtiCaseFnUpper, 149, 1},
        {EPtiKeyQwertyK, EPtiCaseFnUpper, 150, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 151, 1},
        {EPtiKeyQwertyComma, EPtiCaseFnUpper, 152, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 153, 1},
        {EPtiKeyQwertyX, EPtiCaseFnUpper, 154, 1},
        {EPtiKeyQwertyC, EPtiCaseFnUpper, 155, 1},
        {EPtiKeyQwertyV, EPtiCaseFnUpper, 156, 1},
        {EPtiKeyQwertyB, EPtiCaseFnUpper, 157, 1},
        {EPtiKeyQwertyN, EPtiCaseFnUpper, 158, 1},
        {EPtiKeyQwertyM, EPtiCaseFnUpper, 159, 1},
    };

const TInt KMaxMiniQwerty3x11Index_HongKongChinese = 110;

//
// HalfQwerty keymap data table
//
const TUint16 HalfQwertyData_table_HongKongChinese[] =
    {
     0x0071,0x0077,
     0x0065,0x0072,0x2461,0x4e00,
     0x0074,0x0079,0x2461,0x4e28,
     0x0075,0x0069,0x2461,0x4e3f,
     0x006f,0x0070,
     0x0061,0x0073,
     0x0064,0x0066,0x2461,0x4e36,
     0x0067,0x0068,0x2461,0x4e5b,
     0x006a,0x006b,0x2461,0x003f,
     0x006c,
     0x007a,0x0078,
     0x0063,0x0076,
     0x0062,0x006e,
     0x006d,
     0x3002,0xFF0C,0xFF1A,0xFF1B,
     0x0020,
     0x005e,
     0x0051,0x0057,
     0x0045,0x0052,
     0x0054,0x0059,
     0x0055,0x0049,
     0x004f,0x0050,
     0x0041,0x0053,
     0x0044,0x0046,
     0x0047,0x0048,
     0x004a,0x004b,
     0x004c,
     0x005a,0x0058,
     0x0043,0x0056,
     0x0042,0x004e,
     0x004d,
     0x3002,0xFF0C,0xFF1A,0xFF1B,
     0x0020,
     0x005e,
     0xFF01,
     0x0031,
     0x0032,
     0x0033,
     0x002d,
     0xFF1F,
     0x0034,
     0x0035,
     0x0036,
     0x0027,
     0x0040,
     0x0037,
     0x0038,
     0x0039,
     0xFF1A, 0xFF1B, 0x3002, 0xFF0C,
     0x002a, 0x002b,
     0x0030,
     0x0023, 0x002f,
     0xFF01,
     0x0031,
     0x0032,
     0x0033,
     0x002d,
     0xFF1F,
     0x0034,
     0x0035,
     0x0036,
     0x0027,
     0x0040,
     0x0037,
     0x0038,
     0x0039,
     0xFF1A, 0xFF1B, 0x3002, 0xFF0C,
     0x002a, 0x002b,
     0x0030,
     0x0023, 0x002f,
    };

const TInt KHalfQwertyDataSize_HongKongChinese = 122;


const TPtiKeyBinding HalfQwertyKeyBindings_table_HongKongChinese[] =
    {
        {EPtiKeyQwertyQ, EPtiCaseLower, 0, 2},
        {EPtiKeyQwerty1, EPtiCaseLower, 2, 4},
        {EPtiKeyQwerty2, EPtiCaseLower, 6, 4},
        {EPtiKeyQwerty3, EPtiCaseLower, 10, 4},
        {EPtiKeyQwertyO, EPtiCaseLower, 14, 2},
        {EPtiKeyQwertyA, EPtiCaseLower, 16, 2},
        {EPtiKeyQwerty4, EPtiCaseLower, 18, 4},
        {EPtiKeyQwerty5, EPtiCaseLower, 22, 4},
        {EPtiKeyQwerty6, EPtiCaseLower, 26, 4},
        {EPtiKeyQwertyL, EPtiCaseLower, 30, 1},
        {EPtiKeyQwertyZ, EPtiCaseLower, 31, 2},
        {EPtiKeyQwerty7, EPtiCaseLower, 33, 2},
        {EPtiKeyQwerty8, EPtiCaseLower, 35, 2},
        {EPtiKeyQwerty9, EPtiCaseLower, 37, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseLower, 38, 4},
        {EPtiKeyQwertyChr, EPtiCaseLower, 42, 0},
        {EPtiKeyQwertySpace, EPtiCaseLower, 42, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseLower, 43, 1},
        {EPtiKeyQwertyQ, EPtiCaseUpper, 44, 2},
        {EPtiKeyQwerty1, EPtiCaseUpper, 46, 2},
        {EPtiKeyQwerty2, EPtiCaseUpper, 48, 2},
        {EPtiKeyQwerty3, EPtiCaseUpper, 50, 2},
        {EPtiKeyQwertyO, EPtiCaseUpper, 52, 2},
        {EPtiKeyQwertyA, EPtiCaseUpper, 54, 2},
        {EPtiKeyQwerty4, EPtiCaseUpper, 56, 2},
        {EPtiKeyQwerty5, EPtiCaseUpper, 58, 2},
        {EPtiKeyQwerty6, EPtiCaseUpper, 60, 2},
        {EPtiKeyQwertyL, EPtiCaseUpper, 62, 1},
        {EPtiKeyQwertyZ, EPtiCaseUpper, 63, 2},
        {EPtiKeyQwerty7, EPtiCaseUpper, 65, 2},
        {EPtiKeyQwerty8, EPtiCaseUpper, 67, 2},
        {EPtiKeyQwerty9, EPtiCaseUpper, 69, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseUpper, 70, 4},
        {EPtiKeyQwertyChr, EPtiCaseUpper, 74, 0},
        {EPtiKeyQwertySpace, EPtiCaseUpper, 74, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseUpper, 75, 1},
        {EPtiKeyQwertyQ, EPtiCaseFnLower, 76, 1},
        {EPtiKeyQwerty1, EPtiCaseFnLower, 77, 1},
        {EPtiKeyQwerty2, EPtiCaseFnLower, 78, 1},
        {EPtiKeyQwerty3, EPtiCaseFnLower, 79, 1},
        {EPtiKeyQwertyO, EPtiCaseFnLower, 80, 1},
        {EPtiKeyQwertyA, EPtiCaseFnLower, 81, 1},
        {EPtiKeyQwerty4, EPtiCaseFnLower, 82, 1},
        {EPtiKeyQwerty5, EPtiCaseFnLower, 83, 1},
        {EPtiKeyQwerty6, EPtiCaseFnLower, 84, 1},
        {EPtiKeyQwertyL, EPtiCaseFnLower, 85, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnLower, 86, 1},
        {EPtiKeyQwerty7, EPtiCaseFnLower, 87, 1},
        {EPtiKeyQwerty8, EPtiCaseFnLower, 88, 1},
        {EPtiKeyQwerty9, EPtiCaseFnLower, 89, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnLower, 90, 4},
        {EPtiKeyQwertyChr, EPtiCaseFnLower, 94, 2},
        {EPtiKeyQwertySpace, EPtiCaseFnLower, 96, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseFnLower, 97, 2},
        {EPtiKeyQwertyQ, EPtiCaseFnUpper, 99, 1},
        {EPtiKeyQwerty1, EPtiCaseFnUpper, 100, 1},
        {EPtiKeyQwerty2, EPtiCaseFnUpper, 101, 1},
        {EPtiKeyQwerty3, EPtiCaseFnUpper, 102, 1},
        {EPtiKeyQwertyO, EPtiCaseFnUpper, 103, 1},
        {EPtiKeyQwertyA, EPtiCaseFnUpper, 104, 1},
        {EPtiKeyQwerty4, EPtiCaseFnUpper, 105, 1},
        {EPtiKeyQwerty5, EPtiCaseFnUpper, 106, 1},
        {EPtiKeyQwerty6, EPtiCaseFnUpper, 107, 1},
        {EPtiKeyQwertyL, EPtiCaseFnUpper, 108, 1},
        {EPtiKeyQwertyZ, EPtiCaseFnUpper, 109, 1},
        {EPtiKeyQwerty7, EPtiCaseFnUpper, 110, 1},
        {EPtiKeyQwerty8, EPtiCaseFnUpper, 111, 1},
        {EPtiKeyQwerty9, EPtiCaseFnUpper, 112, 1},
        {EPtiKeyQwertyFullstop, EPtiCaseFnUpper, 113, 4},
        {EPtiKeyQwertyChr, EPtiCaseFnUpper, 117, 2},
        {EPtiKeyQwertySpace, EPtiCaseFnUpper, 119, 1},
        {EPtiKeyQwertyLeftShift, EPtiCaseFnUpper, 120, 2},
    };

const TInt KMaxHalfQwertyIndex_HongKongChinese = 72;

//
// CustomQwerty keymap data table
//
const TUint16 CustomQwertyData_table_HongKongChinese[] =
    {
     0x0061,
     0x0062 ,
     0x0063,
     0x0064,
     0x0065,
     0x0066,
     0x0067,
     0x0068,
     0x0069,
     0x006A,
     0x006B,
     0x006C,
     0x0031 ,
     0x0032,
     0x0033,
     0x0034,
     0x0035,
     0x0036,
     0x0037,
     0x0038,
     0x0039,
     0x0030,
     0x003f,
     0x003e,
     0x0041,
     0x0042,
     0x0043,
     0x0044,
     0x0045,
     0x0046,
     0x0047,
     0x0048,
     0x0049,
     0x004A,
     0x004B,
     0x004C,
     0x0021 ,
     0x0022,
     0x0023,
     0x0024,
     0x0025,
     0x0026,
     0x0027,
     0x0028,
     0x0029,
     0x002A,
     0x002B,
     0x002C,
    };

const TInt KCustomQwertyDataSize_HongKongChinese = 48;


const TPtiKeyBinding CustomQwertyKeyBindings_table_HongKongChinese[] =
    {
        { EPtiKeyQwerty1, EPtiCaseLower, 0, 1},
        { EPtiKeyQwerty2, EPtiCaseLower, 1, 1},
        { EPtiKeyQwerty3, EPtiCaseLower, 2, 1},
        { EPtiKeyQwerty4, EPtiCaseLower, 3, 1},
        { EPtiKeyQwerty5, EPtiCaseLower, 4, 1},
        { EPtiKeyQwerty6, EPtiCaseLower, 5, 1},
        { EPtiKeyQwerty7, EPtiCaseLower, 6, 1},
        { EPtiKeyQwerty8, EPtiCaseLower, 7, 1},
        { EPtiKeyQwerty9, EPtiCaseLower, 8, 1},
        { EPtiKeyQwerty0, EPtiCaseLower, 9, 1},
        { EPtiKeyQwerty1, EPtiCaseUpper, 24, 1},
        { EPtiKeyQwerty2, EPtiCaseUpper, 25, 1},
        { EPtiKeyQwerty3, EPtiCaseUpper, 26, 1},
        { EPtiKeyQwerty4, EPtiCaseUpper, 27, 1},
        { EPtiKeyQwerty5, EPtiCaseUpper, 28, 1},
        { EPtiKeyQwerty6, EPtiCaseUpper, 29, 1},
        { EPtiKeyQwerty7, EPtiCaseUpper, 30, 1},
        { EPtiKeyQwerty8, EPtiCaseUpper, 31, 1},
        { EPtiKeyQwerty9, EPtiCaseUpper, 32, 1},
        { EPtiKeyQwerty0, EPtiCaseUpper, 33, 1},
    };

const TInt KMaxCustomQwertyIndex_HongKongChinese = 20;

//
// Class implementation.
//

CPtiLangDataImplHongKongChinese* CPtiLangDataImplHongKongChinese::NewL()
    {
    return new (ELeave) CPtiLangDataImplHongKongChinese();
    }


TInt CPtiLangDataImplHongKongChinese::LanguageCode() const
    {
    return ELangHongKongChinese;
    }


const TUint16* CPtiLangDataImplHongKongChinese::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_HongKongChinese;
        return MultitapData_table_HongKongChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aDataSize = KStandardQwertyDataSize_HongKongChinese;
        return StandardQwertyData_table_HongKongChinese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aDataSize = KMiniQwertyDataSize_HongKongChinese;
        return MiniQwertyData_table_HongKongChinese;
        }

    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aDataSize = KMiniQwerty3x11DataSize_HongKongChinese;
        return MiniQwerty3x11Data_table_HongKongChinese;
        }

    if (aKeyboardType == EPtiKeyboardHalfQwerty)
        {
        aDataSize = KHalfQwertyDataSize_HongKongChinese;
        return HalfQwertyData_table_HongKongChinese;
        }

    if (aKeyboardType == EPtiKeyboardCustomQwerty)
        {
        aDataSize = KCustomQwertyDataSize_HongKongChinese;
        return CustomQwertyData_table_HongKongChinese;
        }

    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplHongKongChinese::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_HongKongChinese;
        return MultitapKeyBindings_table_HongKongChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumItems = KMaxStandardQwertyIndex_HongKongChinese;
        return StandardQwertyKeyBindings_table_HongKongChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty4x10)
        {
        aNumItems = KMaxMiniQwertyIndex_HongKongChinese;
        return MiniQwertyKeyBindings_table_HongKongChinese;
        }
    if (aKeyboardType == EPtiKeyboardQwerty3x11)
        {
        aNumItems = KMaxMiniQwerty3x11Index_HongKongChinese;
        return MiniQwerty3x11KeyBindings_table_HongKongChinese;
        }
    if (aKeyboardType == EPtiKeyboardHalfQwerty)
        {
        aNumItems = KMaxHalfQwertyIndex_HongKongChinese;
        return HalfQwertyKeyBindings_table_HongKongChinese;
        }
    if (aKeyboardType == EPtiKeyboardCustomQwerty)
        {
        aNumItems = KMaxCustomQwertyIndex_HongKongChinese;
        return CustomQwertyKeyBindings_table_HongKongChinese;
        }

    aNumItems = 0;
    return NULL;
    }


const TPtiNumericKeyBinding* CPtiLangDataImplHongKongChinese::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const
    {
    if (aKeyboardType == EPtiKeyboardQwerty4x12)
        {
        aNumEntries = KNumStandardQwertyNumericKeysEntries_HongKongChinese;
        return StandardQwertyNumericKeysData_table_HongKongChinese;
        }

    aNumEntries = 0;
    return NULL;
    }



TBool CPtiLangDataImplHongKongChinese::SuitableForLatinOnlyMode() const
    {
    return EFalse;
    }


