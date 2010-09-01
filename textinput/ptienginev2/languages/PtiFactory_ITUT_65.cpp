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












#include <ecom/implementationproxy.h>

#include <languages.hrh>

#include "PtiKeymappings_ITUT_65.h"

class CPtiVariantITUT_65DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_65DataFactory() {}
        static CPtiVariantITUT_65DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_65DataFactory* CPtiVariantITUT_65DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_65DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x2001FDFB, CPtiVariantITUT_65DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_65DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangKorean)
        {
        return CPtiLangDataImplKorean::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_65DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    CleanupClosePushL(aResult);
    aResult.AppendL(ELangKorean);
    CleanupStack::Pop();
    }

// End of file
