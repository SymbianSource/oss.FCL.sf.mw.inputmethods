/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation for arabic peninput finger hwr
*
*/


#ifndef C_PENINPUTFINGERHWRARSTATEMANAGER_H
#define C_PENINPUTFINGERHWRARSTATEMANAGER_H

//SYSTEM INCLUDES
#include <e32base.h>
#include <e32cmn.h>

//FORWARD CLASS
class CPeninputFingerHwrArLayout;
class CPeninputFingerHwrArDataStore;
class CPeninputFingerHwrArStateManagerBase;
class CPeninputFingerHwrArStateManager;


/**
* Class MPeninputFingerHwrArStateHandler
*
* The state handler interfaces 
*
* @lib peninputfingerhwrar.lib
* @since S60 v5.0
*/
class MPeninputFingerHwrArStateHandler
    {
public:    
    /**
    * Handle state event
    *
    * @since Symbian TB9.2
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData ) = 0;
    
    /**
    * Handle the state entering action
    *
    * @since Symbian TB9.2
    */
    virtual void OnEntry() = 0;
    
    /**
    * Handle the state exiting action
    *
    * @since Symbian TB9.2
    */
    virtual void OnExit() = 0;
    };


/**
* The state handler base class.
*
* @since Symbian TB9.2.
*/    
class CPeninputFingerHwrArStateHandlerBase :public CBase 
     ,public MPeninputFingerHwrArStateHandler
    {
    
public:    
    /**
    * The handler base destructor
    *
    * @since Symbian TB9.2
    */
    virtual ~CPeninputFingerHwrArStateHandlerBase();
       
protected:
    /**
    * The constructor.
    *
    * @since Symbian TB9.2
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrArStateHandlerBase( CPeninputFingerHwrArStateManagerBase& aStateManager );
        
protected:
    /**
    * The reference to state manager.
    *
    * @since Symbian TB9.2
    */
    CPeninputFingerHwrArStateManagerBase& iStateManager;
    
    /**
    * The data store reference
    *
    * @since Symbian TB9.2
    */
    CPeninputFingerHwrArDataStore& iDataStore;      
    };



/**
* The finger hwr state manager base class.
*
* @since Symbian TB9.2
*/
class CPeninputFingerHwrArStateManagerBase : public CBase
    {
public:
    enum TStateManagerState
        {
        EStateStandBy = 0
        };
public:
    /**
    * The destructor
    *
    *@since Symbian TB9.2
    */
    virtual ~CPeninputFingerHwrArStateManagerBase();  

public:
    /**
    * Handle the hwr box and canidate events
    *
    * @since Symbian TB9.2
    * @param aEventType The event type
    * @param aEventData The event data
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
    /**
    * Return the layout reference
    *
    * @since Symbian TB9.2
    * CPeninputFingerHwrArLayout& The hwr layout reference
    */
    CPeninputFingerHwrArLayout& HwrLayout();
    
    /**
    * Is stanby state
    *
    * @since Symbian TB9.2
    * TBool The standby state flag
    */    
    TBool IsStandbyState();
    
    /**
    * Set current state.
    * 
    * @since Symbian TB9.2
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    virtual void SetState( TInt aState );     

    
protected:
    /**
    * 2nd phase constructor.
    * 
    * @since Symbian TB9.2
    */
    void BaseConstructL();

    /**
    * The constructor.
    *
    * @since Symbian TB9.2
    * @param aLayout The full screen HWR layout pointer.
    */
    CPeninputFingerHwrArStateManagerBase( CPeninputFingerHwrArLayout* aLayout );

    /**
    * Pointer to the full screen hwr layout.
    * 
    * Not own.
    */
    CPeninputFingerHwrArLayout* iLayout;
    
    /**
    * The pointer to current state.
    * Own
    */    
    CPeninputFingerHwrArStateHandlerBase* iCurrentStateHandler;
    
    /**
    * The current state.
    * Own
    */
    TInt iCurrentState;
    
    };

/**
* The finger hwr state manager for arabic.
*
* @since Symbian TB9.2
*/
class CPeninputFingerHwrArStateManager : public CPeninputFingerHwrArStateManagerBase
    {
public:
    /**
    * The factory function
    *
    * @since Symbian TB9.2
    * @param aLayout The layout reference
    * @return CPeninputFingerHwrArStateManager* The newly created CPeninputFingerHwrArStateManager object
    */
    static CPeninputFingerHwrArStateManager* NewL( CPeninputFingerHwrArLayout* aLayout );
    
    /**
    * The destructor
    *
    *@since Symbian TB9.2
    */
    ~CPeninputFingerHwrArStateManager();

public:    
    /**
    * The hwr states.
    *
    *@since Symbian TB9.2
    */    
    enum TPeninputFingerHwrArState
    {
    EPeninputFingerHwrArStateStandby = EStateStandBy,
    EPeninputFingerHwrArStateWriting,
    EPeninputFingerHwrArStateCandidateSelecting,
    EPeninputFingerHwrArStateCount
    };    

public:
    /**
    * Handle the hwr box and canidate events
    *
    * @since Symbian TB9.2
    * @param aEventType The event type
    * @param aEventData The event data
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
    /**
    * Set current state.
    * 
    * @since Symbian TB9.2
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    virtual void SetState( TInt aState );
    
private:
    /**
    * The constructor.
    *
    * @since Symbian TB9.2
    * @param aLayout The full screen HWR layout pointer.
    */
    CPeninputFingerHwrArStateManager( CPeninputFingerHwrArLayout* aLayout );
    
    /**
    * 2nd phase constructor.
    * 
    * @since Symbian TB9.2
    */
    void ConstructL();
    
    /**
    * Find the current state object.
    *
    * @since Symbian TB9.2
    * @param aState According to this state type, 
    *        we could find the corresponding state obect.
    */
    CPeninputFingerHwrArStateHandlerBase* Find( TPeninputFingerHwrArState );
    
private:    
    /**
    * Store all states pointers.
    * Own.
    */
    CPeninputFingerHwrArStateHandlerBase* iStateHandlers[EPeninputFingerHwrArStateCount];
    
    /**
    * Store all states' types.
    * Own
    */
    TPeninputFingerHwrArState iStates[EPeninputFingerHwrArStateCount];    
    };



/**
* The Standby state class.
*
* @since Symbian TB9.2
*/    
class CPeninputFingerHwrArStateStandby : public CPeninputFingerHwrArStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Symbian TB9.2
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerStandby* Return the 
          CPeninputFingerHwrStateHandlerStandby object pointer. 
    */
    static CPeninputFingerHwrArStateStandby* NewL( 
        CPeninputFingerHwrArStateManager& aStateManager );
public:
    
    /**
    * Handle state event
    *
    * @since Symbian TB9.2
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Symbian TB9.2
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Symbian TB9.2
    */
    void OnExit();
    
private:
    /**
    * The constructor
    *
    * @since Symbian TB9.2
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrArStateStandby( 
         CPeninputFingerHwrArStateManager& aStateManager );
            
    };


/**
* The Writing state class.
*
* @since Symbian TB9.2
*/
class CPeninputFingerHwrArStateWriting : public CPeninputFingerHwrArStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Symbian TB9.2
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerWriting* Return the 
          CPeninputFingerHwrStateHandlerWriting object pointer. 
    */
    static CPeninputFingerHwrArStateWriting* NewL( 
        CPeninputFingerHwrArStateManager& aStateManager );    
public:
    
    /**
    * Handle state event
    *
    * @since Symbian TB9.2
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Symbian TB9.2
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Symbian TB9.2
    */
    void OnExit();
    
private:
    /**
    * The constructor
    *
    * @since Symbian TB9.2
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrArStateWriting( CPeninputFingerHwrArStateManager& aStateManager );
    
private:
    /**
    * Handle the end stroke event
    *
    * @since Symbian TB9.2
    * @param aEventData The trace data
    */
    void OnEndStrokeL( const TDesC& aEventData );

    /**
    * Handle the end writing event
    *
    * @since Symbian TB9.2
    * @param aEventData The trace data
    */
    void OnEndWritingL( const TDesC& aEventData );
    
    };

/**
* The CandidateSelecting state class for Chinese.
*
* @since Symbian TB9.2
*/
class CPeninputFingerHwrArStateCandidateSelecting : public CPeninputFingerHwrArStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Symbian TB9.2
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerCandidateSelecting Return the 
          CPeninputFingerHwrStateHandlerCandidateSelecting object pointer. 
    */
    static CPeninputFingerHwrArStateCandidateSelecting* NewL( 
        CPeninputFingerHwrArStateManager& aStateManager );    
public:
    
    /**
    * Handle state event
    *
    * @since Symbian TB9.2
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Symbian TB9.2
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Symbian TB9.2
    */
    void OnExit();
    
private:
    /**
    * The constructor
    *
    * @since Symbian TB9.2
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrArStateCandidateSelecting( 
              CPeninputFingerHwrArStateManager& aStateManager );
    
private:
    /**
    * Handle one candiate selected event
    * 
    * @since Symbian TB9.2
    * @param aEventData The selected candidate
    */
    void OnSelectedCandidatesL( const TDesC& aEventData );
    
    
    /**
    * Handle backspace events
    *
    * @since Symbian TB9.2
    */
    void OnClickBackSpaceL();
    };



#endif    //C_PENINPUTFINGERHWRARSTATEMANAGER_H
