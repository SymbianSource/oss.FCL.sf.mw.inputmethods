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
* Description:  Declares document for TrainingUI
*
*/


#ifndef C_TRUIDOCUMENT_H
#define C_TRUIDOCUMENT_H

#include <AknDoc.h>

// Forward references
class CTruiAppUi;
class CAknApplication;

/**
 *  Instance of class CTruiDocument is the Document part of the AVKON
 *  application framework for the TrainingUI application
 *
 *  @code
 *   CApaDocument* document = new (ELeave) CTruiDocument(*this);
 *  @endcode
 *
 *  @lib avkon.lib eikcore.lib eiksrv.lib
 */
class CTruiDocument : public CAknDocument
    {
public:
    /**
     * Perform the first phase of two phase construction
     *
     * @param aApp application creating this document
     */
    CTruiDocument( CAknApplication& aApp );
    
    /**
     * Destroy the object and release all memory objects
     */
    ~CTruiDocument();


    /**
     * Create a CTruiAppUi object and return a pointer to it
     *
     * @return a pointer to the created instance of the AppUi created
     */
    CEikAppUi* CreateAppUiL();

    };

#endif // C_TRUIDOCUMENT_H

