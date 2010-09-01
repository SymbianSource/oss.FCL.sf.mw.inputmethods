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
* Description: data manager of split qwerty
*
*/


#ifndef C_PENINPUTSPLITQWERTYDATAMGR_H
#define C_PENINPUTSPLITQWERTYDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>

// User includes
#include "peninputdatamgr.h"

// Forward decalaration
class MPeninputLayoutContext;

/**
 *  data management class of touch IME split qwerty
 *  
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */
class CPeninputSplitQwertyDataMgr : public CPeninputDataMgr 
    {   

public:

    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @param aContext The layout context
     * @return The pointer to created object
     */
    static CPeninputSplitQwertyDataMgr* NewL( MPeninputLayoutContext* aContext );

    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @param aContext The layout context
     * @return The pointer to created object
     */
    static CPeninputSplitQwertyDataMgr* NewLC( MPeninputLayoutContext* aContext );

    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputSplitQwertyDataMgr();

public: // From base CPeninputDataMgr

    /**
     * From CPeninputDataMgr
     * Additional init
     *
     * @since S60 v5.0
     * @return None
     */
    void InitMore();
    
    /**
     * From CPeninputDataMgr
     * Call back function, which is called when some key changed
     *
     * @since S60 v5.0
     * @param aChangedKey The key whose value is changed
     * @return None
     */
    void HandleGSRepositoryChange( TInt aChangedKey );
    
    /**
     * From CPeninputDataMgr
     * Reset data contained in data manager
     *
     * @since S60 v5.0
     * @return None
     */
    void Reset();
  
protected:

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aContext The layout context
     * @return None
     */
    CPeninputSplitQwertyDataMgr( MPeninputLayoutContext* aContext );
    
    };
   
#endif // C_PENINPUTSPLITQWERTYDATAMGR_H
