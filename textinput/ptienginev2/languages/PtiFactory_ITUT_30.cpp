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

#include "PtiKeymappings_ITUT_30.h"

class CPtiVariantITUT_30DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_30DataFactory() {}
        static CPtiVariantITUT_30DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_30DataFactory* CPtiVariantITUT_30DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_30DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10281B8B, CPtiVariantITUT_30DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_30DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangHongKongChinese)
        {
        return CPtiLangDataImplHongKongChinese::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_30DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    CleanupClosePushL(aResult);
    aResult.AppendL(ELangHongKongChinese);
    CleanupStack::Pop();
    }

// End of file
