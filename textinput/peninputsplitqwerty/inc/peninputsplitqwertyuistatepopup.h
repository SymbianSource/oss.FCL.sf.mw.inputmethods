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
* Description: definition of popup state of IME Split-Qwerty
*
*/


#ifndef C_PENINPUTSPLITQWERTYUISTATEPOPUP_H
#define C_PENINPUTSPLITQWERTYUISTATEPOPUP_H

// System includes
#include <peninputuistatebase.h>

// Forward declarations
class MPeninputUiStateMgr;
class MPeninputLayoutContext;

/**
 *  class CPeninputSplitQwertyUiStatePopup
 *
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */
class CPeninputSplitQwertyUiStatePopup : public CPeninputUiStateBase
    {

public:

    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return The pointer to created object
     */
    static CPeninputSplitQwertyUiStatePopup* NewL( 
                                        MPeninputUiStateMgr* aUiStateMgr,
                                        MPeninputLayoutContext* aContext );
        
    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputSplitQwertyUiStatePopup();
        
public: // From base class CPeninputUiStateBase  
    /**
     * From CPeninputUiStateBase
     * Process key event
     *
     * @since S60 v5.0
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleKeyEventL( const TRawEvent& aData );
    
    /**
     * From CPeninputUiStateBase
     * process internal event
     *
     * @since S60 v5.0
     * @param aEventType The internal event type
     * @param aEventData The internal event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleControlEvent( TInt aEventType, const TDesC& aEventData );
    
protected:

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return None
     */
    CPeninputSplitQwertyUiStatePopup( MPeninputUiStateMgr* aUiStateMgr, 
                                      MPeninputLayoutContext* aContext );
    };
                
#endif  // C_PENINPUTSPLITQWERTYUISTATEPOPUP_H
