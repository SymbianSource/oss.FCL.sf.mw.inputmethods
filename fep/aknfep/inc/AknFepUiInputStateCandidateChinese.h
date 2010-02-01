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
* Description:            header file of Pinyin phrase predictive Qwerty state
*
*/












#ifndef T_AKNFEPUIINPUTSTATECANDIDATECHINESE_H
#define T_AKNFEPUIINPUTSTATECANDIDATECHINESE_H

// User includes 
#include "AknFepUiInputStateCandidateBase.h"

/**
 *  TAknFepInputStateCandidateChinese class.
 *
 *  Implementation of Chinese candidate state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStateCandidateChinese : public TAknFepInputStateCandidateBase 
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */

    TAknFepInputStateCandidateChinese(MAknFepUIManagerStateInterface* aOwner,
                                      MAknFepUICtrlContainerChinese* aUIContainer);

    /**
     * From TAknFepInputStateCandidate
     * Handle system key press event
     *
     * @since S60 v3.2
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    /**
     * From MPtiObserver interface
     * Call this function when getting last word in selection list
     *
     * @since S60 v3.2
     * @return None
     */
     
protected:
    
    /**
     * Handle horizontal navigation Key
     *
     * @since S60 v3.2
     * @param aKey System key
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleHorizontalNavigation(TInt aKey);

    /**
     * Handle vertical navigation Key
     *
     * @since S60 v3.2
     * @param aKey System key
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleVerticalNavigation(TInt aKey);
    
    /**
     * Update indicator 
     *
     * @since S60 v3.2
     * @param aKey System key
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void UpdateIndicator();
    
    };

#endif //T_AKNFEPUIINPUTSTATECANDIDATECHINESE_H

// End of file
