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
* Description:  CPeninputAnim & CPeninputAnimDll from peninputanim.h
*
*/



#ifndef C_CPENINPUTANIMDLL_H
#define C_CPENINPUTANIMDLL_H


#include <e32base.h>
#include <w32adll.h>    // for CSpriteAnim and CAnimDll
#include <e32msgqueue.h>
#include <e32property.h>
#include <AknCapServerClient.h>

#include "peninputmsgqueue.h"

class CSubscriber;
#include "peninputcmdparamext.h"
/**
 * Animation DLL creation class.
 *
 *
 * @since S60 v4.0
 */
class CPeninputAnimDll : public CAnimDll 
    {
public:
    //from base class CAnimDll

    /**
     * From CAnimDll
     * Create instance
     *
     * @since S60 v4.0
     * @param aType Identifies an animation class to be constructed
     */
    CAnim *CreateInstanceL(TInt aType);    
    };
//end of class CPeninputAnimDll

//Advanced Tactile feedback REQ417-47932
struct TTactileControlInfo 
	{
	TInt iId;
	TRect iArea;
	TInt iTactileType;
	
	TTactileControlInfo(TInt aId, TRect aArea, TInt aTactileType)
		{
		iId = aId;
		iArea = aArea;
		iTactileType = aTactileType;
		}
	};

class MTactileFeedbackServer;

// Modify for bug ETMA-7X2C5Y begin
class CPenPointerEventSuppressor;
// Modify for bug ETMA-7X2C5Y end
/**
 * CPeninputAnim
 *
 * CSpriteAnim derived class handling the semi-transparent UI of the PENINPUT.
 *
 * @since S60 v4.0
 */
class CPeninputAnim : public CSpriteAnim
    {
public:
    // Constructors and destructor
    /** 
     * Constructor
     *
     * @since S60 v4.0
     */
    CPeninputAnim();
    /** 
     * Destructor. Frees all used resources. 
     *
     * @since S60 v4.0
     */
    virtual ~CPeninputAnim();

    // from base class MEventHandler
    
    /**
     * From MEventHandler. 
     * Handles the raw events received from the window server
     *
     * @since S60 v4.0
     * @param aRawEvent description of the event
     * @return ETrue if event was handled by this function, EFalse otherwise 
     */
    TBool OfferRawEvent(const TRawEvent &aRawEvent);

    //from base class CAnim
            
    /** 
     * From CAnim.
     * 2nd phrase constructor
     *
     * @since S60 v4.0
     * @param aArgs Construct data
     */
    void ConstructL(TAny *aArgs);
    
    /** 
     * From CAnim.
     * Main animation function called by the window server.
     *
     * @since S60 v4.0
     * @param aDateTime time. See CAnim::Animate
     */
    void Animate(TDateTime *aDateTime);

    /**
     * From CAnim. 
     * The window server calls this function in response to application 
     * call to the client side command function RAnim::Command()
     *
     * @since S60 v4.0
     * @param aOpCode command code
     * @param aArgs command data
     */
    void Command(TInt aOpcode, TAny *aArgs);
    
    /**
     * From CAnim. 
     * The window server calls this function in response to application call
     * to the client side command function RAnim::CommandReplyL(). 
     *
     * @since S60 v4.0
     * @param aOpCode command code
     * @param aArgs command data
     * @return KErrNone if command handled correctly.
     */
    TInt CommandReplyL(TInt aOpcode, TAny *aArgs);  

    /**
     * From CAnim. 
     */
    void HandleNotification(const TWsEvent& aEvent);
private:

    /**
     * Makes PENINPUT window visible
     *
     * @since S60 v4.0
     */
    void Activate();

    /**
     * Makes PENINPUT window invisible
     *
     * @since S60 v4.0
     */
    void Deactivate();      
  

    /**
     * Sets new sprite position.
     *
     * @since S60 v4.0
     * @param aNewPos The new sprite position
     * @return Current position
     */    
    TPoint SetPosition(const TPoint& aNewPos);

    /**
     * Update screen
     *
     * @since S60 v4.0
     * @param aRect The rect to be updated
     * @param aFullUpdate Flag tells whether whole sprite neeeds updating     
     */
    void UpdateArea(const TRect& aRect,TBool aFullUpdate);

    /**
     * Send event to pen input server. Usually it's buffered in the msg queue
     *
     * @since S60 v4.0
     * @param aRawEvent the event to be sent
     */        
    void SendRawEvent(const TRawEvent &aRawEvent);

    /**
     * Initial and start timer
     */
	void StartTimer();
	
    /**
     * Cancel timer
     */
	void StopTimer();	
	
    /**
     * Process raw event, if handled retrun ETrue, or EFalse
     *
     * @since S60 v4.0
     * @param aRawEvent the event to be processed
     * @return ETrue if the event is handled.
     */        
    TBool OnRawKeyEvent(const TRawEvent& aRawEvent);
    TBool OnRawButton1Down(const TRawEvent& aRawEvent);
    TBool OnRawButton1Up(const TRawEvent& aRawEvent);
    TBool OnRawPointerMove(const TRawEvent& aRawEvent);
    
    /**
     * Send all events in the  buffer to the pen input server, and empty event buffer.
     *
     * @since S60 v4.0
     */            
    TInt FlushRawEventBuffer();
    
    /**
     * Timer call back function for periodic timer.
     *
     * @since S60 v4.0
     * @param aPtr The data sends to callback function
     * @return KErrNone if no error.
     */    
    static TInt FlushTimerCallBack(TAny *aPtr);    
    
    /**
     * Save event to buffer. The event is sent to pen input server when 
     * buffer overflowed.
     *
     * @since S60 v4.0
     * @param aRawEvent the event to be saved
     */    
    void PostRawEvent(const TRawEvent& aRawEvent);    

#ifdef RD_TACTILE_FEEDBACK
	/**
     * Compare two TTactileControlInfo Item.
     * 
     * @TTactileControlInfo& aFirst
     * @const TTactileControlInfo& aSecond
     */
	static TBool MatchItemByControlID(const TTactileControlInfo& aFirst, const TTactileControlInfo& aSecond);
        
    /**
     * Compare two TTactileControlInfo Item.
     * 
     * @TTactileControlInfo& aFirst
     * @const TTactileControlInfo& aSecond
     */
	static TBool MatchItemByControlIDAndArea(const TTactileControlInfo& aFirst, const TTactileControlInfo& aSecond);    /**
     
	 * give tactile feedback
     *
     * @since S60 v4.0
     * @param aPos the event position. It's screen coordinator.
     */    
    void DoTactileFeedBack(const TPoint &aPos);
#endif // RD_TACTILE_FEEDBACK
    
    /**
     * Panic current client if its request without parameter
     *
     * @since S60 v4.0
     * @param aParam The param to be checked
     */        
    inline void PanicClientIfNoParam(TAny* aParams)
        {
        if(!aParams)
            {
	        iFunctions->Panic();
            }            
        }
        
#ifdef RD_TACTILE_FEEDBACK   
    // Advanced Tactile feedback REQ417-47932
    void GetFeedbackAreaDataL(RArray<TTactileControlInfo>& aTactileControlArray,
                              const RMessagePtr2* msg,
                              RArray<TRect>* aPrevRectArray = NULL);

#endif // RD_TACTILE_FEEDBACK  

    void SetDSAState(TBool aFlag);
    //For discreetPop
    static TInt DiscreetPopChangeNotification(TAny* aObj);
    void HandleDiscreetPopNotification();
    
    void Refresh();
private:    // Data
    /**
     * Current state 
     */
    TBool iIsActive;

    /** 
     * Current position of the sprite 
     */
    TPoint iSpritePosition;

    /** 
     * Current sprite size
     */
    TSize iSpriteSize;

    /**
     * Flag tells whether pen input server needs the pointer event
     */
    TBool iIsPointerCaptured;
    
    /**
     * Flag tells whether the pen has been down inside the ui layout
     */
    TBool iIsPenDown;

    /**
     * Simulated event flag
     */        
    TBool iIsSimulatedEvent;

    /**
     * message buffer for pointer and key event.
     */    
    RMsgQueue<TRawEventBuffer> iMsgBufQueue;
    
    /**
     * Message buffer for key response. Key event must get the response immediately
     */   
    RMsgQueue<TBool> iKeyMsgResponseQueue;
    
    /**
     * Event buffer for pointer.
     */    
    TRawEventBuffer iEventBuffer;   
    
    /**
     * Semaphore for key handling.
     */        
    RSemaphore iResponseQueueSemaphore;
    
    /**
     * Timer to send pointer event to pen input server
     */    
    CPeriodic* iFlushTimer;      

#ifdef RD_TACTILE_FEEDBACK    
    /**
     * Flag for tactile feedback support
     */        
    TBool iTactileSupported;
     
      
    /**
     * Tactile Feedback instance
     */            
    MTactileFeedbackServer* iFeedback;
    
    /**
     * Array of the Control info
     * Advanced Tactile feedback REQ417-47932
     */
    RArray<TTactileControlInfo>	iTactileControl;
    
    /**
     * Array of the backup Control info
     * Advanced Tactile feedback REQ417-47932
     */
    RArray<TTactileControlInfo>	iBackupTactileControl;
    
    
#endif // RD_TACTILE_FEEDBACK    
    TBool iEnableSprite;
    TBool iDSAState;
	
    /**
     * Pointer event suppressor to improve tap accuracy
     */
    CPenPointerEventSuppressor* iPointerEventSuppressor;
    
    /**
     * Pointer number of primary touch device.
     * Any pointer event with different pointer number( TRawEvent.PointerNumber() ), 
     * is treated as multi-touch event and will be disabled. 
     */
    TUint8 iPrimaryTouchPtNum;

    /**
     * Flag to detect primary touch device at startup
     */
    TBool  iPrimaryTouchPtDetected;
    CSubscriber*    iDiscreetPopSubscriber;
    RProperty       iDiscreetPopProperty;
    TBool iDiscreetPoped;
    TBool iDiscreetPopedCapture;
    TRect iDiscreetPopArea;
    RAknUiServer iAknUiSrv; 
    TBool iDirty;
    };



class CSubscriber : public CActive
    {
public:
    CSubscriber(TCallBack aCallBack, RProperty& aProperty);
    ~CSubscriber();

public: // New functions
    void SubscribeL();
    void StopSubscribe();

private: // from CActive
    void RunL();
    void DoCancel();

private:
    TCallBack   iCallBack;
    RProperty&  iProperty;
    };
#endif //C_CPENINPUTANIMDLL_H
// End of File
