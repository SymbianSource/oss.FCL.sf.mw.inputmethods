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

#include <PtiEngine.h>
#include "PeninputVkbKrDataMgr.h"

// ---------------------------------------------------------------------------
// Constructors & Destructors
// ---------------------------------------------------------------------------
//

CPeninputVkbKrDataMgr* CPeninputVkbKrDataMgr::NewLC(
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    {
    CPeninputVkbKrDataMgr* self = 
        new (ELeave) CPeninputVkbKrDataMgr(aContext,aPtiEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPeninputVkbKrDataMgr* CPeninputVkbKrDataMgr::NewL(
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    {
    CPeninputVkbKrDataMgr* self = 
        CPeninputVkbKrDataMgr::NewLC(aContext,aPtiEngine);
    CleanupStack::Pop(); // self;
    return self;
    }

CPeninputVkbKrDataMgr::CPeninputVkbKrDataMgr(
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    : CPeninputDataMgr(aContext),
      iPtiEngine(aPtiEngine)
    {
    }

CPeninputVkbKrDataMgr::~CPeninputVkbKrDataMgr()
    {
    }

// ---------------------------------------------------------------------------
// from CPeninputDataMgr
// ---------------------------------------------------------------------------
//

