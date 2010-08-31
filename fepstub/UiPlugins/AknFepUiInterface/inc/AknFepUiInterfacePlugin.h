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
*       Interface for FEP UI plugins
*
*/












#ifndef AKNFEPUIINTERFACEPLUGIN_H
#define AKNFEPUIINTERFACEPLUGIN_H

#include <e32base.h>

class CAknFepUIInterface;

class CFepUiInterfacePlugin : public CBase
	{
public:
	enum TInterfaceUid { ETypeId = 0x10285779 };
	virtual TBool UseThisPlugin() const = 0;
	virtual CAknFepUIInterface* NewImplL() = 0;
	};

#endif
