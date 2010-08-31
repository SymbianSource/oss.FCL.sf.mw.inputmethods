/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef R_RPENINPUTSERVER_IMPL_H
#define R_RPENINPUTSERVER_IMPL_H

//  INCLUDES
#include <e32base.h>
#include <e32err.h>
#include <coemain.h>
#include "peninputcmdparam.h"
#include "peninputsrveventhandler.h"
#include <coecntrl.h>
#include <AknFepGlobalEnums.h>

class CPeninputServerObserver;
class MPenUiActivationHandler;
class CPenUiBackgroundWnd;
	  
NONSHARABLE_CLASS(CPeninputServerWaiter) : public CBase
    {
    public:
        static CPeninputServerWaiter* NewL();    
        void Start();
        void Stop(TInt aFlag);
        TBool IsStarted() 
            {
             return iWaitScheduler->IsStarted();
            }
        TInt Error() 
            {
             return iError;
            }
        ~CPeninputServerWaiter();
    protected:
    private:
        //CPeninputServerWaiter();
        void ConstructL();       
        TInt iError;
        CActiveSchedulerWait* iWaitScheduler;                         
    };

NONSHARABLE_CLASS(TUiNotificationHandler)
    {
    public:
        TUiNotificationHandler(MPenUiActivationHandler* aHandler,TInt aType);
    public:
        MPenUiActivationHandler* iHandler;
        TInt iType;
    };
    
/**
 * Client side interface to PeninputSingletonServer.
 *
 *
 * @lib peninputclient.lib
 * @since S60 v4.0 
 */
NONSHARABLE_CLASS(RPeninputServerImpl) : public RSessionBase
    {
public:

    static RPeninputServerImpl* NewL();
    // Constructors and destructor

  
    /**
    * Makes a connection to the server.
    *
    * @since S60 v4.0
    * @return KErrNone if connection was successful, otherwise one of the
    * general error codes.
    */
    //TInt Connect();

    /**
     * Returns the version information of the server.
     *
     * @since S60 v4.0
     * @return TVersion object containing the version of the server.
     */
    TVersion Version() const;


    /**
     * Closes the connection to the server. 
     *
     * @since S60 v4.0
     */
    void Close();

    /**
     * HandleCommandL, handle command from client
     *
     * @since S60 v4.0
     * @param aCmd The commandid
     * @param aBuf Command data
     * @return The command handling result code.KErrNone if operation sucessfully.
     */    
    TInt HandleCommand(TInt aCmd,const TDesC8& aBuf);
    
    /**
     * HandleCommandL, handle command from client
     *
     * @since S60 v4.0
     * @param aCmd The commandid
     * @return The command handling result code.KErrNone if operation sucessfully.
     */
    TInt HandleCommand(TInt aCmd);
    /**
     * HandleCommandL, handle command from client
     *
     * @since S60 v4.0
     * @param aCmd The commandid
     * @param aBuf Command data
     * @param aResult The result of layout handle
     * @return The command handling result code.KErrNone if operation sucessfully.
     */    
    TInt HandleCommand(TInt aCmd,const TDesC8& aBuf,TInt &aResult);
     
    /**
     * Get PENINPUT UI visibility
     *
     * @since S60 v4.0
     * @return ETrue if PENINPUT is visible, EFalse otherwise
     */
    TBool IsVisible();

    /**
     * Get PENINPUT UI dim status
     *
     * @since S60 v4.0
     * @return ETrue if PENINPUT is visible, EFalse otherwise
     */
    TBool IsDimmed();
    
    /**
     * Get the all support input mode
     *
     * @since S60 v4.0
     * @return input mode
     */
    TInt SupportInputMode();

    /**
     * Set UI layout id. It will create the UI by the Id
     *
     * @since S60 v4.0
     * @param aId The layout Id
     * @return The operation result code.KErrNone if successfully.
     */
    TInt SetUiLayoutId(TUid aId);
    
    /**
     * Set UI layout id
     *
     * @since S60 v4.0
     * @param aId The layout Id
     * @param aData The data associated with the layout creation
     * @return The operation result code
     */
    TInt SetUiLayoutId(TUid aId,const TDesC8& aData);
    
    /**
     * Set UI layout position
     *
     * @since S60 v4.0
     * @param aPos The layout position
     * @return The operation result code. KErrNone if successfully.
     */
    TInt SetUiLayoutPos(const TPoint& aPos);        

    /**
     * get UI layout position
     *
     * @since S60 v4.0
     * @return The layout position
     */
    TPoint UiLayoutPos();        

    /**
     * get UI layout size
     *
     * @since S60 v4.0
     * @return The layout size
     */
    TSize UiLayoutSize();
    
    /**
     * Called when resource changed, e.g, screen size changed.
     * To inform layout that resource change event.
     *
     * @since S60 v4.0
     * @param aData The data associated with size change event
     */
    void ResourceChanged(TInt aType);
    
    /**
     * Activate/Deactivate the layout
     *
     * @since S60 V4.0
     * @param aFlag The activation flag. ETrue if activated
     */
    void ActivateLayout(TBool aFlag);        

    /**
     * Get server event data.
     * When server oberver got a event notification, it calls this to get data
     *
     * @since S60 V4.0
     * @param aBuf buf to save the event data on return
     * @return result code. KErrNone if data got. If the length of aBuf is 
     *     smaller than server data, data is not copied, return value is the 
     *     required data length. KNullDesC is returned if no data in server side
     */
    TInt GetServerEventData(TDes& aBuf);

    /**
     * Get current pended server event number
     * 
     * @since S60 V4.0
     * @return The current pended event number in server side
     */
    TInt ServerEventDataNum();        
   
    /**
     * Add server observer. An observer will be notified when server has event
     * 
     * @since S60 V4.0
     * @param aObserver The server observer
     */
    void AddPeninputServerObserverL(
                                   MPeninputServerEventHandler* aObserver);


    /**
     * Remove all pen input observer
     * 
     * @since S60 V5.0
     */
    void RemovePeninputServerObserver();

    /**
     * Remove pen input observer
     * 
     * @since S60 V5.0
     * @param  aObserver The observer to be removed
     */
    void RemovePeninputServerObserver(MPeninputServerEventHandler* aObserver);
    
    /*
     * Set display mode. This must be called before any UI layout created.
     * Otherwise, no change happends.
     *
     * @since S60 V4.0
     * @param aMode The display mode
     * @param aMaskMode The display mode for mask device
     */       
    void SetDisplayMode(TDisplayMode aDisplayMode,
                                               TDisplayMode aMaskDisplayMode);
    /*
     * Clear server events, all pending events for current session is cleared.
     * It's used when current focus changed and previous server event is out of date.
     *
     * @since S60 V4.0
     */                  
    void ClearServerEvent();
    
    /**
     * Return requried Ime plugin list
     *
     * @since S60 V4.0
     * @param aLanguage The required supporting language
     * @param aPluginMode The required supporting mode
     * @param aImplmentationIds A reference to a client owned array
     *        which will be filled with IME plugin implementation.
     */
    void GetImePluginIdListL(TInt aLanguage, 
                                      TInt aPluginMode, 
                                      RArray<TInt>& aImplmentationIds);
                                     
    /**
     * Return all supporing touch input languages
     *
     * @since S60 V4.0
     * @param aLanguageLists A reference to a client owned array
     *        which will be filled with supporting languages.
     */
    void PenSupportLanguagesL(RArray<TInt>& aLanguageLists);                                     
    /**
     * Asynchronous request which must be issued by every client after
     * Connect(). When this request completes it is the clients responsibility
     * to react to the message. Only one
     * client can receive this notification. Client must call CancelMessageNotification()
     * before closing the session.
     *
     * @since S60 v4.0
     *@param aStatus Message Id of the observing active object
     */
    void RequestMessageNotification( TRequestStatus& aStatus );

    /**
     * Cancels the asynchronous request issued by RequestMessageNotification().
     *
     * @since S60 v4.0
     */
    void CancelMessageNotification();

    /**
     * Set foreground application session. Only foreground app session
     * can change the pen input UI attribute. Such command from other session 
     * will be discarded by server.
     *
     * @since S60 V4.0
     * @param aMustConnectFlag If ETrue, the set will always success. If EFalse,
     * operation depends on whether current app is the foreground application.
     */
    TBool SetForeground(TBool aMustConnectFlag);

    /**
     * Indicates current client lost foregound.
     *
     * @since S60 V5.0
     */    
    void LoseForeground();

    /**
     * Test whether this session is the forground application session
     *
     * @since S60 V4.0
     * @return ETrue if current session is foreground application session.
     */
    TBool IsForeground();
    
    /**
     * Add pen input UI activation observer. Only one handler can be added. If 
     * need to change the handler, the old handler must be first removed.
     *
     * @since S60 V4.0
     * @param aHandler The callback handler when pen input UI activated/deactivated
     * @param aType The pen ui type to be handled. The available type is defined in 
     * TPenInputUiType
     * @return KErrNone if sucessfully.
     */    
    TInt AddPenUiActivationHandler(MPenUiActivationHandler* aHandler,
                                                TInt aType);    
    
    /**
     * Remove all pen input UI activation observer
     *
     * @since S60 V4.0
     * 
     */    
    void RemovePenUiActivationHandler();    

    /**
     * Remove a pen input UI activation observer
     *
     * @since S60 V5.0
     */    
    
    void RemovePenUiActivationHandler(MPenUiActivationHandler *aHandler);
    
    
    void ActivatePeninputInNotesL(); 
    void DimUiLayout(TBool aFlag); 
    
    void BackgroudDefaultOri( TInt aOri );
    
    /**
     * Inform server that client has handled the UI notification event
     *
     * @since S60 V4.0
     */    
    void Continue();
    
    /*
     * To disable specified layouts at a time.
     *
     * @since S60 V4.0
     * @param aDisableLayout The disabled layouts.Can disable serveral layouts one time.
     */       
    void SetDisabledLayout( TInt aLayouts );
    
    /*
     * To get disable specified layouts at a time.
     *
     * @since S60 V4.0
     * @return The disabled layouts.
     */       
    TInt DisabledLayout();    
    
    /*
     * Update the application information.
     *
     * @since S60 V5.0
     * @param aInfo The application information.
     * @param aType The information type
     */     
    void UpdateAppInfo(const TDesC& aInfo, TPeninputAppInfo aType);    
    
    void HandleServerExit();
    void OnServerReady(TBool aFlag = ETrue);
    TBool IsForegroundSession();
    //void SetForegroundFlag(TBool aFlag);
    void FinalClose();    
    /*
     * Enable/disable the auto open feature. When auto open feature is on, Focus change event 
     * will open the pen UI automatically. The feature is on by default. When disabled, 
     * all editor in current app won't have the auto-open feature.
     *
     * @since S60 V5.0
     * @param aFlag ETrue to enable the feature, EFalse to disable the feature.
     */         
    void SetAutoOpen(TBool aFlag);
    
    /*
     * Get the auto-open feature flag.
     *
     * @since S60 V5.0
     * @return ETrue if auto-open feature is on, otherwise EFalse.
     */             
    TBool AutoOpen();
    
    /*
     * Set the application prefferred input mode. Application can set this mode to 
     * override the mode selected by Fep. 
     *
     * @since S60 V5.0
     */                 
    void SetPreferredUiMode(TPluginInputMode aMode);
    
    /*
     * Get the application prefferred input mode. 
     *
     * @since S60 V5.0
     * @return The application preferred mode. EPluginInputModeNone if app does set it.
     */                 
    TPluginInputMode PreferredUiMode();
    
    void SetGlobalNotes(TBool aFlag);
    
    void SetInternalPopUp(TBool aFlag);
    void SetEditorPriority(TInt aFlag);
    
    void ClearTouchUI();
    void SetResourceChange(TBool aFlag);
    
private:
        /**
     * Constructor
     * 
     * @since S60 v4.0
     */
    RPeninputServerImpl();
    
    void ConstructL(const TUid& aUid);
    
    TInt DoConnectL();

    void GetUiLayoutPosAndSize();
    
    void DimUiLayoutL(TBool aFlag);
    
private:    // New functions

    /**
     * Starts the server thread
     *
     * @since S60 v4.0
     *@return KErrNone if server was created successfully
     *        KErrAlreadyExists if the server is already running
     * otherwise one of the system wide error codes
     */
    TInt StartThreadL();

    void ReadIntArrayFromBufL(const TDesC8& aBuf, RArray<TInt>& aResult);

    void DoAddPenUiActivationHandler();
private:    // Data
    /**
     * server observer
     */
    CPeninputServerObserver* iObserver; 
	
    /**
     * Ui notification handler.
     */	
    //CPenUiNotificationHandler* iPenUiNotificationHandler;
    RArray<TUiNotificationHandler>  iPenUiNotificationHandler;
    //TUiNotificationHandler*  iPenUiNotificationHandler
    
    /**
     * The type which to be monitored by the client.
     */	    
    //TInt iRegistedType;
    /**
     * Indicates whether current client is the foreground application.
     */	        
    TBool iIsForegroundSession;
    /**
     * server active flag
     */
    TBool  iServerExit;
        
    TBool iLaunchServer;
    
    //CActiveSchedulerWait* iWaitScheduler;
    CPeninputServerWaiter * iWaitScheduler;
    
    TInt iCurPenUiType;    
    
    CPenUiBackgroundWnd* iBackgroundCtrl;
    
    TBool iAutoOpenFlag;
    
    TPluginInputMode iAppPrefferedUiMode; 
    
    TPoint iPosition;
    TSize  iSize;  
    TBool iGlobalNotes;
    TBool iInternalPopup;
    TInt iPriority; 
    TBool iResourceChange;
};


NONSHARABLE_CLASS(CPenInputSingletonClient) : public CCoeStatic
    {
    public:
    //static CPenInputSingletonClient* NewL(RPeninputServer* aServer);
    //private:
        CPenInputSingletonClient(const TUid& aUid,RPeninputServerImpl* aServer);
        ~CPenInputSingletonClient();
        //void ConstructL();
        RPeninputServerImpl* GetSingletonServer();
            
        TInt DecreaseRef() ;
        
        TBool IsValid();
        void SetInvalid();
        void ReplaceServer(RPeninputServerImpl* aServer);                
    private:
        RPeninputServerImpl* iServer;
        TInt iReferrence;
        TBool iIsValid;
    };    


NONSHARABLE_CLASS(CPenUiBackgroundWnd) : public CCoeControl
    {
public: // Constructors and destructor
    /**
     * C++ default constructor.
     * @param aAppUi AppUi of Eikon server.
     */
    static CPenUiBackgroundWnd* NewL(RWindowGroup& aWndGroup,TInt aBmpHandle);

private:
    /**
     * C++ default constructor.
     * @param aAppUi AppUi of Eikon server.
     */
    CPenUiBackgroundWnd(RWindowGroup& aWndGroup);
    /**
     * Two-phased constructor.
     */
    void ConstructL(TInt aBmpHandle);
public:    
    /**
     * Destructor.
     */
    ~CPenUiBackgroundWnd();

public: // New functions
    /**
     * Called from akncapserver appui to run fastswap.
     */

    /**
     * Show pen UI with priority.
     */
    void Show(const TRect& aRect, TBool aGlobalNotes, 
        TBool aInternal, TInt aPriority, TBool aResource);
    
    /**
     * Called from akncapserver appui to close fastswap.
     */
    void Hide();    
    
public: // Functions from base classes.

    /**
     * From CCoeControl    
     * Function where control's drawing is done.
     *
     * @param aRect Control's area.
     */
    void Draw( const TRect &aRect ) const;
protected: 
    void CreateBitmapL(TInt aBmpHandle);    
private: // Data
           
    RWindowGroup& iWndGroup;
    CFbsBitmap * iBitmap;
    CFbsBitmap *iBitmapCpoy;
    TInt iBmpHandle;
    };

//end of class RPeninputServerImpl 
#endif //R_RPENINPUTSERVER_IMPL_H

// End of File
