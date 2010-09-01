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
* Description:  interface of layout context
*
*/


#ifndef M_MPENINPUTLAYOUTCONTEXT_H
#define M_MPENINPUTLAYOUTCONTEXT_H

// System includes
#include <e32base.h>

// User includes
#include "peninputdataprovider.h"

// Class forward decalaration
class CFepUiBaseCtrl;

/**
 *  Peninput common layout interface
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class MPeninputLayoutContext
    {

public:

    /**
     * Send event to window
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The sender control
     * @param aEventData The event data
     * @return None 
     */
    virtual void SendEventToWindow( TInt aEventType, 
                                    CFepUiBaseCtrl* aCtrl, 
                                    const TDesC& aEventData = KNullDesC ) = 0;
                        
    virtual TBool IsShiftPermitted() = 0;
    virtual TBool IsCapsLockPermitted() = 0;

    /**
     * Send key to fep
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @return None
     */
    virtual void Sendkey( TInt aEventType, 
                          const TDesC& aEventData = KNullDesC ) = 0;
    
    /**
     * Request data from data provider
     *
     * @since S60 v3.2
     * @param aDataType The data type request 
     * @return The pointer to TAny type data requested
     */
    virtual TAny* RequestData( TPeninputDataType aDataType ) = 0;

    /**
     * Set data for data provider
     *
     * @since S60 v3.2
     * @param aDataType The data type to set value 
     * @param aData The pointer to TAny type data
     * @return None
     */
    virtual void SetData( TPeninputDataType aDataType, TAny* aData ) = 0;

    /**
     * Save key into CenRep(AknFep)
     *
     * @since S60 v3.2
     * @param aKey The key name
     * @param aValue The new value
     * @return None
     */
    virtual void SaveKey( TInt aKey, TInt aValue ) = 0;

    /**
     * Get the layout type, vkb or hwr.
     * 
     * @since S60 v3.2
     * @return The layout type
     */    
    virtual TInt LayoutType() = 0;
    
    };

#endif  // End of M_PENINPUTLAYOUTCONTEXT_H
