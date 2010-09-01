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
* Description:  vkb layout
*
*/


#ifndef M_AKNFEPVKBLAYOUTCONTEXT_H
#define M_AKNFEPVKBLAYOUTCONTEXT_H

//  INCLUDES
#include "peninputvkbdataprovider.h"

/** class forward decalaration */
class CFepUiBaseCtrl;

/**
 *  peninputvkbcn layout interface implemtation
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class MAknFepVkbLayoutContext
    {
public:

    /**
     * Send event to vkb window
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl  The sender
     * @param aEventData The event data.
     * @return None                
     */
    virtual void SendEventToVkbControl(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                       const TDesC& aEventData = KNullDesC) = 0;


    /**
     * Sendkey to fep
     *
     * @since S60 v3.2
     * @param aEventType Event type
     * @param aEventData Event data
     * @return None.
     */
    virtual void Sendkey(TInt aEventType,const TDesC& aEventData = KNullDesC) = 0;
    
    /**
     * Request data from data provider
     *
     * @since S60 v3.2
     * @param aDataType The data request 
     * @return The data you request
     */
    virtual TInt RequestData(TAknFepDataType aDataType) = 0;

    /**
     * Get resource utils
     *
     * @since S60 v3.2
     * @param aEventType Event type
     * @param aData Event data
     * @return The resource utils
     */
    virtual void SetData(TAknFepDataType aDataType,TInt aData) = 0;

    /**
     * Force edit gain focus
     *
     * @since S60 v3.2
     * @param aWhichEdit The control id which will get focus
     * @return None.
     */    
    virtual void ForceEditGainFocus(TInt aWhichEdit) = 0;

    /**
     * Reset all status in layout
     *
     * @since S60 v3.2
     * @return None.
     */        
    virtual void Reset() = 0;

    /**
     * Get the length in composition field
     *
     * @since S60 v3.2
     * @return The string length in composition field
     */       
    virtual TInt CompositionFieldStringLength() = 0;
    
    /**
     * Get candidates
     *
     * @since S60 v3.2
     * @param aInputString the input string sent to engine
     * @param aIsPredictive The flag showing predictive or not
     * @param aForNextPage Is this for next page candidate
     * @return The canidates count
     */    
    virtual TInt GetCandidatesL(const TDesC& aInputString, 
                                const TBool aIsPredictive = 0,
                                TBool aForNextPage = EFalse) = 0;

    /**
     * Clear trigger string
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void ClearTriggerString() = 0;

    /**
     * Set current range
     *
     * @since S60 v3.2
     * @param aRange The range need to set
     * @param aNeedSave The flag need to save or not
     * @return None.
     */        
    virtual void SetCurrentRange(TInt aRange, TBool aNeedSave = EFalse, 
                                 TBool aNotify = EFalse) = 0;    
    };

#endif  // End of M_AKNFEPVKBLAYOUTCONTEXT_H

//End Of File