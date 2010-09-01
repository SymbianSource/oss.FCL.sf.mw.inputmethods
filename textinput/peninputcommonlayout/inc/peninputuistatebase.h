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
* Description:  Interface of ui state
*
*/


#ifndef C_CPENINPUTUISTATEBASE_H
#define C_CPENINPUTUISTATEBASE_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <peninputcmd.h>

// Forward declarations
class MPeninputUiStateMgr;
class MPeninputLayoutContext;

/**
 *  Basic class of ui state
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputUiStateBase : public CBase
    {

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aUiStateMgr The ui state manager
     * @param aContext The layout context
     * @return The pointer to CPeninputUiStateBase object
     */
    IMPORT_C static CPeninputUiStateBase* NewL( 
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext );
        
    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputUiStateBase();
        
    /**
     * Handle key event
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was consumed, otherwise EFalse
     */
    IMPORT_C virtual TBool HandleKeyEventL( const TRawEvent& aData );
    
    /**
     * Handle control event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was consumed, otherwise EFalse
     */
    IMPORT_C virtual TBool HandleControlEvent( TInt aEventType, 
                                               const TDesC& aEventData );
    
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aUiStateMgr The ui state manager
     * @param aContext The layout context
     * @return None
     */
    IMPORT_C CPeninputUiStateBase( MPeninputUiStateMgr* aUiStateMgr, 
                                   MPeninputLayoutContext* aContext );
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Construct(); 
    
    /**
     * Send key event through layout context
     *
     * @since S60 v3.2
     * @param aEventData Event data, the left most is unicode
     * @return ETrue if key event was consumed, otherwise EFalse
     */
    IMPORT_C TBool SendKey( const TDesC& aEventData );
    
protected:

    /**
     * Ui state manager 
     * Not own
     */
    MPeninputUiStateMgr* iUiStateMgr;
    
    /**
     * Layout context 
     * Not own
     */
    MPeninputLayoutContext* iContext;    

    };
          
#endif  // C_CPENINPUTUISTATEBASE_H
