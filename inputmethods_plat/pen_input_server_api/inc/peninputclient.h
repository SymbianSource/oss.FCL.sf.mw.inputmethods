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
* Description:               This file contains classes of Peninput server client side.
*
*/















#ifndef R_RPENINPUTSERVER_H
#define R_RPENINPUTSERVER_H

//  INCLUDES
#include <e32base.h>
#include "peninputcmdparam.h"
#include <AknFepGlobalEnums.h>

class RPeninputServerImpl;
class MPeninputServerEventHandler;
class MPenUiActivationHandler;

/**
 * Client side interface to PeninputServer.
 *
 *
 * @lib peninputclient.lib
 * @since S60 v4.0 
 */
const TInt KErrLaunchingServer = -2000;   // indicates server is launching. Connect will be failed at this time
NONSHARABLE_CLASS(RPeninputServer)
    {
public:
    // Constructors and destructor

    /**
     * Constructor
     * 
     * @since S60 v4.0
     */
    IMPORT_C RPeninputServer();
   
    /**
    * Makes a connection to the server.
    *
    * @since S60 v4.0
    * @return KErrNone if connection was successful, otherwise one of the
    * general error codes.
    */
    IMPORT_C TInt Connect();

    /**
     * Returns the version information of the server.
     *
     * @since S60 v4.0
     * @return TVersion object containing the version of the server.
     */
    IMPORT_C TVersion Version() const;


    /**
     * Closes the connection to the server. 
     *
     * @since S60 v4.0
     */
    IMPORT_C void Close();

    /**
     * HandleCommandL, handle command from client
     *
     * @since S60 v4.0
     * @param aCmd The commandid
     * @param aBuf Command data
     * @return The command handling result code.KErrNone if operation sucessfully.
     */    
    IMPORT_C TInt HandleCommand(TInt aCmd,const TDesC8& aBuf);
    
    /**
     * HandleCommandL, handle command from client
     *
     * @since S60 v4.0
     * @param aCmd The commandid
     * @return The command handling result code.KErrNone if operation sucessfully.
     */
    IMPORT_C TInt HandleCommand(TInt aCmd);
    /**
     * HandleCommandL, handle command from client
     *
     * @since S60 v4.0
     * @param aCmd The commandid
     * @param aBuf Command data
     * @param aResult The result of layout handle
     * @return The command handling result code.KErrNone if operation sucessfully.
     */    
    IMPORT_C TInt HandleCommand(TInt aCmd,const TDesC8& aBuf,TInt &aResult);
     
    /**
     * Get PENINPUT UI visibility
     *
     * @since S60 v4.0
     * @return ETrue if PENINPUT is visible, EFalse otherwise
     */
    IMPORT_C TBool IsVisible();

    /**
     * Get PENINPUT UI dim status
     *
     * @since S60 v4.0
     * @return ETrue if PENINPUT is visible, EFalse otherwise
     */
    IMPORT_C TBool IsDimmed();
	
    /**
     * Get supported input mode
     *
     * @since S60 v4.0
     * @return input mode
     */	
	IMPORT_C TInt SupportInputMode();

    /**
     * Set UI layout id. It will create the UI by the Id
     *
     * @since S60 v4.0
     * @param aId The layout Id
     * @return The operation result code.KErrNone if successfully.
     */
    IMPORT_C TInt SetUiLayoutId(TUid aId);
    
    /**
     * Set UI layout id
     *
     * @since S60 v4.0
     * @param aId The layout Id
     * @param aData The data associated with the layout creation
     * @return The operation result code
     */
    IMPORT_C TInt SetUiLayoutId(TUid aId,const TDesC8& aData);
    
    /**
     * Set UI layout position
     *
     * @since S60 v4.0
     * @param aPos The layout position
     * @return The operation result code. KErrNone if successfully.
     */
    IMPORT_C TInt SetUiLayoutPos(const TPoint& aPos);        

    /**
     * get UI layout position
     *
     * @since S60 v4.0
     * @return The layout position
     */
    IMPORT_C TPoint UiLayoutPos();        
    
    /**
     * get UI layout size
     *
     * @since S60 v4.0
     * @return The layout size
     */    
    IMPORT_C TSize UiLayoutSize();
    
    /**
     * Called when resource changed, e.g, screen size changed.
     * To inform layout that resource change event.
     *
     * @since S60 v4.0
     * @param aData The data associated with size change event
     */
    IMPORT_C void ResourceChanged(TInt aType);
    
    /**
     * Activate/Deactivate the layout
     *
     * @since S60 V4.0
     * @param aFlag The activation flag. ETrue if activated
     */
    IMPORT_C void ActivateLayout(TBool aFlag);        

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
    IMPORT_C TInt GetServerEventData(TDes& aBuf);

    /**
     * Get current pended server event number
     * 
     * @since S60 V4.0
     * @return The current pended event number in server side
     */
    IMPORT_C TInt ServerEventDataNum();        
   
    /**
     * Add server observer. An observer will be notified when server has event
     * 
     * @since S60 V4.0
     * @param aObserver The server observer
     */
    IMPORT_C void AddPeninputServerObserverL(
                                   MPeninputServerEventHandler* aObserver);


    /**
     * Remove all pen input observer
     * 
     * @since S60 V4.0
     */
    IMPORT_C void RemovePeninputServerObserver();

    /**
     * Remove pen input observer
     * 
     * @since S60 V4.0
     * @param aObserver The server observer to be remvoed
     */
    IMPORT_C void RemovePeninputServerObserver(MPeninputServerEventHandler* aObserver);
    
    /*
     * Set display mode. This must be called before any UI layout created.
     * Otherwise, no change happends.
     *
     * @since S60 V4.0
     * @param aMode The display mode
     * @param aMaskMode The display mode for mask device
     */       
    IMPORT_C void SetDisplayMode(TDisplayMode aDisplayMode,
                                               TDisplayMode aMaskDisplayMode);
    /*
     * Clear server events, all pending events for current session is cleared.
     * It's used when current focus changed and previous server event is out of date.
     *
     * @since S60 V4.0
     */                  
    IMPORT_C void ClearServerEvent();
    
    /**
     * Return requried Ime plugin list
     *
     * @since S60 V4.0
     * @param aLanguage The required supporting language
     * @param aPluginMode The required supporting mode
     * @param aImplmentationIds A reference to a client owned array
     *        which will be filled with IME plugin implementation.
     */
    IMPORT_C void GetImePluginIdListL(TInt aLanguage, 
                                      TInt aPluginMode, 
                                      RArray<TInt>& aImplmentationIds);
                                     
    /**
     * Return all supporing touch input languages
     *
     * @since S60 V4.0
     * @param aLanguageLists A reference to a client owned array
     *        which will be filled with supporting languages.
     */
    IMPORT_C void PenSupportLanguagesL(RArray<TInt>& aLanguageLists);                                     
    
    /**
     * Set foreground application session. Only foreground app session
     * can change the pen input UI attribute. Such command from other session 
     * will be discarded by server.
     *
     * @since S60 V4.0
     * @param aMustConnectFlag If ETrue, the set will always success. If EFalse,
     * operation depends on whether current app is the foreground application.
     */
    IMPORT_C TBool SetForeground(TBool aMustConnectFlag);

    /**
     * Indicates current client lost foregound.
     *
     * @since S60 V5.0
     */    
    IMPORT_C void LoseForeground();

    /**
     * Test whether this session is the forground application session
     *
     * @since S60 V4.0
     * @return ETrue if current session is foreground application session.
     */
    IMPORT_C TBool IsForeground();
    
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
    IMPORT_C TInt AddPenUiActivationHandler(MPenUiActivationHandler* aHandler,
                                                TInt aType);
    
    /**
     * Remove all pen input UI activation observer
     *
     * @since S60 V4.0
     * 
     */    
    IMPORT_C void RemovePenUiActivationHandler();    

    /**
     * Remove a pen input UI activation observer
     *
     * @since S60 V5.0
     */        
    IMPORT_C void RemovePenUiActivationHandler(MPenUiActivationHandler *aHandler);
    
    
    IMPORT_C void ActivatePeninputInNotesL(); 

    /**
     * Dim the UI layout
     *
     * @since S60 V5.0
     * @param aFlag The dim flag. ETrue if going to dim the ui layout
     */      
    IMPORT_C void DimUiLayout(TBool aFlag); 
   
    /*
     * To disable specified layouts at a time.
     *
     * @since S60 V4.0
     * @param aDisableLayout The disabled layouts.Can disable serveral layouts one time.
     */       
    IMPORT_C void SetDisabledLayout( TInt aLayouts );
    
    /*
     * To get disable specified layouts at a time.
     *
     * @since S60 V4.0
     * @return The disabled layouts.
     */       
    IMPORT_C TInt DisabledLayout();    
    
    /*
     * Update the application information.
     *
     * @since S60 V5.0
     * @param aInfo The application information.
     * @param aType The information type
     */     
    IMPORT_C void UpdateAppInfo(const TDesC& aInfo, TPeninputAppInfo aType);    
    
    /*
     * Enable/disable the auto open feature. When auto open feature is on, Focus change event 
     * will open the pen UI automatically. The feature is on by default. When disabled, 
     * all editor in current app won't have the auto-open feature.
     *
     * @since S60 V5.0
     * @param aFlag ETrue to enable the feature, EFalse to disable the feature.
     */         
    IMPORT_C void SetAutoOpen(TBool aFlag);
    
    /*
     * Get the auto-open feature flag.
     *
     * @since S60 V5.0
     * @return ETrue if auto-open feature is on, otherwise EFalse.
     */             
    IMPORT_C TBool AutoOpen();
    
	IMPORT_C void BackgroudDefaultOri(TInt aOri);
	
    /*
     * Set the application prefferred input mode. Application can set this mode to 
     * override the mode selected by Fep. 
     *
     * @since S60 V5.0
     */                 
    IMPORT_C void SetPreferredUiMode(TPluginInputMode aMode);
    
    /*
     * Get the application prefferred input mode. 
     *
     * @since S60 V5.0
     * @return The application preferred mode. EPluginInputModeNone if app does set it.
     */                 
    IMPORT_C  TPluginInputMode PreferredUiMode();
    
    IMPORT_C void SetGlobalNotes(TBool aFlag);
    IMPORT_C void SetInternalPopUp(TBool aFlag);
    IMPORT_C void SetEditorPriority(TInt aFlag); 
    IMPORT_C void ClearTouchUI();
    IMPORT_C void SetResourceChange(TBool aFlag); 
private:    // Data
    /**
     * The singleton client to the input server
     */    		    
    RPeninputServerImpl* iSingletonServer;
};
//end of class RPeninputServer 
#endif //R_RPENINPUTSERVER_H

// End of File
