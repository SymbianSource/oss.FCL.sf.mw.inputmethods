/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Defination for peninput server application
*
*/


#ifndef PENINPUTSERVERAPP_H
#define PENINPUTSERVERAPP_H

// INCLUDES
#include <aknapp.h>
// CONSTANTS
// UID of the application
const TUid KUidPeninputserver = { 0x10281855 };
class CPeninputServer;
// CLASS DECLARATION

/**
* CPeninputServerApp application class.
* Provides factory to create concrete document object.
* 
*/
class CPeninputServerApp : public CAknApplication
    {    
public: // Functions from base classes
    /**
     * Destructor
     *
     * @since S60 v4.0
     */        
     ~CPeninputServerApp();
     
private:

    //from CApaApplication

    /** 
     * From CApaApplication
     * creates CPeninputServerDocument document object.
     *
     * @since S60 v4.0
     * @return A pointer to the created document object.
     */
    CApaDocument* CreateDocumentL();
       
    /** 
     * From CApaApplication
     * returns application's UID (KUidPeninputserver).
     *
     * @since S60 v4.0
     * @return The value of KUidPeninputserver.
     */        
    TUid AppDllUid() const;
    };

#endif //PENINPUTSERVERAPP_H
// End of File

