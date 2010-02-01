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
* Description:  gs key handle callback header file.
*
*/


#ifndef M_GSCENREPNOTIFYHANDLERCALLBACK_H
#define M_GSCENREPNOTIFYHANDLERCALLBACK_H

/**
 *  This class provides a callback interface for handling the
 *  notifification events from the CenRep. The Client derives a
 *  class from this interface and implements the HandleNotify-methods
 *  that interest it.
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class MGsCenRepNotifyHandlerCallback
    {

    public:

    /**
     * This callback method is used to notify the client about
     * key changing
     * 
     * @since S60 v3.2
     * @param aId Key id 
     * @param aNewValue New value
     * @return None
     */
    virtual void HandleNotifyInt(TUint32 aId, TInt aNewValue) = 0;
    
    /**
     * This callback method is used to notify the client about
     * key changing
     * 
     * @param aRepositoryUid The reposiroty uid for each key
     * @param aId Key id 
     * @param aNewValue New value
     * @return None
     */
    virtual void HandleNotifyInt(TUid aRepositoryUid, TUint32 aId, TInt aNewValue) = 0;
    
    };
    
#endif // M_GSCENREPNOTIFYHANDLERCALLBACK_H

// End Of File    
