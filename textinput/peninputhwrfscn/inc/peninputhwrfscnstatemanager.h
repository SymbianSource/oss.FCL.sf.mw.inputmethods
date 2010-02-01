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
* Description:  Implementation for chinese peninput full screen hwr
*
*/


#ifndef __PENINPUTHWRFSCNSTATEMANAGER_H__
#define __PENINPUTHWRFSCNSTATEMANAGER_H__
//SYSTEM INCLUDES
#include <e32base.h>
#include <e32cmn.h>
//FORWARD CLASS
class CPeninputHwrfscnLayout;
class CPeninputHwrfscnDataStore;
class CPeninputHwrfscnStateManager;

/**
* Class MPeninputHwrfscnStateHandler
*
* The state handler interfaces 
*
* @lib peninputhwrfscn.lib
* @since S60 v5.0
*/
class MPeninputHwrfscnStateHandler
    {
public:
    /**
    * Handle command
    *
    * @since Series 60 5.0
    */
    virtual void HandleCommand( TInt aCmd, TUint8* aData ) = 0;
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    virtual void HandleEventL( TInt aEventType, const TDesC& aEventData ) = 0;
    
    /**
    * Handle the state entering action
    *
    * @since Series 60 5.0
    */
    virtual void OnEntry() = 0;
    
    /**
    * Handle the state exiting action
    *
    * @since Series 60 5.0
    */
    virtual void OnExit() = 0;
    };

/**
* The state handler base class.
*
* @since Series 60 5.0.
*/    
class CPeninputHwrfscnStateHandlerBase :public CBase 
     ,public MPeninputHwrfscnStateHandler
    {
    
public:    
    /**
    * The handler base destructor
    *
    * @since Series 60 5.0
    */
    virtual ~CPeninputHwrfscnStateHandlerBase();
       
protected:
    /**
    * The constructor.
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    */
    CPeninputHwrfscnStateHandlerBase( CPeninputHwrfscnStateManager& aStateManager );
        
protected:
    /**
    * The reference to state manager.
    *
    * @since Series 60 5.0
    */
    CPeninputHwrfscnStateManager& iStateManager;
    
    /**
    * The data store reference
    *
    * @since Series 60 5.0
    */
    CPeninputHwrfscnDataStore& iDataStore;      
    };

/**
* The Standby state class.
*
* @since Series 60 5.0
*/    
class CPeninputHwrfscnStateHandlerStandby : public CPeninputHwrfscnStateHandlerBase
    {
public:
    /**
     * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    * @return CPeninputHwrfscnStateHandlerStandby* Return the 
          CPeninputHwrfscnStateHandlerStandby object pointer. 
    */
    static CPeninputHwrfscnStateHandlerStandby* NewL( 
        CPeninputHwrfscnStateManager& aStateManager );
public:
    /**
    * Handle command
    *
    * @since Series 60 5.0
    */
    void HandleCommand( TInt aCmd, TUint8* aData );
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Series 60 5.0
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Series 60 5.0
    */
    void OnExit();
    
private:
    /**
    * The constructor
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    */
    CPeninputHwrfscnStateHandlerStandby( 
         CPeninputHwrfscnStateManager& aStateManager );
            
    };
    
/**
* The Writing state class.
*
* @since Series 60 5.0
*/
class CPeninputHwrfscnStateHandlerWriting : public CPeninputHwrfscnStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    * @return CPeninputHwrfscnStateHandlerWriting* Return the 
          CPeninputHwrfscnStateHandlerWriting object pointer. 
    */
    static CPeninputHwrfscnStateHandlerWriting* NewL( 
        CPeninputHwrfscnStateManager& aStateManager );    
public:
    /**
    * Handle command
    *
    * @since Series 60 5.0
    */
    void HandleCommand( TInt aCmd, TUint8* aData );
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Series 60 5.0
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Series 60 5.0
    */
    void OnExit();
    
private:
    /**
    * The constructor
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    */
    CPeninputHwrfscnStateHandlerWriting( CPeninputHwrfscnStateManager& aStateManager );
    
private:
    /**
    * Handle the end writing event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnEndWritingL( const TDesC& aEventData );        
    };

/**
* The CandidateSelecting state class.
*
* @since Series 60 5.0
*/
class CPeninputHwrfscnStateHandlerCandidateSelecting : public CPeninputHwrfscnStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    * @return CPeninputHwrfscnStateHandlerCandidateSelecting Return the 
          CPeninputHwrfscnStateHandlerCandidateSelecting object pointer. 
    */
    static CPeninputHwrfscnStateHandlerCandidateSelecting* NewL( 
        CPeninputHwrfscnStateManager& aStateManager );    
public:    
    /**
    * Handle command
    *
    * @since Series 60 5.0
    */
    void HandleCommand( TInt aCmd, TUint8* aData );
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Series 60 5.0
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Series 60 5.0
    */
    void OnExit();
    
private:
    /**
    * The constructor
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    */
    CPeninputHwrfscnStateHandlerCandidateSelecting( 
              CPeninputHwrfscnStateManager& aStateManager );
    
private:
    /**
    * Handle one candiate selected event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */
    void OnSelectedCandidatesL( const TDesC& aEventData );
    
    /**
    * Handle predictive candidate selected events
    *
    * @since Series 60 5.0
    * @param aEventData The selected predictive candidate
    * @param aAppend ETrue if the aEventData need to be appended onto trigger string
             EFalse if the aEventData need to set trigger string and do not
             submit it to layout
    */
    void OnSelectedPredictivesL( const TDesC& aEventData, TBool aAppend = ETrue );
    };

/**
* The HomophonicSelecting state class.
*
* @since Series 60 5.0
*/
class CPeninputHwrfscnStateHandlerHomophonicSelecting : public CPeninputHwrfscnStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    * @return CPeninputHwrfscnStateHandlerHomophonicSelecting Return the 
          CPeninputHwrfscnStateHandlerHomophonicSelecting object pointer. 
    */
    static CPeninputHwrfscnStateHandlerHomophonicSelecting* NewL( 
        CPeninputHwrfscnStateManager& aStateManager );    
public:
    /**
    * Handle command
    *
    * @since Series 60 5.0
    */
    void HandleCommand( TInt aCmd, TUint8* aData );
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Series 60 5.0
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Series 60 5.0
    */
    void OnExit();
    
private:
    /**
    * The constructor
    
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    */
    CPeninputHwrfscnStateHandlerHomophonicSelecting( 
          CPeninputHwrfscnStateManager& aStateManager );
    
private:
    /**
    * Handle one candiate selected event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */
    void OnSelectedCandidatesL( const TDesC& aEventData );
    
    /**
    * Handle one predictive candiate selected event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */
    void OnSelectedPredictivesL( const TDesC& aEventData );              
    };

/**
* The PredictSelecting state class.
*
* @since Series 60 5.0
*/
class CPeninputHwrfscnStateHandlerPredictSelecting : public CPeninputHwrfscnStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    * @return CPeninputHwrfscnStateHandlerPredictSelecting Return the 
          CPeninputHwrfscnStateHandlerPredictSelecting object pointer. 
    */
    static CPeninputHwrfscnStateHandlerPredictSelecting* NewL( 
        CPeninputHwrfscnStateManager& aStateManager );    
public:
    /**
    * Handle command
    *
    * @since Series 60 5.0
    */
    void HandleCommand( TInt aCmd, TUint8* aData );
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the state entering action
    * 
    * @since Series 60 5.0
    */
    void OnEntry();
    
    /**
    * Handle the state exiting action
    *
    * @since Series 60 5.0
    */
    void OnExit();
    
private:
    /**
    * The constructor
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputHwrfscnStateManager.
    */
    CPeninputHwrfscnStateHandlerPredictSelecting( 
         CPeninputHwrfscnStateManager& aStateManager );
              
private:
    /**
    * Handle one predictive candiate selected event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */
    void OnSelectedPredictivesL( const TDesC& aEventData );              
    };
        
/**
* The full screen hwr state manager.
*
* @since Series 60 5.0
*/
class CPeninputHwrfscnStateManager : public CBase
    {
public:
    /**
    * The factory function
    *
    * @since Series 60 5.0
    * @param aLayout The layout reference
    * @return CPeninputHwrfscnStateManager* The newly created CPeninputHwrfscnStateManager object
    */
    static CPeninputHwrfscnStateManager* NewL( CPeninputHwrfscnLayout* aLayout );
    
    /**
    * The destructor
    *
    *@since Series 60 5.0
    */
    ~CPeninputHwrfscnStateManager();

public:    
    /**
    * The hwr states.
    *
    *@since Series 60 5.0
    */    
    enum TPeninputHwrfscnState
    {
    EPeninputHwrfscnStateStandby = 0,
    EPeninputHwrfscnStateWriting,
    EPeninputHwrfscnStateCandidateSelecting,
    EPeninputHwrfscnStateHomophonicSelecting,
    EPeninputHwrfscnStatePredictSelecting,
    EPeninputHwrfscnStateCount
    };    

public:
    /**
    * Handle the command
    *
    * @since Series 60 5.0
    * @param aCmd The command ID
    * @param aData The command data
    */
    void HandleCommand( TInt aCmd, TUint8* aData );
    
    /**
    * Handle the hwr box and canidate events
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aEventData The event data
    */
    void HandleEventL( TInt aEventType, const TDesC& aEventData );
    
    /**
    * Return the layout reference
    *
    * @since Series 60 5.0
    * CPeninputHwrfscnLayout& The hwr layout reference
    */
    CPeninputHwrfscnLayout& HwrLayout();
    
    /**
    * Set current state.
    * 
    * @since Series 60 5.0
    * @param aState The state to be set. @See TPeninputHwrfscnState.
    */    
    void SetState( TPeninputHwrfscnState aState );
    
private:
    /**
    * The constructor.
    *
    * @since Series 60 5.0
    * @param aLayout The full screen HWR layout pointer.
    */
    CPeninputHwrfscnStateManager( CPeninputHwrfscnLayout* aLayout );
    
private:
    /**
    * 2nd phase constructor.
    * 
    * @since Series 60 5.0
    */
    void ConstructL();
    
    /**
    * Find the current state object.
    *
    * @since Series 60 5.0
    * @param aState According to this state type, 
    *        we could find the corresponding state obect.
    */
    CPeninputHwrfscnStateHandlerBase* Find( TPeninputHwrfscnState );
    
private:
    /**
    * Pointer to the full screen hwr layout.
    * 
    * Not own.
    */
    CPeninputHwrfscnLayout* iLayout;
    
    /**
    * Store all states pointers.
    * Own.
    */
    CPeninputHwrfscnStateHandlerBase* iStateHandlers[EPeninputHwrfscnStateCount];
    
    /**
    * Store all states' types.
    * Own
    */
    TPeninputHwrfscnState iStates[EPeninputHwrfscnStateCount];
    
    /**
    * The pointer to current state.
    * Own
    */    
    CPeninputHwrfscnStateHandlerBase* iCurrentStateHandler;
    
    /**
    * The current state type.
    *
    */
    TPeninputHwrfscnState iCurrentState;
    };

#endif    //__PENINPUTHWRFSCNSTATEMANAGER_H__