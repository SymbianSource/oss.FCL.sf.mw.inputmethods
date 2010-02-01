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
* Description: Implementation for chinese peninput finger hwr
*
*/

#ifndef C_PENINPUTFINGERHWRSTATEMANAGER_H
#define C_PENINPUTFINGERHWRSTATEMANAGER_H

//SYSTEM INCLUDES
#include <e32base.h>
#include <e32cmn.h>

//FORWARD CLASS
class CPeninputFingerHwrLayout;
class CPeninputFingerHwrDataStore;
class CPeninputFingerHwrStateManagerBase;
class CPeninputFingerHwrStateManagerCn;
class CPeninputFingerHwrStateManagerEn;
class CPeninputFingerHwrStateManagerNum;


/**
* Class MPeninputFingerHwrStateHandler
*
* The state handler interfaces 
*
* @lib peninputFingerHwr.lib
* @since S60 v5.0
*/
class MPeninputFingerHwrStateHandler
    {
public:    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData ) = 0;
    
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
class CPeninputFingerHwrStateHandlerBase :public CBase 
     ,public MPeninputFingerHwrStateHandler
    {
    
public:    
    /**
    * The handler base destructor
    *
    * @since Series 60 5.0
    */
    virtual ~CPeninputFingerHwrStateHandlerBase();
       
protected:
    /**
    * The constructor.
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrStateHandlerBase( CPeninputFingerHwrStateManagerBase& aStateManager );
        
protected:
    /**
    * The reference to state manager.
    *
    * @since Series 60 5.0
    */
    CPeninputFingerHwrStateManagerBase& iStateManager;
    
    /**
    * The data store reference
    *
    * @since Series 60 5.0
    */
    CPeninputFingerHwrDataStore& iDataStore;      
    };



/**
* The finger hwr state manager base class.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrStateManagerBase : public CBase
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
    *@since Series 60 5.0
    */
    virtual ~CPeninputFingerHwrStateManagerBase();  

public:
    /**
    * Handle the hwr box and canidate events
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aEventData The event data
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
    /**
    * Return the layout reference
    *
    * @since Series 60 5.0
    * CPeninputFingerHwrLayout& The hwr layout reference
    */
    CPeninputFingerHwrLayout& HwrLayout();
    
    /**
    * Is stanby state
    *
    * @since Series 60 5.0
    * TBool The standby state flag
    */    
    TBool IsStandbyState();
    
    /**
    * Set current state.
    * 
    * @since Series 60 5.0
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    virtual void SetState( TInt aState );     

    
protected:
    /**
    * 2nd phase constructor.
    * 
    * @since Series 60 5.0
    */
    void BaseConstructL();

    /**
    * The constructor.
    *
    * @since Series 60 5.0
    * @param aLayout The full screen HWR layout pointer.
    */
    CPeninputFingerHwrStateManagerBase( CPeninputFingerHwrLayout* aLayout );

    /**
    * Pointer to the full screen hwr layout.
    * 
    * Not own.
    */
    CPeninputFingerHwrLayout* iLayout;
    
    /**
    * The pointer to current state.
    * Own
    */    
    CPeninputFingerHwrStateHandlerBase* iCurrentStateHandler;
    
    /**
    * The current state.
    * Own
    */
    TInt iCurrentState;
    
    };

/**
* The finger hwr state manager for Chinese.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrStateManagerCn : public CPeninputFingerHwrStateManagerBase
    {
public:
    /**
    * The factory function
    *
    * @since Series 60 5.0
    * @param aLayout The layout reference
    * @return CPeninputFingerHwrStateManager* The newly created CPeninputFingerHwrStateManager object
    */
    static CPeninputFingerHwrStateManagerCn* NewL( CPeninputFingerHwrLayout* aLayout );
    
    /**
    * The destructor
    *
    *@since Series 60 5.0
    */
    ~CPeninputFingerHwrStateManagerCn();

public:    
    /**
    * The hwr states.
    *
    *@since Series 60 5.0
    */    
    enum TPeninputFingerHwrStateCn
    {
    EPeninputFingerHwrStateStandbyCn = EStateStandBy,
    EPeninputFingerHwrStateWritingCn,
    EPeninputFingerHwrStateCandidateSelectingCn,
    EPeninputFingerHwrStatePredictSelectingCn,
    EPeninputFingerHwrStateCountCn
    };    

public:
    /**
    * Handle the hwr box and canidate events
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aEventData The event data
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
    /**
    * Set current state.
    * 
    * @since Series 60 5.0
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    virtual void SetState( TInt aState );
    
private:
    /**
    * The constructor.
    *
    * @since Series 60 5.0
    * @param aLayout The full screen HWR layout pointer.
    */
    CPeninputFingerHwrStateManagerCn( CPeninputFingerHwrLayout* aLayout );
    
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
    CPeninputFingerHwrStateHandlerBase* Find( TPeninputFingerHwrStateCn );
    
private:    
    /**
    * Store all states pointers.
    * Own.
    */
    CPeninputFingerHwrStateHandlerBase* iStateHandlers[EPeninputFingerHwrStateCountCn];
    
    /**
    * Store all states' types.
    * Own
    */
    TPeninputFingerHwrStateCn iStates[EPeninputFingerHwrStateCountCn];    
    };


/**
* The finger hwr state manager for English.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrStateManagerEn : public CPeninputFingerHwrStateManagerBase
    {
public:
    /**
    * The factory function
    *
    * @since Series 60 5.0
    * @param aLayout The layout reference
    * @return CPeninputFingerHwrStateManagerEn* The newly created CPeninputFingerHwrStateManagerEn object
    */
    static CPeninputFingerHwrStateManagerEn* NewL( CPeninputFingerHwrLayout* aLayout );
    
    /**
    * The destructor
    *
    *@since Series 60 5.0
    */
    ~CPeninputFingerHwrStateManagerEn();
    
    /**
    * Set current state.
    * 
    * @since Series 60 5.0
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    virtual void SetState( TInt aState );
    
public:    
    /**
    * The hwr states.
    *
    *@since Series 60 5.0
    */    
    enum TPeninputFingerHwrStateEn
    {
    EPeninputFingerHwrStateStandbyEn = EStateStandBy,
    EPeninputFingerHwrStateWritingEn,
    EPeninputFingerHwrStateCandidateSelectingEn,
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    EPeninputFingerHwrStatePredictSelectingEn,
#endif
    EPeninputFingerHwrStateCountEn
    };    

public:
    
    /**
    * Handle the hwr box and canidate events
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aEventData The event data
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData );   
    
    /**
    * Set current state.
    * 
    * @since Series 60 5.0
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    void SetState( TPeninputFingerHwrStateEn aState );
    
private:
    /**
    * The constructor.
    *
    * @since Series 60 5.0
    * @param aLayout The full screen HWR layout pointer.
    */
    CPeninputFingerHwrStateManagerEn( CPeninputFingerHwrLayout* aLayout );
    
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
    CPeninputFingerHwrStateHandlerBase* Find( TPeninputFingerHwrStateEn );
    
private:    
    /**
    * Store all states pointers.
    * Own.
    */
    CPeninputFingerHwrStateHandlerBase* iStateHandlers[EPeninputFingerHwrStateCountEn];
    
    /**
    * Store all states' types.
    * Own
    */
    TPeninputFingerHwrStateEn iStates[EPeninputFingerHwrStateCountEn];
    
    };    

/**
* The finger hwr state manager for Number.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrStateManagerNum : public CPeninputFingerHwrStateManagerBase
    {
public:
    /**
    * The factory function
    *
    * @since Series 60 5.0
    * @param aLayout The layout reference
    * @return CPeninputFingerHwrStateManager* The newly created CPeninputFingerHwrStateManager object
    */
    static CPeninputFingerHwrStateManagerNum* NewL( CPeninputFingerHwrLayout* aLayout );
    
    /**
    * The destructor
    *
    *@since Series 60 5.0
    */
    ~CPeninputFingerHwrStateManagerNum();
    
    /**
    * Set current state.
    * 
    * @since Series 60 5.0
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    virtual void SetState( TInt aState );
    
public:    
    /**
    * The hwr states.
    *
    *@since Series 60 5.0
    */    
    enum TPeninputFingerHwrStateNum
    {
    EPeninputFingerHwrStateStandbyNum = EStateStandBy,
    EPeninputFingerHwrStateCountNum
    };    

public:
    
    /**
    * Handle the hwr box and canidate events
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aEventData The event data
    */
    virtual void HandleEventL( const TInt aEventType, const TDesC& aEventData );   
    
    /**
    * Set current state.
    * 
    * @since Series 60 5.0
    * @param aState The state to be set. @See TPeninputFingerHwrState.
    */    
    void SetState( TPeninputFingerHwrStateNum aState );
    
private:
    /**
    * The constructor.
    *
    * @since Series 60 5.0
    * @param aLayout The full screen HWR layout pointer.
    */
    CPeninputFingerHwrStateManagerNum( CPeninputFingerHwrLayout* aLayout );
    
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
    CPeninputFingerHwrStateHandlerBase* Find( TPeninputFingerHwrStateNum );
    
private:    
    /**
    * Store all states pointers.
    * Own.
    */
    CPeninputFingerHwrStateHandlerBase* iStateHandlers[EPeninputFingerHwrStateCountNum];
    
    /**
    * Store all states' types.
    * Own
    */
    TPeninputFingerHwrStateNum iStates[EPeninputFingerHwrStateCountNum];

    };


/**
* The Standby state class.
*
* @since Series 60 5.0
*/    
class CPeninputFingerHwrChineseStateStandby : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerStandby* Return the 
          CPeninputFingerHwrStateHandlerStandby object pointer. 
    */
    static CPeninputFingerHwrChineseStateStandby* NewL( 
        CPeninputFingerHwrStateManagerCn& aStateManager );
public:
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrChineseStateStandby( 
         CPeninputFingerHwrStateManagerCn& aStateManager );
            
    };


/**
* The Writing state class.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrChineseStateWriting : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerWriting* Return the 
          CPeninputFingerHwrStateHandlerWriting object pointer. 
    */
    static CPeninputFingerHwrChineseStateWriting* NewL( 
        CPeninputFingerHwrStateManagerCn& aStateManager );    
public:
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrChineseStateWriting( CPeninputFingerHwrStateManagerCn& aStateManager );
    
private:
    /**
    * Handle the end stroke event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnEndStrokeL( const TDesC& aEventData );

    /**
    * Handle the end writing event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnEndWritingL( const TDesC& aEventData );
    
    /**
    * Handle the backspace event
    *
    * @since Series 60 5.0
    */
    void OnClickBackSpaceL();
    
    /**
    * Handle the candidate selection event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnSelectedCandidatesL( const TDesC& aEventData );
    
    /**
    * Handle the predictives selection event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnSelectedPredictivesL( const TDesC& aEventData );
    };

/**
* The CandidateSelecting state class for Chinese.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrChineseStateCandidateSelecting : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerCandidateSelecting Return the 
          CPeninputFingerHwrStateHandlerCandidateSelecting object pointer. 
    */
    static CPeninputFingerHwrChineseStateCandidateSelecting* NewL( 
        CPeninputFingerHwrStateManagerCn& aStateManager );    
public:
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrChineseStateCandidateSelecting( 
              CPeninputFingerHwrStateManagerCn& aStateManager );
    
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
    
    /**
    * Handle backspace events
    *
    * @since Series 60 5.0
    */
    void OnClickBackSpaceL();
    };


/**
* The PredictSelecting state class for Chinese.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrChineseStatePredictSelecting : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerPredictSelecting Return the 
          CPeninputFingerHwrStateHandlerPredictSelecting object pointer. 
    */
    static CPeninputFingerHwrChineseStatePredictSelecting* NewL( 
        CPeninputFingerHwrStateManagerCn& aStateManager );    
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
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrChineseStatePredictSelecting( 
         CPeninputFingerHwrStateManagerCn& aStateManager );
              
private:
    /**
    * Handle one Candidate selected event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */
    void OnSelectedCandidatesL(const TDesC& aEventData);

    /**
    * Handle one predictive candiate selected event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */
    void OnSelectedPredictivesL( const TDesC& aEventData );
    
    /**
    * Handle one backspace event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */    
    void OnClickBackSpaceL();              
    };


/**
* The Standby state class for English.
*
* @since Series 60 5.0
*/    
class CPeninputFingerHwrEnglishStateStandby : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
     * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerStandby* Return the 
          CPeninputFingerHwrStateHandlerStandby object pointer. 
    */
    static CPeninputFingerHwrEnglishStateStandby* NewL( 
        CPeninputFingerHwrStateManagerEn& aStateManager );
public:
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrEnglishStateStandby( 
         CPeninputFingerHwrStateManagerEn& aStateManager );
            
    };


/**
* The Writing state class for English.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrEnglishStateWriting : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerWriting* Return the 
          CPeninputFingerHwrStateHandlerWriting object pointer. 
    */
    static CPeninputFingerHwrEnglishStateWriting* NewL( 
        CPeninputFingerHwrStateManagerEn& aStateManager );    
public:
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrEnglishStateWriting( CPeninputFingerHwrStateManagerEn& aStateManager );

    /**
    * Handle the end stroke event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnEndStrokeL( const TDesC& aEventData );

    /**
    * Handle the end writing event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnEndWritingL( const TDesC& aEventData );
    
    /**
    * Handle the end writing event
    *
    * @since Series 60 5.0
    */
    void OnClickBackSpaceL();
    
    /**
    * Handle the candidates selection event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */    
    void OnSelectedCandidatesL( const TDesC& aEventData );

    /**
    * Handle the predictive selection event
    *
    * @since Series 60 5.0
    * @param aEventData The trace data
    */
    void OnSelectedPredictivesL( const TDesC& aEventData );
           
    };

/**
* The CandidateSelecting state class for English.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrEnglishStateCandidateSelecting : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerCandidateSelecting Return the 
          CPeninputFingerHwrStateHandlerCandidateSelecting object pointer. 
    */
    static CPeninputFingerHwrEnglishStateCandidateSelecting* NewL( 
        CPeninputFingerHwrStateManagerEn& aStateManager );    
public:
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrEnglishStateCandidateSelecting( 
              CPeninputFingerHwrStateManagerEn& aStateManager );
    
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
    void OnSelectedPredictivesL( const TDesC& aEventData );
        
    void OnClickBackSpaceL();
    
    };


#ifdef RD_INTELLIGENT_TEXT_INPUT
/**
* The PredictSelecting state class for English.
*
* @since Series 60 5.0
*/
class CPeninputFingerHwrEnglishStatePredictSelecting : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
    * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerPredictSelecting Return the 
          CPeninputFingerHwrStateHandlerPredictSelecting object pointer. 
    */
    static CPeninputFingerHwrEnglishStatePredictSelecting* NewL( 
        CPeninputFingerHwrStateManagerEn& aStateManager );    
public:
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrEnglishStatePredictSelecting( 
         CPeninputFingerHwrStateManagerEn& aStateManager );
              
private:
    void OnSelectedCandidatesL( const TDesC& aEventData );


    /**
    * Handle one predictive candiate selected event
    * 
    * @since Series 60 5.0
    * @param aEventData The selected candidate
    */
    void OnSelectedPredictivesL( const TDesC& aEventData );
    
    void OnClickBackSpaceL();            
    };
#endif // RD_INTELLIGENT_TEXT_INPUT    
    
    
/**
* The Standby state class for number.
*
* @since Series 60 5.0
*/    
class CPeninputFingerHwrNumericStateStandby : public CPeninputFingerHwrStateHandlerBase
    {
public:
    /**
     * Factory function
    *
    * @since Series 60 5.0
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    * @return CPeninputFingerHwrStateHandlerStandby* Return the 
          CPeninputFingerHwrStateHandlerStandby object pointer. 
    */
    static CPeninputFingerHwrNumericStateStandby* NewL( 
        CPeninputFingerHwrStateManagerNum& aStateManager );
public:
    
    /**
    * Handle state event
    *
    * @since Series 60 5.0
    */
    void HandleEventL( const TInt aEventType, const TDesC& aEventData );
    
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
    * @param aStateManager The reference to state manager. @see CPeninputFingerHwrStateManager.
    */
    CPeninputFingerHwrNumericStateStandby( 
         CPeninputFingerHwrStateManagerNum& aStateManager );
            
    };    

#endif    //C_PENINPUTFINGERHWRSTATEMANAGER_H