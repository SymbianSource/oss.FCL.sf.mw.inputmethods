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
* Description:  Interface of vkb ui state
*
*/


#ifndef C_AKNFEPVKBUISTATE_H
#define C_AKNFEPVKBUISTATE_H

// system includes
#include <e32base.h>
#include <w32std.h>
#include <peninputcmd.h>
#include <peninputlayoutbasecontrol.h>

// user includes
#include "peninputvkbuistatemgr.h"
#include "peninputvkblayoutcontext.h"
#include "peninputvkb.hrh"

const TInt KLengthSymbolNeedToFilter = 27;

const TUint16 KSymbolsNeedToFilter[] = {0xFF1A,  // £º
                                        0xFF01,  // £¡
                                        0x2026,  // ...
                                        0xFF0C,  // £¬
                                        0x3002,  // ¡£
                                        0xFF1F,  // £¿
                                        0x3001,  // ¡¢
                                        0x0020,  // space 
                                        0x007e,  // ~
                                        0x0030,  // 0
                                        0x0031,
                                        0x0032,
                                        0x0033,
                                        0x0034,
                                        0x0035,
                                        0x0036,
                                        0x0037,
                                        0x0038,
                                        0x0039,  // 9
                                        0x002D,  // -
                                        0x003B,  // halfwidth semicolon;
                                        0xFF1B,  // fullwidth semicolon
                                        0xFF0F,  // /
                                        0x201C,  // "
                                        0x201D,  // "
                                        0xFF08,  // (
                                        0xFF09   // )
                                        }; 
                                        
// class declarations
/**
 *  Basic event handling of vkb ui state
 *  It provides key event handling and internal event handling
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class MAknFepVkbUiState
    {        
public:        

    /**
     * Process key event
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleKeyEventL(const TRawEvent &aData) = 0;
    
    /**
     * process internal event
     *
     * @since S60 v3.2
     * @param aEventType The internal event type
     * @param aEventData The internal event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleControlEvent(TInt aEventType, const TDesC& aEventData) = 0;    
    };

/**
 *  Basic class of vkb ui state
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbUiState : public CBase,
                          public MAknFepVkbUiState
    {        
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return The pointer point to CAknFepVkbUiState type object
     */
    static CAknFepVkbUiState* NewL(MAknFepVkbUiStateMgr* aUiStateMgr, 
                                   MAknFepVkbLayoutContext* aContext);
        
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CAknFepVkbUiState();
        
    // from MAknFepVkbUiState  

    /**
     * Process key event
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleKeyEventL(const TRawEvent &aData);     
    
    /**
     * process internal event
     *
     * @since S60 v3.2
     * @param aEventType The internal event type
     * @param aEventData The internal event data
     * @return ETrue means event was responsed, otherwise EFalse
     */
    virtual TBool HandleControlEvent(TInt aEventType, const TDesC& aEventData);
    
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     */
    CAknFepVkbUiState(MAknFepVkbUiStateMgr* aUiStateMgr, MAknFepVkbLayoutContext* aContext);
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL();
    
    /**
     * Judge if given unicode should be filtered
     *
     * @since S60 v3.2
     * @param aUnicode The given unicode
     * @return ETrue if given unicode should be filtered, otherwise EFalse
     */
    virtual TBool IsFilterUnicode(TUint16 aUnicode);    

    /**
     * Send signal key
     *
     * @since S60 v3.2
     * @param aEventData Event data, the left most is unicode
     * @return ETrue if aEventData was responsed, otherwise EFalse
     */
    TBool SendKey(const TDesC& aEventData);
    
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
    
	virtual TBool HandleArrowKeyEvent(TInt aEventType, const TDesC& aEventData);    
protected:

    /**
     * Vkb ui state manager (not own)
     */
    MAknFepVkbUiStateMgr* iUiStateMgr;
    
    /**
     * Vkb layout context (not own)
     */
    MAknFepVkbLayoutContext* iContext;    
    };
                
#endif  // C_AKNFEPVKBUISTATE_H

// End Of File
