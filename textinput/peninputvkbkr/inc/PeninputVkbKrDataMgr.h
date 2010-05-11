/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninput VKB data manager
*
*/

#ifndef PENINPUTVKBKRDATAMGR_H
#define PENINPUTVKBKRDATAMGR_H

#include <e32std.h>
#include <e32base.h>
#include <peninputdatamgr.h>

class CPtiEngine;

class CPeninputVkbKrDataMgr : public CPeninputDataMgr
    {
public:
    
    static CPeninputVkbKrDataMgr* NewLC(
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    static CPeninputVkbKrDataMgr* NewL(
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    virtual ~CPeninputVkbKrDataMgr();

private:

    CPeninputVkbKrDataMgr(
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

private: // from CPeninputDataMgr
    
    CPtiEngine& iPtiEngine;

    };

#endif // PENINPUTVKBKRDATAMGR_H