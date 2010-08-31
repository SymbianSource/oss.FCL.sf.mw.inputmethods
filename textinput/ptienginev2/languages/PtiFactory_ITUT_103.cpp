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

#include "PtiKeymappings_ITUT_103.h"

class CPtiVariantITUT_103DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_103DataFactory() {}
        static CPtiVariantITUT_103DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_103DataFactory* CPtiVariantITUT_103DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_103DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10282548, CPtiVariantITUT_103DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_103DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == KLangGalician)
        {
        return CPtiLangDataImplGalician::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_103DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    aResult.AppendL(KLangGalician);
    }

// End of file
