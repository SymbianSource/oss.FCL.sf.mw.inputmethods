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

#ifndef PENUIWNDEVENTHANDLER_H_
#define PENUIWNDEVENTHANDLER_H_

#include "penuiwndeventobserver.h"

class CPeninputServer;

/**
 *  CPenUiWndEventHandler
 *  
 *  The pen ui window event handler
 * 
 */
class CPenUiWndEventHandler : public CBase, public MPenUiWndEventObserver
    {
public:
    /**
     * Two-phased constructor.
     * 
     * @since symbian^3
     * @param aServer the pointer to the owner of the control     
     * @return Pointer to CPenUiCandidateWnd's instance
     */
    static CPenUiWndEventHandler* NewL( CPeninputServer* aServer );

    /**
     * Two-phased constructor.
     * 
     * @since symbian^3
     * @param aServer the pointer to the owner of the control     
     * @return Pointer to CPenUiCandidateWnd's instance
     */
    static CPenUiWndEventHandler* NewLC( CPeninputServer* aServer );
    
    /**
     * Destructor
     */
    ~CPenUiWndEventHandler();

// From MPenUiWndEventObserver
    /**
     * Signal owner that there is an event
     *
     * @since symbian^3     
     * @param aEventType The event type
     * @param aEventData The event data
     */
    virtual void SignalOwner( TInt aEventType, const TDesC& aEventData );
    
    /**
     * Handle layout command event.
     *
     * @since symbian^3  
     * @param aCmd Command Id.
     * @param aData Data for command.
     */
    virtual void HandleCommand( TInt aCmd, TUint8* aData );

private:
    
    /**
     * C++ Default Constructor 
     */
    CPenUiWndEventHandler( CPeninputServer* aServer );
    
    /**
     * 2nd phase constructor. 
     * 
     * @since Symbain^3
     * @return None
     */
    void ConstructL();
    
private:
    /**
     * The owner of the control
     * 
     * Now own
     */
    CPeninputServer* iServer;    
    };

#endif /* PENUIWNDEVENTHANDLER_H_ */
