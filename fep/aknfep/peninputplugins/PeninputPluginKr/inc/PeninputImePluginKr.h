/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#ifndef PENINPUTIMEPLUGINKR_H
#define PENINPUTIMEPLUGINKR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CPeninputImePluginKr
 * 
 */
class CPeninputImePluginKr : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CPeninputImePluginKr();

    /**
     * Two-phased constructor.
     */
    static CPeninputImePluginKr* NewL();

    /**
     * Two-phased constructor.
     */
    static CPeninputImePluginKr* NewLC();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CPeninputImePluginKr();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    };

#endif // PENINPUTIMEPLUGINKR_H
