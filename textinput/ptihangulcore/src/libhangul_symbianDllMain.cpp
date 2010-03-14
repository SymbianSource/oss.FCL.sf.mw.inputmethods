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


//  Include Files  

#include <e32std.h>		 // GLDEF_C
#include "libhangul_symbian.pan"		// panic codes

//  Global Functions

GLDEF_C void Panic(Tlibhangul_symbianPanic aPanic)
// Panics the thread with given panic code
    {
    User::Panic(_L("libhangul_symbian"), aPanic);
    }

//  Exported Functions

#ifndef EKA2 // for EKA1 only
EXPORT_C TInt E32Dll(TDllReason /*aReason*/)
// Called when the DLL is loaded and unloaded. Note: have to define
// epoccalldllentrypoints in MMP file to get this called in THUMB.
    {
    return KErrNone;
    }
#endif

