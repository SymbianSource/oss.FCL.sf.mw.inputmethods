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
* Description:            Inline file for Ime plugin China.
*
*/












const TInt KHwrLayoutInterfaceId = 0x102072FC; // This is for layout interface
const TInt KInputMethodImplementationId = 0x1028235F;
const TInt KHwrUiId = 0x1028235B;
const TInt KHwrArabicUiId = 0x20019599;
const TInt KVkbUiId = 0x10282359;
const TInt KFsqUiId = 0x2001114D;
const TInt KSsqUiId = 0x2001E2BB;
const TInt KPrtFsqUiId = 0x20026837;
const TInt KFingerHwrUiId = 0x2001E2C4;
inline CPluginFepManagerBase* CPenInputImePluginGeneric::GetCurrentUi()
    {
    switch ( iPenInputMode )
        {
        case EPluginInputModeHwr:
        case EPluginInputModeVkb:
        case EPluginInputModeFingerHwr:            
        case EPluginInputModeFSQ:
        // Portrait FSQ is added as a new mode
        case EPluginInputModePortraitFSQ:
            return iPluginUiManager;
        default:
            return NULL;
        }
    }

//End Of File
