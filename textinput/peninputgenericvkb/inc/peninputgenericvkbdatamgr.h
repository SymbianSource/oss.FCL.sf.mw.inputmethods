/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninput vkb data manager
*
*/


#ifndef C_CPENINPUTGENERICVKBDATAMGR_H
#define C_CPENINPUTGENERICVKBDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>

// User includes
#include "peninputdatamgr.h"

// Forward decalaration
class MPeninputLayoutContext;

/**
 *  Peninput VKB data management class
 *  This class manager the data of VKB
 *  
 *  @lib peninputgenericvkb.lib
 *  @since S60 v3.2
 */
class CPeninputGenericVkbDataMgr : public CPeninputDataMgr 
    {   

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputGenericVkbDataMgr object
     */
    static CPeninputGenericVkbDataMgr* NewL( 
        MPeninputLayoutContext* aContext );

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputGenericVkbDataMgr object
     */
    static CPeninputGenericVkbDataMgr* NewLC( 
        MPeninputLayoutContext* aContext );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericVkbDataMgr();

// From base class CPeninputDataMgr

    /**
     * From CPeninputDataMgr
     * Do nothing
     *
     * @since S60 v3.2
     * @return None
     */
    void InitMore();
    
    /**
     * From CPeninputDataMgr
     * Call back function, which is called when some key changed
     *
     * @since S60 v3.2
     * @param aChangedKey The key whose value is changed
     * @return None
     */
    void HandleGSRepositoryChange( TInt aChangedKey );
    
    /**
     * From CPeninputDataMgr
     * Reset data contained in data manager
     *
     * @since S60 v3.2
     * @return None
     */
    void Reset();
  
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return None
     */
    CPeninputGenericVkbDataMgr( MPeninputLayoutContext* aContext );
    
    };
   
#endif // C_CPENINPUTGENERICVKBDATAMGR_H
