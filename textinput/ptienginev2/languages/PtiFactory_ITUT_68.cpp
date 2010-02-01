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

#include "PtiKeymappings_ITUT_68.h"

class CPtiVariantITUT_68DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_68DataFactory() {}
        static CPtiVariantITUT_68DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_68DataFactory* CPtiVariantITUT_68DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_68DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10281B79, CPtiVariantITUT_68DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_68DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangLithuanian)
        {
        return CPtiLangDataImplLithuanian::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_68DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    aResult.AppendL(ELangLithuanian);
    }

// End of file
