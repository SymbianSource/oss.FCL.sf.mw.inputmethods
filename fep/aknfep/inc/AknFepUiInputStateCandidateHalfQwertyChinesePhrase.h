/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            ?Description
*
*/











#ifndef AKNFEPUIINPUTSTATECANDIDATEHALFQWERTYCHINESEPHRASE_H_
#define AKNFEPUIINPUTSTATECANDIDATEHALFQWERTYCHINESEPHRASE_H_

// User includes 
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

/**
 *  TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase class.
 *
 *  Implementation of Pinyin and Stroke phrase candidate state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepUiInputStateCandidateHalfQwertyChinesePhrase : 
    public TAknFepInputStateCandidateQwertyBaseChinesePhrase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepUiInputStateCandidateHalfQwertyChinesePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);
    
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
#endif /*AKNFEPUIINPUTSTATECANDIDATEHALFQWERTYCHINESEPHRASE_H_*/
