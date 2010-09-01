#ifndef AKNFEPUIINPUTSTATEPREDICTIVECANDIDATEHALFQWERTYCHINESEPHRASE_H_
#define AKNFEPUIINPUTSTATEPREDICTIVECANDIDATEHALFQWERTYCHINESEPHRASE_H_/*
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












#include "AknFepUiInputStatePredictiveInputQwertyChinesePhrase.h"



class TAknFepInputStatePredictiveCandidateHalfQwertyChinesePhrase : 
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

    TAknFepInputStatePredictiveCandidateHalfQwertyChinesePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);

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

#endif /*AKNFEPUIINPUTSTATEPREDICTIVECANDIDATEHALFQWERTYCHINESEPHRASE_H_*/
