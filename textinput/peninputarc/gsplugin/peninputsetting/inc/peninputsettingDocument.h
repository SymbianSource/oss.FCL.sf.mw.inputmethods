/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares document for PenInputSetting
*
*/


#ifndef C_PENINPUTSETTINGDOCUMENT_H
#define C_PENINPUTSETTINGDOCUMENT_H


#include <AknDoc.h>

// Forward references
class CPenInputSettingAppUi;
class CEikApplication;


/**
*  CPeninputServerDocument application class.
*/
class CPenInputSettingDocument : public CAknDocument
    {
    
public:

    /**
    * Two-phased constructor.
    *
    * @since S60 v4.0        
    * @param aApp The application
    * @return The created document
    */
    static CPenInputSettingDocument* NewL( CEikApplication& aApp );

    /**
    * Two-phased constructor.
    *
    * @since S60 v4.0        
    * @param aApp The application
    * @return The created document
    */
    static CPenInputSettingDocument* NewLC( CEikApplication& aApp );

    /**
    * Destructor.
    *
    * @since S60 v4.0     
    */
    ~CPenInputSettingDocument();

public: //from base class CAknDocument

    /**
    * From CAknDocument
    * Create CPeninputServerAppUi "App UI" object.
    *
    * @since S60 v4.0     
    * @return The created application UI    
    */
    CEikAppUi* CreateAppUiL();

private:

    /**
    * Default constructor.
    *
    * @since S60 v4.0     
    * @param aApp The application
    */
    void ConstructL();

    /**
    * Default constructor.
    *
    * @since S60 v4.0     
    * @param aApp The application
    */
    CPenInputSettingDocument( CEikApplication& aApp );

    };


#endif // C_PENINPUTSETTING_DOCUMENT_H
