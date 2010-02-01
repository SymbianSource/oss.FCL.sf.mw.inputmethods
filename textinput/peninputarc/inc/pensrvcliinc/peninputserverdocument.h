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
* Description:  Defination for peninput server application document
*
*/


#ifndef PENINPUTSERVERDOCUMENT_H
#define PENINPUTSERVERDOCUMENT_H

// INCLUDES
#include <AknDoc.h>

// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CPeninputServerDocument application class.
*/
class CPeninputServerDocument : public CAknDocument
    {
public: // Constructors and destructor
    /**
    * Two-phased constructor.
    *
    * @since S60 v4.0        
    * @param aApp The application
    * @return The created document
    */
    static CPeninputServerDocument* NewL(CEikApplication& aApp);

    /**
    * Destructor.
    *
    * @since S60 v4.0     
    */
    virtual ~CPeninputServerDocument();

private:

    /**
    * Default constructor.
    *
    * @since S60 v4.0     
    * @param aApp The application
    */
    CPeninputServerDocument(CEikApplication& aApp);
    
    /**
    * 2nd phrase constructor
    *
    * @since S60 v4.0     
    */    
    void ConstructL();

private:
    //from base class CAknDocument
    /**
    * From CAknDocument
    * Create CPeninputServerAppUi "App UI" object.
    *
    * @since S60 v4.0     
    * @return The created application UI    
    */
    CEikAppUi* CreateAppUiL();
    };

#endif

// End of File

