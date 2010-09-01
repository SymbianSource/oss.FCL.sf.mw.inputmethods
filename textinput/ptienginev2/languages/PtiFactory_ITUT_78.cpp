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

#include "PtiKeymappings_ITUT_78.h"

class CPtiVariantITUT_78DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_78DataFactory() {}
        static CPtiVariantITUT_78DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_78DataFactory* CPtiVariantITUT_78DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_78DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10281B7D, CPtiVariantITUT_78DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_78DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangRomanian)
        {
        return CPtiLangDataImplRomanian::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_78DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    CleanupClosePushL(aResult);
    aResult.AppendL(ELangRomanian);
    CleanupStack::Pop();
    }

// End of file
