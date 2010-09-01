/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
*       Provides the  TAknFepInputStatePredictiveCandidateMiniQwertyPinyinPhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_PREDICTIVECANDIDATE_MINIQWERTY_CHINESEPHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_PREDICTIVECANDIDATE_MINIQWERTY_CHINESEPHRASE_H__

#include "AknFepUiInputStatePredictiveInputQwertyChinesePhrase.h"



class TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase : 
    public TAknFepInputStatePredictiveInputQwertyChinesePhrase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */

    TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);

    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
protected:

    /**
     * From TAknFepInputStatePredictiveInputQwertyPhrase
     * Handle system key press event
     *
     * @since S60 v3.2
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    };



#endif __AKN_FEP_UI_INPUT_STATE_PREDICTIVECANDIDATE_MINIQWERTY_CHINESEPHRASE_H__