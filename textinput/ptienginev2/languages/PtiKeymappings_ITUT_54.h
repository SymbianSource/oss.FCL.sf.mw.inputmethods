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












#ifndef __PTI_LANGUAGES_ITUT_54_H__
#define __PTI_LANGUAGES_ITUT_54_H__

#include <PtiKeyMapData.h>


//
// Class declaration for language Greek 
//
class CPtiLangDataImplGreek : public CPtiKeyMapData
    {
    public:
        static CPtiLangDataImplGreek* NewL();
        TInt LanguageCode() const;
        const TUint16* KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const;
        const TPtiKeyBinding* KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const;
        const TUint16* DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const;
        const TPtiNumericKeyBinding* NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const;
        TBool SuitableForLatinOnlyMode() const;
    };

#endif

// End of file. 
