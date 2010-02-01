/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file for peninput session
*
*/




#ifndef C_CPENINPUTSESSION_H
#define C_CPENINPUTSESSION_H

//  INCLUDES
#include <e32base.h>
#include "peninputcmdparam.h"

class CPeninputServer;
class RMessage2;


/**
 * This class represents a session in the server.
 *
 * Functions are provided to respond appropriately to client messages
 *
 * @since S60 v4.0
 */
NONSHARABLE_CLASS(CPeninputServerSession) : public CSession2
    {
public: 
    // Constructors and destructor
    /**
     * Constructor.
     *
     * @since S60 v4.0
     * @param aClient reference to client thread
     * @param aServer pointer to CPeninputServer
     */
    CPeninputServerSession(RThread &aClient, CPeninputServer& aServer);
    
    /**
     * Destructor.
     *
     * @since S60 v4.0
     */
    ~CPeninputServerSession();
    
    /**
     * Static initializer.
     *
     * @since S60 v4.0
     * @param aClient reference to client thread
     * @param aServer pointer to CPeninputServer
     * @return Sever session created
     */
    static CPeninputServerSession* NewL(RThread &aClient, 
    											CPeninputServer& aServer);

    /**
     * Signals the client with the given code, if the client has
     * registered as observer.
     *
     * @since S60 v4.0
     * @param aSignalCode The event code
     * @param aEventData The event data
     * @return ETrue if client was signalled succesfully, EFalse otherwise.
     */
    TBool SignalClientL(TInt aSignalCode,const TDesC& aEventData);

    /**
     * Signals the client that UI is activated or deactivaed.
     *
     * @since S60 v4.0
     * @param aSignalCode The event code
     * @param aType pen ui type associated with this event
     * @return ETrue if client was signalled succesfully, EFalse otherwise.
     */
    TBool SignalUiActivationObserver(TInt aSignalCode,TInt aType);
    
    
    /**
     * Handling pending event before this session is closed.
     *
     * @since S60 v4.0
     */    
    void ProcessPendingEventBeforeExit();
    
    /*
     * Returns id of the application thread that created this session.
     *
     * @since S60 v4.0
     * @return client thread id
     */
    TInt ClientId();
    
    /**
     * Increase the delayed command count
     *
     * @since S60 v4.0
     */    
    void AddDelayedCmd();
    

    /**
     * Regiseter the ui type to be monitored.
     *
     * @since S60 v4.0
     * @aType The ui type to be monitored.
     */    
    void RegisterPenUiType(TInt aType);
    
    /**
     * Panic client
     *
     * @since S60 v4.0
     * @param aMessage The client message
     * @param aPanic panic code
     */
    static void PanicClient(const RMessage2& aMessage ,TInt aPanic );    

    // from base classes CSession2

    /**
     * From CSession2.
     * handle client request
     *
     * @since S60 v4.0
     * @param aMessage The client request message
     */
    virtual void ServiceL( const RMessage2 &aMessage );
    
    /**
     * To get disabled layouts for the session of RMessage
     *
     * @since S60 v6.0
     * @param aDisabledLayouts The disabled mode to set
     */   
    const TProcessId& ClientProcessId();
    TInt DisabledUiType();
    
    TInt DoUpdateAppInfo();
    
    inline TBool DsaEverHappened() const;
private:    // New functions
    /**
     * 2nd phase constuctor
     *
     * @since S60 v4.0
     */
    void ConstructL();
    
    /**
     * handle client request
     *
     * @since S60 v4.0
     * @param aMessage The client request message
     * @return The operation result code
     */
    TInt DoServiceL( const RMessage2 &aMessage );


    /**
     * Remove the given event data
     *
     * @since S60 v4.0
     * @param aIndex The event to be remvoed. If aIndex < 0, all events
     * are removed.
     */
    void RemoveEventDataByIdx(TInt aIndex);
    
    /**
     * Save data and complete the pending request
     *
     * @since S60 v4.0
     * @param aSignalCode The event code
     * @param aEventData The event data
     * @param aPos The event data position. if -1, then append to the end.
     * @return ETrue if OK.
     */    
    TBool CompletePendingRequestL(TInt aSignalCode,const TDesC& aEventData,
                                                            int aPos = -1);
    
    TInt HandleAppInfoChange(const RMessage2& aMessage);
    TInt HandleAppInfoChangeL(const RMessage2& aMessage);
    /**
     * Build the event buf to send to client. 
     * It checks whether we can combine several events data together to send in one round.
     *
     * @since S60 v5.0
     * @return The number of combined event. -1 if there is no memory to build data buffer.
     */        
    TInt BuildEventData();
    
    /**
     * Remove the given number of event data
     *
     * @since S60 v5.0
     * @param aNum The number of events to be remvoed.
     */    
    void RemoveEventDatas(TInt aNum);
    
    /**
     * Send the event data via the given message.
     *
     * @since S60 v5.0
     * @return KErrNone if succefully, otherwise returns the length needed for msg buf.
     */                
    TInt SendEventData(const RMessage2& aMessage, const TDesC& aData);
private:    // Data

    /** 
     * Reference to PENINPUT server 
     */
    CPeninputServer& iPenInputSvr;  

    /** 
     * Each client can have one pending request which is issued with function 
     * RequestMessageNotification and is signalled when the client has to switch
     * FEP or launch help application. Pointer not owned.
     */
    RMessage2 iPendingRequest;

    /**
     * Flag to indicate whether client has issued a notification request.
     */ 
    TBool iObserverActive;

    /** 
     * Id of the client thread for debugging purposes 
     */
    TInt iClientId;    
    
    /** 
     * Delayed command count for this session 
     */    
    TInt iDelayedCmd;
    
    /**
     * The pen ui type to be monitored
     */ 
    TInt iRegistedPenUiType;    

    /**
     * event code array
     */
    RArray<TInt> iSignalCodeArray;

    /**
     * session data buffer 
     * Own
     */
    RPointerArray<HBufC> iDataBuf;    
    
    RBuf iCombinedEventBuf;    
    TInt iCombinedMsgNum;
    
    TInt iCombinedEventId;    
    
    TProcessId iClientProcess;
    
    HBufC* iAppInfo;
    
    TPeninputAppInfo iAppInfoType;
    TInt iDisabledUiType;
    TBool iDsaEverHappened;
    };
#endif //C_CPENINPUTSESSION_H

inline TBool CPeninputServerSession::DsaEverHappened() const
	{
	return iDsaEverHappened;
	}
// End of File
