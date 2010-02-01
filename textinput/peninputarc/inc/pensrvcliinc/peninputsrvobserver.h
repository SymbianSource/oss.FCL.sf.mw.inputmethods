/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This file contains classes of Peninput server client side.
*
*/


#ifndef R_RPENINPUTSERVER_OBSERVER_H
#define R_RPENINPUTSERVER_OBSERVER_H

#include <peninputsrveventhandler.h>

class RPeninputServerImpl;
/**
 * Peninput server observer
 *
 *
 * @lib peninputclient.lib
 * @since S60 v4.0 
 */
NONSHARABLE_CLASS(CPeninputServerObserver) : public CActive,
                                             public MPeninputServerEventHandler
    {
public:
    static CPeninputServerObserver* NewL(RPeninputServerImpl* aPeninputServer,
    						                    TThreadId aSrvThreadId);
    /**
     * Constructor 
     *
     * @since S60 v4.0
     * @param aPeninputServer The server to be observered
     * @param aHandler A server event handler
     */
    CPeninputServerObserver(RPeninputServerImpl* aPeninputServer,
    						                        TThreadId aSrvThreadId);

    /**
     * Destructor 
     *
     * @since S60 v4.0
     */
    ~CPeninputServerObserver();

    /**
     * Set UI activation handler
     *
     * @since S60 v4.0
     * @param aHandler A server event handler
     */
	void AddUiActivationHandler(MPeninputServerEventHandler* aHandler);
	
    /**
     * Add server event handler
     *
     * @since S60 v4.0
     * @param aHandler A server event handler
     */	
	void AddEventHandler(MPeninputServerEventHandler* aHandler);
	void SetActive()
	    {
	    CActive::SetActive();
	    }
	TRequestStatus& RequestStatus() 
	    {
	    return iStatus;
	    }
    void ReConstructL(TThreadId aId);	    
protected: 
     void ConstructL();
    //From base class CActive

    /** 
     * From CActive
     * Cancels all notification requests 
     *
     * @since S60 v4.0
     */
    void DoCancel();

    /** 
     * From CActive
     * Receives messages and carries out requested operations
     *
     * @since S60 v4.0
     */
    void RunL();

    /** 
     * From CActive
     * Handles error if RunL() leaves
     *
     * @since S60 v4.0
     */
    TInt RunError(TInt aError);
private:
    virtual TBool HandleServerEventL(TInt aEventId);	

public:
    /**
     * server event handler
     * Not own
     */
    MPeninputServerEventHandler* iHandler;
    
    /**
     * UI activation event handler
     * Not own
     */
    MPeninputServerEventHandler* iUiActivationHandler;

private:
    /**
     * Reference to connected RPeninputServerImpl session
     * Not own
     */
    RPeninputServerImpl* iPeninputServer;

    /**
     * Server thread id
     */        
    TThreadId iSrvThreadId;
    
    /**
     * Server status flag
     */        
    TBool iServerExit;
};


//class CPenUiNotificationHandler
NONSHARABLE_CLASS(CPenUiNotificationHandler): public CBase,public MPeninputServerEventHandler
    {
    public:
        static CPenUiNotificationHandler* NewL(MPenUiActivationHandler* aHandler,
                                                RPeninputServerImpl* aClient,
                                                TInt aType);
        TBool HandleServerEventL(TInt aEventId);
        MPenUiActivationHandler* Handler() 
            {
            return iHandler;
            }
        TInt RegisteredType() 
            {
             return iType;
            }
    private:
        CPenUiNotificationHandler(MPenUiActivationHandler* aHandler,
                                    RPeninputServerImpl* aClient,TInt aType);
        void ConstructL();
    private:
        MPenUiActivationHandler* iHandler;        
        RPeninputServerImpl* iClient; //not owned
        TInt iType;
    };

#endif //R_RPENINPUTSERVER_OBSERVER_H
