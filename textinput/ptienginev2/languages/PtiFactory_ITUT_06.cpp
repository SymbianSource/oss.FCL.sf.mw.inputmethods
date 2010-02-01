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

#include "PtiKeymappings_ITUT_06.h"

class CPtiVariantITUT_06DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_06DataFactory() {}
        static CPtiVariantITUT_06DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_06DataFactory* CPtiVariantITUT_06DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_06DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x1028189c, CPtiVariantITUT_06DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_06DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangSwedish)
        {
        return CPtiLangDataImplSwedish::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_06DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    aResult.AppendL(ELangSwedish);
    }

// End of file
