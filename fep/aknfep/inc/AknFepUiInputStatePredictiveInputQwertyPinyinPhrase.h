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












#ifndef T_AKNFEPUIINPUTSTATEPREDICTIVEINPUTQWERTYPINYIN_PHRASE_H
#define T_AKNFEPUIINPUTSTATEPREDICTIVEINPUTQWERTYPINYIN_PHRASE_H

// User includes 
#include "AknFepUiInputStatePredictiveInputQwertyChinesePhrase.h"

/**
 *  TAknFepInputStatePredictiveInputQwertyPinyinPhrase class.
 *
 *  Implementation of Pinyin phrase predictive Qwerty state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStatePredictiveInputQwertyPinyinPhrase : 
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

    TAknFepInputStatePredictiveInputQwertyPinyinPhrase(
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

private:

    /**
     * Judge the kay is in the valid pinyin key group 
     *
     * @since S60 v3.2
     * @param aKey System key
     * @return ETrue if it is pinyin key, otherwise it is not
     */
    TBool IsValidPinyinKey(TInt aKey);

    /**
     * Judge the kay is in the valid entry key group 
     *
     * @since S60 v3.2
     * @param aKey System key
     * @return ETrue if it is pinyin key, otherwise it is not
     */
    TBool IsValidEntryKey(TInt aKey);
    };

#endif T_AKNFEPUIINPUTSTATEPREDICTIVEINPUTQWERTYPINYIN_PHRASE_H

// End of file
