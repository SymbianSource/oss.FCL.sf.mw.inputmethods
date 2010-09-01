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


#ifndef M_MPENINPUTUISTATEMGR_H
#define M_MPENINPUTUISTATEMGR_H

// Forward declarations
class CPeninputUiStateBase;

/**
 *  The ui state manager interfaces
 *  It provides intefaces to access ui state manager
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class MPeninputUiStateMgr
    {
    
public:

    /**
     * Get current ui state
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputUiStateBase object
     */
    virtual CPeninputUiStateBase* CurrentUiState() = 0;
    
    /**
     * Get ui state object according to the ui state type
     *
     * @since S60 v3.2
     * @param aUiStateType The type of ui state
     * @return The pointer to CPeninputUiStateBase object
     */
    virtual CPeninputUiStateBase* GetUiState( TInt aUiStateType ) = 0; 
        
    /**
     * Set current ui state by state object
     *
     * @since S60 v3.2
     * @param aUiState The state to be set as current ui state
     * @return None
     */
    virtual void SetCurrentUiState( CPeninputUiStateBase* aUiState ) = 0;

    /**
     * Set current ui state by state type
     *
     * @since S60 v3.2
     * @param aUiStateType The type of ui state
     * @return None
     */
    virtual void SetCurrentUiState( TInt aUiStateType ) = 0;
    
    /**
     * Add ui state to list
     *
     * @since S60 v3.2
     * @param aUiState The ui state object to be added to list
     * @param aUiStateType The ui state type to be added to list
     * @return None
     */
    virtual void AddUiState( CPeninputUiStateBase* aUiState, 
                             TInt aUiStateType ) = 0;
    
    };

#endif  // M_MPENINPUTUISTATEMGR_H
