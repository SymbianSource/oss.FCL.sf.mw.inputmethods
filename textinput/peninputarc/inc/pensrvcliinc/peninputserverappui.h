/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Defination for peninput server application UI
*
*/


#ifndef PENINPUTSERVERAPPUI_H
#define PENINPUTSERVERAPPUI_H

// INCLUDES
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknappui.h>

// FORWARD DECLARATIONS
class CPeninputServer;


// CLASS DECLARATION

/**
* Class CPeninputServerAppUi
* Application UI class.
* 
*/
class CPeninputServerAppUi : public CAknAppUi
    {
public: // // Constructors and destructor

    /**
    * 2nd phase constructor.
    *
    * @since S60 v4.0        
    */
    void ConstructL();

    /**
    * Destructor.
    *
    * @since S60 v4.0            
    */      
    ~CPeninputServerAppUi();    

    /**
    * BackgroundTaskL. CIdle call back function
    *
    * @since S60 v4.0
    * @param aPtr The parameter passed in by CIdle.
    * @return ETrue if next step background task needed. Otherwise EFalse            
    */          
    static TInt BackgroundTaskL(TAny* aPtr);
    
    /**
    * Call back function. To exit the application after gets Exit command.
    *
    * @since S60 v4.0
    * @param aPtr The parameter passed in by CIdle.
    * @return ETrue. Will always return ETrue, which indicates we finished the task.
    */              
    static TInt ExiterCallback(TAny* aPtr);    
    //from base CAknAppUi
    /**
    * From CAknAppUi    
    * Handles a change to the application's resources which
    * are shared across the environment. 
    *
    * @param aType The type of resources that have changed. 
    */
    void HandleResourceChangeL(TInt aType); 

    void HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination);

    /**
    * From CAknAppUi    
    * Handle exit command 
    *
    * @param aCommand Command code 
    */
    void HandleCommandL(TInt aCommand);
private: 
    
    void DoBackgroundTaskL();
    
	/**
    * Exit server application.
    *
    */    
    void ExitServerApp(); 
private: //Data
    /**
     * Input server
     */           
    CPeninputServer* iPeninputServer;   
    
    /**
     * Idle for background processing.
     */               
    CIdle *iIdle;                     
    };

#endif

// End of File
