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

#include "PtiKeymappings_ITUT_83.h"

class CPtiVariantITUT_83DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_83DataFactory() {}
        static CPtiVariantITUT_83DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_83DataFactory* CPtiVariantITUT_83DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_83DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x2002BC84, CPtiVariantITUT_83DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_83DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangSpanish)
        {
        return CPtiLangDataImplSpanish::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_83DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    CleanupClosePushL(aResult);
    aResult.AppendL(ELangSpanish);
    CleanupStack::Pop();
    }

// End of file
