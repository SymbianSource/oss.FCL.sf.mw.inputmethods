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
* Description:  application class of peninputsetting
*
*/


#ifndef C_PENINPUTSETTINGAPPLICATION_H
#define C_PENINPUTSETTINGAPPLICATION_H

#include <aknapp.h>

/**
*  CPenInputSettingApplication application class.
*  Provides factory to create concrete document object.
*/
class CPenInputSettingApplication : public CAknApplication
    {
public:  // from CAknApplication

    /** 
     * From CApaApplication
     * returns application's UID (KUidPenInputSetting).
     *
     * @since S60 v4.0
     * @return The value of KUidPenInputSetting.
     */        
    TUid AppDllUid() const;

protected: // from CAknApplication

    /** 
     * From CApaApplication
     * creates CPenInputSettingDocument document object.
     *
     * @since S60 v4.0
     * @return A pointer to the created document object.
     */
    CApaDocument* CreateDocumentL();
    };

#endif //C_PENINPUTSETTINGAPPLICATION_H
