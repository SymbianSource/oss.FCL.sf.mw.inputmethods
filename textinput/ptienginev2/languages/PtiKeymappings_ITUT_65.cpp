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

#include "PtiKeymappings_ITUT_65.h"

//
// Multitap keymap data table
//
const TUint16 MultitapData_table_Korean[] =
    {
     0x3131,
     0x3134,
     0x314f,0x3153,
     0x3139,
     0x3141,
     0x3157,0x315C,
     0x3145,
     0x3147,
     0x3161,
     0x3157,0x315C,
     0x3131,
     0x3134,
     0x314f,0x3153,
     0x3139,
     0x3141,
     0x3157,0x315C,
     0x3145,
     0x3163,
     0x3161,
     0x3157,0x315C,
    };

const TInt KMultitapDataSize_Korean = 26;


const TPtiKeyBinding MultitapKeyBindings_table_Korean[] =
    {
        {EPtiKey1, EPtiCaseLower, 0, 1},
        {EPtiKey2, EPtiCaseLower, 1, 1},
        {EPtiKey3, EPtiCaseLower, 2, 2},
        {EPtiKey4, EPtiCaseLower, 4, 1},
        {EPtiKey5, EPtiCaseLower, 5, 1},
        {EPtiKey6, EPtiCaseLower, 6, 2},
        {EPtiKey7, EPtiCaseLower, 8, 1},
        {EPtiKey8, EPtiCaseLower, 9, 1},
        {EPtiKey9, EPtiCaseLower, 10, 1},
        {EPtiKeyStar, EPtiCaseLower, 11, 0},
        {EPtiKey0, EPtiCaseLower, 11, 2},
        {EPtiKeyHash, EPtiCaseLower, 13, 0},
        {EPtiKey1, EPtiCaseUpper, 13, 1},
        {EPtiKey2, EPtiCaseUpper, 14, 1},
        {EPtiKey3, EPtiCaseUpper, 15, 2},
        {EPtiKey4, EPtiCaseUpper, 17, 1},
        {EPtiKey5, EPtiCaseUpper, 18, 1},
        {EPtiKey6, EPtiCaseUpper, 19, 2},
        {EPtiKey7, EPtiCaseUpper, 21, 1},
        {EPtiKey8, EPtiCaseUpper, 22, 1},
        {EPtiKey9, EPtiCaseUpper, 23, 1},
        {EPtiKeyStar, EPtiCaseUpper, 24, 0},
        {EPtiKey0, EPtiCaseUpper, 24, 2},
        {EPtiKeyHash, EPtiCaseUpper, 26, 0},
    };

const TInt KMaxMultitapIndex_Korean = 24;

//
// Class implementation.
//

CPtiLangDataImplKorean* CPtiLangDataImplKorean::NewL()
    {
    return new (ELeave) CPtiLangDataImplKorean();
    }


TInt CPtiLangDataImplKorean::LanguageCode() const
    {
    return ELangKorean;
    }


const TUint16* CPtiLangDataImplKorean::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aDataSize = KMultitapDataSize_Korean;
        return MultitapData_table_Korean;
        }
    aDataSize = 0;
    return NULL;
    }


const TPtiKeyBinding* CPtiLangDataImplKorean::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const
    {
    if (aKeyboardType == EPtiKeyboard12Key)
        {
        aNumItems = KMaxMultitapIndex_Korean;
        return MultitapKeyBindings_table_Korean;
        }

    aNumItems = 0;
    return NULL;
    }
