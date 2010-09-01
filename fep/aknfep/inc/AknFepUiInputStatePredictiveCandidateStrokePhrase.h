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
* Description:            header file of Stroke phrase predictive state
*
*/












#ifndef T_AKNFEPUIINPUTSTATEPREDICTIVECANDIDATESTROKE_PHRASE_H
#define T_AKNFEPUIINPUTSTATEPREDICTIVECANDIDATESTROKE_PHRASE_H

// User includes 
#include "AknFepUiInputStatePredictiveCandidateChinesePhrase.h"

/**
 *  TAknFepInputStatePredictiveCandidateStrokePhrase class.
 *
 *  Implementation of Stroke phrase predictive state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStatePredictiveCandidateStrokePhrase : 
    public TAknFepInputStatePredictiveCandidateChinesePhrase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStatePredictiveCandidateStrokePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer); 

protected:

    /**
     * From TAknFepInputStatePredictiveCandidatePhrase
     * Handle system key press event
     *
     * @since S60 v3.2
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif T_AKNFEPUIINPUTSTATEPREDICTIVECANDIDATESTROKE_PHRASE_H

// End of file
