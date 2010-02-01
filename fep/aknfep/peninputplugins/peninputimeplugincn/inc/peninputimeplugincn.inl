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
const TInt KInputMethodImplementationId = 0x10207361;
const TInt KHwrUiId = 0x10281856;
const TInt KFScUiId = 0x2000B132;
const TInt KVkbUiId = 0x1028185C;
const TInt KFsqUiId = 0x2001114E;
const TInt KFingerHwrUiId = 0x2001CB83;

inline CPluginFepManagerBase* CPenInputImePluginCn::GetCurrentUi()
    {
    switch(iPenInputMode)
        {
        case EPluginInputModeHwr:
        case EPluginInputModeVkb:
        case EPluginInputModeFSc:
        case EPluginInputModeFingerHwr:
        case EPluginInputModeFSQ:
            return iPluginUiManager;
        default:
            return NULL;
        }
    }

//End Of File
