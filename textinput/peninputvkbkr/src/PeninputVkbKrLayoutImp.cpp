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

#include <AknFepGlobalEnums.h>

#include "PeninputVkbKrLayoutImp.h"
#include "PeninputVkbKrLayout.h"

// ---------------------------------------------------------------------------
// Constructors & Destructors
// ---------------------------------------------------------------------------
//

CPeninputVkbKrLayoutImp* CPeninputVkbKrLayoutImp::NewVkbL( TAny* /*aInitParams*/ )
    {
    CPeninputVkbKrLayoutImp* self = 
        new(ELeave) CPeninputVkbKrLayoutImp(EPluginInputModeVkb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CPeninputVkbKrLayoutImp* CPeninputVkbKrLayoutImp::NewFsqL( TAny* /*aInitParams*/ )
    {
    CPeninputVkbKrLayoutImp* self = 
        new(ELeave) CPeninputVkbKrLayoutImp(EPluginInputModeFSQ);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CPeninputVkbKrLayoutImp::CPeninputVkbKrLayoutImp(
        const TInt& aInputMode)
    : iInputMode(aInputMode)
    {
    // See ConstructL() for initialisation completion.
    }

CPeninputVkbKrLayoutImp::~CPeninputVkbKrLayoutImp()
    {
    }

void CPeninputVkbKrLayoutImp::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// from CFepUiLayoutInterface
// ---------------------------------------------------------------------------
//

CFepUiLayout* CPeninputVkbKrLayoutImp::CreateFepUiLayoutL(
        MLayoutOwner* aLayoutOwner,
        const TAny* aData)
    {
    CPeninputVkbKrLayout* aUiLayout = 
        CPeninputVkbKrLayout::NewL(aLayoutOwner,aData,iInputMode);
    return aUiLayout;
    }
