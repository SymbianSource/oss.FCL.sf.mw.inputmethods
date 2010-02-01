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
* Description:  Interface of vkb composition with chars ui state
*
*/


#ifndef C_AKNFEPVKBUISTATECOMPOSITIONWITHCHARS_H
#define C_AKNFEPVKBUISTATECOMPOSITIONWITHCHARS_H

// user includes
#include "peninputvkbuistate.h"

// class declarations
/**
 *  Composition with chars ui state
 *  It provides key event handling and internal event handling in composition with chars state
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbUiStateCompositionWithChars : public CAknFepVkbUiState
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return The pointer point to CAknFepVkbUiStateCompositionWithChars type object
     */
    static CAknFepVkbUiStateCompositionWithChars* NewL(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                                       MAknFepVkbLayoutContext* aContext);
        
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CAknFepVkbUiStateCompositionWithChars();
        
    /**
     * Process key event
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleKeyEventL(const TRawEvent &aData);     
        
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     */
    CAknFepVkbUiStateCompositionWithChars(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                          MAknFepVkbLayoutContext* aContext);
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    //void ConstructL();
    
    /**
     * Process virtual key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleVkbEvent(TInt aEventType, const TDesC& aEventData);

    /**
     * Process backspace key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleBackEvent(TInt aEventType, const TDesC& aEventData);

    /**
     * Process enter, space key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleEnterSpaceEvent(TInt aEventType, const TDesC& aEventData);   
        
    /**
     * Process tab key event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleTabEvent(TInt aEventType, const TDesC& aEventData);
    
	virtual TBool HandleArrowKeyEvent(TInt aEventType, const TDesC& aEventData);        
    };

#endif  // C_AKNFEPVKBUISTATECOMPOSITIONWITHCHARS_H

// End Of File
