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


#ifndef __PENINPUTHWRFSCNLAYOUT_H__
#define __PENINPUTHWRFSCNLAYOUT_H__

//FEP INCLUDES
#include "peninputfullscrlayoutbase.h"

const TInt KControlButtonSize = 30;

//FORWARD CLASS
class CPeninputHwrfscnStateManager;
class CPeninputHwrfscnDataStore;
class CPeninputHwrfscnCandidatePanel;
class CPeninputHwrfscnControlBar;
class CPeninputHwrfscnWritingBox;

/**
*  Class CPeninputHwrfscnLayout.
*  Chinese full screen handwriting ui layout class.
*
*  
*  @since Series 60 5.0
*/

class CPeninputHwrfscnLayout : public CPenInputFullScreenLayoutBase
{
public:
    /**
    * The factory function
    *
    * @since Series 60 5.0
    * @param aLayoutOwner The MLayoutOwner
    * @param aInitData Init data for layout
    * @return Pointer to created CPeninputHwrfscnLayout object
    */
    static CPeninputHwrfscnLayout* NewL( MLayoutOwner* aLayoutOwner,
        const TAny* aInitData );

public://from CFepUiLayout
    /**
    * From CFepUiLayout.
    * Handle layout command event
    *
    * @since Series 60 5.0
    * @param aCmd Command Id.
    * @param aData Data for command.
    * @return Errors when return value small than 0.    
    */
    TInt HandleCommand( TInt aCmd,TUint8* aData );
    
    /**
    * From CFepUiLayout.
    * Inform layout the size changing event
    *
    * @since Series 60 5.0
    * @param pData The event data.
    * @return KErrNone if successfully size changing. 
    */
    TInt SizeChanged( const TAny* pData );
    
    /**
    * From CFepUiLayout.
    * Handle editor text coming
    * Called by animation dll when app editor text is coming.
    * If a ui layout has a editor area, it must implements this function to get the text
    *
    * @since Series 60 5.0
    * @param aData The fep input context field data
    * @return KErrNone is operation succeeded.    
    */
    TInt OnAppEditorTextComing( const TFepInputContextFieldData& aData );
    
    /**
    * Return the peninput ui type
    *
    * @since Series 60 5.0
    * @return The ui type @see TPenInputUiType
    */
    TInt PenInputType();

public://from MEventObserver;    
    /**
    * From MEventObserver, handle control event.
    *
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aCtrl The control who sends the event
    * @param aEventData The event data
    */
    void HandleControlEvent( TInt aEventType, 
        CFepUiBaseCtrl* aCtrl, const TDesC& aEventData );
    
    /**
    * Get the data stroe
    * 
    * @since Series 60 5.0
    * @return The CPeninputHwrfscnDataStore reference
    */
    CPeninputHwrfscnDataStore& DataStore();
    
    /**
    * Get the candidate panel reference
    * 
    * @since Series 60 5.0
    * @return The CPeninputHwrfscnCandidatePanel reference
    */
    CPeninputHwrfscnCandidatePanel& CandidatePanel();

    /**
    * Replace the char before the cursor in forground app with a new char.
    *
    * @since Series 60 5.0
    * @param aOldCharCode The old char need to be replaced
    * @param aNewCharCode The new char for replacing the old char
    */
    void Replace( const TDesC& aOldCharCode, 
        const TDesC& aNewCharCode );
    
    /**
    * Submit the buffer to layoutowner.
    *
    * @param aCharCode The buffer needed to be submitted.
    * @since Series 60 5.0
    */
    void SubmitStringToFep( const TDesC& aCharCode );

    void ClearScreen();
    
    CPeninputHwrfscnWritingBox& HwrfscnWritingBox();

protected:
    /**
     * From CFepUiLayout        
     * OnResourceChange. Inform layout the system resource changed.
     *
     * @since S60 v4.0
     * @param aType the event type
     * @return KErrNone if successfully size changing. 
     */
    TInt OnResourceChange(TInt aType);
private:
    /**
    * The panel type
    *
    * @since Series 60 5.0
    */
    enum TPeninputHwrfscnPanel
        {
        ENone = 0x0000,            
        EHwrWndBox = 0x0010,
        ECtrlBar = 0x0020,
        ECandidatePanel = 0x0040
        };
        
private:
   /**
    * The constructor
    *
    * @since Series 60 5.0
    * @param aLayoutOwner The MLayoutOwner pointer.
    */
    CPeninputHwrfscnLayout( MLayoutOwner* aLayoutOwner );
    
    /**
    * The destructor
    *
    * @since Series 60 5.0
    */
    ~CPeninputHwrfscnLayout();
    
    /**
    * 2nd phase constructor
    *
    * @since Series 60 5.0
    * @param aInitData The init data passed by layout engine.
    */
    void  ConstructL( const TAny* aInitData );
    
    /**
    * Handle raw key event. Call back function. Called when raw key event happens
    * Do nothing in default implementation
    *
    * @since S60 v5.0
    * @param aKeyEvent Key event
    * @return EFalse if the key has not been processed. Otherwise ETrue. 
    *      If key is not processed, it will be passed to CONE's control stack.
    */                
    TBool HandleRawKeyEventL( const TRawEvent& aKeyEvent );
    
    /**
	* From CPenInputFullScreenLayoutBase
	* 
	*/
	void PreHandleEvent( TEventType aType, 
	    const TRawEvent &aEvent );
	
	/**
    * Do someting before sending the events to background
    *
    * @since S60 v5.0
    */
    void BeforeSendingEventsToBackground();
    
    /**
     * From MFepLayoutBase        
     * OnDeActivate. Called by owner when the layout is going to be hidden
     *
     * @since Series 60 5.0
     */        
    void OnDeActivate();
    
    /**
    * Create the control bar
    *
    * @since Series 60 5.0
    */
    void CreateControlBarL();
    
    /**
    * Create the candidate panel
    *
    * @since Series 60 5.0
    */
    void  CreateCandidatePanelL();
    
    /**
    * Create the hwr box window
    *
    * @since Series 60 5.0
    */
    void  CreateHwrWindowL();
    
    /**
    * Create the state manager
    *
    * @since Series 60 5.0
    */
    void CreateStateManagerL();
    
    /**
    * Handle control bar events
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aCtrl The control who sends the event
    * @param aEventData The event data     
    */
    void HandleControlBarEventL( TInt aEventType, 
        CFepUiBaseCtrl* aCtrl,
        const TDesC& aEventData );
    
    /**
    * Handle function list events
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aCtrl The control who sends the event
    * @param aEventData The event data    
    */    
    void HandleFunctionListEventL( TInt aEventType, 
        CFepUiBaseCtrl* aCtrl, 
        const TDesC& aEventData );
    
    /**
    * Handle the Hwr Box events and candiate list events.
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aEventData The event data
    */
    void HandleHwrWndEventL( TInt aEventType, const TDesC& aEventData );
    
    /**
    * Handle the candidate list events.
    *
    * @since Series 60 5.0
    * @param aEventType The event type
    * @param aEventData The event data    
    */
    void HandleCandidateWndEventL( TInt aEventType, 
        const TDesC& aEventData );
    
    /**
    * Notify the candidate and control bar the outside window events.
    *
    * @since Series 60 5.0
    * @param aPanel Which panel need to be hidden
    */
    void NotifyOutsideEventL( TInt aPanel );
    
    /**
    * Notify the layout owner the primary has changed.
    *
    * @since Series 60 5.0
    * @param aPrimaryRange The range which will be changed to
    */
    void NotifyPrimaryRangeChanged( TInt aPrimaryRange );
    
private:    
    /**
    * The data store
    * Own
    */
    CPeninputHwrfscnDataStore* iDataStore;
    
    /**
    * The state manager
    * Own
    */
    CPeninputHwrfscnStateManager* iStateMgr;
    
    /**
    * The candidate panel
    * Own
    */
    CPeninputHwrfscnCandidatePanel* iCandidatePanel;
    
    /**
    * The control bar panel
    * Own
    */
    CPeninputHwrfscnControlBar* iControlBar;
    
    /**
    * The full screen hwr box
    * Own
    */
    CPeninputHwrfscnWritingBox* iFsHwrBox;
    
    /**
    * The resource Id
    * 
    */
    TInt iResId; 
};  
#endif //__PENINPUTHWRFSCNLAYOUT_H__
    
    
    
    
    
     
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    