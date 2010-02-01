/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Control's observer
*
*/


#ifndef M_TRUIHWRBOXOBSERVER_H
#define M_TRUIHWRBOXOBSERVER_H

// CLASS DECLARATION

/**
* MTruiHwrBoxObserver.
* An instance of a class derived from MTrainingUIHwrBoxObserver is an object
* which can be informed when an event defined in this interface occured.
*/
class MTruiHwrBoxObserver 
    {
public:
    enum TMessageType
        {
        EModelExist,
        EModelDelete
        };
    /**
     * TimeOut
     * Handle the event when a time out occurs.
     * @param aModel the array of points where the model is stored
     */
    virtual void SaveNewModelL() = 0;
    
    /**
     * HandleEventL
     * Handle the message event from the HwrBox
     * @param aMessage According to the aMessage to handle the event
     */
    virtual void HandleEventL( TMessageType aMessage ) = 0;
    };

#endif //M_TRUIHWRBOXOBSERVER_H

