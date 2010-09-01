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
* Description:  boxbase hwr states
*
*/


#ifndef AKNFEPHWRBXSTATE_H
#define AKNFEPHWRBXSTATE_H

//  INCLUDES
#include <e32base.h>

class CAknFepHwrBxLayout;
class CAknFepHwrBxWnd;
class CAknFepHwrBxDataStore;

/**
 *  base state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStateBase : public CBase
    {
public:

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStateBase();

    /**
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd The command id.
     * @param aData The command data.
     * @return KErrNone for success;otherwise return error code
     */
    virtual TInt HandleCommandL(TInt aCmd, TUint8* aData);
    
    /**
     * Handle raw event key
     *
     * @since S60 v3.2
     * @param aData The key raw event data
     * @return ETrue if the key was handled, otherwise EFalse
     */            
    virtual TBool HandleKeyEventL(const TRawEvent& aKeyEvent);

    /**
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return ETrue if handled otherwise EFalse                
     */
    virtual TBool HandleEventL(TInt aEventType,const TDesC& aEventData);

    /**
     * OnEntry
     *
     * @since S60 v3.2
     * @return None
     */            
    virtual void OnEntry();

    /**
     * OnExit
     *
     * @since S60 v3.2
     * @return None
     */            
    virtual void OnExit();

protected:  

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */            
    CAknFepHwrBxStateBase(CAknFepHwrBxLayout* aLayout);

protected: //data
    
    /**
     * layout (Not own)
     */
    CAknFepHwrBxLayout* iLayout;

    /**
     * window (Not own)
     */
    CAknFepHwrBxWnd* iWnd;

    /**
     * Data store (Not own)
     */
    CAknFepHwrBxDataStore* iDataStore;

    };

/**
 *  base closeable state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStateCloseable : public CAknFepHwrBxStateBase
    {
public:

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStateCloseable();

    /**
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd The command id.
     * @param aData The command data.
     * @return KErrNone for success;otherwise return error code
     */
    TInt HandleCommandL(TInt aCmd, TUint8* aData);
    
protected:  

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */            
    CAknFepHwrBxStateCloseable(CAknFepHwrBxLayout* aLayout);

    };


/**
 *  inti state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStateInit : public CAknFepHwrBxStateBase
    {
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return Pointer to the created CAknFepHwrBxStateInit object
     */          
    static CAknFepHwrBxStateInit* NewLC(CAknFepHwrBxLayout* aLayout);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStateInit();

    /**
     * Set auto close flag
     *
     * @since S60 v3.2
     * @param aAutoFlag The auto close flag
     * @return None
     */          
    void SetAutoClosed(TBool aAutoFlag);

//from CAknFepHwrBxStateBase

    /**
     * From CAknFepHwrBxStateBase.
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd The command id.
     * @param aData The command data.
     * @return KErrNone for success;otherwise return error code
     */
    TInt HandleCommandL(TInt aCmd, TUint8* aData);
    
    /**
     * From CAknFepHwrBxStateBase.
     * OnEntry
     *
     * @since S60 v3.2
     * @return None
     */            
    void OnEntry();

    /**
     * From CAknFepHwrBxStateBase.
     * OnExit
     *
     * @since S60 v3.2
     * @return None
     */            
    void OnExit();

private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */          
    CAknFepHwrBxStateInit(CAknFepHwrBxLayout* aLayout);

private: //data
    
    /**
     * The auto close flag EFalse when user click the close button;otherwise ETrue
     */
    TBool iAutoClosed;

    };  
    
/**
 *  standby state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStateStandby : public CAknFepHwrBxStateCloseable
    {
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return Pointer to the created CAknFepHwrBxStateStandby object
     */          
    static CAknFepHwrBxStateStandby* NewLC(CAknFepHwrBxLayout* aLayout);
    
    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return Pointer to the created CAknFepHwrBxStateStandby object
     */
    static CAknFepHwrBxStateStandby* NewL(CAknFepHwrBxLayout* aLayout);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStateStandby();

//from CAknFepHwrBxStateBase

    /**
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return ETrue if handled otherwise EFalse                
     */
    TBool HandleEventL(TInt aEventType, const TDesC& aEventData);

    /**
     * From CAknFepHwrBxStateBase.
     * OnEntry
     *
     * @since S60 v3.2
     * @return None
     */            
    void OnEntry();

private:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */            
    CAknFepHwrBxStateStandby(CAknFepHwrBxLayout* aLayout);

private: //data    
    
    TBool iClearCandidate;
    
    };  
    
/**
 *  begin writing state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStateBeginWriting : public CAknFepHwrBxStateCloseable
    {
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return Pointer to the created CAknFepHwrBxStateBeginWriting object
     */          
    static CAknFepHwrBxStateBeginWriting* NewLC(CAknFepHwrBxLayout* aLayout);
    
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStateBeginWriting();

//from CAknFepHwrBxStateBase

    /**
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return ETrue if handled otherwise EFalse                
     */
    TBool HandleEventL(TInt aEventType, const TDesC& aEventData);

    /**
     * From CAknFepHwrBxStateBase.
     * OnEntry
     *
     * @since S60 v3.2
     * @return None
     */            
    void OnEntry();

private:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */            
    CAknFepHwrBxStateBeginWriting(CAknFepHwrBxLayout* aLayout);

    }; 
    
/**
 *  End writing state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStateEndWriting : public CAknFepHwrBxStateCloseable
    {
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return Pointer to the created CAknFepHwrBxStateEndWriting object
     */          
    static CAknFepHwrBxStateEndWriting* NewLC(CAknFepHwrBxLayout* aLayout);
    
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStateEndWriting();

//from CAknFepHwrBxStateBase

    /**
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return ETrue if handled otherwise EFalse                
     */
    TBool HandleEventL(TInt aEventType, const TDesC& aEventData);
    
    /**
     * From CAknFepHwrBxStateBase.
     * Handle raw event key
     *
     * @since S60 v3.2
     * @param aData the key raw event data
     * @return ETrue if the key was handled, otherwise EFalse
     */            
    TBool HandleKeyEventL(const TRawEvent& aKeyEvent);

    /**
     * From CAknFepHwrBxStateBase.
     * OnExit
     *
     * @since S60 v3.2
     * @return None
     */            
    void OnExit();

private:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */            
    CAknFepHwrBxStateEndWriting(CAknFepHwrBxLayout* aLayout);
    
    /**
     * Handle the CandidateSelected event
     *
     * @since S60 v3.2
     * @param aEventData The event data.
     * @return None
     */
    void OnCandidateSelectedL(const TDesC& aEventData);
    
    }; 

/**
 *  Candidate selecting state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStateCandidateSelecting : public CAknFepHwrBxStateCloseable
    {
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return Pointer to the created CAknFepHwrBxStateCandidateSelecting object
     */          
    static CAknFepHwrBxStateCandidateSelecting* NewLC(CAknFepHwrBxLayout* aLayout);
    
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStateCandidateSelecting();

//from CAknFepHwrBxStateBase

    /**
     * From CAknFepHwrBxStateBase.
     * Handle raw event key
     *
     * @since S60 v3.2
     * @param aData The key raw event data
     * @return ETrue if the key was handled, otherwise EFalse
     */            
    TBool HandleKeyEventL(const TRawEvent& aKeyEvent);

    /**
     * From CAknFepHwrBxStateBase.
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return ETrue if handled otherwise EFalse                
     */
    TBool HandleEventL(TInt aEventType, const TDesC& aEventData);

    /**
     * From CAknFepHwrBxStateBase.
     * OnEntry
     *
     * @since S60 v3.2
     * @return None
     */            
    void OnEntry();

private:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */            
    CAknFepHwrBxStateCandidateSelecting(CAknFepHwrBxLayout* aLayout);

    /**
     * Handle the CandidateSelected event
     *
     * @since S60 v3.2
     * @param aEventData The event data.
     * @return None
     */
    void OnCandidateSelectedL(const TDesC& aEventData);

    }; 
    
/**
 *  Predictive selecting state
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxStatePredictiveSelecting : public CAknFepHwrBxStateCloseable
    {
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return Pointer to the created CAknFepHwrBxStatePredictiveSelecting object
     */          
    static CAknFepHwrBxStatePredictiveSelecting* NewLC(CAknFepHwrBxLayout* aLayout);
    
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxStatePredictiveSelecting();

//from CAknFepHwrBxStateBase

    /**
     * From CAknFepHwrBxStateBase.
     * Handle raw event key
     *
     * @since S60 v3.2
     * @param aData The key raw event data
     * @return ETrue if the key was handled, otherwise EFalse
     */            
    TBool HandleKeyEventL(const TRawEvent& aKeyEvent);

    /**
     * From CAknFepHwrBxStateBase.
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return ETrue if handled otherwise EFalse                
     */
    TBool HandleEventL(TInt aEventType, const TDesC& aEventData);

    /**
     * From CAknFepHwrBxStateBase.
     * OnEntry
     *
     * @since S60 v3.2
     * @return None
     */            
    void OnEntry();

private:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aLayout The Layout
     * @return None
     */            
    CAknFepHwrBxStatePredictiveSelecting(CAknFepHwrBxLayout* aLayout);
    
    /**
     * Handle the PredictiveSelected event
     *
     * @since S60 v3.2
     * @param aEventData The event data.
     * @return None
     */
    void OnPredictiveSelectedL(const TDesC& aEventData);
    
    /**
     * Handle the Keyback event
     *
     * @since S60 v3.2
     * @return None
     */
    void OnKeyBackL();

    };     
        
#endif // AKNFEPHWRBXSTATE_H

// End Of File
