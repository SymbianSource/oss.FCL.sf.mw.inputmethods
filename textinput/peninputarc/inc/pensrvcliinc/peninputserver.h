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
* Description:  Defination for peninput server
*
*/


#ifndef C_CPENINPUTSERVER_H
#define C_CPENINPUTSERVER_H

//  INCLUDES
#include <e32base.h>
#include <w32std.h>
#include <e32msgqueue.h>
#include <peninputlayoutplugininterface.h>
#include <peninputlayouteng.h>
#include "peninputmsgqueue.h"
#include <e32property.h>
#include <centralrepository.h>

#include "peninputcmdparamext.h"

// the maximum count of TUpdateArea that area pool can hold
const TInt KMaxRectNumInBuf = 100;
class CCoeControl;
class CPeninputAnimObj;
class CPeninputUiLayoutOwner;
class CPenUiWndCtrl;
class CInternalBkCtrl;
class CKeyRotator;
class CPenInputCrpServiceClient;
// panic code
enum TPeninputServerPanic
    {
    EBadRequest = -2000,
    EBadDescriptor,
    EMainSchedulerError,
    ESvrCreateServer,
    ESvrStartServer,
    ECreateTrapCleanup,
    ENotImplementedYet,
    ESessionTimeout,
    ENullSession
    };

//#define __LOG_WNDGROU__

// ==================== GLOBAL FUNCTIONS ====================

/** 
 * Function to panic the server
 *
 * @since S60 ?S60_version
 * @param aPanic The panic id
 */
GLREF_C void PanicServer(TPeninputServerPanic aPanic );


// CONSTANTS

// FORWARD DECLARATIONS
class CPeninputServerSession;
class RMessage;
class CEventQueue;
class CPtiEngine;
class CAknLayoutChangeWatcher;
// DESCRIPTION
/**
 * Class MRawEventHandler
 * Raw event handler interface
 *
 * @since S60 v4.0
 */

class MRawEventHandler
    {
public:
    /**
     * Handle event. Handle raw event from pen input animation
     *
     * @since S60 v4.0
     * @param aEvent The event to be processed
     */                
    virtual void HandleRawEventL(const TRawEvent& aEvent) = 0 ;
    };
    
    
// DESCRIPTION
/**
 * The server class; an active object.
 *
 * Contains an instance of RServer; a handle to the kernel server representation which is used 
 * to receive messages. 
 *
 * @since S60 v4.0
 */

NONSHARABLE_CLASS(CPeninputServer) : public CServer2, MRawEventHandler
{
public:
    // reasons for server panic
    // Constructors and destructor
    /** 
     * Epoc constructor. Leaves the created object on cleanup stack.
     *
     * @since S60 v4.0
     */
    static CPeninputServer* NewL();    
    
    /**
     * Destructor
     *
     * @since S60 v4.0
     */
    virtual ~CPeninputServer();
 
    /**
     * Increase number of sessions
     *
     * @since S60 v4.0
     */
    void IncreaseSessionCount();

    /**
     * Decrease number of sessions
     *
     * @since S60 v4.0
     * @param aSession The session to be closed.
     */
    void DecreaseSessionCount(CPeninputServerSession* aSession);

    /**
     * Handle client command
     *
     * @since S60 v4.0
     * @param aMessage The client messasge
     * @return Operation result code
     */
    TInt HandleCommandL(const RMessage2& aMessage);
    
    /**
     * Handle client message
     *
     * @since S60 v4.0
     * @param aMessage The client messasge
     * @return KErrNone if operation successfully. Otherwise any operation 
     *                  specified error
     */
    TInt HandleMessageL(const RMessage2& aMessage);
    
    /**
     * Signal server obserer
     *
     * @since S60 v4.0
     * @param aEventId The event id
     * @param aEventData The event data
     */
    void SignalObserverL(TInt aEventId , const TDesC& aEventData);

    /**
     * Create ui layout
     *
     * @since S60 v4.0
     * @param aLayoutId The layout id
     * @return ETrue if creation successfully. Otherwise EFalse
     */
    TBool SetLayoutId(TInt aLayoutId);

    /**
     * Handling client sieze change event
     *
     * @since S60 v4.0
     */
    void SizeChanged();
    
    /**
     * Change display mode
     *
     * @since S60 v4.0
     * @param aMode The new display mode
     * @param aMaskMode The new display mode for mask device
     */    
    void ChangeDisplayModeL(TDisplayMode aMode,TDisplayMode aMaskMode);
    
    /**
     * Timer call back for CPeriod
     *
     * @since S60 v4.0
     * @param aPtr The data passed to callback function
     * @return The processed result. Always ETrue, not used here.
     */  
    static TInt FlushTimerCallBack(TAny *aPtr);    

    /**
     * Flush the window session. This causes all bufferd command sent to pen 
     * input animation.
     *
     * @since S60 v4.0     
     */      
     void FlushWsSession();
     
    /**
     * low priority construction.
     *
     * @since S60 v4.0     
     */           
    void DoIdleConstructL();
    
    /**
     * Handle application resource change
     *
     * @since S60 v4.0     
     * @param aType The event type
     */               
    void HandleResourceChange(TInt aType);
    
    /**
     * Handle property change
     *
     * @since S60 v4.0     
     * @param aData The event data
     */                   
    static TInt PropertyChangeCallBack(TAny* aData);
    
    /**
     * Hide layout temporary
     *
     * @since S60 v4.0     
     * @param aData The event data
     */                       
    void HideLayoutTemporaryL();    
    void PrepareExit();
    
    /**
     * Set new disabled layouts of current session
     *
     * @since S60 v4.0     
     * @return KErrNone if everything ok.
     */         
    TInt SetDisabledLayouts( TInt aDisabledLayouts );
    
    /**
     * Get new disabled layouts of current TProcessId
     *
     * @since S60 v4.0     
     * @return KErrNone if everything ok.
     */  
    TInt GetProcessLayouts( const TProcessId& aProcessId, TInt& aLayouts );
  
    /**
     * Set new disabled layouts of current TProcessId
     *
     * @since S60 v4.0     
     * @return KErrNone if everything ok.
     */  
    TInt SetProcessLayouts( const TProcessId& aProcessId, TInt aLayouts);
    
    /**
     * Delete new disabled layouts of current TProcessId
     *
     * @since S60 v4.0     
     * @return KErrNone if everything ok.
     */  
    void DelProcessLayouts( const TProcessId& aProcessId );
    //from base class CServer2

    /** 
     * From CServer2
     * Create sesssion
     *
     * @since S60 v4.0
     * @param aVersion server version
     * @param aMessage Client message
     * @return a CSharableSession instance
     */
    CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
    
public: // From MRawEventHandler
    /**
     * From MRawEventHandler
     * handle raw event
     *
     * @since S60 v4.0
     * @param aEvent The event to be processed.
     */    
    void HandleRawEventL(const TRawEvent& aEvent);

public: // handle layoutowner event
    /** 
     * Update the UI area
     * @since S60 v4.0
     * @param aRect The rect to be updated
     * @param aFullUpdate Full update flag
     * @param aImmedFlag Flags indicates immediately update
     */
    void UpdateArea(const TRect& aRect,TBool aFullUpdate, TBool aImmedFlag );

    /**
     * Set layout position
     *
     * @since S60 v4.0
     * @param aNewPos The layout position
     */
    TPoint SetPosition(const TPoint& aNewPos);

    /**
     * Get bitmap context
     *
     * @since S60 v4.0
     * @return The bitmap context
     */
    CBitmapContext* BitmapContext();

    /**
     * Get bitmap device
     *
     * @since S60 v4.0
     * @return The bitmap device
     */
    CFbsBitmapDevice* BitmapDevice();

    /**
     * Get mask bitmap device
     *
     * @since S60 v4.0
     * @return The mask bitmap device
     */
    CFbsBitmapDevice* MaskBmpDevice();

    /**
     * Show or hide sprite
     *
     * @since S60 v4.0
     * @param aHideFlag ETrue if hide sprite, otherwise show.
     */
    void Hide(TBool aHideFlag=ETrue);

    /**
     * Signal owner that there is an event
     *
     * @since S60 v4.0     
     * @param aEventType The event type
     * @param aEventData The event data
     */        
    void SignalOwner(TInt aEventType, const TDesC& aEventData);

    /**
     * Handle layout size change event
     *
     * @since S60 v4.0
     * @param aBut The character buffer
     * @param aCtrlId The control which sends the data
     */
    TInt LayoutSizeChanged(const TSize& aSize, TBool aNeedUpdate = ETrue);
    
    /**
     * Get layout position and it's rect.
     * The position is relative to it's parent window or screen.
     * The size includes all invisible layout area.
     *
     * @since S60 v4.0     
     * @return layout position.
     */       
    TRect LayoutRect();    
    
    void ExecutePendingAnimCmd();
  
    void HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination);    
    
    TBool AnimOpNeedWaiting(TInt aSignalCode);

    TInt HandleAppInfoChange(CPeninputServerSession* aSession,
                            const TDesC& aAppInfo, TPeninputAppInfo aType) const;
protected:  // New functions

    /**
     * Default constructor.
     *
     * @since S60 v4.0
     * @param aPriority priority of the server
     */
    CPeninputServer( TInt aPriority );


    //from base class CServer2
    /**
     * From CServer.
     *Handles possible leaves in 
     *
     * @since S60 v4.0
     * @param aError leave code of ServiceL()
     * @return error code after error handling
     */
    TInt RunError( TInt aError );

private:    // New functions
                   
    /**
     * Second phase constructror.
     *
     * @since S60 v4.0
     */
    void ConstructL( );

    /**
     * Constructs the sprite anim ie. PENINPUT UI which is a window server plugin
     *
     * @since S60 v4.0
     */
    void ConstructSpriteL();
    

    /**
     * Constructs the sprite anim ie. PENINPUT UI which is a window server plugin
     *
     * @since S60 v4.0
     */
    void ConstructAnimL();    

    /**
     * Creates bimaps for the sprite.
     *
     * @since S60 v4.0
     * @param aBitmapSize size of the bitmap
     * @param aDisplayMode display mode of the bitmap
     * @return The initialized bitmap object (cleared to white)
     */
    CFbsBitmap* CreateBitmapL( const TSize& aBitmapSize, TDisplayMode aDisplayMode ) const;

    /**
     * Make peninput window visible
     *
     * @since S60 v4.0
     */
    void ActivateSprite();

    /**
     * Make peninput window visible
     *
     * @since S60 v5.0
     * @param aScrMode The required screen mode
     */
    void ActivateSprite(TInt aScrMode);

    /**
     * Show pen UI in global notes editor.
     *
     * @since S60 v5.0
     */
    void ActivateSpriteInGlobalNotesL();
    /**
     * Make peninput window invisible
     *
     * @since S60 v4.0
     * @param aImmediateHideFlag ETrue if UI is asked to be hidden immediately. Otherwise
     * hidding is wait until UI observer handles the notification
     */
    void DeactivateSprite(TBool aImmediateHideFlag = EFalse, TBool aRotation = EFalse);

    /**
     * Deactivate pen ui, but not close the UI.
     *
     * @since S60 v4.0
     * @param aWaitFlag Flag indicates whether operation should be waited until
     * observer processed.
     * @return ETrue if operation sucessfully.     
     */    
    TBool DeactivatePenUiLayout(TBool aWaitFlag);    

    /**
     * Deactivate pen ui, but not close the UI.
     *
     * @since S60 v4.0
     * @param aWaitFlag Flag indicates whether operation should be waited until
     * observer processed.
     * @return ETrue if operation sucessfully.     
     */    
    TBool DeactivateDimPenUiLayout(TBool aWaitFlag); 

    /**
     * Activate pen ui, but not redraw/open the UI.
     *
     * @since S60 v4.0
     * @param aWaitFlag Flag indicates whether operation should be waited until
     * observer processed.
     * @return ETrue if operation sucessfully.
     */    
    TBool ActivatePenUiLayout(TBool aWaitFlag, TBool aSkipDraw = EFalse);    
    
    /**
     * Create UI layout.
     *
     * @since S60 v4.0
     * @param aMessage The msg data from client
     * @return KErrNone if layout is created. Otherwise KErrNoLayout.
     */    
    TInt CreateLayoutL(const RMessage2& aMessage );

    /**
     * Clear the sprite
     *
     * @since S60 v4.0
     */        
    void ClearSpriteGc();
    
    /**
     * Draw sprite. It will draw the UI layout
     *
     * @since S60 v4.0
     */        
    void DrawSprite();
    /**
     * Get the session represents the foreground application
     *
     * @since S60 v4.0
     * @return The session represents foregound application
     */
    //CPeninputServerSession* ForegroundSession();
    
    /**
     * Get IME plugin implementation id list.
     *
     * @since S60 v4.0
     * @param aMessage The msg data from client
     * @return None.
     */    
    void GetImePluginIdListL(const RMessage2& aMessage);
    
    /**
     * Get touch input support language lists.
     *
     * @since S60 v4.0
     * @param aMessage The msg data from client
     * @return None.
     */    
    void GetPenSupportLanguagesL(const RMessage2& aMessage);
    
    /**
     * Set the fore ground ui handler which handles the UI notification.
     * 
     * @since S60 v4.0
     */
    void SetForeGroundUiHandlerL();
    
    /**
     * Get support input mode
     * 
     * @since S60 v4.0
     */
    TInt GetSupportModeL();

#ifdef RD_TACTILE_FEEDBACK    
    /**
     * Add feedback area.
     * 
     * @since S60 v4.0
     * @param aAreaData The area data
     */    
    void AddFeedbackArea(const TTactileFeedbackArea& aAreaData);
    
    /**
     * Remove feedback area.
     * 
     * @since S60 v4.0
     * @param aAreaData The area data     
     */        
    void RemoveFeedbackArea(const TTactileFeedbackArea& aAreaData);
    
    /**
     * Change feedback area.
     * 
     * @since S60 v4.0
     * @param aAreaData The area data     
     */        
    void ChangeFeedbackArea(const TTactileFeedbackArea& aAreaData);

    /**
     * Change feedback type.
     * 
     * @since S60 v4.0
     * @param aAreaData The area data     
     */        
    void ChangeFeedbackType(const TTactileFeedbackArea& aAreaData);    
    
    /**
     * Find the feedback area.
     * 
     * @since S60 v4.0
     * @param aArea The area data
     */                    
    TInt FindFeedbackArea(const TTactileFeedbackArea& aArea);
    
    /**
     * converter event data to feedback area data.
     * 
     * @since S60 v4.0
     * @param aAreaData The area data     
     */                
    inline TTactileFeedbackArea FeedbackData(const TDesC& aAreaData)
        {
        return *(reinterpret_cast<const TTactileFeedbackArea*>(aAreaData.Ptr()));
        }
#endif // RD_TACTILE_FEEDBACK

    //bug fix for missing simulated key up event when UI is closed.
    void RecoverSimulatedKeyEventState(); 
    
    void RecoverButtonEventState();       

    /**
     * Add a new TUpdateArea to area pool, and make sure if area pool is full,
     * it's sent to sprite for updating
     * 
     * @since S60 v4.0
     * @param aArea The area data
     * @return None
     */
    void AddFlushArea(const TUpdateArea& aArea);    

    /**
     * Make sure if area pool is full, it's sent to sprite for updating
     * 
     * @since S60 v4.0
     * @return None     
     */
    void EnsureUpdateBuffer();

    void DimPenUiWnd(TBool aFlag);
    
    void CleanAll();
    
    void PackFlushArea();

    /**
     * Request to enable or disable window group change event
     * 
     * @since S60 v5.0
     * @param aMonitorFlag ETrue indicates to request events. Default value is ETrue
     */    
    void MonitorWsGroupEvent(TBool aMonitorFlag = ETrue);

    /**
     * Tests whether two sessions from same process.
     * 
     * @since S60 v5.0
     * @param aSession1 The session to be tested
     * @param aSession2 The session to be tested
     * @return ETrue if they are from same process, otherwise, EFalse.
     */                
    inline TBool IsSameProcess(CPeninputServerSession* aSession1,
                                        CPeninputServerSession* aSession2) const;
    
    /**
     * Tests whether a given session is foreground session.
     * 
     * @since S60 v5.0
     * @param aSession The session to be tested
     * @return ETrue if it's foreground session, otherwise, EFalse.
     */        
    inline TBool IsForegroundSession(CPeninputServerSession* aSession) const;
    
    /**
     * Set up the size of drawing device
     * 
     * @since S60 v5.0
     * @param aSize The new size
     */            
    void ResizeDrawingDevice(const TSize& aSize);
    
    void DimPenUiForMenu(TInt aPriority);
    void BackgroudDefaultOri( TInt aOri );
    void DimPenInternalUi(TBool aFlag,TInt aPriority);
    void DimPenUi();
    void CheckSessionValidL(CPeninputServerSession* aSession1,
                            CPeninputServerSession* aSession2) const;
    
public:    
    TInt DisabledByDSA(); 
    TInt DisabledByDSAFromAnim();   
    TInt DisabledByOrientation();
    
private:    // Data

    /** 
    /** 
     * Number of connected client sessions. 
     */
    TInt iSessionCount;


    /** 
     * Sprite member of iSprite containing bitmap and mask 
     */
    TSpriteMember iSpriteMember;
    
    /** 
     * Id of the client thread that activated PENINPUT last 
     */
    TInt iForegroundAppId;    

    /** 
     * Semaphore that make the key event handling to be critical section
     */
    RSemaphore iKeyEventSemaphore;
    /**
     * the sprite active flag
     */
    TBool iActive ; 
    
    /**
     * Display mode. Defualt value = EColor16MU
     */
    TDisplayMode iDispMode;
    
    /**
     * Display mode for mask device. By default it's the same as display mode
     */
    TDisplayMode iDispModeForMask;    
    
    /**
     * current layout id
     */
    TUid iLayoutId;   

    /** 
     * Current sprite size
     */
    TSize iSpriteSize;    
    
    /** 
     * Current position of the sprite 
     */
    TPoint iSpritePosition;
    /**
     * event queue for key response. Pen input server needs send the 
     * key event response immediately.
     */    
    RMsgQueue<TBool> iKeyMsgResponseQueue;
    
    /**
     * The rect to be updated.
     */
    TUpdateArea iFlushArea[KMaxRectNumInBuf];

    /**
     * Flag tells whether full rect updating needed.
     */ 
    TBool iFullUpdate;    

    /**
     * Window server animation client id.
     */    
    CPeninputServerSession* iForegroundSession;

    /**
     * Layout engine
     * Own
     */
    CFepLayoutEngine* iLayoutEng;

    /**
     * Current ui layout
     */
    MPenUiLayoutBase* iUiLayout;
    
    /** 
     * Bitmap device for sprite 
     * Own
     **/
    CFbsBitmapDevice *iBitmapDevice;
    /** 
     * Bitmap device for spite mask 
     * Own
     */
    CFbsBitmapDevice *iMaskBitmapDevice;

    /** 
     * Graphics context for sprite 
     * Own
     */
    CFbsBitGc *iSpriteGc; 
    
    /** 
     * Window GC. Used for window based control to draw content.
     * Own
     */    
    CWindowGc* iWindowGc;
    
    /** 
     * Server application GC. It's used to keep the original window GC.
     * Not Own
     */    
    CWindowGc* iAppGc; 
    
    /** 
     * event queue for pointer event
     */    
    CEventQueue* iEventBufferQueue;
        
    /**
     * Timer to flush the window session command
     */
    CPeriodic* iFlushTimer;   
    
    /**
     * Ptiengine
     */
    CPtiEngine* iPtiEngine;
    
    /**
     * Previous ui layout  -- Not used now
     */
    //MPenUiLayoutBase* iPrevUiLayout;

    CPeninputUiLayoutOwner* iLayoutOwner;	    
    /**
     * Previous layout id
     */
    TUid iPrevLayoutId;   
    
    /**
     * Flag indicating whether layout is active when Hide is called. It's used
     * to restore the state.
     */
    TBool iPrevActive;    
    /**
     * Watcher to monitor the screen dynamically changing.
     */    
    CAknLayoutChangeWatcher* iHardwareLayoutChangeWatcher;       
    
    /**
     * Watcher to monitor the screen layout changing which is caused by touch pane
     */        
    CAknLayoutChangeWatcher* iSoftwareLayoutChangeWatcher; 
    
    /**
     * Foreground UI handler which will handle pen ui activattion/deactivation event.
     */            
    CPeninputServerSession* iForegroundUiHandler;
    
    /**
     * Ui handler list. See iForegroundUiHandler.
     */            
    RArray<CPeninputServerSession*> iUiHandlerArray;
    
    /**
     * Pen input animation object which managers different animation command
     */                
    CPeninputAnimObj* iAnimObj;												
    
    /**
     * Pen UI ready flag.
     */
    TInt iUILayoutReady;     
     
    /**
     * Flag tells whether tactile feedback is supported.
     */     
    TBool iSupportFeedback;
    
#ifdef RD_TACTILE_FEEDBACK     
    /**
     * Array of feedback area 
     */          
    RArray<TTactileFeedbackArea> iFeedbackAreaArray;       
#endif // RD_TACTILE_FEEDBACK    
    /**
     * Record the last key event simulated by pen UI
     */          
    TRawEvent iLastSimulatedKeyEvent;
    
    /**
     * The CCoeControl where the pen UI is bitblted.
     */              
    CPenUiWndCtrl* iPenUiCtrl;
    
    /**
     * The CCoeControl which acts as the parent of all child controls in the pen UI. 
     */              
    CInternalBkCtrl* iInternalBackgroundCtrl; 
    
    /**
     * Flag tells whether pen UI is shown in sprite or CCoeControl 
     */              
    TBool iUseWindowCtrl;
    
    /**
     * Flag tells whether current pen UI is opaque. 
     */              
    TBool iOpaqueUi;

    // TUpdateArea count in area pool
    TInt iCurRectNumInBuf;

    RArray<TProcessId> iClientProcess;
    RArray<TInt> iClientLayouts; 
    TBool iDimmed;
    /**
     * Flag tells whether currently app is globle notes app
     */          
    TBool iInGlobalNotesApp; 
    
    /**
     * Flag tells whether currently global notes is shown. This is used in Finger UI case, where finger UI is asked 
     * to dimmed instead of closed when global notes shown.
     */     
    TBool iInGlobalEditorState;
    
    /**
     * Record previous active window group which is not global notes. 
     */             
    TInt iPreNonGloebalNotesWndGrpId;
    
    /**
     * current global notes app id. 
     */              
    TInt iCurGlobalNotesId;
    
    /**
     * Flag tells whether cleanning needed durint destruction to avoid unnecessary clear
     */         
    TBool iNoNeedClean; 
    /**
     * Focused window group id in current screen
     */             
    TInt iCurScreenFocusedWndGrpId; 
    
    /**
     * Flag indicating the layout is being destroyed/
     */
    TBool  iDestroyingLayout;

    /**
     * Current screen mode
     */
    TInt iCurScrMode;    
    
    TInt iBackgroudDefaultOri;
    
    TBool iInGlobalNotesState;
    
    TBool iResourceChange;
    
    TRawEvent iLastRawEvent;
            
#ifdef __WINS__
    TBool iCloseServer;
#endif

#ifdef __LOG_WNDGROU__ 
    RFile iLogFile;
#endif  
    CPenInputCrpServiceClient* iCrpService;
    //TBool iUiLayoutChange;
    CRepository* iSensorRepository;
    };

/**
 * Class CEventQueue
 * Event message queue. It's an active object.
 *
 * @since S60 v4.0
 */

class CEventQueue : public CActive
    {
public:
    /** 
     * factory constructor. 
     *
     * @since S60 v4.0
     * @param aHandler The event handler.
     * @param aName The event queue global name
     * @return The created messge queue object
     */   
    static CEventQueue* NewL(MRawEventHandler* aHandler,const TDesC& aName);
    /** 
     * desstructor. 
     *
     * @since S60 v4.0
     */       
    ~CEventQueue();
    
    /** 
     * Request event.
     *
     * @since S60 v4.0
     */      
    void GetEvent();
    
protected:
    /** 
     * 2nd phase constructor.
     *
     * @since S60 v4.0
     * @aName The message queue name
     */      
     void ConstructL(const TDesC& aName);
    /** 
     * Default construcotr.
     *
     * @since S60 v4.0
     * @aHandler The event handler
     */         
    CEventQueue(MRawEventHandler* aHandler);
    
    //From CActive        
    /**
     * From CActive
     * Called when there is event in the queue
     *
     * @since S60 v4.0
     */
    void RunL();

    /**
     * From CActive
     * will be called if RunL leaves
     *
     * @since S60 v4.0
     * @param aError The error number
     */
    TInt RunError(TInt aError);

    /**
     * From CActive
     * will be called when Cancel is issued
     *
     * @since S60 v4.0
     */
    void DoCancel();

private:
    /**
     * Message buffer queue
     */    
    RMsgQueue<TRawEventBuffer> iMsgBufQueue;    
    
    /**
     * Event handler
     */       
    MRawEventHandler* iHandler;
    };


/**
 * CAknLayoutChangeWatcher
 * Watcher to monitor the screen dynamical changing.
 *
 * @since S60 v4.0
 */    
class CAknLayoutChangeWatcher : public CActive
    {
public:
    /** 
     * factory constructor. 
     *
     * @since S60 v4.0
     * @param aHandler The event handler.
     * @param aKey The property key
     * @return The created watcher
     */   
    static CAknLayoutChangeWatcher* NewL(CPeninputServer* aHandler, TUint aKey);
    
    /** 
     * desstructor. 
     *
     * @since S60 v4.0
     */       
    ~CAknLayoutChangeWatcher();
    
    /** 
     * Start monitoring.
     *
     * @since S60 v4.0
     */      
    void StartWatching();
    
protected:
    /** 
     * Default construcotr.
     *
     * @since S60 v4.0
     * @aHandler The event handler
     */         
    CAknLayoutChangeWatcher(CPeninputServer* aHandler,TUint aKey);
    
    //From CActive        
    /**
     * From CActive
     * Called when there is event in the queue
     *
     * @since S60 v4.0
     */
    void RunL();

    /**
     * From CActive
     * will be called if RunL leaves
     *
     * @since S60 v4.0
     * @param aError The error number
     */
    TInt RunError(TInt aError);

    /**
     * From CActive
     * will be called when Cancel is issued
     *
     * @since S60 v4.0
     */
    void DoCancel();

private:
    /**
     * Event handler. Pen input server shall handle the chaning event.
     */       
    CPeninputServer* iHandler;
    
    /**
     * The property to be monitored.
     */           
    RProperty iProperty;

    /**
     * The Ui layout state.
     */           
    TInt iLayoutState;

    /**
     * The orientation state.
     */           
    TInt iOrientationState;
    };
    
#endif //C_CPENINPUTSERVER_H

// End of File
