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
const TInt KInputMethodImplementationId = 0x2000B133;
// modified by txin in 2007.06.04, change vkb's implementation uid to finger's
const TInt KItutUiId = 0x2000b101;
const TInt KSplitItutUiId = 0x2001E2BA;
// end modifying


inline CPluginFepManagerBase* CPenInputImePluginItut::GetCurrentUi()
    {
    switch ( iPenInputMode )
        {
        case EPluginInputModeHwr:
        case EPluginInputModeVkb:
        case EPluginInputModeItut:
            return iPluginUiManager;
        default:
            return NULL;
        }
    }

//End Of File
