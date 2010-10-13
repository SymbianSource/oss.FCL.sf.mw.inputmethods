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
* Description:
*
*/

#ifndef PENUIWNDEVENTOBSERVER_H_
#define PENUIWNDEVENTOBSERVER_H_

/**
 *  MPenUiWndEventObserver
 *  
 *  The observer to handle pen ui window event
 * 
 */
class MPenUiWndEventObserver
    {
public:
    /**
     * Signal owner that there is an event
     *
     * @since symbian^3     
     * @param aEventType The event type
     * @param aEventData The event data
     */
    virtual void SignalOwner( TInt aEventType, const TDesC& aEventData ) = 0;
    
    /**
     * Handle layout command event.
     *
     * @since symbian^3  
     * @param aCmd Command Id.
     * @param aData Data for command.
     */    
    virtual void HandleCommand( TInt aCmd, TUint8* aData ) = 0;
    };

#endif /* PENUIWNDEVENTOBSERVER_H_ */
