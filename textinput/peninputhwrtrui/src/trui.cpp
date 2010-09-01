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
* Description:  Application entrance
*
*/


#include <eikstart.h>
#include "truiapplication.h"

// ================= EXPORTED FUNCTIONS =================================
// ---------------------------------------------------------------------------
// Create an application, and return a pointer to it
// ---------------------------------------------------------------------------
//
CApaApplication* NewApplication()
	{
	return new CTruiApplication;
	}


TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );	
	}

