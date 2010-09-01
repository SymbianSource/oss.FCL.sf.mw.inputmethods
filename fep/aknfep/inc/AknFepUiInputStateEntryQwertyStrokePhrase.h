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
* Description:            header file of Stroke phrase Qwerty entry state
*
*/












#ifndef T_AKNFEPUIINPUTSTATEENTRYQWERTYSTROKE_PHRASE_H
#define T_AKNFEPUIINPUTSTATEENTRYQWERTYSTROKE_PHRASE_H

// User includes 
#include "AknFepUiInputStateEntryQwertyBaseChinesePhrase.h"

/**
 *  TAknFepInputStateEntryQwertyStrokePhrase class.
 *
 *  Implementation of Stroke phrase Qwerty entry state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStateEntryQwertyStrokePhrase : 
    public TAknFepInputStateEntryQwertyBaseChinesePhrase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEntryQwertyStrokePhrase(MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);

protected:

    /**
     * From TAknFepInputStateEntryQwertyBasePhrase
     * Handle system key press event
     *
     * @since S60 v3.2
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepInputStateEntryQwertyBasePhrase
     * Update popup window
     *
     * @since S60 v3.2
     * @return None
     */
    void RefreshUI();
    };

#endif //T_AKNFEPUIINPUTSTATEENTRYQWERTYSTROKE_PHRASE_H

// End of file
