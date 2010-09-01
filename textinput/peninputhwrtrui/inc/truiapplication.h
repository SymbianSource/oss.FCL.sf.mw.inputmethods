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
* Description:  Application class of TrainingUI
*
*/


#ifndef C_TRUIAPPLICATION_H
#define C_TRUIAPPLICATION_H

// UID for the application, this should correspond to the uid defined in the mmp file
static const TUid KUidtruiApp = { 0x2000B104 };

#include <aknapp.h>

/**
 *  Instance of CTruiApplication is the application part of the AVKON
 *  application framework for the TrainingUI application
 *
 *  @code
 *   new CTruiApplication;
 *  @endcode
 *
 *  @lib avkon.lib eikcore.lib eiksrv.lib
 */
class CTruiApplication : public CAknApplication
    {
public:  // from CAknApplication

    /**
     * Returns the application DLL UID value
     *
     * @return the UID of this Application/Dll
     */
    TUid AppDllUid() const;

protected: // from CAknApplication

    /**
     * Create a CApaDocument object and return a pointer to it
     *
     * @return a pointer to the created document
     */
    CApaDocument* CreateDocumentL();
    
	/**
    * Opens the .ini file associated with the application.
    *
    * @param aFs File server session to use. Not used.
    * @return Pointer to the dictionary store object representing the
    * application's .ini file.
    */    
    CDictionaryStore* OpenIniFileLC( RFs& aFs ) const;    
    };

#endif // C_TRUIAPPLICATION_H

