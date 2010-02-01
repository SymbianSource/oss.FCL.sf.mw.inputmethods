/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PENINPUTPLUGINKR_PAN__
#define __PENINPUTPLUGINKR_PAN__

//  Data Types

enum TPeninputPluginKrPanic
    {
    EPeninputPluginKrNullPointer
    };

//  Function Prototypes

GLREF_C void Panic(TPeninputPluginKrPanic aPanic);

#endif  // __PENINPUTPLUGINKR_PAN__

