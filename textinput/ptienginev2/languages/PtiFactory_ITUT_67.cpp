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

#include "PtiKeymappings_ITUT_67.h"

class CPtiVariantITUT_67DataFactory : public CPtiKeyMapDataFactory
    {
    public:
        CPtiVariantITUT_67DataFactory() {}
        static CPtiVariantITUT_67DataFactory* NewL();
        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);
        void ListLanguagesL(RArray<TInt>& aResult);
    };


CPtiVariantITUT_67DataFactory* CPtiVariantITUT_67DataFactory::NewL()
    {
    return new (ELeave) CPtiVariantITUT_67DataFactory();
    }


const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10281B77, CPtiVariantITUT_67DataFactory::NewL)
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


MPtiKeyMapData* CPtiVariantITUT_67DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)
    {
    if (aLanguageCode == ELangLatvian)
        {
        return CPtiLangDataImplLatvian::NewL();
        }

    return NULL;
    }


void CPtiVariantITUT_67DataFactory::ListLanguagesL(RArray<TInt>& aResult)
    {
    aResult.AppendL(ELangLatvian);
    }

// End of file
