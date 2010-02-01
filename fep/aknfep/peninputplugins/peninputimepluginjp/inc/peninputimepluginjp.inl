/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Inline file for Ime plugin Japan.
*
*/












const TInt KHwrLayoutInterfaceId = 0x102072FC; //this is for layout interface
const TInt KInputMethodImplementationId = 0x10282419;
const TInt KHwrUiId = 0x1028241B;
const TInt KVkbUiId = 0x1028241A;
const TInt KHwrUiIndex = 0;
const TInt KVkbUiIndex = 1;

inline CPluginFepManagerBase* CPenInputImePluginJp::GetCurrentUi()
    {
    switch(iPenInputMode)
        {
        case EPluginInputModeHwr:
            return iAllUiManagers[KHwrUiIndex];
        case EPluginInputModeVkb:
            return iAllUiManagers[KVkbUiIndex];
        default:
            return NULL;
        }
    }

//End Of File
