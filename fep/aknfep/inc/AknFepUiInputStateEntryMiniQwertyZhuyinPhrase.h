/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryMiniQwertyZhuyinPhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_MINIQWERTY_ZHUYIN_PHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_MINIQWERTY_ZHUYIN_PHRASE_H__

#include "AknFepUiInputStateEntryQwertyBaseChinesePhrase.h"

class TAknFepInputStateEntryMiniQwertyZhuyinPhrase : 
public TAknFepInputStateEntryQwertyBaseChinesePhrase
    {
public:
    TAknFepInputStateEntryMiniQwertyZhuyinPhrase(MAknFepUIManagerStateInterface* aOwner,
                                 MAknFepUICtrlContainerChinese* aUIContainer);

    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    TBool HandleKeyForMiniQwertyL( TInt aKey,
        TKeyPressLength aLength );
    
    TBool HandleKeyForHalfAndCustomQwertyL( TInt aKey,
        TKeyPressLength aLength );
public: // Functions from MPtiObserver interface
    void KeyTimerExpired();
private:
    void DeliberatelyUpdateSelection();
    void ImplicitlyUpdateSelection();
    void ClearDeliberateSelection();
    void RefreshUI(TInt aSelection);
    void InitializeStateL();
    void UpdateIndicator();    
    
    /**
     * Check whether tone mark border upon.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue,if the tone mark border upon, EFalse otehrs.
     */
    TBool CheckToneMarkBorderUpon();
    
    void CommitToneMarkL( TInt aKey );
    };

#endif __AKN_FEP_UI_INPUT_STATE_INPUT_MINIQWERTY_ZHUYIN_PHRASE_H__

// End of file
