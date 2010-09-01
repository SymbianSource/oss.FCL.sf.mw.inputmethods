/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepUIFingerItutInputStateChineseGeneric definition.
*
*/












#ifndef __AKN_FEP_UI_FINGERITUT_INPUT_STATE_INIT_CHIN_GEN_H__
#define __AKN_FEP_UI_FINGERITUT_INPUT_STATE_INIT_CHIN_GEN_H__

// INCLUDES
#include "AknFepUiInputStateBase.h"

/**
 *  TAknFepUIFingerItutInputStateChineseGeneric state machine
 *
 *  This class handle the key event, which is not handled by finger ITUT ui.
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepUIFingerItutInputStateChineseGeneric : public TAknFepInputStateBase//TAknFepInputStateInitialChineseMultitapBase
    {
    
public:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aOwner Pointer to finger ITUT ui manager
     * @param aUIContainer Pointer to ui container
     * @return None
     */  
    TAknFepUIFingerItutInputStateChineseGeneric(
        MAknFepUIManagerStateInterface* aOwner/*,
        MAknFepUICtrlContainerChinese* aUIContainer*/);
        
    /**
     * Handle key event  
     *
     * @since S60 v3.2
     * @param aKey Event key
     * @param aLength The length of key press
     * @return ETrue for handling by state machine; otherwise EFalse
     */ 
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    /**
     * Key timer expired called by ptiengine  
     *
     * @since S60 v3.2
     * @return none
     */ 
    void KeyTimerExpired();
    
private:

    /**
     * Set Previous ptiengine observer  
     *
     * @since S60 v3.2
     * @param aPreObserver Pointer to previous observer
     * @return none
     */ 
    void SetPreObserver(MPtiObserver* aPreObserver);
    
    /**
     * Get Previous ptiengine observer  
     *
     * @since S60 v3.2
     * @return Pointer to previous observer
     */ 
    MPtiObserver* PreObserver();

private:
    
    /**
     * Previous observer
     */
    MPtiObserver* iPreObserver;    

    };

#endif //__AKN_FEP_UI_FINGERITUT_INPUT_STATE_INIT_CHIN_GEN_H__

// End of file
