/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#include <ECom/ImplementationProxy.h>

#include "PeninputVkbKrLayoutImp.h"

const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0xEB339F5D, CPeninputVkbKrLayoutImp::NewVkbL),
    IMPLEMENTATION_PROXY_ENTRY(0xEB339F5E, CPeninputVkbKrLayoutImp::NewFsqL)
    };

// ---------------------------------------------------------
// Exported proxy for instantiation method resolution
// ---------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

//  End Of File
