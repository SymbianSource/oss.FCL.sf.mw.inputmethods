/*
* Copyright (c) 2005,2006 Choe Hwanjin
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
* Description:
*
*/


#include "OssHanja.h"

COssHanja::COssHanja()
    {
    // No implementation required
    }

COssHanja::~COssHanja()
    {
    }

COssHanja* COssHanja::NewLC()
    {
    COssHanja* self = new (ELeave) COssHanja();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

COssHanja* COssHanja::NewL()
    {
    COssHanja* self = COssHanja::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void COssHanja::ConstructL()
    {

    }
