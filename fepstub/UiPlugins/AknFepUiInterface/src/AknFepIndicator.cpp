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
* Description:           
*       Provides the CAknFepIndicator methods.
*
*/












#include <eikenv.h>         //CEikEnv
#include <eikspane.h>       //CEikStatusPane
#include <avkon.hrh>
#include <aknnavi.h>        //CAknNavigationControlContainer
#include <aknnavide.h>      //CAknNavigationDecorator
#include <AknIndicatorContainer.h>  //CAknIndicatorContainer
#include <aknEditStateIndicator.h>
#include <AknUtils.h>
#include "AknFepIndicator.h"

#include "AknFepGlobalEnums.h"

EXPORT_C CAknFepIndicator::~CAknFepIndicator()
	{
	}

EXPORT_C void CAknFepIndicator::SetCopyMode(TBool aCopyMode)
	{
	iCopyMode = aCopyMode;
	iCopyModeChanged = ETrue;
	}


