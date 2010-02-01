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

#ifndef PENINPUTVKBLAYOUTIMP_H
#define PENINPUTVKBLAYOUTIMP_H

#include <PeninputLayout.h>
#include <PeninputLayoutPluginInterface.h>

class CPeninputVkbKrLayoutImp : public CFepUiLayoutInterface
    {
public:

    static CPeninputVkbKrLayoutImp* NewVkbL(TAny* aInitParams);
    
    static CPeninputVkbKrLayoutImp* NewFsqL(TAny* aInitParams);

    virtual ~CPeninputVkbKrLayoutImp();
    
private:

    CPeninputVkbKrLayoutImp(const TInt& aInputMode);

    void ConstructL();

private: // from CFepUiLayoutInterface

    CFepUiLayout* CreateFepUiLayoutL(
        MLayoutOwner* aLayoutOwner,
        const TAny* aData);

private:
    
    TInt iInputMode;
    
    };

#endif // PENINPUTVKBLAYOUTIMP_H
