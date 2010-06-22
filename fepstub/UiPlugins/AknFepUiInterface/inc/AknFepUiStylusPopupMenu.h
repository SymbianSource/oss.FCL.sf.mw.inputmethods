/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Interface through which the FEP can use UI components
*
*/












#ifndef AKNFEPUISTYLUSPOPUPMENU_H
#define AKNFEPUISTYLUSPOPUPMENU_H

#include <e32base.h>


class CAknFepUiInterfaceStylusPopUpMenu : public CBase
	{
public:
    virtual void AddMenuItemL(const TDesC& aItem, const TInt aCommandId) = 0;
    virtual void SetPosition(const TPoint& aPoint) = 0;    
    virtual void ShowMenu() = 0;
	};


#endif
