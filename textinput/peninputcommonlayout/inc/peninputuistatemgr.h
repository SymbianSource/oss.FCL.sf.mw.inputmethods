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
* Description:  Interface of ui state manager
*
*/


#ifndef C_CPENINPUTUISTATEMGR_H
#define C_CPENINPUTUISTATEMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>

// User includes
#include "peninputuistatemgrinterface.h"

// Forward declarations
class CPeninputUiStateBase;
class MPeninputLayoutContext;

/**
 *  The ui state manager
 *  It provides get method of all ui state and current ui state
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputUiStateMgr : public CBase, 
                            public MPeninputUiStateMgr
    {

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputUiStateMgr object
     */
    IMPORT_C static CPeninputUiStateMgr* NewL( 
        MPeninputLayoutContext* aContext );
    
    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputUiStateMgr object
     */    
    IMPORT_C static CPeninputUiStateMgr* NewLC( 
        MPeninputLayoutContext* aContext );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputUiStateMgr();
    
// from base class MPeninputUiStateMgr

    /**
     * From MPeninputUiStateMgr
     * Get current ui state
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputUiStateBase object
     */
    IMPORT_C CPeninputUiStateBase* CurrentUiState();
    
    /**
     * From MPeninputUiStateMgr
     * Get ui state object according to the input state type
     *
     * @since S60 v3.2
     * @param aUiStateType The type of ui state
     * @return The pointer to CPeninputUiStateBase object
     */
    IMPORT_C CPeninputUiStateBase* GetUiState( TInt aUiStateType ); 
        
    /**
     * From MPeninputUiStateMgr
     * Set current ui state by state object
     *
     * @since S60 v3.2
     * @param aUiState The state to be set as current ui state
     * @return None
     */
    IMPORT_C void SetCurrentUiState( CPeninputUiStateBase* aUiState );

    /**
     * From MPeninputUiStateMgr
     * Set current ui state by state type
     *
     * @since S60 v3.2
     * @param aUiStateType The type of ui state
     * @return None
     */
    IMPORT_C void SetCurrentUiState( TInt aUiStateType );
    
    /**
     * From MPeninputUiStateMgr
     * Add ui state to list
     *
     * @since S60 v3.2
     * @param aUiState The ui state object to be added to list
     * @param aUiStateType The ui state type to be added to list
     * @return None
     */
    IMPORT_C void AddUiState( CPeninputUiStateBase* aUiState, 
                              TInt aUiStateType );

protected: //method

    /**
     * Symbian second phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void Construct();

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return None
     */
    CPeninputUiStateMgr( MPeninputLayoutContext* aContext );

private: // Data

    /**
     * The pointer points to current ui state 
     * Own
     */
    CPeninputUiStateBase* iCurrentState;

    /**
     * The pointer points to layout context 
     * Not own
     */
    MPeninputLayoutContext* iContext;
    
    /**
     * Ui state list
     */
    RPointerArray<CPeninputUiStateBase> iUiStateList;
    
    /**
     * Ui type list
     */
    RArray<TInt> iUiStateTypeList;
    
    };

#endif // C_CPENINPUTUISTATEMGR_H
