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












#include <ecom/implementationproxy.h>

#include <languages.hrh>

#include "PtiKeymappings_ITUT_29.h"

class CPtiVariantITUT_29DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_29DataFactory() {}
        static CPtiVariantITUT_29DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_29DataFactory* CPtiVariantITUT_29DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_29DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10281B8C, CPtiVariantITUT_29DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_29DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangTaiwanChinese)
        {
        return CPtiLangDataImplTaiwanChinese::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_29DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    aResult.AppendL(ELangTaiwanChinese);
    }

// End of file
