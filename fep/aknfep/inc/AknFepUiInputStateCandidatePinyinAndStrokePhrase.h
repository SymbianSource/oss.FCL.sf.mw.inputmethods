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
* Description:            header file of Pinyin and Stroke phrase candidate state
*
*/












#ifndef T_AKNFEPUIINPUTSTATECANDIDATEPINYINANDSTROKE_PHRASE_H
#define T_AKNFEPUIINPUTSTATECANDIDATEPINYINANDSTROKE_PHRASE_H

// User includes 
#include "AknFepUiInputStateCandidateBaseChinesePhrase.h"

/**
 *  TAknFepUiInputStateCandidatePinyinAndStrokePhrase class.
 *
 *  Implementation of Pinyin and Stroke phrase candidate state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepUiInputStateCandidatePinyinAndStrokePhrase : 
    public TAknFepInputStateCandidateBaseChinesePhrase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepUiInputStateCandidatePinyinAndStrokePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);
        
    void HandleCommandL(TInt aCommandID);
    
    
protected:

    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif //T_AKNFEPUIINPUTSTATECANDIDATEPINYINANDSTROKE_PHRASE_H

// End of file
